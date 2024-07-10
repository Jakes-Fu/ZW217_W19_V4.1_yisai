
#include "tb_comm.h"
#include "tb_drv.h"
#include "tb_hal.h"
#include "diskio.h"
#include "ff.h"
#include "flash.h"

#ifdef PLATFORM_SC8800G
#include "sc8800g_reg_base.h"
#include "sc8800g_reg_global.h"
#include "adi_hal_internal.h"
#endif

#ifdef	PLATFORM_SC6800H
#include "sc6800h_reg_base.h"
#include "sc6800h_reg_global.h"
#include "gpio_reg_v0.h"
#endif

#ifdef	PLATFORM_SC6530
#if defined(CHIP_VER_6531)
#include "sc6531_reg_base.h"
#include "sc6531_reg_global.h"
#else
#include "sc6530_reg_base.h"
#include "sc6530_reg_global.h"
#endif

#include "gpio_reg_v5.h"
#endif
#include "cmddef.h"
#include "stdarg.h"
#include "tf_load.h"
#include "tf_main.h"
#include "tf_cfg.h"
#include "tf_cfg.c"
#include "tf_str.h"
//#include "dma_hal.h"
#include "tf_parse_nor.h"
#ifdef  FLASH_TYPE_SPI
#include "spiflash_spec.h"
#include "spiflash.h"
#include "tf_spiflash.h"
#else
#include "tf_nv_nor.h"
#include "tf_drv_nor.h"
#endif
#include "arm_reg.h"
//#ifdef TF_LOAD_PROTECT
#include "tf_lcd.h"
#include "card_sdio.h"
//#endif
#include "version.h"

#include "mmu_drvapi.h"
#include "ldo_drvapi.h"
#include "mem_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Macro  Definition                                     *
 **---------------------------------------------------------------------------*/
#define TF_STACK_LIMIT         0x10000
#define SIO_TX_EMPTY(s)        ((s) & 0xFF00)

#define TF_M18_REGION_SIZE     (1024) //unit:byte
#define TF_M18_MODE_DIFF_ADDR  (0x10)
#define TF_BUF_SIZE            (128*1024)
#define TF_UMEM_ERASE_FLAG     (0x66)
#define TF_MAX_BUF_SIZE        (0x40000)
//#define FIXNV_SECTOR_NUM       (2)
#define REG32(x)               (*((volatile uint32 *)(x)))
#define TF_UartTrace           rprintf

/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 stack_space[TF_STACK_LIMIT>>2] = {0x0};
LOCAL const TCHAR m_szPacVersion[24] = _T("BP_R1.0.0");
LOCAL FATFS Fatfs;		/* File system object for each logical drive */
LOCAL BYTE buff_write[TF_BUF_SIZE];
LOCAL uint8 tf_erase_buf[TF_M18_REGION_SIZE] = {0x0};
LOCAL uint8 buf_pal_data[TF_MAX_BUF_SIZE] = {0x0};
LOCAL uint8 buf_nv_data[TF_MAX_BUF_SIZE];
PUBLIC TF_UPDATE_INFO_PTR tf_update_info_ptr = NULL;

/**---------------------------------------------------------------------------*
 **                         Global  Variables                                  *
 **---------------------------------------------------------------------------*/
const unsigned long stack_bottom = ((unsigned long)stack_space + TF_STACK_LIMIT);
const uint16  s_calibrationparam_version = 0xff0a;  //for version_common.c
const uint16  s_downloadparam_version    = 0xff05; // for version_common.c

#ifdef FLASH_TYPE_SPI
static const SPIFLASH_SPEC_T *pSpiFlashSpec[2] = {NULL};
uint32 prj_total_size = 0;
uint32 flash_cs0_size = 0;
uint32 flash_cs1_size = 0;
uint8  nID_MID[2], nID_DID1[2], nID_DID2[2];
#else
uint32  g_tf_manufacturer_id,g_tf_device_id,g_tf_extend_id ;
#endif

extern const char*  const s_version_info[];
extern uint32 g_sector_size;
extern uint32 flash_cs0_desity ;
extern uint32 flash_cs1_desity;

extern uint8   flash_cs0_MID ;
extern uint8   flash_cs0_DID1 ;
extern uint8   flash_cs0_DID2 ;

extern uint8   flash_cs1_MID ;
extern uint8   flash_cs1_DID1 ;
extern uint8   flash_cs1_DID2 ;

typedef struct
{
    uint32   start_addr;  // == 0xFFFFFFFF means reach tail
    uint32   size;        // == 0
}FLASH_INFOR_T;

LOCAL const FLASH_INFOR_T  tf_flash_infor[]=
{
    {0x0, 0x4000000},  // the max we support is 64M
    {0x4000000, 0x4000000},  // the max we support is 64M
    {0x8000000, 0x4000000},  // the max we support is 64M
    {0xc000000, 0x4000000},  // the max we support is 64M    
    {0xFFFFFFFF, 0x0}  // last one, indicate tail
};

TF_UPDATE_INFO_T s_tf_update_info;


/**---------------------------------------------------------------------------*
 **                         Extern  Func                                  *
 **---------------------------------------------------------------------------*/                                     

/**---------------------------------------------------------------------------*
 **                          Functions    Definition                            *
 **---------------------------------------------------------------------------*/
void SPIFLASH_HWInit (void)
{
    //static const SPIFLASH_SPEC_T *pSpiFlashSpec = NULL;
    {
        Spiflash_ExtCfg_PRT flash_ext_cfg;            
        int cs = 0;
        /* spiflash cs0 initial */
        /* SC6530:@0x20A00204 Default is 3 ,
SC6531:@0x20A00204 Default is 0 */
        *(volatile uint32 *)0x20A00204 |= 3;
        *(volatile uint32 *)0x20A0000C = 3; //clear sfc int
        cs = SPIFLASH_CS0;
        SFCDRV_CSCfg(cs);
        SPIFLASH_ResetQPIFlash(cs);

        SPIFLASH_ReadID(cs, &nID_MID[cs], &nID_DID1[cs], &nID_DID2[cs]);        

        /* set cs1 start address register before spiflash_cs1 initial */
        pSpiFlashSpec[cs] = SPIFLASH_SPEC_Get(nID_MID[cs], nID_DID1[cs], nID_DID2[cs]);

        if( NULL ==  pSpiFlashSpec[cs])
        {
            return;
        }

        SPIFLASH_Init(cs);

        prj_total_size = pSpiFlashSpec[cs]->spiflash_cfg->flash_size;    

        flash_ext_cfg = (Spiflash_ExtCfg_PRT)pSpiFlashSpec[cs]->pExtcfg;
        flash_cs0_size = flash_ext_cfg->desity;                    

        //SFCDRV_SetCS1StartAddr(flash_cs0_size);
        SPIFLASH_SetCS0DesityFlag(nID_MID[cs], nID_DID1[cs], nID_DID2[cs], flash_cs0_size);

        // dual cs spiflash are used
        // if(prj_total_size > flash_cs0_size)
        {
            /* spiflash cs1s initial */
            cs = SPIFLASH_CS1;
            SFCDRV_CSCfg(SPIFLASH_CS1);
            SPIFLASH_ResetQPIFlash(SPIFLASH_CS1);

            SPIFLASH_ReadID(cs, &nID_MID[cs], &nID_DID1[cs], &nID_DID2[cs]);

            SPIFLASH_Init(cs);

            pSpiFlashSpec[cs] = SPIFLASH_SPEC_Get(nID_MID[cs], nID_DID1[cs], nID_DID2[cs]);

            if( NULL ==  pSpiFlashSpec[cs])
            {
                return;
            }

            SPIFLASH_Init(cs);

            flash_ext_cfg = (Spiflash_ExtCfg_PRT)pSpiFlashSpec[cs]->pExtcfg;
            flash_cs1_size = flash_ext_cfg->desity;

            SPIFLASH_SetCS1DesityFlag(nID_MID[cs], nID_DID1[cs], nID_DID2[cs], flash_cs1_size);
        }
    }

}

#if defined(CHIP_VER_UWS6121E)
void FDL_FlashSPIInit(int cs)
{
    Spiflash_ExtCfg_PRT flash_ext_cfg;

    SFCDRV_CSCfg (cs);
    SFCDRV_ClkCfg(cs, (2<<4) | (8<<8));

    SPIFLASH_ResetQPIFlash (cs);

    SPIFLASH_ReadID (cs, &nID_MID[cs], &nID_DID1[cs], &nID_DID2[cs]);

    if ((nID_MID[cs] == 0) && (nID_DID1[cs] == 0) && (nID_DID2[cs] == 0))
    {
        TF_UartTrace("cs%d unknown flash1\n", cs);//no device
        return;
    }
    else
    {
        TF_UartTrace ("cs%d ID:0x%x 0x%x 0x%x\n", cs, nID_MID[0], nID_DID1[0], nID_DID2[0]);
    }

    /* set cs1 start address register before spiflash_cs1 initial */
    pSpiFlashSpec[cs] = SPIFLASH_SPEC_Get (nID_MID[cs], nID_DID1[cs], nID_DID2[cs]);
    if (NULL == pSpiFlashSpec[cs])
    {
        TF_UartTrace("cs%d unknown flash2\n", cs);
        return;
    }

    SPIFLASH_Init (cs);

    prj_total_size += pSpiFlashSpec[cs]->spiflash_cfg->flash_size;

    flash_ext_cfg = (Spiflash_ExtCfg_PRT) pSpiFlashSpec[cs]->pExtcfg;
    if (SPIFLASH_CS0 == cs)
    {
        flash_cs0_size = flash_ext_cfg->desity;
        SPIFLASH_SetCS0DesityFlag (nID_MID[cs], nID_DID1[cs], nID_DID2[cs], flash_cs0_size);
    }
    else if (SPIFLASH_CS1 == cs)
    {
        flash_cs1_size = flash_ext_cfg->desity;
        SPIFLASH_SetCS1DesityFlag (nID_MID[cs], nID_DID1[cs], nID_DID2[cs], flash_cs1_size);
    }
    TF_UartTrace ("flash_cs0_size=0x%x, flash_cs1_size=0x%x, prj_total_size=0x%x\n",
            flash_cs0_size, flash_cs1_size, prj_total_size);
}
#endif

void _FDL_HWInit(void)
{
#if defined(CHIP_VER_UWS6121E)
    {
        int cs = 0;
        /* spiflash cs0 initial */
        cs = SPIFLASH_CS0;
        FDL_FlashSPIInit(cs);
#ifndef SIMULATE_FLASH2_SUPPORT
        /* spiflash cs1 initial */
        *(volatile unsigned int *)(0x5010c098) = 0x1;
        *(volatile unsigned int *)(0x5010c09C) = 0x1;
        *(volatile unsigned int *)(0x5010c0A0) = 0x1;
        *(volatile unsigned int *)(0x5010c0A4) = 0x1;
        *(volatile unsigned int *)(0x5010c0A8) = 0x1;
        *(volatile unsigned int *)(0x5010c0AC) = 0x1;
        cs = SPIFLASH_CS1;
        FDL_FlashSPIInit(cs);
#endif
    }
#else
    {
        Spiflash_ExtCfg_PRT flash_ext_cfg;
        int cs = 0;

        /* spiflash cs0 initial */
        cs = SPIFLASH_CS0;
        SFCDRV_CSCfg (cs);
        SFCDRV_ClkCfg(cs, (2<<4) | (8<<8));

        SPIFLASH_ResetQPIFlash (cs);

        SPIFLASH_ReadID (cs, &nID_MID[cs], &nID_DID1[cs], &nID_DID2[cs]);

        if ( (nID_MID[cs] == 0) && (nID_DID1[cs] == 0) && (nID_DID2[cs] == 0))
        {
            TF_UartTrace("unknown flash device1\n");//no device
            return;
        }
        else
        {
            TF_UartTrace("cs0 ID:0x%x 0x%x 0x%x\n", nID_MID[0], nID_DID1[0], nID_DID2[0]);
        }

        /* set cs1 start address register before spiflash_cs1 initial */
        pSpiFlashSpec[cs] = SPIFLASH_SPEC_Get (nID_MID[cs], nID_DID1[cs], nID_DID2[cs]);
        if (NULL == pSpiFlashSpec[cs])
        {
            TF_UartTrace("unknown flash device2\n");
            return;
        }

        SPIFLASH_Init (cs);

        prj_total_size = pSpiFlashSpec[cs]->spiflash_cfg->flash_size;

        flash_ext_cfg = (Spiflash_ExtCfg_PRT) pSpiFlashSpec[cs]->pExtcfg;
        flash_cs0_size = flash_ext_cfg->desity;

        //SFCDRV_SetCS1StartAddr (flash_cs0_size);
        SPIFLASH_SetCS0DesityFlag (nID_MID[cs], nID_DID1[cs], nID_DID2[cs], flash_cs0_size);

        TF_UartTrace("flash_cs0_size=0x%x, flash_cs1_size=0x%x, prj_total_size=0x%x\n",
                flash_cs0_size, flash_cs1_size, prj_total_size);
    }
#endif
}

/*****************************************************************************/
// Description :TF_HardWareInit
// Global resource dependence : 
// Author : 
// Note :
/*****************************************************************************/
PUBLIC  void TF_HardWareInit(void)
{
    tf_update_info_ptr = TF_GetUpdateInfo();
    _FDL_HWInit();
    LDO_Init();

    //_GoudaClkDisable();
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_SYS_AHB_DISABLE = SYS_CTRL_DISABLE_SYS_AHB_CLK_ID_GOUDA;
    hwp_sysCtrl->CLK_AP_APB_DISABLE = SYS_CTRL_DISABLE_AP_APB_CLK_ID_GOUDA;
    LDO_TurnOffLDO(LDO_LDO_LCD);
    hal_gouda_term();
    //

    LDO_TurnOnLDO(LDO_LDO_LCD); 
    //_GoudaClkEnable();
    hwp_sysCtrl->REG_DBG = 0xa50001;     /*allow to set disable_reg*/
    hwp_sysCtrl->CLK_SYS_AHB_ENABLE = SYS_CTRL_ENABLE_SYS_AHB_CLK_ID_GOUDA;
    hwp_sysCtrl->CLK_AP_APB_ENABLE = SYS_CTRL_ENABLE_AP_APB_CLK_ID_GOUDA;
}

/*****************************************************************************/
//  Discription: tf load find file in dedicated  dir
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL int32 TF_Findfile (const TCHAR* path, const TCHAR* dst_name, TCHAR* file_path)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    int i;
    TCHAR *fn;
    int find_counts = 0;
    int ret = ERR_SUCCESS;
    TCHAR temp_fn[_MAX_LFN] = {0};    
    TCHAR file_pac_fn[_MAX_LFN] = {0};

#if _USE_LFN
    static TCHAR lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif

    res = f_opendir(&dir, path);
    if (res == FR_OK) 
    {
        i = TF_Load_Wstrlen(path);
        for (;;) 
        {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0)
            {
                break;
            }
            if (fno.fname[0] == '.')
            {
                continue;
            }
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR) 
            {
                continue;
            }
            else
            {
                memset(temp_fn,0,sizeof(temp_fn));
                TF_Load_Wstrcpy(temp_fn,fn);
                TF_Load_Wstrtolower(temp_fn);

                if(TF_Load_Wstrstr(temp_fn, dst_name))
                {                 
                    find_counts++;
                    TF_Load_Wstrcpy(file_pac_fn, fn);
                }                
            }
        }
    }

    if(0 == find_counts)
    {
        ret = ERR_OPEN_FILE;
    }
    else if(find_counts > 1)
    {
        ret = ERR_TOO_MANY_FILE;
    }
    else
    {
        TF_Load_Wstrcpy(file_path, path);
        TF_Load_Wstrcat(file_path, _T("\\"));
        TF_Load_Wstrcat(file_path, file_pac_fn);
    }
    return ret;
}


/*****************************************************************************/
//  Discription: This func will fill find partition index in partition table
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 TF_FindPartitionTableIndex(FILE_ID_TYPE part_id)
{
    uint32 i=0x0;

    for (i = 0x0; i < MAX_FILE_ID_TYPE; i++)
    {   
        if (partition_info[i].file_id == part_id)
        {
            break;
        }
    }

    return i;
}

/*****************************************************************************/
//  Discription: This func will fill do flash erase
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL TFLOAD_ERR_TYPE TF_Do_Erase(FILE_ID_TYPE file_type)
{
    uint32 sec_size,i;
    TF_PARTITION_PTR   flash_part_info_ptr = PNULL;

    rprintf("TF_Do_Erase: address 0x%x, size 0x%x\n", 
            partition_info[file_type].base_address, 
            partition_info[file_type].size);

    flash_part_info_ptr = TF_GetPartitionInfo();
    sec_size = g_sector_size ;
#ifndef FLASH_TYPE_SPI
    TF_FlashWriteEnable(get_flash_base_addr(partition_info[file_type].base_address), 1);
#endif
    switch (file_type)
    {
        case EraseUdisk:
            if (0xFFFFFFFF == partition_info[file_type].size)
            {
                TF_UartTrace("TF_Do_Erase: size is 0xFFFFFFFF, do nothing\n");
                break;
            }

            if (0xFFFFFFFF == partition_info[file_type].base_address)
            {
                TF_UartTrace("TF_Do_Erase: address is 0xFFFFFFFF, do nothing\n");
                break;
            }

            for (i = 0; i < partition_info[file_type].size/sec_size; i++)
            {
                //TF_ShowProgress(sec_size*i, partition_info[file_type].size);
                if (!TF_FlashErase(partition_info[file_type].base_address + i*sec_size, sec_size))
                {
                    return ERR_FLASH_ERASE;
                }
            }
            tf_erase_buf[0] = TF_UMEM_ERASE_FLAG;

            if (TF_FlashWriteArea(partition_info[file_type].base_address, \
                        (char *)&tf_erase_buf[0], TF_M18_REGION_SIZE) != TF_M18_REGION_SIZE)	  
            {
                return ERR_FLASH_ERASE;
            }	
            break;

        case FLASH:
            for (i = 0; i < partition_info[file_type].size/sec_size; i++)
            {
                //TF_ShowProgress(sec_size*i, partition_info[file_type].size);
                if (!TF_FlashErase(partition_info[file_type].base_address + i*sec_size, sec_size))
                {
                    return ERR_FLASH_ERASE;
                }
            }
            break;

        default:
            break;
    }

    //TF_ShowProgress(100, 100);//100%

    return ERR_SUCCESS;
}

/*****************************************************************************/
//  Discription: This func will fill do flash program
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL TFLOAD_ERR_TYPE TF_Do_Program(
        FIL *file_handle,
        uint32 file_size, 
        uint8* file_buf, 
        FILE_ID_TYPE file_type)
{
    FRESULT fret;
    uint8*  buf_ptr =PNULL;
    uint32  uReadSize,oper_size, sec_size, fixnv_addr_in_ps;
    uint32  file_base_addr, total_file_size, offset;
    TF_PARTITION_PTR flash_part_info_ptr = PNULL;
    TFLOAD_ERR_TYPE err_no = ERR_SUCCESS;
    uint32 show_progress_cnts = 0;

    TF_UartTrace("TF_Do_Program: address 0x%x, file_size %u\n", 
            partition_info[file_type].base_address, file_size);
    
    total_file_size = file_size;
    file_base_addr = partition_info[file_type].base_address;
#ifndef FLASH_TYPE_SPI
    TF_FlashWriteEnable( get_flash_base_addr( file_base_addr ), 1);
#endif

    do
    {
        oper_size = ((file_size >= TF_BUF_SIZE) ? TF_BUF_SIZE : file_size);

        fret = f_read(file_handle, file_buf, oper_size, &uReadSize);
        if (FR_OK != fret)
        {
            TF_UartTrace("Read file err\n\n");
            return ERR_READ_FILE;
        }

        switch (file_type)
        {
            case NV:
                flash_part_info_ptr = TF_GetPartitionInfo();
                sec_size = g_sector_size;
                buf_ptr = tf_update_info_ptr->tf_update_flash.alloc_buf_addr;
                fixnv_addr_in_ps = tf_update_info_ptr->tf_update_flash.fixnv_addr;
                TF_FlashRead(fixnv_addr_in_ps, (uint8*)buf_ptr, FIXNV_SECTOR_NUM*sec_size);

#ifdef FLASH_TYPE_SPI
                {    
                    uint32 fix_nv_size = flash_part_info_ptr->ProdinfoAdress - flash_part_info_ptr->NvAdress;
                    memcpy(buf_ptr, file_buf, oper_size); 
                    TF_Prepare_Slave_image(buf_ptr, oper_size);	
                    //TF_ShowProgress(20, 100);//20%
                    err_no = TF_Update_Fixnv(buf_ptr, oper_size);
                }
#else
                memcpy(buf_ptr, file_buf, oper_size);
                //TF_ShowProgress(20, 100);//20%
                err_no = TF_Update_Fixnv(buf_ptr, oper_size);
#endif
                if (err_no != ERR_SUCCESS)
                {
                    return err_no;
                }
                //TF_ShowProgress(100, 100);//100%
                break;

            case UDiSK:
                err_no = TF_Update_Udisk_Image(file_base_addr, file_buf, oper_size, file_size);
                if(err_no != ERR_SUCCESS)
                {
                    return err_no;
                }
                file_base_addr += oper_size;
                //TF_ShowProgress((total_file_size-file_size), total_file_size);
                break;

            case PhaseCheck:
#ifndef FLASH_TYPE_SPI      		
                flash_part_info_ptr = TF_GetPartitionInfo();
                sec_size = g_sector_size ;

                if( partition_info[NV].base_address /sec_size == partition_info[PhaseCheck].base_address/sec_size)
                {
                    buf_ptr = tf_update_info_ptr->tf_update_flash.alloc_buf_addr;
                    fixnv_addr_in_ps = partition_info[NV].base_address;
                    TF_FlashRead(fixnv_addr_in_ps, (uint8*)buf_ptr, sec_size);
                    offset = partition_info[PhaseCheck].base_address - partition_info[NV].base_address;
                    TF_Memcpy(buf_ptr+offset, file_buf, oper_size);
                    err_no = TF_Update_Productinfo(buf_ptr,oper_size);
                }
                else
                {
                    err_no = TF_Update_Productinfo(file_buf,oper_size);
                }

                if(err_no != ERR_SUCCESS)
                {
                    return err_no;
                }
#endif    		  	
                break;

            default:
                if(TF_FlashWriteArea(file_base_addr, file_buf, oper_size) != oper_size)
                {
                    return ERR_FLASH_PROGRAM;
                }
                TF_UartTrace("WriteArea: 0x%x\n", file_base_addr);
                file_base_addr += oper_size;

                if((show_progress_cnts % 10) == 0)
                {
                    //TF_ShowProgress((total_file_size-file_size), total_file_size);
                }
                show_progress_cnts++;
                break;
        }

        file_size -= oper_size;
    } while (file_size);

    //TF_ShowProgress(100, 100);//100%

    return ERR_SUCCESS;
}

/*****************************************************************************/
//  Discription: This func will fill partition info table
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TF_FillPartitionTable(TF_PARTITION_PTR part_info)
{
    uint32 i = 0x0;

    if (part_info->NvSize != 0x0) /* Fix NV */
    {
        i = TF_FindPartitionTableIndex(NV);
        if (i == MAX_FILE_ID_TYPE)
        {
            return FALSE;
        }
        partition_info[i].base_address = part_info->NvAdress;
        partition_info[i].size = part_info->NvSize;
    }
    if (part_info->EfsSize != 0x0) /* Running NV */
    {
        i = TF_FindPartitionTableIndex(FLASH);
        if (i == MAX_FILE_ID_TYPE)
        {
            return FALSE;
        }
        partition_info[i].base_address = part_info->EfsAdress;
        partition_info[i].size = part_info->EfsSize;
    }
    if (part_info->PSSize != 0x0) /* PS */
    {
        i= TF_FindPartitionTableIndex(PS);
        if (i == MAX_FILE_ID_TYPE)
        {
            return FALSE;
        }
        partition_info[i].base_address = part_info->PsAdress;
        partition_info[i].size = part_info->PSSize;
    }
    if (part_info->ResSize != 0x0) /* MMI Resource */
    {
        i= TF_FindPartitionTableIndex(MMIRes);
        if (i == MAX_FILE_ID_TYPE)
        {
            return FALSE;
        }
        partition_info[i].base_address = part_info->ResAdress;
        partition_info[i].size = part_info->ResSize;
    }
    if (part_info->UmemAdress != 0x0)
    {
        i = TF_FindPartitionTableIndex(UDiSK);
        if (i == MAX_FILE_ID_TYPE)
        {
            return FALSE;
        }
        partition_info[i].base_address = part_info->UmemAdress;
        partition_info[i].size = part_info->UmemSize;

        i = TF_FindPartitionTableIndex(EraseUdisk);
        if (i == MAX_FILE_ID_TYPE)
        {
            return FALSE;
        }
        partition_info[i].base_address = part_info->UmemAdress;
        partition_info[i].size = part_info->UmemSize;
    }

    if (part_info->sploadaddr != 0x0)
    {
        i = TF_FindPartitionTableIndex(SPLOAD);
        if (i == MAX_FILE_ID_TYPE)
        {
            return FALSE;
        }
        partition_info[i].base_address = part_info->sploadaddr;
        partition_info[i].size = part_info->sploadsize;
    }

#ifdef CHIP_VER_UWS6121E
    if (part_info->cp_addr != 0x0)
    {
        i = TF_FindPartitionTableIndex(CP); //CP
        if (i == MAX_FILE_ID_TYPE)
        {
            return FALSE;
        }
        partition_info[i].base_address = part_info->cp_addr;
        partition_info[i].size = part_info->cp_size;
    }
    if (part_info->operdata_addr != 0x0)
    {
        i = TF_FindPartitionTableIndex(IMS); 
        if (i == MAX_FILE_ID_TYPE)
        {
            return FALSE;
        }
        partition_info[i].base_address = part_info->operdata_addr;
        partition_info[i].size = part_info->operdata_size;
    }
    if (part_info->config_nv_addr != 0x0)
    {
        i = TF_FindPartitionTableIndex(ConfigNV); 
        if (i == MAX_FILE_ID_TYPE)
        {
            return FALSE;
        }
        partition_info[i].base_address = part_info->config_nv_addr;
        partition_info[i].size = part_info->config_nv_size;
    }
    if (part_info->apn_addr != 0x0)
    {
        i = TF_FindPartitionTableIndex(APN); 
        if (i == MAX_FILE_ID_TYPE)
        {
            return FALSE;
        }
        partition_info[i].base_address = part_info->apn_addr;
        partition_info[i].size = part_info->apn_size;
    }
#endif

    return TRUE;
}
#ifdef FLASH_TYPE_SPI
PUBLIC void Mark_SPIFlash_Type()
{
    SPIFLASH_TYPE_CONFIG_T* s_spiflash_type_cfg_ptr = NULL;
    uint32 len = 0;

    while(len < 0x100000)
    {
        s_spiflash_type_cfg_ptr = (SPIFLASH_TYPE_CONFIG_T*)(0 + len);

        if((s_spiflash_type_cfg_ptr->magic_first == SPIFLASH_MAGIC_FIRST) &&
                (s_spiflash_type_cfg_ptr->magic_second == SPIFLASH_MAGIC_SECOND) &&
                (s_spiflash_type_cfg_ptr->magic_end == SPIFLASH_MAGIC_END))
        {
            /* record spiflash type info in PS*/
            SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs0_desity), flash_cs0_desity & 0xFFFF);
            SPIFLASH_WriteWord((uint32)(&s_spiflash_type_cfg_ptr->cs0_desity) + 2, flash_cs0_desity >> 16);
            SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs0_MID), flash_cs0_MID);
            SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs0_MID) + 2, 0);
            SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs0_DID1), flash_cs0_DID1);
            SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs0_DID1) + 2, 0);
            SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs0_DID2), flash_cs0_DID2);
            SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs0_DID2) + 2, 0);

            SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs1_desity), flash_cs1_desity & 0xFFFF);
            SPIFLASH_WriteWord((uint32)(&s_spiflash_type_cfg_ptr->cs1_desity) + 2, flash_cs1_desity >> 16);
            SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs1_MID), flash_cs1_MID);
            SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs1_MID) + 2, 0);
            SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs1_DID1), flash_cs1_DID1);
            SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs1_DID1) + 2, 0);
            SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs1_DID2), flash_cs1_DID2);
            SPIFLASH_WriteWord((uint32)&(s_spiflash_type_cfg_ptr->cs1_DID2) + 2, 0);
            break;
        }

        len += 4;
    }    

}
#endif

PUBLIC int TF_Load_IMS(const TCHAR *dir)
{
    FIL fp;
    FRESULT fret;
    TCHAR filename[_MAX_LFN];
    TFLOAD_ERR_TYPE errno = ERR_SUCCESS;
    uint8 *pSaveBuff = (BYTE*)(&buff_write);

    errno = TF_Findfile(dir, _T("delta_nv.bin"), filename);
    if (ERR_SUCCESS != errno)
    {
        TF_UartTrace("TF_Load_IMS: ERR_FIND_FILE=0x%x\n", errno);
        return ERR_SUCCESS;
    }
    TF_UartTrace("\nTF_Load_IMS: \n");

    fret = f_open(&fp, filename, FA_READ);         
    if (FR_OK != fret)
    {
        TF_UartTrace("TF_Load_IMS: ERR_OPEN_FILE=0x%x\n", errno);
        errno = ERR_OPEN_FILE;
        goto LOAD_ERR;
    }

    errno = TF_Do_Program(&fp, fp.fsize, pSaveBuff, IMS);
    if (errno != ERR_SUCCESS)
    {
        TF_UartTrace("TF_Load_IMS: err do Program fsize %d\n", fp.fsize);
        goto LOAD_ERR;
    }

LOAD_ERR:    
    return errno;
}

PUBLIC int TF_Load_ConfigNV(const TCHAR *dir)
{
    FIL fp;
    FRESULT fret;
    TCHAR filename[_MAX_LFN];
    TFLOAD_ERR_TYPE errno = ERR_SUCCESS;
    uint8 *pSaveBuff = (BYTE*)(&buff_write);

    errno = TF_Findfile(dir, _T("config_nv.bin"), filename);
    if (ERR_SUCCESS != errno)
    {
        TF_UartTrace("TF_Load_ConfigNV: ERR_FIND_FILE=0x%x\n", errno);
        return ERR_SUCCESS;
    }
    TF_UartTrace("\nTF_Load_ConfigNV: \n");

    fret = f_open(&fp, filename, FA_READ);         
    if (FR_OK != fret)
    {
        TF_UartTrace("TF_Load_ConfigNV: ERR_OPEN_FILE=0x%x\n", errno);
        errno = ERR_OPEN_FILE;
        goto LOAD_ERR;
    }

    errno = TF_Do_Program(&fp, fp.fsize, pSaveBuff, ConfigNV);
    if (errno != ERR_SUCCESS)
    {
        TF_UartTrace("TF_Load_ConfigNV: err do Program fsize %d\n", fp.fsize);
        goto LOAD_ERR;
    }

LOAD_ERR:    
    return errno;
}

PUBLIC int TF_Load_APN(const TCHAR *dir)
{
    FIL fp;
    FRESULT fret;
    TCHAR filename[_MAX_LFN];
    TFLOAD_ERR_TYPE errno = ERR_SUCCESS;
    uint8 *pSaveBuff = (BYTE*)(&buff_write);

    errno = TF_Findfile(dir, _T("apn.bin"), filename);
    if (ERR_SUCCESS != errno)
    {
        TF_UartTrace("TF_Load_APN: ERR_FIND_FILE=0x%x\n", errno);
        return ERR_SUCCESS;
    }
    TF_UartTrace("\nTF_Load_APN: \n");

    fret = f_open(&fp, filename, FA_READ);         
    if (FR_OK != fret)
    {
        TF_UartTrace("TF_Load_APN: ERR_OPEN_FILE=0x%x\n", errno);
        errno = ERR_OPEN_FILE;
        goto LOAD_ERR;
    }

    errno = TF_Do_Program(&fp, fp.fsize, pSaveBuff, APN);
    if (errno != ERR_SUCCESS)
    {
        TF_UartTrace("TF_Load_APN: err do Program fsize %d\n", fp.fsize);
        goto LOAD_ERR;
    }

LOAD_ERR:    
    return errno;
}

/*****************************************************************************/
//  Discription: TF load main routine
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC int TF_Load(const TCHAR *dir)
{
    FIL fp;            
    FRESULT fret;
    FILE_T *paFile = NULL, *pFT = NULL;
    FILE_ID_TYPE nfiletype;
    TF_PARTITION_PTR flash_part_info_ptr = PNULL;
    TFLOAD_ERR_TYPE errno = ERR_SUCCESS;
    BIN_PACKET_HEADER_T bph;
    UINT uReadSize = 0, nPacketSize;
    uint32 SaveSize, i, j;
    uint8 *pSaveBuff = (BYTE*)(&buff_write);
    BOOLEAN fill_ret = FALSE;
    TCHAR pacfilename[_MAX_LFN];
    DWORD tmp_pos;
    uint32 tmp = 0x55;

    TF_UartTrace("TF_Load: Start updating ...\n");

    f_mount(0,&Fatfs);    
    errno = TF_Findfile(dir, _T("tfload.pac"), pacfilename);
    if (ERR_SUCCESS != errno)
    {
        TF_UartTrace("TF_Load: ERR_FIND_FILE=0x%x\n", errno);
        errno = ERR_FIND_FILE;
        goto LOAD_ERR;
    }
    fret = f_open(&fp, pacfilename, FA_READ);         
    if (FR_OK != fret)
    {
        TF_UartTrace("TF_Load: ERR_OPEN_FILE=0x%x\n", errno);
        errno = ERR_OPEN_FILE;
        goto LOAD_ERR;
    }

    nPacketSize = fp.fsize;        
    //check packet size, it must large than the header struct
    if (nPacketSize < sizeof(BIN_PACKET_HEADER_T))
    {
        errno = ERR_PAC_FILE_SIZE;
        TF_UartTrace("TF_Load: nPacketSize = 0x%x\n", nPacketSize);
        goto LOAD_ERR;
    }

    fret = f_read(&fp, &bph, sizeof(BIN_PACKET_HEADER_T), &uReadSize);
    if (FR_OK != fret)
    {
        errno = ERR_READ_FILE;
        TF_UartTrace("TF_Load: err packet header size = 0x%x\n", uReadSize);
        goto LOAD_ERR;
    }

    bph.dwIsNvBackup = TRUE; //The Backup NV Function should be disable in Production Line.
    TF_ShowProgress(0, bph.nFileCount + 4);

    // check packet version
    TF_UartTrace("TF_Load: Pac Version   ");
    for (i = 0; i < 24; i++)
    {
        TF_UartTrace("%c", bph.szVersion[i]);
    }
    TF_UartTrace("\n");

    if (uReadSize != sizeof(BIN_PACKET_HEADER_T) 
            || TF_Load_wstrcmp(bph.szVersion, m_szPacVersion) != 0)
    {
        errno = ERR_PAC_VERSION;
        TF_UartTrace("TF_Load: err .pac ver = %s\n", bph.szVersion);
        goto LOAD_ERR;
    }

    // check packet product name
    TF_UartTrace("TF_Load: Pac Product Name   ");
    for (i = 0; i < 256; i++)
    {
        TF_UartTrace("%c", bph.szPrdName[i]);
    }
    TF_UartTrace("\n");

    if (TF_Load_wstrcmp(bph.szPrdName, TFLOAD_PRODUCT_NAME) != 0)
    {
        errno = ERR_PAC_PRDNAME;
        TF_UartTrace("TF_Load: err .pac product name = %s\n", bph.szPrdName);
        goto LOAD_ERR;
    }

    // check packet size recorded by itself
    if (bph.dwSize != (DWORD)nPacketSize)
    {
        errno = ERR_PAC_FILE_SIZE;
        TF_UartTrace("TF_Load: err packet file size = %u\n", bph.dwSize);
        goto LOAD_ERR;
    }

    // check the number of files packeted by the packet
    if (bph.nFileCount == 0)
    {
        errno = ERR_PAC_FILE_NUM;
        TF_UartTrace("TF_Load: err packet file count = %s", bph.nFileCount);
        goto LOAD_ERR;
    }
    TF_ShowProgress(1, bph.nFileCount + 4);

    uReadSize = 0;
    paFile = (FILE_T*)&buf_pal_data;
    memset(paFile, 0, sizeof(FILE_T)*bph.nFileCount);

    fret = f_read(&fp, paFile, bph.nFileCount*sizeof(FILE_T), &uReadSize);
    if (FR_OK != fret)
    {
        errno = ERR_READ_FILE;
        TF_UartTrace("TF_Load: err read file size1 = %u\n", uReadSize);
        goto LOAD_ERR;
    }
    if (uReadSize != bph.nFileCount * sizeof(FILE_T))
    {        
        errno = ERR_PAC_FILE_SIZE;
        TF_UartTrace("TF_Load: err read  file size2 = %u\n", uReadSize);
        goto LOAD_ERR;
    }

    tmp_pos = ((&fp)->fptr);
    /* The last one file is xxx.xml, and we don't parse the xml file */
    /* first will parse ps and get flash partition info */
    bph.nFileCount -= 1;
    for (i = 0; i < bph.nFileCount; i++)
    {
        pFT = paFile + i;

        // check file size if validate
        if (pFT->nFileSize < 0 || pFT->nFileSize >= nPacketSize)
        {
            errno = ERR_PAC_FILE_SIZE;
            TF_UartTrace("TF_Load: err read file size3 = %u\n", pFT->nFileSize);
            goto LOAD_ERR;
        }

        if (pFT->nFileSize == 0)
        {
            continue;
        }        

        TF_UartTrace("TF_Load:  ");
        for (j = 0; j < 256; j++)
        {
            TF_UartTrace("%c", pFT->szFileID[j]);
        }
        TF_UartTrace("  FileSize %d\n", pFT->nFileSize);

        if (0 == TF_Load_wstrcmp(pFT->szFileID, sz_fileID[PS]))
        {   
            /* we must fill 0xff with the Save buffer */
            memset(pSaveBuff, 0xff, MAX_PS_PARSE_SIZE);
            /* Read from the pac file to savebuff */
            SaveSize = pFT->nFileSize;
            if (SaveSize <= MAX_PS_PARSE_SIZE)
            {
                errno = ERR_PAC_FILE_SIZE;
                goto LOAD_ERR;
            }
            /* for parse, only need read flash 8k size */
            fret = f_read(&fp, pSaveBuff, MAX_PS_PARSE_SIZE, &uReadSize);
            if (FR_OK != fret)
            {
                TF_UartTrace("TF_Load: read partition table err\n");
                errno = ERR_READ_FILE;
                goto LOAD_ERR;
            }
            TF_UartTrace("TF_Load: Partition table Buff 0x%x, uReadSize 0x%x\n", pSaveBuff, uReadSize);

            flash_part_info_ptr = TF_GetPartInfoInPac(pSaveBuff, &fp);
            fill_ret = TF_FillPartitionTable(flash_part_info_ptr);
            if (fill_ret == FALSE)
            {
                errno = ERR_NO_FIND_COMPATIBLE_FILE;
                TF_UartTrace("TF_Load: not find ps file in .pac\n");
                goto LOAD_ERR;
            }
            break;        
        }
        else
        {
            f_lseek(&fp, (((&fp)->fptr) + (pFT->nFileSize)));
            continue;
        }
    } 

    if (i == bph.nFileCount)
    {
        errno = ERR_NO_PS_IN_PAC;
        goto LOAD_ERR;
    }
    TF_ShowProgress(2, bph.nFileCount + 4);

    TF_UartTrace("\nTF_Load: Start Download, FileCount %d\n", bph.nFileCount);

    TF_Initialize_NV_Status();
    TF_ShowProgress(3, bph.nFileCount + 4);

#ifdef TF_LOAD_PROTECT
    errno = TF_UpdateFixnvAddress();
    if(errno != ERR_SUCCESS)
    {
        TF_UartTrace("TF_Load: err get valid fixnv addr, errno=0x%x", errno);
        return errno;
    }
    TF_Update_Sector_By_Slave();
#endif
    TF_ShowProgress(4, bph.nFileCount + 4);

    f_lseek(&fp, tmp_pos);

    for (i = 0; i < bph.nFileCount; i++)
    {
        pFT = paFile + i;

        tmp = REG32(0x60000000);
        if (tmp == 0)
        {
            TF_UartTrace("TF_Load: bootloader broken\n");
            SCI_Sleep(20*1000);
        }

        // check file size if validate
        TF_UartTrace("\nTF_Load: %u -- ", i);
        for (j = 0; j < 256; j++)
        {
            TF_UartTrace("%c", pFT->szFileID[j]);
        }
        TF_UartTrace("\n");

        for (nfiletype = (MAX_FILE_ID_TYPE - 1); nfiletype > 0; nfiletype--)
        {
            if (0 == TF_Load_wstrcmp(pFT->szFileID, sz_fileID[nfiletype]))
            {   
                char szstep[100] = {0};
                sprintf(szstep, "%s", sz_filetype[nfiletype]);
                //TF_DisplayText(szstep,strlen(szstep));
                TF_UartTrace("TF_Load: %s\n", szstep);
                break;        
            }
        }
        if ((pFT->nFileSize < 0) || (pFT->nFileSize >= nPacketSize))
        {
            TF_ShowProgress(i + 4, bph.nFileCount + 4);
            continue;
        }

        if (partition_info[nfiletype].file_type == TF_PARTITION_TYPE_IGNORE)
        {
            f_lseek(&fp, (((&fp)->fptr) + (pFT->nFileSize)));
            TF_ShowProgress(i + 4, bph.nFileCount + 4);
            continue;
        }
        else if (partition_info[nfiletype].file_type == TF_PARTITION_TYPE_CMD)
        {
            errno = TF_Do_Erase(nfiletype);
            if (errno != ERR_SUCCESS)
            {
                TF_UartTrace("TF_Load: err do flash erase: file type=0x%x\n", nfiletype);
                goto LOAD_ERR;
            }
            f_lseek(&fp, (((&fp)->fptr) + (pFT->nFileSize)));
        }
        else if (partition_info[nfiletype].file_type == TF_PARTITION_TYPE_FILE)
        {
            if (pFT->nFileSize != 0x0)
            {
                errno = TF_Do_Program(&fp, pFT->nFileSize, pSaveBuff, nfiletype);
                if (errno != ERR_SUCCESS)
                {
                    TF_UartTrace("TF_Load: err do program nfiletype=0x%x, pFT->nFileSize=0x%x \n", nfiletype, pFT->nFileSize);
                    goto LOAD_ERR;
                }
            }
        }
        else
        {
            f_lseek(&fp, (((&fp)->fptr) + (pFT->nFileSize)));
        }
        TF_ShowProgress(i + 4, bph.nFileCount + 4);
    }

#ifdef FLASH_TYPE_SPI
    Mark_SPIFlash_Type();
#endif
    TF_ShowProgress(bph.nFileCount + 4, bph.nFileCount + 4);

    return ERR_SUCCESS;

LOAD_ERR:    
    return errno;
}

/*****************************************************************************/
//  Discription: TF paraminit
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void TF_ParamInit(void)
{
#if !defined(PLATFORM_UWS6121E)
#ifdef TF_LOAD_SUPPORT
    //	uint32 manufacturer_id,device_id,extend_id;
    LCD_INFO_T lcd_info;
    NOR_FLASH_CONFIG_PTR   tf_flash_config_info_in_ps = NULL;
#endif

    tf_update_info_ptr->tf_update_flash.alloc_buf_addr = (uint32)buf_pal_data;
    tf_update_info_ptr->tf_update_lcd.block_buf_ptr  = TF_GetMainLCDBuffer();

#ifdef FLASH_TYPE_SPI
    tf_update_info_ptr->tf_update_flash.manu_id = nID_MID[0];
    tf_update_info_ptr->tf_update_flash.dev_id = nID_DID1[0];
    tf_update_info_ptr->tf_update_flash.ext_id = nID_DID2[0];
#else	
#ifdef TF_LOAD_PROTECT
    if(tf_update_info_ptr->tf_update_magic != TF_UPDATE_TYPE_MAGIC)
    {    
        TF_FlashWriteEnable(0x0, 1);
        TF_Get_Flash_Id(&manufacturer_id, &device_id, &extend_id);
        tf_update_info_ptr->tf_update_flash.manu_id = manufacturer_id;
        tf_update_info_ptr->tf_update_flash.dev_id = device_id;
        tf_update_info_ptr->tf_update_flash.ext_id  = extend_id;
    }
    else
    {
        manufacturer_id = tf_update_info_ptr->tf_update_flash.manu_id;
        device_id = tf_update_info_ptr->tf_update_flash.dev_id;
        extend_id = tf_update_info_ptr->tf_update_flash.ext_id;        
    }
#endif
#endif   
    //LCD_GetInfo(MAIN_LCD_ID, &lcd_info);
    tf_update_info_ptr->tf_update_lcd.block_buf_ptr = lcd_info.lcdbuff_ptr;
    //tf_update_info_ptr->tf_update_lcd.bus_width = TF_GetLcdBusWidth(MAIN_LCD_ID);
    //tf_update_info_ptr->tf_update_lcd.data_addr = LCM_GetDataAddr(MAIN_LCD_ID);
    //tf_update_info_ptr->tf_update_lcd.dir_type   = TF_GetLcdDirection(MAIN_LCD_ID);
    //tf_update_info_ptr->tf_update_lcd.rotate_mode = TF_GetLcdRotateMode();
    tf_update_info_ptr->tf_update_lcd.lcd_info.lcd_height = lcd_info.lcd_height;
    tf_update_info_ptr->tf_update_lcd.lcd_info.lcd_width  = lcd_info.lcd_width;
    tf_flash_config_info_in_ps = TF_GetPartInfoInPs();
    tf_update_info_ptr->tf_update_flash.fixnv_addr = tf_flash_config_info_in_ps->fixnv_addr;
    //TF_LcmInit(tf_update_info_ptr);
#endif
}

#if defined(FOTA_SUPPORT_ABUP)
extern const NOR_FLASH_CONFIG_T s_platform_patitiion_config;
#endif
/*****************************************************************************/
//  Discription: TF get update info
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC TF_UPDATE_INFO_PTR TF_GetUpdateInfo(void)
{
    //return (TF_UPDATE_INFO_PTR)TFLOAD_SHARE_RAM_ADDRESS;
    s_tf_update_info.tf_update_flash.alloc_buf_addr = (uint32)buf_nv_data;
#ifdef CHIP_VER_UWS6121E
    s_tf_update_info.tf_update_flash.fixnv_addr = 0x705A0000;
#else
    s_tf_update_info.tf_update_flash.fixnv_addr = 0x60F60000;
#endif
    s_tf_update_info.tf_update_flash.manu_id = 0xC8;
    s_tf_update_info.tf_update_flash.dev_id = 0x60;
    s_tf_update_info.tf_update_flash.ext_id = 0x18;
#if defined(FOTA_SUPPORT_ABUP)
	s_tf_update_info.tf_update_flash.fixnv_addr = s_platform_patitiion_config.fixnv_addr;
	g_sector_size = s_platform_patitiion_config.sect_size;
#endif

    return &s_tf_update_info;
}

/*****************************************************************************/
//  Discription: TF download process err string generate
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC char* TF_Errstring_Gen(TFLOAD_ERR_TYPE errno)
{
    return (char*)sz_error_type[errno];
}

PUBLIC  uint32 get_flash_base_addr( uint32 start_addr )
{
    uint32  index = 0, end_addr;

    while( 0xFFFFFFFF != tf_flash_infor[index].start_addr )
    {
        end_addr = tf_flash_infor[index].start_addr + tf_flash_infor[index].size;

        if( (tf_flash_infor[index].start_addr <= start_addr)
                && (end_addr > start_addr) )
        {
            return tf_flash_infor[index].start_addr;
        }

        index++;
    }
    return 0;
}

