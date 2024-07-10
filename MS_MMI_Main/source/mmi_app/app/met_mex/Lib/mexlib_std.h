/************************************************************************
* 版权所有 (C)2010,扬讯科技。
* 
* 文件名称： // MexLib_Std.h
* 文件标识：
* 内容摘要： // mex平台std库函数头文件
*
************************************************************************/

#if defined(MEX_TARGET_SERVER) && !defined(__MEXLIB_STD_SERVER_H__) || defined(MEX_TARGET_CLIENT) && !defined(__MEXLIB_STD_CLIENT_H__)

#if defined(MEX_TARGET_SERVER)
	#define __MEXLIB_STD_SERVER_H__
#elif defined(MEX_TARGET_CLIENT)
	#define __MEXLIB_STD_CLIENT_H__
#endif

#if defined(assert)
#undef assert
#endif

#if defined(MEX_USE_ASSERT)
	#if defined(MEX_TARGET_SERVER)
		#define assert(exp) (void)( (exp) || (mex_assertmsg(#exp, __FILE__, __LINE__), 0) )
	#elif defined(MEX_TARGET_CLIENT)
		#define assert(exp) (void)( (exp) || (assertmsg(#exp, __FILE__, __LINE__), 0) )
	#endif
#else
	#define assert(exp)	
#endif

//common macro definitions for STD
#define MEX_H_LAUNCH_FUNC_STD					MEX_H_LAUNCH_FUNC( Std )
#define MEX_C_LIB_FUNC_NAME_STD					MEX_C_LIB_FUNC_NAME( Std )

#define MEX_H_GET_FUNC_DEFINE_STD(x)			MEX_H_GET_FUNC_DEFINE( Std, x )

#define MEX_H_ENUM_FUNC_START_STD				MEX_H_ENUM_FUNC_START( Std )
#define MEX_H_ENUM_FUNC_STD(x)					MEX_H_ENUM_FUNC( Std, x )
#define MEX_H_ENUM_FUNC_END_STD					MEX_H_ENUM_FUNC_END( Std )

#define MEX_H_NB_FUNC_STD						MEX_H_NB_FUNC( Std )
//end common macro definitions

/* do not change the order */
typedef enum _E_MEX_CHSET_PAIR
{
	E_MCP_BASE_ENCODING_SCHEME,

	E_MCP_ARABIC_ISO_TO_UCS2,
	E_MCP_UCS2_TO_ARABIC_ISO,

	E_MCP_ARABIC_WINDOWS_TO_UCS2,
	E_MCP_UCS2_TO_ARABIC_WINDOWS,
	/* PMT START PERSIAN */
	E_MCP_PERSIAN_CP1097_TO_UCS2,
	E_MCP_UCS2_TO_PERSIAN_CP1097,

	E_MCP_PERSIAN_CP1098_TO_UCS2,
	E_MCP_UCS2_TO_PERSIAN_CP1098,
	/* PMT END PERSIAN */
	E_MCP_BALTIC_ISO_TO_UCS2,
	E_MCP_UCS2_TO_BALTIC_ISO,

	E_MCP_BALTIC_WINDOWS_TO_UCS2,
	E_MCP_UCS2_TO_BALTIC_WINDOWS,

	E_MCP_CENTRAL_EUROPEAN_ISO_TO_UCS2,
	E_MCP_UCS2_TO_CENTRAL_EUROPEAN_ISO,

	E_MCP_CENTRAL_EUROPEAN_WINDOWS_TO_UCS2,
	E_MCP_UCS2_TO_CENTRAL_EUROPEAN_WINDOWS,

	E_MCP_GREEK_ISO_TO_UCS2,
	E_MCP_UCS2_TO_GREEK_ISO,

	E_MCP_GREEK_WINDOWS_TO_UCS2,
	E_MCP_UCS2_TO_GREEK_WINDOWS,

	E_MCP_HEBREW_ISO_TO_UCS2,
	E_MCP_UCS2_TO_HEBREW_ISO,

	E_MCP_HEBREW_WINDOWS_TO_UCS2,
	E_MCP_UCS2_TO_HEBREW_WINDOWS,

	E_MCP_LATIN_ISO_TO_UCS2,
	E_MCP_UCS2_TO_LATIN_ISO,

	E_MCP_NORDIC_ISO_TO_UCS2,
	E_MCP_UCS2_TO_NORDIC_ISO,

	E_MCP_SOUTH_EUROPEAN_ISO_TO_UCS2,
	E_MCP_UCS2_TO_SOUTH_EUROPEAN_ISO,

	E_MCP_TURKISH_ISO_TO_UCS2,
	E_MCP_UCS2_TO_TURKISH_ISO,

	E_MCP_TURKISH_WINDOWS_TO_UCS2,
	E_MCP_UCS2_TO_TURKISH_WINDOWS,

	E_MCP_WESTERN_ISO_TO_UCS2,
	E_MCP_UCS2_TO_WESTERN_ISO,

	E_MCP_WESTERN_WINDOWS_TO_UCS2,
	E_MCP_UCS2_TO_WESTERN_WINDOWS,

	/* Add new (8-bit) encodings above this line */
	E_MCP_MMI_CHSET_8_BITS_END_MARKER1,
	E_MCP_MMI_CHSET_8_BITS_END_MARKER2,

	E_MCP_BIG5_TO_UCS2,
	E_MCP_UCS2_TO_BIG5,

	E_MCP_GB2312_TO_UCS2,
	E_MCP_UCS2_TO_GB2312,

	E_MCP_EUCKR_WINDOWS_TO_UCS2,
	E_MCP_UCS2_TO_EUCKR_WINDOWS,

	E_MCP_THAI_WINDOWS_TO_UCS2,
	E_MCP_UCS2_TO_THAI_WINDOWS,

	E_MCP_VIETNAMESE_WINDOWS_TO_UCS2,
	E_MCP_UCS2_TO_VIETNAMESE_WINDOWS,

	E_MCP_KOI8_R_TO_UCS2,
	E_MCP_UCS2_TO_KOI8_R,

	/* PLace all encodings using algo to above this one */

	E_MCP_UTF8_TO_UCS2,
	E_MCP_UCS2_TO_UTF8,
	E_MCP_CHSET_PAIR_TOTAL
}E_MEX_CHSET_PAIR;

//debug and memory functions
MEX_H_DECLARE_FUNC( void,		assertmsg,		(const char *pExp, const char *pFile, uint32 iLine) )
#if defined(MEX_MEMORY_GUARD)
MEX_H_DECLARE_FUNC( void*,		mallocex,		(size_t size, const char *szFile, int32 iLine) )
MEX_H_DECLARE_FUNC( void,		freeex,			(void *ptr, const char *szFile, int32 iLine) )
#else
MEX_H_DECLARE_FUNC( void*,		mallocex,		(size_t size) )
MEX_H_DECLARE_FUNC( void,		freeex,			(void *ptr) )
#endif
MEX_H_DECLARE_FUNC( int32,		memcmp,			(const void *pData0, const void *pData1, size_t iSize) )
MEX_H_DECLARE_FUNC( void*,		memchr,			(const void *pData, int32 iChr, size_t iSize) )
MEX_H_DECLARE_FUNC( void*,		memcpy,			(void *pDst, const void *pSrc, size_t iSize) )
MEX_H_DECLARE_FUNC( void*,		memset,			(void *pDst, int32 iData, size_t iSize) )
MEX_H_DECLARE_FUNC( void*,		memset16,		(void *pDst, int32 iData, size_t iCount) )
MEX_H_DECLARE_FUNC( void*,		memset32,		(void *pDst, int32 iData, size_t iCount) )
MEX_H_DECLARE_FUNC( void*,		memmove,		(void *pDst, const void *pSrc, size_t iSize) )

//string functions
MEX_H_DECLARE_FUNC( size_t,		strlen,			( const char *str ) )
MEX_H_DECLARE_FUNC( int32,		strcmp,			( const char *str0, const char *str1 ) )
MEX_H_DECLARE_FUNC( int32,		stricmp,		( const char *str0, const char *str1 ) )
MEX_H_DECLARE_FUNC( char*,		strcat,			( char *strDest, const char *strSource ) )
MEX_H_DECLARE_FUNC( char*,		strcpy,			( char *strDest, const char *strSource ) )
MEX_H_DECLARE_FUNC( char*,		strchr,			( const char *str, int32 chr ) )
MEX_H_DECLARE_FUNC( char*,		strstr,			( const char *str, const char *strsearch ) )

MEX_H_DECLARE_FUNC( char*,		strncpy,		( char *strDst, const char *strSrc, size_t iSize ) )
MEX_H_DECLARE_FUNC( char*,		strncat,		( char *strDst, const char *strSrc, size_t iSize ) )
MEX_H_DECLARE_FUNC( int32,		strncmp,		( const char *str0, const char *str1, size_t iSize ) )
MEX_H_DECLARE_FUNC( int32,		atoi,			( const char *str ) )

MEX_H_DECLARE_FUNC( char*,		strlwr,			( char *strDst ) )
MEX_H_DECLARE_FUNC( char*,		strupr,			( char *strDst ) )

MEX_H_DECLARE_FUNC( size_t,		wcslen,			( const wchar_t *str ) )
MEX_H_DECLARE_FUNC( int32,		wcscmp,			( const wchar_t *str0, const wchar_t *str1 ) )
MEX_H_DECLARE_FUNC( int32,		wcsicmp, 		( const wchar_t *str0, const wchar_t *str1 ) )
MEX_H_DECLARE_FUNC( wchar_t*,	wcscat,			( wchar_t *strDest, const wchar_t *strSource ) )
MEX_H_DECLARE_FUNC( wchar_t*,	wcscpy,			( wchar_t *strDest, const wchar_t *strSource ) )
MEX_H_DECLARE_FUNC( wchar_t*,	wcschr,			( const wchar_t *str, wchar_t chr ) )
MEX_H_DECLARE_FUNC( wchar_t*,	wcsstr,			( const wchar_t *str, const wchar_t *strsearch ) )

MEX_H_DECLARE_FUNC( wchar_t*,	wcsncpy,		( wchar_t *strDst, const wchar_t *strSrc, size_t iSize ) )
MEX_H_DECLARE_FUNC( wchar_t*,	wcsncat,		( wchar_t *strDst, const wchar_t *strSrc, size_t iSize ) )
MEX_H_DECLARE_FUNC( int32,		wcsncmp,		( const wchar_t *str0, const wchar_t *str1, size_t iSize ) )

MEX_H_DECLARE_FUNC( wchar_t*,	wcslwr, 		( wchar_t *strDst ) )
MEX_H_DECLARE_FUNC( wchar_t*,	wcsupr, 		( wchar_t *strDst ) )


MEX_H_DECLARE_FUNC( wchar_t*,	str2nwcs,		( wchar_t *wcsDest, uint16 uDestSize, const char *strSrc ) )
MEX_H_DECLARE_FUNC( char*,		wcs2nstr,		( char *strDest, uint16 uDestSize, const wchar_t *wcsSrc ) )

MEX_H_DECLARE_FUNC( wchar_t*,	tstr2wcs,		( const char *strSrc ) )		//return temporary converted string
MEX_H_DECLARE_FUNC( char*,		twcs2str,		( const wchar_t *wcsSrc ) )

MEX_H_DECLARE_FUNC( int32,		snprintf,		( char *strDest, int32 iLen, const char *strFormat, ... ) )
MEX_H_DECLARE_FUNC( int32,		wsnprintf,		( wchar_t *strDest, int32 iLen, const char *strFormat, ... ) )		//note: mtk is a lot different from standard swprintf

//math functions
MEX_H_DECLARE_FUNC( void,		srand,			( uint32 uSeed ) )
MEX_H_DECLARE_FUNC( int16,		rand,			( void ) )
MEX_H_DECLARE_FUNC( float,		sqrt,			( float x ) )
MEX_H_DECLARE_FUNC( int32,		isqrt,			( int32 x ) )
MEX_H_DECLARE_FUNC( int32,		atan,			( int32 sita ) )
MEX_H_DECLARE_FUNC( int32,		sin1024,		( int32 angle ) )
MEX_H_DECLARE_FUNC( int32,		cos1024,		( int32 angle ) )

MEX_H_DECLARE_FUNC( int32,		sprintf,		( char *strDest, const char *strFormat, ... ) )			//for 3rd party requirements...

MEX_H_DECLARE_FUNC( void,		qsort,			(void *pBase, size_t num, size_t width, int (*compare)(const void *, const void *)) )
MEX_H_DECLARE_FUNC( void*,		bsearch,		(const void *pKey, const void *pBase, size_t num, size_t width, int (*compare)(const void *, const void *)) )

MEX_H_DECLARE_FUNC( wchar_t*,	utf8_to_ucs2,	(char *dest, int32 dest_size, const char *src))
MEX_H_DECLARE_FUNC( char*,		ucs2_to_utf8,	(char *dest, int32 dest_size, const char *src))
MEX_H_DECLARE_FUNC( char *,		EncodingDecoding,(E_MEX_CHSET_PAIR Scheme, char *pOutBuffer, char *pInBuffer, int32 dest_size))

#if MEX_SDK_VER >= 1007
MEX_H_DECLARE_FUNC( int32,		sscanf,			(const char*, const char*, ...) )
#endif
#if defined(MEX_MEMORY_GUARD)
MEX_H_DECLARE_FUNC( void*,		secmallocex,		(int32 admid, size_t size, const char* pFile, int32 iLine) )
MEX_H_DECLARE_FUNC( void,		secfreeex,			(int32 admid, void* ptr, const char* pFile, int32 iLine) )
#else
MEX_H_DECLARE_FUNC( void*,		secmallocex,		(int32 admid, size_t size) )
MEX_H_DECLARE_FUNC( void,		secfreeex,			(int32 admid, void* ptr) )
#endif
MEX_H_DECLARE_FUNC( int32,		createadm,			(void*, int32) )
MEX_H_DECLARE_FUNC( void,		destroyadm,			(int32) )

MEX_H_ENUM_FUNC_START_STD
	MEX_H_ENUM_FUNC_STD( assertmsg )
	MEX_H_ENUM_FUNC_STD( mallocex )
	MEX_H_ENUM_FUNC_STD( freeex )
	MEX_H_ENUM_FUNC_STD( memcmp )
	MEX_H_ENUM_FUNC_STD( memchr )
	MEX_H_ENUM_FUNC_STD( memcpy )
	MEX_H_ENUM_FUNC_STD( memset )
	MEX_H_ENUM_FUNC_STD( memset16 )
	MEX_H_ENUM_FUNC_STD( memset32 )
	MEX_H_ENUM_FUNC_STD( memmove )
	MEX_H_ENUM_FUNC_STD( strlen )
	MEX_H_ENUM_FUNC_STD( strcmp )
	MEX_H_ENUM_FUNC_STD( stricmp )
	MEX_H_ENUM_FUNC_STD( strcat )
	MEX_H_ENUM_FUNC_STD( strcpy )
	MEX_H_ENUM_FUNC_STD( strchr )
	MEX_H_ENUM_FUNC_STD( strstr )
	MEX_H_ENUM_FUNC_STD( strncpy )
	MEX_H_ENUM_FUNC_STD( strncat )
	MEX_H_ENUM_FUNC_STD( strncmp )
	MEX_H_ENUM_FUNC_STD( atoi )
	MEX_H_ENUM_FUNC_STD( strlwr )
	MEX_H_ENUM_FUNC_STD( strupr )
	MEX_H_ENUM_FUNC_STD( wcslen )
	MEX_H_ENUM_FUNC_STD( wcscmp )
	MEX_H_ENUM_FUNC_STD( wcsicmp )
	MEX_H_ENUM_FUNC_STD( wcscat )
	MEX_H_ENUM_FUNC_STD( wcscpy )
	MEX_H_ENUM_FUNC_STD( wcschr )
	MEX_H_ENUM_FUNC_STD( wcsstr )
	MEX_H_ENUM_FUNC_STD( wcsncpy )
	MEX_H_ENUM_FUNC_STD( wcsncat )
	MEX_H_ENUM_FUNC_STD( wcsncmp )
	MEX_H_ENUM_FUNC_STD( wcslwr )
	MEX_H_ENUM_FUNC_STD( wcsupr )
	MEX_H_ENUM_FUNC_STD( str2nwcs )
	MEX_H_ENUM_FUNC_STD( wcs2nstr )
	MEX_H_ENUM_FUNC_STD( tstr2wcs )
	MEX_H_ENUM_FUNC_STD( twcs2str )
	MEX_H_ENUM_FUNC_STD( snprintf )
	MEX_H_ENUM_FUNC_STD( wsnprintf )
	MEX_H_ENUM_FUNC_STD( srand )
	MEX_H_ENUM_FUNC_STD( rand )
	MEX_H_ENUM_FUNC_STD( atan )
	MEX_H_ENUM_FUNC_STD( sqrt )
	MEX_H_ENUM_FUNC_STD( isqrt )
	MEX_H_ENUM_FUNC_STD( sin1024 )
	MEX_H_ENUM_FUNC_STD( cos1024 )
	MEX_H_ENUM_FUNC_STD( sprintf )
	MEX_H_ENUM_FUNC_STD( qsort )
	MEX_H_ENUM_FUNC_STD( bsearch )
	MEX_H_ENUM_FUNC_STD( utf8_to_ucs2 )
	MEX_H_ENUM_FUNC_STD( ucs2_to_utf8 )
	MEX_H_ENUM_FUNC_STD( EncodingDecoding )
#if MEX_SDK_VER >= 1007
	MEX_H_ENUM_FUNC_STD( sscanf )
#endif
	MEX_H_ENUM_FUNC_STD( secmallocex )
	MEX_H_ENUM_FUNC_STD( secfreeex )
	MEX_H_ENUM_FUNC_STD( createadm )
	MEX_H_ENUM_FUNC_STD( destroyadm )

MEX_H_ENUM_FUNC_END_STD

#if defined (MEX_TARGET_CLIENT)	//making patch

	#define assertmsg		MEX_H_GET_FUNC_DEFINE_STD( assertmsg )		//(const char *pExp, const char *pFile, unsigned int32 iLine)
	#define mallocex		MEX_H_GET_FUNC_DEFINE_STD( mallocex )		//(size_t size)
	#define freeex			MEX_H_GET_FUNC_DEFINE_STD( freeex )			//(void *ptr)
	#define memcmp			MEX_H_GET_FUNC_DEFINE_STD( memcmp )			//(const void *pData0, const void *pData1, size_t iSize)
	#define memchr			MEX_H_GET_FUNC_DEFINE_STD( memchr )			//(const void *pData, int32 iChr, size_t iSize)
	#define memcpy			MEX_H_GET_FUNC_DEFINE_STD( memcpy )			//(void *pDst, const void *pSrc, size_t iSize)
	#define memset			MEX_H_GET_FUNC_DEFINE_STD( memset )			//(void *pDst, int32 iData, size_t iSize)
	#define memset16		MEX_H_GET_FUNC_DEFINE_STD( memset16 )		//(void *pDst, int32 iData, size_t iCount)
	#define memset32		MEX_H_GET_FUNC_DEFINE_STD( memset32 )		//(void *pDst, int32 iData, size_t iCount)
	#define memmove			MEX_H_GET_FUNC_DEFINE_STD( memmove )		//(void *pDst, const void *pSrc, size_t iSize)

	#define strlen			MEX_H_GET_FUNC_DEFINE_STD( strlen )			//( const char *str )
	#define strcmp			MEX_H_GET_FUNC_DEFINE_STD( strcmp )			//( const char *str0, const char *str1 )
	#define stricmp			MEX_H_GET_FUNC_DEFINE_STD( stricmp ) 		//( const char *str0, const char *str1 )
	#define strcat			MEX_H_GET_FUNC_DEFINE_STD( strcat )			//( char *strDest, const char *strSource )
	#define strcpy			MEX_H_GET_FUNC_DEFINE_STD( strcpy )			//( char *strDest, const char *strSource )
	#define strchr			MEX_H_GET_FUNC_DEFINE_STD( strchr )			//( const char *str, int32 chr )
	#define strstr			MEX_H_GET_FUNC_DEFINE_STD( strstr )			//( const char *str, const char *strsearch )
	#define strncpy			MEX_H_GET_FUNC_DEFINE_STD( strncpy )		//( const char *str, const char *strsearch )
	#define strncat			MEX_H_GET_FUNC_DEFINE_STD( strncat )		//( char *strDst, const char *strSrc, size_t iSize )
	#define strncmp			MEX_H_GET_FUNC_DEFINE_STD( strncmp )		//( const char *str0, const char *str1, size_t iSize )
	#define atoi			MEX_H_GET_FUNC_DEFINE_STD( atoi )			//( const char *str )
	#define strlwr 			MEX_H_GET_FUNC_DEFINE_STD( strlwr )			//( char *strDst )
	#define strupr 			MEX_H_GET_FUNC_DEFINE_STD( strupr )			//( char *strDst )
	#define wcslen			MEX_H_GET_FUNC_DEFINE_STD( wcslen )			//( const wchar_t *str )
	#define wcscmp			MEX_H_GET_FUNC_DEFINE_STD( wcscmp )			//( const wchar_t *str0, const wchar_t *str1 )
	#define wcsicmp			MEX_H_GET_FUNC_DEFINE_STD( wcsicmp ) 		//( const wchar_t *str0, const wchar_t *str1 )
	#define wcscat			MEX_H_GET_FUNC_DEFINE_STD( wcscat )			//( wchar_t *strDest, const wchar_t *strSource )
	#define wcscpy			MEX_H_GET_FUNC_DEFINE_STD( wcscpy )			//( wchar_t *strDest, const wchar_t *strSource )
	#define wcschr			MEX_H_GET_FUNC_DEFINE_STD( wcschr )			//( const wchar_t *str, wchar_t chr )
	#define wcsstr			MEX_H_GET_FUNC_DEFINE_STD( wcsstr )			//( const wchar_t *str, const wchar_t *strsearch )
	#define wcsncpy			MEX_H_GET_FUNC_DEFINE_STD( wcsncpy )		//( wchar_t *strDst, const wchar_t *strSrc, size_t iSize ) 
	#define wcsncat			MEX_H_GET_FUNC_DEFINE_STD( wcsncat )		//( wchar_t *strDst, const wchar_t *strSrc, size_t iSize )
	#define wcsncmp			MEX_H_GET_FUNC_DEFINE_STD( wcsncmp )		//( const wchar_t *str0, const wchar_t *str1, size_t iSize )
	#define wcslwr			MEX_H_GET_FUNC_DEFINE_STD( wcslwr ) 		//( wchar_t *strDst )
	#define wcsupr			MEX_H_GET_FUNC_DEFINE_STD( wcsupr ) 		//( wchar_t *strDst )
	#define str2nwcs		MEX_H_GET_FUNC_DEFINE_STD( str2nwcs )		//( wchar_t *wcsDest, uint16 uDestSize, const char *strSrc )
	#define wcs2nstr		MEX_H_GET_FUNC_DEFINE_STD( wcs2nstr )		//( char *strDest, uint16 uDestSize, const wchar_t *wcsSrc )
	#define tstr2wcs		MEX_H_GET_FUNC_DEFINE_STD( tstr2wcs )		//( const char *strSrc )
	#define twcs2str		MEX_H_GET_FUNC_DEFINE_STD( twcs2str )		//( const wchar_t *wcsSrc )
	#define snprintf		MEX_H_GET_FUNC_DEFINE_STD( snprintf )		//( char *strDest, int32 iLen, const char *strFormat, ... )
	#define wsnprintf		MEX_H_GET_FUNC_DEFINE_STD( wsnprintf )		//( wchar_t *strDest, int32 iLen, const char *strFormat, ... )

	#define srand			MEX_H_GET_FUNC_DEFINE_STD( srand )			//( uint32 uSeed )
	#define rand			MEX_H_GET_FUNC_DEFINE_STD( rand )			//( void )
	#define sqrt			MEX_H_GET_FUNC_DEFINE_STD( sqrt )			//( float x )
	#define isqrt			MEX_H_GET_FUNC_DEFINE_STD( isqrt )			//( int32 x )
	#define atan			MEX_H_GET_FUNC_DEFINE_STD( atan )			//( int32 sita )
	#define sin1024			MEX_H_GET_FUNC_DEFINE_STD( sin1024 )		//( int32 angle )
	#define cos1024			MEX_H_GET_FUNC_DEFINE_STD( cos1024 )		//( int32 angle )
	#define sprintf			MEX_H_GET_FUNC_DEFINE_STD( sprintf )		//( char *strDest, const char *strFormat, ... )

	#define qsort			MEX_H_GET_FUNC_DEFINE_STD( qsort )			//(void *pBase, size_t num, size_t width, int (*compare)(const void *, const void *)
	#define bsearch			MEX_H_GET_FUNC_DEFINE_STD( bsearch )		//(const void *pKey, const void *pBase, size_t num, size_t width, int (*compare)(const void *, const void *))

	#define utf8_to_ucs2	MEX_H_GET_FUNC_DEFINE_STD( utf8_to_ucs2 )
	#define ucs2_to_utf8	MEX_H_GET_FUNC_DEFINE_STD( ucs2_to_utf8 )
	#define EncodingDecoding MEX_H_GET_FUNC_DEFINE_STD( EncodingDecoding )
#if MEX_SDK_VER >= 1007
	#define	sscanf			MEX_H_GET_FUNC_DEFINE_STD( sscanf )
#endif
	#define	secmallocex			MEX_H_GET_FUNC_DEFINE_STD( secmallocex )
	#define	secfreeex			MEX_H_GET_FUNC_DEFINE_STD( secfreeex )
	#define	createadm			MEX_H_GET_FUNC_DEFINE_STD( createadm )
	#define	destroyadm			MEX_H_GET_FUNC_DEFINE_STD( destroyadm )

#endif

#if defined(MEX_MEMORY_GUARD)
	#if defined (MEX_TARGET_CLIENT)
		#define malloc(size)		mallocex(size,__FILE__,__LINE__)
		#define free(ptr)			freeex(ptr,__FILE__,__LINE__)
	#endif
	#if defined(MEX_TARGET_SERVER)
		#define mex_malloc(size)	((void*(*)(size_t,const char*,int32))MEX_H_GET_FUNC_DEFINE_BASE(Std, mallocex))(size, __FILE__, __LINE__ )
		#define mex_free(ptr)		((void(*)(void*,const char*,int32))MEX_H_GET_FUNC_DEFINE_BASE(Std, freeex))(ptr, __FILE__, __LINE__ )
	#endif
#else
	#if defined (MEX_TARGET_CLIENT)
		#define malloc(size)		mallocex(size)
		#define free(ptr)			freeex(ptr)
	#endif
	#if defined(MEX_TARGET_SERVER)
		#define mex_malloc(size)	((void*(*)(size_t))MEX_H_GET_FUNC_DEFINE_BASE(Std, mallocex))(size)
		#define mex_free(ptr)		((void(*)(void*))MEX_H_GET_FUNC_DEFINE_BASE(Std, freeex))(ptr)
	#endif
#endif
#if defined(MEX_MEMORY_GUARD)
	#if defined (MEX_TARGET_CLIENT)
		#define secmalloc(admid, size)		secmallocex(admid, size,__FILE__,__LINE__)
		#define secfree(admid, ptr)			secfreeex(admid, ptr,__FILE__,__LINE__)
	#endif
	#if defined(MEX_TARGET_SERVER)
		#define mex_secmalloc(admid, size)	((void*(*)(int32, size_t,const char*,int32))MEX_H_GET_FUNC_DEFINE_BASE(Std, secmallocex))(admid, size, __FILE__, __LINE__ )
		#define mex_secfree(admid, ptr)		((void(*)(int32, void*,const char*,int32))MEX_H_GET_FUNC_DEFINE_BASE(Std, secfreeex))(admid, ptr, __FILE__, __LINE__ )
	#endif
#else
	#if defined (MEX_TARGET_CLIENT)
		#define secmalloc(admid, size)		secmallocex(admid, size)
		#define secfree(admid, ptr)			secfreeex(admid, ptr)
	#endif
	#if defined(MEX_TARGET_SERVER)
		#define mex_secmalloc(admid, size)	((void*(*)(int32, size_t))MEX_H_GET_FUNC_DEFINE_BASE(Std, secmallocex))(admid, size)
		#define mex_secfree(admid, ptr)		((void(*)(int32, void*))MEX_H_GET_FUNC_DEFINE_BASE(Std, secfreeex))(admid, ptr)
	#endif
#endif

//write comments below for VA to display them
#if defined(_MEX_COMMENTS_NEVER_DEFINE_THIS_)

//断言conditions为真。
void assert(const char *pExp);											//pExp: 表达式

//申请一块size大小的内存
void* malloc(size_t size);

//释放内存指针。
void free(void *ptr);

//比较两块内存内容。返回0则为一致
int32 memcmp(const void *pData0, const void *pData1, size_t iSize);

//在内存中查找指定字符
void* memchr(const void *pData, int32 iChr, size_t iSize);

//拷贝内存到指定内存块
void* memcpy(void *pDst, const void *pSrc, size_t iSize);

//将一块内存设置为指定的值
void* memset(void *pDst, int32 iData, size_t iSize);

//双子节memset
void* memset16(void *pDst, int32 iData, size_t iCount);

//四子节memset
void* memset32(void *pDst, int32 iData, size_t iCount);

//拷贝内存到指定内存块。两块内存有重叠的情况，可以继续工作
void* memmove(void *pDst, const void *pSrc, size_t iSize);

//返回char型字串的长度
size_t strlen( const char *str );

//比较两个字串内容。返回0则为一致
int32 strcmp( const char *str0, const char *str1 );

//比较两个字串内容。忽略大小写。返回0则为一致
int32 stricmp( const char *str0, const char *str1 );

//将strSource字串连接到strDest得后面
char* strcat( char *strDest, const char *strSource );

//将strSource字串拷贝到strDest里
char* strcpy( char *strDest, const char *strSource );

//在字串中查找指定字符，没有则返回NULL
char* strchr( const char *str, int32 chr );

//在字串中查找指定字串，没有则返回NULL
char* strstr( const char *str, const char *strsearch );


//strSource字串拷贝到strDest里。注意指定拷贝iSize大小
char* strncpy( char *strDst, const char *strSrc, size_t iSize );

//将strSource字串连接到strDest得后面。注意指定连接iSize大小
char* strncat( char *strDst, const char *strSrc, size_t iSize );

//比较两个字串内容。返回0则为一致。注意指定比较iSize大小
int32 strncmp( const char *str0, const char *str1, size_t iSize );

//将字串转成整型
int32 atoi( const char *str );

//转换字符串为小写
char* strlwr( char *strDst );

//转换字符串为大写
char* strupr( char *strDst );


//取得宽字串长度
size_t wcslen( const wchar_t *str );

//比较两个宽字串内容。返回0则为一致
int32 wcscmp( const wchar_t *str0, const wchar_t *str1 );

//比较两个宽字串内容。忽略大小写。返回0则为一致
int32 wcsicmp( const wchar_t *str0, const wchar_t *str1 );

//将strSource字串连接到strDest得后面
wchar_t* wcscat( wchar_t *strDest, const wchar_t *strSource );

//将strSource字串拷贝到strDest里
wchar_t* wcscpy( wchar_t *strDest, const wchar_t *strSource );

//在宽字串中查找指定字符，没有则返回NULL
wchar_t* wcschr( const wchar_t *str, wchar_t chr );

//在宽字串中查找指定字串，没有则返回NULL
wchar_t* wcsstr( const wchar_t *str, const wchar_t *strsearch );


//将strSource字串拷贝到strDest里。注意指定拷贝iSize大小
wchar_t* wcsncpy( wchar_t *strDst, const wchar_t *strSrc, size_t iSize );

//将strSource字串连接到strDest得后面。注意指定连接iSize大小
wchar_t* wcsncat( wchar_t *strDst, const wchar_t *strSrc, size_t iSize );

//比较两个字串内容。返回0则为一致。注意指定比较iSize大小
int32 wcsncmp( const wchar_t *str0, const wchar_t *str1, size_t iSize );

//转换宽字符串为小写
wchar_t* wcslwr( wchar_t *strDst );

//转换宽字符串为大写
wchar_t* wcsupr( wchar_t *strDst );


//将字符串转成宽字符
wchar_t* str2nwcs( wchar_t *wcsDest, uint16 uDestSize, const char *strSrc );

//将宽字符串转成字符
char* wcs2nstr( char *strDest, uint16 uDestSize, const wchar_t *wcsSrc );


//将字符串转成宽字符串。返回值可以直接使用。
//注意只能使用保存一次结果, 长度不得超过150
wchar_t* tstr2wcs( const char *strSrc );		//return temporary converted string

//将宽字符串转成字符串。返回值可以直接使用。
//注意只能使用保存一次结果, 长度不得超过300
char* twcs2str( const wchar_t *wcsSrc );


//按格式拼接字符串
int32 snprintf( char *strDest, int32 iLen, const char *strFormat, ... );

//按格式拼接宽字符串
int32 wsnprintf( wchar_t *strDest, int32 iLen, const char *strFormat, ... );		//note: mtk is a lot different from standard swprintf

//math functions
void srand( uint32 uSeed );

//返回一个随机数
int16 rand( void );

//浮点数开方
float sqrt( float x );

//整数开方
int32 isqrt( int32 x );

//角度正切值
int32 atan( int32 sita );

//任意角度正弦值。结果被放大了1024倍
int32 sin1024( int32 angle );

//任意角度余弦值。结果被放大了1024倍
int32 cos1024( int32 angle );

int32 sprintf( char *strDest, const char *strFormat, ... );

void qsort(void *pBase, size_t num, size_t width, int (*compare)(const void *, const void *));

void * bsearch(const void *pKey, const void *pBase, size_t num, size_t width, int (*compare)(const void *, const void *));


//字符集编码转换 UTF-8 TO UCS2
//参数1:转换后保存的目标缓存地址
//参数2:目标的缓存的大小（字节数）
//参数3:转换的字符串源
//返回: 转换后的字符串地址
wchar_t *utf8_to_ucs2(char *dest, int32 dest_size, const char *src);
//字符集编码转换 UCS2 TO UTF-8
//参数1:转换后保存的目标缓存地址
//参数2:目标的缓存的大小（字节数）
//参数3:转换的数据源地址
//返回: 转换后的字符串地址
char *ucs2_to_utf8(char *dest, int32 dest_size, const char *src);
//字符集编码转换 
//参数1:转换类型
//参数2:字符编码转换输出BUFFER
//参数3:字符编码转换的字符串源
//参数4:源字符串的字节数
//返回: 转换后的字符串地址
char *EncodingDecoding(E_MEX_CHSET_PAIR Scheme, char *pOutBuffer, char *pInBuffer, int32 dest_size);


//从一个字符串中读进与指定格式相符的数据.
int32 sscanf(const char* strSrc, const char* strFormat, ...); 

//从指定的ADM二次分配
void* secmalloc(int32 iAdmId, int32 iSize);
//从指定的ADM释放内存
void secfree(int32 iAdmId, void* pPtr);
//创建一个ADM
int32 createadm(void* pPtr, int32 iSize);
//创建一个ADM
void destroyadm(int32 iAdmId);
#endif


#endif

