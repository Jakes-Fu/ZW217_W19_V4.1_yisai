#ifndef _GLOBAL_H__
#define _GLOBAL_H__

#include <afxtempl.h>
#include "spml_def.h"

#define MAX_FONT_SIDE 42

/* message define */
#define WM_PROGRESS         (WM_USER+1000)     
// 过程的状态                                           
const int PROG_BEGIN   = 0;
const int PROG_PROCEED = 1;
const int PROG_END     = 2;
const int PROG_TITLE   = 3;

#define FT_SUCCESS   (0)

/* Code Table Struct Define */


#define SAFE_FREE_ARRAY(p)	\
do \
{\
    if (p != NULL) \
       delete []p; \
	p=NULL;\
} while(0)


//#define _TEST
#define VECTOR_POINT_SIZE		32
//#define VECTOR_SHK_POINT_SIZE	32
#define VECTOR_DISP_POINT_SIZE  16

#define VECTOR_NONE_ZOOM        (0)
#define VECTOR_BASE_ZOOM        (1.0)
#define VECTOR_MID_ZOOM         (0.875)
#define VECTOR_SMALL_ZOOM       (0.5)

typedef struct _DMF_HEADER_T
{
	BYTE	bFontType;     	//字库类型
	BYTE	bFontWidth;		//字库字符的宽度	
	BYTE	bFontHeight;	//字库字符的高度
	BYTE	bCodeSize;		//字符占存储空间
	DWORD	dwCodeStart;	//字库中第一个字符的编码
	DWORD	dwCodeEnd;		//字库中最后一个字符的编码
	BYTE	bReserved1;		//保留字段
	BYTE	bReserved2;		//保留字段
    _DMF_HEADER_T()
	{
		memset(this,0,sizeof(_DMF_HEADER_T));
	}
	
}DMF_HEADER_T,*PDMF_HEADER_T;



typedef struct _DMF_HEADER_NEW_T
{
	    BYTE flag[4];  //"SCIF"
        BYTE ver[4];   //"v1.0"
        DWORD offset_to_sector;  //point to unicode sector 
				             //you can define it as
                             //SCI_UNICODE_SECTOR_PTR sector_ptr;  
        WORD sector_number;
		BYTE height;
        BYTE width;
        BYTE asent;
        BYTE desasent;
        BYTE equal_width_flag;        
        BYTE reserved[5];  

    _DMF_HEADER_NEW_T()
	{
		memset(this,0,sizeof(_DMF_HEADER_NEW_T));
		flag[0]=(BYTE)'S';
		flag[1]=(BYTE)'C';
		flag[2]=(BYTE)'I';
		flag[3]=(BYTE)'F';
		ver[0] =(BYTE)'v';
		ver[1] =(BYTE)'1';
		ver[2] =(BYTE)'.';
		ver[3] =(BYTE)'0';


	}
	
}DMF_HEADER_NEW_T,*PDMF_HEADER_NEW_T;

typedef struct _UC_SECTOR_T
{
	WORD wUStart;
	WORD wUEnd;
	DWORD dwOffset;
	_UC_SECTOR_T()
	{
		memset(this,0,sizeof(_UC_SECTOR_T));
	}
}UC_SECTOR_T,*UC_SECTOR_PTR;


typedef struct _BDF_CHAR_T
{
	DWORD  dwUCCode;	// Unicode code
	DWORD  dwLCCode;	// local code
	DWORD  dwCharID;
	DWORD  dwBBw;		// glyph width	
	DWORD  dwBBh;		// glyph height
	DWORD  dwCustWidth; // customer width
	int    nBBxoff0x;	// glyph x offset opposite to 
	int    nBByoff0y;	// glyph y offset
	BYTE*  bBitmap;
	BYTE*  bDotMatrix;     

    _BDF_CHAR_T()
	{
		memset(this,0,sizeof(_BDF_CHAR_T));
	}
	BOOL operator <(_BDF_CHAR_T item)
	{
		if(dwUCCode < item.dwUCCode)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}BDF_CHAR_T,*PBDF_CHAR_T;


typedef CMap<int,int,int,int> MAP_INT; 

typedef struct  X_SUBGLYPH_T
{
	WORD		index;
	WORD		flags;
	short		arg1;
	short		arg2;
	SPML_MATRIX_T transform;
	X_SUBGLYPH_T()
	{
		memset(this,0,sizeof(X_SUBGLYPH_T));
	}
} X_SUBGLYPH,* X_SUBGLYPH_PTR;


typedef struct _UC_BOUND_T
{
	DWORD dwUCStart;
	DWORD dwUCEnd;
}UC_BOUND_T,* UC_BOUND_PTR;

typedef struct _FONT_PROPERTY_T
{
	int  nFontSize;
	int  nShrinkSize;
	int  nFontType;
}FONT_PROPERTY_T,*FONT_PROPERTY_PTR;

typedef CMap<int,int,FONT_PROPERTY_PTR,FONT_PROPERTY_PTR> MAP_FTPROP;

typedef struct _CHAR_SET_INFO_T
{
	_TCHAR		 szCharSetName[_MAX_PATH];
	BYTE         tag[4];
	int			 nCharCount;
	int			 nWidthEqualFlag; // not used
	DWORD		 dwLeftSpaceBits;
	DWORD		 dwRightSpaceBits;
	int          nSectorNum;
	UC_BOUND_PTR pUCBoundTable; 
	int          nFtTblNum;
	int         *pFtTblSize;

	int          nExSectorNum;
	UC_BOUND_PTR pExUCBoundTable; 
	int          nExFtTblNum;
	int         *pExFtTblSize;
	
	MAP_FTPROP  *pmapFTProp;

	BOOL         bVector;
	int          nVectorShrinkPercent;
	int          nVectorMaxBearingYDelta;
	double       dZoomRate;
	BOOL         bVecCmp;
	BOOL         bVecCmpMore;

	BOOL         bSupportSubGlyph;

	_CHAR_SET_INFO_T()
	{
		memset(this,0,sizeof(_CHAR_SET_INFO_T));
		nWidthEqualFlag = TRUE;
		dZoomRate = VECTOR_BASE_ZOOM;
		bVecCmpMore = TRUE;
		bVecCmp = FALSE;
	}
}CHAR_SET_INFO,*CHAR_SET_INFO_PTR;

typedef CMap<CString,LPCTSTR,CHAR_SET_INFO_PTR,CHAR_SET_INFO_PTR> MAP_CHAR_SET_INFO;

typedef struct _BDF_HEADER_T
{
	_TCHAR szFontName[_MAX_PATH];
	DWORD  dwUCBegin;
	DWORD  dwUCEnd;
	DWORD  dwCharCount;
	DWORD  dwPointSize;
	DWORD  dwXres;
	DWORD  dwYres;
	DWORD  dwFBBX;
	DWORD  dwFBBY;
	DWORD  dwFBBXns; //initial value
	DWORD  dwFBBYns; //initial value
	int    nXoff;
	int    nYoff;
	int    nXoffns; //initial value
	int    nYoffns; //initial value

    _BDF_HEADER_T()
	{
		memset(this,0,sizeof(_BDF_HEADER_T));
	}
}BDF_HEADER_T,*PBDF_HEADER_T;


typedef struct _GLYPH_TABLE_HDR_T
{
	BYTE	size;
	BYTE	style;
	BYTE	ppem_x;
	BYTE	ppem_y;
	BYTE	ascent;
	BYTE	descent;
	BYTE	max_width;
	BYTE	default_width;
	char	typo_ascender;
	char	typo_descender;
	char	typo_linegap;
	char	baseline;
	char	min_x;
	char	max_x;
	char	min_y;
	char	max_y;
	BYTE    advance;
	char    bearing_x;
	BYTE    bitmap_w;
	char    bearing_y;
	BYTE    bitmap_h;
	char    max_bearing_y;
	BYTE    max_height;
	BYTE	type;
	BYTE    shrink;
	BYTE    reserved;
	WORD	glyph_count;	
	DWORD	page_offsets_base;
}GLYPH_TABLE_HDR_T,*GLYPH_TABLE_HDR_PTR;

typedef SPML_VECTOR_T XFT_VECTOR;

typedef SPML_OUTLINE_SPEC_T  XFT_OUTLINE;
typedef SPML_OUTLINE_SPEC_T* XFT_OUTLINE_PTR;

typedef struct _XGLYPH_METRICS_T
{
	BYTE   dwidth;
	char   x;
	BYTE   width;
	char   y;
	BYTE   height;	
	DWORD  advance_org;
}XGLYPH_METRICS_T;

typedef struct _GLYPH_BODY_T
{
	WORD   flag;
	WORD   dwidth;
	short   x;
	WORD   width;
	short   y;
	WORD   height;	
	BYTE   *bitmap;
	DWORD  initialized;
	XFT_OUTLINE outline;
	XGLYPH_METRICS_T metrics;
	USHORT    nNumberSubGlyph;
	X_SUBGLYPH_PTR pSubGlyf;

	void Init()
	{
		memset(this,0,sizeof(_GLYPH_BODY_T));
	}
	void Clear()
	{		
		SAFE_FREE_ARRAY(bitmap);
		SAFE_FREE_ARRAY(outline.outline.points);
		SAFE_FREE_ARRAY(outline.outline.tags);
		SAFE_FREE_ARRAY(outline.outline.contours);
		SAFE_FREE_ARRAY(pSubGlyf);

		Init();
	}

}GLYPH_BODY_T,*GLYPH_BODY_PTR;

typedef struct _GLYPH_PAGE_T
{
	WORD   end;
	WORD   start;
	WORD   offset;
}GLYPH_PAGE_T,*GLYPH_PAGE_PTR;

typedef struct _GLYPH_TABLE_T
{
	GLYPH_TABLE_HDR_T hdr;
	WORD              page_count;
	GLYPH_PAGE_PTR    page_ptr;
	DWORD*            offset_ptr;	
	GLYPH_BODY_PTR	  glyphs_ptr;
	UINT             *pIDs;

	void Init()
	{
		memset(this,0,sizeof(_GLYPH_TABLE_T));
	}
	void Clear()
	{
		int i = 0;	

		SAFE_FREE_ARRAY(page_ptr);		
		SAFE_FREE_ARRAY(offset_ptr);
		SAFE_FREE_ARRAY(pIDs);
		if(NULL != glyphs_ptr)
		{
			for(i=0; i<hdr.glyph_count;i++)
			{
				SAFE_FREE_ARRAY(glyphs_ptr[i].bitmap);
			}
			SAFE_FREE_ARRAY(glyphs_ptr);
		}


		Init();
	}

}GLYPH_TABLE_T,*GLYPH_TABLE_PTR;

typedef struct _FONT_HDR_T
{
	BYTE    magic[4];
	BYTE    fver[4];
	BYTE	tag[4];
	BYTE	version[2];
	WORD	glyph_table_count;
	WORD	flags;
	WORD	units_per_em;
	DWORD	cmap_offset;
	DWORD	gdef_offset;
	DWORD	gsub_offset;
	DWORD	gpos_offset;
	DWORD	kern_offset;
	DWORD *	gtbl_offset_ptr;

	void Init()
	{
		memset(this,0,sizeof(_FONT_HDR_T));
	}
	void Clear()
	{
		SAFE_FREE_ARRAY(gtbl_offset_ptr);
		Init();
	}

}FONT_HDR_T,*FONT_HDR_PTR;

typedef struct _CMAP_DATA_T
{
	BYTE  version[2];
	WORD  table_count;   //only one table
	WORD  platform_id;   //only support 3
	WORD  encoding_id;   //only support 1
	DWORD table_offset;
	WORD  format;
	WORD  length;
	WORD  language;
	WORD  seg_countX2;
	WORD  search_range;
	WORD  entry_selector;
	WORD  range_shift;
	WORD  padd;

	WORD * end_count_ptr;
	WORD * start_count_ptr;
	short* id_delta_ptr;
	WORD * id_range_offset_ptr;

	DWORD  glyf_id_count;
	WORD * glyf_id_ptr;

	void Init()
	{
		memset(this,0,sizeof(_CMAP_DATA_T));
		platform_id = 3;
		encoding_id = 1;
	}
	void Clear()
	{
		SAFE_FREE_ARRAY(end_count_ptr);
		SAFE_FREE_ARRAY(start_count_ptr);
		SAFE_FREE_ARRAY(id_delta_ptr);
		SAFE_FREE_ARRAY(id_range_offset_ptr);	
		SAFE_FREE_ARRAY(glyf_id_ptr);	
		Init();
	}

}CMAP_DATA_T,* CMAP_DATA_PTR;

typedef struct _GDEF_DATA_T
{
	DWORD  offset;
	DWORD  size;
	BYTE * data_ptr;
	void Init()
	{
		memset(this,0,sizeof(_GDEF_DATA_T));
	}
	void Clear()
	{
		SAFE_FREE_ARRAY(data_ptr);	
		Init();
	}

}GDEF_DATA_T;

typedef struct _GSUB_DATA_T
{
	DWORD  offset;
	DWORD  size;
	BYTE * data_ptr;

	void Init()
	{
		memset(this,0,sizeof(_GSUB_DATA_T));
	}
	void Clear()
	{
		SAFE_FREE_ARRAY(data_ptr);	
		Init();
	}

}GSUB_DATA_T;

typedef struct _GPOS_DATA_T
{
	DWORD  offset;
	DWORD  size;
	BYTE * data_ptr;

	void Init()
	{
		memset(this,0,sizeof(_GPOS_DATA_T));
	}
	void Clear()
	{
		SAFE_FREE_ARRAY(data_ptr);	
		Init();
	}

}GPOS_DATA_T;

typedef struct _GLYPH_METRICS_ORG_T
{
	DWORD dwWidth;
	DWORD dwHeight;

	DWORD dwHoriBearingX;
    DWORD dwHoriBearingY;
    DWORD dwHoriAdvance;

    DWORD dwVertBearingX;
    DWORD dwVertBearingY;
    DWORD dwVertAdvance;
    
}GLYPH_METRICS_ORG_T,*GLYPH_METRICS_ORG_PTR;


#define OTF_TAG(a, b, c, d) ((DWORD)(((a)<<24)|((b)<<16)|((c)<<8)|(d)))

#define FONT_TYPE_MONO				(0)
#define FONT_TYPE_GRAY				(1)
#define FONT_TYPE_GRAY_RLE			(2)
#define FONT_TYPE_VECTOR			(3)
#define FONT_TYPE_VECTOR_CMP		(4)
//#define FONT_TYPE_VECTOR_CMP_EX		(5)     // Compress outlines

#define OF_POINTS_NUM_BYTE		(1)			// set is BYTE, not set is WORD
#define OF_CONTOURS_NUM_BYTE	(1<<1)		// set is BYTE, not set is WORD
#define OF_CONTOURS_BYTE		(1<<2)		// set is BYTE, not set is WORD
#define OF_CMP_X				(1<<3)		// set is to compress X coordinate and add X flags
#define OF_CMP_Y				(1<<4)		// set is to compress Y coordinate and add Y flags

#define FLAG_COMPOSE_GLYPH	(1)
#define FLAG_CMP_EX	        (1 << 1)
#define FLAG_SIZE    		(1 << 2)
#define FLAG_ADVANCE		(1 << 3)
#define FLAG_BEARING_X		(1 << 4)
#define FLAG_BEARING_Y		(1 << 5)
#define FLAG_BITMAP_WIDTH	(1 << 6)
#define FLAG_BITMAP_HEIGHT	(1 << 7)
#define FLAG_GLYF_ALL       (FLAG_ADVANCE|FLAG_BEARING_X|FLAG_BEARING_Y|FLAG_BITMAP_WIDTH|FLAG_BITMAP_HEIGHT)

#define SUBGLYPH_ARGS_ARE_WORDS             0x0001
#define SUBGLYPH_ARGS_ARE_XY_VALUES         0x0002
#define SUBGLYPH_ROUND_XY_TO_GRID           0x0004
#define SUBGLYPH_WE_HAVE_A_SCALE            0x0008
/* reserved                        0x0010 */
//#define SUBGLYPH_MORE_COMPONENTS            0x0020
#define SUBGLYPH_WE_HAVE_AN_XY_SCALE        0x0040
#define SUBGLYPH_WE_HAVE_A_2X2              0x0080
// #define SUBGLYPH_WE_HAVE_INSTR              0x0100
// #define SUBGLYPH_USE_MY_METRICS             0x0200
// #define SUBGLYPH_OVERLAP_COMPOUND           0x0400
// #define SUBGLYPH_SCALED_COMPONENT_OFFSET    0x0800
// #define SUBGLYPH_UNSCALED_COMPONENT_OFFSET  0x1000

// #define FT_SUBGLYPH_FLAG_ARGS_ARE_WORDS          1
// #define FT_SUBGLYPH_FLAG_ARGS_ARE_XY_VALUES      2
// #define FT_SUBGLYPH_FLAG_ROUND_XY_TO_GRID        4
// #define FT_SUBGLYPH_FLAG_SCALE                   8
// #define FT_SUBGLYPH_FLAG_XY_SCALE             0x40
// #define FT_SUBGLYPH_FLAG_2X2                  0x80
// #define FT_SUBGLYPH_FLAG_USE_MY_METRICS      0x200

typedef CMap<DWORD,DWORD,GLYPH_BODY_PTR,GLYPH_BODY_PTR> MAP_GLYPH;
typedef CMap<DWORD,DWORD,DWORD,DWORD> MAP_UN_ID;
typedef CUIntArray* UnicodeArray_ptr;
typedef CMap<DWORD,DWORD,UnicodeArray_ptr,UnicodeArray_ptr> MAP_ID_UN;
typedef CMap<UINT,UINT,UINT,UINT> MAP_NUM; 


#endif // _GLOBAL_H__s