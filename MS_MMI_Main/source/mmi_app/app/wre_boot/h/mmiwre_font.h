#ifdef WRE_SUPPORT
#ifndef _MMIWRE_FONT_H_
#define _MMIWRE_FONT_H_

typedef struct
{
    unsigned short  width;  //宽度
    unsigned short  height; //高度
    //以下字段暂时保留
    unsigned short  ascent;
    unsigned short  leading;
    short   lsb;
    short   rsb;
    short   bearing_y;

}ADPTXTMETRIC,*LPADPTXTMETERC;

/************************************************************
Function:        GetMwinFontByMocorFont
Author:          ZhangXinFang
Description:     根据Mocor的字体枚举，获取mwin的字体枚举  
Params:			 int nFont[IN]--- 字体枚举
Return:          Mwin的字体枚举

*************************************************************/
int GetMwinFontByMocorFont (int nFont);
/************************************************************
Function:        GetMocorFontByMwinFont
Author:          ZhangXinFang
Description:     根据Mwin的字体枚举，获取mocor的字体枚举  
Params:			 int nFont[IN]--- 字体枚举
Return:          Mocor的字体枚举

*************************************************************/
int GetMwinFontByMocorDefFont(void);
int GetMocorFontByMwinFont (int nFont);
int GetStringWidthForMocor(int nFontIdx,unsigned short* pszStr,unsigned short nCCount);
int GetTextMetricsForMocor(int nFontIdx, LPADPTXTMETERC lptm);
int GetTextExtentPointForMocor(
									   int nFontIdx,		/* handle to DC*/
									   const unsigned short* lpszStr,	/* character string*/
									   int cchString,		/* number of characters*/
							   unsigned short* cx,unsigned short* cy);		/* string dimensions*/
int GetTextExtentExPointForMocor(int nFontIdx,	/* handle to DC*/
								  const unsigned short* lpszStr,	/* character string*/
								  int cchString,	/* number of characters*/
								  int nMaxExtent,	/* maximum width of formatted string*/
								  int* lpnFit,		/* maximum number of characters*/
								  int* alpDx,	 	/* array of partial string widths*/
								 unsigned short* cx,unsigned short* cy);	/* string dimensions*/
#endif //_ADAPTOR_FONT_
#endif


