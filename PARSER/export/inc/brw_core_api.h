/*****************************************************************
 **  File Name:     brw_core_api.h                               *
 **  Author:	jiqun.li                                         *
 **  Date:                                                       *
 **  Description: browser core api                               *
******************************************************************
******************************************************************
**  Edit History											     *
**---------------------------------------------------------------*
 **  DATE             NAME         DESCRIPTION                   *
 **  12/12/2008     jiqun.li          Create.                    *
*****************************************************************/
#ifndef BRW_CORE_API_H
#define BRW_CORE_API_H
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "brw_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef void * LAYOUT_HANDLE;
typedef uint8 BRW_PARSE_FONT_SIZE_T;

#define BRW_INVALID_LAYOUT_HANDLE   0

typedef struct {
    const uint8*        url_ptr;        //absolute URL
    const uint8*        referer_ptr;    //Refefer:
    HTTP_METHOD_E       method;         //HTTP method
    HTTP_BODY_DATA_T    body_data_info; //body parameter
    BOOLEAN is_by_click;//fenxie:MakeRequestByUrlClick
}LAYOUT_REQ_PARAM_T;

typedef enum
{
    LAYOUT_CAUSE_PAGE,         //new web page
    LAYOUT_CAUSE_JUMP_PAGE,    //jump in current page
    LAYOUT_CAUSE_FONT,         //font changed
    LAYOUT_CAUSE_CSS,          //CSS coming
    LAYOUT_CAUSE_PIC,          //<img> data coming
    LAYOUT_CAUSE_SOUND,        //<bgsound> data coming
    LAYOUT_CAUSE_SCREENPROFIE, //lcd switch
    LAYOUT_CAUSE_JS,           // js change dom
    LAYOUT_CAUSE_UNKNOWN
}LAYOUT_CAUSE_E;

//media request type
typedef enum 
{
	MEDIA_REQ_TYPE_CSS = 0,      //css
	MEDIA_REQ_TYPE_JS,
	MEDIA_REQ_TYPE_BGSOUND,      //bgsound
	MEDIA_REQ_TYPE_IMG,          //<img>
    MEDIA_REQ_TYPE_BG_IMG,       //css background image
    MEDIA_REQ_TYPE_INPUT_IMG,    //input img
    MEDIA_REQ_TYPE_LIST_IMG,     //list img
    MEDIA_REQ_TYPE_ICO,     //ico img
    MEIDA_REQ_TYPE_UNKNOWN   
}MEDIA_REQ_TYPE_E;

//CSS data type
typedef enum 
{
	CSS_TYPE_GLOBAL = 1,      //global css
	CSS_TYPE_INLINE,          //inline css
	CSS_TYPE_IMPEND           //impending css
}CSS_TYPE_E;

//event type
typedef enum 
{
	EVENT_TYPE_LOAD = 1,      //
	EVENT_TYPE_UNLOAD,          //
	EVENT_TYPE_CLICK,           //
	EVENT_TYPE_BLUR,
    EVENT_TYPE_FOCUS,
    EVENT_TYPE_TIMER,
    EVENT_TYPE_CHANGE
}EVENT_TYPE_E;


//begin for parse buffer
typedef enum
{
    BRW_PARSE_TAG_TYPE_TEXT,    //text tag: p,h1--h6
    BRW_PARSE_TAG_TYPE_ANCHOR,  //a tag      
    BRW_PARSE_TAG_TYPE_BR,      //Br Tag, need to change new line
    BRW_PARSE_TAG_TYPE_MAX          
} BRW_PARSE_TAG_TYPE_E;

typedef enum 
{
    BRW_PARSE_FONT_STYLE_NORMAL,  //normal  
    BRW_PARSE_FONT_STYLE_ITALIC,  //italic
    BRW_PARSE_FONT_STYLE_MAX			
}BRW_PARSE_FONT_STYLE_E;

typedef enum 
{
	BRW_PARSE_FONT_WEIGHT_NORMAL,  //normal		
	BRW_PARSE_FONT_WEIGHT_BOLD,	   //bold	
	BRW_PARSE_FONT_WEIGHT_MAX	
}BRW_PARSE_FONT_WEIGHT_E;

typedef enum 
{
	BRW_PARSE_TEXT_DECORATION_NORMAL,		
	BRW_PARSE_TEXT_DECORATION_UNDERLINE,		
	BRW_PARSE_TEXT_DECORATION_MAX	
}BRW_PARSE_TEXT_DECORATION_E;

typedef struct
{
    BRW_PARSE_TEXT_DECORATION_E font_decoration;
}BRW_PARSE_TEXT_STYLE_T;

typedef struct _BRW_PARSE_BUF_PARAM
{
    uint8*              buf_ptr;    //the buffer of html/wml 
    uint32              buf_len;    //buffer len
    char*               charset_ptr;//content charset,always get from http header ind
    uint32              font_type;  //app default font type 
    BRW_MIME_TYPE_E     buf_mime;   //the mime type of html/wml
} BRW_PARSE_BUF_PARAM_T;

typedef struct
{
    BRW_PARSE_FONT_SIZE_T  		font_size;      //font size 
    BRW_PARSE_FONT_WEIGHT_E 	font_weight;    //font weight
    BRW_PARSE_FONT_STYLE_E  	font_style;     //font style
}BRW_PARSE_FONT_T;


typedef struct 
{
    wchar           	    *data_ptr;  //a tag display content ptr
    uint32      		    data_len;  //a tag display content len      
    uint8           	    *href_ptr;  //a tag href content ptr
    uint32  			    href_len;  //a tag href content len
    BRW_PARSE_FONT_T        font_type; //content font type
    BRW_PARSE_TEXT_STYLE_T  text_style;
} BRW_PARSE_A_T;

typedef struct 
{
    wchar           	    *data_ptr;  //text tag content ptr
    uint32      		    data_len;   //text tag content len
    BRW_PARSE_FONT_T        font_type;  //text tag font type
    BRW_PARSE_TEXT_STYLE_T  text_style;
} BRW_PARSE_TEXT_T;


typedef union 
{
    BRW_PARSE_A_T           a;
    BRW_PARSE_TEXT_T       text;
} BRW_PARSE_TAG_U;


typedef struct _BRW_PARSE_TAG_INFO
{
    BRW_PARSE_TAG_TYPE_E    type;   //tag type 
    BRW_PARSE_TAG_U         info;   //tag info
} BRW_PARSE_TAG_INFO_T;


typedef struct _BRW_PARSE_BUF_RESULT
{
    struct _BRW_PARSE_BUF_RESULT   *next_node_ptr;  //next node info for result
    BRW_PARSE_TAG_INFO_T		    tag_info;       //current node info
} BRW_PARSE_BUF_RESULT_T;   
//end for parse buffer

//保存image信息
typedef struct  
{
    uint8           *url_ptr;
    uint32          url_len;
    BRW_MIME_TYPE_E mime_type;
}BRW_IMAGE_INFO_T;

typedef enum {
    BRW_DIRECTION_LEFT  = 0,  //move to last hotlink which is in screen
    BRW_DIRECTION_RIGHT = 1,  //move to first hotlink which is in screen
    BRW_DIRECTION_UP    = 2,  //move to next hotlink in screen
    BRW_DIRECTION_DOWN  = 3,  //move to first hotlink in screen 
    BRW_MOVE_TO_FIRST   = 4,  //move to first hotlink in web
    BRW_MOVE_TO_LAST    = 5   //move to last hotlink  in web
}BRW_DIRECTION_E;

/*
    return type
*/
typedef enum {
    BRW_NOMEMORY            = -1,   //no memory
    BRW_ERROR               = 0,    //other error
    BRW_SUCCESS              = 1    //success
}BRW_RET;

/*request a new web page callback*/
typedef BOOLEAN (*BRW_START_PAGE_REQ_CBF)(const LAYOUT_HANDLE handle,const uint32 control_handler);

/*navigate callback*/
typedef BOOLEAN (*BRW_HISTORY_OP_CBF)(const uint32 control_handler, BRW_NAVIGATE_EVENT_E op);
/*append a new media request callback*/
typedef BOOLEAN (*BRW_ADD_MEDIA_REQ_CBF)(const uint8       *url_ptr,
                                         MEDIA_REQ_TYPE_E  meida_type,
                                         const uint32      control_handler,
                                         const uint32      media_param);

typedef BOOLEAN (*BRW_CANCEL_WAIT_FUNC)(void* param,uint16 size_of_param);

typedef struct {
    uint8*                  url_ptr;      //URL, must be absolute
    uint8*                  data_ptr;     //web page data
    uint32                  data_len;     //web page data length
    uint8*                  charset_ptr;  //charset
    uint32                  req_id;       //control layer user data
    BRW_START_PAGE_REQ_CBF  start_new_pagereq_cbf;
    BRW_HISTORY_OP_CBF      history_op_cbf;
    BRW_ADD_MEDIA_REQ_CBF   add_new_media_req_cbf; // append a new media request callback
    uint32                  font_type;     //GUI_FONT_T font_type;
    void*                   prgbar_ptr;    //scroll bar control handle; prgbar_ptr=GetProBoxPtr(ctrl_id);
    uint8                   prgbar_width;  //scroll bar width
    BRW_SCREEN_OPTION_T     screen_option; //web page display region, include scroll bar*/
    BOOLEAN                 is_show_image; //flag for displaying image 
    BOOLEAN                 is_play_bgsound; //flag for playing background sound
    uint32					http_context_id;
#ifdef JS_SUPPORT
    uint8*                  user_agent_ptr; //UA string. for JS
    BOOLEAN                 is_cookie_enabled;//cookie enable, for js
    BRW_WIN_HANDLE          win_handle;//windows handle
	BRW_CANCEL_WAIT_FUNC    cancel_wait;
#endif
}PARSER_INPUT_INFO_T;

typedef struct _BRW_IMAGE_INFO_LIST_T
{
	BRW_IMAGE_INFO_T image_info;
	uint8			 *image_data_ptr;
    uint32           image_data_len;
	struct _BRW_IMAGE_INFO_LIST_T *next;
	
}BRW_IMAGE_INFO_LIST_T;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/******************************************************************************************
* DESCRIPTION:
*   create a layout handle
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   mime_e:  [IN] MIME type
*
* RETURN:
*   TRUE: no use, always return TRUE now
* NOTE:
******************************************************************************************/
LAYOUT_HANDLE BrwCreateLayoutCtrl(BRW_MIME_TYPE_E mime_e);

/******************************************************************************************
* DESCRIPTION:
*   destroy the layout handle created by BrwCreateLayoutCtrl()
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle:  [IN] layout handle
*
* RETURN:
*   void
* NOTE:
******************************************************************************************/
void BrwDestroyLayoutCtrl(LAYOUT_HANDLE handle);


/******************************************************************************************
* DESCRIPTION:
*   Initialize Parser module.
* AUTHOR:
*   Sally.he
* RETURN:
* NOTE:
******************************************************************************************/
PUBLIC void BrwParseInit(void);

/******************************************************************************************
* DESCRIPTION:
*   Destroy Parser module.
* AUTHOR:
*   Sally.he
* RETURN:
* NOTE:
******************************************************************************************/
PUBLIC void BrwParseDestroy(void);

/******************************************************************************************
* DESCRIPTION:  Destroy brw hash table.
* AUTHOR: wenmei.hua
* RETURN:
* NOTE:
******************************************************************************************/
PUBLIC void BrwHashDestroy(void);
/******************************************************************************************
* DESCRIPTION:
*   parse web page, result is a layout tree in layout handle
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle:    [IN]  layout handle
*   param_ptr: [IN]  parse information
*
* RETURN:
*   BRW_SUCCESS:  parse successfully
*   BRW_ERROR:    parse error
*   BRW_NOMEMORY: no memory
* NOTE:
******************************************************************************************/
PUBLIC BRW_RET BrwParseWebPage(const PARSER_INPUT_INFO_T *param_ptr, LAYOUT_HANDLE handle);

/*******************************************************************************************
* DESCRIPTION:
*   set layout information
*
* AUTHOR：
*   jiqun.li
*
* PARAMETERS:
*   screen_option:[IN]web page display region, include scroll bar
*   prgbar_width: [IN]scroll bar width
*   font_type:    [IN]GUI_FONT_T font_type
*   handle        [OUT]current layout handle

* RETURN:
*   TRUE:  parameter is ok
*   FALSE: not supported parameter
******************************************************************************************/
PUBLIC BOOLEAN BrwSetLayoutInfo(
                                BRW_SCREEN_OPTION_T screen_option, //[IN]web page display region, include scroll bar
                                uint8               prgbar_width,  //[IN]scroll bar width
                                uint32              font_type,     //[IN]GUI_FONT_T font_type
                                LAYOUT_HANDLE       handle         //[OUT]
                                );
/******************************************************************************************
* DESCRIPTION:
*   when a layout lost focus by navigation event, control layer must call this function before 
*   make a new web page request
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle:  [IN]  layout handle
*
* RETURN:
*   void
* NOTE:
******************************************************************************************/
void BrwLayoutNavigateLostFocus(LAYOUT_HANDLE handle);

/******************************************************************************************
* DESCRIPTION:
*   when a layout get focus by navigation event, control layer must call this function before 
*   call BrwLayoutDispalyWebPage()
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle:  [IN]  layout handle. 
*   event    [IN]  what navigation event happened before this layout get focus
*   url_ptr: [IN]  only used when event is BRW_NAV_EVENT_BACKWARD
*   is_error:[IN]  TRUE:means that function is called by BRWProcErr. FALSE:other
*
* RETURN:
*   void
* NOTE:modify by xianhe.yang 2011.11.30
******************************************************************************************/
void BrwLayoutNavigateGetFocus(LAYOUT_HANDLE handle, BRW_NAVIGATE_EVENT_E event, const uint8 *url_ptr,BOOLEAN is_error);

/******************************************************************************************
* DESCRIPTION:
*   layout current web page, every element is relative coordination. before any event occur
*   before display web page, no need to call this function more times.
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle: [IN]  layout handle
*   cause:  [IN]  why layout , no use now
*
* RETURN:
*   BRW_SUCCESS:  layout successfully
*   BRW_ERROR:    error occurs
*   BRW_NOMEMORY: no memory
* NOTE:
******************************************************************************************/
BRW_RET BrwLayout(LAYOUT_HANDLE handle, LAYOUT_CAUSE_E cause);

/******************************************************************************************
* DESCRIPTION:
*   display current web page
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle: [IN]  layout handle
*
* RETURN:
*   void
* NOTE:
******************************************************************************************/
BOOLEAN BrwDisplayWebPage(BRW_WIN_HANDLE win_handle, LAYOUT_HANDLE handle);

/******************************************************************************************
* DESCRIPTION:
*   repaint current web page
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle: [IN]  layout handle
*
* RETURN:
*   void
* NOTE:
******************************************************************************************/
BOOLEAN BrwRepaintWebPage(BRW_WIN_HANDLE win_handle, LAYOUT_HANDLE handle);

PUBLIC void BrwURLStrEscape(uint8 **dst_pptr, const uint8 *utf8_ptr);

#ifdef JS_SUPPORT
/******************************************************************************************
* DESCRIPTION: Xhtml Check If Js Start New Page
* AUTHOR:wenmei.hua
* PARAMETERS:
* RETURN:bool  
* NOTE:
******************************************************************************************/
PUBLIC BOOLEAN  XhtmlCheckIfJsStartNewPage(LAYOUT_HANDLE layout_handle);

/******************************************************************************************
* DESCRIPTION: check if page need recalc by js
* AUTHOR:wenmei.hua
* PARAMETERS:
* RETURN:  
* NOTE:
******************************************************************************************/
PUBLIC BOOLEAN  XhtmlCheckIfJsRecalcPage(LAYOUT_HANDLE layout_handle);
/******************************************************************************************
* DESCRIPTION: set recalc flag
* AUTHOR:wenmei.hua
* PARAMETERS:
* RETURN:  
* NOTE:
******************************************************************************************/
PUBLIC void  XhtmlSetJsRecalcFlag(LAYOUT_HANDLE layout_handle);
/******************************************************************************************
* DESCRIPTION: set js new request flag false 
* AUTHOR:wenmei.hua
* PARAMETERS:
* RETURN:  
* NOTE:
******************************************************************************************/
PUBLIC void  XhtmlSetJsStartNewPageFlag(LAYOUT_HANDLE layout_handle);

/******************************************************************************************
* DESCRIPTION: check if need back
* AUTHOR:wenmei.hua
* PARAMETERS:
* RETURN:  
* NOTE:
******************************************************************************************/
PUBLIC BOOLEAN  XhtmlCheckIfJsBackPage(LAYOUT_HANDLE layout_handle);

/******************************************************************************************
* DESCRIPTION: check if need submit form
* AUTHOR:wenmei.hua
* PARAMETERS:
* RETURN:  
* NOTE:
******************************************************************************************/
PUBLIC BOOLEAN  XhtmlCheckIfJsSubmitForm(LAYOUT_HANDLE layout_handle);

/******************************************************************************************
* DESCRIPTION: check if input is click by js
* AUTHOR:wenmei.hua
* PARAMETERS:
* RETURN:  
* NOTE:
******************************************************************************************/
PUBLIC BOOLEAN  XhtmlCheckIfJsClickInput(LAYOUT_HANDLE layout_handle);

/******************************************************************************************
* DESCRIPTION: set submit form flag
* AUTHOR:wenmei.hua
* PARAMETERS:
* RETURN:  
* NOTE:
******************************************************************************************/
PUBLIC void  XhtmlSetJsSubmitFormFlag(LAYOUT_HANDLE layout_handle);

/******************************************************************************************
* DESCRIPTION: set submit form flag
* AUTHOR:wenmei.hua
* PARAMETERS:
* RETURN:  
* NOTE:
******************************************************************************************/
PUBLIC void  XhtmlSetJsClickInputFlag(LAYOUT_HANDLE layout_handle);

/******************************************************************************************
* DESCRIPTION: set js back request flag false 
* AUTHOR:wenmei.hua
* PARAMETERS:
* RETURN:  
* NOTE:
******************************************************************************************/
PUBLIC void  XhtmlSetJsBackwPageFlag(LAYOUT_HANDLE layout_handle);

/******************************************************************************************
* DESCRIPTION: Xhtml Check If Js Start New Page
* AUTHOR:wenmei.hua
* PARAMETERS:
* RETURN:bool  
* NOTE:
******************************************************************************************/
PUBLIC BOOLEAN  XHTMLMakeNewRequestByJs(LAYOUT_HANDLE layout_handle);

/******************************************************************************************
* DESCRIPTION: make new request from js
* AUTHOR:wenmei.hua
* PARAMETERS:
* RETURN:  
* NOTE:
******************************************************************************************/
PUBLIC void  XHTMLMakeFormSubmitByJs(LAYOUT_HANDLE layout_handle);

/******************************************************************************************
* DESCRIPTION: process js click
* AUTHOR:wenmei.hua
* PARAMETERS:
* RETURN:  
* NOTE:
******************************************************************************************/
PUBLIC void  XhtmlLayoutInputClickProcessByJs(LAYOUT_HANDLE layout_handle);

/******************************************************************************************
* DESCRIPTION: process input change event
* AUTHOR:wenmei.hua
* PARAMETERS:
*   input_ptr: [IN] handle of xhtml <input> element
* RETURN:bool  
* NOTE:
******************************************************************************************/
void XhtmlProcInputChangeEvent(LAYOUT_HANDLE layout_handle, void *layout_ptr);
/******************************************************************************************
* DESCRIPTION: process select change event
* AUTHOR:wenmei.hua
* PARAMETERS:
* RETURN:bool  
* NOTE:
******************************************************************************************/
void XhtmlProcSelectChangeEvent(LAYOUT_HANDLE layout_handle, void *layout_ptr);

/******************************************************************************************
* DESCRIPTION: process the load event
* AUTHOR: wenmei.hua
* PARAMETERS: handle: [IN]  layout handle
* RETURN:   void
* NOTE:
******************************************************************************************/
PUBLIC BOOLEAN BrwProcEvent(LAYOUT_HANDLE handle, EVENT_TYPE_E event_type, const void *func_ptr, int32 x, int32 y, void *layout_ptr);

/******************************************************************************************
* DESCRIPTION:  parse JS
* AUTHOR: wenmei.hua
** RETURN:
* NOTE:
******************************************************************************************/
PUBLIC BOOLEAN BrwParseJavaScript(const uint8    *data_ptr,        //[IN] point to js data
                                  const uint8    *url_ptr,     //[IN] url of js
                                  LAYOUT_HANDLE  layout_handle_ptr,
                                  void           *native_ptr);

/******************************************************************************************
* DESCRIPTION:
*   compile javascript
* AUTHOR: wenmei.hua
** RETURN:
*   TRUE:  compile successfully
*   FALSE: compile failed
* NOTE:
******************************************************************************************/
PUBLIC void *BrwCompileTextToJSFunc(const uint8    *str_ptr,
                                    LAYOUT_HANDLE  layout_handle);

#endif
/******************************************************************************************
* DESCRIPTION:
*   layout process pen down event
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle:           [IN] layout handle
*   pendown_info_ptr: [IN] pen down information
*
* RETURN:
*   TRUE: no use, always return TRUE now
* NOTE:
******************************************************************************************/
BOOLEAN BrwLayoutPenDown(LAYOUT_HANDLE handle,const BRW_PEN_CLICK_INFO_T* pendown_info_ptr);

/******************************************************************************************
* DESCRIPTION:
*   layout process pen up event
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle:           [IN] layout handle
*   penup_info_ptr: [IN] pen up information
*
* RETURN:
*   TRUE: no use, always return TRUE now
* NOTE:
******************************************************************************************/
BOOLEAN BrwLayoutPenUp(LAYOUT_HANDLE handle,const BRW_PEN_CLICK_INFO_T* penup_info_ptr);

/******************************************************************************************
* DESCRIPTION:
*   process TP scroll event
*
* AUTHOR:
*   fen.xie
*
* PARAMETERS:
*   handle:  [IN] layout handle
*   move_y: [IN] vertical move position
*
* RETURN:
*   TRUE:  tp scroll event processed
*   FALSE: invalid handle
* NOTE:
******************************************************************************************/
BOOLEAN  BrwProcTpScroll(LAYOUT_HANDLE handle, int16 move_y);

/******************************************************************************************
* DESCRIPTION:
*   get page title
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle:  [IN] layout handle
*   buf_ptr: [OUT] buffer for saving the page title
*   bu_max_len: [IN] buffer size
*   title_len_ptr: [OUT] return title Unicode number in buf_ptr
*
* RETURN:
*   TRUE:  success
*   FALSE: invalid handle
* NOTE:
******************************************************************************************/
BOOLEAN BrwGetPageTitle(LAYOUT_HANDLE handle, uint16* buf_ptr,uint16 buf_max_len,uint16* title_len_ptr);

/******************************************************************************************
* DESCRIPTION:
*   process WML script
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   data_ptr: [IN] wmlscript data
*   data_len: [IN] wmlscript data length
*   mime_type:[IN] MIME type
*   handle: [IN] layout handle
*
* RETURN:
*   BRW_SUCCESS:  WML script is processed
*   BRW_ERROR  :  error occurs
*   BRW_NOMEMORY: no memory
*   
* NOTE:
******************************************************************************************/
PUBLIC BRW_RET BrwProcWmlScript(const PARSER_INPUT_INFO_T *param_ptr, LAYOUT_HANDLE handle);

/******************************************************************************************
* DESCRIPTION:
*   parse CSS
*
* AUTHOR:
*   sally.he

** RETURN:
*   TRUE:  parse successfully
*   FALSE: parse failed
* NOTE:
******************************************************************************************/
PUBLIC BOOLEAN BrwParseCSS(uint8          *data_ptr,        //[IN] point to css data
                           uint32         data_len,         //[IN] css data len
                           const char     *url_ptr,         //[IN] url of css
                           LAYOUT_HANDLE  ctrl_handle,      //[IN] LAYOUT_CTRL_T handle
                           LAYOUT_HANDLE  layout_handle,    //[IN] for not inline style css, it's NULL
                           uint32         brw_sheet_handle, //[IN] impending css_sheet 
                           CSS_TYPE_E     css_type          //[IN] css type                         
                           );

/******************************************************************************************
* DESCRIPTION:
*   set image data of <img> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle:   [IN]  layout handle
*   mime:     [IN]  MIME type of the image
*   index:    [IN]  image index in image list from zero
*   data_ptr: [IN]  image data
*   data_len: [IN]  image data length
*
* RETURN:
*   TRUE:  there is a <img> in image list by this index. 
*   FALSE: the index is too large than image list size, no this index in list
* NOTE:
******************************************************************************************/
void BrwSetImgBuf( uint8*          data_ptr,        //[IN]image data，free by ???
                   uint32          data_len,        //[IN]image data length
                   BRW_MIME_TYPE_E mime,            //[IN]image type
                   LAYOUT_HANDLE   handle,          //[IN]layout handle
                   uint32          index            //[IN]image index in list
                 );
#ifdef CSS_SUPPORT
void BrwSetBgImgBuf( uint8           *data_ptr,
                    uint32          data_len,
                    BRW_MIME_TYPE_E mime,
                    LAYOUT_HANDLE   handle,
                    uint32          index);
#endif
void BrwSetInputImgBuf( uint8 *data_ptr,
                           uint32 data_len,
                           BRW_MIME_TYPE_E mime,
                           LAYOUT_HANDLE handle,
                           uint32 index);
/******************************************************************************************
* DESCRIPTION:
*   get image data of <img> element
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle:   [IN]  layout handle
*   mime:     [IN]  MIME type of the image
*   index:    [IN]  image index in image list from zero
*   data_pptr: [OUT] image data
*   data_len: [IN]  image data length
*
* RETURN:
*   TRUE:  there is a <img> in image list by this index. 
*   FALSE: the index is too large than image list size, no this index in list
* NOTE:
*   caller must not free *data_pptr
******************************************************************************************/
BOOLEAN BrwGetImgBuf(uint8           **data_pptr,        //[OUT]image data
                     uint32          *data_len,          //[OUT]image data length
                     BRW_MIME_TYPE_E *mime,              //[OUT]image type
                     LAYOUT_HANDLE   handle,             //[IN]layout handle
                     uint32          index);             //[IN]image index in list

/******************************************************************************************
* DESCRIPTION:
*   set image data of list-style-image
*
* AUTHOR:
*   sally.he
*
* PARAMETERS:
*   data_ptr: [IN]  image data
*   data_len: [IN]  image data length
*   mime:     [IN]  MIME type of the image
*   handle:   [IN]  layout handle
*   param:    [IN]  XHTML_LAYOUT_T list
*
* RETURN:
* NOTE:
******************************************************************************************/
PUBLIC BOOLEAN BrwSetListImgBuf(uint8           *data_ptr,
                                uint32          data_len,
                                BRW_MIME_TYPE_E mime,
                                LAYOUT_HANDLE   handle,
                                uint32          param);
/******************************************************************************************
* DESCRIPTION:
*   get url of a image
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle:   [IN]   layout handle
*   url_pptr: [OUT]  return absolute url of this image, return value maybe NULL.
*   index:    [IN]   image index in image list from zero
*
* RETURN:
*   TRUE:  there is a <img> in image list by this index. 
*   FALSE: the index is too large than image list size, no this index in list
* NOTE:
*   caller must not free url_pptr, it's a const string
******************************************************************************************/
BOOLEAN BrwGetImgUrl(LAYOUT_HANDLE handle, uint8 **url_pptr, uint32 index, BOOLEAN *is_url_ptr);
BOOLEAN BrwGetInputImgUrl(LAYOUT_HANDLE handle, uint8 **url_pptr, uint32 index);

/******************************************************************************************
* DESCRIPTION:
*   set <bgsound> data
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle:  [IN] layout handle
*   data_ptr:[IN] <bgsound> media data
*   data_len:[IN] <bgsound> media data length
*   index:   [IN] <bgsound> index in <bgsound> list
*   mime_type: [IN] MIME type
*
* RETURN:
*   TRUE: 
*   FALSE: not find <bgsound> element
*
* NOTE:
*   caller must not free the return value
******************************************************************************************/
BOOLEAN BrwSetBgsoundBuf(uint8           *data_ptr,
                         uint32          data_len,
                         BRW_MIME_TYPE_E mime_type,
                         LAYOUT_HANDLE   handle,
                         uint32          index);

/******************************************************************************************
* DESCRIPTION:
*   get <bgsound> data
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle:   [IN] layout handle
*   data_pptr:[OUT] <bgsound> media data,, caller must not free it
*   data_len_ptr:[OUT] <bgsound> media data length
*   index:   [IN] <bgsound> index in <bgsound> list
*   mime_type_ptr: [OUT] <bgsound> media MIME type
*   index:   [IN] <bgsound> index in <bgsound> list
*
* RETURN:
*   TRUE: 
*   FALSE: not find <bgsound> element
*
* NOTE:
******************************************************************************************/
BOOLEAN BrwGetBgsoundBuf(uint8           ** data_ptr,
                         uint32          *data_len,
                         BRW_MIME_TYPE_E *mime_type,
                         int32           *loop_ptr,
                         LAYOUT_HANDLE   layout,
                         uint32          index);

/******************************************************************************************
* DESCRIPTION:
*   get URL of a <bgsound>
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle:   [IN]   layout handle
*   url_pptr: [OUT]  return absolute url of this <bgsound> element, return value maybe NULL.
*   index:    [IN]   index in <bgsound> list from zero
*
* RETURN:
*   TRUE:  there is a <bgsound> in <bgsound> list. 
*   FALSE: the index is too large than <bgsound> list length, no this index in list
* NOTE:
*   caller must not free url_pptr, it's a const string
******************************************************************************************/
BOOLEAN BrwGetBgsoundUrl(LAYOUT_HANDLE handle, uint8 **url_pptr, uint32 index);

/******************************************************************************************
* DESCRIPTION:
*   free <bgsound> data
*
* AUTHOR:
*   xianhe.yang
*
* PARAMETERS:
*   handle:  [IN] layout handle
*
* RETURN:
*   TRUE: 
*   FALSE: input param error or not find <bgsound> element
******************************************************************************************/
BOOLEAN BrwFreeBgsoundBuf(LAYOUT_HANDLE handle);

/******************************************************************************************
* DESCRIPTION:
*   when current layout lost focus,call this function to stop timer.
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle: [IN] layout handle
*
* RETURN:
*   TRUE:
* NOTE:   
******************************************************************************************/

void BrwLayoutStopTimer(LAYOUT_HANDLE handle);

/******************************************************************************************
* DESCRIPTION:
*   when timer message coming, if timer id same as current layout timer id, stop it.
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle: [IN] layout handle
*   timer_id: [IN] current timer message id
*
* RETURN:
*   TRUE:
* NOTE:   
******************************************************************************************/
BOOLEAN BrwLayoutStopTimerById(LAYOUT_HANDLE handle, uint32 timer_id);

/******************************************************************************************
* DESCRIPTION:
*   process timer event in layout
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle:  [IN] layout handle
*   timer_id:[IN] timer id.
*
* RETURN:
*   void
* NOTE:
******************************************************************************************/
void BrwProcessTimerEvent(LAYOUT_HANDLE handle, uint32 timer_id);

/******************************************************************************************
* DESCRIPTION:
*   get the number of background images
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   handle:   [IN]   layout handle
*
* RETURN:
*   the number of background images
* NOTE:
******************************************************************************************/
uint32 BrwGetImgNumber(LAYOUT_HANDLE handle);

#ifdef BRW_PARSE_BUF_SUPPORT
/******************************************************************************************
* DESCRIPTION:
* parse html/wml buffer

* AUTHOR:
*   kelly.li
*
* PARAMETERS:
*   param_ptr:  [IN]the param info for html/wml buffer
*   result_pptr:[OUT]the result for the parse
* RETURN:
*   TRUE:  parse successfully
*   FALSE: parse error
* NOTE:if return TRUE,app should call function BRW_ParseBufDestructor to free buffer
******************************************************************************************/
PUBLIC BRW_RET BRW_ParseBufConstructor(
                                       const BRW_PARSE_BUF_PARAM_T  *param_ptr,
                                       BRW_PARSE_BUF_RESULT_T       **result_pptr
                                       );

/******************************************************************************************
* DESCRIPTION:
*   free the buffer for parse buffer
*
* AUTHOR:
*   kelly.li
*
* PARAMETERS:
*   result_pptr:[IN]the result that need to be freed
* RETURN:
*   action result
******************************************************************************************/
PUBLIC BOOLEAN BRW_ParseBufDestructor (BRW_PARSE_BUF_RESULT_T       *result_ptr);
#endif
/******************************************************************************************
* DESCRIPTION:
*   scroll layout one page upward, 
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   ctrl_ptr:  [IN] layout handle
* RETURN:
*   return TRUE if have scroll one page upward
******************************************************************************************/
PUBLIC BOOLEAN BrwLayoutPageUp(LAYOUT_HANDLE handle);

/******************************************************************************************
* DESCRIPTION:
*   scroll layout one page downward, 
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   ctrl_ptr:  [IN] layout handle
* RETURN:
*   return TRUE if have scroll one page downward
******************************************************************************************/
PUBLIC BOOLEAN BrwLayoutPageDown(LAYOUT_HANDLE handle);

/******************************************************************************************
* DESCRIPTION:
*   scroll layout one line upward, 
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   ctrl_ptr:  [IN] layout handle
* RETURN:
*   always return TRUE now
******************************************************************************************/
PUBLIC BOOLEAN BrwLayoutLineUp(LAYOUT_HANDLE handle);

/******************************************************************************************
* DESCRIPTION:
*   scroll layout one line downward, 
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   ctrl_ptr:  [IN] layout handle
* RETURN:
*   always return TRUE now
******************************************************************************************/
PUBLIC BOOLEAN BrwLayoutLineDown(LAYOUT_HANDLE handle);

/******************************************************************************************
* DESCRIPTION:
*   move focus in layout
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   ctrl_ptr:  [IN] layout handle
*   direction: [IN] show to move foucs, left, right, up, down.
* RETURN:
*   always return TRUE now
******************************************************************************************/
PUBLIC BOOLEAN BrwLayoutMoveFocused(LAYOUT_HANDLE handle, BRW_DIRECTION_E direction);

/******************************************************************************************
* DESCRIPTION:
*   process click event in layout
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   ctrl_ptr:   [IN] layout handle
*
* RETURN:
*    alway return TRUE now
* NOTE:
******************************************************************************************/
PUBLIC BOOLEAN BrwLayoutClickFocused(LAYOUT_HANDLE handle);

/******************************************************************************************
* DESCRIPTION:
*  make a vertical  scroll event and move focus
*
* AUTHOR:
*   jiqun.li
*
* PARAMETERS:
*   ctrl_ptr: [IN]  layout handle
*   top_index:  [IN]  top line index
*
* RETURN:
*   TRUE: yes, scrolled. FALSE: no scroll occurs
*
* NOTE:
******************************************************************************************/
PUBLIC BOOLEAN BrwLayoutMoveVertical(LAYOUT_HANDLE handle, uint32 top_index);
/******************************************************************************************
* DESCRIPTION:
*   set record clicked url flag
*
* AUTHOR:
*   qiong.huang
*
* PARAMETERS:
*
* RETURN:
*   void
*   
* NOTE:
******************************************************************************************/
void BrwSetRecordClickedUrl(BOOLEAN flag);

/******************************************************************************************
* DESCRIPTION:
*   check if record clicked url enable
*
* AUTHOR:
*   qiong.huang
*
* PARAMETERS:
*
* RETURN:
*   BOOLEAN
*   
* NOTE:
******************************************************************************************/
BOOLEAN BrwIsRecordClickedUrl(void);

/******************************************************************************************
* DESCRIPTION:
*   获取当前焦点图片信息.
*
* AUTHOR:
*   xianhe.yang
*
* PARAMETERS:
*   handle[IN] layout句柄
*   imageinfo_ptr[IN/OUT] 用于保存获取的图片信息，包括url和mime_type，其中url数据不需要调用者释放
*
* RETURN:
*   如果当前焦点是图片，返回TRUE，并填写该图片信息到imageinfo_ptr中，否则返回FALSE
*   
* NOTE:不允许入参imageinfo_ptr为空，从本函数获取的url_ptr资源千万外部不要释放
******************************************************************************************/
PUBLIC BOOLEAN BrwGetFocusImageInfo(LAYOUT_HANDLE handle, 
                                    BRW_IMAGE_INFO_T *imageinfo_ptr);


/******************************************************************************************
* DESCRIPTION:
*   获取触摸屏点击的图片信息.
*
* AUTHOR:
*   xianhe.yang
*
* PARAMETERS:
*   handle[IN] layout句柄
*   x[IN] 触摸点的x轴坐标
*   y[IN] 触摸点的y轴坐标
*   imageinfo_ptr[IN/OUT] 用于保存获取的图片信息，包括url和mime_type，其中url数据不需要调用者释放
* RETURN:
*   如果触摸屏点击的是图片，返回TRUE，并填写该图片信息到imageinfo_ptr中，否则返回FALSE
*   
* NOTE:不允许入参imageinfo_ptr为空，从本函数获取的url_ptr资源外部千万不要释放
******************************************************************************************/
PUBLIC BOOLEAN BrwGetTpPressImageInfo(LAYOUT_HANDLE handle,
                                      uint32 x, 
                                      uint32 y, 
                                      BRW_IMAGE_INFO_T *imageinfo_ptr);

/******************************************************************************************
* DESCRIPTION:
*   获得当前焦点图片的数据信息
*
* AUTHOR:
*   xianhe.yang
*
* PARAMETERS:
*   handle:[IN] layout句柄
*   data_pptr:[IN/OUT]    保存图片数据，获得的图片数据不需要调用者释放
*   data_len_ptr:[IN/OUT] 保存图片数据长度
*
* RETURN:
*   如果当前焦点对应的是图片则返回TRUE,否则返回FALSE.入参不正确也返回FALSE.
*   
* NOTE:允许入参data_ptr和data_len_ptr有一个为NULL，data_pptr获取的图片数据调用者千万不要释放
******************************************************************************************/
PUBLIC BOOLEAN BrwGetFocusImgBuf(LAYOUT_HANDLE   handle,          //[IN]layout handle
                                 const uint8     **data_pptr,    //[OUT]image data
                                 uint32          *data_len_ptr);     //[OUT]image data length

/******************************************************************************************
* DESCRIPTION:
*   get image list
*
* AUTHOR:
*   xianhe.yang
*
* PARAMETERS:
*   img_list_ptr [IN/OUT]
*   handle:   [IN]  layout handle
*
* RETURN:
*   ret_num:  image list number
*   
* NOTE:
******************************************************************************************/
uint32 BrwGetImgInfoList(BRW_IMAGE_INFO_LIST_T *img_list_ptr, LAYOUT_HANDLE   handle);

/******************************************************************************************
* DESCRIPTION:
*   free image list
*
* AUTHOR:
*   xianhe.yang
*
* PARAMETERS:
*   img_list_ptr [IN/OUT]
*
* RETURN:
*   
* NOTE:
******************************************************************************************/
BOOLEAN BrwFreeImgInfoList(BRW_IMAGE_INFO_LIST_T *img_list_ptr , uint32 list_len);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif
