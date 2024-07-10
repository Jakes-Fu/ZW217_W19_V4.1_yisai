#ifdef HERO_CAMERA_SUPPORT
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
#include "heroEngineInternal.h"
#include "heroEngineId.h"
#include "heroEngineExport.h"
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

#include "os_api.h"
#include "dal_audio.h"
#include "adm_dp.h"
#include "tasks_id.h"
#include "sci_api.h"
#include "threadx_os.h"

#include "dsp_codec_adp.h"
#include "dal_audio.h"
#include "aud_gen.h"
#include "audio_api.h"
#include "asm.h"
#include "vb_drv.h"
#include "audio_config.h"
#include "adpcmrecord_adp.h"

#include "heroEngineApi.h"
#include "mmi_common.h"

#ifndef HERO_VIDEO_CHAT_SUPPORT
#define HERO_VIDEO_CHAT_SUPPORT
#endif
#include "os_api.h"
#include "jpeg_interface.h"
#include "dal_jpeg.h"
#include "dal_dcamera.h"
#include "isp_service_uix8910.h"

#include "heroEngineApi.h"
#include "heroAppApi.h"

#define HERO_CAMERA_OUT_YUV_W  (320)
#define HERO_CAMERA_OUT_YUV_H  (240)

int hero_camera_close(char *input, int inputsize);

static void *hero_camera_malloc(int size)
{
	void *ptrData = SCI_ALLOC_APPZ(size);
	if(ptrData != NULL)
	{
		memset(ptrData, 0, size);
	}
	return ptrData; 
}

static void hero_camera_free(void *ptr)
{
	if(ptr != NULL)
	{
		SCI_FREE(ptr);
	}
}

typedef struct tagHeroCameraContextSt{
	unsigned char isInit;
	unsigned char isLock;
	unsigned char isTake;

	unsigned short outW;
	unsigned short outH;
	unsigned char *outbuf;
}HeroCameraContextSt;

static HeroCameraContextSt *hero_get_camera_context()
{
	static unsigned char gIsInit = 0;
	static HeroCameraContextSt gHeroCameraContext = {0};
	if(!gIsInit){
		gIsInit = 1;
		memset(&gHeroCameraContext, 0, sizeof(HeroCameraContextSt));
	}
	return &gHeroCameraContext;
}

typedef struct tagHeroCameraCommonOutSt{
	unsigned int param1;
	unsigned int param2;
	unsigned int param3;
	unsigned int param4;
}HeroCameraCommonOutSt;

static HeroCameraCommonOutSt gCameraCommonOutData = {0};

typedef struct tagHeroCameraOpenSt {
	unsigned short reqW;
	unsigned short reqH;
}HeroCameraOpenSt;

static void heroCameraUYVY422ToYuv420p(HeroCameraContextSt *pContext, unsigned char *pYuv422)
{
	int x = 0, y = 0; int isCpUV = 0;
	int yuvWdith = pContext->outW;
	int yuvHeight = pContext->outH;
	unsigned char *pTempYuv422 = NULL;
	unsigned char *y420p = NULL; unsigned char *u420p = NULL; unsigned char *v420p = NULL;

	if(pYuv422 == NULL || pContext->outbuf == NULL) {
		return;
	}
	pTempYuv422 = pYuv422;

	y420p = pContext->outbuf;
	u420p = y420p + (pContext->outW * pContext->outH);
	v420p = u420p + ((pContext->outW * pContext->outH)>>2);

	for(y = 0; y < yuvHeight; ++y)
	{
		isCpUV = (y%2 == 0);

		for(x = 0; x < yuvWdith; x+=2)
		{
			*y420p++ = pTempYuv422[1];
			*y420p++ = pTempYuv422[3];

			if(isCpUV)
			{
				*u420p++ = pTempYuv422[0];
				*v420p++ = pTempYuv422[2];
			}
			pTempYuv422 += 4;
		}
	}
}

static int heroCameraDCAMERAEncodeCallback(void *frame_ptr)
{
    ISP_FRAME_T *pframe = (ISP_FRAME_T *)frame_ptr;
	do
	{
		char *yAddr = NULL; char *uAddr = NULL; char *vAddr = NULL;
		HeroCameraContextSt *pContext = hero_get_camera_context();

		if(pframe == NULL || !pContext->isInit) {
			break;
		}

		if(HERO_CAMERA_OUT_YUV_W != pframe->width || HERO_CAMERA_OUT_YUV_H != pframe->height) {
			break;
		}

		yAddr = (char*)pframe->yaddr;
		if(yAddr == NULL) {
			break;
		}

		if(pContext->isInit && pContext->outbuf != NULL && !pContext->isTake) {
			pContext->isLock = 1;
			heroCameraUYVY422ToYuv420p(pContext, (unsigned char*)yAddr);
			pContext->isLock = 0;
		}
	}while(0);

    return ISP_CB_PROC_DONE;
}

static int heroCameraStartCameraPreview(int width, int height)
{
	int ret = 0;
	do
	{
		int iRet = 0;
		DCAMERA_PREVIEW_PARAM_T param = {0};

		memset(&param, 0, sizeof(param));

	#if 1
		param.capture_mode	  = 0; 
		param.dc_atv_callback = NULL;
		param.disp_mode 	  = 0x01;
		param.lcd_block_id	  = 0;
		param.lcd_id		  = 0;	
		param.rotation_mode   = 0;
		param.panorama_dir	  = 0;

		param.target_rect.x   = 0;
		param.target_rect.y   = 0;
		param.target_rect.w   = width;
		param.target_rect.h   = height;

		param.disp_rect.x	  = 0;
		param.disp_rect.y	  = 0;
		param.disp_rect.w	  = width;
		param.disp_rect.h	  = height; 
	#else
	    param.capture_mode    = 0; 
	    param.dc_atv_callback = NULL;
	    param.disp_mode       = 0x01;
	    param.lcd_block_id    = 0;
	    param.lcd_id          = 0;  
	    param.rotation_mode   = 0;
	    param.panorama_dir    = 0;

	    param.target_rect.x   = 0;
	    param.target_rect.y   = 0;
	    param.target_rect.w   = 240;
	    param.target_rect.h   = 320;
	    
	    param.disp_rect.x     = 0;
	    param.disp_rect.y     = 0;
	    param.disp_rect.w     = 240;
	    param.disp_rect.h     = 320;  
	#endif

		iRet = DCAMERA_Open(DCAMERA_OP_VIDEOCALL);
		MMIHR_Trace(("heroVideoChatStartCameraPreview iRet = %d, W = %d, h = %d", iRet, width, height));
		if(DCAMERA_OP_SUCCESS != iRet)
		{
			break;
		}
		DCAMERA_RigisterVideoChatCallback((int(*)())heroCameraDCAMERAEncodeCallback);

		//DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_IMG_SENSOR_MODE, SENSOR_MAIN, PNULL);

		//需要设置默认参数，否则会画面暗
		DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_BRIGHTNESS, DCAMERA_EV_05,PNULL);

		DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_CONTRAST, DCAMERA_EV_03,PNULL);

		DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_EFFECT, DCAMERA_EFFECT_NORMAL,PNULL);

		DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_EXPOSURE_COMPENSATION, DCAMERA_EV_03,PNULL); 

		DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_FLICKER_COMPENSATION, DCAMERA_EV_00,PNULL);

		DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_PREVIEW_MODE, DCAMERA_ENVIRONMENT_NORMAL,PNULL);

		DCAMERA_ParamCtrl(DCAMERA_PARAM_CTRL_WB_MODE, DCAMERA_WB_MODE_AUTO,PNULL);

		iRet = DCAMERA_StartPreview(&param);
		MMIHR_Trace(("DCAMERA_StartPreview ret = %d", iRet));
		if(iRet != DCAMERA_OP_SUCCESS)
		{
			break;
		}
		ret = 1;
	}while(0);
	MMIHR_Trace(("heroVideoChatStartCameraPreview ret = %d", ret));

	return ret;
}

int hero_camera_open(char *input, int inputsize, char **output, int *outputsize)
{
	int ret = 0;
	do
	{
		HeroCameraOpenSt *reqData = (HeroCameraOpenSt*)input;
		HeroCameraContextSt *pContext = hero_get_camera_context();

		MMIHR_Trace(("hero_camera_open init=%d,inputsize=%d,reqData=0x%x,output=0x%x,outputsize=0x%x,%d", pContext->isInit, inputsize, reqData, output, outputsize, sizeof(HeroCameraOpenSt)));
		if(pContext->isInit || reqData == NULL || sizeof(HeroCameraOpenSt) != inputsize || output == NULL || outputsize == NULL){
			break;
		}

		if(pContext->outbuf == NULL) {
			pContext->outbuf = (unsigned char*)hero_camera_malloc(((HERO_CAMERA_OUT_YUV_W * HERO_CAMERA_OUT_YUV_H) * 3)>>1);
		}
		MMIHR_Trace(("hero_camera_open pContext->outbuf = 0x%x", pContext->outbuf));
		if(pContext->outbuf == NULL) {
			break;
		}

		pContext->isInit = 1;
		if(!heroCameraStartCameraPreview(176, 144)){
			pContext->isInit = 0;
			MMIHR_Trace(("mci_videocall_camera_openpreview failed"));
			break;
		}

		pContext->outW = HERO_CAMERA_OUT_YUV_W;
		pContext->outH = HERO_CAMERA_OUT_YUV_H;

		gCameraCommonOutData.param1 = HERO_CAMERA_OUT_YUV_W;
		gCameraCommonOutData.param2 = HERO_CAMERA_OUT_YUV_H;

		*output = (char*)&gCameraCommonOutData;
		*outputsize = sizeof(HeroCameraCommonOutSt);

		ret = 1;
		pContext->isLock = 0;
		pContext->isTake = 0;
	}while(0);
	MMIHR_Trace(("hero_camera_open ret = %d", ret));
	if(!ret){
		hero_camera_close(NULL, 0);
	}
	return ret;
}

int hero_camera_close(char *input, int inputsize)
{
	HeroCameraContextSt *pContext = hero_get_camera_context();
	if(pContext->isInit) {
		DCAMERA_StopPreview();

		DCAMERA_Close();

		DCAMERA_UnRigisterVideoChatCallback();
	}

	if(pContext->outbuf != NULL){
		hero_camera_free(pContext->outbuf);
		pContext->outbuf = NULL;
	}

	pContext->isLock = 0;
	pContext->isTake = 0;
	pContext->isInit = 0;
}

enum{
	HERO_CAMERA_OUT_YUV420p,
	HERO_CAMERA_OUT_YUV420sp,
	HERO_CAMERA_OUT_RGB565,
};
int hero_camera_capture(char **output, int *outputsize)
{
	HeroCameraContextSt *pContext = hero_get_camera_context();
	if(pContext->isInit){
		pContext->isTake = 1;

		if(pContext->isLock){
			return 0;
		}
		gCameraCommonOutData.param2 = pContext->outW;
		gCameraCommonOutData.param3 = pContext->outH;
		gCameraCommonOutData.param4 = HERO_CAMERA_OUT_YUV420p;
		gCameraCommonOutData.param1 = (unsigned int)pContext->outbuf;

		*output = (char*)&gCameraCommonOutData;
		*outputsize = sizeof(HeroCameraCommonOutSt);
		return 1;
	}
	return 0;
}

int hero_camera_capture_release(char *input, int inputsize)
{
	HeroCameraContextSt *pContext = hero_get_camera_context();
	if(pContext->isInit){
		pContext->isTake = 0;
		return 1;
	}
	return 0;
}

int hero_camera_get_rotation_mode(char *input, int inputsize)
{
	return 90;
}

#endif

