#ifndef  _MV_INPUT_API_H
#define  _MV_INPUT_API_H

#include "_Types.h"

enum
{
    CM_NO_KEY,     /*�޼�ֵ*/
    CM_KEY_CANCEL, /*C��*/
    CM_KEY_CLEAR,  /*�޼�ֵ*/
    CM_KEY_DIGIT,  /*���ּ�����ʱ��Ҫͬʱ�����ּ���ֵ��Ϊ�ڶ��������ݹ���*/
    CM_KEY_OK,     /*ȷ����*/ 
    CM_KEY_UP,     /*�ϼ�*/  
    CM_KEY_DOWN,   /*�¼�*/
    CM_KEY_LEFT,   /*���*/
    CM_KEY_RIGHT,  /*��*/
    CM_KEY_SEND,   /*���м����̼�*/
    CM_KEY_END,    /*�һ��������*/
    CM_KEY_MENU,   /*�޼�ֵ*/
    CM_KEY_STAR,   /*�޼�ֵ*/
    CM_KEY_HASH,   /*�޼�ֵ*/
    CM_KEY_SKL, 		/*�����*/
    CM_KEY_SKR, 		/*�����*/
    CM_KEY_SOFTKEY3, /*�޼�ֵ*/
    CM_KEY_SIDEUP, /* �������ϼ�*/
    CM_KEY_SIDEDOWN, /*�������¼�*/
    CM_KEY_CAMERA, /* �������*/
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
    CM_TP_DOWN,		//��갴�£�ĿǰӦ��ֻ��Ҫһ�����µ���Ϣ
    CM_TP_UP,		//��굯��
};


_INT CM_Key_CallBack(_INT keyCode, _INT Digital,_INT TypeOfPress);
_INT CM_Touch_CallBack(_INT vEvent, _INT x, _INT y, _INT vParam);

#endif
