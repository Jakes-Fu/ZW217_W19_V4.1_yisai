#ifndef VOICE_PROCESS_H
#define VOICE_PROCESS_H
#include "sci_types.h"
#include "audio_api.h"
#include "asm.h"
#include "nv_dev_adp.h"
#include "adm.h"

#include "vsi_amr_nb.h"
#include "vsi_amr_wb.h"
#include "hal_volte.h"
#include "hal_speech.h"
/**---------------------------------------------------------------------------*
 **                           Macros & Enums                                  *
 **---------------------------------------------------------------------------*/
#define VOLTE_NB_SAMPLE_RATE (8000)
#define VOLTE_WB_SAMPLE_RATE (16000)

#define AUD_VOCODE_MODE_NB_VOLTE 0x2
#define AUD_VOCODE_MODE_WB_BE 0x4
//#define MEASURE_PERFORMANCE
#define LVVE_DEBUG
#ifdef LVVE_DEBUG
	#define LVVE_TRACE SCI_TRACE_LOW
#else
	#define LVVE_TRACE
#endif /* LVVE_DEBUG */

typedef int (*VOICE_WRITE_NETDATA_FUNC) (const char *write_buff, uint32 buf_size);
typedef int (*VOICE_READ_NETDATA_FUNC) (char *read_buff, uint32 buf_size);
// definition of downlink callback function to config
typedef int (*VOICE_DOWNLINK_PROCESS_CONFIG) (int16 *in, int16 *noise,
		int16 *out);
// definition of downlink callback function to execution
typedef int (*VOICE_DOWNLINK_PROCESS_EXP) (void);

/**
 * \brief function type, override keytone detect push voice data
 */
typedef void (*VOICE_ktdetectHook_t)(int16 *data, uint16 SampleLen);

// input parameter of extend operation of downlink data process
typedef struct {
	VOICE_DOWNLINK_PROCESS_CONFIG downlink_pro_config;
	VOICE_DOWNLINK_PROCESS_EXP  downlink_pro_exe;
} VOICE_DOWNLINK_PROCESS_T;

/**---------------------------------------------------------------------------**
 **   uplink data process like nxp :                                          **
 **---------------------------------------------------------------------------**/
// definition of uplink callback function to config
typedef int (*VOICE_UPLINK_PROCESS_CONFIG) (int16 *in1, int16 *in2, int16 *out);
// definition of uplink callback function to execution
typedef int (*VOICE_UPLINK_PROCESS_EXP) (void);

// input parameter of extend operation of uplink data process
typedef struct {
	VOICE_UPLINK_PROCESS_CONFIG uplink_pro_config;
	VOICE_UPLINK_PROCESS_EXP  uplink_pro_exe;
} VOICE_UPLINK_PROCESS_T;

//2014.07.28--shitao.li--add
//enum AUDIO_VOLUME_TYPE
typedef enum {
	AUDIO_VOLUME_MAX = 100,
	AUDIO_VOLUME_CUR = 111
} AUDIO_VOLUME;
//enum BAND_MODE_E
typedef enum {
	NB_MODE,
	WB_MODE,
	SWB_MODE,
	UNKOWN_MODE,
	MAX_MODE,
	CUR_MODE
} BAND_MODE_E;

typedef enum {
    VOICE_MODE_NB = 0,
    VOICE_MODE_WB,
    VOICE_MODE_LOOPBACK_NB,
    VOICE_MODE_LOOPBACK_WB,
} VOICE_MODE;

typedef enum {
    VOICE_PCM = 0,
    VOICE_AMR,
} VOICE_CODEC_TYPE;

typedef struct
{
	BOOLEAN bIsLoopback;
}VOICE_IF_LOOPBACK_T;

#pragma anon_unions
typedef struct {
    union {
        /* Speech Parameters */
        HAL_SPEECH_ENC_OUT_T txAMRBuffer;
        HAL_VOLTE_ENC_OUT_T txAMRVolte;
    };
    union {
        /* Speech Parameters */
        HAL_SPEECH_DEC_IN_T rxAMRBuffer;
        HAL_VOLTE_DEC_IN_T rxAMRVolte;
    };
} VOICE_SHAREMEM_T;

typedef struct {
    VOICE_MODE mode;
    BOOLEAN init;
    uint16 dtx;
    void *dec;
    void *enc;
    void *dec_origin;
    void *enc_origin;
} AMR_CFG_T;

//--end
int32 voice_process_init();
int32 voice_process_start();
int32 voice_process_stop();
//int32 voice_process_deinit();
int32 voice_process_set_param (BAND_MODE_E band_mode,
	AUDIO_MODE_APP_TYPE_E app_type,
	AUDIO_DEVICE_MODE_TYPE_E mode, AUDIO_VOLUME uiVol);
PUBLIC HAUDIOCODEC VoiceProcess_RegCodecPlugger (//return codec handle
	HCOLLISION hCollision,               //collision handle
	AUDIO_CODEC_EXT_CFGINFO_T *ptCodecExtCfgInfo
);

PUBLIC uint16 VoiceProcess_GetWbMode (void);
PUBLIC void VoiceProcess_WriteNetdataFunc_Register(VOICE_WRITE_NETDATA_FUNC write_func );
PUBLIC void VoiceProcess_ReadNetdataFunc_Register(VOICE_READ_NETDATA_FUNC read_func );
PUBLIC void VoiceProcess_WriteNetdataFunc_Unregister(void);
PUBLIC void VoiceProcess_ReadNetdataFunc_Unregister(void);
PUBLIC void VoiceProcess_UplinkData_notify(uint32 datacount);
PUBLIC void VoiceProcess_DownlinkData_notify();
PUBLIC void VoiceProcess_device_Register(AUDIO_DEVICE_HANDLE_T* device);
PUBLIC void VoiceProcess_device_Unregister(void);
PUBLIC uint32 VoiceProcess_get_samplerate(uint32 uiSampleRate);

#endif
#ifdef CONFIG_IRAM
	PUBLIC uint32 *VoiceProcess_refdumpinfo (void);
#endif
