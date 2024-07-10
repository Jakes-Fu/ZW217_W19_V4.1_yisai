#include "os_api.h"
#include "gdidef.h"



void DrawFont(const uint8 *bmp_ptr, uint16  x, uint16 y, uint w, uint h)
{
	bmp_ptr = bmp_ptr;
	x = x; y = y;
	w = w; h = h;
}


#ifndef INLINE 
#ifdef WIN32	
#define INLINE  __inline
#else
#define INLINE 
#endif
#endif

#define uint  unsigned int


extern const uint8 *GetAscii_8x16_bmp(uint32 wch);
extern const uint8 *GetAscii_8x8_bmp(uint32 wch);
extern unsigned short ucs2_to_gb2312( unsigned short ucs2);
extern const uint8 *GetGB2312_80_Simple_16x16_bmp(uint32 wch);



static uint32 ansi_8x16_CharAt(const uint8 *str, uint index)
{
    while( (*str) && (index) )
    {
        index--;
        str++;
    }
    return (uint32)(*str);
}

static uint32 ansi_8x16_GetNextChar(const uint8 *str, uint8 **next_ptr)
{
    *next_ptr = (uint8 *)(str + 1);
    return (uint32)(*str);
}
static 
INLINE 
uint 
ansi_8x16_GetCharWidth(
const GDI_LOGFONT *font_ptr,
 uint32 wch)
{
	font_ptr = font_ptr;
	wch = wch;
    return 8;
}
static INLINE uint ansi_8x16_GetCharHeight(const GDI_LOGFONT *font_ptr, uint32 wch)
{
	font_ptr = font_ptr;
	wch = wch;
    return 16;
}
static uint ansi_8x16_GetStringWidth(const GDI_LOGFONT *font_ptr, const uint8 *str, uint len)
{
    int     w = 0;
    
	font_ptr = font_ptr;
    while( (*str++) && len-- ){
        w += 8;
    }
    return w;
}


const uint8 *ansi_8x16_GetFontBitmap(const GDI_LOGFONT *font_ptr, uint32  wch)
{
	font_ptr = font_ptr;
	return GetAscii_8x16_bmp(wch);
}
static const FONT_OPS ansi_8x16_ops =
{
    ansi_8x16_CharAt,
    ansi_8x16_GetNextChar,
    ansi_8x16_GetCharWidth,
    ansi_8x16_GetCharHeight,
    ansi_8x16_GetStringWidth,
    ansi_8x16_GetFontBitmap    
};

static uint32 ansi_8x8_CharAt(const uint8 *str, uint index)
{
    while( (*str) && (index) )
    {
        index--;
        str++;
    }
    return (uint32)(*str);
}

static uint32 ansi_8x8_GetNextChar(const uint8 *str, uint8 **next_ptr)
{
    if( NULL != next_ptr ){
        *next_ptr = (uint8 *)(str + 1);
    }
    return (uint32)(*str);
}
static INLINE uint ansi_8x8_GetCharWidth(const GDI_LOGFONT *font_ptr, uint32 wch)
{
	font_ptr = font_ptr;
	wch = wch;
    return 8;
}
static INLINE uint ansi_8x8_GetCharHeight(const GDI_LOGFONT *font_ptr, uint32 wch)
{
	font_ptr = font_ptr;
	wch = wch;
    return 8;
}
static uint ansi_8x8_GetStringWidth(const GDI_LOGFONT *font_ptr, const uint8 *str, uint len)
{
    uint     w = 0;
	
	font_ptr = font_ptr;
    
    while( (*str++) && len-- )
    {
        w += 8;
    }
    return w;
}

const uint8 *ansi_8x8_GetFontBitmap(const GDI_LOGFONT *font_ptr, uint32  wch)
{	
	font_ptr = font_ptr;
	return GetAscii_8x8_bmp( wch );
}
static const FONT_OPS ansi_8x8_ops =
{
    ansi_8x8_CharAt,
    ansi_8x8_GetNextChar,
    ansi_8x8_GetCharWidth,
    ansi_8x8_GetCharHeight,
    ansi_8x8_GetStringWidth,
    ansi_8x8_GetFontBitmap    
};

/*
    GB2312_80 Text Info. ( '\0' Terminate )
*/
static uint32 gb2312_80_CharAt(const uint8 * str, uint index)
{
    uint32      wch;
    
    while( (*str) && (index--) )
    {
        if( (*str++) & 0x80 ) // first byte of GB2312 Code
		{
            SCI_ASSERT( (*str) & 0x80 );/*assert verified */
			str++;
        }
    }
    wch = (uint32)(*str++);
    if( wch & 0x80 ){
        wch += ((uint32)(*str)) << 8; // get high byte
    }
    return wch;
}
static uint32 gb2312_80_GetNextChar(const uint8 *str, uint8 **next_ptr)
{
    uint32  wch;
    
    wch = (uint32)(*str++);
    if( wch & 0x80 ){
        SCI_ASSERT( (*str) & 0x80 );/*assert verified */
        wch += ((uint32)(*str++) << 8); // get high byte
    }
    
    if( NULL != next_ptr ){
         *next_ptr = (uint8 *)str; 
    }
    return wch;
}

INLINE uint  gb2312_80_GetCharWidth(const GDI_LOGFONT *font_ptr, uint32  wch)
{
	font_ptr = font_ptr;
    /* Only support one type of gb2312_80, so return immidiately.  */
    if( wch ){
        return (uint)((wch & 0x80) ? 16 : 8);
    }
    return 0;    
}

INLINE uint gb2312_80_GetCharHeight(const GDI_LOGFONT *font_ptr, uint32 wch )
{
	font_ptr = font_ptr;
    return (uint)( wch ? 16 : 0);
}
static uint gb2312_80_GetStringWidth(const GDI_LOGFONT *font_ptr, const uint8 *str, uint len)
{
    uint    w = 0;
 
	font_ptr = font_ptr;
    while( (*str) && (len--) )
    {
        if( (*str++) & 0x80 ){
            w += 16;
            SCI_ASSERT( (*str) & 0x80 );/*assert verified */
            str++; // skip high byte
        }
        else{
            w += 8;
        }
    } /* End of While */
    return w;    
}



static const uint8 *gb2312_80_GetFontBitmap(const GDI_LOGFONT *font_ptr, uint32 wch)
{
	if( wch < 0x80 ){
		/* Can map to ASCII char */
		return ansi_8x16_GetFontBitmap(font_ptr, wch);
	}
	else{    
		return GetGB2312_80_Simple_16x16_bmp( wch );
	}
}

static const FONT_OPS gb2312_80_ops =
{
    gb2312_80_CharAt,
    gb2312_80_GetNextChar,
    gb2312_80_GetCharWidth,
    gb2312_80_GetCharHeight,
    gb2312_80_GetStringWidth,
    gb2312_80_GetFontBitmap
};

/*
    All char are two-bytes.
    0x0000 Terminate
*/
static uint32 ucs2_CharAt(const uint8 *str, uint index)
{
    uint32  wch;
#if 1
    const uint16  *ucs2_ptr = (const uint16 *)str;
    
    SCI_ASSERT( 0 == (((uint32)str) & 0x1) ); /*assert verified : check msg_ptr*/// must be word boundary.
	while( (index--) && (*ucs2_ptr) )
	{
		ucs2_ptr++;
	}
	wch = (uint32)(*ucs2_ptr);
#else
    while( index && ( (*str) || (*(str+1)) ) )
    {
        index--;
        str += 2;
    } /* End of while */
    wch = (index) ? ((uint32)*str) + ( ((uint32)(*(str+1))) << 8) : 0;    
#endif
	return wch;
}
static uint32 ucs2_GetNextChar(const uint8 *str, uint8 **next_ptr)
{
    uint32      wch = 0;    
#if 1
	const uint16     *ucs2_ptr = (const uint16 *)str;

	SCI_ASSERT( 0 == (((uint32)str) & 0x1) );/*assert verified : check msg_ptr*/ // must be word boundary.
	if( *ucs2_ptr ){
		wch  = (uint32)(*ucs2_ptr);
		str += 2;
	}
#else
    if( (*str) || (*(str+1)) ){ // Not tail
        wch = ((uint32)*str) + (uint32) (*(str+1)) ;
        str += 2;
    }
#endif
    if( NULL != next_ptr ) {
         *next_ptr = (uint8 *)str; 
    }
    
    return wch;    
}

static INLINE uint ucs2_GetCharWidth(const GDI_LOGFONT *font_ptr, uint32 wch)
{
    uint     w = 0;
    
	font_ptr = font_ptr;
    if( wch ){
        if( wch < 0x80 ){ // Can map to ANSI
            w = 8;
        }
        else if( (0x4E00 <= wch) && (0x9FFF >= wch) ){ 
            /* CJK Unified Ideographs  */
            w = 16;
        }        
    }
    return w;
}
static INLINE uint ucs2_GetCharHeight(const GDI_LOGFONT *font_ptr, uint32 wch)
{
    uint     h = 0;
    
	font_ptr = font_ptr;
    if( wch ){
        if( wch < 0x80 ){ // Can map to ANSI
            h = 16;
        }
        else if( (0x4E00 <= wch) && (0x9FFF >= wch) ){
            /* CJK Unified Ideographs  */
            h = 16;
        }        
    }
    return h;
}

static uint ucs2_GetStringWidth(const GDI_LOGFONT *font_ptr, const uint8 *str, uint len)
{
    uint     w = 0;
#if 1
	const uint16     *ucs2_ptr = (const uint16 *)str;

	SCI_ASSERT( 0 == (((uint32)str) & 0x1) ); /*assert verified : check msg_ptr*/// must be word boundary.
	while( (len--) && (*ucs2_ptr) ){
		if( (0x4E00 <= *ucs2_ptr) && (0x9FFF >= *ucs2_ptr) ){
			/* CJK Unified Ideographs */
			w += 16;
		}
		else if( 0x100 > *ucs2_ptr ){
			/* ANSI Symbol */
			w += 8;
		}
		else { /* Not support  */ }

		ucs2_ptr++;
	}
#else
    while( len-- && ( (*str) || (*(str+1)) )   ){
        if( (0x4E <= *(str+1)) && (*(str+1) <= 0x9F) ) {
            /* CJK Unified Ideographs  */
            w += 16;
        }
        else if( 0 == *(str+1) ){
            /* ANSI Symbol */
            w += 8;
        }
        else{ /* Not Support */ }
        
        str += 2;
    }
#endif
	font_ptr = font_ptr; // avoid compile warning
    return w;
}


const uint8 *ucs2_GetFontBitmap(const GDI_LOGFONT * font_ptr, uint32 wch)
{
	// Convert ucs2 to GB2312_80
	if( wch >= 0x100 )
		wch = (uint32)ucs2_to_gb2312( (unsigned short)wch );
	return gb2312_80_GetFontBitmap( font_ptr, wch );
}

static const FONT_OPS ucs2_ops =
{
    ucs2_CharAt,
    ucs2_GetNextChar,
    ucs2_GetCharWidth,
    ucs2_GetCharHeight,
    ucs2_GetStringWidth,
    ucs2_GetFontBitmap    
};




static const GDI_LOGFONT  FontTable_ansi[]=
{
    {
        0, 
        8, 16,
        CHARSET_ANSI,
        "ANSI(8x16)",
        (struct FONT_OPS_tag *)&ansi_8x16_ops,
    },
    {
        0,
        8, 8,
        CHARSET_ANSI,
        "ANSI(8x8)",
        (struct FONT_OPS_tag *)&ansi_8x8_ops,
    }
};
static const GDI_LOGFONT  FontTable_gb2312_80[]=
{
    {
        0,						// style
        16, 16,					// width X height
        CHARSET_GB2312_80,      // charset
        "Song(16X16)",			// face name
        (struct FONT_OPS_tag *)&gb2312_80_ops  // ops
    }
};
static const GDI_LOGFONT FontTable_ucs2[]=
{
    {
        0,
        16, 16,         // 0x0000--0x007F is same
        CHARSET_UCS2,
        "UCS2(16x16)",
        (struct FONT_OPS_tag *)&ucs2_ops
    }
};
static const GDI_LOGFONT DefaultFont = 
{
    0,						// style
    16, 16,					//width X height
    CHARSET_GB2312_80,      // charset
    "Song(16x16)",			// face name
    (struct FONT_OPS_tag *)&gb2312_80_ops  // ops
};

GDI_LOGFONT *GDI_GetDefaultFont(void)
{
    return (GDI_LOGFONT *)&DefaultFont;
}

/*****************************************************************************/
//  Description:    
//       Delete the font that created previous.
//	Global resource dependence: 
//  Author: Lin.liu
//	Note:
/*****************************************************************************/
BOOLEAN GDI_DeleteFont(GDI_LOGFONT *font_ptr)
{    
    /* 
        For All Font are const data, nothing need to do.
    */
	font_ptr = font_ptr;
    return TRUE;
}

/*****************************************************************************/
//  Description:    
//      creates a logical font with the specified characteristics.
//      For all height/width comparisons, the font mapper looks for the
//      largest font that does not exceed the requested size. 
//	Global resource dependence: 
//  Author: Lin.liu
//	Note:
/*****************************************************************************/
GDI_LOGFONT *GDI_CreateFont(
    uint16		nHeight,        // height of font
    uint16		nWidth,         // average character width
    BOOLEAN		bItalic,        // Specifies an italic font if set to TRUE. 
    BOOLEAN		bUnderLine,     // Specifies a UnderLine font if set to TRUE. 
    BOOLEAN		bBold,          // Specifies a Bold font if set to TRUE. 
    uint16		nCharset,       // Specifies the character set
    const char *faceName)       // Type face name.
{
	faceName = faceName;
    switch( nCharset )
    {
    case CHARSET_DEFAULT:
        return (GDI_LOGFONT *)&DefaultFont;
    //break;
    case CHARSET_ANSI:
        return (GDI_LOGFONT *)( (12 < nHeight) ? &FontTable_ansi[1] : &FontTable_ansi[0]);            
    //break;    
    case CHARSET_GB2312_80:
        return (GDI_LOGFONT *)&FontTable_gb2312_80[0];
    //break;
    case CHARSET_UCS2:
        return (GDI_LOGFONT *)&FontTable_ucs2[0];
    //break;
    default:
        return NULL;
    //break;
    }
} /* End CreateFont */

