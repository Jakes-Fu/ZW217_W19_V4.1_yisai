#ifndef MMIWATCHIDLEEXPORT_H
#define MMIWATCHIDLEEXPORT_H

#include "mmk_type.h"


#ifdef ANALOG_CLK_SUPPORT  //yangyu add

typedef void (*HANDLE_FUNC_CALLBACK)(void *param);



typedef BOOLEAN (*HANDLE_FUNC_OLDCALLBACK)(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

typedef struct sWatchPanelData
{
    uint16 mWidth;
    uint16 mHeight;

    MMI_WIN_ID_T mWinId;
    GUI_LCD_DEV_INFO mLcdInfo;
    uint16 mBgColor; //±³¾°É«

    MMI_IMAGE_ID_T mPanelImgId;
    MMI_IMAGE_ID_T mHourImgId;
    MMI_IMAGE_ID_T mMinuteImgId;
    MMI_IMAGE_ID_T mSecondImgId;
}sWatchPanelData;

typedef struct 
{
    HANDLE_FUNC_CALLBACK funCreateBg;
    HANDLE_FUNC_CALLBACK funCreateClock;
    HANDLE_FUNC_CALLBACK funUpdateBg;
    HANDLE_FUNC_CALLBACK funUpdateClock;	
    HANDLE_FUNC_CALLBACK funDestoryBg;
    HANDLE_FUNC_CALLBACK funDestoryClock;	

    //
    sWatchPanelData *m_pData;
    
}t_watchidle_callback;


//watchidle effect 
typedef enum eWatchIdleEffect
{
    WATCHIDLE_EFFECT0 = 0,
    WATCHIDLE_EFFECT1,

    WATCHIDLE_EFFECT_COUNT
}eWatchIdleEffect;




//Definition minute of closk update time
#define CLOCK_UPDATE_PERIOD 1000

/*****************************************************************************/
// 	Description : open watch idle window
//	Global resource dependence :
//  Author: zhikun.lv
//	Note: Nouse
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWatchIdle_Open(void);

PUBLIC BOOLEAN MMIAPIWatchIdle_SetEffect(eWatchIdleEffect effect);

PUBLIC eWatchIdleEffect MMIAPIWatchIdle_GetEffect();

/******************************************************************************/
//  Description : MMIWatchIdle_HandleWinMsg
//  Global resource dependence : none
//  Author: zhikun.lv
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWatchIdle_HandleWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
PUBLIC int8 MMIWatchIdle_GetCurrentPageIndex();
PUBLIC void MMIWatchIdle_CreateAnimWin(void *param);


PUBLIC t_watchidle_callback *CreateWatchEffect1(const sWatchPanelData *data);
PUBLIC void DestoryWatchEffect1(t_watchidle_callback *obj);
PUBLIC t_watchidle_callback *CreateWatchEffect0(const sWatchPanelData *data);
PUBLIC void DestoryWatchEffect0(t_watchidle_callback *obj);

#endif


PUBLIC BOOLEAN MMIWatchIdle_HandleAnimMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);//select watch face panel
#endif
