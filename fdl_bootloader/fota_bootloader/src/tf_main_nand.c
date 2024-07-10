/******************************************************************************
 ** File Name:      tf_main.c                                               *
 ** Author:           tao.feng                                              *
 ** DATE:            20/12/2010                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE               NAME             DESCRIPTION                               *
 ** 20/10/2010     tao.feng          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
 
 /**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "tb_comm.h"
#include "tb_drv.h"
#include "tb_hal.h"
#include "diskio.h"
#include "ff.h"
#ifdef PLATFORM_SC8800G
#include "sc8800g_reg_base.h"
#include "sc8800g_reg_global.h"
#include "adi_hal_internal.h"
#else	//PLATFORM_SC6800H
#include "sc6800h_reg_base.h"
#include "sc6800h_reg_global.h"
#include "gpio_reg_v0.h"
#endif
#include "gpio_phy.h"
#include "cmddef.h"
#include "stdarg.h"
#include "tf_load.h"
#include "tf_main.h"
#include "tf_cfg.h"
#include "tf_cfg.c"
#include "tf_str.h"
//#ifdef TF_LOAD_PROTECT
#include "tf_lcd.h"
#include "card_sdio.h"
//#endif

#include "sci_ftl_api.h"
#include "version.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Macro  Definition                                     *
 **---------------------------------------------------------------------------*/
#define   TF_STACK_LIMIT		    0x10000
#define   SIO_TX_EMPTY(s)    	    ((s) & 0xFF00)
#define   WAIT_FIFO_EMPTY        \
    		{                                      \
        		while( SIO_TX_EMPTY(*(volatile uint32*)(0x8400000c)));\
    		}

#define TRACE_LOG_MSG \
        	va_start(args, x_format);\
        	nBuf = vsprintf(format_str, x_format, args);\
        	/* was there an error? */\
        	/* Was the expanded string too long? */\
        	va_end(args);\
        	/* Send message to serial buffer! */ \
        	TF_SendMsgOut(format_str, nBuf + 1);

#define TF_MAX_BUF_SIZE                     	128*1024
#define FIXNV_DEF_SIZE							128*1024
#define REG32(x)								(*((volatile uint32 *)(x)))

#define SMALL_PAGE		0
#define LARGE_PAGE		1
#define SECTOR_SIZE			512

typedef struct _OS_INFO {
	int8    Name[24];
	int8    Description[48];
	int8	Version[24];
	uint16	Offset;  // in sectors
	uint16	Size;    // in sectors
} OS_INFO, * POS_INFO;

typedef struct _BOOT_PARAM {
	uint8	Magic[4];
	uint16  Size;
	uint16  TotalSize;
	long    timeStamp;
	uint8	TraceOn;
	uint8	Reserved;
	uint8	CurrentOS;
	uint8	NumOfOS;
	uint16  SizeOfOSInfo;
	uint16  OSOffset;
} BOOT_PARAM, * PBOOT_PARAM;

/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL uint32 stack_space[TF_STACK_LIMIT>>2] = {0x0};
LOCAL const TCHAR m_szPacVersion[24] = _T("BP_R1.0.0");
static const uint8 MAGIC[] = { 'S', 'T', 'B', 'P' };

#ifdef TF_LITTLE_ENDIAN
#define BYTESWITCH(s) ((((s) >> 8) & 0xff) | (((s) << 8) & 0xff00))
#else
#define BYTESWITCH(s) (s)
#endif

LOCAL FATFS Fatfs;		/* File system object for each logical drive */
LOCAL uint8 buf_pal_data[TF_MAX_BUF_SIZE] ={0x0};	//buffer to store FIL
LOCAL uint8 buff_read[TF_MAX_BUF_SIZE] ={0x0};
LOCAL uint8 buff_write[TF_MAX_BUF_SIZE] ={0x0};
__align(8)  unsigned char g_buffer[1*1024*1024] = {0};

LOCAL unsigned int g_SctsPerBlk = 0x0;
LOCAL unsigned int g_BlkSize = 0x0;
LOCAL SCI_FTL_DEV_INFO	ftlDevInfo;
LOCAL SCI_FTL_HANDLE ftlHandle;
PUBLIC TF_UPDATE_INFO_PTR tf_update_info_ptr = NULL;

/**---------------------------------------------------------------------------*
 **                         Global  Variables                                  *
 **---------------------------------------------------------------------------*/
const unsigned long stack_bottom = ((unsigned long)stack_space + TF_STACK_LIMIT);
const uint16  s_calibrationparam_version = 0xff0a;  //for version_common.c
const uint16  s_downloadparam_version    = 0xff05; // for version_common.c
extern const char*  const s_version_info[];
/**---------------------------------------------------------------------------*
 **                         Extern  Func                                  *
 **---------------------------------------------------------------------------*/                                     
extern uint32 TF_GetShareRamAddress(void);
extern uint32 LCM_GetDataAddr(uint32 lcd_id);
//extern PUBLIC void DMA_HAL_Init (void);
/**---------------------------------------------------------------------------*
 **                          Functions    Definition                            *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : write char into Uart_txd fifo
// Global resource dependence : 
// Author : 
// Note :
/*****************************************************************************/
LOCAL void WriteCharToUART(char c)
{
    while ((((*(volatile uint32*)0x8400000c) >> 8 )&0xFF) >= 32 ) {};

    *(volatile uint32*)0x84000000 = c;
}

/*****************************************************************************/
// Description : send message to uart char by char
// Global resource dependence : 
// Author : 
// Note :
/*****************************************************************************/
LOCAL void TF_SendMsgOut(char * buf, int size)
{
    while (size --)
    {
        WriteCharToUART(*(buf++));
    }
    
    WriteCharToUART('\r');
    WriteCharToUART('\n');
}

#define UART_BASE_CLK 26000000
/*****************************************************************************/
// Description :To config uart1, including enable uart, baud-rate:115200, 
//   stop-bit:8, disable dma.
// Global resource dependence : 
// Author : 
// Note :
/*****************************************************************************/
LOCAL  void TF_UartInit(void)
{
    unsigned long baud_rate;
    baud_rate = UART_BASE_CLK/115200;   //for 48MHz clk src

	*(volatile uint32 *)GR_GEN0 |= BIT_21;
#ifdef PLATFORM_SC8800G
	//uart clk sel and div config
	REG32(GR_CLK_DLY)  |=  (BIT_22 | BIT_23);
	REG32(GR_CLK_GEN5) &= ~(BIT_5 | BIT_4 | BIT_3);
#endif
	*(volatile uint32 *)0x84000010 = 0x00;
	*(volatile uint32 *)0x84000018 = 0x1C;
	*(volatile uint32 *)0x8400001C = 0x00;
	*(volatile uint32 *)0x84000024 = (uint16)baud_rate; //115200
	*(volatile uint32 *)0x84000028 = (baud_rate>>8);
}

/*****************************************************************************/
// Description : Send message to uart1 for boot-up log
// Global resource dependence : 
// Author : 
// Note :
/*****************************************************************************/
PUBLIC void TF_UartTrace(
    const char *x_format, ...)
{
    char       format_str[256];
    va_list    args;
    int        nBuf;
    
    WAIT_FIFO_EMPTY
    
    memset (format_str,0,256);
    TRACE_LOG_MSG		/*assert verified*/
    
    WAIT_FIFO_EMPTY
}

uint8 *RB_GetBuffer()
{
	return g_buffer;
}

//#ifdef TF_LOAD_PROTECT
#define CHIP_REG_OR(reg_addr, value)    (*(volatile uint32 *)(reg_addr) |= (uint32)(value))
#define CHIP_REG_AND(reg_addr, value)   (*(volatile uint32 *)(reg_addr) &= (uint32)(value))
#define CHIP_REG_GET(reg_addr)          (*(volatile uint32 *)(reg_addr))
#define CHIP_REG_SET(reg_addr, value)   (*(volatile uint32 *)(reg_addr)  = (uint32)(value))

LOCAL void TF_PM_GPIOInit (void)
{
    int i = 0;
    uint16 gpio_num;

    // Enable all GPIO pages
#if defined (PLATFORM_SC8800G)
    CHIP_REG_OR ( (GR_GEN0), (GEN0_GPIO_EN | GEN0_GPIO_RTC_EN));
    ANA_REG_OR (ANA_AGEN,AGEN_RTC_GPIO_EN);
    SCI_Sleep (5);
    ANA_REG_OR (ANA_AGEN,AGEN_GPIO_EN);
#endif

    for (;;)
    {
        if (pm_gpio_default_map[i].gpio_num == PM_INVALID_SHORT_VAL)
        {
            break;
        }

        gpio_num  = pm_gpio_default_map[i].gpio_num;

        if (pm_gpio_default_map[i].dir == PM_OUTPUT)
        {
            // Output only
            // Config it can be READ and WRITE.(1)
            GPIO_SetDataMask (gpio_num,SCI_TRUE);
            // Config it to be OUTPUT.(1)
            GPIO_SetDirection (gpio_num,SCI_TRUE);
            // MASK it.(0)
            GPIO_DisableIntCtl (gpio_num);

            // Set default value
            if (0xFFFF != pm_gpio_default_map[i].default_val)
            {
                GPIO_SetValue (gpio_num,pm_gpio_default_map[i].default_val);
            }

        }
        else
        {
            // Config it can be READ and WRITE.(1)
            GPIO_SetDataMask (gpio_num,SCI_TRUE);
            // Config it to be INPUT.(0)
            GPIO_SetDirection (gpio_num,SCI_FALSE);
        }

        i++;
    }
}


PUBLIC void TF_PM_Init (void)
{
    int i = 0;
#ifdef PLATFORM_SC8800G
    CHIP_REG_OR ( (GR_GEN0), (GEN0_PIN_EN));
    ANA_REG_OR (ANA_AGEN, (AGEN_PINREG_EN)); // enable gpio base romcode
#endif

    for (;;)
    {
        // Check if search to end
        if (pm_func[i].addr == PM_INVALID_VAL)
        {
            break;
        }

#ifdef PLATFORM_SC8800G

        // Write the value to chip
        //if (0x02000000 != (pm_func[i].addr & 0x02000000))  //d-die 0x8c000000; a-die 0x82000000
        if (!ANA_IS_ANA_REG (pm_func[i].addr))
        {
            CHIP_REG_SET (pm_func[i].addr ,pm_func[i].value);
        }
        else
        {
            ANA_REG_SET (pm_func[i].addr ,pm_func[i].value);
        }

#else
        CHIP_REG_SET (pm_func[i].addr ,pm_func[i].value);
#endif
        i++;
    }

    TF_PM_GPIOInit();
}
//#endif

#define _GETMID(ID) ((uint8)(ID>>8))
extern int g_xsr_probe;

SCI_FTL_PROBE* SCI_GetVendorSwProbe(unsigned __int64 ID)
{
	uint8	MID;

	MID	= _GETMID(ID);

    return &g_xsr_probe;

}

/*****************************************************************************/
//  Discription: This func will fill partition info table
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TF_FillPartitionTable (SCI_FTL_DEV_INFO *ftlDevInfo_ptr)
{
	SCI_FTL_PART	partsInfo;
	uint32 i, j;
	
	if(!ftlDevInfo_ptr)
	{
		return FALSE;
	}

	partsInfo = ftlDevInfo_ptr->partsInfo;
	
	for(i=0; i<partsInfo.partsNum; i++)
	{
		for(j=0; j<MAX_FILE_ID_TYPE; j++)
		{
			if(partition_info[j].part_id == partsInfo.parts[i].partId)
			{
				partition_info[j].base_address = partsInfo.parts[i].ofst;
				partition_info[j].size = partsInfo.parts[i].len;
			}
		}
	}

	return TRUE;
}


/*****************************************************************************/
// Description :TF_HardWareInit
// Global resource dependence : 
// Author : 
// Note :
/*****************************************************************************/
LOCAL  void TF_HardWareInit(void)
{
	uint32 lRet;
	uint32 nandId = 0x0;

    tf_update_info_ptr = TF_GetUpdateInfo();

	TF_UartInit();
	// enable system count
#ifdef PLATFORM_SC8800G
   	REG32(GR_GEN0) |= (BIT_19|BIT_27);
#else
   	REG32(GR_GEN0) |= (BIT_7|BIT_12);
	REG32(GR_MISC0) |= (BIT_27|BIT_28);	// 24KB XRAM switch to ARM
#endif

//#ifdef TF_LOAD_PROTECT
	TF_PM_Init();
//#endif

	NANDCTL_Init (100000000);
	TF_LcdInit();
	LDO_Init();
	
	if(SCI_FTL_ERROR_NONE != SCI_FTL_MODULE_Init())
	{
		return;
	}
		
	lRet = SCI_FTL_Load(nandId);
	if(SCI_FTL_ERROR_NONE != lRet)
	{
		return;
	}
	
	if(SCI_FTL_ERROR_NONE != SCI_FTL_GetDevInfo(nandId, &ftlDevInfo))
	{
		SCI_FTL_UnLoad(nandId);
		return;
	}

	TF_FillPartitionTable(&ftlDevInfo);

	g_SctsPerBlk = ftlDevInfo.pgPerBlk * ftlDevInfo.sctsPerPg;
	g_BlkSize = g_SctsPerBlk * ftlDevInfo.sctSize;
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
LOCAL  uint32 TF_FindPartitionTableIndex(FILE_ID_TYPE part_id)

{
	uint32 i=0x0;
	
	for(i=0x0; i<MAX_FILE_ID_TYPE; i++)
	{
		if(partition_info[i].file_id == part_id)
		{
			break;
		}
	}

	return i;
}

void * MakekernelHeader(uint32 ps_size,uint32 *pimg_hdr_size, int32 page_type )
{

	int32  sects_per_block;
	uint32 buf_size = 0;
	uint32 boot_size= 0;
	BOOT_PARAM * pBoot;
	OS_INFO    * pOS;

	BOOT_PARAM * pBootBak;
	OS_INFO    * pOSBak;


	void * pbuf = NULL;

	sects_per_block = (SMALL_PAGE == page_type) ? 32 : 256;
	boot_size = sizeof(BOOT_PARAM) + sizeof( OS_INFO );

	buf_size = sects_per_block * SECTOR_SIZE;

	pbuf = RB_GetBuffer();
	memset(pbuf,0xFF,buf_size);

	//init boot param	
	pBoot = (BOOT_PARAM *)pbuf;
	memset(pBoot,0,sizeof(BOOT_PARAM));
	memcpy( pBoot->Magic, MAGIC, sizeof(pBoot->Magic) );
	pBoot->Size = BYTESWITCH((uint16)sizeof( BOOT_PARAM ));
	pBoot->TotalSize = BYTESWITCH((uint16)boot_size);
	//time(&pBoot->timeStamp);
	pBoot->TraceOn   = ( uint8 )1;
	pBoot->Reserved  = ( uint8 )0;
	pBoot->CurrentOS = ( uint8 )0;
	pBoot->NumOfOS   = ( uint8 )1;
	pBoot->SizeOfOSInfo =  BYTESWITCH((uint16)sizeof( OS_INFO ));
	pBoot->OSOffset  = BYTESWITCH((uint16) sizeof( BOOT_PARAM ));

	//init OS info
	pOS = (OS_INFO*)(((uint8 *)pbuf) + sizeof(BOOT_PARAM));
	memset(pOS,0,sizeof(OS_INFO));
	pOS->Offset = BYTESWITCH((uint16)sects_per_block);
	pOS->Size = BYTESWITCH((uint16)(( ps_size + SECTOR_SIZE - 1 ) / SECTOR_SIZE ));

	pBootBak = (BOOT_PARAM*)(((uint8 *)pbuf) + buf_size/2);
	pOSBak = (OS_INFO*)(((uint8 *)pBootBak) + sizeof(BOOT_PARAM));
	memcpy(pBootBak,pBoot,sizeof(BOOT_PARAM));
	memcpy(pOSBak,pOS,sizeof(OS_INFO));

	*pimg_hdr_size = buf_size;

	return pbuf;
	
}

/*****************************************************************************/
//  Discription: This func will fill do flash erase
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL  TFLOAD_ERR_TYPE TF_Do_Erase(FILE_ID_TYPE file_type)
{
	 SCI_FTL_PARTITION_INFO ftlPartitionInfo;
	 uint32 nandId = 0;

	if(SCI_FTL_ERROR_NONE != SCI_FTL_Open(nandId, partition_info[file_type].part_id, &ftlHandle, 4))
	{
		return ERR_FLASH_OPEN;
	}

	if(SCI_FTL_ERROR_NONE != SCI_FTL_GetPartInfo(ftlHandle, &ftlPartitionInfo))
	{
		SCI_FTL_Close(ftlHandle);
		return ERR_FLASH_OPEN;
	}
		 
 	if(SCI_FTL_ERROR_NONE != SCI_FTL_Erase(ftlHandle, 0, ftlPartitionInfo.sctTotalNum))
  	{
		  SCI_FTL_Close(ftlHandle);
		  return ERR_FLASH_ERASE;
	}

	 SCI_FTL_Close(ftlHandle);
     TF_ShowProgress(100, 100);

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
	uint32  uReadSize; //,oper_size, sec_size, fixnv_addr_in_ps;
	TFLOAD_ERR_TYPE err_no = ERR_SUCCESS;
	uint32 nandId = 0;
	uint32	rRet;
	SCI_FTL_PARTITION_INFO ftlPartitionInfo;
    uint32 new_kernel_length;
    uint8   *new_address;
	UINT buff_size;
	UINT uTotalReadSize = 0;
	UINT uRemainReadSize = 0;
	uint32 show_progress_cnts = 0;
	uint8 current_percent = 0;
	uint32 	write_sector;
	uint32 i;
		
	if(SCI_FTL_ERROR_NONE != SCI_FTL_Open(nandId, partition_info[file_type].part_id, &ftlHandle, 4))
	{
		return ERR_FLASH_OPEN;
	}

	if(SCI_FTL_ERROR_NONE != SCI_FTL_GetPartInfo(ftlHandle, &ftlPartitionInfo))
	{
		SCI_FTL_Close(ftlHandle);
		return ERR_FLASH_OPEN;
	}
	
	if(file_type == NV) /****************对于NV,读取FIXNV_DEF_SIZE(128KB)或者sctTotalNum size*****************************/
	{
        uint8* pReadBuff =  (BYTE*)(&buff_read);
        uint8* pSaveBuff = (BYTE*)(&buff_write);
        uint32 ReadSize,SaveSize;

		if(ftlPartitionInfo.sctTotalNum < FIXNV_DEF_SIZE/ftlDevInfo.sctSize)
		{
			ReadSize = ftlPartitionInfo.sctTotalNum * ftlDevInfo.sctSize;
		}
		else
		{
			ReadSize = FIXNV_DEF_SIZE;
		}
			
		/* we must fill 0xff with the Read buffer */
        memset(pReadBuff, 0xff, sizeof(buff_read));
		/* we must fill 0xff with the Save buffer */
        memset(pSaveBuff, 0xff, sizeof(buff_write));
		
        /* Read from the pac file to savebuff */
        SaveSize = file_size;
        fret = f_read(file_handle, pSaveBuff, SaveSize, &uReadSize);
        if(FR_OK != fret)
        {
            err_no = ERR_READ_FILE;
            return err_no;
        }                                   

        /* Read from the original nand flash to readbuff */            
		rRet = SCI_FTL_Read(ftlHandle, 0, ReadSize/ftlDevInfo.sctSize, pReadBuff);
		if((SCI_FTL_ERROR_NONE != rRet)&&(SCI_FTL_ERROR_RISK!= rRet))
		{
			SCI_FTL_Close(ftlHandle);
			return ERR_FLASH_READ;
		}
         
        rRet =tf_savebackupnv(pReadBuff, ReadSize, pSaveBuff, SaveSize);
		if(TRUE == rRet)
		{
			/* add fixnv power-off protect function here!!! */     
		}
		else
		{
			//return ERR_FLASH_PROGRAM;
		}
	             
	  	if(SCI_FTL_ERROR_NONE != SCI_FTL_Erase(ftlHandle, 0, ReadSize/ftlDevInfo.sctSize))
	  	{
			  SCI_FTL_Close(ftlHandle);
			  return ERR_FLASH_ERASE;
		}

		if(SCI_FTL_ERROR_NONE != SCI_FTL_Write(ftlHandle, 0, ReadSize/ftlDevInfo.sctSize, pSaveBuff))
		{
			return ERR_FLASH_PROGRAM;
		}
                    
        return ERR_SUCCESS;
	}

	if(0 != ftlPartitionInfo.eraseUnit)
	{
	  	if(SCI_FTL_ERROR_NONE != SCI_FTL_Erase(ftlHandle, 0, ftlPartitionInfo.sctTotalNum))
	  	{
			  SCI_FTL_Close(ftlHandle);
			  return ERR_FLASH_ERASE;
		}
	}

	uTotalReadSize = 0;
	write_sector = 0;
	
	if(file_type == KernelImg)
	{
        buf_ptr = (BYTE*)(&buff_read);
		/* we must fill 0xff with the Read buffer */
        memset(buf_ptr, 0xff, sizeof(buff_read));

        new_address =(uint8 *) MakekernelHeader(file_size,&new_kernel_length,0);
        memcpy(buf_ptr,new_address,new_kernel_length);

        buff_size = g_BlkSize - new_kernel_length;
        buf_ptr += new_kernel_length;

        fret = f_read(file_handle, buf_ptr, buff_size, &uReadSize);
        uTotalReadSize += uReadSize;
        if(FR_OK != fret)
        {
            err_no = ERR_READ_FILE;
            return err_no;
        }
        
		if(SCI_FTL_ERROR_NONE != SCI_FTL_Write(ftlHandle, write_sector, g_SctsPerBlk, (uint8*)&buff_read))
		{
			return ERR_FLASH_PROGRAM;
		}		

        write_sector += g_SctsPerBlk;
	}

	buf_ptr = (BYTE*)(&buff_read);
	
    do
    {            
        uRemainReadSize = file_size - uTotalReadSize;

        /* Everytime show the progress is too slow, so we can show progress
        every 5 times */
        if((show_progress_cnts % 10) == 0)
        {
            current_percent = ((uTotalReadSize * 100) / (file_size));
            TF_ShowProgress(current_percent, 100);
        }
        show_progress_cnts ++;
        
        if(uRemainReadSize > g_BlkSize)
        {
            buff_size = g_BlkSize;
        }
        else
        {
            buff_size = uRemainReadSize;
        }

		/* we must fill 0xff with the Read buffer */
    	memset(buf_ptr, 0xff, sizeof(buff_read));
				
        fret = f_read(file_handle, buf_ptr, buff_size, &uReadSize);
        if(FR_OK != fret)
        {
            err_no = ERR_READ_FILE;
            return err_no;
        }
        /* 读出block size数据，然后写NAND */
		if(SCI_FTL_ERROR_NONE != SCI_FTL_Write(ftlHandle, write_sector, g_SctsPerBlk, buf_ptr))
		{
			return ERR_FLASH_PROGRAM;
		}	
		
        write_sector += g_SctsPerBlk;

        uTotalReadSize += uReadSize;

        if(uTotalReadSize == file_size)
        {
            break;
        }
    }while(TRUE); /* Every time we process one file and write to nandflash */

	SCI_FTL_Close(ftlHandle);

	TF_ShowProgress(100, 100);
		
	return ERR_SUCCESS;

}


/*****************************************************************************/
//  Discription: TF load main routine
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL int TF_Load(const TCHAR *dir)
{
	FIL fp;             /* open file file object */  
	FRESULT fret;
	FILE_T * paFile = NULL, * pFT = NULL, * paFile_bak = NULL;
	FILE_ID_TYPE nfiletype;
	TFLOAD_ERR_TYPE   errno = ERR_SUCCESS;
	BIN_PACKET_HEADER_T bph;
	UINT uReadSize = 0, nPacketSize;
	uint32  SaveSize, i,j;
	uint8* pSaveBuff = (BYTE*)(&buff_write);
	TCHAR pacfilename[_MAX_LFN];

	for(i=0; i<2; i++)
	{
		f_mount(i,&Fatfs);    
		errno = TF_Findfile(dir, _T("tfload.pac"), pacfilename);
		if(ERR_SUCCESS == errno)
		{        
			break;
		}
	}
	if(2 == i)
	{        
		errno = ERR_FIND_FILE;
		goto LOAD_ERR;
	}
	
	fret = f_open(&fp, pacfilename, FA_READ);         
	if(FR_OK != fret)
	{
		errno = ERR_OPEN_FILE;
		goto LOAD_ERR;
	}

	nPacketSize = fp.fsize;        
	//check packet size, it must large than the header struct
	if(nPacketSize < sizeof(BIN_PACKET_HEADER_T))
	{
		errno = ERR_PAC_FILE_SIZE;
		TF_UartTrace("TF_Load: nPacketSize=0x%x", nPacketSize);
		goto LOAD_ERR;
	}

	fret = f_read(&fp, &bph, sizeof(BIN_PACKET_HEADER_T),&uReadSize);
	if(FR_OK != fret)
	{
		errno = ERR_READ_FILE;
		TF_UartTrace("TF_Load: err packet header size =0x%x", uReadSize);
		goto LOAD_ERR;
	}

	bph.dwIsNvBackup = TRUE; //The Backup NV Function should be disable in Production Line.

	// check packet version we must swap the little endian to bigendian
	for(i = 0; i < 24; i++ )
	{
		bph.szVersion[i] = TF_WORD_SWAP(bph.szVersion[i]);
	}

	if( uReadSize !=sizeof(BIN_PACKET_HEADER_T) 
		|| TF_Load_wstrcmp(bph.szVersion, m_szPacVersion)!=0 )
	{
		errno = ERR_PAC_VERSION;
		TF_UartTrace("TF_Load: err .pac ver = %s", bph.szVersion);
		goto LOAD_ERR;
	}

	// check packet product name we must swap the little endian to bigendian
	for(i = 0; i < 256; i++ )
	{
		bph.szPrdName[i] = TF_WORD_SWAP(bph.szPrdName[i]);
	}

	if(TF_Load_wstrcmp(bph.szPrdName, TFLOAD_PRODUCT_NAME)!=0 )
	{
		errno = ERR_PAC_PRDNAME;
		TF_UartTrace("TF_Load: err .pac product name = %s", bph.szPrdName);
		goto LOAD_ERR;
	}

	// check packet size recorded by itself
	bph.dwSize = TF_DWORD_SWAP(bph.dwSize);
	if(bph.dwSize != (DWORD)nPacketSize)
	{
		errno = ERR_PAC_FILE_SIZE;
		TF_UartTrace("TF_Load: err packet file size = %s", bph.dwSize );
		goto LOAD_ERR;
	}

	// check the number of files packeted by the packet
	bph.nFileCount = TF_DWORD_SWAP(bph.nFileCount);
	if(bph.nFileCount == 0)
	{
		errno = ERR_PAC_FILE_NUM;
		TF_UartTrace("TF_Load: err packet file count = %s", bph.nFileCount );
		goto LOAD_ERR;
	}

	uReadSize = 0;
	paFile = (FILE_T*)&buf_pal_data;
	memset(paFile,0,sizeof(FILE_T)*bph.nFileCount);

	fret = f_read(&fp, paFile, bph.nFileCount * sizeof(FILE_T), &uReadSize);
	if(FR_OK != fret)
	{
		errno = ERR_READ_FILE;
		TF_UartTrace("TF_Load: err read  file size 1= %s", uReadSize);
		goto LOAD_ERR;
	}
	if(uReadSize != bph.nFileCount * sizeof(FILE_T))
	{        
		errno = ERR_PAC_FILE_SIZE;
		TF_UartTrace("TF_Load: err read  file size 2= %s", uReadSize);
		goto LOAD_ERR;
	}

	/* The last one file is xxx.xml, and we don't parse the xml file */
	/* first will parse ps and get flash partition info*/
	bph.nFileCount -= 1;

	for(i = 0; i < bph.nFileCount; i++)
	{
		pFT = paFile + i;        
		pFT->nFileSize = TF_DWORD_SWAP(pFT->nFileSize);

		// check file size if validate
		for(j = 0; j < 256; j++)
		{
			pFT->szFileID[j] = TF_WORD_SWAP(pFT->szFileID[j]);
		}

		for(nfiletype = 0; nfiletype < MAX_FILE_ID_TYPE; nfiletype++)
		{
			if(0 == TF_Load_wstrcmp(pFT->szFileID,sz_fileID[nfiletype]))
			{   
				char szstep[100] = {0};
				sprintf(szstep, "%s", sz_filetype[nfiletype]);
				TF_DisplayText(szstep,strlen(szstep));
				break;        
			}
		}
		if((pFT->nFileSize < 0) || (pFT->nFileSize>=nPacketSize))
		{
			continue;
		}
		
		if(partition_info[nfiletype].file_type == TF_PARTITION_TYPE_IGNORE)
		{
			 f_lseek(&fp, (((&fp)->fptr) + (pFT->nFileSize)));
			 continue;
		}
		else if(partition_info[nfiletype].file_type == TF_PARTITION_TYPE_CMD)
		{
			  errno = TF_Do_Erase(nfiletype);
			  if(errno != ERR_SUCCESS)
			  {
			  	TF_UartTrace("TF_Load: err do flash  erase: file type=0x%x", nfiletype);
				goto LOAD_ERR;
			  }
			  f_lseek(&fp, (((&fp)->fptr) + (pFT->nFileSize)));
		}
		else if(partition_info[nfiletype].file_type == TF_PARTITION_TYPE_FILE)
		{
			if(pFT->nFileSize != 0x0)
			{
				errno = TF_Do_Program(&fp, pFT->nFileSize, pSaveBuff,  nfiletype);
				if(errno != ERR_SUCCESS)
				{
				 	TF_UartTrace("TF_Load: err do program nfiletype=0x%x, pFT->nFileSize=0x%x ", nfiletype, pFT->nFileSize);
					goto LOAD_ERR;
				 }
			}
		}
		else
		{
			f_lseek(&fp, (((&fp)->fptr) + (pFT->nFileSize)));
		}
	}

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
LOCAL void TF_ParamInit(void)
{
//	#ifdef TF_LOAD_PROTECT
	uint32 manufacturer_id,device_id,extend_id;
	LCD_INFO_T lcd_info;
//	#endif

	tf_update_info_ptr->tf_update_flash.alloc_buf_addr = (uint32)buf_pal_data;
	tf_update_info_ptr->tf_update_lcd.block_buf_ptr  = TF_GetMainLCDBuffer();
	
//	#ifdef TF_LOAD_PROTECT
	LCD_GetInfo(MAIN_LCD_ID, &lcd_info);
	tf_update_info_ptr->tf_update_flash.manu_id = manufacturer_id;
	tf_update_info_ptr->tf_update_flash.dev_id = device_id;
	tf_update_info_ptr->tf_update_flash.ext_id  = extend_id;
	tf_update_info_ptr->tf_update_lcd.block_buf_ptr = lcd_info.lcdbuff_ptr;
	tf_update_info_ptr->tf_update_lcd.lcd_info.lcd_height = lcd_info.lcd_height;
	tf_update_info_ptr->tf_update_lcd.lcd_info.lcd_width  = lcd_info.lcd_width;
//	#endif
	TF_LcmInit(tf_update_info_ptr);
}

/*****************************************************************************/
//  Discription: TF get update info
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC  TF_UPDATE_INFO_PTR TF_GetUpdateInfo(void)
{
	return (TF_UPDATE_INFO_PTR)TFLOAD_SHARE_RAM_ADDRESS;
}

/*****************************************************************************/
//  Discription: TF download process err string generate
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC char* TF_Errstring_Gen(TFLOAD_ERR_TYPE errno)
{
	return (char*)(sz_error_type[errno]);
}

/*****************************************************************************/
// 	Description : update agent main function
//	Global resource dependence : 
//  Author: tao.feng
//	Note:
/*****************************************************************************/
#if 0
int  main(void)
{
	uint8 errno = ERR_SUCCESS;
	char szstep[100] = {0};

	TF_HardWareInit();
	TF_ParamInit();
	TF_UartTrace("TF update routin enter!");

	sprintf(szstep, "Image Updating Enter!\r\n%s", s_version_info[PROJECT_VERSION]);
	TF_DisplayText(szstep, strlen(szstep));
	TF_SetLcdBackLight(TRUE, 0x1f);
	
	errno= TF_Load(TFLOAD_DEFAULT_DIR);
	TF_Show_Error(errno);

    if(ERR_SUCCESS == errno)
    {
      	tf_update_info_ptr->tf_update_magic = 0x0;    
        TF_SetLcdBackLight(FALSE, 0);
        TF_CHIP_PHY_ResetHWFlag(RESET_MODE);
        TF_CHIP_ResetMCU();
    }
    
	while(1);
}

#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 
