#ifdef WRE_SUPPORT
#ifndef _MMIWRE_GDI_H_
#define _MMIWRE_GDI_H_
//////////////////////////////////////

#define WRE_FORMAT_BOLD         0x00000200         //粗体
#define WRE_FORMAT_ITALIC       0x00000400         //斜体
#define WRE_FORMAT_UNDERLINE    0x00000020         //下划线效果
typedef struct
{
#if 0
    /*not be used,replace by format*/
    unsigned int     dir;            //文字方向
#else
    unsigned int                format;         //粗体、斜体等格式   add by heng.xiao 20111014
#endif
    unsigned short		size;        //文字大小
    unsigned short      glyph_space; //字形间距
	
    //以下参数, 在绘制文字时需要填充
    unsigned  int      font_color;     //文字颜色
    unsigned  int      bg_color;       //背景颜色
    unsigned  int      edge_color;     //文字边缘颜色
    unsigned  int      line_color;     //下划线的颜色
	
    unsigned  short    buffer_width;   //buffer的宽度
    unsigned  short    buffer_height;  //buffer的高度
    void*              buffer_ptr;     //显示的buffer, 不能为空    
}TXTPARAM,*LPTXTPARAM;
///////////////////////////////////
void GetLcdWHMocor(unsigned char lcd_type, unsigned short *width, unsigned short *height);

void  LCD_Refresh(int lcd,int x1,int y1,int x2,int y2);

void OpenLcd(unsigned char bLcdID);
void CloseLcd(unsigned char bLcdID);
//由应用设置需要显示的BUFFER
//void * SetDrawBuffer(void *pBuffer);
unsigned int MyExtTextOutMocor(int x, int y,TXTPARAM *pTxtParam,unsigned short* lpszString, unsigned short cbCount);
//void * SetSubScreenBuffer(void);
void WRE_GetScreenLogicSize(int* width,int* height);

#endif
#endif

