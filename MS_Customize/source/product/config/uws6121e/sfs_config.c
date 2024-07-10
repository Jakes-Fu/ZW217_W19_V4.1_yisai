/******************************************************************************
 ** File Name:      sfs_config.c                                              *
 ** Author:         Jason.Wu                                                  *
 ** DATE:           05/05/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the product configurable content of SFS * 
 **                 module                                                    *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/05/2010     	                Create.                         		  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "sci_types.h"
#include "sfs.h"
#include "os_api.h"
#include "priority_system.h"
/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
 


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
 /*
	1 g_fileNum:	�����ļ�ϵͳ��ͬʱ���ļ��ĸ���,Ϊ�˷�ֹӦ��������ڳ������̴�����������ƴ򿪶����ǹرգ��˷�ϵͳ
		��Դ����˸�����ֵ�����ʵ����ɣ�����Խ��Խ�á���ֵ�ĵ����������	FAT_HANLDE_MAX
	g_bufMgr_opt:	������ѡ���ļ�ϵͳ��bufferʹ�÷�ʽ
			1 ��ʾÿ�����򿪵��ļ����Լ��ĵ�����ռ��buffer���������ж�д�������,�û��������ļ��Ĺرն����ͷ�
			2 ��ʾ��ǰ�������б��򿪵��ļ���ʹ�ù��ù���Ļ��棬�û������Ŵ��̵�ж�زű��ͷ�
	g_fileBufCnt:	g_bufMgr_optΪ��ʽ1ʱ����ʾ�ļ�����ĸ���		��Сֵ 3
			g_bufMgr_optΪ��ʽ2ʱ����ʾ������Ļ���ĸ���
	g_fileBufSize:	g_bufMgr_optΪ��ʽ1ʱ����ʾÿ���ļ�����Ĵ�С
			g_bufMgr_optΪ��ʽ2ʱ����ʾÿ��������Ļ���Ĵ�С
	g_foldBufCnt:	g_bufMgr_optΪ��ʽ1ʱ����ʾĿ¼�ļ�����ĸ���		������0����С��3���κ���ֵ
			g_bufMgr_optΪ��ʽ2ʱ��û�κ�����
	g_foldBufSize:	g_bufMgr_optΪ��ʽ1ʱ����ʾÿ��Ŀ¼�ļ�����Ĵ�С
			g_bufMgr_optΪ��ʽ2ʱ����g_fileBufSize����һ�¼���
	g_tmpFoldBufCnt:g_bufMgr_optΪ��ʽ1ʱ����ʾ��ʱĿ¼�ļ�����ĸ���	������0����С��3���κ���ֵ
			g_bufMgr_optΪ��ʽ2ʱ��û�κ�����
	g_tmpFoldBufSize:g_bufMgr_optΪ��ʽ1ʱ����ʾÿ����ʱĿ¼�ļ�����Ĵ�С
			g_bufMgr_optΪ��ʽ2ʱ����g_fileBufSize����һ�¼���
	
*/

#if defined(WRE_SUPPORT)
	#define WRE_DISK_NUM	1
#else
	#define WRE_DISK_NUM	0
#endif

#if defined(WRE_VIRTUALDISK_SUPPORT)
	#define WREVIR_DISK_NUM	1
#else
	#define WREVIR_DISK_NUM	0
#endif

#if defined(UMEM_SUPPORT)
	#define UMEN_DISK_NUM	1
#else
	#define UMEN_DISK_NUM	0
#endif

#if defined(DUAL_TCARD_SUPPORT)
	#define DUALTCARD_DISK_NUM	1
#else
	#define DUALTCARD_DISK_NUM	0
#endif

#if defined(MSDC_CARD_SUPPORT)
    #define SD_CARD_NUM 1
#else
    #define SD_CARD_NUM	0
#endif


#define		SFS_DISK_NUM	(WRE_DISK_NUM+WREVIR_DISK_NUM+UMEN_DISK_NUM+DUALTCARD_DISK_NUM+SD_CARD_NUM)

LOCAL SFS_DEV_CONFIG_T	_diskCfg[SFS_DISK_NUM] =
{
#if defined(WRE_SUPPORT)
	{
		"C", SCI_SYSTEM_PRI(31),
		2,				// 1 ,1 buf of 1 file	2 1 buf of all file 
		10,				// the number of buf in file
		1024,			// the size of buf in file
		0xFFFFFFFF,		// the number of buf in folder.		ignored when 1 == g_bufMgr_opt
		0xFFFFFFFF,		// the size of buf in folder.		ignored when 1 == g_bufMgr_opt
		512*4,			// the number of buf in temp folder.	ignored when 1 == g_bufMgr_opt
		512*4			// the size of buf in temp folder.	ignored when 1 == g_bufMgr_opt
	},
#endif

#if defined(WRE_VIRTUALDISK_SUPPORT)
	{
		"H", SCI_SYSTEM_PRI(31),
		2,				// 1 ,1 buf of 1 file	2 1 buf of all file 
		10,				// the number of buf in file
		1024,			// the size of buf in file
		0xFFFFFFFF,		// the number of buf in folder.		ignored when 1 == g_bufMgr_opt
		0xFFFFFFFF,		// the size of buf in folder.		ignored when 1 == g_bufMgr_opt
		512*4,			// the number of buf in temp folder.	ignored when 1 == g_bufMgr_opt
		512*4			// the size of buf in temp folder.	ignored when 1 == g_bufMgr_opt
	},
#endif

#if defined(UMEM_SUPPORT)
#if defined(SRAM_SIZE_16MBIT) || defined(SRAM_SIZE_32MBIT)
	{
		"D", SCI_SYSTEM_PRI(31),
		2,				// 1 ,1 buf of 1 file	2 1 buf of all file 
		6,				// the number of buf in file
		1024,			// the size of buf in file
		0xFFFFFFFF,		// the number of buf in folder.		ignored when 1 == g_bufMgr_opt
		0xFFFFFFFF,		// the size of buf in folder.		ignored when 1 == g_bufMgr_opt
		512*2,			// the number of buf in temp folder.	ignored when 1 == g_bufMgr_opt
		512*2			// the size of buf in temp folder.	ignored when 1 == g_bufMgr_opt
	},
#else
	{
		"D", SCI_SYSTEM_PRI(31),
		2,				// 1 ,1 buf of 1 file	2 1 buf of all file 
		10,				// the number of buf in file
		1024,			// the size of buf in file
		0xFFFFFFFF,		// the number of buf in folder.		ignored when 1 == g_bufMgr_opt
		0xFFFFFFFF,		// the size of buf in folder.		ignored when 1 == g_bufMgr_opt
		512*4,			// the number of buf in temp folder.	ignored when 1 == g_bufMgr_opt
		512*4			// the size of buf in temp folder.	ignored when 1 == g_bufMgr_opt
	},
#endif	
#endif

#if defined(DUAL_TCARD_SUPPORT)
	{
		"F", SCI_SYSTEM_PRI(31),
		2,				// 1 ,1 buf of 1 file	2 1 buf of all file 
		10,				// the number of buf in file
		1024,			// the size of buf in file
		0xFFFFFFFF,		// the number of buf in folder.		ignored when 1 == g_bufMgr_opt
		0xFFFFFFFF,		// the size of buf in folder.		ignored when 1 == g_bufMgr_opt
		512*4,			// the number of buf in temp folder.	ignored when 1 == g_bufMgr_opt
		512*4			// the size of buf in temp folder.	ignored when 1 == g_bufMgr_opt
	},
#endif

#ifdef MSDC_CARD_SUPPORT
#ifdef SRAM_SIZE_16MBIT
	{
		"E", SCI_SYSTEM_PRI(31),
		2,				// 1 ,1 buf of 1 file	2 1 buf of all file 
		6,				// the number of buf in file
		1024,			// the size of buf in file
		0xFFFFFFFF,		// the number of buf in folder.		ignored when 1 == g_bufMgr_opt
		0xFFFFFFFF,		// the size of buf in folder.		ignored when 1 == g_bufMgr_opt
		512*2,			// the number of buf in temp folder.	ignored when 1 == g_bufMgr_opt
		512*2			// the size of buf in temp folder.	ignored when 1 == g_bufMgr_opt
	}
#elif defined(SRAM_SIZE_32MBIT)
	{
		"E", SCI_SYSTEM_PRI(31),
		2,				// 1 ,1 buf of 1 file	2 1 buf of all file 
		8,				// the number of buf in file
		4096,			// the size of buf in file
		0xFFFFFFFF,		// the number of buf in folder.		ignored when 1 == g_bufMgr_opt
		0xFFFFFFFF,		// the size of buf in folder.		ignored when 1 == g_bufMgr_opt
		512*16,			// the number of buf in temp folder.	ignored when 1 == g_bufMgr_opt
		512*8			// the size of buf in temp folder.	ignored when 1 == g_bufMgr_opt
	}
#else
	{
		"E", SCI_SYSTEM_PRI(31),
		2,				// 1 ,1 buf of 1 file	2 1 buf of all file 
		8,				// the number of buf in file
		8192,			// the size of buf in file
		0xFFFFFFFF,		// the number of buf in folder.		ignored when 1 == g_bufMgr_opt
		0xFFFFFFFF,		// the size of buf in folder.		ignored when 1 == g_bufMgr_opt
		512*32,			// the number of buf in temp folder.	ignored when 1 == g_bufMgr_opt
		512*16			// the size of buf in temp folder.	ignored when 1 == g_bufMgr_opt
	}
#endif
#endif

};

LOCAL const SFS_XFIND_CONFIG_T _appXFindCfg =
{
	//SCI_SYSTEM_PRI(29),
	SCI_SYSTEM_PRI(30),
	1024,
	10
};


LOCAL SFS_XMOV_CONFIG_T _appXMovCfg =
{
	SCI_SYSTEM_PRI(28),		//priority
	100000,								//version
};
SFS_CONFIG_T g_sfsConfig=
{
	40,
	(SFS_XFIND_CONFIG_T*)&_appXFindCfg,
	&_appXMovCfg,
	SFS_DISK_NUM,
	_diskCfg
};

SFS_CONFIG_T* SFS_GetConfig(void)
{
	return &g_sfsConfig;
}
/**---------------------------------------------------------------------------*
 **                     Public Function Prototypes                            *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }

#endif



