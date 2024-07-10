/*****************************************************************************
** File Name:      mmiset_set_ring.h                                           *
** Author:                                                                   *
** Date:           2006/09/18                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe setting definitions          *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18    kelly.li      Create
******************************************************************************/

#ifndef _MMISET_SET_RING_H_
#define _MMISET_SET_RING_H_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "mmiset.h"
#include "mmiset_internal.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define MMIPHONE_ADJUST_VOL_LEVEL_BOX_LEFT              10
#define MMIPHONE_ADJUST_VOL_LEVEL_BOX_WIDHT             143
#define MMIPHONE_ADJUST_VOL_LEVEL_BOX_HEIGHT            46
#define MMIPHONE_ADJUST_VOL_LEVEL_BOX_DISTANCE          18

#define MMIPHONE_ADJUST_VOL_BG_WIDHT                    107
#define MMIPHONE_ADJUST_VOL_BG_HEIGHT                   89
#define MMIPHONE_ADJUST_VOL_PLUG_WIDTH                  47
#define MMIPHONE_ADJUST_VOL_PLUG_HEIGHT                 25
#define MMIPHONE_ADJUST_VOL_MINUS_LEFT                  22//�������ڼ��ٰ�ť�������
#define MMIPHONE_VOL_WIDTH  					        11//18//�������ڵ������Ŀ��
#define MMIPHONE_VOL_HEIGHT 					        50//�������ڵ������ĸ߶�
#else
#define MMIPHONE_VOL_WIDTH  								15 //�������ڵ������Ŀ��
#define MMIPHONE_VOL_HEIGHT 								64//�������ڵ������ĸ߶�
#define MMIPHONE_ADJUST_VOL_LEVEL_BOX_DISTANCE          24
#define MMIPHONE_ADJUST_VOL_LEVEL_BOX_HEIGHT            MMIPHONE_VOL_HEIGHT
#define MMIPHONE_ADJUST_VOL_LEVEL_BOX_WIDHT             (MMIPHONE_ADJUST_VOL_LEVEL_BOX_DISTANCE*(MMISET_VOL_MAX - 1)+MMIPHONE_VOL_WIDTH)

#define MMIPHONE_ADJUST_VOL_BG_WIDHT                    69
#define MMIPHONE_ADJUST_VOL_BG_HEIGHT                   122
#define MMIPHONE_ADJUST_VOL_PLUG_WIDTH                  55
#define MMIPHONE_ADJUST_VOL_PLUG_HEIGHT                 33
#define MMIPHONE_ADJUST_VOL_MINUS_LEFT                  0 //�������ڼ��ٰ�ť��
#endif 




/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/


//�̶���������ѡ�񴰿ڵĽṹ����
typedef struct  
{
    MN_DUAL_SYS_E               dual_sys;
    uint16                      selected_ring_id; //��ѡ�������ID
    MMI_WIN_ID_T                win_id;           //������Ϣ��Ŀ�Ĵ���
}MMISET_SELECT_WIN_PARAM_T;

//���������Ϳ��ػ������������Ϣ
typedef struct
{
    uint8                       other_ring_volume;    //the volume of other ring
    MMISET_OTHER_RING_TYPE_E    other_ring_type;      //the type of other ring
}MMISET_OTHER_RING_T;

#ifdef MMI_ETWS_SUPPORT
typedef struct
{
    BOOLEAN is_personal;
    BOOLEAN is_need_torch;
    MN_DUAL_SYS_E dual_sys;
    MMISET_MSG_RING_TYPE_E  ring_type;
} MMISET_ETWS_RING_T;
#endif
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/




/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
void MMISET_RingInit(void);

/*****************************************************************************/
//  Description : ������̶������󣬽�ID��Ϣ����nv
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_SetFixedCallRingId(
//                                      MN_DUAL_SYS_E dual_sys,
//                                      uint16 ring_id
//                                       );

/*****************************************************************************/
//  Description : �����������Ϣ�����󣬽���Ϣ����nv
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetMoreMsgRingInfo(
                                      MN_DUAL_SYS_E dual_sys,
                                      MMI_CTRL_ID_T   ctrl_id,
                                      MMISET_CALL_MORE_RING_T ring_info
                                      );

/*****************************************************************************/
//  Description : ��������Ϣ��������������б��У������̶������͸�������
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_AppendMsgRingClassItem(
                                          MN_DUAL_SYS_E dual_sys,
                                          MMI_CTRL_ID_T ctrl_id
                                          );

/*****************************************************************************/
//  Description : get message ring id
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
// PUBLIC uint8 MMISET_GetFixedMsgRingId(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : set the type of call ring
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_SetCallRingType(
//                            MMISET_CALL_RING_TYPE_E  call_ring_type,//in
//                            MN_DUAL_SYS_E            dual_sys
//                             );

/*****************************************************************************/
//  Description : get msg ring info including ring class and index and ring colum
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
// PUBLIC MMISET_MSG_RING_T MMISET_GetMsgRingInfo(MN_DUAL_SYS_E dual_sys,uint8 mode_id);

/*****************************************************************************/
//  Description : ����radio listѡ�е�item������������ʾ��ʽ,��Ϣ��ʾ��ʽ���������ͷ�����ʾ��
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_SetSelectRadioItem(
//                                      MMI_CTRL_ID_T         ctrl_id,
//                                      MMISET_ALL_RING_TYPE_E   ring_type
//                                       );

/*****************************************************************************/
//  Description : ��ӵ͵�ѹ�������Ŀ����رյĵ�ѡlist items
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_AppendVoltageWarningListItem(
//                                                MMI_CTRL_ID_T         ctrl_id
//                                                 );

/*****************************************************************************/
//  Description : ����radio list��ǰ��Ӱ���ڵ�������ʾ��ʽ��
//                  ��������Ӧ���������񶯻��߾�����
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_PlayCallTypeByListItem(
//                                          MMI_CTRL_ID_T   ctrl_id
//                                           );

/*****************************************************************************/
//  Description : ����radio list��ǰ��Ӱ���ڵ���Ϣ��ʾ��ʽ��
//                  ��������Ӧ���������񶯻��߾�����
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_PlayMsgTypeByListItem(
//                                         MMI_CTRL_ID_T    ctrl_id
//                                          );

/*****************************************************************************/
//  Description : ����radio list��ǰ��Ӱ���ڵİ�������
//                  ��������Ӧ���������߾�����
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_PlayKeyToneByListItem(
//                                         MMI_CTRL_ID_T    ctrl_id
//                                          );

/*****************************************************************************/
//  Description : ����radio list��ǰ��Ӱ���ڵİ�������
//                  ��������Ӧ���������߾�����
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_PlayFlipRingByListItem(
//                                          MMI_CTRL_ID_T   ctrl_id
//                                           );

/*****************************************************************************/
//  Description : ���������Ĵ�С��ʾ����ͼƬ
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_DisplayVolumePicture(
//                                 uint8   volume,
//                                 MMI_WIN_ID_T win_id
//                                  );
/*****************************************************************************/
//  Description : ���ҷ�������������Ĵ�С
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_TuneUpVolume(
//                                MMI_MESSAGE_ID_E           msg_id,
//                                MMISET_ALL_VOLUME_TYPE_E   volume_type
//                                 );
/*****************************************************************************/
//  Description : ��������������������б��У������̶������͸�������
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_AppendCallRingCalssItem(MN_DUAL_SYS_E dual_sys, MMI_CTRL_ID_T  ctrl_id);

/*****************************************************************************/
//  Description : ����radio list��ǰ��Ӱ���ڵ�����������������������Ϣ����
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_PlayRingByListItem(
                                      MMI_CTRL_ID_T         ctrl_id,
                                      MMISET_ALL_RING_TYPE_E   ring_type
                                      );

/*****************************************************************************/
//  Description : ����radio list����������Ϣ����
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetMsgRingByRadioItem(
                                         MN_DUAL_SYS_E  dual_sys,
                                         MMI_CTRL_ID_T  ctrl_id
                                         );
/*****************************************************************************/
//  Description : get fixed call ring index from nv
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
// PUBLIC uint16 MMISET_GetFixedCallRingId(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : set open or close low voltage warning
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_IsOpenVoltageWarning(
//                                        BOOLEAN  is_open
//                                         );

/*****************************************************************************/
//  Description : ��ӹ̶���������list��item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendFixedCallRingItem(
                                           MN_DUAL_SYS_E dual_sys,
                                           MMI_CTRL_ID_T    ctrl_id,
                                           MMISET_SELECT_WIN_PARAM_T     *win_param_ptr
                                           );

///*****************************************************************************/
////Description : Ԥ������ 
////Global resource dependence : 
////Author: maqianlin
////Note:
///*****************************************************************************/
//PUBLIC void MMISET_PreviewRing(
//                        MN_DUAL_SYS_E dual_sys,
//                        uint16                       ring_id,    // the ring id
//                        uint8                       ring_vol,   // the ring volume 
//                        uint32                      play_times, // play times
//                        MMISET_ALL_RING_TYPE_E      ring_type,  // in: the type of all ring
//                        MMISET_RING_CALLBACK_PFUNC  callback_func
//                        );
//
//
///*****************************************************************************/
////Description : ������ 
////Global resource dependence : 
////Author: maqianlin
////Note: 
///*****************************************************************************/
//PUBLIC void MMIENVSET_StartVibrator(MMISET_ALL_RING_TYPE_E  ring_type);
//
///*****************************************************************************/
////Description : �����𶯼�ʱ�� 
////Global resource dependence : 
////Author: maqianlin
////Note:�ڿ�����ʱ��
///*****************************************************************************/
//PUBLIC void MMISET_StartVibratTimer(void);
//
///*****************************************************************************/
////Description : �ر��� 
////Global resource dependence : 
////Author: maqianlin
////Note:
///*****************************************************************************/
//PUBLIC void MMISET_StopVibrator(
//                              MMISET_ALL_RING_TYPE_E  ring_type
//                              );
//
///*****************************************************************************/
////Description : �ر��𶯼�ʱ�� 
////Global resource dependence : 
////Author: maqianlin
////Note:
///*****************************************************************************/
//PUBLIC void MMIENVSET_StopVibratTimer(void);
//
///*****************************************************************************/
////Description : �ж��Ƿ��𶯼�ʱ��ʱ�䵽 
////Global resource dependence : 
////Author: maqianlin
////Note:
///*****************************************************************************/
//PUBLIC BOOLEAN MMISET_IsVibratTimerOut(DPARAM param);
//
///*****************************************************************************/
////    Description : to STOP the vibrator
////    Global resource dependence : 
////  Author:jassmine.meng
////    Note: 
///*****************************************************************************/
//PUBLIC void MMISET_RingCallBack(MMIAUDIO_RESULT result, DPARAM param);
//
///*****************************************************************************/
////Description : �ж��𶯼�ʱ���Ƿ��� 
////Global resource dependence : 
////Author: maqianlin
////Note:
///*****************************************************************************/
// PUBLIC BOOLEAN MMISET_IsVibratTimerActive(void);

/*****************************************************************************/
// 	Description : ��NV�б������������������б��У������̶������͸�������
//	Global resource dependence : 
//  Author:jianhui.luo
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_AppendNVCallRingCalssItem(MN_DUAL_SYS_E dual_sys, MMI_CTRL_ID_T  ctrl_id);
#ifdef MONOCHROME_LCD_SUPPORT 
/*****************************************************************************/
// 	Description : DPHONE �������ڻ���
//	Global resource dependence : 
//  Author:yiwen.man
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_DrawAdjustVolumePicture(uint8   volume,
								MMI_WIN_ID_T win_id);
#endif
#endif
