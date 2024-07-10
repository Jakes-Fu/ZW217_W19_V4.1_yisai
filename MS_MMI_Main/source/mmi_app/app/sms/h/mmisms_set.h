/*************************************************************************
 ** File Name:      mmisms_set.h                                         *
 ** Author:         liming.deng                                          *
 ** Date:           2007/9/27                                            *
 ** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about sms setting     *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2007/9/27      liming.deng      Create.                              *
*************************************************************************/

#ifndef _MMISMS_SET_H_
#define _MMISMS_SET_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmisms_app.h"


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : to set the reference number to NV
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void  MMISMS_SetRefNumToNV(
                                  MMINV_SMS_REFERENCE_NUM_T   ref_num
                                 );

/*****************************************************************************/
//     Description : to get the vaild period from NV
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
MMINV_SMS_VALID_PERIOD_T MMISMS_GetValidPeriod( void );

/*****************************************************************************/
//     Description : to set the vaild period from NV
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
void MMISMS_SetValidPeriod(
                           MMINV_SMS_VALID_PERIOD_T   vaild_period
                           );

/*****************************************************************************/
//     Description : to delete a pointed frequent word in NV
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_DeletePointedWord(                    //RETURN:
                                 uint8    item_id        //IN: the item index started form 0
                                 );

/*****************************************************************************/
//     Description : to modify a pointed frequent word in NV
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
BOOLEAN MMISMS_ModifyPointedWord(
                                 uint8                    item_id,        //IN: the item index started form 0
                                 MMI_NV_SMS_PHRASE_T    *item_phase_ptr //IN: the word of pointed item
                                 );

/*****************************************************************************/
//     Description : to get the all frequent word in NV
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
void  MMISMS_GetFrequentWord( 
                             MMINV_SMS_FREQUENT_WORD_T    *word_ptr //[out]:the frequent word
                             );

/*****************************************************************************/
//     Description : init preset SMS
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
void MMISMS_InitPresetSms(void);

/*****************************************************************************/
//     Description : to set the sc addr called by window
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN    MMISMS_AppGetSCAddr(
                                    MN_DUAL_SYS_E   dual_sys,
                                    uint8    *sc_addr,        //OUT:
                                    uint8    *sc_addr_len,    //OUT:
                                    uint8    max_addr_len    //IN:
                                    );
#if 0
/*****************************************************************************/
//     Description : to set the sc addr called by window
//    Global resource dependence : none
//  Author:louis.wei
//    Note:设置信息服务中心号码
/*****************************************************************************/
PUBLIC BOOLEAN    MMISMS_AppSetSCAddr(                    //RETURN:
                                    MN_DUAL_SYS_E   dual_sys,
                                    uint8  *sc_addr,    //IN:
                                    uint8  addr_len        //IN:
                                    );
#endif

/*****************************************************************************/
//     Description : set default preset SMS
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetDefaultPresetSms(BOOLEAN is_clear);

/*****************************************************************************/
//     Description : judge whether the character is R8
//    Global resource dependence : 
//  Author: fengming.huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsR8Single( wchar value );

/*****************************************************************************/
//  Description : decode the sms user data head
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_DecodeUserDataHead (                             // return void
                MN_SMS_USER_DATA_HEAD_T     *user_data_header_ptr_t, // in---the user data head structure before decode
                MN_SMS_USER_HEAD_T          *user_head_ptr_t);        // out---the user data head structure after decode

/*****************************************************************************/
//  Description : free the sms user data head space
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_FreeUserDataHeadSpace(                             // return void
                MN_SMS_USER_HEAD_T          *user_head_ptr_t);        // in---the user data head structure to be free

/*****************************************************************************/
//  Description : judge the language of R8
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC MMISMS_R8_LANGUAGE_E MMISMS_JudgeR8Language(
                MN_SMS_USER_HEAD_T    *user_head_ptr_t);


/*****************************************************************************/
//  Description : judge the language of R8
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsR8Language(
                MMISMS_R8_LANGUAGE_E    language);

/*****************************************************************************/
//  Description : Change from 03.38 alphabet table to International alphabet table
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 MMISMS_Default2NationalR8(                          //RETURN:
                                      uint8     *default_ptr,   //IN:
                                      uint16      *national_ptr,        //OUT:
                                      uint16     str_len,            //IN:
                                      MMISMS_R8_LANGUAGE_E language,   //IN
                                      BOOLEAN is_lock_shift,
                                      BOOLEAN is_single_shift
                                      );

/*****************************************************************************/
//  Description : Decode R8 Header
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC void MMISMS_DecodeR8Header(                          //RETURN:
                                      MN_SMS_USER_HEAD_T    *user_head_ptr_t,   //IN:
                                      BOOLEAN      *is_lock_shift,        //OUT:
                                      BOOLEAN      *is_single_shift            //OUT:
                                      );

/*****************************************************************************/
//  Description : Decode R8 Language to judge language
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC MMISMS_R8_LANGUAGE_E MMISMS_R8_DecodeContentForLanguage(
                                    wchar *content,
                                    uint16 len,
                                    BOOLEAN *is_single_shift);

/*****************************************************************************/
//  Description : Change from Turkish alphabet table to 03.38 alphabet table
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 MMISMS_NationalR82default(                          //RETURN: the length of default string
                         uint16      *national_ptr,     //IN:
                         uint8      *default_ptr,   //OUT:
                         uint16     str_len,         //IN:
                         MMISMS_R8_LANGUAGE_E       lang    //IN:
                         );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMISMS_SET_SIMMODE_T MMISMS_Get_Setmode(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_Set_Setmode(MMISMS_SET_SIMMODE_T *set_mode_ptr);

//tts
#ifdef TTS_SUPPORT
/*****************************************************************************/
// 	Description : init  SMS TTS flag
//	Global resource dependence : 
//  Author: barry peng
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_InitTTSFlag(void);
/*****************************************************************************/
// 	Description : get the info of TTS setting status
//	Global resource dependence : 
//  Author: barry.peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetTTSInfo( void );

/*****************************************************************************/
// 	Description : init  SMS TTS flag
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_InitTTSVol(void);

/*****************************************************************************/
// 	Description : get the info of TTS setting status
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetTTSVol( void );

/*****************************************************************************/
// 	Description : set the info of TTS setting status
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetTTSVol(
                              uint8 vol
                              );
/*****************************************************************************/
// 	Description : set the info of TTS setting status
//	Global resource dependence : 
//  Author: barry.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetTTSInfo(
                              MMI_CTRL_ID_T	    ctrl_id
                              ); 
/*****************************************************************************/
// 	Description : add set tts list item
//	Global resource dependence : 
//  Author: barry.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_AppendTTSListItem(
                                     MMI_CTRL_ID_T	    ctrl_id
                                     );
#endif

/*****************************************************************************/
// 	Description : correct time stamp
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_CorrectTimeStamp(MN_SMS_TIME_STAMP_T *time_stamp_ptr, MMISMS_TYPE_E type);

#ifdef MMI_SMS_CONCATENATED_SETTING_SUPPORT
/*****************************************************************************/
//  Description : Convert Char to Alphabet
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC wchar MMISMS_ConvertChar2Alphbet(wchar ch);
#endif

/*****************************************************************************/
//     Description : to handle the signal APP_MN_WRITE_SMS_PARAM_CNF
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleWriteSMSParamCnf(DPARAM param);

#ifdef _ATEST_
/*****************************************************************************/
//  Description : Decode R8 Language to judge language
//  Global resource dependence : none
//  Author: shuyan.hu
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC MMISMS_R8_LANGUAGE_E Atest_MMISMS_R8_DecodeContentForLanguage(
																	 wchar *content,
																	 uint16 len,
																	 BOOLEAN *is_single_shift);
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
// 	Description : create sms setting main window
//	Global resource dependence : 
//  Author: jixin.xu
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISMS_EnterSettingMainWin(void);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMISMS_SET_H_
