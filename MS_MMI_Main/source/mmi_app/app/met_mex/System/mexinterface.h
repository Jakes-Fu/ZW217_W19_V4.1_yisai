/************************************************************************
* 版权所有 (C)2010,扬讯科技。
* 
* 文件名称： // MexInterface.h
* 文件标识：
* 内容摘要： // 提供给系统内部使用的函数
*
************************************************************************/
#if defined(MEX_TARGET_SERVER)

#ifndef __MEX_INTERFACE_H__
#define __MEX_INTERFACE_H__ 


#if !defined(__MEXLIB_PHS_SERVER_H__)
#error Please Include MexLib_Phs.h instead of MexInterface.h!!!
#endif

#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
// gdi for spread
typedef struct _Mex_GdiLayer
{
	uint16 width, height;
	int16 clipx1, clipx2, clipy1, clipy2;
	uint8 *buf_ptr;
} Mex_GdiLayer;


typedef int gdi_handle;

extern Mex_GdiLayer *gdi_act_layer;

#endif

typedef struct 
{
    int32 m_iTotalByteToUnc;	//总共需要解压缩的字节数
    int32 m_iTotalNbUpdate;		//总共需要更新的文件数
    int32 m_iTotalNbDelete;		//总共需要删除的文件数
    int32 m_iNbBytesUnc;		//已解压的字节数
    int32 m_iNbUpdated;			//已更新文件数
    int32 m_iNbDeleted;			//已删除文件数
} Mex_Install_Info;

typedef enum
{
    E_MIE_ERROR_None,
    E_MIE_ERROR_NoMemory,
    E_MIE_ERROR_InvalidPatch,
    E_MIE_ERROR_OpenFile,
} Mex_Install_Error;

typedef enum
{
	E_MEX_AUD_MEDIA_IDLE,
	E_MEX_AUD_MEDIA_PLAY,
	E_MEX_AUD_MEDIA_PLAY_PAUSED,
	E_MEX_AUD_MEDIA_RECORD,
	E_MEX_AUD_MEDIA_RECORD_PAUSED,
	E_MEX_AUD_MEDIA_SEEK_PAUSED,
	E_MEX_AUD_MEDIA_UNDERFLOW,
	E_MEX_AUD_MEDIA_WAIT_RECORD
} Mex_Audio_Media_State;



#if defined(MEX_G7_UI_BOOT)
typedef enum {
	E_MEX_UI_STYLE_DEFAULT = 0,
	E_MEX_UI_STYLE_FAUI,

	E_MEX_UI_STYLE_MAX
} E_MEX_UI_STYLE;
#endif

typedef enum _MexQQStatus_e_
{
	MEX_QQ_STATUS_NONE,
	MEX_QQ_STATUS_OFFLINE,
	MEX_QQ_STATUS_ONLINE,
	MEX_QQ_STATUS_MSG,
	MEX_QQ_STATUS_HIDE,
	MEX_QQ_STATUS_AWAY,
	MEX_QQ_STATUS_MAX,
}MexQQStatus_e;
typedef enum _MexQQID_
{
	MEX_QQ_MASTER,
	MEX_QQ_SLAVE,
}MexQQID;
enum _Mex_ncenter_app_id_
{
	MEX_NCENTER_QQ1 = 0,
	MEX_NCENTER_QQ2 = 1,
	MEX_NCENTER_MSN = 2,
	MEX_NCENTER_FETION = 3,
};

typedef int(*AQ_CallBack)(int);

//补丁进度回调函数声明
typedef void (*Mex_InstFunc_CB)(Mex_Install_Info *pInfo);
typedef void (*Mex_PatchFinish_CB)(bool bIsOk);
int32 mex_MP_ProcessPatchFile(const wchar_t *szPatchFile, Mex_InstFunc_CB funcCB, Mex_PatchFinish_CB finishCB);
bool mex_MP_IsPatchNeeded(const wchar_t *szPatchFile);
wchar_t *mex_MP_GetCurPatchAppName(int32 hPatch);
bool mex_MP_Cleanup(int32 hPatch);
void Mex_Entry_InstallAndRunPackage(const wchar_t *szPackageName);
void *MexInterface_Med_Alloc(uint32 iSize);
void MexInterface_Med_Free(void *ptr);


#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
#define Mex_CreateMutex(A) kal_create_mutex(A)
#elif defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
#define Mex_CreateMutex(A) SCI_CreateMutex(A,0)
#endif


typedef void* (*MexZlib_alloc_func)(unsigned int);
typedef void (*MexZlib_free_func)(void*);

typedef void (*PsFuncPtr)(void *parameter);
typedef void (*MexPsFuncPtr) (void *);

typedef void (*MexPsFuncPtrU16) (void *, uint16, uint16);

extern E_MEX_Sim_Card_ID Mex_GetPhoneUsableSim(void);
extern void Mex_SetPhoneADChannel(E_MEX_Sim_Card_ID sim,E_MEX_AD_CHANNEL channel_type);
extern void Mex_SetProtocolEventHandler(void *funcPtr,uint16 eventID);

#if (MEX_MTK_CODE_VERSION >= 0x1032)
extern void Mex_SetSlaveProtocolEventHandler(void *funcPtr,uint16 eventID);
extern void Mex_ClearProtocolEventHandler(uint16 eventID, void *funcPtr);
extern void Mex_ClearSlaveProtocolEventHandler(uint16 eventID, void *funcPtr);
#else
extern void Mex_ClearProtocolEventHandler(uint16 eventID);
extern void Mex_SetSlaveProtocolEventHandler(void *funcPtr,uint16 eventID);
extern void Mex_ClearSlaveProtocolEventHandler(uint16 eventID);
#endif

extern void MexInterface_EndCellInfo_Req(E_MEX_Sim_Card_ID sim, void *funcPtr);
extern char *Mex_Get_gprs_data_account_name(void *param);

//simulate incoming call
bool MexPhs_IncomingCall(E_MEX_Sim_Card_ID eSimId, const wchar_t* wszNumber);
//simulate new sms
bool MexPhs_NewSms(E_MEX_Sim_Card_ID eSimId, const char* pSmsCenter, const char* pSmsNumber, const wchar_t *pSmsContent, const Mex_DateTime *pDt);
//delete sms
bool MexPhs_DelSms(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, Mex_Phs_Del_Sms_CB callback);
// Outgoing call
MEX_OUTCALL_HANDLE MexPhs_OutCallStart(E_MEX_Sim_Card_ID eSimId, const char* number, MexOutCallCB callback);
bool MexPhs_OutCallFinish(MEX_OUTCALL_HANDLE handle);
bool MexPhs_OutCallDtmfBegin(MEX_OUTCALL_HANDLE handle, char keycode);
bool MexPhs_OutCallDtmfEnd(MEX_OUTCALL_HANDLE handle);
// Sms
bool MexPhs_SmsReady(void);
uint16 MexPhs_GetNumOfSms(E_MEX_PHS_SMS_MSGBOX msgBox);
bool MexPhs_ReadSmsContent(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, Mex_Phs_Read_Sms_CB callback);
char *MexPhs_GetSmsAddr(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index);
bool MexPhs_GetSmsDateTime(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, Mex_DateTime *pDt);
bool MexPhs_GetSmsCache(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index, wchar_t *pContent, uint16 iContentMemLen);
E_MEX_Sim_Card_ID MexPhs_GetSmsSimId(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index);
void MexPhs_CancelSmsCB(void);
bool MexPhs_SaveSms(E_MEX_PHS_SMS_MSGBOX msgBox, E_MEX_Sim_Card_ID simId, const wchar_t *pContent, const char *pAddr, Mex_Phs_Save_Sms_CB callback);
bool MexPhs_SmsMsgBoxExist(E_MEX_PHS_SMS_MSGBOX msgBox);
bool MexPhs_SmsIsUnread(E_MEX_PHS_SMS_MSGBOX msgBox, uint16 index);

void MexPhs_MotionSensor_Start(MexLib_MotionSensor_CallBack callback,int16 x_ponit,int16 y_point,int16 motionSensitivity);
void MexPhs_MotionSensor_Stop();

#if MEX_SDK_VER >= 1002
bool MexPhs_PhnSetWallPaper(E_MEX_WallPaper_Type eType, const wchar_t *pFileName);
#endif // MEX_SDK_VER

#if defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_MTK)
int MexInterface_mdi_audio_play_file_ext(void *file_name, int play_style, void *cache_p, void* handler, int volume, int path);
int MexInterface_mdi_audio_restore_file_ext(void *file_name, int play_style, void *cache_p, void* handler, int volume, int path);
void MexInterface_AsciiToUSC2(signed char* out_usc2, const signed char* in_ascii, int ascci_len);

#endif


#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD) 
void MexInterface_GetSprdMainLcdInfo(uint16 **ppBuffer, uint16 *puWidth, uint16 *puHeight);
void MexInterface_UpdateSprdMainLcd(void);

// gdi for spread
bool MexInterface_gdi_layer_is_multi_layer_enable(void);
int MexInterface_gdi_layer_multi_layer_enable(void);
int MexInterface_gdi_layer_multi_layer_disable(void);
int MexInterface_gdi_layer_get_base_handle(gdi_handle *handle_ptr);
int MexInterface_gdi_layer_get_buffer_ptr(void **buf_ptr);
int MexInterface_gdi_layer_create(int offset_x, int offset_y, int width, int height, gdi_handle *handle_ptr);
int MexInterface_gdi_layer_free(gdi_handle handle);
void MexInterface_gdi_layer_set_active(gdi_handle handle);
int MexInterface_gdi_layer_set_blt_layer(gdi_handle handle0, gdi_handle handle1, gdi_handle handle2, gdi_handle handle3);
void MexInterface_gui_BLT_double_buffer(int x1, int y1, int x2, int y2);
#define gdi_layer_set_source_key(A,B) (NULL)

#endif

int MexInterface_gdi_layer_create(int offset_x, int offset_y, int width, int height, int *handle_ptr);
int MexInterface_SetBltLayer(uint32 h1, uint32 h2, uint32 h3, uint32 h4);

extern E_Mex_LcdRotateType MexInterface_GetLcdRotateType(void);
extern bool MexInterface_SetLcdRotateType(E_Mex_LcdRotateType eRotateType );
extern bool MexInterface_CheckLcdRotate( uint8 rotate_type );
extern bool MexInterface_EnableLcdRotate(bool flag);

// Adm funcs
#if defined(MEX_PLATFORM_SIMULATOR) || defined(MEX_PLATFORM_SPREAD)
void* mex_adm_create(void* pMemAddr, uint32 iMemSize, uint32* subpool_size, bool islogging);
int32 mex_adm_delete(void* adm_id);
void* mex_adm_alloc(void* adm_id, uint32 size, char* file, uint32 line);
void mex_adm_free(void* adm_id, void *mem_addr);
uint32 mex_adm_get_total_left_size(void* adm_id);

#endif
void MexInterface_Conflict_Resume(void);
void MexInterface_OslMsgSendQueue(void *Message);
void MexInterface_Get_Master_Imsi_Req(void);
void MexInterface_Get_Slave_Imsi_Req(void);

#if (MEX_MTK_CODE_VERSION >= 1024)
uint8 MexInterface_Get_Master_Cell_Info_Ind(void* inMsg, int mod_src, void *data);
uint8 MexInterface_Get_Slave_Cell_Info_Ind(void* inMsg, int mod_src, void *data);

uint8 MexInterface_Get_Third_Cell_Info_Ind(void* inMsg, int mod_src, void *data);
uint8 MexInterface_Get_Fouth_Cell_Info_Ind(void* inMsg, int mod_src, void *data);
#else
void MexInterface_Get_Master_Cell_Info_Ind(void* inMsg);
void MexInterface_Get_Slave_Cell_Info_Ind(void* inMsg);

void MexInterface_Get_Third_Cell_Info_Ind(void* inMsg);
void MexInterface_Get_Fouth_Cell_Info_Ind(void* inMsg);

#endif


#if (MEX_MTK_CODE_VERSION >= 0x1112)
#define MTPNP_AD_Set_Channel(A, B); (1,2);
#define MTPNP_AD_Free_Channel(A); (1,2);
#endif

void* MexInterface_InitCharClusterInfo(void);
void MexInterface_ReleaseCharClusterInfo(void* pCharClusterInfo);
bool MexInterface_GetCharClusterInfo(void* pCharClusterInfo, void* szStart, void* szEnd, int* pAdvX);
void MexInterface_remove_dialer_screen_exit_handler(void);
bool MexConfig_bSupportVectorFont(void);

#endif // __MEX_INTERFACE_H__

#endif // MEX_TARGET_SERVER
