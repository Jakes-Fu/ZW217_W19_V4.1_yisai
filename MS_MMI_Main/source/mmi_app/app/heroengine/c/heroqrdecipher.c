#ifdef HERO_QR_DECODE_SUPPORT
#include "window_parse.h"
#include "std_header.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guimsgbox.h"
#include "guicommon.h"
#include "guitext.h"
#include "guilistbox.h"
#include "guilabel.h"
#include "guimenu.h"
#include "guiedit.h"
#include "guilistbox.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "mmisrvaud_api.h"
#include "mmiset_export.h"
#include "mmipub.h"
#include "ffs.h"
#include "ffs_error.h"
#include "mn_type.h"
#include "Mn_api.h"
#include "mmisms_app.h"
#include "mmisms_export.h"
#include "mmiidle_export.h"
#include "mmidc_export.h"
#include "block_mem.h"
#include "sci_log.h"
#include "arm_reg.h"
#include "tb_hal.h"
#include "aud_gen.h"
#include "mmi_default.h"
#include "Mmiphone_export.h"
#include "Mmipb_export.h"
#include "mmimp3_export.h"
#include "freq_cfg.h"
#include "mmicc_internal.h"
#include "mmicc_text.h"
#include "sig_code.h"
#include "Mmi_filemgr.h"
#include "mmi_nv.h"
#include "mmi_id.h"
#include "mmisms_send.h"
#include "mmisms_edit.h"
#include "stdarg.h"
#include "Mmisd_export.h"
#include "mmi_common.h"
#include "Mmienvset_internal.h"
#include "Guiimg.h"
//#include "Guianim_internal.h"
#include "dal_lcd.h"
#include "mmiudisk_export.h"
#include "version.h"
#ifdef __HR_RECORD_SUPPORT__
#include "mmirecord_export.h"
#endif
#include "mmiset_id.h"
#ifndef WIN32
#include "iram_mgr.h"
#endif
#ifdef WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "mmifmm_export.h"
#include "ctrlanim.h"

#include "mmienvset_export.h" //MMIENVSET_INSIDE_MODE
#include "mmiidle_func.h"

#ifdef FM_SUPPORT
#include "mmifm_export.h"
#endif

#ifndef WIN32
typedef unsigned short		unshort;
typedef unsigned short		u_short;
#include "app_tcp_if.h"
#else
#include "sys\stat.h"
#include "stdio.h"
#endif
#include "mmiconnection_export.h"
#include "mmiset_id.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "wav_adp.h"
#include "apm_express.h"

#include "mmimms_edit.h"
#include "mmimms_internal.h"
#include "pe_process.h"
#include "mmidc_setting.h"
#ifdef __HR_CFG_MULTITP_SUPPORT__
#include "mmk_type.h"
#include "mmk_tp.h"
#endif
#ifdef __HR_CFG_SHORTCUT_SUPPORT__
#include "guianim_internal.h"
#endif
#include "mmiacc_event.h"
#include "socket_api.h"
#include "mmi_wallpaper_export.h"
#include "img_dec_interface.h"
#include "mmi_menutable.h"
#ifdef __HR_RECORD_SUPPORT__
#include "mmisrvrecord_export.h"
#include "mmirecord_appcontrol.h"
#endif
#include "mmismsapp_interface.h"
#include "mmicl_internal.h"
#include "mmicl_export.h"
#include "mmidc_main.h"

#include "mmialarm_service.h"
#include "mmischedule_export.h"
#include "mmicom_time.h"
#include "dal_time.h"
#include "in_message.h"

#include "os_api.h"
#include "dal_audio.h"
#include "adm_dp.h"
#include "tasks_id.h"
#include "sci_api.h"
#include "threadx_os.h"

#include "heroEngineInternal.h"
#include "heroEngineApi.h"
#include "heroAppApi.h"
#include "qrdecodei.h"

#define QR_DECIPHER_DECODE_FLAG      0x1

int hero_qr_decipher_close_context(char *input, int inputsize);

typedef struct tagHeroQrDecodeModuleSigSt {
	SIGNAL_VARS
	int datasize;
	char buf[512];
} HeroQrDecodeModuleSigSt;

typedef struct tagHeroQRDecodeTaskMsgSt{
	SIGNAL_VARS
}HeroQRDecodeTaskMsgSt;

static MMI_RESULT_E handle_hero_qrdecode_module_pp_msg(PWND app_ptr, uint16 msg_id, DPARAM param);

MMI_APPLICATION_T gHeroQrDecodeModlue;

PUBLIC void MMI_Hero_QrDecode_ModlueInit()
{
	gHeroQrDecodeModlue.ProcessMsg = handle_hero_qrdecode_module_pp_msg;
}

static MMI_RESULT_E handle_hero_qrdecode_module_pp_msg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E ret = MMI_RESULT_TRUE;
	if(msg_id >= HERO_QRDECODE_MOUDLE_SIGNAL_BEG && msg_id <= HERO_QRDECODE_MOUDLE_SIGNAL_END)
	{
		HeroQrDecodeModuleSigSt *pp_getdata = (HeroQrDecodeModuleSigSt*)(param);
		if(pp_getdata != NULL)
		{
			switch(msg_id)
			{
				case HERO_QRDECODE_MOUDLE_SIGNAL_EVENT:
					hr_event(HERO_QR_DECODE_EVENT, (int32)pp_getdata->buf, pp_getdata->datasize);
					break;
			}
		}
	}
	else
	{
		ret = MMI_RESULT_FALSE;
	}
	return ret;
}

static int hero_qr_decipher_send_msg_to_ui(int msgId, char *buf, int size)
{
	HeroQrDecodeModuleSigSt *psig = PNULL;

	MMI_CREATE_SIGNAL((xSignalHeaderRec*)psig, msgId, sizeof(HeroQrDecodeModuleSigSt), SCI_IdentifyThread());

	psig->datasize = size;
	memcpy(psig->buf, buf, MIN(size, sizeof(psig->buf)));

	// Send the signal to the dedicated task's queue
	MMI_SEND_SIGNAL((xSignalHeaderRec*)psig, P_APP);
	return 1;
}

void *qrdecode_malloc(int size)
{
	void *ptrData = SCI_ALLOC_APPZ(size);
	if(ptrData != NULL)
	{
		memset(ptrData, 0, size);
	}
	return ptrData; 
}

void qrdecode_free(void *ptr)
{
	if(ptr != NULL)
	{
		SCI_FREE(ptr);
	}
}

typedef struct tagHeroQrDecipherContextSt{
	unsigned char isInit;
	uint32 taskRef;
	SCI_MUTEX_PTR mutex;
	unsigned char isLock;
	void *decodeHandle;
}HeroQrDecipherContextSt;

static HeroQrDecipherContextSt *hero_get_qr_decipher_context(void)
{
	static unsigned char isInit = 0;
	static HeroQrDecipherContextSt sHeroQrDecipherContext = {0};
	if(!isInit){
		isInit = 1;
		memset(&sHeroQrDecipherContext, 0, sizeof(HeroQrDecipherContextSt));
	}
	return &sHeroQrDecipherContext;
}

static void hero_qr_deocde_thread_callback(uint32 param, void* obj_ptr)
{
	HeroQrDecipherContextSt *pContext = (HeroQrDecipherContextSt*)obj_ptr;
	if(pContext != NULL)
	{
		HeroQRDecodeTaskMsgSt signal = {0};
		HeroQRDecodeTaskMsgSt *sig_ptr = NULL;

		while(1)
		{
			sig_ptr = (HeroQRDecodeTaskMsgSt*)SCI_GetSignal(pContext->taskRef);
			signal = *sig_ptr;
			SCI_FREE(sig_ptr);

			MMIHR_Trace(("hero_qr_deocde_thread_callback actual_flags = %d", signal.SignalCode));
			if(signal.SignalCode == QR_DECIPHER_DECODE_FLAG)
			{
				char *payload = NULL; int payloadsize = 0;

			SCI_GetMutex(pContext->mutex, SCI_WAIT_FOREVER);
				pContext->isLock = 1;
			SCI_PutMutex(pContext->mutex);

				if(hero_qrdecode_decode(pContext->decodeHandle, &payload, &payloadsize))
				{
					MMIHR_Trace(("hero_qrdecode_decode payloadsize = %d, payload = %d", payloadsize, payload));
					if(payload != NULL && payloadsize > 0)
					{
						hero_qr_decipher_send_msg_to_ui(HERO_QRDECODE_MOUDLE_SIGNAL_EVENT, payload, payloadsize);
					}
				}

			SCI_GetMutex(pContext->mutex, SCI_WAIT_FOREVER);
				pContext->isLock = 0;
			SCI_PutMutex(pContext->mutex);
			}
		}
	}
}

typedef struct tagQrDecipherOpenReqSt{
	int imgW;
	int imgH;
}QrDecipherOpenReqSt;
int hero_qr_decipher_open_context(char *input, int inputsize)
{
	int ret = 0;
	do
	{
		QrDecipherOpenReqSt *reqData = (QrDecipherOpenReqSt*)input;
		HeroQrDecipherContextSt *pContext = hero_get_qr_decipher_context();

		MMIHR_Trace(("hero_qr_decipher_open_context input = %d, inputsize = %d", input, inputsize));
		if(input == NULL || inputsize != sizeof(QrDecipherOpenReqSt)){
			break;
		}

		if(pContext->isInit){
			break;
		}

		pContext->decodeHandle = hero_qrdecode_open(reqData->imgW, reqData->imgH);
		if(pContext->decodeHandle == NULL){
			break;
		}

		pContext->mutex = SCI_CreateMutex("qrdecodem", SCI_INHERIT);
		pContext->taskRef = SCI_CreateAppThread("qrdecode", "qrdecode",
						hero_qr_deocde_thread_callback,
						0,
						(void*)pContext,
						32 * 1024,
						5,
						TX_USER_NORMALE,
						SCI_PREEMPT,
						SCI_AUTO_START);
		if(pContext->taskRef == NULL)
		{
			break;
		}

		ret = 1;
		pContext->isInit = 1;
	}while(0);
	MMIHR_Trace(("hero_qr_decipher_open_context ret = %d", ret));
	if(!ret){
		hero_qr_decipher_close_context(NULL, 0);
	}
	return ret;
}

int hero_qr_decipher_close_context(char *input, int inputsize)
{
	HeroQrDecipherContextSt *pContext = hero_get_qr_decipher_context();
	if(pContext->taskRef != NULL) {
		SCI_SuspendThread(pContext->taskRef);
		while (1)
		{
			void *sig_ptr = (void*)SCI_PeekSignal(pContext->taskRef);
			if (sig_ptr != PNULL){
				SCI_FREE(sig_ptr);
			}else{
				break;
			}
		}
		SCI_TerminateThread(pContext->taskRef);
		SCI_DeleteThread(pContext->taskRef);
		pContext->taskRef = NULL;
	}
	if(pContext->mutex != 0){
		SCI_DeleteMutex(pContext->mutex);
		pContext->mutex = 0;
	}
	if(pContext->decodeHandle != NULL) {
		hero_qrdecode_close(pContext->decodeHandle);
		pContext->decodeHandle = NULL;
	}
	pContext->isLock = 0;
	pContext->isInit = 0;
	return 1;
}

typedef struct tagQrDecipherPushImgSt{
	char *pImg;
	int imgW;
	int imgH;
}QrDecipherPushImgSt;
int hero_qr_decipher_push_luma_img(char *input, int inputsize)
{
	int ret = 0;
	do
	{
		unsigned char isLock = 0;
		HeroQRDecodeTaskMsgSt *sig_ptr = NULL;
		QrDecipherPushImgSt *reqData = (QrDecipherPushImgSt*)input;
		HeroQrDecipherContextSt *pContext = hero_get_qr_decipher_context();

		MMIHR_Trace(("hero_qr_decipher_push_luma_img input = %d, inputsize = %d, pContext->isInit = %d, reqData->pImg = %d", input, inputsize, pContext->isInit, reqData->pImg));
		if(input == NULL || inputsize != sizeof(QrDecipherPushImgSt)){
			break;
		}

		if(!pContext->isInit || reqData->pImg == NULL){
			break;
		}

		if(SCI_QUEUE_FULL == SCI_IsThreadQueueAvilable(pContext->taskRef)) {
			return 0;
		}

	SCI_GetMutex(pContext->mutex, SCI_WAIT_FOREVER);
		isLock = pContext->isLock;
	SCI_PutMutex(pContext->mutex);

		MMIHR_Trace(("hero_qr_decipher_push_luma_img isLock = %d", isLock));
		if(isLock) {
			break;
		}

		if(!hero_qrdecode_push_luma_img(pContext->decodeHandle, reqData->pImg, reqData->imgW, reqData->imgH)) {
			break;
		}

		sig_ptr = (HeroQRDecodeTaskMsgSt *)SCI_ALLOC_APP(sizeof(HeroQRDecodeTaskMsgSt));
		SCI_MEMSET(sig_ptr, 0, sizeof(HeroQRDecodeTaskMsgSt));

		sig_ptr->SignalCode = QR_DECIPHER_DECODE_FLAG;
		sig_ptr->SignalSize = sizeof(HeroQRDecodeTaskMsgSt);
		sig_ptr->Sender = SCI_IdentifyThread();

		if (SCI_SUCCESS != SCI_SendSignal((xSignalHeader)sig_ptr, pContext->taskRef)) {
			SCI_FREE(sig_ptr);
			break;
		}

		ret = 1;
	}while(0);
	MMIHR_Trace(("hero_qr_decipher_push_luma_img ret = %d", ret));
	return ret;
}

#endif

