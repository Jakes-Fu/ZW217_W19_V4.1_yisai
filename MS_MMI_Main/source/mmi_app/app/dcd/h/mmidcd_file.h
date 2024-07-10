/*****************************************************************************
** File Name:      dcd_file.h                                                  *
** Author:                                                                   *
** Date:           10/2007                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mp3 player                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2007        lisa.lin       Create                                  *
******************************************************************************/


#ifndef  _MMIDCD_FILE_H_
#define  _MMIDCD_FILE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"


#include "sci_types.h"
#include "mmifmm_export.h"
#include "os_api.h"

#ifdef MMI_ENABLE_DCD
#include "mmidcd_xml_parse.h"
#include "mmidcd_setting.h" 
#include "mmidcd_http.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMIDCD_INDEX_FILE_PATH,      
    MMIDCD_NOD_FILE_PATH,
    MMIDCD_OLD_FILE_PATH,    
    MMIDCD_LOCAL_FILE_PATH,    
    MMIDCD_FILE_PATH_TYPE_MAX
}MMIDCD_FILE_PATH_TYPE_E;

typedef enum
{
    MMIDCD_MAIN_PATH,       
    MMIDCD_PICTURE_DIR_PATH,
    MMIDCD_LOCAL_PICTURE_DIR_PATH,
    MMIDCD_DIR_PATH_TYPE_MAX
}MMIDCD_DIR_PATH_TYPE_E;

typedef enum
{
   MMIDCD_ITEM_SAVE_FAIL,
   MMIDCD_ITEM_SAVE_SUCESS,
   MMIDCD_ITEM_ALREADY_EXIST
}MMIDCD_FILE_SAVE_ITEM_RESULT_E;

#define MMIDCD_MAX_FILE_PATH_LEN                60
#define MMIDCD_MAX_TITLE_STR_LEN                64
#define MMIDCD_MAX_SUMMARY_STR_LEN              512
#define MMIDCD_MAX_EXTAND_TYPE_STR_LEN          16
#define MMIDCD_MAX_TEXT_ID_STR_LEN              64


// DCD ��������Ŀ�ڵ�...
typedef struct _MMIDCD_LOCAL_FILE_NODE_T
{
#define DCD_MAX_ID_LENGTH   20
    char        ctxt_id[DCD_MAX_ID_LENGTH + 1]; //Ƶ��������id��ȫ�ֵ�Ψһ�ı�ʶ(by jun.hu for ��¼�±�ʶ���Խ����α���ͬһ��¼�����⣩
    uint8       title_str[MMIDCD_MAX_TITLE_STR_LEN];
    uint16      title_len;

    uint8       summary_str[MMIDCD_MAX_SUMMARY_STR_LEN];
    uint16      summary_len;	

    uint8       content_type_str[MMIDCD_MAX_EXTAND_TYPE_STR_LEN];   //file_extend_name_ptr
    uint8       content_text_id_str[MMIDCD_MAX_TEXT_ID_STR_LEN];    //file_name_ptr
    char        link_str[DCD_MAX_HREF_LEN]; //modified by jun.hu 2009/12/4 for ��DCD�������Ǳ�ͳһ����....  

}MMIDCD_LOCAL_FILE_NODE_T;

#define DCD_LOCAL_MAX_ITEM_NUM   50
// DCD �����������ļ����ļ�ͷ...
typedef struct MMIDCD_LOCAL_FILE_HEADER_T_tag
{
    BOOLEAN     vaild_arr[DCD_LOCAL_MAX_ITEM_NUM];  // ��Ӧ����Ŀ�Ƿ�ɾ��,�����ñ�ɾ������Ŀ��ռ��λ��...
    uint16      total_num;                          // �Ѵ�DCD����Ŀ����..
}MMIDCD_LOCAL_FILE_HEADER_T;

#define DCD_UPDATE_INFO_NUM   1000
/**--------------------------------------------------------------------------*
 **                         PUBLIC FUNCTION DECLARE                          *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :  Convert FileName to ctxt_id
//  Global resource dependence : 
//  %
//   file_name_ptr:  usc2 file name
//   file_name_len: file name length
//   ctxt_id :    gb code of file name(not include suffix)
//  %
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIDCDFFS_FileName2CtxtId( 
                                        const uint16* file_name_ptr,
                                        uint16 file_name_len,
                                        uint8* ctxt_id
                                        );
/*****************************************************************************/
// Description : ��ʼ��s_xml_node_state������0 node state
// Global resource dependence : None
// Author: jun.hu , 2009/8/27
// Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_Init_XML_Node_State(void);

/*****************************************************************************/
// Description : ����ָ��λ�õ�ITEM �Ѷ��ı��
// Global resource dependence : None
// Author: jun.hu 
// Note:  2009/10/13 
/*****************************************************************************/
PUBLIC void MMIDCD_SetItemAlreadyRead(
                                      uint16 pageIndex, 
                                      uint16 itemIndex
                                      );
/*****************************************************************************/
// Description : �õ�ָ��λ�õ�ITEM �Ƿ���ɾ��
// Global resource dependence : None
// Author: jun.hu , 2009/8/27
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_IS_Node_State_Deleted(uint16 pageIndex, uint16 itemIndex);

/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:z.f.
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIDCD_GetRealItemIndex(                                 
                                     uint16 page_index,
                                     uint16 item_index,
                                     BOOLEAN is_disable_read
                                     );
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMIDCD_DeleteLocalItem(
                                   MMIDCD_LOCAL_FILE_NODE_T* xml_doc_ptr,
                                   uint16 index
                                   );
/*****************************************************************************/
//Description :  Create Default XML File
//Global resource dependence : 
//Author:
//Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFFS_ParseDefaultXMLFile(void);
/*****************************************************************************/
// Description : �����ǰ���ļ����Ƚ�....
// Global resource dependence : None
// Author: jun.hu
// Note:   2009/10/13
/*****************************************************************************/
PUBLIC void MMIDCDFILE_CmpTwoXmlFile(
                                     xmlDocPtr new_xml_ptr, 
                                     xmlDocPtr older_xml_ptr
                                     );
/*****************************************************************************/
// Description : ����ĳһƵ��ID�ڸ������ļ����Ƿ����....
// Global resource dependence : None
// Author: jun.hu
// Note:   2009/10/13
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_HasFeedIdInXmlFile(
                                             xmlDocPtr search_xml_ptr,
                                             const char * feedId_ptr,
                                             uint16 * find_feed_Index // out �����ҵ���feedindex
                                             );
/*****************************************************************************/
// Description : ����ĳһƵ����ID�ڸ������ļ��ƶ���Ƶ�����Ƿ����,�����Ƿ��Ѷ�....
// Global resource dependence : None
// Author: jun.hu
// Note:   2009/10/13
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_HasReadEntryIdInXmlFile(
                                                  xmlDocPtr search_xml_ptr,
                                                  const uint16 feedIndex,
                                                  const char * entryId_ptr,
                                                  BOOLEAN *  is_exist_ptr     // out ��������Ŀ�Ƿ����.
                                                  );
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:jun.hu
//	Note: ��ָ����real index ת����show index,(������Ŀǰû�е��øýӿڵ�û��
//        �õ������Ѷ���Ҫ�󣬹�û������һ����...)
/*****************************************************************************/
PUBLIC uint16 MMIDCDFILE_RealIndexToShowIndex(                                 
                                              uint16 page_index,
                                              uint16 real_item_index
                                              );
/*****************************************************************************/
// 	Description :�Ժ��ֱ�ӵ��ô˺�������
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 2009/11/3
/*****************************************************************************/
PUBLIC void MMIDCDFILE_DeleteRealIndexItem(
                                           uint16 page_index,
                                           uint16 real_item_index
                                           );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCDFILE_DeletePageSingleItem(
                                            const uint16 page_index,
                                            const uint16 real_item_index                                                    
                                            );
/*****************************************************************************/
//  Description : ������������þ���һ�ο���ɾ������������㵥����ɾ�����¼DEL  
//                ��FLAG,������ɾ�ڶ���ʱ���real index���ܲ�����...
//  Global resource dependence : none
//  Author: jun.hu
//  Note:2009/11/3
/*****************************************************************************/
PUBLIC void MMIDCDFILE_DeletePageMultItem(
                                          const uint16 page_index,
                                          const uint16 * real_idx_arr, // ע�⣺�����洫���Ǿ���ֵ
                                          const uint16 del_num   // Ҫɾ��������
                                          );
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
//PUBLIC void MMIDCDFILE_GetLocalFileWcharName(wchar * filename_ptr);
/*****************************************************************************/
// 	Description :��ȡ�Ѵ��ļ���ͷ��Ϣ...
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_GetLocalFileHeaderInfo(MMIDCD_LOCAL_FILE_HEADER_T * hear_ptr);
/*****************************************************************************/
// 	Description : д�Ѵ��ļ���ͷ��Ϣ...
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_WriteLocalFileHeaderInfo(MMIDCD_LOCAL_FILE_HEADER_T * hear_ptr);
/*****************************************************************************/
// 	Description : ��ʼ���Ѵ��ļ���ͷ��Ϣ...
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/7
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_InitLocalFileHeaderInfo(void);
/*****************************************************************************/
// 	Description :�õ��Ѵ����Ŀ��...
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
PUBLIC uint16 MMIDCDFILE_GetLocalItemCount(void);
/*****************************************************************************/
// 	Description :��ȡָ��λ�õ��Ѵ���Ŀ...
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5  2010/1/14 ����������ĳ���Ե�ֵ....
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_ReadLocalItemInfo( 
                                            MMIDCD_LOCAL_FILE_NODE_T * read_item_ptr,
                                            uint16  item_index   // [ in ,�������õ����ֵ]
                                            );
/*****************************************************************************/
// 	Description :ɾ��ָ��λ�õĵ�����Ŀ
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_DeleteLocalSingleItem(uint16  real_index);
/*****************************************************************************/
// 	Description :ɾ�������Ŀ���Ѵ�..
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_DeleteLocalMultItem(
                                              uint16 * real_index_ptr,
                                              uint16 del_num
                                              );
/*****************************************************************************/
// 	Description :�ѱ��ص���Ե�indexת���ɾ��Ե�index....
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_LocalIndexRelativeToAbs(
                                                  uint16 * real_index_ptr,
                                                  uint16 item_index     // ��Ե�index
                                                  );
/*****************************************************************************/
// 	Description :����ģʽdcd ���¼�¼�ӿ�...
//	Global resource dependence : none
//  Author: zhuo.wang
//	Note:2009/11/23
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_WriteUpdateInfo( MMIDCD_REQ_TYPE_E req_type,
                                          MMIDCD_ERROR_CODE_E s_dcd_error 
                                          );

/*****************************************************************************/
// 	Description :����ģʽdcd ɾ����¼�ӿ�...
//	Global resource dependence : none
//  Author: zhuo.wang
//	Note:2009/11/23
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_DeleteUpdateInfo( void );


/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: zhuo.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_IsDefaultXML(uint16 page_index );
/*****************************************************************************/
// Description : ��ȡ��¼�ĸ��µ�Ƶ������....
// Global resource dependence : None
// Author: jun.hu 
// Note: 2009/12/2
/*****************************************************************************/
PUBLIC uint16 MMIDCDFILE_GetUpDateNewItemsCount(void);
/*****************************************************************************/
// Description : ���ü�¼�ĸ��µ�Ƶ������....
// Global resource dependence : None
// Author: jun.hu 
// Note: 2009/12/2
/*****************************************************************************/
PUBLIC void MMIDCDFILE_SetUpDateNewItemsCount(uint16 count);
/*****************************************************************************/
// Description : ��ȡxml�ļ����ܵ�Ƶ������....
// Global resource dependence : None
// Author: jun.hu 
// Note: 2009/12/2
/*****************************************************************************/
PUBLIC uint16 MMIDCDFILE_GetXMLItemsCount(xmlDocPtr xml_ptr);
/*****************************************************************************/
// 	Description :��ȡͼƬ����Ϣ....
//	Global resource dependence : none
//  Author:jun.hu
//	Note:���ͼƬ�����ݣ�������ڴ�ռ䣬������ָ��ͼƬ���ݵ�ָ�룬�����򷵻ؿ�ָ�룬
//       �������ֵ��Ϊ��, ��Ҫ���øú����ĵط��ͷŸ��ڴ�ռ� 2009/12/17
/*****************************************************************************/
PUBLIC uint8 *  MMIDCDFILE_GetPictureInfo(wchar * full_file_name_ptr, uint32 * file_size_ptr);
/*****************************************************************************/
//  Description : get dcd index file full path for read
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/12/18
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_GetFilePathForRead(
                                             wchar  *  full_path_name_ptr,    //[OUT]
                                             uint16 *  full_path_len_ptr,     //[OUT]
                                             MMIDCD_FILE_PATH_TYPE_E file_path_type
                                             );
/*****************************************************************************/
//  Description : get dcd index file full path for write
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/12/18
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_GetFilePathForWrite(
                                              wchar  *  full_path_name_ptr,    //[OUT]
                                              uint16 *  full_path_len_ptr,     //[OUT]
                                              MMIDCD_FILE_PATH_TYPE_E file_path_type, //[IN]
                                              uint32    size                    //[IN]
                                              );
/*****************************************************************************/
//  Description : create dcd dir 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:2009/12/18 for ��DCD���ļ�����Ϊϵͳ�ļ�����������....
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_CreateDCDDir(void);
/*****************************************************************************/
//Description :  Create Default XML File
//Global resource dependence : 
//Author:
//Note: jun.hu 2009/12/19 �����Ӿɺ����������
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_CreateDefaultXMLFile(void);
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/12/19 �Ӿɵ�ReadXMLNodeFlag�������..
/*****************************************************************************/
PUBLIC void MMIDCDFILE_ReadItemsState(void);
/*****************************************************************************/
//  Description :delete 
//  Global resource dependence : 
//  Author: 
//  Note: jun.hu 2009/12/21
/*****************************************************************************/
PUBLIC void MMIDCDFILE_DeleteOnePicture(
                                        char * picture_name_ptr, // [in: picture name]
                                        char * extend_name_ptr,   // [in: extend name]
                                        BOOLEAN is_local // [in,�Ƿ���ɾ�����ص�ͼƬ]
                                        );
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author: 
//	Note: jun.hu 2009/12/21 ��MMIDCD_LocalDeleteFiles�������.....
/*****************************************************************************/
PUBLIC void MMIDCDFILE_DeleteLocalPictures(void);
/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note: jun.hu 2009/12/21 ������MMIDCD_CopyContentFile�������......
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_SaveOnePicture(
                                         const char *file_name_ptr,
                                         const char *file_extend_name_ptr,
                                         MMI_WIN_ID_T win_id
                                         );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/12/21 ��ע�⣺Ŀǰ����DCD������ͼƬĿ¼��Ҫ�õ�!!
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_GetDirPathForRead(
                                            wchar  *  full_path_name_ptr,    //[OUT]
                                            uint16 *  full_path_len_ptr,     //[OUT]
                                            MMIDCD_DIR_PATH_TYPE_E dir_path_type
                                            );
/*****************************************************************************/
//  Description :��ȡһ��ͼƬ������·�� ...
//  Global resource dependence : 
//  Author: 
//  Note: jun.hu 2009/12/21 
/*****************************************************************************/
PUBLIC void MMIDCDFILE_GetOnePictureFullPath(
                                             wchar * full_path_ptr,   // [out: full path]
                                             const char * picture_name_ptr, // [in: picture name]
                                             const char * extend_name_ptr,  // [in: extend name]
                                             MMIDCD_DIR_PATH_TYPE_E dir_path_type
                                             );
/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:jun.hu
//	Note: 2010/1/21
/*****************************************************************************/
PUBLIC MMIDCD_FILE_SAVE_ITEM_RESULT_E MMIDCDFILE_SaveOneItemText(
                                                                 uint16 page_index,
                                                                 uint16 real_item_index
                                                                 );
/*****************************************************************************/
//  Description :�ҵ������õ�ͼƬ�����Ѷ�Ӧ��CONGTENT��ͼƬ��Ч��־��ΪTRUE
//               �����û�õ�ͼƬ����������.....
//  Global resource dependence : 
//  Author: jun.hu 
//  Note: 2010/1/26 for������¹����а�����������
/*****************************************************************************/
PUBLIC void MMIDCDFILE_FindUseablePictures(void);

/*****************************************************************************/
// 	Description :��ȡͼƬ�ĵĴ�С....
//	Global resource dependence : none
//  Author:jun.hu
//	Note:
/*****************************************************************************/
PUBLIC uint32  MMIDCDFILE_GetPictureSize(wchar * full_file_name_ptr);
/*****************************************************************************/
//Description : Rename dcd xml file 
//Global resource dependence : 
//Author: jun.hu 
//Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIDCDFILE_RenameXMLFile(
                                        MMIDCD_FILE_PATH_TYPE_E src_file_type, //[in]
                                        MMIDCD_FILE_PATH_TYPE_E dest_file_type //[in]
                                        );
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif /*#ifdef MMI_ENABLE_DCD*/
#endif /*#ifdef _MMIDCD_FILE_H_*/
