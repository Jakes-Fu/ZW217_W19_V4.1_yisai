#ifndef _INC_BOOTDOWNLOAD_H
#define _INC_BOOTDOWNLOAD_H

/******************************************************************************/
//  Description:   check if in download mode
//  Parameter:
//      None
//  Return:
//      TRUE: in download mode
//		FALSE: not in download mode
/******************************************************************************/
BOOLEAN BOOT_CheckDownloadMode(void);
/******************************************************************************/
//  Description:   return image check flag
//  Parameter:
//      None
//  Return:
//      TRUE: image check is sucessful
//		FALSE: image check is false
/******************************************************************************/
BOOLEAN BOOT_GetImageCheckFlag(void);
/******************************************************************************/
//  Description:   begin to download
//  Parameter:
//      None
//  Return:
//      None
/******************************************************************************/
void BOOT_Download(void);
#endif
