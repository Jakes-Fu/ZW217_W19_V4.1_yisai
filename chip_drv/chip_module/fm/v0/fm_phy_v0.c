/******************************************************************************
 ** File Name:      fm_phy_v0.c                                                    *
 ** Author:         Jackey Ling                                           *
 ** DATE:           15/10/2012                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic information on chip.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 15/10/2012     Jackey.Ling      Create.                                   *                      *
 ******************************************************************************/
/*lint -save -e529 */	
#include "sci_types.h"
#include "tb_comm.h"
#include "adi_hal_internal.h"
#include "chip_plf_export.h"
#include "fm_reg_v0.h"
#include "task_monitor.h"
#include "deepsleep_drvapi.h"
/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif

/**---------------------------------------------------------------------------*
 **                        Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define RF_UDC_LO   0
#define FM_BASE_WRITE_ADDR 0x82000C00
#define GSM_BASE_WRITE_ADDR 0x82000800
#define RF_FREQ_DIV 2

const uint32 FM_SPRD_AGC_TABLE[] = {
    15848,   12589,    10000,    7943,     6309, 
    5011,    3981,     3162,     2511,     1995,  
    1584,    1258,     1000,     794,      630,  
    501,     398,      316,      251,      199,  
    158,     125,      100,      79,       63,    
    50,      39,       31,       25,       19,   
    15,      12,       10,       7,        6,     
    5,       3,        3,        2,        1, 
    1  
};

#define FM_SPRD_DEBUG

#ifdef FM_SPRD_DEBUG
#include "chip_plf_export.h"

#define ADI_FIFO_IS_EMPTY   ( (*(volatile uint32*)ADI_ARM_STS) & ADI_STS_FIFO_EMPTY_MASK )   
#define ADI_FIFO_IS_FULL    ( (*(volatile uint32*)ADI_ARM_STS) & ADI_STS_FIFO_FULL_MASK ) 

uint32 seek_int_num = 0;


typedef struct{
  uint32 fail;
  uint32 freq;
  uint32 rssi_db;
  uint32 agc_tab_idx;
  uint32 bit_adpsh;
  uint32 fm_pwr;
  uint32 freq_offset;
  uint32 wbrssi_db;
  uint32 pilot_det; 
}Channel_T;

Channel_T seek_channel[20];

PUBLIC uint16 ADI_READ_RF_REG (uint32 addr)
{
    uint32 cur_tick, next_tick;
    uint32 adi_rd_data;
	
    SCI_DisableIRQ();
    SCI_DisableFIQ();

    // wait write fifo empty before read
    cur_tick = next_tick = SCI_GetTickCount();
    while(ADI_FIFO_IS_EMPTY == 0)
    {
        if ( (next_tick - cur_tick) > 60)
        {
            TM_SendTestPointRequest (0x15170004, (next_tick - cur_tick));
            SCI_PASSERT(0, ("ADI Wait fifo empty Timeout!"));/*assert to do*/
        }
        next_tick = SCI_GetTickCount();
    }

    // Set read address
    *(volatile uint32*)ADI_ARM_RD_CMD = ((addr)&0xFFC) | (3<<11);
    
    // wait read operation complete, RD_data[31] will be cleared after the read operation complete
    cur_tick = next_tick;
    do
    {
        adi_rd_data = *(volatile uint32*)ADI_ARM_RD_DATA;

        //Timeout
        if ( (next_tick - cur_tick) > 60)
        {
            TM_SendTestPointRequest (0x15170002, (next_tick - cur_tick));
			SCI_PASSERT(0, ("ADI Read Timeout!"));/*assert to do*/
        }

        next_tick = SCI_GetTickCount();
    }
    while(adi_rd_data & ADI_RD_CMD_BUSY_MASK);
    
    //read operation complete
    SCI_RestoreFIQ();
    SCI_RestoreIRQ();

    return((uint16)(adi_rd_data & ADI_ARM_RD_DATA_MASK));
}

uint16 FM_SPRD_RF_RegisteRead(uint16 addr)
{
    uint16 value = 0;
   
    ANA_REG_SET(0x82000800+0x6E*4, 1<<6);
    ANA_REG_SET(0x82000800+0x7B*4, 0);  
    
    value = ADI_READ_RF_REG(0x82000C00+(addr&0xFF)*4);
    
    ANA_REG_SET(0x82000800+0x6E*4, 0);
    ANA_REG_SET(0x82000800+0x7B*4, 1<<15); 
    SCI_TRACE_LOW("FM_SPRD_READ addr: %x ,value: %x",addr,value);

    return value;
}

uint16 GSM_SPRD_RF_RegisteRead(uint16 addr)
{
    uint16 value = 0;
   
    ANA_REG_SET(0x82000800+0x6E*4, 1<<6);
    ANA_REG_SET(0x82000800+0x7B*4, 0);  
    
    value = ADI_READ_RF_REG(0x82000800+(addr&0xFF)*4);
    
    ANA_REG_SET(0x82000800+0x6E*4, 0);
    ANA_REG_SET(0x82000800+0x7B*4, 1<<15); 
    SCI_TRACE_LOW("FM_SPRD_READ addr: %x ,value: %x",addr,value);

    return value;
}



void FM_SPRD_GetDebugInfo(void)
{

    if(seek_int_num > 19)
    {
        seek_int_num = 0;
    }
    
    seek_channel[seek_int_num].fail = (TB_REG_GET(REG_RSSI_STS)&RSSI_STS_SF)>>16; 
        
    seek_channel[seek_int_num].freq  = (TB_REG_GET(REG_CHAN_FREQ_STS) + RF_FREQ_DIV)/RF_FREQ_DIV;  
	if(seek_channel[seek_int_num].freq == 1)
	{
	   SCI_PASSERT(0,("freq = %d, FM_EB(0x%X) = 0x%X", /*assert to do*/
	   		seek_channel[seek_int_num].freq, 
    		TB_REG_GET(APB_EB0), TB_REG_GET(APB_EB0) & FM_EB_SET));
    }

    seek_channel[seek_int_num].rssi_db = TB_REG_GET(REG_RSSI_STS)&0xFF;       
    if(seek_channel[seek_int_num].rssi_db > 127) {
        seek_channel[seek_int_num].rssi_db = seek_channel[seek_int_num].rssi_db -256;       
    }
      
    seek_channel[seek_int_num].fm_pwr = TB_REG_GET(REG_INPWR_STS)&0x1FF; 
    if(seek_channel[seek_int_num].fm_pwr>255) {
        seek_channel[seek_int_num].fm_pwr = seek_channel[seek_int_num].fm_pwr -512;
    }
      
    seek_channel[seek_int_num].agc_tab_idx= TB_REG_GET(REG_AGC_TBL_STS)&0xF;
      
    seek_channel[seek_int_num].bit_adpsh = (TB_REG_GET(REG_ADP_BIT_SFT)&0x1F0000)>>16;
    if(seek_channel[seek_int_num].bit_adpsh > 7) {
        seek_channel[seek_int_num].bit_adpsh = seek_channel[seek_int_num].bit_adpsh - 16;
    }
      
         seek_channel[seek_int_num].freq_offset = TB_REG_GET(REG_FREQ_OFF_STS)&0xFFFF;
    if(seek_channel[seek_int_num].freq_offset > 32767) {
        seek_channel[seek_int_num].freq_offset = seek_channel[seek_int_num].freq_offset - 65536;
    }
      
    seek_channel[seek_int_num].pilot_det = (TB_REG_GET(REG_RSSI_STS)>>18)&0x3;
    seek_channel[seek_int_num].wbrssi_db = TB_REG_GET(REG_WBRSSI_STS)&0xFF;
	if(seek_channel[seek_int_num].wbrssi_db> 127) {
        seek_channel[seek_int_num].wbrssi_db = seek_channel[seek_int_num].wbrssi_db-256;
        
    }

	//Edit by luther.li 2012-11-30
    SCI_TRACE_LOW("seek_int_num: %d  rssi_db: %d,pilot_det: %d,wbrssi_db: %d,fail: %x, freq: %d, fm_pwr: %x, agc_tab_idx: %x, freq_offset: %x",
    seek_int_num, seek_channel[seek_int_num].rssi_db, seek_channel[seek_int_num].pilot_det, seek_channel[seek_int_num].wbrssi_db,seek_channel[seek_int_num].fail, seek_channel[seek_int_num].freq,
    seek_channel[seek_int_num].fm_pwr, seek_channel[seek_int_num].agc_tab_idx,seek_channel[seek_int_num].freq_offset);
   
    seek_int_num ++;
}

#endif


LOCAL void FM_SPRD_AGC_SET(void)
{
	int32 i = 0;
	volatile AGC_TBL_CLK_U      *AGC_TBL_CLK     = (AGC_TBL_CLK_U      *)REG_AGC_TBL_CLK ; 
	
	AGC_TBL_CLK->mBits.sel = 1;
	
	SCI_Sleep(10);
	
	for(i=0;i<41;i++)
	{
		TB_REG_SET(AGC_DB_TBL_BASE+i*4,FM_SPRD_AGC_TABLE[i]);/*lint !e737*/
	}

	SCI_TraceLow("FM_SPRD_READ AGC_TABLE  %x , %x,  %x, %x,  %x, %x, %x, %x",TB_REG_GET(AGC_DB_TBL_BASE),
	TB_REG_GET(AGC_DB_TBL_BASE+1*4),
	TB_REG_GET(AGC_DB_TBL_BASE+2*4),
	TB_REG_GET(AGC_DB_TBL_BASE+3*4),
	TB_REG_GET(AGC_DB_TBL_BASE+4*4),
	TB_REG_GET(AGC_DB_TBL_BASE+38*4),	
	TB_REG_GET(AGC_DB_TBL_BASE+39*4),	
	TB_REG_GET(AGC_DB_TBL_BASE+40*4)	
	);
	
	AGC_TBL_CLK->mBits.sel = 0;
	SCI_Sleep(10);
}


//Add by luther.li 2013-11-21 check new fm chip id
//extern PUBLIC  uint32 CHIP_GetHWChipID (void);
/*LOCAL BOOLEAN FM_Is_New_Chip()
{
    if ((TB_REG_GET(0x8800202C) && TB_REG_GET(0x8800203c)) != 0 )
    {
         return TRUE;
    }else{
         return FALSE;
    }
}*/

void FM_SPRD_RegInit(void)
{
  volatile FM_CTRL_U          *FM_CTRL         = (FM_CTRL_U          *)REG_FM_CTRL        ; 
  volatile FM_EN_U            *FM_EN           = (FM_EN_U            *)REG_FM_EN          ; 
  volatile RF_INIT_GAIN_U     *RF_INIT_GAIN    = (RF_INIT_GAIN_U     *)REG_RF_INIT_GAIN   ;
  volatile CHAN_U             *CHAN            = (CHAN_U             *)REG_CHAN           ; 
  volatile AGC_TBL_CLK_U      *AGC_TBL_CLK     = (AGC_TBL_CLK_U      *)REG_AGC_TBL_CLK    ; 
  volatile SEEK_LOOP_U        *SEEK_LOOP       = (SEEK_LOOP_U        *)REG_SEEK_LOOP      ; 
  volatile FMCTL_STI_U        *FMCTL_STI       = (FMCTL_STI_U        *)REG_FMCTL_STI      ; 
  volatile BAND_LMT_U         *BAND_LMT        = (BAND_LMT_U         *)REG_BAND_LMT       ; 
  volatile BAND_SPACE_U       *BAND_SPACE      = (BAND_SPACE_U       *)REG_BAND_SPACE     ; 
  volatile INT_EN_U           *INT_EN          = (INT_EN_U           *)REG_INT_EN         ; 
  volatile INT_CLR_U          *INT_CLR         = (INT_CLR_U          *)REG_INT_CLR        ; 
  volatile INT_STS_U          *INT_STS         = (INT_STS_U          *)REG_INT_STS        ; 
  volatile SEEK_CH_TH_U       *SEEK_CH_TH      = (SEEK_CH_TH_U       *)REG_SEEK_CH_TH     ;  
  volatile AGC_TBL_RFRSSI0_U  *AGC_TBL_RFRSSI0 = (AGC_TBL_RFRSSI0_U  *)REG_AGC_TBL_RFRSSI0; 
  volatile AGC_TBL_RFRSSI1_U  *AGC_TBL_RFRSSI1 = (AGC_TBL_RFRSSI1_U  *)REG_AGC_TBL_RFRSSI1;  
  volatile AGC_TBL_WBRSSI_U   *AGC_TBL_WBRSSI  = (AGC_TBL_WBRSSI_U   *)REG_AGC_TBL_WBRSSI ; 
  volatile AGC_IDX_TH_U       *AGC_IDX_TH      = (AGC_IDX_TH_U       *)REG_AGC_IDX_TH     ; 
  volatile AGC_RSSI_TH_U      *AGC_RSSI_TH     = (AGC_RSSI_TH_U      *)REG_AGC_RSSI_TH    ; 
  volatile SEEK_ADPS_U        *SEEK_ADPS       = (SEEK_ADPS_U        *)REG_SEEK_ADPS      ; 
  volatile STER_PWR_U         *STER_PWR        = (STER_PWR_U         *)REG_STER_PWR       ; 
  volatile AGC_CTRL_U         *AGC_CTRL        = (AGC_CTRL_U         *)REG_AGC_CTRL       ; 
  volatile AGC_ITV_TH_U       *AGC_ITV_TH      = (AGC_ITV_TH_U       *)REG_AGC_ITV_TH     ; 
  volatile AGC_ADPS0_U        *AGC_ADPS0       = (AGC_ADPS0_U        *)REG_AGC_ADPS0      ; 
  volatile AGC_ADPS1_U        *AGC_ADPS1       = (AGC_ADPS1_U        *)REG_AGC_ADPS1      ; 
  volatile PDP_TH_U           *PDP_TH          = (PDP_TH_U           *)REG_PDP_TH         ; 
  volatile PDP_DEV_U          *PDP_DEV         = (PDP_DEV_U          *)REG_PDP_DEV        ; 
  volatile ADP_ST_CONF_U      *ADP_ST_CONF     = (ADP_ST_CONF_U      *)REG_ADP_ST_CONF    ; 
  volatile ADP_LPF_CONF_U     *ADP_LPF_CONF    = (ADP_LPF_CONF_U     *)REG_ADP_LPF_CONF   ; 
  volatile DEPHA_SCAL_U       *DEPHA_SCAL      = (DEPHA_SCAL_U       *)REG_DEPHA_SCAL     ; 
  volatile HW_MUTE_U          *HW_MUTE         = (HW_MUTE_U          *)REG_HW_MUTE        ; 
  volatile SW_MUTE_U          *SW_MUTE         = (SW_MUTE_U          *)REG_SW_MUTE        ; 
  volatile UPD_CTRL_U         *UPD_CTRL        = (UPD_CTRL_U         *)REG_UPD_CTRL       ; 
  volatile AUD_BLD0_U         *AUD_BLD0        = (AUD_BLD0_U         *)REG_AUD_BLD0       ; 
  volatile AUD_BLD1_U         *AUD_BLD1        = (AUD_BLD1_U         *)REG_AUD_BLD1       ; 
  volatile AGC_HYS_U          *AGC_HYS         = (AGC_HYS_U          *)REG_AGC_HYS        ;  
  volatile MONO_PWR_U         *MONO_PWR        = (MONO_PWR_U         *)REG_MONO_PWR       ; 
  volatile RF_CFG_DLY_U       *RF_CFG_DLY      = (RF_CFG_DLY_U       *)REG_RF_CFG_DLY     ;   
  volatile AGC_TBL_STS_U      *AGC_TBL_STS     = (AGC_TBL_STS_U      *)REG_AGC_TBL_STS    ; 
  volatile ADP_BIT_SFT_U      *ADP_BIT_SFT     = (ADP_BIT_SFT_U      *)REG_ADP_BIT_SFT    ; 
  volatile SEEK_CNT_U         *SEEK_CNT        = (SEEK_CNT_U         *)REG_SEEK_CNT       ; 
  
  volatile RSSI_STS_U         *RSSI_STS        = (RSSI_STS_U         *)REG_RSSI_STS       ; 
  volatile CHAN_FREQ_STS_U    *CHAN_FREQ_STS   = (CHAN_FREQ_STS_U    *)REG_CHAN_FREQ_STS  ; 
  volatile FREQ_OFF_STS_U     *FREQ_OFF_STS    = (FREQ_OFF_STS_U     *)REG_FREQ_OFF_STS   ; 
  volatile INPWR_STS_U        *INPWR_STS       = (INPWR_STS_U        *)REG_INPWR_STS      ; 
  volatile RF_RSSI_STS_U      *RF_RSSI_STS     = (RF_RSSI_STS_U      *)REG_RF_RSSI_STS  ; 
  volatile NO_LPF_STS_U       *NO_LPF_STS      = (NO_LPF_STS_U       *)REG_NO_LPF_STS   ; 
  volatile SMUTE_STS_U        *SMUTE_STS       = (SMUTE_STS_U        *)REG_SMUTE_STS      ; 

  volatile AGC_BITS_TBL0_U    *AGC_BITS_TBL0   = (AGC_BITS_TBL0_U    *)REG_AGC_BITS_TBL0  ; 
  volatile AGC_BITS_TBL1_U    *AGC_BITS_TBL1   = (AGC_BITS_TBL1_U    *)REG_AGC_BITS_TBL1  ; 
  volatile AGC_BITS_TBL2_U    *AGC_BITS_TBL2   = (AGC_BITS_TBL2_U    *)REG_AGC_BITS_TBL2  ; 
  volatile AGC_BITS_TBL3_U    *AGC_BITS_TBL3   = (AGC_BITS_TBL3_U    *)REG_AGC_BITS_TBL3  ; 
  volatile AGC_BITS_TBL4_U    *AGC_BITS_TBL4   = (AGC_BITS_TBL4_U    *)REG_AGC_BITS_TBL4  ; 
  volatile AGC_BITS_TBL5_U    *AGC_BITS_TBL5   = (AGC_BITS_TBL5_U    *)REG_AGC_BITS_TBL5  ; 
  volatile AGC_BITS_TBL6_U    *AGC_BITS_TBL6   = (AGC_BITS_TBL6_U    *)REG_AGC_BITS_TBL6  ; 
  volatile AGC_BITS_TBL7_U    *AGC_BITS_TBL7   = (AGC_BITS_TBL7_U    *)REG_AGC_BITS_TBL7  ; 
  volatile AGC_BITS_TBL8_U    *AGC_BITS_TBL8   = (AGC_BITS_TBL8_U    *)REG_AGC_BITS_TBL8  ; 
  volatile AGC_BITS_TBL9_U    *AGC_BITS_TBL9   = (AGC_BITS_TBL9_U    *)REG_AGC_BITS_TBL9  ; 
  volatile AGC_BITS_TBL10_U   *AGC_BITS_TBL10  = (AGC_BITS_TBL10_U   *)REG_AGC_BITS_TBL10 ; 
  volatile AGC_BITS_TBL11_U   *AGC_BITS_TBL11  = (AGC_BITS_TBL11_U   *)REG_AGC_BITS_TBL11 ; 
  volatile AGC_BITS_TBL12_U   *AGC_BITS_TBL12  = (AGC_BITS_TBL12_U   *)REG_AGC_BITS_TBL12 ; 
  volatile AGC_BITS_TBL13_U   *AGC_BITS_TBL13  = (AGC_BITS_TBL13_U   *)REG_AGC_BITS_TBL13 ; 
  volatile AGC_BITS_TBL14_U   *AGC_BITS_TBL14  = (AGC_BITS_TBL14_U   *)REG_AGC_BITS_TBL14 ; 
  volatile AGC_BITS_TBL15_U   *AGC_BITS_TBL15  = (AGC_BITS_TBL15_U   *)REG_AGC_BITS_TBL15 ; 
  volatile AGC_BITS_TBL16_U   *AGC_BITS_TBL16  = (AGC_BITS_TBL16_U   *)REG_AGC_BITS_TBL16 ; 
  volatile AGC_BITS_TBL17_U   *AGC_BITS_TBL17  = (AGC_BITS_TBL17_U   *)REG_AGC_BITS_TBL17 ; 
  volatile AGC_VAL_TBL0_U     *AGC_VAL_TBL0    = (AGC_VAL_TBL0_U     *)REG_AGC_VAL_TBL0   ; 
  volatile AGC_VAL_TBL1_U     *AGC_VAL_TBL1    = (AGC_VAL_TBL1_U     *)REG_AGC_VAL_TBL1   ; 
  volatile AGC_VAL_TBL2_U     *AGC_VAL_TBL2    = (AGC_VAL_TBL2_U     *)REG_AGC_VAL_TBL2   ; 
  volatile AGC_VAL_TBL3_U     *AGC_VAL_TBL3    = (AGC_VAL_TBL3_U     *)REG_AGC_VAL_TBL3   ; 
  volatile AGC_VAL_TBL4_U     *AGC_VAL_TBL4    = (AGC_VAL_TBL4_U     *)REG_AGC_VAL_TBL4   ; 
  volatile AGC_VAL_TBL5_U     *AGC_VAL_TBL5    = (AGC_VAL_TBL5_U     *)REG_AGC_VAL_TBL5   ; 
  volatile AGC_VAL_TBL6_U     *AGC_VAL_TBL6    = (AGC_VAL_TBL6_U     *)REG_AGC_VAL_TBL6   ; 
  volatile AGC_VAL_TBL7_U     *AGC_VAL_TBL7    = (AGC_VAL_TBL7_U     *)REG_AGC_VAL_TBL7   ; 
  volatile AGC_VAL_TBL8_U     *AGC_VAL_TBL8    = (AGC_VAL_TBL8_U     *)REG_AGC_VAL_TBL8   ; 
  volatile AGC_VAL_TBL9_U     *AGC_VAL_TBL9    = (AGC_VAL_TBL9_U     *)REG_AGC_VAL_TBL9   ; 
  volatile AGC_VAL_TBL10_U    *AGC_VAL_TBL10   = (AGC_VAL_TBL10_U    *)REG_AGC_VAL_TBL10  ; 
  volatile AGC_VAL_TBL11_U    *AGC_VAL_TBL11   = (AGC_VAL_TBL11_U    *)REG_AGC_VAL_TBL11  ; 
  volatile AGC_VAL_TBL12_U    *AGC_VAL_TBL12   = (AGC_VAL_TBL12_U    *)REG_AGC_VAL_TBL12  ; 
  volatile AGC_VAL_TBL13_U    *AGC_VAL_TBL13   = (AGC_VAL_TBL13_U    *)REG_AGC_VAL_TBL13  ; 
  volatile AGC_VAL_TBL14_U    *AGC_VAL_TBL14   = (AGC_VAL_TBL14_U    *)REG_AGC_VAL_TBL14  ; 
  volatile AGC_VAL_TBL15_U    *AGC_VAL_TBL15   = (AGC_VAL_TBL15_U    *)REG_AGC_VAL_TBL15  ; 
  volatile AGC_VAL_TBL16_U    *AGC_VAL_TBL16   = (AGC_VAL_TBL16_U    *)REG_AGC_VAL_TBL16  ; 
  volatile AGC_VAL_TBL17_U    *AGC_VAL_TBL17   = (AGC_VAL_TBL17_U    *)REG_AGC_VAL_TBL17  ; 
  

  volatile AGC_BOND_TBL0_U    *AGC_BOND_TBL0   = (AGC_BOND_TBL0_U    *)REG_AGC_BOND_TBL0  ; 
  volatile AGC_BOND_TBL1_U    *AGC_BOND_TBL1   = (AGC_BOND_TBL1_U    *)REG_AGC_BOND_TBL1  ; 
  volatile AGC_BOND_TBL2_U    *AGC_BOND_TBL2   = (AGC_BOND_TBL2_U    *)REG_AGC_BOND_TBL2  ; 
  volatile AGC_BOND_TBL3_U    *AGC_BOND_TBL3   = (AGC_BOND_TBL3_U    *)REG_AGC_BOND_TBL3  ; 
  volatile AGC_BOND_TBL4_U    *AGC_BOND_TBL4   = (AGC_BOND_TBL4_U    *)REG_AGC_BOND_TBL4  ; 
  volatile AGC_BOND_TBL5_U    *AGC_BOND_TBL5   = (AGC_BOND_TBL5_U    *)REG_AGC_BOND_TBL5  ; 
  volatile AGC_BOND_TBL6_U    *AGC_BOND_TBL6   = (AGC_BOND_TBL6_U    *)REG_AGC_BOND_TBL6  ; 
  volatile AGC_BOND_TBL7_U    *AGC_BOND_TBL7   = (AGC_BOND_TBL7_U    *)REG_AGC_BOND_TBL7  ; 
  volatile AGC_BOND_TBL8_U    *AGC_BOND_TBL8   = (AGC_BOND_TBL8_U    *)REG_AGC_BOND_TBL8  ; 
  volatile AGC_BOND_TBL9_U    *AGC_BOND_TBL9   = (AGC_BOND_TBL9_U    *)REG_AGC_BOND_TBL9  ; 
  
  
  volatile SPI_CTRL_U         *SPI_CTRL        = (SPI_CTRL_U         *)REG_SPI_CTRL       ; 
  volatile SPI_WD0_U          *SPI_WD0         = (SPI_WD0_U          *)REG_SPI_WD0        ; 
  volatile SPI_WD1_U          *SPI_WD1         = (SPI_WD1_U          *)REG_SPI_WD1        ; 
  volatile SPI_RD_U           *SPI_RD          = (SPI_RD_U           *)REG_SPI_RD         ; 
  volatile SPI_FIFO_STS_U     *SPI_FIFO_STS    = (SPI_FIFO_STS_U     *)REG_SPI_FIFO_STS   ; 
  
  
    //-----------------------------------
  FM_CTRL->mBits.seek_up   = 0;
  FM_CTRL->mBits.ster_swap = 0;
  FM_CTRL->mBits.pol_dcon  = 0;
  FM_CTRL->mBits.rxm       = 0; ///:mono mode 0:stereo mode
  FM_CTRL->mBits.dem       = 1;
  FM_CTRL->mBits.no_lpf    = 3; 


  if(CHIP_GetHWChipID() == CHIP_ID_SC6531BA)
  {
     TB_REG_OR(0x88002000,0x1011);
     //FM_CTRL->mBits.reserved_6  = 0x80;//add by luther.li 2013-11-21

	 SCI_TRACE_LOW("FM_CTRL value =%x\n",TB_REG_GET(REG_FM_CTRL));
     
     TB_REG_OR(0x88002340,0x81E);//add by luther.li 2013-11-21 to remove 104m
  }
  //------------------------------------
  
  FM_EN->mBits.seek_adps   = 0;
  FM_EN->mBits.dcc         = 1;
  FM_EN->mBits.rssi        = 1;
  FM_EN->mBits.agc         = 1;
  if(CHIP_GetHWChipID() == CHIP_ID_SC6531BA)
  {
	FM_EN->mBits.agc_adps    = 0;  
  }else{
  	FM_EN->mBits.agc_adps    = 1; 
  }
  
  FM_EN->mBits.fom         = 1;
  FM_EN->mBits.foc         = 1;
  FM_EN->mBits.plt_dect    = 1;
  FM_EN->mBits.adp_lpf     = 1;
  
  FM_EN->mBits.hw_mute     = 0;  // 1:enable  0:disable
  FM_EN->mBits.sw_mute     = 0;  //Modify by luther.li 2012-12-11 set mBits.sw_mute to 0
  FM_EN->mBits.aud_bld     = 1;
  FM_EN->mBits.adc_1bit    = 1;
  //------------------------------------  
  //Edit by luther.li modify mBits.freq   2012-12-06
  //CHAN->mBits.freq = 800*RF_FREQ_DIV; //9263
  CHAN->mBits.freq = 875*RF_FREQ_DIV; //9263
  //------------------------------------
  AGC_TBL_CLK->mBits.sel   = 0;                           
  //------------------------------------
  //Edit by luther.li modify mBits.high   2012-12-06
  //BAND_LMT->mBits.high = (1080+RF_UDC_LO+1)*RF_FREQ_DIV;
  BAND_LMT->mBits.high = (1080+RF_UDC_LO-1)*RF_FREQ_DIV;
  BAND_LMT->mBits.low =  (875+RF_UDC_LO-1)*RF_FREQ_DIV; //(860+RF_UDC_LO+1)*RF_FREQ_DIV;
 // BAND_LMT->mBits.low =  (800+RF_UDC_LO+1)*RF_FREQ_DIV; //(860+RF_UDC_LO+1)*RF_FREQ_DIV;
  //------------------------------------
  //Edit by luther.li modify mBits.val   2012-12-06
  //SEEK_LOOP->mBits.val = (1080-800);//(1080-800);
  SEEK_LOOP->mBits.val = 10;
  //------------------------------------  
  BAND_SPACE->mBits.val  = RF_FREQ_DIV;
  //------------------------------------    
  SEEK_CH_TH->mBits.val = 2000;//modify by luther.li set val to 2000
  //-----------------------------------
  FMCTL_STI->mBits.val = 0x0; //Seek_tune:0:normal 1:seek 2:tune 
  //------------------------------------  
  AGC_TBL_RFRSSI0->mBits.mode = 0;//don't set this bit to high
  AGC_TBL_RFRSSI0->mBits.hyst = 8;
  AGC_TBL_RFRSSI0->mBits.upd_cnt = 32;   
  //------------------------------------  
  AGC_TBL_RFRSSI1->mBits.th = 3000;  
  AGC_TBL_RFRSSI1->mBits.avg_len = 31;  
  AGC_TBL_RFRSSI1->mBits.avg_sh = 5; 
  //------------------------------------  
  AGC_TBL_WBRSSI->mBits.mode = 0;
  /*lint -e542*/
  AGC_TBL_WBRSSI->mBits.th = -10;
  AGC_TBL_WBRSSI->mBits.hyst = 12;
  AGC_TBL_WBRSSI->mBits.wbrssi_shway = 1;
   if(CHIP_GetHWChipID() == CHIP_ID_SC6531BA){
  	AGC_TBL_WBRSSI->mBits.wbrssi_sh = 2;
   }else{
   	AGC_TBL_WBRSSI->mBits.wbrssi_sh = 0;
   }
  //------------------------------------   
  AGC_IDX_TH->mBits.max = 2;
  AGC_IDX_TH->mBits.mid = 8;//new modify for seek in weak signal  by feng.deng @2014-05-13
  AGC_IDX_TH->mBits.min = 7;
  //------------------------------------
  AGC_RSSI_TH->mBits.max = -10;
  AGC_RSSI_TH->mBits.min = -40;
  //------------------------------------
  SEEK_ADPS->mBits.sft = -4;
  SEEK_ADPS->mBits.lbnd = -40;
  //------------------------------------
  STER_PWR->mBits.th = -85;//-90;    //modified by xuede @2012-12-29
  //-----------------------------------
  /* 
  MONO_PWR->mBits.th2 = -90;//-100;   //modified by xuede @2012-12-29
  MONO_PWR->mBits.th1 = -105;//-110;    //modified by xuede @2012-12-29
  */
  //new modify for seek in weak signal  by feng.deng
  MONO_PWR->mBits.th2 = -94;//-100;   //modified by feng.deng @2014-05-13
  MONO_PWR->mBits.th1 = -109;//-110;    //modified by feng.deng @2014-05-13
  
  //------------------------------------  
  AGC_CTRL->mBits.tab_size = 9;
  AGC_CTRL->mBits.sw_gain = 4;
  AGC_CTRL->mBits.gain_step = 1;
  AGC_CTRL->mBits.mthd = 2;
  //------------------------------------  
  AGC_ITV_TH->mBits.max = -2;
  AGC_ITV_TH->mBits.min = -18;
  //------------------------------------  
  AGC_ADPS0->mBits.hbnd = -3;
  AGC_ADPS0->mBits.lbnd = -9;
  AGC_ADPS0->mBits.sft_lmt = -7;
  //------------------------------------  
  AGC_ADPS1->mBits.hlev = -2;
  AGC_ADPS1->mBits.llev = -8;
  AGC_ADPS1->mBits.mthd = 3;	
  //------------------------------------  
  PDP_TH->mBits.val = 400;
  //------------------------------------  
  //PDP_DEV->mBits.val = 120;//old for test
  PDP_DEV->mBits.val = 172;//new modify for seek in weak signal  by feng.deng @2014-05-13
  //------------------------------------  
  ADP_ST_CONF->mBits.mode = 2;
  ADP_ST_CONF->mBits.hys=  5;
  ADP_ST_CONF->mBits.th = -70;
  //------------------------------------  
  ADP_LPF_CONF->mBits.val = 2;
  //------------------------------------  
  DEPHA_SCAL->mBits.val = 0x5;
  //------------------------------------  
  HW_MUTE->mBits.mnd = 0;
  //------------------------------------  
  SW_MUTE->mBits.hbnd = -90;
  SW_MUTE->mBits.lbnd = -95;
  //------------------------------------  
  UPD_CTRL->mBits.stdif_dly   = 1;
  UPD_CTRL->mBits.agc_vs_rssi = 0; 
  UPD_CTRL->mBits.seek_agc_dly= 1; 

  //modify by luther.li 2012-12-06 mBits.agc_adps = 1
  UPD_CTRL->mBits.agc_adps    = 1; 
  
  UPD_CTRL->mBits.predem_dly  = 3; 
  UPD_CTRL->mBits.soft_mute   = 5; 
  UPD_CTRL->mBits.adp_st      = 6; 
  //------------------------------------  
  AUD_BLD0->mBits.hbnd = -83;        
  AUD_BLD0->mBits.lbnd = -95;        
  //------------------------------------  
  AUD_BLD1->mBits.mode = 2;
  AUD_BLD1->mBits.lev2 = -87;
  AUD_BLD1->mBits.lev1 = -91;
  //------------------------------------  
  AGC_HYS->mBits.thresh = 2;
  AGC_HYS->mBits.step   = 2;

  //------------------------------------
if(CHIP_GetHWChipID() == CHIP_ID_SC6531BA){
  	RF_INIT_GAIN->mBits.val   = 0x0335;  
}else{
	RF_INIT_GAIN->mBits.val   = 0x0333;  
}
  //------------------------------------
  
  //-----------------------------------
  //modify by luther.li 2012-12-06 mBits.val = 0xffff
  RF_CFG_DLY->mBits.val = 0xffff;  
  //-----------------------------------
 if(CHIP_GetHWChipID() == CHIP_ID_SC6531BA){
	AGC_BITS_TBL0->mBits.val  = 0x0000; 
	AGC_BITS_TBL1->mBits.val = 0x0001;
	AGC_BITS_TBL2->mBits.val = 0x0301;
	AGC_BITS_TBL3->mBits.val = 0x0302;
	AGC_BITS_TBL4->mBits.val = 0x0303;
	AGC_BITS_TBL5->mBits.val = 0x0313;
	AGC_BITS_TBL6->mBits.val = 0x0323;
	AGC_BITS_TBL7->mBits.val = 0x0333;
	AGC_BITS_TBL8->mBits.val = 0x0334;

	AGC_BITS_TBL9->mBits.val  = 0x0000; 
	AGC_BITS_TBL10->mBits.val = 0x0001;
	AGC_BITS_TBL11->mBits.val = 0x0301;
	AGC_BITS_TBL12->mBits.val = 0x0302;
	AGC_BITS_TBL13->mBits.val = 0x0303;
	AGC_BITS_TBL14->mBits.val = 0x0313;
	AGC_BITS_TBL15->mBits.val = 0x0323;
	AGC_BITS_TBL16->mBits.val = 0x0333;
	AGC_BITS_TBL17->mBits.val = 0x0335;

	AGC_VAL_TBL0->mBits.val  = 7;////13;  
	AGC_VAL_TBL1->mBits.val  = 21;////21;  
	AGC_VAL_TBL2->mBits.val  = 29;////29;  
	AGC_VAL_TBL3->mBits.val  = 35;////35;  
	AGC_VAL_TBL4->mBits.val  = 41;////41;  
	AGC_VAL_TBL5->mBits.val  = 47;////47;  
	AGC_VAL_TBL6->mBits.val  = 53;////53;  
	AGC_VAL_TBL7->mBits.val  = 59;////59;  
	AGC_VAL_TBL8->mBits.val  = 65;////65; 
 }
 else{
	AGC_BITS_TBL0->mBits.val  = 0x0000; 
	AGC_BITS_TBL1->mBits.val = 0x0100;
	AGC_BITS_TBL2->mBits.val = 0x0300;
	AGC_BITS_TBL3->mBits.val = 0x0301;
	AGC_BITS_TBL4->mBits.val = 0x0302;
	AGC_BITS_TBL5->mBits.val = 0x0312;
	AGC_BITS_TBL6->mBits.val = 0x0322;
	AGC_BITS_TBL7->mBits.val = 0x0332;
	AGC_BITS_TBL8->mBits.val = 0x0333;

	AGC_BITS_TBL9->mBits.val  = 0x0000; 
	AGC_BITS_TBL10->mBits.val = 0x0100;
	AGC_BITS_TBL11->mBits.val = 0x0300;
	AGC_BITS_TBL12->mBits.val = 0x0301;
	AGC_BITS_TBL13->mBits.val = 0x0302;
	AGC_BITS_TBL14->mBits.val = 0x0312;
	AGC_BITS_TBL15->mBits.val = 0x0322;
	AGC_BITS_TBL16->mBits.val = 0x0332;
	AGC_BITS_TBL17->mBits.val = 0x0333;

	AGC_VAL_TBL0->mBits.val  = 7;////13;  
	AGC_VAL_TBL1->mBits.val  = 15;////21;  
	AGC_VAL_TBL2->mBits.val  = 23;////29;  
	AGC_VAL_TBL3->mBits.val  = 29;////35;  
	AGC_VAL_TBL4->mBits.val  = 35;////41;  
	AGC_VAL_TBL5->mBits.val  = 41;////47;  
	AGC_VAL_TBL6->mBits.val  = 57;////53;  
	AGC_VAL_TBL7->mBits.val  = 53;////59;  
	AGC_VAL_TBL8->mBits.val  = 59;////65; 

 }
  //-----------------------------------
  AGC_BOND_TBL0->mBits.val  = -120; //-105
  AGC_BOND_TBL1->mBits.val  = -89; 
  AGC_BOND_TBL2->mBits.val  =  -83; 
  AGC_BOND_TBL3->mBits.val  =  -77; 
  AGC_BOND_TBL4->mBits.val  =  -71; 
  AGC_BOND_TBL5->mBits.val  =  -41; 
  AGC_BOND_TBL6->mBits.val  =  -35; 
  AGC_BOND_TBL7->mBits.val  =  -29; 
  AGC_BOND_TBL8->mBits.val  = -13; 
  AGC_BOND_TBL9->mBits.val  =  10; 
/*lint -e542*/
  //----------------------------------- 
}


void FM_SPRD_RF_RegisteWrite(uint32 addr, uint16 value)
{
     ANA_REG_SET(FM_BASE_WRITE_ADDR+(addr&0xFF)*4,value);
}

void GSM_SPRD_RF_RegisteWrite(uint8 addr, uint16 value)
{
     ANA_REG_SET(GSM_BASE_WRITE_ADDR+(addr&0xFF)*4,value);
}

void FM_SPRD_RFInit(void)
{  
    /*FM_SPRD_RF_RegisteWrite(0x151,0x0022); 
	
    GSM_SPRD_RF_RegisteWrite(0x7D,0x0100);   //added by james cheng

    FM_SPRD_RF_RegisteWrite(0x104,0x0333); 
    FM_SPRD_RF_RegisteWrite(0x100,0x0011);  
    FM_SPRD_RF_RegisteWrite(0x102,0x086A);  
    SCI_Sleep(10);
    FM_SPRD_RF_RegisteWrite(0x104,0x0333);  */

   if(CHIP_GetHWChipID() == CHIP_ID_SC6531BA){	
	    GSM_SPRD_RF_RegisteWrite(0x77,0x0f000);
	    GSM_SPRD_RF_RegisteWrite(0x7D,0x0100);

	    FM_SPRD_RF_RegisteWrite(0x131, 0x8022);   //enable FM overload detect
	    //FM_SPRD_RF_RegisteWrite(0x178, 0x009f); 
	    FM_SPRD_RF_RegisteWrite(0x104, 0x0333);
	    FM_SPRD_RF_RegisteWrite(0x100, 0x0011);
	    FM_SPRD_RF_RegisteWrite(0x102, 0x07A6);
	    FM_SPRD_RF_RegisteWrite(0x171, 0x0FFF);
		
		SCI_Sleep(10);
   	}else{
	    FM_SPRD_RF_RegisteWrite(0x151,0x0022); 
	    FM_SPRD_RF_RegisteWrite(0x104,0x0333); 
           FM_SPRD_RF_RegisteWrite(0x100,0x0011);  
           FM_SPRD_RF_RegisteWrite(0x102,0x086A);  
           SCI_Sleep(10);
	    FM_SPRD_RF_RegisteWrite(0x104,0x0333); 
	}
}

void FM_SPRD_RF_IDLE(void)
{  
    FM_SPRD_RF_RegisteWrite(0x100,0x0000); 
}

LOCAL void FM_SPRD_OpenClk(void)
{
    TB_REG_OR(APB_EB0_SET,FM_EB_SET);
    TB_REG_AND(APB_CLK_CFG0,~(BIT_23|BIT_22));	
    TB_REG_OR(APB_CLK_CFG0,BIT_22);	 //set FM clk 26M
}

LOCAL void FM_SPRD_CloseClk(void)
{
    TB_REG_OR(APB_EB0_CLR,FM_EB_CLR);    
}


PUBLIC void FM_SPRD_Enable(void)
{
	SCI_FM_EnableDeepSleep(DISABLE_DEEP_SLEEP); 
	FM_SPRD_OpenClk();  
	SCI_Sleep(50);  
	FM_SPRD_RFInit(); 
    SCI_Sleep(50); 		
	FM_SPRD_RegInit(); 
	FM_SPRD_AGC_SET();
	SCI_Sleep(10); 
}

PUBLIC void FM_SPRD_Disable(void)
{       
    FM_SPRD_RF_IDLE();
	FM_SPRD_CloseClk();
	SCI_Sleep(10);	
	SCI_FM_EnableDeepSleep(ENABLE_DEEP_SLEEP);
	
}



/*****************************************************************************/
//  FUNCTION:     FM_SPRD_GetCurFreq
//  Description:  This function will return curent playing freq
//  Note: is_up: 
/*****************************************************************************/
PUBLIC uint16 FM_SPRD_GetCurFreq(void)
{
#if 1
   volatile AGC_BITS_TBL8_U    *AGC_BITS_TBL8   = (AGC_BITS_TBL8_U    *)REG_AGC_BITS_TBL8  ;
   volatile AGC_VAL_TBL8_U     *AGC_VAL_TBL8    = (AGC_VAL_TBL8_U     *)REG_AGC_VAL_TBL8   ; 

   if(1040 ==  (uint16)((TB_REG_GET(REG_CHAN_FREQ_STS) + RF_FREQ_DIV)/RF_FREQ_DIV))
  {  
  	if(CHIP_GetChipID() == 0x08)
	   {
			AGC_BITS_TBL8->mBits.val = 0x0333;
			AGC_VAL_TBL8->mBits.val  = 59;////65; 
	   }
  }else
  {
		 if(CHIP_GetChipID() == 0x08)
		 {
			AGC_BITS_TBL8->mBits.val = 0x0334;
			AGC_VAL_TBL8->mBits.val  = 65;////65; 
		 }
  }
#endif 
    return (uint16)((TB_REG_GET(REG_CHAN_FREQ_STS) + RF_FREQ_DIV)/RF_FREQ_DIV);  
}

//Add by luther.li 2012-12-01
/*****************************************************************************/
//  FUNCTION:     FM_SPRD_RecStereo
//  Description:  This function will return receive rssi strength
//  Note: is_up: 
/*****************************************************************************/
PUBLIC uint8 FM_SPRD_RecStereo(void)
{
    return (uint8)((TB_REG_GET(REG_FM_CTRL)& FM_CTRL_RXM)>>16);
}


/*****************************************************************************/
//  FUNCTION:     FM_SPRD_RecRSSI
//  Description:  This function will return receive rssi strength
//  Note: is_up: 
/*****************************************************************************/
PUBLIC uint16 FM_SPRD_RecRSSI(void)
{
    return (uint16)((TB_REG_GET(REG_RSSI_STS)) & 0xFF);  
}


/*****************************************************************************/
//  FUNCTION:     FM_SPRD_Seek
//  Description:  This function will seek freq 
//  Note: is_up: 0: seek down, 1: seek up  
//  the hardware will check if beyond up limit, if up to limit.it will also set 
//  interrupt bit!
/*****************************************************************************/
PUBLIC uint16 FM_SPRD_Seek(uint32  freq, uint32 is_up)
{

    uint32 val32;
    uint32 result = 0;
    //uint16 s_frequency = 0;
    uint32 loop = 0;
       
    TB_REG_AND(REG_FM_EN,~FM_EN_GLBEN);     //disable FM 
    
    val32= TB_REG_GET(REG_CHAN)& 0xffff0000;    //set the start freq   
    
    TB_REG_SET(REG_CHAN, val32 | ((freq-1)*RF_FREQ_DIV&0xFFFF));
    
    if(is_up)
    TB_REG_OR (REG_FM_CTRL,FM_CTRL_SEEK_UP);
    else          
    TB_REG_AND(REG_FM_CTRL,~FM_CTRL_SEEK_UP); 
    
    TB_REG_SET(REG_FMCTL_STI,1);    
    TB_REG_OR(REG_INT_EN,INT_EN_STC);    //enable interrupt 

    SCI_Sleep(10);    //Add by luther.li 2012-12-06 add delay time 

	TB_REG_OR(REG_FM_EN,FM_EN_GLBEN);      //start fm search action  

    
	TB_REG_SET(0x8800202c,0x551f1f);
	TB_REG_SET(0x8800203c,0xa03ff);
    //SCI_TRACE_LOW("2013-12-02 0x8800202c =%d\n",TB_REG_GET(0x8800202c));
	//SCI_TRACE_LOW("2013-12-02 0x8800203c =%d\n",TB_REG_GET(0x8800203c));

	
    do{ 
        SCI_Sleep(50);       
        loop++;
		SCI_TRACE_LOW("2012-12-27 loop =%d\n",loop);//Add by luther.li 2012-12-27 
    }while((!(TB_REG_GET(REG_INT_STS)&INT_STS_STC))&&(loop<15)); 
   
    TB_REG_OR(REG_INT_CLR,INT_CLR_STC);  //clear int bit  
    
    if(!((TB_REG_GET(REG_RSSI_STS)&RSSI_STS_SF)>>16))  
        result = 1;  
    
    TB_REG_AND(REG_INT_EN,~INT_EN_STC);    //disable interrupt

#ifdef FM_SPRD_DEBUG  
    FM_SPRD_GetDebugInfo();     
#endif   

    TB_REG_SET(0x8800202c,0x88ffff);
	TB_REG_SET(0x8800203c,0x10ffff);
    return result;  

}

/*****************************************************************************/
//  FUNCTION:     FM_SPRD_Seek
//  Description:  This function will seek freq 
//  Note: is_up: 
/*****************************************************************************/
PUBLIC uint16 FM_SPRD_Tune(uint32 freq, uint32 is_up)
{
    uint32 val32;
    uint32 result = 0;
    uint32 loop = 0;
    //uint16 s_frequency = 0;    
   
    TB_REG_AND(REG_FM_EN,~FM_EN_GLBEN); 
 
    val32 = TB_REG_GET(REG_CHAN)& 0xffff0000;
    
    TB_REG_SET(REG_CHAN, val32 | ((freq - 1)*RF_FREQ_DIV & 0xFFFF ));
        
    TB_REG_SET(REG_FMCTL_STI,2);  

    TB_REG_OR(REG_INT_EN,INT_EN_STC);

    TB_REG_OR(REG_FM_EN,FM_EN_GLBEN);     

	TB_REG_SET(0x8800202c,0x551f1f);
	TB_REG_SET(0x8800203c,0xa03ff);
    //SCI_TRACE_LOW("2013-12-02 0x8800202c =%d\n",TB_REG_SET(0x8800202c,0x88ffff));
	//SCI_TRACE_LOW("2013-12-02 0x8800203c =%d\n",TB_REG_SET(0x8800203c,0x88ffff));
	
	
    do{ 
        SCI_Sleep(50);       
        loop++;
		SCI_TRACE_LOW("2012-12-27 loop =%d\n",loop);//Add by luther.li 2012-12-27 
    }while((!(TB_REG_GET(REG_INT_STS)&INT_STS_STC)) && (loop<15));//Modify by luther.li 2012-12-11 set loop to 15 
     
    TB_REG_OR(REG_INT_CLR,INT_CLR_STC);  //clear int bit  
    
    if(!((TB_REG_GET(REG_RSSI_STS)&RSSI_STS_SF)>>16))   
        result = 1;
        
    TB_REG_AND(REG_INT_EN,~INT_EN_STC);
    
#ifdef FM_SPRD_DEBUG        
    FM_SPRD_GetDebugInfo();
#endif

    
    TB_REG_SET(0x8800202c,0x88ffff);
	TB_REG_SET(0x8800203c,0x10ffff);
    return result;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  
