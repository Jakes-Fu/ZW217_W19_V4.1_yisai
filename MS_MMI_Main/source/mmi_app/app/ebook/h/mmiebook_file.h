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
#define MMIEBOOK_DEFAULT_AND_LRC_NAME	"*.txt *.lrc"//������֧�ֵ����и�ʽ
#define MMIEBOOK_TXT_NAME_MAX_LEN                   MMIFILE_FILE_NAME_MAX_LEN//(MMIFILE_FILE_NAME_MAX_LEN*2)      //�ı��ļ����ļ������ȣ��ֽ�
#define MMIEBOOK_FULLNAME_MAX_LEN 			        FILEARRAY_FILENAME_MAX_LEN//(FILEARRAY_FILENAME_MAX_LEN*2+2) 	//�ı��ļ���ȫ·���ļ������ȣ��ֽ�
#define MMIEBOOK_BOOKMARK_MAX_NUM                   50  //һ������༸����ǩ

#define MMIEBOOK_DEV_TAB_MAX_LEN                    10


//����Ķ��ĵ������б��ļ�ͷ�ṹ�����仯,�������MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME!!!
#ifdef MMIEBOOK_BOOKSHELF_SUPPORT
#define MMIEBOOK_LATEST_LIST_MAX_NUM                    50//����Ķ���n��������
#define MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME           "EBOOK LAST LIST FILE 09.10.0a.03"
#else
#define MMIEBOOK_LATEST_LIST_MAX_NUM                    10//����Ķ���n��������
#define MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME           "EBOOK LAST LIST FILE 09.10.0b.03"
#endif
#define MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME_LEN       32
#define MMIEBOOK_LATEST_LIST_FILE_SIZE                  (MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME_LEN +\
                                                        sizeof(MMIEBOOK_LATEST_LIST_T))                                                       
//����Ķ��ĵ������б��ļ�ͷ�ṹ�����仯,�������MMIEBOOK_LATEST_LIST_FILE_VER_ID_NAME!!!

#define UNUSED_PARAM(param)                         param = param
/*---------------------------------------------------------------------------*/
/*                          STRUCT DEFINE                                    */
/*---------------------------------------------------------------------------*/
//�ļ��豸
/*typedef enum
{
    MMIEBOOK_FILE_DEV_MIN,
    MMIEBOOK_FILE_DEV_UDISK,
    MMIEBOOK_FILE_DEV_SD,
    MMIEBOOK_FILE_DEV_SD1,//��ֹASSERT��add by charlie 2011.02.17
    MMIEBOOK_FILE_DEV_MAX
} MMIEBOOK_FILE_DEV_E;*/

//�ļ�����
typedef enum
{
    MMIEBOOK_ROOT_FILE_TYPE_TXT,//@zhaohui,cr108528
    MMIEBOOK_ROOT_FILE_TYPE_LRC,//@zhaohui,cr108528
    MMIEBOOK_FILE_TYPE_TXT,
    MMIEBOOK_FILE_TYPE_LRC,
    MMIEBOOK_FILE_TYPE_MAX
} MMIEBOOK_FILE_TYPE_E;

//������Ķ��ĵ������б��ļ���д�Ĳ�������
typedef enum
{
    MMIEBOOK_LATEST_LIST_FILE_MIN = 0,
    MMIEBOOK_LATEST_LIST_FILE_VER_ID,       //�ļ���һ���֣�����Ķ��ĵ������б��ļ��汾��Ϣ
    MMIEBOOK_LATEST_LIST_FILE_INFO,         //�ļ��ڶ����֣�����Ķ��ĵ������б���Ϣ
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
    EBOOK_NODE_TXT_OPT_JUMP_TO_BEGIN,       //��ת����ʼ
    EBOOK_NODE_TXT_OPT_JUMP_TO_END,         //��ת����β
    EBOOK_NODE_TXT_OPT_JUMP_PERCENT,        //��ת���ٷֱ�
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

//�Ե�������ǩ�ļ���д�Ĳ�������
typedef enum
{
    MMIEBOOK_BOOKMARK_FILE_MIN = 0,
//    MMIEBOOK_BOOKMARK_FILE_VER_ID,       //�ļ���һ���֣�����Ķ��ĵ������б��ļ��汾��Ϣ
    MMIEBOOK_BOOKMARK_FILE_CUR_READ_OFFSET,     //�ļ���һ����,��ǰ�Ķ���ƫ��λ��
    MMIEBOOK_BOOKMARK_FILE_INFO,         //�ļ��ڶ����֣�����Ķ��ĵ������б���Ϣ
    MMIEBOOK_BOOKMARK_FILE_MAX
} MMIEBOOK_BOOKMARK_FILE_OPT_E;


//��������ǩ��Ϣ
typedef struct MMIEBOOK_BOOKMARK_S
{
    int32 offset_tab[MMIEBOOK_BOOKMARK_MAX_NUM];//��ǩ��Ϣ����
    uint32 total_num;                            //��ǩ����Ŀ
}MMIEBOOK_BOOKMARK_T;

//�����������Ϣ
typedef struct MMIEBOOK_FILE_BRIEF_S
{
    //wchar               name_wstr[MMIEBOOK_TXT_NAME_MAX_LEN + 1];   //�ļ�����,ucs
    //uint16              name_len;           //���ֳ��ȣ��ֽ�
    int32              file_size;          //�ļ���С
    //int32				open_size;	//�ļ��ɴ򿪵Ĵ�С
    MMIFILE_DEVICE_E    file_dev;           //�ļ������豸
    //int32              detail_id;          //�ļ�������Ϣ�ṹ����������ļ��еĴ���λ��

     //add by rongjin, open ebook in my document when it isn't in ebook dir.
    wchar               full_name_wstr[MMIEBOOK_FULLNAME_MAX_LEN + 1];   //�ļ�����,ucs
    uint16             full_name_len;           						//���ֳ��ȣ��ֽ�
    														//>0, read from fmm, had full name;
     														//=0, read from ebook or read from fmm, hadn't full name;
   //add end 	
    //uint16                      current;    //��ǰѡ�е��ļ�@maggie add
	//MMIFILE_FILE_INFO_T         file[MMIEBOOK_TXT_MAX_NUM];//@maggie add 
} MMIEBOOK_FILE_BRIEF_T;

//�����豸�ϵĵ�����������Ϣ
typedef struct MMIEBOOK_ALL_DEV_FILE_BRIEF_S
{
    MMIFILE_DEVICE_E dev_tab[MMIEBOOK_DEV_TAB_MAX_LEN];
    uint16  dev_file_num_tab[MMIEBOOK_DEV_TAB_MAX_LEN];//��Ӧȫ�ֱ���g_ebook_dev_info.dev_tab������
    uint16  dev_num;
    uint16  total_num;                          //�����豸�ϵ���������
    MMIEBOOK_FILE_BRIEF_T  * file_brief_ptr;    //�����ı��ļ��ļ�����Ϣ
} MMIEBOOK_ALL_DEV_FILE_BRIEF_T;

typedef struct MMIEBOOK_LATEST_EBOOK_FILE_INFO_S
{
    uint32                              total_item_num;             //����Ķ����ļ�����
    MMIEBOOK_FILE_BRIEF_T               item_brief[MMIEBOOK_LATEST_LIST_MAX_NUM]; //����Ķ���10�����������Ϣ����
} MMIEBOOK_LATEST_LIST_T;//����Ķ��ĵ������б�

//open file error type
typedef enum
{
    //������ǰ��true, false���
    MMIEBOOK_OPEN_FILE_NO_ERROR,            //����
    MMIEBOOK_OPEN_FILE_ERROR,               //���󣬲���������

    //��չDRM���ĵ����ͣ�
    MMIEBOOK_OPEN_FILE_NO_RIGHTS,           //��Ȩ��Ч
    MMIEBOOK_OPEN_FILE_NO_SYS_TIME,         //δͬ������ʱ��
    
    MMIEBOOK_OPEN_FILE_ERROR_MAX
    
} MMIEBOOK_OPEN_FILE_ERROR_E;

extern PUBLIC MMIEBOOK_ALL_DEV_FILE_BRIEF_T     g_ebook_all_dev_file_brief;     //���д洢�豸�еĵ����������Ϣ


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
//  Description : �ͷ��ļ�������Դ
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_FreeEbookFileRes(void);

/*****************************************************************************/
//  Description : ��ָ���豸�ϲ���������Ŀ¼
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIEBOOK_CreateEbookDir(MMIFILE_DEVICE_E dev);
/*****************************************************************************/
//  Description : ��ָ���豸�ϲ���������Ŀ¼
//  Global resource dependence : 
//  Author: rongjin
//  Note:
//  Return:
/*****************************************************************************/
//add by rongjin, fixed bug 21540, 070723        
PUBLIC void MMIEBOOK_SetTxtBrief(void);

/*****************************************************************************/
//  Description : �򿪵������б���ѡ��ĵ�����
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC MMIEBOOK_OPEN_FILE_ERROR_E MMIEBOOK_OpenShowFile(
								  uint16  cur_selection//IN: �б��е�ID
								  );

/*****************************************************************************/
//  Description : �Ķ��ĵ������Ƿ����
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->����
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsShowFileOpen(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
//  Description : �ر������Ķ��ĵ�����
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_CloseShowFile(void);

/*****************************************************************************/
//  Description : ��ȡ�����Ķ��ĵ�������
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:��������
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMIEBOOK_GetShowFileHandle(void);

/*****************************************************************************/
//  Description : �����ļ��豸
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:�豸����
/*****************************************************************************/
PUBLIC uint16 MMIEBOOK_GetDevInfo(void);

/*****************************************************************************/
//  Description : �����ļ��豸���ͻ�ȡ�豸��
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
//  Description : ��ȡ�ļ�ȫ·��
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
						   MMIFILE_HANDLE  file_handle,    //�ļ�
						   int32           offset,
						   uint32          origin      //0��1��2 �ֱ��ʾ�ӵ�ǰλ�ã��ļ���β���ļ���ʼ
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
//  Description :��ʼ��s_ebook_file_brief
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
//  Description : �򿪵�����ʱ,��������Ķ��ĵ������б��ļ�
//  Global resource dependence : 
//  Author: zhaohui
//  Note:v
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_UpdateLatestList(void);

/*����Ķ��ĵ������б��ļ�������ؽӿ�,begin*/
/*****************************************************************************/
//  Description : ������Ķ��ĵ������б��ļ�
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_OpenLatestListFile(MMIFILE_DEVICE_E dev, const char * mode);

/*****************************************************************************/
//  Description : ��������Ķ��ĵ������б��ļ�
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_CreateLatestListFile(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
//  Description : ����Ķ��ĵ������б��ļ��Ƿ��
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE:�Ѵ� FALSE:δ��
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsLatestListFileOpen(void);

/*****************************************************************************/
//  Description : �õ�ָ���豸������Ķ��ĵ������б��ļ��ľ��
//  Global resource dependence : 
//  Author: zhaohui
//  Note:�ļ��Ѿ���
//  Return:�ļ����
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMIEBOOK_GetLatestListFileHandle(void);

/*****************************************************************************/
//  Description : �ر�����Ķ��ĵ������б��ļ�
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_CloseLatestListFile(void);

/*****************************************************************************/
//  Description : ��ȡ����Ķ��ĵ������б�
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_ReadLatestListFile(
                                       MMIFILE_HANDLE file_handle,
                                       MMIEBOOK_LATEST_LIST_FILE_OPT_E type,//IN: ����һ����
                                       void *param
//                                       MMIEBOOK_LATEST_LIST_T *latest_list_ptr
                                       );
                                       
/*****************************************************************************/
//  Description : ��ȡ����Ķ��ĵ������б�
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_WriteLatestListFile(
                                        MMIFILE_HANDLE file_handle,
                                        MMIEBOOK_LATEST_LIST_FILE_OPT_E type,//IN: ����һ����
                                        void *param
                                        );
                                        
/*****************************************************************************/
//  Description : ָ���豸�Ƿ�������Ķ��ĵ������б��ļ��Ƿ�������ǿ��õ�(���汾һ��),
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->���õ�
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsLatestListFileOK(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
//  Description : ָ���豸�Ƿ��������ļ�����
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->����
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsExistLatestListFile(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
//  Description : ����Ķ��ĵ������б��ļ��Ƿ��뵱ǰ����汾һ��
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:TRUE->�汾һ��
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsLatestListFileVersionVaild(MMIFILE_DEVICE_E dev);

/*****************************************************************************/
//  Description : ɾ������Ķ��ĵ������б��ļ�
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_DeleteLatestListFile(
                    MMIFILE_DEVICE_E dev
                    );   
/*����Ķ��ĵ������б��ļ�������ؽӿ�,end*/

/*****************************************************************************/
//  Description : �������ļ�
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
//  Description : �õ�ָ���豸������Ķ��ĵ������б��ļ��ľ��
//  Global resource dependence : 
//  Author: zhaohui
//  Note:�ļ��Ѿ���
//  Return:�ļ����
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMIEBOOK_GetBookmarkFileHandle(void);

/*****************************************************************************/
//  Description : �ر�����Ķ��ĵ������б��ļ�
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_CloseBookmarkFile(void);

/*****************************************************************************/
//  Description : ��ȡ����Ķ��ĵ������б�
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_ReadBookmarkFile(
					                        MMIFILE_HANDLE file_handle,         //in
					                        MMIEBOOK_BOOKMARK_FILE_OPT_E type,  //IN,����һ����
                                            void *param                         //in,д�������,
                                            );
                                            
/*****************************************************************************/
//  Description : ��ȡ����Ķ��ĵ������б�
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->OK
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_WriteBookmarkFile(
											MMIFILE_HANDLE file_handle,         //in
											MMIEBOOK_BOOKMARK_FILE_OPT_E type,  //IN,����һ����
                                            void *param                         //in,д�������,
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

