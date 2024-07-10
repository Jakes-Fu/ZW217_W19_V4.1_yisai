#ifndef WDP_SG_ZHANXUNFUNC_C
#define WDP_SG_ZHANXUNFUNC_C
#include "wdp_SG_zhanxunfunc.h"
#include "wdp_update_wintab.h"
#include "window_parse.h"
#include "mmiim_text.h"
#include "mmipub.h"

LOCAL FuncPtr s_kernel_yes_callback = 0;

/******************************************************************************/
// Description: 超时提示窗口函数
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
LOCAL MMI_RESULT_E HandleTimeOutQueryWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/******************************************************************************/
// Description: 继续下载
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
LOCAL void sg_kernel_continue_download(void)
{
	if (s_kernel_yes_callback != 0)
	{
		s_kernel_yes_callback();
	}
	s_kernel_yes_callback = 0;
	MMK_CloseWin(SG_WIN_ID_UPDATE_WORDS_CONTINUE);
}

/******************************************************************************/
// Description: 搜狗输入法确认回调函数
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
PUBLIC void mmi_sogou_ime_confirm(SG_UINT16* sg_text, FuncPtr yes_callback, FuncPtr get_word_stop)
{
	MMI_STRING_T note1 = {0};
	MMI_STRING_T note2 = {0};
	MMI_WIN_ID_T query_win_id = SG_WIN_ID_UPDATE_WORDS_CONTINUE;

	s_kernel_yes_callback = yes_callback;
	MMI_GetLabelTextByLang(TXT_SOGOU_CONTINUE_UPDATE, &note1);
	MMI_GetLabelTextByLang(STXT_CANCEL, &note2);
			
	MMIPUB_OpenQueryWinByTextPtr(&note1, &note2, IMAGE_PUBWIN_QUERY, &query_win_id, HandleTimeOutQueryWinMsg);
}

/******************************************************************************/
// Description: 超时提示窗口函数
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note: 
/******************************************************************************/
LOCAL MMI_RESULT_E HandleTimeOutQueryWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:  
    case MSG_APP_WEB:
		sg_kernel_continue_download();
        break;    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
		MMK_CloseWin(win_id);
        break;		
    default:
		recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
		break;
    }	
    return (recode);
}
#endif //WDP_SG_ZHANXUNFUNC_C
