#ifndef _MMIASP_WINTAB_H
#define _MMIASP_WINTAB_H

#include "window_parse.h"
#include "sci_types.h"
#include "mmiasp_cortrol_layer.h"

typedef enum
{
    NORMAL,
    FOCUS,
    GREY,
}MMIASP_DISPLAY_MODE;

//Button type
typedef enum
{
    MMIASP_BUTTON_VOLUME = 0,
    MMIASP_BUTTON_BACKWARD ,
    MMIASP_BUTTON_PLAY_OR_PAUSE,
    MMIASP_BUTTON_FORWARD,
    MMIASP_BUTTON_MAX
}MMIASP_BUTTON_TYPE_E;

typedef enum
{
    MMIASP_CLICKED_BACKWARD_RECT        = 0,
    MMIASP_CLICKED_PLAY_OR_PAUSE_RECT,
    MMIASP_CLICKED_FORWARD_RECT,

    MMIASP_CLICKED_VOLUME_INC_RECT,
    MMIASP_CLICKED_VOLUME_DEC_RECT,

    MMIASP_CLICKED_RECT_NUM
}MMIASP_CLICKED_RECT;

typedef enum
{
    MMIASP_ANSWER_PHONE_FLAG = 0,
  
    MMIASP_ANSWER_AUDIO,
    MMIASP_ANSWER_AUDIO_TIME,
    MMIASP_MSG_TIME,
    MMIASP_WAIT_TIME,
    MMIASP_SPEAKER_FLAG,
    MMIASP_MSG_SAVE_PATH,

    MMIASP_SETTING_MAX_NUM
}MMIASP_SETTING_E;


#define MMIASP_FIXED_ANSWERRING_NUM              8

#define MMIASP_DEFAULT_VOLUME                           3

#define MMIASP_SETTING_SUB_MAX_LIST_NUM       13
#define MMIASP_MAX_STRING_LEN                            512

#define MMIASP_MAX_NAME_LEN           (((MMIPB_MAX_STRING_LEN)+1)*2)
#define MMIASP_SEC_IN_MIN              60        //1M=60S
#define MMIASP_MIN_IN_HOUR             60        //1H=60M
#define MMIASP_SEC_TO_MSEL             1000        //1s = 1000ms
#define MMIASP_DISPLAY_TIME_LENGTH      32 
/*****************************************************************************/
//   Description : 
//   Global resource dependence : none
//   Author: byte.guo
//   Note:    
/*****************************************************************************/
PUBLIC void MMIASP_ShowNewAspMsgWin(void);
/*****************************************************************************/
//  Description : register www module applet info
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_RegAppletInfo(void);
/*****************************************************************************/
//  Description : get ans_mathine module info
//  Global resource dependence : 
//  Author:byte.guo
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_EnterMainFrame(void);
/*****************************************************************************/
//  Description : open 答录的界面
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIASP_OpenRecordingWin(void);
/*****************************************************************************/
//  Description : close 答录的界面
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIASP_CloseRecordingWin(void);

/*****************************************************************************/
//  Description : close 正在释放的界面
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIASP_CloseReleasingWin(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIASP_GetFixedAnswerRingTextId(uint8 ring_id);
/*****************************************************************************/
//  Description : open 答录的界面
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIASP_OpenRingSelectWin(PRINGSELECT_CALLBACKFUNC callback_fun_ptr);
/*****************************************************************************/
//  Description : open 播放界面的界面
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   此函数专供从外部直接进入到播放界面(不是从留言列表界面进入的）
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_OpenPlayWinFromExternal(uint16 node_index);
/*****************************************************************************/
//  Description : 自动播放答录留言.....
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_AutoPlayRecordMessages(void);
/*****************************************************************************/
//  Description : Display the progress according to dynamic playing info..
//  Global resource dependence : 
//  Author: koki.gao
//  Note: 
/*****************************************************************************/
PUBLIC void DisplayASPWinProgress(BOOLEAN is_end);
/*****************************************************************************/
//  Description : 播放界面时间信息显示
//  Global resource dependence : 
//  Author:koki gao
//  Note:
/*****************************************************************************/
PUBLIC void DisplayASPWinProgressTime(BOOLEAN is_end);
/*****************************************************************************/
//  Description :刷新录音界面记时时间
//  Global resource dependence : none
//  Author: koki gao
//  Note:   
/*****************************************************************************/
PUBLIC void MMIASP_UpdateRecordingTime(BOOLEAN is_refresh);
#endif

