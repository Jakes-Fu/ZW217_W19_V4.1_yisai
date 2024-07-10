/****************************************************************************
** File Name:      mmimrapp_export.h                                               *
** Author:                                                                  *
** Date:           03/08/2012                                               *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.        *
** Description:    this file is for the Third Part *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 03/2012        		       Create
** 
****************************************************************************/
#ifndef  _MMIMRAPP_EXPORT_H_    
#define  _MMIMRAPP_EXPORT_H_  

 /**--------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmidisplay_data.h"
#include "mmiset_export.h"
#include "mn_type.h"
#include "mmk_type.h"
#include "block_mem_def.h"
#include "guistatusbar.h"
#include "mmipdp_export.h"
#include "Mmimrapp.h"
#include "Mrporting.h"
#include "Mrapp_mmi_image.h"
#include "Mrapp_mmi_text.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef __cplusplus
   extern   "C"
   {
#endif


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct 
{
    int32 app_id;
    int32 icon_state;
}MRAPP_BACKRUN_INFO_T;

typedef enum
{
	MRAPP_STATE_ONLINE,
	MRAPP_STATE_OFFLINE,
	MRAPP_STATE_AWAY,
	MRAPP_STATE_INVISIBLE,
	MRAPP_STATE_MAX
}MMIMRAPP_BACKRUN_STATE_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTINS                                         *
 **--------------------------------------------------------------------------*/


/*****************************************************************************/
// 	Description : the mr app (games) entity entry
//	Global resource dependence : 
//  Author: qgp
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_AppEntry(MRAPP_APPENTRY_E index);

/*****************************************************************************/
// 	Description : �ر�mrp Ӧ�ü�����
//	Global resource dependence : 
//  Author: qgp
//	Note: ������������·��Ŀ�ݼ����л��˵�ʱ,��Ҫ�ر�mrp,��
// Ϊ�ڴ湲���ԭ��,����������ͻ.
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_closeMrapp(void);

/*****************************************************************************/
// 	Description : save mr app content entry
//	Global resource dependence : 
//  Author: qgp
//	Note:
//return : 
//0		�����ļ���mr �ļ����ұ���ɹ�
//-1	�����ļ���mr�ļ���������ʧ��
// 1	�����ļ�����mr �ļ�

/*****************************************************************************/
PUBLIC int32 MMIMRAPP_saveMrApp(uint8 *data,uint32 len);

/*****************************************************************************/
// 	Description : ����Ƿ���DSM��������Ϣ��
//	Global resource dependence : 
//  Author: qgp
//  Note : return:
// 0��DSM������Ϣ�����ò����ɹ�����Ҫɾ������Ϣ
//-1��DSM������Ϣ�����ò���ʧ�ܣ���Ҫɾ������Ϣ
// 1����DSM������Ϣ
/*****************************************************************************/
PUBLIC int32 MMIMRAPP_dsmSMSfilter(MN_SMS_ALPHABET_TYPE_E  alphabet_type, MN_SMS_USER_VALID_DATA_T *data, MN_CALLED_NUMBER_T *number);

/*****************************************************************************/
// 	Description : ��ѯmr app�Ƿ�������(������mr app���ڴ桢�������Դ
//   ��ͻ��ģ�����)
//	Global resource dependence : 
//  Author: qgp
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_mrappIsActived(void);

/*****************************************************************************/
// 	Description : ���浱ǰ�ֻ��ı�Ҫ״̬.
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC void MMIMRAPP_setPhoneState(e_mrapp_phone_mod mod, int32 state);

/*****************************************************************************/
// 	Description : register mrp module nv len and max item
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC void MMIMRAPP_initModule(void);

/*****************************************************************************/
// 	Description :  ��ʼ������
//	Global resource dependence : 
//  Author:qgp
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIMRAPP_InitSetting(void);

/*****************************************************************************/
// 	Description : ֪ͨӦ�ûָ�ǰ̨����(���app��ǰû�����У�����app���˵�)
//	Global resource dependence : 
//  Author: qgp
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_activeMrapp(void);

/*****************************************************************************/
// 	Description : VM��̨����ʱ����WAP��Camera��ģ��֮ǰ�ȵ��øýӿڣ��ɸýӿ�
//                ���ݴ����ģ��ID������ǿ���˳���̨Ӧ�û��Ǹ�����ʾ
//	Global resource dependence : none
//  Author: zack
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_HandleEventFromOtherModule(int32 module_id);

/*****************************************************************************/
// 	Description : ��ȡ���������״̬
//	Global resource dependence : none
//  Author: zack
//	Note:  
/*****************************************************************************/
PUBLIC int32 MMIMRAPP_GetVMState(void);

/*****************************************************************************/
// 	Description : ��ȡ��ǰmr�е�sim����
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIMRAPP_GetActiveSim(void);

/*****************************************************************************/
// 	Description : ��ȡ��ǰ��̨����Ӧ�õ������Ϣ
//	Global resource dependence : none
//  Author: zack
//	Note:
/*****************************************************************************/
PUBLIC void MMIMRAPP_GetBackRunAPPInfo(MRAPP_BACKRUN_INFO_T *info);

/*****************************************************************************/
// 	Description : �������
//	Global resource dependence : 
//  Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_ProcessEntrStr(
									   uint8* str_ptr, //[IN] input string pointer
									   uint16 str_len  //[IN] input string length
									   );

/*****************************************************************************/
//	Description : Ӧ����Ϣ�������(ֻ�����Լ�Ӧ�õ���Ϣ)
//	Global resource dependence : none
//	Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIMRAPP_handleMrMsg(MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//	Description : �����յ������Ƿ���ʾ
//	Global resource dependence : none
//	Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIMRAPP_GetNewSmsAction(void);

/*****************************************************************************/
//	Description : �жϴ����Ƿ���SKY��Ӧ�ô���
//	Global resource dependence : none
//	Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_IsWindowCreatedBySky(void);

/*****************************************************************************/
//	Description : �ж϶��Ŷ�ȡ�����Ƿ���SKY��Ӧ�ý���
//	Global resource dependence : none
//	Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_IsReadBySMS(void);
/*****************************************************************************/
//	Description : ���ö��Ŷ�ȡ�����Ƿ���SKY��Ӧ�ý���
//	Global resource dependence : none
//	Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_SetIsReadBySMS(BOOLEAN IsReadBySMS);
/*****************************************************************************/
//	Description : �жϵ绰�������Ƿ���SKY��Ӧ�ô�
//	Global resource dependence : none
//	Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_IsWindowOfPB(void);
/*****************************************************************************/
//	Description : ���õ绰�������Ƿ���SKY��Ӧ�ô�
//	Global resource dependence : none
//	Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_SetIsWindowOfPB(BOOLEAN isWindowOfPB);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef   __cplusplus
   }
#endif

#endif/*_MMIMRAPP_H_*/

