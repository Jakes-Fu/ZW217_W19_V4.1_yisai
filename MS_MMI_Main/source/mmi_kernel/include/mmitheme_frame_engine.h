/***************************************************************************
** File Name:      MySample_image.h                                        *
** Author:         hua.fang                                                *
** Date:           07/16/2011                                              *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 07/2010        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _MMITHEME_ANIMATION_ENGINE_H_
#define _MMITHEME_ANIMATION_ENGINE_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "sci_types.h"
#include "cafcontrol.h"
#include "mmk_app.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
typedef void* ANIM_HANDLE_T;                                    // �������

// ��������
typedef enum
{
    MMIANIM_TYPE_BLANK  = 0,
    MMIANIM_TYPE_SIZE,
    MMIANIM_TYPE_MAX,
} MMIANIM_TYPE_E;

// ������Ϣ
typedef struct
{
    BOOLEAN                         is_stop_after_end_frame;    // �������������Ƿ���Ҫ�Ӷ���������ժ��
    uint16                          start_frame_index;          // ��ʼ֡
    uint16                          end_frame_index;            // ����֡
} MMIANIM_INFO_T;

// blank anim param
typedef struct
{
    uint32                          user_data;                  // �û�����
} BLANK_ANIM_CALLBACK_PARAM_T;

// size anim param
typedef struct
{
    GUI_SIZE_T                      start_size;                 // ��ʼλ�ô�С
    GUI_SIZE_T                      end_size;                   // ����λ�ô�С
    MMK_DAMP_PLAY_TYPE_E            play_type;                  // �仯����
} SIZE_ANIM_PARAM_T;

typedef struct
{
    uint32                          user_data;                  // �û�����
    GUI_SIZE_T                      cur_size;                   // ��ǰ�ĳߴ�
} SIZE_ANIM_CALLBACK_PARAM_T;

// param in MMIFRAME_AddAnim
typedef union
{
    SIZE_ANIM_PARAM_T               size_anim;                  // �ߴ綯���Ĳ���
} MMIANIM_ADD_PARAM_U;

// param in DO_ANIM_FUNC
typedef union
{
    BLANK_ANIM_CALLBACK_PARAM_T     blank_anim;                 // �հ׶����Ļص�����
    SIZE_ANIM_CALLBACK_PARAM_T      size_anim;                  // �ߴ綯���Ļص�����
} MMIANIM_CALLBACK_PARAM_U;

// callback
typedef void (*DO_ANIM_FUNC)(ANIM_HANDLE_T anim_handle, uint16 frame_index, MMIANIM_CALLBACK_PARAM_U *callback_param);  // ����һ֡�Ķ����ص�
typedef void (*ANIM_DONE_FUNC)(ANIM_HANDLE_T anim_handle, uint32 user_data);                                            // ���������Ļص�
typedef void (*ANIM_RESET_FUNC)(ANIM_HANDLE_T anim_handle);                                                             // �������õĻص�

// ��������Ļص�����
typedef struct
{
    DO_ANIM_FUNC                    DoAnimFunc;                 // ������
    ANIM_DONE_FUNC                  AnimDoneFunc;               // ��������
    ANIM_RESET_FUNC                 AnimResetFunc;              // ����
} MMIANIM_FUNC_T;

// ���������ṹ
typedef struct
{
    MMIANIM_TYPE_E                  anim_type;                  // ��������
    uint32                          user_data;                  // �û�����

    MMIANIM_FUNC_T                  anim_func;                  // ���ݲ�ͬ�����Ͷ���ͬ
    MMIANIM_ADD_PARAM_U             add_param;                  // ��Ը��������Ĳ���
} MMIANIM_PARAM_T;

// ��ʼ�����Ĳ���
typedef struct
{
    BOOLEAN                         is_async;                   // �Ƿ��첽
    uint16                          frame_count;                // ��֡��
    uint32                          period;                     // ÿ֮֡��ļ��(��λ:ms)
} MMIANIM_START_PARAM_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : reset frame engine
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIFRAME_Reset(void);

/*****************************************************************************/
//  Description : start animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFRAME_Start(
                              MMIANIM_START_PARAM_T     *start_param_ptr
                              );

/*****************************************************************************/
//  Description : stop animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIFRAME_Stop(void);

/*****************************************************************************/
//  Description : remove animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIFRAME_RemoveAnim(
                                ANIM_HANDLE_T       anim_handle
                                );

/*****************************************************************************/
//  Description : add animation
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC ANIM_HANDLE_T MMIFRAME_AddAnim(
                                      MMIANIM_PARAM_T       *anim_param_ptr
                                      );

/*****************************************************************************/
//  Description : set animation param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFRAME_SetAnimParam(
                                     ANIM_HANDLE_T          anim_handle,
                                     MMIANIM_PARAM_T        *anim_param_ptr
                                     );

/*****************************************************************************/
//  Description : get animation param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMIANIM_PARAM_T MMIFRAME_GetAnimParam(
                                             ANIM_HANDLE_T          anim_handle
                                             );

/*****************************************************************************/
//  Description : create start and end frame index
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIANIM_SetInfo(
                            ANIM_HANDLE_T       anim_handle,
                            MMIANIM_INFO_T      *anim_info_ptr
                            );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif  // _MMITHEME_ANIMATION_ENGINE_H_
