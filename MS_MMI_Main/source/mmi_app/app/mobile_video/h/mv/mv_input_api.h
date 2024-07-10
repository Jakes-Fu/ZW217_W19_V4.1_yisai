#ifndef  _MV_INPUT_API_H
#define  _MV_INPUT_API_H

#include "_Types.h"

enum
{
    CM_NO_KEY,     /*无键值*/
    CM_KEY_CANCEL, /*C键*/
    CM_KEY_CLEAR,  /*无键值*/
    CM_KEY_DIGIT,  /*数字键，此时需要同时将数字键数值作为第二参数传递过来*/
    CM_KEY_OK,     /*确定键*/ 
    CM_KEY_UP,     /*上键*/  
    CM_KEY_DOWN,   /*下键*/
    CM_KEY_LEFT,   /*左键*/
    CM_KEY_RIGHT,  /*右*/
    CM_KEY_SEND,   /*呼叫键，绿键*/
    CM_KEY_END,    /*挂机键，红键*/
    CM_KEY_MENU,   /*无键值*/
    CM_KEY_STAR,   /*无键值*/
    CM_KEY_HASH,   /*无键值*/
    CM_KEY_SKL, 		/*左软键*/
    CM_KEY_SKR, 		/*右软键*/
    CM_KEY_SOFTKEY3, /*无键值*/
    CM_KEY_SIDEUP, /* 侧音量上键*/
    CM_KEY_SIDEDOWN, /*侧音量下键*/
    CM_KEY_CAMERA, /* 照相机键*/
};

enum
{
   CM_NO_DIGIT,
   CM_DIGIT_1, /*1*/
   CM_DIGIT_2, /*2*/
   CM_DIGIT_3, /*3*/
   CM_DIGIT_4, /*4*/
   CM_DIGIT_5, /*5*/
   CM_DIGIT_6, /*6*/
   CM_DIGIT_7, /*7*/
   CM_DIGIT_8, /*8*/
   CM_DIGIT_9, /*9*/
   CM_DIGIT_0, /*0*/
   CM_MAX_DIGIT_VALUE
};

enum
{
    CM_SHORT_PRESS,
    CM_LONG_PRESS,
    CM_REPEAT_PRESS,
    CM_RELEASE_PRESS,
    CM_MAX_TYPE_OF_PRESS
};

enum			
{
    CM_TP_DOWN,		//鼠标按下，目前应用只需要一个按下的消息
    CM_TP_UP,		//鼠标弹起
};


_INT CM_Key_CallBack(_INT keyCode, _INT Digital,_INT TypeOfPress);
_INT CM_Touch_CallBack(_INT vEvent, _INT x, _INT y, _INT vParam);

#endif
