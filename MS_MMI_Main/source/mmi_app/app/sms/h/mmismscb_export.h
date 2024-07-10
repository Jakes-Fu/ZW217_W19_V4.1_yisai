/******************************************************************************
**
**
**
**  С���㲥ģ��
**
**
**
**
**
**
*******************************************************************************/
#if defined(MMI_SMSCB_SUPPORT)

#ifndef  _MMISMSCB_EXPORT_H_    
#define  _MMISMSCB_EXPORT_H_   
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mn_type.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif


/******************************************************************************************************************/
//
//
//                        С���㲥ģ�����ӿں�������
//
//
/******************************************************************************************************************/

/***************************************************************************/
//     Description : ��ʼ��С���㲥�ӿں���(���ֻ���ʼ��ʱ����)
//    Global resource dependence : none
//  Author:Ф��
//    Note:
/***************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMIAPISMSCB_Init(MN_DUAL_SYS_E dual_sys);

//window related
/*****************************************************************************/
//     Description : open SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_OpenMainMenu(void);

/*****************************************************************************/
//     Description : open SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_RestoreFactorySetting(void);

/*****************************************************************************/
//  Description : open SMSCB setting window      
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_OpenSettingWin(void);

/*****************************************************************************/
//     Description : MMIAPISMSCB_OpenCBList
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_OpenCBList(void);

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
//  Description : Set SMS CB Status
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SwitchSmsCBStatus(BOOLEAN cb_enabled);
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSCB_HandleCBMsgInd
//  ��������:        �յ���С���㲥�Ĵ�����
//  ȫ�ֱ���ʹ��:    s_cb_msg
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
void MMIAPISMSCB_HandleCBMsgInd(DPARAM param);       //�յ�С���㲥�źŴ���

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSCB_HandleMarcoCBMsgInd
//  ��������:        �յ���С���㲥�Ĵ�����
//  ȫ�ֱ���ʹ��:    s_cb_msg
//  ����ֵ:          ��
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
void MMIAPISMSCB_HandleMarcoCBMsgInd(DPARAM param);       //�յ�С���㲥�źŴ���

#ifdef PDA_UI_DROPDOWN_WIN
/*****************************************************************************/
//  Description : Get Unread SMS CB Number
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPISMSCB_GetUnreadSMSCBNum(void);

/*****************************************************************************/
//  Description : Get Unread SMS CB Channel
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetUnreadSMSCBChannel(wchar *name_ptr, uint16 max_len);

/*****************************************************************************/
//  Description : Get Unread SMS CB Time
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetUnreadSMSCBTime(wchar *time_ptr, uint16 max_len);
#endif

#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
/*****************************************************************************/
//  Description : ��ӹ�����Ϣ
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetScrollItemNewSMSCB(void);
#endif


#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author: zhaohui
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_StopETWSVibrateTimer(void);

//----------------------------------------------------------------------------------------------------------------
//
//  ��������         MMISMSCB_ConvertCBTimeToStr
//  ��������:        ��С���㲥ʱ��תΪUCS2���׼�ַ�����ʽ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ת������ַ�������
//  ����:            Ф��
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC uint16 MMISMSCB_ConvertCBTimeToStr(                            //RETURN:
								   SCI_DATE_T    date,        //IN: Ҫ����ת�������ڽṹ
								   SCI_TIME_T    time,       //IN: Ҫ����ת����ʱ��ṹ
								   wchar            *str_ptr    //OUT: ����ַ�����ָ��
								   );
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif//_MMISMSCB_EXPORT_H_
#endif//#if defined(MMI_SMSCB_SUPPORT)

