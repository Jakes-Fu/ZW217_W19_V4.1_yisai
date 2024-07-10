#define _MV_STRING_API_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#ifndef WIN32
#include "sfs.h"
#include "ffs.h"
#endif
//#include "mv_debug.h"
#include "sci_api.h"
#include "mv_string_api.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "gui_ucs2b_converter.h"


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


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
//  Description : CM_MultiBytesToWideChar
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_INT CM_MultiBytesToWideChar(
							_UINT nCodePage,
							_CHAR* strMultiBytes,
							_INT nMultiBytesLen,
							_WCHAR* wstrWideChar,
							_INT nWideCharLen
)
{
	
    uint16 len = 0;
    wchar *wstr_ptr = PNULL;
    uint16 wstr_len = 0;
    const uint8 *utf8_ptr = PNULL;
    uint16 utf8_len = 0;
    
    wstr_len = (uint16)nWideCharLen;
    wstr_ptr =  wstrWideChar;
    utf8_ptr =  (uint8*)strMultiBytes;
    utf8_len =  (uint16 )nMultiBytesLen;
    
    if((utf8_ptr==NULL)||(wstr_ptr==NULL))
    {
        return 0;
    }

    len = GUI_UTF8ToWstr( wstr_ptr, (uint32)wstr_len, utf8_ptr, (uint32)utf8_len);
    return len;
}


/*****************************************************************************/
//  Description : CM_WideCharToMultiBytes
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_INT CM_WideCharToMultiBytes(
							_UINT nCodePage,
							_WCHAR* wstrWideChar,
							_INT nWideCharLen,
							_CHAR* strMultiBytes,
							_INT nMultiBytesLen
)
{
    uint16 len=0;
    uint8 *utf8_ptr = PNULL; 	    
    uint16 utf8_buf_len = 0;    
    const wchar *wstr_ptr = PNULL;
    uint16 wstr_len = 0;	
      
    wstr_ptr = wstrWideChar;
    utf8_ptr =  (uint8*)strMultiBytes ;
    wstr_len = (uint16)(nWideCharLen/2);
    utf8_buf_len = (uint16)nMultiBytesLen;
  
    if((utf8_ptr==NULL)||(wstr_ptr==NULL))
    {
        return 0;
    }
      
   len = GUI_WstrToUTF8(utf8_ptr,(uint32)utf8_buf_len,wstr_ptr, (uint32)wstr_len);
   
   return len;
	
}




