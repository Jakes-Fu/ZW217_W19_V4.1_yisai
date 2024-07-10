#ifdef WRE_SUPPORT
#ifndef _MMIWRE_GDI_H_
#define _MMIWRE_GDI_H_
//////////////////////////////////////

#define WRE_FORMAT_BOLD         0x00000200         //����
#define WRE_FORMAT_ITALIC       0x00000400         //б��
#define WRE_FORMAT_UNDERLINE    0x00000020         //�»���Ч��
typedef struct
{
#if 0
    /*not be used,replace by format*/
    unsigned int     dir;            //���ַ���
#else
    unsigned int                format;         //���塢б��ȸ�ʽ   add by heng.xiao 20111014
#endif
    unsigned short		size;        //���ִ�С
    unsigned short      glyph_space; //���μ��
	
    //���²���, �ڻ�������ʱ��Ҫ���
    unsigned  int      font_color;     //������ɫ
    unsigned  int      bg_color;       //������ɫ
    unsigned  int      edge_color;     //���ֱ�Ե��ɫ
    unsigned  int      line_color;     //�»��ߵ���ɫ
	
    unsigned  short    buffer_width;   //buffer�Ŀ��
    unsigned  short    buffer_height;  //buffer�ĸ߶�
    void*              buffer_ptr;     //��ʾ��buffer, ����Ϊ��    
}TXTPARAM,*LPTXTPARAM;
///////////////////////////////////
void GetLcdWHMocor(unsigned char lcd_type, unsigned short *width, unsigned short *height);

void  LCD_Refresh(int lcd,int x1,int y1,int x2,int y2);

void OpenLcd(unsigned char bLcdID);
void CloseLcd(unsigned char bLcdID);
//��Ӧ��������Ҫ��ʾ��BUFFER
//void * SetDrawBuffer(void *pBuffer);
unsigned int MyExtTextOutMocor(int x, int y,TXTPARAM *pTxtParam,unsigned short* lpszString, unsigned short cbCount);
//void * SetSubScreenBuffer(void);
void WRE_GetScreenLogicSize(int* width,int* height);

#endif
#endif

