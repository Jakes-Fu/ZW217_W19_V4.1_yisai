/*****************************************************************
 **  File Name: 	brw_utility.h    							 *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:common function of browser                      *
******************************************************************
******************************************************************
**  Edit History											     *
**---------------------------------------------------------------*
 **  DATE			NAME			DESCRIPTION				     *
 **  30/12/2008		yuzhi.chen      Create. 					 *
*****************************************************************/
#ifndef H_BRW_UTILITY
#define H_BRW_UTILITY

#include "brw_plinterface.h"
#include "brw_control_api.h"

//#define TODO #error
#define TODO 
#define	FILE_DEVICE_SLASH ":\\"
//����ṹ�嶨��
typedef struct tagBRW_LIST_T{
  void                    *object;
  struct tagBRW_LIST_T    *next;
}BRW_LIST_T;
typedef enum
{
    BRW_CHARSET_UTF8,
    BRW_CHARSET_GB2312,
    BRW_CHARSET_UNKNOWN
}BRW_CHARSET_E;
//��������ڴ��ͷŻص��ӿ�
typedef void (*BRW_LIST_FREE_F)(void *object_ptr);
typedef void* (*BRW_LIST_DUP_F)(void *object_ptr);
//�������Ƚϻص��ӿ�
typedef BOOLEAN (*BRW_LIST_CMP_F)(void *object_in_list_ptr, void *object_ptr);

/*
desc:init a BRW_LIST_T struct
param:list_ptr  pointer of the list
*/
PUBLIC void BRWList_Construct(BRW_LIST_T  *list_ptr);
/*
desc:free mem of BRW_LIST_T struct
param:list_ptr pointer of list_ptr struct
      func     callback function for free member object of BRW_LIST_T
*/
PUBLIC void BRWList_Destruct(BRW_LIST_T   *list_ptr, BRW_LIST_FREE_F func);
/*
desc:alloc a BRW_LIST_T
return:the pointer of BRW_LIST_T
*/
PUBLIC BRW_LIST_T* BRWList_New(void);
/*
desc:free BRW_LIST_T pointer
param:list_ptr pointer of BRW_LIST_T struct
      func  callback function for free member object of BRW_LIST_T struct
*/
PUBLIC void BRWList_Delete(BRW_LIST_T * list_ptr, BRW_LIST_FREE_F func);
/*
desc:remove object form list
parm:list_ptr    pointer of BRW_LIST_T struct
     position    position of object want to remove
return:the appointed object
caution:the mem of object havn't been freed
*/
PUBLIC void* BRWList_RemoveObjectAt(BRW_LIST_T* list_ptr, uint32 position);
/*
desc: append object to list
param:list_prt  the pointer of the BRW_LIST_T
      new_obj   new object pointer want to append
return:TRUE if SUCCESS,else FALSE
*/
PUBLIC BOOLEAN BRWList_AppendObject(BRW_LIST_T * list_ptr, void * new_obj);
/*
desc:insert a object at the head of list
param: list_ptr    pointer of the list
       new_object  new object want to insert
return:TRUE if SUCCESS,else FALSE
*/
PUBLIC BOOLEAN BRWList_InsertObjectAtHead(BRW_LIST_T * list_ptr, void * new_object);
/*
desc:remove an object from list
param:list_prt  the pointer of the BRW_LIST_T
      new_obj   new object pointer want to remove
return:TRUE if SUCCESS,else FALSE
*/
PUBLIC BOOLEAN BRWList_RemoveObject(BRW_LIST_T* list_ptr,void* object_ptr);
/*
desc:get count of the list
param:list_ptr  pointer of the list
return:count of the list
*/
PUBLIC uint32 BRWList_GetListCount(BRW_LIST_T * list_ptr);
/*
desc:get object of appointed position
param:list_ptr  pointer of the list
      position  the appointed position
return:the object pointer
*/
PUBLIC void* BRWList_GetObjectByPos(BRW_LIST_T * list_ptr, uint32 position);
/*
desc:duplicate a list
param:dst_list_ptr  destination list pointer
      src_list_ptr  source list pointer
      dup           callback function for duplicate object
return:TRUE if SUCCESS,else FALSE
*/
PUBLIC BOOLEAN  BRWList_Duplicate(BRW_LIST_T* dst_list_ptr,BRW_LIST_T *src_list_ptr, BRW_LIST_DUP_F dup);
/*
desc:duplicate a list
param:dst_list_ptr  destination list pointer
      src_list_ptr  source list pointer
      dup           callback function for duplicate object
return:TRUE if SUCCESS,else FALSE
*/
PUBLIC void* BRWList_SearchObject(BRW_LIST_T* list_ptr, BRW_LIST_CMP_F cmp, void* object_ptr);
/*
 *	desc:move object in list to head position
 *  param: list_ptr  list pointer
 *         obj_ptr   object pointer
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BRWList_MoveObjToHead(BRW_LIST_T* list_ptr,void* obj_ptr);
/*
 *	desc:��url�ִ�����ȡ��Դ����
 *  return:�����ȡ����Դ���֣����䷵�أ����򷵻�PNULL
 *  note:�ں����ܻ�����ڴ棬�Դ洢��Դ����
 */
PUBLIC uint8*   BRWUti_GetResNameFrUrl(uint8*   url_ptr);

PUBLIC char* BRWUtil_GetUrlScheme(const char *url_ptr);
PUBLIC BOOLEAN BrwIsUrlIsEquExceptFragment(const uint8 *url1_ptr, const uint8 *url2_ptr);
PUBLIC void BrwUrlRemoveFragment(
    const uint8 *src_url_ptr, 
    uint8   *dest_url_ptr,//[in/out]
    uint16  dest_url_len
    );

/*****************************************************************************/
//  Description : get the ico url
//  Global resource dependence : 
//  Author: fen.xie
//  Note:need to FREE the return value
/*****************************************************************************/
PUBLIC char* BRWUtil_GetIcoUrl(const char *url_ptr);

/*
 *	�����ַ�����ö�Ӧ��charset����
 */
PUBLIC BRW_CHARSET_E BRWUtil_GetCharset(const uint8* charset_ptr);  
/*
 *	desc:��ȡbrowserϵͳĿ¼�µ��ļ�
 *  param:file_name_ptr  asc��ʽ�ļ���,'\0'����
 *        buf_ptr   buffer to store data
 *        buf_max_len  max length of buffer
 *        read_len_ptr[output]  real length readed 
 *  return:TRUE if exist else FALSE
 */
PUBLIC BOOLEAN BRWUtil_ReadFileSyn(
								   const uint8* file_name_ptr,
								   const uint8 *dir_name_ptr,
								   uint8* buf_ptr,
                                   uint32 buf_max_len,
								   uint32* read_len_ptr
								   );
/*
 *	desc:дbrowserϵͳĿ¼�µ��ļ�
 *  param:file_name_ptr  asc��ʽ�ļ���,'\0'����
 *        full_path_len  length of path
 *        buf_ptr   address of data to write
 *        to_write_len  the length of data want to write
 *  return:TRUE if exist else FALSE
 */
PUBLIC BOOLEAN BRWUtil_WriteFileSyn(
									const uint8* file_name_ptr,
									const uint8 *dir_name_ptr,
									const uint8* buf_ptr,
									uint32 to_write_len
									);
/*
 *	desc:���browserĿ¼��ĳ���ļ��ĳ���
 *  param:file_name_ptr  asc��ʽ�ļ���,'\0'���� 
 *        size_ptr[output]   size of file
 *  return:TRUE if exist else FALSE
 */
PUBLIC BOOLEAN BRWUtil_GetFileSize(const uint8 *file_name_ptr,uint32* size_ptr);
/*
 *	desc:ɾ��browserĿ¼�µ�ĳ���ļ�(ͬ��)
 *  param:file_name_ptr  asc��ʽ�ļ���,'\0'����
 *  return:TRUE if success else FALSE
 */
PUBLIC BOOLEAN BRWUtil_DelFileSyn(const uint8 *file_name_ptr);
/*
 *	desc:����browserϵͳĿ¼(����Ѿ�����,�򷵻�TRUE)
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BRWUtil_CreateDir(void);

PUBLIC BOOLEAN BRWUtil_GetBrwRootPath(uint16 *path_ptr, uint32 path_max_len);

/*
desc:decode the gzip buffer
param:
return:BRW_ERROR_E:the decode result
caution:*dest_buf_ptr need to free outside
*/
PUBLIC BRW_ERROR_E BRWUtil_DecodeGzip(
                uint8 *src_buf_ptr, //[IN]
                uint32    src_len,//[IN]
                uint8 **dest_buf_ptr,//[IN/OUT]*dest_buf_ptr need to free outside
                uint32    *dest_len_ptr,//[IN/OUT]
                uint32  max_size
                );

/*
desc:decode the gzip file
param:
return:BRW_ERROR_E:the decode result
caution:
*/
PUBLIC BRW_ERROR_E BRWUtil_DecodeGzipByFile(
                uint16 *src_file_name_ptr,
                uint16 *dest_file_name_ptr
                );

#endif
