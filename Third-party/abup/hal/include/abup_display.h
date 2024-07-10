#ifndef _ABUP_DISPLAY_H_
#define _ABUP_DISPLAY_H_

typedef enum  {   
	FOTA_STATE_CV,    
	FOTA_STATE_DL,    
	FOTA_STATE_RD,    
	FOTA_STATE_RU,
	FOTA_STATE_RG,
	FOTA_STATE_ERROR,
	FOTA_STATE_LAST_VER
}FotaDisplayState;

abup_extern void abup_display_notify(abup_int32 message);

abup_extern void abup_display_progress(abup_int32 message,abup_uint16 percentage);


abup_extern void abup_stop_progress(void);



#endif

