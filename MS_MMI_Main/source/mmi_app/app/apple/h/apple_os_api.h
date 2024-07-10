#ifndef APPLE_OS_API_H
#define APPLE_OS_API_H

#include "std_header.h"
#include "sci_types.h"
#include "window_parse.h"
#include "mmk_app.h"
//#include "guitextbox.h"
#include "guilcd.h"
#include "guilistbox.h"
//#include "guieditbox.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
// #include "mmicalc_main.h"
// #include "mmiwclk_main.h"
#include "tb_dal.h"
// #include "mmiset.h"
// #include "mmiacc_winmain.h"
#include "math.h"
#include "float.h"
#include <stdio.h>
#include <setjmp.h>
#include "mmipub.h"
#include "mmiacc_id.h"
#include "mmiacc_image.h"
#include "mmiacc_text.h"
#include "mmiacc_menutable.h"
#include "mmiacc_nv.h"
#include "mmi_nv.h"
#include "guibutton.h"
#include "mmk_timer.h"
#include "gui_ucs2b_converter.h"
#include "mmk_kbd.h"

#include "sci_types.h"
#include "lcd_cfg.h"

#include "mmi_filemgr.h"
#include "mmi_id.h"
#include "mmi_default.h"
// #include "mainapp.h"
#include "mmiudisk_internal.h"

#if !defined(SXH_SPT_H7810) && !defined(SXH_SPT_0839)
#include "mmicom_time.h"
#endif

#include "app_tcp_if.h"
#include "mn_api.h"
#include "mn_type.h"
//#include "mmibrowser.h"
#include "guifont.h"

#include "sig_code.h"
#include "iram_mgr.h"
#include "block_mem.h"

#include "sci_log.h"

#if defined(SXH_SPT_H7810)
#include "sc6600h_freq.h"
#else
#include "freq_cfg.h"
#endif
#ifndef WIN32
#include "chip.h"
#include "dal_lcd.h"

#endif

// #include "mmipb_internal.h"
// #include "mmipb_mnapi.h"
#include "mmiudisk_export.h"
#include "mmisms_app.h"
#include "mmisms_send.h"
#include "mp3_adp.h"
#ifndef WIN32
//#include "digitalcamera.h"

#ifdef PLATFORM_SC6600R
#include "dc_app_sc6600r.h"
extern DC_CONTEXT_T *s_pst_dc_context;
#elif defined(PLATFORM_SC6600L)
#include "dc_app_6600l.h"
#endif
#include "sensor_drv.h"
#include "Deep_sleep.h"
//#include "DC_misc.h"//liuhongmou
#include "dal_dcamera.h"
extern DCAMERA_RETURN_VALUE_E DC_Open(void) ;
#endif

#include "videocodec_task.h"
//#include "video_display.h"
#include "jpeg_interface.h"

#include "mmisms_read.h"
#include "apple_nv.h"
#include "apple_id.h"

#include <stdarg.h>

// #include "mmienvset.h"


//liuhongmou
#include "isp_service.h"
// #include "mmiconnection.h"
//#include "lcd.h"
#include "ref_lcd.h"

#include "mmiconnection_export.h"
#include "mmienvset_export.h"
#include "mmipdp_export.h"



#include "mn_type.h"
#include "mmisms_app.h"
#include "mmiset_export.h"
#include "Mmisrvaud_api.h"



#ifdef AAC_SUPPORT
#ifdef AAC_DECODE_IN_ARM
#include "aac_adp.h"
#else
#include "aac_lc_adp.h"
#endif
#endif

//#include "aac_lc_adp.h"
#include "Wav_adp.h"
#include "mp3_adp.h"
#include "dsp_codec_adp.h"
#include "dal_audio.h"	

#include "Mmisrvaud_api.h"
#include "aud_gen.h"

#include "mmisms_set.h"

#include "ucs_gb.h"

#include "mmu_drvapi.h"
#include "mmisms_delete.h"
#include "mmisms_receive.h"

#if defined(SXH_SPT_H7810) 
typedef enum
{
	FREQ_INDEX_APP_HIGH = 5
}PROD_FREQ_INDEX_E;
#endif

#if defined(SXH_SPT_H7810) || defined(SXH_SPT_0839)
#define MSG_USER_START 0x80000000
extern uint16* MMIAPICOM_StrToWstr(
                                  const char* src,
                                  uint16* dst
                                  );
extern char* MMIAPICOM_WstrToStr( 
                                 const uint16* src,
                                 char* dst
                                 );
extern uint16* MMIAPICOM_Wstrcpy( 
                                uint16* dst,
                                const uint16* src
                                );
extern uint16* MMIAPICOM_Wstrcat( 
                                uint16* dst,
                                const uint16* src
                                );
extern size_t MMIAPICOM_Wstrlen( const uint16* str );
extern int MMIAPICOM_Wstrcmp(
                             const uint16 * string1,
                             const uint16 * string2
                             );
extern uint16 GUI_WstrToUTF8(//utf8 len
                             uint8 *utf8_ptr,//out
                             uint16 utf8_buf_len,//in
                             const uint16 *wstr_ptr,//in
                             uint16 wstr_len//in
                             );
extern uint16 GUI_UTF8ToWstr(//ucs2b len
                             uint16 *wstr_ptr,//out
                             uint16 wstr_len,//in
                             const uint8 *utf8_ptr,//in
                             uint16 utf8_len//in
                             );
extern uint16 GUI_GetStringWidth(
                        GUI_FONT_T	font_type,     
                        uint8* str_ptr,
                        uint16 str_len,
                        BOOLEAN is_ucs2
                        );
extern uint16 GUI_GetStringHeight(
	 GUI_FONT_T	font_type,     
	 uint8* str_ptr,
	 uint16 str_len, 
	 BOOLEAN is_ucs2
	 );
extern void MMIAUDIO_SetCurDevMode(AUD_DEV_MODE_E mode);
#endif

extern void CHIP_ValidateArmClk(uint32 arm_clk_level);
extern uint32 MMI_SetFreq(PROD_FREQ_INDEX_E freq);
uint32 MMI_RestoreFreq(void);

extern void LCD_DisplayText(uint16 x, uint16 y, char *text, uint16 len);

extern ERR_LCD_E LCD_ConfigBlock(
								 LCD_ID_E lcd_id, 
								 uint32 blk_num, 
								 BLOCKCFG_T config
								);
extern ERR_LCD_E LCD_SetBlockBuffer(
									LCD_ID_E lcd_id, 
									uint32 blk_num, 
									uint32 *buf_ptr
                                   );

extern JINF_RET_E SWJPEG_Decode ( JINF_DEC_IN_PARAM_T *in_param_ptr, JINF_DEC_OUT_PARAM_T *out_param_ptr ); 
extern JINF_RET_E SWJPEG_Encode(JINF_ENC_IN_PARAM_T * in_param_ptr,  JINF_ENC_OUT_PARAM_T* out_param_ptr); 

#ifndef WIN32
extern uint32 ISP_ServiceSetReviewParam(ISP_REVIEW_PARAM_T *param_ptr);
extern void   ISP_ServiceStartReview(void);
extern uint32  ISP_ServiceSetScaleParam(ISP_SCALE_PARAM_T *param_ptr);
extern void  ISP_ServiceStartScale(void);
extern void Sensor_SelectSensorFormat(SENSOR_IMAGE_FORMAT image_format);
extern void MMIDEFAULT_SetAlwaysHalfOnBackLight(BOOLEAN b_half_on);
#endif



typedef struct _APPLE_OemApiTbl
{
	void* test;
}APPLE_OemApiTbl;

#endif
