/************************************************************************
* ��Ȩ���� (C)2010,��Ѷ�Ƽ���
* 
* �ļ����ƣ� // MexLib_Std.h
* �ļ���ʶ��
* ����ժҪ�� // mexƽ̨std�⺯��ͷ�ļ�
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

//����conditionsΪ�档
void assert(const char *pExp);											//pExp: ���ʽ

//����һ��size��С���ڴ�
void* malloc(size_t size);

//�ͷ��ڴ�ָ�롣
void free(void *ptr);

//�Ƚ������ڴ����ݡ�����0��Ϊһ��
int32 memcmp(const void *pData0, const void *pData1, size_t iSize);

//���ڴ��в���ָ���ַ�
void* memchr(const void *pData, int32 iChr, size_t iSize);

//�����ڴ浽ָ���ڴ��
void* memcpy(void *pDst, const void *pSrc, size_t iSize);

//��һ���ڴ�����Ϊָ����ֵ
void* memset(void *pDst, int32 iData, size_t iSize);

//˫�ӽ�memset
void* memset16(void *pDst, int32 iData, size_t iCount);

//���ӽ�memset
void* memset32(void *pDst, int32 iData, size_t iCount);

//�����ڴ浽ָ���ڴ�顣�����ڴ����ص�����������Լ�������
void* memmove(void *pDst, const void *pSrc, size_t iSize);

//����char���ִ��ĳ���
size_t strlen( const char *str );

//�Ƚ������ִ����ݡ�����0��Ϊһ��
int32 strcmp( const char *str0, const char *str1 );

//�Ƚ������ִ����ݡ����Դ�Сд������0��Ϊһ��
int32 stricmp( const char *str0, const char *str1 );

//��strSource�ִ����ӵ�strDest�ú���
char* strcat( char *strDest, const char *strSource );

//��strSource�ִ�������strDest��
char* strcpy( char *strDest, const char *strSource );

//���ִ��в���ָ���ַ���û���򷵻�NULL
char* strchr( const char *str, int32 chr );

//���ִ��в���ָ���ִ���û���򷵻�NULL
char* strstr( const char *str, const char *strsearch );


//strSource�ִ�������strDest�ע��ָ������iSize��С
char* strncpy( char *strDst, const char *strSrc, size_t iSize );

//��strSource�ִ����ӵ�strDest�ú��档ע��ָ������iSize��С
char* strncat( char *strDst, const char *strSrc, size_t iSize );

//�Ƚ������ִ����ݡ�����0��Ϊһ�¡�ע��ָ���Ƚ�iSize��С
int32 strncmp( const char *str0, const char *str1, size_t iSize );

//���ִ�ת������
int32 atoi( const char *str );

//ת���ַ���ΪСд
char* strlwr( char *strDst );

//ת���ַ���Ϊ��д
char* strupr( char *strDst );


//ȡ�ÿ��ִ�����
size_t wcslen( const wchar_t *str );

//�Ƚ��������ִ����ݡ�����0��Ϊһ��
int32 wcscmp( const wchar_t *str0, const wchar_t *str1 );

//�Ƚ��������ִ����ݡ����Դ�Сд������0��Ϊһ��
int32 wcsicmp( const wchar_t *str0, const wchar_t *str1 );

//��strSource�ִ����ӵ�strDest�ú���
wchar_t* wcscat( wchar_t *strDest, const wchar_t *strSource );

//��strSource�ִ�������strDest��
wchar_t* wcscpy( wchar_t *strDest, const wchar_t *strSource );

//�ڿ��ִ��в���ָ���ַ���û���򷵻�NULL
wchar_t* wcschr( const wchar_t *str, wchar_t chr );

//�ڿ��ִ��в���ָ���ִ���û���򷵻�NULL
wchar_t* wcsstr( const wchar_t *str, const wchar_t *strsearch );


//��strSource�ִ�������strDest�ע��ָ������iSize��С
wchar_t* wcsncpy( wchar_t *strDst, const wchar_t *strSrc, size_t iSize );

//��strSource�ִ����ӵ�strDest�ú��档ע��ָ������iSize��С
wchar_t* wcsncat( wchar_t *strDst, const wchar_t *strSrc, size_t iSize );

//�Ƚ������ִ����ݡ�����0��Ϊһ�¡�ע��ָ���Ƚ�iSize��С
int32 wcsncmp( const wchar_t *str0, const wchar_t *str1, size_t iSize );

//ת�����ַ���ΪСд
wchar_t* wcslwr( wchar_t *strDst );

//ת�����ַ���Ϊ��д
wchar_t* wcsupr( wchar_t *strDst );


//���ַ���ת�ɿ��ַ�
wchar_t* str2nwcs( wchar_t *wcsDest, uint16 uDestSize, const char *strSrc );

//�����ַ���ת���ַ�
char* wcs2nstr( char *strDest, uint16 uDestSize, const wchar_t *wcsSrc );


//���ַ���ת�ɿ��ַ���������ֵ����ֱ��ʹ�á�
//ע��ֻ��ʹ�ñ���һ�ν��, ���Ȳ��ó���150
wchar_t* tstr2wcs( const char *strSrc );		//return temporary converted string

//�����ַ���ת���ַ���������ֵ����ֱ��ʹ�á�
//ע��ֻ��ʹ�ñ���һ�ν��, ���Ȳ��ó���300
char* twcs2str( const wchar_t *wcsSrc );


//����ʽƴ���ַ���
int32 snprintf( char *strDest, int32 iLen, const char *strFormat, ... );

//����ʽƴ�ӿ��ַ���
int32 wsnprintf( wchar_t *strDest, int32 iLen, const char *strFormat, ... );		//note: mtk is a lot different from standard swprintf

//math functions
void srand( uint32 uSeed );

//����һ�������
int16 rand( void );

//����������
float sqrt( float x );

//��������
int32 isqrt( int32 x );

//�Ƕ�����ֵ
int32 atan( int32 sita );

//����Ƕ�����ֵ��������Ŵ���1024��
int32 sin1024( int32 angle );

//����Ƕ�����ֵ��������Ŵ���1024��
int32 cos1024( int32 angle );

int32 sprintf( char *strDest, const char *strFormat, ... );

void qsort(void *pBase, size_t num, size_t width, int (*compare)(const void *, const void *));

void * bsearch(const void *pKey, const void *pBase, size_t num, size_t width, int (*compare)(const void *, const void *));


//�ַ�������ת�� UTF-8 TO UCS2
//����1:ת���󱣴��Ŀ�껺���ַ
//����2:Ŀ��Ļ���Ĵ�С���ֽ�����
//����3:ת�����ַ���Դ
//����: ת������ַ�����ַ
wchar_t *utf8_to_ucs2(char *dest, int32 dest_size, const char *src);
//�ַ�������ת�� UCS2 TO UTF-8
//����1:ת���󱣴��Ŀ�껺���ַ
//����2:Ŀ��Ļ���Ĵ�С���ֽ�����
//����3:ת��������Դ��ַ
//����: ת������ַ�����ַ
char *ucs2_to_utf8(char *dest, int32 dest_size, const char *src);
//�ַ�������ת�� 
//����1:ת������
//����2:�ַ�����ת�����BUFFER
//����3:�ַ�����ת�����ַ���Դ
//����4:Դ�ַ������ֽ���
//����: ת������ַ�����ַ
char *EncodingDecoding(E_MEX_CHSET_PAIR Scheme, char *pOutBuffer, char *pInBuffer, int32 dest_size);


//��һ���ַ����ж�����ָ����ʽ���������.
int32 sscanf(const char* strSrc, const char* strFormat, ...); 

//��ָ����ADM���η���
void* secmalloc(int32 iAdmId, int32 iSize);
//��ָ����ADM�ͷ��ڴ�
void secfree(int32 iAdmId, void* pPtr);
//����һ��ADM
int32 createadm(void* pPtr, int32 iSize);
//����һ��ADM
void destroyadm(int32 iAdmId);
#endif


#endif

