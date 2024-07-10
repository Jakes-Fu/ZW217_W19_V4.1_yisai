/******************************************************************************
 ** File Name:      nand_controller.c                                         *
 ** Author:         Nick.zhao                                                 *
 ** DATE:           01/06/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/

#include "fdl_bootloader_trc.h"
#include "tf_drv_nand.h"
#include "os_api.h"
#include "sc_reg.h"
//#include "sc_fdl_reg.h"
//#include "fdl_stdio.h"
//#include "chip_drv_common_io.h"

/*lint -e551*/
/******************************************************************************
                          Macro define
******************************************************************************/
//NandFlash command
#define NF_READ_ID          0x90
#define NF_READ_STATUS      0x70
#define NF_READ_1ST         0x00
#define NF_READ_2ND         0x01
#define NF_RESET            0xFF
#define NF_READ_SPARE       0x50
#define NF_SWITCH_SPARE     0x50fd
#define NF_ID_ADDR          0x00
#define NF_BKERASE_ID       0x60
#define NF_BKERASE_EXC_ID   0xD0
#define NF_WRITE_ID         0x80
#define NF_WRITE_EXC_ID     0x10
#define NF_SWITCH_MAIN      0x00fd
#define NF_RANDOM_WRITE_ID  0x85

#define NF_TIMEOUT_VAL      30                 //the timeout value is 30ms
#define PAGE_SIZE_S         512
#define SPARE_SIZE_S        16
#define PAGE_SIZE_L         2048
#define SPARE_SIZE_L        64

#define MT29F4G_SET_FEATURE 0xEF
#define MT29F4G_GET_FEATURE 0xEE
#define MT29F4G_ECC_ENABLE  0x08

#undef _USE_THREADX

#undef VM_SUPPORT

#undef SCI_PASSERT
#define SCI_PASSERT

/*lint -e652 */
#undef SCI_DisableIRQ                 
#define SCI_DisableIRQ()    ((void)0)
#undef SCI_RestoreIRQ
#define SCI_RestoreIRQ()   ((void)0)
/*lint +e652 */

#undef SCI_TRACE_LOW
#define SCI_TRACE_LOW(...)



#define TM_SendTestPointRequest(param1, param2)

#ifdef VM_SUPPORT
#define NLC_DISABLEIRQ  do {SCI_DisableIRQ();}while(0)
#define NLC_RESTOREIRQ  do {SCI_RestoreIRQ();}while(0)
#else
#define NLC_DISABLEIRQ
#define NLC_RESTOREIRQ
#endif

#define MAX_AHB_TIMING_NUM                             (10)
#define DEFAULT_AHB_VAL                                (0xffffffff)
#define DEFAULT_NAND_TIMING                            (0xffffffff)
#define ORIGINAL_NAND_TIMING                           (0xfff0f)

/******************************************************************************
                          Struct Definition
******************************************************************************/
typedef struct
{
    uint32 ahb_clk;
    uint32 timing_param;
} NAND_AHB_TIMING_T;

typedef enum
{
    NF_STATE_IDLE = 0x00,
    NF_STATE_WRITE,
    NF_STATE_READ,
    NF_STATE_ERASE,
    NF_STATE_RESET,

    NF_STATE_MAX
} NF_STATE_E;
typedef struct _HwECCCtrl
{
    uint8 mID;
    uint8 dID;
    uint8 open; //if open set 1, else set 0
    ERR_NF_E  (*eccOpen) (BOOLEAN enable, struct _HwECCCtrl *ctl);
    ERR_NF_E  (*correct)(struct _HwECCCtrl *ctl);
} HwECCCtrl;
/******************************************************************************
                          Global Variable
******************************************************************************/
static NAND_PARA_T g_Nand_Para;     //current nand's specification
static uint32 g_CmdSetting;          //used to increase the speed of operate Nand

LOCAL uint32 g_Nand_Fisrt_clk;
LOCAL NAND_TIMING_T g_Nand_Timing;
LOCAL NAND_AHB_TIMING_T g_Nand_Ahb_Timing[MAX_AHB_TIMING_NUM] =
{
    {DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
    {DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
    {DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
    {DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
    {DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
    {DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
    {DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
    {DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
    {DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING},
    {DEFAULT_AHB_VAL, DEFAULT_NAND_TIMING}
};

LOCAL NF_STATE_E s_nf_state = NF_STATE_IDLE;
LOCAL ERR_NF_E   s_nf_operation_status = ERR_NF_SUCCESS;
LOCAL BOOLEAN    s_conv_flag = SCI_FALSE;
BOOLEAN s_NandHwOnDieEccEnable = FALSE;
LOCAL BOOLEAN s_NandHwOnDieEccOpen = FALSE;
LOCAL HwECCCtrl s_NandHwOnDieEccCtrl = {0};

#if defined(CHIP_ENDIAN_DEFAULT_LITTLE) && defined(FDL2_MODULE)
extern BOOLEAN   g_IsNbl;
#endif //defined(CHIP_ENDIAN_DEFAULT_LITTLE)

LOCAL volatile NAND_CTL_REG_T  *s_nandctl_ptr= (volatile NAND_CTL_REG_T *) NAND_CTL_BASE;

/******************************************************************************
                        Local Functions Prototype
******************************************************************************/
LOCAL ERR_NF_E NANDCTL_Ecc_Trans (uint8 *pEccIn, uint8 *pEccOut,uint8 nSct);
LOCAL void NandCopy (uint8 *pSrc,uint8 *pDst,uint32 len);
LOCAL ERR_NF_E _MT29F4G_ecc_enable(BOOLEAN enable, struct _HwECCCtrl *ctl);
LOCAL ERR_NF_E _MT29F4G_ecc_correct(struct _HwECCCtrl *ctl);
PUBLIC ERR_NF_E NANDCTL_Read_Status();
PUBLIC void NANDCTL_ONDIE_ECCOpen(BOOLEAN open);
LOCAL ERR_NF_E NANDCTL_ONDIE_ECCCorrect();
/******************************************************************************
                        Functions
******************************************************************************/
unsigned int EndianConv_32 (unsigned int value)
{
#ifdef TF_LITTLE_ENDIAN
    unsigned int nTmp = (value >> 24 | value << 24);

    nTmp |= ( (value >> 8) & 0x0000FF00);
    nTmp |= ( (value << 8) & 0x00FF0000);
    return nTmp;
#else
    return value;
#endif
}

LOCAL void NFC_SET_COMMAND (uint32 cmd)
{
    s_nandctl_ptr->cmd = g_CmdSetting| (cmd);
}
LOCAL void NFC_WAIT_COMMAND_FINISH()
{
    uint32 nfc_cmd, counter;

    nfc_cmd = s_nandctl_ptr->cmd;
    counter=SCI_GetTickCount();

    while ( (nfc_cmd& (BIT_31)) && (SCI_GetTickCount()-counter<NF_TIMEOUT_VAL))
    {
        nfc_cmd=s_nandctl_ptr->cmd;
    }

    if (s_nandctl_ptr->cmd& (BIT_31))
    {
        //SCI_TRACE_LOW:"NFC timeout happened!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_DRV_NAND_176_112_2_18_1_1_28_11,(uint8*)"");
        TM_SendTestPointRequest (0x6e616e64, 2);
        NANDCTL_Reset();
    }
}

LOCAL ERR_NF_E _NF_Read(
    uint8 nSct,
    uint8 *pMBuf,
    uint8 *pSBuf,
    uint8 *pEcc
)
{
    uint8 Ecc_val_in[16];
    uint32 *pEcc_val= (uint32 *) Ecc_val_in;
    uint32 i;
    uint32 cmd;
    ERR_NF_E ret = ERR_NF_SUCCESS;

    //send cmd
    if(NULL!=pEcc && NULL!=pMBuf)
    {
        cmd=NF_READ_1ST| (0x1<<21);
    }
    else
    {
        cmd=NF_READ_1ST;    
    }

    //Configure nfc_cmd
    NFC_SET_COMMAND (cmd);
    //Read nfc_cmd until the command has been executed completely.
    NFC_WAIT_COMMAND_FINISH();
    if(s_NandHwOnDieEccCtrl.open)
    {
        ret = NANDCTL_ONDIE_ECCCorrect();
        if(ret != ERR_NF_SUCCESS)
        {
            return ret;
        }
    }
    
    //get main data if needed
    if(NULL!=(uint32*)pMBuf)
    {
        NandCopy ( (uint8 *) NFC_MBUF_ADDR, (uint8 *) pMBuf,nSct*PAGE_SIZE_S);
    }
    
    //get spare data if needed
    if(NULL!=pSBuf)
    {
        s_conv_flag = SCI_TRUE;
        NandCopy ( (uint8 *) NFC_SBUF_ADDR, (uint8 *) pSBuf,nSct*SPARE_SIZE_S);
        s_conv_flag = SCI_FALSE;
    }
    
    //Get and check ECC if needed
    if(NULL!=pEcc && NULL!=pSBuf && (!s_NandHwOnDieEccCtrl.open))
    {
        switch (nSct)
        {
            case 4:
                pEcc_val[3] = EndianConv_32 (s_nandctl_ptr->ecc3);
                //lint -fallthrough
            case 3:
                pEcc_val[2] = EndianConv_32 (s_nandctl_ptr->ecc2);
                //lint -fallthrough
            case 2:
                pEcc_val[1] = EndianConv_32 (s_nandctl_ptr->ecc1);
                //lint -fallthrough
            case 1:
                pEcc_val[0] = EndianConv_32 (s_nandctl_ptr->ecc0);
                NANDCTL_Ecc_Trans (Ecc_val_in,pEcc,nSct);
                break;
            default:
                break;
        }
        for(i = 0;i < nSct*4;i += 4)
        {
            if ((pEcc[i]!=pSBuf[g_Nand_Para.eccpos+i*4]) ||
                (pEcc[i+1]!=pSBuf[g_Nand_Para.eccpos+i*4+1]) ||
                (pEcc[i+2]!=pSBuf[g_Nand_Para.eccpos+i*4+2]))
            {
            return ERR_NF_FAIL;
            }
        }
    }
    return ret;
}


LOCAL void _NF_SetState (NF_STATE_E state)
{
    s_nf_state = state;
}

LOCAL NF_STATE_E _NF_GetState (void)
{
    return s_nf_state;
}

LOCAL void _NF_SaveOperationStatus (void)
{
    s_nf_operation_status = NANDCTL_Read_Status();
}

LOCAL ERR_NF_E _NF_GetOperationStatus (void)
{
    return s_nf_operation_status;
}

LOCAL void _NF_StateSync (void)
{

    NF_STATE_E     state;

    NAND_CTL_REG_T  *nandctl_ptr= (NAND_CTL_REG_T *) NAND_CTL_BASE;

    //get the nand state
    state = _NF_GetState();

    switch (state)
    {
        case NF_STATE_IDLE:
            break;

        case NF_STATE_WRITE:
            {
                uint32 nfc_cmd;
                uint32 counter;

                nfc_cmd=nandctl_ptr->cmd;
                counter=0;

                NFC_WAIT_COMMAND_FINISH();

                //save the nand status
                _NF_SaveOperationStatus();

                _NF_SetState (NF_STATE_IDLE);
            }
            break;

        case NF_STATE_ERASE:

            //Set reset command
            NANDCTL_Reset();

            //Change the nand state
            _NF_SetState (NF_STATE_IDLE);

            break;

        case NF_STATE_RESET:
            break;

        default:
            break;

    }

    return;

}

/******************************************************************************
                          Functions
******************************************************************************/
#if 0
uint32 SCI_GetTickCount (void)
{
    volatile uint32 tmp_tick1;
    volatile uint32 tmp_tick2;

    tmp_tick1 = SYSTEM_CURRENT_CLOCK;
    tmp_tick2 = SYSTEM_CURRENT_CLOCK;

    while (tmp_tick1 != tmp_tick2)
    {
        tmp_tick1 = tmp_tick2;
        tmp_tick2 = SYSTEM_CURRENT_CLOCK;
    }

    return tmp_tick1;
}
#endif

#define NF_MC_CMD_ID            (uint16)(0xFD)
#define NF_MC_ADDR_ID           (uint16)(0xF1)
#define NF_MC_WAIT_ID           (uint16)(0xF2)
#define NF_MC_READWORD_ID       (uint16)(0xF3)
#define NF_MC_READBLOCK_ID      (uint16)(0xF4)
#define NF_MC_WRITEWORD_ID      (uint16)(0xF6)
#define NF_MC_WRITEBLOCK_ID     (uint16)(0xF7)
#define NF_MC_DEACTIVECS_ID     (uint16)(0xF9)
#define NF_MC_NOP_ID            (uint16)(0xFA)


#define NAND_MC_BUFSIZE         16

#define NAND_MC_ASMCMD(x)       (uint16)((((x) & 0xff) << 8) | (NF_MC_CMD_ID))
#define NAND_MC_ASMADDR(x)      (uint16)((((x) & 0xff) << 8) | (NF_MC_ADDR_ID))
#define NAND_MC_ASMWAIT(x)      (uint16)((((x) & 0xff) << 8) | (NF_MC_WAIT_ID))
#define NAND_MC_ASMRWORD(x)     (uint16)(((((x)-1)  & 0xff) << 8) | (NF_MC_READWORD_ID))
#define NAND_MC_ASMRBLOCK(x)    (uint16)(((((x)-1)  & 0xff) << 8) | (NF_MC_READBLOCK_ID))
#define NAND_MC_ASMWWORD(x)     (uint16)(((((x)-1)  & 0xff) << 8) | (NF_MC_WRITEWORD_ID))
#define NAND_MC_ASMWBLOCK(x)    (uint16)(((((x)-1)  & 0xff) << 8) | (NF_MC_WRITEBLOCK_ID))
#define NAND_MC_ASMDECS(x)      (uint16)(((((x)-1)  & 0xff) << 8) | (NF_MC_DEACTIVECS_ID))
#define NAND_MC_ASMNOP(x)       (uint16)(((((x)-1)  & 0xff) << 8) | (NF_MC_NOP_ID))



#define  NFC_MC_CMD_SETVALID    BIT_31

#define  NFC_MC_SETBUSWIDTH8    0
#define  NFC_MC_SETBUSWIDTH16   BIT_19

#define  NFC_MC_MODE_ENABLE     BIT_0
#define  NFC_MC_ECC_ENABLE      BIT_21

#define  NFC_MC_CMD_NUM_POS     (22)


typedef enum
{
    NAND_MC_MODE_ENABLE         = 0,
    NAND_MC_MODE_DISABLE        = 1
} Nand_MC_MODE_E;

typedef enum
{
    NAND_MC_ECC_ENABLE          = 0,
    NAND_MC_ECC_DISABLE         = 1
} Nand_MC_ECC_E;

typedef enum
{
    NAND_MC_BUSWIDTH_X8         = 0,
    NAND_MC_BUSWIDTH_X16        = 1
} Nand_MC_BUSWIDTH_E;


LOCAL  uint32  g_mc_buf_pos = 0;


LOCAL void _nand_mc_init (void)
{
    g_mc_buf_pos = 1;
}

LOCAL ERR_NF_E _nand_mc_storebuf (uint16 val)
{
    uint32  pos32;
    uint32  high_flag;
    uint32  *buf_base_ptr = (uint32 *) NAND_CTL_BASE;
    uint32  *buf_ptr;

    if ( (g_mc_buf_pos > NAND_MC_BUFSIZE-1) || (0 == g_mc_buf_pos))
    {
        SCI_PASSERT (0, ("Nand MC Wrong Pos %d", g_mc_buf_pos));/*assert verified*/
    }

    pos32       = (g_mc_buf_pos+1) >> 1;
    high_flag   = (g_mc_buf_pos+1) & 0x1;

    buf_ptr     = buf_base_ptr + pos32;

    if (high_flag)
    {
        *buf_ptr &= 0x0000ffff;
        *buf_ptr |= ( (uint32) val << 16);
    }
    else
    {
        *buf_ptr &= 0xffff0000;
        *buf_ptr |= (uint32) val;
    }

    g_mc_buf_pos++;

    return ERR_NF_SUCCESS;
}

LOCAL ERR_NF_E _nand_mc_excute_cmd (uint16 cmd, Nand_MC_ECC_E ecc_flag_e, Nand_MC_BUSWIDTH_E bus_width_e)
{
    uint32 val;
    uint32 tmp_cmd = 0;

    if ( (g_mc_buf_pos > NAND_MC_BUFSIZE) || (g_mc_buf_pos < 2))
    {
        SCI_PASSERT (0, ("Nand MC Wrong Pos %d", g_mc_buf_pos));/*assert verified*/
    }

    if (NAND_MC_BUSWIDTH_X8 == bus_width_e)
    {
        tmp_cmd = NFC_MC_CMD_SETVALID | NFC_MC_SETBUSWIDTH8;
    }
    else
    {
        tmp_cmd = NFC_MC_CMD_SETVALID | NFC_MC_SETBUSWIDTH16;
    }

    if (NAND_MC_ECC_ENABLE == ecc_flag_e)
    {
        val = tmp_cmd | ( ( (g_mc_buf_pos - 2) & 0xf) << NFC_MC_CMD_NUM_POS) | (uint32) cmd | NFC_MC_ECC_ENABLE;
    }
    else
    {
        val = tmp_cmd | ( ( (g_mc_buf_pos - 2) & 0xf) << NFC_MC_CMD_NUM_POS) | (uint32) cmd;
    }

    //Enable micro code mode
    CHIP_REG_OR (NFC_CMD_SET, NFC_MC_MODE_ENABLE);//lint !e718

    CHIP_REG_SET (NFC_CMD, val);//lint !e718

    return ERR_NF_SUCCESS;
}
/******************************************************************************/
//  Description:   get nand flash on die ecc  enable status
//  Parameter:
//      None
//		
//  Return:
//      SCI_TRUE: on die ecc enable
//      SCI_FALSE: on die ecc disable
/******************************************************************************/
LOCAL BOOLEAN _MT29F4G_get_ecc_flag()
{
    uint8 data[4] ={0};
    uint32 nfc_cmd;
    uint32 counter;
    NAND_CTL_REG_T  *nandctl_ptr= (NAND_CTL_REG_T *) NAND_CTL_BASE;
    ERR_NF_E ret = SCI_FALSE;
    
    _nand_mc_init();

    _nand_mc_storebuf (NAND_MC_ASMNOP (1)); /*lint !e778 confirmed by xuepeng*/

    _nand_mc_storebuf (NAND_MC_ASMADDR (0x90));
    
    _nand_mc_storebuf (NAND_MC_ASMWAIT (0));

    _nand_mc_storebuf (NAND_MC_ASMNOP (1)); /*lint !e778 confirmed by xuepeng*/

     _nand_mc_storebuf (NAND_MC_ASMRWORD (0x4));

     _nand_mc_excute_cmd (NAND_MC_ASMCMD (MT29F4G_GET_FEATURE), NAND_MC_ECC_ENABLE, NAND_MC_BUSWIDTH_X8);

    //Read nfc_cmd until the command has been executed completely.
    NFC_WAIT_COMMAND_FINISH();
    
    NandCopy ( (uint8 *) (NFC_MBUF_ADDR), (uint8 *) data, 4);
    if(MT29F4G_ECC_ENABLE == data[0])
    {
        ret = SCI_TRUE;
    }
    return ret;
}
/******************************************************************************/
//  Description:   Enable or nand flash on mt29f2g die ecc
//  Parameter:
//      ecc_en: SCI_TRUE -- open the on die ecc function
//		ecc_en: SCI_FALSE -- close the on die ecc function
//  Return:
//      ERR_NF_SUCCESS
/******************************************************************************/
LOCAL ERR_NF_E _MT29F4G_ecc_enable(BOOLEAN enable, struct _HwECCCtrl *ctl)
{
    uint8 data[4] ={0};
    uint32 nfc_cmd;
    uint32 counter;
    NAND_CTL_REG_T  *nandctl_ptr= (NAND_CTL_REG_T *) NAND_CTL_BASE;

    if(enable)
    {
        data[0] = MT29F4G_ECC_ENABLE; //if ecc is enable
    }
    NandCopy ( (uint8 *) data, (uint8 *) NFC_MBUF_ADDR, 4);
 
    _nand_mc_init();

    _nand_mc_storebuf (NAND_MC_ASMNOP (1)); /*lint !e778 confirmed by xuepeng*/

    _nand_mc_storebuf (NAND_MC_ASMADDR (0x90));    

    _nand_mc_storebuf (NAND_MC_ASMNOP (1)); /*lint !e778 confirmed by xuepeng*/

    _nand_mc_storebuf (NAND_MC_ASMWWORD (0x4));
     
    _nand_mc_storebuf (NAND_MC_ASMWAIT (0));     

    {
        _nand_mc_excute_cmd (NAND_MC_ASMCMD (MT29F4G_SET_FEATURE), NAND_MC_ECC_ENABLE, NAND_MC_BUSWIDTH_X8);
    }
    //Read nfc_cmd until the command has been executed completely.
    NFC_WAIT_COMMAND_FINISH();
    
    return ERR_NF_SUCCESS;	
}
/******************************************************************************/
//  Description:   Get the ecc correct status
//  Parameter:
//      ctl: ecc ctrl infor
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
LOCAL ERR_NF_E _MT29F4G_ecc_correct(struct _HwECCCtrl *ctl)
{
    uint32 nfc_cmd;
    uint32 counter;
    uint32 status;
    NAND_CTL_REG_T  *nandctl_ptr= (NAND_CTL_REG_T *) NAND_CTL_BASE;

    nfc_cmd=NF_READ_STATUS|BIT_31;

    //Write Command
    nandctl_ptr->cmd=nfc_cmd;

    NFC_WAIT_COMMAND_FINISH();

    //read status
    status=nandctl_ptr->idstatus;

    if ( (status & 0x1) != 0)
    {
        return ERR_NF_FAIL;
    }
    else if ( (status & 0x40) == 0)
    {
        return ERR_NF_BUSY;
    }
    else if((status & 0x08) != 0)
    {
        return ERR_NF_SUCCESS;
    }
    else
    {
        return ERR_NF_SUCCESS;
    }
}

LOCAL ERR_NF_E _nand_ms_write_l_mc (
    uint32 page_addr,
    uint8 index,
    uint8 nSct,
    uint8 *pMBuf,
    uint8 *pSBuf
)
{
    //uint32 nfc_cmd;
    //uint32 counter;
    //NAND_CTL_REG_T  *nandctl_ptr= (NAND_CTL_REG_T *) NAND_CTL_BASE;
    ERR_NF_E ret;
    uint32 nand_addr = 0;
    uint8  col_addr0, col_addr1, row_addr0, row_addr1, row_addr2;
    Nand_MC_BUSWIDTH_E bus_width_e = (Nand_MC_BUSWIDTH_E) g_Nand_Para.buswidth;

    nand_addr = (page_addr + index * 256) * (2-bus_width_e);

    col_addr0= nand_addr & 0x00ff;
    nand_addr = nand_addr>>8;

    if (bus_width_e == NAND_MC_BUSWIDTH_X8)
    {
        col_addr1 =nand_addr & 0x000f;
        nand_addr = nand_addr>>4;
    }
    else
    {
        col_addr1 = nand_addr & 0x0007;
        nand_addr = nand_addr>>3;
    }

    row_addr0 = nand_addr & 0x00ff;
    nand_addr = nand_addr>>8;

    row_addr1 = nand_addr & 0x00ff;
    nand_addr = nand_addr>>8;

    row_addr2 = nand_addr & 0x00ff;

    _nand_mc_init();

    _nand_mc_storebuf (NAND_MC_ASMNOP (1)); /*lint !e778*/

    _nand_mc_storebuf (NAND_MC_ASMADDR (col_addr0));

    _nand_mc_storebuf (NAND_MC_ASMADDR (col_addr1));

    _nand_mc_storebuf (NAND_MC_ASMADDR (row_addr0));

    _nand_mc_storebuf (NAND_MC_ASMADDR (row_addr1));

    if (g_Nand_Para.acycle==5)
    {
        _nand_mc_storebuf (NAND_MC_ASMADDR (row_addr2));
    }

    _nand_mc_storebuf (NAND_MC_ASMNOP (1));/*lint !e778*/

    _nand_mc_storebuf (NAND_MC_ASMWBLOCK (nSct* (2-bus_width_e)));



    _nand_mc_storebuf (NAND_MC_ASMCMD (NF_RANDOM_WRITE_ID));

    _nand_mc_storebuf (NAND_MC_ASMNOP (1));/*lint !e778*/

    nand_addr = (page_addr + 1024 + index*8) * (2-bus_width_e);

    //set start_addr and end_addr
    col_addr0 = (uint8) ( (nand_addr) & 0xff);
    col_addr1 = (uint8) ( (nand_addr >> 8) & 0xf);

    _nand_mc_storebuf (NAND_MC_ASMADDR (col_addr0));

    _nand_mc_storebuf (NAND_MC_ASMADDR (col_addr1));

    _nand_mc_excute_cmd (NAND_MC_ASMCMD (NF_WRITE_ID), NAND_MC_ECC_DISABLE, bus_width_e);
    //Read nfc_cmd until the command has been executed completely.
    NFC_WAIT_COMMAND_FINISH();

    s_conv_flag = SCI_TRUE;
    NandCopy ( (uint8 *) pSBuf, (uint8 *) NFC_MBUF_ADDR,nSct*16);
    s_conv_flag = SCI_FALSE;

    _nand_mc_init();

    _nand_mc_storebuf (NAND_MC_ASMWWORD ( (uint8) ( (nSct*8* (2-bus_width_e)) & 0xff)));

    _nand_mc_storebuf (NAND_MC_ASMNOP (1));/*lint !e778*/

    _nand_mc_storebuf (NAND_MC_ASMCMD (NF_WRITE_EXC_ID));

    _nand_mc_storebuf (NAND_MC_ASMNOP (1));/*lint !e778*/

    _nand_mc_storebuf (NAND_MC_ASMWAIT (0));

    _nand_mc_excute_cmd (NAND_MC_ASMNOP (1), NAND_MC_ECC_DISABLE, bus_width_e);/*lint !e778*/

    _NF_SetState (NF_STATE_WRITE);

    //Read nfc_cmd until the command has been executed completely.

    NFC_WAIT_COMMAND_FINISH();


    if (NF_STATE_WRITE!=_NF_GetState())
    {
        ret = _NF_GetOperationStatus();
    }
    else
    {
        ret = NANDCTL_Read_Status();
    }

    _NF_SetState (NF_STATE_IDLE);

    return ret;

}



/******************************************************************************/
//  Description:   Init nand controller
//  Parameter:
//      ahb_clk:   the frequency of AHB clock
//  Return:
//      ERR_NF_SUCCESS    Init nand controller successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Init (uint32 ahb_clk)
{
    //Enable NFC AHB_CLK
#if defined (PLATFORM_SC6800H)
    REG32 (AHB_CTL0) |= BIT_7;        //lint !e718

#elif defined PLATFORM_SC8800G
    * (volatile uint32 *) AHB_CTL0|= BIT_8;
#else
    //Enable NFC AHB_CLK
    * (volatile uint32 *) AHB_CTL0|= (0x1<<5);
#endif


#if defined(PLATFORM_SC8800G)
    {
        uint32 i;
        /* reset nlc */
        * (volatile uint32 *) AHB_SOFT_RST |= BIT_5;

        for (i = 0; i < 100; i++) {}

        * (volatile uint32 *) AHB_SOFT_RST &= ~BIT_5;
    }
#elif defined(PLATFORM_SC6800H)
    {
        uint32 i;
        /* reset nlc */
        * (volatile uint32 *) AHB_SOFT_RST |= BIT_12;

        for (i = 0; i < 100; i++) {}

        * (volatile uint32 *) AHB_SOFT_RST &= ~BIT_12;
    }
#endif
    //Clear all Nand INT
    REG32 (NFC_INT_SRC) |= (BIT_0|BIT_4|BIT_5);//lint !e718
    //Set NFC_WPN 1     1: read or write  0: only read
    REG32 (NFC_WPN) = 0x1;

    //Set Nand Parameter
    //NANDCTL_SetParam(ahb_clk);
    g_Nand_Fisrt_clk = ahb_clk;

    //Set timeout parameter

#ifdef _USE_INT
    g_Nand_Int_flag=0;
    //Enable NAND timeout int and wpn int
    REG32 (NFC_INT_EN) |= (BIT_0|BIT_4|BIT_5);

    //Register Nand ISR if needed

    //Enable nlc_irq
    CHIPDRV_EnableIRQINT (TB_NLC_INT);
#endif

    return ERR_NF_SUCCESS;

}
/******************************************************************************/
//  Description:   Open nand controller
//  Parameter:
//      pNandPara    the parameter of nandflash
//  Return:
//      ERR_NF_SUCCESS    Open nand controller successfully
/******************************************************************************/

PUBLIC ERR_NF_E NANDCTL_Open (NAND_PARA_T *pNandPara)
{
    uint32 addr_cycle;    //Advance 0: can be set 3 or 4
    //Advance 1: can be set 4 or 5
    uint32 pagetype;      //0: Small page  1: Large page
    uint32 advance;       //Can be set 0 or 1
    uint32 buswidth;      //0: X8 bus width 1: X16 bus width
    //uint32 pin_sel;

    //Initialize g_Nand_Para and g_Nand_Cmd
    g_Nand_Para.acycle = pNandPara->acycle;
    g_Nand_Para.pagesize = pNandPara->pagesize;
    g_Nand_Para.sparesize = pNandPara->sparesize;
    g_Nand_Para.buswidth = pNandPara->buswidth;
    g_Nand_Para.advance = pNandPara->advance;
    g_Nand_Para.eccpos = pNandPara->eccpos;

    if (512==g_Nand_Para.pagesize)
    {
        pagetype=0;
    }
    else
    {
        pagetype=1;
    }

    if (g_Nand_Para.acycle==3)
    {
        addr_cycle=0;
    }
    else if ( (g_Nand_Para.acycle==4) && (g_Nand_Para.advance==1))
    {
        addr_cycle=0;
    }
    else
    {
        addr_cycle=1;
    }

    advance=g_Nand_Para.advance;
    buswidth=g_Nand_Para.buswidth;
    g_CmdSetting= (addr_cycle<<24) | (advance<<23) | (buswidth<<19) | (pagetype<<18) | (0<<16) | (BIT_31);
    if(s_NandHwOnDieEccEnable)
    {
	    NANDCTL_ONDIE_ECCOpen(SCI_TRUE);
    }
    return ERR_NF_SUCCESS;
}

/******************************************************************************/
//  Description:   Close nand controller
//  Parameter:
//      NULL
//  Return:
//      ERR_NF_SUCCESS    Close nand controller successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Close()
{

    //Set g_Nand_Para and g_Nand_Cmd Zero
    g_Nand_Para.acycle = 0;
    g_Nand_Para.pagesize = 0;
    g_Nand_Para.sparesize = 0;
    g_Nand_Para.buswidth = 0;
    g_Nand_Para.advance = 0;
    g_Nand_Para.eccpos = 0;

    g_CmdSetting=0;

    return ERR_NF_SUCCESS;
}

/******************************************************************************/
//  Description:   Set NF parameter according to AHB_CLK
//  Parameter:
//      ahb_clk    the frequency of AHB_CLK
//  Return:
//      ERR_NF_SUCCESS    Set NF parameter successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_SetParam (uint32 ahb_clk)
{
    uint32 counter, i, temp, nCycle;
    BOOLEAN bFind;

    NAND_CTL_REG_T  *nandctl_ptr= (NAND_CTL_REG_T *) NAND_CTL_BASE;

    counter = SCI_GetTickCount();
    // #ifdef _USE_THREADX
    SCI_DisableIRQ();

    //#endif
    //Wait until last Nand cmd completed.
    while ( ( (nandctl_ptr->cmd>>31) &0x1) && ( (counter+NF_TIMEOUT_VAL) >= (SCI_GetTickCount()))) {}

#ifdef _USE_THREADX
    SCI_ASSERT (counter!=NF_TIMEOUT_VAL);/*assert verified*/
#else

    if ( ( (nandctl_ptr->cmd>>31) &0x1) && ( (counter+NF_TIMEOUT_VAL) < (SCI_GetTickCount())))
    {
        //SCI_TRACE_LOW:"Set NandParameter Failed!\n\r"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_DRV_NAND_903_112_2_18_1_1_29_12,(uint8*)"");
        SCI_RestoreIRQ();
        return ERR_NF_FAIL;
    }

#endif

    bFind = FALSE;
    i=0;

    do
    {
        if (DEFAULT_AHB_VAL==g_Nand_Ahb_Timing[i].ahb_clk)
        {
            g_Nand_Ahb_Timing[i].ahb_clk = ahb_clk;
            break;
        }
        else if (ahb_clk == g_Nand_Ahb_Timing[i].ahb_clk)
        {
            bFind = TRUE;
            break;
        }
        else
        {
            i++;
        }
    }
    while (i<MAX_AHB_TIMING_NUM);

#ifdef _USE_THREADX
    SCI_ASSERT (MAX_AHB_TIMING_NUM!=i);/*assert verified*/
#else

    if (MAX_AHB_TIMING_NUM == i)
    {
        SCI_RestoreIRQ();
        return ERR_NF_FAIL;
    }

#endif

    if (bFind)
    {
        REG32 (NFC_PARA) =  g_Nand_Ahb_Timing[i].timing_param;/*lint !e661 confirmed by xuepeng*/
    }
    else
    {
        temp = (1000000000>>20) / (ahb_clk>>20);
        nCycle = g_Nand_Timing.nTwcTime/temp;
        g_Nand_Ahb_Timing[i].timing_param = ORIGINAL_NAND_TIMING| (nCycle<<4);/*lint !e661 confirmed by xuepeng*/
        REG32 (NFC_PARA) =  g_Nand_Ahb_Timing[i].timing_param;/*lint !e661 confirmed by xuepeng*/
    }

    // #ifdef _USE_THREADX
    SCI_RestoreIRQ();
    //#endif
    return ERR_NF_SUCCESS;
}



/******************************************************************************/
//  Description:   Input the nandflash parameter
//  Parameter:
//      pNandTiming    the frequency of AHB_CLK
//  Return:
//      ERR_NF_SUCCESS    Set NF parameter successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_InitParam (NAND_TIMING_PTR pNandTiming)
{
    g_Nand_Timing.nTeTime = pNandTiming->nTeTime;
    g_Nand_Timing.nTpTime = pNandTiming->nTpTime;
    g_Nand_Timing.nTrTime = pNandTiming->nTrTime;
    g_Nand_Timing.nTwcTime = pNandTiming->nTwcTime;

    //Set Nand timing Parameter
    return NANDCTL_SetParam (g_Nand_Fisrt_clk);

}
/******************************************************************************/
//  Description:   update the pID to update the hw ecc ctrl
//  Parameter:
//      pID      nandflash id
//  Return:
//      None
/******************************************************************************/
LOCAL void NANDCTL_SetHwEccCtrl(uint8 *pID)
{
    /*only take effect for nandflash with MId:0x2c, DevID:0xbc/0xac*/
    if((0x2c == pID[0]) && ((0xbc == pID[1]) || (0xac == pID[1])))
    {
        s_NandHwOnDieEccCtrl.eccOpen = _MT29F4G_ecc_enable;
        s_NandHwOnDieEccCtrl.correct = _MT29F4G_ecc_correct;
        s_NandHwOnDieEccCtrl.open = SCI_FALSE;
        s_NandHwOnDieEccEnable = SCI_TRUE;
    }
    else
    {
        s_NandHwOnDieEccCtrl.eccOpen = NULL;
        s_NandHwOnDieEccCtrl.correct = NULL;   
        s_NandHwOnDieEccCtrl.open = SCI_FALSE;        
        s_NandHwOnDieEccEnable = SCI_FALSE;
    }
}
/******************************************************************************/
//  Description:   Enable or nand flash on  die ecc fucntion
//  Parameter:
//      ecc_en: SCI_TRUE -- open the on die ecc function
//		ecc_en: SCI_FALSE -- close the on die ecc function
//  Return:
//      ERR_NF_SUCCESS
/******************************************************************************/
PUBLIC void NANDCTL_ONDIE_ECCOpen(BOOLEAN open)
{
    if(open && (!s_NandHwOnDieEccCtrl.open))
    {
        if(s_NandHwOnDieEccCtrl.eccOpen)
        {
            s_NandHwOnDieEccCtrl.eccOpen(SCI_TRUE, &s_NandHwOnDieEccCtrl);
            s_NandHwOnDieEccCtrl.open = SCI_TRUE;
        }
    }
    else if((!open) && (s_NandHwOnDieEccCtrl.open))
    {
        if(s_NandHwOnDieEccCtrl.eccOpen)
        {
            s_NandHwOnDieEccCtrl.eccOpen(SCI_FALSE, &s_NandHwOnDieEccCtrl);
            s_NandHwOnDieEccCtrl.open = SCI_FALSE;
        }
    }
}
/******************************************************************************/
//  Description:   get on die ecc status
//  Parameter:
//		None
//  Return:
//      ERR_NF_SUCCESS
/******************************************************************************/
LOCAL ERR_NF_E NANDCTL_ONDIE_ECCCorrect()
{
    return s_NandHwOnDieEccCtrl.correct(&s_NandHwOnDieEccCtrl);
}
/******************************************************************************/
//  Description:   Read nandflash ID
//  Parameter:
//      pID      out  the address of nandflash id
//  Return:
//      ERR_NF_SUCCESS     Get the ID successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Read_ID (uint8 *pID)
{
    volatile uint32 nfc_cmd;
    //uint32 counter;
    NAND_CTL_REG_T  *nandctl_ptr= (NAND_CTL_REG_T *) NAND_CTL_BASE;



    if (NULL==pID)
    {
        return ERR_NF_FAIL;
    }

    nfc_cmd=NF_READ_ID|BIT_31;

    //Write command
    nandctl_ptr->cmd=nfc_cmd;

    //NFC_WAIT_COMMAND_FINISH();
    NFC_WAIT_COMMAND_FINISH();

    *pID= (nandctl_ptr->idstatus) &0xff;
    * (pID+1) = ( (nandctl_ptr->idstatus) & 0xff00) >>8;
    * (pID+2) = ( (nandctl_ptr->idstatus) & 0xff0000) >>16;
    * (pID+3) = ( (nandctl_ptr->idstatus) & 0xff000000) >>24;
    NANDCTL_SetHwEccCtrl(pID);
    return ERR_NF_SUCCESS;

}
/******************************************************************************/
//  Description:   Get nand status
//  Parameter:
//      none
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Read_Status()
{
    uint32 nfc_cmd;
    //uint32 counter;
    uint32 status;
    NAND_CTL_REG_T  *nandctl_ptr= (NAND_CTL_REG_T *) NAND_CTL_BASE;

    nfc_cmd=NF_READ_STATUS|BIT_31;

    //Write Command
    nandctl_ptr->cmd=nfc_cmd;


    NFC_WAIT_COMMAND_FINISH();


    //read status
    status=nandctl_ptr->idstatus;

    if ( (status & 0x1) != 0)
    {
        return ERR_NF_FAIL;
    }
    else if ( (status & 0x40) == 0)
    {
        return ERR_NF_BUSY;
    }
    else
    {
        return ERR_NF_SUCCESS;
    }

}

/******************************************************************************/
//  Description:   Reset NandFlash
//  Parameter:
//      None
//  Return:
//      ERR_NF_SUCCESS     Reset nandflash successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Reset(void)
{
    uint32 nfc_cmd;
    uint32 counter;
    NAND_CTL_REG_T  *nandctl_ptr= (NAND_CTL_REG_T *) NAND_CTL_BASE;
    uint32 i;

RESET_LOOP:
    nfc_cmd=NF_RESET| (0x1UL<<31);

    nandctl_ptr->cmd=nfc_cmd;

    //Read nfc_cmd until the command has been executed completely.
    // NFC_WAIT_COMMAND_FINISH();
    {
        nfc_cmd = nandctl_ptr->cmd;
        counter=SCI_GetTickCount();

        while ( (nfc_cmd& (BIT_31)) && (SCI_GetTickCount()-counter<NF_TIMEOUT_VAL))
        {
            nfc_cmd=nandctl_ptr->cmd;
        }

        if (nandctl_ptr->cmd& (BIT_31))
        {
            //SCI_TRACE_LOW:"NFC timeout happened!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_DRV_NAND_1142_112_2_18_1_1_30_13,(uint8*)"");
            TM_SendTestPointRequest (0x6e616e64, 2);

            for (i=0; i<0x1000; i++) {}

            goto RESET_LOOP;
        }
    }

    return ERR_NF_SUCCESS;
}
/******************************************************************************/
//  Description:   Read main part and spare part of nand, ECC value can be get
//                 if necessary. This function is used for small page nandflash.
//  Parameter:
//      page_addr:   in   the nand page address.
//      pMBuf:       in   the address of Main part
//      pSBuf:       in   the address of Spare part
//      pEcc:        out  the address of ECC value
//  Return:
//      ERR_NF_SUCCESS    Read nand successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_MS_Read_S (
    uint32 page_addr,
    uint8 *pMBuf,
    uint8 *pSBuf,
    uint8 *pEcc
)
{
    uint32 rd_cnt;
    BOOLEAN bReadNand;
    ERR_NF_E ret;
    //set start_addr and end_addr
    s_nandctl_ptr->start_addr0=page_addr;
    s_nandctl_ptr->end_addr0=0xffffffff;

    rd_cnt = 0;     
    bReadNand = TRUE;
    do
    {
        ret = _NF_Read(1, pMBuf, pSBuf, pEcc);
        if(ERR_NF_SUCCESS == ret)
        {
            bReadNand = TRUE;
        }
        else
        {
            bReadNand = FALSE;
        }
        rd_cnt++;
    }
    while ((rd_cnt<=3) && (!bReadNand));

    return ERR_NF_SUCCESS;

}
/******************************************************************************/
//  Description:   Read main part of nand, ECC value can be get
//                 if necessary.
//  Parameter:
//      page_addr:   in   the nand page address.
//      pMBuf:       in   the address of Main part
//  Return:
//      ERR_NF_SUCCESS    Read nand successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_M_Read_S (
    uint32 page_addr,
    uint8 *pMBuf
)
{
    //set start_addr and end_addr
    s_nandctl_ptr->start_addr0=page_addr;
    s_nandctl_ptr->end_addr0=page_addr+PAGE_SIZE_S-1;

    _NF_Read(1, pMBuf, NULL, NULL);
    return ERR_NF_SUCCESS;

}
/******************************************************************************/
//  Description:   Read spare part of nand
//  Parameter:
//      page_addr:   in   the nand page address.
//      pSBuf:       in   the address of Spare part
//  Return:
//      ERR_NF_SUCCESS    Read nand successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_S_Read_S (
    uint32 page_addr,
    uint8 *pSBuf
)
{
    uint32 cmd;

    //set start_addr and end_addr
    s_nandctl_ptr->start_addr0=page_addr;
    s_nandctl_ptr->end_addr0=page_addr+SPARE_SIZE_S-1;     //It can also be set as (start_addr+g_Nand_Para->sparesize-1)

    cmd=NF_READ_SPARE;
    //Configure nfc_cmd
    NFC_SET_COMMAND (cmd);
    //Read nfc_cmd until the command has been executed completely.
    NFC_WAIT_COMMAND_FINISH();
    cmd=NF_SWITCH_MAIN;
    //Configure nfc_cmd
    NFC_SET_COMMAND (cmd);
    //Read nfc_cmd until the command has been executed completely.
    NFC_WAIT_COMMAND_FINISH();


    s_conv_flag = SCI_TRUE;
    NandCopy ( (uint8 *) NFC_SBUF_ADDR, (uint8 *) pSBuf,SPARE_SIZE_S);
    s_conv_flag = SCI_FALSE;

    return ERR_NF_SUCCESS;

}

/******************************************************************************/
//  Description:   Write main part and spare part of nand
//  Parameter:
//      page_addr: in  the nand page address.
//      pMBuf:     in  the address of main part
//      pSBuf:     in  the address of spare part
//      ecc_en:    in  the flag to enable ECC
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_MS_Write_S (uint32 page_addr,uint8 *pMBuf,uint8 *pSBuf,uint8 ecc_en)
{
    uint32 cmd;//,nfc_cmd;
    //uint32 counter;
    NAND_CTL_REG_T  *nandctl_ptr= (NAND_CTL_REG_T *) NAND_CTL_BASE;
    ERR_NF_E ret;
    uint8 Ecc_val_in[4];
    uint8 Ecc_val_out[4];
    uint32 *pEcc_val= (uint32 *) Ecc_val_in;

    NLC_DISABLEIRQ

    //set start_addr and end_addr
    nandctl_ptr->start_addr0=page_addr;
    nandctl_ptr->end_addr0=0xffffffff;


    if (1==ecc_en)
    {
        //Open ECC
        REG32 (NFC_ECC_EN) =0x1;


        //Copy main data to Nand Buffer
        NandCopy (pMBuf, (uint8 *) NFC_MBUF_ADDR,PAGE_SIZE_S);
        //Get ECC
        *pEcc_val = EndianConv_32 (nandctl_ptr->ecc0);
        //Close ECC
        REG32 (NFC_ECC_EN) =0x0;
        //Transite the nand_controller's ECC to XSR ECC
        NANDCTL_Ecc_Trans (Ecc_val_in,Ecc_val_out,1);
        //Put the ecc value into the spare buffer
        pSBuf[g_Nand_Para.eccpos] = Ecc_val_out[0];
        pSBuf[g_Nand_Para.eccpos+1] = Ecc_val_out[1];
        pSBuf[g_Nand_Para.eccpos+2] = Ecc_val_out[2];
    }
    else
    {

        //Copy main data to Nand Buffer
        NandCopy (pMBuf, (uint8 *) NFC_MBUF_ADDR,PAGE_SIZE_S);

    }

    //Copy spare data to Nand Buffer
    s_conv_flag = SCI_TRUE;
    NandCopy (pSBuf, (uint8 *) NFC_SBUF_ADDR,SPARE_SIZE_S);
    s_conv_flag = SCI_FALSE;

    //Begin to write
    cmd=NF_WRITE_ID;

    NFC_SET_COMMAND (cmd);

    _NF_SetState (NF_STATE_WRITE);

    //Read nfc_cmd until the command has been executed completely.

    NFC_WAIT_COMMAND_FINISH();

    if (NF_STATE_WRITE!=_NF_GetState())
    {
        ret = _NF_GetOperationStatus();
    }
    else
    {
        ret = NANDCTL_Read_Status();
    }

    _NF_SetState (NF_STATE_IDLE);

    NLC_RESTOREIRQ

    return ret;

}
/******************************************************************************/
//  Description:   Write main part of nand
//  Parameter:
//      page_addr: in  the nand page address.
//      pMBuf:     in  the main part address.
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_M_Write_S (uint32 page_addr,uint8 *pMBuf)
{

    NAND_CTL_REG_T  *nandctl_ptr= (NAND_CTL_REG_T *) NAND_CTL_BASE;
    ERR_NF_E ret;
    uint32 cmd;//,nfc_cmd;
    //uint32 counter;

    NLC_DISABLEIRQ

    //Copy main data to Nand Buffer
    NandCopy (pMBuf, (uint8 *) NFC_MBUF_ADDR,PAGE_SIZE_S);

    //set start_addr and end_addr
    nandctl_ptr->start_addr0=page_addr;
    nandctl_ptr->end_addr0=page_addr +PAGE_SIZE_S - 1;

    //Begin to write
    cmd=NF_WRITE_ID;
    NFC_SET_COMMAND (cmd);

    _NF_SetState (NF_STATE_WRITE);

    //Read nfc_cmd until the command has been executed completely.
    NFC_WAIT_COMMAND_FINISH();

    if (NF_STATE_WRITE!=_NF_GetState())
    {
        ret = _NF_GetOperationStatus();
    }
    else
    {
        ret = NANDCTL_Read_Status();
    }

    _NF_SetState (NF_STATE_IDLE);

    NLC_RESTOREIRQ

    return ret;

}
/******************************************************************************/
//  Description:   Write spare part of nand
//  Parameter:
//      page_addr: in  the nand page address.
//      pSBuf      in  the address of spare part
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_S_Write_S (uint32 page_addr,uint8 *pSBuf)
{
    uint32 cmd,nfc_cmd;
    //uint32 counter;
    NAND_CTL_REG_T  *nandctl_ptr= (NAND_CTL_REG_T *) NAND_CTL_BASE;
    ERR_NF_E ret;

    NLC_DISABLEIRQ

    //Copy spare data to Nand Buffer
    s_conv_flag = SCI_TRUE;
    NandCopy (pSBuf, (uint8 *) NFC_SBUF_ADDR,SPARE_SIZE_S);
    s_conv_flag = SCI_FALSE;

    //set start_addr and end_addr
    nandctl_ptr->start_addr0=page_addr;
    // nandctl_ptr->end_addr0=0xffffffff;     //It can also be set as (start_addr+g_Nand_Para->sparesize-1)
    nandctl_ptr->end_addr0=page_addr+SPARE_SIZE_S-1;
    //Begin to write
    //Area switch
    nfc_cmd=NF_SWITCH_SPARE|BIT_31;
    nandctl_ptr->cmd=nfc_cmd;

    NFC_WAIT_COMMAND_FINISH();
    //Send write command
    cmd=NF_WRITE_ID;
    //Configure nfc_cmd
    NFC_SET_COMMAND (cmd);
    //Read nfc_cmd until the command has been executed completely.

    NFC_WAIT_COMMAND_FINISH();

    TM_SendTestPointRequest (0x6e616e64, 1);

    //Swith Main part
    cmd=NF_SWITCH_MAIN;
    //Configure nfc_cmd
    NFC_SET_COMMAND (cmd);
    _NF_SetState (NF_STATE_WRITE);

    //Read nfc_cmd until the command has been executed completely.
    NFC_WAIT_COMMAND_FINISH();

    if (NF_STATE_WRITE!=_NF_GetState())
    {
        ret = _NF_GetOperationStatus();
    }
    else
    {
        ret = NANDCTL_Read_Status();
    }

    _NF_SetState (NF_STATE_IDLE);


    NLC_RESTOREIRQ

    return ret;

}
/******************************************************************************/
//  Description:   Erase a block of nandflash
//  Parameter:
//      block_addr:  in   the nand block address.
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Erase_Block_S (uint32 block_addr)
{
    uint32 cmd;//,nfc_cmd;
    //uint32 counter;
    NAND_CTL_REG_T  *nandctl_ptr= (NAND_CTL_REG_T *) NAND_CTL_BASE;
    ERR_NF_E ret;

    for( ; ; )
    {
        NLC_DISABLEIRQ

        //set start_addr and end_addr
        nandctl_ptr->start_addr0=block_addr;

        cmd=NF_BKERASE_ID;

        NFC_SET_COMMAND (cmd);

        _NF_SetState (NF_STATE_ERASE);

        NLC_RESTOREIRQ

        //Read nfc_cmd until the command has been executed completely.
        NFC_WAIT_COMMAND_FINISH();

        NLC_DISABLEIRQ

        if (NF_STATE_ERASE == _NF_GetState())
        {
            ret=NANDCTL_Read_Status();
            _NF_SetState (NF_STATE_IDLE);

            NLC_RESTOREIRQ
            break;
        }
        else
        {
            ret=NANDCTL_Read_Status();
        }

        NLC_RESTOREIRQ
    }    /*lint !e506 */

    return ret;
}

/******************************************************************************/
//  Description:   Read main part and spare part of nand, ECC value can be get
//                 if necessary. This function is used for large page nandflash.
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number
//      pMBuf:       in   the address of Main part
//      pSBuf:       in   the address of Spare part
//      pEcc:        out  the address of ECC value
//  Return:
//      ERR_NF_SUCCESS    Read nand successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_MS_Read_L (
    uint32 page_addr,
    uint8 index,
    uint8 nSct,
    uint8 *pMBuf,
    uint8 *pSBuf,
    uint8 *pEcc
)
{
    uint32 rd_cnt;
    BOOLEAN bReadNand;
    ERR_NF_E ret;

    rd_cnt = 0;     
    bReadNand = TRUE;
    do
    {
        //read main data
        bReadNand = FALSE;
        s_nandctl_ptr->start_addr0 = (page_addr + index * 256) * (2-g_Nand_Para.buswidth);
        s_nandctl_ptr->end_addr0 = (page_addr + (index + nSct) * 256) * (2-g_Nand_Para.buswidth) - 1;
        ret = _NF_Read(nSct, pMBuf, NULL, pEcc);
        if(ERR_NF_SUCCESS == ret) 
        {
            //read spare data
            s_nandctl_ptr->start_addr0 = (page_addr + 1024 + index*8) * (2-g_Nand_Para.buswidth);
            s_nandctl_ptr->end_addr0 = (page_addr + 1024 + (index+nSct) *8) * (2-g_Nand_Para.buswidth) - 1;
            ret = _NF_Read(nSct, NULL, pSBuf, pEcc);
            if(ERR_NF_SUCCESS == ret)
            {
                bReadNand = TRUE;
            }
            else
            {
                bReadNand = FALSE;
            }
        }
        else
        {
            bReadNand = FALSE;
        }
        rd_cnt++;
    }
    while ( (rd_cnt<=3) && (!bReadNand));
    
    return ret;

}
/******************************************************************************/
//  Description:   Read main part of nand, ECC value can be get
//                 if necessary.
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number
//      pMBuf:       in   the address of Main part
//      pEcc:        out  the address of ECC value
//  Return:
//      ERR_NF_SUCCESS    Read nand successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_M_Read_L (
    uint32 page_addr,
    uint8 index,
    uint8 nSct,
    uint8 *pMBuf
)
{
    ERR_NF_E ret;
    //read main data
  
    s_nandctl_ptr->start_addr0 = (page_addr + index * 256) * (2-g_Nand_Para.buswidth);
    s_nandctl_ptr->end_addr0 = (page_addr + (index + nSct) * 256) * (2-g_Nand_Para.buswidth) - 1;
    ret = _NF_Read(nSct, pMBuf, NULL, NULL);

    return ret;
}
/******************************************************************************/
//  Description:   Read spare part of nand
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number
//      pSBuf:       in   the address of Spare part
//  Return:
//      ERR_NF_SUCCESS    Read nand successfully
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_S_Read_L (
    uint32 page_addr,
    uint8 index,
    uint8 nSct,
    uint8 *pSBuf
)
{
    ERR_NF_E ret;
    uint32 rd_cnt;
    BOOLEAN bReadNand;
    
    rd_cnt = 0;     
    bReadNand = TRUE;
    do
    {
        //read spare data
        s_nandctl_ptr->start_addr0 = (page_addr + 1024 + index*8) * (2-g_Nand_Para.buswidth);
        s_nandctl_ptr->end_addr0 = (page_addr + 1024 + (index+nSct) *8) * (2-g_Nand_Para.buswidth) - 1;
        ret = _NF_Read(nSct, NULL, pSBuf, NULL);
        if(ret != ERR_NF_SUCCESS){
            bReadNand = FALSE;
        }
        
        rd_cnt++;
    } while ( (rd_cnt<=3) && (!bReadNand));


    return ret;
}

/******************************************************************************/
//  Description:   Write main part and spare part of nand
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number
//      pMBuf:       in   the address of Main part
//      pSBuf:       in   the address of Spare part
//      ecc_en:      in   the flag to enable ECC
//      Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_MS_Write_L (
    uint32 page_addr,
    uint8 index,
    uint8 nSct,
    uint8 *pMBuf,
    uint8 *pSBuf,
    uint8 ecc_en
)
{
    //uint32 counter;
    NAND_CTL_REG_T  *nandctl_ptr= (NAND_CTL_REG_T *) NAND_CTL_BASE;
    ERR_NF_E ret;
    uint8 Ecc_val_in[16];
    uint8 Ecc_val_out[16];
    uint32 *pEcc_val= (uint32 *) Ecc_val_in;

    NLC_DISABLEIRQ

    if (1==ecc_en)
    {
        //Open ECC
        REG32 (NFC_ECC_EN) =0x1;
        //Copy main data to Nand Buffer
        NandCopy (pMBuf, (uint8 *) NFC_MBUF_ADDR,nSct*512);
        if(!s_NandHwOnDieEccCtrl.open)
        {
        //Get ECC
        switch (nSct)
        {
            case 1:
                pEcc_val[0] = EndianConv_32 (nandctl_ptr->ecc0);
                NANDCTL_Ecc_Trans (Ecc_val_in,Ecc_val_out,nSct);
                //Put the ecc value into the spare buffer
                pSBuf[g_Nand_Para.eccpos] = Ecc_val_out[0];
                pSBuf[g_Nand_Para.eccpos+1] = Ecc_val_out[1];
                pSBuf[g_Nand_Para.eccpos+2] = Ecc_val_out[2];

                break;
            case 2:
                pEcc_val[0] = EndianConv_32 (nandctl_ptr->ecc0);
                pEcc_val[1] = EndianConv_32 (nandctl_ptr->ecc1);
                NANDCTL_Ecc_Trans (Ecc_val_in,Ecc_val_out,nSct);
                //Put the ecc value into the spare buffer
                pSBuf[g_Nand_Para.eccpos] = Ecc_val_out[0];
                pSBuf[g_Nand_Para.eccpos+1] = Ecc_val_out[1];
                pSBuf[g_Nand_Para.eccpos+2] = Ecc_val_out[2];
                pSBuf[g_Nand_Para.eccpos+16] = Ecc_val_out[4];
                pSBuf[g_Nand_Para.eccpos+17] = Ecc_val_out[5];
                pSBuf[g_Nand_Para.eccpos+18] = Ecc_val_out[6];
                break;
            case 3:
                pEcc_val[0] = EndianConv_32 (nandctl_ptr->ecc0);
                pEcc_val[1] = EndianConv_32 (nandctl_ptr->ecc1);
                pEcc_val[2] = EndianConv_32 (nandctl_ptr->ecc2);
                NANDCTL_Ecc_Trans (Ecc_val_in,Ecc_val_out,nSct);
                //Put the ecc value into the spare buffer
                pSBuf[g_Nand_Para.eccpos] = Ecc_val_out[0];
                pSBuf[g_Nand_Para.eccpos+1] = Ecc_val_out[1];
                pSBuf[g_Nand_Para.eccpos+2] = Ecc_val_out[2];
                pSBuf[g_Nand_Para.eccpos+16] = Ecc_val_out[4];
                pSBuf[g_Nand_Para.eccpos+17] = Ecc_val_out[5];
                pSBuf[g_Nand_Para.eccpos+18] = Ecc_val_out[6];
                pSBuf[g_Nand_Para.eccpos+32] = Ecc_val_out[8];
                pSBuf[g_Nand_Para.eccpos+33] = Ecc_val_out[9];
                pSBuf[g_Nand_Para.eccpos+34] = Ecc_val_out[10];
                break;
            case 4:
                pEcc_val[0] = EndianConv_32 (nandctl_ptr->ecc0);
                pEcc_val[1] = EndianConv_32 (nandctl_ptr->ecc1);
                pEcc_val[2] = EndianConv_32 (nandctl_ptr->ecc2);
                pEcc_val[3] = EndianConv_32 (nandctl_ptr->ecc3);
                NANDCTL_Ecc_Trans (Ecc_val_in,Ecc_val_out,nSct);
                //Put the ecc value into the spare buffer
                pSBuf[g_Nand_Para.eccpos] = Ecc_val_out[0];
                pSBuf[g_Nand_Para.eccpos+1] = Ecc_val_out[1];
                pSBuf[g_Nand_Para.eccpos+2] = Ecc_val_out[2];
                pSBuf[g_Nand_Para.eccpos+16] = Ecc_val_out[4];
                pSBuf[g_Nand_Para.eccpos+17] = Ecc_val_out[5];
                pSBuf[g_Nand_Para.eccpos+18] = Ecc_val_out[6];
                pSBuf[g_Nand_Para.eccpos+32] = Ecc_val_out[8];
                pSBuf[g_Nand_Para.eccpos+33] = Ecc_val_out[9];
                pSBuf[g_Nand_Para.eccpos+34] = Ecc_val_out[10];
                pSBuf[g_Nand_Para.eccpos+48] = Ecc_val_out[12];
                pSBuf[g_Nand_Para.eccpos+49] = Ecc_val_out[13];
                pSBuf[g_Nand_Para.eccpos+50] = Ecc_val_out[14];
                break;
            default:
                break;
        }

        //Close ECC
        REG32 (NFC_ECC_EN) =0x0;
        }       
    }
    else
    {
        //Copy main data to Nand Buffer
        NandCopy (pMBuf, (uint8 *) NFC_MBUF_ADDR,nSct*512);

    }

    ret = _nand_ms_write_l_mc (page_addr, index, nSct, pMBuf, pSBuf);


    NLC_RESTOREIRQ

    return ret;
}
/******************************************************************************/
//  Description:   Write main part of nand
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number
//      pMBuf:       in   the address of Main part
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_M_Write_L (uint32 page_addr,uint8 index,uint8 nSct,uint8 *pMBuf)
{
    NAND_CTL_REG_T  *nandctl_ptr= (NAND_CTL_REG_T *) NAND_CTL_BASE;
    ERR_NF_E ret;
    uint32 cmd;//,nfc_cmd;
    //uint32 counter;

    NLC_DISABLEIRQ

    //set start_addr and end_addr
    nandctl_ptr->start_addr0 = (page_addr + index * 256) * (2-g_Nand_Para.buswidth);
    nandctl_ptr->end_addr0 = (page_addr + (index + nSct) * 256) * (2-g_Nand_Para.buswidth) - 1;

    //Copy main data to Nand Buffer
    NandCopy (pMBuf, (uint8 *) NFC_MBUF_ADDR,nSct*512);

    //Begin to write
    cmd=NF_WRITE_ID;

    NFC_SET_COMMAND (cmd);

    _NF_SetState (NF_STATE_WRITE);

    //Read nfc_cmd until the command has been executed completely.

    NFC_WAIT_COMMAND_FINISH();

    if (NF_STATE_WRITE!=_NF_GetState())
    {
        ret = _NF_GetOperationStatus();
    }
    else
    {
        ret = NANDCTL_Read_Status();
    }

    _NF_SetState (NF_STATE_IDLE);

    NLC_RESTOREIRQ

    return ret;
}
/******************************************************************************/
//  Description:   Write spare part of nand
//  Parameter:
//      page_addr:   in   the nand page address.
//      index        in   the sector index of one page
//      nSct         in   the sector number
//      pSBuf:       in   the address of Spare part
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_S_Write_L (uint32 page_addr,uint8 index,uint8 nSct,uint8 *pSBuf)
{
    uint32 cmd;//,nfc_cmd;
    //uint32 counter;
    NAND_CTL_REG_T  *nandctl_ptr= (NAND_CTL_REG_T *) NAND_CTL_BASE;
    ERR_NF_E ret;

    NLC_DISABLEIRQ

    //set spare start_addr and end_addr
    nandctl_ptr->start_addr0 = (page_addr + 1024 + index*8) * (2-g_Nand_Para.buswidth);
    nandctl_ptr->end_addr0 = (page_addr + 1024 + (index+nSct) *8) * (2-g_Nand_Para.buswidth) - 1;

    //Copy spare data to Nand Buffer
    s_conv_flag = SCI_TRUE;
    NandCopy (pSBuf, (uint8 *) NFC_SBUF_ADDR,nSct*16);
    s_conv_flag = SCI_FALSE;

    //Begin to write
    cmd=NF_WRITE_ID;

    NFC_SET_COMMAND (cmd);

    _NF_SetState (NF_STATE_WRITE);

    //Read nfc_cmd until the command has been executed completely.

    NFC_WAIT_COMMAND_FINISH();

    if (NF_STATE_WRITE!=_NF_GetState())
    {
        ret = _NF_GetOperationStatus();
    }
    else
    {
        ret = NANDCTL_Read_Status();
    }

    _NF_SetState (NF_STATE_IDLE);

    NLC_RESTOREIRQ

    return ret;
}
/******************************************************************************/
//  Description:   Erase a block of nandflash
//  Parameter:
//      block_addr:   in   the nand block address.
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_BUSY       nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
/******************************************************************************/
PUBLIC ERR_NF_E NANDCTL_Erase_Block_L (uint32 block_addr)
{
    uint32 cmd;//,nfc_cmd;
    //uint32 counter;
    NAND_CTL_REG_T  *nandctl_ptr= (NAND_CTL_REG_T *) NAND_CTL_BASE;
    ERR_NF_E ret;


    do
    {
        NLC_DISABLEIRQ

        //set start_addr and end_addr
        nandctl_ptr->start_addr0=block_addr * (2-g_Nand_Para.buswidth);


        cmd=NF_BKERASE_ID;

        NFC_SET_COMMAND (cmd);

        _NF_SetState (NF_STATE_ERASE);


        NLC_RESTOREIRQ

        //Read nfc_cmd until the command has been executed completely.
        NFC_WAIT_COMMAND_FINISH();

        NLC_DISABLEIRQ

        if (NF_STATE_ERASE == _NF_GetState())
        {
            ret=NANDCTL_Read_Status();
            _NF_SetState (NF_STATE_IDLE);

            NLC_RESTOREIRQ
            break;
        }
        else
        {
            ret=NANDCTL_Read_Status();
        }

        NLC_RESTOREIRQ
    }
    while (1);        /*lint !e506 */

    return ret;

}


/*lint -e{616} -e{825}*/
LOCAL ERR_NF_E NANDCTL_Ecc_Trans (uint8 *pEccIn, uint8 *pEccOut,uint8 nSct)
{
    if (g_Nand_Para.buswidth==0)
    {
        //BUS_WIDTH = 8
        switch (nSct)
        {
            case 4:
                pEccOut[12] = pEccIn[14]<<2 | pEccIn[15]>>6;
                pEccOut[13] = pEccIn[13]<<2 | pEccIn[14]>>6;
                pEccOut[14] = pEccIn[15]<<2 | pEccIn[13]>>6;
            case 3:
                pEccOut[8] = pEccIn[10]<<2 | pEccIn[11]>>6;
                pEccOut[9] = pEccIn[9]<<2 | pEccIn[10]>>6;
                pEccOut[10] = pEccIn[11]<<2 | pEccIn[9]>>6;
            case 2:
                pEccOut[4] = pEccIn[6]<<2 | pEccIn[7]>>6;
                pEccOut[5] = pEccIn[5]<<2 | pEccIn[6]>>6;
                pEccOut[6] = pEccIn[7]<<2 | pEccIn[5]>>6;
            case 1:
                pEccOut[0] = pEccIn[2]<<2 | pEccIn[3]>>6;
                pEccOut[1] = pEccIn[1]<<2 | pEccIn[2]>>6;
                pEccOut[2] = pEccIn[3]<<2 | pEccIn[1]>>6;
                break;
            default:
                break;

        }
    }
    else
    {
        //BUS_WIDTH = 16
        switch (nSct)
        {
            case 4:
                pEccOut[12] = pEccIn[14];
                pEccOut[13] = pEccIn[13];
                pEccOut[14] = pEccIn[15];
            case 3:
                pEccOut[8] = pEccIn[10];
                pEccOut[9] = pEccIn[9];
                pEccOut[10] = pEccIn[11];
            case 2:
                pEccOut[4] = pEccIn[6];
                pEccOut[5] = pEccIn[5];
                pEccOut[6] = pEccIn[7];
            case 1:
                pEccOut[0] = pEccIn[2];
                pEccOut[1] = pEccIn[1];
                pEccOut[2] = pEccIn[3];
                break;
            default:
                break;

        }
    }

    return ERR_NF_SUCCESS;
}

LOCAL void NandCopy (uint8 *pSrc,   //Source Address
                     uint8 *pDst,    //Destination Address
                     uint32 len       //the byte length
                    )
{

#if defined CHIP_ENDIAN_LITTLE && defined SC8800S_LITTLE_ENDIAN && !defined _BIG_ENDIAN
    /* ADS does not support ARM assembly inline in Thumb mode.  Use C temporarily */
    uint32 i, d;
    char *pD = (char *) &d;

    for (i = 0; i < len / 4; i++)
    {
        pD[0] = pSrc[3];
        pD[1] = pSrc[2];
        pD[2] = pSrc[1];
        pD[3] = pSrc[0];
        memcpy (pDst,pD,4);
        pSrc+=4;
        pDst+=4;
    }

#elif defined(CHIP_ENDIAN_DEFAULT_LITTLE) && defined(NAND_ENDIAN_SWITCH)
    uint32 i, d;
    char *pD = (char *) &d;

#if defined FDL2_MODULE && defined CHIP_ENDIAN_BIG

    if (g_IsNbl && s_conv_flag) // only when fdl2 download bootloader, should converce to little
#elif defined FDL2_MODULE && defined CHIP_ENDIAN_LITTLE
        if (!g_IsNbl && s_conv_flag) // only when fdl2 download other, should converce to big
#elif defined CHIP_ENDIAN_LITTLE
            if (s_conv_flag)
#else
                if (0)
#endif
                {
                    for (i = 0; i < len / 4; i++)
                    {
                        pD[0] = pSrc[3];
                        pD[1] = pSrc[2];
                        pD[2] = pSrc[1];
                        pD[3] = pSrc[0];
                        memcpy (pDst,pD,4);
                        pSrc+=4;
                        pDst+=4;
                    }

                    s_conv_flag = SCI_FALSE;
                }
                else
                {
                    memcpy (pDst,pSrc,len);
                }

#else
    memcpy (pDst,pSrc,len);
#endif
}

