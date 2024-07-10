/****************************************************************************
** File Name:      heroEngineExport.h                                               *
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
//#include "mmisms_export.h"
#include "block_mem_def.h"
#include "guistatusbar.h"
#include "mmipdp_export.h"
#include "heroEngineApp.h"
#include "heroEngineApi.h"
#include "heroEngine_mmi_image.h"
#include "heroEngine_mmi_text.h"

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
}HERO_APP_BACKRUN_INFO_T;

typedef enum
{
	HEROAPP_STATE_ONLINE,
	HEROAPP_STATE_OFFLINE,
	HEROAPP_STATE_AWAY,
	HEROAPP_STATE_INVISIBLE,
	HEROAPP_STATE_MAX
}MMIHEROAPP_BACKRUN_STATE_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTINS                                         *
 **--------------------------------------------------------------------------*/


/*****************************************************************************/
// 	Description : the mr app (games) entity entry
//	Global resource dependence : 
//  Author: qgp
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIHEROAPP_AppEntry(HERO_APP_APPENTRY_E index);

/*****************************************************************************/
// 	Description : �ر�mrp Ӧ�ü�����
//	Global resource dependence : 
//  Author: qgp
//	Note: ������������·��Ŀ�ݼ����л��˵�ʱ,��Ҫ�ر�mrp,��
// Ϊ�ڴ湲���ԭ��,����������ͻ.
/*****************************************************************************/
PUBLIC BOOLEAN MMIHEROAPP_closeHeroApp(void);

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
PUBLIC int32 MMIHEROAPP_saveMrApp(uint8 *data,uint32 len);

/*****************************************************************************/
// 	Description : ����Ƿ���DSM��������Ϣ��
//	Global resource dependence : 
//  Author: qgp
//  Note : return:
// 0��DSM������Ϣ�����ò����ɹ�����Ҫɾ������Ϣ
//-1��DSM������Ϣ�����ò���ʧ�ܣ���Ҫɾ������Ϣ
// 1����DSM������Ϣ
/*****************************************************************************/
PUBLIC int32 MMIHEROAPP_hrSMSfilter(MN_SMS_ALPHABET_TYPE_E  alphabet_type, MN_SMS_USER_VALID_DATA_T *data, MN_CALLED_NUMBER_T *number);

/*****************************************************************************/
// 	Description : ��ѯmr app�Ƿ�������(������mr app���ڴ桢�������Դ
//   ��ͻ��ģ�����)
//	Global resource dependence : 
//  Author: qgp
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIHEROAPP_appIsActived(void);

/*****************************************************************************/
// 	Description : ���浱ǰ�ֻ��ı�Ҫ״̬.
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC void MMIHEROAPP_setPhoneState(e_hero_phone_mod mod, int32 state);

/*****************************************************************************/
// 	Description : register mrp module nv len and max item
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC void MMIHEROAPP_initModule(void);

/*****************************************************************************/
// 	Description :  ��ʼ������
//	Global resource dependence : 
//  Author:qgp
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIHEROAPP_InitSetting(void);

/*****************************************************************************/
// 	Description : ֪ͨӦ�ûָ�ǰ̨����(���app��ǰû�����У�����app���˵�)
//	Global resource dependence : 
//  Author: qgp
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIHEROAPP_activeMrapp(void);

/*****************************************************************************/
// 	Description : VM��̨����ʱ����WAP��Camera��ģ��֮ǰ�ȵ��øýӿڣ��ɸýӿ�
//                ���ݴ����ģ��ID������ǿ���˳���̨Ӧ�û��Ǹ�����ʾ
//	Global resource dependence : none
//  Author: zack
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIHEROAPP_HandleEventFromOtherModule(int32 module_id);

/*****************************************************************************/
// 	Description : ��ȡ���������״̬
//	Global resource dependence : none
//  Author: zack
//	Note:  
/*****************************************************************************/
PUBLIC int32 MMIHEROAPP_GetVMState(void);

/*****************************************************************************/
// 	Description : ��ȡ��ǰmr�е�sim����
//	Global resource dependence : none
//  Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIHEROAPP_GetActiveSim(void);

/*****************************************************************************/
// 	Description : ��ȡ��ǰ��̨����Ӧ�õ������Ϣ
//	Global resource dependence : none
//  Author: zack
//	Note:
/*****************************************************************************/
PUBLIC void MMIHEROAPP_GetBackRunAPPInfo(HERO_APP_BACKRUN_INFO_T *info);

/*****************************************************************************/
// 	Description : �������
//	Global resource dependence : 
//  Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIHEROAPP_ProcessEntrStr(
									   uint8* str_ptr, //[IN] input string pointer
									   uint16 str_len  //[IN] input string length
									   );

/*****************************************************************************/
//	Description : Ӧ����Ϣ�������(ֻ�����Լ�Ӧ�õ���Ϣ)
//	Global resource dependence : none
//	Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIHEROAPP_handleMrMsg(MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//	Description : �����յ������Ƿ���ʾ
//	Global resource dependence : none
//	Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIHEROAPP_GetNewSmsAction(void);

/*****************************************************************************/
//	Description : �жϴ����Ƿ���SKY��Ӧ�ô���
//	Global resource dependence : none
//	Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIHEROAPP_IsWindowCreatedBySky(void);

/*****************************************************************************/
//	Description : �ж�VM�Ƿ���pause״̬
//	Global resource dependence : none
//	Author: zack
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIHEROAPP_IsVMPaused(void);

/*****************************************************************************/
//	Description : �ж϶��Ŷ�ȡ�����Ƿ���SKY��Ӧ�ý���
//	Global resource dependence : none
//	Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIHEROAPP_IsReadBySMS(void);
/*****************************************************************************/
//	Description : ���ö��Ŷ�ȡ�����Ƿ���SKY��Ӧ�ý���
//	Global resource dependence : none
//	Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIHEROAPP_SetIsReadBySMS(BOOLEAN IsReadBySMS);
/*****************************************************************************/
//	Description : �жϵ绰�������Ƿ���SKY��Ӧ�ô�
//	Global resource dependence : none
//	Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIHEROAPP_IsWindowOfPB(void);
/*****************************************************************************/
//	Description : ���õ绰�������Ƿ���SKY��Ӧ�ô�
//	Global resource dependence : none
//	Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_SetIsWindowOfPB(BOOLEAN isWindowOfPB);

/*****************************************************************************/
// 	Description :  ��ȡ������Ϣ��order id
//	Global resource dependence : 
//  Author: Tommy.yan
//   note:
/*****************************************************************************/
//PUBLIC MMISMS_STATE_T * MMIHEROAPP_GetSaveSMSOrderId(void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef HERO_APP_SHORTCUT_SUPPORT
void hero_init_shortcut_menu(MMI_CTRL_ID_T ctrl_id);

void hero_shortcut_menu_event(MMI_MENU_ID_T menu_id);

int32 hr_get_shortcut_data_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

int32 hr_set_shortcut_data_req(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);

#endif

#ifdef   __cplusplus
   }
#endif

#endif/*_HERO_ENGINE_APP_H_*/

