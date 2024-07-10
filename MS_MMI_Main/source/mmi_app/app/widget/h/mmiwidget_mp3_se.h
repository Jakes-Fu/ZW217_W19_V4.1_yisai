#ifndef _WIDGET3DMUSICPLAYER_H_

    #define _WIDGET3DMUSICPLAYER_H_

#define MP3_SE_DISPLAY_NUM 4

    typedef enum
    {
        MMIWIDGET_MP3_SE_ANIM_INVALID = 0,
        MMIWIDGET_MP3_SE_ANIM_SHOW_IN,           
        MMIWIDGET_MP3_SE_ANIM_PLAY,           
        MMIWIDGET_MP3_SE_ANIM_PAUSE2PLAY,      
        MMIWIDGET_MP3_SE_ANIM_PLAY2PAUSE,           
        MMIWIDGET_MP3_SE_ANIM_SWITCH_ON_PAUSE,           
        MMIWIDGET_MP3_SE_ANIM_SWITCH_ON_PLAY,           
        MMIWIDGET_MP3_SE_ANIM_MAX ,
    }MMIWIDGET_MP3_SE_ANIM_TYPE_E;

    #define USE_QVGA_TYPE

typedef struct{
    OBJECT_T* LoadingBG;
    OBJECT_T* music_note;
    OBJECT_T* loading_Rot;
    OBJECT_T* loading_pOutBuf;
} TURNTABLE_LOAD_RES_T;

typedef struct{
    OBJECT_T LoadingBG;
    OBJECT_T music_note;
    OBJECT_T loading_Rot;
    OBJECT_T loading_pOutBuf;
} MMI_TURNTABLE_LOAD_RES_T;

typedef struct{
    OBJECT_T* PlayerSliderLight;
    OBJECT_T* PlayerSliderBG;
    //OBJECT_T* PlayerSliderPoint;
    OBJECT_T* PlayerOutsideFUImg;
    OBJECT_T* PlayerOutsideFDImg;
    OBJECT_T* PlayerOutsideBUImg;
    OBJECT_T* PlayerOutsideBDImg;
    OBJECT_T* PlayerInsideBImg;
    OBJECT_T* PlayerInsideF_PLAYImg;
    OBJECT_T* PlayerInsideF_PAUSEImg;
    OBJECT_T* CoverImg;
    OBJECT_T* CoverImgHeadline;
    uint32 CoverImgNum;
    BOOLEAN IsPlaying;
    int32 PlayPercentage;//0~100
} TURNTABLE_RES_T;

typedef struct{
    OBJECT_T PlayerSliderLight;
    OBJECT_T PlayerSliderBG;
    //OBJECT_T PlayerSliderPoint;
    OBJECT_T PlayerOutsideFUImg;
    OBJECT_T PlayerOutsideFDImg;
    OBJECT_T PlayerOutsideBUImg;
    OBJECT_T PlayerOutsideBDImg;
    OBJECT_T PlayerInsideBImg;
    OBJECT_T PlayerInsideF_PLAYImg;
    OBJECT_T PlayerInsideF_PAUSEImg;
    OBJECT_T CoverImg[MP3_SE_DISPLAY_NUM + 1];
    OBJECT_T CoverImgHeadline[MP3_SE_DISPLAY_NUM + 1];
    uint32 CoverImgNum;
    BOOLEAN IsPlaying;
    int32 PlayPercentage;//0~100
} MMI_TURNTABLE_RES_T;

PUBLIC void WidgetMusicPlayer_CtrlBar(IMGREF_SIZE_T window,
										uint32 *pColor,
										TURNTABLE_RES_T* res,	
										IMGREF_3D_POINT_T Base_3daxis_PointL,
										IMGREF_3D_POINT_T Base_3daxis_PointR,
										IMGREF_3D_POINT_T Rot_3d_axis,
										int32 rot_yAxis_angle,
										int32 rot_yAxis_angle_max,
										int32 player_rot_angle);

PUBLIC void WidgetMusicPlayer_PauseCDChoose(int32 CurFrame, int32 totalFarme,BOOLEAN IsGoBack,IMGREF_SIZE_T window,uint32 *pColor,TURNTABLE_RES_T* res);

PUBLIC void WidgetMusicPlayer_ShowIn(int32 CurFrame, int32 totalFarme, IMGREF_SIZE_T window,uint32 *pColor,TURNTABLE_RES_T* res);

PUBLIC void WidgetMusicPlayer_Pause2Play(int32 CurFrame, int32 totalFarme, IMGREF_SIZE_T window,uint32 *pColor,TURNTABLE_RES_T* res);

PUBLIC void WidgetMusicPlayer_PlayCDChoose(int32 CurFrame, int32 totalFarme,BOOLEAN IsGoBack, IMGREF_SIZE_T window,uint32 *pColor,TURNTABLE_RES_T* res);

PUBLIC void WidgetMusicPlayer_Close(int32 CurFrame, int32 totalFarme, IMGREF_SIZE_T window,uint32 *pColor,TURNTABLE_RES_T* res);

PUBLIC void WidgetMusicPlayer_Play(IMGREF_SIZE_T window,uint32 *pColor,TURNTABLE_RES_T* res);

PUBLIC void TurnTableShow(IMGREF_SIZE_T window,uint32 *pColor,TURNTABLE_RES_T* res, MMIWIDGET_MP3_SE_ANIM_TYPE_E type, BOOLEAN is_goback);

PUBLIC void WidgetMusicPlayer_PicLoadingAnimat(TURNTABLE_LOAD_RES_T* pLoading);

#endif
