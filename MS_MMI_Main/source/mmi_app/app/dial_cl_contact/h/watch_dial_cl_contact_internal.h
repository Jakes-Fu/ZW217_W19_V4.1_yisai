/*****************************************************************************
** File Name:      watch_dial_cl_contact_internal.h                                                *
** Author:                                                                   *
** Date:           2021/01/18                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe dial cl cotact win data     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2021/01/18     feiyue.ji        Create                                    *
******************************************************************************/

#ifndef  _DIAL_CL_CONTACT_H_
#define  _DIAL_CL_CONTACT_H_

/*----------------------------------------------------------------------------*/
/*                         Include Files                                      */
/*----------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmipb_export.h"
#include "mmi_custom_define.h"
#include "mmicl_internal.h"
#include "mmi_appmsg.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#ifndef WIN32
#define TRACE_APP_DIAL_CL_CONTACT(_format,...) \
    do \
    { \
        SCI_TraceLow("{%s}[%s]<%d> "_format, "DIAL_CL_CONTACT", __FUNCTION__, __LINE__, ##__VA_ARGS__);\
    }while(0)
#else
#define TRACE_APP_DIAL_CL_CONTACT(_format,...) SCI_TRACE_LOW
#endif

#define WATCH_MAX_CALL_TIME_LEN   MMICL_MAX_CALL_TIMES //��������ִ��ĳ���
#define WATCH_PHONE_NUMMAX_LEN   (MMIPB_BCD_NUMBER_MAX_LEN << 1)//�绰����ĳ���
#define DISPLAY_STR_MAX_LEN    (WATCH_MAX_CALL_TIME_LEN+WATCH_PHONE_NUMMAX_LEN)//��ʾ�绰����������ִ�����󳤶�
#define WATCH_DATA_TIME_STR_MAX_LEN 60 //��ʾʱ���ִ�����󳤶�
#define WATCH_CL_RECORD_MAX_NUM MMICL_RECORD_MAX_NUM

/*----------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                    */
/*----------------------------------------------------------------------------*/
#include "mmk_type.h"


//one calllog view data
typedef struct _CALLLOG_VIEW_DATA_STRUCT
{
    uint32 calltype_image_id;//���磬ȥ�磬δ��ͼƬID
    wchar  disp_str[DISPLAY_STR_MAX_LEN+1];//�绰�������ϵ�˵�����
    wchar  datetime_wstr[WATCH_DATA_TIME_STR_MAX_LEN + 1];//�յ��绰��ʱ��
    uint8   number[WATCH_PHONE_NUMMAX_LEN + 1];   //the display number
    BOOLEAN  is_unread_missed_call;//�Ƿ���û�в鿴��δ������
}CL_VIEW_DATA_T;

typedef struct _CALLLOG_VIEW_DATA_INFO_STRUCT
{
    CL_VIEW_DATA_T calllog_view_data_array[WATCH_CL_RECORD_MAX_NUM];//calllog view data
    uint16 calllog_count;//calllog����Ŀ
    MMI_TEXT_ID_T calllog_title_text_id;//calllog�ı���ID
    GUI_COLOR_T title_color;
    MMI_TEXT_ID_T calllog_empty_text_id;//calllogΪ��ʱ��ʾ���ִ�ID
    uint16       missedcall_count;//δ����δ���������Ŀ
}CL_VIEW_DATA_INFO_T;

//single contact view data
typedef struct _CONTACT_VIEW_DATA_STRUCT
{
    MMI_IMAGE_ID_T contact_photo_id;//ͷ��
    wchar  name_str[DISPLAY_STR_MAX_LEN+1];//�绰�������ϵ�˵�����
    uint16 contact_index;//��ϵ�˵�index
    uint8   number[WATCH_PHONE_NUMMAX_LEN + 1];   //the display number
}CONTACT_VIEW_DATA_T;

typedef struct _CONTACT_VIEW_DATA_INFO_STRUCT
{
    CONTACT_VIEW_DATA_T  contact_view_data_array[WATCHPB_CONTACT_NUM_MAX];//��ϵ�� view data
    MMI_TEXT_ID_T contact_title_text_id;//contact�ı���ID
    GUI_COLOR_T title_color;
    MMI_TEXT_ID_T contact_empty_text_id;//contactΪ��ʱ��ʾ���ִ�ID
    uint16       contact_count;//contact����Ŀ
}CONTACT_VIEW_DATA_INFO_T;
//window view data
typedef struct _DIAL_CL_CONTACT__VIEW_DATA_STRUCT
{
    //dial image id
    MMI_IMAGE_ID_T       dial_image_id;//dial ��ť��ͼƬid

    //calllog view data
    CL_VIEW_DATA_INFO_T* calllog_view_data_ptr;

    CONTACT_VIEW_DATA_INFO_T*       contact_view_data_ptr;//contact data������ָ��
}DIAL_CL_CONTACT_VIEW_DATA_T;

/*****************************************************************************/
//  Description : to create dial calllog contact view data
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC DIAL_CL_CONTACT_VIEW_DATA_T* Watch_Dial_Cl_Contact_CreateViewData(void);

/*****************************************************************************/
//  Description : to destroy dial calllog contact view data
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC void Watch_Dial_Cl_Contact_DestroyViewData(DIAL_CL_CONTACT_VIEW_DATA_T* view_data_ptr);

/*****************************************************************************/
//  Description : to load dial calllog contact view data
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC void Watch_Dial_Cl_Contact_LoadViewData(DIAL_CL_CONTACT_VIEW_DATA_T* view_data_ptr);

/*****************************************************************************/
//  Description : to lsave calllog  data to nv
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC void Watch_Dial_Cl_Contact_SaveMissedCallData(void);

PUBLIC void Watch_Dial_Cl_Contact_Win_Enter(void);

#endif//_DIAL_CL_CONTACT_H_

