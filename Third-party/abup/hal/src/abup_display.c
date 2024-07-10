#include "abup_typedef.h"
#include "abup_debug.h"
#include "abup_define.h"
#include "abup_display.h"


void abup_display_fota_status(abup_int32 status);
extern void ShowUpdateInfo(FotaDisplayState state);


void abup_display_notify(abup_int32 message)
{   
    abup_int32 status = 0;
    
    if(E_ABUP_LAST_VERSION == message)
        status = FOTA_STATE_LAST_VER;
    else 
        status = FOTA_STATE_ERROR;
        
    abup_display_fota_status(status);
}


void abup_Entry_Progress_Scr(abup_uint16 strid)
{

}

void abup_display_progress(abup_int32 message,abup_uint16 percentage)
{
#ifndef ABUP_LCD_NONE
	abup_uint16 strid;
	
	if(message==E_ABUP_NET_CONNECTING)
		strid=STR_GLOBAL_CONNECTING;
	else if(message==E_ABUP_DOWNLOAD_CONNECTING)
		strid=STR_GLOBAL_DOWNLOADING;	

	abup_Entry_Progress_Scr(strid);
#endif
}

void abup_stop_progress(void)
{

}

void abupHighlightCV(void)
{
}

abup_extern abup_int32 abup_file_size(void);
abup_extern abup_int32 abup_recvd_size(void);
abup_extern void abup_stop_downlaod(void);
abup_extern ABUP_BOOL  abup_fota_running(void);
abup_extern abup_int32 abup_get_fota_result(void);
abup_extern void abup_get_new_version(void);
abup_extern void abup_reset_stop_downlaod(void);

abup_int32 abup_get_download_percent(void)
{
	abup_int32 DownloadPercent = 0;
	abup_int32 FileSize = 0,RecvdSize = 0;
	FileSize = abup_file_size();
	RecvdSize = abup_recvd_size();
	if(FileSize == 0)
		return 0;
	DownloadPercent = RecvdSize*100/FileSize;
	abup_DebugPrint("abup_get_download_percent = %d" , DownloadPercent);
	return DownloadPercent;
}
ABUP_BOOL abup_auto_patch(void)
{
	return ABUP_TRUE;
}

void abup_stop_download_pre(void)
{
	abup_stop_downlaod();
}

ABUP_BOOL  abup_fota_running_pre(void)
{
	return abup_fota_running();
}

abup_int32 abup_get_fota_result_pre(void)
{
	// 1success; 99 failed
	return abup_get_fota_result();
}

void abup_get_new_version_pre(void)
{
	abup_reset_stop_downlaod();
	abup_get_new_version();
}

void abup_display_fota_status(abup_int32 status)
{
    ShowUpdateInfo(status);
}

