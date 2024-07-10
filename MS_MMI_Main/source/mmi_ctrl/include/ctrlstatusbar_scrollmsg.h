/*! @file ctrlstatusbar_scrollmsg.h
@brief ���ļ�������statusbar_data�ؼ��Ķ���ӿں���������
@author bin.ji
@version 1.0
@date 8/5/2008
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note 09/2012     xiyuan.ma      Create
*******************************************************************************/

/*! @page page_StatusBar_data StatusBar_data

(����)

*******************************************************************************/

/*! @addtogroup statusbar_dataGrp StatusBar_data
@brief statusbar_dataģ��
@details ���ļ������⿪�ţ�����app��Ӧ�õ���ctrlwin.h�еķ�װ�ӿ�
@{
*******************************************************************************/

#ifndef _CTRL_STATUSBAR_SCROLLMSG_H_
#define _CTRL_STATUSBAR_SCROLLMSG_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/
#include "guictrl.h"
#include "guicommon.h"
#include "guifont.h"
#include "mmitheme_statusbar.h"

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


/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
#define GUISTBSCROLLMSG_SCROLL_FRAME_NUM        5       //����Ч��֡��
#define GUISTBSCROLLMSG_MAX_SCROLL_ITEM_NUM     20      //��������Ϣ��Ŀ
#define GUISTBSCROLLMSG_TEXT_WIDTH              DP2PX_VALUE(200)     //�ı�������

/*! @struct GUISTBSCROLLMSG_SCROLL_STATE_E
@brief ��Ҫ������ʾ������
*/
typedef enum
{
    GUISTBSCROLLMSG_SCROLL_NONE = 0, /*!< ��*/
    GUISTBSCROLLMSG_SCROLL_START,  	/*!< ��ʼ����*/
    GUISTBSCROLLMSG_SCROLL_RUNNING,  /*!< ���ڹ���*/
    GUISTBSCROLLMSG_SCROLL_END		/*!< ��������*/
} GUISTBSCROLLMSG_SCROLL_STATE_E;

/*! @struct GUISTBSCROLLMSG_SCROLL_ITEM_E
@brief ��Ҫ������ʾ������
*/
typedef enum
{
    GUISTBSCROLLMSG_SCROLL_ITEM_NONE = 0,     /*!< ��*/
    GUISTBSCROLLMSG_SCROLL_ITEM_NEW_MESSAGE,     /*!< δ������*/
    GUISTBSCROLLMSG_SCROLL_ITEM_NEW_SMSCB,     /*!< С���㲥*/
    GUISTBSCROLLMSG_SCROLL_ITEM_ALARM,  	        /*!< ����*/
    GUISTBSCROLLMSG_SCROLL_ITEM_MISSED_CALL,       /*!< δ������*/
    GUISTBSCROLLMSG_SCROLL_ITEM_NEW_MMS,       /*!< δ������*/
    GUISTBSCROLLMSG_SCROLL_ITEM_SCHEDULE,       /*!< �ճ�*/

    GUISTBSCROLLMSG_SCROLL_ITEM_MAX		        /*!< �������*/
} GUISTBSCROLLMSG_SCROLL_ITEM_E;

/*! @struct GUISTBSCROLLMSG_SCROLL_ITEM_T
@brief ��Ҫ������ʾ������
*/
typedef struct
{
    GUISTBSCROLLMSG_SCROLL_ITEM_E type;
    BOOLEAN is_valid;
    MMI_IMAGE_ID_T icon_id;
    MMI_STRING_T string_info;
} GUISTBSCROLLMSG_SCROLL_ITEM_T;

/*! @struct GUISTBSCROLLMSG_SCROLL_MESSAGE_T
@brief ��Ҫ������ʾ������
*/
typedef struct
{
    GUISTBSCROLLMSG_SCROLL_STATE_E scroll_state;
    uint32 cur_frame;       //��ǰ֡
    uint32 cur_item_index;  //��ǰitem index
    uint32 cur_str_pos;     //��ǰ�ַ���λ��
    GUISTBSCROLLMSG_SCROLL_ITEM_T item[GUISTBSCROLLMSG_MAX_SCROLL_ITEM_NUM];
} GUISTBSCROLLMSG_SCROLL_MESSAGE_T;


/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
#ifdef GUIF_STATUSBAR_SCROLL
/*****************************************************************************/
//  Description : �õ���ǰ���ڼ���֡
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC uint32 CTRLSTBSCROLLMSG_GetCurFrame (
    void
);


/*****************************************************************************/
//  Description : �õ���ǰ����״̬
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC GUISTBSCROLLMSG_SCROLL_STATE_E CTRLSTBSCROLLMSG_GetCurScrollState (
    void
);


/*****************************************************************************/
//  Description : ��ʾ��һ��
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTBSCROLLMSG_ConsumeOneLine (
    void
);


/*****************************************************************************/
//  Description : ��ʾ��һ֡
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTBSCROLLMSG_ConsumeOneFrame (
    void
);


/*****************************************************************************/
//  Description : ���һ��������Ϣ
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBSCROLLMSG_AppendScrollItem (
    GUISTBSCROLLMSG_SCROLL_ITEM_T *item_ptr
);


/*****************************************************************************/
//  Description : �õ���ǰ����ʾ����
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBSCROLLMSG_GetCurLine (
    GUISTBSCROLLMSG_SCROLL_ITEM_T *item_ptr
);


/*****************************************************************************/
//  Description : �õ���һ����ʾ����
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBSCROLLMSG_GetNextLine (
    GUISTBSCROLLMSG_SCROLL_ITEM_T *item_ptr
);


/*****************************************************************************/
//  Description : �Ƿ���Ӧ��ֹͣ
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTBSCROLLMSG_SetIsValid (
    GUISTBSCROLLMSG_SCROLL_ITEM_E type,
    BOOLEAN is_valid
);
#endif


/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif // _CTRL_STATUSBAR_DATA_H_


/*@}*/
