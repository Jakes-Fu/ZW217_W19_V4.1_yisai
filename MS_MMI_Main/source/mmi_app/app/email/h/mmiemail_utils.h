/****************************************************************************
** File Name:      mmiemail_utils.h                                        *
** Author:                                                                 *
** Date:           05/13/2011                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2011                           create
** 
****************************************************************************/
#ifndef _EMAILA_UTILS_H_
#define _EMAILA_UTILS_H_

/**---------------------------------------------------------------------------*
**                         Include Files                                      *
**---------------------------------------------------------------------------*/

#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "we_log.h"
#include "mmk_app.h"
#include "mmicom_time.h"
#include "guires.h"
#include "tf3rd_for_hz.h"


/**---------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern  "C"
{
#endif
   
/**---------------------------------------------------------------------------*
**                         Macro Declaration    							  *
**---------------------------------------------------------------------------*/
//define error
#define EMAIL_SUCCESS       SCI_SUCCESS
#define EMAIL_NOMEMORY      SCI_NO_MEMORY           
#define EMAIL_BADPARAM      SCI_PARAM_ERROR
#define EMAIL_ERROR         SCI_ERROR 

#define    MMIEMAIL_CLEAR_CLIENT(win_id)\
    {\
    GUI_RECT_T de_rect = MMITHEME_GetClientRect();\
    GUI_LCD_DEV_INFO    de_lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};\
    de_lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;\
    de_lcd_dev_info.block_id = GUI_BLOCK_MAIN;\
    GUIRES_DisplayImg(PNULL,&de_rect,&de_rect,win_id,IMAGE_COMMON_BG,&de_lcd_dev_info);\
}


#ifndef EMA_UTILS_MEMMGR_FOR_FREE
#define EMA_UTILS_MEMMGR_FOR_FREE   0xAAAAAAAA
#endif
   
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/

typedef struct email_mem_status_tag
{
    unsigned long lib_mem_obj;//num of memory blocks allocated
    unsigned long lib_mem_used;//memory used for data content
    unsigned long lib_mem_max;
}EMAIL_MEM_STATUS_T;
 
typedef struct email_mem_link_tag
{
    unsigned long              cc1;// check sum , used to check if it is a valid node[10/6/2006 Samuel]
    struct email_mem_link_tag  *next_ptr;// reverse list [10/6/2006 Samuel]
    struct email_mem_link_tag  *last_ptr;
    
    unsigned long             length;// memory length allocated [10/6/2006 Samuel]
    char                          *file_name_ptr;// file where the memory was allocated [10/6/2006 Samuel]
    unsigned long             line ;// line where memory was allocated [10/6/2006 Samuel]
    unsigned long             id;// times memory allocated [10/6/2006 Samuel]
    unsigned long             cc2;//checksum   ,used to check if it is a valid node
    void              *mem_mgr_ptr;
}EMAIL_MEM_LINK_T;
 
typedef struct email_mem_hangle_tag
{
    struct email_mem_link_tag  header;
    struct email_mem_link_tag * tail_ptr;
    struct email_mem_status_tag status;
}EMAIL_MEM_HANDLE_T;


typedef enum
{
    EMA_DATE_YMD = 0,
    EMA_DATE_MD,
    EMA_TIME,
    EMA_DATETIME,
    EMA_DATE_TYPE_MAX_FOURBTYES = 0x7fffffff
}EMAIL_DATE_TYPE_E;

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description: out put memory that used
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_OutputMemUsed(EMAIL_MEM_HANDLE_T* mem_mgr_ptr);

/*****************************************************************************/
// 	Description: malloc memory
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void* MMIEMAIL_Malloc(EMAIL_MEM_HANDLE_T* mem_mgr_ptr, unsigned long size,const char * file_name_ptr, int line);

/*****************************************************************************/
// 	Description: realloc memory
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void* MMIEMAIL_Realloc(EMAIL_MEM_HANDLE_T* mem_mgr_ptr,void *src_ptr, unsigned long size,const char * file_name_ptr,int line);

/*****************************************************************************/
// 	Description: free memory
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int MMIEMAIL_Free(EMAIL_MEM_HANDLE_T* mem_mgr_ptr, void * mem_free_ptr,const char * file_name_ptr,int line);

/*****************************************************************************/
// 	Description: change the w string to uft8 format string
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC char *MMIEMAIL_WstrToUtf8(EMAIL_MEM_HANDLE_T* mem_mgr_ptr, wchar *src_ptr);

/*****************************************************************************/
// 	Description: change the uft8 to w string format string
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC wchar *MMIEMAIL_Utf8ToWstr(EMAIL_MEM_HANDLE_T* mem_mgr_ptr, char *src_ptr);

/*****************************************************************************/
// 	Description: change the w string to string format string
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC char *MMIEMAIL_WstrToStr(EMAIL_MEM_HANDLE_T* mem_mgr_ptr, wchar *src_ptr);

/*****************************************************************************/
// 	Description: change the w string to string format string
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC wchar *MMIEMAIL_StrToWstr(EMAIL_MEM_HANDLE_T* mem_mgr_ptr, char *src_ptr); 

/*****************************************************************************/
// 	Description: w string copy
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC wchar * MMIEMAIL_WstrDup(void* mem_mgr_ptr, wchar* src_ptr);

/*****************************************************************************/
// 	Description: string copy
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC char* MMIEMAIL_StrDup(void* mem_mgr_ptr, char* src_ptr);

/*****************************************************************************/
// 	Description: find the char in w string
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC wchar *MMIEMAIL_Wstrchr(wchar *string_ptr, wchar flag);

/*****************************************************************************/
// Description : charset usc2 to utf8 len
// Global resource dependence : 
// Author: 
// Note:
/*****************************************************************************/
PUBLIC char* MMIEMAIL_StrStrNC(char *to_search_ptr, const char *search_for_ptr);


/*****************************************************************************/
// 	Description: get email address
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC wchar *MMIEMAIL_GetEmailAddress(wchar *full_addr_ptr);

/*****************************************************************************/
// 	Description: get date time
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIEMAIL_GetDateTime(WE_VOID *mem_mgr_ptr, uint32 date, EMAIL_DATE_TYPE_E date_type);

/*****************************************************************************/
// 	Description: judge is today
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIEMAIL_IsToday(uint32 date);

/*****************************************************************************/
// 	Description: set size text
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32  MMIEMAIL_SetSize2Text(void *mem_mgr_ptr,uint32 size, wchar ** out_text_pptr); 

/*****************************************************************************/
// 	Description: trace
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_DbgPrintf(const char *format_ptr,...);

/**---------------------------------------------------------------------------*
**                         Macro Declaration    							  *
**---------------------------------------------------------------------------*/
 
#ifndef EMA_UTILS_MALLOC
#ifdef EMA_UTILS_MEMORY_MGR
#define EMA_UTILS_MALLOC(p,n) MMIEMAIL_Malloc(p, n, __FILE__, __LINE__) 
#else
#define EMA_UTILS_MALLOC(p,n) TF3RD_MALLOCMEMORY(n)//SCI_ALLOCA(n)
#endif
#endif

#ifndef EMA_UTILS_REALLOC
#ifdef EMA_UTILS_MEMORY_MGR
#define EMA_UTILS_REALLOC(p,src,n) MMIEMAIL_Realloc(p,src, n, __FILE__, __LINE__) 
#else
#define EMA_UTILS_REALLOC(p,src,n)   TF3RD_REALLOC(src,n)
#endif
#endif

#ifndef EMA_UTILS_FREE
#ifdef EMA_UTILS_MEMORY_MGR
#define EMA_UTILS_FREE(mem_mgr_ptr, p) MMIEMAIL_Free(mem_mgr_ptr, (void*)p, __FILE__, __LINE__)
#else 
#define EMA_UTILS_FREE(mem_mgr_ptr, p) {TF3RD_MEMFREE(p);p=PNULL;}
#endif
#endif

#ifndef EMA_UTILS_FREEIF
#define EMA_UTILS_FREEIF(mem_mgr_ptr, p)    if (PNULL != p) {TF3RD_MEMFREE(p); p=PNULL;}
#endif

#ifndef EMA_UTILS_MEMCPY
#define EMA_UTILS_MEMCPY     TF3RD_MEMCPY
#endif 

#ifndef EMA_UTILS_MEMSET
#define EMA_UTILS_MEMSET     TF3RD_MEMSET
#endif 

#ifndef EMA_UTILS_MEMCMP
#define EMA_UTILS_MEMCMP     TF3RD_MEMCMP
#endif

#ifndef EMA_UTILS_MEMMOVE
#define EMA_UTILS_MEMMOVE     TF3RD_MEMMOVE
#endif

#ifndef EMA_UTILS_GETSECONDS
#define EMA_UTILS_GETSECONDS    MMIAPICOM_GetCurTime()
#endif

#ifndef EMA_UTILS_WSTRTOSTR
#define EMA_UTILS_WSTRTOSTR     MMIEMAIL_WstrToStr
#endif


#ifndef EMA_UTILS_STRTOWSTR
#define EMA_UTILS_STRTOWSTR     MMIEMAIL_StrToWstr
#endif

#ifndef EMA_UTILS_WSTRTOUTF8
#define EMA_UTILS_WSTRTOUTF8    MMIEMAIL_WstrToUtf8
#endif

#ifndef EMA_UTILS_UTF8TOWSTR
#define EMA_UTILS_UTF8TOWSTR    MMIEMAIL_Utf8ToWstr
#endif
// 
// #ifndef EMA_UTILS_STRCMPNC
// #define EMA_UTILS_STRCMPNC      STRICMP
// #endif
//  
#ifndef EMA_UTILS_STRCMP
#define EMA_UTILS_STRCMP       TF3RD_STRCMP
#endif

#ifndef EMA_UTILS_STRLEN
#define EMA_UTILS_STRLEN         (unsigned int)TF3RD_STRLEN
#endif 
// 
// #ifndef EMA_UTILS_STRCPY
// #define EMA_UTILS_STRCPY        STRCPY
// #endif 
// 
// #ifndef EMA_UTILS_STRNCPY
// #define EMA_UTILS_STRNCPY        STRNCPY
// #endif 
// 
// #ifndef EMA_UTILS_STRCMP
// #define EMA_UTILS_STRCMP        STRCMP
// #endif 
// 

#ifndef EMA_UTILS_STRNCMP
#define EMA_UTILS_STRNCMP        TF3RD_STRNCMP
#endif


#ifndef EMA_UTILS_SPRINTF
#define EMA_UTILS_SPRINTF       TF3RD_SPRINTF
#endif 

#ifndef EMA_UTILS_SNPRINTF
#define EMA_UTILS_SNPRINTF       TF3RD_SNPRINTF
#endif 

#ifndef EMA_UTILS_STRCAT
#define EMA_UTILS_STRCAT         TF3RD_STRCAT
#endif


#ifndef EMA_UTILS_STRSTR
#define EMA_UTILS_STRSTR         TF3RD_STRSTR
#endif


#ifndef EMA_UTILS_ATOI
#define EMA_UTILS_ATOI           TF3RD_STRATOI//atoi
#endif

#ifndef EMA_UTILS_WSTRCPY
#define EMA_UTILS_WSTRCPY        MMIAPICOM_Wstrcpy
#endif 

#ifndef EMA_UTILS_WSTRLCPY		
#define EMA_UTILS_WSTRLCPY        MMIAPICOM_Wstrncpy
#endif

#ifndef EMA_UTILS_WSTRCMP
#define EMA_UTILS_WSTRCMP         MMIAPICOM_Wstrcmp
#endif

// 
// #ifndef EMA_UTILS_WSTRICMP
// #define EMA_UTILS_WSTRICMP       WSTRICMP
// #endif
// 

#ifndef EMA_UTILS_WSTRNICMP
#define EMA_UTILS_WSTRNICMP       MMIAPICOM_Wstrncmp
#endif

#ifndef EMA_UTILS_WSTRCAT
#define EMA_UTILS_WSTRCAT       MMIAPICOM_Wstrcat
#endif

#ifndef EMA_UTILS_WSTRLCAT
#define EMA_UTILS_WSTRLCAT       MMIAPICOM_Wstrncat
#endif

#ifndef EMA_UTILS_STRCHR
#define EMA_UTILS_STRCHR       TF3RD_STRCHR
#endif
 
#ifndef EMA_UTILS_WSTRLEN
#define EMA_UTILS_WSTRLEN       MMIAPICOM_Wstrlen
#endif

#ifndef EMA_UTILS_STRLEN
#define EMA_UTILS_STRLEN       TF3RD_STRLEN
#endif

// #ifndef EMA_UTILS_STRLOWER
// #define EMA_UTILS_STRLOWER      STRLOWER
// #endif
// 
// #ifndef EMA_UTILS_STRUPPER
// #define EMA_UTILS_STRUPPER      STRUPPER
// #endif
// 
// #ifndef EMA_UTILS_STRUPPER
// #define EMA_UTILS_STRUPPER      STRUPPER
// #endif
// 
// 
#ifndef EMA_UTILS_ISNULLSTR
#define EMA_UTILS_ISNULLSTR(p)  ((!(p)) || (0 == SCI_STRLEN(p)))
#endif
 

#ifndef EMA_UTILS_ISNULLWSTR
#define EMA_UTILS_ISNULLWSTR(p) ((!(p)) || (0 == MMIAPICOM_Wstrlen(p)))
#endif


#ifndef EMA_UTILS_WSTRDUP
#define EMA_UTILS_WSTRDUP(mem_mgr_ptr,p) MMIEMAIL_WstrDup(mem_mgr_ptr, p)
#endif

#ifndef EMA_UTILS_STRDUP
#define EMA_UTILS_STRDUP(mem_mgr_ptr, p) MMIEMAIL_StrDup(mem_mgr_ptr, p)
#endif

#ifndef EMA_UTILS_STRLCPY
#define EMA_UTILS_STRLCPY		   strncpy
#endif

#ifndef EMA_UTILS_WSTRCHR
#define EMA_UTILS_WSTRCHR         MMIEMAIL_Wstrchr
#endif

// #ifndef EMA_UTILS_ASSERT
// #define EMA_UTILS_ASSERT          //SCI_ASSERT
// #endif

#ifndef EMA_UTILS_TRACE
//#define EMA_UTILS_TRACE       MMIEMAIL_DbgPrintf("%s, %d", (strrchr(__FILE__, '\\') + 1), __LINE__);MMIEMAIL_DbgPrintf 
#define EMA_UTILS_TRACE       MMIEMAIL_DbgPrintf 
#endif

#ifndef EMA_UTILS_LOG_INFO
#define EMA_UTILS_LOG_INFO(x)    EMA_UTILS_TRACE x
#endif

#ifndef EMA_UTILS_LOG_ERROR
#define EMA_UTILS_LOG_ERROR(x)   EMA_UTILS_TRACE x
#endif  


#ifndef EMA_UTILS_BREAKIF
#define EMA_UTILS_BREAKIF(iResult, desc) { \
    if(EMAIL_SUCCESS != (iResult)) { \
{\
    EMA_UTILS_LOG_ERROR(("%s,ECode:%d", desc, iResult));\
    break;\
}\
    } \
}
#endif

#ifndef EMA_UTILS_BREAKNULL
#define EMA_UTILS_BREAKNULL(p,iResult,desc) {\
    if(PNULL == p)\
{\
    iResult = EMAIL_NOMEMORY;\
    EMA_UTILS_LOG_ERROR(("%s,ECode:%d", desc, iResult));\
    break;\
}\
}
#endif

      
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


#endif  /* _EMAILA_UTILS_H_*/
