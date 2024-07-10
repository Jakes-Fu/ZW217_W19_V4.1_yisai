 /****************************************************************************
** File Name:      mmk_shell.h                                     *
** Author:         gang.tong                                               *
** Date:           06/05/2008                                              *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the shell                  *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/2008        gang.tong        Create
** 
****************************************************************************/

#ifndef  _MMK_SHELL_H_
#define  _MMK_SHELL_H_

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                      */
/*----------------------------------------------------------------------------*/
#include "cafshell.h"
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
/*要验证的CRC个数*/
#define DYNAMIC_FILE_CRC_NUM 128   
#define  XML_CRC_NUM_MAX_NUM_LEN 16
#define CRC_MASK 0xFFFFFFFFL
#define CRC32_POLYNOMIAL 0xEEB88320

#ifdef WIN32
#define MMK_DYN_MODULE_FILE_SIZE_MAX        (1024*1024)
#else
#define MMK_DYN_MODULE_FILE_SIZE_MAX        (1024*30)
#endif

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Free CRC table
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
PUBLIC void MMK_FreeCRCTable( uint32* table_ptr );
/*****************************************************************************/
// 	Description : 建立CRC表
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
PUBLIC uint32* MMK_CreateCRCTable( void );
/*****************************************************************************/
// 	Description : 计算一块内存的CRC值
//	Global resource dependence : 
//  Author:peng
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMK_CalculateBlockCRC32(
                                      uint32* table_ptr,
                                      uint32 count, 
                                      uint32 crc, 
                                      void *buffer
                                      );
/*****************************************************************************/
// 	Description :  create instance of interface from ishell interface.
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC CAF_RESULT_E  ISHELL_CreateInstance(CAF_GUID_T guid, void ** object_pptr);



// delete by songliang.liu
#if 0
/*****************************************************************************/
// 	Description : get Image data pointer and its size
//	Global resource dependence : none
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC CAF_RESULT_E ISHELL_LoadImage(
                                     MMI_IMAGE_ID_T image_id, 
                                     CAF_HANDLE_T owner_handle, 
                                     uint8 ** data_pptr, 
                                     uint32 * size_ptr
                                     );

/*****************************************************************************/
// 	Description : get string data pointer and its size
//	Global resource dependence : none
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC CAF_RESULT_E ISHELL_LoadText(
                                    MMI_TEXT_ID_T text_id, 
                                    CAF_HANDLE_T owner_handle, 
                                    MMI_STRING_T *str_ptr
                                    );

/*****************************************************************************/
// 	Description : get anim data pointer and its size
//	Global resource dependence : none
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC CAF_RESULT_E ISHELL_LoadAnim(
                                    MMI_ANIM_ID_T anim_id, 
                                    CAF_HANDLE_T owner_handle, 
                                    uint8 ** data_pptr, 
                                    uint32 * size_ptr
                                    );

/*****************************************************************************/
// 	Description : get anim data pointer and its size
//	Global resource dependence : none
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC CAF_RESULT_E ISHELL_LoadRing(
                                    MMI_RING_ID_T ring_id, 
                                    CAF_HANDLE_T  owner_handle, 
                                    void * pRingInfo
                                    );




#endif



//add by songliang.liu
PUBLIC CAF_RESULT_E ISHELL_LoadImage( const wchar *  imagefile_ptr, IImage ** image_pptr);
PUBLIC CAF_RESULT_E ISHELL_LoadRing(const wchar * ringfile_ptr, IMedia** media_pptr );
PUBLIC CAF_RESULT_E ISHELL_LoadAnim(const wchar * animfile_ptr, IImage ** image_pptr);
PUBLIC CAF_RESULT_E ISHELL_LoadResImage( uint32 image_id, CAF_HANDLE_T applet_handle, IImage ** image_pptr );
PUBLIC CAF_RESULT_E ISHELL_LoadResText( uint32 text_id, CAF_HANDLE_T applet_handle, CAF_STRING_T * str_ptr );
PUBLIC CAF_RESULT_E ISHELL_LoadResAnim( uint32 anim_id, CAF_HANDLE_T applet_handle, IImage ** image_pptr );
PUBLIC CAF_RESULT_E ISHELL_LoadResRing( uint32 ring_id, uint32 ring_type,CAF_HANDLE_T applet_handle, IMedia ** media_pptr );
PUBLIC CAF_RESULT_E ISHELL_LoadResData(uint32 res_id, CAF_RESOURCE_TYPE_E type ,CAF_HANDLE_T applet_handle,void ** buffer_pptr);
PUBLIC void ISHELL_GetScreenInfo( CAF_SCREENINFO_T* info_ptr ) ;
PUBLIC CAF_RESULT_E ISHELL_GetMemoryInfo( CAF_MEMORYINFO_T* info_ptr,CAF_MEMORY_TYPE_E memory_type);
PUBLIC uint16 ISHELL_ConvertUCS2GB( const uint8 *ucs_ptr, uint16 length, uint8 *gb_ptr);
PUBLIC uint16 ISHELL_ConvertGB2UCS(const uint8 *gb_ptr, uint16 length, uint8 *ucs_ptr);
PUBLIC uint16 ISHELL_ConvertUCS2B2UTF8( const uint8 *ucs2b_ptr, uint16 ucs2b_len, uint8 *utf8_ptr, uint16 utf8_buf_len);
PUBLIC uint16 ISHELL_ConvertUTF82UCS2B( const uint8 *utf8_ptr, uint16 utf8_len, uint8 *ucs2b_ptr, uint16 ucs2b_buf_len);
PUBLIC uint16 ISHELL_ConvertUCS2BE2UCS2LE( const uint8 *ucs2b_ptr, uint16 ucs2b_len, uint8 *ucs2l_ptr, uint16 ucs2l_buf_len );
PUBLIC uint16 ISHELL_ConvertUCS2LE2UCS2BE( const uint8 *ucs2l_ptr, uint16 ucs2l_len, uint8 *ucs2b_ptr, uint16 ucs2b_buf_len );
PUBLIC int ISHELL_DetectType (const void * buf_ptr, uint32 * size_ptr, const char * name_ptr, const char ** mime_pprt );

PUBLIC 	CAF_SEMAPHORE_PTR ISHELL_CreateSemaphore( //if successful, return semaphore
														 //pointer, else return SCI_NULL
		const char *name_ptr, //name of the semaphore
		uint32 initial_count //initial value of semaphore counter
	);

PUBLIC 	uint32 ISHELL_DeleteSemaphore ( //if successful return SCI_SUCCESS,
											   //else return SCI_ERROR
		CAF_SEMAPHORE_PTR sem_ptr //semaphore pointer
	);


PUBLIC 	uint32 ISHELL_GetSemaphore (//if successful return SCI_SUCCESS,
                                     //else return SCI_ERROR
    CAF_SEMAPHORE_PTR sem_ptr,//semaphore pointer
    uint32 wait_option       //wait option: SCI_WAIT_FOREVER, SCI_NO_WAIT,
                               //             1~0x0FFFFFFFE wait time(ms)
);


PUBLIC 	uint32 ISHELL_PutSemaphore ( //if successful return SCI_SUCCESS,
							  //else return SCI_ERROR
		CAF_SEMAPHORE_PTR sem_ptr //semaphore pointer
	);







#define SOUND_MIME_BASE "snd/" 
#define AUDIO_MIME_BASE "audio/" 
#define VIDEO_MIME_BASE "video/" 
#define MIDI_EXTENSION "mid"
#define MP3_EXTENSION "mp3" 
#define QCP_EXTENSION "qcp" 
#define MPEG4_EXTENSION "mp4" 
#define MMF_EXTENSION "mmf" 
#define PHR_EXTENSION "spf" 
#define ADPCM_EXTENSION "wav" 
#define AAC_EXTENSION "aac" 
#define HVS_EXTENSION "hvs" 
#define SAF_EXTENSION "saf" 
#define MT_MIDI "snd/midi" 
#define MT_MP3 "snd/mp3" 
#define MT_QCP "snd/qcp" 
#define MT_AUDIO_MIDI "audio/mid" 
#define MT_AUDIO_MP3 "audio/mp3" 
#define MT_AUDIO_QCP "audio/qcp" 
#define MT_AUDIO_VND_QCELP "audio/vnd.qcelp" 
#define MT_AUDIO_QCF "audio/qcf" 
#define MT_AUDIO_MMF "audio/mmf" 
#define MT_AUDIO_PHR "audio/spf" 
#define MT_AUDIO_IMELODY "audio/imy" 
#define MT_AUDIO_ADPCM "audio/wav" 
#define MT_AUDIO_AAC "audio/aac" 
#define MT_AUDIO_HVS "audio/hvs" 
#define MT_AUDIO_SAF "audio/saf" 
#define MT_VIDEO_PMD "video/pmd" 
#define MT_VIDEO_MPEG4 "video/mp4"



/*****************************************************************************/
// 	Description : create a window in parseing window table
//	Global resource dependence : none
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T ISHELL_CreateWinTable(
                                          const CAF_WINDOW_TABLE_CREATE_T* create_ptr
                                          );

/*****************************************************************************/
// 	Description : close a window 
//	Global resource dependence : none
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_CloseWindow(
                                  MMI_HANDLE_T win_handle
                                  );

/*****************************************************************************/
// 	Description : set window softkey id
//	Global resource dependence : none
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_SetWinSoftkeyId( 
                                      MMI_HANDLE_T  win_handle,
                                      MMI_TEXT_ID_T leftsoft_id,
                                      MMI_TEXT_ID_T middlesoft_id,
                                      MMI_TEXT_ID_T rightsoft_id,
                                      BOOLEAN       is_need_update
                                      );

/*****************************************************************************/
// 	Description : get dc
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC IDC_T* ISHELL_GetDC( 
                           MMI_HANDLE_T  handle
                           );

/*****************************************************************************/
// 	Description : get applet by win
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC IAPPLET_T* ISHELL_GetAppletByWin( 
                                        MMI_HANDLE_T win_handle
                                        );

/*****************************************************************************/
// 	Description : get ctrl by win
//	Global resource dependence : none
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC IGUICTRL_T* ISHELL_GetCtrlByWin(
                                       MMI_HANDLE_T win_handle,
                                       MMI_CTRL_ID_T ctrl_id
                                       );

/*****************************************************************************/
// 	Description : set the active control 
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_SetActiveCtrl(
                                    MMI_HANDLE_T  ctrl_handle // the control handle
                                    );

/*****************************************************************************/
// 	Description : start an applet by guid
//	Global resource dependence : none
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC CAF_RESULT_E ISHELL_StartApplet(
                                       const CAF_APPLET_START_T* start_ptr
                                       );

/*****************************************************************************/
// 	Description : close an applet 
//	Global resource dependence : none
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC CAF_RESULT_E ISHELL_CloseApplet(
                                       MMI_HANDLE_T applet_handle
                                       );

/*****************************************************************************/
// 	Description : post message
//	Global resource dependence : none
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_PostMessage(
                                  MMI_HANDLE_T        handle,         // the handle
                                  MMI_MESSAGE_ID_E    msg_id,         //the identify of message
                                  DPARAM              param_ptr,      //the param of the message
                                  uint32              size_of_param   //the size of the param
                                  );

/*****************************************************************************/
// 	Description : send message
//	Global resource dependence : none
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_SendMessage(
                                  MMI_HANDLE_T        handle,         // the handle
                                  MMI_MESSAGE_ID_E    msg_id,         //the identify of message
                                  DPARAM              param_ptr       //the param of the message                        
                                  );

/*****************************************************************************/
// 	Description : create a timer
//	Global resource dependence : none
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC uint8 ISHELL_CreateTimer(
                                MMI_HANDLE_T handle,
                                uint32 time_out,
                                BOOLEAN is_period
                                );

/*****************************************************************************/
// 	Description : start a timer
//	Global resource dependence : none
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_StartTimer(
                                 MMI_HANDLE_T handle,
                                 uint8 timer_id, 
                                 uint32 time_out, 
                                 BOOLEAN is_period
                                 );

/*****************************************************************************/
// 	Description : stop a timer
//	Global resource dependence : none
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_StopTimer(uint8 timer_id);
/*****************************************************************************/
// 	Description : pause a timer
//	Global resource dependence : none
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_PauseTimer(uint8 timer_id);
/*****************************************************************************/
// 	Description : resume a timer
//	Global resource dependence : none
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_ResumeTimer(uint8 timer_id);
/*****************************************************************************/
// 	Description : resume a timer
//	Global resource dependence : none
//  Author: gang.tong
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_IsTimerActive(uint8 timer_id);

/*****************************************************************************/
// 	Description : get device info
//	Global resource dependence : none
//  Author: james.zhang
//	Note: 
/*****************************************************************************/
//delete by songliang.liu
#if 0
PUBLIC void ISHELL_GetDeviceInfo( CAF_DEVICE_INFO* info_ptr );
#endif


//add by songliang.liu
PUBLIC void ISHELL_GetScreenInfo( CAF_SCREENINFO_T* info_ptr );
PUBLIC CAF_RESULT_E ISHELL_GetMemoryInfo( CAF_MEMORYINFO_T* info_ptr ,CAF_MEMORY_TYPE_E memory_type);



/*****************************************************************************/
// 	Description : message box
//	Global resource dependence : none
//  Author: james.zhang
//	Note: 
/*****************************************************************************/
PUBLIC void ISHELL_MessageBox( const CAF_MESSAGEBOX_CREATE_T* msgbox_ptr );

/*****************************************************************************/
// 	Description : allow turn off back light
//	Global resource dependence : none
//  Author: james.zhang
//	Note: 
/*****************************************************************************/
PUBLIC void ISHELL_AllowTurnOffBackLight( BOOLEAN allow_turn_off );

//added by andrew.zhang at 2009/05/26
/*****************************************************************************/
//  Description : set window title
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_SetWinTitle( CAF_HANDLE_T handle, CAF_STRING_T * str_ptr, BOOLEAN is_need_update );

/*****************************************************************************/
//  Description : set window title by id
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_SetWinTitleByID( CAF_HANDLE_T handle, uint32 text_id, BOOLEAN is_need_update );

/*****************************************************************************/
//  Description : get applet install directory
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_GetAppletInstallDirectory( CAF_HANDLE_T applet_handle, wchar *path_ptr, uint16 *wstr_len_ptr );

/*****************************************************************************/
//  Description : whether the window is focus
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_IsFocusWindow( CAF_HANDLE_T win_handle );

/*****************************************************************************/
//  Description : whether the window is focus
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ISHELL_GetWinRect( CAF_HANDLE_T handle, CAF_RECT_T*    rect_ptr );


/*****************************************************************************/
//  Description :  create instance of interface from ishell interface.
//  Global resource dependence : none
//  Author: Andrew.Zhang
//  Note:
/*****************************************************************************/
CAF_RESULT_E  ISHELL_CreateInstanceEx(CAF_GUID_T guid, void ** object_pptr, void * param_ptr);

/*****************************************************************************/
//  Description :  close instance
//  Global resource dependence : none
//  Author: Andrew.Zhang
//  Note:
/*****************************************************************************/
CAF_RESULT_E  ISHELL_CloseInstance(CAF_GUID_T guid, void * object_ptr);


/*****************************************************************************/
//  Description : send notify message to window
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 ISHELL_SendNotify( CAF_HANDLE_T ctrl_handle, CAF_MESSAGE_ID_E msg_id );

/*****************************************************************************/
//  Description : post notify message to window
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint32 ISHELL_PostNotify( CAF_HANDLE_T ctrl_handle, CAF_MESSAGE_ID_E msg_id );


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif // _MMK_MODULEINFO_H_
