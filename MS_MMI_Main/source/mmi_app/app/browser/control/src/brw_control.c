/*****************************************************************
 **  File Name: 	brw_control.c    							 *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:implemention of control  message sub module     *
******************************************************************
******************************************************************
**  Edit History											     *
**---------------------------------------------------------------*
 **  DATE			NAME			DESCRIPTION				     *
 **  30/12/2008		yuzhi.chen      Create. 					 *
*****************************************************************/
#include "mmi_app_browser_trc.h"
#include "brw_utility.h"
#include "brw_control_api.h"
#include "brw_control.h"
#include "brw_history.h"
#include "brw_dlinterface.h"
#include "mmipdp_export.h"
#include "mmi_font.h"
#include "mmibrowser_wintable.h"
#include "mmibrowser_setting.h"
#include "mmibrowser_func.h"

#ifdef OMADL_SUPPORT
#include "in_message.h"
#endif
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "brw_core_adapter.h"

#ifdef ENG_SUPPORT
#include "mmieng_export.h" 
#endif
/*----------------macro definition begin------------------------*/
#define MAX_MIME_TYPE_LEN   40  //最大的mime_type字串长度
//#define COOKIE_NAME_PATH ":\\browser\\cookie.dat"
//#define CACHE_FOLDER_PATH ":\\browser\\caches\\"
#define CACHE_FOLDER_PATH  (s_browser_cache_path) 
#define CACHE_FOLDER_PATH_LEN  15 
#define COOKIE_NAME  (s_browser_cookie_name) 
#define COOKIE_NAME_LEN  10 
#define WEB_ICO_NAME  (s_browser_web_ico_name) 
#define WEB_ICO_NAME_LEN  7 

/*----------------struct definition begin------------------------*/
const wchar s_browser_cache_path[] = {'b' , 'r', 'o', 'w', 's', 'e', 'r', '\\', 'c','a','c','h','e','s', '\\', 0 };
const wchar s_browser_cookie_name[] = {'c' , 'o', 'o', 'k', 'i', 'e', '.', 'd', 'a','t',0 };
const wchar s_browser_web_ico_name[] = {'w' , 'e', 'b',  '.', 'i', 'c','o',0 };//web.ico

typedef struct tagHASH_UNIT_T
{
    BRW_MIME_TYPE_E mime_type_value;
    uint8           mime_type_str[MAX_MIME_TYPE_LEN];
    uint32          hash_value;
}HASH_UNIT_T;

/*----------------struct definition end--------------------------*/
/*----------------global variable declaration  begin-------------*/
BRW_INSTANCE_LIST_STRUCT g_brw_instance_list_info = {0};

LOCAL BRW_LIST_T s_brw_instance_list = {0};
LOCAL uint32  g_next_req_id=1;
LOCAL uint32  g_next_instance_id = 1;
LOCAL BOOLEAN  g_is_net_linked=FALSE;
LOCAL const uint8 g_version_str[]="BRW_Ver_Base1.1.1_20090120";
LOCAL BRW_PDP_STATUS_E  s_brw_pdp_status = BRW_PDP_STATUS_NONE;

LOCAL const HASH_UNIT_T   g_mime_type_hash_table[]=
{
    {BRW_MIME_HTML,"text/html",0},
    {BRW_MIME_HTML,"application/xhtml+xml",0},
    {BRW_MIME_HTML,"application/vnd.wap.xhtml+xml",0},
    {BRW_MIME_HTML,"application/xhtml",0},
    {BRW_MIME_HTML,"text/orangeml",0},//NEWMS00120446
    {BRW_MIME_WML,"text/vnd.wap.wml",0},
    {BRW_MIME_WML,"application/wml+xml",0},
    {BRW_MIME_WMLS,"text/vnd.wap.wmlscript",0},
    {BRW_MIME_WMLSC,"application/vnd.wap.wmlscriptc",0},
    {BRW_MIME_WMLC,"application/vnd.wap.wmlc",0},
    {BRW_MIME_WMLC,"application/vnd.wap.wbxml",0},
    {BRW_MIME_PLAINTEXT,"text/plain",0},
    {BRW_MIME_CSS,"text/css",0},
    {BRW_MIME_IMG_BMP,"image/bmp",0},
    {BRW_MIME_IMG_IEF,"image/ief",0},
    {BRW_MIME_IMG_GIF,"image/gif",0},
    {BRW_MIME_IMG_JPEG,"image/jpeg",0},
    {BRW_MIME_IMG_JPEG,"image/jpg",0},
    {BRW_MIME_IMG_PNG,"image/png",0},
    {BRW_MIME_IMG_TIF,"image/tiff",0},
    {BRW_MIME_IMG_WBMP,"image/vnd.wap.wbmp",0},
    {BRW_MIME_AUDIO_AMR,"audio/amr",0},
    {BRW_MIME_AUDIO_MIDI,"audio/midi",0},
    {BRW_MIME_AUDIO_MID,"audio/mid",0},
    {BRW_MIME_AUDIO_MPA,"audio/mpa",0},
    {BRW_MIME_AUDIO_MPA_ROBUST,"audio/mpa-robust",0},
    {BRW_MIME_AUDIO_MPA_LATM,"audio/mp4a-latm",0},
    {BRW_MIME_AUDIO_MPEG,"audio/mpeg",0},
    {BRW_MIME_AUDIO_MPEG,"audio/mpg",0},
    {BRW_MIME_AUDIO_MPEG,"audio/x-mp3",0},
    {BRW_MIME_AUDIO_TONE,"audio/tone",0},
    {BRW_MIME_AUDIO_RM,"application/vnd.rn-realmedia",0},
    {BRW_MIME_AUDIO_WAV,"audio/x-wav",0},
    {BRW_MIME_AUDIO_WAV,"audio/wav",0},
    {BRW_MIME_AUDIO_3GP,"audio/3gp",0},
#ifdef AAC_SUPPORT
    {BRW_MIME_AUDIO_X_AAC,"audio/x-aac",0},
    {BRW_MIME_AUDIO_X_AAC,"audio/aac",0},
#endif    
    {BRW_MIME_AUDIO_WMA,"audio/x-ms-wma",0},
    {BRW_MIME_VIDEO_3GP,"video/3gpp",0},
    {BRW_MIME_VIDEO_MP4,"video/mp4",0},
    {BRW_MIME_VIDEO_MPEG,"video/mpeg",0},
    {BRW_MIME_VIDEO_WMV,"video/x-ms-wmv",0},
    {BRW_MIME_OMA_DD,"application/vnd.oma.dd+xml",0},
    {BRW_MIME_OMA_DRM_MESSAGE,"application/vnd.oma.drm.message",0},
    {BRW_MIME_OMA_DRM_CONTENT,       "application/vnd.oma.drm.content",0},
    {BRW_MIME_OMA_DRM_RIGHTS_XML,    "application/vnd.oma.drm.rights+xml",0},
    {BRW_MIME_OMA_DRM_RIGHTS_WBXML,  "application/vnd.oma.drm.rights+wbxml",0},
    {BRW_MIME_JAD,"text/vnd.sun.j2me.app-descriptor",0},
    {BRW_MIME_JAR,"application/Java-archive",0},
    {BRW_MIME_SDP,"application/sdp",0}
#ifdef QBTHEME_SUPPORT
	,
	{BRW_MIME_QBT,"application/sprd.qbt",0}
#endif
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
    ,
	{BRW_MIME_PNL,"application/sprd.pnl",0}
#endif
#ifdef DATA_ACCOUNT_IN_TCARD_SUPPORT
    ,
	{BRW_MIME_NTAC,"application/sprd.ntac",0}
#endif

};

LOCAL BRW_LIST_T    g_image_list_info = {0};

extern BOOLEAN g_is_http_log_file_save; 
#ifdef JS_SUPPORT
LOCAL BRW_MSG_PROC_FUNC    s_msg_proc_func = NULL;
#endif
extern char* HttpMovetoHostEnd(char *absolute_url_ptr);
/*----------------global variable declaration  end---------------*/

/*----------------LOCAL function declaration  begin--------------*/

LOCAL uint32 GenReqId(void);
LOCAL uint32 GenInstanceId(void);
LOCAL BOOLEAN SetProfile(BRW_INFO_T *brw_info_ptr,BRW_PROFILE_T* profile);
LOCAL void InitBrwInfoStruct(BRW_INFO_T *brw_info_ptr);
LOCAL void FreeBrwInfoStruct(BRW_INFO_T *brw_info_ptr);
LOCAL void SetNetLinked(BOOLEAN is_net_linked);
//////////////////////////////////////////////////////////////////////////
// modified by feng.xiao
LOCAL void ProcNetLinkCnf(BRW_INFO_T *brw_info_ptr, MMIPDP_RESULT_E result);
LOCAL void ProcNetLinkDisConnInd(BRW_INFO_T *brw_info_ptr);
LOCAL void ProcNetLinkDisConnCnf(BRW_INFO_T *brw_info_ptr,BOOLEAN result);
LOCAL void FreeMediaObjStruct(void* media_obj_info_ptr);
LOCAL BOOLEAN IsSameReq(WAP_REQUEST_T *req,uint32 req_id);
LOCAL BOOLEAN   IsReqBelongToInstance(BRW_INFO_T *brw_info_ptr,uint32 req_id);
LOCAL BOOLEAN IsDLSessionBelongtoInstance(BRW_INFO_T* brw_info_ptr,uint32 session_id);
LOCAL BOOLEAN IsHttpReqBelongToInstance(BRW_INFO_T *brw_info_ptr,HTTP_REQ_INFO_T* http_req_ptr);
LOCAL BOOLEAN CompareHttpReq(WAP_REQUEST_T* req_ptr,HTTP_REQ_INFO_T *http_req_ptr);
LOCAL BOOLEAN MediaCompareHttpReq(MEDIA_OBJ_T* media_obj_ptr,HTTP_REQ_INFO_T *http_req_ptr);
LOCAL MEDIA_OBJ_T* GetMediaObjByHttpReq(WAP_REQUEST_T* wap_req_ptr,HTTP_REQ_INFO_T* req_ptr);
LOCAL BOOLEAN ProcHisEvent(const uint32 req_id, BRW_NAVIGATE_EVENT_E op);
LOCAL BRW_RET ProcPageContent(BRW_INFO_T *brw_info_ptr,WAP_REQUEST_T* req_ptr);
LOCAL BOOLEAN ProcMediaContent(BRW_INFO_T *brw_info_ptr,HTTP_REQ_INFO_T* req_ptr);
LOCAL BOOLEAN IsMediaObjListEmpty(WAP_REQUEST_T* req);
LOCAL BOOLEAN IsMediaFinished(WAP_REQUEST_T* req);
LOCAL BOOLEAN CompareObj(void* dst_obj_ptr,void* src_obj_ptr);
//LOCAL void InitMineTypeHashTable(void);
LOCAL void BrwStartNextMediaReq(BRW_INFO_T* brw_info_ptr);
LOCAL void BrwStartMediaReq(BRW_INFO_T* brw_info_ptr);
LOCAL BOOLEAN CompareDLSessionID(WAP_REQUEST_T* req_ptr,uint32 session_id);
LOCAL MEDIA_OBJ_T* BrwAllocMediaObj(void);
LOCAL void BrwGenerateMediaObjList(BRW_INFO_T *brw_info_ptr, WAP_REQUEST_T* req_ptr);
//LOCAL BOOLEAN CheckIfSupportUrlScheme(const char *url_ptr, BRW_INFO_T *brw_info_ptr);
LOCAL BOOLEAN BrwSetWspDlFileNm(WAP_REQUEST_T *wap_req_ptr,const uint16* file_name_ptr,uint16 file_name_len);
LOCAL BOOLEAN HandleCardJumping(BRW_INFO_T *brw_info_ptr,const char *jumpto_ulr_ptr,BRW_NAVIGATE_EVENT_E nav_event);
LOCAL BOOLEAN IsInstanceIdEqual(void *brw_info_ptr,void* instance_id);
//LOCAL BOOLEAN IsWinHandleEqual(void *brw_info_ptr,void* win_handler);
//LOCAL BRW_INFO_T* BrwGetBrwInfoPtrByWinHandle(BRW_WIN_HANDLE win_handler);
LOCAL BOOLEAN BrwBackwardUsingCacheReq(BRW_INFO_T *brw_info_ptr);
LOCAL BOOLEAN BrwFileCopyForWspDl(WAP_REQUEST_T *wap_req_ptr);
LOCAL BOOLEAN  BRW_SetScreenProfile(BRW_INFO_T *brw_info_ptr, BRW_SCREEN_OPTION_T screen_opt);//MS00195411
LOCAL BOOLEAN  BRW_SetFontProfile(BRW_INFO_T *brw_info_ptr, uint32 font_type);
LOCAL void BrwSaveDataToFile(uint8* buff_ptr, int32 len);
LOCAL LAYOUT_HANDLE GetCurDisplayLayout(BRW_INSTANCE instance);
LOCAL BOOLEAN BrwDecodeFileForDl(WAP_REQUEST_T *wap_req_ptr);
PUBLIC BOOLEAN BRW_AppendInstance(BRW_INFO_T *new_brw_info_ptr);
PUBLIC BOOLEAN BRW_RemoveInstance(BRW_INFO_T *brw_info_ptr);
LOCAL BOOLEAN ProcAfterNetLinkCnf(BRW_INFO_T *brw_info_ptr);
LOCAL BOOLEAN StopAndFreeBGsound(
            LAYOUT_HANDLE    *layout_handle,//[in]priority 1
            BRW_INFO_T *brw_info_ptr//[in]priority 2
            );

/*****************************************************************************/
//  Description : brw save data to file
//  Global resource dependence : 
//  Author: sally.he
//  Note: 
/*****************************************************************************/
LOCAL void BrwSaveDataToFile(uint8* buff_ptr, int32 len)
{
    uint16          file_name[]   = {'E',':','\\','h','t','t','p','.','t','x','t',0};//save log
    uint8           endof[2]      = {0x0d, 0x0a};
    SFS_HANDLE      file_handle   = 0;
    uint32 	        bytes_written = 0;

    if(buff_ptr == PNULL || len < 1)
    {
        return;
    }

    //save brw data to file
    if(g_is_http_log_file_save)
    {
        file_handle = SFS_CreateFile(file_name, SFS_MODE_OPEN_ALWAYS | SFS_MODE_WRITE| SFS_MODE_APPEND,0,0);/*lint !e655*/      
        if (file_handle == 0)
        {
            //BRWDebug_Printf:"brw BrwSaveDataToFile create file error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_208_112_2_18_2_6_49_341,(uint8*)"");
        }
        else
        {
            SFS_WriteFile(file_handle, endof, 2, &bytes_written, 0);//append /r/n
            SFS_WriteFile(file_handle, buff_ptr, len, &bytes_written, 0);
            SFS_WriteFile(file_handle, endof, 2, &bytes_written, 0);//append /r/n
            SFS_CloseFile(file_handle);
        }
    }   
}
/*----------------LOCAL function declaration  end----------------*/
/*
@desc:set param for the browser instance
@param: instance    instance of browser
        profile     parameter
        profile_type   the param want to set,the flag may be bit and
@return:TRUE if success,else FALSE
*/
PUBLIC BOOLEAN  BRW_SetPublicParam(BRW_PUBLIC_PROFILE_T* public_profile,BRW_PROFILE_TYPE_E profile_type)
{
    uint32 temp_profile_type = (uint32)profile_type;
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = PNULL;
	
    //protect: to free
    public_profile_ptr = &g_brw_instance_list_info.public_profile;

    //BRWDebug_Printf:"[BRW]BRW_SetParam,temp_profile_type=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_234_112_2_18_2_6_49_342,(uint8*)"d", temp_profile_type);
    
    if ((PNULL == public_profile) || (BRW_NULL_PROFILE_TYPE == temp_profile_type))
    {
        //BRWDebug_Printf:"[BRW]W BRW_SetPublicParam,profile=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_238_112_2_18_2_6_49_343,(uint8*)"d", public_profile);
        return FALSE;
    }
    
    if(temp_profile_type&BRW_CACHE_PROFILE)//set enable cache option
    {
        public_profile_ptr->is_cache_enable=public_profile->is_cache_enable;
    }
    if(temp_profile_type & BRW_COOKIE_PROFILE)
    {
        public_profile_ptr->is_cookie_enable=public_profile->is_cookie_enable;
    }
    if(temp_profile_type & BRW_IMG_DL_PROFILE)
    {
        public_profile_ptr->is_image_download=public_profile->is_image_download;
	    public_profile_ptr->is_preview_image=public_profile->is_preview_image;
    }
    if(temp_profile_type & BRW_BGSOUND_DL_PROFILE)
    {
        public_profile_ptr->is_bgsound_download = public_profile->is_bgsound_download;
    }
	if(temp_profile_type&BRW_GWT_PROFILE)
	{
		public_profile_ptr->is_gwt_enable = public_profile->is_gwt_enable;
		public_profile_ptr->lang = public_profile->lang;
	}
    if(temp_profile_type & BRW_USER_AGENT_PROFILE)
    {        
        if(public_profile->user_agent_ptr != PNULL)
        {
            uint32 ua_str_len = 0;

            ua_str_len = BRW_Strlen(public_profile->user_agent_ptr);
            if(ua_str_len > 0)
            {
                if(public_profile_ptr->user_agent_ptr != PNULL)
                {
                    BRWMem_Free(public_profile_ptr->user_agent_ptr);
                    public_profile_ptr->user_agent_ptr = PNULL;
                }
            
                public_profile_ptr->user_agent_ptr = (uint8*)BRWMem_Alloc(ua_str_len+1);   
                if(public_profile_ptr->user_agent_ptr != PNULL)
                {
                    BRW_Memcpy(public_profile_ptr->user_agent_ptr, public_profile->user_agent_ptr, (ua_str_len+1));
                }
                else
                {
                    BRWDebug_PrintWarning("BRW_SetParam,set ua failed because of mem!");
                }
            }//if(ua_str_len>0)
            else
            {
                //BRWDebug_Printf:"[BRW]BRW_SetParam:set ua null!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_290_112_2_18_2_6_49_344,(uint8*)"");
            }
        }//if(profile->user_agent_ptr)     
    }//if(temp_profile_type & BRW_USER_AGENT_PROFILE)

    if(temp_profile_type & BRW_UAPROF_PROFILE)
    {
        if(public_profile->ua_prof_ptr != PNULL)
        {
            uint32 uaprof_len=0;
            
            uaprof_len = BRW_Strlen(public_profile->ua_prof_ptr);
            if(uaprof_len > 0)
            {
                if(public_profile_ptr->ua_prof_ptr != PNULL)
                {
                    BRWMem_Free(public_profile_ptr->ua_prof_ptr);
                    public_profile_ptr->ua_prof_ptr = PNULL;
                }
                
                public_profile_ptr->ua_prof_ptr = (uint8*)BRWMem_Alloc(uaprof_len+1);
                if(public_profile_ptr->ua_prof_ptr != PNULL)
                {
                    BRW_Memcpy(public_profile_ptr->ua_prof_ptr, public_profile->ua_prof_ptr, (uaprof_len+1));
                }
                else
                {
                    BRWDebug_PrintWarning("BRW_SetParam,set uaprof failed because of mem!");
                }
            }//if(uaprof_len>0)
            else
            {
                //BRWDebug_Printf:"[BRW]BRW_SetParam:set uaprof null!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_322_112_2_18_2_6_50_345,(uint8*)"");
            }
        }//if(profile->ua_prof_ptr)
    }//if(temp_profile_type&BRW_UAPROF_PROFILE)
    if(temp_profile_type & BRW_PROXY_PROFILE)
    {
        uint32 temp_len = 0;
            
        public_profile_ptr->proxy_opt.is_use_proxy = public_profile->proxy_opt.is_use_proxy;
        public_profile_ptr->proxy_opt.proxy_port = public_profile->proxy_opt.proxy_port;

        //proxy_addr_ptr
        temp_len=BRW_Strlen(public_profile->proxy_opt.proxy_addr_ptr);
        if(public_profile_ptr->proxy_opt.proxy_addr_ptr)
        {
            BRWMem_Free(public_profile_ptr->proxy_opt.proxy_addr_ptr);
            public_profile_ptr->proxy_opt.proxy_addr_ptr=PNULL;
        }
        if(temp_len>0)
        {
            public_profile_ptr->proxy_opt.proxy_addr_ptr=(uint8*)BRWMem_Alloc(temp_len+1);
            if(public_profile_ptr->proxy_opt.proxy_addr_ptr)
            {
                BRW_Memcpy(public_profile_ptr->proxy_opt.proxy_addr_ptr,
                            public_profile->proxy_opt.proxy_addr_ptr,temp_len+1);
            }
            else
            {
                BRWDebug_PrintWarning("BRW_SetParam,set proxy_addr failed because of mem!");
            }
        }
        else
        {
            //BRWDebug_Printf:"[BRW]BRW_SetParam,proxy_addr_ptr null!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_352_112_2_18_2_6_50_346,(uint8*)"");
        }

        //username_ptr
        if (public_profile_ptr->username_ptr != PNULL)
        {
            BRWMem_Free(public_profile_ptr->username_ptr);
            public_profile_ptr->username_ptr=PNULL;
        }        
        temp_len=BRW_Strlen((uint8*)public_profile->username_ptr);
        if (temp_len > 0)
        {
            public_profile_ptr->username_ptr = (char*)BRWMem_Alloc(temp_len+1);
            if(public_profile_ptr->username_ptr != PNULL)
            {
                BRW_Memcpy(public_profile_ptr->username_ptr, public_profile->username_ptr, temp_len+1);
            }
            else
            {
                SCI_TRACE_LOW("BRW_SetParam,set username_ptr failed because of mem!");
            }
        }
        
        //psw_ptr
        if (public_profile_ptr->psw_ptr != PNULL)
        {
            BRWMem_Free(public_profile_ptr->psw_ptr);
            public_profile_ptr->psw_ptr = PNULL;
        }        
        temp_len=BRW_Strlen((uint8*)public_profile->psw_ptr);
        if (temp_len > 0)
        {
            public_profile_ptr->psw_ptr = (char*)BRWMem_Alloc(temp_len+1);
            if(public_profile_ptr->psw_ptr != PNULL)
            {
                BRW_Memcpy(public_profile_ptr->psw_ptr, public_profile->psw_ptr, temp_len+1);
            }
            else
            {
                SCI_TRACE_LOW("BRW_SetParam,set psw_ptr failed because of mem!");
            }
        }

        //NEWMS00202303
        public_profile_ptr->auth_type = public_profile->auth_type;
        public_profile_ptr->apn_ptr = public_profile->apn_ptr;
        public_profile_ptr->protocol = public_profile->protocol;
    }//if(profile & BRW_PROXY_PROFILE)

    return TRUE;
}

/*
@desc:set param for the browser instance
@param: instance    instance of browser
        profile     parameter
        profile_type   the param want to set,the flag may be bit and
@return:TRUE if success,else FALSE
*/
PUBLIC BOOLEAN  BRW_SetParam(BRW_INSTANCE instance,BRW_PROFILE_T* profile,BRW_PROFILE_TYPE_E profile_type)
{
    BRW_INFO_T  *brw_info_ptr = PNULL;
    uint32 temp_profile_type = (uint32)profile_type;

    //BRWDebug_Printf:"[BRW]BRW_SetParam,temp_profile_type=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_370_112_2_18_2_6_50_347,(uint8*)"d", temp_profile_type);
    
    if ((BRW_INVALID_INSTANCE == instance) || (PNULL == profile) || (BRW_NULL_PROFILE_TYPE == temp_profile_type))
    {
        //BRWDebug_Printf:"[BRW]W BRW_SetParam,instance=0x%x,profile=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_374_112_2_18_2_6_50_348,(uint8*)"dd", instance, profile);
        return FALSE;
    }
    
    brw_info_ptr=BrwGetBrwInfoPtrByInstance(instance);
    if (PNULL == brw_info_ptr)
    {
        BRWDebug_PrintWarning("BRW_SetParam,get brw_info_ptr failed");
        return FALSE;
    }
    if(temp_profile_type & BRW_FONT_PROFILE)
    {
        BRW_SetFontProfile(brw_info_ptr, profile->font_type);
        //BRW_RepaintPage(instance);
    }
    if(temp_profile_type & BRW_SCREEN_PROFILE)
    {
        BRW_SetScreenProfile(brw_info_ptr, profile->screen_opt);//MS00195411
    }//if(profile & BRW_SCREEN_PROFILE)

    BRW_Memcpy(&(brw_info_ptr->profile), profile, sizeof(BRW_PROFILE_T));
    return TRUE;
}

/*
* Description:API for init browser engine
* Param:   BRW_PROFILE_T option  option of browser
* Return:  instance handle of browser,0 if FALSE   
* caution: if success,alloc memory,must call BRW_Destroy to free it
*/
PUBLIC BRW_INSTANCE BRW_InitInstance(BRW_PROFILE_T *option_ptr, BOOLEAN is_fixed)
{
    //BRW_LIST_T  *instance_list_ptr = PNULL;
    BRW_INFO_T  *new_brw_info_ptr = PNULL;
    
    if ((PNULL == option_ptr) || (PNULL == option_ptr->msg_proc_func))
    {
        //BRWDebug_Printf:"[BRW]W BRW_InitInstance,ERROR option_ptr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_410_112_2_18_2_6_50_349,(uint8*)"d", option_ptr);
        return BRW_INVALID_INSTANCE;
    }

#if 0//the memory is alloced only in MMIBROWSER_Init. only use it in here!
    if(!BRW_InitMem())
    {
        return BRW_INVALID_INSTANCE;
    }
#endif

    new_brw_info_ptr = (BRW_INFO_T*)BRWMem_Alloc(sizeof(BRW_INFO_T));
    if (PNULL == new_brw_info_ptr)
    {
        BRWDebug_PrintWarning("BRW_InitInstance:malloc brw_info_t failed");
        return BRW_INVALID_INSTANCE;
    }
    
    InitBrwInfoStruct(new_brw_info_ptr);
    new_brw_info_ptr->is_fixed = is_fixed;
    
    if(!SetProfile(new_brw_info_ptr,option_ptr))
    {
        BRWDebug_PrintWarning("BRW_InitInstance:SetProfile failed!");
        FreeBrwInfoStruct(new_brw_info_ptr);
        return BRW_INVALID_INSTANCE;
    }
#ifdef JS_SUPPORT
	s_msg_proc_func = option_ptr->msg_proc_func;
#endif
    /*
    instance_list_ptr = BrwGetInstanceList();
    */
    //if(!BRWList_AppendObject(instance_list_ptr,new_brw_info_ptr))
    if (!BRW_AppendInstance(new_brw_info_ptr))
    {
        BRWDebug_PrintWarning("BRW_InitInstance:BRWList_AppendObject failed!");
        FreeBrwInfoStruct(new_brw_info_ptr);
        return BRW_INVALID_INSTANCE;
    }
    
    //Brw_VisitHisInit();
    if (!is_fixed)//need to do for multi-instance
    {
        BrwNavHisInit();
    }
    //BRW_SnapshotInit();
    //BrwInitHttpSession(new_brw_info_ptr);
    //BrwParseInit();
    
    //BRWDebug_Printf:"BRW_InitInstance:instance_id =0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_460_112_2_18_2_6_50_350,(uint8*)"d",new_brw_info_ptr->instance_id);
    return (BRW_INSTANCE)new_brw_info_ptr->instance_id;
}
/*
* Description:release relevant resouce and memory of browser engine
* Param: instance   instance handle of browser
* Return:    
*/
PUBLIC void BRW_DestroyInstance(BRW_INSTANCE instance)
{
    //BRW_LIST_T	*instance_list_ptr = PNULL;
    BRW_INFO_T	*brw_info_ptr = PNULL;
    
    //BRWDebug_Printf:"[BRW]BRW_DestroyInstance instance=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_472_112_2_18_2_6_50_351,(uint8*)"d", instance);

    //DeactiveNetwork(instance);
    
    if (instance != BRW_INVALID_INSTANCE)
    {
        brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
        if (brw_info_ptr != PNULL)
        {
            //instance_list_ptr = BrwGetInstanceList();
            FreeBrwInfoStruct(brw_info_ptr); //must before BrwNavHisExit
            //BRW_VisitHisExit();
            BrwNavHisExit();
            //BRW_SnapshotExit();
            BRW_SnapshotEndOfflinePage();//NEWMS00123749
            //BrwHashDestroy();
            //BrwParseDestroy();
            BRW_RemoveInstance(brw_info_ptr);
#if 0            
            BRWList_RemoveObject(instance_list_ptr,brw_info_ptr);
            if(0 == BRWList_GetListCount(instance_list_ptr))
            {
                BRWList_Destruct(BrwGetHttpReqList(), PNULL);  //delete http_req_list
                //BRWMem_DeleteMemHandle();
            }
#endif
#ifdef BRW_MEM_DEBUG
            //	BRWMem_print_mem();
#endif
        }
        else
        {
            //BRWDebug_Printf:"[BRW]W BRW_DestroyInstance brw_info_ptr PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_504_112_2_18_2_6_50_352,(uint8*)"");
        }
    }

}
#if 0
/*
* Description:call this api to start a web page request
* Param: instance   instance handle of browse 
*        uint8* url_ptr         url of web page,end with '\0'
* Return:  unique req id  
*/
LOCAL BOOLEAN CheckIfSupportUrlScheme(const char *url_ptr, BRW_INFO_T *brw_info_ptr)
{
    char *scheme_ptr = PNULL;
    BOOLEAN result = TRUE;

    if  (PNULL == url_ptr)
    {
        return FALSE;
    }
    scheme_ptr = BRWUtil_GetUrlScheme(url_ptr);
    if (PNULL == scheme_ptr)
    {
        return TRUE;
    }
    
    if(0 == BRW_Stricmp((const uint8*)scheme_ptr,(const uint8*)"http"))
    {
    
    }
    else if(0 == BRW_Stricmp((const uint8*)scheme_ptr,(const uint8*)"https"))
    {
    
    }
    else if(0 == BRW_Stricmp((const uint8*)scheme_ptr,(const uint8*)"wtai"))
    {
    
    }
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    else if(0 == BRW_Stricmp((const uint8*)scheme_ptr,(const uint8*)"rtsp"))
    {
        if (brw_info_ptr != PNULL)
        {
            brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_OPEN_RTSP_IND, (void *)url_ptr, (BRW_Strlen((const uint8 *)url_ptr) + 1));
        }
        else
        {
            BRWDebug_PrintWarning("CheckIfSupportUrlScheme rtsp: brw_info_ptr PNULL!");
        }
        result = FALSE;
    }
#endif    
    else
    {
        result = FALSE;
    }
    
    BRWMem_Free(scheme_ptr);
    scheme_ptr = PNULL;

    return result;
}
#endif

PUBLIC void BRW_DisconnetNetwork(BRW_INSTANCE instance)
{
	BRW_INFO_T *brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

	if(brw_info_ptr == 0)
	{
		return;
	}
	//BRWDebug_Printf:"BRW_DisconnetNetwork"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_577_112_2_18_2_6_50_353,(uint8*)"");
	BRW_StopPageReq(instance);
	DeactiveNetwork(instance);
	if(brw_info_ptr->http_state == BRW_HTTP_STATE_INITED)
	{
		BrwSetHttpState(brw_info_ptr, BRW_HTTP_STATE_CLOSING);
		BrwCloseHttpSession(brw_info_ptr->http_context_id, public_profile_ptr->protocol);
	}
	else if(brw_info_ptr->http_state == BRW_HTTP_STATE_INITING)
	{
		brw_info_ptr->is_need_to_close_http = TRUE;
	}
}

PUBLIC uint32 BRW_AccessPageEx(BRW_INSTANCE instance,
            uint8* url_ptr,//[in]
            uint16* ico_filename_ptr//[in]full filename:if not null,need to load ico of the url and save by it
            )
{
    BOOLEAN			is_net_linked = FALSE;
    BRW_INFO_T		*brw_info_ptr=PNULL;
    WAP_REQUEST_T	*new_wap_req_ptr = PNULL;
    WAP_REQUEST_T	*prev_wap_req_ptr=PNULL;
    uint16			url_len = 0;
    BOOLEAN			result=TRUE;
    char			*temp_url_ptr = PNULL;
    char			*connect_url_ptr = PNULL;
#if defined(MMI_WIFI_SUPPORT) || defined(MMIDORADO_GOOGLE_TRANSCODE_SUPPORT) || defined(MMIDORADO_WRE_TRANSCODE_SUPPORT) 
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();
#endif
    uint16  ico_filename_len = 0;
    uint16  http_scheme_len = 0;
    BOOLEAN is_host_without_slash = FALSE;
    char    *host_end_ptr = PNULL;

    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    
    if (PNULL == brw_info_ptr)
    {
        BRWDebug_PrintWarning("BRW_AccessPage:brw_info_ptr null!");
        return BRW_INVALID_REQ_ID;
    }
    
    if (Brw_IsIncludeInvalidUrl(url_ptr))
    {
        BrwProcErr(brw_info_ptr, BRW_ERR_URL_INVALID);
        return BRW_INVALID_REQ_ID;
    }
#ifdef MMI_WIFI_SUPPORT
    if(MMIBROWSER_GetIsUseWifi() && public_profile_ptr->proxy_opt.is_use_proxy && MMIAPIWIFI_IsExistInExcludingList((char *)url_ptr))
    {        
        BRWDebug_PrintWarning("BRW_AccessPage:proxy not support url_ptr");
        return BRW_INVALID_REQ_ID;
    }
#endif

    if (MMIAPIBROWSER_HandleExScheme(url_ptr))
    {
        return BRW_INVALID_REQ_ID;
    }  
    
    temp_url_ptr = BRWUtil_GetUrlScheme((const char*)url_ptr);
    url_len = (uint16)BRW_Strlen(url_ptr);
    if(PNULL == temp_url_ptr)//need to add http scheme
    {
        http_scheme_len = (uint16)BRW_Strlen((const uint8*)"http://");
        url_len += http_scheme_len;
    }
    else
    {
        http_scheme_len = 0;
        BRWMem_Free(temp_url_ptr);
        temp_url_ptr = PNULL;
    }

    temp_url_ptr = BRWMem_Alloc(url_len+2);//for '/'
    BRW_Memset(temp_url_ptr,0,url_len+2);
    if (http_scheme_len > 0)
    {
        BRW_Strcpy((uint8*)temp_url_ptr,(const uint8*)"http://");
    }
    BrwUrlRemoveFragment(url_ptr, (uint8*)(temp_url_ptr + http_scheme_len), (url_len - http_scheme_len));//NEWMS00147333
    
    host_end_ptr = HttpMovetoHostEnd(temp_url_ptr);
    if ((PNULL == host_end_ptr) || (0 == host_end_ptr[0]))
    {
        is_host_without_slash = TRUE;
    }
    //wap request must keep a copy of url
    if (is_host_without_slash && (temp_url_ptr[url_len-1] !=  '/'))
    {       
        //copy '/' at the end
        temp_url_ptr[url_len] = '/';
    }

#if defined(MMIDORADO_GOOGLE_TRANSCODE_SUPPORT)
    if(public_profile_ptr->is_gwt_enable)
    {
        connect_url_ptr = BrwGwtChangeUrl(temp_url_ptr,public_profile_ptr->lang);
        if(0==connect_url_ptr)
        {
        	//BRWDebug_Printf:"BRW_AccessPage, gwt fail:%s"
        	SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_661_112_2_18_2_6_50_354,(uint8*)"s",temp_url_ptr);
        	connect_url_ptr = temp_url_ptr;
        }
        else
        {
        	BrwUpdateUrl(brw_info_ptr,(uint8*)connect_url_ptr);
        }
    }
    else
    {
        connect_url_ptr = temp_url_ptr;
    }
#elif defined(MMIDORADO_WRE_TRANSCODE_SUPPORT) //for sprd wre transcoding
#ifndef WIN32
    if (!public_profile_ptr->is_use_wifi)
    {
#ifdef ENG_SUPPORT
        if(MMIAPIENG_GetIsBrowserChangeURL())
#endif
        {
            connect_url_ptr = BrwGwtChangeUrl(temp_url_ptr,public_profile_ptr->lang);
        }
        
    }
#endif    
    if(0==connect_url_ptr)
    {
    	connect_url_ptr = temp_url_ptr;
    }
    else
    {
        //do nothing
    }
#else
    connect_url_ptr = temp_url_ptr;
#endif

    url_len = (uint16)BRW_Strlen((const uint8*)connect_url_ptr);
    if(0 == url_len)
    {
        BRWDebug_PrintWarning("BRW_AccessPage,url_len 0!");
        return BRW_INVALID_REQ_ID;
    }
   //先停止当前请求
    result = BRW_StopPageReq(instance);
    if(brw_info_ptr->cur_display_req_ptr!=0)
    {
        BrwLayoutNavigateLostFocus(brw_info_ptr->cur_display_req_ptr->layout_handle);
    }
    else if(brw_info_ptr->snapshot_open_layout_handler!=0)
    {
        BrwLayoutNavigateLostFocus(brw_info_ptr->snapshot_open_layout_handler);
    }
    new_wap_req_ptr = (WAP_REQUEST_T*)BRWMem_Alloc(sizeof(WAP_REQUEST_T));
    //BRWDebug_Printf:"brw alloc wap request in BRW_AccessPage:%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_691_112_2_18_2_6_50_355,(uint8*)"d", new_wap_req_ptr);
    if(PNULL==new_wap_req_ptr)
    {
        BRWDebug_PrintWarning("BRW_AccessPage:malloc wap req failed!");
        return BRW_INVALID_REQ_ID;
    }
    BRW_Memset(new_wap_req_ptr,0,sizeof(WAP_REQUEST_T));

    new_wap_req_ptr->absolute_url = BRWMem_Alloc(url_len+1);
    if(PNULL==new_wap_req_ptr->absolute_url)
    {
        BRWDebug_PrintWarning("BRW_AccessPage:malloc uri failed!");
        BRWMem_Free(new_wap_req_ptr);
        return BRW_INVALID_REQ_ID;
    }
    //wap request must keep a copy of url
    BRW_Memset(new_wap_req_ptr->absolute_url, 0x00, (url_len+1));
    BRW_Memcpy(new_wap_req_ptr->absolute_url,connect_url_ptr,url_len);
    
    new_wap_req_ptr->original_url = BRWMem_Alloc(url_len+1);
    if(PNULL==new_wap_req_ptr->original_url)
    {
        BRWDebug_PrintWarning("BRW_AccessPage:malloc original uri failed!");
        BRWMem_Free(new_wap_req_ptr->absolute_url);
        BRWMem_Free(new_wap_req_ptr);
        return BRW_INVALID_REQ_ID;
    }
    BRW_Memcpy(new_wap_req_ptr->original_url,new_wap_req_ptr->absolute_url,url_len+2);
    
    if(connect_url_ptr!=temp_url_ptr)
    {
        BRWMem_Free(connect_url_ptr);
        connect_url_ptr= PNULL;
    }
    if((uint8 *)temp_url_ptr!=url_ptr)
    {
        BRWMem_Free(temp_url_ptr);
        temp_url_ptr = PNULL;
    }
    //generate unique req id
    new_wap_req_ptr->req_id = GenReqId();
    new_wap_req_ptr->method =BRW_HTTP_METHOD_GET;    
    //set state of request
    BrwSetWapReqState(new_wap_req_ptr,BRW_STATE_PAGE_REQUESTING);
    prev_wap_req_ptr=brw_info_ptr->cur_display_req_ptr;
    brw_info_ptr->cur_req_ptr = new_wap_req_ptr;

    if (!brw_info_ptr->is_fixed)
    {
#ifdef DYNAMIC_MAINMENU_SUPPORT
        new_wap_req_ptr->is_need_ico = TRUE;//for add url to main menu
#endif        
        ico_filename_len = BRW_WStrlen(ico_filename_ptr);
        if (ico_filename_len != 0)
        {
            new_wap_req_ptr->is_need_ico = TRUE;
            new_wap_req_ptr->ico_filename_ptr = BRWMem_Alloc((ico_filename_len+1) * sizeof(uint16));
            BRW_Memset(new_wap_req_ptr->ico_filename_ptr, 0, ((ico_filename_len+1) * sizeof(uint16)));
            BRW_WstrCat(new_wap_req_ptr->ico_filename_ptr, ico_filename_ptr);
        }
    }

/* BEGIN: Modified by George.Liu 01526, 2010/8/5   PN:MergeToMw1029forCustom */
    is_net_linked = BRW_IsNetLinked();

/* END:   Modified by George.Liu 01526, 2010/8/5   PN:MergeToMw1029forCustom */

    if(is_net_linked
        || (brw_info_ptr->is_fixed 
            && (BRW_PDP_STATUS_CONNECTING != BRW_GetPdpStatus()))
        )//physical linked
    {
        if(brw_info_ptr->http_state == BRW_HTTP_STATE_NONE ||
            brw_info_ptr->http_state == BRW_HTTP_STATE_CLOSING)
        {
            brw_info_ptr->http_context_id = 0;
            BrwInitHttpSession(brw_info_ptr);
            //SCI_TRACE_LOW:"BRW BRW_AccessPage init http session"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_753_112_2_18_2_6_51_356,(uint8*)"");
        }
        else if(brw_info_ptr->http_state == BRW_HTTP_STATE_INITED)
        {
            if(!BrwStartPageRequest(brw_info_ptr))
            {
                return BRW_INVALID_REQ_ID;
            }
        }
    }
    else
    {
        //BRWDebug_Printf:"BRW_AccessPage to ActiveNetwork"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_765_112_2_18_2_6_51_357,(uint8*)"");
        if(!ActiveNetwork(instance))
        {
            BRWDebug_PrintWarning("BRW_AccessPage, ActiveNetwork fail");
            BrwProcErr(brw_info_ptr,BRW_ERR_GPRS_LINK_FAILED);
        }
    }

    return new_wap_req_ptr->req_id;
}

PUBLIC uint32 BRW_AccessPage(BRW_INSTANCE instance,uint8* url_ptr)
{
    return BRW_AccessPageEx(instance, url_ptr, PNULL);
}

PUBLIC void BrwSetSnapshotOpenLayoutHandler(BRW_INSTANCE instance,LAYOUT_HANDLE handler)
{
	BRW_INFO_T* brw_info_ptr=PNULL;

	if(0==instance)
	{
		return;
	}
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if(0==brw_info_ptr)
    {
        BRWDebug_PrintWarning("BrwSetSnapshotOpenLayoutHandler:brw_info_ptr null!");
        return ;
    }
	brw_info_ptr->snapshot_open_layout_handler = handler;
}
PUBLIC LAYOUT_HANDLE BrwGetSnapshotOpenLayoutHandler(BRW_INSTANCE instance)
{
	BRW_INFO_T* brw_info_ptr=PNULL;

	if(PNULL == instance)
	{
		return PNULL;
	}
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if(PNULL == brw_info_ptr)
    {
        BRWDebug_PrintWarning("BrwGetSnapshotOpenLayoutHandler:brw_info_ptr null!");
        return PNULL;
    }
	return brw_info_ptr->snapshot_open_layout_handler;
}

PUBLIC BOOLEAN BRW_StopLayoutTimerById(BRW_INSTANCE instance, uint32 timer_id)
{
	BRW_INFO_T		*brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
	LAYOUT_HANDLE	handler = 0;

	if(0==brw_info_ptr)
	{
		BRWDebug_PrintWarning("BRW_ProcLayoutTimer,brw_info_ptr null");
		return FALSE;
	}
	if(0==brw_info_ptr->cur_display_req_ptr)
	{
		handler=brw_info_ptr->snapshot_open_layout_handler;
	}
	else
	{
		handler = brw_info_ptr->cur_display_req_ptr->layout_handle;
	}
	if(0==handler)
	{
		BRWDebug_PrintWarning("BRW_ProcLayoutTimer,layout handler null");
		return FALSE;
	}
	return BrwLayoutStopTimerById(handler, timer_id);
}
PUBLIC BOOLEAN BRW_ProcLayoutTimer(BRW_INSTANCE instance,uint32 timer_id)
{
	BRW_INFO_T		*brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
	LAYOUT_HANDLE	handler = 0;

	if(0==brw_info_ptr)	
	{
		BRWDebug_PrintWarning("BRW_ProcLayoutTimer,brw_info_ptr null");
		return FALSE;
	}
	if(0==timer_id)
	{
		BRWDebug_PrintWarning("BRW_ProcLayoutTimer,timer_id null");
		return FALSE;
	}
	if(0==brw_info_ptr->cur_display_req_ptr)
	{
		handler=brw_info_ptr->snapshot_open_layout_handler;
	}
	else
	{
		handler = brw_info_ptr->cur_display_req_ptr->layout_handle;
	}
	if(0==handler)
	{
		BRWDebug_PrintWarning("BRW_ProcLayoutTimer,layout handler null");
		return FALSE;
	}
	BrwProcessTimerEvent(handler,timer_id);
	return TRUE;
}
/*
* Description:display current web page
* Param: instance   instance handle of browse 
* Return: id of page req   
*/
PUBLIC BOOLEAN BRW_DisplayPage(BRW_INSTANCE instance)
{
    BRW_INFO_T		*brw_info_ptr=PNULL;
    WAP_REQUEST_T	*wap_request_ptr=PNULL;
    BOOLEAN			result = TRUE;
    LAYOUT_HANDLE	layout_handler = 0;

    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if(PNULL == brw_info_ptr)
    {
        //BRWDebug_Printf:"BRW_DisplayPage,brw_info_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_884_112_2_18_2_6_51_358,(uint8*)"");
        return FALSE;
    }
    wap_request_ptr=brw_info_ptr->cur_display_req_ptr;
    if( PNULL == wap_request_ptr)
    {
        layout_handler = brw_info_ptr->snapshot_open_layout_handler;
    }
    else
    {
        layout_handler = wap_request_ptr->layout_handle;
    }

    if(0 == layout_handler)
    {
        //BRWDebug_Printf:"BRW_DisplayPage,layout_handler null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_899_112_2_18_2_6_51_359,(uint8*)"");
        return FALSE;
    }
    result = BrwDisplayWebPage(brw_info_ptr->profile.win_handle,layout_handler);

    //BRWDebug_Printf:"BrwDisplayWebPage ret = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_904_112_2_18_2_6_51_360,(uint8*)"d", result);
    return result;
}

PUBLIC BOOLEAN BRW_RepaintPage(BRW_INSTANCE instance)
{
    BRW_INFO_T		*brw_info_ptr=PNULL;
    WAP_REQUEST_T	*wap_request_ptr=PNULL;
    BOOLEAN			result = TRUE;
	LAYOUT_HANDLE	layout_handler=0;

    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if(PNULL==brw_info_ptr)
    {
        BRWDebug_PrintWarning("BRW_RepaintPage,brw_info_ptr null");
        return FALSE;
    }
    wap_request_ptr=brw_info_ptr->cur_display_req_ptr;
    if(PNULL==wap_request_ptr)
    {
		layout_handler = brw_info_ptr->snapshot_open_layout_handler;
    }
	else
	{
		layout_handler = wap_request_ptr->layout_handle;
	}
	if(0==layout_handler)
	{
		//BRWDebug_Printf:"BRW_RepaintPage,layout_handler null"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_932_112_2_18_2_6_51_361,(uint8*)"");
		return FALSE;
	}
    result = BrwRepaintWebPage(brw_info_ptr->profile.win_handle,layout_handler);
    
    //BRWDebug_Printf:"BRW_RepaintPage ret = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_937_112_2_18_2_6_51_362,(uint8*)"d", result);

    return result;
}
/*
* Description:stop current page request
* Param:instance   instance handle of browser 
* Return:    
*/
PUBLIC BOOLEAN BRW_StopPageReq(BRW_INSTANCE instance)
{
    BRW_INFO_T  *brw_info_ptr=PNULL;
    WAP_REQUEST_T   *wap_request_ptr=PNULL;
    BRW_REQ_STATE_E state = BRW_STATE_MAX;
    BOOLEAN result = TRUE;

    //BRWDebug_Printf:"[BRW]BRW_StopPageReq"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_952_112_2_18_2_6_51_363,(uint8*)"");
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if(PNULL == brw_info_ptr)
    {
        BRWDebug_PrintWarning("BRW_StopPageReq,brw_info_ptr null");
        return FALSE;
    }
    
    wap_request_ptr=brw_info_ptr->cur_req_ptr;
    if(PNULL == wap_request_ptr)
    {
        //BRWDebug_Printf:"BRW_StopPageReq,wap_request_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_963_112_2_18_2_6_51_364,(uint8*)"");
        return FALSE;
    }
    
#ifdef MMIDORADO_BGSOUND_SUPPORT
    StopAndFreeBGsound(PNULL, brw_info_ptr);
#endif

    state = BrwGetWapReqState(wap_request_ptr);
    //BRWDebug_Printf:"BRW_StopPageReq,state =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_972_112_2_18_2_6_51_365,(uint8*)"d", state);

    switch (state)
    {
    case BRW_STATE_PAGE_REQUESTING:
    {
        //BrwFreeWapReqStruct会停掉http并释放相关资源
        BrwFreeWapReqStruct(wap_request_ptr);
        
        //将当前显示请求设置为当前请求
        if(brw_info_ptr->cur_req_ptr != brw_info_ptr->cur_display_req_ptr)
        {
            brw_info_ptr->cur_req_ptr = brw_info_ptr->cur_display_req_ptr;
        }
        else
        {
            brw_info_ptr->cur_req_ptr = PNULL;
            brw_info_ptr->cur_display_req_ptr = PNULL;
        }
        brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_PAGE_STOP_IND,PNULL,0);		
    }
        break;
        
    case BRW_STATE_MEDIA_REQUESTING:
    {
        //停止在途子媒体http请求,销毁子媒体请求队列       
        BRWList_Destruct(&wap_request_ptr->media_obj_info.meida_list,(BRW_LIST_FREE_F)FreeMediaObjStruct);
        BRW_Memset(&wap_request_ptr->media_obj_info, 0, sizeof(MEDIA_OBJ_INFO_T));  
        BrwSetWapReqState(wap_request_ptr, BRW_STATE_ALL_FINISHED);
        brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_PAGE_STOP_IND,PNULL,0);
    }
        break;
        
    default:
        break;
    }

    return  result;
}
/*
 *	desc:stop download which req_id equal req_id
 *  param:instance   instance handle of browser 
 *        req_id     request id which identify download request
 *  return:TRUE if success,else false
 */
PUBLIC BOOLEAN BRW_StopDL(BRW_INSTANCE instance,uint32 req_id)
{
    WAP_REQUEST_T	*wap_req_ptr=PNULL;
    BRW_INFO_T		*brw_info_ptr=BrwGetBrwInfoPtrByInstance(instance);
	
    //BRWDebug_Printf:"[BRW]BRW_StopDL"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1021_112_2_18_2_6_51_366,(uint8*)"");
    if ((BRW_INVALID_REQ_ID == req_id) || (PNULL == brw_info_ptr))
    {
        //BRWDebug_Printf:"[BRW]BRW_StopDL,brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1024_112_2_18_2_6_51_367,(uint8*)"d",brw_info_ptr);
        return FALSE;
    }
    wap_req_ptr = BrwGetWapReq(req_id,brw_info_ptr);
    if ((PNULL == wap_req_ptr) || (BrwGetWapReqState(wap_req_ptr) != BRW_STATE_DOWNLOADING))
    {
        //BRWDebug_Printf:"[BRW]BRW_StopDL,invalid param,wap_req_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1030_112_2_18_2_6_51_368,(uint8*)"d",wap_req_ptr);
        return FALSE;
    }
    
    return BrwCancelDL(wap_req_ptr,brw_info_ptr);
}
/*
 *	desc:to check if current displaying page is Blank
 *  param:instance   instance handle of browser 
 *  return:TRUE is blank, FALSE is NOT
 */
PUBLIC BOOLEAN BRW_IsPageBlank(BRW_INSTANCE instance)
{
    BRW_INFO_T		*brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);

    if(0==brw_info_ptr)
    {
        //BRWDebug_Printf:"[BRW]BRW_CheckIfPageNull,brw_info_ptr 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1046_112_2_18_2_6_51_369,(uint8*)"");
        return TRUE;
    }
    return (brw_info_ptr->cur_display_req_ptr==0);
}

/*
@desc:get state of current page request
@param:instance   instance handle of browse 
@return:state of current page request;
*/
PUBLIC BRW_REQ_STATE_E Brw_GetPageReqState(BRW_INSTANCE instance)
{
    BRW_INFO_T		*brw_info_ptr=PNULL;
    WAP_REQUEST_T	*wap_request_ptr=PNULL;
    BRW_REQ_STATE_E state = BRW_STATE_MAX;
	
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if(!brw_info_ptr)
    {
        BRWDebug_PrintWarning("Brw_GetPageReqState:brw_info_ptr null");
        return BRW_STATE_MAX;
    }
    wap_request_ptr=brw_info_ptr->cur_req_ptr;
    if(PNULL==wap_request_ptr)
    {
        BRWDebug_PrintWarning("Brw_GetPageReqState,wap_request_ptr null");
        return BRW_STATE_MAX;
    }
    state = BrwGetWapReqState(wap_request_ptr);
    return state;
}
/*
@Desc:process the key event of ui
@param: instance   instance handle of browse 
*       key_event   key event type
@return:TRUE if success,else FALSE
*/
PUBLIC BOOLEAN  BRW_ProcKeyEvent(BRW_INSTANCE instance,BRW_KEY_EVENT_E key_event)
{
    BRW_INFO_T		*brw_info_ptr=PNULL;
    WAP_REQUEST_T	*wap_request_ptr=PNULL;
	LAYOUT_HANDLE	layout_handler = 0;
    BOOLEAN			result = TRUE;

    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);

	MMI_CheckAllocatedMemInfo();

    if(NULL == brw_info_ptr)
    {
        //BRWDebug_Printf:"BRW_ProcKeyEvent:brw_info_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1092_112_2_18_2_6_51_370,(uint8*)"");
        return FALSE;
    }
    wap_request_ptr=brw_info_ptr->cur_display_req_ptr;
    if(NULL == wap_request_ptr)
    {
        layout_handler = brw_info_ptr->snapshot_open_layout_handler;
    }
    else
    {
        /*MS00192364:是否stop page req,在start new page时会调用，此处过早stop
          if(wap_request_ptr!=brw_info_ptr->cur_req_ptr && key_event==BRW_KEY_EVENT_OK)
          {
              BRW_StopPageReq(instance);
          }
        */
        layout_handler = wap_request_ptr->layout_handle;
    }
    if(layout_handler != BRW_INVALID_LAYOUT_HANDLE)
    {
        switch(key_event) 
        {
        case BRW_KEY_EVENT_LEFT:
            result = BrwLayoutPageUp(layout_handler);
            if(!result) 
            {
                result = BrwLayoutMoveFocused(layout_handler,BRW_MOVE_TO_FIRST);
                if(!result) 
                {
                    goto key_up;
                }
            }
            break;
        case BRW_KEY_EVENT_LEFT_REPEAT:
            result = BrwLayoutPageUp(layout_handler);
            if(!result) 
            {
                result = BrwLayoutMoveFocused(layout_handler, BRW_DIRECTION_UP);
                if(!result) 
                {
                    goto up_keep;
                }
            }
            break;
        case BRW_KEY_EVENT_RIGHT:
            result = BrwLayoutPageDown(layout_handler);
            if(!result) 
            {
                /*last page. right key same as down key*/
                result = BrwLayoutMoveFocused(layout_handler,BRW_MOVE_TO_LAST);
                if(!result) 
                {
                    goto key_down;
                }
            }
            break;
        case BRW_KEY_EVENT_RIGHT_REPEAT:
            result = BrwLayoutPageDown(layout_handler);
            if(!result) 
            {
                result = BrwLayoutLineDown(layout_handler);
                if(!result) 
                {
                    goto down_keep;
                }
            }
            break;
        case BRW_KEY_EVENT_DOWN:
key_down:
            result = BrwLayoutMoveFocused(layout_handler, BRW_DIRECTION_RIGHT);
            break;
        case BRW_KEY_EVENT_UP:
key_up:
            result = BrwLayoutMoveFocused(layout_handler, BRW_DIRECTION_LEFT);
            break;
        case BRW_KEY_EVENT_DOWN_REPEAT:
down_keep:
            result = BrwLayoutMoveFocused(layout_handler, BRW_DIRECTION_DOWN);
            if(!result) 
            {
                goto key_down;
            }
            break;
        case BRW_KEY_EVENT_UP_REPEAT:
up_keep:
            result = BrwLayoutMoveFocused(layout_handler, BRW_DIRECTION_UP);
            if(!result) 
            {
                goto key_up;
            }
            break;
        case BRW_KEY_EVENT_OK:
            result = BrwLayoutClickFocused(layout_handler);
            break;
        case BRW_KEY_EVENT_STAR: /* '*' */
        case BRW_KEY_EVENT_HASH: /* '#' */
        case BRW_KEY_EVENT_1:
        case BRW_KEY_EVENT_2:
        case BRW_KEY_EVENT_3:
        case BRW_KEY_EVENT_4:
        case BRW_KEY_EVENT_5:
        case BRW_KEY_EVENT_6:
        case BRW_KEY_EVENT_7:
        case BRW_KEY_EVENT_8:
        case BRW_KEY_EVENT_9:
            /*WAP access key, parser and layout have supported it, skip it now*/
            break;
        default:
            break;
        }
        
    }
    else
    {
        //BRWDebug_Printf:"BRW_ProcKeyEvent,layout_handle invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1206_112_2_18_2_6_52_371,(uint8*)"");
        result = FALSE;
    }
    return result;
}
/*
@Desc:process the pen down event
@param: instance   instance handle of browse 
        pen down relevant info
@return:TRUE if success,else FALSE
*/
PUBLIC BOOLEAN  BRW_ProcPenDownEvent(BRW_INSTANCE instance,BRW_PEN_CLICK_INFO_T *pendow_info_ptr)
{
    BRW_INFO_T		*brw_info_ptr=PNULL;
    WAP_REQUEST_T	*wap_request_ptr=PNULL;
	LAYOUT_HANDLE	layout_handler = 0;
    BOOLEAN			result = TRUE;

    if(PNULL==pendow_info_ptr)
    {
        BRWDebug_PrintWarning("BRW_ProcPenDownEvent,pendow_info_ptr null");
        return FALSE;
    }
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if(!brw_info_ptr)
    {
        //BRWDebug_Printf:"BRW_ProcPenDownEvent:brw_info_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1231_112_2_18_2_6_52_372,(uint8*)"");
        return FALSE;
    }
    wap_request_ptr=brw_info_ptr->cur_display_req_ptr;
    if(wap_request_ptr==0)
    {
        layout_handler = brw_info_ptr->snapshot_open_layout_handler;
    }
    else
    {
        /*MS00192364:是否stop page req,在start new page时会调用，此处过早stop
          if(wap_request_ptr!=brw_info_ptr->cur_req_ptr)
          {
            BRW_StopPageReq(instance);
          }
        */
        layout_handler = wap_request_ptr->layout_handle;
    }

    if(layout_handler!=BRW_INVALID_LAYOUT_HANDLE)    
	{
        result=BrwLayoutPenDown(layout_handler,pendow_info_ptr);
    }
    else
    {
        //BRWDebug_Printf:"BRW_ProcPenDownEvent,layout_handle invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1256_112_2_18_2_6_52_373,(uint8*)"");
        return FALSE;
    }
    return result;
}
PUBLIC BOOLEAN  BRW_ProcPenUpEvent(BRW_INSTANCE instance,BRW_PEN_CLICK_INFO_T *penup_info_ptr)
{
    BRW_INFO_T		*brw_info_ptr=PNULL;
    WAP_REQUEST_T	*wap_request_ptr=PNULL;
	LAYOUT_HANDLE	layout_handler = 0;
    BOOLEAN			result = TRUE;

    if(PNULL==penup_info_ptr)
    {
        BRWDebug_PrintWarning("BRW_ProcPenUpEvent,pendow_info_ptr null");
        return FALSE;
    }
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if(!brw_info_ptr)
    {
        //BRWDebug_Printf:"BRW_ProcPenUpEvent:brw_info_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1276_112_2_18_2_6_52_374,(uint8*)"");
        return FALSE;
    }
    wap_request_ptr=brw_info_ptr->cur_display_req_ptr;
	if(wap_request_ptr==0)
	{
		layout_handler = brw_info_ptr->snapshot_open_layout_handler;
	}
	else
	{
		/*MS00192364:是否stop page req,在start new page时会调用，此处过早stop
		if(wap_request_ptr!=brw_info_ptr->cur_req_ptr)
		{
			BRW_StopPageReq(instance);
		}*/
		layout_handler = wap_request_ptr->layout_handle;
	}
    if(layout_handler!=BRW_INVALID_LAYOUT_HANDLE)    
    {
        result=BrwLayoutPenUp(layout_handler,penup_info_ptr);
    }
    else
    {
        //BRWDebug_Printf:"BRW_ProcPenUpEvent,layout_handle invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1299_112_2_18_2_6_52_375,(uint8*)"");
        return FALSE;
    }
    return result;
}

/*
@desc:process page navigate event(such as forward,backward)
@param:instance   instance handle of browse 
       nav_event  page navigate event type
@return:TRUE if success,else FALSE
*/
PUBLIC BOOLEAN  BRW_ProcNavEvent(BRW_INSTANCE instance,BRW_NAVIGATE_EVENT_E nav_event)
{
    BRW_INFO_T  *brw_info_ptr = PNULL;
    WAP_REQUEST_T   *wap_request_ptr = PNULL;
    uint8   *new_uri_ptr = PNULL;
    uint8   *cur_uri_ptr = PNULL;
    uint32  new_req_id = BRW_INVALID_REQ_ID;
    BOOLEAN is_card_jumping = FALSE;

    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if (PNULL == brw_info_ptr)
    {
        //BRWDebug_Printf:"BRW_ProcNavEvent:brw_info_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1322_112_2_18_2_6_52_376,(uint8*)"");
        return FALSE;
    }
	
    cur_uri_ptr = BrwNavHisGetCurUrl();
    if (PNULL == cur_uri_ptr)
    {
        //BRWDebug_Printf:"[BRW]BRW_ProcNavEvent:cur_uri_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1329_112_2_18_2_6_52_377,(uint8*)"");
        return FALSE;
    }
    
   //调用历史记录接口获取uri
    switch (nav_event)
    {
    case BRW_NAV_EVENT_BACKWARD:
        new_uri_ptr=BrwGetBackwardUrl();        
        break;
    case BRW_NAV_EVENT_FORWARD:
        new_uri_ptr=BrwGetForwardUrl();        
        break;
    case BRW_NAV_EVENT_REFRESH:
        new_uri_ptr=BrwNavHisGetCurUrl();        
        break;
    default:
        break;
    }
    
    if (PNULL == new_uri_ptr)
    {
        //BRWDebug_Printf:"[BRW]BRW_ProcNavEvent:new_uri_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1351_112_2_18_2_6_52_378,(uint8*)"");
        return FALSE;
    }
	
    wap_request_ptr = brw_info_ptr->cur_display_req_ptr;
    if (PNULL == wap_request_ptr)
    {
        //BRWDebug_Printf:"BRW_ProcNavEvent,snapshot nav_event:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1358_112_2_18_2_6_52_379,(uint8*)"d",nav_event);
    }
    
    if ((wap_request_ptr != brw_info_ptr->cur_req_ptr) && (PNULL != brw_info_ptr->cur_req_ptr))
    {
        BRW_StopPageReq(instance);
    }

    //if is page card jumping
    if(BrwIsUrlIsEquExceptFragment(cur_uri_ptr,new_uri_ptr))
    {
        //BRWDebug_Printf:"[BRW]BRW_ProcNavEvent BrwIsUrlIsEquExceptFragment"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1369_112_2_18_2_6_52_380,(uint8*)"");
        if (BRW_NAV_EVENT_BACKWARD == nav_event)
        {
            is_card_jumping = BrwIsJumpByBackward(brw_info_ptr->cur_display_req_ptr);
        }
        else
        {
            is_card_jumping = TRUE;
        }
    }
    
    if (is_card_jumping)
    {
        //BRWDebug_Printf:"BRW_ProcNavEvent,card jumping,event:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1382_112_2_18_2_6_52_381,(uint8*)"d",nav_event);
        if (HandleCardJumping(brw_info_ptr,(const char*)new_uri_ptr,nav_event))
        {
            if (BRW_NAV_EVENT_BACKWARD == nav_event)
            {
                BrwNavHisBackward();
            }
            else if (BRW_NAV_EVENT_FORWARD == nav_event)
            {
                BrwNavHisForward();
            }
            return TRUE;
        }
        return FALSE;
    }

    //to use nav cache
    if (BRW_NAV_EVENT_BACKWARD == nav_event)
    {
        if(BrwBackwardUsingCacheReq(brw_info_ptr))
        {
            return TRUE;
        }
    }

    new_req_id=BRW_AccessPage(instance,new_uri_ptr);
    if (BRW_INVALID_REQ_ID==new_req_id)
    {
        BRWDebug_PrintWarning("BRW_ProcNavEvent:BRW_AccessPage failed");
        return FALSE;
    }
    else
    {
        BRW_MSG_NEW_PAGE_REQ_IND_T ind ={0};
        
        wap_request_ptr = BrwGetWapReq(new_req_id, brw_info_ptr);
        if (PNULL == wap_request_ptr)
        {
            BRWDebug_PrintWarning("BRW_ProcNavEvent:wap_request_ptr null");
            BrwProcErr((BRW_INFO_T*)instance,BRW_ERR_UNKNOWN);
            return FALSE;
        }
        
        wap_request_ptr->cur_nav_event = nav_event;
        BRW_Strncpy(ind.url_arr, new_uri_ptr, BRW_MAX_URL_LEN);
        brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_NEW_PAGE_REQ_IND,&ind,sizeof(BRW_MSG_NEW_PAGE_REQ_IND_T));
        return TRUE;
    }        
}

LOCAL BOOLEAN BrwBackwardUsingCacheReq(BRW_INFO_T *brw_info_ptr)
{
    WAP_REQUEST_T *cache_req_ptr = BrwNavHisGetBackwardCacheReq();
    BRW_MSG_NEW_PAGE_REQ_IND_T	ind ={0};

    if((PNULL == cache_req_ptr) || (PNULL == brw_info_ptr))
    {
        return FALSE;
    }

    //save current display page content
    if(brw_info_ptr->cur_page_content.page_content_ptr)
    {
        //BRWMem_Free(brw_info_ptr->cur_page_content.page_content_ptr);
        brw_info_ptr->cur_page_content.page_content_ptr = 0;
        brw_info_ptr->cur_page_content.page_content_len = 0;
        brw_info_ptr->cur_page_content.page_mime_type = BRW_MIME_UNKOWN;
    }
    if(brw_info_ptr->cur_page_content.charset_ptr)
    {
        BRWMem_Free(brw_info_ptr->cur_page_content.charset_ptr);
        brw_info_ptr->cur_page_content.charset_ptr = PNULL;
    }
    if(cache_req_ptr->http_req_ptr->charset != PNULL)
    {
        brw_info_ptr->cur_page_content.charset_ptr = BRWMem_Alloc(BRW_Strlen(cache_req_ptr->http_req_ptr->charset)+1);
        if(0 != brw_info_ptr->cur_page_content.charset_ptr)
        {
            BRW_Memset(brw_info_ptr->cur_page_content.charset_ptr,0,BRW_Strlen(cache_req_ptr->http_req_ptr->charset)+1);
            BRW_Strcpy(brw_info_ptr->cur_page_content.charset_ptr,cache_req_ptr->http_req_ptr->charset);
        }
    }
    brw_info_ptr->cur_page_content.page_content_ptr = cache_req_ptr->http_req_ptr->resp_buf_ptr;
    brw_info_ptr->cur_page_content.page_content_len = cache_req_ptr->http_req_ptr->resp_buf_now_len;
    brw_info_ptr->cur_page_content.page_mime_type = cache_req_ptr->http_req_ptr->mime_type;	

    //set the browser info
    if ((brw_info_ptr->cur_display_req_ptr != PNULL) 
        && !brw_info_ptr->cur_display_req_ptr->is_cached_req)
    {
        BrwFreeWapReqStruct(brw_info_ptr->cur_display_req_ptr);		
    }
    brw_info_ptr->cur_req_ptr = cache_req_ptr;
    brw_info_ptr->cur_display_req_ptr = cache_req_ptr;

    //Set the Nav record
    BrwNavHisBackward();

    //callback app
    BRW_Strncpy(ind.url_arr, BrwNavHisGetCurUrl(), BRW_MAX_URL_LEN);
    BrwLayoutNavigateGetFocus(brw_info_ptr->cur_display_req_ptr->layout_handle, BRW_NAV_EVENT_BACKWARD, ind.url_arr, FALSE);
    brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_NEW_PAGE_REQ_IND,&ind,sizeof(BRW_MSG_NEW_PAGE_REQ_IND_T));
    brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_DISPLAY_READY_IND,PNULL,0);
    return TRUE;
}
/*
@desc:process page scroll event
@param:instance   instance handle of browse 
        scroll_event  scroll event type
        top_index top index, only useful when scroll_event is BRW_SCROLL_EVENT_MOVE_*
@return:TRUE if success,else FALSE
*/
PUBLIC BOOLEAN  BRW_ProcScrollEvent(BRW_INSTANCE instance,BRW_SCROLL_EVENT_E scroll_event, uint32 top_index)
{
    BRW_INFO_T		*brw_info_ptr=PNULL;
    WAP_REQUEST_T	*wap_request_ptr=PNULL;
    BOOLEAN			result = TRUE;
    LAYOUT_HANDLE	layout_handler = 0;
    
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if(PNULL == brw_info_ptr)
    {
        //BRWDebug_Printf:"BRW_ProcScrollEvent:brw_info_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1503_112_2_18_2_6_52_382,(uint8*)"");
        return FALSE;
    }
    wap_request_ptr=brw_info_ptr->cur_display_req_ptr;
    if(PNULL==wap_request_ptr)
    {
        layout_handler = brw_info_ptr->snapshot_open_layout_handler;
    }
    else
    {
        layout_handler = wap_request_ptr->layout_handle;
    }
    if(layout_handler != BRW_INVALID_LAYOUT_HANDLE)
    {
        switch(scroll_event) 
        {
        case BRW_SCROLL_EVENT_UPLINE:
            result = BrwLayoutLineUp(layout_handler);
            break;
        case BRW_SCROLL_EVENT_DOWNLINE:
            result = BrwLayoutLineDown(layout_handler);
            break;
        case BRW_SCROLL_EVENT_UPPAGE:
            result = BrwLayoutPageUp(layout_handler);
            break;
        case BRW_SCROLL_EVENT_DOWNPAGE:
            result = BrwLayoutPageDown(layout_handler);
            break;
        case BRW_SCROLL_EVENT_MOVE_VERTICAL:
            result = BrwLayoutMoveVertical(layout_handler, top_index);
            break;
        default:
            break;
        }
    }
    else
    {
        BRWDebug_PrintWarning("BRW_ProcScrollEvent,layout_ctrl_ptr invalid");
        result = FALSE;
    }
    return result;    
}

/*
@desc:process tp scroll event
@param:instance   instance handle of browse 
        move_y  vertical move position
@return:TRUE if success,else FALSE
*/
PUBLIC BOOLEAN  BRW_ProcTpScrollEvent(BRW_INSTANCE instance, int16 move_y)
{
    BRW_INFO_T		*brw_info_ptr=PNULL;
    WAP_REQUEST_T	*wap_request_ptr=PNULL;
    BOOLEAN			result = TRUE;
    LAYOUT_HANDLE	layout_handler = 0;

    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if(!brw_info_ptr)
    {
        //BRWDebug_Printf:"BRW_ProcTpScrollEvent:brw_info_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1561_112_2_18_2_6_52_383,(uint8*)"");
        return FALSE;
    }
    wap_request_ptr=brw_info_ptr->cur_display_req_ptr;
    if(PNULL == wap_request_ptr)
    {
        layout_handler = brw_info_ptr->snapshot_open_layout_handler;
    }
    else
    {
        layout_handler = wap_request_ptr->layout_handle;
    }
    if(layout_handler != BRW_INVALID_LAYOUT_HANDLE)
    {
        result=BrwProcTpScroll(layout_handler, move_y);
    }
    else
    {
        BRWDebug_PrintWarning("BRW_ProcTpScrollEvent,layout_ctrl_ptr invalid");
        return FALSE;
    }
    return result;    
}

/*
@desc:get current page title(include snapshot)
@param: instance   instance handle of browse 
        buf         input buffer to save title
        title_len_ptr   real title len(output)
        buf_max_len max len of buf
@return:TRUE if success,else FALSE
*/
PUBLIC BOOLEAN  BRW_GetPageTitle(BRW_INSTANCE instance,
                uint16* buf,//[IN/OUT]
                uint16* title_len_ptr,//[IN/OUT]
                uint16 buf_max_len//not include an end flag
                )
{
    BOOLEAN			result = TRUE;
    LAYOUT_HANDLE   layout_handle = BRW_INVALID_LAYOUT_HANDLE;

    layout_handle = GetCurDisplayLayout(instance);
    
    if(layout_handle != BRW_INVALID_LAYOUT_HANDLE)
    {
        result = BrwGetPageTitle(layout_handle, buf, buf_max_len, title_len_ptr);
    }
    else
    {
        BRWDebug_PrintWarning("BRW_GetPageTitle,layout_ctrl_ptr invalid");
        return FALSE;
    }   
    return result; 
}

/*
@desc:get current page url(without snapshot)
@return:TRUE if success,else FALSE
*/
PUBLIC BOOLEAN  BRW_GetPageUrl(BRW_INSTANCE instance,
                uint8 *url_buf_ptr, //[IN/OUT]
                uint16 buf_max_len, //not include an end flag
                BOOLEAN *is_snapshot_ptr,//[IN/OUT]
                BOOLEAN *is_blank_page_ptr//[IN/OUT]
                )
{
    BRW_INFO_T  *brw_info_ptr = PNULL;
    BOOLEAN is_snapshot = FALSE;
    uint8   *url_ptr = PNULL;
    uint16  url_len = 0;
    BOOLEAN is_blank_page = FALSE;
    
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if (PNULL == brw_info_ptr)
    {
        //BRWDebug_Printf:"BRW_GetPageUrl:brw_info_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1634_112_2_18_2_6_52_384,(uint8*)"");
        return FALSE;
    }

    if (PNULL != brw_info_ptr->cur_display_req_ptr)
    {
        url_ptr = brw_info_ptr->cur_display_req_ptr->absolute_url;
    }
    else
    {
        is_blank_page = TRUE;
        //BRWDebug_Printf:"BRW_GetPageUrl,cur_display_req_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1645_112_2_18_2_6_52_385,(uint8*)"");
        if (brw_info_ptr->snapshot_open_layout_handler != PNULL)
        {
            //BRWDebug_Printf:"[brw] is snapshot"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1648_112_2_18_2_6_53_386,(uint8*)"");
            is_snapshot = TRUE;
        }
    }
    
    if (PNULL != is_snapshot_ptr)
    {
        *is_snapshot_ptr = is_snapshot;
    }
    if (PNULL != is_blank_page_ptr)
    {
        *is_blank_page_ptr = is_blank_page;
    }
    if (PNULL != url_buf_ptr)
    {
        uint8   *temp_url = PNULL;
        
#ifndef MMIDORADO_WRE_TRANSCODE_SUPPORT//just for chinese 
        temp_url = url_ptr;
#else
        temp_url = BrwGwtMovetoGwtEnd(url_ptr);//for sprd wre transcoding
#endif

        url_len = BRW_Strlen(temp_url);
        BRW_Memcpy(url_buf_ptr, temp_url, MIN(url_len, buf_max_len));
    }
    else
    {
        //BRWDebug_Printf:"BRW_GetPageUrl:url_pptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1668_112_2_18_2_6_53_387,(uint8*)"");
    }

    return TRUE; 
}

/*
 *desc:	process Init Http cnf
 *param:brw_info_ptr browser instance
 *      is_success is http init success
 */
PUBLIC void BrwProcInitHttpCnf(BRW_INFO_T* brw_info_ptr,BOOLEAN is_success)
{
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    //SCI_TRACE_LOW:"BRW BrwProcInitHttpCnf"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1682_112_2_18_2_6_53_388,(uint8*)"");

    if (PNULL == brw_info_ptr)
    {
        //SCI_TRACE_LOW:"BRW BrwProcInitHttpCnf brw_info_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1686_112_2_18_2_6_53_389,(uint8*)"");
        return;
    }
	if(is_success)
	{
        //SCI_TRACE_LOW:"BRW BrwProcInitHttpCnf success"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1691_112_2_18_2_6_53_390,(uint8*)"");
		BrwSetHttpState(brw_info_ptr, BRW_HTTP_STATE_INITED);
		if(brw_info_ptr->is_need_to_close_http)
		{
            //SCI_TRACE_LOW:"BRW BrwProcInitHttpCnf is_need_to_close_http"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1695_112_2_18_2_6_53_391,(uint8*)"");
			BrwSetHttpState(brw_info_ptr, BRW_HTTP_STATE_CLOSING);
			BrwCloseHttpSession(brw_info_ptr->http_context_id, public_profile_ptr->protocol);
			if(brw_info_ptr->cur_req_ptr!=0 
               && BrwGetWapReqState(brw_info_ptr->cur_req_ptr)==BRW_STATE_PAGE_REQUESTING 
               && (BRW_IsNetLinked() || brw_info_ptr->is_fixed)//is_fixed = TRUE:for access the webpage by cache
               )
			{
                //SCI_TRACE_LOW:"BRW BrwProcInitHttpCnf 1"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1703_112_2_18_2_6_53_392,(uint8*)"");
				brw_info_ptr->http_context_id = 0;
				BrwInitHttpSession(brw_info_ptr);
			}
		}
		else
		{
            //SCI_TRACE_LOW:"BRW BrwProcInitHttpCnf 2"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1710_112_2_18_2_6_53_393,(uint8*)"");
			if(brw_info_ptr->cur_req_ptr!=0 
               && BrwGetWapReqState(brw_info_ptr->cur_req_ptr)==BRW_STATE_PAGE_REQUESTING 
               && (BRW_IsNetLinked() || brw_info_ptr->is_fixed)//is_fixed = TRUE:for access the webpage by cache
               )
			{
                //SCI_TRACE_LOW:"BRW BrwProcInitHttpCnf 3"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1716_112_2_18_2_6_53_394,(uint8*)"");
				BrwStartPageRequest(brw_info_ptr);
			}
		}
	}
	else
	{
        //SCI_TRACE_LOW:"BRW BrwProcInitHttpCnf 4"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1723_112_2_18_2_6_53_395,(uint8*)"");
		BrwSetHttpState(brw_info_ptr, BRW_HTTP_STATE_NONE);
		BrwProcErr(brw_info_ptr, BRW_ERR_HTTP_INIT_FAILED);
	}
	brw_info_ptr->is_need_to_close_http = FALSE;
}
/*
 *	process error situation
 */
PUBLIC void BrwProcErr(BRW_INFO_T *brw_info_ptr,BRW_ERROR_E err)
{
    //BRWDebug_Printf:"[BRW]BrwProcErr,err=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1733_112_2_18_2_6_53_396,(uint8*)"d", err);
    
    if (brw_info_ptr != PNULL)
    {
        BRW_MSG_ERROR_IND_T err_ind ={0};
        BOOLEAN is_downloanding = FALSE;
        WAP_REQUEST_T   *wap_req_ptr = PNULL;
        uint32  req_id = MMIBROWSER_GetReqHandle();

        if (req_id != BRW_INVALID_REQ_ID)//MS00218497
        {
            wap_req_ptr = BrwGetWapReq(req_id, brw_info_ptr);
            if((wap_req_ptr != PNULL) && (BrwGetWapReqState(wap_req_ptr) == BRW_STATE_DOWNLOADING))
            {
                is_downloanding = TRUE;
            }
        }
        
        if (is_downloanding)/*报错，停止下载*/
        {        
            BrwProcDownloadErr(brw_info_ptr, wap_req_ptr, err);
        }
        else
        {
            BRW_StopPageReq(brw_info_ptr->instance_id);
            if ((brw_info_ptr->cur_display_req_ptr != PNULL) && (brw_info_ptr->cur_display_req_ptr->layout_handle != PNULL))
            {
                BrwLayoutNavigateGetFocus(brw_info_ptr->cur_display_req_ptr->layout_handle, BRW_NAV_EVENT_NULL, BrwNavHisGetCurUrl(), TRUE);
            }
            err_ind.error = err;
            brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_ERROR_IND, &err_ind, sizeof(BRW_MSG_ERROR_IND_T));
        }
        
    }
}
/*
 *	process download error situation
 */
PUBLIC void BrwProcDownloadErr(BRW_INFO_T* brw_info_ptr,WAP_REQUEST_T* dl_req_ptr,BRW_ERROR_E err)
{
    BRW_MSG_DOWNLOAD_ERROR_IND_T err_ind={0};

    //BRWDebug_Printf:"[BRW]BrwProcDownloadErr,err=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1774_112_2_18_2_6_53_397,(uint8*)"d",err);
    if ((brw_info_ptr != PNULL) && (dl_req_ptr != PNULL))
    {
        err_ind.handle = dl_req_ptr->oma_dl_session_id;
        err_ind.error = err;
        BrwCancelDL(dl_req_ptr,brw_info_ptr);
        brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_DOWNLOAD_ERR_IND,&err_ind,sizeof(BRW_MSG_DOWNLOAD_ERROR_IND_T));
    }
}
/*
 *	process no memory case
 */
PUBLIC void BrwProcNoMem(BRW_INFO_T *brw_info_ptr)
{
    //BRWDebug_Printf:"[BRW]W BrwProcNoMem,brw_info_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1787_112_2_18_2_6_53_398,(uint8*)"d",brw_info_ptr);
    BrwProcErr(brw_info_ptr,BRW_ERR_NO_MEM);
}
/*
 *	desc:process message
 *  param:msg_id  message id
 *        param   message parameter
 *  TRUE if msg is browser's message,else FALSE
 */
PUBLIC BOOLEAN BRW_ProcMsg(uint32 msg_id,void* param)
{
#ifdef OMADL_SUPPORT
    if (ODM_SIGNAL_START <= msg_id && msg_id <= ODM_SIGNAL_END)
    {
        return BrwProcODMMsg(msg_id, param);
    }
    else
    {
        return BrwProcHttpMsg(msg_id,param);
    }
#else
    return BrwProcHttpMsg(msg_id,param);
#endif    
}
/*
 *	process history event(such as backward,forward,refresh ect)
 */
LOCAL BOOLEAN ProcHisEvent(const uint32 control_handler, BRW_NAVIGATE_EVENT_E op)
{
    BRW_INFO_T* brw_info_ptr=0;
	
	if(0==control_handler)
	{
		return FALSE;
	}
	brw_info_ptr = (BRW_INFO_T*)control_handler;

    return BRW_ProcNavEvent((BRW_INSTANCE)brw_info_ptr,op);
}
/****************************************************************************/
//Description : for page card jumping in wml
//Global resource dependence : 
//Author: juan.zhang
//Note:
/****************************************************************************/
LOCAL BOOLEAN HandleCardJumping(BRW_INFO_T *brw_info_ptr,const char *jumpto_ulr_ptr,BRW_NAVIGATE_EVENT_E nav_event)
{
	uint32			len = 0;
	LAYOUT_HANDLE	layout_handle = 0;
	//uint8			*cur_url_ptr=0;

	if(0==brw_info_ptr||0==jumpto_ulr_ptr)
	{
		return FALSE;
	}
	len = BRW_Strlen((const uint8*)jumpto_ulr_ptr);
	if(0==brw_info_ptr->cur_display_req_ptr)
	{
		layout_handle = brw_info_ptr->snapshot_open_layout_handler;
	}
	else
	{
		layout_handle = brw_info_ptr->cur_display_req_ptr->layout_handle;
		if(brw_info_ptr->cur_display_req_ptr->absolute_url)
		{
			BRWMem_Free(brw_info_ptr->cur_display_req_ptr->absolute_url);
			brw_info_ptr->cur_display_req_ptr->absolute_url=PNULL;
		}
		brw_info_ptr->cur_display_req_ptr->absolute_url=BRWMem_Alloc(len+1);
		if(PNULL==brw_info_ptr->cur_display_req_ptr->absolute_url)
		{
			return FALSE;
		}
		BRW_Memset(brw_info_ptr->cur_display_req_ptr->absolute_url,0,len+1);
		BRW_Strcpy(brw_info_ptr->cur_display_req_ptr->absolute_url,(const uint8*)jumpto_ulr_ptr);
	}
	if(0==layout_handle)
	{
		return FALSE;
	}
	BrwLayoutNavigateGetFocus(layout_handle, nav_event, (const uint8*)jumpto_ulr_ptr, FALSE);
	BrwLayout(layout_handle,LAYOUT_CAUSE_JUMP_PAGE);
	brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_DISPLAY_READY_IND,PNULL,0);
	return TRUE;
}
/*
 *	start new page request to open a new instance
 */
PUBLIC BOOLEAN BrwStartNewInstance(const LAYOUT_REQ_PARAM_T* param_ptr,const uint32 control_handler)
{
    BRW_INFO_T		*brw_info_ptr=PNULL;
    BRW_INSTANCE    instance = control_handler;
    uint16			url_len = 0;
    WAP_REQUEST_T   *wap_request_ptr=PNULL;
    BRW_REQ_STATE_E state = BRW_STATE_MAX;

    if ((PNULL == param_ptr) || (PNULL == param_ptr->url_ptr))
    {
        BRWDebug_Printf("[BRW]W BrwStartNewInstance,param_ptr=0x%x!", param_ptr);
        return FALSE;
    }  
    
    if(0 == control_handler)
    {
        BRWDebug_PrintWarning("BrwStartNewInstance,control_handler==0!");
        return FALSE;
    }
    url_len = (uint16)BRW_Strlen(param_ptr->url_ptr);
    if (0 == url_len)
    {
        BRWDebug_PrintWarning("BrwStartNewInstance,url_len 0!");
        return FALSE;
    }
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if(PNULL == brw_info_ptr)
    {
        BRWDebug_PrintWarning("BrwStartNewInstance:brw_info_ptr null!");
        return FALSE;
    }

    if (param_ptr->is_by_click || !brw_info_ptr->is_fixed)
    {
        MMIBROWSER_StartWeb(MMIBROWSER_WEB_ENTRY_URL, (uint8*)(param_ptr->url_ptr), PNULL);
    }
    else
    {
        wap_request_ptr=brw_info_ptr->cur_req_ptr;
        if(PNULL != wap_request_ptr)
        {
            state = BrwGetWapReqState(wap_request_ptr);
            switch (state)
            {
            case BRW_STATE_PAGE_REQUESTING:
            {
                //BrwFreeWapReqStruct会停掉http并释放相关资源
                BrwFreeWapReqStruct(wap_request_ptr);
                if(brw_info_ptr->cur_req_ptr != brw_info_ptr->cur_display_req_ptr)
                {
                    BrwFreeWapReqStruct(brw_info_ptr->cur_display_req_ptr);
                }
                brw_info_ptr->cur_req_ptr = PNULL;
                brw_info_ptr->cur_display_req_ptr = PNULL;
            }
                break;
                
            case BRW_STATE_MEDIA_REQUESTING:
            {
                //停止在途子媒体http请求,销毁子媒体请求队列       
                BRWList_Destruct(&wap_request_ptr->media_obj_info.meida_list,(BRW_LIST_FREE_F)FreeMediaObjStruct);
                BRW_Memset(&wap_request_ptr->media_obj_info, 0, sizeof(MEDIA_OBJ_INFO_T));  
                BrwSetWapReqState(wap_request_ptr, BRW_STATE_ALL_FINISHED);
            }
                break;
                
            default:
                break;
            }
        }

        if (BRW_INVALID_REQ_ID == BRW_AccessPage(instance, (uint8*)param_ptr->url_ptr))
        {
            BRW_MSG_ERROR_IND_T err_ind ={0};
            err_ind.error = BRW_ERR_UNKNOWN;
            brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_ERROR_IND, &err_ind, sizeof(BRW_MSG_ERROR_IND_T));
        }
    }    
    
    return TRUE;
}

/*
 *	start new page request
 */
PUBLIC BOOLEAN BrwStartNewPageReq(const LAYOUT_REQ_PARAM_T* param_ptr,const uint32 control_handler)
{
    //BOOLEAN			ret = TRUE;
    BOOLEAN			is_net_linked = FALSE;
    BRW_INFO_T		*brw_info_ptr=PNULL;
    WAP_REQUEST_T	*new_wap_req_ptr = PNULL;
    WAP_REQUEST_T	*prev_wap_req_ptr=PNULL;
    BRW_INSTANCE    instance = control_handler;
    uint16			url_len = 0;
    BOOLEAN			result=TRUE;
    uint8   *cur_uri_ptr=0;
    BOOLEAN is_host_without_slash = FALSE;
    char    *host_end_ptr = PNULL;
    uint8           *fragment_ptr = NULL;

    if ((PNULL == param_ptr) || (PNULL == param_ptr->url_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BrwStartNewPageReq,param_ptr=0x%x!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1898_112_2_18_2_6_53_400,(uint8*)"d", param_ptr);
        return FALSE;
    }  
    
    if(0 == control_handler)
    {
        BRWDebug_PrintWarning("BrwStartNewPageReq,control_handler==0!");
        return FALSE;
    }
    url_len = (uint16)BRW_Strlen(param_ptr->url_ptr);
    if (0 == url_len)
    {
        BRWDebug_PrintWarning("BrwStartNewPageReq,url_len 0!");
        return FALSE;
    }
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if(PNULL == brw_info_ptr)
    {
        BRWDebug_PrintWarning("BrwStartNewPageReq:brw_info_ptr null!");
        return FALSE;
    }
    cur_uri_ptr = BrwNavHisGetCurUrl();
    if (PNULL == cur_uri_ptr)
    {
        BRWDebug_PrintWarning("BrwStartNewPageReq:cur_uri_ptr null!");
        return FALSE;
    }
    if (MMIAPIBROWSER_HandleExScheme((uint8*)param_ptr->url_ptr))
    {
        return FALSE;
    }  
        
    instance = (BRW_INSTANCE)brw_info_ptr->instance_id;
    //先停止当前请求
    result = BRW_StopPageReq(instance);
    //if card jumping(WML页内跳转)
    if(BrwIsUrlIsEquExceptFragment(cur_uri_ptr,param_ptr->url_ptr))
    {
        if(HandleCardJumping(brw_info_ptr,(const char*)param_ptr->url_ptr,BRW_NAV_EVENT_NULL))
        {
            BrwNavHisAddItem(param_ptr->url_ptr);
            //need NOT be added to visit history
            return TRUE;
        }
        BRWDebug_PrintWarning("BrwStartNewPageReq:card jumping error!");
        return FALSE;
    }
    new_wap_req_ptr = (WAP_REQUEST_T*)BRWMem_Alloc(sizeof(WAP_REQUEST_T));
    //BRWDebug_Printf:"brw alloc wap request in BrwStartNewPageReq:%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_1946_112_2_18_2_6_53_401,(uint8*)"d", new_wap_req_ptr);
    if (PNULL == new_wap_req_ptr)
    {
        BRWDebug_PrintWarning("BrwStartNewPageReq:malloc wap req failed!");
        BrwProcNoMem(brw_info_ptr);
        return FALSE;
    }
    BRW_Memset(new_wap_req_ptr,0,sizeof(WAP_REQUEST_T));
    
    host_end_ptr = HttpMovetoHostEnd((char*)(param_ptr->url_ptr));
    if ((PNULL == host_end_ptr) || (0 == host_end_ptr[0]))
    {
        is_host_without_slash = TRUE;
    }
    if( NULL!=(fragment_ptr=BRW_Strchr(param_ptr->url_ptr, '#')) ) 
    {
        *fragment_ptr = '\0';
        url_len = (uint16)BRW_Strlen(param_ptr->url_ptr);
    }
    
    new_wap_req_ptr->absolute_url = BRWMem_Alloc(url_len+2);//for '/'
    if (PNULL == new_wap_req_ptr->absolute_url)
    {
        BRWDebug_PrintWarning("BrwStartNewPageReq:malloc uri failed!");
        BRWMem_Free(new_wap_req_ptr);
        BrwProcNoMem(brw_info_ptr);
        //recover param_ptr->url_ptr
        if (NULL != fragment_ptr)
        {
            *fragment_ptr = '#';
        }
        return FALSE;
    }
    //wap request must keep a copy of url
    BRW_Memset(new_wap_req_ptr->absolute_url, 0x00, (url_len+2));
    BRW_Memcpy(new_wap_req_ptr->absolute_url, param_ptr->url_ptr,url_len);
    if (is_host_without_slash && (param_ptr->url_ptr[url_len-1] !=  '/'))
    {       
        //copy '/' at the end of absolute_url
        BRW_Strcat(new_wap_req_ptr->absolute_url,(const uint8*)"/");
    }
    
    new_wap_req_ptr->original_url = BRWMem_Alloc(url_len+2);
    if(PNULL == new_wap_req_ptr->original_url)
    {
        BRWDebug_PrintWarning("BrwStartNewPageReq:malloc original uri failed!");
        BRWMem_Free(new_wap_req_ptr->absolute_url);
        BRWMem_Free(new_wap_req_ptr);
        BrwProcNoMem(brw_info_ptr);
        //recover param_ptr->url_ptr
        if (NULL != fragment_ptr)
        {
            *fragment_ptr = '#';
        }
        return FALSE;
    }
    BRW_Memcpy(new_wap_req_ptr->original_url,new_wap_req_ptr->absolute_url,url_len+2);
    
    //recover param_ptr->url_ptr
    if (NULL != fragment_ptr)
    {
        *fragment_ptr = '#';
    }
    
    //generate unique req id
    new_wap_req_ptr->req_id = GenReqId();
    new_wap_req_ptr->method = param_ptr->method;    
    if (BRW_HTTP_METHOD_POST == param_ptr->method)
    {
        new_wap_req_ptr->body_data_info_ptr = BrwBackupRequestBody(&param_ptr->body_data_info);
        if(PNULL == new_wap_req_ptr->body_data_info_ptr)
        {
            BRWMem_Free(new_wap_req_ptr->absolute_url);
            BRWMem_Free(new_wap_req_ptr->original_url);
            BRWMem_Free(new_wap_req_ptr);
            BrwProcNoMem(brw_info_ptr);
            return FALSE;
        }
    }
    
    //set state of request
    BrwSetWapReqState(new_wap_req_ptr, BRW_STATE_PAGE_REQUESTING);
    prev_wap_req_ptr = brw_info_ptr->cur_display_req_ptr;
    brw_info_ptr->cur_req_ptr = new_wap_req_ptr;

    //notify UI to change the url display
    BrwUpdateUrl(brw_info_ptr,new_wap_req_ptr->absolute_url); 
    new_wap_req_ptr->is_need_ico = TRUE;//for add url to main menu
    
/* BEGIN: Modified by George.Liu 01526, 2010/8/5   PN:MergeToMw1029forCustom */
    is_net_linked = BRW_IsNetLinked();

/* END:   Modified by George.Liu 01526, 2010/8/5   PN:MergeToMw1029forCustom */

    if(is_net_linked)//physical linked
    {
        if ((BRW_HTTP_STATE_NONE == brw_info_ptr->http_state)  || (BRW_HTTP_STATE_CLOSING == brw_info_ptr->http_state))
        {
            brw_info_ptr->http_context_id = 0;
            BrwInitHttpSession(brw_info_ptr);
        }
        else if (BRW_HTTP_STATE_INITED == brw_info_ptr->http_state)
        {
            if(!BrwStartPageRequest(brw_info_ptr))
            {
                return FALSE;
            }
        }
    }
    else
    {
        //BRWDebug_Printf:"BrwStartNewPageReq to ActiveNetwork"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2021_112_2_18_2_6_53_402,(uint8*)"");
        if(!ActiveNetwork(instance))
        {
            BRWDebug_PrintWarning("BrwStartNewPageReq, ActiveNetwork fail");
            BrwProcErr(brw_info_ptr,BRW_ERR_GPRS_LINK_FAILED);
        }
    }

    return TRUE;
}

PUBLIC HTTP_BODY_DATA_T* BrwBackupRequestBody(const HTTP_BODY_DATA_T* src_body_ptr)
{
	HTTP_BODY_DATA_T	*body_ptr = 0;
	uint32				len = 0;

	if(src_body_ptr==0)
	{
		return 0;
	}
	body_ptr = BRWMem_Alloc(sizeof(HTTP_BODY_DATA_T));
	if(0==body_ptr)
	{
		return 0;
	}
	len = BRW_Strlen(src_body_ptr->content_type_ptr);
	body_ptr->content_type_ptr = BRWMem_Alloc(len+1);
	if(0==body_ptr->content_type_ptr)
	{
		goto BrwBackupRequestBody_err;
	}
	BRW_Memset(body_ptr->content_type_ptr, 0, len+1);
	BRW_Memcpy(body_ptr->content_type_ptr, src_body_ptr->content_type_ptr, len);

	if(src_body_ptr->mode==BRW_DATA_SAVE_IN_BUF)
	{
//		len = BRW_Strlen(src_body_ptr->data_src.addr_ptr);
                len = src_body_ptr->data_src_len;
		body_ptr->data_src.addr_ptr = BRWMem_Alloc(len+1);
		if(0==body_ptr->data_src.addr_ptr)
		{
			goto BrwBackupRequestBody_err;
		}
		BRW_Memset(body_ptr->data_src.addr_ptr, 0, len+1);
		BRW_Memcpy(body_ptr->data_src.addr_ptr, src_body_ptr->data_src.addr_ptr, len);
	}
	else
	{
		len= BRW_WStrlen(src_body_ptr->data_src.file_name_ptr);
		body_ptr->data_src.file_name_ptr = BRWMem_Alloc(sizeof(uint16)*(len+1));
		if(0==body_ptr->data_src.file_name_ptr)
		{
			goto BrwBackupRequestBody_err;
		}
		BRW_Memset(body_ptr->data_src.file_name_ptr, 0, sizeof(uint16)*(len+1));
		BRW_Memcpy(body_ptr->data_src.file_name_ptr, 
			src_body_ptr->data_src.file_name_ptr, sizeof(uint16)*(len+1));
	}

	body_ptr->mode = src_body_ptr->mode;
	body_ptr->data_src_len = src_body_ptr->data_src_len;
	return body_ptr;
BrwBackupRequestBody_err:
	if(body_ptr->content_type_ptr != PNULL)
	{
		BRWMem_Free(body_ptr->content_type_ptr);
		body_ptr->content_type_ptr = PNULL;
	}
	if(src_body_ptr->mode==BRW_DATA_SAVE_IN_BUF)
	{
		if(body_ptr->data_src.addr_ptr != PNULL)
		{
			BRWMem_Free(body_ptr->data_src.addr_ptr);
			body_ptr->data_src.addr_ptr= PNULL;
		}
	}
	else
	{
		if(body_ptr->data_src.file_name_ptr != PNULL)
		{
			BRWMem_Free(body_ptr->data_src.file_name_ptr);
			body_ptr->data_src.file_name_ptr = PNULL;
		}
	}
	BRWMem_Free(body_ptr);
	body_ptr = PNULL;
	return 0;
}

#if 0 /* Reduce Code size */
PUBLIC void BrwFreeRequestBody(HTTP_BODY_DATA_T* body_ptr)
{
    if(body_ptr == PNULL)
    {
        return;
    }

    if (body_ptr->content_type_ptr != PNULL)
    {
        BRWMem_Free(body_ptr->content_type_ptr);
        body_ptr->content_type_ptr = PNULL;
    }
    
    if (BRW_DATA_SAVE_IN_BUF == body_ptr->mode)
    {
        if(body_ptr->data_src.addr_ptr != PNULL)
        {
            BRWMem_Free(body_ptr->data_src.addr_ptr);
            body_ptr->data_src.addr_ptr = PNULL;
        }
    }
    else
    {
        if(body_ptr->data_src.file_name_ptr != PNULL)
        {
            BRWMem_Free(body_ptr->data_src.file_name_ptr);
            body_ptr->data_src.file_name_ptr = PNULL;
        }
    }
    
    BRWMem_Free(body_ptr);
}
#endif /* Reduce Code size */

LOCAL MEDIA_OBJ_T* BrwAllocMediaObj(void)
{
	MEDIA_OBJ_T *media_obj_ptr = 0;

	media_obj_ptr = BRWMem_Alloc(sizeof(MEDIA_OBJ_T));
	if(0 == media_obj_ptr)
	{
		return 0;
	}
	//BRWDebug_Printf:"BrwAllocMediaObj media_obj_ptr = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2153_112_2_18_2_6_54_403,(uint8*)"d",media_obj_ptr);
	BRW_Memset(media_obj_ptr,0,sizeof(MEDIA_OBJ_T));
	return media_obj_ptr;
}
LOCAL void BrwGenerateMediaObjList(BRW_INFO_T *brw_info_ptr, WAP_REQUEST_T* req_ptr)
{
    uint8 *url_ptr = 0;
    MEDIA_OBJ_T *media_obj_ptr = 0;
    uint32 index = 0;
    BOOLEAN  is_url = TRUE;
    
    if(0==req_ptr||0==brw_info_ptr)
    {
        return;
    }
    if(BRW_INVALID_LAYOUT_HANDLE == req_ptr->layout_handle)
    {
        return;
    }
	if(IsMediaObjListEmpty(req_ptr))
	{
	    BRWList_Construct(&req_ptr->media_obj_info.meida_list);
	}

    //get pic
    if(BrwIsImgDL(brw_info_ptr))
    {		
        index = 0;
        while(BrwGetImgUrl(req_ptr->layout_handle,(uint8**)&url_ptr,index, &is_url))
        {
            if(url_ptr!=0 && is_url)
            {
                media_obj_ptr = BrwAllocMediaObj();
                if(0 == media_obj_ptr)
                {
                    return;
                }
                media_obj_ptr->url_ptr = url_ptr;
                media_obj_ptr->index = index;
                media_obj_ptr->media_req_type = MEDIA_REQ_TYPE_IMG;
                media_obj_ptr->page_req_id = req_ptr->req_id;
                BRWList_AppendObject(&req_ptr->media_obj_info.meida_list,media_obj_ptr);
            }
            else if(!is_url)
            {
                index--;				
            }
            index++;
        }
    }
    index = 0;
    while(BrwGetInputImgUrl(req_ptr->layout_handle,(uint8**)&url_ptr,index))
    {
        
        if(url_ptr!=0)
        {
            media_obj_ptr = BrwAllocMediaObj();
            if(0 == media_obj_ptr)
            {
                return;
            }
            media_obj_ptr->url_ptr = url_ptr;
            media_obj_ptr->index = index;
            media_obj_ptr->media_req_type = MEDIA_REQ_TYPE_INPUT_IMG;
            media_obj_ptr->page_req_id = req_ptr->req_id;
            BRWList_AppendObject(&req_ptr->media_obj_info.meida_list,media_obj_ptr);
        }
        index++;
    }

#ifdef MMIDORADO_BGSOUND_SUPPORT
    if(BrwIsBgsoundDL(brw_info_ptr))
    {
        //get sound
        index = 0;
        if(BrwGetBgsoundUrl(req_ptr->layout_handle,(uint8**)&url_ptr,index))
        {
            if(url_ptr!=0)
            {
                media_obj_ptr = BrwAllocMediaObj();
                if(0 == media_obj_ptr)
                {
                    return;
                }
                media_obj_ptr->url_ptr = url_ptr;
                media_obj_ptr->index = index;
                media_obj_ptr->media_req_type = MEDIA_REQ_TYPE_BGSOUND;
                media_obj_ptr->page_req_id = req_ptr->req_id;
                BRWList_AppendObject(&req_ptr->media_obj_info.meida_list,media_obj_ptr);
            }
        }
    }
#endif
    
    if(req_ptr->is_need_ico)
    {
        //get sound
        index = 0;
        url_ptr = (uint8*)BRWUtil_GetIcoUrl((const char*)(req_ptr->absolute_url));
        if(url_ptr != PNULL)
        {
            media_obj_ptr = BrwAllocMediaObj();
            if(0 == media_obj_ptr)
            {
                req_ptr->is_need_ico = FALSE;
                return;
            }
            media_obj_ptr->url_ptr = url_ptr;
            media_obj_ptr->index = index;
            media_obj_ptr->media_req_type = MEDIA_REQ_TYPE_ICO;
            media_obj_ptr->page_req_id = req_ptr->req_id;
            BRWList_AppendObject(&req_ptr->media_obj_info.meida_list,media_obj_ptr);
        }
        else
        {
            req_ptr->is_need_ico = FALSE;
        }
    }
}
/*
 *desc:	process page content
 *param:brw_info_ptr  browser instance pointer
 *      req_ptr         wap request object pointer
 *return:BRW_SUCCESS: success
 *       BRW_ERROR: 
 *       BRW_NOMEMORY: 
 */
LOCAL BRW_RET ProcPageContent(BRW_INFO_T *brw_info_ptr,WAP_REQUEST_T* req_ptr)
{
    BRW_RET			result = BRW_ERROR;
    uint32			wap_req_id = 0;
    WAP_REQUEST_T	*req_to_be_cached_ptr = 0;
    BOOLEAN         is_parser_again = FALSE;
    PARSER_INPUT_INFO_T parser_input_info={0}; 
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    //BRWDebug_Printf:"[BRW]ProcPageContent"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2286_112_2_18_2_6_54_404,(uint8*)"");
    if(brw_info_ptr && req_ptr && req_ptr->http_req_ptr)
    {   
        HTTP_REQ_INFO_T *http_req_ptr = PNULL;
        BRW_MIME_TYPE_E mime_type = BRW_MIME_UNKOWN;

        wap_req_id = req_ptr->req_id;
        http_req_ptr = req_ptr->http_req_ptr;        
        mime_type = http_req_ptr->mime_type;

ParserAgain:
        //BRWDebug_Printf:"[BRW]ProcPageContent,mime_type:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2297_112_2_18_2_6_54_405,(uint8*)"d",mime_type);
        if(mime_type==BRW_MIME_WML || mime_type==BRW_MIME_WMLC 
            || mime_type==BRW_MIME_WMLS || mime_type==BRW_MIME_HTML || mime_type==BRW_MIME_PLAINTEXT
            || (mime_type >= BRW_MIME_IMG_BMP && mime_type < BRW_MIME_IMG_UNKNOWN) )
        {
            if(req_ptr->layout_handle!=BRW_INVALID_LAYOUT_HANDLE)
            {
                BrwDestroyLayoutCtrl(req_ptr->layout_handle);
                //BRWDebug_Printf:"[BRW]ProcPageContent,BrwDestroyLayoutCtrl: 0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2303_112_2_18_2_6_54_406,(uint8*)"d", req_ptr->layout_handle);
                req_ptr->layout_handle = BRW_INVALID_LAYOUT_HANDLE;
            }
            req_ptr->layout_handle = BrwCreateLayoutCtrl(mime_type);
            //BRWDebug_Printf:"[BRW]ProcPageContent,creat layout:%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2307_112_2_18_2_6_54_407,(uint8*)"d",req_ptr->layout_handle);
            if(req_ptr->layout_handle==BRW_INVALID_LAYOUT_HANDLE)
            {   
                //BRWDebug_Printf:"[BRW]ProcPageContent,creat layout fail, mime_type:%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2310_112_2_18_2_6_54_408,(uint8*)"d",mime_type);
                BrwProcErr(brw_info_ptr,BRW_ERR_PAGE_ERR);
                return BRW_ERROR;
            }
        }

        //只要不是wmlc的mime type，都由排版内核处理（应该支持text和图片）             
        parser_input_info.charset_ptr=http_req_ptr->charset;
        parser_input_info.data_len = http_req_ptr->resp_buf_now_len;
        parser_input_info.data_ptr = http_req_ptr->resp_buf_ptr;
        parser_input_info.history_op_cbf =(BRW_HISTORY_OP_CBF)ProcHisEvent;
        parser_input_info.req_id = (uint32)brw_info_ptr->instance_id;
        if (!brw_info_ptr->is_fixed)
        {
            parser_input_info.start_new_pagereq_cbf = (BRW_START_PAGE_REQ_CBF)BrwStartNewPageReq;
        }
        else    
        {
            parser_input_info.start_new_pagereq_cbf = (BRW_START_PAGE_REQ_CBF)BrwStartNewInstance;
        }

        //parser_input_info.start_new_pagereq_cbf = (BRW_START_PAGE_REQ_CBF)BrwStartNewPageReq;
        parser_input_info.add_new_media_req_cbf = (BRW_ADD_MEDIA_REQ_CBF)BrwAddNewMediaReq;
        parser_input_info.url_ptr = req_ptr->absolute_url;
        parser_input_info.font_type = brw_info_ptr->profile.font_type;
        parser_input_info.is_show_image = public_profile_ptr->is_image_download;
        parser_input_info.is_play_bgsound = public_profile_ptr->is_bgsound_download;
        parser_input_info.screen_option = brw_info_ptr->profile.screen_opt;
        parser_input_info.prgbar_ptr = (void*)brw_info_ptr->profile.scroll_bar_handler;
        parser_input_info.prgbar_width = (uint8)brw_info_ptr->profile.scroll_bar_width;
		parser_input_info.http_context_id = brw_info_ptr->http_context_id;

        if(mime_type ==BRW_MIME_WMLS || mime_type == BRW_MIME_WMLSC)
        {            
            result = BrwProcWmlScript(&parser_input_info,req_ptr->layout_handle);

            //BRWDebug_Printf:"brw proc page, BrwProcWmlScript result:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2320_112_2_18_2_6_54_409,(uint8*)"d",result);
        }
        else
        {
#ifdef JS_SUPPORT
            parser_input_info.user_agent_ptr = public_profile_ptr->user_agent_ptr;
            parser_input_info.is_cookie_enabled = public_profile_ptr->is_cookie_enable;
            parser_input_info.win_handle = brw_info_ptr->profile.win_handle;
            parser_input_info.cancel_wait = BrwCancelWait;
#endif
            //解析网页
            result = BrwParseWebPage(&parser_input_info,req_ptr->layout_handle);
			
            //BRWDebug_Printf:"brw proc page, BrwParseWebPage result:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2360_112_2_18_2_6_54_410,(uint8*)"d",result);
			
            if(BRW_SUCCESS == result)
            {
                //排版
                result = BrwLayout(req_ptr->layout_handle, LAYOUT_CAUSE_PAGE);
                //BRWDebug_Printf:"brw proc page, BrwLayout result:%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2369_112_2_18_2_6_54_411,(uint8*)"d",result);
  
                if(BRW_SUCCESS == result) //BrwLayout success
                {
                    uint32	media_num = 0;
                    BOOLEAN is_cached_req = FALSE;

                    if (!brw_info_ptr->is_fixed)//is_fixed don't in history,only one wap request
                    {
                        if (brw_info_ptr->cur_display_req_ptr != PNULL)
                        {
                            is_cached_req = brw_info_ptr->cur_display_req_ptr->is_cached_req;
                        }

                        BrwFreeWapReqStruct(brw_info_ptr->cur_display_req_ptr);
                        brw_info_ptr->cur_display_req_ptr = req_ptr;
                                           
                        //history record
                        if(req_ptr->cur_nav_event == BRW_NAV_EVENT_REFRESH)
                        {
                            if (is_cached_req)
                            {
                                //BRWDebug_Printf:"brw ProcPageContent free cache"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2389_112_2_18_2_6_54_412,(uint8*)"");
                                BrwNavHisFreeCurCacheReq();
                            }
                        }
                        else if(req_ptr->cur_nav_event==BRW_NAV_EVENT_BACKWARD)
                        {
                            BrwNavHisBackward();
                            BrwLayoutNavigateGetFocus(req_ptr->layout_handle, BRW_NAV_EVENT_BACKWARD, BrwNavHisGetCurUrl(), FALSE);
                        }
                        else if(req_ptr->cur_nav_event==BRW_NAV_EVENT_FORWARD)
                        {
                            BrwNavHisForward();
                        }
                        else if(req_ptr->cur_nav_event==BRW_NAV_EVENT_NULL)
                        {
                            BrwNavHisAddItem(req_ptr->absolute_url);
                        }
                    }
                    else
                    {
                        BrwFreeWapReqStruct(brw_info_ptr->cur_display_req_ptr);
                        brw_info_ptr->cur_display_req_ptr = req_ptr;
                    }
                
#ifdef JS_SUPPORT
                    //判断JS解析执行过程中是否需要打开新的页面或者回退
                    if (XhtmlCheckIfJsStartNewPage(req_ptr->layout_handle))
                    {
                        XhtmlSetJsStartNewPageFlag(req_ptr->layout_handle);
                        XHTMLMakeNewRequestByJs(req_ptr->layout_handle);
                        return BRW_SUCCESS;
                    }
                    else if (XhtmlCheckIfJsBackPage(req_ptr->layout_handle))
                    {
                        XhtmlSetJsBackwPageFlag(req_ptr->layout_handle);
                        BRW_ProcNavEvent(brw_info_ptr->instance_id, BRW_NAV_EVENT_BACKWARD);
                        return BRW_SUCCESS;
                    }
                    else if (XhtmlCheckIfJsSubmitForm(req_ptr->layout_handle))
                    {
                        XhtmlSetJsSubmitFormFlag(req_ptr->layout_handle);
                        XHTMLMakeFormSubmitByJs(req_ptr->layout_handle);
                        return BRW_SUCCESS;
                    }
                    else if (XhtmlCheckIfJsClickInput(req_ptr->layout_handle))
                    {
                        XhtmlSetJsClickInputFlag(req_ptr->layout_handle);
                        XhtmlLayoutInputClickProcessByJs(req_ptr->layout_handle);
                        return BRW_SUCCESS;
                    }
                    else if (XhtmlCheckIfJsRecalcPage(req_ptr->layout_handle))
                    {
                        XhtmlSetJsRecalcFlag(req_ptr->layout_handle);
                        BrwLayout(req_ptr->layout_handle, LAYOUT_CAUSE_JS);
                    }
#endif

                    if (!brw_info_ptr->is_fixed)//is_fixed don't in visit history
                    {
                        uint16 title_arr[BRW_MAX_HIS_NAME_LEN + 1]={0};
                        uint16 title_len=0;
                
                        BrwGetPageTitle(req_ptr->layout_handle,title_arr,BRW_MAX_HIS_NAME_LEN,&title_len);
                        BrwVisitHisAddItem(req_ptr->absolute_url,title_arr,title_len);
                    }
                    
                    if (mime_type==BRW_MIME_WML || mime_type==BRW_MIME_HTML)
                    {
	                    BrwGenerateMediaObjList(brw_info_ptr, req_ptr);
	                    if(public_profile_ptr->is_image_download)
	                    {
	                        media_num = BrwGetImgNumber(req_ptr->layout_handle);
	                    }
	                    else
	                    {
	                        media_num = 0;
	                    }
	                    
	                    if(req_ptr->is_need_ico)
	                    {
	                        media_num++;
	                    }       
	                    
#ifdef MMIDORADO_BGSOUND_SUPPORT
	                    if (!brw_info_ptr->is_fixed && BrwIsBgsoundDL(brw_info_ptr))
	                    {
	                        BGSOUND_INFO bgsound = {0};
	                        
	                        if (BrwGetBgsoundBuf(&(bgsound.data_ptr),&(bgsound.data_len),&(bgsound.bgsound_type),PNULL,req_ptr->layout_handle, 0))
	                        {
	                            media_num++;
	                        }
	                    }
#endif
                    }

                    brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_PAGE_GET_IND,&media_num,sizeof(uint32));
                    brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_DISPLAY_READY_IND,PNULL,0);               
                }
                else //BrwLayout failed
                {
                    /* If http_req_ptr->mime_type(Content-Type in http head of server response) is "WML",
                     * and content_type of web page is "HTML", BrwLayout must fail. But if 
                     * http_req_ptr->mime_type(Content-Type in http head of server response) is "HTML",
                     * and content_type of web page is "WML", page can still show. So only when mime_type 
                     * is "WML" and BrwLayout failed, we parser the page again using mime_type "HTML".
                     */
                    if(result == BRW_ERROR && !is_parser_again && mime_type == BRW_MIME_WML)
                    {
                        is_parser_again = TRUE;
                        mime_type = BRW_MIME_HTML;
                        goto ParserAgain;
                    }
                }
            }//BrwParseWebPage success
        }
        
        if(BRW_SUCCESS == result)
        {
            //在DisplayWebPage时发起了新的页面请求
            if(wap_req_id!=brw_info_ptr->cur_req_ptr->req_id)
            {
                return BRW_SUCCESS;
            }
            
            //release the snapshot layout if current page is snapshot
            BRW_SnapshotEndOfflinePage();
            //save current display page content
            if(brw_info_ptr->cur_page_content.page_content_ptr)
            {
                //BRWMem_Free(brw_info_ptr->cur_page_content.page_content_ptr);
                brw_info_ptr->cur_page_content.page_content_ptr = 0;
                brw_info_ptr->cur_page_content.page_content_len = 0;
                brw_info_ptr->cur_page_content.page_mime_type = BRW_MIME_UNKOWN;
            }
            if(brw_info_ptr->cur_page_content.charset_ptr != PNULL)
            {
                BRWMem_Free(brw_info_ptr->cur_page_content.charset_ptr);
                brw_info_ptr->cur_page_content.charset_ptr = PNULL;
            }
            if(http_req_ptr->charset!=0)
            {
                brw_info_ptr->cur_page_content.charset_ptr = BRWMem_Alloc(BRW_Strlen(http_req_ptr->charset)+1);
                if(0!=brw_info_ptr->cur_page_content.charset_ptr)
                {
                    BRW_Memset(brw_info_ptr->cur_page_content.charset_ptr,0,BRW_Strlen(http_req_ptr->charset)+1);
                    BRW_Strcpy(brw_info_ptr->cur_page_content.charset_ptr,http_req_ptr->charset);
                }
            }
            brw_info_ptr->cur_page_content.page_content_ptr = http_req_ptr->resp_buf_ptr;
            brw_info_ptr->cur_page_content.page_content_len = http_req_ptr->resp_buf_now_len;
            brw_info_ptr->cur_page_content.page_mime_type = mime_type;

            if (!brw_info_ptr->is_fixed)
            {
                req_to_be_cached_ptr = BrwBackupWapRequest(req_ptr);	
                if(req_to_be_cached_ptr!=0)
                {
                    BrwNavHisAddCache(req_to_be_cached_ptr);
                }
            }
            req_ptr->http_req_ptr->resp_buf_ptr=PNULL;
            req_ptr->http_req_ptr->resp_buf_now_len=0;
            req_ptr->http_req_ptr->resp_buf_max_len=0;
            req_ptr->http_req_ptr->http_content_len = 0;
        }        
    }
    else
    {
        //BRWDebug_Printf:"[BRW]W brw ProcPageOK:req_ptr=0x%x,brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2536_112_2_18_2_6_54_413,(uint8*)"dd",req_ptr,brw_info_ptr);
        result = BRW_ERROR;
    }
    return result;
}
/*
 *desc:	process media content
 *param:brw_info_ptr        浏览器句柄
 *      req_ptr             http request object pointer
 *return:TRUE if success
 */
LOCAL BOOLEAN ProcMediaContent(BRW_INFO_T *brw_info_ptr,HTTP_REQ_INFO_T* req_ptr)
{
    BOOLEAN			result = TRUE;
    MEDIA_OBJ_T		*media_obj_ptr=PNULL;
    WAP_REQUEST_T	*wap_req_ptr = PNULL;
    uint32          css_len       = 0;
    uint8           *css_data_ptr = NULL;
#ifdef JS_SUPPORT
    uint8           *ext_js_ptr = NULL;
#endif

    if(brw_info_ptr==PNULL || req_ptr==PNULL || brw_info_ptr->cur_req_ptr==PNULL)
    {
        //BRWDebug_Printf:"[BRW]W ProcMediaContent:brw_info_ptr=0x%x,req_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2554_112_2_18_2_6_54_414,(uint8*)"dd",brw_info_ptr,req_ptr);
        return FALSE;
    }
    wap_req_ptr = brw_info_ptr->cur_req_ptr;
    media_obj_ptr = (MEDIA_OBJ_T*)GetMediaObjByHttpReq(wap_req_ptr,req_ptr);

    if(media_obj_ptr)
    {
        BRW_MIME_TYPE_E mime_type=BRW_MIME_UNKOWN;   
		
        //BRWDebug_Printf:"[BRW] ProcMediaContent:req_ptr->resp_buf_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2564_112_2_18_2_6_54_415,(uint8*)"d",req_ptr->resp_buf_ptr);
        //BRWDebug_Printf:"[BRW] ProcMediaContent:media_obj_ptr->media_req_type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2565_112_2_18_2_6_54_416,(uint8*)"d",media_obj_ptr->media_req_type);
		
        mime_type = req_ptr->mime_type;
		media_obj_ptr->is_req_handling = FALSE;
        if ((BRW_STATE_MEDIA_REQUESTING == BrwGetWapReqState(wap_req_ptr))
            && (wap_req_ptr->layout_handle != BRW_INVALID_LAYOUT_HANDLE))
        {
            switch(media_obj_ptr->media_req_type)
            {
            case MEDIA_REQ_TYPE_CSS:
                if(req_ptr->mime_type == BRW_MIME_CSS)
                {
                    result = BrwParseCSS(req_ptr->resp_buf_ptr,
                                    req_ptr->resp_buf_now_len,
                                     (const char*)media_obj_ptr->url_ptr,
                                     wap_req_ptr->layout_handle,
                                     0,
                                     media_obj_ptr->index, 
                                     CSS_TYPE_IMPEND);
                }
                else
                {
                    //find <%Response.ContentType="text/css"%>
                    css_data_ptr = (uint8*)BRW_CaseStrstr(req_ptr->resp_buf_ptr, (uint8*)"Response.ContentType=\"text/css");
                    if(css_data_ptr != NULL)
                    {
                        css_data_ptr = (uint8*)BRW_CaseStrstr(req_ptr->resp_buf_ptr, (uint8*)"%>");
                        if(css_data_ptr != NULL
                            && (uint32)(css_data_ptr + 2) < (uint32)(req_ptr->resp_buf_ptr + req_ptr->resp_buf_now_len))
                        {
                            css_data_ptr += 2; // 2 for %>
                            css_len = (uint32)(req_ptr->resp_buf_ptr + req_ptr->resp_buf_now_len) - (uint32)(css_data_ptr);
                            result = BrwParseCSS(css_data_ptr,
                                css_len,
                                (const char*)media_obj_ptr->url_ptr,
                                wap_req_ptr->layout_handle,
                                0,
                                media_obj_ptr->index, 
                                CSS_TYPE_IMPEND);                       
                        }
                    }
                }
                if(req_ptr->resp_buf_ptr != NULL)
                {
                    BRWMem_Free(req_ptr->resp_buf_ptr);
                }
                if(result)
                {
                    result = BrwLayout(wap_req_ptr->layout_handle, LAYOUT_CAUSE_CSS);
                }               
                //不管成功失败，都返回TRUE，不影响流程  
                break;
                
#ifdef MMIDORADO_BGSOUND_SUPPORT
            case MEDIA_REQ_TYPE_BGSOUND://背景音
                if (BrwIsBgsoundDL(brw_info_ptr))
                {
                    BrwSetBgsoundBuf(req_ptr->resp_buf_ptr,req_ptr->resp_buf_now_len,mime_type,
                                     wap_req_ptr->layout_handle,PNULL);
                    result = BrwLayout(wap_req_ptr->layout_handle, LAYOUT_CAUSE_SOUND);
                    if(!result)
                    {
                        //BRWDebug_Printf:"[BRW]ProcMediaReq:BrwLayout failed"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2602_112_2_18_2_6_54_417,(uint8*)"");
                    }     
                    else
                    {
                        BGSOUND_INFO bgsound = {0};
                        
                        if(BrwGetBgsoundBuf(&(bgsound.data_ptr),&(bgsound.data_len),&(bgsound.bgsound_type),&(bgsound.loop),wap_req_ptr->layout_handle, 0))
                        {
                            if (bgsound.data_ptr != PNULL)
                            {
                                BrwPlayBGsound(&bgsound);
                            }
                            else
                            {
                                BrwFreeBgsoundBuf(wap_req_ptr->layout_handle);
                            }
                        }
                    }
                    brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_MEDIA_GET_ONE_IND,PNULL,0);
                }
                else
                {
                    //BRWDebug_Printf:"[BRW]ProcMediaReq:MEDIA_REQ_TYPE_BGSOUND FALSE"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2624_112_2_18_2_6_54_418,(uint8*)"");
                }                    
                break;
#endif

            case MEDIA_REQ_TYPE_IMG://图片
                BrwSetImgBuf(req_ptr->resp_buf_ptr,req_ptr->resp_buf_now_len,mime_type,wap_req_ptr->layout_handle,media_obj_ptr->index);
                result = BrwLayout(wap_req_ptr->layout_handle,LAYOUT_CAUSE_PIC);
                if(!result)
                {
                    //BRWDebug_Printf:"[BRW]ProcMediaReq:BrwLayout failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2634_112_2_18_2_6_54_419,(uint8*)"");
                }
                brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_MEDIA_GET_ONE_IND,PNULL,0);
                break;               
#ifdef CSS_SUPPORT
            case MEDIA_REQ_TYPE_BG_IMG:
                BrwSetBgImgBuf(req_ptr->resp_buf_ptr,req_ptr->resp_buf_now_len,mime_type,wap_req_ptr->layout_handle,media_obj_ptr->index);
                result = BrwLayout(wap_req_ptr->layout_handle,LAYOUT_CAUSE_PIC);
                if(!result)
                {
                    //BRWDebug_Printf:"[BRW]ProcMediaReq:BrwLayout failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2634_112_2_18_2_6_54_419,(uint8*)"");
                }
                brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_MEDIA_GET_ONE_IND,PNULL,0);
                break;
#endif
            case MEDIA_REQ_TYPE_INPUT_IMG:
                BrwSetInputImgBuf(req_ptr->resp_buf_ptr,req_ptr->resp_buf_now_len,mime_type,wap_req_ptr->layout_handle,media_obj_ptr->index);
                result = BrwLayout(wap_req_ptr->layout_handle,LAYOUT_CAUSE_PIC);
                if(!result)
                {
                    //BRWDebug_Printf:"[BRW]ProcMediaReq:BrwLayout failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2643_112_2_18_2_6_54_420,(uint8*)"");
                }
                brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_MEDIA_GET_ONE_IND,PNULL,0);
                break;   
            case MEDIA_REQ_TYPE_LIST_IMG:
                if(BrwSetListImgBuf(req_ptr->resp_buf_ptr,req_ptr->resp_buf_now_len,mime_type,wap_req_ptr->layout_handle,media_obj_ptr->index))
                {
                    if(!BrwLayout(wap_req_ptr->layout_handle,LAYOUT_CAUSE_PIC))
                    {
                        //BRWDebug_Printf:"[BRW]ProcMediaReq:BrwLayout failed"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2652_112_2_18_2_6_54_421,(uint8*)"");
                    }
                }
                else
                {
                    if(req_ptr->resp_buf_ptr != NULL)
                    {
                        BRWMem_Free(req_ptr->resp_buf_ptr);
                    }
                }
                break;  
#ifdef JS_SUPPORT
            case MEDIA_REQ_TYPE_JS:
                if (req_ptr->resp_buf_now_len == 0)
                {
                    result = BrwParseJavaScript(NULL,NULL,wap_req_ptr->layout_handle, NULL);   
                }
                else
                {
                    ext_js_ptr = BRWMem_Alloc(req_ptr->resp_buf_now_len + 1);
                    if (ext_js_ptr == NULL)
                    {
                        if(req_ptr->resp_buf_ptr != NULL)
                        {
                            BRWMem_Free(req_ptr->resp_buf_ptr);
                        }
                        break;
                    }
                    BRW_Memset(ext_js_ptr, 0, req_ptr->resp_buf_now_len + 1);
                    BRW_Memcpy(ext_js_ptr, req_ptr->resp_buf_ptr, req_ptr->resp_buf_now_len);
                    result = BrwParseJavaScript(ext_js_ptr,
                        media_obj_ptr->url_ptr,wap_req_ptr->layout_handle, NULL);   
                    
                    if(req_ptr->resp_buf_ptr != NULL)
                    {
                        BRWMem_Free(req_ptr->resp_buf_ptr);
                    }
                    BRWMem_Free(ext_js_ptr);
                }
                break;
#endif
            case MEDIA_REQ_TYPE_ICO:
            {
                uint16  filename_len = BRW_WStrlen(wap_req_ptr->ico_filename_ptr);
                    
                if (filename_len > 0)//for MMIDORADO_MY_NAV_SUPPORT
                {
                    BRWPL_DelFileSyn(wap_req_ptr->ico_filename_ptr, filename_len);
                    BRWPL_WriteFileSyn(wap_req_ptr->ico_filename_ptr, filename_len, req_ptr->resp_buf_ptr, req_ptr->resp_buf_now_len);
#ifdef MMIDORADO_MY_NAV_SUPPORT
                    MMIBROWSER_SetIsIcoUpdate(TRUE);
#endif
                }
                else
                {
                    uint16  web_ico_name[BRW_FULL_PATH_MAX_LEN + 1] = {0};
                    uint16  web_ico_name_len = BRW_FULL_PATH_MAX_LEN;
                    
                    BrwGetWebIcoFullName(web_ico_name, &web_ico_name_len);
                    BRWPL_DelFileSyn(web_ico_name, web_ico_name_len);
                    BRWPL_WriteFileSyn(web_ico_name, web_ico_name_len, req_ptr->resp_buf_ptr, req_ptr->resp_buf_now_len);
                }

                if(req_ptr->resp_buf_ptr != PNULL)
                {
                    BRWMem_Free(req_ptr->resp_buf_ptr);
                }
                wap_req_ptr->is_need_ico = FALSE;
                if (wap_req_ptr->ico_filename_ptr != PNULL)
                {
                    BRWMem_Free(wap_req_ptr->ico_filename_ptr);
                    wap_req_ptr->ico_filename_ptr = PNULL;
                }
                brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_MEDIA_GET_ONE_IND,PNULL,0);
            }
                break;
                
            default:
                break;
            }            
            brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_REPAINT_READY_IND,PNULL,0);
            //为减少一次buf拷贝，此处直接将buf置空，buf的管理权已经交给layout ctrl
            req_ptr->resp_buf_ptr=PNULL;
            req_ptr->resp_buf_now_len=0;
            req_ptr->resp_buf_max_len=0;
            req_ptr->http_content_len = 0;  

            //add protect
            if  ((wap_req_ptr->media_obj_info.wait_process_obj_list_ptr != PNULL)
                && (media_obj_ptr == wap_req_ptr->media_obj_info.wait_process_obj_list_ptr->object))
            {
                //将当前媒体请求从请求队列中删除
                BRWList_RemoveObject(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr);
                //protect:避免在BrwStartNextMediaReq中使用了野指针
                SCI_TRACE_LOW("[BRW]W ProcMediaContent:wait_process_obj_list_ptr ERROR");
                if (wap_req_ptr->media_obj_info.meida_list.next != PNULL)
                {
                    wap_req_ptr->media_obj_info.wait_process_obj_list_ptr = wap_req_ptr->media_obj_info.meida_list.next;
                }
                else
                {
                    wap_req_ptr->media_obj_info.wait_process_obj_list_ptr = PNULL;
                }
            }
            else
            {
                //将当前媒体请求从请求队列中删除
                BRWList_RemoveObject(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr);
            }
            FreeMediaObjStruct(media_obj_ptr);

            //发起下一个子媒体请求
            BrwStartNextMediaReq(brw_info_ptr);
        }
        else
        {
            //BRWDebug_Printf:"[BRW]W ProcMediaReq,layout_handle=0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2707_112_2_18_2_6_55_422,(uint8*)"d",wap_req_ptr->layout_handle);
            //销毁media队列
            BRWList_Destruct(&wap_req_ptr->media_obj_info.meida_list,(BRW_LIST_FREE_F)FreeMediaObjStruct);
            BrwSetWapReqState(wap_req_ptr,BRW_STATE_ALL_FINISHED);
            BrwFreeHttpRequest(req_ptr);
            return FALSE;
        }
    }
    else
    {
        //BRWDebug_Printf:"[BRW]ProcMediaReq:media_obj_ptr = NULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2717_112_2_18_2_6_55_423,(uint8*)"");
        BrwFreeHttpRequest(req_ptr);
        return FALSE;
    }
    return TRUE;
}
/*
 *	功能：比较两个对象指针是否相等
 */
LOCAL BOOLEAN CompareObj(void* dst_obj_ptr,void* src_obj_ptr)
{
    return (dst_obj_ptr==src_obj_ptr);
}

PUBLIC BOOLEAN BrwStartPageRequest(BRW_INFO_T	*brw_info_ptr)
{
    HTTP_REQ_PARAM_T    param = {0};
    HTTP_REQ_INFO_T *http_req_ptr = 0;
    WAP_REQUEST_T   *new_wap_req_ptr= 0;
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    if ((PNULL == brw_info_ptr) || (PNULL == brw_info_ptr->cur_req_ptr))
    {
        return FALSE;
    }

    new_wap_req_ptr = brw_info_ptr->cur_req_ptr;
    param.method = new_wap_req_ptr->method;
    param.uri_ptr = new_wap_req_ptr->absolute_url;
    param.ua_prof_ptr = BrwGetUAProfStr(brw_info_ptr);
    param.user_agent_ptr=BrwGetUserAgentStr(brw_info_ptr);
    if(public_profile_ptr->proxy_opt.is_use_proxy)
    {
        param.is_using_relative_url = FALSE;
    }
    else
    {
        param.is_using_relative_url = TRUE;
    }
 	
    if (BRW_HTTP_METHOD_POST == new_wap_req_ptr->method)
    {
        BRW_Memcpy((void*)&param.body_data, new_wap_req_ptr->body_data_info_ptr, sizeof(HTTP_BODY_DATA_T));
    }

    http_req_ptr = BrwSendHttpRequest(&param,brw_info_ptr->http_context_id);
    if (PNULL == http_req_ptr)
    {
        //BRWDebug_Printf:"BRW_AccessPage:BrwSendHttpRequest failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2764_112_2_18_2_6_55_424,(uint8*)"");
        BrwFreeWapReqStruct(new_wap_req_ptr);
        brw_info_ptr->cur_req_ptr = brw_info_ptr->cur_display_req_ptr;
        BrwProcErr(brw_info_ptr,BRW_ERR_NO_MEM);
        return FALSE;
    } 
    else
    {
        new_wap_req_ptr->http_req_ptr = http_req_ptr;
    }
    return TRUE;
}
#ifdef HTTP2_SUPPORT
LOCAL BOOLEAN BrwIsSameHost(BRW_INFO_T* brw_info_ptr, char* url_ptr)
{
    if (PNULL == brw_info_ptr || PNULL == url_ptr)
    {
        SCI_TRACE_LOW("[%s:%d]brw_info_ptr: %p, url_ptr: %p",__FUNCTION__,__LINE__, brw_info_ptr, url_ptr);
        return FALSE;
    }
    SCI_TRACE_LOW("[%s:%d]curr_host_url: %s, url_ptr: %s\n",__FUNCTION__,__LINE__,brw_info_ptr->curr_host_url, url_ptr);
    if ((PNULL != HttpStrCaseStr(url_ptr, BRW_Strlen(url_ptr), brw_info_ptr->curr_host_url)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif
/*
 *	功能：发起下一个媒体请求
 *  参数：brw_info_ptr  浏览器句柄
 *
 */
LOCAL void BrwStartNextMediaReq(BRW_INFO_T* brw_info_ptr)
{
    WAP_REQUEST_T   *wap_req_ptr = PNULL;
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    if ((PNULL == brw_info_ptr) || (PNULL == brw_info_ptr->cur_req_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BrwStartNextMediaReq:brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2787_112_2_18_2_6_55_425,(uint8*)"d",brw_info_ptr);
        return;
    }

    wap_req_ptr = brw_info_ptr->cur_req_ptr;
#ifdef HTTP2_SUPPORT
	if(wap_req_ptr->media_obj_info.wait_process_obj_list_ptr != PNULL)
    {  
        if (brw_info_ptr->curr_host_is_http2 == FALSE)
        {
            HTTP_REQ_PARAM_T    param = {0};
            HTTP_REQ_INFO_T *http_req_ptr = PNULL;
            MEDIA_OBJ_T *media_obj_ptr = PNULL;
            BRW_LIST_T  *cur_media_list_ptr = wap_req_ptr->media_obj_info.wait_process_obj_list_ptr;
            SCI_TRACE_LOW("[%s:%d]qin.he add debug curr_host_is_http2 is false",__FUNCTION__,__LINE__);
            media_obj_ptr = (MEDIA_OBJ_T*)cur_media_list_ptr->object;
            if (PNULL == media_obj_ptr)
            {
                BRWDebug_PrintWarning("BrwStartNextMediaReq:wrong media data!");
                return;
            }

            if (media_obj_ptr->is_req_handling == TRUE)
            {
                BRWDebug_PrintWarning("BrwStartNextMediaReq:the media object has handling");
                return;
            }
            
            //start http request     
            SCI_TRACE_LOW("[%s:%d]dispose media obj: %p\n",__FUNCTION__,__LINE__,media_obj_ptr);
            BRW_Memset(&param, 0, sizeof(HTTP_REQ_PARAM_T));
            param.method = BRW_HTTP_METHOD_GET;
            param.uri_ptr = (uint8*)media_obj_ptr->url_ptr;
            param.ua_prof_ptr = BrwGetUAProfStr(brw_info_ptr);
            param.user_agent_ptr = BrwGetUserAgentStr(brw_info_ptr);
            param.is_sub_media_req = TRUE;
            
            if(public_profile_ptr->proxy_opt.is_use_proxy)
            {
                param.is_using_relative_url = FALSE;
            }
            else
            {
                param.is_using_relative_url = TRUE;
            }
            
            media_obj_ptr->page_req_id = wap_req_ptr->req_id;
            http_req_ptr = BrwSendHttpRequest(&param, brw_info_ptr->http_context_id);
            media_obj_ptr->http_req_ptr = http_req_ptr;
            media_obj_ptr->is_req_handling = TRUE;
            wap_req_ptr->media_obj_info.wait_process_obj_list_ptr = cur_media_list_ptr->next; 
            if (PNULL == http_req_ptr)
            {
                SCI_TRACE_LOW("[%s:%d]http_req_ptr is NULL\n",__FUNCTION__,__LINE__);
                BRWList_RemoveObject(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr);
                media_obj_ptr->is_req_handling = FALSE;
                FreeMediaObjStruct(media_obj_ptr);
                BrwStartNextMediaReq(brw_info_ptr);
            } 
        }
        else
        {
            HTTP_REQ_PARAM_T    param = {0};
            HTTP_REQ_INFO_T *http_req_ptr = PNULL;
            MEDIA_OBJ_T *media_obj_ptr = PNULL;
            BRW_LIST_T  *cur_media_list_ptr = wap_req_ptr->media_obj_info.wait_process_obj_list_ptr;
            BOOLEAN  has_start_http_req = FALSE;

            SCI_TRACE_LOW("[%s:%d]qin.he add debug curr_host_is_http2 is true",__FUNCTION__,__LINE__);
            while(cur_media_list_ptr)
            {
                media_obj_ptr = (MEDIA_OBJ_T*)cur_media_list_ptr->object;
                cur_media_list_ptr = cur_media_list_ptr->next;
                if (PNULL == media_obj_ptr)
                {
                    BRWDebug_PrintWarning("BrwStartNextMediaReq:wrong media data!");
                    continue;
                }

                if (media_obj_ptr->is_req_handling == TRUE)
                {
                    BRWDebug_PrintWarning("BrwStartNextMediaReq:media object has handling!");
                    has_start_http_req = TRUE;
                    continue;
                }
                
                //css file should be downloaded at first.
                if (media_obj_ptr->media_req_type == MEDIA_REQ_TYPE_CSS && BrwIsSameHost(brw_info_ptr, media_obj_ptr->url_ptr) == FALSE)
                {
                    //start http request    
                    SCI_TRACE_LOW("[%s:%d]dispose media obj: %p\n",__FUNCTION__,__LINE__,media_obj_ptr);
                    BRW_Memset(&param, 0, sizeof(HTTP_REQ_PARAM_T));
                    param.method = BRW_HTTP_METHOD_GET;
                    param.uri_ptr = (uint8*)media_obj_ptr->url_ptr;
                    param.ua_prof_ptr = BrwGetUAProfStr(brw_info_ptr);
                    param.user_agent_ptr = BrwGetUserAgentStr(brw_info_ptr);
                    param.is_sub_media_req = TRUE;
                    
                    if(public_profile_ptr->proxy_opt.is_use_proxy)
                    {
                        param.is_using_relative_url = FALSE;
                    }
                    else
                    {
                        param.is_using_relative_url = TRUE;
                    }
            
                    media_obj_ptr->page_req_id = wap_req_ptr->req_id;
                    http_req_ptr = BrwSendHttpRequest(&param, brw_info_ptr->http_context_id);
                    media_obj_ptr->http_req_ptr = http_req_ptr;
                    media_obj_ptr->is_req_handling = TRUE;
                    
                    has_start_http_req = TRUE;

                    if (PNULL == http_req_ptr)
                    {
                        SCI_TRACE_LOW("[%s:%d]http_req_ptr is NULL\n",__FUNCTION__,__LINE__);
                        //add protect
                        if  ((wap_req_ptr->media_obj_info.wait_process_obj_list_ptr != PNULL)
                            && (media_obj_ptr == wap_req_ptr->media_obj_info.wait_process_obj_list_ptr->object))
                        {
                            //将当前媒体请求从请求队列中删除
                            BRWList_RemoveObject(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr);
                            //protect:避免在BrwStartNextMediaReq中使用了野指针
                            SCI_TRACE_LOW("[BRW]W ProcMediaContent:wait_process_obj_list_ptr ERROR");
                            if (wap_req_ptr->media_obj_info.meida_list.next != PNULL)
                            {
                                wap_req_ptr->media_obj_info.wait_process_obj_list_ptr = wap_req_ptr->media_obj_info.meida_list.next;
                            }
                            else
                            {
                                wap_req_ptr->media_obj_info.wait_process_obj_list_ptr = PNULL;
                            }
                        }
                        else
                        {
                            //将当前媒体请求从请求队列中删除
                            BRWList_RemoveObject(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr);
                        }
                        media_obj_ptr->is_req_handling = FALSE;
                        FreeMediaObjStruct(media_obj_ptr);
                    } 
                    else
                    {
                        SCI_TRACE_LOW("[BRW] [%s:%d] css file should be downloaded at first. ",__FUNCTION__,__LINE__);
                        break;
                    }
                }

                if (BrwIsSameHost(brw_info_ptr, media_obj_ptr->url_ptr))
                {
                    //start http request     
                    SCI_TRACE_LOW("[%s:%d]dispose media obj: %p\n",__FUNCTION__,__LINE__,media_obj_ptr);
                    BRW_Memset(&param, 0, sizeof(HTTP_REQ_PARAM_T));
                    param.method = BRW_HTTP_METHOD_GET;
                    param.uri_ptr = (uint8*)media_obj_ptr->url_ptr;
                    param.ua_prof_ptr = BrwGetUAProfStr(brw_info_ptr);
                    param.user_agent_ptr = BrwGetUserAgentStr(brw_info_ptr);
                    param.is_sub_media_req = TRUE;
                    
                    if(public_profile_ptr->proxy_opt.is_use_proxy)
                    {
                        param.is_using_relative_url = FALSE;
                    }
                    else
                    {
                        param.is_using_relative_url = TRUE;
                    }
            
                    media_obj_ptr->page_req_id = wap_req_ptr->req_id;
                    http_req_ptr = BrwSendHttpRequest(&param, brw_info_ptr->http_context_id);
                    media_obj_ptr->http_req_ptr = http_req_ptr;
                    media_obj_ptr->is_req_handling = TRUE;
                    
                    has_start_http_req = TRUE;
                    if (PNULL == http_req_ptr)
                    {
                        SCI_TRACE_LOW("[%s:%d]http_req_ptr is NULL\n",__FUNCTION__,__LINE__);
                        //add protect
                        if  ((wap_req_ptr->media_obj_info.wait_process_obj_list_ptr != PNULL)
                            && (media_obj_ptr == wap_req_ptr->media_obj_info.wait_process_obj_list_ptr->object))
                        {
                            //将当前媒体请求从请求队列中删除
                            BRWList_RemoveObject(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr);
                            //protect:避免在BrwStartNextMediaReq中使用了野指针
                            SCI_TRACE_LOW("[BRW]W ProcMediaContent:wait_process_obj_list_ptr ERROR");
                            if (wap_req_ptr->media_obj_info.meida_list.next != PNULL)
                            {
                                wap_req_ptr->media_obj_info.wait_process_obj_list_ptr = wap_req_ptr->media_obj_info.meida_list.next;
                            }
                            else
                            {
                                wap_req_ptr->media_obj_info.wait_process_obj_list_ptr = PNULL;
                            }
                        }
                        else
                        {
                            //将当前媒体请求从请求队列中删除
                            BRWList_RemoveObject(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr);
                        }
                        media_obj_ptr->is_req_handling = FALSE;
                        FreeMediaObjStruct(media_obj_ptr);
                    } 
                }
            }

            if (has_start_http_req == FALSE)
            {
                cur_media_list_ptr = wap_req_ptr->media_obj_info.wait_process_obj_list_ptr;
            
                media_obj_ptr = (MEDIA_OBJ_T*)cur_media_list_ptr->object;
                if (PNULL == media_obj_ptr)
                {
                    BRWDebug_PrintWarning("BrwStartNextMediaReq:wrong media data!");
                    return;
                }
                //start http request   
                SCI_TRACE_LOW("[%s:%d]dispose media obj: %p\n",__FUNCTION__,__LINE__,media_obj_ptr);
                BRW_Memset(&param, 0, sizeof(HTTP_REQ_PARAM_T));
                param.method = BRW_HTTP_METHOD_GET;
                param.uri_ptr = (uint8*)media_obj_ptr->url_ptr;
                param.ua_prof_ptr = BrwGetUAProfStr(brw_info_ptr);
                param.user_agent_ptr = BrwGetUserAgentStr(brw_info_ptr);
                param.is_sub_media_req = TRUE;
                
                if(public_profile_ptr->proxy_opt.is_use_proxy)
                {
                    param.is_using_relative_url = FALSE;
                }
                else
                {
                    param.is_using_relative_url = TRUE;
                }
                
                media_obj_ptr->page_req_id = wap_req_ptr->req_id;
                http_req_ptr = BrwSendHttpRequest(&param, brw_info_ptr->http_context_id);
                media_obj_ptr->http_req_ptr = http_req_ptr;
                media_obj_ptr->is_req_handling = TRUE;
                wap_req_ptr->media_obj_info.wait_process_obj_list_ptr = cur_media_list_ptr->next; 
                
                if (PNULL == http_req_ptr)
                {
                    SCI_TRACE_LOW("[%s:%d]http_req_ptr is NULL\n",__FUNCTION__,__LINE__);
                    BRWList_RemoveObject(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr);
                    media_obj_ptr->is_req_handling = FALSE;
                    FreeMediaObjStruct(media_obj_ptr);
                    BrwStartNextMediaReq(brw_info_ptr);
                } 
            }
        }
    }
#else
    if(wap_req_ptr->media_obj_info.wait_process_obj_list_ptr != PNULL)
    {    
        HTTP_REQ_PARAM_T    param = {0};
        HTTP_REQ_INFO_T *http_req_ptr = PNULL;
        MEDIA_OBJ_T *media_obj_ptr = PNULL;
        BRW_LIST_T  *cur_media_list_ptr = wap_req_ptr->media_obj_info.wait_process_obj_list_ptr;

        media_obj_ptr = (MEDIA_OBJ_T*)cur_media_list_ptr->object;
        if (PNULL == media_obj_ptr)
        {
            BRWDebug_PrintWarning("BrwStartNextMediaReq:wrong media data!");
            return;
        }
        //start http request        
        BRW_Memset(&param, 0, sizeof(HTTP_REQ_PARAM_T));
        param.method = BRW_HTTP_METHOD_GET;
        param.uri_ptr = (uint8*)media_obj_ptr->url_ptr;
        param.ua_prof_ptr = BrwGetUAProfStr(brw_info_ptr);
        param.user_agent_ptr = BrwGetUserAgentStr(brw_info_ptr);
        param.is_sub_media_req = TRUE;
        
        if(public_profile_ptr->proxy_opt.is_use_proxy)
        {
            param.is_using_relative_url = FALSE;
        }
        else
        {
            param.is_using_relative_url = TRUE;
        }
        
        media_obj_ptr->page_req_id = wap_req_ptr->req_id;
        http_req_ptr = BrwSendHttpRequest(&param, brw_info_ptr->http_context_id);
        media_obj_ptr->http_req_ptr = http_req_ptr;
        media_obj_ptr->is_req_handling = TRUE;
        wap_req_ptr->media_obj_info.wait_process_obj_list_ptr = cur_media_list_ptr->next; 

        if (PNULL == http_req_ptr)
        {
            BRWList_RemoveObject(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr);
            media_obj_ptr->is_req_handling = FALSE;
            FreeMediaObjStruct(media_obj_ptr);
            BrwStartNextMediaReq(brw_info_ptr);
        }       
    }
#endif
}
/*
 *	功能：发起媒体请求
 *  参数：brw_info_ptr  浏览器句柄
 *
 */
LOCAL void BrwStartMediaReq(BRW_INFO_T* brw_info_ptr)
{
    WAP_REQUEST_T *wap_req_ptr = PNULL;
    if(brw_info_ptr==PNULL || brw_info_ptr->cur_req_ptr==PNULL)
    {
        //BRWDebug_Printf:"[BRW]W BrwStartMediaReq:brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2847_112_2_18_2_6_55_426,(uint8*)"d",brw_info_ptr);
        return;
    }
    wap_req_ptr = brw_info_ptr->cur_req_ptr;   
    wap_req_ptr->media_obj_info.wait_process_obj_list_ptr = wap_req_ptr->media_obj_info.meida_list.next;
    BrwStartNextMediaReq(brw_info_ptr);
}
/*
 *	功能：重新发起当前的媒体请求
 *  参数：brw_info_ptr  浏览器句柄
 *
 */
LOCAL BOOLEAN BrwRestartCurMediaReq(BRW_INFO_T* brw_info_ptr, const MEDIA_OBJ_T *cur_media_obj_ptr)
{
    WAP_REQUEST_T   *wap_req_ptr = PNULL;
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();
    
    if ((PNULL == brw_info_ptr) || (PNULL == brw_info_ptr->cur_req_ptr) || (PNULL == cur_media_obj_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BrwRestartCurMediaReq:brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_2865_112_2_18_2_6_55_427,(uint8*)"d",brw_info_ptr);
        return FALSE;
    }
    
    wap_req_ptr = brw_info_ptr->cur_req_ptr;

    //重新发起当前的媒体请求
    wap_req_ptr->media_obj_info.wait_process_obj_list_ptr = wap_req_ptr->media_obj_info.meida_list.next;

    if(wap_req_ptr->media_obj_info.wait_process_obj_list_ptr != PNULL)
    {    
        HTTP_REQ_PARAM_T    param = {0};
        HTTP_REQ_INFO_T *http_req_ptr = PNULL;
        MEDIA_OBJ_T *media_obj_ptr = PNULL;
        BRW_LIST_T  *cur_media_list_ptr = wap_req_ptr->media_obj_info.wait_process_obj_list_ptr;

        media_obj_ptr = (MEDIA_OBJ_T*)cur_media_list_ptr->object;
        if ((PNULL == media_obj_ptr) || (cur_media_obj_ptr != media_obj_ptr))
        {
            BRWDebug_PrintWarning("BrwRestartCurMediaReq:wrong media data!");
            return FALSE;
        }
        
        //start http request        
        BRW_Memset(&param, 0, sizeof(HTTP_REQ_PARAM_T));
        param.method = BRW_HTTP_METHOD_GET;
        param.uri_ptr = (uint8*)media_obj_ptr->url_ptr;
        param.ua_prof_ptr = BrwGetUAProfStr(brw_info_ptr);
        param.user_agent_ptr = BrwGetUserAgentStr(brw_info_ptr);
        param.is_sub_media_req = TRUE;
        
        if(public_profile_ptr->proxy_opt.is_use_proxy)
        {
            param.is_using_relative_url = FALSE;
        }
        else
        {
            param.is_using_relative_url = TRUE;
        }
        
        media_obj_ptr->page_req_id = wap_req_ptr->req_id;
        http_req_ptr = BrwSendHttpRequest(&param, brw_info_ptr->http_context_id);
        media_obj_ptr->http_req_ptr = http_req_ptr;
        media_obj_ptr->is_req_handling = TRUE;
        wap_req_ptr->media_obj_info.wait_process_obj_list_ptr = cur_media_list_ptr->next; 

        if (PNULL == http_req_ptr)
        {
            BRWList_RemoveObject(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr);
            media_obj_ptr->is_req_handling = FALSE;
            FreeMediaObjStruct(media_obj_ptr);
            BrwStartNextMediaReq(brw_info_ptr);
        }       
    }
    return TRUE;
}
/*
 *	append a new media request
 */
PUBLIC BOOLEAN BrwAddNewMediaReq(const uint8 *url_ptr,
                                 MEDIA_REQ_TYPE_E          meida_type,
                                 const uint32              control_handler,
                                 const uint32              media_param)
{
    BRW_INFO_T		*brw_info_ptr  = NULL;
    BRW_INSTANCE	instance       = control_handler;
    WAP_REQUEST_T   *wap_req_ptr   = NULL;
	MEDIA_OBJ_T     *media_obj_ptr = NULL;
    uint32          url_len        = BRW_Strlen(url_ptr);
    
    BRWDebug_PrintWarning("[BRW]BrwAddNewMediaReq");
    if(NULL == url_ptr || 0 == control_handler || 0 == url_len)
    {
        BRWDebug_PrintWarning("BrwAddNewMediaReq, input param error!");
        return FALSE;
    }  
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if(brw_info_ptr == PNULL || brw_info_ptr->cur_req_ptr==PNULL)
    {
        BRWDebug_PrintWarning("BrwAddNewMediaReq:brw_info_ptr=0x%x",brw_info_ptr);
        return FALSE;
    }
    wap_req_ptr = brw_info_ptr->cur_req_ptr;   
    
    media_obj_ptr = BrwAllocMediaObj();
    if(0 == media_obj_ptr)
    {
        return FALSE;
    }
    
    media_obj_ptr->url_ptr = BRWMem_Alloc(url_len+1);
    if(media_obj_ptr->url_ptr != NULL)
    {
        BRW_Memset((uint8*)media_obj_ptr->url_ptr, 0, url_len+1);
        BRW_Memcpy((uint8*)media_obj_ptr->url_ptr, url_ptr, url_len);
    }
    else
    {
       BRWMem_Free(media_obj_ptr);
       media_obj_ptr = NULL;
       return FALSE;
    }
    media_obj_ptr->media_req_type = meida_type;
    media_obj_ptr->page_req_id = wap_req_ptr->req_id;
    media_obj_ptr->index = media_param;
    
    SCI_TRACE_LOW("[BRW]BrwAddNewMediaReq:media_obj_ptr = 0x%x, media_obj_ptr->url_ptr =0x%x, %s",media_obj_ptr, media_obj_ptr->url_ptr,media_obj_ptr->url_ptr);
    SCI_TRACE_LOW("[BRW]BrwAddNewMediaReq:media_obj_ptr->media_req_type =%d", media_obj_ptr->media_req_type );

    if(IsMediaObjListEmpty(wap_req_ptr))
    {
        BRWList_Construct(&wap_req_ptr->media_obj_info.meida_list);
        if (media_obj_ptr->media_req_type == MEDIA_REQ_TYPE_CSS)
        {
            BRWList_InsertObjectAtHead(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr); 
        }
        else
        {
            BRWList_AppendObject(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr);
        }
        if(BRW_STATE_PAGE_REQUESTING !=  BrwGetWapReqState(wap_req_ptr))
        {
            BrwSetWapReqState(wap_req_ptr,BRW_STATE_MEDIA_REQUESTING);
            BrwStartMediaReq(brw_info_ptr);
        }
    }
    else
    {
        //css file should be downloaded at first.
        if (media_obj_ptr->media_req_type == MEDIA_REQ_TYPE_CSS)
        {
            BRWList_InsertObjectAtHead(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr); 
            wap_req_ptr->media_obj_info.wait_process_obj_list_ptr = wap_req_ptr->media_obj_info.meida_list.next;
        }
        else
        {
            BRWList_AppendObject(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr);
        }
    }
    SCI_TRACE_LOW("[BRW]BrwAddNewMediaReq:wait_process_obj_list_ptr = 0x%x", wap_req_ptr->media_obj_info.wait_process_obj_list_ptr);

    BRWDebug_PrintWarning("[BRW]BrwAddNewMediaReq");

    return TRUE;
}

/*
 *	desc：process http data，called when http data recv finished
 *  param:brw_info_ptr  browser instance object pointer
 *        req_pptr      http request object pointer's address      
 */
PUBLIC void BrwProcHttpReqDone(BRW_INFO_T *brw_info_ptr,HTTP_REQ_INFO_T** req_pptr)
{
    WAP_REQUEST_T   *wap_req_ptr  = PNULL;
    uint32          wap_req_id    = 0;
    HTTP_REQ_INFO_T *req_ptr      = PNULL;
    BRW_RET         ret           = BRW_ERROR;
    uint8           *dest_buf_ptr = PNULL;
    uint32          dest_len      = 0;
    BRW_ERROR_E     decode_result = BRW_ERR_NO_ERROR;//MS00243653
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();
                    
    if ((PNULL == brw_info_ptr) || (PNULL == req_pptr) || (PNULL == (*req_pptr)))
    {
        //BRWDebug_Printf:"[BRW]W BrwProcHttpReqDone:brw_info_ptr=0x%x,req_pptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3005_112_2_18_2_6_55_428,(uint8*)"dd",brw_info_ptr,req_pptr);
        if ((PNULL != req_pptr) && (PNULL != (*req_pptr)))
        {
            BrwFreeHttpRequest(*req_pptr);
            *req_pptr = PNULL;
        }
        return;
    }
    
    req_ptr = *req_pptr;
    //BRWDebug_Printf:"[BRW]BrwProcHttpReqDone"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3015_112_2_18_2_6_55_429,(uint8*)"");

    //先查找看有没有匹配的wap request
    wap_req_ptr = BrwGetWapReqByHttpReq(brw_info_ptr,req_ptr);
    if (wap_req_ptr !=  PNULL)
    {
        wap_req_id = wap_req_ptr->req_id;
        switch (wap_req_ptr->state)
        {
        case BRW_STATE_PAGE_REQUESTING:
            {
                if (PNULL == wap_req_ptr->http_req_ptr)
                {
                    //BRWDebug_Printf:"[BRW]BrwProcHttpReqDone wap_req_ptr->http_req_ptr == 0"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3028_112_2_18_2_6_55_430,(uint8*)"");
                    return;
                }
                
                //gzip decoding
                if (BRW_HTTP_ENCODING_TYPE_GZIP == wap_req_ptr->http_req_ptr->transfer_encoding)
                {                   
                    decode_result = BRWUtil_DecodeGzip(wap_req_ptr->http_req_ptr->resp_buf_ptr,
                                wap_req_ptr->http_req_ptr->resp_buf_now_len,
                                &dest_buf_ptr, &dest_len, BRW_DEFAULT_MAX_PAGE_CONTENT_LEN);
                                
                    if (BRW_ERR_NO_ERROR != decode_result)
                    {
                        //BRWDebug_Printf:"[BRW]W BrwProcHttpReqDone BRWUtil_DecodeGzip result =%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3041_112_2_18_2_6_55_431,(uint8*)"d", decode_result);
                        BrwProcErr(brw_info_ptr,decode_result);
                        return;
                    }
                    BRWMem_Free(wap_req_ptr->http_req_ptr->resp_buf_ptr);
                    wap_req_ptr->http_req_ptr->resp_buf_ptr = dest_buf_ptr;
                    wap_req_ptr->http_req_ptr->resp_buf_now_len = dest_len;
                    wap_req_ptr->http_req_ptr->resp_buf_max_len = dest_len;
                    
                    //save http data to file
                    BrwSaveDataToFile(dest_buf_ptr, dest_len);
                }

                //调用ProcPageContent处理页面数据
                ret = ProcPageContent(brw_info_ptr,wap_req_ptr);
                if(BRW_SUCCESS != ret)
                {
                    /*MS00218222:出错处理都放置在BrwProcErr中,此处直接修改
                        WapReqState为BRW_STATE_ALL_FINISHED将导致wap req未停止
                    */
                    //BrwSetWapReqState(wap_req_ptr,BRW_STATE_ALL_FINISHED);//处理失败，将状态置为ALL_FINISHED
                    //BRWDebug_Printf:"BrwProcHttpReqDone page err"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3062_112_2_18_2_6_55_432,(uint8*)"");
                    if(BRW_NOMEMORY == ret)
                    {
                        BrwProcErr(brw_info_ptr,BRW_ERR_NO_MEM);
                    }
                    else
                    {
                        BrwProcErr(brw_info_ptr,BRW_ERR_PAGE_ERR);
                    }
                    *req_pptr = PNULL;  
                    return;
                }
                
                //在ProcPageContent中发起了新的页面请求
                if(brw_info_ptr->cur_req_ptr->req_id != wap_req_id)
                {
                    //BRWDebug_Printf:"BrwProcHttpReqDone, new page has been started"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3078_112_2_18_2_6_55_433,(uint8*)"");
                    return;
                }

                if(!IsMediaObjListEmpty(wap_req_ptr))//存在子媒体请求
                {
                    //发起子媒体请求
                    BrwStartMediaReq(brw_info_ptr);
                    if(!IsMediaFinished(wap_req_ptr))
                    {
                        BrwSetWapReqState(wap_req_ptr,BRW_STATE_MEDIA_REQUESTING);
                    }
                }
                else
                {
                    BrwSetWapReqState(wap_req_ptr,BRW_STATE_ALL_FINISHED);
#ifdef JS_SUPPORT
                    //onload event happen
                    BrwProcEvent(wap_req_ptr->layout_handle, EVENT_TYPE_LOAD, NULL, 0, 0,NULL);
#endif
                }
                //BRWDebug_Printf:"[BRW]BrwProcHttpReqDone,777,%x,respbuf:%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3099_112_2_18_2_6_55_434,(uint8*)"dd",req_ptr,req_ptr->resp_buf_ptr);
                //释放http请求资源
                BrwFreeHttpRequest(req_ptr);
                wap_req_ptr->http_req_ptr = PNULL;

            }//BRW_STATE_PAGE_REQUESTING
            break;
            
        case BRW_STATE_DOWNLOADING:
            {            
#ifdef BROWSER_GZIP_SUPPORT
                BOOLEAN is_gzip = FALSE;

                if (wap_req_ptr->http_req_ptr != PNULL)
                {
                    is_gzip = (BRW_HTTP_ENCODING_TYPE_GZIP == wap_req_ptr->http_req_ptr->transfer_encoding)? TRUE: FALSE;
                    //BRWDebug_Printf:"[BRW]BrwProcHttpReqDone,BRW_STATE_DOWNLOADING:transfer_encoding = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3115_112_2_18_2_6_55_435,(uint8*)"d", wap_req_ptr->http_req_ptr->transfer_encoding);
                }
#endif
                if (BRW_DL_STATE_HTTP_CONTENT == wap_req_ptr->download_state)//HTTP下载
                {
                    //释放http请求资源
                    BrwFreeHttpRequest(req_ptr);
                    wap_req_ptr->http_req_ptr = PNULL;
                    
                    if (BRW_PROTOCOL_WAP == public_profile_ptr->protocol)
                    {
                        if(wap_req_ptr->ffs_handler != 0)
                        {
                            BRWPL_CloseFile(wap_req_ptr->ffs_handler);
                            wap_req_ptr->ffs_handler = 0;
                        } 

                        if(PNULL == wap_req_ptr->dl_file_name_ptr)
                        {
                            BrwSetWapReqState(wap_req_ptr,BRW_STATE_ALL_FINISHED);
                        }
                        else
                        {
 #ifdef BROWSER_GZIP_SUPPORT
                            if (is_gzip)
                            {
                                if(!BrwDecodeFileForDl(wap_req_ptr))
                                {
                                    BrwProcDownloadErr(brw_info_ptr,wap_req_ptr,BRW_ERR_UNKNOWN);
                                }
                            }
                            else
#endif/*BROWSER_GZIP_SUPPORT*/                       
                            {
                                if(!BrwFileCopyForWspDl(wap_req_ptr))
                                {
                                    BrwProcDownloadErr(brw_info_ptr,wap_req_ptr,BRW_ERR_UNKNOWN);
                                }
                            }
                        }
                    }
                    else
                    {
#ifdef BROWSER_GZIP_SUPPORT
                        if (is_gzip)
                        {
                           if(!BrwDecodeFileForDl(wap_req_ptr))
                            {
                                BrwProcDownloadErr(brw_info_ptr,wap_req_ptr,BRW_ERR_UNKNOWN);
                            }
                        }
                        else
#endif                       
                        {
                             //通知用户下载完成
                            BrwProcDLSuccess(wap_req_ptr,brw_info_ptr);
                        }
                    }
                }
                else  if ((BRW_DL_STATE_OMA_DD == wap_req_ptr->download_state) 
                    || (BRW_DL_STATE_OMA_CONTENT == wap_req_ptr->download_state))//OMA下载
                {
                    //do nothing
                }
                else
                {
                    BrwProcDownloadErr(brw_info_ptr, wap_req_ptr, BRW_ERR_UNKNOWN);
                }
            }//BRW_STATE_DOWNLOADING
            break;
        
        case BRW_STATE_MEDIA_REQUESTING:/*/媒体请求总*/
            {
               //在ProcMediaContent中会释放req_ptr，此处不需要释放
                //gzip decoding
                if (BRW_HTTP_ENCODING_TYPE_GZIP == req_ptr->transfer_encoding)
                {                   
                    decode_result = BRWUtil_DecodeGzip(req_ptr->resp_buf_ptr,
                                                       req_ptr->resp_buf_now_len,
                                                       &dest_buf_ptr,&dest_len, BRW_DEFAULT_MAX_HTTP_CONTENT_LEN);                                
                    if (BRW_ERR_NO_ERROR == decode_result)
                    {
                        BRWMem_Free(req_ptr->resp_buf_ptr);
                        req_ptr->resp_buf_ptr = dest_buf_ptr;
                        req_ptr->resp_buf_now_len = dest_len;
                        req_ptr->resp_buf_max_len = dest_len;
                        //save http data to file
                        BrwSaveDataToFile(dest_buf_ptr, dest_len);
                    }
                    else
                    {
                        BRWMem_Free(req_ptr->resp_buf_ptr);
                        req_ptr->resp_buf_ptr = PNULL;
                        req_ptr->resp_buf_now_len = 0;
                        req_ptr->resp_buf_max_len = 0;                        
                    }
                    //BRWDebug_Printf:"[BRW]BrwProcHttpReqDone:BRWUtil_DecodeGzip result =%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3211_112_2_18_2_6_56_436,(uint8*)"d", decode_result);
                }

                if(!ProcMediaContent(brw_info_ptr,req_ptr))
                {
                    BRWDebug_PrintWarning("BrwProcHttpReqDone:ProcMediaContent fail");
                }
                if(IsMediaFinished(wap_req_ptr))
                {
                    BrwSetWapReqState(wap_req_ptr,BRW_STATE_ALL_FINISHED);
#ifdef JS_SUPPORT
                    //判断JS解析执行过程中是否需要打开新的页面或者回退
                    if (XhtmlCheckIfJsStartNewPage(wap_req_ptr->layout_handle))
                    {
                        XhtmlSetJsStartNewPageFlag(wap_req_ptr->layout_handle);
                        XHTMLMakeNewRequestByJs(wap_req_ptr->layout_handle);
                    }
                    else if (XhtmlCheckIfJsBackPage(wap_req_ptr->layout_handle))
                    {
                        XhtmlSetJsBackwPageFlag(wap_req_ptr->layout_handle);
                        BRW_ProcNavEvent(brw_info_ptr->instance_id, BRW_NAV_EVENT_BACKWARD);
                    }
                    else if (XhtmlCheckIfJsSubmitForm(wap_req_ptr->layout_handle))
                    {
                        XhtmlSetJsSubmitFormFlag(wap_req_ptr->layout_handle);
                        XHTMLMakeFormSubmitByJs(wap_req_ptr->layout_handle);
                    }
                    else if (XhtmlCheckIfJsClickInput(wap_req_ptr->layout_handle))
                    {
                        XhtmlSetJsClickInputFlag(wap_req_ptr->layout_handle);
                        XhtmlLayoutInputClickProcessByJs(wap_req_ptr->layout_handle);
                    }
                    else if (XhtmlCheckIfJsRecalcPage(wap_req_ptr->layout_handle))
                    {
                        XhtmlSetJsRecalcFlag(wap_req_ptr->layout_handle);
                        BrwLayout(wap_req_ptr->layout_handle, LAYOUT_CAUSE_JS);
                    }
                    //onload event happen, make event object and add
                    BrwProcEvent(wap_req_ptr->layout_handle, EVENT_TYPE_LOAD, NULL, 0, 0, NULL);
#endif
                }
            }//BRW_STATE_MEDIA_REQUESTING
            break;            
        default:
            {
                BRWDebug_PrintWarning("BrwProcHttpReqDone:wrong wap request state");
                //释放http请求资源
                BrwFreeHttpRequest(req_ptr);
                wap_req_ptr->http_req_ptr = PNULL;
            }  
            break;
        }
    }
    else
    {
        BRWDebug_PrintWarning("BrwProcHttpReqDone:failed to find relevant handler");
        //释放http请求资源
        BrwFreeHttpRequest(req_ptr);            
    }
    *req_pptr = PNULL;    
    return;
}
/*
 *desc:	process close Http cnf
 *param:brw_info_ptr browser instance
 */
PUBLIC void BrwProcCloseHttpCnf(BRW_INFO_T* brw_info_ptr)
{
    if(brw_info_ptr && brw_info_ptr->profile.msg_proc_func)
    {
		BrwSetHttpState(brw_info_ptr, BRW_HTTP_STATE_NONE);
        brw_info_ptr->profile.msg_proc_func(BRW_MSG_EXIT_CNF,PNULL,0);
    }
    else
    {
        //BRWDebug_Printf:"BrwProcCloseHttpCnf,brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3259_112_2_18_2_6_56_437,(uint8*)"d",brw_info_ptr);
    }
}
LOCAL BOOLEAN IsInstanceIdEqual(void *brw_info_ptr,void* instance_id)
{
    if ((PNULL==brw_info_ptr) || (0 == instance_id))
    {
        return FALSE;
    }
    return (((BRW_INFO_T*)brw_info_ptr)->instance_id==(uint32)instance_id);
}
#if 0
LOCAL BOOLEAN IsWinHandleEqual(void *brw_info_ptr, void* win_handler)
{
	if(0==brw_info_ptr||0==win_handler)
	{
		return FALSE;
	}
	return (((BRW_INFO_T*)brw_info_ptr)->profile.win_handle==(BRW_WIN_HANDLE)win_handler);
}
#endif
/*
 *	get the browser instance pointer
 */
PUBLIC BRW_INFO_T* BrwGetBrwInfoPtrByInstance(BRW_INSTANCE instance)
{
    BRW_INFO_T	*brw_info_ptr = PNULL;
    
    if (instance != BRW_INVALID_INSTANCE)
    {
        //brw_info_ptr = BRWList_SearchObject(BrwGetInstanceList(),IsInstanceIdEqual,(void*)instance);
        brw_info_ptr = BRW_SearchInstance(IsInstanceIdEqual,(void*)instance);
    }
    return brw_info_ptr;



}
#if 0
LOCAL BRW_INFO_T* BrwGetBrwInfoPtrByWinHandle(BRW_WIN_HANDLE win_handler)
{	
    BRW_INFO_T  *brw_info_ptr = PNULL;

    brw_info_ptr = BRWList_SearchObject(BrwGetInstanceList(),IsWinHandleEqual,(void*)win_handler);
    return brw_info_ptr;
}
#endif
/*
 *	get wap request's parent browser instance pointer
 */
PUBLIC BRW_INFO_T* BrwGetBrwInfoPtrByWapReqID(uint32 req_id)
{
/*
    BRW_INFO_T	*brw_info_ptr=PNULL;
    BRW_LIST_T	*instance_list_ptr = BrwGetInstanceList();

    if(req_id==BRW_INVALID_REQ_ID || !instance_list_ptr)
    {
        //BRWDebug_Printf:"[BRW]W GetBrwInfoPtrByWapReqID,req_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3314_112_2_18_2_6_56_438,(uint8*)"d",req_id);
    }
    else
    {
        brw_info_ptr = BRWList_SearchObject(instance_list_ptr,(BRW_LIST_CMP_F)IsReqBelongToInstance,(void*)req_id);
    }    
    return brw_info_ptr;
    */
    BRW_INFO_T	*brw_info_ptr=PNULL;

    if(BRW_INVALID_REQ_ID == req_id)
    {
        //BRWDebug_Printf:"[BRW]W GetBrwInfoPtrByWapReqID,req_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3326_112_2_18_2_6_56_439,(uint8*)"d",req_id);
    }
    else
    {
        brw_info_ptr = BRW_SearchInstance((BRW_LIST_CMP_F)IsReqBelongToInstance,(void*)req_id);
    }    
    return brw_info_ptr;

}
/*
 *	get browser instance which download session with seesion_id belong to
 */
PUBLIC BRW_INFO_T*   BrwGetBrwInfoPtrByDLSessionID(uint32 session_id)
{
/*
    BRW_INFO_T	*brw_info_ptr=PNULL;
    BRW_LIST_T	*instance_list_ptr = BrwGetInstanceList();

    if(BRW_INVALID_OMA_DL_SESSION_ID==session_id || PNULL==instance_list_ptr)
    {
        //BRWDebug_Printf:"[BRW]W BrwGetBrwInfoPtrByDLSessionID,session_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3344_112_2_18_2_6_56_440,(uint8*)"d",session_id);
    }
    else
    {
        brw_info_ptr = BRWList_SearchObject(instance_list_ptr,(BRW_LIST_CMP_F)IsDLSessionBelongtoInstance,(void*)session_id);
    }    
    return brw_info_ptr;
    */
    BRW_INFO_T	*brw_info_ptr=PNULL;

    if(BRW_INVALID_OMA_DL_SESSION_ID==session_id )
    {
        //BRWDebug_Printf:"[BRW]W BrwGetBrwInfoPtrByDLSessionID,session_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3356_112_2_18_2_6_56_441,(uint8*)"d",session_id);
    }
    else
    {
        brw_info_ptr = BRW_SearchInstance((BRW_LIST_CMP_F)IsDLSessionBelongtoInstance,(void*)session_id);
    }    
    return brw_info_ptr;
}
/*
 *	get browser instance which http_req_ptr belongs to
 */
PUBLIC BRW_INFO_T*   BrwGetBrwInfoPtrByHttpReq(HTTP_REQ_INFO_T* http_req_ptr)
{
/*
    BRW_INFO_T	*brw_info_ptr=PNULL;
    BRW_LIST_T	*instance_list_ptr = BrwGetInstanceList();

    if(http_req_ptr==PNULL || PNULL==instance_list_ptr)
    {
        //BRWDebug_Printf:"[BRW]W BrwGetBrwInfoPtrByHttpReq,http_req_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3373_112_2_18_2_6_56_442,(uint8*)"d",http_req_ptr);
    }
    else
    {
        brw_info_ptr = BRWList_SearchObject(instance_list_ptr,(BRW_LIST_CMP_F)IsHttpReqBelongToInstance,(void*)http_req_ptr);
    }    
    return brw_info_ptr;
    */
    
    BRW_INFO_T	*brw_info_ptr=PNULL;

    if(PNULL == http_req_ptr)
    {
        //BRWDebug_Printf:"[BRW]W BrwGetBrwInfoPtrByHttpReq,http_req_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3386_112_2_18_2_6_56_443,(uint8*)"d",http_req_ptr);
    }
    else
    {
        brw_info_ptr = BRW_SearchInstance((BRW_LIST_CMP_F)IsHttpReqBelongToInstance,(void*)http_req_ptr);
    }    
    return brw_info_ptr;    
}
/*
 *	generate unique page req id
 */
LOCAL uint32 GenReqId(void)
{
    return g_next_req_id++;
}
LOCAL uint32 GenInstanceId(void)
{
    return g_next_instance_id++;
}

/*
 *	get wap request struct pointer acording to request id
 *  req_id:page request id
 */
PUBLIC WAP_REQUEST_T* BrwGetWapReq(uint32 req_id,BRW_INFO_T *brw_info_ptr)
{
    BRW_INFO_T		*temp_brw_info=PNULL;
    WAP_REQUEST_T	*temp_wap_req=PNULL;
    BRW_LIST_T		*temp_req_list_ptr=PNULL;
    uint32  i = 0;

    if ((BRW_INVALID_REQ_ID == req_id) || (PNULL == brw_info_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BrwGetWapReq,req_id=0x%x,brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3417_112_2_18_2_6_56_444,(uint8*)"dd",req_id,brw_info_ptr);
        return PNULL;
    }
    
    for (i=0; i<BRW_INSTANCE_NUM_MAX; i++)
    {
        temp_brw_info = g_brw_instance_list_info.browser_info_ptr[i];
        if (brw_info_ptr == temp_brw_info)
        {
            if(temp_brw_info->cur_req_ptr && (temp_brw_info->cur_req_ptr->req_id==req_id))
            {
                //first check if it's the first request id
                return (WAP_REQUEST_T*)temp_brw_info->cur_req_ptr;
            }
            
            temp_req_list_ptr=temp_brw_info->dl_req_list.next;
            while(temp_req_list_ptr != PNULL)//download req list
            {
                temp_wap_req = (WAP_REQUEST_T*)temp_req_list_ptr->object;
                if((temp_wap_req != PNULL) && (temp_wap_req->req_id == req_id))
                {
                    return temp_wap_req;
                }
                else
                {
                    temp_req_list_ptr = temp_req_list_ptr->next;
                }
            }    
            
            break;//not find
        }
    }

    BRWDebug_PrintWarning("BrwGetWapReq return NULL!");
    return PNULL;
}
/*
 *	get wap request object which oma download session id member equal to session_id
 */
PUBLIC WAP_REQUEST_T* BrwGetWapReqByDLSessionID(BRW_INFO_T* brw_info_ptr,uint32 session_id)
{
    WAP_REQUEST_T *wap_req_ptr=PNULL;

    if(PNULL==brw_info_ptr || BRW_INVALID_OMA_DL_SESSION_ID==session_id)
    {
        //BRWDebug_Printf:"[BRW]W BrwGetWapReqByDLSessionID:brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3461_112_2_18_2_6_56_445,(uint8*)"d",brw_info_ptr);
        return PNULL;
    }
    wap_req_ptr = (WAP_REQUEST_T*)BRWList_SearchObject(&brw_info_ptr->dl_req_list,(BRW_LIST_CMP_F)CompareDLSessionID,(void*)session_id);
    return wap_req_ptr;  
}
/*
 *	get wap request object which http request member equal to req_ptr
 */
PUBLIC WAP_REQUEST_T* BrwGetWapReqByHttpReqEx(BRW_INFO_T *brw_info_ptr,
                                            HTTP_REQ_INFO_T* req_ptr, 
                                            MEDIA_OBJ_T **media_obj_pptr//[IN]/[OUT]:if not pnull,means the http req is for a media obj
                                            )
{
    WAP_REQUEST_T *wap_req_ptr = PNULL;
    MEDIA_OBJ_T *media_obj_ptr = PNULL;

    if ((PNULL == brw_info_ptr) || (PNULL == req_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BrwGetWapReqByHttpReq:brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3479_112_2_18_2_6_56_446,(uint8*)"d",brw_info_ptr);
        return PNULL;
    }
    if(CompareHttpReq(brw_info_ptr->cur_req_ptr, req_ptr))
    {
        wap_req_ptr = brw_info_ptr->cur_req_ptr;
    }
    else
    {
        wap_req_ptr = (WAP_REQUEST_T*)BRWList_SearchObject(&brw_info_ptr->dl_req_list,(BRW_LIST_CMP_F)CompareHttpReq,req_ptr);
    }

    if ((PNULL == wap_req_ptr) && (brw_info_ptr->cur_req_ptr != PNULL) 
        && (BRW_STATE_MEDIA_REQUESTING == brw_info_ptr->cur_req_ptr->state))
    {
        media_obj_ptr = GetMediaObjByHttpReq(brw_info_ptr->cur_req_ptr, req_ptr);
        if (media_obj_ptr != PNULL)
        {
            wap_req_ptr = brw_info_ptr->cur_req_ptr;
        }
    }
    
    if (media_obj_pptr != PNULL)
    {
        *media_obj_pptr = media_obj_ptr;
    }

    return wap_req_ptr;
}

/*
 *	get wap request object which http request member equal to req_ptr
 */
PUBLIC WAP_REQUEST_T* BrwGetWapReqByHttpReq(BRW_INFO_T *brw_info_ptr,HTTP_REQ_INFO_T *req_ptr)
{
    return BrwGetWapReqByHttpReqEx(brw_info_ptr, req_ptr, PNULL);
}
/*
 *	check if mime_type is a webpage mime type
 */
PUBLIC BOOLEAN IsWebPageMimeType(BRW_MIME_TYPE_E mime_type)
{
    if(mime_type== BRW_MIME_HTML ||mime_type == BRW_MIME_WML
        || mime_type == BRW_MIME_WMLC || mime_type== BRW_MIME_PLAINTEXT
        || mime_type == BRW_MIME_WMLS || mime_type==BRW_MIME_WMLSC
        || (mime_type >= BRW_MIME_IMG_BMP && mime_type < BRW_MIME_IMG_UNKNOWN) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*
 *	get media object which http request member equal to req_ptr
 */
LOCAL MEDIA_OBJ_T* GetMediaObjByHttpReq(WAP_REQUEST_T* wap_req_ptr,HTTP_REQ_INFO_T* req_ptr)
{
    MEDIA_OBJ_T	*media_obj_ptr=PNULL;

    if(wap_req_ptr && req_ptr)
    {
        media_obj_ptr = (MEDIA_OBJ_T*)BRWList_SearchObject(&wap_req_ptr->media_obj_info.meida_list,
                                    (BRW_LIST_CMP_F)MediaCompareHttpReq,req_ptr);
    }
    return media_obj_ptr;    
}
/*
 *	check if wap request object's oma_dl_session_id is same as session_id
 */
LOCAL BOOLEAN CompareDLSessionID(WAP_REQUEST_T* req_ptr,uint32 session_id)
{
    if(PNULL!=req_ptr && BRW_INVALID_OMA_DL_SESSION_ID!=session_id)
    {
        return (req_ptr->oma_dl_session_id==session_id);
    }
    return FALSE;
}
/*
 *	check if wap request object's http_req_ptr is same as http_req_ptr
 */
LOCAL BOOLEAN CompareHttpReq(WAP_REQUEST_T* req_ptr,HTTP_REQ_INFO_T *http_req_ptr)
{
    if(req_ptr && http_req_ptr)
    {
        return (req_ptr->http_req_ptr == http_req_ptr);
    }
    return FALSE;
}
/*
 *	check if media object's http_req_ptr is same as http_req_ptr
 */
LOCAL BOOLEAN MediaCompareHttpReq(MEDIA_OBJ_T* media_obj_ptr,HTTP_REQ_INFO_T *http_req_ptr)
{
    if(media_obj_ptr && http_req_ptr)
    {
        return (media_obj_ptr->http_req_ptr == http_req_ptr);
    }
    return FALSE;
}
/*
 *	get instance list
 */
/*
PUBLIC BRW_LIST_T* BrwGetInstanceList(void)
{
    return (BRW_LIST_T*)&s_brw_instance_list;
}*/
/*
 *	get wap request state
 */
PUBLIC BRW_REQ_STATE_E BrwGetWapReqState(WAP_REQUEST_T* req)
{
    if(req)
    {
        return req->state;
    }
    else
    {
        return BRW_STATE_MAX;
    }
}
/*
 *	set wap request state
 */
PUBLIC void BrwSetWapReqState(WAP_REQUEST_T* req,BRW_REQ_STATE_E state)
{
    if(req)
    {
        req->state = state;
    }
}
/*
 *	check if a wap request object exist media request
 */
LOCAL BOOLEAN IsMediaObjListEmpty(WAP_REQUEST_T* req)
{
    if(req)
    {
        return (req->media_obj_info.meida_list.next==PNULL);
    }
    else
    {
        return FALSE;
    }    
}
/*
 *	功能：媒体请求是否完成
 *  参数:req   wap请求对象指针
 *  返回值：完成返回TRUE
 */
LOCAL BOOLEAN IsMediaFinished(WAP_REQUEST_T* req)
{
    return IsMediaObjListEmpty(req);
}

#if 0 /* Reduce Code size */
/*
 *	check if cache is enabled,TRUE if enabled
 */
PUBLIC BOOLEAN BrwIsCacheEnable(BRW_INFO_T *brw_info_ptr)
{
    if(brw_info_ptr)
    {
        return brw_info_ptr->profile.is_cache_enable;
    }
    else
    {
        return FALSE;
    }
}
/*
 *	check if cookie is enabled,TRUE if enabled
 */
PUBLIC BOOLEAN BrwIsCookieEnable(BRW_INFO_T *brw_info_ptr)
{
    if(brw_info_ptr)
    {
        return brw_info_ptr->profile.is_cookie_enable;
    }
    else
    {
        return FALSE;
    }
}

#endif /* Reduce Code size */
/*
 *	if image is set to download
 */
PUBLIC BOOLEAN BrwIsImgDL(BRW_INFO_T *brw_info_ptr)
{
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    return public_profile_ptr->is_image_download;
}

#ifdef MMIDORADO_BGSOUND_SUPPORT
/*
 *	if bgsound is set to download
 */
PUBLIC BOOLEAN BrwIsBgsoundDL(BRW_INFO_T *brw_info_ptr)
{
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    return public_profile_ptr->is_bgsound_download;
}
#endif

/*
 *	get user-agent string
 */
PUBLIC uint8*  BrwGetUserAgentStr(BRW_INFO_T *brw_info_ptr)
{
    uint8   *user_agent_ptr = PNULL;
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();
    
    if (brw_info_ptr != PNULL)
    {
         if (public_profile_ptr->user_agent_ptr != PNULL)
         {
             user_agent_ptr = public_profile_ptr->user_agent_ptr;
         }
         else
         {
             //Get different User_Agent according to different network. 
            MMIBROWSER_GetDefaultUserAgent(&user_agent_ptr);
            if (user_agent_ptr != PNULL)//sava to profile
            {
                uint32  ua_str_len = 0;

                ua_str_len = BRW_Strlen(user_agent_ptr);
                if(ua_str_len > 0)
                {
                    public_profile_ptr->user_agent_ptr = (uint8*)BRWMem_Alloc(ua_str_len+1);   
                    if(public_profile_ptr->user_agent_ptr != PNULL)
                    {
                        BRW_Memcpy(public_profile_ptr->user_agent_ptr, user_agent_ptr, (ua_str_len+1));
                    }
                    else
                    {
                        BRWDebug_PrintWarning("[BRW]BrwGetUserAgentStr,set ua failed because of mem!");
                    }
                }
                else
                {
                    BRWDebug_PrintWarning("[BRW]BrwGetUserAgentStr: ua null!");
                }
            }//if(profile->user_agent_ptr)     
            else
            {
                BRWDebug_PrintWarning("[BRW]BrwGetUserAgentStr: ua null!");
            }
        }
    }
    else
    {
        BRWDebug_PrintWarning("[BRW]BrwGetUserAgentStr:brw_info_ptr NULL");
    }
    
    return user_agent_ptr;

}
/*
 *	get uaprof string
 */
PUBLIC uint8*  BrwGetUAProfStr(BRW_INFO_T* brw_info_ptr)
{
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    if(brw_info_ptr)
    {
        return public_profile_ptr->ua_prof_ptr;
    }
    else
    {
        return PNULL;
    }
}

#if 0 /* Reduce Code size */
/*
 *	check if use proxy
 */
PUBLIC BOOLEAN BrwIsUseProxy(BRW_INFO_T *brw_info_ptr)
{
    if(brw_info_ptr)
    {
        return brw_info_ptr->profile.proxy_opt.is_use_proxy;
    }
    else
    {
        BRWDebug_PrintWarning("BrwIsUseProxy:brw_info_ptr NULL");
        return FALSE;
    }
}
/*
 *	get proxy port
 */
PUBLIC uint16 BrwGetProxyPort(BRW_INFO_T *brw_info_ptr)
{
    if(brw_info_ptr)
    {
        return brw_info_ptr->profile.proxy_opt.proxy_port;
    }
    else
    {
        BRWDebug_PrintWarning("BrwGetProxyPort:brw_info_ptr NULL");
        return 0;
    }
}
/*
 *	get proxy addr
 */
PUBLIC uint8*  BrwGetProxyAddr(BRW_INFO_T* brw_info_ptr)
{
    if(brw_info_ptr)
    {
        return brw_info_ptr->profile.proxy_opt.proxy_addr_ptr;
    }
    else
    {
        BRWDebug_PrintWarning("BrwGetProxyAddr:brw_info_ptr NULL");
        return PNULL;
    }
}
#endif /* Reduce Code size */

PUBLIC void BrwSetHttpState(BRW_INFO_T *brw_info_ptr, BRW_HTTP_STATE_E http_state)
{
	if(brw_info_ptr)
    {
        brw_info_ptr->http_state = http_state;
    }
    else
    {
        BRWDebug_PrintWarning("BrwSetHttpInitState: brw_info_ptr NULL");
    }

}
/*
 *	set value of  member sub_http_module_id of BRW_INFO_T
 */
PUBLIC void BrwSetSubModuleID(BRW_INFO_T *brw_info_ptr,uint32 sub_module_id)
{
    if(brw_info_ptr)
    {
        brw_info_ptr->sub_http_module_id = sub_module_id;
    }
    else
    {
        BRWDebug_PrintWarning("BrwSetSubModuleID:brw_info_ptr NULL");
    }
}

#if 0 /* Reduce Code size */
/*
 *	get value of  member sub_http_module_id of BRW_INFO_T
 */
PUBLIC uint32 BrwGetSubModuleID(BRW_INFO_T *brw_info_ptr)
{
    return brw_info_ptr->sub_http_module_id;
}
#endif /* Reduce Code size */

/*
 *	desc:get profile of BRW_INFO_T struct
 */
PUBLIC BRW_PROFILE_T*  BrwGetProfile(BRW_INFO_T *brw_info_ptr)
{
    if(!brw_info_ptr)
    {
        return PNULL;
    }
    return &brw_info_ptr->profile;
}
/*
 *	desc:set profile for BRW_INFO_T struct
 *  param:brw_info_ptr BRW_INFO_T pointer want to set
 *        profile      profile pointer
 *  return:if SUCCESS return TRUE
 *  
 */
LOCAL BOOLEAN SetProfile(BRW_INFO_T *brw_info_ptr,BRW_PROFILE_T* profile_ptr)
{
    BRW_PROFILE_T	*new_profile_ptr=PNULL;
	
    if((PNULL == brw_info_ptr) || (PNULL == profile_ptr))
    {
        return FALSE;
    }

    //protect: to free
    new_profile_ptr = &brw_info_ptr->profile;
    //copy profile
    BRW_Memcpy(new_profile_ptr, profile_ptr, sizeof(BRW_PROFILE_T));

    return TRUE;
}
/*
 *	desc:init BRW_INFO_T struct
 */
LOCAL void InitBrwInfoStruct(BRW_INFO_T *brw_info_ptr)
{
    if (brw_info_ptr != PNULL)
    { 
        BRW_Memset(brw_info_ptr, 0, sizeof(BRW_INFO_T));
        brw_info_ptr->http_context_id = BrwGetInvalidHttpContextIDValue();
        brw_info_ptr->instance_id = GenInstanceId();
    }    
}
/*
 *	free BRW_INFO_T relevant memory
 */
LOCAL void FreeBrwInfoStruct(BRW_INFO_T *brw_info_ptr)
{
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    if (brw_info_ptr != PNULL)
    {
        if((brw_info_ptr->cur_display_req_ptr  != PNULL) && (brw_info_ptr->cur_req_ptr != brw_info_ptr->cur_display_req_ptr))
        {
            BrwFreeWapReqStruct(brw_info_ptr->cur_display_req_ptr);
            brw_info_ptr->cur_display_req_ptr = PNULL;
        }
        
        if (brw_info_ptr->cur_req_ptr != PNULL)
        {
            BrwFreeWapReqStruct(brw_info_ptr->cur_req_ptr);
            brw_info_ptr->cur_req_ptr = PNULL;
            brw_info_ptr->cur_display_req_ptr = PNULL;
        }
        
        if(0!=brw_info_ptr->cur_page_content.page_content_ptr)
        {
            //BRWMem_Free(brw_info_ptr->cur_page_content.page_content_ptr);
        }
        if(PNULL !=brw_info_ptr->cur_page_content.charset_ptr)
        {
            BRWMem_Free(brw_info_ptr->cur_page_content.charset_ptr);
            brw_info_ptr->cur_page_content.charset_ptr = PNULL;
        }
        if(brw_info_ptr->dl_req_list.next)
        {
            //调用DL接口释放下载队列内存
            BRWList_Destruct(&brw_info_ptr->dl_req_list,(BRW_LIST_FREE_F)BrwFreeWapReqStruct);
        }
        
        if (brw_info_ptr->is_fixed && (brw_info_ptr->snapshot_open_layout_handler != BRW_INVALID_LAYOUT_HANDLE))
        {
            BrwDestroyLayoutCtrl(brw_info_ptr->snapshot_open_layout_handler);
            brw_info_ptr->snapshot_open_layout_handler = BRW_INVALID_LAYOUT_HANDLE;
        }

        if(brw_info_ptr->http_context_id!=BrwGetInvalidHttpContextIDValue())
        {
			BrwSetHttpState(brw_info_ptr, BRW_HTTP_STATE_CLOSING);
            BrwCloseHttpSession(brw_info_ptr->http_context_id,public_profile_ptr->protocol);
        }
        BRWMem_Free(brw_info_ptr);        
    }
}

/*
 *	free BRW_INFO_T relevant memory
 */
PUBLIC void BRW_FreeBrwInfoStructForFixed(BRW_INSTANCE instance)
{
    BRW_INFO_T		*brw_info_ptr=PNULL;
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    
    if (brw_info_ptr != PNULL)
    {
        if((brw_info_ptr->cur_display_req_ptr  != PNULL) && (brw_info_ptr->cur_req_ptr != brw_info_ptr->cur_display_req_ptr))
        {
            BrwFreeWapReqStruct(brw_info_ptr->cur_display_req_ptr);
            brw_info_ptr->cur_display_req_ptr = PNULL;
        }
        
        if (brw_info_ptr->cur_req_ptr != PNULL)
        {
            BrwFreeWapReqStruct(brw_info_ptr->cur_req_ptr);
            brw_info_ptr->cur_req_ptr = PNULL;
            brw_info_ptr->cur_display_req_ptr = PNULL;
        }
        if(0!=brw_info_ptr->cur_page_content.page_content_ptr)
        {
            //BRWMem_Free(brw_info_ptr->cur_page_content.page_content_ptr);
        }
        if(PNULL !=brw_info_ptr->cur_page_content.charset_ptr)
        {
            BRWMem_Free(brw_info_ptr->cur_page_content.charset_ptr);
            brw_info_ptr->cur_page_content.charset_ptr = PNULL;
        }
        
        if (brw_info_ptr->is_fixed && (brw_info_ptr->snapshot_open_layout_handler != BRW_INVALID_LAYOUT_HANDLE))
        {
            BrwDestroyLayoutCtrl(brw_info_ptr->snapshot_open_layout_handler);
            brw_info_ptr->snapshot_open_layout_handler = BRW_INVALID_LAYOUT_HANDLE;
        }
        
        if(brw_info_ptr->dl_req_list.next)
        {
            //调用DL接口释放下载队列内存
            BRWList_Destruct(&brw_info_ptr->dl_req_list,(BRW_LIST_FREE_F)BrwFreeWapReqStruct);
        }

        if(brw_info_ptr->http_context_id!=BrwGetInvalidHttpContextIDValue())
        {
			BrwSetHttpState(brw_info_ptr, BRW_HTTP_STATE_CLOSING);
            BrwCloseHttpSession(brw_info_ptr->http_context_id,public_profile_ptr->protocol);
        }
    }
}

/****************************************************************************/
//Description : to backup the wap request, including the info of: http page data
//				layout, url
//Global resource dependence : 
//Author: juan.zhang
/****************************************************************************/
PUBLIC WAP_REQUEST_T* BrwBackupWapRequest(WAP_REQUEST_T *req)
{
    WAP_REQUEST_T   *backup_req_ptr = PNULL;
    uint32  len = 0;

    backup_req_ptr = BRWMem_Alloc(sizeof(WAP_REQUEST_T));
    //BRWDebug_Printf:"brw alloc wap request in BrwBackupWapRequest:%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_3972_112_2_18_2_6_57_447,(uint8*)"d", backup_req_ptr);
    if(backup_req_ptr != PNULL)
    {
        BRW_Memset(backup_req_ptr, 0, sizeof(WAP_REQUEST_T));
        len = BRW_Strlen(req->absolute_url);
        if (len > 0)
        {
            backup_req_ptr->absolute_url = BRWMem_Alloc(len+1);
            if(PNULL == backup_req_ptr->absolute_url)
            {
                BRWMem_Free(backup_req_ptr);
                return PNULL;
            }
            BRW_Memset(backup_req_ptr->absolute_url, 0, len+1);
            BRW_Strcpy(backup_req_ptr->absolute_url, req->absolute_url);
        }

        backup_req_ptr->layout_handle = req->layout_handle;
        req->has_been_cached = TRUE;

        backup_req_ptr->http_req_ptr = BRWMem_Alloc(sizeof(HTTP_REQ_INFO_T));
        if (PNULL == backup_req_ptr->http_req_ptr)
        {
            if(backup_req_ptr->absolute_url != PNULL)
            {
                BRWMem_Free(backup_req_ptr->absolute_url);
                backup_req_ptr->absolute_url = PNULL;
            }
            BRWMem_Free(backup_req_ptr);
            return PNULL;
        }
        BRW_Memset(backup_req_ptr->http_req_ptr, 0, sizeof(HTTP_REQ_INFO_T));
        backup_req_ptr->http_req_ptr->resp_buf_ptr = req->http_req_ptr->resp_buf_ptr;
        req->http_req_ptr->resp_buf_ptr = PNULL;
        backup_req_ptr->http_req_ptr->resp_buf_now_len = req->http_req_ptr->resp_buf_now_len;
        backup_req_ptr->http_req_ptr->resp_buf_max_len = req->http_req_ptr->resp_buf_max_len;
        backup_req_ptr->http_req_ptr->charset = req->http_req_ptr->charset;
        req->http_req_ptr->charset = PNULL;
        backup_req_ptr->http_req_ptr->mime_type = req->http_req_ptr->mime_type;
        backup_req_ptr->is_cached_req = TRUE;
    }
    return backup_req_ptr;
}
/*
 *	free wap request memory
 */
PUBLIC void BrwFreeWapReqStruct(WAP_REQUEST_T* req)
{
    BRW_INFO_T  *brw_info_ptr = PNULL;
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    if (req != PNULL)
    {
#ifdef MMIDORADO_BGSOUND_SUPPORT
        StopAndFreeBGsound(req->layout_handle, PNULL);
#endif

        //被nav记录cache的req由history负责释放
        if(req->is_cached_req)
        {
            //SCI_TRACE_LOW:"[BRW]BrwFreeWapReqStruct:is_cached_req!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4031_112_2_18_2_6_57_448,(uint8*)"");
            return;
        }
        //SCI_TRACE_LOW:"[BRW]free wap request:0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4034_112_2_18_2_6_57_449,(uint8*)"d",req);
        
        brw_info_ptr = BrwGetBrwInfoPtrByWapReqID(req->req_id);
        if (req->http_req_ptr != PNULL)
        {
            if ((PNULL != brw_info_ptr) && (BRW_HTTP_STATE_INITED == brw_info_ptr->http_state))
            {
                BrwCancelHttpRequest(req->http_req_ptr,
            			brw_info_ptr->http_context_id,
            			public_profile_ptr->protocol);
            }
            else
            {
                BRWDebug_PrintWarning("[BRW]BrwFreeWapReqStruct:http_req_ptr isn't null,but can't get context id!");
            }
            BrwFreeHttpRequest(req->http_req_ptr);
            req->http_req_ptr = PNULL;
        }
        
        BrwFreeDLResource(req, brw_info_ptr);
        
        if(req->media_obj_info.meida_list.next != PNULL)
        {
            //释放媒体信息链表
            BRWList_Destruct(&req->media_obj_info.meida_list, FreeMediaObjStruct);
        }
        
        if(req->absolute_url != PNULL)
        {
            BRWMem_Free(req->absolute_url);
            req->absolute_url = PNULL;
        }
        if(req->original_url != PNULL)
        {
            BRWMem_Free(req->original_url);
            req->original_url = PNULL;
        }
        if(req->body_data_info_ptr != PNULL)
        {
            if(req->body_data_info_ptr->content_type_ptr != PNULL)
            {
                BRWMem_Free(req->body_data_info_ptr->content_type_ptr);
                req->body_data_info_ptr->content_type_ptr = PNULL;
            }
            if (BRW_DATA_SAVE_IN_BUF == req->body_data_info_ptr->mode)
            {
                BRWMem_Free(req->body_data_info_ptr->data_src.addr_ptr);
                req->body_data_info_ptr->data_src.addr_ptr = PNULL;
            }
            else
            {
                BRWMem_Free(req->body_data_info_ptr->data_src.file_name_ptr);
                req->body_data_info_ptr->data_src.file_name_ptr = PNULL;
            }
            
            BRWMem_Free(req->body_data_info_ptr);
            req->body_data_info_ptr = PNULL;
        }//if(req->body_data_info_ptr)

        if(req->layout_handle && !req->has_been_cached)
        {
            BrwDestroyLayoutCtrl(req->layout_handle);
            //BRWDebug_Printf:"[BRW]BrwFreeWapReqStruct,BrwDestroyLayoutCtrl: 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4096_112_2_18_2_6_57_450,(uint8*)"d", req->layout_handle);
            req->layout_handle = BRW_INVALID_LAYOUT_HANDLE;
        }
        if(req->ffs_handler != 0)
        {
            BRWPL_CloseFile(req->ffs_handler);
            req->ffs_handler = 0;
        }
        if(req->dl_file_name_ptr != PNULL)
        {
            BRWMem_Free(req->dl_file_name_ptr);
            req->dl_file_name_ptr = PNULL;
        }
        if(req->wsp_dl_temp_file_name_ptr != PNULL)
        {
            BRWPL_DelFileSyn(req->wsp_dl_temp_file_name_ptr, BRW_WStrlen(req->wsp_dl_temp_file_name_ptr));
            BRWMem_Free(req->wsp_dl_temp_file_name_ptr);
            req->wsp_dl_temp_file_name_ptr = PNULL;
        }

        if (req->ico_filename_ptr != PNULL)
        {
            BRWMem_Free(req->ico_filename_ptr);
            req->ico_filename_ptr = PNULL;
        }

        {
            uint16  web_ico_name[BRW_FULL_PATH_MAX_LEN + 1] = {0};
            uint16  web_ico_name_len = BRW_FULL_PATH_MAX_LEN;
            
            BrwGetWebIcoFullName(web_ico_name, &web_ico_name_len);
            BRWPL_DelFileSyn(web_ico_name, web_ico_name_len);
        }
        
#ifdef BROWSER_GZIP_SUPPORT
        BrwDelDlTempFile();
#endif

        BRWMem_Free(req);
    }
}
/*
 *	desc:free memory of media object
 *  caution:if media request is associate with a http request,please cancel and free it before call FreeMediaObjStruct
 */
LOCAL void FreeMediaObjStruct(void* media_obj_info_ptr)
{
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();
    MEDIA_OBJ_T *media_ptr = (MEDIA_OBJ_T*)media_obj_info_ptr;

    if (media_ptr != PNULL)
    {
        //BRWDebug_Printf:"BRW FreeMediaObjStruct media_obj_info_ptr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4139_112_2_18_2_6_57_451,(uint8*)"d", media_obj_info_ptr);

        if(media_ptr->http_req_ptr != PNULL)
        {
            BRW_INFO_T  *brw_info_ptr = BrwGetBrwInfoPtrByWapReqID(media_ptr->page_req_id);
            
            if ((PNULL != brw_info_ptr) && (BRW_HTTP_STATE_INITED == brw_info_ptr->http_state))
            {
                if(media_ptr->is_req_handling)
                {
                    BrwCancelHttpRequest(media_ptr->http_req_ptr,
                            brw_info_ptr->http_context_id,
                            public_profile_ptr->protocol);
                }
            }
            else
            {
                BRWDebug_PrintWarning("BRW FreeMediaObjStruct:http_req_ptr isn't null but cann't get context id!");
            }
            BrwFreeHttpRequest(media_ptr->http_req_ptr);
            media_ptr->http_req_ptr = PNULL;
        }
//        if(media_obj_info_ptr->url_ptr)
//        {
//            BRWMem_Free(media_obj_info_ptr->url_ptr);
//        }


        if (((MEDIA_REQ_TYPE_ICO == media_ptr->media_req_type) || (MEDIA_REQ_TYPE_CSS == media_ptr->media_req_type))
            && (media_ptr->url_ptr != PNULL))
        {
            BRWMem_Free(media_ptr->url_ptr);/*lint !e605*/
        }

        BRWMem_Free(media_obj_info_ptr);
    }
    else
    {
        BRWDebug_PrintWarning("BRW FreeMediaObjStruct:media_obj_info_ptr null");
    }
}
/*
 *	check if WAP REQUEST OBJECT with req_id belong to browser instance brw_info_ptr
 */
LOCAL BOOLEAN   IsReqBelongToInstance(BRW_INFO_T *brw_info_ptr,uint32 req_id)
{
    if(brw_info_ptr && req_id!=BRW_INVALID_REQ_ID)
    {
        if(brw_info_ptr->cur_req_ptr && brw_info_ptr->cur_req_ptr->req_id==req_id)
        {
            return TRUE;
        }
        else if(brw_info_ptr->cur_display_req_ptr && brw_info_ptr->cur_display_req_ptr->req_id==req_id)
        {
            return TRUE;
        }
        else if(BRWList_SearchObject(&brw_info_ptr->dl_req_list,(BRW_LIST_CMP_F)IsSameReq,(void*)req_id))
        {
            return TRUE;
        }
    }
    return FALSE;
}
/*
 *	desc:check if oma download session with session_id is belong to browser instance brw_info_ptr
 *  input: brw_info_ptr browser instance
 *         session_id  oma download session id
 *  return:TRUE if oma download session with session_id is belong to browser instance brw_info_ptr,else FALSE
 */
LOCAL BOOLEAN IsDLSessionBelongtoInstance(BRW_INFO_T* brw_info_ptr,uint32 session_id)
{
    if(PNULL!=BrwGetWapReqByDLSessionID(brw_info_ptr,session_id))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*
 *	desc:check if http_req_ptr is belong to browser instance brw_info_ptr
 *  input: brw_info_ptr browser instance
 *         http_req_ptr  http request pointer
 *  return:TRUE if http_req_ptr belong to brw_info_ptr,else false
 */
LOCAL BOOLEAN IsHttpReqBelongToInstance(BRW_INFO_T *brw_info_ptr,HTTP_REQ_INFO_T* http_req_ptr)
{
    if(PNULL!=BrwGetWapReqByHttpReq(brw_info_ptr,http_req_ptr))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*
 *	check if wap request object req is same with request object with req_id
 */
LOCAL BOOLEAN IsSameReq(WAP_REQUEST_T *req,uint32 req_id)
{
    if(req && req_id!=BRW_INVALID_REQ_ID)
    {
        if(req->req_id==req_id)
        {
            return TRUE;
        }
    }
    return FALSE;
}
/*
 *	if net is linked
 */
LOCAL void HandleMsgCb(MMIPDP_CNF_INFO_T *msg_ptr)
{
    BOOLEAN result = FALSE;
    BRW_INFO_T  *brw_info_ptr = PNULL;
    uint32	module_id = MMIBRW_MODULE_ID;

    if (PNULL == msg_ptr)
    {
        return;
    }
    
    if (MMIPDP_RESULT_SUCC == msg_ptr->result)
    {
        result = TRUE;
    }
    
    //BRWDebug_Printf:"BRW HandleMsgCb,id:%d,result:%d,handler:%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4264_112_2_18_2_6_58_452,(uint8*)"ddd", msg_ptr->msg_id, msg_ptr->result, msg_ptr->app_handler);

    if (module_id != msg_ptr->app_handler || (!MMIBROWSER_GetIsUseWifi() && MMIPDP_INTERFACE_WIFI == msg_ptr->ps_interface))
    {
        return;
    }
/*
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(MMIBROWSER_GetInstance());
    if (PNULL == brw_info_ptr)
*/
    if (g_brw_instance_list_info.cur_instance_index < 0)//exit browser already
    {
        if ((MMIPDP_DEACTIVE_CNF == msg_ptr->msg_id)//MS00223556:处理MMIPDP_DEACTIVE_CNF,设置参数
            || (MMIPDP_DEACTIVE_IND == msg_ptr->msg_id)//MS00240532:protect
            )
        {
            ProcNetLinkDisConnCnf(PNULL, result);
        }
        return;
    }
    
    brw_info_ptr = BrwGetCurInstanceInfo();
    switch (msg_ptr->msg_id)
    {
        //////////////////////////////////////////////////////////////////////////
        // modified by feng.xiao
    case MMIPDP_ACTIVE_CNF:
        //BRWDebug_Printf:"BRW HandleMsgCb,MMIPDP_ACTIVE_CNF,nsapi:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4290_112_2_18_2_6_58_453,(uint8*)"d", msg_ptr->nsapi);
        g_brw_instance_list_info.net_id = (uint32)msg_ptr->nsapi;
        ProcNetLinkCnf(brw_info_ptr, msg_ptr->result);
        break;
        
    case MMIPDP_DEACTIVE_CNF:
        ProcNetLinkDisConnCnf(brw_info_ptr, result);
        break;
        
    case MMIPDP_DEACTIVE_IND:
        ProcNetLinkDisConnInd(brw_info_ptr);
        break;
        
    case MMIPDP_SUSPEND_IND:
        break;
        
    case MMIPDP_RESUME_IND:
        break;
        
    default:
        break;
    }

	MMI_CheckAllocatedMemInfo();
    //SCI_FREE(msg_ptr);
}
PUBLIC BOOLEAN ActiveNetwork(BRW_INSTANCE instance)
{
    MMIPDP_ACTIVE_INFO_T    active_info = {0};
    //BRW_INFO_T  *brw_info_ptr = PNULL;
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();
    BOOLEAN result = FALSE;
        
    if(BRW_IsNetLinked())
    {
        //BRWDebug_Printf:"[BRW] ActiveNetwork is actived"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4322_112_2_18_2_6_58_454,(uint8*)"");
        return TRUE;
    }
    /*
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if(PNULL == brw_info_ptr)
    {
        //BRWDebug_Printf:"[BRW]W ActiveNetwork brw_info_ptr PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4329_112_2_18_2_6_58_455,(uint8*)"");
        return FALSE;
    }*/
    
#ifdef MMI_WIFI_SUPPORT
    if (public_profile_ptr->is_use_wifi)
    {
        active_info.app_handler = MMIBRW_MODULE_ID;
        active_info.handle_msg_callback = HandleMsgCb;
        active_info.ps_interface = MMIPDP_INTERFACE_WIFI;
        result =  MMIAPIPDP_Active(&active_info);
    }
    else
#endif
    {
        if((PNULL == public_profile_ptr->apn_ptr) || (public_profile_ptr->dual_sys >= MMI_DUAL_SYS_MAX))
        {
            //BRWDebug_Printf:"[BRW]W ActiveNetwork apn_ptr =0x%x, dualz_sys =%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4345_112_2_18_2_6_58_456,(uint8*)"dd",public_profile_ptr->apn_ptr, public_profile_ptr->dual_sys );
            result = FALSE;
        }
        else
        {
            active_info.dual_sys = public_profile_ptr->dual_sys;
            active_info.auth_type = public_profile_ptr->auth_type;
            active_info.priority = public_profile_ptr->browser_pdp_priority;
            active_info.app_handler = MMIBRW_MODULE_ID;
            active_info.handle_msg_callback = HandleMsgCb;
            active_info.apn_ptr = public_profile_ptr->apn_ptr;
            active_info.user_name_ptr = public_profile_ptr->username_ptr;
            active_info.psw_ptr = public_profile_ptr->psw_ptr;
            active_info.ps_service_rat = MN_TD_PREFER;
            active_info.ps_service_type = BROWSER_E;
            active_info.storage = MN_GPRS_STORAGE_ALL;
#ifdef IPVERSION_SUPPORT_V4_V6
	     	active_info.ip_type = public_profile_ptr->ip_type;
#endif
            result =  MMIAPIPDP_Active(&active_info);
        }
    }

    if (result)
    {
        s_brw_pdp_status = BRW_PDP_STATUS_CONNECTING;
    }
    else
    {
        s_brw_pdp_status = BRW_PDP_STATUS_CONNECT_FAIL;
    }
    
    return result;
}
PUBLIC BOOLEAN DeactiveNetwork(BRW_INSTANCE instance)
{
#if 0//@fen.xie 与instance无关
    BRW_INFO_T  *brw_info_ptr = PNULL;

    if(instance != 0)
    {
        brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
        if(PNULL == brw_info_ptr)
        {
            //BRWDebug_Printf:"[BRW] DeactiveNetwork brw_info_ptr is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4374_112_2_18_2_6_58_457,(uint8*)"");
            return FALSE;
        }
        SetNetLinked(FALSE);
        return MMIAPIPDP_Deactive(MMIBRW_MODULE_ID);	
    }
    else
    {
        return FALSE;
    }
#endif

    if (MMIBROWSER_GetIsUseWifi())//wifi情况下,直接SetNetLinked;gprs情况,等待MMIPDP_DEACTIVE_CNF
    {
        SetNetLinked(FALSE);
    }
    
    return MMIAPIPDP_Deactive(MMIBRW_MODULE_ID);	

}
PUBLIC BOOLEAN BRW_IsNetLinked(void)
{
    return g_is_net_linked;
}
PUBLIC void BRW_SetNetLinked(BOOLEAN is_net_linked)
{
    SetNetLinked(is_net_linked);
}

/*
 *	set net linked flag
 */
LOCAL void SetNetLinked(BOOLEAN is_net_linked)
{
    g_is_net_linked = is_net_linked;
    if (g_is_net_linked)
    {
        s_brw_pdp_status = BRW_PDP_STATUS_CONNECT_OK;
    }
}

PUBLIC BRW_PDP_STATUS_E BRW_GetPdpStatus(void)
{
    return s_brw_pdp_status;
}

/*
 *	process confirm of  net link request 
 */
//////////////////////////////////////////////////////////////////////////
// modified by feng.xiao
LOCAL void ProcNetLinkCnf(BRW_INFO_T *brw_info_ptr, MMIPDP_RESULT_E result)
{
    //BRWDebug_Printf:"[BRW]ProcNetLinkCnf,result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4415_112_2_18_2_6_58_458,(uint8*)"d",result);
    
    if (MMIPDP_RESULT_SUCC == result)
    {
        SetNetLinked(TRUE);
    } 
    else
    {
        SetNetLinked(FALSE);
        s_brw_pdp_status = BRW_PDP_STATUS_CONNECT_FAIL;
    }

    if(PNULL == brw_info_ptr)
    {
        //BRWDebug_Printf:"BRW ProcNetLinkCnf:brw_info_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4429_112_2_18_2_6_58_459,(uint8*)"");
        return;
    }
    
    if(MMIPDP_RESULT_SUCC == result)    
    {        
        {
            uint32  i = 0;

            for (i=0; i<BRW_INSTANCE_NUM_MAX; i++)
            {
                if (g_brw_instance_list_info.browser_info_ptr[i] != PNULL
                    && g_brw_instance_list_info.browser_info_ptr[i]->is_fixed)
                {
                    g_brw_instance_list_info.browser_info_ptr[i]->profile.msg_proc_func(BRW_MSG_TYPE_PDP_ACTIVE_CNF, PNULL, 0);
                    break;
                }
            }
        }
    
        ProcAfterNetLinkCnf(brw_info_ptr);
#if 0        
        if ((brw_info_ptr->cur_req_ptr != PNULL) 
            && (BrwGetWapReqState(brw_info_ptr->cur_req_ptr) == BRW_STATE_PAGE_REQUESTING))
        {
            if ((BRW_HTTP_STATE_NONE == brw_info_ptr->http_state) || (BRW_HTTP_STATE_CLOSING == brw_info_ptr->http_state))
            {
                brw_info_ptr->http_context_id = 0;
                BrwInitHttpSession(brw_info_ptr);
            }
            else if (BRW_HTTP_STATE_INITED == brw_info_ptr->http_state)
            {
                BrwStartPageRequest(brw_info_ptr);
            }
            else
            {
                //do nothing
            }
        }
#endif        
    }
    else
    {
        //打开物理链接失败
        BRW_ERROR_E     error = BRW_ERR_NO_ERROR;

#ifdef DATA_ROAMING_SUPPORT
        if (MMIPDP_RESULT_NOT_PERMIT_ROAMING == result)
        {
            error = BRW_ERR_DATA_ROAMING_DISABLED;
        }
        else
#endif
        if(MMIPDP_RESULT_FDN_NOT_PERMIT == result)
        {
            error = BRW_ERR_GPRS_LINK_FDN_ONLY;
        }
        else
        {
            error = BRW_ERR_GPRS_LINK_FAILED;
        }

        if (!brw_info_ptr->is_fixed)
        {
            BrwProcErr(brw_info_ptr,error);
        }
        else
        {
            BRW_MSG_ERROR_IND_T err_ind ={0};

            err_ind.error = error;
            brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_ERROR_IND, &err_ind, sizeof(BRW_MSG_ERROR_IND_T));
        }
    }
}
/*
 *	process net link disconnect indication
 */
LOCAL void ProcNetLinkDisConnInd(BRW_INFO_T *brw_info_ptr)
{
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    //BRWDebug_Printf:"[BRW]ProcNetLinkDisConnInd,brw_info_ptr:0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4496_112_2_18_2_6_58_460,(uint8*)"d", brw_info_ptr);
    
    SetNetLinked(FALSE);
    s_brw_pdp_status = BRW_PDP_STATUS_NONE;

    if(brw_info_ptr != PNULL)
    {
        BOOLEAN is_downloanding = FALSE;
        WAP_REQUEST_T   *wap_req_ptr = PNULL;
        uint32  req_id = MMIBROWSER_GetReqHandle();
        BRW_MSG_DOWNLOAD_ERROR_IND_T dl_err_ind={0};
        
        if (req_id != BRW_INVALID_REQ_ID)//MS00218497
        {
            wap_req_ptr = BrwGetWapReq(req_id, brw_info_ptr);
            if((wap_req_ptr != PNULL) && (BrwGetWapReqState(wap_req_ptr) == BRW_STATE_DOWNLOADING))
            {
                is_downloanding = TRUE;
                dl_err_ind.handle = wap_req_ptr->oma_dl_session_id;
            }
        }
        
        if (!is_downloanding)//停止页面请求
        {
            BRW_StopPageReq(brw_info_ptr->instance_id);
        }
        else/*停止文件的下载*/
        {        
            BrwCancelDL(wap_req_ptr, brw_info_ptr);
        }
        
        if (BRW_HTTP_STATE_INITED == brw_info_ptr->http_state)
        {
            BrwSetHttpState(brw_info_ptr, BRW_HTTP_STATE_CLOSING);
            BrwCloseHttpSession(brw_info_ptr->http_context_id, public_profile_ptr->protocol);
        }
        else if (BRW_HTTP_STATE_INITING == brw_info_ptr->http_state)
        {
            brw_info_ptr->is_need_to_close_http = TRUE;
        }

        //send msg to MMI
        if (!is_downloanding)
        {
            BrwProcErr(brw_info_ptr,BRW_ERR_GPRS_LINK_DISCONNECT);
        }
        else//MS00218497
        {
            dl_err_ind.error = BRW_ERR_GPRS_LINK_DISCONNECT;
            brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_DOWNLOAD_ERR_IND, &dl_err_ind, sizeof(BRW_MSG_DOWNLOAD_ERROR_IND_T));
        }
    }
}
/*
 *	process net link disconnect confirm
 */
LOCAL void ProcNetLinkDisConnCnf(BRW_INFO_T *brw_info_ptr,BOOLEAN result)
{
    if(result)
    {
        SetNetLinked(FALSE);
        s_brw_pdp_status = BRW_PDP_STATUS_NONE;
    }    
}
/*
*得到字符串的哈希值
*/
LOCAL uint32 GetHashValue(uint8* str_ptr)
{
    //哈奇表的构建：字串的后六位为关键字，从后往前
    uint32	h=0;
    uint8	*p=PNULL;
    uint8	i=0;
    uint32 len=0;
    
    if(!str_ptr)
    {
        return 0;
    }
    len=BRW_Strlen(str_ptr);
    p=str_ptr+len;
    len = (len>6)?6:len;
    for(;*p && i<len;p--,i++)
    {
        h=33*h+(*p);//33是英文字串哈希表的经验值
    }
    return h;
}
#if 0
/*
 *	init mine type hash table
 */
LOCAL void InitMineTypeHashTable(void)
{
    uint8 num=0;
    uint8 i=0;
    num = sizeof(g_mime_type_hash_table)/sizeof(HASH_UNIT_T);
    for(;i<num;i++)
    {
        g_mime_type_hash_table[i].hash_value = GetHashValue(g_mime_type_hash_table[i].mime_type_str);
    }
}
#endif
/*
 *	根据mine_type字串得到对应的mine type枚举值
 */
PUBLIC  BRW_MIME_TYPE_E BRW_GetMineType(uint8*   mime_type_str)
{
    uint32	hash_value=0;
    uint8	i=0;
    uint32	num=sizeof(g_mime_type_hash_table)/sizeof(HASH_UNIT_T);

    if(!mime_type_str)
    {
        return BRW_MIME_UNKOWN;
    }
    hash_value=GetHashValue(mime_type_str);
    for(;i<num;i++)
    {
        if(hash_value==g_mime_type_hash_table[i].hash_value)
        {
            if(!BRW_Stricmp(mime_type_str,g_mime_type_hash_table[i].mime_type_str))
            {
                return g_mime_type_hash_table[i].mime_type_value;
            }
        }//if(hash_value==g_mime_type_hash_table[i].hash_value)
    }//for(;i<num;i++)
    switch(*mime_type_str)
    {
    case 'a':
    case 'A':
        if(!BRW_Strnicmp(mime_type_str,(const uint8*)"audio/",BRW_Strlen((const uint8*)"audio/")))
        {
            return BRW_MIME_AUDIO_UNKNOWN;
        }
        break;
    case 'i':
    case 'I':
        if(!BRW_Strnicmp(mime_type_str,(const uint8*)"image/",BRW_Strlen((const uint8*)"image/")))
        {
            return BRW_MIME_IMG_UNKNOWN;
        }
        break;
    case 'v':
    case 'V':
        if(!BRW_Strnicmp(mime_type_str,(const uint8*)"video/",BRW_Strlen((const uint8*)"video/")))
        {
            return BRW_MIME_VIDEO_UNKNOWN;
        }
        break;
        
    default:
        break;
    }
    return BRW_MIME_UNKOWN;
}
/*
 *功能：更新进度消息
 *参数：brw_info_ptr  浏览器句柄
 *      wap_req_ptr   请求句柄
 *      received_data_len:已经收到的数据长度
 *      total_len:总的数据长度，如果无法获知，则传BRW_DEFAULT_MAX_HTTP_CONTENT_LEN
 */
PUBLIC void BrwUpdateProgressInfo(BRW_INFO_T* brw_info_ptr,WAP_REQUEST_T* wap_req_ptr,uint32 received_data_len,uint32 total_len)
{
    BRW_MSG_PROGRESS_IND_T  progress_ind={0};

    if ((brw_info_ptr != PNULL) && (wap_req_ptr != PNULL))
    {
        progress_ind.received_data_len= received_data_len;
        progress_ind.total_data_len = total_len;        
        progress_ind.state = wap_req_ptr->state;
        
        if (BRW_STATE_DOWNLOADING == wap_req_ptr->state)
        {
            progress_ind.is_download_req=TRUE;
            progress_ind.download_req_handle=wap_req_ptr->req_id;            
        }
        brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_PROGRESS_IND,&progress_ind,sizeof(progress_ind));
    }    
}
/*
 *	功能：更新url信息
 *  参数：brw_info_ptr  浏览器句柄
 *        url_ptr       url指针
 */
PUBLIC void BrwUpdateUrl(BRW_INFO_T* brw_info_ptr,uint8* url_ptr)
{
    if(brw_info_ptr != PNULL)
    {
        BRW_MSG_CHANGE_URL_IND_T ind = {0};
        
        BRW_Strncpy(ind.url_arr, url_ptr, BRW_MAX_URL_LEN);
        brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_CHANGE_URL_IND, &ind, sizeof(ind));
    }
}
/*
 *	处理http错误的情况
 *  参数：brw_info_ptr  浏览器句柄
 *        http_req_ptr  http请求指针
 *        err           错误类型
 */
PUBLIC void BrwProcHttpErr(BRW_INFO_T* brw_info_ptr,HTTP_REQ_INFO_T* http_req_ptr,BRW_ERROR_E err)
{
    WAP_REQUEST_T   *wap_req_ptr = PNULL;

    if ((PNULL == brw_info_ptr) || (PNULL == http_req_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BrwProcHttpErr,brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4695_112_2_18_2_6_58_461,(uint8*)"d",brw_info_ptr);
        return;
    }
    
    wap_req_ptr = BrwGetWapReqByHttpReq(brw_info_ptr, http_req_ptr);
    if(wap_req_ptr != PNULL)	
    {
        BRW_REQ_STATE_E state = BrwGetWapReqState(wap_req_ptr);
        //BRWDebug_Printf:"[BRW]BrwProcHttpErr,state = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4703_112_2_18_2_6_58_462,(uint8*)"d",state);

        switch(state)
        {
        case BRW_STATE_PAGE_REQUESTING:
            //无需对失败的子媒体重试,继续其他的子媒体下载
            if (wap_req_ptr->is_retried || (err != BRW_ERR_SERVER_RESET))
            {
                BrwProcErr(brw_info_ptr,err);
            }
            else
            {
                //BRWDebug_Printf:"[BRW]BrwProcHttpErr retry"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4715_112_2_18_2_6_59_463,(uint8*)"");
                BrwFreeHttpRequest(http_req_ptr);
                wap_req_ptr->http_req_ptr = PNULL;
                
                //对该失败的页面请求进行重试
                wap_req_ptr->is_retried = TRUE;
                BrwStartPageRequest(brw_info_ptr);
            }

            break;
        	
        case BRW_STATE_DOWNLOADING:
            //报错，停止下载
            BrwProcDownloadErr(brw_info_ptr,wap_req_ptr,err);
            break;
            
        case BRW_STATE_MEDIA_REQUESTING:
        {
            MEDIA_OBJ_T *media_obj_ptr = GetMediaObjByHttpReq(wap_req_ptr, http_req_ptr);
            
            if (media_obj_ptr != PNULL)
            {
                //无需对失败的子媒体重试,继续其他的子媒体下载
                if (media_obj_ptr->is_retried || (err != BRW_ERR_SERVER_RESET))
                {                    
                    if (MEDIA_REQ_TYPE_IMG == media_obj_ptr->media_req_type)
                    {
                        //notify layout that the image download has failed
                        BrwSetImgBuf(0,0,http_req_ptr->mime_type, wap_req_ptr->layout_handle, media_obj_ptr->index);
                    }
                    //css data
                    else if(media_obj_ptr->media_req_type == MEDIA_REQ_TYPE_CSS)
                    {
                        BrwParseCSS(NULL,0,
                            (const char*)media_obj_ptr->url_ptr,
                            wap_req_ptr->layout_handle,
                            0,
                            media_obj_ptr->index,
                            CSS_TYPE_IMPEND);
                        //need layout again.
                        BrwLayout(wap_req_ptr->layout_handle, LAYOUT_CAUSE_CSS);
                    }
#ifdef JS_SUPPORT
                    else if (media_obj_ptr->media_req_type == MEDIA_REQ_TYPE_JS)
                    {
                        //BrwParseJavaScript(NULL, NULL, wap_req_ptr->layout_handle, NULL);
                    }
#endif
                    brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_MEDIA_GET_ONE_IND,PNULL,0);
                    brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_REPAINT_READY_IND,PNULL,0);

                    //add protect
                    if  ((wap_req_ptr->media_obj_info.wait_process_obj_list_ptr != PNULL)
                        && (media_obj_ptr == wap_req_ptr->media_obj_info.wait_process_obj_list_ptr->object))
                    {
                        //将当前媒体请求从请求队列中删除
                        BRWList_RemoveObject(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr);
                        //protect:避免在BrwStartNextMediaReq中使用了野指针
                        SCI_TRACE_LOW("[BRW]W ProcMediaContent:wait_process_obj_list_ptr ERROR");
                        if (wap_req_ptr->media_obj_info.meida_list.next != PNULL)
                        {
                            wap_req_ptr->media_obj_info.wait_process_obj_list_ptr = wap_req_ptr->media_obj_info.meida_list.next;
                        }
                        else
                        {
                            wap_req_ptr->media_obj_info.wait_process_obj_list_ptr = PNULL;
                        }
                    }
                    else
                    {
                        //将当前媒体请求从请求队列中删除
                        BRWList_RemoveObject(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr);
                    }
                    FreeMediaObjStruct(media_obj_ptr);
                    //发起下一个子媒体请求
                    BrwStartNextMediaReq(brw_info_ptr);
                }
                else
                {
                    //BRWDebug_Printf:"[BRW]BrwProcHttpErr retry"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4770_112_2_18_2_6_59_464,(uint8*)"");
                    
                    BrwFreeHttpRequest(http_req_ptr);
                    media_obj_ptr->http_req_ptr = PNULL;
                    
                    //对该失败的子媒体请求进行重试(不将本子媒体请求删除)
                    media_obj_ptr->is_retried = TRUE;
                    
                    //重新发起当前的子媒体请求
                    BrwRestartCurMediaReq(brw_info_ptr, media_obj_ptr);
                }
                
            }
            else
            {
                //BRWDebug_Printf:"[BRW]BrwProcHttpErr:media_obj_ptr PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4785_112_2_18_2_6_59_465,(uint8*)"");
                BrwFreeHttpRequest(http_req_ptr);
            }            
        }
            break;
            
        default:
            break;
        }
    }
}
/*
 *	desc:cancel media request which belong to brw_info_ptr and wap_req_ptr,and it's http request equal to  http_req_ptr
 */
PUBLIC void BrwCancelMediaReq(BRW_INFO_T* brw_info_ptr,WAP_REQUEST_T* wap_req_ptr,HTTP_REQ_INFO_T* http_req_ptr)
{
    MEDIA_OBJ_T *media_obj_ptr = GetMediaObjByHttpReq(wap_req_ptr, http_req_ptr);
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    if ((PNULL == brw_info_ptr) || (PNULL == wap_req_ptr) || (PNULL == http_req_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BrwCancelMediaReq,brw_info_ptr=0x%x,wap_req_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4805_112_2_18_2_6_59_466,(uint8*)"dd",brw_info_ptr,wap_req_ptr);
        return;
    }
    
    if (media_obj_ptr != PNULL)
    {
        //for UI to calculate the media process
        brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_MEDIA_GET_ONE_IND,PNULL,0);
        brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_REPAINT_READY_IND,PNULL,0);
        
        //add protect
        if  ((wap_req_ptr->media_obj_info.wait_process_obj_list_ptr != PNULL)
            && (media_obj_ptr == wap_req_ptr->media_obj_info.wait_process_obj_list_ptr->object))
        {
            //将当前媒体请求从请求队列中删除
            BRWList_RemoveObject(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr);
            //protect:避免在BrwStartNextMediaReq中使用了野指针
            SCI_TRACE_LOW("[BRW]W ProcMediaContent:wait_process_obj_list_ptr ERROR");
            if (wap_req_ptr->media_obj_info.meida_list.next != PNULL)
            {
                wap_req_ptr->media_obj_info.wait_process_obj_list_ptr = wap_req_ptr->media_obj_info.meida_list.next;
            }
            else
            {
                wap_req_ptr->media_obj_info.wait_process_obj_list_ptr = PNULL;
            }
        }
        else
        {
            //将当前媒体请求从请求队列中删除
            BRWList_RemoveObject(&wap_req_ptr->media_obj_info.meida_list,media_obj_ptr);
        }
        FreeMediaObjStruct(media_obj_ptr);
        //发起下一个子媒体请求
        BrwStartNextMediaReq(brw_info_ptr);
    }    
    else
    {
        //NEWMS00205146:can't find media_obj,so handle the http request only.
        if (BRW_HTTP_STATE_INITED == brw_info_ptr->http_state)
        {
            BrwCancelHttpRequest(http_req_ptr,brw_info_ptr->http_context_id,public_profile_ptr->protocol);
        }
        
        BrwFreeHttpRequest(http_req_ptr);
    }
}
/*
 *	处理http鉴权
 *  参数：brw_info_ptr  浏览器句柄
 *        http_req_ptr  http请求指针
 */
PUBLIC BOOLEAN BrwProcHttpAuth(BRW_INFO_T* brw_info_ptr,HTTP_REQ_INFO_T* http_req_ptr)
{
    WAP_REQUEST_T   *wap_req_ptr=PNULL;

    if ((PNULL == brw_info_ptr) || (PNULL == http_req_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BrwProcHttpAuth,brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4834_112_2_18_2_6_59_467,(uint8*)"d",brw_info_ptr);
        return FALSE;
    }
    
    wap_req_ptr = BrwGetWapReqByHttpReq(brw_info_ptr, http_req_ptr);

    if (wap_req_ptr != PNULL)
    {
        BRW_MSG_AUTH_REQ_T auth_req={0};
        BRW_REQ_STATE_E state = BrwGetWapReqState(wap_req_ptr);
        
        switch (state)
        {
#if 0//delete addr_arr by fen.xie for useless
        case BRW_STATE_PAGE_REQUESTING:
        case BRW_STATE_DOWNLOADING:
            BRW_Strncpy(auth_req.addr_arr, wap_req_ptr->absolute_url, BRW_MAX_URL_LEN);
            break;
            
        case BRW_STATE_MEDIA_REQUESTING:
        {
            MEDIA_OBJ_T *media_obj_ptr = PNULL;
            
            media_obj_ptr = GetMediaObjByHttpReq(wap_req_ptr, http_req_ptr);
            if(media_obj_ptr != PNULL)
            {
                BRW_Strncpy(auth_req.addr_arr, media_obj_ptr->url_ptr, BRW_MAX_URL_LEN);
            }
        }
            break;
#else
        case BRW_STATE_PAGE_REQUESTING:
        case BRW_STATE_DOWNLOADING:
        case BRW_STATE_MEDIA_REQUESTING:
            break;
#endif
            
        default:
            return FALSE;
        }

        auth_req.handle = http_req_ptr->id;
        auth_req.auth_cbf = BrwHttpAuthResp;
        brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_AUTH_REQ, &auth_req, sizeof(auth_req));
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

LOCAL BOOLEAN BrwFileCopyForWspDl(WAP_REQUEST_T *wap_req_ptr)
{
	BRW_INFO_T	*brw_info_ptr = 0;

	if(0==wap_req_ptr || 
		0==wap_req_ptr->dl_file_name_ptr ||
		0==wap_req_ptr->wsp_dl_temp_file_name_ptr)
	{
		return FALSE;
	}

	brw_info_ptr = BrwGetBrwInfoPtrByWapReqID(wap_req_ptr->req_id);
	if(0==brw_info_ptr)
	{
		//BRWDebug_Printf:"BrwFileCopyForWspDl brw_info_ptr=0"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4900_112_2_18_2_6_59_468,(uint8*)"");
		return FALSE;
	}
	if(!BRWPL_FileCopy(wap_req_ptr->wsp_dl_temp_file_name_ptr,wap_req_ptr->dl_file_name_ptr))
	{
		//BRWDebug_Printf:"BrwFileCopyForWspDl copy err"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4905_112_2_18_2_6_59_469,(uint8*)"");
		BrwProcDownloadErr(brw_info_ptr,wap_req_ptr,BRW_ERR_UNKNOWN);
		return FALSE;
	}
	BrwProcDLSuccess(wap_req_ptr,brw_info_ptr);
	return TRUE;
}
LOCAL BOOLEAN BrwSetWspDlFileNm(WAP_REQUEST_T *wap_req_ptr,const uint16* file_name_ptr,uint16 file_name_len)
{
    //BRWPL_FFS_HANDLE	ffs_handler = 0;
    BRW_INFO_T  *brw_info_ptr = PNULL;
#ifdef BROWSER_GZIP_SUPPORT
    BOOLEAN is_gzip = FALSE;
#endif
    BOOLEAN result = FALSE;

    if(PNULL==wap_req_ptr||0==file_name_len||PNULL==file_name_ptr)
    {
        return FALSE;
    }

    wap_req_ptr->dl_file_name_ptr = BRWMem_Alloc(sizeof(uint16)*(file_name_len+1));
    if(PNULL == wap_req_ptr->dl_file_name_ptr)
    {
        //BRWDebug_Printf:"BrwSetWspDlFileNm err, dl_file_name_ptr=0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4929_112_2_18_2_6_59_470,(uint8*)"");
        return FALSE;
    }
    BRW_Memset(wap_req_ptr->dl_file_name_ptr,0,sizeof(uint16)*(file_name_len+1));
    BRW_WstrCat(wap_req_ptr->dl_file_name_ptr, file_name_ptr);

    if(wap_req_ptr->state == BRW_STATE_ALL_FINISHED)
    {
        brw_info_ptr = BrwGetBrwInfoPtrByWapReqID(wap_req_ptr->req_id);
        if(PNULL==brw_info_ptr)
        {
            //BRWDebug_Printf:"BrwSetWspDlFileNm brw_info_ptr=0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4940_112_2_18_2_6_59_471,(uint8*)"");
            return FALSE;
        }

#ifdef BROWSER_GZIP_SUPPORT
        if (wap_req_ptr->http_req_ptr != PNULL)
        {
            //BRWDebug_Printf:"[BRW]BrwSetWspDlFileNm,:transfer_encoding = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4947_112_2_18_2_6_59_472,(uint8*)"d", wap_req_ptr->http_req_ptr->transfer_encoding);
            is_gzip = (BRW_HTTP_ENCODING_TYPE_GZIP == wap_req_ptr->http_req_ptr->transfer_encoding)? TRUE: FALSE;
        }

        //BRWDebug_Printf:"[BRW]BrwSetWspDlFileNm,:is_gzip = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4951_112_2_18_2_6_59_473,(uint8*)"d", is_gzip);
        if (is_gzip)
        {
            result = BrwDecodeFileForDl(wap_req_ptr);
        }
        else
 #endif
        {
            result = BrwFileCopyForWspDl(wap_req_ptr);
        }
        
        if(!result)
        {
            BrwProcDownloadErr(brw_info_ptr,wap_req_ptr,BRW_ERR_UNKNOWN);
        }
    }
    return TRUE;
}
/*
 *	desc:应用输入保存的文件名后的处理
 *  param: wap_req_id       请求id
 *         file_name_ptr    文件名(全路径,unicode)
 *         file_name_len    文件名长度
 */
PUBLIC void BrwEnterFileNmCbf(uint32 wap_req_id,const uint16* file_name_ptr,uint16 file_name_len, BOOLEAN result)
{
    WAP_REQUEST_T	*wap_req_ptr=PNULL;   
    BRW_INFO_T		*brw_info_ptr = BrwGetBrwInfoPtrByWapReqID(wap_req_id);
    BOOLEAN temp_result = FALSE;
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    wap_req_ptr = BrwGetWapReq(wap_req_id,brw_info_ptr);
    if ((PNULL == brw_info_ptr) || (PNULL == wap_req_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BrwEnterFileNmCbf:brw_info_ptr=0x%x,wap_req_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4984_112_2_18_2_6_59_474,(uint8*)"dd",brw_info_ptr,wap_req_ptr);
        return;
    }
    
    if ((BRW_PROTOCOL_HTTP == public_profile_ptr->protocol) && (PNULL == wap_req_ptr->http_req_ptr))
    {
        BRWDebug_PrintWarning("BrwEnterFileNmCbf:http_req_ptr=0");
        return;
    }
    
    if ((PNULL == file_name_ptr) || (0 == file_name_len))
    {
        //BRWDebug_Printf:"[BRW]BrwEnterFileNmCbf,file_name_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_4996_112_2_18_2_6_59_475,(uint8*)"d",file_name_len);
        BRW_StopDL((BRW_INSTANCE)brw_info_ptr, wap_req_id);
        return;
    }

    if (BRW_PROTOCOL_HTTP == public_profile_ptr->protocol)
    {
#ifdef BROWSER_GZIP_SUPPORT
        BOOLEAN is_gzip = FALSE;

         //BRWDebug_Printf:"[BRW]BrwSetWspDlFileNm,:transfer_encoding = %d"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5006_112_2_18_2_6_59_476,(uint8*)"d", wap_req_ptr->http_req_ptr->transfer_encoding);
         is_gzip = (BRW_HTTP_ENCODING_TYPE_GZIP == wap_req_ptr->http_req_ptr->transfer_encoding)? TRUE: FALSE;

        if (is_gzip)
        {
            if (BrwSetWspDlFileNm(wap_req_ptr,file_name_ptr,file_name_len))//save the actual file name
            {
                temp_result = BrwSetHttpDLFileNm(brw_info_ptr->http_context_id,wap_req_ptr->http_req_ptr,
                            wap_req_ptr->wsp_dl_temp_file_name_ptr,MMIAPICOM_Wstrlen(wap_req_ptr->wsp_dl_temp_file_name_ptr));//set to http dl file name for http download
            }
            else
            {
                temp_result = FALSE;
            }
        }
        else
#endif
        {
            temp_result = BrwSetHttpDLFileNm(brw_info_ptr->http_context_id,wap_req_ptr->http_req_ptr,file_name_ptr,file_name_len);
        }

        if (temp_result)
        {
            //do nothing
        }
        else
        {
            //BRWDebug_Printf:"[BRW]BrwEnterFileNmCbf,BrwSetHttpDLFileNm failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5033_112_2_18_2_6_59_477,(uint8*)"");
            BRW_StopDL((BRW_INSTANCE)brw_info_ptr,wap_req_id);
        }
    }
    else if (BRW_PROTOCOL_WAP == public_profile_ptr->protocol)
    {
        if(BrwSetWspDlFileNm(wap_req_ptr,file_name_ptr,file_name_len))
        {
            //success
        }
        else
        {
            //BRWDebug_Printf:"[BRW]BrwEnterFileNmCbf,BrwSetWspDlFileNm failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5045_112_2_18_2_6_59_478,(uint8*)"");
            BRW_StopDL((BRW_INSTANCE)brw_info_ptr,wap_req_id);
        }
    }
    return;
}

#ifdef OMADL_SUPPORT
/*
 *	desc:DD类型文件,应用输入保存的文件名后的处理
 *  param: wap_req_id       请求id
 *         file_name_ptr    文件名(全路径,unicode)
 *         file_name_len    文件名长度
 */
PUBLIC void BrwEnterDdFileNmCbf(uint32 wap_req_id,const uint16* file_name_ptr,uint16 file_name_len, BOOLEAN result)
{
    WAP_REQUEST_T	*wap_req_ptr=PNULL;   
    BRW_INFO_T  *brw_info_ptr = BrwGetBrwInfoPtrByWapReqID(wap_req_id);
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    wap_req_ptr = BrwGetWapReq(wap_req_id,brw_info_ptr);
    if ((PNULL == brw_info_ptr) || (PNULL == wap_req_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BrwEnterFileNmCbf:brw_info_ptr=0x%x,wap_req_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5067_112_2_18_2_6_59_479,(uint8*)"dd",brw_info_ptr,wap_req_ptr);
        return;
    }
    
    if ((PNULL == file_name_ptr) || (0 == file_name_len))
    {
        //BRWDebug_Printf:"[BRW]BrwEnterFileNmCbf,file_name_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5073_112_2_18_2_6_59_480,(uint8*)"d",file_name_len);
        BRW_StopDL((BRW_INSTANCE)brw_info_ptr, wap_req_id);
        return;
    }

    if ((BRW_PROTOCOL_HTTP == public_profile_ptr->protocol)
        || (BRW_PROTOCOL_WAP == public_profile_ptr->protocol))
    {
        //发起odm请求进行oma download
        if(!BrwStartOMADL(wap_req_ptr,brw_info_ptr, file_name_ptr, file_name_len))
        {
            BrwProcDownloadErr(brw_info_ptr,wap_req_ptr,BRW_ERR_UNKNOWN);
            return;
        }

    }
    else
    {
        BrwProcDownloadErr(brw_info_ptr,wap_req_ptr,BRW_ERR_UNKNOWN);
    }
    
    return;
}
#endif

/*
*   desc:get the path of cache
*   param:
*   return:result
*/
PUBLIC BOOLEAN BrwGetCachePath(
                uint16 *cache_path_ptr,//[in/out]
                uint16   *cache_path_len_ptr  //[in/out], 双字节为单位(the max len of cache_path_ptr)
                )
{
    wchar	*device_name_ptr = PNULL;
    uint16	device_name_len = 0;
    BOOLEAN result = FALSE;

    device_name_ptr = BRWPL_GetValidateDevName(&device_name_len);//MS00254277
    result = BRWPL_CombinePath(device_name_ptr, device_name_len,
                    (wchar*)CACHE_FOLDER_PATH, CACHE_FOLDER_PATH_LEN,
                    PNULL, 0,
                    cache_path_ptr, cache_path_len_ptr);
                    
    //BRWDebug_Printf:"[BRW]BrwGetCachePath:result =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5117_112_2_18_2_6_59_481,(uint8*)"d", result);

    return result;
}
/*
*   desc:get the path of cache for fixed
*   param:
*   return:result
*/
PUBLIC BOOLEAN BrwGetFixedCachePath(
                uint16 *cache_path_ptr,//[in/out]
                uint16   *cache_path_len_ptr  //[in/out], 双字节为单位(the max len of cache_path_ptr)
                )
{
    BOOLEAN result = FALSE;
    wchar   cachepath_name_ptr[] = {'\\', 'c','a','c','h','e','s','\\', 0 };
    uint16  cachepath_name_len = BRW_WStrlen(cachepath_name_ptr);
    uint16  cache_path_len_max = 0;
  
    if ((PNULL == cache_path_ptr) || (PNULL == cache_path_len_ptr))
    {
        return FALSE;
    }

    cache_path_len_max = *cache_path_len_ptr;
    
    if (BRWPL_GetValidDevSysBrwRootForWrite(cache_path_ptr, cache_path_len_ptr, PNULL))
    {
        if (cache_path_len_max >= (*cache_path_len_ptr + cachepath_name_len))
        {
            MMI_WSTRNCPY(cache_path_ptr+*cache_path_len_ptr, cachepath_name_len, cachepath_name_ptr, cachepath_name_len, cachepath_name_len);
            result = TRUE;
        }
        else
        {
            
        }
    }

    //BRWDebug_Printf:"[BRW]BrwGetCachePath:result =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5155_112_2_18_2_6_59_482,(uint8*)"d", result);
    return result;
}

/*
*   desc:get the path of cookie(actually the cookie full path name)
*   param:
*   return:result
*/
PUBLIC BOOLEAN BrwGetCookiePath(
                uint16 *cookie_path_ptr,//[in/out]
                uint16   *cookie_path_len_ptr  //[in/out], 双字节为单位(the max len of cookie_path_ptr)
                )
{
    wchar	*device_name_ptr = PNULL;
    uint16	device_name_len = 0;
    BOOLEAN result = FALSE;
    wchar   unicode_dir_name[BRW_ROOT_DIR_LEN+1] = {0};

    device_name_ptr = BRWPL_GetBrwCookieDevName(&device_name_len);
    BRWPL_StrToWstr(BRW_ROOT_DIR, unicode_dir_name);
    result = BRWPL_CombinePath(device_name_ptr, device_name_len,
                    unicode_dir_name, BRW_ROOT_DIR_LEN,
                    (wchar*)COOKIE_NAME, COOKIE_NAME_LEN,
                    cookie_path_ptr, cookie_path_len_ptr);
                    
    //BRWDebug_Printf:"[BRW]BrwGetCookiePath:result =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5180_112_2_18_2_6_59_483,(uint8*)"d", result);

    return result;
}

PUBLIC BOOLEAN BRW_ClearCookie(BRW_INSTANCE instance)
{
    BRW_INFO_T  *brw_info_ptr = PNULL;
    BOOLEAN result = FALSE;
    uint16  cookie_path[BRW_FFS_MAX_PATH + 1] = {0};
    uint16  cookie_path_len = BRW_FFS_MAX_PATH;
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    if(0 == instance)
    {
        //SCI_TRACE_LOW:"BRW BRW_ClearCookie instance =0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5195_112_2_18_2_6_59_484,(uint8*)"");
        return FALSE;
    }
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if(PNULL == brw_info_ptr)
    {
        //SCI_TRACE_LOW:"BRW BRW_ClearCookie brw_info_ptr =0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5201_112_2_18_2_6_59_485,(uint8*)"");
        return FALSE;
    }
    
    if (BrwGetCookiePath(cookie_path, &cookie_path_len))
    {
        if (brw_info_ptr->http_state != BRW_HTTP_STATE_CLOSING)
        {
            result = BrwClearCookie(cookie_path,public_profile_ptr->protocol,brw_info_ptr->http_context_id);
        }
        else
        {
            brw_info_ptr->is_need_to_clear_cookie = TRUE;
            result = TRUE;
        }
    }
    
    return result;
}
PUBLIC BOOLEAN BRW_ClearCache(BRW_INSTANCE instance)
{
    BRW_INFO_T  *brw_info_ptr = PNULL;
    uint16  cache_path[BRW_FFS_MAX_PATH + 1] = {0};
    BOOLEAN result = FALSE;
    uint16  cache_path_len = BRW_FFS_MAX_PATH;
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    if (0 == instance)
    {
        return FALSE;
    }
    
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if (PNULL == brw_info_ptr)
    {
        return FALSE;
    }
    
    if (BrwGetCachePath(cache_path, &cache_path_len))
    {
        result = BrwClearCache(cache_path, public_profile_ptr->protocol);
    }
        
    return result;
}

/*
 *	desc:wap 清理cache
 *  param:
 *  result：
 */
PUBLIC BOOLEAN BRW_ClearAllCache(void)
{
    uint16  cache_path[BRW_FFS_MAX_PATH + 1] = {0};
    BOOLEAN result_1 = FALSE;
    BOOLEAN result_2 = FALSE;
    uint16  cache_path_len = BRW_FFS_MAX_PATH;

    if (BrwGetCachePath(cache_path, &cache_path_len))
    {
        result_1 = BrwClearCache(cache_path, BRW_PROTOCOL_HTTP);
        //BRWDebug_Printf:"[BRW]BRW_ClearAllCache result_1 = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5261_112_2_18_2_7_0_486,(uint8*)"d", result_1);
        result_2 = BrwClearCache(cache_path, BRW_PROTOCOL_WAP);
        //BRWDebug_Printf:"[BRW]BRW_ClearAllCache result_2 = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5263_112_2_18_2_7_0_487,(uint8*)"d", result_2);
    }

    return (result_1 && result_2);
}

/*
 *	desc:wap 清理cookie
 *  param:
 *  result：
 */
PUBLIC BOOLEAN BRW_ClearAllCookie(void)
{
    BOOLEAN result_1 = FALSE;
    BOOLEAN result_2 = FALSE;
    uint16  cookie_path[BRW_FFS_MAX_PATH + 1] = {0};
    uint16  cookie_path_len = BRW_FFS_MAX_PATH;
    uint32  i = 0;

    for (i=0; i<BRW_INSTANCE_NUM_MAX; i++)
    {
        if ((PNULL != g_brw_instance_list_info.browser_info_ptr[i]) && !g_brw_instance_list_info.browser_info_ptr[i]->is_fixed)
        {
            result_1 = BRW_ClearCookie(g_brw_instance_list_info.browser_info_ptr[i]->instance_id);
            SCI_TRACE_LOW("[BRW] BRW_ClearAllCookie result = %d", result_1);
            return result_1;
        }
    }
    
    if (BrwGetCookiePath(cookie_path, &cookie_path_len))
    {
        result_1 = BrwClearCookie(cookie_path, BRW_PROTOCOL_HTTP, 0);
        //BRWDebug_Printf:"[BRW] BRW_ClearAllCookie result_1 = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5283_112_2_18_2_7_0_488,(uint8*)"d", result_1);
        result_2  = BrwClearCookie(cookie_path, BRW_PROTOCOL_WAP, 0);
        //BRWDebug_Printf:"[BRW] BRW_ClearAllCookie result_2 = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5285_112_2_18_2_7_0_489,(uint8*)"d", result_2);
    }

    return (result_1 && result_2);
}

/*
 *	desc:oma download 应用输入文件名确认后的处理
 *  param: wap_req_id       请求id
 *         file_name_ptr    文件名(全路径,unicode)
 *         file_name_len    文件名长度
 *         result：是否进行下载
 */
PUBLIC void BrwOMADLConfirmCbf(uint32 wap_req_id,const uint16* file_name_ptr,uint16 file_name_len,BOOLEAN result)
{
#ifdef OMADL_SUPPORT
    WAP_REQUEST_T   *wap_req_ptr=PNULL;    
    BRW_INFO_T  *brw_info_ptr = BrwGetBrwInfoPtrByWapReqID(wap_req_id);
	
    wap_req_ptr = BrwGetWapReq(wap_req_id,brw_info_ptr);
    if ((PNULL == brw_info_ptr) || (PNULL == wap_req_ptr) 
        || (BRW_INVALID_OMA_DL_SESSION_ID == wap_req_ptr->oma_dl_session_id))
    {
        //BRWDebug_Printf:"[BRW]W BrwOMADLConfirmCbf:brw_info_ptr=0x%x,wap_req_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5307_112_2_18_2_7_0_490,(uint8*)"dd",brw_info_ptr,wap_req_ptr);
        return;
    }   
    
    if (!result || (PNULL == file_name_ptr)|| (0 == file_name_len))//it means cancel
    {
        //BRWDebug_Printf:"[BRW]BrwOMADLConfirmCbf,result =%d, file_name_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5313_112_2_18_2_7_0_491,(uint8*)"dd", result, file_name_ptr);
        BrwCancelDL(wap_req_ptr,brw_info_ptr);   
    }
    else if(!BrwOMADLResp(wap_req_ptr,brw_info_ptr,file_name_ptr,file_name_len))
    {
        BrwProcDownloadErr(brw_info_ptr,wap_req_ptr,BRW_ERR_UNKNOWN); 
    }
    else
    {
        //do nothing
    }
#endif    
    return;
}

PUBLIC BOOLEAN BrwIsCacheUsing(WAP_REQUEST_T* cached_wap_req_ptr)
{
#if 0
    BRW_INFO_T  *brw_info_ptr = PNULL;
    BRW_LIST_T  *instance_list_ptr = BrwGetInstanceList();
    
    if ((PNULL == cached_wap_req_ptr) || (PNULL == instance_list_ptr))
    {
        return FALSE;
    }
    
    instance_list_ptr = instance_list_ptr->next;
    while (PNULL != instance_list_ptr)
    {
        brw_info_ptr = instance_list_ptr->object;
        if(brw_info_ptr != NULL && brw_info_ptr->cur_display_req_ptr != NULL &&
            brw_info_ptr->cur_display_req_ptr->layout_handle == cached_wap_req_ptr->layout_handle)
        {
            return TRUE;
        }
        instance_list_ptr = instance_list_ptr->next;
    }
    return FALSE;
#endif    
    BRW_INFO_T  *brw_info_ptr = PNULL;
    uint32  i = 0;
    
    if (PNULL == cached_wap_req_ptr)
    {
        return FALSE;
    }

    for (i=0; i<BRW_INSTANCE_NUM_MAX; i++)
    {
        brw_info_ptr = g_brw_instance_list_info.browser_info_ptr[i];
        if((brw_info_ptr != PNULL) && (brw_info_ptr->cur_display_req_ptr != PNULL)
            && (brw_info_ptr->cur_display_req_ptr->layout_handle == cached_wap_req_ptr->layout_handle))
        {
            return TRUE;
        }
    }

    return FALSE;

}
PUBLIC BOOLEAN BRW_RamCleanUp(void)
{
	return BrwNavClearCache();
}
#ifdef SSL_SUPPORT
/*
 *	desc:处理不安全证书的确认
 *  参数：brw_info_ptr  浏览器句柄
 *        http_req_ptr  http请求指针
 */
PUBLIC BOOLEAN BrwProcSSLCert(BRW_INFO_T*   brw_info_ptr,HTTP_REQ_INFO_T*   http_req_ptr)
{
    if(brw_info_ptr==PNULL || http_req_ptr==PNULL)
    {
        //BRWDebug_Printf:"[BRW]W BrwProcSSLCert,brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5386_112_2_18_2_7_0_492,(uint8*)"d",brw_info_ptr);
        return FALSE;
    }
    else
    {
        BRW_MSG_SSL_CERT_IND_T ind={0};
		
        ind.handle = http_req_ptr->id;
        ind.cert_cbf = BrwHttpSSLCertResp;
        brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_SSL_CERT_IND,&ind,sizeof(ind));
        return TRUE;
    }
}
#endif
/*
@desc:set screen profile and layout all wap(cur_display_req, snapshot, nav history in cache)//MS00195411
@return:TRUE if success,else FALSE
*/
LOCAL BOOLEAN  BRW_SetScreenProfile(BRW_INFO_T *brw_info_ptr, BRW_SCREEN_OPTION_T screen_opt)
{
    //WAP_REQUEST_T   *wap_req_ptr = PNULL;
    WAP_REQUEST_T   *wap_cache_req_ptr = PNULL;
    LAYOUT_HANDLE   wap_layout_handle = 0;

    if (PNULL == brw_info_ptr)
    {
        //BRWDebug_Printf:"BRW_SetScreenProfile:brw_info_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5411_112_2_18_2_7_0_493,(uint8*)"");
        return FALSE;
    }

    //set profile
    BRW_Memcpy(&brw_info_ptr->profile.screen_opt, &screen_opt, sizeof(BRW_SCREEN_OPTION_T));

    //当前显示的 网页(或已存网页)的layout_handle, 备份的历史导航网页都需要重新的layout

    //当前显示的网页layout_handle
    if (brw_info_ptr->cur_display_req_ptr != PNULL)
    {
        wap_layout_handle = brw_info_ptr->cur_display_req_ptr->layout_handle;
    }
    if (wap_layout_handle != 0)
    {
        BrwSetLayoutInfo(brw_info_ptr->profile.screen_opt, 
                brw_info_ptr->profile.scroll_bar_width, 
                brw_info_ptr->profile.font_type, 
                wap_layout_handle);
        BrwLayout(wap_layout_handle, LAYOUT_CAUSE_SCREENPROFIE);
    }
    
    //当前显示的页面为snapshot时(避免对同一个layout_handler进行重复的layout)
    if ((brw_info_ptr->snapshot_open_layout_handler != 0) && (wap_layout_handle != brw_info_ptr->snapshot_open_layout_handler))
    {
        BrwSetLayoutInfo(brw_info_ptr->profile.screen_opt, 
                brw_info_ptr->profile.scroll_bar_width, 
                brw_info_ptr->profile.font_type, 
                brw_info_ptr->snapshot_open_layout_handler);
        BrwLayout(brw_info_ptr->snapshot_open_layout_handler, LAYOUT_CAUSE_SCREENPROFIE);
    }
    
    //备份的历史导航网页
    while ((wap_cache_req_ptr = BrwNavHisGetPrevLoop()) != PNULL)
    {
        if ((wap_cache_req_ptr->layout_handle != 0) && (wap_layout_handle != wap_cache_req_ptr->layout_handle))
        {
            BrwSetLayoutInfo(brw_info_ptr->profile.screen_opt, 
                    brw_info_ptr->profile.scroll_bar_width, 
                    brw_info_ptr->profile.font_type, 
                    wap_cache_req_ptr->layout_handle);
            BrwLayout(wap_cache_req_ptr->layout_handle, LAYOUT_CAUSE_SCREENPROFIE);
        }
    }

    return TRUE;
}

/*
@desc:set screen profile and layout all wap(cur_display_req, snapshot, nav history in cache)//MS00195411
@return:TRUE if success,else FALSE
*/
LOCAL BOOLEAN  BRW_SetFontProfile(BRW_INFO_T *brw_info_ptr, uint32 font_type)
{
    //WAP_REQUEST_T   *wap_req_ptr = PNULL;
    WAP_REQUEST_T   *wap_cache_req_ptr = PNULL;
    LAYOUT_HANDLE   wap_layout_handle = 0;

    if (PNULL == brw_info_ptr)
    {
        //BRWDebug_Printf:"BRW_SetScreenProfile:brw_info_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5471_112_2_18_2_7_0_494,(uint8*)"");
        return FALSE;
    }

    //set profile
    brw_info_ptr->profile.font_type = font_type;

    //当前显示的 网页(或已存网页)的layout_handle, 备份的历史导航网页都需要重新的layout

    //当前显示的网页layout_handle
    if (brw_info_ptr->cur_display_req_ptr != PNULL)
    {
        wap_layout_handle = brw_info_ptr->cur_display_req_ptr->layout_handle;
    }
    if (wap_layout_handle != 0)
    {
        BrwSetLayoutInfo(brw_info_ptr->profile.screen_opt, 
                brw_info_ptr->profile.scroll_bar_width, 
                brw_info_ptr->profile.font_type, 
                wap_layout_handle);
        BrwLayout(wap_layout_handle, LAYOUT_CAUSE_FONT);
    }
    
    //当前显示的页面为snapshot时(避免对同一个layout_handler进行重复的layout)
    if ((brw_info_ptr->snapshot_open_layout_handler != 0) && (wap_layout_handle != brw_info_ptr->snapshot_open_layout_handler))
    {
        BrwSetLayoutInfo(brw_info_ptr->profile.screen_opt, 
                brw_info_ptr->profile.scroll_bar_width, 
                brw_info_ptr->profile.font_type, 
                brw_info_ptr->snapshot_open_layout_handler);
        BrwLayout(brw_info_ptr->snapshot_open_layout_handler, LAYOUT_CAUSE_FONT);
    }
    
    //备份的历史导航网页
    while ((wap_cache_req_ptr = BrwNavHisGetPrevLoop()) != PNULL)
    {
        if ((wap_cache_req_ptr->layout_handle != 0) && (wap_layout_handle != wap_cache_req_ptr->layout_handle))
        {
            BrwSetLayoutInfo(brw_info_ptr->profile.screen_opt, 
                    brw_info_ptr->profile.scroll_bar_width, 
                    brw_info_ptr->profile.font_type, 
                    wap_cache_req_ptr->layout_handle);
            BrwLayout(wap_cache_req_ptr->layout_handle, LAYOUT_CAUSE_FONT);
        }
    }

    return TRUE;
}

/*
 *  desc:search the url that is include invalid char
 *  param: url_ptr  url
 *  return:TRUE: contain invalid char. FALSE: do not contain invalid char
 */
PUBLIC BOOLEAN Brw_IsIncludeInvalidUrl(const uint8* buf_ptr)
{
    //const uint8 invalid_char[] = {' ', '{', '}', '|', '\\', '^', '~', '[', ']', '`', 0x0D, 0x0A};
    const uint8 invalid_char[] = {0x0D, 0x0A};//回车换行符
    int32 i = 0;
    int32 j = 0;
    uint8 invalid_char_num  = sizeof(invalid_char) ;
    uint16  len = 0;
    
    len = BRW_Strlen(buf_ptr);
    
    if ((PNULL == buf_ptr) || (0 == len))
    {
        return TRUE;
    }

    for (i = 0; i < len; i++)
    {
        if (0x7f < *buf_ptr) /* MS00214875: isn't ASCII*/
        {
            return TRUE;
        }

        for (j = 0; j< invalid_char_num; j++)
        {
            if (*(buf_ptr+i) == invalid_char[j])
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/*
 *  desc:init the memory for browser
 *  param: url_ptr  url
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BRW_InitMem(void)
{
    return BRWMem_InitMem();
}

/*
 *  desc:free the memory for browser
 *  param: url_ptr  url
 *  return:
 */
PUBLIC void BRW_DeleteMem(void)
{
    BRWMem_DeleteMemHandle();
}
/*
 *	free BRW_INFO_T relevant memory
 */
LOCAL void FreeBrwInfoStructEx(void *brw_info_ptr)
{
    FreeBrwInfoStruct((BRW_INFO_T *)brw_info_ptr);
}


/*
* Description:release relevant resouce and memory of s_brw_instance_list
* Param: 
* Return:    
*/
/*
PUBLIC void BRW_DestroyInstanceList(void)
{
    BRW_LIST_T	*instance_list_ptr = PNULL;
    
    instance_list_ptr = BrwGetInstanceList();
    
    BRWList_Destruct(instance_list_ptr, FreeBrwInfoStructEx);
}*/

/*
* Description:for BrwHashDestroy
* Param: 
* Return:    
*/
PUBLIC void BRW_DestroyHash(void)
{
    BrwHashDestroy();
}    

/*
* Description:for BrwParseDestroy
* Param: 
* Return:    
*/
PUBLIC void BRW_DestroyParse(void)
{
    BrwParseDestroy();
}    

/*
 *	desc:获取存放浏览器系统文件的设备名(unicode格式)
 *  param:device_name_len_ptr[out]  设备名长度
 *  return:unicode格式的设备名
 */
PUBLIC uint16* BRW_GetSysDevName(uint16 *device_name_len_ptr)
{
    return BRWPL_GetBrwSysDevName(device_name_len_ptr);
}

/*
 *	desc:获取存放浏览器系统文件的设备名(unicode格式)
 *  param:device_name_len_ptr[out]  设备名长度
 *  return:unicode格式的设备名
 */
PUBLIC uint16* BRW_GetCookieDevName(uint16 *device_name_len_ptr)
{
    return BRWPL_GetBrwCookieDevName(device_name_len_ptr);
}
/*
 *  desc:get the layout handle for current display web page(include snapshot)
 *  param: instance
 *  return:LAYOUT_HANDLE
 */
LOCAL LAYOUT_HANDLE GetCurDisplayLayout(BRW_INSTANCE instance)
{
    BRW_INFO_T		*brw_info_ptr=PNULL;
    WAP_REQUEST_T	*wap_request_ptr=PNULL;
    LAYOUT_HANDLE   layout_handle = BRW_INVALID_LAYOUT_HANDLE;
    
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if (PNULL == brw_info_ptr)
    {
        //BRWDebug_Printf:"GetCurDisplayLayout:brw_info_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5637_112_2_18_2_7_0_495,(uint8*)"");
        return BRW_INVALID_LAYOUT_HANDLE;
    }
    
    wap_request_ptr = brw_info_ptr->cur_display_req_ptr;
    if(PNULL == wap_request_ptr)
    {
        //BRWDebug_Printf:"GetCurDisplayLayout,wap_request_ptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5644_112_2_18_2_7_0_496,(uint8*)"");

        if (brw_info_ptr->snapshot_open_layout_handler != PNULL)//get the snapshot page title
        {
            //BRWDebug_Printf:"GetCurDisplayLayout,get snapshot"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5648_112_2_18_2_7_0_497,(uint8*)"");
            layout_handle = brw_info_ptr->snapshot_open_layout_handler;
        }
    }
    else 
    {
        layout_handle = wap_request_ptr->layout_handle;
    }

    return layout_handle;
}

/*
 *  desc:get the mime type of focus hot link(only for image temp)
 *  param: 
 *  return:
 */
LOCAL  BOOLEAN BrwGetFocusHotLinkInfo(LAYOUT_HANDLE layout_handle,
                uint8 **url_pptr,
                uint16 *url_len_ptr,
                BRW_MIME_TYPE_E *mime_type_ptr
                )
{    
    BRW_IMAGE_INFO_T hotlinkInfo = {0};

    if (layout_handle != BRW_INVALID_LAYOUT_HANDLE)
    {
        if (BrwGetFocusImageInfo(layout_handle, &hotlinkInfo))
        {
            //do nothing
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        BRWDebug_PrintWarning("BRW_GetFocusHotLinkMineType,layout_ctrl_ptr invalid");
        return FALSE;
    }   

    if (url_pptr != PNULL)
    {
        *url_pptr = hotlinkInfo.url_ptr;
    }
    if (url_len_ptr != PNULL)
    {
        *url_len_ptr = hotlinkInfo.url_len;
    }    
    if (mime_type_ptr != PNULL)
    {
        *mime_type_ptr = hotlinkInfo.mime_type;
    }
    
    return TRUE;
}

/*
 *  desc:judge that whether the mime type of focus hot link is an image
 *  param: 
 *  return:
 */
PUBLIC  BOOLEAN BRW_IsFocusHotLinkImg(BRW_INSTANCE instance)
{
    BRW_IMAGE_INFO_T hotlinkInfo = {0};

    BrwGetFocusImageInfo(GetCurDisplayLayout(instance), &hotlinkInfo);
    if ((hotlinkInfo.mime_type >= BRW_MIME_IMG_BMP) && (hotlinkInfo.mime_type < BRW_MIME_IMG_UNKNOWN))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*
 *  desc:get the info of focus hot link(only for image temp)
 *  param: 
 *  return:
 */
PUBLIC  BOOLEAN BRW_GetFocusLinkInfo(BRW_INSTANCE instance,
                uint16 *file_name_ptr,//[in/out]
                uint16 file_name_len_max,
                BRW_MIME_TYPE_E *mime_type_ptr,//[in/out]
                uint8   **data_pptr,    //[in/out]image data
                uint32  *data_len_ptr//[in/out]
                )
{
    
    uint16   url_len = 0;
    uint8   *url_ptr = PNULL;
    LAYOUT_HANDLE   layout_handle = GetCurDisplayLayout(instance);
    BRW_MIME_TYPE_E mime_type = BRW_MIME_UNKOWN;

    if ((mime_type_ptr != PNULL) || (file_name_ptr != PNULL))
    {
        if (BrwGetFocusHotLinkInfo(layout_handle, &url_ptr,  &url_len, &mime_type))
        {
            if (mime_type_ptr != PNULL)
            {
                *mime_type_ptr = mime_type;
            }
        
            if (file_name_ptr != PNULL)
            {
                Brw_DLGetDefaultFileName(file_name_ptr, file_name_len_max, url_ptr, TRUE, PNULL, mime_type);
            }
        }
    }

    BrwGetFocusImgBuf(layout_handle, (const uint8**)data_pptr, data_len_ptr);
    
    return TRUE;
}

#ifdef TOUCH_PANEL_SUPPORT
/*
 *  desc:get the info of current page by tp point(only for image temp)
 *  param:
 *  return:
 */
PUBLIC  BOOLEAN BRW_GetTpPointInfo(BRW_INSTANCE instance,
                uint16 *file_name_ptr,//[in/out]
                uint16 file_name_len_max,
                BRW_MIME_TYPE_E *mime_type_ptr,//[in/out]
                uint8   **data_pptr,    //[in/out]image data
                uint32  *data_len_ptr,//[in/out]
                uint32 x, 
                uint32 y
                )
{    
    LAYOUT_HANDLE   layout_handle = GetCurDisplayLayout(instance);
    BRW_IMAGE_INFO_T hotlinkInfo = {0};

    if ((mime_type_ptr != PNULL) || (file_name_ptr != PNULL))
    {
        if (BrwGetTpPressImageInfo(layout_handle, x, y, &hotlinkInfo))
        {
            if (mime_type_ptr != PNULL)
            {
                *mime_type_ptr = hotlinkInfo.mime_type;
            }
        
            if (file_name_ptr != PNULL)
            {
                Brw_DLGetDefaultFileName(file_name_ptr, file_name_len_max, hotlinkInfo.url_ptr, TRUE, PNULL, hotlinkInfo.mime_type);
            }
            
        }
    }
    
    BrwGetFocusImgBuf(layout_handle, (const uint8**)data_pptr, data_len_ptr);
    return TRUE;
}

/*
 *  desc:judge that whether the mime type of focus hot link is an image
 *  param: url_ptr  url
 *  return:TRUE: contain invalid char. FALSE: do not contain invalid char
 */
PUBLIC  BOOLEAN BRW_IsTpPointImg(BRW_INSTANCE instance,uint32 x, uint32 y)
{
    BRW_IMAGE_INFO_T hotlinkInfo = {0};

    BrwGetTpPressImageInfo(GetCurDisplayLayout(instance), x, y, &hotlinkInfo);
    if ((hotlinkInfo.mime_type >= BRW_MIME_IMG_BMP) && (hotlinkInfo.mime_type < BRW_MIME_IMG_UNKNOWN))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif

#ifdef BROWSER_GZIP_SUPPORT
LOCAL BOOLEAN BrwDecodeFileForDl(WAP_REQUEST_T *wap_req_ptr)
{
    BRW_INFO_T	*brw_info_ptr = 0;
    BRW_ERROR_E decode_result = BRW_ERR_NO_ERROR;
    
    if ((PNULL == wap_req_ptr) 
        || (PNULL ==wap_req_ptr->dl_file_name_ptr) 
        || (PNULL ==wap_req_ptr->wsp_dl_temp_file_name_ptr))
    {
        return FALSE;
    }

    brw_info_ptr = BrwGetBrwInfoPtrByWapReqID(wap_req_ptr->req_id);
    if (PNULL == brw_info_ptr)
    {
        //BRWDebug_Printf:"BrwFileCopyForWspDl brw_info_ptr=0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5836_112_2_18_2_7_1_498,(uint8*)"");
        return FALSE;
    }

    decode_result = BRWUtil_DecodeGzipByFile(wap_req_ptr->wsp_dl_temp_file_name_ptr,wap_req_ptr->dl_file_name_ptr);
    //BRWDebug_Printf:"[BRW]BrwDecodeFileForDl:decode_result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5841_112_2_18_2_7_1_499,(uint8*)"d", decode_result);
    if(BRW_ERR_NO_ERROR == decode_result)
    {
        BrwProcDLSuccess(wap_req_ptr,brw_info_ptr);
    }
    else
    {
        //BrwProcDownloadErr(brw_info_ptr,wap_req_ptr,decode_result);//do it outside
        return FALSE;
    }
    return TRUE;
}
#endif

#ifdef JS_SUPPORT
PUBLIC BOOLEAN BrwCancelWait(void *param, uint16 size_of_param)
{
    BOOLEAN ret = FALSE;

    if(NULL != s_msg_proc_func)
    {
		ret = s_msg_proc_func(BRW_MSG_TYPE_LAYOUTCTRL_DESTROY_REQ, param, size_of_param);
    }

    return ret;
}
#endif

PUBLIC BRW_INFO_T* BrwGetCurInstanceInfo(void)
{
    int16  cur_instance_index = g_brw_instance_list_info.cur_instance_index;
    BRW_INFO_T	*brw_info_ptr = PNULL;
   
    if ((cur_instance_index >= 0) && (cur_instance_index < BRW_INSTANCE_NUM_MAX))
    {
        brw_info_ptr = (g_brw_instance_list_info.browser_info_ptr[cur_instance_index]);
    }
    else
    {
    }

    return brw_info_ptr;
}

/*
 *	desc:set profile for g_brw_instance_list_info
 *  param:profile      profile pointer
 *  return:if SUCCESS return TRUE
 *  
 */
PUBLIC BOOLEAN BrwSetPublicProfile(BRW_PUBLIC_PROFILE_T* profile_ptr)
{
    uint32			len= 0;
    BRW_PUBLIC_PROFILE_T    *new_profile_ptr = PNULL;
    BOOLEAN result = FALSE;
    
    if (PNULL == profile_ptr)
    {
        return FALSE;
    }

    //protect: to free
    new_profile_ptr = &g_brw_instance_list_info.public_profile;
    BrwDestoryPublicProfile();
    
    //copy profile
    do
    {
        BRW_Memcpy(new_profile_ptr, profile_ptr, sizeof(BRW_PUBLIC_PROFILE_T));
        
        if (profile_ptr->proxy_opt.proxy_addr_ptr != PNULL)
        {
            len = BRW_Strlen(profile_ptr->proxy_opt.proxy_addr_ptr);
            if (len > 0)
            {
                new_profile_ptr->proxy_opt.proxy_addr_ptr = (uint8*)BRWMem_Alloc(len+1);
                if(new_profile_ptr->proxy_opt.proxy_addr_ptr != PNULL)
                {
                    BRW_Memcpy(new_profile_ptr->proxy_opt.proxy_addr_ptr, profile_ptr->proxy_opt.proxy_addr_ptr, len+1);
                }
                else
                {
                    //BRWDebug_Printf:"[BRW]W SetPublicProfile:malloc proxy addr failed,len=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5933_112_2_18_2_7_1_500,(uint8*)"d",len);
                    break;
                }
            }
            else
            {
                new_profile_ptr->proxy_opt.proxy_addr_ptr=PNULL;//reset
            }
        }

        if(profile_ptr->ua_prof_ptr  != PNULL)
        {
            len = BRW_Strlen(profile_ptr->ua_prof_ptr);
            if (len > 0)
            {
                new_profile_ptr->ua_prof_ptr = (uint8*)BRWMem_Alloc(len+1);
                if (new_profile_ptr->ua_prof_ptr  != PNULL)
                {
                    BRW_Memcpy(new_profile_ptr->ua_prof_ptr, profile_ptr->ua_prof_ptr, len+1);
                }//if(brw_info_ptrua_prof_ptr)
                else
                {
                    //BRWDebug_Printf:"[BRW]W SetPublicProfile:malloc ua_prof failed,len=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_5956_112_2_18_2_7_1_501,(uint8*)"d",len);
                    break;
                }
            }
            else
            {
                new_profile_ptr->ua_prof_ptr=PNULL;
            }    
        }

        if(profile_ptr->user_agent_ptr != PNULL)
        {
            len=BRW_Strlen(profile_ptr->user_agent_ptr);
            if(len>0)
            {
                new_profile_ptr->user_agent_ptr=(uint8*)BRWMem_Alloc(len+1);
                if(new_profile_ptr->user_agent_ptr != PNULL)
                {
                    BRW_Memcpy(new_profile_ptr->user_agent_ptr,profile_ptr->user_agent_ptr,len+1);
                }
                else
                {
                    SCI_TRACE_LOW("[BRW]W SetPublicProfile:malloc user_agent_ptr failed,len=%d", len);
                    break;
                }
            }
            else
            {
                new_profile_ptr->user_agent_ptr=PNULL;
            }    
        }

        if(profile_ptr->username_ptr != PNULL)
        {
            len=BRW_Strlen((uint8*)profile_ptr->username_ptr);
            if(len>0)
            {
                new_profile_ptr->username_ptr=(char*)BRWMem_Alloc(len+1);
                if(new_profile_ptr->username_ptr != PNULL)
                {
                    BRW_Memcpy(new_profile_ptr->username_ptr,profile_ptr->username_ptr,len+1);
                }
                else
                {
                    SCI_TRACE_LOW("[BRW]W SetPublicProfile:malloc username_ptr failed,len=%d", len);
                    break;
                }
            }
            else
            {
                new_profile_ptr->username_ptr=PNULL;
            }    
        }
        
        if(profile_ptr->psw_ptr != PNULL)
        {
            len=BRW_Strlen((uint8*)profile_ptr->psw_ptr);
            if(len>0)
            {
                new_profile_ptr->psw_ptr=(char*)BRWMem_Alloc(len+1);
                if(new_profile_ptr->psw_ptr != PNULL)
                {
                    BRW_Memcpy(new_profile_ptr->psw_ptr,profile_ptr->psw_ptr,len+1);
                }
                else
                {
                    SCI_TRACE_LOW("[BRW]W SetPublicProfile:malloc psw_ptr failed,len=%d", len);
                    break;
                }
            }
            else
            {
                new_profile_ptr->psw_ptr=PNULL;
            }    
        }

        result = TRUE;
    }while(0);

    if (!result)
    {
        BrwDestoryPublicProfile();
    }

    return result;
}

/*
 *	desc:free profile for g_brw_instance_list_info
 *  param:profile      profile pointer
 *  
 */
PUBLIC void BrwDestoryPublicProfile(void)
{
    BRW_PUBLIC_PROFILE_T    *profile_ptr = PNULL;
	
    //protect: to free
    profile_ptr = &g_brw_instance_list_info.public_profile;
    if(profile_ptr->proxy_opt.proxy_addr_ptr)
    {
        BRWMem_Free(profile_ptr->proxy_opt.proxy_addr_ptr);
        profile_ptr->proxy_opt.proxy_addr_ptr=PNULL;
    }
    if(profile_ptr->ua_prof_ptr != PNULL)
    {
        BRWMem_Free(profile_ptr->ua_prof_ptr);
        profile_ptr->ua_prof_ptr = PNULL;
    }
    if(profile_ptr->user_agent_ptr != PNULL)
    {
        BRWMem_Free(profile_ptr->user_agent_ptr);
        profile_ptr->user_agent_ptr = PNULL;
    }
    if(profile_ptr->username_ptr != PNULL)
    {
        BRWMem_Free(profile_ptr->username_ptr);
        profile_ptr->username_ptr=PNULL;
    }
    if(profile_ptr->psw_ptr != PNULL)
    {
        BRWMem_Free(profile_ptr->psw_ptr);
        profile_ptr->psw_ptr=PNULL;
    }
}

/*
 *	desc:get profile of BRW_PUBLIC_PROFILE_T struct
 */
PUBLIC BRW_PUBLIC_PROFILE_T* BrwGetPublicProfile(void)
{
    return &g_brw_instance_list_info.public_profile;
}

PUBLIC BOOLEAN BRW_AppendInstance(BRW_INFO_T *new_brw_info_ptr)
{
    uint32  i = 0;

    for (i=0; i<BRW_INSTANCE_NUM_MAX; i++)
    {
        if (PNULL == g_brw_instance_list_info.browser_info_ptr[i])
        {
            g_brw_instance_list_info.browser_info_ptr[i] = new_brw_info_ptr;
            g_brw_instance_list_info.cur_instance_index = i;
            return TRUE;
        }
    }

	MMI_CheckAllocatedMemInfo();

    return FALSE;
}


PUBLIC BRW_INFO_T* BRW_SearchInstance(BRW_LIST_CMP_F cmp, void* object_ptr)
{
    uint32  i = 0;

    for (i=0; i<BRW_INSTANCE_NUM_MAX; i++)
    {
        if (cmp(g_brw_instance_list_info.browser_info_ptr[i], object_ptr))
        {
            return g_brw_instance_list_info.browser_info_ptr[i];
        }
    }

    return PNULL;
}

PUBLIC BOOLEAN BRW_RemoveInstance(BRW_INFO_T *brw_info_ptr)
{
    uint32  i = 0;

    for (i=0; i<BRW_INSTANCE_NUM_MAX; i++)
    {
        if (brw_info_ptr == g_brw_instance_list_info.browser_info_ptr[i])
        {
            g_brw_instance_list_info.browser_info_ptr[i] = PNULL;
            g_brw_instance_list_info.cur_instance_index = 0;
            return TRUE;
        }
    }

    return FALSE;
}

PUBLIC void BRW_DestructInstanceList(void)
{
    uint32  i = 0;

    for (i=0; i<BRW_INSTANCE_NUM_MAX; i++)
    {
        if (g_brw_instance_list_info.browser_info_ptr[i] != PNULL)
        {
            BRW_DestroyInstance(g_brw_instance_list_info.browser_info_ptr[i]->instance_id);
            g_brw_instance_list_info.browser_info_ptr[i] = PNULL;
            g_brw_instance_list_info.cur_instance_index = 0;
        }
    }
}
/*
 *	process  after  net link request 
 */
LOCAL BOOLEAN ProcAfterNetLinkCnf(BRW_INFO_T *brw_info_ptr)
{
    if(PNULL != brw_info_ptr)
    {        
        if ((brw_info_ptr->cur_req_ptr != PNULL) 
            && (BrwGetWapReqState(brw_info_ptr->cur_req_ptr) == BRW_STATE_PAGE_REQUESTING))
        {
            if ((BRW_HTTP_STATE_NONE == brw_info_ptr->http_state) || (BRW_HTTP_STATE_CLOSING == brw_info_ptr->http_state))
            {
                brw_info_ptr->http_context_id = 0;
                BrwInitHttpSession(brw_info_ptr);
            }
            else if (BRW_HTTP_STATE_INITED == brw_info_ptr->http_state)
            {
                if(!BrwStartPageRequest(brw_info_ptr))
                {
                    return FALSE;
                }
            }
            else
            {
                //do nothing:BRW_HTTP_STATE_INITING,so do in BrwProcInitHttpCnf
            }
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*
 *	process  after  net link request 
 */
PUBLIC BOOLEAN Brw_ProcAfterNetLinkCnf(BRW_INSTANCE instance)
{
    BRW_INFO_T		*brw_info_ptr=PNULL;

    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
        
    return ProcAfterNetLinkCnf(brw_info_ptr);
}

/*
* Description:for BRW_ParseInit
* Param: 
* Return:    
*/
PUBLIC void BRW_ParseInit(void)
{
    BrwParseInit();
}    

#ifdef MMIDORADO_BGSOUND_SUPPORT
/*****************************************************************************/
//  Description : stop BG sound and free the bgsound data
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StopAndFreeBGsound(
            LAYOUT_HANDLE    *layout_handle,//[in]priority 1
            BRW_INFO_T *brw_info_ptr//[in]priority 2
            )
{
    LAYOUT_HANDLE cur_layout_handle = layout_handle;
    BGSOUND_INFO bgsound = {0};

    //SCI_TRACE_LOW:"[BRW]StopAndFreeBGsound"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_6172_112_2_18_2_7_1_503,(uint8*)"");
    
    if (cur_layout_handle != PNULL)//stop the bgsound of layout_handle
    {
        //BRWDebug_Printf:"StopAndFreeBGsound,layout_handle=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_6176_112_2_18_2_7_1_504,(uint8*)"d", cur_layout_handle);
    }
    else
    {
        BRW_INFO_T	*cur_brw_info_ptr = brw_info_ptr;
        WAP_REQUEST_T	*wap_request_ptr = PNULL;

        if (PNULL == brw_info_ptr)
        {
            cur_brw_info_ptr = BrwGetCurInstanceInfo();//stop the bgsound of currnet instance's current display req
        }
        else
        {
            //stop the bgsound of brw_info_ptr's current display req
        }

        if (PNULL != cur_brw_info_ptr)
        {
            wap_request_ptr = cur_brw_info_ptr->cur_display_req_ptr;
            if(PNULL == wap_request_ptr)
            {
                //BRWDebug_Printf:"StopAndFreeBGsound,wap_request_ptr null"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_6197_112_2_18_2_7_1_505,(uint8*)"");

                if (cur_brw_info_ptr->snapshot_open_layout_handler != PNULL)//get the snapshot page title
                {
                    //BRWDebug_Printf:"StopAndFreeBGsound,get snapshot"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_6201_112_2_18_2_7_1_506,(uint8*)"");
                    cur_layout_handle = cur_brw_info_ptr->snapshot_open_layout_handler;
                }
            }
            else 
            {
                cur_layout_handle = wap_request_ptr->layout_handle;
            }
        }        
    }
    
    //BRWDebug_Printf:"StopAndFreeBGsound,cur_layout_handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_CONTROL_6212_112_2_18_2_7_1_507,(uint8*)"d", cur_layout_handle);

    if(BrwGetBgsoundBuf(&(bgsound.data_ptr),&(bgsound.data_len),&(bgsound.bgsound_type),PNULL,cur_layout_handle, 0))
    { 
        if (bgsound.data_ptr != PNULL)
        {
            BrwFreeBgsoundBuf(cur_layout_handle);//NEWMS00168941,for memory,free the bgsound data immediately
            BrwStopBGsound();
        }
    }

	MMI_CheckAllocatedMemInfo();

    return TRUE;
}

/*****************************************************************************/
//  Description : stop BG sound and free the bgsound data
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Brw_StopAndFreeBGsound(BRW_INSTANCE instance)
{
    BRW_INFO_T  *brw_info_ptr = PNULL;    
    
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);

    return StopAndFreeBGsound(PNULL, brw_info_ptr);
}
#endif

/*
*   desc:get the full file name of the ico of current web page
*   param:
*   return:result
*/
PUBLIC BOOLEAN BrwGetWebIcoFullName(
                uint16 *ico_name_ptr,//[in/out]
                uint16   *ico_name_len_ptr  //[in/out], 双字节为单位(the max len of cache_path_ptr)
                )
{
    wchar	*device_name_ptr = PNULL;
    uint16	device_name_len = 0;
    BOOLEAN result = FALSE;
    wchar   unicode_dir_name[BRW_ROOT_DIR_LEN+1] = {0};

    device_name_ptr = BRWPL_GetValidateDevName(&device_name_len);
    BRWPL_StrToWstr(BRW_ROOT_DIR, unicode_dir_name);
    result = BRWPL_CombinePath(device_name_ptr, device_name_len,
                    unicode_dir_name, BRW_ROOT_DIR_LEN,
                    WEB_ICO_NAME, WEB_ICO_NAME_LEN,
                    ico_name_ptr, ico_name_len_ptr);

    return result;
}

/*
 *	desc:check if layout_handle is belong to browser instance brw_info_ptr->cur_display_req_ptr 
 *  input: brw_info_ptr browser instance
 *         layout_handle
 *  return:TRUE if layout belong to brw_info_ptr->cur_display_req_ptr ,else false
 */
LOCAL BOOLEAN IsLayoutBelongToInstanceCurDisplay(BRW_INFO_T *brw_info_ptr,LAYOUT_HANDLE layout_handle)
{
    if ((brw_info_ptr != PNULL) && (brw_info_ptr->cur_display_req_ptr != PNULL))
    {
        if (layout_handle == brw_info_ptr->cur_display_req_ptr->layout_handle)
        {
            return TRUE;
        }
    }
    return FALSE;
}

#ifndef MMI_BROWSER_MINI_SUPPORT
LOCAL void BrwFreeItemInfoMem(BRW_IMAGE_LIST_ITEM_INFO_T *item_info_ptr)
{
	if (PNULL == item_info_ptr)/* parasoft-suppress BD-RES-INVFREE "有判断检查" */
	{
		return;
	}
    
	if(item_info_ptr->name_ptr != PNULL)/* parasoft-suppress BD-RES-INVFREE "有判断检查" */
	{
		BRWMem_Free(item_info_ptr->name_ptr);
	}
	BRWMem_Free(item_info_ptr);
}

LOCAL void BrwImageListItemDestruct(void *item_info_ptr)
{
	BrwFreeItemInfoMem((BRW_IMAGE_LIST_ITEM_INFO_T*)item_info_ptr);/* parasoft-suppress BD-RES-INVFREE "有判断检查" */
}

LOCAL BRW_IMAGE_LIST_ITEM_INFO_T* BrwSetItemInfo(BRW_IMAGE_INFO_LIST_T *img_list_ptr)
{
    BRW_IMAGE_LIST_ITEM_INFO_T *item_info_ptr = PNULL;
    wchar   name[BRW_MAX_URL_LEN+1] = {0};
    uint16  name_len = BRW_MAX_URL_LEN;
    
    if (PNULL == img_list_ptr)
    {
        return PNULL;
    }
    item_info_ptr = BRWMem_Alloc(sizeof(BRW_IMAGE_LIST_ITEM_INFO_T));
    if(PNULL == item_info_ptr)
    {
        return PNULL;
    }

    item_info_ptr->data_ptr = img_list_ptr->image_data_ptr;
    item_info_ptr->data_len = img_list_ptr->image_data_len;
    item_info_ptr->mime_type = img_list_ptr->image_info.mime_type;
    Brw_DLGetDefaultFileName(name, name_len, img_list_ptr->image_info.url_ptr, TRUE, PNULL, item_info_ptr->mime_type);
    
    name_len = BRW_WStrlen(name);
    item_info_ptr->name_ptr = BRWMem_Alloc(sizeof(wchar) * (name_len+1));
    if(PNULL == item_info_ptr->name_ptr)
    {
        BRWMem_Free(item_info_ptr);
        return PNULL;
    }
    BRW_Memset(item_info_ptr->name_ptr,  0x00, sizeof(wchar) * (name_len+1));
    BRW_WstrCat(item_info_ptr->name_ptr, name);

    return item_info_ptr;
}

LOCAL void BrwInsertImageItem(BRW_IMAGE_INFO_LIST_T *img_list_ptr)
{
    BRW_IMAGE_LIST_ITEM_INFO_T *item_info_ptr = 0;

    item_info_ptr = BrwSetItemInfo(img_list_ptr);
    
    if(PNULL != item_info_ptr)
    {
        BRWList_AppendObject(&g_image_list_info,item_info_ptr);
    }
}

/*
 *  desc:
 *  input: 
 *  return:
 */
PUBLIC uint32 Brw_InitImageListInfo(BRW_INSTANCE instance)
{
    BRW_IMAGE_INFO_LIST_T img_list = {0};
    BRW_IMAGE_INFO_LIST_T *temp_img_list_ptr;
    uint32  total_num = 0;
    uint32  temp_total_num = 0;
    LAYOUT_HANDLE   layout_handle = GetCurDisplayLayout(instance);

    BRWList_Destruct(&g_image_list_info, BrwImageListItemDestruct);
    
    total_num = BrwGetImgInfoList(&img_list, layout_handle);
    
    temp_img_list_ptr = img_list.next;
    temp_total_num = total_num;
    while ((temp_total_num > 0) && (temp_img_list_ptr != PNULL))
    {
        BrwInsertImageItem(temp_img_list_ptr);
        temp_img_list_ptr = temp_img_list_ptr->next;
        temp_total_num--;
    }
    
    BrwFreeImgInfoList(&img_list, total_num);

    return BRWList_GetListCount(&g_image_list_info);
}

PUBLIC uint32 Brw_GetImageListCount(void)
{
    return BRWList_GetListCount(&g_image_list_info);
}

PUBLIC void Brw_FreeImageListInfo(void)
{
    BRWList_Destruct(&g_image_list_info, BrwImageListItemDestruct);
}
#endif

PUBLIC const BRW_IMAGE_LIST_ITEM_INFO_T* BRW_GetImageListItemInfo(uint32 index)
{
    return (const BRW_IMAGE_LIST_ITEM_INFO_T*)BRWList_GetObjectByPos(&g_image_list_info, index);
}


/*
 *	desc:check if layout_handle is belong to browser instance brw_info_ptr->cur_display_req_ptr 
 *  input: brw_info_ptr browser instance
 *         layout_handle
 *  return:TRUE if layout belong to brw_info_ptr->cur_display_req_ptr ,else false
 */
PUBLIC BOOLEAN BrwIsLayoutBelongToCurDisplay(void* layout_handle)
{
    if (PNULL == layout_handle)
    {
        return FALSE;
    }

    if (PNULL != BRW_SearchInstance((BRW_LIST_CMP_F)IsLayoutBelongToInstanceCurDisplay, layout_handle))
    {
        return TRUE;
    }
    return FALSE;
}

PUBLIC BOOLEAN Brw_ResetLinkSettings(BRW_INSTANCE instance)
{
    BRW_INFO_T		*brw_info_ptr=PNULL;
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    BRW_StopPageReq(instance);
    brw_info_ptr = BrwGetBrwInfoPtrByInstance(instance);
    if ((PNULL != brw_info_ptr) && (brw_info_ptr->http_context_id!=BrwGetInvalidHttpContextIDValue()))
    {
        BrwSetHttpState(brw_info_ptr, BRW_HTTP_STATE_CLOSING);
        BrwCloseHttpSession(brw_info_ptr->http_context_id, public_profile_ptr->protocol);
        brw_info_ptr->http_context_id = 0;
    }
  
    DeactiveNetwork(BRW_INVALID_INSTANCE);

    return TRUE;
}
