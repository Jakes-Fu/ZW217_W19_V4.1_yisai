
#include "mmiwatchidle.h"
#include "mmiwatchidle_export.h"
#include "mmidisplay_data.h"

#include "Os_api.h"
#include "mmiset_display.h"

#define m_abs(x) ((x) > 0 ? (x) :(-(x)))


#ifdef ANALOG_CLK_SUPPORT  //yangyu add

#define TOUCH_PANEL_SUPPORT 




typedef enum eScrollDirection
{
    SCROLL_NONE = 0X0,
    SCROLL_LEFT = 0X01,
    SCROLL_RIGHT = 0X02
}eScrollDirection;



typedef void (*DESTROY_FUNC)(void *param);//yangyu add
typedef t_watchidle_callback* (*CREATE_FUNC)(const sWatchPanelData *data ); //yangyu add

typedef struct _WATCH_TYPE_CONTAINER_T_
{
    uint8 watch_index;
    CREATE_FUNC create_func;
    DESTROY_FUNC destroy_func;
}WATCH_TYPE_CONTAINER_T;

WATCH_TYPE_CONTAINER_T analog_watch_list[2] = 
{
    {3, CreateWatchEffect0, DestoryWatchEffect0},   //xiongkai  注意index要和模拟时钟的对应
    {4, CreateWatchEffect1, DestoryWatchEffect1}
};

#define ANALOG_WATCH_COUNT (sizeof(analog_watch_list)/sizeof(WATCH_TYPE_CONTAINER_T))


//the watch page number
#define MAXPAGENUM 2// xiongkai only two analog clock 3
#define doubThres 0.00001

GUI_LCD_DEV_INFO s_layer[MAXPAGENUM] = {0};
LOCAL t_watchidle_callback *mWatchIdleCB[MAXPAGENUM] = {0};
LOCAL int s_clock_timer_id = 0;
eWatchIdleEffect mWatchIdleEffect = WATCHIDLE_EFFECT0;
BOOLEAN bScroll = FALSE;
int8 nCurrentPageIndex = 0;
eScrollDirection scrollDirection = SCROLL_NONE;
LOCAL uint32 nFpsRedrawTimer = 0;

#define SCREEN_WIDTH 240//368
#define SCREEN_HEIGHT 240//448

WATCH_TYPE_CONTAINER_T getCurrentWatchController()
{
    WATCH_TYPE_CONTAINER_T controller = analog_watch_list[0];

    WATCH_PANEL_TYPE_T style = MMISET_GetWatchPanelStyle();
    if(!style.is_anolog_panel)
    {
        SCI_TRACE_LOW("yyu getCurrentWatchController ,error, is not anolog watch_index=%d ", style.watch_index);       
    }
    else 
    {
        int i = 0;
        if(style.watch_index > analog_watch_list[ANALOG_WATCH_COUNT - 1].watch_index)
        {
            SCI_TRACE_LOW("yyu getCurrentWatchController ,error, index overflow");  

        }
        for(i = 0 ; i < ANALOG_WATCH_COUNT  ; i++)
        {
          if(analog_watch_list[i].watch_index == style.watch_index)
          {
              controller = analog_watch_list[i];
              break;
          }
            

        }  
    }
      
    return  controller;
}

LOCAL void fpsRedraw(uint8  timer_id, uint32 param)
{
    if (nFpsRedrawTimer == timer_id)
    {
        //MMITHEME_DisplayFps(0,0,0);//yangyu delete,no func
    }
}

LOCAL void initLayers(uint16 width, uint16 height)
{
    uint8 i = 0;
    SCI_MEMSET(s_layer, 0, sizeof(s_layer));

    for (i = 0; i < MAXPAGENUM; i++)
    {
        UILAYER_CREATE_T create_info = {0};
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = VIRTUAL_WIN_ID;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = width;
        create_info.height = height;
        create_info.is_bg_layer = TRUE;
        create_info.is_static_layer = TRUE;
        UILAYER_CreateLayer(
                &create_info,
                &s_layer[i]);
    }
}

LOCAL void ReleaseLayers()
{
    uint8 i = 0;
    for (i = 0; i < MAXPAGENUM; i++)
    {
        UILAYER_ReleaseLayer(&s_layer[i]);
    }
    SCI_MEMSET(s_layer, 0, sizeof(s_layer));
}

LOCAL void initWatchPanel()
{
    sWatchPanelData panelData = {0};
    WATCH_TYPE_CONTAINER_T watchController = getCurrentWatchController();//yangyu add
    panelData.mLcdInfo = s_layer[0];
    panelData.mBgColor = MMI_BLACK_COLOR;
    panelData.mPanelImgId = IMAGE_WATCHIDLE_ACLOCK_PANEL1;
    
    //mWatchIdleCB[0] = CreateWatchEffect1(&panelData);
    mWatchIdleCB[0] = watchController.create_func(&panelData);
    updateBackground(mWatchIdleCB[0]);
    updateClock(mWatchIdleCB[0]);
    	
    panelData.mLcdInfo = s_layer[1];
    panelData.mBgColor = MMI_BLACK_COLOR;//RGB888_TO_RGB565(46,46,46);
    panelData.mPanelImgId = IMAGE_WATCHIDLE_ACLOCK_PANEL2;
    //mWatchIdleCB[1] = CreateWatchEffect1(&panelData);
    mWatchIdleCB[1] = watchController.create_func(&panelData);
    updateBackground(mWatchIdleCB[1]);
    updateClock(mWatchIdleCB[1]);

    #if 0  //xiongkai 	 only 2 analog clock	
    panelData.mLcdInfo = s_layer[2];
    panelData.mBgColor = MMI_BLACK_COLOR;
    panelData.mPanelImgId = IMAGE_WATCHIDLE_ACLOCK_PANEL3;
    //mWatchIdleCB[2] = CreateWatchEffect1(&panelData);
    mWatchIdleCB[2] = watchController.create_func(&panelData);
    updateBackground(mWatchIdleCB[2]);
    updateClock(mWatchIdleCB[2]);
    #endif	
}

LOCAL void ReleaseWatchPanel()
{
    int i = 0;
    WATCH_TYPE_CONTAINER_T watchController = getCurrentWatchController();//yangyu add
    for (i = 0; i < MAXPAGENUM; i++)
    {
        //DestoryWatchEffect1(mWatchIdleCB[i]);
        watchController.destroy_func(mWatchIdleCB[i]);
    }
}

LOCAL BOOLEAN isChangedPage(uint16 valChanged, uint16 valMax)
{
    const double thres = 0.25;
    return ((double)valChanged/(double)valMax > thres);
}

LOCAL void updateAllScreen()
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T box = {0,0,239,239};//{0,0,368,448};
    MMITHEME_StoreUpdateRect(&lcd_dev_info,box);
    MMITHEME_UpdateRect();
}

LOCAL void displayLinerPixmap(const uint16 *bufDst, 
                                                         const uint16 *buf1, 
                                                         const uint16 *buf2,
                                                         uint16 width,
                                                         uint16 height,
                                                         uint16 dx)
{
    int i = 0;
    if (bufDst == NULL || buf1 == NULL || buf2 == NULL)
    {
        return;
    }

    if (width == 0 || height == 0)
    {
        return;
    }

    //dx = 0;

    for (i = 0; i < height; i++)
    {
        SCI_MEMCPY(bufDst + width * i,
            buf1 + width * i + dx,
            (width - dx) * sizeof(uint16));
        SCI_MEMCPY(bufDst + width * i + width - dx,
            buf2 + width * i,
            dx * sizeof(uint16));
    }
}

LOCAL void startLinerAnimation_X(const GUI_LCD_DEV_INFO *pLcdInfo,
                                                            const uint16 *bufDst, 
                                                            const uint16 *buf1, 
                                                            const uint16 *buf2,
                                                            uint16 width,
                                                            uint16 height,
                                                            int16 beginVal,
                                                            int16 endVal,
                                                            int16 nAnmations)
{

    const int16 stepPixel = 15;
    uint16 loop = 0;
    uint32 timeBegin = 0;
    uint32 timeEnd = 0;
    
    if (pLcdInfo == NULL 
        || bufDst == NULL 
        || buf1 == NULL 
        || buf2 == NULL)
    {
        return;
    }  

    if (beginVal < endVal)
    {
        int16 tempBegin = beginVal;
        while (tempBegin < endVal)
        {
            loop++;
            timeBegin = SCI_GetTickCount();
            //MMITHEME_StartFps();
            displayLinerPixmap(bufDst, buf1, buf2, width, height, tempBegin);
            timeEnd = SCI_GetTickCount();
            tempBegin += stepPixel;
            //MMITHEME_StopFps();
            updateAllScreen();
            
            timeEnd = SCI_GetTickCount();
            SCI_TRACE_LOW("lzk log. startLinerAnimation_X elapsed = %d, loop = %d.", timeEnd - timeBegin, loop);
            //SCI_Sleep(2);
        }

        displayLinerPixmap(bufDst, buf1, buf2, width, height, endVal);
        updateAllScreen();
    }
    else
    {
        int16 tempBegin = beginVal;
        while (tempBegin > endVal)
        {
            loop++;
            timeBegin = SCI_GetTickCount();
            //MMITHEME_StartFps();
            displayLinerPixmap(bufDst, buf1, buf2, width, height, tempBegin);
            tempBegin -= stepPixel;
            //SCI_Sleep(1);

            updateAllScreen();
            //timeEnd = SCI_GetTickCount();
            SCI_TRACE_LOW("lzk log. startLinerAnimation_X elapsed = %d, loop = %d.", timeEnd - timeBegin, loop);
            //SCI_Sleep(2);
        }

        displayLinerPixmap(bufDst, buf1, buf2, width, height, endVal);
        updateAllScreen();
    }
}


LOCAL void displayImageOnScreen(uint32 srcWidth, 
                                                                uint32 srcHeight, 
                                                                void *pSrcDataBuf,
                                                                const GUI_RECT_T displayRect, 
                                                                const GUI_LCD_DEV_INFO *p_lcd_dev_info)
{
    int i = 0;
    GUI_POINT_T disPointStart = {0};
    GUIIMG_BITMAP_T imgBitmap = {0};
    SCALE_IMAGE_IN_T scale_in = {0};
    SCALE_IMAGE_OUT_T scale_out = {0};
    uint16 picWidth = 0;
    uint16 picHeight = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = *p_lcd_dev_info;

    disPointStart.x = displayRect.left;
    disPointStart.y = displayRect.top;
    picWidth = displayRect.right - displayRect.left;
    picHeight = displayRect.bottom- displayRect.top;

    scale_in.src_format           = IMGREF_FORMAT_RGB565;
    scale_in.src_chn.chn0.ptr     = pSrcDataBuf;
    scale_in.src_chn.chn0.size    = srcWidth * srcHeight * sizeof(uint16) ;
    scale_in.src_size.w           = srcWidth;
    scale_in.src_size.h           = srcHeight;        
    scale_in.src_trim_rect.x      = 0;
    scale_in.src_trim_rect.y      = 0;
    scale_in.src_trim_rect.w      = srcWidth;
    scale_in.src_trim_rect.h      = srcHeight;
    scale_in.target_buf.size      = picWidth * picHeight * sizeof(uint16);          
    scale_in.target_buf.ptr       = malloc(scale_in.target_buf.size);  
    scale_in.target_format        = IMGREF_FORMAT_RGB565;
    scale_in.target_size.w        = picWidth;
    scale_in.target_size.h        = picHeight;
    scale_in.scale_mode           =SCALE_MODE_NO_DISTORT;

    GRAPH_ScaleImage(&scale_in, &scale_out);

    imgBitmap.img_width = picWidth;
    imgBitmap.img_height = picHeight;
    imgBitmap.bit_ptr = scale_in.target_buf.ptr;
    GUIIMG_DisplayBmp(FALSE, &displayRect, &disPointStart, &imgBitmap, &lcd_dev_info);

    MMITHEME_StoreUpdateRect(&lcd_dev_info, displayRect);
    MMITHEME_UpdateRect();

    if (NULL != scale_in.target_buf.ptr)
        free(scale_in.target_buf.ptr);
}

PUBLIC BOOLEAN tranform2d_tranlate(const GUI_RECT_T *srcRect, 
                                                                 GUI_RECT_T *dstRect, 
                                                                 const int16 transX,
                                                                 const int16 transY)
{
    if (srcRect == NULL || dstRect == NULL)
    {
        return FALSE;
    }

    dstRect->left = srcRect->left + transX;
    dstRect->right = srcRect->right + transX;
    dstRect->top = srcRect->top + transY;
    dstRect->bottom = srcRect->bottom+ transY;
    return TRUE;
}

LOCAL void tranform2d_Scale(const GUI_RECT_T *srcRect, 
                                                     GUI_RECT_T *dstRect, 
                                                     const GUI_POINT_T originPoint,
                                                     const double scaleX,
                                                     const double scaleY)
{
    int16 srcWidth = 0;
    int16 srcHeight = 0;
    int16 dstWidth = 0;
    int16 dstHeight = 0;
    GUI_POINT_T srcCenter = {0};
    GUI_POINT_T dstCenter = {0};

    if (srcRect == NULL || dstRect == NULL)
    {
        return ;
    }

    if (scaleX > -doubThres && scaleX < doubThres)
        return ;

    if (scaleY > -doubThres && scaleY < doubThres)
        return ;

    srcWidth = srcRect->right - srcRect->left + 1;
    srcHeight = srcRect->bottom- srcRect->top + 1;

    srcCenter.x = srcRect->left + srcWidth >> 1;
    srcCenter.y = srcRect->top + srcHeight >> 1;

    dstWidth = srcWidth * scaleX;
    dstHeight = srcHeight * scaleY;

    //dstCenter.x = originPoint.x - srcWidth * scaleX;
    //dstCenter.y = originPoint.y - srcHeight * scaleY;

    dstRect->left = dstCenter.x - (dstWidth >> 1);
    dstRect->right = dstRect->left + dstWidth;
    dstRect->top = dstCenter.y - (dstHeight >> 1);
    dstRect->bottom= dstRect->top + dstHeight;
    return;
}

LOCAL uint8 reCalcPageIndex(int8 currentPageIndex, eScrollDirection action)
{
    int8 tempIndex = currentPageIndex;

    if (action == SCROLL_RIGHT)
    {
        if(tempIndex == 0)
            tempIndex = MAXPAGENUM - 1;
        else
        {
            tempIndex--;
        }
    }
    else if (action == SCROLL_LEFT)
    {
        if(tempIndex == MAXPAGENUM - 1)
            tempIndex = 0;
        else
        {
            tempIndex++;
        }
    }

    return tempIndex;
}

LOCAL uint8 getPrePageIndex(int8 currentPageIndex)
{
    return reCalcPageIndex(currentPageIndex, SCROLL_RIGHT);
}

LOCAL uint8 getNextPageIndex(int8 currentPageIndex)
{
    return reCalcPageIndex(currentPageIndex, SCROLL_LEFT);
}

//export apis
///////////////////////////////////////////////////////////////////////////////
//NoUse
PUBLIC BOOLEAN MMIAPIWatchIdle_Open(void)
{
    MMIAPIWatchIdle_SetEffect(1);
    if( !MMK_IsOpenWin( MAIN_IDLE_WIN_ID ) )
    {
        MMI_WINDOW_CREATE_T win_create = {0};
        
        win_create.applet_handle = MMK_GetFirstAppletHandle();
        win_create.win_id = MAIN_IDLE_WIN_ID;
        win_create.func = MMIWatchIdle_HandleWinMsg;
        win_create.win_priority = WIN_LOWEST_LEVEL;
        win_create.window_style = WS_NO_DEFAULT_STYLE|WS_HAS_ANIM_BG|WS_DISABLE_COMMON_BG;//idle锟斤拷锟斤拷锟斤拷anim锟截硷拷为锟斤拷锟斤拷, 锟饺伙拷anim锟襟画达拷锟斤拷
        MMK_CreateWindow( &win_create );
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

uint32 sMoveTimer = 0;
LOCAL void startMoveTimer()
{
    if (0 == sMoveTimer)
    {
        sMoveTimer = MMK_CreateWinTimer(MAIN_IDLE_WIN_ID, 60,TRUE);
    }	
}

LOCAL void stopMoveTimer()
{
    if (0 != sMoveTimer)
    {
        MMK_StopTimer(sMoveTimer);
    }	

    sMoveTimer = 0;
}

LOCAL void drawScrollScene(int32 moveOffset)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T box = {0,0,239,239};//{0,0,368,448};
    //const GUI_COLOR_T *pDstData = GUILCD_GetMainLcdBufPtr();
    const GUI_COLOR_T *pDstData = GetLcdInfoByLcdDev_Ex(&lcd_dev_info);
    if (moveOffset>0)
    {
        uint16 *pDataFirst = UILAYER_GetLayerBufferPtr(&s_layer[getPrePageIndex(nCurrentPageIndex)]);
        uint16 *pDataNext = UILAYER_GetLayerBufferPtr(&s_layer[nCurrentPageIndex]);
        displayLinerPixmap(pDstData, 
            pDataFirst, 
            pDataNext,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SCREEN_WIDTH - moveOffset);
    }
    else
    {
        uint16 *pDataFirst = UILAYER_GetLayerBufferPtr(&s_layer[nCurrentPageIndex]);
        uint16 *pDataNext= UILAYER_GetLayerBufferPtr(&s_layer[getNextPageIndex(nCurrentPageIndex)]);

        SCI_TRACE_LOW("moveleft. moveOffset = %d.", moveOffset);
        displayLinerPixmap(pDstData, 
            pDataFirst, 
            pDataNext,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            -moveOffset);
    }
    MMITHEME_StoreUpdateRect(&lcd_dev_info,box);
}

//WindProc
PUBLIC MMI_RESULT_E MMIWatchIdle_HandleWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    LOCAL GUI_POINT_T tp_press = {0};
    LOCAL GUI_POINT_T tp_move = {0};
    LOCAL GUI_POINT_T tp_release = {0};
    LOCAL int32 moveOffset = 0;
    LOCAL uint32 size = 240*240*sizeof(uint16);//368*448*sizeof(uint16);
    LOCAL GUI_LCD_DEV_INFO lcd_dev_info = {0};
    const GUI_COLOR_T *pDstData = GetLcdInfoByLcdDev_Ex(&lcd_dev_info);
    //const GUI_COLOR_T *pDstData = GUILCD_GetMainLcdBufPtr();
    
    LOCAL GUI_RECT_T box = {0,0,239,239};//{0,0,368,448};
    LOCAL uint32 screenWidth = 240;//368;
    LOCAL uint32 screenHeight = 240;//448;
    BOOLEAN ret = TRUE;
    
    SCI_TRACE_LOW("MMIWatchIdle_HandleWinMsg win_id = %0x, msg_id = %0x.", win_id, msg_id);

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            SCI_TRACE_LOW("MMIWatchIdle_HandleWinMsg. MSG_OPEN_WINDOW.");

             #if 0   //xiongkai   模拟表盘状态栏刷不出来
	       if((watch_style.watch_index==3))	
	       {
                GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_WITHOUT_HEADER, TRUE);
                MMIAPICOM_StatusAreaWithheaderSet();
	       }
             #endif
				
            MMK_PostMsg(win_id,MSG_GET_FOCUS,PNULL,0);

            #if 0   //xiongkai
            if (nFpsRedrawTimer == 0)
            {
                nFpsRedrawTimer = MMK_CreateTimerCallback(1000, fpsRedraw, PNULL, TRUE);
            }
	     #endif		
            break;
        }
		
        case MSG_GET_FOCUS:
        {
            SCI_TRACE_LOW("MMIWatchIdle_HandleWinMsg. MSG_GET_FOCUS.");
            initLayers(screenWidth, screenHeight);
            initWatchPanel();

            bScroll = FALSE;
            stopMoveTimer();
            SCI_MEMSET(&tp_press, 0, sizeof(GUI_POINT_T));
            SCI_MEMSET(&tp_move, 0, sizeof(GUI_POINT_T));
            MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);

            //restart timer.
            MMIWatchIdle_StopClockTimer();
            MMIWatchIdle_StartClockTimer(CLOCK_UPDATE_PERIOD);
            break;
        }

        case MSG_CLOSE_WINDOW:
        {
	 #if 0   //xiongkai   模拟表盘状态栏刷不出来		
	 if((watch_style.watch_index==3))	
	 {
            GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_WITHOUT_HEADER, FALSE);
            MMIAPICOM_StatusAreaWithheaderSet();
	 }
	#endif		
            //ReleaseLayers();
            //ReleaseWatchPanel();			
            MMIWatchIdle_StopClockTimer();
            break;
        }
        case MSG_LOSE_FOCUS:
        {
            SCI_TRACE_LOW("MMIWatchIdle_HandleWinMsg. MSG_LOSE_FOCUS.");
	 #if 0   //xiongkai   模拟表盘状态栏刷不出来	
	 if((watch_style.watch_index==3))	
	 {
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_WITHOUT_HEADER, FALSE);
        MMIAPICOM_StatusAreaWithheaderSet();
	 }
	 #endif	
            ReleaseLayers();
            ReleaseWatchPanel();
            MMIWatchIdle_StopClockTimer();
            break;
        }

#ifdef TOUCH_PANEL_SUPPORT
        case MSG_TP_PRESS_DOWN:
        {
            SCI_TRACE_LOW("MMIWatchIdle_HandleWinMsg. MSG_TP_PRESS_DOWN.");
            tp_press.x = MMK_GET_TP_X(param);
            tp_press.y = MMK_GET_TP_Y(param);
            tp_move = tp_press;
            bScroll = FALSE;
            stopMoveTimer();
            break;
        }
	
        case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T tp_cur = {0};
            SCI_TRACE_LOW("MMIWatchIdle_HandleWinMsg. MSG_TP_PRESS_MOVE.");
            tp_cur.x = MMK_GET_TP_X(param);
            tp_cur.y = MMK_GET_TP_Y(param);
            moveOffset = tp_cur.x - tp_press.x;

            //MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL, 0);
            //MMK_SendMsg(win_id,MSG_FULL_PAINT, 0);
            if (!bScroll)
            {
                bScroll = TRUE;
                startMoveTimer();
            }
            MMIWatchIdle_StopClockTimer();
            break;
        }

        case MSG_APP_GREEN:
        {
            if (!bScroll)
            {
                static BOOLEAN b2D = TRUE;
                if (nCurrentPageIndex == 2)
                    MMIWatchIdle_CreateAnimWin(&b2D);
                else
                {
                    int i = 0;
                    const uint8 oneStep = 10;
                    GUI_POINT_T originPoint = {0};
                    GUI_RECT_T displayRect = {0};
                    GUI_RECT_T pageRect[MAXPAGENUM] = {0};
                    
                    displayRect.right = screenWidth - 1;
                    displayRect.bottom= screenHeight - 1;

                    originPoint.x = screenWidth >> 1;
                    originPoint.y = screenHeight >> 1;

                    pageRect[1] = displayRect;
                    tranform2d_tranlate(&pageRect[1], &pageRect[0], screenWidth * (-1), 0);
                    tranform2d_tranlate(&pageRect[1], &pageRect[2], screenWidth, 0);
                  
                    tranform2d_Scale(&pageRect[0], &pageRect[0], originPoint, 0.8, 0.8);
                    tranform2d_Scale(&pageRect[1], &pageRect[1], originPoint, 0.8, 0.8);
                    tranform2d_Scale(&pageRect[2], &pageRect[2], originPoint, 0.8, 0.8);

                    LCD_FillRect(&lcd_dev_info, box, MMI_BLACK_COLOR);

                    displayImageOnScreen(screenWidth, 
                            screenHeight, 
                            UILAYER_GetLayerBufferPtr(&s_layer[0]),
                            pageRect[0],
                            &lcd_dev_info);
                    displayImageOnScreen(screenWidth, 
                            screenHeight, 
                            UILAYER_GetLayerBufferPtr(&s_layer[1]),
                            pageRect[1],
                            &lcd_dev_info);

		      #if 0		//xiongkai  only 2 analog clock	
                    displayImageOnScreen(screenWidth, 
                            screenHeight, 
                            UILAYER_GetLayerBufferPtr(&s_layer[2]),
                            pageRect[2],
                            &lcd_dev_info);
			#endif
			
                    #if 0
                    for (i = 0 ; i < 20; i++)
                    {
                        LCD_FillRect(&lcd_dev_info, box, MMI_BLACK_COLOR);

                        displayRect.left += oneStep;
                        displayRect.top += oneStep;
                        displayRect.right -= oneStep;
                        displayRect.bottom -= oneStep;

                        displayImageOnScreen(screenWidth, 
                            screenHeight, 
                            UILAYER_GetLayerBufferPtr(&s_layer[nCurrentPageIndex]),
                            displayRect,
                            &lcd_dev_info);
                        SCI_Sleep(1);
                    }
                    #endif
                    
                }
                break;
            }
            break;
        }

        case MSG_TP_PRESS_UP:
        {
            SCI_TRACE_LOW("MMIWatchIdle_HandleWinMsg. MSG_TP_PRESS_UP.");
            tp_release.x = MMK_GET_TP_X(param);
            tp_release.y = MMK_GET_TP_Y(param);
            if (!bScroll)
            {
                if (nCurrentPageIndex == 2)
                    MMIWatchIdle_CreateAnimWin(NULL);
                else
                    MMIAPIMENU_CreatMainMenu();
                break;
            }
            else
            {
                int16 distenceX = tp_release.x - tp_press.x;
                BOOLEAN bChangePage = isChangedPage(abs(tp_release.x-tp_press.x), screenWidth);
                if (bChangePage)
                {
                    int8 nOldPageIndex = nCurrentPageIndex;
                    eScrollDirection action = distenceX > 0 ? SCROLL_RIGHT : SCROLL_LEFT;
                    nCurrentPageIndex = reCalcPageIndex(nOldPageIndex, action);

                    if (action == SCROLL_LEFT)
                    {
                        int16 offset = -distenceX;
                        startLinerAnimation_X(&lcd_dev_info, 
                            pDstData, 
                            UILAYER_GetLayerBufferPtr(&s_layer[nOldPageIndex]), 
                            UILAYER_GetLayerBufferPtr(&s_layer[getNextPageIndex(nOldPageIndex)]),
                            screenWidth, 
                            screenHeight, 
                            offset, 
                            screenWidth,
                            0);
                    }
                    else
                    {
                        int16 offset = screenWidth - distenceX;
                        startLinerAnimation_X(&lcd_dev_info, 
                            pDstData, 
                            UILAYER_GetLayerBufferPtr(&s_layer[getPrePageIndex(nOldPageIndex)]), 
                            UILAYER_GetLayerBufferPtr(&s_layer[nOldPageIndex]), 
                            screenWidth, 
                            screenHeight, 
                            offset, 
                            0, 
                            0);
                    }
                }
                else
                {
                    eScrollDirection action = distenceX > 0 ? SCROLL_RIGHT : SCROLL_LEFT;
                    if (action == SCROLL_LEFT)
                    {
                        int16 offset = -distenceX;
                        startLinerAnimation_X(&lcd_dev_info, 
                            pDstData, 
                            UILAYER_GetLayerBufferPtr(&s_layer[nCurrentPageIndex]), 
                            UILAYER_GetLayerBufferPtr(&s_layer[getNextPageIndex(nCurrentPageIndex)]),
                            screenWidth, 
                            screenHeight, 
                            offset, 
                            0,
                            0);
                    }
                    else
                    {
                        int16 offset = screenWidth -distenceX;
                        startLinerAnimation_X(&lcd_dev_info, 
                            pDstData, 
                            UILAYER_GetLayerBufferPtr(&s_layer[getPrePageIndex(nCurrentPageIndex)]), 
                            UILAYER_GetLayerBufferPtr(&s_layer[nCurrentPageIndex]), 
                            screenWidth, 
                            screenHeight, 
                            offset, 
                            screenWidth, 
                            0);
                    }
                }

                updateBackground(mWatchIdleCB[nCurrentPageIndex]);
                updateClock(mWatchIdleCB[nCurrentPageIndex]);
                SCI_MEMCPY(pDstData, UILAYER_GetLayerBufferPtr(&s_layer[nCurrentPageIndex]), size);
                MMITHEME_StoreUpdateRect(&lcd_dev_info,box);
                MMITHEME_UpdateRect();
                
                bScroll = FALSE;
                stopMoveTimer();
                MMIWatchIdle_StartClockTimer(CLOCK_UPDATE_PERIOD);
            }
            break;
        }
#endif

        case MSG_FULL_PAINT:
        {
            SCI_TRACE_LOW("MMIWatchIdle_HandleWinMsg. MSG_FULL_PAINT.");
            if (!bScroll)
            {
                uint8 *pData = NULL;
                SCI_TRACE_LOW("MMIWatchIdle_HandleWinMsg. draw clock begin.");

                pData = UILAYER_GetLayerBufferPtr(&s_layer[nCurrentPageIndex]);

                updateBackground(mWatchIdleCB[nCurrentPageIndex]);
                updateClock(mWatchIdleCB[nCurrentPageIndex]);
				
                SCI_MEMCPY(pDstData, pData, size);
                MMITHEME_StoreUpdateRect(&lcd_dev_info,box);
                //MMITHEME_UpdateRect();

                SCI_TRACE_LOW("MMIWatchIdle_HandleWinMsg. draw clock end.");
            }
            else
            {
                #if 0
                //uint32 Time001 = SCI_GetTickCount();
                //uint32 Time002 = 0;
                //uint32 Time003 = 0;
                //uint32 Time004 = 0;
                //uint32 Time005 = 0;
                //move right
                if (moveOffset>0)
                {
                    uint16 *pDataFirst = UILAYER_GetLayerBufferPtr(&s_layer[getPrePageIndex(nCurrentPageIndex)]);
                    uint16 *pDataNext = UILAYER_GetLayerBufferPtr(&s_layer[nCurrentPageIndex]);
                    //Time002 = SCI_GetTickCount();
                    displayLinerPixmap(pDstData, 
                        pDataFirst, 
                        pDataNext,
                        screenWidth,
                        screenHeight,
                        screenWidth - moveOffset);
                    //Time003 = SCI_GetTickCount();
                }
                else
                {
                    uint16 *pDataFirst = UILAYER_GetLayerBufferPtr(&s_layer[nCurrentPageIndex]);
                    uint16 *pDataNext= UILAYER_GetLayerBufferPtr(&s_layer[getNextPageIndex(nCurrentPageIndex)]);

                    //Time002 = SCI_GetTickCount();
                    SCI_TRACE_LOW("moveleft. moveOffset = %d.", moveOffset);
                    displayLinerPixmap(pDstData, 
                        pDataFirst, 
                        pDataNext,
                        screenWidth,
                        screenHeight,
                        -moveOffset);
                    //Time003 = SCI_GetTickCount();
                }
                
                MMITHEME_StoreUpdateRect(&lcd_dev_info,box);
                //Time004 = SCI_GetTickCount();
                //MMITHEME_UpdateRect();
                //GUILCD_Invalidate(GUI_MAIN_LCD_ID, 0);
                //Time005 = SCI_GetTickCount();

                //SCI_TRACE_LOW("lzk log. MSG_FULL_PAINT times = %d, %d, %d, %d, %d.",
                //    Time001, Time002,Time003,Time004,Time005);
                //SCI_TRACE_LOW("lzk log. MSG_FULL_PAINT elapsed = %d, %d, %d, %d.",
                //    Time002-Time001, Time003 - Time002, Time004 - Time003, Time005- Time004);
                #endif
            }

            break;
        }

        case MSG_TIMER:
        {
            if (NULL == param)
                break;
            if (s_clock_timer_id == *(uint8*)param)
            {
                if(MMK_GetFocusWinId() !=MAIN_IDLE_WIN_ID)   //xiongkai
                {
                    MMIWatchIdle_StopClockTimer();
                    //ReleaseLayers();
                    //ReleaseWatchPanel();		
			break;		
                }
                MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
            }
            else if (sMoveTimer== *(uint8*)param)
            {
                //MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
                drawScrollScene(moveOffset);
            }
            break;
        }

        case MSG_APP_WEB:
        {
            SCI_TRACE_LOW("MMIWatchIdle_HandleWinMsg. MSG_APP_WEB.");
                if (nCurrentPageIndex == 2)
                    MMIWatchIdle_CreateAnimWin(NULL);
                else
                    MMIAPIMENU_CreatMainMenu();
            break;	
        }

        case MSG_APP_LEFT:
        case MSG_APP_UP:
        {
            uint16 *pDataFirst = UILAYER_GetLayerBufferPtr(&s_layer[getPrePageIndex(nCurrentPageIndex)]);
            uint16 *pDataNext = UILAYER_GetLayerBufferPtr(&s_layer[nCurrentPageIndex]);
            startLinerAnimation_X(&lcd_dev_info, 
                pDstData, 
                pDataFirst, 
                pDataNext,
                screenWidth, 
                screenHeight, 
                screenWidth, 
                0,
                0);
            nCurrentPageIndex = getPrePageIndex(nCurrentPageIndex);
            break;
        }

        case MSG_APP_RIGHT:
        case MSG_APP_DOWN:
        case MSG_APP_OK:
        {
            uint16 *pDataFirst = UILAYER_GetLayerBufferPtr(&s_layer[nCurrentPageIndex]);
            uint16 *pDataNext= UILAYER_GetLayerBufferPtr(&s_layer[getNextPageIndex(nCurrentPageIndex)]);
            startLinerAnimation_X(&lcd_dev_info, 
                pDstData, 
                pDataFirst, 
                pDataNext,
                screenWidth, 
                screenHeight, 
                0, 
                screenWidth,
                0);
            nCurrentPageIndex = getNextPageIndex(nCurrentPageIndex);
            
            break;
        }

        default:
            ret = FALSE;
            break;
    }
    SCI_TRACE_LOW("MMIWatchIdle_HandleWinMsg. end.");
    return ret;
}


PUBLIC void MMIWatchIdle_StartClockTimer(int usec)
{
    if (0 == s_clock_timer_id)
    {
        s_clock_timer_id = MMK_CreateWinTimer(MAIN_IDLE_WIN_ID,usec,TRUE);
    }	
}

PUBLIC void MMIWatchIdle_StopClockTimer(void)
{
    BOOLEAN result = FALSE;

    if (0 < s_clock_timer_id)
    {
        MMK_StopTimer(s_clock_timer_id);
        s_clock_timer_id = 0;

        result = TRUE;
    }

    return;
}

PUBLIC BOOLEAN MMIAPIWatchIdle_SetEffect(eWatchIdleEffect effect)
{
    if (effect >= WATCHIDLE_EFFECT_COUNT)
    {
        SCI_TRACE_LOW("MMIAPIWatchIdle_SetEffect(), effect value invalid.effect = %d.\n", effect);
    }
    SCI_TRACE_LOW("MMIAPIWatchIdle_SetEffect(), effect = %d.\n", effect);
    return TRUE;
}


PUBLIC eWatchIdleEffect MMIAPIWatchIdle_GetEffect()
{
    return mWatchIdleEffect;
}


PUBLIC void updateBackground(void* param)
{
    t_watchidle_callback *obj = (t_watchidle_callback *)param;
    if (obj != NULL)
    {
        obj->funUpdateBg(obj);
    }
}


PUBLIC void updateClock(void* param)
{
    t_watchidle_callback *obj = (t_watchidle_callback *)param;
    if (obj != NULL)
    {
        obj->funUpdateClock(obj);
    }
}




PUBLIC int8 MMIWatchIdle_GetCurrentPageIndex()
{
    return nCurrentPageIndex;
}
#endif


//yangyu add 
typedef struct 
{
    uint8 page_index;
    BOOLEAN is_anolog_watch;
    MMI_IMAGE_ID_T img_id;
    GUI_RECT_T l_black_rect;
    GUI_RECT_T img_rect;
    GUI_RECT_T r_black_rect;
    
}PAGE_HOLD_T;

typedef struct 
{
    uint8 page_index;
    MMI_CTRL_ID_T ctrl_id; 
    MMI_IMAGE_ID_T img_id;
    GUI_RECT_T ctrl_rect;
}PAGE_HOLD2_T;


#define SCREEN_W    MMI_MAINSCREEN_WIDTH
#define SCREEN_H    MMI_MAINSCREEN_HEIGHT
#define BLACK_GAP   0//10
#define IMG_WIDTH    MMI_MAINSCREEN_WIDTH//200

//#define PAGE_COUNT  4///3 //all watch panel face count

LOCAL uint16 x_move = 0;
LOCAL uint8 tp_down_page_index  = 0;

#define MAX_PAGE_COUNT 10

//LOCAL PAGE_HOLD_T page_list[PAGE_COUNT] = 
LOCAL PAGE_HOLD_T page_list[] = 
{
    //{0, FALSE, PANEL_SAMPLE_1, {0}, {0}, {0}},
    //{1, TRUE, PANEL_SAMPLE_2, {0}, {0}, {0}},
    //{2, TRUE, PANEL_SAMPLE_3, {0}, {0}, {0}}
    {0, FALSE, PANEL_SAMPLE_1, {0}, {0}, {0}},
    {1, FALSE, PANEL_SAMPLE_2, {0}, {0}, {0}},
    {2, FALSE, PANEL_SAMPLE_3, {0}, {0}, {0}},
#ifdef ANALOG_CLK_SUPPORT    //   4  5  是模拟时钟    
    {3, TRUE, PANEL_SAMPLE_4, {0}, {0}, {0}},
    {4, TRUE, PANEL_SAMPLE_5, {0}, {0}, {0}},    
#endif    
  // //xiongkai ADD_DEL_IDLE_DIGIT_CLOCK_NUMBER   {3, FALSE, PANEL_SAMPLE_4, {0}, {0}, {0}}

};

LOCAL PAGE_HOLD2_T page_list2[MAX_PAGE_COUNT] = {0};

#define PAGE_COUNT  (sizeof(page_list)/sizeof(PAGE_HOLD_T))//3//4///3 //all watch panel face count




#if 0//old
LOCAL uint8 getCurrentPageIndex()
{
    uint8 cur_index = 0;
    int i = 0;
    for(i = 0; i < PAGE_COUNT ; i++)
    {
       SCI_TRACE_LOW("yyu getCurrentPageIndex. i = %d, left =%d, right =%d", i,page_list[i].img_rect.left, page_list[i].img_rect.right); 
       if(page_list[i].img_rect.left >= 0 && page_list[i].img_rect.right <= SCREEN_W-1)
       {
            cur_index = i;
            break;
       }
    }

    return cur_index;


}
#else
LOCAL uint8 getCurrentPageIndex()
{
    uint8 cur_index = 0;
    int i = 0;
    for(i = 0; i < PAGE_COUNT ; i++)
    {
       SCI_TRACE_LOW("yyu getCurrentPageIndex. i = %d, left =%d, right =%d", i,page_list[i].img_rect.left, page_list[i].img_rect.right); 
       if(page_list2[i].ctrl_rect.left >= 0 && page_list2[i].ctrl_rect.right <= SCREEN_W-1)
       {
            cur_index = i;
            break;
       }
    }

    return cur_index;


}

#endif




void initPageListRect()
{
    int i = 0;
    uint16 first_left_gap = SCREEN_W > IMG_WIDTH ? (SCREEN_W/2  - IMG_WIDTH/2 - 1):0;
    uint16 last_right_gap = first_left_gap;
    int16 all_move_x = 0;
    uint16 top = (SCREEN_H - IMG_WIDTH)/2;
    uint16 bottom = SCREEN_H - (SCREEN_H - IMG_WIDTH)/2;
    WATCH_PANEL_TYPE_T watch_style = MMISET_GetWatchPanelStyle();
    all_move_x = -watch_style.watch_index*(IMG_WIDTH+BLACK_GAP*2);
    for(i = 0; i < PAGE_COUNT ; i++)
    {
        if(SCREEN_W == IMG_WIDTH)
        {
            if(i == 0)
            {
                page_list[i].img_rect.top = top;
                page_list[i].img_rect.bottom = bottom;
                page_list[i].img_rect.left = 0+all_move_x;
                page_list[i].img_rect.right = SCREEN_W-1+all_move_x;

            }
            else
            {

                page_list[i].img_rect.top = top;
                page_list[i].img_rect.bottom = bottom;
                page_list[i].img_rect.left = page_list[i-1].img_rect.right + 1;
                page_list[i].img_rect.right = page_list[i].img_rect.left + IMG_WIDTH - 1;

            }

        }
        else
        {
            if(i == 0)
            {
                page_list[i].l_black_rect.top = top;
                page_list[i].l_black_rect.bottom = bottom;
                page_list[i].l_black_rect.left = 0+all_move_x;
                page_list[i].l_black_rect.right = first_left_gap+all_move_x;
                page_list[i].img_rect.top = top;
                page_list[i].img_rect.bottom = bottom;
                page_list[i].img_rect.left = page_list[i].l_black_rect.right + 1;
                page_list[i].img_rect.right = page_list[i].img_rect.left + IMG_WIDTH-1;
                page_list[i].r_black_rect.top = top;
                page_list[i].r_black_rect.bottom = bottom;
                page_list[i].r_black_rect.left = page_list[i].img_rect.right + 1;
                page_list[i].r_black_rect.right = page_list[i].r_black_rect.left + BLACK_GAP-1;
            }
            else
            {
                page_list[i].l_black_rect.top = top;
                page_list[i].l_black_rect.bottom = bottom;
                page_list[i].l_black_rect.left = page_list[i-1].r_black_rect.right + 1;
                page_list[i].l_black_rect.right = page_list[i].l_black_rect.left + BLACK_GAP - 1;
                page_list[i].img_rect.top = top;
                page_list[i].img_rect.bottom = bottom;
                page_list[i].img_rect.left = page_list[i].l_black_rect.right + 1;
                page_list[i].img_rect.right = page_list[i].img_rect.left + IMG_WIDTH - 1;
                page_list[i].r_black_rect.top = top;
                page_list[i].r_black_rect.bottom = bottom;
                page_list[i].r_black_rect.left = page_list[i].img_rect.right + 1;
                page_list[i].r_black_rect.right = page_list[i].r_black_rect.left + BLACK_GAP - 1;
            }
            if(PAGE_COUNT - 1 == i)
            {
                page_list[i].r_black_rect.right = page_list[i].r_black_rect.left + last_right_gap;
            }
        }
        
    }
    //getCurrentPageIndexP();
    if(1) 
    {
        //PAGE_HOLD_T *ptr = page_list;

    }
}

LOCAL BOOLEAN isRectShouldDraw(GUI_RECT_T rect)
{
    BOOLEAN ret = FALSE;
    if((rect.left >= 0 && rect.left <= SCREEN_W - 1)
        ||(rect.right >= 0 && rect.right <= SCREEN_W - 1)
    )
    {
        ret = TRUE;
    }
    if(rect.left == rect.right)
    {
        ret = FALSE;

    }
    return ret;
}


#if 0
void flingMoveDisplayAllPage(int16 move_x)
{
    int i = 0;
    GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
    int16 last_right = (SCREEN_W == IMG_WIDTH) ? page_list[PAGE_COUNT-1].img_rect.right : page_list[PAGE_COUNT-1].r_black_rect.right;
    if(last_right + move_x < SCREEN_W-1)
    {
        move_x = SCREEN_W-1 - last_right;
    }

    
    MMK_GetWinLcdDevInfo(MMIWATCHIDLE_ANIM_WIN_ID, &lcd_dev_info);
    SCI_TRACE_LOW("yyu flingMoveDisplayAllPage. move_x = %d, last_right = %d.", move_x,last_right);
    for(i = 0; i < PAGE_COUNT ; i++)
    {
        page_list[i].l_black_rect.left += move_x;
        page_list[i].l_black_rect.right += move_x;
        page_list[i].img_rect.left += move_x;
        page_list[i].img_rect.right += move_x;
        page_list[i].r_black_rect.left += move_x;
        page_list[i].r_black_rect.right += move_x;
        if(isRectShouldDraw(page_list[i].l_black_rect))
        {
            SCI_TRACE_LOW("yyu l_black_rect i=%d move_x = %d.",i, move_x);
        	GUI_FillRect(&lcd_dev_info,page_list[i].l_black_rect, MMI_BLACK_COLOR);
        }
        if(isRectShouldDraw(page_list[i].img_rect))
        {
            if(page_list[i].img_id != IMAGE_NULL)
            {
                SCI_TRACE_LOW("yyu img_rect i=%d move_x = %d.",i, move_x);
                GUIRES_DisplayImg(PNULL,
                                     &page_list[i].img_rect,
                                     PNULL,
                                     MMIWATCHIDLE_ANIM_WIN_ID,
                                     page_list[i].img_id,
                                     &lcd_dev_info);
             }
        }
        if(isRectShouldDraw(page_list[i].r_black_rect))
        {
        SCI_TRACE_LOW("yyu r_black_rect i=%d move_x = %d.",i, move_x);
        	GUI_FillRect(&lcd_dev_info,page_list[i].r_black_rect, MMI_BLACK_COLOR);
        }


    }
}
#else

void flingMoveDisplayAllPage(int16 move_x)
{
    int i = 0;
    GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
   

    
    //MMK_GetWinLcdDevInfo(MMIWATCHIDLE_ANIM_WIN_ID, &lcd_dev_info);
    SCI_TRACE_LOW("yyu flingMoveDisplayAllPage. move_x = %d", move_x);
    for(i = 0; i < PAGE_COUNT ; i++)
    {
        page_list2[i].ctrl_rect.left += move_x;
        page_list2[i].ctrl_rect.right += move_x;

        GUIANIM_SetCtrlRect(page_list2[i].ctrl_id, &page_list2[i].ctrl_rect);
    }
    //MMITHEME_UpdateRect();
    //MMK_UpdateScreen();
    MMK_SendMsg(MMIWATCHIDLE_ANIM_WIN_ID, MSG_FULL_PAINT, NULL);
}
#endif
#if 0
LOCAL void checkAllPageInCorrectPosition()
{
    
    int16 moved_dis = m_abs(page_list[0].l_black_rect.left);
    int16 return_gap = moved_dis % (2*BLACK_GAP + IMG_WIDTH);
    int16 back_gap = 0;
    int16 first_left = page_list[0].l_black_rect.left;
    int16 last_right = (SCREEN_W == IMG_WIDTH) ? page_list[PAGE_COUNT-1].img_rect.right : page_list[PAGE_COUNT-1].r_black_rect.right;
    if(SCREEN_W == IMG_WIDTH)
    {
        first_left = page_list[0].img_rect.left;
        moved_dis = m_abs(first_left);
        return_gap = moved_dis % (2*BLACK_GAP + IMG_WIDTH);
    }

    if(last_right < SCREEN_W-1)
    {
         back_gap = (SCREEN_W-1) - last_right;
         flingMoveDisplayAllPage(back_gap);
         return;   
    }
    if(first_left > 0)
    {
        back_gap = 0 - first_left;
        flingMoveDisplayAllPage(back_gap);
        return; 
    }
    
    if(return_gap != 0)
    {
        if(return_gap < (2*BLACK_GAP + IMG_WIDTH)/2)
        {
            if(first_left > 0)
            {
                back_gap = -return_gap;
            }
            else
            {
                back_gap = return_gap;

            }
        }
        else
        {
            if(first_left > 0)
            {
                back_gap = -return_gap;
            }
            else
            {
                back_gap = return_gap;

            }
        }
        SCI_TRACE_LOW("yyu checkAllPageInCorrectPosition. back_gap = %d. first_left = %d.", back_gap ,first_left);    
        flingMoveDisplayAllPage(back_gap);
    }
}
#else
LOCAL void checkAllPageInCorrectPosition()
{
    
    int16 moved_dis = m_abs(page_list2[0].ctrl_rect.left);
    int16 return_gap = moved_dis % IMG_WIDTH;
    int16 back_gap = 0;
    int16 first_left = page_list2[0].ctrl_rect.left;
    int16 last_right = page_list2[PAGE_COUNT-1].ctrl_rect.right;//(SCREEN_W == IMG_WIDTH) ? page_list[PAGE_COUNT-1].img_rect.right : page_list[PAGE_COUNT-1].r_black_rect.right;
    
    SCI_TRACE_LOW("yyu checkAllPageInCorrectPosition. last_right = %d. first_left = %d.", last_right ,first_left);

    if(last_right < SCREEN_W-1)
    {
         back_gap = (SCREEN_W-1) - last_right;
         flingMoveDisplayAllPage(back_gap);
         return;   
    }
    else if(first_left > 0)
    {
        back_gap = 0 - first_left;
        flingMoveDisplayAllPage(back_gap);
        return; 
    }
    
    else if(return_gap != 0)
    {
        if(return_gap < IMG_WIDTH/2)
        {
            if(first_left > 0)
            {
                back_gap = -return_gap;
            }
            else
            {
                back_gap = return_gap;

            }
        }
        else
        {
            if(first_left > 0)
            {
                back_gap = -return_gap;
            }
            else
            {
                back_gap = return_gap;

            }
        }
        SCI_TRACE_LOW("yyu checkAllPageInCorrectPosition. back_gap = %d. first_left = %d.", back_gap ,first_left);    
        flingMoveDisplayAllPage(back_gap);
    }
}

#endif



//yangyu end


/**/
//picture full screen preview
WINDOW_TABLE (MMIWATCHIDLE_ANIM_WIN_TAB) =
{
    WIN_FUNC ( (uint32) MMIWatchIdle_HandleAnimMsg),
    WIN_ID (MMIWATCHIDLE_ANIM_WIN_ID),
    WIN_HIDE_STATUS,
    WIN_STYLE (WS_HAS_ANIM_BG | WS_DISABLE_COMMON_BG),
    CREATE_ANIM_CTRL (IMAGE_SAMPLE_ANIM_CTRL_ID_1, MMIWATCHIDLE_ANIM_WIN_ID), //same count with PAGE_COUNT
    CREATE_ANIM_CTRL (IMAGE_SAMPLE_ANIM_CTRL_ID_2, MMIWATCHIDLE_ANIM_WIN_ID),
    CREATE_ANIM_CTRL (IMAGE_SAMPLE_ANIM_CTRL_ID_3, MMIWATCHIDLE_ANIM_WIN_ID),
    #if defined(ANALOG_CLK_SUPPORT)   //  4 5 是模拟时钟   这里不控好，长按选表盘缩略图就显示不出来
    CREATE_ANIM_CTRL (IMAGE_SAMPLE_ANIM_CTRL_ID_4, MMIWATCHIDLE_ANIM_WIN_ID),   // //xiongkai ADD_DEL_IDLE_DIGIT_CLOCK_NUMBER  
    CREATE_ANIM_CTRL (IMAGE_SAMPLE_ANIM_CTRL_ID_5, MMIWATCHIDLE_ANIM_WIN_ID),     
    #endif
    //CREATE_SETLIST_CTRL (MMIFMM_PREVIEW_SETLIST_CTRL_ID),
    //WIN_SOFTKEY (IMAGE_NULL, IMAGE_NULL, IMAGE_NULL),
    END_WIN
};

LOCAL void createAnimCtrl(MMI_CTRL_ID_T ctrl_id, MMI_ANIM_ID_T anim_id)
{
    GUI_RECT_T rect = {0};
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    ctrl_info.is_ctrl_id = TRUE;
    ctrl_info.ctrl_id    = ctrl_id;

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_update   = TRUE;
    display_info.bg.bg_type = GUI_BG_COLOR;
    display_info.bg.img_id  = MMI_RED_COLOR;
    //display_info.is_zoom = TRUE;
    display_info.is_auto_zoom_in = TRUE;
    data_info.img_id= anim_id;
    GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
    //GUIANIM_SetCtrlRect(ctrl_id, &rect);
}

LOCAL void InitPageImageAnimCtrl()
{
    int i = 0;
    GUI_RECT_T rect_full = {0, 0, MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT};
    WATCH_PANEL_TYPE_T watch_style = MMISET_GetWatchPanelStyle();
    MMI_CTRL_ID_T start_ctrl_id = IMAGE_SAMPLE_ANIM_CTRL_ID_1;
    MMI_ANIM_ID_T start_img_id = PANEL_SAMPLE_1;
    
    for(i = 0; i < PAGE_COUNT; i++)
    {
        GUI_RECT_T rect = rect_full;
        rect.left = (i-watch_style.watch_index)*MMI_MAINSCREEN_WIDTH;
        rect.right = rect.left + MMI_MAINSCREEN_WIDTH-1;
        page_list2[i].page_index = i;
        page_list2[i].ctrl_id = start_ctrl_id + i;
        page_list2[i].img_id = start_img_id + i;
        page_list2[i].ctrl_rect = rect;
        createAnimCtrl(page_list2[i].ctrl_id, page_list2[i].img_id);
        
        GUIANIM_SetCtrlRect(page_list2[i].ctrl_id, &page_list2[i].ctrl_rect);
    }
    
    

}


LOCAL uint16 i_down_x = 0;
LOCAL int16 last_x = 0;
LOCAL uint16 i_up_x = 0;
LOCAL int16 total_move_x = 0;
LOCAL BOOLEAN is_just_move = FALSE;

LOCAL BOOLEAN fling_moving = FALSE;
LOCAL WATCH_PANEL_TYPE_T watch_type_array[3] =
{
    {FALSE, 0},
    {TRUE, 1},
    {TRUE, 2}
};
LOCAL uint8 index = 0;

LOCAL void startFlingMoveTimer();
LOCAL void stopFlingMoveTimer();





#define FLING_TIMER_PERIOD  20


LOCAL uint8 fling_timer_id = 0;
LOCAL int16 step_gap = 0;
#if 0
void flingMoveFunc( uint8 timer_id, uint32 param)
//void flingMoveFunc( uint8 timer_id, uint32 param , MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr)
{
  
    int16 fling_offset = 0;
    uint16 fling_abs = 0;
    BOOLEAN is_reach_end = FALSE;
    uint16 old_total_move_x = total_move_x;
    
    

    if(!fling_moving)
    {
        stopFlingMoveTimer();    
        return;
    }

    if(m_abs(total_move_x) == 2*BLACK_GAP + IMG_WIDTH )
    {
        checkAllPageInCorrectPosition();
        is_reach_end = TRUE;
        fling_moving = FALSE;
        stopFlingMoveTimer();
        return;
    }
    if(total_move_x > 0)
    {
        total_move_x += step_gap;
        if(total_move_x > 2*BLACK_GAP + IMG_WIDTH)
        {
            total_move_x = 2*BLACK_GAP + IMG_WIDTH;
        }
        flingMoveDisplayAllPage(total_move_x - old_total_move_x);
    }
    else if(total_move_x < 0)
    {
        total_move_x -= step_gap;
        if(total_move_x < -(2*BLACK_GAP + IMG_WIDTH))
        {
            total_move_x = -(2*BLACK_GAP + IMG_WIDTH);
        }
        flingMoveDisplayAllPage(total_move_x - old_total_move_x);

    }

    if(m_abs(total_move_x) >= 2*BLACK_GAP + IMG_WIDTH)
    {
        checkAllPageInCorrectPosition();
        fling_moving = FALSE;
        stopFlingMoveTimer();
        
        
    }
    
    
    return;

}
#else
void flingMoveFunc( uint8 timer_id, uint32 param)
//void flingMoveFunc( uint8 timer_id, uint32 param , MMIMAINMENU_MATRIX_DATA_T *menu_ctrl_ptr)
{
  
    int16 fling_offset = 0;
    uint16 fling_abs = 0;
    BOOLEAN is_reach_end = FALSE;
    uint16 old_total_move_x = total_move_x;
    
    

    if(!fling_moving)
    {
        stopFlingMoveTimer();    
        return;
    }

    if(m_abs(total_move_x) == IMG_WIDTH )
    {
        checkAllPageInCorrectPosition();
        is_reach_end = TRUE;
        fling_moving = FALSE;
        stopFlingMoveTimer();
        return;
    }
    if(total_move_x > 0)
    {
        total_move_x += step_gap;
        if(total_move_x > IMG_WIDTH)
        {
            total_move_x = IMG_WIDTH;
        }
        flingMoveDisplayAllPage(total_move_x - old_total_move_x);
    }
    else if(total_move_x < 0)
    {
        total_move_x -= step_gap;
        if(total_move_x < -IMG_WIDTH)
        {
            total_move_x = -IMG_WIDTH;
        }
        flingMoveDisplayAllPage(total_move_x - old_total_move_x);

    }

    if(m_abs(total_move_x) >= IMG_WIDTH)
    {
        checkAllPageInCorrectPosition();
        fling_moving = FALSE;
        stopFlingMoveTimer();
        
        
    }
    
    
    return;

}

#endif


LOCAL void stopFlingMoveTimer()
{
    if(fling_timer_id != 0)
    {
        MMK_StopTimer(fling_timer_id);
    }
    fling_timer_id = 0;
    fling_moving = FALSE;

}

LOCAL void startFlingMoveTimer()
//LOCAL void StartFlingMoveTimer(MMI_HANDLE_T ctrl_handle)
{
    if(!fling_moving)
    {
        return;
    }
    if(fling_timer_id != 0)
    {
        MMK_StopTimer(fling_timer_id);
    }
    
    step_gap = 30;//40;//240/FLING_FRAME_NUM;//;40*2/3;
    fling_timer_id =  MMK_CreateTimerCallback(FLING_TIMER_PERIOD, flingMoveFunc, 0, TRUE);
    //mainmenu_fling_timer_id =  MMK_CreateWinTimer(ctrl_handle, FLING_TIMER_GAP, TRUE);
    

}


PUBLIC BOOLEAN MMIWatch_IsInSelectWatchWin()
{
    MMI_WIN_ID_T win_id = MMK_GetFocusWinId();
    return win_id == MMIWATCHIDLE_ANIM_WIN_ID ;
}

#ifdef ZDT_BH_W9_FEATRUE
#define MENU_TP_OFFSET_VALUE 20
LOCAL uint8 s_menu_pen_timer_falg=0;
LOCAL GUI_POINT_T   s_menu_tp_down_point = {0};
LOCAL int select_watch_index=0;
LOCAL uint8 s_menu_tp_timer_id = 0;

//MMI_IMAGE_ID_T watch_switch_img_id[PAGE_COUNT]={PANEL_SAMPLE_1,PANEL_SAMPLE_2,PANEL_SAMPLE_3,
						  //PANEL_SAMPLE_4,PANEL_SAMPLE_5,PANEL_SAMPLE_6};

MMI_IMAGE_ID_T watch_switch_img_id[]={PANEL_SAMPLE_1,PANEL_SAMPLE_2,PANEL_SAMPLE_3};

void watch_menu_timer_handler(
                                                                        uint8 timer_id,
                                                                        uint32 param
                                                                        )
{
	s_menu_pen_timer_falg=0;
      if(0 != s_menu_tp_timer_id)
      {
          MMK_StopTimer(s_menu_tp_timer_id);
          s_menu_tp_timer_id = 0;
      }
}

/*****************************************************************************
 * FUNCTION
 *  watch_factory_register_input_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void watch_menu_pen_down_hdlr(GUI_POINT_T pt)
{
    s_menu_tp_down_point = pt;
    s_menu_pen_timer_falg=1;
    if(0 != s_menu_tp_timer_id)
    {
        MMK_StopTimer(s_menu_tp_timer_id);
        s_menu_tp_timer_id = 0;
    }
    
    s_menu_tp_timer_id = MMK_CreateTimerCallback(100, 
                                                                        watch_menu_timer_handler, 
                                                                        (uint32)PNULL, 
                                                                        FALSE);
}

static void watch_menu_pen_up_hdlr(GUI_POINT_T pt)
{
    int offset_y =  pt.y - s_menu_tp_down_point.y;//up to down
    int offset_x =  pt.x - s_menu_tp_down_point.x;//up to down

	if(offset_x <= -(MENU_TP_OFFSET_VALUE))	
	{
		select_watch_index+=1;
		if(select_watch_index>2)
		{
		    select_watch_index=0;
		}
	       MMK_SendMsg(MMIWATCHIDLE_ANIM_WIN_ID, MSG_FULL_PAINT, PNULL);
	}	
	else if(offset_x >= (MENU_TP_OFFSET_VALUE))
	{
		select_watch_index-=1;
		if(select_watch_index<0)
		{
		    select_watch_index=2;
		}
		MMK_SendMsg(MMIWATCHIDLE_ANIM_WIN_ID, MSG_FULL_PAINT, PNULL);
	}
	else if(s_menu_pen_timer_falg==0)
	{
            if((pt.x>40)&&(pt.x<200)&&(pt.y>40)&&(pt.y<200))
            {
                WATCH_PANEL_TYPE_T cur_type = {0};
                cur_type.watch_index=select_watch_index;
                MMISET_SetWatchPanelStyle(cur_type);
                MMK_CloseWin(MMIWATCHIDLE_ANIM_WIN_ID);
            }
	}
}

LOCAL Display_Watch_Switch_Img(MMI_WIN_ID_T win_id)
{
	WATCH_PANEL_TYPE_T style = MMISET_GetWatchPanelStyle();
	GUI_POINT_T         dis_point = {0};
	GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	GUIRES_DisplayImg(&dis_point,
				  PNULL,
				  PNULL,
				  win_id,
				  watch_switch_img_id[select_watch_index],
				  &lcd_dev_info);
}

PUBLIC BOOLEAN MMIWatchIdle_HandleAnimMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    BOOLEAN ret = MMI_RESULT_TRUE;
    WATCH_PANEL_TYPE_T cur_type = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
    GUI_RECT_T full_rect = {0,0,MMI_MAINSCREEN_WIDTH-1,MMI_MAINSCREEN_HEIGHT-1};
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
	       SCI_TRACE_LOW("ZW100_W9_LOG:select_watch_index00=%d",select_watch_index);
	      cur_type = MMISET_GetWatchPanelStyle();
	      select_watch_index=cur_type.watch_index;
	      Display_Watch_Switch_Img(win_id);
            break;
        }
        case MSG_FULL_PAINT:
        {
            Display_Watch_Switch_Img(win_id);
            break;
        }  
        
        case MSG_CLOSE_WINDOW:
        {
	     MMK_CloseWin(win_id);
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            watch_menu_pen_down_hdlr(point);
        }
        break;
        
    case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            watch_menu_pen_up_hdlr(point);
        }
        break;
        
    case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
    case MSG_TP_PRESS_LONG:
        {
            GUI_POINT_T   point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_CTL_ANIM_GIF_UPDATE:
        {
         }
	  break;
        case MSG_LOSE_FOCUS:
        {  
        	MMK_CloseWin(win_id);
        	break;
        }        

        case MSG_APP_CANCEL:
        {
        	MMK_CloseWin(win_id);
        	break;
        }
        case MSG_KEYUP_DOWN:
        {
		select_watch_index+=1;
		if(select_watch_index>2)
		{
		    select_watch_index=0;
		}
		SCI_TRACE_LOW("ZW100_W9_LOG:select_watch_index01=%d",select_watch_index);
	       MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        case MSG_KEYUP_UP:
        {	
		
		select_watch_index-=1;
		SCI_TRACE_LOW("ZW100_W9_LOG:select_watch_index02=%d",select_watch_index);
		if(select_watch_index<0)
		{
		    select_watch_index=2;
		}
		MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        case MSG_KEYUP_OK:
        {  
	    SCI_TRACE_LOW("ZW100_W9_LOG:select_watch_index03=%d",select_watch_index);
	     cur_type.watch_index=select_watch_index;
            MMISET_SetWatchPanelStyle(cur_type);
            MMK_CloseWin(win_id);
        }
        break;        
        default:
        {
            ret = MMI_RESULT_FALSE;
            break;
        }
    }
    return ret;
}

#else
PUBLIC BOOLEAN MMIWatchIdle_HandleAnimMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    BOOLEAN ret = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIWATCHIDLE_ANIM_CTRL_ID;
    WATCH_PANEL_TYPE_T cur_type = MMISET_GetWatchPanelStyle();
    GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
    GUI_RECT_T full_rect = {0,0,MMI_MAINSCREEN_WIDTH-1,MMI_MAINSCREEN_HEIGHT-1};
    //SCI_TRACE_LOW("MMIWatchIdle_HandleAnimMsg. win_id = %0x, msg_id = %0x.", win_id, msg_id);
    
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            
            void *param = MMK_GetWinAddDataPtr(win_id);
            if (param == NULL)
            {
                //createAnimCtrl(ctrl_id, IMAGE_WATCHIDLE_ANIM_2D_1);
            }
            else
            {
                //createAnimCtrl(ctrl_id, IMAGE_WATCHIDLE_ANIM_3D_1);
            }
            
            //initPageListRect();
            InitPageImageAnimCtrl();
            
            break;
        }
        case MSG_FULL_PAINT:
        {
            //GUI_FillRect(&lcd_dev_info, full_rect, MMI_DARK_BLUE_COLOR);
            //flingMoveDisplayAllPage(0);
            break;
        }  
        
        case MSG_CLOSE_WINDOW:
        {
            break;
        }

        case MSG_CTL_ANIM_GIF_UPDATE:
        {

            break;
        }
        case MSG_LOSE_FOCUS:
        {  
        	stopFlingMoveTimer();
        	break;
        }        

        case MSG_APP_CANCEL:
        {
            
        	MMK_CloseWin(win_id);
        	break;
        }

        case MSG_APP_RED:
        {
        	MMK_ReturnIdleWin();
        	break;
        }
        
        case MSG_TP_PRESS_DOWN:
        {
            i_down_x = MMK_GET_TP_X(param);
            is_just_move = FALSE;
            SCI_TRACE_LOW("yyu MSG_TP_PRESS_DOWN. i_down_x = %d. fling_moving=%d", i_down_x, fling_moving);
            if(fling_moving)
            {
                break;
            }
            last_x = i_down_x;
            total_move_x = 0;
            tp_down_page_index = getCurrentPageIndex();
        }
        break;
        case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T point = {0};
            uint8 cur_index = 0;
            int16 gap = 0;
            point.x = MMK_GET_TP_X(param);
            is_just_move = TRUE;
            SCI_TRACE_LOW("yyu MSG_TP_PRESS_MOVE. i_down_x = %d, x =%d, fling_moving =%d", i_down_x,point.x, fling_moving);
            
            if(fling_moving)
            {
                break;
            }
            cur_index = tp_down_page_index;
            gap = point.x - last_x;
            SCI_TRACE_LOW("yyu MSG_TP_PRESS_MOVE. cur_index = %d,  gap =%d", cur_index,gap);
            
            if((cur_index == 0 && gap > 0)
               || (cur_index == (PAGE_COUNT - 1) && gap < 0)

            )
            {
            
            }
            else
            {
                
                if(1)//for bug
                {
                    int16 temp_total = total_move_x + gap;
                    if(temp_total > IMG_WIDTH)
                    {
                        checkAllPageInCorrectPosition();
                        total_move_x = 0; //for bug
                        
                        break;
                    }
                }
                flingMoveDisplayAllPage(gap);
                
                total_move_x += gap;
                last_x = point.x;
                SCI_TRACE_LOW("yyu MSG_TP_PRESS_MOVE. i_down_x = %d. total_move=%d,last_x=%d", i_down_x, total_move_x, last_x);
            }
            
        }
        break;
        case MSG_TP_PRESS_UP:
        {  
           uint16 up_x= MMK_GET_TP_X(param);
           SCI_TRACE_LOW("yyu MSG_TP_PRESS_UP. i_down_x = %d. total_move=%d,last_x=%d, up_x=%d", i_down_x, total_move_x, last_x,up_x);
           if(m_abs(total_move_x) > 0 && m_abs(total_move_x) < 2*BLACK_GAP + IMG_WIDTH)
           {
                fling_moving  = TRUE;
           }
           else
           {
               fling_moving  = FALSE;
               checkAllPageInCorrectPosition();//for bug
           }

           if(fling_moving)
           {
               startFlingMoveTimer(); 
           }
           if(!is_just_move && !fling_moving)
           {
                uint8 cur_index = getCurrentPageIndex();
                WATCH_PANEL_TYPE_T style = {0};
                style.watch_index= page_list[cur_index].page_index;
                style.is_anolog_panel = page_list[cur_index].is_anolog_watch;
                MMISET_SetWatchPanelStyle(style);
                MMK_CloseWin(win_id);
           }
           else if( !fling_moving && up_x - i_down_x > 40 && getCurrentPageIndex() == 0)
           {
                MMK_SendMsg(MMIWATCHIDLE_ANIM_WIN_ID, MSG_APP_CANCEL, PNULL);
           }
           

           /*
           if(up_x == i_down_x)
           {
                index = cur_type.watch_index;
                if(index < sizeof(watch_type_array)/sizeof(WATCH_PANEL_TYPE_T) - 1)
                {
                    index++;
                }
                else
                {
                    index = 0;
                }
                MMISET_SetWatchPanelStyle(watch_type_array[index]);

           }
           i_down_x = 0;
           */
        }
        break;        
        default:
        {
            ret = MMI_RESULT_FALSE;
            break;
        }
    }
    return ret;
}
#endif

extern BOOLEAN     is_creat_anim;
PUBLIC void MMIWatchIdle_CreateAnimWin(void *param)
{
    if(is_creat_anim)	//xiongkai   太空人动画没停止下来，切换到模拟时钟后不断跳出太空人动画
    {
        is_creat_anim=FALSE;
        MMK_DestroyControl(MMIIDLE_ANIM_CTRL_ID);   
    }	

    MMK_CreateWin((uint32 *)MMIWATCHIDLE_ANIM_WIN_TAB, param);
}




