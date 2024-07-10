/****************************************************************************
** File Name:      mmiebook_task.h                            
** Author:         songbin.zeng                                                        
** Date:           2006/06/9 
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.       
** Description:    This file is used to ebook server
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 06/6/2006         songbin.zeng          Create
** 
****************************************************************************/

#ifndef _MMIEBOOK_TASK_H_
#define _MMIEBOOK_TASK_H_

#ifdef EBOOK_SUPPORT
#include "mmi_module.h"
#include "mmiebook_position.h"


//begin: modified by zgy. cr96485.  2007/09/29
// ���������ĳЩ�ض��ַ��£�һ�е�����ֽ�����ﵽ120
//#define MMIEBOOK_ONE_LINE_MAX_LEN                   64
#define MMIEBOOK_ONE_LINE_MAX_LEN                   120    //һ��������༸���ֽ�,(Ӣ���ַ����Դﵽ�������44)
//end: modified by zgy. cr96485.  2007/09/29
#define MMIEBOOK_BOOKMARK_NAME_LEN                  10   //������ǩ�б�����ʾ������󳤶�,˫�ֽ�


#define MMIEBOOK_TASK_STACK_SIZE                    4096 //����ջ��С
#define MMIEBOOK_TASK_QUEUE_NUM                     8   //������Ϣ���д�С
//�������ȼ�,��IDLE����һ��������ٸߣ��԰�����������Ӱ�죬����ɸ�idleһ������֪���᲻��������
#define MMIEBOOK_TASK_PRIORITY                      SCI_PRIORITY_IDLE//29 //�������ȼ�Ӧ��ͣ��Բ�Ӱ����������
#define MMIEBOOK_SEGTAB_TASK_PRIORITY				SCI_PRIORITY_IDLE
#define MMIEBOOK_TASK_SLEEP_TIME                    5//ms���������ʱ��

#define MMIEBOOK_TXT_DIRECTION_V 0
#define MMIEBOOK_TXT_DIRECTION_H 1

//��������ǩ������Ϣ
typedef struct MMIEBOOK_BOOKMARK_NAME_S
{
    wchar  name_wstr[MMIEBOOK_BOOKMARK_NAME_LEN + 1];//��ǩ����,��Ÿ�ҳ�����ı���������ǩ�б����ʾ��UCS2
    uint16  name_len;                       //��ǩ���ֳ���
} MMIEBOOK_BOOKMARK_NAME_T;

/*������buffer���ڷ�����Ϣ�������֮�����������ص�*/
typedef struct MMIEBOOK_FILE_SEGMENT_S
{
    int32	this_seg_offset;	    //�öε�һ���ֽ��ڵ������е�ƫ����
    int32 	next_seg_offset;	    //��Ҫ��ȡ����һ���ڵ������е�ƫ����
    wchar*	ucs2_str_ptr;
    int16*	line_offset_tab_ptr;	//ÿһҳ�����ucs2_str_ptr��ƫ����
    int32	ucs2_str_len;
    int32 	total_line_num;         //line_offset_tab_len;//�ö����ܵ�����
}MMIEBOOK_FILE_SEGMENT_T;//����ԭʼ��GB�������ݿ�����ʹ�õĵ�ǰ����
  
//��ǰ�Ķ�����Ϣ
typedef struct MMIEBOOK_SHOW_INFO_S
{
    MMIEBOOK_FILE_SEGMENT_T*	    pre_show_txt_ptr;	//ָ����һ�ε���ʾ����      
    MMIEBOOK_FILE_SEGMENT_T*	    cur_show_txt_ptr;	//��ǰ��ʾ�������ĸ��� 
    MMIEBOOK_FILE_SEGMENT_T*	    next_show_txt_ptr;	//ָ����һ�ε���ʾ����  
    
    int16                          cur_line;		    //��ǰ��ʼ��ʾ������segment�е�����
    int16                          one_page_lines;	    //һҳ������  
    int16                          encode_type;        //(��ѡ��)������ı�������
    int16                          percent;            //0~100
    int32                          cur_read_offset;      //��¼��ǰ�Ķ���λ��,�Ա��´δ򿪵�����,�ҵ���λ��
    MMIEBOOK_BOOKMARK_T            bookmark_info;     //��ǩ��Ϣ�ṹ��
}MMIEBOOK_SHOW_INFO_T;//������֮ǰ��MMIEBOOK_FILE_DETAIL_T

//��ǰ��ʾrect��Χ��Ϣ
typedef struct MMIEBOOK_SHOW_RECT_INFO_S
{
    GUI_FONT_T      font;
    uint16          txt_show_left;
    uint16          txt_show_right;
    uint16          txt_show_top;
    uint16          txt_show_bottom;
    uint16          txt_show_line_space;
    uint16          txt_show_char_space;

}MMIEBOOK_SHOW_RECT_INFO_T;//������֮ǰ��MMIEBOOK_FILE_DETAIL_T

//���ڻ�����֮�����Ϣ����
typedef enum
{
    MMIEBOOK_MSG_MIN = 0x204,
    MMIEBOOK_MSG_LOAD_BOOKLIST = MMIEBOOK_MSG_MIN,//1 2
    MMIEBOOK_MSG_TASK_EXIT,//1
    MMIEBOOK_MSG_SHOW_TXT_RDY,//2
    MMIEBOOK_MSG_SHOW_TXT_PROCESS,//2,3
    MMIEBOOK_MSG_SEG_TAB_RDY,//3
    MMIEBOOK_MSG_SHOW_WIN_OPT,//1,2
    MMIEBOOK_MSG_SHOW_WIN_CLOSE,//1,2
    MMIEBOOK_MSG_SET_PAGE,//1,3
    // 	MMIEBOOK_MSG_PAGE_CUR,//1,3
    // 	MMIEBOOK_MSG_PAGE_UP,
    // 	MMIEBOOK_MSG_PAGE_DOWN,
    MMIEBOOK_MSG_DELE,//1,2
    MMIEBOOK_MSG_DELALL,//1,2
    MMIEBOOK_MSG_CREATE_DETAIL_FILE,//1,2
    MMIEBOOK_MSG_SHOW_TXT_WIN_OPEN,//1
    MMIEBOOK_MSG_OPEN_SHOW_FILE_FAIL,//2
    MMIEBOOK_MSG_MAKE_TEMPORARY_SEG_TAB,//3
    MMIEBOOK_MSG_MAKE_SEG_TAB,//3
    MMIEBOOK_MSG_INIT_DETAIL_FILE,//3
    MMIEBOOK_MSG_CREATE_DETAIL_FILE_FAIL,//2	
    MMIEBOOK_MSG_LOAD_TXT_BRIEF,	//1,2
    MMIEBOOK_MSG_BOOKMARK,//1,2
    MMIEBOOK_MSG_ERROR_EXIT,//1 

    MMIEBOOK_MSG_TURNTO_PERCENT,
    MMIEBOOK_MSG_TURNTO_OFFSET,    
    MMIEBOOK_MSG_LAYOUT_PRE_SEGMENT,
    MMIEBOOK_MSG_LAYOUT_NEXT_SEGMENT,
    MMIEBOOK_MSG_SCREEN_SWITCH,
    MMIEBOOK_MSG_FONT_SIZE_CHANGE,
    MMIEBOOK_MSG_FULL_SCREEN_SWITCH,
    MMIEBOOK_MSG_SLIDE_SET_PAGE,
    MMIEBOOK_MSG_SLIDE_LAYOUT_PRE_SEGMENT,
    MMIEBOOK_MSG_SLIDE_LAYOUT_NEXT_SEGMENT,
#ifdef TTS_SUPPORT
    MMIEBOOK_MSG_TTS_PLAY,
#endif    
    MMIEBOOK_MSG_MAX
} MMIEBOOK_MSG_TYPE_E;

typedef struct
{
    SIGNAL_VARS
	int32 data;
} MMIEBOOK_SIG_T;

//�ļ���Ϣ������
typedef enum
{
    MMIEBOOK_CUR_FILE_INFO_MISSED,
    MMIEBOOK_ALL_FILE_INFO_MISSED,
    MMIEBOOK_FILE_INFO_MISSED_TYPE_MAX
} MMIEBOOK_FILE_INFO_MISSED_TYPE_E;

extern PUBLIC BLOCK_ID                      g_ebook_task_id;//���������ļ�����ID
extern PUBLIC MMIEBOOK_FILE_BRIEF_T *       g_ebook_show_file_brief_ptr ;       //��ǰ�Ķ��ĵ����������Ϣָ��
extern PUBLIC MMIEBOOK_BOOKMARK_NAME_T *    g_ebook_bookmark_name_tab;//��ǰ�Ķ��ĵ�������ǩ������Ϣ

/*****************************************************************************/
//  Description : ��ʼ����������Դ
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_AllocEbookTaskRes(void);

/*****************************************************************************/
//  Description : �ͷŵ�����ռ�õ���Դ
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_FreeEbookTaskRes(void);

/*****************************************************************************/
//  Description : MMI������Ϣ��ebook task
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIEBOOK_SendMsgtoTask(
                                      MMIEBOOK_MSG_TYPE_E type, 
                                      BLOCK_ID task_id,
                                      int32 data
                                      );
                                   
/*****************************************************************************/
//  Description : ȡ����ʾ�ı�
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/ 
PUBLIC void MMIEBOOK_GetShowTxt(
                             MMI_STRING_T *mmi_string_ptr//OUT:
                             );

/*****************************************************************************/
//  Description : ����ebook task��Ϣ��MMI����
//  Global resource dependence : MMIEBOOK_SetTaskBusy
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIEBOOK_SendMsgtoMMI(
                           MMI_HANDLE_T        handle,         // the window id
                           MMI_MESSAGE_ID_E    msg_id,         //the identify of message
                           DPARAM              param_ptr,      //the param of the message
                           uint32              size_of_param   //the size of the param
                           );

/*****************************************************************************/
//  Description : �����������Ƿ�æ
//  Global resource dependence : MMIEBOOK_IsTaskBusy
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsTaskBusy( void );

/*****************************************************************************/
//  Description : ShowCurPercent
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_ShowCurPercent( 
		MMI_CTRL_ID_T ctrl_id
		
);

/*****************************************************************************/
//  Description : MMIEbook_GetFont
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
PUBLIC GUI_FONT_T *MMIEbook_GetFont(void);

/*****************************************************************************/
//  Description : ShowCurPercent
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_SetPercent( 
                                uint32 percent
                                );

/*****************************************************************************/
//  Description : IsLayoutLastPageOffset
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsInLastPage(void);//�ж��Ƿ����ǵ��������һҳ

/*****************************************************************************/
//  Description : ��ӵ�ǰҳ��������Ϊ��ǩ
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:FALSE->��ǩ��
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_AddBookmark(void);

/*****************************************************************************/
//  Description : ɾ��ѡ�����ǩ
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIEBOOK_DeleteBookmark(
								 uint16 cur_select//IN: ��ǩ�б����е�λ��
								 );


/*****************************************************************************/
//  Description : ɾ��������ǩ
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIEBOOK_DeleteAllBookmark(void);

/*****************************************************************************/
//  Description : ��õ�������ʾ��Ϣ
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/    
PUBLIC MMIEBOOK_SHOW_INFO_T* MMIEBOOK_GetShowInfo( void );

/*****************************************************************************/
//  Description : ��õ�������ʾ������Ϣ
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIEBOOK_SHOW_RECT_INFO_T* MMIEBOOK_GetEbookShowRectInfo( void );

/*****************************************************************************/
//  Description : MMIEBOOK_IsTurnLineOrPage
//  Global resource dependence :
//   Author: renyi.hu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsTurnLineOrPage(
									MMI_MESSAGE_ID_E msg_id
                                    );
/*****************************************************************************/
//  Description :  �򿪵�����ʱ��ȡ��ǩ���ϴ��Ķ���λ����Ϣ
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
//  Return:TRUE:��ǩ��Ϣ�ļ��Ѵ��� FALSE:��ǩ��Ϣ�ļ�������
/*****************************************************************************/
PUBLIC uint16 MMIEBOOK_ConvertWstrOffsetToOtherCodeOffset(     
            wchar*	ucs2_str_ptr,
            uint16	ucs2_str_offset
            );

/*****************************************************************************/
//  Description : MMIEBOOK_GetTxtDirection
//  Global resource dependence :
//   Author: renyi.hu
//  Note:   
/*****************************************************************************/
PUBLIC uint8 MMIEBOOK_GetTxtDirection(void);

/*****************************************************************************/
//  Description :  �����������Ϣ������
//  Global resource dependence : 
//  Author: ying.xu
//  Note:	ȥ�����������task
//  Return:
/*****************************************************************************/
void MMIEbook_HandleTaskMessage( MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : IsLayoutLastPageOffset
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsFirstPage(void);

#ifdef MMIEBOOK_PAGESLIDE_SUPPORT
/*****************************************************************************/
//  Description : MMIEBOOK_TurnLineOrPage
//  Global resource dependence :
//   Author: reni.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIEBOOK_GetNextPageForEffect(SE_PAGE_TURN_MODE_E turn_mode);
#endif//MMI_PDA_SUPPORT

#endif//EBOOK_SUPPORT
#endif

