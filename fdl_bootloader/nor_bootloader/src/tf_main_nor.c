/******************************************************************************
 ** File Name:      TF_load.c                                               *
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
#include "diskio.h"
#include "ff.h"
#include "stdarg.h"
#include "tf_load.h"
#include "tf_main.h"
#include "tf_cfg.h"
#include "tf_cfg.c"
#include "tf_str.h"
#include "tf_parse_nor.h"

#include "card_sdio.h"
#include "version.h"

#include "mmu_drvapi.h"

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
#define TF_STACK_LIMIT         0x10000
#define SIO_TX_EMPTY(s)        ((s) & 0xFF00)

#define TF_M18_REGION_SIZE     (1024) //unit:byte
#define TF_M18_MODE_DIFF_ADDR  (0x10)
#define TF_BUF_SIZE            (512*1024)
#define TF_UMEM_ERASE_FLAG     (0x66)
#define TF_MAX_BUF_SIZE        (0x40000)
#define REG32(x)               (*((volatile uint32 *)(x)))

#define TF_UartTrace rprintf

/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL BYTE buff_write[TF_BUF_SIZE];
LOCAL FATFS Fatfs;		/* File system object for each logical drive */
LOCAL BYTE buff_pal_file[TF_BUF_SIZE];
LOCAL uint8 buf_pal_data[TF_MAX_BUF_SIZE];
LOCAL const TCHAR m_szPacVersion[24] = _T("BP_R1.0.0");
LOCAL const uint16 g_tfboot_wstr[]= { 'T', 'F', 'B', 'O', 'O', 'T', 0 };

/**---------------------------------------------------------------------------*
 **                         Global  Variables                                  *
 **---------------------------------------------------------------------------*/
static TCHAR lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];

/**---------------------------------------------------------------------------*
 **                         Extern  Func                                  *
 **---------------------------------------------------------------------------*/                                     

/**---------------------------------------------------------------------------*
 **                          Functions    Definition                            *
 **---------------------------------------------------------------------------*/


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
//  Discription: TF load main routine
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC int TF_Load(const TCHAR *dir)
{
	FIL fp;             /* open file file object */  
	FRESULT fret;
	FILE_T *paFile = NULL, *pFT = NULL, *paFile_bak = NULL;
	FILE_ID_TYPE nfiletype;
	TF_PARTITION_PTR flash_part_info_ptr =PNULL;
	TFLOAD_ERR_TYPE   errno = ERR_SUCCESS;
	BIN_PACKET_HEADER_T bph;
	UINT uReadSize = 0, nPacketSize;
	uint32 SaveSize, i,j;
	uint8 *pSaveBuff = (BYTE*)(&buff_write);
	BOOLEAN fill_ret = FALSE;
	TCHAR pacfilename[_MAX_LFN];
	DWORD tmp_pos;

    TF_UartTrace("TF_Load: Start find tfboot ...\n");

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
		TF_UartTrace("TF_Load: err .pac product name\n");
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

	uReadSize = 0;
	paFile = (FILE_T*)&buf_pal_data;
	paFile_bak = (FILE_T*)&buff_pal_file;
	memset(paFile, 0, sizeof(FILE_T)*bph.nFileCount);
	memset(paFile_bak, 0, sizeof(FILE_T)*bph.nFileCount);


	fret = f_read(&fp, paFile, bph.nFileCount * sizeof(FILE_T), &uReadSize);
	if (FR_OK != fret)
	{
		errno = ERR_READ_FILE;
		TF_UartTrace("TF_Load: err read file size 1= %u\n", uReadSize);
		goto LOAD_ERR;
	}
	if (uReadSize != bph.nFileCount * sizeof(FILE_T))
	{        
		errno = ERR_PAC_FILE_SIZE;
		TF_UartTrace("TF_Load: err read file size 2= %u\n", uReadSize);
		goto LOAD_ERR;
	}
	TF_Memcpy((void*)paFile_bak, (void*)paFile, uReadSize);

	tmp_pos = ((&fp)->fptr);
	/* The last one file is xxx.xml, and we don't parse the xml file */
	/* first will parse ps and get flash partition info*/
	bph.nFileCount -= 1;
	for (i = 0; i < bph.nFileCount; i++)
	{
		pFT = paFile + i;

		TF_UartTrace("TF_Load: pFT->nFileSize %d\n", pFT->nFileSize);

		// check file size if validate
		if (pFT->nFileSize < 0 || pFT->nFileSize >= nPacketSize)
		{
			errno = ERR_PAC_FILE_SIZE;
			TF_UartTrace("TF_Load: err read file size 3= %u\n", pFT->nFileSize);
			goto LOAD_ERR;
		}

		if (pFT->nFileSize == 0)
		{
			continue;
		}        

		for (j = 0; j < 256; j++)
		{
			TF_UartTrace("%c", pFT->szFileID[j]);
		}
		TF_UartTrace("\n");

		if (0 == TF_Load_wstrcmp(pFT->szFileID, g_tfboot_wstr))
		{   
			memset(pSaveBuff, 0x00, TF_BUF_SIZE);
			
			SaveSize = pFT->nFileSize;
			if (SaveSize >= TF_BUF_SIZE)
			{
				errno = ERR_PAC_FILE_SIZE;
				goto LOAD_ERR;
			}
			
			fret = f_read(&fp, pSaveBuff, SaveSize, &uReadSize);
			if (FR_OK != fret)
			{
				TF_UartTrace("TF_Load: read tfboot.bin err\n");
				errno = ERR_READ_FILE;
				goto LOAD_ERR;
			}
			else
			{
				TF_UartTrace("TF_Load: uReadSize %u\n", uReadSize);
				TF_UartTrace("TF_Load: JumpToTarget tfboot.bin\n");
				JumpToTarget(0x80000000);
			}
			break;        
		}
		else
		{
			f_lseek(&fp, (((&fp)->fptr) + (pFT->nFileSize)));
			continue;
		}
	}
        
	return ERR_SUCCESS;

LOAD_ERR:    
	return errno;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 
