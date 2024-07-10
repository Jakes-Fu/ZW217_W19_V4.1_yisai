/*************************************************************************
** File Name:      mmirecord_export.h                                           *
** Author:         bin.ji                                                *
** Date:           3/2/2005                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.    *
** Description:     This file declare the function about nv              *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 3/2/2005     bin.ji        Create.                              *
*************************************************************************/

#ifndef _MMIRECORD_H_
#define _MMIRECORD_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_text.h"
#include "mmi_id.h"
#include "mmk_type.h"
#include "mmi_filemgr.h"
#include "mmisrvaud_api.h"
#include "mmi_string.h"

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
typedef enum
{
    MMIRECORD_RESULT_SUCCESS,   //�������óɹ�
    MMIRECORD_RESULT_ERROR,     //�������ô���
    MMIRECORD_RESULT_NO_SPACE,   //�ռ䲻��
	MMIRECORD_RESULT_NO_EXIST,
	MMIRECORD_RESULT_MAX
}MMIRECORD_RESULT_E;    //�������ý��

typedef enum
{
   MMIRECORD_WORK_SUCCESS,  //�ɹ�
   MMIRECORD_WORK_ERROR,     //��������
   MMIRECORD_WORK_ERROR_MAX
}MMIRECORD_WORK_ERROR_E;

typedef enum
{
    MMIRECORD_WORK_FORMAT_WAV = 0x01,
    MMIRECORD_WORK_FORMAT_AMR = 0x02,
    MMIRECORD_WORK_FORMAT_MP3 = 0x04,
    MMIRECORD_WORK_FORMAT_MAX
} MMIRECORD_WORK_FORMAT_E;



typedef struct
{
    uint16       file_name_len;//�ļ�·������ 
    wchar        *file_name;//�ļ����ƣ�����·��)
}MMIRECORD_WORK_DATA_INFO_T;

typedef  void(*MMIRECORD_WORK_CALLBACK)( MMIRECORD_WORK_ERROR_E error_ret, MMIRECORD_WORK_DATA_INFO_T  *data_info_ptr);

typedef struct
{
    uint32      max_file_time;                      //�¼��ʱ�䣬ʵ��ʱ�����С�ڵ��ڴ�ֵ
    uint32      max_file_size;                       //����ļ��ߴ磬ʵ�ʳߴ����С�ڵ��ڴ�ֵ
    uint32      record_support_type;           // ����ģ����֧�ֵ�¼����ʽ
    MMIRECORD_WORK_CALLBACK     work_callback; //����ģ��Ļص�����
}MMIRECORD_WORK_PARAM_T;

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/ 


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : recorder exit
//  Global resource dependence : none
//  Author: haiyanghu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIRECORD_Exit(void);


/*****************************************************************************/
//  Description : whether record is open
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIRECORD_IsOpened(void);

/*****************************************************************************/
//  FUNCTION:      init record module
//  Description :   
//  Global resource dependence : 
//  Author:        haiyang.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIRECORD_InitModule(void);

/*****************************************************************************/
//  FUNCTION:      MMIAPIRECORD_Init
//  Description :
//  Global resource dependence : 
//  Author:        haiyang.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIRECORD_Init(void);


/*****************************************************************************/
//  Description : open record main win
//  Global resource dependence : none
//  Author: haiyanghu
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_WORK_ERROR_E MMIAPIRECORD_OpenMainWin(MMIRECORD_WORK_PARAM_T *work_param_ptr);


/*****************************************************************************/
//  Description : Get record storage setting in nv.as APIs.
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPIRECORD_GetRecordStorage(void);

/*****************************************************************************/
//  Description : stop record and stop play
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIRECORD_StopRecordOrPlay(void);

/*****************************************************************************/
//  Description : whether is recoding  
//  Global resource dependence : none
//  Author: louis.wei
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIRECORD_IsRecording(void);

/*****************************************************************************/
//  Description : Close Menu
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIRECORD_CloseMenu(void);

/*****************************************************************************/
//  Description : MMIAPIRECORD_GetRecordFmt
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RECORD_FMT_E MMIAPIRECORD_GetRecordFmt(void);

#endif

