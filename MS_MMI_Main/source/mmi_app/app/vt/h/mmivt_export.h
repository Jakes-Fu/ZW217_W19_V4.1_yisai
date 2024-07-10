
/*****************************************************************************
** File Name:      mmivt_export.h                                                   *
** Author:                                                                   *
** Date:           12/7/2005                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/7/2005       bin.ji       Create
******************************************************************************/

#ifndef _MMIVT_EXPORT_H_
#define _MMIVT_EXPORT_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "sci_types.h"
#include "mmk_type.h"
#include "mn_type.h"
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#include "mmifmm_export.h"//@zhaohui
#include "mmipb_export.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef enum
{
    MMIVT_RESULT_SUCCESS,  //ACTIONִ�гɹ�
    MMIVT_RESULT_INVALID_ACTION,   //�Ƿ���ACTION
    MMIVT_RESULT_ERROR_STATE,  //ACTIONִ��ʱ��״̬����
    MMIVT_RESULT_WRONG_APPLET, //UIDIAL��UIREMOTEINCOMINGִ��ʱ�����ڷǷ���applet
    MMIVT_RESULT_NULL_APPLET, //UI ACTIONִ��ʱ��applet������
    MMIVT_RESULT_ERROR_PARM,   //�Ƿ�����
    MMIVT_RESULT_START_PS_FAILED, //�绰����ʱ����psʧ��
    MMIVT_RESULT_FAILED,   //ACTIONִ��ʧ�ܣ�����ԭ����
    MMIVT_RESULT_MAX
        
}MMIVT_RESULT_E;


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : request a videomode call
//	Global resource dependence : 
//  Author: jibin
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIVT_ReqVideoModeCall(MN_DUAL_SYS_E dual_sys, uint8 *tele_num, uint8 tele_len);

/*****************************************************************************/
//  Description : init msg handle func & local variable
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVT_Init(void);

/*****************************************************************************/
//  Description : reg VT MENU group
//  Global resource dependence : none
//  Author: SAMBOO.SHEN
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVT_InitModule(void);

/*****************************************************************************/
//  Description : MMIAPIVT_IsVtCalling
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:��ǰ�Ƿ����VTͨ��
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsVtCalling (void);

/*****************************************************************************/
//  Description : MMIAPIVT_IsVtConnecting
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:�Ƿ��Ѿ�����VTͨ��״̬
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsVtConnecting (void);

/*****************************************************************************/
//  Description : MMIAPIVT_IsVtMoCalling
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:�Ƿ���VT������
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsVtMoCalling (void);

/*****************************************************************************/
//  Description : MMIAPIVT_IsVtMtIncoming
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:�Ƿ���VT������
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsVtMtIncoming (void);

/*****************************************************************************/
//  Description : MMIAPIVT_IsVTIncomingMsg
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:�Ƿ���VT call��setup��Ϣ
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsVTIncomingMsg (uint16 msg_id, DPARAM param);

/*****************************************************************************/
//  Description : MMIVT_GetCallDirect
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIVT_GetCallPbBCDNumber(MMIPB_BCD_NUMBER_T *pb_number_ptr);

/*****************************************************************************/
//  Description : MMIAPIVT_ReleaseVTCall
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MMIVT_RESULT_E MMIAPIVT_ReleaseVTCallByRedkey(void);

/*****************************************************************************/
//  Description : MMIAPIVT_SendUserInput
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MMIVT_RESULT_E MMIAPIVT_SendUserInput(uint32 msg_id);

/*****************************************************************************/
//  Description : MMIAPIVT_IsFallBackCall
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_IsFallBackCall (MN_CALLED_NUMBER_T num);

/*****************************************************************************/
//  Description : MMIAPIVT_ConnectCall
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIVT_ConnectCall (void);
/*****************************************************************************/
//  Description : MMIAPIVT_GetCallName
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MMIVT_RESULT_E MMIAPIVT_GetCallName( MMI_STRING_T* out_name_str_ptr);

/*****************************************************************************/
//  Description : MMIAPIVT_SendMsgToApplet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: �ýӿ�Ŀǰ���ڽ��ն��������Ϣ
/*****************************************************************************/
PUBLIC void MMIAPIVT_SendMsgToApplet(MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description : MMIAPIVT_SetRecordAudioType
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:����ģʽ��VT¼����¼�ƻ���������ڹ���ģʽ�����޸� 0:���� 1:Զ���� 2:������
/*****************************************************************************/
PUBLIC void MMIAPIVT_SetRecordAudioType(uint16 record_audio_type);

/*****************************************************************************/
//  Description : MMIAPIVT_GetRecordAudioType
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:����ģʽ��VT¼����¼�ƻ���������ڹ���ģʽ�����޸� 0:���� 1:Զ���� 2:������
/*****************************************************************************/
PUBLIC uint16 MMIAPIVT_GetRecordAudioType(void);

/*****************************************************************************/
//  Description : MMIAPIVT_ActiveVT
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: ��VT applet��ý��㣬����ֵTRUE������VT applet���ڣ�FALSE����ʾapplet������
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_ActiveVT(void);

/*****************************************************************************/
//  Description : MMIAPIVT_SetIsEPDefault
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: ����ģʽ������Ĭ������ģʽ�Ƿ�Ϊ����
/*****************************************************************************/
PUBLIC void MMIAPIVT_SetIsEPDefault(BOOLEAN is_default_earphone);

/*****************************************************************************/
//  Description : MMIAPIVT_GetIsEPDefault
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: ����ģʽ�»��Ĭ������ģʽ�Ƿ�Ϊ����
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVT_GetIsEPDefault(void);
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif



