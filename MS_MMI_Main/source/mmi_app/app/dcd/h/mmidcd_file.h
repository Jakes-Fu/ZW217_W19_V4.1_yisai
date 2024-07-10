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


// DCD 已内容条目节点...
typedef struct _MMIDCD_LOCAL_FILE_NODE_T
{
#define DCD_MAX_ID_LENGTH   20
    char        ctxt_id[DCD_MAX_ID_LENGTH + 1]; //频道项内容id的全局的唯一的标识(by jun.hu for 记录下标识，以解决多次保存同一记录的问题）
    uint8       title_str[MMIDCD_MAX_TITLE_STR_LEN];
    uint16      title_len;

    uint8       summary_str[MMIDCD_MAX_SUMMARY_STR_LEN];
    uint16      summary_len;	

    uint8       content_type_str[MMIDCD_MAX_EXTAND_TYPE_STR_LEN];   //file_extend_name_ptr
    uint8       content_text_id_str[MMIDCD_MAX_TEXT_ID_STR_LEN];    //file_name_ptr
    char        link_str[DCD_MAX_HREF_LEN]; //modified by jun.hu 2009/12/4 for 和DCD对网络那边统一起来....  

}MMIDCD_LOCAL_FILE_NODE_T;

#define DCD_LOCAL_MAX_ITEM_NUM   50
// DCD 已内容内容文件的文件头...
typedef struct MMIDCD_LOCAL_FILE_HEADER_T_tag
{
    BOOLEAN     vaild_arr[DCD_LOCAL_MAX_ITEM_NUM];  // 对应的条目是否被删除,以利用被删除的条目所占的位置...
    uint16      total_num;                          // 已存DCD的条目总数..
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
// Description : 初始化s_xml_node_state，即清0 node state
// Global resource dependence : None
// Author: jun.hu , 2009/8/27
// Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_Init_XML_Node_State(void);

/*****************************************************************************/
// Description : 设置指定位置的ITEM 已读的标记
// Global resource dependence : None
// Author: jun.hu 
// Note:  2009/10/13 
/*****************************************************************************/
PUBLIC void MMIDCD_SetItemAlreadyRead(
                                      uint16 pageIndex, 
                                      uint16 itemIndex
                                      );
/*****************************************************************************/
// Description : 得到指定位置的ITEM 是否已删除
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
// Description : 与更新前的文件做比较....
// Global resource dependence : None
// Author: jun.hu
// Note:   2009/10/13
/*****************************************************************************/
PUBLIC void MMIDCDFILE_CmpTwoXmlFile(
                                     xmlDocPtr new_xml_ptr, 
                                     xmlDocPtr older_xml_ptr
                                     );
/*****************************************************************************/
// Description : 查找某一频道ID在给定的文件中是否存在....
// Global resource dependence : None
// Author: jun.hu
// Note:   2009/10/13
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_HasFeedIdInXmlFile(
                                             xmlDocPtr search_xml_ptr,
                                             const char * feedId_ptr,
                                             uint16 * find_feed_Index // out 传出找到的feedindex
                                             );
/*****************************************************************************/
// Description : 查找某一频道项ID在给定的文件制定的频道中是否存在,并且是否已读....
// Global resource dependence : None
// Author: jun.hu
// Note:   2009/10/13
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_HasReadEntryIdInXmlFile(
                                                  xmlDocPtr search_xml_ptr,
                                                  const uint16 feedIndex,
                                                  const char * entryId_ptr,
                                                  BOOLEAN *  is_exist_ptr     // out 返回这条目是否存在.
                                                  );
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 由指定的real index 转化成show index,(条件：目前没有调用该接口的没有
//        用到隐藏已读的要求，故没有设这一参数...)
/*****************************************************************************/
PUBLIC uint16 MMIDCDFILE_RealIndexToShowIndex(                                 
                                              uint16 page_index,
                                              uint16 real_item_index
                                              );
/*****************************************************************************/
// 	Description :以后可直接调用此函数即可
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
//  Description : 这个函数的作用就是一次可以删除多个，否则当你单个的删，会记录DEL  
//                的FLAG,当你再删第二个时求的real index可能不对了...
//  Global resource dependence : none
//  Author: jun.hu
//  Note:2009/11/3
/*****************************************************************************/
PUBLIC void MMIDCDFILE_DeletePageMultItem(
                                          const uint16 page_index,
                                          const uint16 * real_idx_arr, // 注意：这里面传的是绝对值
                                          const uint16 del_num   // 要删除的数量
                                          );
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
//PUBLIC void MMIDCDFILE_GetLocalFileWcharName(wchar * filename_ptr);
/*****************************************************************************/
// 	Description :获取已存文件的头信息...
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_GetLocalFileHeaderInfo(MMIDCD_LOCAL_FILE_HEADER_T * hear_ptr);
/*****************************************************************************/
// 	Description : 写已存文件的头信息...
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_WriteLocalFileHeaderInfo(MMIDCD_LOCAL_FILE_HEADER_T * hear_ptr);
/*****************************************************************************/
// 	Description : 初始化已存文件的头信息...
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/7
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_InitLocalFileHeaderInfo(void);
/*****************************************************************************/
// 	Description :得到已存的条目数...
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
PUBLIC uint16 MMIDCDFILE_GetLocalItemCount(void);
/*****************************************************************************/
// 	Description :读取指定位置的已存条目...
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5  2010/1/14 把这个参数改成相对的值....
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_ReadLocalItemInfo( 
                                            MMIDCD_LOCAL_FILE_NODE_T * read_item_ptr,
                                            uint16  item_index   // [ in ,是外面用的相对值]
                                            );
/*****************************************************************************/
// 	Description :删除指定位置的单条条目
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_DeleteLocalSingleItem(uint16  real_index);
/*****************************************************************************/
// 	Description :删除多个条目的已存..
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_DeleteLocalMultItem(
                                              uint16 * real_index_ptr,
                                              uint16 del_num
                                              );
/*****************************************************************************/
// 	Description :把本地的相对的index转换成绝对的index....
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_LocalIndexRelativeToAbs(
                                                  uint16 * real_index_ptr,
                                                  uint16 item_index     // 相对的index
                                                  );
/*****************************************************************************/
// 	Description :工程模式dcd 更新记录接口...
//	Global resource dependence : none
//  Author: zhuo.wang
//	Note:2009/11/23
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_WriteUpdateInfo( MMIDCD_REQ_TYPE_E req_type,
                                          MMIDCD_ERROR_CODE_E s_dcd_error 
                                          );

/*****************************************************************************/
// 	Description :工程模式dcd 删除记录接口...
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
// Description : 获取记录的更新的频道项数....
// Global resource dependence : None
// Author: jun.hu 
// Note: 2009/12/2
/*****************************************************************************/
PUBLIC uint16 MMIDCDFILE_GetUpDateNewItemsCount(void);
/*****************************************************************************/
// Description : 设置记录的更新的频道项数....
// Global resource dependence : None
// Author: jun.hu 
// Note: 2009/12/2
/*****************************************************************************/
PUBLIC void MMIDCDFILE_SetUpDateNewItemsCount(uint16 count);
/*****************************************************************************/
// Description : 获取xml文件中总的频道项数....
// Global resource dependence : None
// Author: jun.hu 
// Note: 2009/12/2
/*****************************************************************************/
PUBLIC uint16 MMIDCDFILE_GetXMLItemsCount(xmlDocPtr xml_ptr);
/*****************************************************************************/
// 	Description :获取图片的信息....
//	Global resource dependence : none
//  Author:jun.hu
//	Note:如果图片有数据，则分配内存空间，并返回指向图片数据的指针，否则则返回空指针，
//       如果返回值不为空, 需要调用该函数的地方释放该内存空间 2009/12/17
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
//  Note:2009/12/18 for 把DCD的文件夹作为系统文件夹隐藏起来....
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_CreateDCDDir(void);
/*****************************************************************************/
//Description :  Create Default XML File
//Global resource dependence : 
//Author:
//Note: jun.hu 2009/12/19 把它从旧函数改造而来
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_CreateDefaultXMLFile(void);
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/12/19 从旧的ReadXMLNodeFlag改造而来..
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
                                        BOOLEAN is_local // [in,是否是删除本地的图片]
                                        );
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author: 
//	Note: jun.hu 2009/12/21 由MMIDCD_LocalDeleteFiles改造而来.....
/*****************************************************************************/
PUBLIC void MMIDCDFILE_DeleteLocalPictures(void);
/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note: jun.hu 2009/12/21 把它从MMIDCD_CopyContentFile改造而来......
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
//  Note: 2009/12/21 请注意：目前仅有DCD的两个图片目录需要用到!!
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_GetDirPathForRead(
                                            wchar  *  full_path_name_ptr,    //[OUT]
                                            uint16 *  full_path_len_ptr,     //[OUT]
                                            MMIDCD_DIR_PATH_TYPE_E dir_path_type
                                            );
/*****************************************************************************/
//  Description :获取一张图片的完整路径 ...
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
//  Description :找到还有用的图片，并把对应的CONGTENT的图片有效标志置为TRUE
//               如果是没用的图片则把它处理掉.....
//  Global resource dependence : 
//  Author: jun.hu 
//  Note: 2010/1/26 for解决更新过程中按键慢的问题
/*****************************************************************************/
PUBLIC void MMIDCDFILE_FindUseablePictures(void);

/*****************************************************************************/
// 	Description :获取图片的的大小....
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
