/****************************************************************************
** File Name:      mmivt_kernel_refAdapter.h                               *
** Author:                                                                 *
** Date:           12/18/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/2009        samboo.shen         Create
****************************************************************************/
#ifndef _MMIVT_KERNEL_REFADAPTER_H_
#define _MMIVT_KERNEL_REFADAPTER_H_

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/


//#include "mmivt_export.h"
#ifdef VT_SUPPORT

#include "sci_types.h"

//#include "mmiset.h"
#include "mmiset_export.h"

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
    MMIVT_DISP_MODE_REMOTE_LOCAL,
    MMIVT_DISP_MODE_LOCAL_REMOTE,
    MMIVT_DISP_MODE_REMOTE_ONLY,
    MMIVT_DISP_MODE_LOCAL_ONLY,
    MMIVT_DISP_MODE_PREVIEW,
    MMIVT_DISP_MODE_MAX
}MMIVT_DISP_MODE_E;

typedef enum
{
    VT_IMAGE_ROTATE_90_DEGREE,  //90����ת
    VT_IMAGE_ROTATE_180_DEGREE, //180����ת
    VT_IMAGE_ROTATE_MAX
}MMIVT_IMAGE_ROTATE_MODE_E; 

typedef enum
{
    VT_IMAGE_STATUS_ROTATE_0_DEGREE,
    VT_IMAGE_STATUS_ROTATE_90_DEGREE,  //90����ת
    VT_IMAGE_STATUS_ROTATE_180_DEGREE, //180����ת
    VT_IMAGE_STATUS_ROTATE_270_DEGREE,
    VT_IMAGE_STATUS_ROTATE_MAX
}MMIVT_IMAGE_ROTATE_STATUS_E; //��¼��ǰͼ����ת״̬

typedef enum
{
    MMIVT_VIDEO_LEVEL_ONE,
    MMIVT_VIDEO_LEVEL_TWO,
    MMIVT_VIDEO_LEVEL_THREE,
    MMIVT_VIDEO_LEVEL_FOUR,
    MMIVT_VIDEO_LEVEL_FIVE,
    MMIVT_VIDEO_LEVEL_SIX,
    MMIVT_VIDEO_LEVEL_SEVEN
}MMIVT_VIDEO_LEVEL_E; //���ȣ��Աȶȣ�����Ĺ��õȼ���1~7


typedef enum
{
    MMIVT_CAMERA_ID_MAIN,
    MMIVT_CAMERA_ID_SECONDARY,
    MMIVT_CAMERA_ID_MAX
}MMIVT_CAMERA_ID_E; //sensor ID


typedef enum
{
    MMIVT_LOCATION_LOCAL, //����
    MMIVT_LOCATION_REMOTE,//Զ��
    MMIVT_LOCATION_MAX
}MMIVT_LOCATION_E; //local or remote

typedef enum
{
    MMIVT_MEDIA_TYPE_PIC, //ͼƬ
    MMIVT_MEDIA_TYPE_VIDEO,//��Ƶ
    MMIVT_MEDIA_TYPE_DEFAULT, //Ĭ����Դ
    MMIVT_MEDIA_TYPE_MAX
}MMIVT_MEDIA_TYPE_E; //media type


typedef enum
{
    MMIVT_RECORD_TYPE_RV_DA, //Զ��ͼ��ͻ���
    MMIVT_RECORD_TYPE_RV, //Զ��ͼ��
    MMIVT_RECORD_TYPE_DA, //����
    MMIVT_RECORD_TYPE_MAX
}MMIVT_RECORD_TYPE_E;

typedef enum
{
    MMIVT_REF_RET_SUCCESS  = 0,
    MMIVT_REF_RET_FINISHED,
    MMIVT_REF_RET_NOT_SUPPORTED,
    MMIVT_REF_RET_MEMORY_ERR,
    MMIVT_REF_RET_HARDWARE_ERR,
    MMIVT_REF_RET_SENSOR_ERR,
    MMIVT_REF_RET_INVALID_PARAM,
    MMIVT_REF_RET_INVALID_STATE,
    MMIVT_REF_RET_INVALID_IMAGE,
    MMIVT_REF_RET_FAIL,
    MMIVT_REF_RET_DISK_FULL,   //--recorder disk is almost full.
    MMIVT_REF_RET_FILE_FULL,   //--recoreder file is too bigger.
    MMIVT_REF_RET_FILE_ERR,    //--file operation error.

    /*���ϲ��ֱ����Ӧ�ṹ�� DVT_MW_RET_E ����Ϊmmi�Լ���ӵĴ������ */
    
    MMIVT_REF_RET_REPLACE_UDISK_IN_USE, //���ʱ��U������ʹ����
    MMIVT_REF_RET_REPLACE_FILE_NOT_EXIST, //���ʱ������ļ�������
    MMIVT_REF_RET_BLUETOOTH_IN_USE, //����ʹ��ʱ����¼������
    MMIVT_REF_RET_MAX
}MMIVT_REF_RET_E; 
//typedef enum
//{
//    MMIVT_DISP_MODE_REMOTE_LOCAL,   //Զ��ͼ��Ϊ��ͼ������ͼ��Ϊ��ͼ
//    MMIVT_DISP_MODE_LOCAL_REMOTE,   //����ͼ��Ϊ��ͼ��Զ��ͼ��Ϊ��ͼ
//    MMIVT_DISP_MODE_REMOTE_ONLY,    //Զ��ͼ��Ϊ��ͼ���ޱ���ͼ��
//    MMIVT_DISP_MODE_LOCAL_ONLY,     //����ͼ��Ϊ��ͼ���ޱ���ͼ��
//    MMIVT_DISP_MODE_MAX
//}MMIVT_DISP_MODE_E;//��ͼΪ��ͼ��ͼ���ں󣻸�ͼΪСͼ��ͼ����ǰ
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : MMIVT_REF_OpenVT
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:�����ײ��豸
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_OpenVT(void);


/*****************************************************************************/
//  Description : MMIVT_REF_CloseVT
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:�رյײ��豸
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_CloseVT(void);


/*****************************************************************************/
//  Description : MMIVT_REF_StartPreview
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:����previewģʽ����ʾ���˻���
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_StartPreview(void);


/*****************************************************************************/
//  Description : MMIVT_REF_StopPreview
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:�˳�previewģʽ
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_StopPreview(void);


/*****************************************************************************/
//  Description : MMIVT_REF_StartTalk
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:����dialģʽ,��ʾ˫������
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_StartTalk(void);


/*****************************************************************************/
//  Description : MMIVT_REF_StopTalk
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:�˳�dialģʽ
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_StopTalk(void);


/*****************************************************************************/
//  Description : MMIVT_REF_SetDispArea
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:������Ƶ��ʾģʽ
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_SetDispArea(MMIVT_IMAGE_ROTATE_STATUS_E rotate_status,  MMIVT_DISP_MODE_E disp_mode);

/*****************************************************************************/
//  Description : MMIVT_REF_HideVideo
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:������Ƶ
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_HideVideo(MMIVT_DISP_MODE_E disp_mode);

/*****************************************************************************/
//  Description : MMIVT_REF_ReplaceStart
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:��ʼ�滻
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_ReplaceStart(MMISET_VT_REPLACE_TYPE_E replace_type, BOOLEAN is_default);

/*****************************************************************************/
//  Description : MMIVT_REF_ReplaceStop
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:ֹͣ�滻
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_ReplaceStop(MMISET_VT_REPLACE_TYPE_E replace_type);

/*****************************************************************************/
//  Description : MMIVT_REF_RecordStart
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:��ʼ¼��
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_RecordStart(MMIVT_RECORD_TYPE_E record_type, MMI_STRING_T* record_path_str_ptr);

/*****************************************************************************/
//  Description : MMIVT_REF_RecordStop
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:����¼��
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_RecordStop(void);

/*****************************************************************************/
//  Description : MMIVT_InitSensorParam
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:��ʼ��sensor����
/*****************************************************************************/
PUBLIC void MMIVT_REF_InitSensorParam(void);

/*****************************************************************************/
//  Description : MMIVT_REF_SetLcdBrightness
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:����LCD ����
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_SetLcdBrightness(MMIVT_VIDEO_LEVEL_E lcd_brightness_level);

/*****************************************************************************/
//  Description : MMIVT_REF_SetContrast
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:���ñ���ͼ��Աȶ�
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_SetContrast(MMIVT_VIDEO_LEVEL_E contrast_level);

/*****************************************************************************/
//  Description : MMIVT_REF_SetBrightness
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:���ñ���ͼ������
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_SetBrightness(MMIVT_VIDEO_LEVEL_E brightness_level);

/*****************************************************************************/
//  Description : MMIVT_REF_RotateLocal
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:��ת����ͼ��
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_RotateLocal(MMIVT_IMAGE_ROTATE_STATUS_E rotate_status,  MMIVT_DISP_MODE_E disp_mode);

/*****************************************************************************/
//  Description : MMIVT_REF_SwitchCamera
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:�л���������ͷ
/*****************************************************************************/
PUBLIC MMIVT_REF_RET_E MMIVT_REF_SwitchCamera(MMIVT_CAMERA_ID_E camera_id);


     
#ifdef   __cplusplus
}
#endif

#endif //VT_SUPPORT

#endif  //_MMIVT_KERNEL_REFADAPTER_H_