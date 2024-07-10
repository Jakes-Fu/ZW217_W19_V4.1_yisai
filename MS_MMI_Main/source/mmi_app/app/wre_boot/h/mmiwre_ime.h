#ifndef _MMIWRE_IME_H_
#define _MMIWRE_IME_H_
#ifdef WRE_SUPPORT
//typedef unsigned int DWORD;

#define VIRTUAL_IME_ID		0xff010000
#define MSG_IME_CALLPROC	0xff100000

#define  _IN_ALL					0x00 //编辑字符不限
#define  _IN_ONLYNUMBER				0x01 //编辑仅限于数字
#define  _IN_ONLYPHONENUMBER		0x02 //编辑仅限于拨打专用数字

typedef struct tagIMECALLPROCPARAM_T
{
    void  (*EditProc)(const void*  hWnd, unsigned int wParam,unsigned int lParam); 
    //wParam：通常编辑后的字符串																				 //lParam：通常编辑后的字符串长度（高16位）和光标位置（低16位）
    void* hWnd; 	 //Edit自身窗口句柄（与EditProc的第一个参数对应）
    int  wLimitLen; //字符长度限制，0表示无限制
    int *pszInitData;	 //初始化字符串
    int  wInitDataLen;	//初始化字符串长度
    int  bIsMask;	//掩码标志,TRUE 表示为掩码;FALSE 表示非掩码
    int cMask;	//掩码
    int  wCursorPos;	//光标位置,在那个字符之后,指一定字符串的长度
    int   dwImeType;	//输入类型
    int  wImeCurKb;   //输入法当前键盘
    int dwImeSupportKBs;//输入法支持的键盘组
	
    int dwHelper;
	int dwReserved[8];
	
} IMECALLPROCPARAM,*PIMECALLPROCPARAM;

/************************************************************************
* DESCRIPTION:
*    调用MOCOR的输入法
* PARAMETERS:
*     hIMEWnd:      [IN] WRE的窗口句柄
*     msg:          [IN] 触发消息
*     wParam:       [IN] 消息参数
*     lParam:       [reserved]
* RETURN:
*     0             失败
*     !0            成功
************************************************************************/
long MMIWRE_TriggerMocorIme(void* hIMEWnd,uint32 msg,uint32 wParam,uint32 lParam);

#define CLIP_BORRD_CLEAR 0
#define CLIP_BORRD_GET 1
#define CLIP_BORRD_SET 2
unsigned short clipboardfuncproc(void *pBuffer,unsigned short wDataLength,int opt);
#define  WreClipBoardOpt clipboardfuncproc

#endif

#endif //_ADAPTOR_IME_
