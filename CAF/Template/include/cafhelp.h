/****************************************************************************
** File Name:      cafhelp.h                                               *
** Author:         James.Zhang                                             *
** Date:           05/21/2008                                              *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2008        James.Zhang       Create
** 
****************************************************************************/

#ifndef  _CAF_HELP_H_    
#define  _CAF_HELP_H_  

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                      */
/*----------------------------------------------------------------------------*/
#include "cafstdlib.h"
#include "cafshell.h"

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/

typedef struct  
{
    const CAF_STDLIB_T* stdlib_ptr;
    const CAF_SHELL_T*  shell_ptr;

}CAF_HELP_T;

#ifdef CAF_DYANMIC_MODULE

#ifdef WIN32

#define __CAF_MODULE_DEF__  __FILE__

extern const CAF_HELP_T *g_caf_help_ptr;

#define CAF_GET_HELPPTR()     g_caf_help_ptr
#else/* else WIN32 */

#define __CAF_MODULE_DEF__  __MODULE__

extern CAF_RESULT_E CAFMODULE_Load( const void* help_ptr, IMODULE_T** mod_pptr );

#define CAF_GET_HELPPTR()      ( (*(((CAF_HELP_T **)((uint32)CAFMODULE_Load & 0xfffffffe)) - 1))  )
#endif/* endif WIN32 */

#define CAF_GET_STDLIB()       (CAF_GET_HELPPTR()->stdlib_ptr)
#define CAF_GET_SHELL()        (CAF_GET_HELPPTR()->shell_ptr)

/* ANSI c function, CAF base lib -> ANSI lib, functions one by one, if user like ANSI, change to # if (FALSE) */
#if  (TRUE)

//string
#define CAF_MEMCPY             CAF_GET_STDLIB()->Memcpy
#define CAF_MEMMOVE            CAF_GET_STDLIB()->Memmove
#define CAF_STRCPY             CAF_GET_STDLIB()->Strcpy
#define CAF_STRNCPY            CAF_GET_STDLIB()->Strncpy
#define CAF_STRCAT             CAF_GET_STDLIB()->Strcat
#define CAF_STRNCAT            CAF_GET_STDLIB()->Strncat
#define CAF_MEMCMP             CAF_GET_STDLIB()->Memcmp
#define CAF_STRCMP             CAF_GET_STDLIB()->Strcmp
#define CAF_STRNCMP            CAF_GET_STDLIB()->Strncmp
#define CAF_STRCOLL            CAF_GET_STDLIB()->Strcoll
#define CAF_STRXFRM            CAF_GET_STDLIB()->Strxfrm
#define CAF_MEMCHR             CAF_GET_STDLIB()->Memchr
#define CAF_STRCHR             CAF_GET_STDLIB()->Strchr
#define CAF_STRCSPN            CAF_GET_STDLIB()->Strcspn
#define CAF_STRPBRK            CAF_GET_STDLIB()->Strpbrk
#define CAF_STRRCHR            CAF_GET_STDLIB()->Strrchr
#define CAF_STRSPN             CAF_GET_STDLIB()->Strspn
#define CAF_STRSTR             CAF_GET_STDLIB()->Strstr
#define CAF_STRTOK             CAF_GET_STDLIB()->Strtok
#define CAF_MEMSET             CAF_GET_STDLIB()->Memset
#define CAF_STRERROR           CAF_GET_STDLIB()->Strerror
#define CAF_STRLEN             CAF_GET_STDLIB()->Strlen
#define CAF_STRLOWER           CAF_GET_STDLIB()->Strlower
#define CAF_STRUPPER           CAF_GET_STDLIB()->Strupper

//ctype
#define CAF_ISALNUM            CAF_GET_STDLIB()->Isalnum
#define CAF_ISALPHA            CAF_GET_STDLIB()->Isalpha
#define CAF_ISCNTRL            CAF_GET_STDLIB()->Iscntrl
#define CAF_ISDIGIT            CAF_GET_STDLIB()->Isdigit
#define CAF_ISGRAPH            CAF_GET_STDLIB()->Isgraph
#define CAF_ISLOWER            CAF_GET_STDLIB()->Islower
#define CAF_ISPUNCT            CAF_GET_STDLIB()->Ispunct
#define CAF_ISPRINT            CAF_GET_STDLIB()->Isprint
#define CAF_ISSPACE            CAF_GET_STDLIB()->Isspace
#define CAF_ISUPPER            CAF_GET_STDLIB()->Isupper
#define CAF_ISXDIGIT           CAF_GET_STDLIB()->Isxdigit
#define CAF_TOLOWER            CAF_GET_STDLIB()->Tolower
#define CAF_TOUPPER            CAF_GET_STDLIB()->Toupper

//stdlib
#define CAF_ATOI               CAF_GET_STDLIB()->Atoi
#define CAF_ATOL               CAF_GET_STDLIB()->Atol
#define CAF_STRTOL             CAF_GET_STDLIB()->Strtol
#define CAF_STRTOUL            CAF_GET_STDLIB()->Strtoul
#define CAF_RAND               CAF_GET_STDLIB()->Rand
#define CAF_SRAND              CAF_GET_STDLIB()->Srand
#define CAF_QSORT              CAF_GET_STDLIB()->Qsort
#define CAF_ABS                CAF_GET_STDLIB()->Abs

//stdio
#define CAF_SPRINTF            CAF_GET_STDLIB()->Sprintf
#define CAF_VSPRINTF           CAF_GET_STDLIB()->Vsprintf

#define CAF_ALLOC(_SIZE_)      CAF_GET_STDLIB()->Malloc(_SIZE_, __CAF_MODULE_DEF__, __LINE__)
#define CAF_FREE               CAF_GET_STDLIB()->Free

//wstring
#define CAF_WSTRCPY            CAF_GET_STDLIB()->Wstrcpy
#define CAF_WSTRNCPY           CAF_GET_STDLIB()->Wstrncpy
#define CAF_WSTRCAT            CAF_GET_STDLIB()->Wstrcat
#define CAF_WSTRNCAT           CAF_GET_STDLIB()->Wstrncat
#define CAF_WSTRCMP            CAF_GET_STDLIB()->Wstrcmp
#define CAF_WSTRNCMP           CAF_GET_STDLIB()->Wstrncmp
#define CAF_WSTRLEN            CAF_GET_STDLIB()->Wstrlen
#define CAF_STRTOWSTR          CAF_GET_STDLIB()->Strtowstr
#define CAF_WSTRTOSTR          CAF_GET_STDLIB()->Wstrtostr
#define CAF_WSTRLOWER          CAF_GET_STDLIB()->Wstrlower
#define CAF_WSTRUPPER          CAF_GET_STDLIB()->Wstrupper

//socket
#define CAF_HTONL              CAF_GET_STDLIB()->Htonl
#define CAF_HTONS              CAF_GET_STDLIB()->Htons
#define CAF_NTOHL              CAF_GET_STDLIB()->Ntohl
#define CAF_NTOHS              CAF_GET_STDLIB()->Ntohs
#define CAF_INETATON           CAF_GET_STDLIB()->INetaton
#define CAF_INETNTOA           CAF_GET_STDLIB()->INetntoa
#define CAF_GETTICKCOUNT       CAF_GET_STDLIB()->GetTicket

#else

//string
#define memcpy                 CAF_GET_STDLIB()->Memcpy
#define memmove                CAF_GET_STDLIB()->Memmove
#define strcpy                 CAF_GET_STDLIB()->Strcpy
#define strncpy                CAF_GET_STDLIB()->Strncpy
#define strcat                 CAF_GET_STDLIB()->Strcat
#define strncat                CAF_GET_STDLIB()->Strncat
#define memcmp                 CAF_GET_STDLIB()->Memcmp
#define strcmp                 CAF_GET_STDLIB()->Strcmp
#define strncmp                CAF_GET_STDLIB()->Strncmp
#define strcoll                CAF_GET_STDLIB()->Strcoll
#define strxfrm                CAF_GET_STDLIB()->Strxfrm
#define memchr                 CAF_GET_STDLIB()->Memchr
#define strchr                 CAF_GET_STDLIB()->Strchr
#define strcspn                CAF_GET_STDLIB()->Strcspn
#define strpbrk                CAF_GET_STDLIB()->Strpbrk
#define strrchr                CAF_GET_STDLIB()->Strrchr
#define strspn                 CAF_GET_STDLIB()->Strspn
#define strstr                 CAF_GET_STDLIB()->Strstr
#define strtok                 CAF_GET_STDLIB()->Strtok
#define memset                 CAF_GET_STDLIB()->Memset
#define strerror               CAF_GET_STDLIB()->Strerror
#define strlen                 CAF_GET_STDLIB()->Strlen
#define strlwr                 CAF_GET_STDLIB()->Strlower
#define srtupr                 CAF_GET_STDLIB()->Strupper

//ctype
#define isalnum                CAF_GET_STDLIB()->Isalnum
#define isalpha                CAF_GET_STDLIB()->Isalpha
#define iscntrl                CAF_GET_STDLIB()->Iscntrl
#define isdigit                CAF_GET_STDLIB()->Isdigit
#define isgraph                CAF_GET_STDLIB()->Isgraph
#define islower                CAF_GET_STDLIB()->Islower
#define ispunct                CAF_GET_STDLIB()->Ispunct
#define isprint                CAF_GET_STDLIB()->Isprint
#define isspace                CAF_GET_STDLIB()->Isspace
#define isupper                CAF_GET_STDLIB()->Isupper
#define isxdigit               CAF_GET_STDLIB()->Isxdigit
#define tolower                CAF_GET_STDLIB()->Tolower
#define toupper                CAF_GET_STDLIB()->Toupper

//stdlib                      
#define atoi                   CAF_GET_STDLIB()->Atoi
#define atol                   CAF_GET_STDLIB()->Atol
#define strtol                 CAF_GET_STDLIB()->Strtol
#define strtoul                CAF_GET_STDLIB()->Strtoul
#define rand                   CAF_GET_STDLIB()->Rand
#define srand                  CAF_GET_STDLIB()->Srand
#define qsort                  CAF_GET_STDLIB()->Qsort
#define abs                    CAF_GET_STDLIB()->Abs

//stdio                    
#define sprintf                CAF_GET_STDLIB()->Sprintf
#define vsprintf               CAF_GET_STDLIB()->Vsprintf

#define malloc(s)              CAF_GET_STDLIB()->Malloc(s, __CAF_MODULE_DEF__, __LINE__)
#define free                   CAF_GET_STDLIB()->Free

#define sleep                  CAF_GET_STDLIB()->Sleep

//wstring
#define wcscpy                 CAF_GET_STDLIB()->Wstrcpy
#define wcsncpy                CAF_GET_STDLIB()->Wstrncpy
#define wcscat                 CAF_GET_STDLIB()->Wstrcat
#define wcsncat                CAF_GET_STDLIB()->Wstrncat
#define wcscmp                 CAF_GET_STDLIB()->Wstrcmp
#define wcsncmp                CAF_GET_STDLIB()->Wstrncmp
#define wcslen                 CAF_GET_STDLIB()->Wstrlen
#define strtowstr              CAF_GET_STDLIB()->Strtowstr
#define wstrostr               CAF_GET_STDLIB()->Wstrtostr
#define wcslwr                 CAF_GET_STDLIB()->Wstrlower
#define wcsupr                 CAF_GET_STDLIB()->Wstrupper

//socket
#define htonl                  CAF_GET_STDLIB()->Htonl
#define htons                  CAF_GET_STDLIB()->Htons
#define ntohl                  CAF_GET_STDLIB()->Ntohl
#define ntohs                  CAF_GET_STDLIB()->Ntohs
#define inet_aton              CAF_GET_STDLIB()->INetaton
#define inet_ntoa              CAF_GET_STDLIB()->INetntoa
#define gettickcount           CAF_GET_STDLIB()->GetTicket

#endif

#define CAF_ASSERT( _EXP_ )    (_EXP_) ? (void)0 : CAF_GET_STDLIB()->Assert(#_EXP_, __CAF_MODULE_DEF__, __LINE__)
#define CAF_TRACE              CAF_GET_STDLIB()->Trace
#define CAF_SLEEP              CAF_GET_STDLIB()->Sleep

#define CAF_UCS2GB             CAF_GET_STDLIB()->ConvertUCS2GB
#define CAF_GB2UCS             CAF_GET_STDLIB()->ConvertGB2UCS
#define CAF_UCS2B2UTF8         CAF_GET_STDLIB()->ConvertUCS2B2UTF8
#define CAF_UTF82UCS2B         CAF_GET_STDLIB()->ConvertUTF82UCS2B
#define CAF_UCS2BE2UCS2LE      CAF_GET_STDLIB()->ConvertUCS2BE2UCS2LE
#define CAF_UCS2LE2UCS2BE      CAF_GET_STDLIB()->ConvertUCS2LE2UCS2BE


/* shell api */

static __inline CAF_RESULT_E ISHELL_CreateInstance( CAF_GUID_T guid, void ** object_pptr ) 
{
    return CAF_GET_SHELL()->CreateInstance( guid, object_pptr );
}

static __inline CAF_HANDLE_T ISHELL_CreateWinTable( const CAF_WINDOW_TABLE_CREATE_T* win_table_ptr ) 
{
    return CAF_GET_SHELL()->CreateWinTable( win_table_ptr );
}

static __inline BOOLEAN ISHELL_CloseWindow( CAF_HANDLE_T win_handle )
{
    return CAF_GET_SHELL()->CloseWindow( win_handle );
}

static __inline BOOLEAN ISHELL_SetWinSoftkeyId( CAF_HANDLE_T win_handle, uint32 leftsoft_id, uint32 middlesoft_id, uint32 rightsoft_id, BOOLEAN is_need_update )
{
    return CAF_GET_SHELL()->SetWinSoftkeyId( win_handle, leftsoft_id, middlesoft_id, rightsoft_id, is_need_update );
}

static __inline IDC_T* ISHELL_GetDC( CAF_HANDLE_T handle )
{
    return CAF_GET_SHELL()->GetDC( handle );
}

static __inline IAPPLET_T* ISHELL_GetAppletByWin( CAF_HANDLE_T win_handle ) 
{
    return CAF_GET_SHELL()->GetAppletByWin( win_handle );
}

static __inline IGUICTRL_T* ISHELL_GetCtrlByWin( CAF_HANDLE_T win_handle, uint32 ctrl_id ) 
{
    return CAF_GET_SHELL()->GetCtrlByWin( win_handle, ctrl_id );
}

static __inline BOOLEAN ISHELL_SetActiveCtrl( CAF_HANDLE_T ctrl_handle ) 
{
    return CAF_GET_SHELL()->SetActiveCtrl( ctrl_handle );
}

static __inline CAF_RESULT_E ISHELL_GetSystemDate( CAF_DATE_T* date_ptr ) 
{
    return CAF_GET_SHELL()->GetSystemDate( date_ptr );
}

static __inline CAF_RESULT_E ISHELL_GetSystemTime( CAF_TIME_T* time_ptr ) 
{
    return CAF_GET_SHELL()->GetSystemTime( time_ptr );
}

static __inline BOOLEAN ISHELL_GetTpPoint( void* param, CAF_POINT_T* point_ptr ) 
{
    return CAF_GET_SHELL()->GetTpPoint( param, point_ptr );
}

static __inline BOOLEAN ISHELL_PointIsInRect( const CAF_POINT_T* point_ptr, const CAF_RECT_T* rect_ptr ) 
{
    return CAF_GET_SHELL()->PointIsInRect( point_ptr, rect_ptr );
}


static __inline CAF_RESULT_E ISHELL_LoadImage( const wchar *  imagefile_ptr, IImage ** image_pptr)
{
	return CAF_GET_SHELL()->LoadImage( imagefile_ptr, image_pptr);
}

static __inline CAF_RESULT_E ISHELL_LoadRing(const wchar * ringfile_ptr, IMedia** media_pptr )
{
	return CAF_GET_SHELL()->LoadRing( ringfile_ptr, media_pptr);
}

static __inline CAF_RESULT_E ISHELL_LoadAnim(const wchar * animfile_ptr, IImage ** image_pptr)
{
	return CAF_GET_SHELL()->LoadAnim( animfile_ptr, image_pptr);
}

static __inline CAF_RESULT_E ISHELL_LoadResImage( uint32 image_id, CAF_HANDLE_T applet_handle, IImage** image_pptr )
{
    return CAF_GET_SHELL()->LoadResImage( image_id, applet_handle, image_pptr);
}

static __inline CAF_RESULT_E ISHELL_LoadResText( uint32 text_id, CAF_HANDLE_T applet_handle, CAF_STRING_T * str_ptr )
{
    return CAF_GET_SHELL()->LoadResText( text_id, applet_handle, str_ptr );
}

static __inline CAF_RESULT_E ISHELL_LoadResAnim( uint32 anim_id, CAF_HANDLE_T applet_handle, IImage** image_pptr  )
{
    return CAF_GET_SHELL()->LoadResAnim( anim_id, applet_handle, image_pptr );
}

static __inline CAF_RESULT_E ISHELL_LoadResRing( uint32 ring_id, uint32 ring_type, CAF_HANDLE_T applet_handle, IMedia** media_pptr )
{
    return CAF_GET_SHELL()->LoadResRing( ring_id,ring_type,applet_handle, media_pptr );
}

static __inline CAF_RESULT_E ISHELL_LoadResData(
	uint32 res_id, 
	CAF_RESOURCE_TYPE_E type ,
	CAF_HANDLE_T applet_handle,
	void ** buffer_pptr
)
{
    return CAF_GET_SHELL()->LoadResData( res_id, type,applet_handle, buffer_pptr );
}

static __inline CAF_RESULT_E ISHELL_StartApplet( const CAF_APPLET_START_T* start_ptr ) 
{
    return CAF_GET_SHELL()->StartApplet( start_ptr );
}

static __inline CAF_RESULT_E ISHELL_CloseApplet( CAF_HANDLE_T applet_handle ) 
{
    return CAF_GET_SHELL()->CloseApplet( applet_handle );
}

static __inline BOOLEAN ISHELL_PostMessage( CAF_HANDLE_T handle, CAF_MESSAGE_ID_E msg_id, void* param_ptr, uint32 size_of_param ) 
{
    return CAF_GET_SHELL()->PostMessage( handle, msg_id, param_ptr, size_of_param );
}

static __inline BOOLEAN ISHELL_SendMessage( CAF_HANDLE_T handle, CAF_MESSAGE_ID_E msg_id, void* param_ptr ) 
{
    return CAF_GET_SHELL()->SendMessage( handle, msg_id, param_ptr );
}

static __inline uint8 ISHELL_CreateTimer( CAF_HANDLE_T owner_handle, uint32 time_out, BOOLEAN is_period ) 
{
    return CAF_GET_SHELL()->CreateTimer( owner_handle, time_out, is_period );
}

static __inline BOOLEAN ISHELL_StartTimer( CAF_HANDLE_T owner_handle, uint8 timer_id, uint32 time_out, BOOLEAN is_period ) 
{
    return CAF_GET_SHELL()->StartTimer( owner_handle, timer_id, time_out, is_period );
}

static __inline BOOLEAN ISHELL_StopTimer( uint8 timer_id ) 
{
    return CAF_GET_SHELL()->StopTimer( timer_id );
}

static __inline BOOLEAN ISHELL_PauseTimer( uint8 timer_id ) 
{
    return CAF_GET_SHELL()->PauseTimer( timer_id );
}

static __inline BOOLEAN ISHELL_ResumeTimer( uint8 timer_id ) 
{
    return CAF_GET_SHELL()->ResumeTimer( timer_id );
}

static __inline BOOLEAN ISHELL_IsTimerActive( uint8 timer_id ) 
{
    return CAF_GET_SHELL()->IsTimerActive( timer_id );
}

static __inline void ISHELL_GetScreenInfo( CAF_SCREENINFO_T* info_ptr ) 
{
    CAF_GET_SHELL()->GetScreenInfo( info_ptr );
}

static __inline CAF_RESULT_E ISHELL_GetMemoryInfo( CAF_MEMORYINFO_T* info_ptr ,CAF_MEMORY_TYPE_E memory_type)
{
	return CAF_GET_SHELL()->GetMemoryInfo( info_ptr ,memory_type);
}


static __inline void ISHELL_MessageBox( const CAF_MESSAGEBOX_CREATE_T* msgbox_ptr ) 
{
    CAF_GET_SHELL()->MessageBox( msgbox_ptr );
}
static __inline void ISHELL_AllowTurnOffBackLight( BOOLEAN allow_turn_off ) 
{
    CAF_GET_SHELL()->AllowTurnOffBackLight( allow_turn_off );
}

static __inline CAF_SEMAPHORE_PTR ISHELL_CreateSemaphore( const char *name_ptr, uint32 initial_count )
{
	return CAF_GET_SHELL()->CreateSemaphore ( name_ptr, initial_count );
}
static __inline uint32 ISHELL_DeleteSemaphore ( CAF_SEMAPHORE_PTR sem_ptr )
{
	return CAF_GET_SHELL()->DeleteSemaphore ( sem_ptr );
}

static __inline uint32 ISHELL_GetSemaphore (CAF_SEMAPHORE_PTR sem_ptr, uint32 wait_option )
{
	return CAF_GET_SHELL()->GetSemaphore ( sem_ptr,wait_option );
}

static __inline uint32 ISHELL_PutSemaphore (CAF_SEMAPHORE_PTR sem_ptr )
{
	return CAF_GET_SHELL()->PutSemaphore ( sem_ptr );
}


//added by andrew.zhang at 2009/05/26
/*****************************************************************************/
//  Description : set window title
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN ISHELL_SetWinTitle( CAF_HANDLE_T handle, CAF_STRING_T * str_ptr, BOOLEAN is_need_update )
{
    return CAF_GET_SHELL()->SetWinTitle ( handle, str_ptr, is_need_update );
}

/*****************************************************************************/
//  Description : set window title by id
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN ISHELL_SetWinTitleByID( CAF_HANDLE_T handle, uint32 text_id, BOOLEAN is_need_update )
{
    return CAF_GET_SHELL()->SetWinTitleByID ( handle, text_id, is_need_update );
}

/*****************************************************************************/
//  Description : get applet install directory
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
static __inline BOOLEAN ISHELL_GetAppletInstallDirectory( CAF_HANDLE_T handle, wchar *path_ptr, uint16 *wstr_len_ptr )
{
    return CAF_GET_SHELL()->GetAppletInstallDirectory ( handle, path_ptr, wstr_len_ptr );
}

/*****************************************************************************/
//  Description : whether the window is focus
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
static __inline PUBLIC BOOLEAN ISHELL_IsFocusWindow( CAF_HANDLE_T handle )
{
    return CAF_GET_SHELL()->IsFocusWindow ( handle );
}


/*****************************************************************************/
//  Description :  create instance of interface from ishell interface.
//  Global resource dependence : none
//  Author: Andrew.Zhang
//  Note:
/*****************************************************************************/
static __inline PUBLIC BOOLEAN ISHELL_GetWinRect( CAF_HANDLE_T handle, CAF_RECT_T*    rect_ptr )
{
    return CAF_GET_SHELL()->GetWinRect ( handle, rect_ptr );
}

/*****************************************************************************/
//  Description : start an DLL by guid
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
static __inline PUBLIC CAF_RESULT_E ISHELL_CreateInstanceEx(CAF_GUID_T guid, void ** object_pptr, void * param_ptr)
{
    return CAF_GET_SHELL()->CreateInstanceEx( guid, object_pptr, param_ptr );
}

/*****************************************************************************/
//  Description :  close instance
//  Global resource dependence : none
//  Author: Andrew.Zhang
//  Note:
/*****************************************************************************/
static __inline PUBLIC CAF_RESULT_E ISHELL_CloseInstance(CAF_GUID_T guid, void * object_ptr)
{
    return CAF_GET_SHELL()->CloseInstance( guid, object_ptr );
}

/*****************************************************************************/
//  Description : send notify message to window
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
static __inline PUBLIC uint32 ISHELL_SendNotify( CAF_HANDLE_T ctrl_handle, CAF_MESSAGE_ID_E msg_id )
{
    return CAF_GET_SHELL()->SendNotify( ctrl_handle, msg_id );
}

/*****************************************************************************/
//  Description : post notify message to window
//  Global resource dependence : none
//  Author: andrew.zhang
//  Note: 
/*****************************************************************************/
static __inline PUBLIC uint32 ISHELL_PostNotify( CAF_HANDLE_T ctrl_handle, CAF_MESSAGE_ID_E msg_id )
{
    return CAF_GET_SHELL()->PostNotify( ctrl_handle, msg_id );
}

#else/* else CAF_DYANMIC_MODULE */
#include "cafstatichelp.h"
#endif/* endif CAF_DYANMIC_MODULE */

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/

static __inline CAF_RESULT_E IUNKNOWN_QueryInterface( IUNKNOWN_T *me, CAF_GUID_T guid, void ** object_pptr ) 
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IUNKNOWN_T )->QueryInterface( me, guid, object_pptr );
}

static __inline uint32 IUNKNOWN_AddRef( IUNKNOWN_T *me ) 
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IUNKNOWN_T )->AddRef( me );
}

static __inline uint32 IUNKNOWN_Release( IUNKNOWN_T *me ) 
{
    CAF_ASSERT( PNULL != me );
    return CAF_GET_VTBLPTR( me, IUNKNOWN_T )->Release( me );
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
