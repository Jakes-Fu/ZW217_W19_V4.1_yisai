/****************************************************************************
** File Name:      mmiebook_task.c                              
** Author:         songbin.zeng                                                        
** Date:           2006/06/9 
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to ebook server
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 06/6/2006         songbin.zeng          Create
** 
****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_ebook_trc.h"
#ifdef EBOOK_SUPPORT
#include "window_parse.h" 
#include "mmidisplay_data.h"
#include "guitext.h"
#include "guilabel.h"
#include "guifont.h"
#include "gui_ucs2b_converter.h"

#include "mmifmm_export.h"
#include "mmi_nv.h"
#include "mmi_modu_main.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiudisk_export.h"
#include "mmi_theme.h"
#include "mmitheme_text.h"
#ifdef TTS_SUPPORT
#include "mmitts_export.h"
#endif
#include "mmiebook_id.h"
#include "mmiebook_file.h"
//#include "mmiebook.h"
#include "mmiebook_internal.h"
#include "mmiebook_task.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIEBOOK_SEG_BUF_UNIT_LEN	    ( 2048 )		//一次读取几个字节的文本（一段），GB
//here, MMIEBOOK_SEG_BUF_UNIT_LEN must ensure that the lines of one segment > 2* lines of one page 

/**--------------------------------------------------------------------------*
**                         STRUCT DEFINITION                                *
**--------------------------------------------------------------------------*/
typedef uint32 MMIEBOOK_MSG_STATE_T;
/* item style state */
#define MMIEBOOK_MSG_STATE_BUSY            0x000000001   //Busy msg,which can change ebook task to be busy (external to internal msg)
#define MMIEBOOK_MSG_STATE_IDLE            0x000000002   //idle msg,which can change ebook task to be idle (internal to external msg)
#define MMIEBOOK_MSG_STATE_NOT_CARE_BUSY   0x000000004   //free msg,which not care busy

typedef enum
{
    MMIEBOOK_MAKESEG_FAILED,    //失败
    MMIEBOOK_MAKESEG_SUCCESSED, //成功，并引起段表的变化
    MMIEBOOK_MAKESEG_UNCHANGED,  //成功，但没引起段表变化
    MMIEBOOK_MAKESEG_ENDMARK      //遇到结束符'\0',则就解析到结束符之前的内容
} MMIEBOOK_MAKESEG_RESULT_E;//电子书分段计算段表信息返回结果

typedef struct MMIEBOOK_MSG_INFO_S
{
    MMIEBOOK_MSG_TYPE_E	msg_id;
    MMIEBOOK_MSG_STATE_T	msg_state;
}MMIEBOOK_MSG_INFO_T;//电子书消息状态信息

/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//跳转百分比的时候，前后共load三段，当中的一段作为当前显示的段，前后的段作为预load段，以避免上下翻页时，要重新load段导致的延迟

LOCAL MMIEBOOK_FILE_SEGMENT_T s_ebook_seg_part1 = {0};
LOCAL MMIEBOOK_FILE_SEGMENT_T s_ebook_seg_part2 = {0};


LOCAL MMIEBOOK_SHOW_INFO_T s_ebook_show_info = {0};//电子书显示用的信息
LOCAL GUI_CHAR_CODE_TYPE_E s_ebook_file_type = GUI_CHAR_CODE_ANSI;//电子书原始文件的编码类型
//LOCAL wchar s_ucs2_str[MMIEBOOK_SEG_BUF_UNIT_LEN + 1] = {0}; 
LOCAL wchar *s_ucs2_str = 0; 

LOCAL MMIEBOOK_SHOW_RECT_INFO_T s_show_rect_info = {0};

//LOCAL uint16                            s_ebook_file_head_len = 0;
LOCAL MMI_STRING_T                      s_ebook_show_file_txt = {0};  
LOCAL BOOLEAN                           s_mmiebook_task_is_busy = FALSE;

const MMIEBOOK_MSG_INFO_T s_mmiebook_msg_info[] = {
    { MMIEBOOK_MSG_LOAD_BOOKLIST, MMIEBOOK_MSG_STATE_BUSY|MMIEBOOK_MSG_STATE_IDLE },//1 2
    { MMIEBOOK_MSG_TASK_EXIT, MMIEBOOK_MSG_STATE_BUSY|MMIEBOOK_MSG_STATE_IDLE|MMIEBOOK_MSG_STATE_NOT_CARE_BUSY },//1
    { MMIEBOOK_MSG_SHOW_TXT_RDY, MMIEBOOK_MSG_STATE_IDLE },//2
    { MMIEBOOK_MSG_SHOW_TXT_PROCESS, 0 },//2,3
    { MMIEBOOK_MSG_SEG_TAB_RDY, 0 },//3
    { MMIEBOOK_MSG_SHOW_WIN_OPT, MMIEBOOK_MSG_STATE_BUSY|MMIEBOOK_MSG_STATE_IDLE },//1,2
    { MMIEBOOK_MSG_SHOW_WIN_CLOSE, MMIEBOOK_MSG_STATE_BUSY|MMIEBOOK_MSG_STATE_IDLE|MMIEBOOK_MSG_STATE_NOT_CARE_BUSY },//1,2
    { MMIEBOOK_MSG_SET_PAGE, MMIEBOOK_MSG_STATE_BUSY },//1,3
    { MMIEBOOK_MSG_DELE, MMIEBOOK_MSG_STATE_BUSY|MMIEBOOK_MSG_STATE_IDLE },//1,2
    { MMIEBOOK_MSG_DELALL, MMIEBOOK_MSG_STATE_BUSY|MMIEBOOK_MSG_STATE_IDLE },//1,2
    { MMIEBOOK_MSG_CREATE_DETAIL_FILE, MMIEBOOK_MSG_STATE_BUSY|MMIEBOOK_MSG_STATE_IDLE|MMIEBOOK_MSG_STATE_NOT_CARE_BUSY },//1,2
    { MMIEBOOK_MSG_SHOW_TXT_WIN_OPEN, MMIEBOOK_MSG_STATE_BUSY },//1
    { MMIEBOOK_MSG_OPEN_SHOW_FILE_FAIL, MMIEBOOK_MSG_STATE_IDLE },//2
    { MMIEBOOK_MSG_MAKE_TEMPORARY_SEG_TAB, 0 },//3
    { MMIEBOOK_MSG_MAKE_SEG_TAB, MMIEBOOK_MSG_STATE_NOT_CARE_BUSY },//1,3
    { MMIEBOOK_MSG_INIT_DETAIL_FILE, 0 },//3
    { MMIEBOOK_MSG_CREATE_DETAIL_FILE_FAIL, MMIEBOOK_MSG_STATE_IDLE },//2	
    { MMIEBOOK_MSG_LOAD_TXT_BRIEF, MMIEBOOK_MSG_STATE_BUSY|MMIEBOOK_MSG_STATE_IDLE },	//1,2
    { MMIEBOOK_MSG_BOOKMARK, 0 },//
    { MMIEBOOK_MSG_ERROR_EXIT, MMIEBOOK_MSG_STATE_BUSY|MMIEBOOK_MSG_STATE_IDLE|MMIEBOOK_MSG_STATE_NOT_CARE_BUSY },//1
    { MMIEBOOK_MSG_TURNTO_PERCENT,  MMIEBOOK_MSG_STATE_BUSY},
    { MMIEBOOK_MSG_LAYOUT_PRE_SEGMENT,  MMIEBOOK_MSG_STATE_BUSY},
    { MMIEBOOK_MSG_LAYOUT_NEXT_SEGMENT, MMIEBOOK_MSG_STATE_BUSY},
    { MMIEBOOK_MSG_SCREEN_SWITCH,   MMIEBOOK_MSG_STATE_BUSY},
    { MMIEBOOK_MSG_FONT_SIZE_CHANGE,    MMIEBOOK_MSG_STATE_BUSY},
    { MMIEBOOK_MSG_FULL_SCREEN_SWITCH,   MMIEBOOK_MSG_STATE_BUSY},
    { MMIEBOOK_MSG_TURNTO_OFFSET,   MMIEBOOK_MSG_STATE_BUSY},
};

PUBLIC MMIEBOOK_FILE_BRIEF_T *          g_ebook_show_file_brief_ptr = PNULL;        //当前阅读的电子书简略信息指针
PUBLIC BLOCK_ID                         g_ebook_task_id = PNULL;                    //电子书服务器任务ID
PUBLIC uint16                           g_ebook_new_bookmark_pos = 0;
PUBLIC MMIEBOOK_BOOKMARK_NAME_T *       g_ebook_bookmark_name_tab = PNULL; 

uint8 s_txt_direction=0;

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                  *
**-------------------------------------------------------------------------- */
/*****************************************************************************/
// 	Description : CountLineNumPerPage
//	Global resource dependence : 
//  Author: hui.zhao
//	Note:   
/*****************************************************************************/
LOCAL void CountOnePageLines( void );//横竖屏切换后，需要重新获取该数据
/*****************************************************************************/
// 	Description : MMIEBOOK_LayoutLineInfo
//	Global resource dependence : 
//  Author: hui.zhao
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN LayoutSegmentLineInfo(//解析一段数据的行信息
                                              MMIEBOOK_FILE_SEGMENT_T*    ebook_seg_ptr, 
                                              MMIFILE_HANDLE              orig_file_handle,
                                              uint32                      orig_file_offset,         //从txt文件的某个偏移量读出一段内容来，做解析；这个是否要提出去?
                                              uint32                      file_size,                //IN: 控制读出文件大小
                                              BOOLEAN                     is_find_line_start_offest, //寻找断行的起始位置
                                              BOOLEAN                     is_layout_first_line       //是否要断出第一行
                                              );
                                              
/*****************************************************************************/
//  Description : FindLastParagraphSperator
//  Global resource dependence :
//   Author: hui.zhao
//  Note:   
/*****************************************************************************/
LOCAL int32 FindLastParagraphSperator(wchar *text, int32 length, int32 offset);//找到一段的最后一个结尾符，
                                                                          
/*****************************************************************************/
// 	Description : PutShowTxt
//	Global resource dependence : 
//  Author: hui.zhao
//	Note:   
/*****************************************************************************/
//java刷新函数
LOCAL int PutShowTxt(  
                                      MMIEBOOK_SHOW_INFO_T           *ebook_show_info_ptr
                                      );
                                       
/*****************************************************************************/
// 	Description : GetLineInfo
//	Global resource dependence : 
//  Author: hui.zhao
//	Note:   
/*****************************************************************************/
//通过行数来得到text,length,dir,以便显示
LOCAL BOOLEAN GetLineInfo(
                          MMIEBOOK_FILE_SEGMENT_T*    uni_show_txt_ptr,
                          int                         lineno,         //行信息
                          wchar                       *text_ptr,          //out
                          uint32                      *length_ptr       //out
                          );
  

/*****************************************************************************/
//  Description : ReadSegmentDataFromFile
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadSegmentDataFromFile(
                    MMIEBOOK_FILE_SEGMENT_T*	    cur_show_txt_ptr        //当前段信息指针
                    );  //返回失败说明有结束符
  
/*****************************************************************************/
//  Description : MakeCurSeg
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MakeCurSeg(
                    MMIEBOOK_FILE_SEGMENT_T*	    cur_show_txt_ptr       //当前段信息指针
                    );//返回失败说明有结束符
                    
/*****************************************************************************/
//  Description : MakeLineTabInSeg
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MakePreSeg(void);//返回失败说明有结束符  

/*****************************************************************************/
//  Description : MakeNextSeg
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MakeNextSeg(void);//返回失败说明有结束符    

/*****************************************************************************/
//  Description : ReMakeSegmentAfterScreenSwitch
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReMakeSegmentAfterScreenSwitch( void );

/*****************************************************************************/
//  Description : SwitchAndMakePreSegment
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SwitchAndMakePreSegment( void );

/*****************************************************************************/
//  Description : SwitchAndMakeNextSegment
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SwitchAndMakeNextSegment( void );

/*****************************************************************************/
//  Description : ClearFileSegmentInfo
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL void ClearFileSegmentInfo(MMIEBOOK_FILE_SEGMENT_T*    ebook_seg_ptr);

/*****************************************************************************/
//  Description : InitEbookShowRectInfo
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL void InitEbookShowRectInfo( MMI_WIN_ID_T win_id );        


/*****************************************************************************/
//  Description : MakeSegmentByOffset
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MakeSegmentByOffset( MMIEBOOK_MSG_TYPE_E msg );

/*****************************************************************************/
//  Description : 产生书签列表
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void MakeBookmarkNameTab(void);

/*****************************************************************************/
//  Description : 从文本中(gb)读取能够正好显示一行(ucs2)字符串
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:字符串字节数
/*****************************************************************************/
LOCAL uint16 ReadOneLineUCS2(
                             MMIFILE_HANDLE file_handle,
                             MMI_CTRL_ID_T  ctrl_id,//IN: 文本控件ID
                             uint32 offset,//IN: 字符串在文本中的偏移量
                             wchar  *ucs2_str_ptr//OUT: 要求空间大于能够容纳一行ucs2字符
                             );

/*****************************************************************************/
//  Description : InitFileSegmentInfo
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL void InitFileSegmentInfo( void );

/*****************************************************************************/
//  Description :  打开电子书时读取书签和上次阅读的位置信息
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
//  Return:TRUE:书签信息文件已存在 FALSE:书签信息文件不存在
/*****************************************************************************/
LOCAL BOOLEAN ReadBookmarkInfo( void );

/*****************************************************************************/
//  Description :  去掉字符串最后的换行符
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void ClearNewlineChar( MMI_STRING_T *text);

/*****************************************************************************/
//  Description :  将UNICODE编码中的0转换成空格
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
//  Return:转换的数量
/*****************************************************************************/
LOCAL int32 ConversionWcharZeroToSpace( wchar* ptr,int32 len);

/*****************************************************************************/
//  Description : 书签信息是否有效 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
//  Return:TRUE:书签信息有效；FALSE:书签信息无效
/*****************************************************************************/
LOCAL BOOLEAN IsBookmarkInfoValid( MMIEBOOK_BOOKMARK_T * bookmark_info,int32 cur_read_offset);

/*---------------------------------------------------------------------------*/
/*                          PUBLIC FUNCTION DEFINITION                       */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 初始化电子书资源
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_AllocEbookTaskRes(void)
{
    //为电子书读取的数据开辟缓存区
    //SCI_TRACE_LOW:"MMIEBOOK_TASK MMIEBOOK_AllocEbookTaskRes enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_297_112_2_18_2_15_19_34,(uint8*)"");
    
    //段一
    if(PNULL != s_ebook_seg_part1.ucs2_str_ptr)
    {
    	SCI_FREE(s_ebook_seg_part1.ucs2_str_ptr);
		s_ebook_seg_part1.ucs2_str_ptr=PNULL;
    }
    s_ebook_seg_part1.ucs2_str_ptr = (wchar*)SCI_ALLOCA((MMIEBOOK_SEG_BUF_UNIT_LEN + 1)*sizeof(wchar));
    if(PNULL == s_ebook_seg_part1.ucs2_str_ptr)
    {
        goto ERROR_LABEL;
    }
    
    if(PNULL != s_ebook_seg_part1.line_offset_tab_ptr)
    {
    	SCI_FREE(s_ebook_seg_part1.line_offset_tab_ptr);
		s_ebook_seg_part1.line_offset_tab_ptr=PNULL;
    }
    s_ebook_seg_part1.line_offset_tab_ptr = (int16*)SCI_ALLOCA((MMIEBOOK_SEG_BUF_UNIT_LEN + 1)*sizeof(wchar));
    if(PNULL == s_ebook_seg_part1.line_offset_tab_ptr)
    {
        goto ERROR_LABEL;
    }
    //段二
    if(PNULL != s_ebook_seg_part2.ucs2_str_ptr)
    {
    	SCI_FREE(s_ebook_seg_part2.ucs2_str_ptr);
		s_ebook_seg_part2.ucs2_str_ptr=PNULL;
    }
    s_ebook_seg_part2.ucs2_str_ptr = (wchar*)SCI_ALLOCA((MMIEBOOK_SEG_BUF_UNIT_LEN + 1)*sizeof(wchar));
    if(PNULL == s_ebook_seg_part2.ucs2_str_ptr)
    {
        goto ERROR_LABEL;
    }
    
    if(PNULL != s_ebook_seg_part2.line_offset_tab_ptr)
    {
    	SCI_FREE(s_ebook_seg_part2.line_offset_tab_ptr);
		s_ebook_seg_part2.line_offset_tab_ptr=PNULL;
    }
    s_ebook_seg_part2.line_offset_tab_ptr = (int16*)SCI_ALLOCA((MMIEBOOK_SEG_BUF_UNIT_LEN + 1)*sizeof(wchar));
    if(PNULL == s_ebook_seg_part2.line_offset_tab_ptr)
    {
        goto ERROR_LABEL;
    }

    //为书签名字信息开辟空间
    if(PNULL != g_ebook_bookmark_name_tab)
    {
    	SCI_FREE(g_ebook_bookmark_name_tab);
		g_ebook_bookmark_name_tab=PNULL;
    }
    g_ebook_bookmark_name_tab = SCI_ALLOCA(sizeof(MMIEBOOK_BOOKMARK_NAME_T) * MMIEBOOK_BOOKMARK_MAX_NUM);
    if(PNULL == g_ebook_bookmark_name_tab)
    {
        goto ERROR_LABEL;
    }
    SCI_MEMSET(g_ebook_bookmark_name_tab, 0, (sizeof(MMIEBOOK_BOOKMARK_NAME_T) * MMIEBOOK_BOOKMARK_MAX_NUM));

    SCI_MEMSET(&s_ebook_show_info,0,(sizeof(MMIEBOOK_SHOW_INFO_T)));
    if(PNULL != s_ucs2_str)
    {
    	SCI_FREE(s_ucs2_str);
		s_ucs2_str=PNULL;
    }
    s_ucs2_str = (uint16*)SCI_ALLOCA((MMIEBOOK_SEG_BUF_UNIT_LEN + 1)*sizeof(wchar));
    if(PNULL == s_ucs2_str)
    {
        goto ERROR_LABEL;
    }
    SCI_MEMSET(s_ucs2_str, 0, (MMIEBOOK_SEG_BUF_UNIT_LEN + 1)*sizeof(wchar));
        
    return TRUE;

 ERROR_LABEL:
    MMIEBOOK_FreeEbookTaskRes();
    //SCI_TRACE_LOW:"MMIEBOOK_TASK MMIEBOOK_AllocEbookTaskRes error"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_374_112_2_18_2_15_20_35,(uint8*)"");
    return FALSE;

}   

/*****************************************************************************/
//  Description : 释放电子书占用的资源
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_FreeEbookTaskRes(void)
{
    //释放电子书读取数据的缓存区
    
    //释放段内分行信息内存，
    //SCI_TRACE_LOW:"MMIEBOOK_TASK MMIEBOOK_FreeEbookTaskRes enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_389_112_2_18_2_15_20_36,(uint8*)"");
    SCI_FREE(s_ebook_seg_part1.ucs2_str_ptr);
    SCI_FREE(s_ebook_seg_part1.line_offset_tab_ptr);
    SCI_MEMSET(&s_ebook_seg_part1, 0, sizeof(MMIEBOOK_FILE_SEGMENT_T));
    
    SCI_FREE(s_ebook_seg_part2.ucs2_str_ptr);
    SCI_FREE(s_ebook_seg_part2.line_offset_tab_ptr);
    SCI_MEMSET(&s_ebook_seg_part2, 0, sizeof(MMIEBOOK_FILE_SEGMENT_T));
    
 
     //释放书签列表信息内存
    SCI_FREE(g_ebook_bookmark_name_tab);
    SCI_FREE(s_ucs2_str);
       
    //若有互斥量，则释放
    return TRUE;
}

/*****************************************************************************/
//  Description :  电子书相关消息处理函数
//  Global resource dependence : 
//  Author: ying.xu
//  Note:	去掉电子书独立task
//  Return:
/*****************************************************************************/
void MMIEbook_HandleTaskMessage( MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMIEBOOK_OPEN_FILE_ERROR_E open_file_ret = MMIEBOOK_OPEN_FILE_ERROR;    
    //SCI_TRACE_LOW:"MMIEBOOK_TASK MMIEbook_HandleTaskMessage msg_id == %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_415_112_2_18_2_15_20_37,(uint8*)"d",msg_id);
    switch (msg_id)
    {              
        case MMIEBOOK_MSG_SHOW_TXT_WIN_OPEN:     
            g_ebook_show_file_brief_ptr = MMIEBOOK_GetFileBrief();
            //SCI_TRACE_LOW:"MMIEBOOK_task MMIEBOOK_MSG_SHOW_TXT_WIN_OPEN file_dev = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_420_112_2_18_2_15_20_38,(uint8*)"d", g_ebook_show_file_brief_ptr->file_dev);
               //电子书是否能够打开
            open_file_ret = MMIEBOOK_OpenShowFile((uint16)*((int32*)param));
            
#ifdef DRM_SUPPORT
            //电子书是否版权有效
            if(MMIEBOOK_OPEN_FILE_NO_RIGHTS == open_file_ret)
            {
                MMIEBOOK_OPEN_FILE_ERROR_E open_ret = MMIEBOOK_OPEN_FILE_NO_RIGHTS;
                
                //失败，要求失败处理
                MMIEBOOK_SendMsgtoMMI(MMIEBOOK_TXT_SHOW_WIN_ID, MMIEBOOK_MSG_OPEN_SHOW_FILE_FAIL, (DPARAM)&open_ret, sizeof(MMIEBOOK_OPEN_FILE_ERROR_E));
                break;
            }
#endif     
         
            if( MMIEBOOK_OPEN_FILE_ERROR == open_file_ret )
            {
                //失败，要求失败处理
                MMIEBOOK_SendMsgtoMMI(MMIEBOOK_TXT_SHOW_WIN_ID, MMIEBOOK_MSG_OPEN_SHOW_FILE_FAIL, PNULL, 0);
                break;
            }
			
            //获得电子书的编码类型
            if (!MMIEBOOK_GetTextFileType(MMIEBOOK_GetShowFileHandle(), &s_ebook_file_type))
            {
                //失败，要求失败处理
                MMIEBOOK_SendMsgtoMMI(MMIEBOOK_TXT_SHOW_WIN_ID, MMIEBOOK_MSG_OPEN_SHOW_FILE_FAIL, PNULL, 0);
                break;
            }
			
            /*if (MMIEBOOK_IsExistZero(MMIEBOOK_GetShowFileHandle(),g_ebook_show_file_brief_ptr->file_size,s_ebook_file_type))
            {
                MMIEBOOK_SendMsgtoMMI(MMIEBOOK_TXT_SHOW_WIN_ID, MMIEBOOK_MSG_OPEN_SHOW_FILE_FAIL, PNULL, 0);
                break;
            }*/

            
            //获得电子书的头信息
            /*
            if(!MMIEBOOK_GetTextFileHeadLen(MMIEBOOK_GetShowFileHandle(), &s_ebook_file_head_len))
            {
                //失败，要求失败处理
                MMIEBOOK_SendMsgtoMMI(MMIEBOOK_TXT_SHOW_WIN_ID, MMIEBOOK_MSG_OPEN_SHOW_FILE_FAIL, PNULL, 0);
                break;
            }               
            */
            if(MMIEBOOK_isEbookTypeFile(g_ebook_show_file_brief_ptr->full_name_wstr, g_ebook_show_file_brief_ptr->full_name_len))
            {
                MMIEBOOK_UpdateLatestList();
            }
            
            InitEbookShowRectInfo(MMIEBOOK_TXT_SHOW_WIN_ID);
            
            //计算当前模式下一页的行数
            CountOnePageLines();

            //初始化segment信息
            InitFileSegmentInfo();
            if(MMIEBOOK_isEbookTypeFile(g_ebook_show_file_brief_ptr->full_name_wstr, g_ebook_show_file_brief_ptr->full_name_len))
            {
                if (ReadBookmarkInfo())//书签文件是否存在，若存在则打开并读取书签信息；若不存在，则创建书签文件
                {
                    s_ebook_show_info.cur_show_txt_ptr->this_seg_offset = s_ebook_show_info.cur_read_offset;
                }
                else
                {
                    s_ebook_show_info.cur_show_txt_ptr->this_seg_offset = 0;
                }
            }
            else
            {
                s_ebook_show_info.cur_show_txt_ptr->this_seg_offset = 0;
            }
            
            //要求显示当前页面
            MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_TURNTO_OFFSET, 0 , 0);
            break;
            
        case MMIEBOOK_MSG_TURNTO_PERCENT://跳转到百分比
            MakeSegmentByOffset(MMIEBOOK_MSG_TURNTO_PERCENT);
            //要求显示当前页面            
            MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_SET_PAGE, 0 , 0);
            break;

        case MMIEBOOK_MSG_TURNTO_OFFSET://跳转到偏移值            
            MakeSegmentByOffset(MMIEBOOK_MSG_TURNTO_OFFSET);
            //要求显示当前页面
            MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_SET_PAGE, 0 , 0);
            break;
                        
        case MMIEBOOK_MSG_LAYOUT_PRE_SEGMENT://向上解析一段
            SwitchAndMakePreSegment();
            //要求显示当前页面
            MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_SET_PAGE, 0 , 0);
            break;
            
        case MMIEBOOK_MSG_LAYOUT_NEXT_SEGMENT://向下解析一段
            SwitchAndMakeNextSegment();    
            //要求显示当前页面
            MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_SET_PAGE, 0 , 0);
            break;
            
        case MMIEBOOK_MSG_SCREEN_SWITCH://横竖屏切换处理         
            //重新计算显示用的参数
            InitEbookShowRectInfo(MMIEBOOK_TXT_SHOW_WIN_ID);
            CountOnePageLines();
            ReMakeSegmentAfterScreenSwitch();  
            MakeSegmentByOffset(MMIEBOOK_MSG_TURNTO_OFFSET);
            PutShowTxt(&s_ebook_show_info);    
            break;
        
        case MMIEBOOK_MSG_FONT_SIZE_CHANGE://字体变化
            //重新计算显示用的参数
            InitEbookShowRectInfo(MMIEBOOK_TXT_SHOW_WIN_ID);
            CountOnePageLines();
            ReMakeSegmentAfterScreenSwitch(); 
            MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_TURNTO_OFFSET, 0 , 0);
            //要求显示当前页面
            break;
            
        case MMIEBOOK_MSG_FULL_SCREEN_SWITCH://全屏切换处理
            InitEbookShowRectInfo(MMIEBOOK_TXT_SHOW_WIN_ID);
            CountOnePageLines();
            //要求显示当前页面
            MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_SET_PAGE, 0 , 0);
            break;            
            
        case MMIEBOOK_MSG_SET_PAGE://不翻页，找到显示内容
            PutShowTxt(&s_ebook_show_info);           
            MMIEBOOK_SendMsgtoMMI(MMIEBOOK_TXT_SHOW_WIN_ID,MMIEBOOK_MSG_SHOW_TXT_RDY, PNULL,0);
            break;
#ifdef MMIEBOOK_PAGESLIDE_SUPPORT
         case MMIEBOOK_MSG_SLIDE_SET_PAGE://不翻页，找到显示内容
            PutShowTxt(&s_ebook_show_info);           
            break;
         case MMIEBOOK_MSG_SLIDE_LAYOUT_PRE_SEGMENT://向上解析一段
            SwitchAndMakePreSegment();
            //要求显示当前页面
            MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_SLIDE_SET_PAGE, 0 , 0);
            break;
            
        case MMIEBOOK_MSG_SLIDE_LAYOUT_NEXT_SEGMENT://向下解析一段
            SwitchAndMakeNextSegment();    
            //要求显示当前页面
            MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_SLIDE_SET_PAGE, 0 , 0);
            break;
#endif
        default:
            break;
        }    
}

/*****************************************************************************/
// 	Description : CountLineNumPerPage
//	Global resource dependence : 
//  Author: hui.zhao
//	Note:   
/*****************************************************************************/
LOCAL void CountOnePageLines( void )//横竖屏切换后，需要重新获取该数据
{
    uint16                      height = 0;
    uint16                      max_lines = 0;  //一屏最多几行

    height = GUIFONT_GetHeight(s_show_rect_info.font);
    max_lines = (uint16)((s_show_rect_info.txt_show_bottom - s_show_rect_info.txt_show_top-MMITHEME_TEXT_MARGIN_SPACE)
        / (height + MMIEBOOK_TXT_SHOW_TEXTBOX_LINE_SPACE));
    
    s_ebook_show_info.one_page_lines = max_lines;
    //SCI_TRACE_LOW:"MMIEBOOK_task CountOnePageLines font height = %d, lines = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_587_112_2_18_2_15_20_39,(uint8*)"dd", height, max_lines);
}

/*****************************************************************************/
// 	Description : MMIEBOOK_LayoutLineInfo
//	Global resource dependence : 
//  Author: hui.zhao
//	Note:   
/*****************************************************************************/
LOCAL BOOLEAN LayoutSegmentLineInfo(//解析一段数据的行信息
                                              MMIEBOOK_FILE_SEGMENT_T*    ebook_seg_ptr, 
                                              MMIFILE_HANDLE              orig_file_handle,
                                              uint32                      orig_file_offset,         //从txt文件的某个偏移量读出一段内容来，做解析；这个是否要提出去?
                                              uint32                      file_size,                //IN: 控制读出文件大小
                                              BOOLEAN                     is_find_line_start_offest, //寻找断行的起始位置
                                              BOOLEAN                     is_layout_first_line       //是否要断出第一行
                                              )
{
    BOOLEAN         result = TRUE;
        uint16                      cur_len = 0;
        uint16                      line_length = 0;
        uint16                      pixel_num_line=0;
        uint16                      i = 0;
        
    //计算分行信息
    //SCI_TRACE_LOW:"MMIEBOOK_task LayoutSegmentLineInfo enter time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_610_112_2_18_2_15_20_40,(uint8*)"d", SCI_GetTickCount());
      
        while((cur_len < ebook_seg_ptr->ucs2_str_len) && (i < MMIEBOOK_SEG_BUF_UNIT_LEN))
        {
            if (!GUI_GetStringInfoInLine(
                &ebook_seg_ptr->ucs2_str_ptr[cur_len], 
                (uint16)(ebook_seg_ptr->ucs2_str_len - cur_len),
                s_show_rect_info.font,//MMIEBOOK_TXT_SHOW_TEXTBOX_FONT,
                s_show_rect_info.txt_show_char_space,//MMIEBOOK_TXT_SHOW_TEXTBOX_CHAR_SPACE,
                (s_show_rect_info.txt_show_right+1)-2*MMITHEME_TEXT_MARGIN_SPACE,//(MMIEBOOK_TXT_SHOW_TEXTBOX_RIGHT - MMIEBOOK_TXT_SHOW_TEXTBOX_LEFT-2*MMITHEME_TEXT_MARGIN_SPACE),//需要减去左右空隙的宽度
                &line_length,
                &pixel_num_line,
                TRUE
                ))
            {   //若碰到'/0',或line_length==0时做保护处理
                ebook_seg_ptr->next_seg_offset = file_size;   
                break;
            }
            
            cur_len += line_length;
            ebook_seg_ptr->line_offset_tab_ptr[++i] = cur_len;//从第i+1行的length，映射存储到ebook_seg_ptr->line_offset_tab_ptr[i]中;
        }
        
        ebook_seg_ptr->total_line_num = i;//该段的总行数
    //SCI_TRACE_LOW:"MMIEBOOK_task LayoutSegmentLineInfo leave time = %d, line num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_635_112_2_18_2_15_20_41,(uint8*)"dd",SCI_GetTickCount(), i);
    return result;//FALSE:遇到了结束符
        }

/*****************************************************************************/
//  Description : FindFirstParagraphSperator
//  Global resource dependence :
//   Author: hui.zhao
//  Note:   
/*****************************************************************************/
LOCAL int32 FindFirstParagraphSperator(wchar *text, int length, int offset)//找到一段的最后一个结尾符，
{
    int32 index = 0;
    
    for (index = offset; index < length; ++index)
    {
        if (text[index] == L'\r')
        {
            if (index < length - 1)
             {
                if( text[index + 1] == L'\n')  //WINDOWS CR + LF \r\n
                {
                    index += 2;
                    break;
                }
            }
            else        //unix CR \r
            {
                index += 1;
                break;
            }
        }
        else  if (text[index] == L'\n' || text[index] == 0x2029)  
        //mac CR \n   0x2029 unknowed char
        {
            index += 1;            
            break;
        }        
    } 

    return index;
}

/*****************************************************************************/
//  Description : FindLastParagraphSperator
//  Global resource dependence :
//   Author: hui.zhao
//  Note:   
/*****************************************************************************/
LOCAL int32 FindLastParagraphSperator(wchar *text, int32 length, int32 offset)//找到一段的最后一个结尾符，
{
    int32 index = 0;
    
    for (index = length - 1 ; index >= offset ; index--)
    { 
        if (text[index] == L'\r' || text[index] == L'\n' || text[index] == 0x2029)  
        //WINDOWS CR + LF \r\n  unix LF \n    mac CR \r   0x2029 unknowed char
        {
            index += 1;            
            break;
        }
    }  
    if (index == -1)
    {
        index = length;
    }
    return index;
}

#ifdef MMIEBOOK_PAGESLIDE_SUPPORT
/*****************************************************************************/
//  Description : MMIEBOOK_TurnLineOrPage
//  Global resource dependence :
//   Author: reni.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIEBOOK_GetNextPageForEffect(SE_PAGE_TURN_MODE_E turn_mode)
{      
    switch (turn_mode)
    {
    case SE_PAGE_TURN_LEFT_TOP:
    case SE_PAGE_TURN_LEFT_BOTTOM:
        if (s_ebook_show_info.cur_line < s_ebook_show_info.one_page_lines)//翻到上一个段中去
        {
            if (s_ebook_show_info.cur_show_txt_ptr->this_seg_offset == 0)//已经到电子书开头了
            {
                if (s_ebook_show_info.cur_line > 0)
                {
                    s_ebook_show_info.cur_line = 0;
                    //要求显示当前页面
                    MMIEBOOK_SendMsgtoTask( MMIEBOOK_MSG_SLIDE_SET_PAGE, g_ebook_task_id, MMIEBOOK_PAGE_CUR);
                }
                else
                {
                    //已经从电子书第一页开始看了，则不做处理
                    MMIEBOOK_SetIsNeedCloseTimer(FALSE);
                    break;
                }
            }
            //当前在下一段显示，
            else if(s_ebook_show_info.cur_show_txt_ptr == s_ebook_show_info.next_show_txt_ptr)
            {
                if (s_ebook_show_info.pre_show_txt_ptr->total_line_num - (s_ebook_show_info.one_page_lines - s_ebook_show_info.cur_line - 1) > 0)
                {
                    s_ebook_show_info.cur_line = s_ebook_show_info.pre_show_txt_ptr->total_line_num - 
                        (s_ebook_show_info.one_page_lines - s_ebook_show_info.cur_line - 1) - 1;
                }
                else
                {
                    s_ebook_show_info.cur_line = 0;//往上翻时已经翻到上一段第一页开始处了
                }
                //显示指针调整到上一段
                s_ebook_show_info.cur_show_txt_ptr = s_ebook_show_info.pre_show_txt_ptr;
                MMIEBOOK_SendMsgtoTask(  MMIEBOOK_MSG_SLIDE_SET_PAGE, g_ebook_task_id, 0);
            }
            else
            {                
                //发消息翻到上一段
                MMIEBOOK_SendMsgtoTask(  MMIEBOOK_MSG_SLIDE_LAYOUT_PRE_SEGMENT, g_ebook_task_id, 0);
            }
        }
        else
        {
            s_ebook_show_info.cur_line -= s_ebook_show_info.one_page_lines;
            //要求显示当前页面
            MMIEBOOK_SendMsgtoTask(  MMIEBOOK_MSG_SLIDE_SET_PAGE, g_ebook_task_id, MMIEBOOK_PAGE_CUR );
        }
        break;
        
       
    case SE_PAGE_TURN_RIGHT_TOP:
    case SE_PAGE_TURN_RIGHT_BOTTOM:   //向下翻页    
        if(s_ebook_show_info.cur_show_txt_ptr == s_ebook_show_info.pre_show_txt_ptr)
        {
            if (s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines >= s_ebook_show_info.cur_show_txt_ptr->total_line_num)//翻到下一个段中去
            {
                if(s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines 
                    - s_ebook_show_info.cur_show_txt_ptr->total_line_num + 1 > s_ebook_show_info.next_show_txt_ptr->total_line_num)
                {
                    break;  //do nothing
                }
                else
                {
                    s_ebook_show_info.cur_line += s_ebook_show_info.one_page_lines - 
                            s_ebook_show_info.pre_show_txt_ptr->total_line_num;
                    s_ebook_show_info.cur_show_txt_ptr = s_ebook_show_info.next_show_txt_ptr;
                    MMIEBOOK_SendMsgtoTask(  MMIEBOOK_MSG_SLIDE_SET_PAGE, g_ebook_task_id, 0);
                }
            }
            else
            {
                s_ebook_show_info.cur_line += s_ebook_show_info.one_page_lines;
                MMIEBOOK_SendMsgtoTask(  MMIEBOOK_MSG_SLIDE_SET_PAGE, g_ebook_task_id, 0);
            }
        }
        else
        {       
            if (s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines >= s_ebook_show_info.cur_show_txt_ptr->total_line_num)//翻到下一个段中去
            {
                if (s_ebook_show_info.cur_show_txt_ptr->next_seg_offset == g_ebook_show_file_brief_ptr->file_size)
                {//已经到了电子书末尾，不做处理
                    break; 
                }
				else if(s_ebook_show_info.cur_show_txt_ptr->next_seg_offset < g_ebook_show_file_brief_ptr->file_size)
				{
					s_ebook_show_info.cur_line += s_ebook_show_info.one_page_lines;
 					MMIEBOOK_SendMsgtoTask( MMIEBOOK_MSG_SLIDE_LAYOUT_NEXT_SEGMENT, g_ebook_task_id, 0 );
				}
            }
            else if (s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines*2 >= s_ebook_show_info.cur_show_txt_ptr->total_line_num)//翻到下一个段中去
            {
                if (s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines < s_ebook_show_info.cur_show_txt_ptr->total_line_num )//翻到下一个段中去
                {     //发消息翻到下一段                
                    s_ebook_show_info.cur_line += s_ebook_show_info.one_page_lines;
                    MMIEBOOK_SendMsgtoTask( MMIEBOOK_MSG_SLIDE_LAYOUT_NEXT_SEGMENT, g_ebook_task_id, 0 );
                }
            }
            else       
            {
                s_ebook_show_info.cur_line += s_ebook_show_info.one_page_lines;
                MMIEBOOK_SendMsgtoTask(  MMIEBOOK_MSG_SLIDE_SET_PAGE, g_ebook_task_id, 0);
            }
        }
        break;
        
    default:
        break;
    }
}
#endif

/*****************************************************************************/
//  Description : MMIEBOOK_TurnLineOrPage
//  Global resource dependence :
//   Author: hui.zhao
//  Note:   
/*****************************************************************************/
PUBLIC void MMIEBOOK_TurnLineOrPage(
                                    MMIEBOOK_PAGE_DIRECTION_E       direction //IN: 翻页方向
                                    )//向上/向下翻行或翻页
{
    //SCI_TRACE_LOW:"MMIEBOOK_task MMIEBOOK_TurnLineOrPage enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_823_112_2_18_2_15_20_42,(uint8*)"");
    switch (direction)
    {
    case MMIEBOOK_PAGE_UP:
        if (s_ebook_show_info.cur_line < s_ebook_show_info.one_page_lines)//翻到上一个段中去
        {
            if (s_ebook_show_info.cur_show_txt_ptr->this_seg_offset == 0)//已经到电子书开头了
            {
                if (s_ebook_show_info.cur_line > 0)
                {
                    s_ebook_show_info.cur_line = 0;
                    //要求显示当前页面
                    MMIEBOOK_SendMsgtoTask( MMIEBOOK_MSG_SET_PAGE, g_ebook_task_id, MMIEBOOK_PAGE_CUR);
                }
                else
                {
                    //已经从电子书第一页开始看了，则不做处理
                    MMIEBOOK_SetIsNeedCloseTimer(FALSE);
                    break;
                }
            }
            //当前在下一段显示，
            else if(s_ebook_show_info.cur_show_txt_ptr == s_ebook_show_info.next_show_txt_ptr)
            {
                if (s_ebook_show_info.pre_show_txt_ptr->total_line_num - (s_ebook_show_info.one_page_lines - s_ebook_show_info.cur_line - 1) > 0)
                {
                    s_ebook_show_info.cur_line = s_ebook_show_info.pre_show_txt_ptr->total_line_num - 
                        (s_ebook_show_info.one_page_lines - s_ebook_show_info.cur_line - 1) - 1;
                }
                else
                {
                    s_ebook_show_info.cur_line = 0;//往上翻时已经翻到上一段第一页开始处了
                }
                //显示指针调整到上一段
                s_ebook_show_info.cur_show_txt_ptr = s_ebook_show_info.pre_show_txt_ptr;
                MMIEBOOK_SendMsgtoTask(  MMIEBOOK_MSG_SET_PAGE, g_ebook_task_id, 0);
            }
            else
            {                
                //发消息翻到上一段
                MMIEBOOK_SendMsgtoTask(  MMIEBOOK_MSG_LAYOUT_PRE_SEGMENT, g_ebook_task_id, 0);
            }
        }
        else
        {
            s_ebook_show_info.cur_line -= s_ebook_show_info.one_page_lines;
            //要求显示当前页面
            MMIEBOOK_SendMsgtoTask(  MMIEBOOK_MSG_SET_PAGE, g_ebook_task_id, MMIEBOOK_PAGE_CUR );
        }
        break;
        
    case MMIEBOOK_LINE_UP:
        if (s_ebook_show_info.cur_line == 0)//翻到上一个段中去
        {
            if (s_ebook_show_info.cur_show_txt_ptr->this_seg_offset == 0)//已经到电子书开头了
            {
                //已经从电子书第一页开始看了，则不做处理
                MMIEBOOK_SetIsNeedCloseTimer(FALSE);
            }            
            //当前在下一段显示，显示指针调整到上一段
            else if(s_ebook_show_info.cur_show_txt_ptr == s_ebook_show_info.next_show_txt_ptr)            
            {
                s_ebook_show_info.cur_line = s_ebook_show_info.pre_show_txt_ptr->total_line_num - 1;
                //显示指针调整到上一段
                s_ebook_show_info.cur_show_txt_ptr = s_ebook_show_info.pre_show_txt_ptr;      
                MMIEBOOK_SendMsgtoTask(  MMIEBOOK_MSG_SET_PAGE, g_ebook_task_id, MMIEBOOK_PAGE_CUR  );
            }            
            else
            {
                //发消息翻到上一段
                //翻页时会自动加上一页的page_line_num
                s_ebook_show_info.cur_line =  s_ebook_show_info.one_page_lines - 1; 
                MMIEBOOK_SendMsgtoTask(  MMIEBOOK_MSG_LAYOUT_PRE_SEGMENT, g_ebook_task_id, 0);
            }
        }
        else
        {
            s_ebook_show_info.cur_line -= 1;
            //要求显示当前页面
            MMIEBOOK_SendMsgtoTask(   MMIEBOOK_MSG_SET_PAGE, g_ebook_task_id, 0  );
        }
        break;
        
    case MMIEBOOK_PAGE_CUR://不翻页
        //nothing
        break;
        
    case MMIEBOOK_LINE_DOWN:    //向下翻行    
        if(s_ebook_show_info.cur_show_txt_ptr == s_ebook_show_info.pre_show_txt_ptr)
        {
            if (s_ebook_show_info.cur_line == s_ebook_show_info.cur_show_txt_ptr->total_line_num - 1)//翻到下一个段中去
            {          
                 if(s_ebook_show_info.next_show_txt_ptr->total_line_num == 0)
                 {
                    break;
                 }
                 s_ebook_show_info.cur_line = 0;     
                 s_ebook_show_info.cur_show_txt_ptr = s_ebook_show_info.next_show_txt_ptr;
                 MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_SET_PAGE,  g_ebook_task_id, 0 );
            }
            else
            {
                s_ebook_show_info.cur_line += 1;
                //要求显示当前页面
                MMIEBOOK_SendMsgtoTask( MMIEBOOK_MSG_SET_PAGE, g_ebook_task_id, MMIEBOOK_PAGE_CUR  );
            }            
        }
        else
        {  
            if(s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines >= 
                s_ebook_show_info.cur_show_txt_ptr->total_line_num )       
            {
                if (s_ebook_show_info.cur_show_txt_ptr->next_seg_offset == g_ebook_show_file_brief_ptr->file_size)
                {//已经到了电子书末尾，不做处理
                    break; 
                }
                s_ebook_show_info.cur_line += 1;
                MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_LAYOUT_NEXT_SEGMENT, g_ebook_task_id,  0 );
            }      
            else
            {
                s_ebook_show_info.cur_line += 1;
                //要求显示当前页面
                MMIEBOOK_SendMsgtoTask( MMIEBOOK_MSG_SET_PAGE, g_ebook_task_id, MMIEBOOK_PAGE_CUR  );
            }            
        }
        break;
        
    case MMIEBOOK_PAGE_DOWN:   //向下翻页    
        if(s_ebook_show_info.cur_show_txt_ptr == s_ebook_show_info.pre_show_txt_ptr)
        {
            if (s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines >= s_ebook_show_info.cur_show_txt_ptr->total_line_num)//翻到下一个段中去
            {
                if(s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines 
                    - s_ebook_show_info.cur_show_txt_ptr->total_line_num + 1 > s_ebook_show_info.next_show_txt_ptr->total_line_num)
                {
                    break;  //do nothing
                }
                else
                {
                    s_ebook_show_info.cur_line += s_ebook_show_info.one_page_lines - 
                            s_ebook_show_info.pre_show_txt_ptr->total_line_num;
                    s_ebook_show_info.cur_show_txt_ptr = s_ebook_show_info.next_show_txt_ptr;
                    MMIEBOOK_SendMsgtoTask(  MMIEBOOK_MSG_SET_PAGE, g_ebook_task_id, 0);
                }
            }
            else
            {
                s_ebook_show_info.cur_line += s_ebook_show_info.one_page_lines;
                MMIEBOOK_SendMsgtoTask(  MMIEBOOK_MSG_SET_PAGE, g_ebook_task_id, 0);
            }
        }
        else
        {       
            if (s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines >= s_ebook_show_info.cur_show_txt_ptr->total_line_num)//翻到下一个段中去
            {
                if (s_ebook_show_info.cur_show_txt_ptr->next_seg_offset == g_ebook_show_file_brief_ptr->file_size)
                {//已经到了电子书末尾，不做处理
                    break; 
                }
            }
            else if (s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines*2 >= s_ebook_show_info.cur_show_txt_ptr->total_line_num)//翻到下一个段中去
            {
                if (s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines < s_ebook_show_info.cur_show_txt_ptr->total_line_num )//翻到下一个段中去
                {     //发消息翻到下一段                
                    s_ebook_show_info.cur_line += s_ebook_show_info.one_page_lines;
                    MMIEBOOK_SendMsgtoTask( MMIEBOOK_MSG_LAYOUT_NEXT_SEGMENT, g_ebook_task_id, 0 );
                }
            }
            else       
            {
                s_ebook_show_info.cur_line += s_ebook_show_info.one_page_lines;
                MMIEBOOK_SendMsgtoTask(  MMIEBOOK_MSG_SET_PAGE, g_ebook_task_id, 0);
            }
        }
        break;
        
    default:
        //SCI_ASSERT(0);/*assert verified*/
        break;
    }
}

/*****************************************************************************/
// 	Description : PutShowTxt
//	Global resource dependence : 
//  Author: hui.zhao
//	Note:   
/*****************************************************************************/
LOCAL int PutShowTxt(  
                                      MMIEBOOK_SHOW_INFO_T           *ebook_show_info_ptr
                                      )
{
    uint32          line_text_length = 255;
    int             y = 0;
    uint16          height = 0;
    int             start_line = 0;
    uint32          page_text_length = 0;   
    SCI_TRACE_LOW("ebook_time:PutShowTxt  start=%d",SCI_GetTickCount());
    //SCI_TRACE_LOW:"MMIEbook_TASK PutShowTxt total_page is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1021_112_2_18_2_15_21_43,(uint8*)"d",ebook_show_info_ptr->cur_show_txt_ptr->total_line_num);
    
    height = GUIFONT_GetHeight(s_show_rect_info.font); 
    
    //common condition
    for (start_line = 0; start_line < ebook_show_info_ptr->one_page_lines; 
    ++start_line)
    {            
        if(ebook_show_info_ptr->cur_line + start_line 
            == ebook_show_info_ptr->cur_show_txt_ptr->total_line_num)
        {
            break;
        }
        //in case s_ucs2_str arrary overflow
        if(page_text_length >= MMIEBOOK_SEG_BUF_UNIT_LEN)
        {
            break;
        }
        if (GetLineInfo(
            ebook_show_info_ptr->cur_show_txt_ptr,
            ebook_show_info_ptr->cur_line + start_line, 
            &s_ucs2_str[page_text_length],
            &line_text_length
            ))
        {
            page_text_length += line_text_length;
        }
        else
        {
            break;
        }
        y += (height + MMIEBOOK_TXT_SHOW_TEXTBOX_LINE_SPACE);//每一行的高度
    }  
    
    //没有拷贝完一页,
    if(start_line != ebook_show_info_ptr->one_page_lines)         
    {
        //SCI_TRACE_LOW:"MMIEBOOK_task PutShowTxt from next segment start_line = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1058_112_2_18_2_15_21_44,(uint8*)"d", start_line);
        //当前段是上一段,且下一段有数据，则从下一段继续拷贝
        if(ebook_show_info_ptr->cur_show_txt_ptr == ebook_show_info_ptr->pre_show_txt_ptr
            &&ebook_show_info_ptr->next_show_txt_ptr->total_line_num != 0)
        {
            //剩余行数
            int32 left_line = ebook_show_info_ptr->one_page_lines - start_line;
            for (start_line = 0;  start_line < left_line;   ++start_line)
            {            
                if(start_line == ebook_show_info_ptr->next_show_txt_ptr->total_line_num)
                {
                    break;
                }
                if(page_text_length >= MMIEBOOK_SEG_BUF_UNIT_LEN)
                {
                    break;
                }
                if (GetLineInfo(
                    ebook_show_info_ptr->next_show_txt_ptr,
                    start_line, 
                    &s_ucs2_str[page_text_length],
                    &line_text_length
                    ))
                {
                    page_text_length += line_text_length;
                }
                else
                {
                    break;
                }
                y += (height + MMIEBOOK_TXT_SHOW_TEXTBOX_LINE_SPACE);//每一行的高度
            }  
        }
    }
 
    //设置该页内容  
    s_ebook_show_file_txt.wstr_ptr = s_ucs2_str;
    s_ebook_show_file_txt.wstr_len = page_text_length;
	
	//需要去掉结尾的换行符，否则textbox解析行数会多一行
	ClearNewlineChar(&s_ebook_show_file_txt);
	SCI_TRACE_LOW("ebook_time:PutShowTxt  end=%d",SCI_GetTickCount());
    return page_text_length;
}

/*****************************************************************************/
// 	Description : GetLineInfo
//	Global resource dependence : 
//  Author: hui.zhao
//	Note:   
/*****************************************************************************/
//通过行数来得到text,length,dir,以便显示
LOCAL BOOLEAN GetLineInfo(
                          MMIEBOOK_FILE_SEGMENT_T*    uni_show_txt_ptr,
                          int                         lineno,         //行信息
                          wchar                       *text_ptr,          //out
                          uint32                      *length_ptr       //out
                          )
{
    uint16  item_length = 0;
	if(lineno>=uni_show_txt_ptr->total_line_num || lineno < 0)
	{
		return FALSE;
	}
    
    item_length = (uint16)(uni_show_txt_ptr->line_offset_tab_ptr[lineno + 1] - uni_show_txt_ptr->line_offset_tab_ptr[lineno]);
    
    SCI_MEMCPY(text_ptr,&uni_show_txt_ptr->ucs2_str_ptr[uni_show_txt_ptr->line_offset_tab_ptr[lineno]],item_length*sizeof(wchar));
    *length_ptr = item_length;
    
    return TRUE;
}

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
                                         )
{
    int32 param = 0;
    
    //SCI_TRACE_LOW("MMIEBOOK_TASK MMIEBOOK_SendMsgtoTask type is %x",type);
    {
        param = data;
        //MMIEBOOK_SendMsgtoMMI( MMIEBOOK_TXT_SHOW_WIN_ID, type, &param,  sizeof(param) );
         MMIEbook_HandleTaskMessage(  type,  &param);
    }
}

/*****************************************************************************/
//  Description : 取得显示文本
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/ 
PUBLIC void MMIEBOOK_GetShowTxt(
                                MMI_STRING_T *mmi_string_ptr//OUT:
                                )
{
    //SCI_ASSERT(PNULL != mmi_string_ptr);/*assert verified*/
    if(PNULL == mmi_string_ptr)
    {
        return;
    }
    mmi_string_ptr->wstr_ptr = s_ebook_show_file_txt.wstr_ptr;
    mmi_string_ptr->wstr_len = s_ebook_show_file_txt.wstr_len;
}

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
  )
{
	//SCI_TRACE_LOW("MMIEBOOK_TASK MMIEBOOK_SendMsgtoMMI msg_id is %x",msg_id);
	MMK_PostMsg( handle, msg_id, param_ptr, size_of_param );
}

/*****************************************************************************/
//  Description : 电子书任务是否忙
//  Global resource dependence : MMIEBOOK_IsTaskBusy
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsTaskBusy( void )
{
  return s_mmiebook_task_is_busy;
}

/*****************************************************************************/
//  Description : MakeCurSeg
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadSegmentDataFromFile(
                    MMIEBOOK_FILE_SEGMENT_T*	    cur_show_txt_ptr        //当前段信息指针
                    )//返回失败说明有结束符
{
    int32           read_len = 0;
    uint8*          data_str_ptr = PNULL;
    int32          data_str_len = MMIEBOOK_SEG_BUF_UNIT_LEN;
    MMIFILE_HANDLE  orig_file_handle = MMIEBOOK_GetShowFileHandle();
    int32           orig_file_offset = cur_show_txt_ptr->this_seg_offset ;
    int32           uft8_start_offset = 0;
    int32           ansi_start_offset = 0;

    //SCI_TRACE_LOW:"MMIEBOOK_TASK ReadSegmentDataFromFile enter time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1208_112_2_18_2_15_21_45,(uint8*)"d", SCI_GetTickCount());
    data_str_ptr = (uint8 *)SCI_ALLOCA(MMIEBOOK_SEG_BUF_UNIT_LEN+1);
    if(PNULL == data_str_ptr)
    {
    	return FALSE;
    }
    SCI_MEMSET(data_str_ptr, 0, data_str_len+1);
    if((orig_file_offset == 0) && (GUI_CHAR_CODE_UTF8 == s_ebook_file_type))
	   orig_file_offset = 3;
    if((orig_file_offset == 0) && ((GUI_CHAR_CODE_UCS2_BE == s_ebook_file_type) || (GUI_CHAR_CODE_UCS2_LE == s_ebook_file_type)))
	   orig_file_offset = 2;
    //定位文件中的偏移量，后续增加返回值保护
    MMIEBOOK_FFS_fseek(orig_file_handle,orig_file_offset,MMIFILE_SEEK_BEGIN);//current_orig_offset,记录了当前解析到txt文件的偏移值

    //长度不相等，还要做保护处理
    read_len = MIN(data_str_len, cur_show_txt_ptr->next_seg_offset - cur_show_txt_ptr->this_seg_offset);
    
    //读取一段txt原始内容
    data_str_len = (int32)MMIEBOOK_FFS_fread(data_str_ptr, (uint32)read_len, 1,orig_file_handle );
    
    if(0 == data_str_len)
    {
	    SCI_FREE(data_str_ptr);
        return FALSE;
    }
    if( GUI_CHAR_CODE_UTF8 == s_ebook_file_type)
    {
        uft8_start_offset = GUI_FindFirstUTF8CodeStartPosition(data_str_ptr, data_str_len);
        //data_str_ptr += uft8_start_offset;
        //data_str_len -= uft8_start_offset;
        cur_show_txt_ptr->this_seg_offset += uft8_start_offset;
    }
#ifndef GBK_SUPPORT	
    else if (GUI_CHAR_CODE_ANSI == s_ebook_file_type)
    {
        ansi_start_offset = GUI_FindFirstANSICodeStartPosition(data_str_ptr, data_str_len);
        cur_show_txt_ptr->this_seg_offset += ansi_start_offset;
    }
#endif
    //转换成unicode 
    SCI_TRACE_LOW("ebook_time:GUI_OtherCodeToWstr  start=%d",SCI_GetTickCount());

    SCI_MEMSET(cur_show_txt_ptr->ucs2_str_ptr, 0, MMIEBOOK_SEG_BUF_UNIT_LEN);
    cur_show_txt_ptr->ucs2_str_len = GUI_OtherCodeToWstr(
        cur_show_txt_ptr->ucs2_str_ptr, 
        MMIEBOOK_SEG_BUF_UNIT_LEN, 
        s_ebook_file_type, 
        data_str_ptr + uft8_start_offset + ansi_start_offset, 
        data_str_len - uft8_start_offset - ansi_start_offset
        );
    SCI_TRACE_LOW("ebook_time:GUI_OtherCodeToWstr  end=%d",SCI_GetTickCount());
    if(0 == cur_show_txt_ptr->ucs2_str_len)
    {
	    SCI_FREE(data_str_ptr);
        return FALSE;
    }
    	
    SCI_FREE(data_str_ptr);
    
    if (cur_show_txt_ptr->ucs2_str_len>MMIEBOOK_SEG_BUF_UNIT_LEN)
    {
        //SCI_TRACE_LOW:"MMIEBOOK_TASK ReadSegmentDataFromFile cur_show_txt_ptr->ucs2_str_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1249_112_2_18_2_15_21_46,(uint8*)"d", cur_show_txt_ptr->ucs2_str_len);
        return FALSE;
    }
    ConversionWcharZeroToSpace(cur_show_txt_ptr->ucs2_str_ptr,cur_show_txt_ptr->ucs2_str_len);  
    //SCI_TRACE_LOW:"MMIEBOOK_TASK ReadSegmentDataFromFile leave time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1253_112_2_18_2_15_21_47,(uint8*)"d", SCI_GetTickCount());
    return TRUE;
}

/*****************************************************************************/
//  Description : MakeCurSeg
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MakeCurSeg(
                    MMIEBOOK_FILE_SEGMENT_T*	    cur_show_txt_ptr        //当前段信息指针
                    )//返回失败说明有结束符
{
     BOOLEAN         result = TRUE;
     uint16          offset = 0;    
     int32           first_delimeter_offset = 0;
     uint8*          data_str_ptr = PNULL;
     uint32          data_str_len = MMIEBOOK_SEG_BUF_UNIT_LEN; 
     
    result = LayoutSegmentLineInfo(//段做layout断行
         cur_show_txt_ptr,
         MMIEBOOK_GetShowFileHandle(),
        (uint16) cur_show_txt_ptr->this_seg_offset,
        (uint32)g_ebook_show_file_brief_ptr->file_size,
         FALSE,
         FALSE
         );
    
     //断行到文件结束。
     if(cur_show_txt_ptr->next_seg_offset == g_ebook_show_file_brief_ptr->file_size)    
     {
         return TRUE;
     }
    
     //本段只有一行，则最后一行不能丢弃
     if (cur_show_txt_ptr->total_line_num == 1)
     {
         return TRUE;
     }
     
     //本段最后一行的处理
     first_delimeter_offset = FindFirstParagraphSperator(
         &cur_show_txt_ptr->ucs2_str_ptr[cur_show_txt_ptr->line_offset_tab_ptr[cur_show_txt_ptr->total_line_num - 1]],
         cur_show_txt_ptr->ucs2_str_len - cur_show_txt_ptr->line_offset_tab_ptr[cur_show_txt_ptr->total_line_num - 1] + 1,
         0);
         
         //not find any delimeter
         if(first_delimeter_offset ==  cur_show_txt_ptr->ucs2_str_len - cur_show_txt_ptr->line_offset_tab_ptr[cur_show_txt_ptr->total_line_num - 1] + 1)
        {
             cur_show_txt_ptr->total_line_num -= 1;//将最后一行视为不完整行，舍弃
         }    
        
         data_str_ptr = (uint8 *)SCI_ALLOCA(data_str_len+1);
         if(PNULL == data_str_ptr)
         {
             return FALSE;
         }
        SCI_MEMSET(data_str_ptr, 0 , data_str_len + 1);
         cur_show_txt_ptr->ucs2_str_len = cur_show_txt_ptr->line_offset_tab_ptr[cur_show_txt_ptr->total_line_num];
         //计算下一段在文本中的起始地址
         offset = GUI_WstrToOtherCode(
             s_ebook_file_type, 
             data_str_ptr, 
             data_str_len, 
             cur_show_txt_ptr->ucs2_str_ptr, 
             (uint32)(cur_show_txt_ptr->ucs2_str_len)//该行的结束位置
             );      
    
     cur_show_txt_ptr->next_seg_offset = cur_show_txt_ptr->this_seg_offset + offset;   
     
     SCI_FREE(data_str_ptr);
     return result;
}

/*****************************************************************************/
//  Description : MakeLineTabInSeg
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MakePreSeg(void)//返回失败说明有结束符
{   
    int32           first_delimeter_offset = 0;
    int32           temp_cur_offset =  s_ebook_show_info.pre_show_txt_ptr->next_seg_offset ;
    BOOLEAN         result = FALSE;

    //SCI_TRACE_LOW:"MMIEBOOK_task MakePreSeg enter time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1336_112_2_18_2_15_21_48,(uint8*)"d", SCI_GetTickCount());
    // 0 offset jump, there is no previous segment
    if(0 == temp_cur_offset)
    {
        s_ebook_show_info.pre_show_txt_ptr->next_seg_offset = 0;
        return FALSE;
    }

    //in case to the file head
    if (temp_cur_offset - MMIEBOOK_SEG_BUF_UNIT_LEN > 0)
    {
        temp_cur_offset -= MMIEBOOK_SEG_BUF_UNIT_LEN;
    }
    else
    {
        temp_cur_offset = 0;
    }
    s_ebook_show_info.pre_show_txt_ptr->this_seg_offset = temp_cur_offset;
    
    //read file 
    if(!ReadSegmentDataFromFile(s_ebook_show_info.pre_show_txt_ptr))
    {
        return FALSE;
    }
    
    if(0 != s_ebook_show_info.pre_show_txt_ptr->this_seg_offset)
    {
        //find first delimeter offset  
        first_delimeter_offset = FindFirstParagraphSperator(
            s_ebook_show_info.pre_show_txt_ptr->ucs2_str_ptr,
            s_ebook_show_info.pre_show_txt_ptr->ucs2_str_len,0);

         //not find any delimeter
        if(first_delimeter_offset == s_ebook_show_info.pre_show_txt_ptr->ucs2_str_len)
        {
            first_delimeter_offset = 0;   
        }
        //first_delimeter_offset转换为实际文件的offset
        if(first_delimeter_offset != 0)
        {
            int32          offset = 0;
            uint8*         data_str_ptr = PNULL;
            uint32          data_str_len = MMIEBOOK_SEG_BUF_UNIT_LEN;
            
            data_str_ptr = (uint8 *)SCI_ALLOCA((data_str_len+1)*sizeof(wchar));
            if(PNULL == data_str_ptr)
            {
                return FALSE;
            }
            SCI_MEMSET(data_str_ptr, 0, (data_str_len+1)*sizeof(wchar));
            offset = GUI_WstrToOtherCode(
                s_ebook_file_type, 
                data_str_ptr, 
                data_str_len, 
                s_ebook_show_info.pre_show_txt_ptr->ucs2_str_ptr, 
                (uint32)MIN(first_delimeter_offset, MMIEBOOK_SEG_BUF_UNIT_LEN)//该行的结束位置
                );      
            //上一次只用了first_delimeter_offset的长度
            SCI_MEMSET(data_str_ptr, 0, (uint32)first_delimeter_offset+1);
            
            //调整本段在原始文件中的起始offset，以及数据长度
            s_ebook_show_info.pre_show_txt_ptr->this_seg_offset += offset;
            s_ebook_show_info.pre_show_txt_ptr->ucs2_str_len -= first_delimeter_offset;

            //move memory
            SCI_MEMCPY(data_str_ptr, 
                        &s_ebook_show_info.pre_show_txt_ptr->ucs2_str_ptr[first_delimeter_offset],
                        (uint32)s_ebook_show_info.pre_show_txt_ptr->ucs2_str_len*sizeof(wchar));       

            //FREE old buf, use new buf
            SCI_FREE(s_ebook_show_info.pre_show_txt_ptr->ucs2_str_ptr);
            s_ebook_show_info.pre_show_txt_ptr->ucs2_str_ptr = (wchar*)data_str_ptr;            
          }
    }
    result = MakeCurSeg(s_ebook_show_info.pre_show_txt_ptr);

    //pre seg line num < line in one page,move to next segment
    if(s_ebook_show_info.pre_show_txt_ptr->total_line_num <= 1)
    {        
        s_ebook_show_info.pre_show_txt_ptr->next_seg_offset = 
                    s_ebook_show_info.pre_show_txt_ptr->this_seg_offset;
        s_ebook_show_info.pre_show_txt_ptr->total_line_num = 0;
        s_ebook_show_info.pre_show_txt_ptr->ucs2_str_len = 0;        
        SCI_MEMSET(s_ebook_show_info.pre_show_txt_ptr->ucs2_str_ptr, 0, MMIEBOOK_SEG_BUF_UNIT_LEN);
    }
    //SCI_TRACE_LOW:"MMIEBOOK_task MakePreSeg leave time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1410_112_2_18_2_15_22_49,(uint8*)"d", SCI_GetTickCount());
    return result;
}

/*****************************************************************************/
//  Description : MakeNextSeg
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MakeNextSeg(void)//返回失败说明有结束符
{    
    int32 temp_cur_offset = s_ebook_show_info.next_show_txt_ptr->this_seg_offset;
    BOOLEAN result = FALSE;
    
    //SCI_TRACE_LOW:"MMIEBOOK_task MakeNextSeg enter time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1423_112_2_18_2_15_22_50,(uint8*)"d", SCI_GetTickCount());
    //100% offset jump
    if(temp_cur_offset == g_ebook_show_file_brief_ptr->file_size)
    {
        return FALSE;
    }
    
    //in case to the file tail
    if (temp_cur_offset + MMIEBOOK_SEG_BUF_UNIT_LEN < g_ebook_show_file_brief_ptr->file_size)
    {
        temp_cur_offset += MMIEBOOK_SEG_BUF_UNIT_LEN;
    }
    else
    {
        temp_cur_offset = g_ebook_show_file_brief_ptr->file_size;
    }    
    s_ebook_show_info.next_show_txt_ptr->next_seg_offset = temp_cur_offset; 

    //read file 
    if(!ReadSegmentDataFromFile(s_ebook_show_info.next_show_txt_ptr))
    {
        return FALSE;
    }
    
    result = MakeCurSeg(s_ebook_show_info.next_show_txt_ptr);
    //SCI_TRACE_LOW:"MMIEBOOK_task MakeNextSeg leave time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1448_112_2_18_2_15_22_51,(uint8*)"d", SCI_GetTickCount());
    return result;
}

/*****************************************************************************/
//  Description : ReMakeSegmentAfterScreenSwitch
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReMakeSegmentAfterScreenSwitch( void )
{
    int32          offset = 0;
    uint8*          data_str_ptr = PNULL;
    uint32          data_str_len = MMIEBOOK_SEG_BUF_UNIT_LEN;
    int16          cur_line_offset = 0;
    
    //SCI_TRACE_LOW:"MMIEBOOK_task ReMakeSegmentAfterScreenSwitch enter time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1463_112_2_18_2_15_22_52,(uint8*)"d", SCI_GetTickCount());
    data_str_ptr = (uint8 *)SCI_ALLOCA(data_str_len+1);
    if(PNULL == data_str_ptr)   /*lint !e774 */
    {
        return FALSE;
    }
    SCI_MEMSET(data_str_ptr, 0, data_str_len+1); /*lint !e668 */

    //调整本段在原始文件中的起始offset 
    cur_line_offset = s_ebook_show_info.cur_show_txt_ptr->line_offset_tab_ptr[s_ebook_show_info.cur_line];
    offset = GUI_WstrToOtherCode(
        s_ebook_file_type, 
        data_str_ptr, 
        data_str_len, 
        s_ebook_show_info.cur_show_txt_ptr->ucs2_str_ptr, 
        (uint32)MIN(cur_line_offset, MMIEBOOK_SEG_BUF_UNIT_LEN)//该行的结束位置
        );      

    SCI_FREE(data_str_ptr);
    s_ebook_show_info.cur_show_txt_ptr->this_seg_offset += offset;    
  
    return TRUE;
}

/*****************************************************************************/
//  Description : SwitchAndMakePreSegment
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SwitchAndMakePreSegment( void )
{  
    MMIEBOOK_FILE_SEGMENT_T* temp_pre_show_txt_ptr = 0;
    //SCI_TRACE_LOW:"MMIEBOOK_task SwitchAndMakePreSegment enter time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1494_112_2_18_2_15_22_53,(uint8*)"d", SCI_GetTickCount());
    temp_pre_show_txt_ptr = s_ebook_show_info.next_show_txt_ptr;
    s_ebook_show_info.next_show_txt_ptr = s_ebook_show_info.pre_show_txt_ptr;   
    s_ebook_show_info.pre_show_txt_ptr = temp_pre_show_txt_ptr;            

    ClearFileSegmentInfo(s_ebook_show_info.pre_show_txt_ptr);    
   
    
    s_ebook_show_info.pre_show_txt_ptr->next_seg_offset = 
        s_ebook_show_info.next_show_txt_ptr->this_seg_offset; 

    MakePreSeg();

    //adjust display line
    s_ebook_show_info.cur_line = s_ebook_show_info.one_page_lines - s_ebook_show_info.cur_line - 1;
    if(s_ebook_show_info.pre_show_txt_ptr->total_line_num < s_ebook_show_info.cur_line)
    {
        s_ebook_show_info.cur_line = 0;
    }
    else
    {
        s_ebook_show_info.cur_line = s_ebook_show_info.pre_show_txt_ptr->total_line_num - s_ebook_show_info.cur_line - 1;
    }
    s_ebook_show_info.cur_show_txt_ptr = s_ebook_show_info.pre_show_txt_ptr;

    //so far, the same process as turning page down
    
    // the last line in pre segment was dropped down
    if(s_ebook_show_info.pre_show_txt_ptr->next_seg_offset 
        < s_ebook_show_info.next_show_txt_ptr->this_seg_offset)
    {
        //adjust the display line
        s_ebook_show_info.cur_line += 1;
        if(s_ebook_show_info.cur_line == s_ebook_show_info.pre_show_txt_ptr->total_line_num)
        {
            //turn to next segment
            s_ebook_show_info.cur_line = 0;
            s_ebook_show_info.cur_show_txt_ptr = s_ebook_show_info.next_show_txt_ptr;
        }
        //adjust the next segment this seg offset and make next segment
        s_ebook_show_info.next_show_txt_ptr->this_seg_offset = s_ebook_show_info.pre_show_txt_ptr->next_seg_offset;
        MakeNextSeg();
    }
    //SCI_TRACE_LOW:"MMIEBOOK_task SwitchAndMakePreSegment leave time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1537_112_2_18_2_15_22_54,(uint8*)"d", SCI_GetTickCount());
    return TRUE;
}

/*****************************************************************************/
//  Description : SwitchAndMakeNextSegment
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SwitchAndMakeNextSegment( void )
{
    MMIEBOOK_FILE_SEGMENT_T* temp_pre_show_txt_ptr = 0;
    //SCI_TRACE_LOW:"MMIEBOOK_task SwitchAndMakeNextSegment enter time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1548_112_2_18_2_15_22_55,(uint8*)"d", SCI_GetTickCount());
    temp_pre_show_txt_ptr = s_ebook_show_info.pre_show_txt_ptr;
    s_ebook_show_info.pre_show_txt_ptr = s_ebook_show_info.next_show_txt_ptr;   
    s_ebook_show_info.next_show_txt_ptr = temp_pre_show_txt_ptr;            

    ClearFileSegmentInfo(s_ebook_show_info.next_show_txt_ptr);
    
    s_ebook_show_info.next_show_txt_ptr->this_seg_offset = 
            s_ebook_show_info.pre_show_txt_ptr->next_seg_offset;    
            
    MakeNextSeg();
    
    //adjust current segment
    s_ebook_show_info.cur_show_txt_ptr = s_ebook_show_info.pre_show_txt_ptr;
    //SCI_TRACE_LOW:"MMIEBOOK_task SwitchAndMakeNextSegment leave time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1562_112_2_18_2_15_22_56,(uint8*)"d", SCI_GetTickCount());
    return TRUE;
}

/*****************************************************************************/
//  Description : ClearFileSegmentInfo
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL void ClearFileSegmentInfo(MMIEBOOK_FILE_SEGMENT_T*    ebook_seg_ptr)
{
    //SCI_TRACE_LOW:"MMIEBOOK_task ClearFileSegmentInfo enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1572_112_2_18_2_15_22_57,(uint8*)"");
    ebook_seg_ptr->this_seg_offset = 0;
    ebook_seg_ptr->next_seg_offset = 0;
    
    
    SCI_MEMSET(ebook_seg_ptr->ucs2_str_ptr,0,((MMIEBOOK_SEG_BUF_UNIT_LEN + 1)*sizeof(wchar)));
    ebook_seg_ptr->ucs2_str_len = 0;
    SCI_MEMSET(ebook_seg_ptr->line_offset_tab_ptr,0,(MMIEBOOK_SEG_BUF_UNIT_LEN + 1));
    ebook_seg_ptr->total_line_num = 0;   
}

/*****************************************************************************/
//  Description : ShowCurPercent
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_ShowCurPercent( 
                           MMI_CTRL_ID_T ctrl_id                           
                           )
{
    int32 cur_percent = 0;
    uint8           cur_percent_str[16] = {0};
    wchar           wchar_cur_percent_str[16] = {0};  
    MMI_STRING_T    cur_percent_s = {0};
    int32           offset = 0;
    
     //当翻到电子书最后一页时，处理显示成100%，否则都按照当前页首行在电子书中的偏移百分比来显示
    if(MMIEBOOK_IsInLastPage()) 
    {
        cur_percent = 100;
    }
    else
    {
        int16 line=s_ebook_show_info.cur_line;
        if(s_ebook_show_info.cur_line+1<=s_ebook_show_info.cur_show_txt_ptr->total_line_num)
        {
           line+=1; 
        }
        offset = MMIEBOOK_ConvertWstrOffsetToOtherCodeOffset(
            s_ebook_show_info.cur_show_txt_ptr->ucs2_str_ptr,
           (uint16)( s_ebook_show_info.cur_show_txt_ptr->line_offset_tab_ptr[line])
            );
        
       s_ebook_show_info.cur_read_offset = s_ebook_show_info.cur_show_txt_ptr->this_seg_offset + offset;
        
        cur_percent = (int32)((int64)s_ebook_show_info.cur_read_offset * 100/(int64)g_ebook_show_file_brief_ptr->file_size);
        
        if(cur_percent>100)
        {
            cur_percent = 100;
        }
    }   


    //显示页码
    sprintf((char *)cur_percent_str, "%d%%", cur_percent);
    cur_percent_s.wstr_len = (uint16)strlen((char *)cur_percent_str); 
    MMI_STRNTOWSTR( wchar_cur_percent_str, 15, (const uint8*)cur_percent_str, 15, cur_percent_s.wstr_len );
    cur_percent_s.wstr_ptr = wchar_cur_percent_str;
    
    GUILABEL_SetText(ctrl_id,&cur_percent_s,FALSE/*need_update*/);    
}

/*****************************************************************************/
//  Description : ShowCurPercent
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_SetPercent( 
                                uint32 percent
                                )
{
    s_ebook_show_info.percent = percent;
}

/*****************************************************************************/
//  Description : InitEbookShowRectInfo
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL void InitEbookShowRectInfo( MMI_WIN_ID_T win_id )
{
    GUI_RECT_T     client_rect = {0};
    MMIEBOOK_FONT_SIZE_E font_size = MMIEBOOK_FONT_SIZE_MAX;
    //SCI_TRACE_LOW:"MMIEBOOK_task InitEbookShowRectInfo enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1656_112_2_18_2_15_22_58,(uint8*)"");

    font_size = MMIAPIEBOOK_GetFontSize();

    s_show_rect_info.font = MMIEBOOK_ConvertFontSize(font_size);
    
    if(MMIEBOOK_GetIsFullScreenOff())
    {
        client_rect = MMITHEME_GetClientRect();
        s_show_rect_info.txt_show_bottom = (uint16)(client_rect.bottom - MMIEBOOK_COUNT_BAR_HEIGHT);
    }
    else
    {
        client_rect = MMITHEME_GetFullScreenRect();
        s_show_rect_info.txt_show_bottom = (uint16)client_rect.bottom ;
    }

    s_show_rect_info.txt_show_left = 0;
    s_show_rect_info.txt_show_top = 0;
    s_show_rect_info.txt_show_right = (uint16)client_rect.right;
    
    if (MMITHEME_IsMainScreenLandscape())
    {
        s_txt_direction=MMIEBOOK_TXT_DIRECTION_H;
    }
    else
    {

        s_txt_direction=MMIEBOOK_TXT_DIRECTION_V;
    }    
    
    s_show_rect_info.txt_show_line_space = MMITHEME_TEXT_LINE_SPACE;
    s_show_rect_info.txt_show_char_space =  MMITHEME_TEXT_CHAR_SPACE;
}

/*****************************************************************************/
//  Description : MMIEbook_GetFont
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
PUBLIC GUI_FONT_T* MMIEbook_GetFont(void)
{
    //SCI_TRACE_LOW:"MMIEBOOK_TASK MMIEbook_GetFont font is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1697_112_2_18_2_15_22_59,(uint8*)"d",s_show_rect_info.font);
    return (GUI_FONT_T*)&s_show_rect_info.font;
}

/*****************************************************************************/
//  Description : IsLayoutLastPageOffset
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsInLastPage(void)//判断是否已是电子书最后一页
{
    BOOLEAN result = FALSE;
    /*if((s_ebook_show_info.cur_show_txt_ptr->next_seg_offset)==(g_ebook_show_file_brief_ptr->file_size))
    {
    	if(s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines >=s_ebook_show_info.cur_show_txt_ptr->total_line_num)
    	{	   
    	    result =TRUE;	  		
    	}
	}*/
	if(s_ebook_show_info.cur_show_txt_ptr == s_ebook_show_info.pre_show_txt_ptr)
    {
        if (s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines >= s_ebook_show_info.cur_show_txt_ptr->total_line_num)
        {
            if(s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines 
                - s_ebook_show_info.cur_show_txt_ptr->total_line_num + 1 > s_ebook_show_info.next_show_txt_ptr->total_line_num)
            {
                result =TRUE;
            }
        }

    }
    else
    {       
        if (s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines >= s_ebook_show_info.cur_show_txt_ptr->total_line_num)
        {
            if (s_ebook_show_info.cur_show_txt_ptr->next_seg_offset == g_ebook_show_file_brief_ptr->file_size)
            {
                result =TRUE;
            }
        }
    }
    //SCI_TRACE_LOW:"MMIEBOOK_TASK MMIEBOOK_IsInLastPage == %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1737_112_2_18_2_15_22_60,(uint8*)"d",result);
	return result;
}

/*****************************************************************************/
//  Description : IsLayoutLastPageOffset
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsFirstPage(void)
{
    BOOLEAN result = FALSE;
    
    if (s_ebook_show_info.cur_show_txt_ptr->this_seg_offset == 0)
    {
        if (s_ebook_show_info.cur_line == 0)
        {
            result =TRUE;
        }
    }

    //SCI_TRACE_LOW:"MMIEBOOK_TASK MMIEBOOK_IsInFirstPage == %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1757_112_2_18_2_15_22_61,(uint8*)"d",result);
	return result;
}

//书签部分
/*****************************************************************************/
//  Description : 添加当前页的首行作为书签
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:FALSE->书签满
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_AddBookmark( void )
{
    uint16      offset = 0;
    uint16      line_length = 0;
    int32       bookmark_offset = 0;
    MMIEBOOK_FILE_SEGMENT_T*    ebook_seg_ptr = PNULL;
    uint8*      data_str_ptr = PNULL;
    uint32      data_str_len = MMIEBOOK_SEG_BUF_UNIT_LEN;
    wchar       bookmark_file_full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
	BOOLEAN result=FALSE;
    
    //SCI_ASSERT (PNULL != s_ebook_show_info.cur_show_txt_ptr);/*assert verified*/
    //SCI_PASSERT( s_ebook_show_info.cur_line <= s_ebook_show_info.cur_show_txt_ptr->total_line_num,
    //("MMIEBOOK_AddBookmark:s_ebook_show_info.cur_line is %d,s_ebook_show_info.cur_show_txt_ptr->total_line_num is %d",
    //s_ebook_show_info.cur_line,s_ebook_show_info.cur_show_txt_ptr->total_line_num));/*assert verified*/
    //SCI_ASSERT(PNULL != g_ebook_bookmark_name_tab);/*assert verified*/
    
    if (PNULL == s_ebook_show_info.cur_show_txt_ptr ||PNULL ==g_ebook_bookmark_name_tab)
    {
        //SCI_TRACE_LOW:"MMIEBOOK_AddBookmark show_ptr=%d,tab=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1786_112_2_18_2_15_22_62,(uint8*)"dd",s_ebook_show_info.cur_show_txt_ptr,g_ebook_bookmark_name_tab);
        return FALSE;
    }
    if (s_ebook_show_info.cur_line >s_ebook_show_info.cur_show_txt_ptr->total_line_num)
    {
        //SCI_TRACE_LOW:"MMIEBOOK_AddBookmark:s_ebook_show_info.cur_line is %d,s_ebook_show_info.cur_show_txt_ptr->total_line_num is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1792_112_2_18_2_15_22_63,(uint8*)"dd",s_ebook_show_info.cur_line,s_ebook_show_info.cur_show_txt_ptr->total_line_num);
        return FALSE;
    }
    if (s_ebook_show_info.bookmark_info.total_num >= MMIEBOOK_BOOKMARK_MAX_NUM)//书签数已满
    {
        //SCI_TRACE_LOW:"MMIEBOOK_AddBookmark s_ebook_show_info.bookmark_info.total_num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1797_112_2_18_2_15_22_64,(uint8*)"d",s_ebook_show_info.bookmark_info.total_num);
        return FALSE;
    }
    
    //计算当前书签所在文件中的偏移值
    ebook_seg_ptr = s_ebook_show_info.cur_show_txt_ptr;
    data_str_ptr = (uint8 *)SCI_ALLOC_APP(data_str_len+1);

    SCI_MEMSET(data_str_ptr, 0, data_str_len+1);
    
    offset = GUI_WstrToOtherCode(
        s_ebook_file_type, 
        data_str_ptr, 
        data_str_len, 
        ebook_seg_ptr->ucs2_str_ptr, 
        (uint32)(ebook_seg_ptr->line_offset_tab_ptr[s_ebook_show_info.cur_line])
        );
    
    bookmark_offset = ebook_seg_ptr->this_seg_offset + offset;
    
    //找到详情中书签列表插入位置
    /*for (insert_pos = 0; insert_pos < s_ebook_show_info.bookmark_info.total_num; insert_pos ++)
    {
        if (bookmark_offset == s_ebook_show_info.bookmark_info.offset_tab[insert_pos])
        {
            //书签已经存在
            SCI_FREE(data_str_ptr);
            return TRUE;
        }
        else if (bookmark_offset < s_ebook_show_info.bookmark_info.offset_tab[insert_pos])
        {
            //找到位置了
            break;
        }
        else
        {
        }
    }*/
    
//    g_ebook_new_bookmark_pos = insert_pos;
//    MMINV_WRITE(MMINV_EBOOK_NEW_BOOKMARK, &g_ebook_new_bookmark_pos);
    
    //将要插入位置之后(包括)的书签后移
    /*for (i = s_ebook_show_info.bookmark_info.total_num; ((i > insert_pos)&&(i>=1)); i --)
    {
        s_ebook_show_info.bookmark_info.offset_tab[i] = s_ebook_show_info.bookmark_info.offset_tab[i - 1];
        MMI_WSTRNCPY(g_ebook_bookmark_name_tab[i].name_wstr, MMIEBOOK_BOOKMARK_NAME_LEN,g_ebook_bookmark_name_tab[i - 1].name_wstr, MMIEBOOK_BOOKMARK_NAME_LEN,MMIEBOOK_BOOKMARK_NAME_LEN);
        g_ebook_bookmark_name_tab[i].name_len = g_ebook_bookmark_name_tab[i - 1].name_len;
    }*/

    line_length = MMIEBOOK_BOOKMARK_NAME_LEN;

    SCI_MEMSET(g_ebook_bookmark_name_tab[s_ebook_show_info.bookmark_info.total_num].name_wstr, 0, (MMIEBOOK_BOOKMARK_NAME_LEN*sizeof(wchar)));
    
    MMI_WSTRNCPY(
	g_ebook_bookmark_name_tab[s_ebook_show_info.bookmark_info.total_num].name_wstr, MMIEBOOK_BOOKMARK_NAME_LEN, 
	&s_ebook_show_info.cur_show_txt_ptr->ucs2_str_ptr[s_ebook_show_info.cur_show_txt_ptr->line_offset_tab_ptr[s_ebook_show_info.cur_line]], MMIEBOOK_BOOKMARK_NAME_LEN, 
	MMIEBOOK_BOOKMARK_NAME_LEN);
    
    //去除最后的回车换行，防止书签显示时滚动
    line_length = MIN(line_length,FindLastParagraphSperator(g_ebook_bookmark_name_tab[s_ebook_show_info.bookmark_info.total_num].name_wstr,line_length,0));/*lint !e666*/
 
    g_ebook_bookmark_name_tab[s_ebook_show_info.bookmark_info.total_num].name_len = line_length;
    
    //保存书签的页面和在文本中的偏移量
    
    s_ebook_show_info.bookmark_info.offset_tab[s_ebook_show_info.bookmark_info.total_num] = bookmark_offset;
	s_ebook_show_info.bookmark_info.total_num ++;

	MMIEBOOK_GetBookmarkFileName(g_ebook_show_file_brief_ptr->full_name_wstr,bookmark_file_full_path);
	if(!MMIEBOOK_OpenBookmarkFile(bookmark_file_full_path,"r+"))
	{
		MMIEBOOK_CloseBookmarkFile();
		return FALSE;
	}
    result=MMIEBOOK_WriteBookmarkFile(MMIEBOOK_GetBookmarkFileHandle(),MMIEBOOK_BOOKMARK_FILE_INFO,&s_ebook_show_info.bookmark_info);
	MMIEBOOK_CloseBookmarkFile();
	if(!result)
	{
		s_ebook_show_info.bookmark_info.total_num --;
		SCI_MEMSET(g_ebook_bookmark_name_tab[s_ebook_show_info.bookmark_info.total_num].name_wstr, 0, (MMIEBOOK_BOOKMARK_NAME_LEN*sizeof(wchar)));
		s_ebook_show_info.bookmark_info.offset_tab[s_ebook_show_info.bookmark_info.total_num] = 0;  
		g_ebook_bookmark_name_tab[s_ebook_show_info.bookmark_info.total_num].name_len = 0;  
	}
	
    SCI_FREE(data_str_ptr);
    return result;
}

/*****************************************************************************/
//  Description : 删除选择的书签
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIEBOOK_DeleteBookmark(
                                 uint16 cur_select//IN: 书签列表窗口中的位置
                                 )                                
{
    uint16 i = 0;
    wchar              bookmark_file_full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    SCI_PASSERT(cur_select < s_ebook_show_info.bookmark_info.total_num, 
		("MMIEBOOK_DeleteBookmark: cur_select is %d,s_ebook_show_info.bookmark_info.total_num is %d",cur_select,s_ebook_show_info.bookmark_info.total_num));/*assert verified*/		
    SCI_ASSERT(PNULL != g_ebook_bookmark_name_tab);/*assert verified*/


	cur_select=s_ebook_show_info.bookmark_info.total_num-1-cur_select;
    //将要删除的书签清0
    SCI_MEMSET(g_ebook_bookmark_name_tab[cur_select].name_wstr, 0, (MMIEBOOK_BOOKMARK_NAME_LEN*sizeof(wchar)));
    
    s_ebook_show_info.bookmark_info.total_num --;
    
    //将要删除位置之后(包括)的书签前移
    for (i = cur_select; i < s_ebook_show_info.bookmark_info.total_num; i ++)
    {
        s_ebook_show_info.bookmark_info.offset_tab[i] = s_ebook_show_info.bookmark_info.offset_tab[i + 1];
	    MMI_WSTRNCPY(g_ebook_bookmark_name_tab[i].name_wstr, MMIEBOOK_BOOKMARK_NAME_LEN,
	 	g_ebook_bookmark_name_tab[i + 1].name_wstr, MMIEBOOK_BOOKMARK_NAME_LEN,
	 	MMIEBOOK_BOOKMARK_NAME_LEN);
        g_ebook_bookmark_name_tab[i].name_len = g_ebook_bookmark_name_tab[i+1].name_len;
    }
    
    //将原先最后一个书签占用的信息清零
    s_ebook_show_info.bookmark_info.offset_tab[s_ebook_show_info.bookmark_info.total_num] = 0;
    SCI_MEMSET(g_ebook_bookmark_name_tab[s_ebook_show_info.bookmark_info.total_num].name_wstr, 0, (MMIEBOOK_BOOKMARK_NAME_LEN*sizeof(wchar)));
    g_ebook_bookmark_name_tab[s_ebook_show_info.bookmark_info.total_num].name_len = 0;    

    MMIEBOOK_GetBookmarkFileName(g_ebook_show_file_brief_ptr->full_name_wstr,bookmark_file_full_path);
    if(MMIEBOOK_OpenBookmarkFile(bookmark_file_full_path,"r+"))
    {
        MMIEBOOK_WriteBookmarkFile(MMIEBOOK_GetBookmarkFileHandle(),MMIEBOOK_BOOKMARK_FILE_INFO,&s_ebook_show_info.bookmark_info);
        MMIEBOOK_CloseBookmarkFile();
    }
}

/*****************************************************************************/
//  Description : 删除所有书签
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIEBOOK_DeleteAllBookmark( void )
{
    wchar bookmark_file_full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    SCI_ASSERT(PNULL != g_ebook_bookmark_name_tab);/*assert verified*/
    
    SCI_MEMSET(g_ebook_bookmark_name_tab, 0, (sizeof(MMIEBOOK_BOOKMARK_NAME_T) * MMIEBOOK_BOOKMARK_MAX_NUM));
    SCI_MEMSET(&s_ebook_show_info.bookmark_info, 0, (sizeof(MMIEBOOK_BOOKMARK_T)));
    
	MMIEBOOK_GetBookmarkFileName(g_ebook_show_file_brief_ptr->full_name_wstr,bookmark_file_full_path);
	if(MMIEBOOK_OpenBookmarkFile(bookmark_file_full_path,"r+"))
    {
        MMIEBOOK_WriteBookmarkFile(MMIEBOOK_GetBookmarkFileHandle(),MMIEBOOK_BOOKMARK_FILE_INFO,&s_ebook_show_info.bookmark_info);
        MMIEBOOK_CloseBookmarkFile();
    }
}

/*****************************************************************************/
//  Description : 获得电子书显示信息
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/    
PUBLIC MMIEBOOK_SHOW_INFO_T* MMIEBOOK_GetShowInfo( void )
{
    return &s_ebook_show_info;
}

/*****************************************************************************/
//  Description : InitFileSegmentInfo
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL void InitFileSegmentInfo( void )
{
    //SCI_TRACE_LOW:"MMIEBOOK_TASK InitFileSegmentInfo enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1968_112_2_18_2_15_23_65,(uint8*)"");
    ClearFileSegmentInfo(&s_ebook_seg_part1);
    ClearFileSegmentInfo(&s_ebook_seg_part2);
    
    s_ebook_show_info.pre_show_txt_ptr = &s_ebook_seg_part1;
    s_ebook_show_info.cur_show_txt_ptr = &s_ebook_seg_part2;
    s_ebook_show_info.next_show_txt_ptr = &s_ebook_seg_part2;
}

/*****************************************************************************/
//  Description : MakeSegmentByOffset
//  Global resource dependence : 
//  Author:hui.zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MakeSegmentByOffset( MMIEBOOK_MSG_TYPE_E msg )
{   
    int32  temp_cur_offset = 0;
    //SCI_TRACE_LOW:"MMIEBOOK_task MakeSegmentByOffset msg == %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1984_112_2_18_2_15_23_66,(uint8*)"d", msg);
    switch(msg)
    {
        case MMIEBOOK_MSG_TURNTO_PERCENT://计算百分比
            temp_cur_offset = (int32)(((int64)s_ebook_show_info.percent*(int64)g_ebook_show_file_brief_ptr->file_size)/100);
            break;

        case MMIEBOOK_MSG_TURNTO_OFFSET:
            temp_cur_offset = s_ebook_show_info.cur_show_txt_ptr->this_seg_offset;
            break;

        default:
            //SCI_PASSERT(0, ("MakeSegmentByOffset:msg is %d",msg));/*assert verified*/
            //SCI_TRACE_LOW:"MakeSegmentByOffset:msg is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_1997_112_2_18_2_15_23_67,(uint8*)"d",msg);
            break;            
    }

    InitFileSegmentInfo();

	//unicode编码时避免跳到半字，utf8的没法保证
	if(GUI_CHAR_CODE_UCS2_LE==s_ebook_file_type || GUI_CHAR_CODE_UCS2_BE==s_ebook_file_type)
	{
		if(0!=(temp_cur_offset & 0x01))
		{
			if(temp_cur_offset>0)
			{
				temp_cur_offset-=1;
			}
			else
			{
				temp_cur_offset=0;
			}
		}
	}

    s_ebook_show_info.pre_show_txt_ptr->next_seg_offset = temp_cur_offset;
    

    MakePreSeg();
    
    //设置下一段断行条件
    s_ebook_show_info.next_show_txt_ptr->this_seg_offset = s_ebook_show_info.pre_show_txt_ptr->next_seg_offset ;
      
    MakeNextSeg();
    

    //jump to end of file
    if(s_ebook_show_info.pre_show_txt_ptr->next_seg_offset == g_ebook_show_file_brief_ptr->file_size)
    {
        s_ebook_show_info.cur_show_txt_ptr = s_ebook_show_info.pre_show_txt_ptr;
       
        if(s_ebook_show_info.cur_show_txt_ptr->total_line_num > s_ebook_show_info.one_page_lines)
        {
            s_ebook_show_info.cur_line =  
                s_ebook_show_info.cur_show_txt_ptr->total_line_num -  s_ebook_show_info.one_page_lines;           
        }
        else
        {
            s_ebook_show_info.cur_line =  0;  //only one page
        }
    }
		else
		{
            s_ebook_show_info.cur_show_txt_ptr = s_ebook_show_info.next_show_txt_ptr;
            //line 0 is the first line
            s_ebook_show_info.cur_line = 0;  
            if(msg==MMIEBOOK_MSG_TURNTO_OFFSET)
            {
                int32          offset = 0;
                uint8*         data_str_ptr = PNULL;
                uint32          data_str_len = MMIEBOOK_SEG_BUF_UNIT_LEN;
                
                data_str_ptr = (uint8 *)SCI_ALLOCA((data_str_len+1)*sizeof(wchar));
                if(PNULL == data_str_ptr)
                {
                    return FALSE;
                }
                SCI_MEMSET(data_str_ptr, 0, (data_str_len+1)*sizeof(wchar));
                offset = GUI_WstrToOtherCode(
                    s_ebook_file_type, 
                    data_str_ptr, 
                    data_str_len, 
                    s_ebook_show_info.cur_show_txt_ptr->ucs2_str_ptr, 
                    (uint32)MIN(s_ebook_show_info.cur_show_txt_ptr->line_offset_tab_ptr[1], MMIEBOOK_SEG_BUF_UNIT_LEN)//该行的结束位置
                    ); 
                if(temp_cur_offset>=(s_ebook_show_info.cur_show_txt_ptr->this_seg_offset+offset))
                {
                   s_ebook_show_info.cur_line = 1;  
                }
                SCI_FREE(data_str_ptr);
           }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : 产生书签列表
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void MakeBookmarkNameTab(void)
{
    uint16 i = 0;
    uint16 ucs2_str_len = 0;
    MMIFILE_HANDLE file_handle = PNULL;

    SCI_ASSERT(PNULL != g_ebook_bookmark_name_tab);/*assert verified*/

    file_handle = MMIEBOOK_GetShowFileHandle();
    //SCI_ASSERT(PNULL != file_handle);/*assert verified*/
    if(PNULL == file_handle)
    {
    	return;
    }
    
    if (0 == s_ebook_show_info.bookmark_info.total_num)
    {
        //没有书签
        return;
    }
    
    SCI_MEMSET(g_ebook_bookmark_name_tab, 0, (sizeof(MMIEBOOK_BOOKMARK_NAME_T) * MMIEBOOK_BOOKMARK_MAX_NUM));
    
    //为书签列表的内容赋值
    for (i = 0; i < s_ebook_show_info.bookmark_info.total_num; i ++)
    {       
        //读取电子书书签所在页的第一行文字信息到书签中,作为书签列表的显示内容
        ucs2_str_len = ReadOneLineUCS2(
                                    file_handle, 
                                    MMIEBOOK_TXT_SHOW_CTRL_ID,
                                    (uint32)s_ebook_show_info.bookmark_info.offset_tab[i],
                                    g_ebook_bookmark_name_tab[i].name_wstr
                                    );

        g_ebook_bookmark_name_tab[i].name_len = ucs2_str_len;
    }
}

/*****************************************************************************/
//  Description : 从文本中(gb)读取能够正好显示一行(ucs2)字符串
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return:字符串字节数
/*****************************************************************************/
LOCAL uint16 ReadOneLineUCS2(
                             MMIFILE_HANDLE file_handle,
                             MMI_CTRL_ID_T  ctrl_id,//IN: 文本控件ID
                             uint32 offset,//IN: 字符串在文本中的偏移量
                             wchar  *ucs2_str_ptr//OUT: 要求空间大于能够容纳一行ucs2字符
                             )
{
    uint8       str[MMIEBOOK_ONE_LINE_MAX_LEN + 1] = {0};
    wchar       ucs2_str[MMIEBOOK_ONE_LINE_MAX_LEN  + 1] = {0};
    uint16      str_len = 0;
    uint16      line_length = MMIEBOOK_BOOKMARK_NAME_LEN;
    GUI_CHAR_CODE_TYPE_E file_type = s_ebook_file_type;
    MMI_STRING_T    text = {0};
    ctrl_id = ctrl_id;   
    
    //读取文本信息
    MMIEBOOK_FFS_fseek(file_handle, offset, MMIFILE_SEEK_BEGIN);
    str_len = (uint16)MMIEBOOK_FFS_fread(str, MMIEBOOK_ONE_LINE_MAX_LEN, 1, file_handle);

    str_len = GUI_OtherCodeToWstr(
                                    ucs2_str, 
                                    MMIEBOOK_ONE_LINE_MAX_LEN, 
                                    file_type, 
                                    str, 
                                    str_len
                                    );
	ConversionWcharZeroToSpace(ucs2_str,str_len);    
    MMI_WSTRNCPY(ucs2_str_ptr, line_length,ucs2_str, line_length,line_length); 
    //去除书签最后的回车换行，    
    line_length = MIN(line_length,FindLastParagraphSperator(ucs2_str_ptr,str_len,0)); /*lint !e666*/
    text.wstr_ptr = ucs2_str_ptr;
    text.wstr_len = str_len;
    ClearNewlineChar(&text);    
    return line_length;
}

/*****************************************************************************/
//  Description :  打开电子书时读取书签和上次阅读的位置信息
//  Global resource dependence : 
//  Author: hui.zhao
//  Note:
//  Return:TRUE:书签信息文件已存在 FALSE:书签信息文件不存在
/*****************************************************************************/
LOCAL BOOLEAN ReadBookmarkInfo( void )
{
    wchar              bookmark_file_full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    BOOLEAN            result = FALSE;
    
    MMIEBOOK_GetBookmarkFileName(g_ebook_show_file_brief_ptr->full_name_wstr,bookmark_file_full_path);
    SCI_TRACE_LOW("ebook_time:ReadBookmarkInfo  start=%d",SCI_GetTickCount());
    //书签文件是否存在，若存在则打开并读取书签信息；若不存在，则创建书签文件    
    if (MMIAPIFMM_IsFileExist(bookmark_file_full_path,MMIAPICOM_Wstrlen(bookmark_file_full_path)))
    {
        if(MMIEBOOK_OpenBookmarkFile(bookmark_file_full_path,"r+"))
        {
            MMIEBOOK_ReadBookmarkFile(MMIEBOOK_GetBookmarkFileHandle(),MMIEBOOK_BOOKMARK_FILE_INFO,&s_ebook_show_info.bookmark_info);
            MMIEBOOK_ReadBookmarkFile(MMIEBOOK_GetBookmarkFileHandle(),MMIEBOOK_BOOKMARK_FILE_CUR_READ_OFFSET,&s_ebook_show_info.cur_read_offset);
		    if(!IsBookmarkInfoValid(&s_ebook_show_info.bookmark_info,s_ebook_show_info.cur_read_offset))
		    {
			    s_ebook_show_info.cur_read_offset=0;
			    SCI_MEMSET(&s_ebook_show_info.bookmark_info,0,sizeof(MMIEBOOK_BOOKMARK_T));
			    MMIEBOOK_CloseBookmarkFile();
			    MMIEBOOK_OpenBookmarkFile(bookmark_file_full_path,"w+");//创建
        	    MMIEBOOK_WriteBookmarkFile(MMIEBOOK_GetBookmarkFileHandle(), MMIEBOOK_BOOKMARK_FILE_CUR_READ_OFFSET, &s_ebook_show_info.cur_read_offset);
		    }
            //初始化书签名字内容
            MakeBookmarkNameTab();        
		    MMIEBOOK_CloseBookmarkFile();
            result = TRUE;
        }
    }
    else
    {
        if(MMIEBOOK_OpenBookmarkFile(bookmark_file_full_path,"w"))//创建
        {
            MMIEBOOK_WriteBookmarkFile(MMIEBOOK_GetBookmarkFileHandle(), MMIEBOOK_BOOKMARK_FILE_CUR_READ_OFFSET, &s_ebook_show_info.cur_read_offset);        
		    MMIEBOOK_CloseBookmarkFile();
        }
    }
    SCI_TRACE_LOW("ebook_time:ReadBookmarkInfo  end=%d",SCI_GetTickCount());
    return result;
} 

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
            )
{
    uint8*          data_str_ptr = PNULL;
    uint32          data_str_len = MMIEBOOK_SEG_BUF_UNIT_LEN;
    uint32          offset = 0;
        
    //计算当前书签所在文件中的偏移值
    data_str_ptr = (uint8 *)SCI_ALLOCA(data_str_len + 1);
    if(PNULL != data_str_ptr)
    {
	    SCI_MEMSET(data_str_ptr, 0, data_str_len + 1);

	    offset = GUI_WstrToOtherCode(
	        s_ebook_file_type, 
	        data_str_ptr, 
	        data_str_len, 
	        ucs2_str_ptr,
	        MIN(ucs2_str_offset, MMIEBOOK_SEG_BUF_UNIT_LEN)
	        );

	    SCI_FREE(data_str_ptr);
    }

    return offset;
}

/*****************************************************************************/
//  Description :  去掉字符串最后的换行符
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void ClearNewlineChar( MMI_STRING_T *text)
{
	if(1>=text->wstr_len||text->wstr_ptr==PNULL)
	{
		return ;
	}
	else if(2==text->wstr_len)
	{
		if((text->wstr_ptr[0]=='\r' && text->wstr_ptr[1]=='\n')
			||(text->wstr_ptr[0]=='\n' && text->wstr_ptr[1]=='\r'))
		{
			return ;
		}
	}

	
	if(text->wstr_len>1)
	{
		if(text->wstr_ptr[text->wstr_len-1]=='\r')
		{
			text->wstr_ptr[text->wstr_len-1]=0;
			text->wstr_len--;
		}
		else if(text->wstr_ptr[text->wstr_len-1]=='\n')
		{
			text->wstr_ptr[text->wstr_len-1]=0;
			text->wstr_len--;
			if(text->wstr_len>0)
			{
				if(text->wstr_ptr[text->wstr_len-1]=='\r')
				{
					text->wstr_ptr[text->wstr_len-1]=0;
					text->wstr_len--;
				}
			}
		}
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIEBOOK_SHOW_RECT_INFO_T* MMIEBOOK_GetEbookShowRectInfo( void )
{
	return &s_show_rect_info;
}

/*****************************************************************************/
//  Description :  将UNICODE编码中的0转换成空格
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
//  Return:转换的数量
/*****************************************************************************/
LOCAL int32 ConversionWcharZeroToSpace( wchar* ptr,int32 len)
{
	int32 count=0;
	int i=0;
	
	if(ptr==PNULL ||len==0)
	{
		return 0;
	}
	for(i=0;i<len;i++)
	{
		if(* ptr==0)
		{
			* ptr=0x20;
			count++;
		}
		ptr++;
	}

	return count;
}

/*****************************************************************************/
//  Description : 书签信息是否有效 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
//  Return:TRUE:书签信息有效；FALSE:书签信息无效
/*****************************************************************************/
LOCAL BOOLEAN IsBookmarkInfoValid( MMIEBOOK_BOOKMARK_T * bookmark_info,int32 cur_read_offset)
{
	int32 i=0;
	if(cur_read_offset>=g_ebook_show_file_brief_ptr->file_size||cur_read_offset<0)/*lint !e574 !e737*/
	{
		return FALSE;
	}
	if(bookmark_info!=PNULL)
	{
		if(bookmark_info->total_num>MMIEBOOK_BOOKMARK_MAX_NUM)
		{
			return FALSE;
		}

		for(i=0;i<bookmark_info->total_num;i++)/*lint !e574 !e737*/
		{
			if(bookmark_info->offset_tab[i]>=g_ebook_show_file_brief_ptr->file_size ||bookmark_info->offset_tab[i]<0)/*lint !e574 !e737*/
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

/*****************************************************************************/
//  Description : MMIEBOOK_IsTurnLineOrPage
//  Global resource dependence :
//   Author: renyi.hu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsTurnLineOrPage(
									MMI_MESSAGE_ID_E msg_id
                                    )
{
    BOOLEAN result = TRUE;
	MMIEBOOK_PAGE_DIRECTION_E direction =MMIEBOOK_PAGE_CUR;

	switch(msg_id)
	{
		case MSG_APP_UP:
			direction=MMIEBOOK_LINE_UP;
			break;
		case MSG_APP_LEFT:
//		case MSG_APP_UPSIDE:
			direction=MMIEBOOK_PAGE_UP;
			break;
		case MSG_APP_DOWN:
			direction=MMIEBOOK_LINE_DOWN;
			break;
		case MSG_APP_RIGHT:
//		case MSG_APP_DOWNSIDE:
			direction=MMIEBOOK_PAGE_DOWN;
			break;
		default:
			break;
	}
    
    switch (direction)
    {
    case MMIEBOOK_PAGE_UP:
        if (s_ebook_show_info.cur_line < s_ebook_show_info.one_page_lines)//翻到上一个段中去
        {
            if (s_ebook_show_info.cur_show_txt_ptr->this_seg_offset == 0)//已经到电子书开头了
            {
                if (s_ebook_show_info.cur_line == 0)
                {
                    //已经从电子书第一页第一行开始看了，则不做处理
                    result =FALSE;
                }
            }     
        }
        break;
        
    case MMIEBOOK_LINE_UP:
        if (s_ebook_show_info.cur_line == 0)//翻到上一个段中去
        {
            if (s_ebook_show_info.cur_show_txt_ptr->this_seg_offset == 0)//已经到电子书开头了
            {
                //已经从电子书第一页开始看了，则不做处理
                result =FALSE;
            }
        }
        break;
        
    case MMIEBOOK_LINE_DOWN:    //向下翻行 
        if (s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines >= s_ebook_show_info.cur_show_txt_ptr->total_line_num)//翻到下一个段中去
        {
            if (s_ebook_show_info.cur_show_txt_ptr->next_seg_offset == g_ebook_show_file_brief_ptr->file_size)
            {
                result =FALSE;
            }  
        }        
        break;
        
    case MMIEBOOK_PAGE_DOWN:   //向下翻页    
        if (s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines >= s_ebook_show_info.cur_show_txt_ptr->total_line_num)//翻到下一个段中去
        {
            if (s_ebook_show_info.cur_show_txt_ptr->next_seg_offset == g_ebook_show_file_brief_ptr->file_size)
            {
                result =FALSE;
            }       
			else if ((s_ebook_show_info.cur_line + s_ebook_show_info.one_page_lines - s_ebook_show_info.cur_show_txt_ptr->total_line_num) 
                    >= s_ebook_show_info.next_show_txt_ptr->total_line_num)//切换到next段的时候,已到了电子书末尾,且呢next段的内容不够显示新的一页
            {
                result =FALSE;
            }
        }
        break;
        
    default:
        result =FALSE;
        break;
    }
    //SCI_TRACE_LOW:"MMIEBOOK_TASK MMIEBOOK_IsTurnLineOrPage result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_TASK_2440_112_2_18_2_15_24_68,(uint8*)"d", result);
	return result;
}

/*****************************************************************************/
//  Description : MMIEBOOK_GetTxtDirection
//  Global resource dependence :
//   Author: renyi.hu
//  Note:   
/*****************************************************************************/
PUBLIC uint8 MMIEBOOK_GetTxtDirection(void)
{
	return s_txt_direction;
}

/*****************************************************************************/
//  Description : MMIEBOOK_ConvertFontSize
//  Global resource dependence :
//   Author: paul.huang
//  Note:   
/*****************************************************************************/
PUBLIC GUI_FONT_T MMIEBOOK_ConvertFontSize(MMIEBOOK_FONT_SIZE_E font_size)
{
    GUI_FONT_T font = 0;
        
    switch(font_size)
    {
#if defined( MMI_GUI_STYLE_MINISCREEN)    //#ifdef MMI_LOW_MEMORY_EBOOK
        case MMIEBOOK_FONT_BIG:
            font = SONG_FONT_16;
            break;
    
        case MMIEBOOK_FONT_NORMAL:
            font = SONG_FONT_14;
            break;

         case MMIEBOOK_FONT_SMALL:
            font = SONG_FONT_12;
            break;

         default:
            font = SONG_FONT_16;
            break;     
#else
        case MMIEBOOK_FONT_BIG:
            font = SONG_FONT_29;//SONG_FONT_28
            break;
    
        case MMIEBOOK_FONT_NORMAL:
            font = SONG_FONT_16;//SONG_FONT_24
            break;

         case MMIEBOOK_FONT_SMALL:
            font = SONG_FONT_14;//SONG_FONT_20
            break;

         default:
            font = SONG_FONT_16;//SONG_FONT_24;
            break;     
#endif
    }

    return font;
}
#endif

