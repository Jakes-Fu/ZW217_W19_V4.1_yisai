#ifndef _MMIWRE_IME_H_
#define _MMIWRE_IME_H_
#ifdef WRE_SUPPORT
//typedef unsigned int DWORD;

#define VIRTUAL_IME_ID		0xff010000
#define MSG_IME_CALLPROC	0xff100000

#define  _IN_ALL					0x00 //�༭�ַ�����
#define  _IN_ONLYNUMBER				0x01 //�༭����������
#define  _IN_ONLYPHONENUMBER		0x02 //�༭�����ڲ���ר������

typedef struct tagIMECALLPROCPARAM_T
{
    void  (*EditProc)(const void*  hWnd, unsigned int wParam,unsigned int lParam); 
    //wParam��ͨ���༭����ַ���																				 //lParam��ͨ���༭����ַ������ȣ���16λ���͹��λ�ã���16λ��
    void* hWnd; 	 //Edit�����ھ������EditProc�ĵ�һ��������Ӧ��
    int  wLimitLen; //�ַ��������ƣ�0��ʾ������
    int *pszInitData;	 //��ʼ���ַ���
    int  wInitDataLen;	//��ʼ���ַ�������
    int  bIsMask;	//�����־,TRUE ��ʾΪ����;FALSE ��ʾ������
    int cMask;	//����
    int  wCursorPos;	//���λ��,���Ǹ��ַ�֮��,ָһ���ַ����ĳ���
    int   dwImeType;	//��������
    int  wImeCurKb;   //���뷨��ǰ����
    int dwImeSupportKBs;//���뷨֧�ֵļ�����
	
    int dwHelper;
	int dwReserved[8];
	
} IMECALLPROCPARAM,*PIMECALLPROCPARAM;

/************************************************************************
* DESCRIPTION:
*    ����MOCOR�����뷨
* PARAMETERS:
*     hIMEWnd:      [IN] WRE�Ĵ��ھ��
*     msg:          [IN] ������Ϣ
*     wParam:       [IN] ��Ϣ����
*     lParam:       [reserved]
* RETURN:
*     0             ʧ��
*     !0            �ɹ�
************************************************************************/
long MMIWRE_TriggerMocorIme(void* hIMEWnd,uint32 msg,uint32 wParam,uint32 lParam);

#define CLIP_BORRD_CLEAR 0
#define CLIP_BORRD_GET 1
#define CLIP_BORRD_SET 2
unsigned short clipboardfuncproc(void *pBuffer,unsigned short wDataLength,int opt);
#define  WreClipBoardOpt clipboardfuncproc

#endif

#endif //_ADAPTOR_IME_
