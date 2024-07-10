/******************************************************************************
 ** File Name:     Mv_output_api.c                                               *
 ** Author:                                                         *
 ** DATE:           05/11/2010                                                *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    The file is created for mobie video,mainly define the output  interfaces         *
 **                 implementation.                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/11/2010                      Create.                                   *
 ******************************************************************************/
#define _MV_OUTPUT_API_C_
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "mmi_app_mobile_video_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "sfs.h"
#include "sci_api.h"
#include "mv_output_api.h"
#include "guistring.h"
#include "mmi_font.h"
#include "guiblock.h"
#include "guiref_scale.h"
#include "guilcd.h"
#include "guifont.h"
#include "mmk_type.h"
#include "mmK_app.h"
#include "mmi_position.h"
#include "mv_player_api.h"
#include "mmi_theme.h"
#include "mv_debug.h"
#include "mv_base_api.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MV_RGB32_2_RGB16(TXTcolor, color)  \
    do{ \
        TXTcolor = (GUI_COLOR_T) \
            (\
              ( ((color&0x00FF0000)>>19)<<11) | \
              ( ((color&0x0000FF00)>>10)<<5 ) | \
              ( ((color&0x000000FF)>>3)     ) \
            );\
    }while(0)

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

#define ScreenWidth (CM_GetScreenWidth())
#define ScreenHeight (CM_GetScreenHeight())


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

typedef struct {

    _INT nStyle;   //font type : normal , bold, Italic , Underline ,etc
    _INT nSize;    // font size 
 
}MV_FONT; 

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

                                                 
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:  create font
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_HANDLE CM_Font_Create(_WCHAR* nw , _INT nSIze , _UINT nStyle)
{
    MV_FONT* pFont = PNULL;

    UNUSED(nw);
    pFont = (MV_FONT*) SCI_ALLOCA(sizeof(MV_FONT));

    if( PNULL == pFont )
    {
 #ifdef DEBUG_OUTPUT
        //MV_TRACE_LOW:"[MV]--WD[CM_Font_Create]: create font failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_OUTPUT_API_93_112_2_18_2_39_16_126,(uint8*)"");
 #endif
        return PNULL;
    }
    
    SCI_MEMSET(pFont,0,sizeof(MV_FONT));  
    
    pFont->nSize = nSIze;
    pFont->nStyle = nStyle;
#ifdef  DEBUG_OUTPUT
    //MV_TRACE_LOW:"[MV]--WD[CM_Font_Create]: create font ok nSIze = %d, nStyle = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_OUTPUT_API_103_112_2_18_2_39_16_127,(uint8*)"dd", nSIze,nStyle);
#endif
    return pFont;   
	
}

/*****************************************************************************/
//  Description: release font
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_VOID CM_Font_Release(_HANDLE hFont)
{
#ifdef  DEBUG_OUTPUT
    //MV_TRACE_LOW:"[MV]--WD[CM_Font_Release]: release font"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_OUTPUT_API_116_112_2_18_2_39_16_128,(uint8*)"");
#endif
    if (PNULL != hFont)
    {
        SCI_FREE(hFont);
        hFont = PNULL; 
    }	   
}

/*****************************************************************************/
//  Description:  get font height
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_INT CM_Font_GetFontHeight(_HANDLE hFont)
{
    MV_FONT* pFont = (MV_FONT*)hFont;
#ifdef DEBUG_OUTPUT
    //MV_TRACE_LOW("[MV]--WD[CM_Font_GetFontHeight]: font height =%d\n",pFont->nSize);  
#endif        
    return GUIFONT_GetHeight((GUI_FONT_T)pFont->nSize);
	
}
/*****************************************************************************/
//  Description:  get char width
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_INT  CM_Font_GetCharWidth(_HANDLE hFont,_WCHAR wChar)
{
     MV_FONT* pFont = (MV_FONT*)hFont;
#ifdef  DEBUG_OUTPUT
    // MV_TRACE_LOW("[MV]--WD[CM_Font_GetCharWidth]: font width =%d\n",pFont->nSize);   
#endif
     return GUI_GetFontWidth((GUI_FONT_T)pFont->nSize,(_UINT16)wChar);		
}
/*****************************************************************************/
//  Description:  display text string
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_INT CM_Font_TextOut(	_HANDLE hFont,
							_WCHAR wChar,
							_INT nLength,
							_UINT16* pBuffer,
							_UINT16 vBufWidth,
							_UINT16 vBufHeight,
							_UINT16 x,
							_UINT y,
							_UINT16 vMode, 		//输出位置和字体样式
							_UINT16 clipX,
							_UINT16 clipY,
							_UINT16 clipRight,
							_UINT16 clipBottom,//剪裁区域
							_UINT32 vColor
      )									//当前字体颜色
{

    GUI_POINT_T point = {0};
    GUI_RECT_T  clip_rect ={0};
    GUISTR_STYLE_T text_style ={0};
    MMI_STRING_T  str_data={0};
   
    GUI_COLOR_T txt_color =0;
  
    GUISTR_STATE_T  state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE;
    MV_FONT* pFont = (MV_FONT*) hFont;
    GUISTR_BUF_T dest_buf = {0};

//    MV_TRACE_LOW("CM_Font_TextOut,vColor=0x%x,wchar=0x%x,len=%d", vColor,(uint16)wchar,nLength );

#ifdef  DEBUG_OUTPUT
   //MV_TRACE_LOW:"[MV] CM_Font_TextOut hFont = %d, wchar = %x, nLength = %d, pBUffer = %x, x= %d, y =%d, vMode = %d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_OUTPUT_API_186_112_2_18_2_39_16_129,(uint8*)"ddddddd",hFont,wChar,nLength,pBuffer,x,y,vMode);

   //MV_TRACE_LOW:"[MV] CM_Font_TextOut vBufWidth = %d, vBufHeight = %x, clipx = %d, clipy = %x, clipright= %d, clipbottom =%d, vColor = %d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_OUTPUT_API_189_112_2_18_2_39_16_130,(uint8*)"ddddddd",vBufWidth,vBufHeight,clipX,clipY,clipRight,clipBottom,vColor);
#endif

    clip_rect.left = clipX;
    clip_rect.top = clipY;        
    clip_rect.right = clipRight;
    clip_rect.bottom = clipBottom;      

    str_data.wstr_ptr = &wChar;
    str_data.wstr_len = 1;//MMIAPICOM_Wstrlen(&wChar);
    
#ifdef  DEBUG_OUTPUT
    //MV_TRACE_LOW:"[MV] CM_Font_TextOut str_data.wstr_len = %d, ptr = %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_OUTPUT_API_201_112_2_18_2_39_16_131,(uint8*)"ddd",str_data.wstr_len, *(str_data.wstr_ptr),*(str_data.wstr_ptr + 1));
#endif

    //font size
    text_style.font = pFont->nSize;         
    //transfer rgb 32 to rgb 16
    MV_RGB32_2_RGB16(txt_color,vColor);

    //should handle font statle here

    text_style.font_color = txt_color;
    text_style.angle = ANGLE_0;
#ifdef  DEBUG_OUTPUT
    //MV_TRACE_LOW:"[MV] CM_Font_TextOut txt_color = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_OUTPUT_API_214_112_2_18_2_39_16_132,(uint8*)"d",txt_color);
#endif
    //text_style.font_color = 0x0000;   //red color


    dest_buf.buffer = pBuffer;
    dest_buf.mem_width = vBufWidth;
 
    dest_buf.height = ScreenHeight;
    dest_buf.width  = ScreenWidth;
    point.x = x;
    point.y = y;
    

    GUISTR_DrawTextSingleLine( 
                        (GUISTR_BUF_T *)&dest_buf,
                        (const GUI_POINT_T *)&point,       //the fixed display area
                        (const MMI_STRING_T *)&str_data,
                        &text_style,
                        state
                        );
    
    
        return 1;
}

/*****************************************************************************/
//  Description:  display text string
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_INT CM_Font_TextOut_TextOnly(	_HANDLE hFont,
							_WCHAR* pwchar,
							_INT nLength,
							_UINT16* pBuffer,
							_UINT16 vBufWidth,
							_UINT16 vBufHeight,
							_UINT16 x,
							_UINT y,
							_UINT16 vMode, 		//输出位置和字体样式
							_UINT16 clipX,
							_UINT16 clipY,
							_UINT16 clipRight,
							_UINT16 clipBottom,//剪裁区域
							_UINT32 vColor
      )									//当前字体颜色
{

    GUI_POINT_T point = {0};
    GUI_RECT_T  clip_rect ={0};
    GUI_RECT_T  disp_rect ={0};
    GUISTR_STYLE_T text_style ={0};
    MMI_STRING_T  str_data={0};
     
    GUI_COLOR_T txt_color =0;
    GUISTR_STATE_T  state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE;
    MV_FONT* pFont = (MV_FONT*) hFont;
  //  GUISTR_INFO_T str_info = {0};
    GUISTR_BUF_T dest_buf = {0};


#ifdef  DEBUG_OUTPUT
/*   MV_TRACE_LOW("[MV] CM_Font_TextOut_TextOnly hFont = %d, wchar = %x, nLength = %d, pBUffer = %x, x= %d, y =%d, vMode = %d",
       hFont,pwchar,nLength,pBuffer,x,y,vMode);*/
#endif
    clip_rect.left = clipX;
    clip_rect.top = clipY;        
    clip_rect.right = clipRight;
    clip_rect.bottom = clipBottom;      

    disp_rect.left = x;
    disp_rect.top = y;
    disp_rect.right = clipRight;
    disp_rect.bottom = clipBottom;
    str_data.wstr_ptr = pwchar;
    str_data.wstr_len = nLength;//MMIAPICOM_Wstrlen(pwchar);

    //font size
    text_style.font = pFont->nSize;         
    //transfer rgb 32 to rgb 16
    MV_RGB32_2_RGB16(txt_color,vColor);
    //should handle font statle here
    text_style.font_color = txt_color;   

    dest_buf.buffer = pBuffer;
    dest_buf.mem_width = vBufWidth;
    dest_buf.width  = ScreenWidth;
    dest_buf.height = ScreenHeight;

    point.x = x;
    point.y = y;

    GUISTR_DrawTextInRect(
         (GUISTR_BUF_T *)&dest_buf,
         &disp_rect,       //the fixed display area
         &clip_rect,
         (const MMI_STRING_T *)&str_data,
         &text_style,
         state,
         GUISTR_TEXT_DIR_AUTO
        );

        return 1;
}
 	 								 

/*****************************************************************************/
//  Description:  refresh area screen
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_VOID CM_RefreshAreaScreen(void* pBuffer,  
									_INT nX,
									_INT nY,
									_INT nWidth,
									_INT nHeight
								)
{
    _UINT16* pDest = PNULL;
    _UINT16* pSrc = PNULL;
    int32 i = 0;
    GUI_RECT_T box = {0};
#ifdef  DEBUG_OUTPUT
//   MV_TRACE_LOW("[MV] CM_RefreshAreaScreen pBuffer = %d, nX = %d, nY = %d, nWidth = %d, nHeight= %d",pBuffer,nX,nY,nWidth,nHeight);
#endif
    //uint32 cur_refresh_time = SCI_GetTickCount();
    /* 如果当前是全屏状态，则不调用刷屏接口 */
    if (MV_MediaPlayer_IsFullScreen())
    {
        return;
    }
/*
    nWidth++;
    nHeight++;
*/
    /*  得到lcd buffer 指针 */
    pDest = CM_GetScreenBuffer(); /* 得到lcd buffer 指针*/
    if(PNULL==pDest)
    {
        return;
    }
    pSrc  = (_UINT16*)pBuffer;

    /* 将传入的buffer内容复制到lcd buffer */
    pDest += nY * ScreenWidth + nX;
    pSrc +=  nY * ScreenWidth + nX;

    for(i=0;i<=nHeight-1;i++)
    {
        SCI_MEMCPY(pDest, pSrc, nWidth<<1);
        pDest += ScreenWidth;
        pSrc  += ScreenWidth;
    }

    {
#ifdef  DEBUG_OUTPUT
 //       SCI_TRACE_LOW("[MV] CM_RefreshAreaScreen nX:%d, nY:%d, nWidth:%d, nHeight:%d", nX, nY, nWidth, nHeight);
#endif
        box.left = nX;
        box.top = nY;
#if 0
        box.right = nX + nWidth - 1 ;
        box.bottom = nY + nHeight - 1 ;
#else
        box.right = nX + nWidth ;
        box.bottom = nY + nHeight ;
#endif
#ifdef  DEBUG_OUTPUT
//       MV_TRACE_LOW("[MV] CM_RefreshAreaScreen  box.left =%d;box.top = %d;box.right = %d ; box.bottom = %d", box.left,	box.top ,box.right, box.bottom);
#endif
        //GUILCD_SetBrushMode(GUI_MAIN_LCD_ID,LCD_BRUSH_MODE_ASYNC);//fresh screen asycronis
        GUILCD_InvalidateRect(
		    GUI_MAIN_LCD_ID, 
		    box,
		    GUIREF_GetUpdateBlockSet(GUI_BLOCK_MAIN));	  
    }
    /* 刷新屏幕 */
	
}

/*****************************************************************************/
//  Description:  get screen buffer
//	Global resource dependence: 
//  Author: 
//	Note:
/*****************************************************************************/
_UINT16*   CM_GetScreenBuffer(void)
{
    GUI_COLOR_T*        lcdbuff_ptr =  PNULL;   

    lcdbuff_ptr = (GUI_COLOR_T*)UILAYER_GetLayerBufferPtr((GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev());
#ifdef  DEBUG_OUTPUT     
    //MV_TRACE_LOW:"[MV]--WD[CM_GetScreenBuffer]: lcdbuff_ptr = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_OUTPUT_API_399_112_2_18_2_39_16_133,(uint8*)"d", lcdbuff_ptr);
#endif      
    return lcdbuff_ptr;	
}

/*************************************MOBILE VIDEO 2.2.0 ADD START**************************************/
_INT  CM_Font_GetSize(_HANDLE hFont)
{
    MV_FONT* pFont = (MV_FONT*)hFont;
    //SCI_TRACE_LOW:"[MV] CM_Font_GetSize pFont->nStyle:%d, pFont->nSize:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_OUTPUT_API_407_112_2_18_2_39_16_134,(uint8*)"dd", pFont->nStyle, pFont->nSize);
    return GUI_ConvertSizeToGUIFont((GUI_FONT_T)pFont->nSize);
}

_BOOLEAN CM_Font_IsBold (_HANDLE hFont)
{
    MV_FONT* pFont = (MV_FONT*)hFont;
    //SCI_TRACE_LOW:"[MV] CM_Font_GetSize pFont->nStyle:%d, pFont->nSize:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_OUTPUT_API_414_112_2_18_2_39_16_135,(uint8*)"dd", pFont->nStyle, pFont->nSize);
    return FALSE;
}

_BOOLEAN CM_Font_IsItalic(_HANDLE hFont)
{
    MV_FONT* pFont = (MV_FONT*)hFont;
    //SCI_TRACE_LOW:"[MV] CM_Font_GetSize pFont->nStyle:%d, pFont->nSize:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_OUTPUT_API_421_112_2_18_2_39_16_136,(uint8*)"dd", pFont->nStyle, pFont->nSize);
    return FALSE;
}

_BOOLEAN CM_Font_IsUnderline(_HANDLE hFont)
{
    MV_FONT* pFont = (MV_FONT*)hFont;
    //SCI_TRACE_LOW:"[MV] CM_Font_GetSize pFont->nStyle:%d, pFont->nSize:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_OUTPUT_API_428_112_2_18_2_39_16_137,(uint8*)"dd", pFont->nStyle, pFont->nSize);
    return FALSE;
}
/*************************************MOBILE VIDEO 2.2.0 ADD END****************************************/

