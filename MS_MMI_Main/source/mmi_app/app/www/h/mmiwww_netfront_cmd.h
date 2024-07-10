/***************************************************************************
** File Name:      mmiwww_netfront_cmd.h                                            *
** Author:                                                                 *
** Date:           15/06/2009                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the netfront cmd define            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/2009        wancan.you        Create
****************************************************************************/
#ifndef  _MMIWWW_NETFRONT_CMD_H_    
#define  _MMIWWW_NETFRONT_CMD_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmiwww_internal.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Callback function which gives notification of error information.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyErrorProc(slim_int in_index, slim_int in_code, slim_int in_nf);

/*****************************************************************************/
//  Description : Callback function which gives notification of content error information.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyContentErrorProc(slim_int in_index, slim_int in_code, slim_int in_nf);

/*****************************************************************************/
//  Description : Callback function which requests network connection.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestConnectionProc(slim_int in_index, slim_int in_nf);

/*****************************************************************************/
//  Description : Callback function which requests to get special scheme other than HTTP/HTTPS.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestExSchemeRequestProc(slim_int in_index, slim_char *in_url, slim_int in_nf);

/*****************************************************************************/
//  Description : Callback function which requests authentication information.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestAuthProc(slim_char *in_url, slim_char *in_realm, slim_char *in_user, slim_char *in_pass, slim_int in_nf);

/*****************************************************************************/
//  Description : Callback function which requests confirmation to accept cookie, it can only be called in prompt cookie mode.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestCookieConfirmProc(slim_char *in_url, slim_char *in_cookie, slim_int in_nf);

/*****************************************************************************/
//  Description : Callback function which requests confirmation of server certificate, it's called due to server certificate verification problem during SSL communication.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestServerCertConfirmProc(slim_int in_verify_result, slim_int in_nf);

/*****************************************************************************/
//  Description : Callback function which requests displaying JavaScript warning message, it's called when the JavaScript window.alert method is called.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestJSAlertProc(slim_int in_index, slim_char *in_message, slim_int in_nf);

/*****************************************************************************/
//  Description : Callback function which requests displaying of JavaScript confirmation dialog box, it's called when the JavaScript window.confirm method is called.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestJSConfirmProc(slim_int in_index, slim_char *in_message, slim_int in_nf);

/*****************************************************************************/
//  Description : Callback function which requests displaying of JavaScript prompt, it's called when the JavaScript window.prompt method is called.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestJSPromptProc(slim_int in_index, slim_char *in_message,slim_char *in_value, slim_int in_nf);

/*****************************************************************************/
//  Description : Callback function which requests to open a new window.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestOpenWindowProc(slim_int in_index, slim_int in_request, slim_int in_nf);

/*****************************************************************************/
//  Description : Callback function which notifies that NF browser engine started getting content in the window.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyWindowStartProc(slim_int in_index, slim_int in_nf);

/*****************************************************************************/
//  Description : Callback function which notifies that document analysis within the target window is complete, and all linked content has been obtained.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyWindowDoneProc(slim_int in_index, slim_int in_nf);

#ifdef SLIM_CONFIG_USE_WML
/*****************************************************************************/
//  Description : Callback function which notifies that an error occurred in WML content.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyWMLErrorProc(slim_char *in_message, slim_int in_nf);
#endif

/*****************************************************************************/
//  Description : Callback function which gives input format error notification. It's called when there is input that violates the format attribute specified in WML input element or the specification of the CSS -wap-input-format property.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyInputFormatErrorProc(slim_int in_index, slim_char *in_format, slim_char *in_value, slim_int in_nf);

/*****************************************************************************/
//  Description : Callback function which notifies that a syntax error occurred in XML content.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyXMLSAXErrorProc(slim_char *in_message, slim_int in_nf);

#ifdef SLIM_CONFIG_USE_WMLSCRIPT
/*****************************************************************************/
//  Description : Callback function which requests displaying WMLScript warning message, it's called when the WMLScript window.alert method is called.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestWMLSAlertProc(slim_char *in_message, slim_int in_nf);

/*****************************************************************************/
//  Description : Callback function which requests displaying WMLScript confirmation message, it's called when the WMLScript window.confirm method is called.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestWMLSConfirmProc(slim_char *in_message, slim_int in_nf);

/*****************************************************************************/
//  Description : Callback function which requests displaying of WMLScript prompt dialog box, it's called when the WMLScript window.prompt method is called.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_RequestWMLSPromptProc(slim_char *in_message, slim_char *in_value, slim_int in_nf);
#endif

/*****************************************************************************/
//  Description : notify helper app start 
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_HelperProc(NFBrowserDownloadInfo *in_info, slim_int in_self);

/*****************************************************************************/
//  Description : notify helper app end
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_HelperDoneProc (NFBrowserDownloadInfo *in_info, slim_int in_nf);

/*****************************************************************************/
//  Description : notify entering text control, inline IME should be started in this callback
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_EnterTextControlProc(void* in_arg, NFBrowserControlInfo *in_info);

/*****************************************************************************/
//  Description : notify leaving text control, inline IME should be quited in this callback
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_LeaveTextControlProc(void* in_arg);

/*****************************************************************************/
//  Description : notify text control position moved,  inline IME should be moved accordingly
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_MoveTextControlProc(void* in_arg, NFBrowserControlInfo *in_info);

/*****************************************************************************/
//  Description : get text control value from inline IME, charset of out_string must be utf-8
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_GetTextControlValueProc(void *in_arg, slim_char *out_string);

/*****************************************************************************/
//  Description : get text control value length from inline IME
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int MMIWWW_GetTextControlValueLengthProc(void *in_arg);

/*****************************************************************************/
//  Description : set text control value to inline IME, charset of in_string is utf-8
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_SetTextControlValueProc(void *in_arg, slim_char *in_string);

/*****************************************************************************/
//  Description : notify inline IME visible or invisible
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_ShowTextControlProc(void *in_arg, slim_bool in_show);

/*****************************************************************************/
//  Description : enter the file selection process
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_FileSelectEnterProc(slim_int in_select, slim_int in_nf);

/*****************************************************************************/
//  Description : leave the file selection process
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_FileSelectLeaveProc(slim_int in_nf);

/*****************************************************************************/
//  Description : get display name based on full path of file
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_FileSelectQueryNameProc(slim_char* in_fullpath, slim_char* out_name, slim_int in_nf);

/*****************************************************************************/
//  Description : show favicon callback
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_FaviconNotifyShowProc(slim_int in_index, NFBrowserPixelMap *out_pixelmap, slim_int in_nf);

/*****************************************************************************/
//  Description : notify pointer shape callback
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyPointerStatesProc(slim_int in_x,slim_int in_y,slim_int in_shape,slim_int in_show, slim_int in_nf);

/*****************************************************************************/
//  Description : notify callback for copy selected text
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_CopySelectedTextProc(slim_char* in_text, slim_int in_len, slim_int in_nf);
                                                 
#ifdef SLIM_CONFIG_USE_RSS
/*****************************************************************************/
//  Description : notify callback for RSS auto discovery
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_FeedNotifyItemCountProc(slim_int in_index, NFBrowserPixelMap *out_pixelmap, slim_int in_nf);
#endif

/*****************************************************************************/
//  Description : set timer proc
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC NFBrowserTimer MMIWWW_Netfront_SetTimer(NFBrowserTimerImpl *self, slim_int in_time, NFBrowserTimer_TimerProc in_proc, slim_opaque in_arg);

/*****************************************************************************/
//  Description : kill timer proc
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_bool MMIWWW_Netfront_KillTimer(NFBrowserTimerImpl *self, NFBrowserTimer in_timer);

/*****************************************************************************/
// Description : Update down/up progress call back
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_UpdateWindowProgressProc(slim_int  in_downlink_percent, 
                                            slim_int  in_uplink_percent,
                                            slim_int  in_nf);

/*****************************************************************************/
// Description : Update page title call back
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIWWW_UpdateWinTitleProc(slim_int   in_index,
                                      slim_char *in_title, 
                                      slim_int   in_charset, 
                                      slim_int   in_nf);

/*****************************************************************************/
//  Description : NF browser engine notifies NF browser application whether current webpage is secure.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NotifySecurePageProc(slim_bool in_secure, slim_int in_nf);

/*****************************************************************************/
//  Description : NF browser engine notifies NF browser application whether bgsound exists in the window where webpage was just now loaded.
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_NotifyBGSoundExistProc(slim_bool in_exist, slim_int in_index, slim_int in_nf);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif

