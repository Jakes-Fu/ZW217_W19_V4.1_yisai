/****************************************************************************
** File Name:      mmiebook_file.h                              
** Author:         songbin.zeng                                                        
** Date:           2006/06/9 
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.       
** Description:    This file is used to read/write text file
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 06/6/2006         songbin.zeng          Create
** 
****************************************************************************/

#ifndef _MMIEBOOK_FILE_H_
#define _MMIEBOOK_FILE_H_

#ifdef EBOOK_SUPPORT
#include "mmifmm_export.h"
#include "mmifilearray_export.h"
#include "gui_ucs2b_converter.h"
#include "mmiebook_nv.h"
//#include "mmiebook.h"
#include "mmiebook_internal.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//#define MMIEBOOK_DIR_UCS2                            (g_mmiebook_dir_wstr)//"\x00\x45\x00\x62\x00\x6F\x00\x6F\x00\x6B\x00\x00"
//#define MMIEBOOK_DIR_UCS2_LEN                        5
//#define MMIEBOOK_DEFAULT_NAME_UCS2_LEN              10
#define MMIEBOOK_DEFAULT_AND_LRC_NAME	"*.txt *.lrc"//电子书支持的所有格式
#define MMIEBOOK_TXT_NAME_MAX_LEN                   MMIFILE_FILE_NAME_MAX_LEN//(MMIFILE_FILE_NAME_MAX_LEN*2)      //文本文件的文件名长度，字节
#define MMIEBOOK_FULLNAME_MAX_LEN 			        FILEARRAY_FILENAME_MAX_LEN//(FILEARRAY_FILENAME_MAX_LEN*2+2) 	//文本文件的全路径文件名长度，字节
#define MMIEBOOK_BOOKMARK_MAX_NUM                   50  //一本书最多几个书签

#define MMIEBOOK_DEV_TAB_MAX_LEN                    10


//最近阅读的电子书列表文件头结构发生变化,必须更新MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME!!!
#ifdef MMIEBOOK_BOOKSHELF_SUPPORT
#define MMIEBOOK_LATEST_LIST_MAX_NUM                    50//最近阅读的n本电子书
#define MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME           "EBOOK LAST LIST FILE 09.10.0a.03"
#else
#define MMIEBOOK_LATEST_LIST_MAX_NUM                    10//最近阅读的n本电子书
#define MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME           "EBOOK LAST LIST FILE 09.10.0b.03"
#endif
#define MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME_LEN       32
#define MMIEBOOK_LATEST_LIST_FILE_SIZE                  (MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME_LEN +\
                                                        sizeof(MMIEBOOK_LATEST_LIST_T))                                                       
//最近阅读的电子书列表文件头结构发生变化,必须更新MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME!!!

#define UNUSED_PARAM(param)                         param = param
/*---------------------------------------------------------------------------*/
/*                          STRUCT DEFINE                                    */
/*---------------------------------------------------------------------------*/
//文件设备
/*typedef enum
{
    MMIEBOOK_FILE_DEV_MIN,
    MMIEBOOK_FILE_DEV_UDISK,
    MMIEBOOK_FILE_DEV_SD,
    MMIEBOOK_FILE_DEV_SD1,//防止ASSERT，add by charlie 2011.02.17
    MMIEBOOK_FILE_DEV_MAX
} MMIEBOOK_FILE_DEV_E;*/

//文件类型
typedef enum
{
    MMIEBOOK_ROOT_FILE_TYPE_TXT,//@zhaohui,cr108528
    MMIEBOOK_ROOT_FILE_TYPE_LRC,//@zhaohui,cr108528
    MMIEBOOK_FILE_TYPE_TXT,
    MMIEBOOK_FILE_TYPE_LRC,
    MMIEBOOK_FILE_TYPE_MAX
} MMIEBOOK_FILE_TYPE_E;

//对最近阅读的电子书列表文件读写的操作类型
typedef enum
{
    MMIEBOOK_LATEST_LIST_FILE_MIN = 0,
    MMIEBOOK_LATEST_LIST_FILE_VER_ID,       //文件第一部分，最近阅读的电子书列表文件版本信息
    MMIEBOOK_LATEST_LIST_FILE_INFO,         //文件第二部分，最近阅读的电子书列表信息
    MMIEBOOK_LATEST_LIST_FILE_MAX
} MMIEBOOK_LATEST_LIST_FILE_OPT_E;

//@maggie add begin for ebook
typedef enum
{
    EBOOK_NODE_TXT_OPT_NULL,
    EBOOK_NODE_TXT_OPT_ADD_BOOKMARK,    
    EBOOK_NODE_TXT_OPT_SEE_BOOKMARK,
    EBOOK_NODE_TXT_OPT_DISPLAY_SETTING,
    EBOOK_NODE_TXT_OPT_DAY_NIGHT_MODE,
    EBOOK_NODE_TXT_OPT_JUMP_TO_BEGIN,       //跳转至开始
    EBOOK_NODE_TXT_OPT_JUMP_TO_END,         //跳转至结尾
    EBOOK_NODE_TXT_OPT_JUMP_PERCENT,        //跳转至百分比
    EBOOK_NODE_TXT_OPT_BACKLIGHT_TURN_OFF,  
    EBOOK_NODE_TXT_OPT_BACKLIGHT_TURN_ON,
    EBOOK_NODE_TXT_OPT_FULL_SCREEN_OFF,  
    EBOOK_NODE_TXT_OPT_FULL_SCREEN_ON,
    EBOOK_NODE_TXT_OPT_BROWSE_PAGE_MODULE,
    EBOOK_NODE_TXT_OPT_CODING_TYPE,    
    EBOOK_NODE_TXT_OPT_AUTO_VIEW,
    EBOOK_NODE_TXT_OPT_EXIT,
    EBOOK_NODE_TXT_OPT_READ_START,
    EBOOK_NODE_TXT_OPT_READ_STOP,
    EBOOK_NODE_TXT_OPT_VCARD_IMPORT
    
}EBOOK_NODE_TXT_OPT_E;
//@maggie add end for ebook

//对电子书书签文件读写的操作类型
typedef enum
{
    MMIEBOOK_BOOKMARK_FILE_MIN = 0,
//    MMIEBOOK_BOOKMARK_FILE_VER_ID,       //文件第一部分，最近阅读的电子书列表文件版本信息
    MMIEBOOK_BOOKMARK_FILE_CUR_READ_OFFSET,     //文件第一部分,当前阅读的偏移位置
    MMIEBOOK_BOOKMARK_FILE_INFO,         //文件第二部分，最近阅读的电子书列表信息
    MMIEBOOK_BOOKMARK_FILE_MAX
} MMIEBOOK_BOOKMARK_FILE_OPT_E;


//电子书书签信息
typedef struct MMIEBOOK_BOOKMARK_S
{
    int32 offset_tab[MMIEBOOK_BOOKMARK_MAX_NUM];//书签信息数组
    uint32 total_num;                            //书签的数目
}MMIEBOOK_BOOKMARK_T;

//电子书简略信息
typedef struct MMIEBOOK_FILE_BRIEF_S
{
    //wchar               name_wstr[MMIEBOOK_TXT_NAME_MAX_LEN + 1];   //文件名字,ucs
    //uint16              name_len;           //名字长度，字节
    int32              file_size;          //文件大小
    //int32				open_size;	//文件可打开的大小
    MMIFILE_DEVICE_E    file_dev;           //文件所在设备
    //int32              detail_id;          //文件详情信息结构体的在详情文件中的存贮位置

     //add by rongjin, open ebook in my document when it isn't in ebook dir.
    wchar               full_name_wstr[MMIEBOOK_FULLNAME_MAX_LEN + 1];   //文件名字,ucs
    uint16             full_name_len;           						//名字长度，字节
    														//>0, read from fmm, had full name;
     														//=0, read from ebook or read from fmm, hadn't full name;
   //add end 	
    //uint16                      current;    //当前选中的文件@maggie add
	//MMIFILE_FILE_INFO_T         file[MMIEBOOK_TXT_MAX_NUM];//@maggie add 
} MMIEBOOK_FILE_BRIEF_T;

//所有设备上的电子数简略信息
typedef struct MMIEBOOK_ALL_DEV_FILE_BRIEF_S
{
    MMIFILE_DEVICE_E dev_tab[MMIEBOOK_DEV_TAB_MAX_LEN];
    uint16  dev_file_num_tab[MMIEBOOK_DEV_TAB_MAX_LEN];//对应全局变量g_ebook_dev_info.dev_tab的数量
    uint16  dev_num;
    uint16  total_num;                          //所有设备上电子书总数
    MMIEBOOK_FILE_BRIEF_T  * file_brief_ptr;    //所有文本文件的简略信息
} MMIEBOOK_ALL_DEV_FILE_BRIEF_T;

typedef struct MMIEBOOK_LATEST_EBOOK_FILE_INFO_S
{
    uint32                              total_item_num;             //最近阅读的文件个数
    MMIEBOOK_FILE_BRIEF_T               item_brief[MMIEBOOK_LATEST_LIST_MAX_NUM]; //最近阅读的10本电子书的信息数组
} MMIEBOOK_LATEST_LIST_T;//最近阅读的电子书列表

//open file error type
typedef enum
{
    //兼容以前的true, false设计
    MMIEBOOK_OPEN_FILE_NO_ERROR,            //正常
    MMIEBOOK_OPEN_FILE_ERROR,               //错误，不区分类型

    //扩展DRM关心的类型，
    MMIEBOOK_OPEN_FILE_NO_RIGHTS,           //版权无效
    MMIEBOOK_OPEN_FILE_NO_SYS_TIME,         //未同步网络时间
    
    MMIEBOOK_OPEN_FILE_ERROR_MAX
    
} MMIEBOOK_OPEN_FILE_ERROR_E;

extern PUBLIC MMIEBOOK_ALL_DEV_FILE_BRIEF_T     g_ebook_all_dev_file_brief;     //所有存储设备中的电子书简略信息


//extern const wchar g_mmiebook_dir_wstr[];
/*****************************************************************************/
//  Description : Get text file type
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_GetTextFileType(//type
                                        MMIFILE_HANDLE file_handle, //in
                                        GUI_CHAR_CODE_TYPE_E *file_type // out
                                        );

/*****************************************************************************/
//  Description : Get text file head len
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_GetTextFileHeadLen(//len
                                           MMIFILE_HANDLE file_handle, //in
                                           uint16 *length
                                           );

/*****************************************************************************/
//  Description : 释放文件所需资源
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_FreeEbookFileRes(void);

/*****************************************************************************/
//  Description : 在指定设备上产生电子书目录
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIEBOOK_CreateEbookDir(MMIFILE_DEVICE_E dev);
/*****************************************************************************/
//  Description : 在指定设备上产生电子书目录
//  Global resource dependence : 
//  Author: rongjin
//  Note:
//  Return:
/*****************************************************************************/
//add by rongjin, fixed bug 21540, 070723        
PUBLIC void MMIEBOOK_SetTxtBrief(void);

/*****************************************************************************/
//  Description : 打开电子书列表中选择的电子书
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC MMIEBOOK_OPEN_FILE_ERROR_E MMIEBOOK_OpenShowFile(
								  uint16  cur_selection//IN: 列表中的ID
								  );

/*****************************************************************************/
//  Description : 阅读的电子书是否打开了
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->打开了
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsShowFileOpen(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
//  Description : 关闭正在阅读的电子书
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_CloseShowFile(void);

/*****************************************************************************/
//  Description : 获取正在阅读的电子书句柄
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:电子书句柄
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMIEBOOK_GetShowFileHandle(void);

/*****************************************************************************/
//  Description : 搜索文件设备
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:设备总数
/*****************************************************************************/
PUBLIC uint16 MMIEBOOK_GetDevInfo(void);

/*****************************************************************************/
//  Description : 根据文件设备类型获取设备名
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_GetDevNameByType(
								   MMIFILE_DEVICE_E file_dev,//in
								   wchar* ucs2_dev_name_ptr,//out
								   uint16* ucs2_dev_name_len_ptr//out
								   );

/*****************************************************************************/
//  Description : 获取文件全路径
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_GetFileFullPathUCS2(
                                            MMIFILE_DEVICE_E file_dev,//in
                                            const wchar      *ucs2_dir_ptr,           //in
                                            uint16           ucs2_dir_len,            //in
                                            const wchar      *ucs2_file_name_ptr,     //in, can be NULL
                                            uint16           ucs2_file_name_len,      //in
                                            wchar            *ucs2_full_path_ptr,     //out
                                            uint16           *ucs2_full_path_len_ptr  //out
                                            );

/*****************************************************************************/
//  Description : Sets the current position of a file pointer
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC int MMIEBOOK_FFS_fseek(
						   MMIFILE_HANDLE  file_handle,    //文件
						   int32           offset,
						   uint32          origin      //0，1，2 分别表示从当前位置，文件结尾，文件开始
						   );

/*****************************************************************************/
//  Description : write file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_FFS_fwrite(
							   const void * buffer, 
							   uint32 size, 
							   uint32 count, 
							   MMIFILE_HANDLE file_handle
							   );

/*****************************************************************************/
//  Description :read file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIEBOOK_FFS_fread(
							  void * buffer, 
							  uint32 size, 
							  uint32 count, 
							  MMIFILE_HANDLE file_handle
							  );

/*****************************************************************************/
//  Description : close file
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_FFS_fclose(
							 MMIFILE_HANDLE file_handle
							 );

/*****************************************************************************/
//  Description : return s_ebook_file_brief
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC MMIEBOOK_FILE_BRIEF_T* MMIEBOOK_GetFileBrief(void);

/*****************************************************************************/
//  Description :初始化s_ebook_file_brief
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_InitFileBrief(
											uint8 file_dev,
											const wchar* name_ptr, 
											uint32 name_length, 
											uint32 file_size, 
											const wchar* full_name_ptr, 
											uint32 full_name_length
											);

/*****************************************************************************/
//  Description : return s_ebook_file_list_update
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIEBOOK_SetFilelistupdate(BOOLEAN param);
    
/*****************************************************************************/
//  Description : alloc s_ebook_file_brief
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIEBOOK_CreateFileBrief( void );

/*****************************************************************************/
//  Description : free s_ebook_file_brief
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIEBOOK_DestroyFileBrief( void );
                   
/*****************************************************************************/
//  Description : 打开电子书时,更新最近阅读的电子书列表文件
//  Global resource dependence : 
//  Author: zhaohui
//  Note:v
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_UpdateLatestList(void);

/*最近阅读的电子书列表文件操作相关接口,begin*/
/*****************************************************************************/
//  Description : 打开最近阅读的电子书列表文件
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_OpenLatestListFile(MMIFILE_DEVICE_E dev, const char * mode);

/*****************************************************************************/
//  Description : 创建最近阅读的电子书列表文件
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_CreateLatestListFile(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
//  Description : 最近阅读的电子书列表文件是否打开
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE:已打开 FALSE:未打开
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsLatestListFileOpen(void);

/*****************************************************************************/
//  Description : 得到指定设备上最近阅读的电子书列表文件的句柄
//  Global resource dependence : 
//  Author: zhaohui
//  Note:文件已经打开
//  Return:文件句柄
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMIEBOOK_GetLatestListFileHandle(void);

/*****************************************************************************/
//  Description : 关闭最近阅读的电子书列表文件
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_CloseLatestListFile(void);

/*****************************************************************************/
//  Description : 读取最近阅读的电子书列表
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_ReadLatestListFile(
                                       MMIFILE_HANDLE file_handle,
                                       MMIEBOOK_LATEST_LIST_FILE_OPT_E type,//IN: 读哪一部分
                                       void *param
//                                       MMIEBOOK_LATEST_LIST_T *latest_list_ptr
                                       );
                                       
/*****************************************************************************/
//  Description : 读取最近阅读的电子书列表
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_WriteLatestListFile(
                                        MMIFILE_HANDLE file_handle,
                                        MMIEBOOK_LATEST_LIST_FILE_OPT_E type,//IN: 读哪一部分
                                        void *param
                                        );
                                        
/*****************************************************************************/
//  Description : 指定设备是否有最近阅读的电子书列表文件是否存在且是可用的(即版本一致),
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->可用的
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsLatestListFileOK(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
//  Description : 指定设备是否有详情文件存在
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->存在
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsExistLatestListFile(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
//  Description : 最近阅读的电子书列表文件是否与当前软件版本一致
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->版本一致
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsLatestListFileVersionVaild(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
//  Description : 删除最近阅读的电子书列表文件
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_DeleteLatestListFile(
                    MMIFILE_DEVICE_E dev
                    );   
/*最近阅读的电子书列表文件操作相关接口,end*/

/*****************************************************************************/
//  Description : 打开详情文件
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_OpenBookmarkFile(
                                         wchar* full_path,  //in
                                         const char* mode   //in
                                         );
                                                                                  
/*****************************************************************************/
//  Description : 得到指定设备上最近阅读的电子书列表文件的句柄
//  Global resource dependence : 
//  Author: zhaohui
//  Note:文件已经打开
//  Return:文件句柄
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMIEBOOK_GetBookmarkFileHandle(void);

/*****************************************************************************/
//  Description : 关闭最近阅读的电子书列表文件
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_CloseBookmarkFile(void);

/*****************************************************************************/
//  Description : 读取最近阅读的电子书列表
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_ReadBookmarkFile(
					                        MMIFILE_HANDLE file_handle,         //in
					                        MMIEBOOK_BOOKMARK_FILE_OPT_E type,  //IN,读哪一部分
                                            void *param                         //in,写入的内容,
                                            );
                                            
/*****************************************************************************/
//  Description : 读取最近阅读的电子书列表
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_WriteBookmarkFile(
											MMIFILE_HANDLE file_handle,         //in
											MMIEBOOK_BOOKMARK_FILE_OPT_E type,  //IN,读哪一部分
                                            void *param                         //in,写入的内容,
											);
											
/*****************************************************************************/
//  Description : MMIEBOOK_GetBookmarkFileName
//  Global resource dependence :
//   Author: hui.zhao
//  Note:   
/*****************************************************************************/
PUBLIC void MMIEBOOK_GetBookmarkFileName(
    wchar* src_full_path_ptr,    //in                       
    wchar* des_full_path_ptr    //out
);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//   Author: renyi.hu
//  Note:   
/*****************************************************************************/
/*PUBLIC BOOLEAN MMIEBOOK_IsExistZero(
					    MMIFILE_HANDLE file_handle,                          
					    uint32 file_size ,
					    GUI_CHAR_CODE_TYPE_E type
					);*/
/*****************************************************************************/
//  Description : MMIEBOOK_isEbookTypeFile
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_isEbookTypeFile(const wchar* name_ptr, uint32 name_length);

					
#endif//ENG_SUPPORT
#endif

