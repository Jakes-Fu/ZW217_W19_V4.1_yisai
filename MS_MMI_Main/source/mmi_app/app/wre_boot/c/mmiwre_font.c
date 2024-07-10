#ifdef WRE_SUPPORT
/******************************************************************************
 ** File Name:      adaptor_font.c                                          *
 ** Author:         RiancyZhang                                              *
 ** DATE:           30/12/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file wraps font-layer for mocor os				       *
 **																              *
 ******************************************************************************/
#include "mmiwre_font.h"
#include "spml_api.h"
#include "guifont.h"
#include "mmi_font.h"
#include "mmi_custom_define.h "
#include "../../../../../../wre//wrekernel/include/sdk/font_typedef.h"
//////////////////////////////////////////////////////////////////////
#ifndef WORD
typedef unsigned short WORD;
#endif
#ifndef DWORD
typedef unsigned long DWORD;
#endif
#ifndef BYTE
typedef unsigned char BYTE;
#endif


#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
//#define LOWORD(l)           ((WORD)(l)) /*lint !e767 !e188 !e2573*/
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF)) 
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

//mocor与字体索引对照表
static const unsigned short sw_FONT_TABLE[]={
		//sys font
		MAKEWORD(SYS_FONT06,SONG_FONT_6),
		MAKEWORD(SYS_FONT07,SONG_FONT_7),
		MAKEWORD(SYS_FONT08,SONG_FONT_8),
		MAKEWORD(SYS_FONT09,SONG_FONT_9),
		MAKEWORD(SYS_FONT10,SONG_FONT_10),
		MAKEWORD(SYS_FONT11,SONG_FONT_11),
		MAKEWORD(SYS_FONT12,SONG_FONT_12),//12 dots
		MAKEWORD(SYS_FONT14,SONG_FONT_14),
		MAKEWORD(SYS_FONT16,SONG_FONT_16),//16 dots
		MAKEWORD(SYS_FONT18,SONG_FONT_18),
		MAKEWORD(SYS_FONT20,SONG_FONT_20),//20 dots
		MAKEWORD(SYS_FONT22,SONG_FONT_22),
		MAKEWORD(SYS_FONT24,SONG_FONT_24),//24 dots
		MAKEWORD(SYS_FONT26,SONG_FONT_26),
		MAKEWORD(SYS_FONT28,SONG_FONT_28),
		MAKEWORD(SYS_FONT36,SONG_FONT_36),
		MAKEWORD(SYS_FONT48,SONG_FONT_48),//48 dots
		MAKEWORD(SYS_FONT72,SONG_FONT_64), //MOCOR没有72点的字体,就使用最大的64点
	
};

//////////////////////////////////////////////////////////////////////
/************************************************************
Function:        MwinFontCompare
Author:          ZhangXinFang
Description:     Mwin字体比较接口  
Params:			 const void* arg1[IN] --- 字体组合1
				 const void* arg2[IN] --- 字体组合2
Return:          -1 <
                 0 =
				 1 >

*************************************************************/
static int MwinFontCompare(const void* arg1,const void* arg2);
/************************************************************
Function:        MocorFontCompare
Author:          ZhangXinFang
Description:     Mocor字体比较接口  
Params:			 const void* arg1[IN] --- 字体组合1
				 const void* arg2[IN] --- 字体组合2
Return:          -1 <
                 0 =
				 1 >

*************************************************************/
static int MocorFontCompare(const void* arg1,const void* arg2);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/*****************************************************************************/
//  Description : GetStringWidthForMocor
//  Global resource dependence : 
//  Author:xinfang.zhang
//  Note:
/*****************************************************************************/
int GetStringWidthForMocor(int nFontIdx,unsigned short* pszStr,uint16 nCCount)
{
	GUI_FONT_T	font_type=0;
	SPML_TEXT_METRICS_T metrics = {0};
	SPML_TEXT_PARAM_T param = {0};

    if(pszStr==NULL)
        return 0;

    /*in nFontIdx,high 16 bit is format,low 16 bit is font_type
      add by heng.xiao 20111019*/
    //font_type = (GUI_FONT_T)nFontIdx;
    font_type = (GUI_FONT_T)(nFontIdx&0x0000ffff);
    param.format |= (nFontIdx >> 16);

    param.glyph_space = 0;
    param.dir = TEXT_DIR_AUTO;
    param.size = GUI_ConvertGUIFontToSize( font_type );
    
    SPMLAPI_GetTextMetrics( &param, pszStr, nCCount, &metrics );
    
    return metrics.width;
    
}
/************************************************************
Function:        MwinFontCompare
Author:          ZhangXinFang
Description:     Mwin字体比较接口  
Params:			 const void* arg1[IN] --- 字体组合1
				 const void* arg2[IN] --- 字体组合2
Return:          -1 <
                 0 =
				 1 >

*************************************************************/
static int MwinFontCompare(const void* arg1,const void* arg2)
{
	unsigned char bwf1,bwf2;
	unsigned short *pwf1 = (unsigned short*)arg1;
	unsigned short *pwf2 = (unsigned short*)arg2;
	bwf1 = LOBYTE(*pwf1);
	bwf2 = LOBYTE(*pwf2);

	if( bwf1 < bwf2)
	{
		return -1;
	}
	else if(bwf1 > bwf2)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}
/************************************************************
Function:        MocorFontCompare
Author:          ZhangXinFang
Description:     Mocor字体比较接口  
Params:			 const void* arg1[IN] --- 字体组合1
				 const void* arg2[IN] --- 字体组合2
Return:          -1 <
                 0 =
				 1 >

*************************************************************/
static int MocorFontCompare(const void* arg1,const void* arg2)
{
	unsigned char bwf1,bwf2;
	unsigned short *pwf1 = (unsigned short*)arg1;
	unsigned short *pwf2 = (unsigned short*)arg2;
	bwf1 = HIBYTE(*pwf1);
	bwf2 = HIBYTE(*pwf2);

	if( bwf1 < bwf2)
	{
		return -1;
	}
	else if(bwf1 > bwf2)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}
/************************************************************
Function:        GetMwinFontByMocorFont
Author:          ZhangXinFang
Description:     根据Mocor的字体枚举，获取mwin的字体枚举  
Params:			 int nFont[IN]--- 字体枚举
Return:          Mwin的字体枚举

*************************************************************/
int GetMwinFontByMocorDefFont(void)
{
  return GetMwinFontByMocorFont(MMI_DEFAULT_SMALL_FONT);
}
int GetMwinFontByMocorFont (int nFont)
{
	unsigned short *wRet;
	unsigned short wKeyFont;
	unsigned char BMwin;
	unsigned short wMaxItems = sizeof(sw_FONT_TABLE)/sizeof(unsigned short);

	wKeyFont = MAKEWORD(0xFF,nFont);
	wRet = (unsigned short*)bsearch((const void*)&wKeyFont,(const void*)sw_FONT_TABLE,wMaxItems,sizeof(unsigned short),MocorFontCompare);
	if(NULL != wRet)
	{
		BMwin = LOBYTE(*wRet);
		return BMwin;
	}

	return -1;
}
/************************************************************
Function:        GetMocorFontByMwinFont
Author:          ZhangXinFang
Description:     根据Mwin的字体枚举，获取mocor的字体枚举  
Params:			 int nFont[IN]--- 字体枚举
Return:          Mocor的字体枚举

*************************************************************/
int GetMocorFontByMwinFont (int nFont)
{
	unsigned short *wRet;
	unsigned short wKeyFont;
	unsigned char BMocor;
	unsigned short wMaxItems = sizeof(sw_FONT_TABLE)/sizeof(unsigned short);

	wKeyFont = MAKEWORD(nFont,0xFF);
	wRet = (unsigned short*)bsearch((const void*)&wKeyFont,(const void*)sw_FONT_TABLE,wMaxItems,sizeof(unsigned short),MwinFontCompare);
	if(NULL != wRet)
	{
		BMocor = HIBYTE(*wRet);
		return BMocor;
	}

	return -1;
}


/*****************************************************************************/
//  Description : GetTextMetricsForMocor
//  Global resource dependence : 
//  Author:xinfang.zhang
//  Note:
/*****************************************************************************/
int GetTextMetricsForMocor(int nFontIdx, LPADPTXTMETERC lptm)
{
	GUI_FONT_T	font_type=0;
	SPML_TEXT_METRICS_T metrics = {0};
	SPML_TEXT_PARAM_T param = {0};
	
    if(NULL == lptm)
    {
        return FALSE;
    }

    /*in nFontIdx,high 16 bit is format,low 16 bit is font_type
      add by heng.xiao 20111019*/
    //font_type = (GUI_FONT_T)nFontIdx;
    font_type = (GUI_FONT_T)(nFontIdx&0x0000ffff);
    param.format |= (nFontIdx >> 16);
    
    param.glyph_space = 0;
    param.dir = TEXT_DIR_AUTO;
    param.size = GUI_ConvertGUIFontToSize( font_type );
    
    SPMLAPI_GetTextMetrics( &param, L"W", 1, &metrics );
    
    lptm->ascent = metrics.ascent;
    lptm->bearing_y = metrics.bearing_y;
    lptm->height = param.size;//metrics.height;
    lptm->lsb = metrics.lsb;
    lptm->rsb = metrics.rsb;
    lptm->width = metrics.width;
    lptm->leading=metrics.leading;


	return TRUE;
}

/*****************************************************************************/
//  Description : GetTextExtentPointForMocor
//  Global resource dependence : 
//  Author:xinfang.zhang
//  Note:
/*****************************************************************************/
int GetTextExtentPointForMocor(
							   int nFontIdx,		/* handle to DC*/
							   const unsigned short* lpszStr,	/* character string*/
							   int cchString,		/* number of characters*/
							   unsigned short* cx,
							   unsigned short* cy)		/* string dimensions*/
{
	GUI_FONT_T	font_type=0;
	SPML_TEXT_METRICS_T metrics = {0};
	SPML_TEXT_PARAM_T param = {0};
    
    if(NULL == cx && NULL == cy)
    {
        return FALSE;
    }

    /*in nFontIdx,high 16 bit is format,low 16 bit is font_type
      add by heng.xiao 20111019*/
    //font_type = (GUI_FONT_T)nFontIdx;
    font_type = (GUI_FONT_T)(nFontIdx&0x0000ffff);
    param.format |= (nFontIdx >> 16);
    
    param.glyph_space = 0;
    param.dir = TEXT_DIR_AUTO;
    param.size = GUI_ConvertGUIFontToSize( font_type );
    
    SPMLAPI_GetTextMetrics( &param,lpszStr, cchString, &metrics );
    if(NULL != cx) *cx = metrics.width;
    if(NULL != cy) *cy = param.size;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : GetTextExtentExPointForMocor
//  Global resource dependence : 
//  Author:xinfang.zhang
//  Note:
/*****************************************************************************/
int GetTextExtentExPointForMocor(int nFontIdx,	/* handle to DC*/
								 const unsigned short* lpszStr,	/* character string*/
								 int cchString,	/* number of characters*/
								 int nMaxExtent,	/* maximum width of formatted string*/
								 int* lpnFit,		/* maximum number of characters*/
								 int* alpDx,	 	/* array of partial string widths*/
								 unsigned short* cx,
								 unsigned short* cy)	/* string dimensions*/
{
	BOOLEAN bFlag=FALSE;
	unsigned short out_width=0;
	GUI_FONT_T	font_type=0;
	SPML_TEXT_PARAM_T param = {0};
	
	if(NULL == cx && NULL == cy)
	{
		return FALSE;
	}

    /*in nFontIdx,high 16 bit is format,low 16 bit is font_type
      add by heng.xiao 20111019*/
    //font_type = (GUI_FONT_T)nFontIdx;
    font_type = (GUI_FONT_T)(nFontIdx&0x0000ffff);
    param.format |= (nFontIdx >> 16);

    param.glyph_space = 0;
    param.dir = TEXT_DIR_AUTO;
    param.size = GUI_ConvertGUIFontToSize( font_type );
    
    bFlag=(alpDx)?alpDx[0]:TRUE;
    *lpnFit = SPMLAPI_LineBreak( &param, lpszStr,cchString,nMaxExtent,&out_width,bFlag);
    if( *lpnFit > 0 )
    {
        
        if(NULL != cx) *cx = out_width;
        if(NULL != cy) *cy = param.size;
        
        return TRUE;
    }

	return FALSE;
}

#endif


