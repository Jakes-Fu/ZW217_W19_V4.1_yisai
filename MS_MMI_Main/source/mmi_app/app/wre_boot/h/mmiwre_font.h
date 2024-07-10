#ifdef WRE_SUPPORT
#ifndef _MMIWRE_FONT_H_
#define _MMIWRE_FONT_H_

typedef struct
{
    unsigned short  width;  //���
    unsigned short  height; //�߶�
    //�����ֶ���ʱ����
    unsigned short  ascent;
    unsigned short  leading;
    short   lsb;
    short   rsb;
    short   bearing_y;

}ADPTXTMETRIC,*LPADPTXTMETERC;

/************************************************************
Function:        GetMwinFontByMocorFont
Author:          ZhangXinFang
Description:     ����Mocor������ö�٣���ȡmwin������ö��  
Params:			 int nFont[IN]--- ����ö��
Return:          Mwin������ö��

*************************************************************/
int GetMwinFontByMocorFont (int nFont);
/************************************************************
Function:        GetMocorFontByMwinFont
Author:          ZhangXinFang
Description:     ����Mwin������ö�٣���ȡmocor������ö��  
Params:			 int nFont[IN]--- ����ö��
Return:          Mocor������ö��

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


