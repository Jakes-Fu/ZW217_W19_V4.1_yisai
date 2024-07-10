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
// 在乱码或者某些特定字符下，一行的最大字节数会达到120
//#define MMIEBOOK_ONE_LINE_MAX_LEN                   64
#define MMIEBOOK_ONE_LINE_MAX_LEN                   120    //一行文字最多几个字节,(英文字符可以达到这个数字44)
//end: modified by zgy. cr96485.  2007/09/29
#define MMIEBOOK_BOOKMARK_NAME_LEN                  10   //电子书签列表中显示内容最大长度,双字节


#define MMIEBOOK_TASK_STACK_SIZE                    4096 //任务栈大小
#define MMIEBOOK_TASK_QUEUE_NUM                     8   //任务消息队列大小
//任务优先级,跟IDLE任务一样，如果再高，对按键铃声会有影响，但设成跟idle一样，不知道会不会有问题
#define MMIEBOOK_TASK_PRIORITY                      SCI_PRIORITY_IDLE//29 //任务优先级应最低，以不影响铃声播放
#define MMIEBOOK_SEGTAB_TASK_PRIORITY				SCI_PRIORITY_IDLE
#define MMIEBOOK_TASK_SLEEP_TIME                    5//ms，任务挂起时间

#define MMIEBOOK_TXT_DIRECTION_V 0
#define MMIEBOOK_TXT_DIRECTION_H 1

//电子书书签名字信息
typedef struct MMIEBOOK_BOOKMARK_NAME_S
{
    wchar  name_wstr[MMIEBOOK_BOOKMARK_NAME_LEN + 1];//书签名字,存放该页首行文本，用于书签列表的显示，UCS2
    uint16  name_len;                       //书签名字长度
} MMIEBOOK_BOOKMARK_NAME_T;

/*电子书buffer段内分行信息，段与段之间允许数据重叠*/
typedef struct MMIEBOOK_FILE_SEGMENT_S
{
    int32	this_seg_offset;	    //该段第一个字节在电子书中的偏移量
    int32 	next_seg_offset;	    //需要读取的下一段在电子书中的偏移量
    wchar*	ucs2_str_ptr;
    int16*	line_offset_tab_ptr;	//每一页相对于ucs2_str_ptr的偏移量
    int32	ucs2_str_len;
    int32 	total_line_num;         //line_offset_tab_len;//该段中总的行数
}MMIEBOOK_FILE_SEGMENT_T;//其中原始的GB编码内容可以在使用的当前分配
  
//当前阅读的信息
typedef struct MMIEBOOK_SHOW_INFO_S
{
    MMIEBOOK_FILE_SEGMENT_T*	    pre_show_txt_ptr;	//指向上一段的显示内容      
    MMIEBOOK_FILE_SEGMENT_T*	    cur_show_txt_ptr;	//当前显示内容在哪个段 
    MMIEBOOK_FILE_SEGMENT_T*	    next_show_txt_ptr;	//指向下一段的显示内容  
    
    int16                          cur_line;		    //当前起始显示内容在segment中的行数
    int16                          one_page_lines;	    //一页的行数  
    int16                          encode_type;        //(可选的)电子书的编码类型
    int16                          percent;            //0~100
    int32                          cur_read_offset;      //记录当前阅读的位置,以便下次打开电子书,找到该位置
    MMIEBOOK_BOOKMARK_T            bookmark_info;     //书签信息结构体
}MMIEBOOK_SHOW_INFO_T;//类似与之前的MMIEBOOK_FILE_DETAIL_T

//当前显示rect范围信息
typedef struct MMIEBOOK_SHOW_RECT_INFO_S
{
    GUI_FONT_T      font;
    uint16          txt_show_left;
    uint16          txt_show_right;
    uint16          txt_show_top;
    uint16          txt_show_bottom;
    uint16          txt_show_line_space;
    uint16          txt_show_char_space;

}MMIEBOOK_SHOW_RECT_INFO_T;//类似与之前的MMIEBOOK_FILE_DETAIL_T

//窗口或任务之间的消息类型
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

//文件信息保存结果
typedef enum
{
    MMIEBOOK_CUR_FILE_INFO_MISSED,
    MMIEBOOK_ALL_FILE_INFO_MISSED,
    MMIEBOOK_FILE_INFO_MISSED_TYPE_MAX
} MMIEBOOK_FILE_INFO_MISSED_TYPE_E;

extern PUBLIC BLOCK_ID                      g_ebook_task_id;//创建详情文件任务ID
extern PUBLIC MMIEBOOK_FILE_BRIEF_T *       g_ebook_show_file_brief_ptr ;       //当前阅读的电子书简略信息指针
extern PUBLIC MMIEBOOK_BOOKMARK_NAME_T *    g_ebook_bookmark_name_tab;//当前阅读的电子书书签名字信息

/*****************************************************************************/
//  Description : 初始化电子书资源
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_AllocEbookTaskRes(void);

/*****************************************************************************/
//  Description : 释放电子书占用的资源
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_FreeEbookTaskRes(void);

/*****************************************************************************/
//  Description : MMI发送消息给ebook task
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
//  Description : 取得显示文本
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/ 
PUBLIC void MMIEBOOK_GetShowTxt(
                             MMI_STRING_T *mmi_string_ptr//OUT:
                             );

/*****************************************************************************/
//  Description : 发送ebook task消息给MMI窗口
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
//  Description : 电子书任务是否忙
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
PUBLIC BOOLEAN MMIEBOOK_IsInLastPage(void);//判断是否已是电子书最后一页

/*****************************************************************************/
//  Description : 添加当前页的首行作为书签
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:FALSE->书签满
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_AddBookmark(void);

/*****************************************************************************/
//  Description : 删除选择的书签
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIEBOOK_DeleteBookmark(
								 uint16 cur_select//IN: 书签列表窗口中的位置
								 );


/*****************************************************************************/
//  Description : 删除所有书签
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIEBOOK_DeleteAllBookmark(void);

/*****************************************************************************/
//  Description : 获得电子书显示信息
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/    
PUBLIC MMIEBOOK_SHOW_INFO_T* MMIEBOOK_GetShowInfo( void );

/*****************************************************************************/
//  Description : 获得电子书显示区域信息
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
//  Description :  打开电子书时读取书签和上次阅读的位置信息
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
//  Return:TRUE:书签信息文件已存在 FALSE:书签信息文件不存在
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
//  Description :  电子书相关消息处理函数
//  Global resource dependence : 
//  Author: ying.xu
//  Note:	去掉电子书独立task
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

