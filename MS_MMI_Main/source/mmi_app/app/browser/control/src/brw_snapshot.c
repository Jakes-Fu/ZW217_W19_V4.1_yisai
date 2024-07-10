/****************************************************************************************
** File Name:      brw_snapshot.c                                                       *
** Author:         juan.zhang                                                           *
** Date:           2009.6.18                                                            *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:    This file is for web page snapshot and offline browsing              *
** Note:																				*
		1> snapshot由于使用了MMI的消息机制，同MMI耦合较大。如果以后MMI有了自己的外部消息*
		定义可以将snapshot同ffs的callback交互消息改为外部消息，消除同MMI的耦合			*
*****************************************************************************************
**                         Important Edit History                                       *
** -------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                          *
** 2009.6        juan.zhang         Create                                              *
**                                                                                      *
*****************************************************************************************/

#include "mmi_app_browser_trc.h"
#include "mmk_msg.h"
#include "mmk_app.h"
#include "mmi_appmsg.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "brw_utility.h"
#include "brw_history.h"
#include "brw_control.h"
#include "brw_control_api.h"
#include "mmifmm_export.h"
//#include "mmi_custom_define.h"

#define CSS_FOLDER_NAME	"CSS"
#define IMAGE_FOLDER_NAME "IMAGE"
//#define WCHAR_SLASH 0x005f
#define INDEX_NAME_MAX_LEN 3
#define MAX_INDEX	999
#define SNAPSHOT_FILE_NAME_SUFFIX ".html"
#define SNAPSHOT_FIND_ALL_FILE_STR ":\\Browser\\Snapshot\\*.html" //盘符未知
#define SNAPSHOT_FILE_NAME_SUFFIX_LEN 5
#define	SNAPSHOT_MAX_IMG_SUFFIX_LEN	10
//#define SNAPSHOT_FOLDER_ROUTE ":\\browser\\snapshot"
#define SNAPSHOT_FOLDER_ROUTE_LEN 19 //包括盘符的长度
typedef struct 
{
	uint16	*save_route_ptr;
	char	*name_ptr;
}BRW_SNAPSHOT_CSS_INFO_T;

//typedef struct 
//{
//	uint32		index;
//	BRW_LIST_T	media_name_list;
//}BRW_SAVE_MEDIA_LIST_INFO_T;

typedef struct 
{
	uint16	*save_route_ptr;
	BRW_LIST_T media_name_list; //media name
}BRW_SNAPSHOT_SAVE_MEDIA_T;

typedef struct 
{
	LAYOUT_HANDLE	layout_handler;
	BRWPL_FFS_HANDLE	file_handler;
	uint16 *save_route_ptr;
	uint16 *file_name_ptr;
	BRW_SNAPSHOT_SAVE_CALLBACK	callback;
	BRW_SNAPSHOT_CSS_INFO_T	css_info;
	BRW_SNAPSHOT_SAVE_MEDIA_T	img_info;
}BRW_SNAPSHOT_SAVE_T;

typedef struct 
{
	uint32	head_len;
	BRW_MIME_TYPE_E page_mime_type;
	uint32 charset_len;
	uint32 url_len;
	uint32 img_folder_name_len;
}BRW_SNAPSHOT_FILE_HEAD_T;

typedef struct 
{
	BRW_INSTANCE browser_instance;
	uint16 *full_file_path_ptr;
	uint16 *img_route_ptr;
	BRWPL_FFS_HANDLE	file_handler;
	uint8 *file_buf_ptr;
	uint32 file_size;
	//uint32 cur_img_index;
	BRW_MIME_TYPE_E page_mime_type;
	uint8 *charset_ptr;
	char *url_ptr;
	LAYOUT_HANDLE layout_handle;
	BRW_LIST_T	open_media_list;//char* name_ptr
	BRW_SNAPSHOT_OPEN_CALLBACK callback;	
    BOOLEAN is_fixed;
}BRW_SNAPSHOT_OPEN_T;

LOCAL BRW_LIST_T			g_snapshot_file_list = {0};
LOCAL BRW_SNAPSHOT_SAVE_T	*g_snapshot_save_info_ptr = 0;
LOCAL BRW_SNAPSHOT_OPEN_T	g_snapshot_open_info = {0};

LOCAL char* BrwGetSaveMediaName(BRW_MIME_TYPE_E mime_type, uint32 index);
LOCAL BOOLEAN BrwGenerateImgInfo(BRW_LIST_T *list_ptr,LAYOUT_HANDLE layout_handle);
LOCAL BOOLEAN BrwGenerateCssInfo(BRW_SNAPSHOT_CSS_INFO_T *css_info_ptr,LAYOUT_HANDLE layout_handle);
LOCAL uint16* BrwSnapshotCreatFolder(uint16 *route_ptr,char *folder_char_name_ptr);
LOCAL BOOLEAN BrwSnapshotGenerateAllFileFolder(BRW_SNAPSHOT_SAVE_T *save_info_ptr);
LOCAL void BrwWriteOrReadWebPageCb(
												 SFS_ERROR_E error, //result
												 uint32 irp_param, //app param
												 uint32 param1,	//bytes written
												 uint32 param2
												 );
LOCAL void BrwSnapshotMediaListFreeFunc(void *data_ptr);
LOCAL void BrwDestroySaveInfo(void);
LOCAL void BrwCallbackSaveResult(SFS_ERROR_E sfs_error);
LOCAL BOOLEAN BrwFfsWriteOneImage(void);
LOCAL uint32 BrwGetIndexFromMediaName(char* name_ptr);
LOCAL SFS_ERROR_E BrwFfsWritePageContent(BRW_INFO_T *brw_info_ptr);
LOCAL BOOLEAN BrwFfsWriteCss(void);
LOCAL void BrwCallbackOpenResult(char *url_ptr,BOOLEAN is_succ);
LOCAL void BrwDestroyOpenInfo(void);
LOCAL BRW_MIME_TYPE_E BrwGetPageMimeType(uint8 *file_buf_ptr);
LOCAL uint8* BrwGetPageCharset(uint8 *file_buf_ptr);
LOCAL LAYOUT_HANDLE BrwParseOfflineWebPage(
										   uint8 *page_content_ptr,
										   uint32 page_len,
										   uint8 *charset_ptr,
										   char *url_ptr,
										   BRW_INSTANCE browser_instance,
										   BRW_MIME_TYPE_E mime_type);
LOCAL BOOLEAN BrwGenerateOpenMediaList(void);
LOCAL BOOLEAN BrwOpenOneImage(void);
LOCAL char* BrwGetPageUrl(uint8 *file_buf_ptr);
LOCAL BOOLEAN BrwSnapshotDeleteWebPage(												   
										 const uint16 *file_route_ptr
											);
LOCAL void BrwFreeItemInfoMem(BRW_SNAPSHOT_ITEM_INFO_T *item_info_ptr);
LOCAL void BrwSnapshotReadFfs(const wchar *device_ptr,  uint16 device_len);
LOCAL BRW_SNAPSHOT_ITEM_INFO_T* BrwSetItemInfo(BRWPL_FFS_FIND_DATA_T *file_info_ptr,uint16 *file_path_ptr);
LOCAL void BrwSnapshotListItemDestruct(void *item_info_ptr);
LOCAL void BrwSnapshotInsertItem(uint16 *file_path_ptr);
LOCAL uint16* BrwGetPageImgRoute(uint8 *file_buf_ptr, uint16 *full_file_path_ptr);
LOCAL void SavePageImageCb(
							 SFS_ERROR_E error, //result
							 uint32 irp_param, //app param
							 uint32 param1,	//bytes written/read
							 uint32 param2
							 );

LOCAL char* BrwGetSaveMediaName(BRW_MIME_TYPE_E mime_type, uint32 index)
{
	char *suffix_ptr = 0;
	char *name_ptr = 0;

	switch(mime_type)
	{
	case BRW_MIME_IMG_BMP:
		suffix_ptr = ".bmp";
		break;
	case BRW_MIME_IMG_GIF:
		suffix_ptr = ".gif";
		break;
	case BRW_MIME_IMG_JPEG:
		suffix_ptr = ".jpg";
		break;
	case BRW_MIME_IMG_PNG:
		suffix_ptr = ".png";
		break;
	case BRW_MIME_IMG_TIF:
		suffix_ptr = ".tif";
		break;
	case BRW_MIME_IMG_WBMP:
		suffix_ptr = ".wbmp";
		break;
	case BRW_MIME_IMG_PCX:
		suffix_ptr = ".pcx";
		break;
	default:
		suffix_ptr = ".jpg";
		break;
	}

	name_ptr = BRWMem_Alloc(BRW_Strlen((const uint8*)suffix_ptr)+INDEX_NAME_MAX_LEN+1);
	if(0!=name_ptr)
	{
		BRW_Memset(name_ptr, 0, BRW_Strlen((const uint8*)suffix_ptr)+INDEX_NAME_MAX_LEN+1);
		_snprintf(name_ptr,INDEX_NAME_MAX_LEN,"%d",index);
		BRW_Strcat((uint8*)name_ptr, (const uint8*)suffix_ptr);
	}

	return name_ptr;
}
LOCAL BOOLEAN BrwGenerateImgInfo(BRW_LIST_T *list_ptr,LAYOUT_HANDLE layout_handle)
{
	uint8	*url_ptr = 0;
	uint32	index = 0;
	char	*name_ptr = 0;
	BRW_MIME_TYPE_E mime_type = 0;
	uint8	*data_ptr=0;
	uint32	data_len = 0;
	BOOLEAN is_url = TRUE;
	
	if(0==list_ptr || 0==layout_handle)
	{
		return FALSE;
	}
	
	BRWList_Construct(list_ptr);
	while(BrwGetImgUrl(layout_handle,&url_ptr,index, &is_url))
	{
        if(url_ptr!=0 && is_url)
        {
            BrwGetImgBuf(&data_ptr, &data_len, &mime_type, layout_handle, index);
            name_ptr = BrwGetSaveMediaName(mime_type,index);
            if(0==name_ptr)
            {
                return FALSE;
            }
            BRWList_AppendObject(list_ptr,name_ptr);
        }
        else if(!is_url)
        {
            index--;
            
        }
		index++;
	}
	return TRUE;
}
LOCAL BOOLEAN BrwGenerateCssInfo(BRW_SNAPSHOT_CSS_INFO_T *css_info_ptr,LAYOUT_HANDLE layout_handle)
{
// 	uint8	*url_ptr = 0;
	
	if(0==css_info_ptr || 0==layout_handle)
	{
		return FALSE;
	}
	//get css: marked, no css gonna be saved at present version
//	if(0 != (url_ptr=BrwGetCssUrl(layout_handle)))
//	{
//		css_info_ptr->name_ptr = BrwGetSaveMediaName(url_ptr,0);
//	}
	return TRUE;
}
LOCAL uint16* BrwSnapshotCreatFolder(uint16 *route_ptr,char *folder_char_name_ptr)
{
	uint16 *folder_name_ptr = 0;
	uint32 folder_len = 0;
	uint16	*creat_path_ptr = 0;
	uint32	len = 0;

	if(0==route_ptr || 0==folder_char_name_ptr)
	{
		goto error;
	}
	len = sizeof(uint16)*BRW_WStrlen(route_ptr);
	if(len==0)
	{
		goto error;
	}
	folder_len = sizeof(uint16)*strlen(folder_char_name_ptr);
	folder_name_ptr = BRWMem_Alloc(folder_len+2);
	BRWPL_StrToWstr(folder_char_name_ptr,folder_name_ptr);
	creat_path_ptr = BRWMem_Alloc(len+folder_len+2+2);
	if(0==creat_path_ptr)
	{
		goto error;
	}
	BRW_Memset(creat_path_ptr,0,len+folder_len+2+2);
	BRW_Memcpy(creat_path_ptr,route_ptr,len);
	BRWPL_WstrStrnCatWithAsciiStr(creat_path_ptr,BRW_FILE_SLASH, (len+folder_len)/2+1+1);
	BRW_WstrCat(creat_path_ptr,folder_name_ptr);
	if(!BRWPL_CreateDir(creat_path_ptr,(len+folder_len)/sizeof(uint16)+1+1))
	{
		//SCI_TRACE_LOW:"BrwSnapshotCreatFolder fail"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_272_112_2_18_2_7_25_796,(uint8*)"");
		goto error;
	}
	if(folder_name_ptr!=0)
	{
		BRWMem_Free(folder_name_ptr);
		folder_name_ptr = 0;
	}
	return creat_path_ptr;
	
error:
	if(creat_path_ptr!=0)
	{
		BRWMem_Free(creat_path_ptr);
		creat_path_ptr = 0;
	}
	if(folder_name_ptr!=0)
	{
		BRWMem_Free(folder_name_ptr);
		folder_name_ptr = 0;
	}
	return 0;
}
LOCAL BOOLEAN BrwSnapshotGenerateAllFileFolder(BRW_SNAPSHOT_SAVE_T *save_info_ptr)
{
	uint16	*submedia_folder_route = 0;
	uint32	len = 0;
	if(0==save_info_ptr)
	{
		return FALSE;
	}
	if(0==save_info_ptr->save_route_ptr || 0==save_info_ptr->file_name_ptr)
	{
		return FALSE;
	}

	len = sizeof(uint16)*(BRW_WStrlen(save_info_ptr->save_route_ptr)+BRW_WStrlen(save_info_ptr->file_name_ptr));
	submedia_folder_route = BRWMem_Alloc(len+2+2);
	if(0==submedia_folder_route)
	{
		return FALSE;
	}
	BRW_Memset(submedia_folder_route,0,len+2+2);
	BRW_WstrCat(submedia_folder_route,save_info_ptr->save_route_ptr);
	BRWPL_WstrStrnCatWithAsciiStr(submedia_folder_route,BRW_FILE_SLASH,len/2+1+1);
	BRW_WstrCat(submedia_folder_route,save_info_ptr->file_name_ptr);

	if(!BRWPL_IsFolderExist(submedia_folder_route,len+2+2))
	{		
		if(!BRWPL_CreateDir(submedia_folder_route,len+2+2))
		{
			goto error;
		}
	}
	if(0!=save_info_ptr->css_info.name_ptr)
	{
		if(0 == (save_info_ptr->css_info.save_route_ptr=BrwSnapshotCreatFolder(
									submedia_folder_route,CSS_FOLDER_NAME))
									)
		{
			goto error;
		}
	}
	if(0!=save_info_ptr->img_info.media_name_list.next)
	{
		if(0 == (save_info_ptr->img_info.save_route_ptr=BrwSnapshotCreatFolder(
									submedia_folder_route,IMAGE_FOLDER_NAME))
									)
		{
			goto error;
		}
	}
	BRWMem_Free(submedia_folder_route);
	return TRUE;
error:
	BRWMem_Free(submedia_folder_route);
	return FALSE;
}
LOCAL void BrwWriteOrReadWebPageCb(
												 SFS_ERROR_E error, //result
												 uint32 irp_param, //app param
												 uint32 param1,	//bytes written/read
												 uint32 param2
												 )
{
    if ((0==param1) && (SFS_ERROR_NONE==error))
    {
        error = SFS_ERROR_INVALID_PARAM;
    }
    
    MMK_PostMsg(VIRTUAL_WIN_ID, irp_param, &error, sizeof(SFS_ERROR_E));
    MMI_TriggerMMITask();
}
LOCAL void BrwSnapshotMediaListFreeFunc(void *data_ptr)
{
	if(data_ptr!=0)
	{
		BRWMem_Free(data_ptr);
	}
}
LOCAL void BrwDestroySaveInfo(void)
{
	if(0==g_snapshot_save_info_ptr)
	{
		return;
	}
	if(0!=g_snapshot_save_info_ptr->save_route_ptr)
	{
		BRWMem_Free(g_snapshot_save_info_ptr->save_route_ptr);
	}
	if(0!=g_snapshot_save_info_ptr->file_name_ptr)
	{
		BRWMem_Free(g_snapshot_save_info_ptr->file_name_ptr);
	}
	if(0!=g_snapshot_save_info_ptr->css_info.save_route_ptr)
	{
		BRWMem_Free(g_snapshot_save_info_ptr->css_info.save_route_ptr);
	}
	if(0!=g_snapshot_save_info_ptr->css_info.name_ptr)
	{
		BRWMem_Free(g_snapshot_save_info_ptr->css_info.name_ptr);
	}
	if(0!=g_snapshot_save_info_ptr->img_info.save_route_ptr)
	{
		BRWMem_Free(g_snapshot_save_info_ptr->img_info.save_route_ptr);
	}
	if(0!=g_snapshot_save_info_ptr->img_info.media_name_list.next)
	{
		BRWList_Destruct(&g_snapshot_save_info_ptr->img_info.media_name_list,BrwSnapshotMediaListFreeFunc);
	}
	if(0!=g_snapshot_save_info_ptr->file_handler)
	{
		BRWPL_CloseFile(g_snapshot_save_info_ptr->file_handler);
	}
	BRWMem_Free(g_snapshot_save_info_ptr);
	g_snapshot_save_info_ptr = 0;
}

LOCAL void BrwCallbackSaveResult(SFS_ERROR_E sfs_error)
{
	uint32 file_path_len = 0, folder_path_len = 0;
	uint16 *file_path_ptr=0;
	uint16 *folder_path_ptr = 0;
	
	if(0==g_snapshot_save_info_ptr)
	{
		return;
	}
	//get file path
	file_path_len = sizeof(uint16)*(BRW_WStrlen(g_snapshot_save_info_ptr->save_route_ptr)+
		BRW_WStrlen(g_snapshot_save_info_ptr->file_name_ptr)+
		strlen(BRW_FILE_SLASH)+strlen(SNAPSHOT_FILE_NAME_SUFFIX));
	file_path_ptr=BRWMem_Alloc(file_path_len+2);
	if(0!=file_path_ptr)
	{
		BRW_Memset(file_path_ptr,0,file_path_len+2);
		BRW_WstrCat(file_path_ptr,g_snapshot_save_info_ptr->save_route_ptr);
		BRWPL_WstrStrnCatWithAsciiStr(file_path_ptr,BRW_FILE_SLASH,file_path_len/2+1);
		BRW_WstrCat(file_path_ptr,g_snapshot_save_info_ptr->file_name_ptr);
		BRWPL_WstrStrnCatWithAsciiStr(file_path_ptr,SNAPSHOT_FILE_NAME_SUFFIX,file_path_len/2+1);
	}
	//get folder path
	folder_path_len = sizeof(uint16)*(BRW_WStrlen(g_snapshot_save_info_ptr->save_route_ptr)+
		BRW_WStrlen(g_snapshot_save_info_ptr->file_name_ptr)+strlen(BRW_FILE_SLASH));
	folder_path_ptr = BRWMem_Alloc(folder_path_len+2);
	if(0!=folder_path_ptr)
	{
		BRW_Memset(folder_path_ptr,0,folder_path_len+2);
		BRW_WstrCat(folder_path_ptr,g_snapshot_save_info_ptr->save_route_ptr);
		BRWPL_WstrStrnCatWithAsciiStr(folder_path_ptr,BRW_FILE_SLASH,folder_path_len/2+1);
		BRW_WstrCat(folder_path_ptr,g_snapshot_save_info_ptr->file_name_ptr);
	}

	if(SFS_ERROR_NONE == sfs_error)//save success, add the new file into snapshot list
	{
		if(0!=file_path_ptr)
		{
			BrwSnapshotInsertItem(file_path_ptr);
		}
	}
	else//save fail, delete the file and folder created
	{
		if(0!=folder_path_ptr&&BRWPL_IsFolderExist(folder_path_ptr,folder_path_len/sizeof(uint16)))
		{
			if(!BRWPL_DeleteDir(g_snapshot_save_info_ptr->save_route_ptr,g_snapshot_save_info_ptr->file_name_ptr))
			{
				//SCI_TRACE_LOW:"BRW BrwCallbackSaveResult, delete the folder fail when save is failed!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_459_112_2_18_2_7_26_797,(uint8*)"");
			}
		}
		if(0!=file_path_ptr&&BRWPL_IsFileExist(file_path_ptr, file_path_len/2))
		{
			if(!BRWPL_DelFileSyn(file_path_ptr, file_path_len/2))
			{
				//SCI_TRACE_LOW:"BRW BrwCallbackSaveResult, delete the file fail when save is failed!"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_466_112_2_18_2_7_26_798,(uint8*)"");
			}			
		}
	}
	if(0==g_snapshot_save_info_ptr->callback)
	{
		return;
	}
	(g_snapshot_save_info_ptr->callback)(sfs_error);
	BrwDestroySaveInfo();
	
	if(file_path_ptr!=0)
	{
		BRWMem_Free(file_path_ptr);
	}
	if(folder_path_ptr!=0)
	{
		BRWMem_Free(folder_path_ptr);
	}
}

LOCAL void BrwCallbackOpenResult(char *url_ptr,BOOLEAN is_succ)
{

	if(0!=g_snapshot_open_info.callback)
	{
		if(is_succ)
		{
			(g_snapshot_open_info.callback)(url_ptr, is_succ);
		}
		else
		{
			(g_snapshot_open_info.callback)(0, is_succ);
		}
	}
	//BrwDestroyOpenInfo();
}

PUBLIC void BRW_SnapshotHandleFfsMsg(uint32 msg_id, SFS_ERROR_E sfs_error)
{
    BOOLEAN is_succ = (SFS_ERROR_NONE == sfs_error)? TRUE: FALSE;

    //BRWDebug_Printf:"[BRW]BRW_SnapshotHandleFfsMsg,msg_id:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_506_112_2_18_2_7_26_799,(uint8*)"d",msg_id);
    
    if(msg_id==BRW_SNAPSHOT_FFS_PAGE_SAVE_CNF
        || msg_id==BRW_SNAPSHOT_FFS_CSS_SAVE_CNF
        || msg_id==BRW_SNAPSHOT_FFS_IMAGE_SAVE_CNF
        || msg_id==BRW_SNAPSHOT_FFS_IMAGE_SAVE_ONE_CNF)
    {
        if(PNULL == g_snapshot_save_info_ptr)
        {
            return;
        }
        if(0==g_snapshot_save_info_ptr->file_handler)
        {
            return;
        }
        BRWPL_CloseFile(g_snapshot_save_info_ptr->file_handler);
        g_snapshot_save_info_ptr->file_handler = 0;
    }
    else if(msg_id==BRW_SNAPSHOT_FFS_PAGE_OPEN_CNF 
        ||msg_id==BRW_SNAPSHOT_FFS_IMAGE_OPEN_ONE_CNF)
    {
        if(BRW_INVALID_INSTANCE == g_snapshot_open_info.browser_instance)//NEWMS00135583
        {
            return;
        }

        if(g_snapshot_open_info.file_handler)
        {
            BRWPL_CloseFile(g_snapshot_open_info.file_handler);
            g_snapshot_open_info.file_handler = 0;
        }
    }

    SCI_TRACE_LOW("BRW_SnapshotHandleFfsMsg:msg_id = 0x%x,sfs_error:%d", msg_id, sfs_error);

    switch(msg_id)
    {
    case BRW_SNAPSHOT_FFS_PAGE_SAVE_CNF:
        if(is_succ)
        {
            if(0!=g_snapshot_save_info_ptr->css_info.name_ptr)
            {
                //save css
                if(!BrwFfsWriteCss())
                {
                    BrwCallbackSaveResult(FALSE);
                }
            }
            else if(0!=g_snapshot_save_info_ptr->img_info.media_name_list.next)
            {
                //save img
                if(!BrwFfsWriteOneImage())
                {
                    BrwCallbackSaveResult(SFS_ERROR_NONE);
                }
            }
            else
            {
                BrwCallbackSaveResult(sfs_error);
            }
        }
        else
        {
            BrwCallbackSaveResult(sfs_error);
        }
        break;
        
    case BRW_SNAPSHOT_FFS_CSS_SAVE_CNF:
        if(is_succ)
        {
            if(0!=g_snapshot_save_info_ptr->img_info.media_name_list.next)
            {
                //save img
                if(!BrwFfsWriteOneImage())
                {
                    BrwCallbackSaveResult(SFS_ERROR_NONE);
                }
            }
            else
            {
                BrwCallbackSaveResult(sfs_error);
            }
        }
        else
        {
            BrwCallbackSaveResult(sfs_error);
        }
        break;
        
        case BRW_SNAPSHOT_FFS_IMAGE_SAVE_CNF:
            SCI_TRACE_LOW("BRW_SNAPSHOT_FFS_IMAGE_SAVE_CNF,sfs_error:%d", sfs_error);
            if(is_succ)
            {
                BrwCallbackSaveResult(sfs_error);
            }
            else
            {
                BrwCallbackSaveResult(sfs_error);
            }
            break;
            
        case BRW_SNAPSHOT_FFS_IMAGE_SAVE_ONE_CNF:
            if(is_succ)
            {
                //save img
                if(!BrwFfsWriteOneImage())
                {
                    BrwCallbackSaveResult(SFS_ERROR_NONE);
                }
            }
            else
            {
                BrwCallbackSaveResult(sfs_error);
            }
            break;
        
        case BRW_SNAPSHOT_FFS_PAGE_OPEN_CNF:
            if(is_succ)
            {
                uint32 head_len = 0;

                //@MS00192643:增加保护
                //SCI_TRACE_LOW:"BRW_SNAPSHOT_FFS_PAGE_OPEN_CNF: file_size = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_626_112_2_18_2_7_26_805,(uint8*)"d", g_snapshot_open_info.file_size);

                g_snapshot_open_info.page_mime_type = BrwGetPageMimeType(g_snapshot_open_info.file_buf_ptr);
                if ((g_snapshot_open_info.page_mime_type < BRW_MIME_HTML) || (g_snapshot_open_info.page_mime_type >= BRW_MIME_MAX))
                {
                    g_snapshot_open_info.page_mime_type = BRW_MIME_UNKOWN;
                    BrwCallbackOpenResult(g_snapshot_open_info.url_ptr, FALSE);//error
                    break;
                }

                g_snapshot_open_info.charset_ptr = BrwGetPageCharset(g_snapshot_open_info.file_buf_ptr);

                g_snapshot_open_info.url_ptr = BrwGetPageUrl(g_snapshot_open_info.file_buf_ptr);
                if (PNULL == g_snapshot_open_info.url_ptr)
                {
                    BrwCallbackOpenResult(g_snapshot_open_info.url_ptr, FALSE);
                    break;
                }
			
                g_snapshot_open_info.img_route_ptr = BrwGetPageImgRoute(g_snapshot_open_info.file_buf_ptr,g_snapshot_open_info.full_file_path_ptr);

                if (g_snapshot_open_info.file_buf_ptr != PNULL)
                {
                    head_len = *(g_snapshot_open_info.file_buf_ptr);
                }

                if(0==(g_snapshot_open_info.layout_handle=BrwParseOfflineWebPage(
                                g_snapshot_open_info.file_buf_ptr+head_len,
                                g_snapshot_open_info.file_size-head_len,
                                g_snapshot_open_info.charset_ptr,
                                g_snapshot_open_info.url_ptr,
                                g_snapshot_open_info.browser_instance,
                                g_snapshot_open_info.page_mime_type)))
                {
                    BrwCallbackOpenResult(g_snapshot_open_info.url_ptr, FALSE);
                }
                else
                {
                    BrwSetSnapshotOpenLayoutHandler(g_snapshot_open_info.browser_instance,g_snapshot_open_info.layout_handle);
                    if(g_snapshot_open_info.file_buf_ptr)
                    {
                        BRWMem_Free(g_snapshot_open_info.file_buf_ptr);
                        g_snapshot_open_info.file_buf_ptr = 0;
                        g_snapshot_open_info.file_size = 0;
                    }
                    //generate sub media list
                    if(BrwGenerateOpenMediaList())
                    {
                        //to open image
                        if(0!=g_snapshot_open_info.img_route_ptr)
                        {
                            if(!BrwOpenOneImage())
                            {
                                BrwCallbackOpenResult(g_snapshot_open_info.url_ptr, TRUE);
                            }
                        }
                        else//no image exists
                        {
                            BrwCallbackOpenResult(g_snapshot_open_info.url_ptr, TRUE);
                        }
                    }
                    else
                    {
                        //callback succ
                        BrwCallbackOpenResult(g_snapshot_open_info.url_ptr, TRUE);
                    }
                }
            }
            else
            {
                BrwCallbackOpenResult(g_snapshot_open_info.url_ptr, FALSE);
            }
            break;
            
        case BRW_SNAPSHOT_FFS_IMAGE_OPEN_ONE_CNF:
            if(is_succ)
            {
                //这里set img buffer的MIME_TYPE统一填BRW_MIME_IMG_JPEG，实际的类型在layout里根据数据头部自己判断
                uint32 index = BrwGetIndexFromMediaName(g_snapshot_open_info.open_media_list.next->object);

                if(index <= MAX_INDEX)
                {
                    BrwSetImgBuf(g_snapshot_open_info.file_buf_ptr,
                            g_snapshot_open_info.file_size,BRW_MIME_IMG_JPEG,
                            g_snapshot_open_info.layout_handle,index
                            );
                }
            }
            
            if(g_snapshot_open_info.file_buf_ptr)
            {
                //BRWMem_Free(g_snapshot_open_info.file_buf_ptr);这里的空间由layout自行free
                g_snapshot_open_info.file_buf_ptr = 0;
                g_snapshot_open_info.file_size = 0;
            }
            BRWMem_Free(g_snapshot_open_info.open_media_list.next->object);
            BRWList_RemoveObject(&g_snapshot_open_info.open_media_list,g_snapshot_open_info.open_media_list.next->object);
            if(!BrwOpenOneImage())
            {
                BrwCallbackOpenResult(g_snapshot_open_info.url_ptr, TRUE);
            }
            break;
            
        default:
            break;
	}
}

LOCAL BOOLEAN BrwGenerateOpenMediaList(void)
{
	uint8 *url_ptr = 0;
	char *name_ptr = 0;
	uint32 index = 0;
// 	BRW_MIME_TYPE_E mime_type = 0;
// 	uint8	*data_ptr=0;
// 	uint32	data_len = 0;
	char	temp_name[INDEX_NAME_MAX_LEN+2+1] = {0};
	uint16	media_path[BRW_FFS_MAX_PATH+1] = {0};
	BRWPL_FFS_FIND_DATA_T file_info = {0};
	BRWPL_FFS_HANDLE		find_handler = 0;
	BOOLEAN is_url = TRUE;

	if(g_snapshot_open_info.layout_handle==0)
	{
		return FALSE;
    }
    if (g_snapshot_open_info.page_mime_type == BRW_MIME_HTML
        || g_snapshot_open_info.page_mime_type == BRW_MIME_WML
        || g_snapshot_open_info.page_mime_type == BRW_MIME_WMLC)
    {
        BRWList_Construct(&g_snapshot_open_info.open_media_list);
        while(BrwGetImgUrl(g_snapshot_open_info.layout_handle, &url_ptr, index, &is_url))
        {
            if(url_ptr!=0 && index<MAX_INDEX && is_url)
            {
                BRW_Memset(temp_name, 0, sizeof(temp_name));
                _snprintf(temp_name,INDEX_NAME_MAX_LEN,"%d",index);
                BRW_Strcat((uint8*)temp_name,(const uint8*)".*");
                BRW_Memset(media_path, 0, sizeof(media_path));
                BRW_WstrCat(media_path,g_snapshot_open_info.img_route_ptr);
                BRWPL_WstrStrnCatWithAsciiStr(media_path,BRW_FILE_SLASH,BRW_FFS_MAX_PATH+1);
                BRWPL_WstrStrnCatWithAsciiStr(media_path,temp_name,BRW_FFS_MAX_PATH+1);
                
                find_handler = BRWPL_FfsFindFirst(media_path,&file_info);
                if(find_handler!=0)
                {
                    name_ptr = BRWMem_Alloc(BRW_WStrlen(file_info.name)+1);
                    if(name_ptr==0)
                    {
                        return FALSE;
                    }
                    BRW_Memset(name_ptr, 0, BRW_WStrlen(file_info.name)+1);
                    name_ptr = BRWPL_WstrToStr(file_info.name,name_ptr);
                    //SCI_TRACE_LOW:"brw snapshot open: %s"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_777_112_2_18_2_7_26_807,(uint8*)"s",name_ptr);
                    BRWList_AppendObject(&g_snapshot_open_info.open_media_list,name_ptr);
                    BRWPL_FindClose(find_handler);
                }
            }
            else if(!is_url)
            {
                index -- ;
            }
            index++;
        }
	}
	
	return TRUE;
}
LOCAL BOOLEAN BrwOpenOneImage(void)
{
	BRWPL_FFS_OVERLAPPED_T overlapped = {0};
	uint16 *file_path_ptr = 0;
	uint32 len = 0;

	if(0==g_snapshot_open_info.img_route_ptr||0==g_snapshot_open_info.open_media_list.next || 0 == g_snapshot_open_info.open_media_list.next->object)
	{
		return FALSE;
	}
	len = sizeof(uint16)*BRW_WStrlen(g_snapshot_open_info.img_route_ptr)+
		sizeof(uint16)*strlen(BRW_FILE_SLASH)+
		sizeof(uint16)*strlen(g_snapshot_open_info.open_media_list.next->object);
	file_path_ptr = BRWMem_Alloc(len+2);
	if(file_path_ptr==0)
	{
		return FALSE;
	}
	BRW_Memset(file_path_ptr,0,len+2);
	BRW_WstrCat(file_path_ptr,g_snapshot_open_info.img_route_ptr);
	BRWPL_WstrStrnCatWithAsciiStr(file_path_ptr,BRW_FILE_SLASH,len/2+1);
	BRWPL_WstrStrnCatWithAsciiStr(file_path_ptr,g_snapshot_open_info.open_media_list.next->object,len/2+1);

	while(!BRWPL_GetFileSize((const uint16*)file_path_ptr,len/sizeof(uint16),&g_snapshot_open_info.file_size))
	{	
		BRWMem_Free(g_snapshot_open_info.open_media_list.next->object);
		BRWList_RemoveObject(&g_snapshot_open_info.open_media_list,
						g_snapshot_open_info.open_media_list.next->object);/* parasoft-suppress BD-RES-INVFREE "有判断检查" */
		if(0==g_snapshot_open_info.open_media_list.next)/* parasoft-suppress BD-RES-INVFREE "有判断检查" */
		{
			return FALSE;
		}
	}
	g_snapshot_open_info.file_buf_ptr = BRWMem_Alloc(g_snapshot_open_info.file_size);
	if(0==g_snapshot_open_info.file_buf_ptr)
	{
		return FALSE;
	}
	overlapped.complete_route = BrwWriteOrReadWebPageCb;
	overlapped.param = BRW_SNAPSHOT_FFS_IMAGE_OPEN_ONE_CNF;
	g_snapshot_open_info.file_handler=BRWPL_ReadFileAsync(
														file_path_ptr,
														g_snapshot_open_info.file_buf_ptr,
														g_snapshot_open_info.file_size,&overlapped);
	BRWMem_Free(file_path_ptr);
	if(0==g_snapshot_open_info.file_handler)
	{
		return FALSE;
	}
	return TRUE;
}
LOCAL LAYOUT_HANDLE BrwParseOfflineWebPage(
										   uint8 *page_content_ptr,
										   uint32 page_len,
										   uint8 *charset_ptr,
										   char *url_ptr,
										   BRW_INSTANCE browser_instance,
										   BRW_MIME_TYPE_E mime_type)
{
	LAYOUT_HANDLE layout_handler = 0;
	BOOLEAN		result = FALSE;
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

	if(0==page_content_ptr||BRW_MIME_UNKOWN==mime_type||0==page_len||0==url_ptr)
	{
		return BRW_INVALID_LAYOUT_HANDLE;
	}
	layout_handler = BrwCreateLayoutCtrl(mime_type);
	if(layout_handler!=BRW_INVALID_LAYOUT_HANDLE)
	{
        PARSER_INPUT_INFO_T parser_input_info={0};  
        BRW_INFO_T *brw_info_ptr = BrwGetBrwInfoPtrByInstance(browser_instance);
        
        if(0==brw_info_ptr)
        {
            return BRW_INVALID_LAYOUT_HANDLE;
        }
        parser_input_info.charset_ptr=charset_ptr;
        parser_input_info.data_len = page_len;
        parser_input_info.data_ptr = page_content_ptr;
        parser_input_info.history_op_cbf =0;
        parser_input_info.req_id = browser_instance;
        if (!g_snapshot_open_info.is_fixed)
        {
            parser_input_info.start_new_pagereq_cbf = (BRW_START_PAGE_REQ_CBF)BrwStartNewPageReq;
        }
        else    
        {
            parser_input_info.start_new_pagereq_cbf = (BRW_START_PAGE_REQ_CBF)BrwStartNewInstance;
        }
        parser_input_info.url_ptr = (uint8*)url_ptr;
        parser_input_info.font_type = brw_info_ptr->profile.font_type;
        parser_input_info.is_show_image = public_profile_ptr->is_image_download;
        parser_input_info.is_play_bgsound = public_profile_ptr->is_bgsound_download;
        parser_input_info.screen_option = brw_info_ptr->profile.screen_opt;
        parser_input_info.prgbar_ptr = (void*)brw_info_ptr->profile.scroll_bar_handler;
        parser_input_info.prgbar_width = brw_info_ptr->profile.scroll_bar_width;

        if(mime_type ==BRW_MIME_WMLS || mime_type == BRW_MIME_WMLSC)
        {
            result = BrwProcWmlScript(&parser_input_info, layout_handler);
        }
        else
        {
			result = BrwParseWebPage(&parser_input_info,layout_handler);
		}
		if(!result)
		{
                    BrwDestroyLayoutCtrl(layout_handler);
                    //BRWDebug_Printf:"[BRW]BrwParseOfflineWebPage,BrwDestroyLayoutCtrl: 0x%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_900_112_2_18_2_7_26_808,(uint8*)"d", layout_handler);
                    layout_handler = BRW_INVALID_LAYOUT_HANDLE;
		}
	}
	return layout_handler;
}
LOCAL uint32 BrwGetIndexFromMediaName(char* name_ptr)
{
	char *ch = name_ptr;
	char index_str[INDEX_NAME_MAX_LEN+1] = {0};
	uint32 index = MAX_INDEX+1;
	uint32 i=0;

	if(0==name_ptr)
	{
		return index;
	}
	while(*ch!=0 && *ch!='.'&&i<INDEX_NAME_MAX_LEN)
	{
		index_str[i++]=*ch;
		ch++;
	}
	index = atoi(index_str);
	return index;
}
LOCAL SFS_ERROR_E BrwFfsWritePageContent(BRW_INFO_T *brw_info_ptr)
{
    uint16 *path_ptr = 0;
    uint32 len = 0, charset_len=0,url_len=0,img_foleder_name_len=0;
    BRWPL_FFS_OVERLAPPED_T overlapped = {0};
    BRW_SNAPSHOT_FILE_HEAD_T	*head_ptr = 0;
    SFS_ERROR_E sfs_error = SFS_ERROR_INVALID_PARAM;
    
    if(0==brw_info_ptr||0==g_snapshot_save_info_ptr)
    {
        return sfs_error;
    }
    if(0==brw_info_ptr->cur_page_content.page_content_ptr||0==brw_info_ptr->cur_page_content.page_content_len
        ||0==g_snapshot_save_info_ptr->file_name_ptr||0==g_snapshot_save_info_ptr->save_route_ptr)
    {
        return sfs_error;
    }

    len = sizeof(uint16)*BRW_WStrlen(g_snapshot_save_info_ptr->save_route_ptr)
            +sizeof(uint16)*BRW_WStrlen(g_snapshot_save_info_ptr->file_name_ptr)
            +sizeof(uint16)*BRW_Strlen((const uint8*)BRW_FILE_SLASH)
            +sizeof(uint16)*SNAPSHOT_FILE_NAME_SUFFIX_LEN;
    
    path_ptr = BRWMem_Alloc(len+2);
    if(PNULL == path_ptr)
    {
        return sfs_error;
    }
    BRW_Memset(path_ptr,0,len+2);

    BRW_WstrCat(path_ptr,g_snapshot_save_info_ptr->save_route_ptr);
    BRWPL_WstrStrnCatWithAsciiStr(path_ptr,BRW_FILE_SLASH,len/2+1);
    BRW_WstrCat(path_ptr,g_snapshot_save_info_ptr->file_name_ptr);
    BRWPL_WstrStrnCatWithAsciiStr(path_ptr,SNAPSHOT_FILE_NAME_SUFFIX,len/2+1);

    //write snapshot page head in file
    if(brw_info_ptr->cur_page_content.charset_ptr!=0)
    {
        charset_len = BRW_Strlen(brw_info_ptr->cur_page_content.charset_ptr);
    }
    else
    {
        charset_len = 0;
    }
    url_len = BRW_Strlen(brw_info_ptr->cur_display_req_ptr->absolute_url);
    img_foleder_name_len = sizeof(uint16)*BRW_WStrlen(g_snapshot_save_info_ptr->file_name_ptr);
    head_ptr = BRWMem_Alloc(sizeof(BRW_SNAPSHOT_FILE_HEAD_T)+charset_len+url_len+img_foleder_name_len);
    if(0==head_ptr)
    {
        BRWMem_Free(path_ptr);
        return sfs_error;
    }
    BRW_Memset(head_ptr,0,sizeof(BRW_SNAPSHOT_FILE_HEAD_T)+charset_len+url_len);
    head_ptr->head_len = sizeof(BRW_SNAPSHOT_FILE_HEAD_T)+charset_len+url_len+img_foleder_name_len;
    head_ptr->charset_len = charset_len;
    head_ptr->url_len = url_len;
    head_ptr->img_folder_name_len = img_foleder_name_len;
    head_ptr->page_mime_type = brw_info_ptr->cur_page_content.page_mime_type;
    if(brw_info_ptr->cur_page_content.charset_ptr!=0)
    {
        BRW_Memcpy((uint8*)head_ptr+sizeof(BRW_SNAPSHOT_FILE_HEAD_T),
        brw_info_ptr->cur_page_content.charset_ptr,charset_len);
    }
    BRW_Memcpy((uint8*)head_ptr+sizeof(BRW_SNAPSHOT_FILE_HEAD_T)+charset_len,
            brw_info_ptr->cur_display_req_ptr->absolute_url,
            url_len);
    BRW_Memcpy((uint8*)head_ptr+sizeof(BRW_SNAPSHOT_FILE_HEAD_T)+charset_len+url_len,
            g_snapshot_save_info_ptr->file_name_ptr,
            img_foleder_name_len);

    sfs_error = BRWPL_WriteFileSyn(path_ptr,len/sizeof(uint16),
                        (const uint8*)head_ptr,
                        sizeof(BRW_SNAPSHOT_FILE_HEAD_T)+charset_len+url_len+img_foleder_name_len);
    if (sfs_error != SFS_ERROR_NONE)
    {
        SCI_TRACE_LOW("[BRW]BrwFfsWritePageContent:11 sfs_error =%d", sfs_error);
        BRWMem_Free(path_ptr);
        BRWMem_Free(head_ptr);
        return sfs_error;
    }

    //write page content
    overlapped.complete_route = BrwWriteOrReadWebPageCb;
    overlapped.param = BRW_SNAPSHOT_FFS_PAGE_SAVE_CNF;	
    if(0==(g_snapshot_save_info_ptr->file_handler=
                    BRWPL_AppendWriteFileAsync(path_ptr,brw_info_ptr->cur_page_content.page_content_ptr,
                            brw_info_ptr->cur_page_content.page_content_len,
                            sizeof(BRW_SNAPSHOT_FILE_HEAD_T)+charset_len+url_len+img_foleder_name_len,
                            &overlapped, &sfs_error)))
    {
        SCI_TRACE_LOW("[BRW]BrwFfsWritePageContent:22 sfs_error =%d", sfs_error);
        BRWMem_Free(path_ptr);
        BRWMem_Free(head_ptr);
        return sfs_error;
    }
    BRWMem_Free(path_ptr);
    BRWMem_Free(head_ptr);
    return SFS_ERROR_NONE;
}
LOCAL BOOLEAN BrwFfsWriteOneImage(void)
{
	uint16 *path_ptr = 0;
	uint32 len = 0;
	BRWPL_FFS_OVERLAPPED_T overlapped = {0};
	uint8 *img_buf_ptr = 0;
	uint32 img_data_len = 0;
	BRW_MIME_TYPE_E mime_type = 0;
	uint32 index = 0;

	if(0==g_snapshot_save_info_ptr)
	{
		return FALSE;
	}
	if(0==g_snapshot_save_info_ptr->img_info.save_route_ptr||
		0==g_snapshot_save_info_ptr->img_info.media_name_list.next||
		0==g_snapshot_save_info_ptr->layout_handler)
	{
		return FALSE;
	}

	len = sizeof(uint16)*BRW_WStrlen(g_snapshot_save_info_ptr->img_info.save_route_ptr)+
			sizeof(uint16)*BRW_Strlen(g_snapshot_save_info_ptr->img_info.media_name_list.next->object)+
			sizeof(uint16)*BRW_Strlen((const uint8*)BRW_FILE_SLASH);
	path_ptr = BRWMem_Alloc(len+2);
	if(0==path_ptr)
	{
		BRWMem_Free(g_snapshot_save_info_ptr->img_info.media_name_list.next->object);
		BRWList_RemoveObject(&g_snapshot_save_info_ptr->img_info.media_name_list,
						g_snapshot_save_info_ptr->img_info.media_name_list.next->object);
		return FALSE;
	}
	BRW_Memset(path_ptr,0,len+2);

	BRW_WstrCat(path_ptr,g_snapshot_save_info_ptr->img_info.save_route_ptr);
	BRWPL_WstrStrnCatWithAsciiStr(path_ptr,BRW_FILE_SLASH,len/2+1);
	BRWPL_WstrStrnCatWithAsciiStr(path_ptr,g_snapshot_save_info_ptr->img_info.media_name_list.next->object,len/2+1);

	//get image data
	index = BrwGetIndexFromMediaName(g_snapshot_save_info_ptr->img_info.media_name_list.next->object);
	if(index>MAX_INDEX)
	{
		BRWMem_Free(g_snapshot_save_info_ptr->img_info.media_name_list.next->object);
		BRWList_RemoveObject(&g_snapshot_save_info_ptr->img_info.media_name_list,
						g_snapshot_save_info_ptr->img_info.media_name_list.next->object);
		BRWMem_Free(path_ptr);
		return FALSE;
	}
	BrwGetImgBuf(&img_buf_ptr,&img_data_len,&mime_type,g_snapshot_save_info_ptr->layout_handler,index);
	if(0==img_buf_ptr||0==img_data_len)
	{
		BRWMem_Free(g_snapshot_save_info_ptr->img_info.media_name_list.next->object);
		BRWList_RemoveObject(&g_snapshot_save_info_ptr->img_info.media_name_list,
						g_snapshot_save_info_ptr->img_info.media_name_list.next->object);
		BRWMem_Free(path_ptr);
		return FALSE;
	}
	
	overlapped.complete_route = BrwWriteOrReadWebPageCb;
	if(1<BRWList_GetListCount(&g_snapshot_save_info_ptr->img_info.media_name_list))
	{
		overlapped.param = BRW_SNAPSHOT_FFS_IMAGE_SAVE_ONE_CNF;
	}
	else
	{
		overlapped.param = BRW_SNAPSHOT_FFS_IMAGE_SAVE_CNF;
	}
	
	if(0==(g_snapshot_save_info_ptr->file_handler=
						BRWPL_WriteFileAsync(path_ptr,img_buf_ptr,
												img_data_len,&overlapped)))
	{
		BRWMem_Free(g_snapshot_save_info_ptr->img_info.media_name_list.next->object);
		BRWList_RemoveObject(&g_snapshot_save_info_ptr->img_info.media_name_list,
						g_snapshot_save_info_ptr->img_info.media_name_list.next->object);
		BRWMem_Free(path_ptr);
		return FALSE;
	}
	BRWMem_Free(g_snapshot_save_info_ptr->img_info.media_name_list.next->object);
	BRWList_RemoveObject(&g_snapshot_save_info_ptr->img_info.media_name_list,
					g_snapshot_save_info_ptr->img_info.media_name_list.next->object);
	BRWMem_Free(path_ptr);
	return TRUE;

}

LOCAL BOOLEAN BrwFfsWriteCss(void)
{
	uint16 *path_ptr = 0;
	uint32 len = 0;
	BRWPL_FFS_OVERLAPPED_T overlapped = {0};
// 	uint8 *buf_ptr = 0;

	if(0==g_snapshot_save_info_ptr)
	{
		return FALSE;
	}
	if(0==g_snapshot_save_info_ptr->css_info.name_ptr||0==g_snapshot_save_info_ptr->css_info.save_route_ptr)
	{
		return FALSE;
	}

	len = sizeof(uint16)*BRW_WStrlen(g_snapshot_save_info_ptr->css_info.save_route_ptr)+
			sizeof(uint16)*BRW_Strlen((const uint8*)g_snapshot_save_info_ptr->css_info.name_ptr)+
			sizeof(uint16)*BRW_Strlen((const uint8*)BRW_FILE_SLASH);
	path_ptr = BRWMem_Alloc(len+2);
	if(0==path_ptr)
	{
		return FALSE;
	}
	BRW_Memset(path_ptr,0,len+2);
	
	BRW_WstrCat(path_ptr,g_snapshot_save_info_ptr->css_info.save_route_ptr);
	BRWPL_WstrStrnCatWithAsciiStr(path_ptr,BRW_FILE_SLASH,len/2+1);
	BRWPL_WstrStrnCatWithAsciiStr(path_ptr,g_snapshot_save_info_ptr->css_info.name_ptr,len/2+1);

	overlapped.complete_route = BrwWriteOrReadWebPageCb;
	overlapped.param = BRW_SNAPSHOT_FFS_CSS_SAVE_CNF;
	
//	if(!BRWPL_WriteFileAsync(path_ptr,brw_info_ptr->cur_page_content_ptr,
//			brw_info_ptr->cur_page_content_len,&overlapped))
//	{
//		BRWMem_Free(path_ptr);
//		return FALSE;
//	}
	BRWMem_Free(path_ptr);
	return TRUE;
 }

//return val: 1--file exist; 2--folder exist; 0--either file or folder is NOT exist
PUBLIC uint32 BRW_SnapshotCheckIfNameExist(
											const uint16 *save_route_ptr, //[in]
											const uint16 *file_name_ptr//[in]file name without suffix
											)
{
	uint16 *media_path_ptr = 0;
	uint16 *page_path_ptr = 0;
	uint32 len = 0;
	uint32 ret_val = 0;

	if(0==save_route_ptr || 0==file_name_ptr)
	{
		return 1;
	}
	len = (BRW_WStrlen(save_route_ptr)+BRW_WStrlen(file_name_ptr)+BRW_Strlen((const uint8*)BRW_FILE_SLASH))*sizeof(uint16);
	media_path_ptr = BRWMem_Alloc(len+2);
	if(0==media_path_ptr)
	{
		return 1;
	}
	BRW_Memset(media_path_ptr,0,len+2);
	BRW_WstrCat(media_path_ptr,save_route_ptr);
	BRWPL_WstrStrnCatWithAsciiStr(media_path_ptr,BRW_FILE_SLASH,len/2+1);
	BRW_WstrCat(media_path_ptr,file_name_ptr);

	len = (BRW_WStrlen(media_path_ptr)+BRW_Strlen((const uint8*)SNAPSHOT_FILE_NAME_SUFFIX))*sizeof(uint16);
	page_path_ptr = BRWMem_Alloc(len+2);
	if(0==page_path_ptr)
	{
		BRWMem_Free(media_path_ptr);
		return 1;
	}
	BRW_Memset(page_path_ptr,0,len+2);
	BRW_WstrCat(page_path_ptr,media_path_ptr);
	BRWPL_WstrStrnCatWithAsciiStr(page_path_ptr,SNAPSHOT_FILE_NAME_SUFFIX,len/2+1);

	if(BRWPL_IsFolderExist(media_path_ptr,BRW_WStrlen(media_path_ptr)))
	{
		ret_val = 2;
	}
	if(BRWPL_IsFileExist(page_path_ptr,BRW_WStrlen(page_path_ptr)))
	{
		ret_val = 1;
	}
	return ret_val;
}
PUBLIC SFS_ERROR_E BRW_SnapshotSaveCurWebPage(
												   BRW_INSTANCE	instance,
												   const uint16 *save_route_ptr, //[in]
												   const uint16 *file_name_ptr,//[in]file name without suffix
												   BRW_SNAPSHOT_SAVE_CALLBACK callback
												   )
{
    BRW_INFO_T *brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    LAYOUT_HANDLE layout_handle = 0;
    uint32		len = 0;
    SFS_ERROR_E sfs_error = SFS_ERROR_INVALID_PARAM;

    if(0==instance||0==save_route_ptr||0==file_name_ptr||g_snapshot_save_info_ptr!=0||0==brw_info_ptr)
    {
        return sfs_error;
    }
    if(0 == brw_info_ptr->cur_display_req_ptr)
    {
        return sfs_error;
    }
    if(0 == brw_info_ptr->cur_display_req_ptr->layout_handle)
    {
        return sfs_error;
    }
    layout_handle = brw_info_ptr->cur_display_req_ptr->layout_handle;
    g_snapshot_save_info_ptr = BRWMem_Alloc(sizeof(BRW_SNAPSHOT_SAVE_T));
    if(0==g_snapshot_save_info_ptr)
    {
        return sfs_error;
    }
    BRW_Memset(g_snapshot_save_info_ptr,0,sizeof(BRW_SNAPSHOT_SAVE_T));
    g_snapshot_save_info_ptr->layout_handler = layout_handle;
    g_snapshot_save_info_ptr->callback = callback;

    len = sizeof(uint16)*BRW_WStrlen(file_name_ptr);
    g_snapshot_save_info_ptr->file_name_ptr = BRWMem_Alloc(len+2);
    if(0==g_snapshot_save_info_ptr->file_name_ptr)
    {
        BrwDestroySaveInfo();
        return sfs_error;
    }
    BRW_Memset(g_snapshot_save_info_ptr->file_name_ptr,0,len+2);
    BRW_Memcpy(g_snapshot_save_info_ptr->file_name_ptr,file_name_ptr,len+2);

    len = sizeof(uint16)*BRW_WStrlen(save_route_ptr);
    g_snapshot_save_info_ptr->save_route_ptr = BRWMem_Alloc(len+2);
    if(0==g_snapshot_save_info_ptr->save_route_ptr)
    {
        BrwDestroySaveInfo();
        return sfs_error;
    }
    BRW_Memset(g_snapshot_save_info_ptr->save_route_ptr,0,len+2);
    BRW_Memcpy(g_snapshot_save_info_ptr->save_route_ptr,save_route_ptr,len+2);

    if(!BrwGenerateCssInfo(&g_snapshot_save_info_ptr->css_info,layout_handle))
    {
        //SCI_TRACE_LOW:"BRW_SaveCurWebPage,BrwGenerateCssInfo fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1249_112_2_18_2_7_27_809,(uint8*)"");
        BrwDestroySaveInfo();
        return sfs_error;
        }
    if (brw_info_ptr->cur_page_content.page_mime_type == BRW_MIME_HTML
        || brw_info_ptr->cur_page_content.page_mime_type == BRW_MIME_WML
        || brw_info_ptr->cur_page_content.page_mime_type == BRW_MIME_WMLC)
    {
        if(!BrwGenerateImgInfo(&g_snapshot_save_info_ptr->img_info.media_name_list,layout_handle))
        {
            //SCI_TRACE_LOW:"BRW_SaveCurWebPage,BrwGenerateImgInfo fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1255_112_2_18_2_7_27_810,(uint8*)"");
            BrwDestroySaveInfo();
            return sfs_error;
        }
    }
    
    if(!BrwSnapshotGenerateAllFileFolder(g_snapshot_save_info_ptr))
    {
        //SCI_TRACE_LOW:"BRW_SaveCurWebPage,BrwSnapshotGenerateAllFileFolder fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1261_112_2_18_2_7_27_811,(uint8*)"");
        BrwDestroySaveInfo();
        return sfs_error;
    }
    
    sfs_error = BrwFfsWritePageContent(brw_info_ptr);
    if (sfs_error != SFS_ERROR_NONE)
    {		
        //SCI_TRACE_LOW:"BRW_SaveCurWebPage,BrwFfsWritePageContent fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1267_112_2_18_2_7_27_812,(uint8*)"");
        BrwDestroySaveInfo();
        return sfs_error;
    }
    return SFS_ERROR_NONE;
}

LOCAL BOOLEAN BrwSnapshotDeleteWebPage(												   
										 const uint16 *file_route_ptr
											)
{
	uint16 *folder_name_ptr=0, *folder_path_ptr = 0;
	uint32 len = 0;
// 	BRWPL_FFS_HANDLE ffs_handler = 0;
	uint16 *folder_route_ptr=0;
	uint8 *file_head_buf_ptr = 0;
	uint32	file_head_len = 0;
	BRW_SNAPSHOT_FILE_HEAD_T	file_head = {0};
	
	if(0==file_route_ptr)
	{
		return FALSE;
	}
	
	//find the webpage media folder
	if(!BRWPL_ReadFileSyn(file_route_ptr,BRW_WStrlen(file_route_ptr),(uint8*)&file_head_len,sizeof(uint32),&len))
	{
		//BRWDebug_Printf:"BrwSnapshotDeleteWebPage, read file head len error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1294_112_2_18_2_7_27_813,(uint8*)"");
		return FALSE;
	}
	if(file_head_len==0)
	{
		//BRWDebug_Printf:"BrwSnapshotDeleteWebPage, error file_head_len==0"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1299_112_2_18_2_7_27_814,(uint8*)"");
		return FALSE;
	}
	file_head_buf_ptr = BRWMem_Alloc(file_head_len+2);
	if(0==file_head_buf_ptr)
	{
		//BRWDebug_Printf:"BrwSnapshotDeleteWebPage, file_head_buf_ptr alloc error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1305_112_2_18_2_7_27_815,(uint8*)"");
		return FALSE;		
	}
	BRW_Memset(file_head_buf_ptr, 0, file_head_len+2);
	if(!BRWPL_ReadFileSyn(file_route_ptr,BRW_WStrlen(file_route_ptr),file_head_buf_ptr,file_head_len,&len))
	{	
		//BRWDebug_Printf:"BrwSnapshotDeleteWebPage, read file head error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1311_112_2_18_2_7_27_816,(uint8*)"");
		return FALSE;		
	}
	BRW_Memcpy(&file_head, file_head_buf_ptr, sizeof(BRW_SNAPSHOT_FILE_HEAD_T));

	if(file_head.img_folder_name_len!=0)
	{
		folder_name_ptr = (uint16*)BRWMem_Alloc(file_head.img_folder_name_len+2);
		if(folder_name_ptr==0)
		{
			goto error;
		}
		BRW_Memset(folder_name_ptr, 0, file_head.img_folder_name_len+2);
		BRW_Memcpy(folder_name_ptr,file_head_buf_ptr+
										sizeof(BRW_SNAPSHOT_FILE_HEAD_T)+
										file_head.charset_len+file_head.url_len,
										file_head.img_folder_name_len);
		folder_route_ptr = BRWMem_Alloc(SNAPSHOT_FOLDER_ROUTE_LEN*sizeof(uint16)+2);
		if(0==folder_route_ptr)
		{
			goto error;
		}
		BRW_Memset(folder_route_ptr,0,SNAPSHOT_FOLDER_ROUTE_LEN*sizeof(uint16)+2);
		BRW_Memcpy(folder_route_ptr,file_route_ptr,SNAPSHOT_FOLDER_ROUTE_LEN*sizeof(uint16));
		len = SNAPSHOT_FOLDER_ROUTE_LEN*sizeof(uint16)+
											BRW_WStrlen(folder_name_ptr)*sizeof(uint16)+
											strlen(BRW_FILE_SLASH)*sizeof(uint16)+2;
		folder_path_ptr = BRWMem_Alloc(len);
		if(0==folder_path_ptr)
		{
			goto error;
		}
		BRW_Memset(folder_path_ptr,0,len);
		BRW_WstrCat(folder_path_ptr,folder_route_ptr);
		BRWPL_WstrStrnCatWithAsciiStr(folder_path_ptr,BRW_FILE_SLASH,len/2);
		BRW_WstrCat(folder_path_ptr,folder_name_ptr);
		//delete the folder
		if(BRWPL_IsFolderExist(folder_path_ptr,len/sizeof(uint16)))
		{
			if(!BRWPL_DeleteDir(folder_route_ptr,folder_name_ptr))
			{
				goto error;
			}
		}
	}
	else
	{
		//BRWDebug_Printf:"BrwSnapshotDeleteWebPage, no image folder"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1358_112_2_18_2_7_27_817,(uint8*)"");
	}
	//delete the file
	if(!BRWPL_DelFileSyn(file_route_ptr,BRW_WStrlen(file_route_ptr)))
	{
		goto error;
	}
	BRWMem_Free(folder_path_ptr);
	folder_path_ptr=0;

	BRWMem_Free(folder_route_ptr);
	folder_route_ptr=0;

	BRWMem_Free(file_head_buf_ptr);
	file_head_buf_ptr=0;
	return TRUE;
error:
	if(folder_path_ptr)
	{
		BRWMem_Free(folder_path_ptr);
		folder_path_ptr=0;
	}
	if(folder_route_ptr)
	{
		BRWMem_Free(folder_route_ptr);
		folder_route_ptr=0;
	}
	if(folder_name_ptr)
	{
		BRWMem_Free(folder_name_ptr);
		folder_name_ptr = 0;
	}
	BRWMem_Free(file_head_buf_ptr);
	file_head_buf_ptr=0;
	return FALSE;
}

LOCAL void BrwDestroyOpenInfo(void)
{
	if(g_snapshot_open_info.file_buf_ptr)
	{
		BRWMem_Free(g_snapshot_open_info.file_buf_ptr);
		g_snapshot_open_info.file_buf_ptr = 0;
	}
	if(g_snapshot_open_info.img_route_ptr)
	{
		BRWMem_Free(g_snapshot_open_info.img_route_ptr);
		g_snapshot_open_info.img_route_ptr = 0;
	}
	if(g_snapshot_open_info.full_file_path_ptr)
	{
		BRWMem_Free(g_snapshot_open_info.full_file_path_ptr);
		g_snapshot_open_info.full_file_path_ptr=0;
	}
	if(g_snapshot_open_info.file_handler)
	{
		BRWPL_CloseFile(g_snapshot_open_info.file_handler);
		g_snapshot_open_info.file_handler = 0;
	}
	if(g_snapshot_open_info.charset_ptr)
	{
		BRWMem_Free(g_snapshot_open_info.charset_ptr);
		g_snapshot_open_info.charset_ptr = 0;
	}

    if(!g_snapshot_open_info.is_fixed)
    {
    	if(g_snapshot_open_info.layout_handle)
    	{
                BrwDestroyLayoutCtrl(g_snapshot_open_info.layout_handle);
                //BRWDebug_Printf:"[BRW]BrwDestroyOpenInfo,BrwDestroyLayoutCtrl: 0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1428_112_2_18_2_7_28_818,(uint8*)"d", g_snapshot_open_info.layout_handle);
                g_snapshot_open_info.layout_handle = BRW_INVALID_LAYOUT_HANDLE;
    	}
    
    	BrwSetSnapshotOpenLayoutHandler(g_snapshot_open_info.browser_instance,0);
    }
    
	if(g_snapshot_open_info.open_media_list.next)
	{
		BRWList_Destruct(&g_snapshot_open_info.open_media_list,BrwSnapshotMediaListFreeFunc);
	}
	BRW_Memset(&g_snapshot_open_info,0,sizeof(BRW_SNAPSHOT_OPEN_T));
}
LOCAL BRW_MIME_TYPE_E BrwGetPageMimeType(uint8 *file_buf_ptr)
{
// 	uint32 read_len = 0;
	uint32	type = 0; 
	if(0 == file_buf_ptr)
	{
		return 0;
	}
	
	BRW_Memcpy(&type,file_buf_ptr+sizeof(uint32),sizeof(uint32));
	
    //SCI_TRACE_LOW:"BrwGetPageMimeType: type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1452_112_2_18_2_7_28_819,(uint8*)"d", type);

	return type;
}
LOCAL uint16* BrwGetPageImgRoute(uint8 *file_buf_ptr, uint16 *full_file_path_ptr)
{
    uint16 *img_route_ptr=0;
    uint32 img_folder_size = 0;
    uint32 url_size=0;
    uint32 charset_size=0;
    uint32 img_folder_route_len = 0;

    if ((0 == file_buf_ptr) || (0 == full_file_path_ptr))
    {
        return 0;
    }

    BRW_Memcpy(&charset_size,file_buf_ptr+sizeof(uint32)+sizeof(uint32),sizeof(uint32));
    BRW_Memcpy(&url_size,file_buf_ptr+sizeof(uint32)+sizeof(uint32)+sizeof(uint32),sizeof(uint32));
    BRW_Memcpy(&img_folder_size,file_buf_ptr+sizeof(uint32)+sizeof(uint32)+sizeof(uint32)+sizeof(uint32),sizeof(uint32));

    if(img_folder_size!=0)
    {
        uint16  path[MMIFMM_PATHNAME_LEN + 1] = {0};
        uint16  path_len = MMIFMM_PATHNAME_LEN;

        /*path from MMIAPIFMM_SplitFullPathExt is with slash in the end*/
        MMIAPIFMM_SplitFullPathExt(
            full_file_path_ptr, BRW_WStrlen(full_file_path_ptr),
            path, &path_len,
            PNULL, PNULL,
            PNULL, PNULL
            );
        
        /*例:文件名为e:\m\1.html,则image full route: e:\m\img_folder\IMAGE*/
        img_folder_route_len = path_len + (img_folder_size/2) + 1 + BRW_Strlen((const uint8*)IMAGE_FOLDER_NAME);
        img_folder_route_len += 1;//with end flag
        img_route_ptr = BRWMem_Alloc(sizeof(uint16) * img_folder_route_len);
        if(PNULL != img_route_ptr)
        {
            BRW_Memset(img_route_ptr,0, (sizeof(uint16) * img_folder_route_len));
            
            BRW_WstrCat(img_route_ptr, path);
            BRW_Memcpy((uint8*)(img_route_ptr + path_len),
                file_buf_ptr+sizeof(BRW_SNAPSHOT_FILE_HEAD_T)+charset_size+url_size,  
                img_folder_size);
            BRWPL_WstrStrnCatWithAsciiStr(img_route_ptr,BRW_FILE_SLASH,img_folder_route_len);
            BRWPL_WstrStrnCatWithAsciiStr(img_route_ptr,IMAGE_FOLDER_NAME,img_folder_route_len);
        }
    }
	return img_route_ptr;
}
LOCAL char* BrwGetPageUrl(uint8 *file_buf_ptr)
{
	uint8 *url_ptr = 0;
	uint32 url_len = 0;
	uint32 charset_len = 0;

	if(0==file_buf_ptr)
	{
		return 0;
	}
	BRW_Memcpy(&charset_len,file_buf_ptr+sizeof(uint32)+sizeof(uint32),sizeof(uint32));
	//SCI_TRACE_LOW:"BrwGetPageUrl: charset_len = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1510_112_2_18_2_7_28_820,(uint8*)"d", charset_len);
	if (charset_len >= (g_snapshot_open_info.file_size - sizeof(BRW_SNAPSHOT_FILE_HEAD_T)))//without url
	{
		return 0;//error
	}
	BRW_Memcpy(&url_len,file_buf_ptr+sizeof(uint32)+sizeof(uint32)+sizeof(uint32),sizeof(uint32));
	//SCI_TRACE_LOW:"BrwGetPageUrl: url_len = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1516_112_2_18_2_7_28_821,(uint8*)"d", url_len);
	if((url_len > 0) && (url_len <= (g_snapshot_open_info.file_size - sizeof(BRW_SNAPSHOT_FILE_HEAD_T) - charset_len)))
	{
		url_ptr = BRWMem_Alloc(url_len+1);
		if(0!=url_ptr)
		{
			BRW_Memset(url_ptr,0,url_len+1);
			BRW_Memcpy(url_ptr,file_buf_ptr+sizeof(BRW_SNAPSHOT_FILE_HEAD_T)+charset_len,url_len);
		}
	}
	return (char*)url_ptr;
}
LOCAL uint8* BrwGetPageCharset(uint8 *file_buf_ptr)
{
	uint8 *charset_ptr = 0;
	uint32 charset_len = 0;

	if(0==file_buf_ptr)
	{
		return 0;
	}
	BRW_Memcpy(&charset_len,file_buf_ptr+sizeof(uint32)+sizeof(uint32),sizeof(uint32));
	//SCI_TRACE_LOW:"BrwGetPageCharset: charset_len = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1538_112_2_18_2_7_28_822,(uint8*)"d", charset_len);
	if ((charset_len > 0) && (charset_len <= (g_snapshot_open_info.file_size - sizeof(BRW_SNAPSHOT_FILE_HEAD_T))))
	{
		charset_ptr = BRWMem_Alloc(charset_len+1);
		if(0!=charset_ptr)
		{
			BRW_Memset(charset_ptr,0,charset_len+1);
			BRW_Memcpy(charset_ptr,file_buf_ptr+sizeof(BRW_SNAPSHOT_FILE_HEAD_T),charset_len);
		}
	}
	return charset_ptr;
}
PUBLIC BOOLEAN BRW_SnapshotParseOfflinePage(
										 const uint16 *file_path_ptr, //[IN]file path including route and file name
										 BRW_INSTANCE browser_instance,
										 BRW_SNAPSHOT_OPEN_CALLBACK callback
										 )
{
	uint32 file_size = 0;
	BRWPL_FFS_HANDLE ffs_handler = 0;
	BRWPL_FFS_OVERLAPPED_T overlapped = {0};
	uint32 len=0;
//	uint16 *img_route_ptr = 0;
	BRW_INFO_T* brw_info_ptr=0;

	if((PNULL == file_path_ptr)||(PNULL == callback)||(0 == browser_instance))
	{
		return FALSE;
	}
	
	BrwDestroyOpenInfo();

	//stop the page request
	brw_info_ptr = BrwGetBrwInfoPtrByInstance(browser_instance);
	if(PNULL == brw_info_ptr)
	{
		return FALSE;
	}
	if(brw_info_ptr->cur_req_ptr!=0 && brw_info_ptr->cur_req_ptr!=brw_info_ptr->cur_display_req_ptr)
	{
		BRW_StopPageReq(browser_instance);
	}
    
    //g_snapshot_open_info.is_fixed = is_fixed;
	g_snapshot_open_info.callback = callback;
	g_snapshot_open_info.browser_instance = browser_instance;

	len =sizeof(uint16)*BRW_WStrlen(file_path_ptr);
	g_snapshot_open_info.full_file_path_ptr = BRWMem_Alloc(len+2);
	if(0==g_snapshot_open_info.full_file_path_ptr)
	{
		//SCI_TRACE_LOW:"BRW_SnapshotParseOfflinePage no mem,g_snapshot_open_info.full_file_path_ptr"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1589_112_2_18_2_7_28_823,(uint8*)"");
		return FALSE;
	}
	BRW_Memset(g_snapshot_open_info.full_file_path_ptr,0,len+2);
	BRW_WstrCat(g_snapshot_open_info.full_file_path_ptr,file_path_ptr);
//	//check if image exist
//	len = sizeof(uint16)*BRW_WStrlen(file_path_ptr)-
//		sizeof(uint16)*strlen(SNAPSHOT_FILE_NAME_SUFFIX)+
//		sizeof(uint16)*strlen(BRW_FILE_SLASH)+
//		sizeof(uint16)*strlen(IMAGE_FOLDER_NAME);
//	img_route_ptr = BRWMem_Alloc(len+2);
//	if(0==img_route_ptr)
//	{
//		SCI_TRACE_LOW("BRW_SnapshotOfflineBrowse,ERROR:img_route=0");
//		return FALSE;
//	}
//	BRW_Memset(img_route_ptr,0,len+2);
//	BRW_Memcpy(img_route_ptr,file_path_ptr,
//		sizeof(uint16)*BRW_WStrlen(file_path_ptr)-sizeof(uint16)*strlen(SNAPSHOT_FILE_NAME_SUFFIX));
//	BRWPL_WstrStrCatWithAsciiStr(img_route_ptr,BRW_FILE_SLASH);
//	BRWPL_WstrStrCatWithAsciiStr(img_route_ptr,IMAGE_FOLDER_NAME);
//	if(BRWPL_IsFolderExist(img_route_ptr,len/sizeof(uint16)))
//	{
//		g_snapshot_open_info.img_route_ptr = img_route_ptr;
//	}
//	else
//	{
//		BRWMem_Free(img_route_ptr);
//	}

	//open webpage file
	BRWPL_GetFileSize(file_path_ptr,BRW_WStrlen(file_path_ptr),&file_size);
	if(0==file_size)
	{
		//SCI_TRACE_LOW:"BRW_SnapshotOfflineBrowse,ERROR:file size=0"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1623_112_2_18_2_7_28_824,(uint8*)"");
		BrwDestroyOpenInfo();
		return FALSE;
	}
	g_snapshot_open_info.file_size = file_size;
	g_snapshot_open_info.file_buf_ptr = BRWMem_Alloc(file_size);
	if(0==g_snapshot_open_info.file_buf_ptr)
	{
		//SCI_TRACE_LOW:"BRW_SnapshotOfflineBrowse,ERROR:alloc file buffer fail"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1631_112_2_18_2_7_28_825,(uint8*)"");
		BrwDestroyOpenInfo();
		return FALSE;
	}

	overlapped.complete_route = BrwWriteOrReadWebPageCb;
	overlapped.param = BRW_SNAPSHOT_FFS_PAGE_OPEN_CNF;
	ffs_handler = BRWPL_ReadFileAsync(file_path_ptr,g_snapshot_open_info.file_buf_ptr,file_size,&overlapped);
	if(0==ffs_handler)
	{
		//SCI_TRACE_LOW:"BRW_SnapshotOfflineBrowse,ERROR:BRWPL_ReadFileAsync fail"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1641_112_2_18_2_7_28_826,(uint8*)"");
		BrwDestroyOpenInfo();
		return FALSE;
	}
	g_snapshot_open_info.file_handler = ffs_handler;
	return TRUE;
}

PUBLIC BOOLEAN BRW_SnapshotParseOfflinePageEx(
            	uint8 *file_buf_ptr,
            	uint32 file_size,
                BRW_INSTANCE browser_instance,
                BRW_SNAPSHOT_OPEN_CALLBACK callback, 
                BOOLEAN is_fixed
                )
{
	uint32 head_len = 0;
	uint32 charset_len = 0;
	uint32 url_len = 0;
	BRW_INFO_T* brw_info_ptr=0;
    BOOLEAN result = FALSE;

	if((PNULL == file_buf_ptr) || (0 == file_size)||(PNULL == callback)||(0 == browser_instance))
	{
		return FALSE;
	}
	
	BrwDestroyOpenInfo();

	//stop the page request
	brw_info_ptr = BrwGetBrwInfoPtrByInstance(browser_instance);
	if(PNULL == brw_info_ptr)
	{
		return FALSE;
	}
	if(brw_info_ptr->cur_req_ptr!=0 && brw_info_ptr->cur_req_ptr!=brw_info_ptr->cur_display_req_ptr)
	{
		BRW_StopPageReq(browser_instance);
	}
    
    g_snapshot_open_info.is_fixed = is_fixed;
	//g_snapshot_open_info.callback = callback;
	g_snapshot_open_info.browser_instance = browser_instance;

    g_snapshot_open_info.file_size = file_size;
	g_snapshot_open_info.file_buf_ptr = file_buf_ptr;
    /*
	g_snapshot_open_info.file_buf_ptr = BRWMem_Alloc(file_size);
	if(PNULL == g_snapshot_open_info.file_buf_ptr)
	{
		//SCI_TRACE_LOW:"BRW_SnapshotOfflineBrowse,ERROR:alloc file buffer fail"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1692_112_2_18_2_7_28_827,(uint8*)"");
		BrwDestroyOpenInfo();
		return FALSE;
	}
    else
    {
        BRW_Memcpy(g_snapshot_open_info.file_buf_ptr, file_buf_ptr, file_size);
    }
    */
    do
    {
        //SCI_TRACE_LOW:"BRW_SNAPSHOT_FFS_PAGE_OPEN_CNF: file_size = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_1703_112_2_18_2_7_28_828,(uint8*)"d", g_snapshot_open_info.file_size);

    	g_snapshot_open_info.page_mime_type = BrwGetPageMimeType(g_snapshot_open_info.file_buf_ptr);
    	if ((g_snapshot_open_info.page_mime_type < BRW_MIME_HTML) || (g_snapshot_open_info.page_mime_type >= BRW_MIME_MAX))
    	{
            g_snapshot_open_info.page_mime_type = BRW_MIME_UNKOWN;
            break;
    	}
    	
    	g_snapshot_open_info.charset_ptr = BrwGetPageCharset(g_snapshot_open_info.file_buf_ptr);
    	
    	g_snapshot_open_info.url_ptr = BrwGetPageUrl(g_snapshot_open_info.file_buf_ptr);
    	if (PNULL == g_snapshot_open_info.url_ptr)
    	{
    		break;
    	}
    	
    	g_snapshot_open_info.img_route_ptr = BrwGetPageImgRoute(g_snapshot_open_info.file_buf_ptr,g_snapshot_open_info.full_file_path_ptr);

    	if(g_snapshot_open_info.charset_ptr)
    	{
    		charset_len = BRW_Strlen(g_snapshot_open_info.charset_ptr);
    	}
    	if(g_snapshot_open_info.url_ptr)
    	{
    		url_len = strlen(g_snapshot_open_info.url_ptr);
    	}
    	head_len = sizeof(BRW_SNAPSHOT_FILE_HEAD_T)+charset_len+url_len;
    	
    	if(0==(g_snapshot_open_info.layout_handle=BrwParseOfflineWebPage(
                    g_snapshot_open_info.file_buf_ptr+head_len,
                    g_snapshot_open_info.file_size-head_len,
                    g_snapshot_open_info.charset_ptr,
                    g_snapshot_open_info.url_ptr,
                    g_snapshot_open_info.browser_instance,
                    g_snapshot_open_info.page_mime_type)))
    	{
            break;
    	}
    	else
    	{
    		BrwSetSnapshotOpenLayoutHandler(g_snapshot_open_info.browser_instance,g_snapshot_open_info.layout_handle);
    		//generate sub media list
    		if(BrwGenerateOpenMediaList())//need to do
    		{
    			//to open image
    			if(0!=g_snapshot_open_info.img_route_ptr)
    			{
    				if(!BrwOpenOneImage())
    				{
    				    result = TRUE;
    				}
    			}
    			else//no image exists
    			{
				    result = TRUE;
    			}
    		}
    		else
    		{
    			//callback succ
			    result = TRUE;
    		}
    	}
    }while(0);

	if(g_snapshot_open_info.file_buf_ptr != PNULL)
	{
	    //BRWMem_Free(g_snapshot_open_info.file_buf_ptr);
		g_snapshot_open_info.file_buf_ptr = 0;
		g_snapshot_open_info.file_size = 0;
	}

    return result;
}


PUBLIC void BRW_SnapshotLayoutOfflinePage(void)
{
    BRW_INFO_T  *brw_info_ptr =PNULL;

    if((0 == g_snapshot_open_info.layout_handle)
        || (PNULL == g_snapshot_open_info.url_ptr)
        ||(0 == g_snapshot_open_info.browser_instance)
        )
    {
        return;
    }

    brw_info_ptr = BrwGetBrwInfoPtrByInstance(g_snapshot_open_info.browser_instance);
    if (PNULL == brw_info_ptr)
    {
        return;
    }

    if (!g_snapshot_open_info.is_fixed)
    {
        uint16 title_arr[BRW_MAX_HIS_NAME_LEN+1]={0};
        uint16 title_len=0;
        
        //加入页面浏览历史记录
        BrwGetPageTitle(g_snapshot_open_info.layout_handle,title_arr,BRW_MAX_HIS_NAME_LEN,&title_len);
        BrwVisitHisAddItem((uint8*)g_snapshot_open_info.url_ptr,title_arr,title_len);
        //加入导航历史记录
        BrwNavHisAddItem((const uint8*)g_snapshot_open_info.url_ptr);
    }

    //Free browser displaying wap request
    if(PNULL != brw_info_ptr->cur_display_req_ptr)
    {
        BrwLayoutNavigateLostFocus(brw_info_ptr->cur_display_req_ptr->layout_handle);
        BrwFreeWapReqStruct(brw_info_ptr->cur_display_req_ptr);
        brw_info_ptr->cur_display_req_ptr = PNULL;
        brw_info_ptr->cur_req_ptr = PNULL;
    }

    if(g_snapshot_open_info.layout_handle != 0)
    {
        BrwLayout(g_snapshot_open_info.layout_handle,LAYOUT_CAUSE_PAGE);
    }

    if(!g_snapshot_open_info.is_fixed)
    {
        BrwDisplayWebPage(brw_info_ptr->profile.win_handle, g_snapshot_open_info.layout_handle);
    }
    else
    {
        g_snapshot_open_info.layout_handle = 0;//destory layout_handle, by destory the fixed instance
    }
}

PUBLIC void BRW_SnapshotEndOfflinePage(void)
{
	BrwDestroyOpenInfo();
}
LOCAL BRW_SNAPSHOT_ITEM_INFO_T* BrwSetItemInfo(BRWPL_FFS_FIND_DATA_T *file_info_ptr,uint16 *file_path_ptr)
{
	BRW_SNAPSHOT_ITEM_INFO_T *item_info_ptr = 0;
	uint32 len = 0;

	if(0==file_info_ptr||0==file_path_ptr)
	{
		return 0;
	}
	item_info_ptr = BRWMem_Alloc(sizeof(BRW_SNAPSHOT_ITEM_INFO_T));
	if(0==item_info_ptr)
	{
		return 0;
	}
	item_info_ptr->creat_date.day=file_info_ptr->create_Date.mday;
	item_info_ptr->creat_date.mon=file_info_ptr->create_Date.mon;
	item_info_ptr->creat_date.year=file_info_ptr->create_Date.year;
	item_info_ptr->file_size=file_info_ptr->length;
	len=sizeof(uint16)*(BRW_WStrlen(file_info_ptr->name)+SNAPSHOT_FOLDER_ROUTE_LEN+strlen(BRW_FILE_SLASH));
	item_info_ptr->file_name=BRWMem_Alloc(len+2);
	if(0==item_info_ptr->file_name)
	{
		BRWMem_Free(item_info_ptr);
		return 0;
	}
	BRW_Memset(item_info_ptr->file_name,0,len+2);
	BRW_Memcpy(item_info_ptr->file_name,file_path_ptr,SNAPSHOT_FOLDER_ROUTE_LEN*2);
	BRWPL_WstrStrnCatWithAsciiStr(item_info_ptr->file_name,BRW_FILE_SLASH,len/2+1);
	BRW_WstrCat(item_info_ptr->file_name,file_info_ptr->name);
	return item_info_ptr;
}
LOCAL void BrwSnapshotReadFfs(const wchar *device_ptr,  uint16 device_len)
{
	uint16 *path_ptr = 0;
	uint32 len = 0;
	BRWPL_FFS_HANDLE ffs_handler=0;
	BRWPL_FFS_FIND_DATA_T file_info={0};
	BRW_SNAPSHOT_ITEM_INFO_T *item_info_ptr = 0;

	if(!MMIAPIFMM_GetDeviceStatus(device_ptr,device_len))
    {
		return;
    }
	len = sizeof(uint16)*device_len+sizeof(uint16)*strlen(SNAPSHOT_FIND_ALL_FILE_STR);
	path_ptr = BRWMem_Alloc(len+2);
	if(0==path_ptr)
	{
		return;
	}
	BRW_Memset(path_ptr,0,len+2);
	BRW_WstrCat(path_ptr,device_ptr);
	BRWPL_WstrStrnCatWithAsciiStr(path_ptr,SNAPSHOT_FIND_ALL_FILE_STR,len/2+1);
	if(0!=(ffs_handler=BRWPL_FfsFindFirst(path_ptr,&file_info)))
	{
		item_info_ptr = BrwSetItemInfo(&file_info,path_ptr);
		if(0==item_info_ptr)
		{
			BRWMem_Free(path_ptr);
			return;
		}
		BRWList_AppendObject(&g_snapshot_file_list,item_info_ptr);
		BRW_Memset(&file_info,0,sizeof(BRWPL_FFS_FIND_DATA_T));
		while(BRWPL_FfsFindNext(ffs_handler,&file_info))
		{			
			item_info_ptr = BrwSetItemInfo(&file_info,path_ptr);
			if(0==item_info_ptr)
			{
				BRWMem_Free(path_ptr);
				return;
			}
			BRWList_AppendObject(&g_snapshot_file_list,item_info_ptr);
			BRW_Memset(&file_info,0,sizeof(BRWPL_FFS_FIND_DATA_T));
		}
		BRWPL_FindClose(ffs_handler);
	}
	BRWMem_Free(path_ptr);
}

LOCAL void BrwFreeItemInfoMem(BRW_SNAPSHOT_ITEM_INFO_T *item_info_ptr)
{
	if(0==item_info_ptr)/* parasoft-suppress BD-RES-INVFREE "有判断检查" */
	{
		return;
	}
	if(item_info_ptr->file_name)/* parasoft-suppress BD-RES-INVFREE "有判断检查" */
	{
		BRWMem_Free(item_info_ptr->file_name);
	}
	BRWMem_Free(item_info_ptr);
}
PUBLIC void BRW_SnapshotInit(void)
{
	MMIFILE_DEVICE_E device = MMI_DEVICE_UDISK;
	if(0!=g_snapshot_file_list.next)
	{
		BRWList_Destruct(&g_snapshot_file_list,BrwSnapshotListItemDestruct);
	}
	BRWList_Construct(&g_snapshot_file_list);
	for(device = MMI_DEVICE_UDISK; device < MMI_DEVICE_NUM; device++)
	{
		BrwSnapshotReadFfs(MMIAPIFMM_GetDevicePath(device),MMIAPIFMM_GetDevicePathLen(device));
	}
	//BrwSnapshotReadFfs(MMIFILE_DEVICE_SDCARD,MMIFILE_DEVICE_SDCARD_LEN);
}

LOCAL void BrwSnapshotListItemDestruct(void *item_info_ptr)
{
	BrwFreeItemInfoMem((BRW_SNAPSHOT_ITEM_INFO_T*)item_info_ptr);/* parasoft-suppress BD-RES-INVFREE "有判断检查" */
}

LOCAL void BrwSnapshotInsertItem(uint16 *file_path_ptr)
{
	BRW_SNAPSHOT_ITEM_INFO_T *item_info_ptr = 0;
	BRWPL_FFS_FIND_DATA_T file_info={0};
	BRWPL_FFS_HANDLE ffs_handler=0;

	if(0==file_path_ptr)
	{
		return;
	}
	if(0!=(ffs_handler=BRWPL_FfsFindFirst(file_path_ptr,&file_info)))
	{
		item_info_ptr = BrwSetItemInfo(&file_info,file_path_ptr);
		if(0!=item_info_ptr)
		{
			BRWList_AppendObject(&g_snapshot_file_list,item_info_ptr);
		}
		BRWPL_FindClose(ffs_handler);
	}
}
PUBLIC void BRW_SnapshotExit(void)
{
    BrwDestroySaveInfo();
    BRW_SnapshotEndOfflinePage();
    BRWList_Destruct(&g_snapshot_file_list,BrwSnapshotListItemDestruct);
}
PUBLIC const BRW_SNAPSHOT_ITEM_INFO_T* BRW_SnapshotGetItemInfo(uint32 index)
{
	uint32 count = 0;
	BRW_LIST_T *list_node_ptr = 0;
    const BRW_SNAPSHOT_ITEM_INFO_T *item_info_ptr=0;

	if(index>=BRWList_GetListCount(&g_snapshot_file_list))
	{
		return 0;
	}
	list_node_ptr = g_snapshot_file_list.next;
	while(list_node_ptr)
	{
		if(count==index)
		{
			break;
		}
		else
		{
			list_node_ptr = list_node_ptr->next;
			count++;
		}
	}
    if(list_node_ptr!=0)
    {
	    item_info_ptr = (const BRW_SNAPSHOT_ITEM_INFO_T*)(list_node_ptr->object);
    }
    return item_info_ptr;
}
PUBLIC BOOLEAN BRW_SnapshotRenameFile(uint32 index, uint16 *new_name_path_ptr)
{
	uint32 count = 0;
	BRW_LIST_T *list_node_ptr = 0;
	BRW_SNAPSHOT_ITEM_INFO_T *item_info_ptr = 0;
	uint32	len = 0;

	if(index>=BRWList_GetListCount(&g_snapshot_file_list))
	{
		return FALSE;
	}
	list_node_ptr = g_snapshot_file_list.next;
	while(list_node_ptr)
	{
		if(count==index)
		{
			break;
		}
		else
		{
			list_node_ptr = list_node_ptr->next;
			count++;
		}
	}
    if(PNULL !=list_node_ptr)
    {
	item_info_ptr = list_node_ptr->object;
	if(item_info_ptr->file_name)
	{
		if(!BRWPL_RenameFile(item_info_ptr->file_name,new_name_path_ptr))
		{
			//BRWDebug_Printf:"BRW_SnapshotRenameFile,rename fail!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_2033_112_2_18_2_7_29_829,(uint8*)"");
			return FALSE;
		}
		BRWMem_Free(item_info_ptr->file_name);
	}
	len = sizeof(uint16)*BRW_WStrlen(new_name_path_ptr);
	if(len==0)
	{
		return FALSE;
	}
	item_info_ptr->file_name = BRWMem_Alloc(len+2);
	if(0==item_info_ptr->file_name)
	{
		return FALSE;
	}
	BRW_Memset(item_info_ptr->file_name,0,len+2);
	BRW_WstrCat(item_info_ptr->file_name,new_name_path_ptr);
	return TRUE;
    }
    else
    {
	return FALSE;
    }
}
PUBLIC BOOLEAN BRW_SnapshotDeleteItem(uint32 index)
{
	uint32 count = 0;
	BRW_LIST_T *list_node_ptr = 0;
	BRW_SNAPSHOT_ITEM_INFO_T *item_info_ptr = 0;

	if(index>=BRWList_GetListCount(&g_snapshot_file_list))
	{
		return FALSE;
	}
	list_node_ptr = g_snapshot_file_list.next;
	while(list_node_ptr)
	{
		if(count==index)
		{
			break;
		}
		else
		{
			list_node_ptr = list_node_ptr->next;
			count++;
		}
	}
    if(PNULL != list_node_ptr)
    {
	item_info_ptr = list_node_ptr->object;
	if(!BrwSnapshotDeleteWebPage(item_info_ptr->file_name))
	{
		return FALSE;
	}
	BrwFreeItemInfoMem(item_info_ptr);
	BRWList_RemoveObject(&g_snapshot_file_list,item_info_ptr);
	return TRUE;
    }
    else
    {
	return FALSE;
    }
}

PUBLIC uint32 BRW_SnapshotGetCount(void)
{
	return BRWList_GetListCount(&g_snapshot_file_list);
}

#if 0 /* Reduce Code size */
PUBLIC BOOLEAN BRW_GetPageImageUrl(BRW_INSTANCE instance, uint8 **url_ptr, uint32 *index_ptr)
{
	LAYOUT_HANDLE	layout_handler=0;
	uint32			index = 0;
	BOOLEAN			has_image = FALSE;
	BRW_INFO_T		*brw_info_ptr =0;
	BOOLEAN         is_url = TRUE;

	if(0==instance||0==url_ptr)
	{
		return FALSE;
	}
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if(PNULL==brw_info_ptr)
    {
        //SCI_TRACE_LOW:"BRW_DisplayPage,brw_info_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_2118_112_2_18_2_7_29_830,(uint8*)"");
        return FALSE;
    }
    if(PNULL==brw_info_ptr->cur_display_req_ptr)
    {
		layout_handler = brw_info_ptr->snapshot_open_layout_handler;
    }
	else
	{
		layout_handler = brw_info_ptr->cur_display_req_ptr->layout_handle;
	}	
	if(0==layout_handler)
	{
		//SCI_TRACE_LOW:"BRW_GetPageImageUrl error: LAYOUT PNULL"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_2131_112_2_18_2_7_29_831,(uint8*)"");
		return FALSE;
	}
    has_image = BrwGetImgUrl(layout_handler,url_ptr,index, &is_url);
    if(has_image)
    {
        while(*url_ptr==0 || !is_url)
        {
            index++;
            has_image = BrwGetImgUrl(layout_handler,url_ptr,index, &is_url);
            if(!has_image)
            {
                break;
            }
        }
        if(0!=*url_ptr)
        {
            *index_ptr=index;
            return TRUE;
        }
    }
	return FALSE;
}


PUBLIC BOOLEAN BRW_SavePageImage(
								 BRW_INSTANCE instance, 
								 uint32 index, 
								 uint16 *full_path_ptr,
								 BRW_SAVE_IMAGE_CB_PARAM_T *cb_param_ptr
								 )
{
	LAYOUT_HANDLE	layout_handler=0;
	BRW_INFO_T		*brw_info_ptr =0;
	uint8			*img_data_ptr = 0;
	uint32			data_len = 0;
	BRW_MIME_TYPE_E mime_type = 0;
	BRWPL_FFS_HANDLE file_handler = 0;
	BRWPL_FFS_OVERLAPPED_T overlapped = {0};

	if(0==full_path_ptr||0==instance)
	{
		return FALSE;
	}
	brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if(PNULL==brw_info_ptr)
    {
        //SCI_TRACE_LOW:"BRW_DisplayPage,brw_info_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_2178_112_2_18_2_7_29_832,(uint8*)"");
        return FALSE;
    }
    if(PNULL==brw_info_ptr->cur_display_req_ptr)
    {
		layout_handler = brw_info_ptr->snapshot_open_layout_handler;
    }
	else
	{
		layout_handler = brw_info_ptr->cur_display_req_ptr->layout_handle;
	}	
	if(0==layout_handler)
	{
		//SCI_TRACE_LOW:"BRW_SavePageImage error: LAYOUT PNULL"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_2191_112_2_18_2_7_29_833,(uint8*)"");
		return FALSE;
	}
	if(!BrwGetImgBuf(&img_data_ptr,&data_len,&mime_type,layout_handler,index))
	{
		//SCI_TRACE_LOW:"BRW_SavePageImage error: BrwGetImgBuf error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_2196_112_2_18_2_7_29_834,(uint8*)"");
		return FALSE;
	}
	if(0!=cb_param_ptr)
	{
		overlapped.complete_route = SavePageImageCb;
		overlapped.param = (SFS_PARAM)cb_param_ptr;
		file_handler=BRWPL_WriteFileAsync(full_path_ptr,img_data_ptr,
													data_len,&overlapped);
		if(file_handler==0)
		{
			//SCI_TRACE_LOW:"BRW_SavePageImage error: BRWPL_WriteFileAsync error"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_SNAPSHOT_2207_112_2_18_2_7_29_835,(uint8*)"");
			return FALSE;
		}
		BRWPL_CloseFile(file_handler);
	}
	else
	{
		return BRWPL_WriteFileSyn(full_path_ptr,BRW_WStrlen(full_path_ptr),img_data_ptr,data_len);
	}
	return TRUE;
}

#endif /* Reduce Code size */
LOCAL void SavePageImageCb(
							 SFS_ERROR_E error, //result
							 uint32 irp_param, //app param
							 uint32 param1,	//bytes written/read
							 uint32 param2
							 )
{
	BOOLEAN is_succ = TRUE;
	uint32 msg_id=0;
	uint32 win_id=0;

	if(0==param1 || SFS_ERROR_NONE!=error)
	{
		is_succ = FALSE;
	}
	if(0!=irp_param)
	{
		msg_id = ((BRW_SAVE_IMAGE_CB_PARAM_T*)irp_param)->callback_msg_id;
		win_id = ((BRW_SAVE_IMAGE_CB_PARAM_T*)irp_param)->callback_win_id;
		MMK_PostMsg(win_id,msg_id,&is_succ,sizeof(BOOLEAN));
		MMI_TriggerMMITask();
	}
}
