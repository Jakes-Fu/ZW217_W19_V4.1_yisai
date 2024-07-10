/*****************************************************************************
** File Name:      mmi_common.c                                              *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/2007        Jassmine              Creat
******************************************************************************/
#define MMI_COMMON_C


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_common_trc.h"
#include "os_api.h"
//#include "std_header.h"
#include "mmi_common.h"
#include "mmk_type.h"
//#include "guifont.h"
#include "cafstdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "mmk_modinfo.h"
#include "app_tcp_if.h"
#include "mmisrv_adapt.h"
//#include "guires.h"

#include "mmisrvfmm_export.h"


//#include "mmivcard_export.h"
//#include "gui_ucs2b_converter.h"
#include "os_api.h"
//#include "mmiim.h"

//#ifdef CSC_XML_SUPPORT
//#include "mmicus_export.h"
//#endif
#if defined(DRM_SUPPORT)
#include "mmisrvdrm.h"
//#include "mmidrm_export.h"
#endif
#include "mmk_kbd.h"
//#include "mmi_default.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MAX_GRAYED              50

#define MMICOM_SUFFIX_MAX_LEN   6
#define MMICOM_SUFFIX_DOT       0x002E  //后缀名的"."
#define MMI_RENAME_MAX_TIMES	5000
//write buffer size
#define WRITE_BMPBUFFER_SIZE       (2000 * 3)
#define WRITE_BMP_COUNT 4

#ifdef MMI_DEBUGLAYER_SUPPORT
#define MMI_DEBUGTEXT_BORDER_COLOR      0x00//MMI_WHITE_COLOR
#define MMI_DEBUGTEXT_BG_COLOR          0xffff//MMI_WHITE_COLOR
#define MMI_DEBUGTEXT_FONT_COLOR        0x00//MMI_BLACK_COLOR
#define MMI_DEBUGTEXT_FONT              SONG_FONT_16
#define MMI_DEBUGTEXT_MAX_LINE_NUM      3
#endif

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.
 */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a, b, c, d, x, s, ac) { \
 (a) += F ((b), (c), (d)) + (x) + (uint32)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define GG(a, b, c, d, x, s, ac) { \
 (a) += G ((b), (c), (d)) + (x) + (uint32)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define HH(a, b, c, d, x, s, ac) { \
 (a) += H ((b), (c), (d)) + (x) + (uint32)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define II(a, b, c, d, x, s, ac) { \
 (a) += I ((b), (c), (d)) + (x) + (uint32)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
typedef struct
{
    char *audio_type_str;
    MMISRVAUD_PRI_T pri;
}AUD_PRI_T;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMI_MENU_GRAYED_T s_mmi_menu_grayed_arr[MAX_GRAYED] = {0};   //grayed数组
#ifdef MMI_DEADLOOP_CHECK_ENABLE
LOCAL char *s_deadloop_file = PNULL;    //用来记录死循环之前最后执行到的文件（需要调接口函数print）
LOCAL uint32 s_deadloop_line = 0;       //用来记录死循环之前最后执行到的行号（需要调接口函数print）
LOCAL SCI_TIMER_PTR s_deadloop_check_timer = NULL;   //deadloop check timer
#endif

#ifdef MMI_DEBUGLAYER_SUPPORT
//调试图层
LOCAL GUI_LCD_DEV_INFO s_debug_layer = {0};
//调试字串输出区域
LOCAL const GUI_RECT_T s_debug_text_rect[MMI_DEBUGTEXT_MAX_LINE_NUM] = 
{
    {50, 100, 219, 130},
    {50, 135, 219, 165},
    {50, 170, 219, 200}
};
#endif


LOCAL const wchar* const s_file_default_path[MMIFMM_FILE_TYPE_NUM] =
{
    MMIMULTIM_OTHER_DEFAULT_DIR,                //not support file folder
    MMIMULTIM_DIR_PICTURE,//picture file folder
    MMIMULTIM_DIR_MUSIC,  //music file folder
    MMIMULTIM_DIR_MOVIE,  //movie fiel folder
    MMIMULTIM_DIR_EBOOK,    //ebook file folder
#if defined MMI_VCARD_SUPPORT
    MMIMULTIM_DIR_VCARD,    //vcard file folder
#endif
#ifdef MMI_VCALENDAR_SUPPORT
    MMIMULTIM_DIR_VCALENDAR,
#endif
    MMIMULTIM_OTHER_DEFAULT_DIR,                
	MMIMULTIM_DIR_JAVA, //
    PNULL,
	PNULL
};/*lint !e785 */

//uint32 systemColorDepth = 16;
LOCAL const  uint32 systemRedShift = 11;
LOCAL const  uint32 systemGreenShift = 6;
LOCAL const  uint32 systemBlueShift = 0;

LOCAL const  uint32 color32systemRedShift = 16;
LOCAL const  uint32 color32systemGreenShift = 8;
LOCAL const  uint32 color32systemBlueShift = 0;
//bmp header
struct {
  uint8 ident[2];
  uint8 filesize[4];
  uint8 reserved[4];
  uint8 dataoffset[4];
  uint8 headersize[4];
  uint8 width[4];
  uint8 height[4];
  uint8 planes[2];
  uint8 bitsperpixel[2];
  uint8 compression[4];
  uint8 datasize[4];
  uint8 hres[4];
  uint8 vres[4];
  uint8 colors[4];
  uint8 importantcolors[4];
  //    u8 pad[2];
} BMPheader;

/* The order is from high to low, 
    please pay attention that:
        HIGH - N should bigger than NORMAL + M, and
        NORMAl - N should bigger than LOW + M
*/
LOCAL const AUD_PRI_T s_aud_pri_table[]=
{
    /*----------- HIGH ------------------------*/
    {"MT RING",      MMISRVAUD_PRI_HIGH},
    {"VOICE",         MMISRVAUD_PRI_HIGH},
    

    /*----------- NORMAL ------------------------*/
    {"PLAYER",       MMISRVAUD_PRI_NORMAL},


    /*----------- LOW ------------------------*/
    {"TTS",            MMISRVAUD_PRI_LOW + 10},
    {"KEY TONE",    MMISRVAUD_PRI_LOW},


    /* Default pri is NORMAL */
    {"DEFAULT",    MMISRVAUD_PRI_NORMAL},
    
    {PNULL,           0}
};
LOCAL unsigned char PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef MMI_DEADLOOP_CHECK_ENABLE
/**********************************************************************
//    Description:
//      the funciton to proccess the mmi deadloop timer expire
//    Global resource dependence :
//    Author: bin.ji
//    Note:
***********************************************************************/
LOCAL void DeadloopCheckTimerExpired(
                                uint32 lparam   //the function's input parameter
                                );
#endif

/*****************************************************************************/
//  Description : add menu grayed
//  Global resource dependence : 
//  Author: jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddGrayed(
                        MMI_WIN_ID_T            win_id,
                        MMI_MENU_GROUP_ID_T     group_id,
                        MMI_MENU_ID_T           menu_id
                        );
                
/*****************************************************************************/
//  Description : remove menu grayed
//  Global resource dependence : 
//  Author: jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RemoveGrayed(
                           MMI_WIN_ID_T         win_id,
                           MMI_MENU_GROUP_ID_T  group_id,
                           MMI_MENU_ID_T        menu_id
                           );
#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
//  Description : caf malloc
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void* CAFMalloc( 
                      uint32 size,
                      const char *file,
                      uint32 line
                      );

/*****************************************************************************/
//  Description : caf free
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void CAFFree(
                   void* mem_ptr
                   );

/*****************************************************************************/
//  Description : htonl
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFHtonl( uint32 host_long );

/*****************************************************************************/
//  Description : htons
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint16 CAFHtons( uint16 host_short );

/*****************************************************************************/
//  Description : htonl
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFNtohl( uint32 net_long );

/*****************************************************************************/
//  Description : htons
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint16 CAFNtohs( uint16 net_short );

/*****************************************************************************/
//  Description : inet_aton
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL int CAFINetAton( 
                      const char *cp,
                      uint32* pin
                      );

/*****************************************************************************/
//  Description : inet_ntoa
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL char * CAFINetNtoa(uint32 pin);

/*****************************************************************************/
// 	Description : encapsulation of wstr to gb
//	Global resource dependence : none
//  Author: wei.zhou
//	Note: 
/*****************************************************************************/
LOCAL uint16 MMI_ConvertUCS2GB( 
                                const wchar *ucs_ptr, 
                                uint16 ucslength, 
                                uint8 *gb_ptr
                                );

/*****************************************************************************/
// 	Description : encapsulation of gb to wstr
//	Global resource dependence : none
//  Author: wei.zhou
//	Note: 
/*****************************************************************************/
LOCAL uint16 MMI_ConvertGB2UCS(
                                const uint8 *gb_ptr, 
                                uint16 length, 
                                wchar *ucs_ptr
                                );

/*****************************************************************************/
// 	Description : encapsulation wstr to UTF8
//	Global resource dependence : 
//  Author: wei.zhou
//	Note:
/*****************************************************************************/
LOCAL uint16 MMI_ConvertUCS2B2UTF8( const wchar *ucs2b_ptr, 
                                    uint16 ucs2b_len, 
                                    uint8 *utf8_ptr, 
                                    uint16 utf8_buf_len
                                    );

/*****************************************************************************/
// 	Description : encapsulation UTF8 to wstr 
//	Global resource dependence : 
//  Author: wei.zhou
//	Note:
/*****************************************************************************/
LOCAL uint16 MMI_ConvertUTF82UCS2B( const uint8 *utf8_ptr, 
                                    uint16 utf8_len, 
                                    wchar *ucs2b_ptr, 
                                    uint16 ucs2b_buf_len
                                    );

/******************************************************************************/
/*! \brief convert ucs big endian to ucs little endian
 *  \param[out]		ucs2l_ptr		ucs2 little endian string pointer
 *  \param[in]		ucs2l_buf_len	ucs2 little endian string buffer len
 *  \param[in]		ucs2b_ptr		ucs2 big endian string pongiter
 *  \param[in]		ucs2b_len		ucs2 big endian string len
 *  \author  wei.zhou
 *  \return  uint16
 */
/******************************************************************************/
LOCAL uint16 MMI_ConvertUCS2BE2UCS2LE( const uint8 *ucs2b_ptr, 
                                       uint16 ucs2b_len, 
                                       uint8 *ucs2l_ptr, 
                                       uint16 ucs2l_buf_len 
                                       );

/******************************************************************************/
/*! \brief convert ucs little endian to ucs big endian
 *  \param[out]		ucs2b_ptr		ucs2 big endian string pointer
 *  \param[in]		ucs2b_buf_len	ucs2 big endian string buffer len
 *  \param[in]		ucs2l_ptr		ucs2 little endian string pongiter
 *  \param[in]		ucs2l_len		ucs2 little endian string len
 *  \author  wei.zhou
 *  \return  uint16
 */
/******************************************************************************/
LOCAL uint16 MMI_ConvertUCS2LE2UCS2BE( const uint8 *ucs2l_ptr, 
                                       uint16 ucs2l_len, 
                                       uint8 *ucs2b_ptr, 
                                       uint16 ucs2b_buf_len 
                                       );
#endif
/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
#ifdef DYNAMIC_MODULE_SUPPORT
/* for dynamic module */
const CAF_STDLIB_T g_caf_stdlib_vtbl = 
{
   //ARM string.h
   memcpy,
   memmove,
   strcpy,
   strncpy,
   strcat,
   strncat,
   memcmp,
   strcmp,
   strncmp,
   strcoll,
   strxfrm,
   memchr,
   strchr,
   strcspn,
   strpbrk,
   strrchr,
   strspn,
   strstr,
   strtok,
   memset,
   strerror,
   strlen,

   //ARM ctype.h
   isalnum, /*lint !e123*/
   isalpha, /*lint !e123*/
   iscntrl, /*lint !e123*/
   isdigit, /*lint !e123*/
   isgraph, /*lint !e123*/
   islower, /*lint !e123*/
   isprint, /*lint !e123*/
   ispunct, /*lint !e123*/
   isspace, /*lint !e123*/
   isupper, /*lint !e123*/
   isxdigit,/*lint !e123*/
   tolower,
   toupper,

   //ARM stdlib.h
   atoi,
   atol,
   strtol,
   strtoul,
   rand,
   srand,
   qsort,
   abs,
    
    //ARM stdio.h
   sprintf,
   vsprintf,

   SCI_Malloc,
   SCI_Free, /*lint !e64*/

   SCI_Assert,  /*assert verified*/
#ifdef TRACE_INFO_SUPPORT   
   #ifdef  WIN32
   (uint32)   SCI_Trace, /*lint !e64*/
   #else
#if defined TRACE_INFO_SUPPORT
   SCI_TraceLow,
#else
   PNULL,
#endif
   #endif
#else
   PNULL,
#endif

   SCI_Sleep,

   //wstring
   MMIAPICOM_Wstrcpy,
   MMIAPICOM_Wstrncpy,
   MMIAPICOM_Wstrcat,
   MMIAPICOM_Wstrncat,
   MMIAPICOM_Wstrcmp,
   MMIAPICOM_Wstrncmp,
   MMIAPICOM_Wstrlen,

   MMIAPICOM_StrToWstr,/*lint !e64*/
   MMIAPICOM_WstrToStr,/*lint !e64*/
   MMIAPICOM_Strlower,
   MMIAPICOM_Strupper,
   MMIAPICOM_Wstrlower,
   MMIAPICOM_Wstrupper,

   //socket
   CAFHtonl,
   CAFHtons,
   CAFNtohl,
   CAFNtohs,
   CAFINetAton,
   CAFINetNtoa,
   SCI_GetTickCount,
   MMI_ConvertUCS2GB,
   MMI_ConvertGB2UCS,
   MMI_ConvertUCS2B2UTF8,
   MMI_ConvertUTF82UCS2B,
   MMI_ConvertUCS2BE2UCS2LE,
   MMI_ConvertUCS2LE2UCS2BE
};
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : set menu gray
//  Global resource dependence : 
//  Author: jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_EnableGrayed(
                                      MMI_WIN_ID_T          win_id,
                                      MMI_MENU_GROUP_ID_T   group_id,
                                      MMI_MENU_ID_T         menu_id,
                                      BOOLEAN               is_gray //TRUE:变灰
                                      )
{
    BOOLEAN     result = FALSE;

    win_id = MMK_ConvertIdToHandle( win_id );
    
    if (is_gray)
    {
        result = AddGrayed(win_id,group_id,menu_id);
    }
    else
    {
        result = RemoveGrayed(win_id,group_id,menu_id);
    }

    return (result);
}

/*****************************************************************************/
//  Description : add menu grayed
//  Global resource dependence : 
//  Author: jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AddGrayed(
                        MMI_WIN_ID_T            win_id,
                        MMI_MENU_GROUP_ID_T     group_id,
                        MMI_MENU_ID_T           menu_id
                        )
{
    BOOLEAN     result = FALSE;
    uint32      i = 0;

    //remove
    RemoveGrayed(win_id,group_id,menu_id);

    //add
    for (i=0; i<MAX_GRAYED; i++)
    {
        if (!s_mmi_menu_grayed_arr[i].is_valid)
        {
            s_mmi_menu_grayed_arr[i].is_valid   = TRUE;
            s_mmi_menu_grayed_arr[i].win_id     = win_id;
            s_mmi_menu_grayed_arr[i].group_id   = group_id;
            s_mmi_menu_grayed_arr[i].menu_id    = menu_id;

            result = TRUE;
            break;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : remove menu grayed
//  Global resource dependence : 
//  Author: jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RemoveGrayed(
                           MMI_WIN_ID_T         win_id,
                           MMI_MENU_GROUP_ID_T  group_id,
                           MMI_MENU_ID_T        menu_id
                           )
{
    BOOLEAN     result = TRUE;
    uint32      i = 0;

    for (i=0; i<MAX_GRAYED; i++)
    {
        if ((s_mmi_menu_grayed_arr[i].win_id == win_id) &&
            (s_mmi_menu_grayed_arr[i].group_id == group_id) &&
            (s_mmi_menu_grayed_arr[i].menu_id == menu_id))
        {
            SCI_MEMSET(&s_mmi_menu_grayed_arr[i],0,sizeof(MMI_MENU_GRAYED_T));
            break;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : menu is grayed
//  Global resource dependence : 
//  Author: jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsGrayed(
                                  MMI_WIN_ID_T          win_id,
                                  MMI_MENU_GROUP_ID_T   group_id,
                                  MMI_MENU_ID_T         menu_id
                                  )
{
    BOOLEAN     result = FALSE;
    uint32      i = 0;

    win_id = MMK_ConvertIdToHandle( win_id );
    
    for (i=0; i<MAX_GRAYED; i++)
    {
        if ((s_mmi_menu_grayed_arr[i].win_id == win_id) &&
            (s_mmi_menu_grayed_arr[i].group_id == group_id) &&
            (s_mmi_menu_grayed_arr[i].menu_id == menu_id))
        {
            result = s_mmi_menu_grayed_arr[i].is_valid;
            break;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : free all menu grayed by window id
//  Global resource dependence : 
//  Author: jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_FreeGrayed(
                                 MMI_WIN_ID_T    win_id
                                 )
{
    uint32      i = 0;
    
    win_id = MMK_ConvertIdToHandle( win_id );

    for (i=0; i<MAX_GRAYED; i++)
    {
        if (s_mmi_menu_grayed_arr[i].win_id == win_id)
        {
            SCI_MEMSET(&s_mmi_menu_grayed_arr[i],0,sizeof(MMI_MENU_GRAYED_T));
        }
    }
}

/*****************************************************************************/
//  Description : Delete all fils in dir and dir
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_DelDirAll(
                                   const wchar* path_ptr, //dir path
                                   uint32        path_buf_len //Buffer len  
                                   )

{
    wchar               star_char = '*';
    wchar               *path_star_ptr = NULL;
    wchar               *path_full_ptr = NULL;
    uint16              path_len = 0;
    SFS_FIND_DATA_T     find_data = {0};
    SFS_HANDLE          sfs_handle = 0;

    path_len = MMIAPICOM_Wstrlen( path_ptr );
    if ((path_len + 1)>=path_buf_len)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_DelDirAll path_len =%d,path_buf_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_597_112_2_18_1_58_10_0,(uint8*)"dd",path_len,path_buf_len);
        return FALSE;
    }
    if (path_len >= CAF_FILENAME_MAX_LEN)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_DelDirAll path_len =%d,"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_605_112_2_18_1_58_11_1,(uint8*)"d",path_len);
        return FALSE;
    }

    path_star_ptr = (wchar *)SCI_ALLOCA( ( CAF_FILENAME_MAX_LEN + 1 ) * sizeof(wchar) );
    if (PNULL == path_star_ptr)
    {
        return FALSE;
    }

    SCI_MEMSET( path_star_ptr, 0, (( CAF_FILENAME_MAX_LEN + 1 ) * sizeof(wchar)) );

    MMIAPICOM_CombinePath(path_ptr, 
                          MMIAPICOM_Wstrlen(path_ptr),
                          &star_char, 
                          1, 
                          FALSE,
                          path_star_ptr, 
                          &path_len);   

    sfs_handle = MMIAPIFMM_FindFirstFile(path_star_ptr,path_len, &find_data);

    do 
    {
        if (0 != sfs_handle)    
        {   
            if (MMIAPICOM_Wstrlen(path_ptr) + MMIAPICOM_Wstrlen(find_data.name)>=CAF_FILENAME_MAX_LEN)
            {
                SFS_FindClose(sfs_handle);
                SCI_FREE(path_star_ptr);
                return FALSE;
            }  
            path_full_ptr = (wchar *)SCI_ALLOCA( ( CAF_FILENAME_MAX_LEN + 1 ) * sizeof(wchar) );
            
            if (PNULL == path_full_ptr)
            {
                SFS_FindClose(sfs_handle);
                SCI_FREE(path_star_ptr);
                return FALSE;
            }
            SCI_MEMSET(path_full_ptr, 0, (( CAF_FILENAME_MAX_LEN + 1 ) * sizeof(wchar)) );
            MMIAPICOM_CombinePath(path_ptr, 
                                  MMIAPICOM_Wstrlen(path_ptr),
                                  find_data.name, 
                                  MMIAPICOM_Wstrlen(find_data.name), 
                                  FALSE,
                                  path_full_ptr, 
                                  &path_len);   
            if (find_data.attr & SFS_ATTR_DIR)
            {
                /*发现目录，递归删除*/
                MMIAPICOM_DelDirAll(path_full_ptr, SFS_MAX_PATH);
            }
            else
            {
                /*删除文件*/                                
                SFS_DeleteFile(path_full_ptr, NULL);
            }
            SCI_FREE(path_full_ptr);
        }
    } while(SFS_ERROR_NONE == SFS_FindNextFile(sfs_handle, &find_data));

    /*del the dir self*/
    SFS_DeleteDirectory(path_ptr);
    SFS_FindClose(sfs_handle);
    SCI_FREE(path_star_ptr);
    return TRUE;
}
/*****************************************************************************/
//  Description : for telenumber to translate the string to bcd
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_GenPartyNumber(
                                        uint8               *tele_num_ptr,
                                        int16               tele_len,
                                        MMI_PARTY_NUMBER_T *party_num
                                        )
{    
    uint8   offset   = 0;
    uint8*  tele_num = PNULL;

    //check the param
    if (PNULL == tele_num_ptr || PNULL == party_num)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_GenPartyNumber PNULL == tele_num_ptr || PNULL == party_num"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_674_112_2_18_1_58_11_2,(uint8*)"");
        return FALSE;
    }
    if (0 == tele_len)
    {
        // An empty string is given. The length of the BCD number is set to 0
        party_num->num_len = 0;
        //SCI_TRACE_LOW:"MMI_GenPartyNumber  length = 0\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_681_112_2_18_1_58_11_3,(uint8*)"");
        return FALSE;
    }
    
    // @Great.Tian, CR12051, begin
    tele_len = MIN(MMIPB_NUMBER_MAX_STRING_LEN, tele_len);
    tele_num = (uint8*)SCI_ALLOC_APP(tele_len + 1);
    SCI_MEMSET(tele_num, 0, (tele_len + 1));
    // SCI_MEMCPY(tele_num, tele_num_ptr, tele_len);
    MMI_MEMCPY(
        tele_num, 
        tele_len + 1,
        tele_num_ptr,
        tele_len,
        tele_len);
    // @Great.Tian, CR12051, end
    
    if('+' == tele_num[0])
    {
        party_num->number_type = MN_NUM_TYPE_INTERNATIONAL;
        offset++;
    }
    else
    {
        party_num->number_type = MN_NUM_TYPE_UNKNOW;
    }
    
    party_num->num_len = ( tele_len - offset + 1 ) /2;
    SCI_ASSERT( party_num->num_len <=  MN_MAX_ADDR_BCD_LEN);/*assert verified*/
    MMIAPICOM_StrToBcd( PACKED_LSB_FIRST,
                (char*)( tele_num + offset), 
                party_num->bcd_num );

    // @Great.Tian, CR12051, begin
    SCI_FREE(tele_num);
    // @Great.Tian, CR12051, end

    return TRUE;

}

/*****************************************************************************/
//  Description : for telenumber to translate the string to bcd
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_ConvertWStr2Num(
                                        wchar *wnum_str,
										uint16 wstr_len,
                                        MMIPB_BCD_NUMBER_T *bcdnum_ptr
                                        )
{    
	uint8   bcdCode = 0;
	uint16  offset = 0;
	uint16  i = 0;
	uint16  bcd_index = 0;
    BOOLEAN result = FALSE;
	MN_NUMBER_TYPE_E number_type = MN_NUM_TYPE_UNKNOW;
    //check the param
    if (PNULL == wnum_str || PNULL == bcdnum_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_ConvertWStr2Num PNULL == wnum_str || PNULL == bcdnum_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_741_112_2_18_1_58_11_4,(uint8*)"");
        return FALSE;
    }
    if (0 == wstr_len)
    {
        // An empty string is given. The length of the BCD number is set to 0
        bcdnum_ptr->number_len = 0;
		bcdnum_ptr->npi_ton = 0;
        //SCI_TRACE_LOW:"MMI_GenPartyNumber  length = 0\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_749_112_2_18_1_58_11_5,(uint8*)"");
        return FALSE;
    }
    
    // SCI_MEMCPY(tele_num, tele_num_ptr, tele_len);
    
    if('+' == wnum_str[0])
    {
        number_type = MN_NUM_TYPE_INTERNATIONAL;
        offset++;
    }
	bcdnum_ptr->npi_ton = MMIAPICOM_ChangeTypePlanToUint8(
        number_type,
        0
        );

    /*memset(pBcd,0xFF,(strLen&1) ? (strLen+1)/2 : strLen/2+1);*/
	bcdnum_ptr->number_len = ( wstr_len - offset + 1 ) /2;
    for(i=offset;i < wstr_len;i++)
    {
        /*bcdCode = 0xFF;*/

        if(!((wnum_str[i] >= '0' && wnum_str[i] <= '9')||wnum_str[i] == '*'||
              wnum_str[i] == '#'||(wnum_str[i] == 'P'||wnum_str[i] == 'p')||
              wnum_str[i] == 'W'||wnum_str[i] == 'w'||wnum_str[i]=='+'))
		{
			result = FALSE;
			break;
		}
		bcdCode = (uint8)wnum_str[i];
        switch(bcdCode)
        {
            case '*':
                bcdCode = DIALBCD_STAR;
                break;
            case '#':
                bcdCode = DIALBCD_HASH;
                break;
            case 'P':
            case 'p':
                bcdCode = DIALBCD_PAUSE;
                break;
            case 'W':
            case 'w':
                bcdCode = DIALBCD_WILD;
                break;
            case '+':
		        bcdCode=DIALBCD_EXPANSION;
		        break;
            default:
                bcdCode = bcdCode - '0';
                break;
        }
        bcdnum_ptr->number[bcd_index/2] = ((bcd_index & 1) ? bcdnum_ptr->number[bcd_index/2] : 0) +  (bcdCode << ((bcd_index & 1) ? 4 : 0));
        bcd_index++;
		result = TRUE;
    }
    if(bcd_index&1)
    {
       bcdnum_ptr->number[bcd_index/2] |= 0xf0;
    }
    return result;

}

/*****************************************************************************/
//  Description : convert bcd number to wchar
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_BcdToWchar(
                                    wchar *wnum_str, //[OUT]
                                    uint16 *wstr_len_ptr,//[OUT]
                                    uint16 max_wstr_len,//[IN]
                                    MMIPB_BCD_NUMBER_T *bcdnum_ptr//[IN]                                                                        
                                    )
{
    uint8  strbuf[(MMIPB_BCD_NUMBER_MAX_LEN << 1) + 2] = {0};
    uint16 num_len = 0;

    if(wnum_str == PNULL || bcdnum_ptr == PNULL)
    {
        return FALSE;
    }

    num_len = MMIAPICOM_GenDispNumber(
        MMIPB_GetNumberTypeFromUint8(bcdnum_ptr->npi_ton), 
        bcdnum_ptr->number_len, 
        bcdnum_ptr->number,
        strbuf,
        (MMIPB_BCD_NUMBER_MAX_LEN << 1) + 2
        );
    MMI_STRNTOWSTR(wnum_str, max_wstr_len, strbuf, max_wstr_len, MIN(num_len, max_wstr_len));
    if(wstr_len_ptr != PNULL)
    {
        *wstr_len_ptr = MIN(num_len, max_wstr_len);
    }
    return TRUE;
}
/*****************************************************************************/
//  Description : Generate to the string number
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_GenDispNumber(
                                     MN_NUMBER_TYPE_E  number_type,
                                     uint8             party_len,
                                     uint8             *party_num,
                                     uint8             *tele_num,
                                     uint16            max_tele_len//!!!!!!!!!可显示的号码的最大长度 + 2(/0 字符串结束符号)
                                     )
{
    uint8   offset = 0;
    uint8   len = 0;
	//mdy by qing.yu@for cr238786
    if (PNULL == tele_num || PNULL == party_num || max_tele_len<=2 || party_len == 0)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_GenDispNumber max_tele_len = %d, party_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_865_112_2_18_1_58_11_6,(uint8*)"dd",max_tele_len, party_len);
        return 0;
    }
    if(MN_NUM_TYPE_ALPHANUMERIC == number_type)
    {
        // because the system does not support default character, the number need convert
        len = MIN(max_tele_len, party_len);
        len = MMIAPICOM_Default2Ascii(
                                party_num, 
                                tele_num,
                                len
                                );
        return (len);
    }

    //check the tele_num is internation tele
    if( MN_NUM_TYPE_INTERNATIONAL == number_type )
    {
        offset = 1;
        tele_num[0] = '+';
    }

    
    len = party_len << 1;
    
    if (len <= (max_tele_len - offset - 1))
    {
        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST ,party_num, len, (char*)( tele_num + offset ) );
    }
    else
    {
        return 0;
    }
    //SCI_PASSERT(len <= (max_tele_len - offset - 1) ,(("len = %d ,offset =%d"),len,offset)); /*assert to do*/
    
    return (uint8)strlen((char*)tele_num);
}

/*****************************************************************************/
//  Description : Generate to the string number
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICOM_GenNetDispNumber(
                                     MN_NUMBER_TYPE_E  number_type,
                                     uint8             party_len,
                                     uint8             *party_num,
                                     uint8              *tele_num,
                                     uint8              max_tele_len//!!!!!!!!!可显示的号码的最大长度 + 2(/0 字符串结束符号)
                                     )
{
    uint8   offset = 0;
    uint8   len = 0;

    if (PNULL == tele_num || PNULL == party_num || max_tele_len <= 2 )
    {
        //SCI_TRACE_LOW:"MMIAPICOM_GenDispNumber max_tele_len = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_920_112_2_18_1_58_11_7,(uint8*)"d",max_tele_len);
        return 0;
    }
    
    if(MN_NUM_TYPE_ALPHANUMERIC == number_type)
    {
        // because the system does not support default character, the number need convert
        len = MIN(max_tele_len, party_len);
        len = MMIAPICOM_Default2Ascii(
                                party_num, 
                                tele_num,
                                len
                                );
        return (len);
    }

    //check the tele_num is internation tele
    if( MN_NUM_TYPE_INTERNATIONAL == number_type )
    {
        offset = 1;
        tele_num[0] = '+';
    }

    if( 0 == party_len )
    {
        //SCI_TRACE_LOW:"MMI_GenDispNumber: called_num->num_len = 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_945_112_2_18_1_58_11_8,(uint8*)"");
        return (offset);
    }
    
    len = party_len << 1;
    if (len <= (max_tele_len - offset - 1))
    {
        MMIAPICOM_NetBcdToStr(PACKED_LSB_FIRST ,party_num, len, (char*)( tele_num + offset ) );
    }
    else
    {
        return 0;
    }
    //SCI_PASSERT(len <= (max_tele_len - offset - 1) ,(("len = %d ,offset =%d"),len,offset)); /*assert to do*/

    
    return (uint8)strlen((char*)tele_num);
}


/*****************************************************************************/
//  Description :change enum type and plan of number to uint8 npi_ton,请参考PB
//  Global resource dependence : none
//  Author: jassmine.meng
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint8 MMIAPICOM_ChangeTypePlanToUint8(
                                MN_NUMBER_TYPE_E    number_type,
                                MN_NUMBER_PLAN_E    number_plan
                                )
{
    uint8   npi_ton = 0,num_type = 0,num_plan = 0;

    num_type = (uint8)((number_type & 0x000F));
    num_plan = (uint8)((number_plan & 0x000F));

    npi_ton = (num_type << 4) + num_plan;

    return (npi_ton);
}

/*****************************************************************************/
//  Description : get nosuffix_name and suffix
//  Global resource dependence :                                
//      Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_SplitSuffix(
                                  wchar   *nosuffix_name_ptr,    //out
                                  uint16    wlen,
                                  wchar    *suffix_ptr,           //out
                                  uint16    clen,
                                  wchar   *addsuffix_name_ptr    //in
                                  )
{
    uint32 name_length = 0;
    uint32 i = 0;

    if (PNULL == addsuffix_name_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_SplitSuffix PNULL == addsuffix_name_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1002_112_2_18_1_58_11_9,(uint8*)"");
        return;
    }
    name_length = MMIAPICOM_Wstrlen(addsuffix_name_ptr);
    for (i = name_length; i > 0; i--)
    {
// #ifdef _WIN32
//         if (0x2e00 == addsuffix_name_ptr[i])  //'.' == total_name_ptr[i]
// #else
//         if (0x002e == addsuffix_name_ptr[i])  //'.' == total_name_ptr[i]
// #endif
        if ( MMICOM_SUFFIX_DOT == addsuffix_name_ptr[i])  //'.' == total_name_ptr[i]
        {
            break;
        }
    }
    
    //get nosuffix-filename
    if (PNULL != nosuffix_name_ptr)
    {
        MMI_WSTRNCPY(
            nosuffix_name_ptr,
            i,
            addsuffix_name_ptr,
            i,
            wlen);
        nosuffix_name_ptr[i] = 0x00;
    }
    
    //get suffix
    if (PNULL != suffix_ptr&& clen>=(name_length-i))
    {
        MMIAPICOM_Wstrncpy( suffix_ptr, &addsuffix_name_ptr[i+1], name_length-i );

        //MMIAPICOM_Wchar2char(suffix_ptr, &addsuffix_name_ptr[i+1]);
    }
}

/******************************************************************************/
/*! \brief get the invalid ctrl id
 *  \author jibin 
 *  \return invalid ctrl id
 */
/******************************************************************************/
PUBLIC MMI_CTRL_ID_T MMIAPICOM_GetInvalidCtrlId(void)
{
    return 0xffffffff;
}

/******************************************************************************/
/*! \brief get the invalid window id
 *  \author jibin 
 *  \return invalid window id
 */
/******************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPICOM_GetInvalidWinId(void)
{
    return 0xffffffff;
}

/*****************************************************************************/
//  Description : check the string is phone number or not
//  Global resource dependence : none
//  Return : TRUE: the string is phone number; FALSE: the string is not phone number
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsPhoneNumber(
                             uint8              *tele_num_ptr,
                             int16              tele_len
                             )
{
    int i = 0;
    uint32 ascCode = 0;
    BOOLEAN ret = TRUE;

    if(PNULL != tele_num_ptr && (tele_len > 0))
    {
        for(i=0;i < tele_len;i++)
        {
            ascCode = tele_num_ptr[i];

            if(!((ascCode >= '0' && ascCode <= '9')||ascCode == '*'||
                  ascCode == '#'||(ascCode == 'P'||ascCode == 'p')||
                  ascCode == 'W'||ascCode == 'w' || ascCode == '+'))
            {
                //not phone number
                ret = FALSE;
                break;
            }
        }
    }
    else
    {
        //string is null
        ret = FALSE;
    }
    //SCI_TRACE_LOW:"MMIAPICOM_IsPhoneNumber ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1093_112_2_18_1_58_11_10,(uint8*)"d",ret);
    return ret;
}

/*****************************************************************************/
//  Description : 其他task向mmi发送消息
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:使用MMK_PostMsg和MMIAPICOM_TriggerMMITask存在风险, 用此函数代替
/*****************************************************************************/
PUBLIC void MMIAPICOM_OtherTaskToMMI( 
                                     MMI_HANDLE_T     handle,
                                     MMI_MESSAGE_ID_E msg_id,
                                     DPARAM           param_ptr,
                                     uint32           size_of_param
                                     )
{
    MmiOtherTaskMsgS *sendSignal = PNULL;

    MmiCreateSignal(APP_OTHER_TASK_TO_APP, sizeof(MmiOtherTaskMsgS), (MmiSignalS**)&sendSignal);

    sendSignal->Sender = P_APP;

    sendSignal->handle         = handle;
    sendSignal->msg_id         = msg_id;
    sendSignal->size_of_param  = size_of_param;

    if( PNULL != param_ptr )
    {
        sendSignal->param_ptr = SCI_ALLOC_APP(size_of_param);
        
        MMI_MEMCPY( 
            sendSignal->param_ptr,
            size_of_param, 
            param_ptr,
            size_of_param,
            size_of_param);
    }

    MmiSendSignal(P_APP, (MmiSignalS*)sendSignal);
}

/*****************************************************************************/
//  Description : trigger the mmi task
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_TriggerMMITask(void)
{
    // 触发MMI Task，即向MMI Task发送一个无效消息(MMI不会处理)
    MmiSignalS         *sendSignal;
    MmiCreateSignal(0xFFFF, sizeof(MmiSignalS), (MmiSignalS**)&sendSignal);

    sendSignal->Sender = P_APP;
    MmiSendSignal(TASK_FL_ID, (MmiSignalS*)sendSignal);
}


/******************************************************************************
 * 函数定义:    MMIAPICOM_IsPointValid 判断一个point是否有效
 * 函数说明：   
 * 参数说明：   坐标点
 * 返回值：     BOOLEAN
 * NOTE:        xiaoqing.lu
********************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsPointValid(GUI_POINT_T point)
{
    BOOLEAN is_point_valid = TRUE;

    if (point.x < 0 || point.y < 0)
    {
        is_point_valid = FALSE;
    }

    return is_point_valid;
}


/*****************************************************************************/
//  Description : trigger the mmi task
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMI_TriggerMMITask(void)
{
    MMIAPICOM_TriggerMMITask();
}

/*****************************************************************************/
//  Description : get lcd device infomation 
//  Global resource dependence : 
//  Author: 
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPICOM_GetLcdDevInfo(GUI_LCD_DEV_INFO *lcd_dev_info)
{
    if (PNULL==lcd_dev_info)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_GetLcdDevInfo PNULL==lcd_dev_info"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1184_112_2_18_1_58_12_11,(uint8*)"");
        return ;
    }
    lcd_dev_info->lcd_id = LCD_ID_0/*GUI_MAIN_LCD_ID*/;
    lcd_dev_info->block_id = 0/*GUI_BLOCK_MAIN*/;
}

/*****************************************************************************/
//  Description : display backgroud of input screen for time inoput of ALARM
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPICOM_DisplayBackground(
                               MMI_WIN_ID_T    win_id, 
                               uint32          x,
                               uint32          y,
                               uint32          icon_id
                               )
{
    GUI_POINT_T         dis_point = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0,0};//{GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    //draw background
    dis_point.x = x;
    dis_point.y = y;
    GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            icon_id,
            &lcd_dev_info);
}

/*****************************************************************************/
//  Discription:combine full path name by file name with suffix 
//  Global resource dependence: none 
//  Parameter : filedev: file system staroge
//              file_name_ptr: file name, if this file name include "/" or "\\", the full_path_ptr will return file_name_ptr
//                             if  this file name not include "/", the full_path_ptr will be combine by this file type
//              file_name_len: file name length
//              full_path_ptr : file full path name, it will return to user
//              full_path_max_len: file full path name max. length, if the combined file full path name will be limited full_path_max_len  
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPICOM_CombineDefaultFullPathByFilename(    
                                                MMIFILE_DEVICE_E filedev,            //IN
                                                const wchar     *file_name_ptr,  //IN
                                                uint16           file_name_len,  //IN
                                                wchar *full_path_ptr,            //OUT
                                                uint16 full_path_max_len         //IN
                                                )
{
    MMIFMM_FILE_TYPE_E file_type = MMIFMM_FILE_TYPE_NORMAL;
    wchar  suffix_name[MMICOM_SUFFIX_MAX_LEN + 1] = {0};
    wchar  *file_dev_ptr = PNULL;
    uint16 file_dev_len = 0;

     if(PNULL == file_name_ptr || PNULL == full_path_ptr || 0 == file_name_len)
     {
         //SCI_TRACE_LOW:"MMIAPICOM_GetFullPathByFilename invalid !"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1242_112_2_18_1_58_12_12,(uint8*)"");
     }
     else if(MMIAPICOM_IsIncludeInvalidChar(file_name_ptr, file_name_len))
     {
         //file name include folder, return this folder directly
         MMIAPICOM_Wstrncpy(full_path_ptr, file_name_ptr, MIN(file_name_len, full_path_max_len));
     }
     else
     {
          MMIAPICOM_GetLongFileNameAndSuffix(file_name_ptr, suffix_name, PNULL);
          file_type = MMIAPIFMM_ConvertFileType(suffix_name, MMIAPICOM_Wstrlen(suffix_name));
          if (file_type < MMIFMM_FILE_TYPE_NUM)
          {
              //SCI_PASSERT(file_type < MMIFMM_FILE_TYPE_NUM, ("MMIAPICOM_GetFullPathByFilename file type %d", file_type)); /*assert to do*/
              file_dev_ptr = MMIAPIFMM_GetDevicePath(filedev);
              file_dev_len = MMIAPIFMM_GetDevicePathLen(filedev);
          
              if(PNULL != file_dev_ptr && 0 != file_dev_len)
              {
                MMIAPIFMM_CombineFullPath((wchar *)file_dev_ptr, file_dev_len,
                  (wchar *)s_file_default_path[file_type], MMIAPICOM_Wstrlen((wchar*)s_file_default_path[file_type]),
                  (wchar *)file_name_ptr, file_name_len,
                  (wchar *)full_path_ptr,&full_path_max_len);   
              }
          }
     }
}

/*****************************************************************************/
//  Discription: check full_path_name exist, auto rename full_path_name till file name not exist 
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_AutoRenameFileName(wchar *full_path_name,
                                           uint16 file_name_len)
{
	BOOLEAN result = FALSE;
    
    if(file_name_len + MMIFILE_AUTO_RENAME_SUFIX_MAX_LEN <= MMIFILE_FILE_NAME_MAX_LEN)
	{
		MMIAPICOM_GetNoExistFileName(full_path_name, file_name_len);
		result = TRUE;
	}
	return result;
}

/*****************************************************************************/
//  Discription: check full_path_name exist, auto rename full_path_name till file name not exist 
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPICOM_GetNoExistFileName(wchar *full_path_name,
                                        uint16 file_name_len)
{
	int32				i = 0;
	uint8				gb_add_name[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
	uint16				ucs2_add_name[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
	uint16				ucs2_add_name_len =0;
	wchar				ucs_suffix_ptr[MMICOM_SUFFIX_MAX_LEN + 1] = {0};
	wchar				ucs_name_ptr[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
	uint16				ucs_no_suffix_len = 0;
	uint16				ucs_suffix_len = 0;
    uint16              random = 0;
    SFS_HANDLE          sfs_handle = 0;
    SFS_FIND_DATA_T     find_data = {0};
	BOOLEAN             is_folder = FALSE;

	//MMIAPIFMM_GetFileInfo(full_path_name, file_name_len, );
	sfs_handle = MMIAPIFMM_FindFirstFile(full_path_name,file_name_len, &find_data);
	if(SFS_INVALID_HANDLE == sfs_handle)
	{
		return;
	}
    SFS_FindClose(sfs_handle);
	if(SFS_ATTR_DIR & find_data.attr)
	{
		is_folder = TRUE;
	}
    else
    {
        is_folder = FALSE;
        //ucs_full_path_len = MMIAPICOM_Wstrlen( full_path_name);
        MMIAPICOM_GetLongFileNameAndSuffix((uint16*)full_path_name,ucs_suffix_ptr, PNULL);
        ucs_suffix_len = MMIAPICOM_Wstrlen((wchar*)ucs_suffix_ptr);       
    }
	
    MMIAPICOM_Wstrncpy(ucs_name_ptr, full_path_name, MIN((file_name_len - ucs_suffix_len), MMIFILE_FILE_NAME_MAX_LEN));
    ucs_no_suffix_len = MMIAPICOM_Wstrlen( ucs_name_ptr); 
    if((ucs_suffix_len) > 0 && (ucs_no_suffix_len > 0))
    {
        //this no suffix file include "."
        ucs_no_suffix_len--;
    }
    srand(SCI_GetTickCount());
    do
	{
		if(is_folder)
		{
			if (!MMIAPIFMM_IsFolderExist(full_path_name,MMIAPICOM_Wstrlen( full_path_name)))
			{
				break;
			}
		}
		else
		{
			if (!MMIAPIFMM_IsFileExist(full_path_name,MMIAPICOM_Wstrlen( full_path_name)))
			{
				break;
			}
		}

        random = rand();

        if(ucs_suffix_len > 0)
        {
            sprintf((char*)gb_add_name,"%s%ld%s", "(" , random , ").");
            
        }
        else
        {
            sprintf((char*)gb_add_name,"%s%ld%s", "(" , random , ")");
        }
		//transfer gb to ucs2 
		ucs2_add_name_len = strlen((char*)gb_add_name);
        MMI_STRNTOWSTR(ucs2_add_name, MMIFILE_FILE_NAME_MAX_LEN, gb_add_name, ucs2_add_name_len, ucs2_add_name_len);	
		//get new file name
	
		SCI_MEMSET(full_path_name, 0x00, file_name_len);
		if(ucs_no_suffix_len + ucs2_add_name_len + ucs_suffix_len>= MMIFILE_FILE_NAME_MAX_LEN)
		{
			ucs_no_suffix_len = MMIFILE_FILE_NAME_MAX_LEN - ucs2_add_name_len - ucs_suffix_len;
		}
		if(ucs_no_suffix_len > 0)
		{
			MMIAPICOM_Wstrncpy(full_path_name, ucs_name_ptr, ucs_no_suffix_len);
            full_path_name[ucs_no_suffix_len] = 0;
		}
        MMIAPICOM_Wstrncat(full_path_name, ucs2_add_name, ucs2_add_name_len);
        MMIAPICOM_Wstrncat(full_path_name, ucs_suffix_ptr, ucs_suffix_len);
        i ++;
	}while(i < MMI_RENAME_MAX_TIMES);

}

/*****************************************************************************/
//  Discription:get file with suffix by full path name 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPICOM_GetLongFileNameByFullPath( 
                                                const wchar     *ucs_full_name_ptr,     //IN
                                                wchar *ucs_name_ptr         //OUT
                                                )
{
    uint16               full_file_name_len= 0;
    uint32               m = 0;
    uint16               file_name_len = 0;
    BOOLEAN              is_include_slash = TRUE;
    
    full_file_name_len = MMIAPICOM_Wstrlen( ucs_full_name_ptr);
    if(full_file_name_len > 0)
    {
        //get the position info of "\"
        m = (full_file_name_len) - 1;   
        //SCI_TRACE_LOW:"1 full_file_name_len %d m %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1407_112_2_18_1_58_12_13,(uint8*)"dd",full_file_name_len,m);
        while ( '\\'!= *( ucs_full_name_ptr + m))
        {
            //not find "\",file name does not contain "\"
            if (0 == m)
            {
                is_include_slash = FALSE;
                break;
            }
            m --;
        }
        //SCI_TRACE_LOW:"full_file_name_len %d m %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1418_112_2_18_1_58_12_14,(uint8*)"dd",full_file_name_len,m);
        //copy path name to ucs_name_ptr
        if (PNULL != ucs_name_ptr)
        {
            if(is_include_slash)
            {
                m = m + 1;
            }
            file_name_len = MIN( full_file_name_len - m,MMIFILE_LONG_FULL_PATH_MAX_LEN );
            //SCI_TRACE_LOW:"file_name_len %d m %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1427_112_2_18_1_58_12_15,(uint8*)"dd",file_name_len,m);
            MMI_WSTRNCPY( ucs_name_ptr,
            MMIFILE_LONG_FULL_PATH_MAX_LEN,
            ucs_full_name_ptr  + m,
            file_name_len,
            file_name_len
            );
        }
    }
}



/*****************************************************************************/
//  Discription:get file path by full path name 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPICOM_GetFilePathNameByFullPath( 
                                                const wchar *ucs_full_name_ptr,     //IN
                                                wchar *ucs_name_ptr         //OUT
                                                )
{
    uint16               file_name_len= 0;
    uint32               m = 0;
    uint16               path_name_len = 0;
    
    file_name_len = MMIAPICOM_Wstrlen( ucs_full_name_ptr);

    //get the position info of "\"
    m = (file_name_len) - 1;    
    while ( '\\'!= *( ucs_full_name_ptr + m))
    {
        //not find "\",file name does not contain "\"
        if (0 == m)
        {
            break;
        }
        m --;
    }
    //copy path name to ucs_name_ptr
    if (PNULL != ucs_name_ptr)
    {
        if (m > 1)
        {  
            //m --;
            path_name_len = MIN(m,MMIFILE_FILE_NAME_MAX_LEN);
            // get new file name suffix 
            MMI_WSTRNCPY( ucs_name_ptr,
                        MMIFILE_FILE_NAME_MAX_LEN,
                        ucs_full_name_ptr,
                        path_name_len,
                        path_name_len
                        );
        }
    }
}

/*****************************************************************************/
//  Discription:get file path by full path name 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPICOM_GetLongFilePathNameByFullPath( 
                                                    const wchar *ucs_full_name_ptr,     //IN
                                                    wchar       *ucs_name_ptr           //OUT
                                                    )
{
    uint16               file_name_len= 0;
    uint32               m = 0;
    uint16               path_name_len = 0;
    
    file_name_len = MMIAPICOM_Wstrlen( ucs_full_name_ptr);

    //get the position info of "\"
    m = (file_name_len) - 1;    
    while ( '\\'!= *( ucs_full_name_ptr + m))
    {
        //not find "\",file name does not contain "\"
        if (0 == m)
        {
            break;
        }
        m --;
    }
    //copy path name to ucs_name_ptr
    if (PNULL != ucs_name_ptr)
    {
        if (m > 1)
        {  
            //m --;
            path_name_len = MIN(m,MMIFILE_LONG_FILE_NAME_MAX_LEN);
            // get new file name suffix 
            MMI_WSTRNCPY( ucs_name_ptr,
                        MMIFILE_LONG_FILE_NAME_MAX_LEN,
                        ucs_full_name_ptr,
                        path_name_len,
                        path_name_len
                        );
        }
    }
}

/*****************************************************************************/
//  Discription:get file name suffix from the full file name 
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPICOM_GetLongFileNameAndSuffix( 
                                               const wchar  *ucs_full_name_ptr,     //IN
                                               uint16       *ucs_suffix_ptr,        //OUT
                                               uint16       *ucs_name_ptr           //OUT
                                               )
{
    uint16               file_name_len= 0;
    uint32               m = 0;
    uint16               no_suffix_name_len =0;
    uint16               sufiix_name_len = 0;

    if (PNULL == ucs_full_name_ptr || (PNULL == ucs_suffix_ptr && PNULL == ucs_name_ptr))
    {
        //SCI_TRACE_LOW:"MMIAPICOM_GetLongFileNameAndSuffix PNULL == ucs_full_name_ptr || (PNULL == ucs_suffix_ptr && PNULL == ucs_name_ptr)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1548_112_2_18_1_58_12_16,(uint8*)"");
        return;
    }

    file_name_len = MMIAPICOM_Wstrlen( ucs_full_name_ptr);
    
    //SCI_TRACE_LOW:"GetFileNameAndSuffix: the file length is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1554_112_2_18_1_58_12_17,(uint8*)"d",file_name_len);
    
    //get the position info of "."
    if(0 == file_name_len)
    {
        return;
    }
    m = (file_name_len) - 1;    
    while ( MMICOM_SUFFIX_DOT != *( ucs_full_name_ptr + m))
    {
        //not find ".",file name does not contain "."
        if (0 == m)
        {
            break;
        }
        m --;
    }   
    
    if (PNULL != ucs_suffix_ptr)
    {
        if ( 0 != m)
        {
            sufiix_name_len = MIN( file_name_len - m -1, MMICOM_SUFFIX_MAX_LEN );
            if(file_name_len - m > 1)
            {
                // get new file name suffix 
                MMI_WSTRNCPY( ucs_suffix_ptr, 
                    MMICOM_SUFFIX_MAX_LEN,
                    ucs_full_name_ptr+ m + 1,
                    sufiix_name_len,
                    sufiix_name_len
                    );
            }

        }
    }
    
    if (PNULL != ucs_name_ptr)
    {
        if ( 0 == m)
        {
            //the file does not contain ".'
            no_suffix_name_len = MIN(MMIFILE_LONG_FILE_NAME_MAX_LEN,file_name_len);
        }
        else
        {
            no_suffix_name_len = MIN(MMIFILE_LONG_FILE_NAME_MAX_LEN,m);
        }
        
        MMI_WSTRNCPY(ucs_name_ptr,
            no_suffix_name_len,
            ucs_full_name_ptr,
            no_suffix_name_len,
            no_suffix_name_len
            );
    }
    
    //SCI_TRACE_LOW:"GetFileNameAndSuffix: the value m is %d, no suffix file name len is %d,the suffix len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1611_112_2_18_1_58_13_18,(uint8*)"ddd",m,no_suffix_name_len,(file_name_len - m));
}
/*****************************************************************************/
//  Discription:get file name suffix from the full file name 
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPICOM_GetFileNameAndSuffix( 
                                           const wchar  *ucs_full_name_ptr,     //IN
                                           wchar        *ucs_suffix_ptr,        //OUT
                                           wchar        *ucs_name_ptr           //OUT
                                           )
{
    uint16               file_name_len= 0;
    uint32               m = 0;
    uint16               no_suffix_name_len =0;
    uint16               sufiix_name_len = 0;

    if (PNULL == ucs_full_name_ptr || ( PNULL == ucs_suffix_ptr && PNULL == ucs_name_ptr) || 0 == MMIAPICOM_Wstrlen( ucs_full_name_ptr))
    {
        //SCI_TRACE_LOW:"MMIAPICOM_GetFileNameAndSuffix PNULL == ucs_full_name_ptr || ( PNULL == ucs_suffix_ptr && PNULL == ucs_name_ptr)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1631_112_2_18_1_58_13_19,(uint8*)"");
        return;
    }
    file_name_len = MMIAPICOM_Wstrlen( ucs_full_name_ptr);
    
    //SCI_TRACE_LOW:"GetFileNameAndSuffix: the file length is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1636_112_2_18_1_58_13_20,(uint8*)"d",file_name_len);
    
    //get the position info of "."
    m = (file_name_len) - 1;    
    while ( MMICOM_SUFFIX_DOT != *( ucs_full_name_ptr + m))
    {
        //not find ".",file name does not contain "."
        if (0 == m)
        {
            break;
        }
        m --;
    }   
    
    if (PNULL != ucs_suffix_ptr)
    {
        if ( 0 != m)
        {
            sufiix_name_len = MIN( file_name_len - m, MMICOM_SUFFIX_MAX_LEN );
            // get new file name suffix 
            MMI_WSTRNCPY( ucs_suffix_ptr,
                MMICOM_SUFFIX_MAX_LEN,
                ucs_full_name_ptr+ m,
                sufiix_name_len,
                sufiix_name_len
                );
        }
    }
    
    if (PNULL != ucs_name_ptr)
    {
        if ( 0 == m)
        {
            //the file does not contain ".'
            no_suffix_name_len = MIN(MMIFILE_FILE_NAME_MAX_LEN,file_name_len);
        }
        else
        {
            no_suffix_name_len = MIN(MMIFILE_FILE_NAME_MAX_LEN,m);
        }
        
        MMI_WSTRNCPY(ucs_name_ptr,
            no_suffix_name_len,
            ucs_full_name_ptr,
            no_suffix_name_len,
            no_suffix_name_len
            );
    }
    
    //SCI_TRACE_LOW:"GetFileNameAndSuffix: the value m is %d, no suffix file name len is %d,the suffix len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1685_112_2_18_1_58_13_21,(uint8*)"ddd",m,no_suffix_name_len,(file_name_len - m));
}


/*****************************************************************************/
//  Discription: If the file is the same type with the type array
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsSameFileType( 
                                        uint16 * file_suffix_ptr,        //new file suffix
                                        uint8  * type_suffix_ptr        //file type suffix array
                                        )
{
    wchar ucs_type_suffix[MMICOM_SUFFIX_MAX_LEN + 1] = {0};
    BOOLEAN is_same_type = FALSE;
    
    if (PNULL == file_suffix_ptr || PNULL == type_suffix_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_IsSameFileType PNULL == file_suffix_ptr || PNULL == type_suffix_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1704_112_2_18_1_58_13_22,(uint8*)"");
        return FALSE;
    }
    //compare with suffix type array
    //ucs_type_len = GUI_GB2UCS(ucs_type_suffix, (uint8 *)type_suffix_ptr,strlen((char*)type_suffix_ptr));
    MMIAPICOM_StrToWstr(type_suffix_ptr, ucs_type_suffix );

    if( 0 == MMIAPICOM_Wstrcmp( file_suffix_ptr, ucs_type_suffix ) )
    {
        is_same_type = TRUE;
    }

//  if ( 0 == GUI_StringCompare(
//                              (uint8 *)file_suffix_ptr,
//                              MMIAPICOM_Wstrlen((wchar*)file_suffix_ptr) ,
//                              (uint8 *)ucs_type_suffix, 
//                              ucs_type_len
//                              ))
//  {
//      //the new file is the same type with type array
//      is_same_type = TRUE;
//  }
//  else
//  {
//      is_same_type = FALSE;
//  }

    return is_same_type;    
}



/*****************************************************************************/
//  Discription: If the file is the same type with the type array
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsKingMovieType(
                                             const wchar *file_name_ptr,
                                             uint16      name_len
                                             )
{

    BOOLEAN is_same_type = FALSE;
    MMICOM_VIDEO_TYPE_E videotype = MMICOM_VIDEO_TYPE_MAX;
    if (PNULL == file_name_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_IsSameFileType PNULL == file_suffix_ptr || PNULL == type_suffix_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1704_112_2_18_1_58_13_22,(uint8*)"");
        return FALSE;
    }
    videotype = MMIAPICOM_GetMovieType(file_name_ptr,name_len);
#ifdef MMI_KING_MOVIE_SUPPORT
    if( MMICOM_VIDEO_TYPE_KMV == videotype || MMICOM_VIDEO_TYPE_SMV == videotype)
    {
        is_same_type = TRUE;
    }
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
    else if(MMICOM_VIDEO_TYPE_HMV == videotype)
    {
        is_same_type =TRUE;
    }
#endif
#endif 
    return is_same_type;    
}

/*****************************************************************************/
//  Description : 获得没有后缀名的文件名称，以及后缀名
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC void MMIAPICOM_GetNameAndSuffix(
                                       const wchar     *name_suffix_ptr,   //文件名
                                       uint16          name_suffix_len,    //文件名长度
                                       MMI_STRING_T    *name_ptr,          //没有后缀名的文件名
                                       MMI_STRING_T    *suffix_ptr         //后缀名
                                       )
{
    int32              i = 0;

    if (PNULL == name_suffix_ptr || 0 == name_suffix_len || (PNULL==name_ptr && PNULL == suffix_ptr))
    {
        //SCI_TRACE_LOW:"MMIAPICOM_GetNameAndSuffix PNULL == name_suffix_ptr || 0 == name_suffix_len || (PNULL==name_ptr && PNULL == suffix_ptr)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1750_112_2_18_1_58_13_23,(uint8*)"");
        return;
    }
    for (i=(name_suffix_len-1); i>=0; i--)
    {
        if (MMICOM_SUFFIX_DOT == name_suffix_ptr[i])
        {
            break;
        }
    }
    if (i < 0)
    {
        i=name_suffix_len;
    }

    if (PNULL != name_ptr)
    {
        SCI_ASSERT(PNULL != name_ptr->wstr_ptr);/*assert verified*/

        MMI_WSTRNCPY( name_ptr->wstr_ptr, i, name_suffix_ptr, i, i );
        name_ptr->wstr_len = i;

        //convert uint16 unicode to uint8 ucs2
        //MMIAPICOM_ConvertWstrToMMISTRING(name_suffix_ptr,(uint16)i,name_ptr);
    }

    if (PNULL != suffix_ptr && i<name_suffix_len)
    {
        SCI_ASSERT(PNULL != suffix_ptr->wstr_ptr);/*assert verified*/

        suffix_ptr->wstr_len = MIN(MMICOM_SUFFIX_MAX_LEN, name_suffix_len-i-1);
        MMI_WSTRNCPY( suffix_ptr->wstr_ptr, suffix_ptr->wstr_len, name_suffix_ptr+i+1, suffix_ptr->wstr_len, suffix_ptr->wstr_len);

        //convert uint16 unicode to uint8 ucs2
       // MMIAPICOM_ConvertWstrToMMISTRING(name_suffix_ptr+i+1,(uint16)(name_suffix_len-i-1),suffix_ptr);
    }
}



/*****************************************************************************/
//  Description : 文件名是否有效
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPICOM_FileNameIsValid(
                                         MMI_STRING_T    *file_name_ptr  //in:
                                         )
{
    BOOLEAN     result = FALSE;
    if (PNULL == file_name_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_FileNameIsValid PNULL == file_name_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1801_112_2_18_1_58_13_24,(uint8*)"");
        return FALSE;
    }
    
     if (PNULL == (file_name_ptr->wstr_ptr))
    {
        //SCI_TRACE_LOW:"MMIAPICOM_FileNameIsValid PNULL == file_name_ptr->wstr_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1801_112_2_18_1_58_13_24,(uint8*)"");
        return FALSE;
    }

    if(' ' == *(file_name_ptr->wstr_ptr))
    {
        return FALSE;
    }
	if(1 == file_name_ptr->wstr_len && ('.' == *file_name_ptr->wstr_ptr))
	{
		//name: ".", is invalid
		return FALSE;
	}
    //判断名字长度
    if ((0 < file_name_ptr->wstr_len) && (MMIFILE_FILE_NAME_MAX_LEN > file_name_ptr->wstr_len))
    {
        if (!MMIAPICOM_IsIncludeInvalidChar(file_name_ptr->wstr_ptr,  file_name_ptr->wstr_len ))
        {
            result = TRUE;
        }
    } 
	
    
    return (result);
}


/*****************************************************************************/
//  Description : get image file type
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC GUIANIM_TYPE_E MMIAPICOM_GetImgaeType(
                                             const wchar  *full_path_ptr, //in
                                             uint16       full_path_len   //in
                                             )
{
    const wchar         bmp_str[] = {'b', 'm', 'p', 0};//"bmp";
    const wchar         jpg_str[] = {'j', 'p', 'g', 0};//"jpg";
    const wchar         jpeg_str[] = {'j', 'p', 'e', 'g', 0};//"jpg";
    const wchar         wbmp_str[] = { 'w', 'b', 'm', 'p', 0};//"wbmp"; 
    const wchar         gif_str[] = {'g', 'i', 'f', 0};//"gif";
    const wchar         png_str[] = {'p', 'n', 'g', 0};//"png";
    const wchar         webp_str[] = {'w', 'e', 'b','p', 0}; //"webp"
    MMI_STRING_T        suffix_str = {0};
    GUIANIM_TYPE_E      pic_type = GUIANIM_TYPE_NONE;

    if (PNULL == full_path_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_GetImgaeType PNULL == full_path_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_1848_112_2_18_1_58_13_25,(uint8*)"");
        return pic_type;
    }
    if (0 < full_path_len)
    {
        suffix_str.wstr_ptr = SCI_ALLOC_APP( ( MMICOM_SUFFIX_MAX_LEN + 1 ) * sizeof(wchar) );

        SCI_MEMSET(suffix_str.wstr_ptr, 0, (( MMICOM_SUFFIX_MAX_LEN + 1 ) * sizeof(wchar)) );

        MMIAPICOM_GetNameAndSuffix( full_path_ptr, full_path_len, PNULL, &suffix_str );

        MMIAPICOM_Wstrlower( suffix_str.wstr_ptr );

        if (0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, bmp_str))
        {
            pic_type = GUIANIM_TYPE_IMAGE_BMP;
        }
        else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, jpg_str ))
        {
            pic_type = GUIANIM_TYPE_IMAGE_JPG;
        }
        else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, jpeg_str ))
        {
            pic_type = GUIANIM_TYPE_IMAGE_JPG;
        }
        else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, wbmp_str ))
        {
            pic_type = GUIANIM_TYPE_IMAGE_WBMP;
        }
        else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, gif_str ))
        {
            pic_type = GUIANIM_TYPE_IMAGE_GIF;
        }
        else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, png_str ))
        {
            pic_type = GUIANIM_TYPE_IMAGE_PNG;
        }
        else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, webp_str))
        {
            pic_type = GUIANIM_TYPE_IMAGE_WEBP;
        }

        //free memory
        SCI_FREE(suffix_str.wstr_ptr);
    }

    return (pic_type);
}


/*****************************************************************************/
//  Description : get music file type
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC MMISRVAUD_RING_FMT_E  MMIAPICOM_GetMusicType(
                                               const wchar    *file_name_ptr,
                                               uint16    name_len
                                               )
{
    MMISRVAUD_RING_FMT_E     ring_type = MMISRVAUD_RING_FMT_MAX;

    if(0 == name_len || PNULL == file_name_ptr)
    {
        return MMISRVAUD_RING_FMT_MAX;
    }

    ring_type = MMIAPICOM_GetMusicTypeEx(file_name_ptr, name_len, FALSE);

    return (ring_type);
}

/*****************************************************************************/
//  Description : get music file type(including drm)
//  Global resource dependence : 
//  Author: paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRVAUD_RING_FMT_E  MMIAPICOM_GetMusicTypeEx(
                                               const wchar    *file_name_ptr,
                                               uint16    name_len,
                                               BOOLEAN need_check_drm
                                               )
{
    const wchar               mid_str[] =  { 'm', 'i', 'd', 0 };//"mid";
    const wchar               midi_str[] = { 'm', 'i', 'd', 'i', 0 };//"midi";
    const wchar               mp3_str[] =  { 'm', 'p', '3', 0 };//"mp3";
    //@CR241798 2011.05.30
    const wchar               mp2_str[] =  { 'm', 'p', '2', 0 };//"mp2";
    const wchar               mp1_str[] =  { 'm', 'p', '1', 0 };//"mp1";
    //@CR241798 2011.05.30
    const wchar               wma_str[] =  { 'w', 'm', 'a', 0 };//"wma";
    const wchar               smaf_str[] = { 's', 'm', 'a', 'f', 0 };//"smaf";
    const wchar               wav_str[] =  { 'w', 'a', 'v', 0 };//"wav";
    const wchar               amr_str[] =  { 'a', 'm', 'r', 0 };//"amr";
#ifdef AAC_SUPPORT
    const wchar               aac_str[] =  { 'a', 'a', 'c', 0 };//"aac";
    const wchar               m4a_str[] =  { 'm', '4', 'a', 0 };//"m4a";
#endif    
#ifdef DRM_SUPPORT
    const wchar               drm_str[] =  { 'd', 'm', 0 };//"drm";
    const wchar               dcf_str[] =  { 'd', 'c', 'f', 0 };//"dcf";
    SFS_HANDLE              drm_handle = PNULL;
    DRM_RIGHTS_T            drm_right =  {0};
#endif
    MMI_STRING_T        suffix_str = {0};
    MMISRVAUD_RING_FMT_E     ring_type = MMISRVAUD_RING_FMT_MAX;

    if(0 == name_len || PNULL == file_name_ptr)
    {
        return MMISRVAUD_RING_FMT_MAX;
    }
    //get suffix
    suffix_str.wstr_ptr = SCI_ALLOC_APP( ( MMICOM_SUFFIX_MAX_LEN + 1 ) * sizeof(wchar) );
    SCI_MEMSET( suffix_str.wstr_ptr, 0, (( MMICOM_SUFFIX_MAX_LEN + 1 ) * sizeof(wchar)) );

    MMIAPICOM_GetNameAndSuffix( file_name_ptr, name_len, PNULL, &suffix_str );
    //MMIAPICOM_MakeStringLowerCase(&suffix_str);
    //MMIAPICOM_ConvertLcase(suffix_str.str_ptr,suffix_str.length);
    MMIAPICOM_Wstrlower( suffix_str.wstr_ptr );
    
    if (0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, mid_str))
    {
        ring_type = MMISRVAUD_RING_FMT_MIDI;
    }
    else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, midi_str))
    {
        ring_type = MMISRVAUD_RING_FMT_MIDI;
    }
    else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, mp3_str))
    {
        ring_type = MMISRVAUD_RING_FMT_MP3;
    }
    //@CR241798 2011.05.30
    else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, mp2_str))
    {
        ring_type = MMISRVAUD_RING_FMT_MP3;
    }
    else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, mp1_str))
    {
        ring_type = MMISRVAUD_RING_FMT_MP3;
    }
    //@CR241798 2011.05.30
    else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, wma_str))
    {
        ring_type = MMISRVAUD_RING_FMT_WMA;
    }
    else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, smaf_str))
    {
        ring_type = MMISRVAUD_RING_FMT_SMAF;
    }   
    else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, wav_str))
    {
        ring_type = MMISRVAUD_RING_FMT_WAVE;
    }   
    else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, amr_str))
    {
        ring_type = MMISRVAUD_RING_FMT_AMR;
    }   
#ifdef AAC_SUPPORT
    else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, aac_str))
    {
        ring_type = MMISRVAUD_RING_FMT_AAC;
    }
    else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, m4a_str))
    {
        ring_type = MMISRVAUD_RING_FMT_M4A;
    }
#endif
#ifdef DRM_SUPPORT
    else if (0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, drm_str)
        ||0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, dcf_str) )
    {
        if(need_check_drm)
        {
            drm_handle = SFS_CreateFile(file_name_ptr, 
                SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);           
          
            DRM_GetRightsInfo(drm_handle, &drm_right);

            ring_type = MMIAPIDRM_ConvertAudioMimeType(drm_right.mime_type);

            SFS_CloseFile(drm_handle);
        }
    }
#endif
    else
    {        
        ring_type = MMISRVAUD_RING_FMT_MAX;
    }

    //free memory
    SCI_FREE(suffix_str.wstr_ptr);

    return (ring_type);
}

/*****************************************************************************/
//  Description : get movie file type
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC MMICOM_VIDEO_TYPE_E  MMIAPICOM_GetMovieType(
                                             const wchar *file_name_ptr,
                                             uint16      name_len
                                             )
{
    const wchar         gp_str[] = { '3', 'g', 'p', 0 };//"3gp";
    const wchar         mpeg4_str[] = { 'm', 'p', '4', 0 };//"mp4";
#ifdef AVI_DEC_SUPPORT
    const wchar         avi_str[] = { 'a', 'v', 'i', 0 };//"avi";
#endif
#ifdef FLV_DEC_SUPPORT
    const wchar         flv_str[] = { 'f', 'l', 'v', 0 };//"flv";
#endif
#ifdef MMI_RMVB_SUPPORT
    const wchar         rv_str[] = { 'r', 'v', 0 };//"rv";
    const wchar         rm_str[] = { 'r', 'm', 0 };//"rm";
    const wchar         rmvb_str[] = { 'r', 'm', 'v', 'b', 0 };//"rmvb";
#endif
#ifdef MMI_KING_MOVIE_SUPPORT
    const wchar         kmv_str[]={'k','m','v',0};//"kmv"
    const wchar         smv_str[]={'s','m','v',0};//"smv"
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
    const wchar         hmv_str[]={'h','m','v',0};//"hmv"
#endif
#endif
    MMI_STRING_T        suffix_str = {0};
    MMICOM_VIDEO_TYPE_E       movie_type = MMICOM_VIDEO_TYPE_3GP;

    if (PNULL == file_name_ptr || 0 == name_len)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_GetMovieType PNULL == file_name_ptr || 0 == name_len"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_2028_112_2_18_1_58_13_26,(uint8*)"");
        return MMICOM_VIDEO_TYPE_MAX;
    }
    //get suffix
    suffix_str.wstr_ptr = SCI_ALLOC_APP( ( MMICOM_SUFFIX_MAX_LEN + 1 ) * sizeof(wchar) );
    SCI_MEMSET( suffix_str.wstr_ptr, 0, (( MMICOM_SUFFIX_MAX_LEN + 1 ) * sizeof(wchar)) );

    MMIAPICOM_GetNameAndSuffix( file_name_ptr, name_len, PNULL, &suffix_str );
    //MMIAPICOM_MakeStringLowerCase(&suffix_str);
    //MMIAPICOM_ConvertLcase(suffix_str.wstr_ptr,suffix_str.wstr_ptr);
    
    MMIAPICOM_Wstrlower( suffix_str.wstr_ptr );

    if (0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, gp_str ) )
    {
        movie_type = MMICOM_VIDEO_TYPE_3GP;
    }
    else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, mpeg4_str ) )
    {
        movie_type = MMICOM_VIDEO_TYPE_MP4;
    }
#ifdef AVI_DEC_SUPPORT
    else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, avi_str ) )
    {
        movie_type = MMICOM_VIDEO_TYPE_AVI;
    }
#endif
#ifdef FLV_DEC_SUPPORT
    else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, flv_str ) )
    {
        movie_type = MMICOM_VIDEO_TYPE_FLV;
    }
#endif
#ifdef MMI_RMVB_SUPPORT
    else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, rv_str) 
        ||0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, rm_str)
        ||0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, rmvb_str))
    {
        movie_type = MMICOM_VIDEO_TYPE_RMVB;
    }
#endif
#ifdef MMI_KING_MOVIE_SUPPORT
    else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, kmv_str ) )
    {
        movie_type = MMICOM_VIDEO_TYPE_KMV;
    }     
    else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, smv_str ) ) //smv_support
    {
        movie_type = MMICOM_VIDEO_TYPE_SMV;
    }
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
    else if(0 == MMIAPICOM_Wstrcmp( suffix_str.wstr_ptr, hmv_str ) ) //hmv_support
    {
        movie_type = MMICOM_VIDEO_TYPE_HMV;
    }
#endif
#endif
    else
    {
        movie_type = MMICOM_VIDEO_TYPE_MAX;
    }

    //free memory
    SCI_FREE(suffix_str.wstr_ptr);

    return (movie_type);
}



/*****************************************************************************/
//  Discription: This function is used to send signal from mmi to mmi
//   
//  Global resource dependence: None
//  Author: baokun yin
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPICOM_SendSignalToMMITask(uint32 sig_id)
{
    MmiSignalS *signal = PNULL;

    //creat signal
    MMI_CREATE_SIGNAL(signal,(uint16)sig_id,sizeof(MmiSignalS),P_APP);

    //send signal
    MMI_SEND_SIGNAL(signal, P_APP);
}


/*****************************************************************************/
//  Description : 创建一个随机名字的文件
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_CreateRandFile(
                                        const wchar     *path_ptr,  //in，路径
                                        uint16      path_len,   //in, 双字节单位
                                        uint8       *prefix_str_ptr,    //in，文件名前缀，可以是NULL
                                        uint8       *suffix_str_ptr,    //in，文件名后缀，可以是NULL
                                        wchar       *file_name_ptr,     //out，输出文件名
                                        uint16      *file_name_len_ptr  //out，双字节单位
                                        )
{
    SFS_HANDLE      sfs_handle = NULL;
    SFS_FIND_DATA_T find_data = {0};
    uint16          random = 0;
    //MMI_STRING_T    ucs2_str = {0};
    uint16          file_name_len = 0;
    //uint16            len = 0;
    BOOLEAN         result = TRUE;
    wchar           *temp_wstr = PNULL;
    uint8           *temp_str = PNULL;
    uint16          len = 0; 

    if (PNULL == path_ptr || 0 == path_len || PNULL == file_name_ptr|| PNULL == file_name_len_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_CreateRandFile PNULL == path_ptr || 0 == path_len || PNULL == file_name_ptr|| PNULL == file_name_len_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_2140_112_2_18_1_58_14_27,(uint8*)"");
        return FALSE;
    }
    temp_wstr = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));

    if (PNULL == temp_wstr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_GenPartyNumber1 PNULL == temp_wstr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_2147_112_2_18_1_58_14_28,(uint8*)"");
        return FALSE;
    }
    temp_str = SCI_ALLOCA(sizeof(uint8)*(MMIFILE_FULL_PATH_MAX_LEN + 1));

    if (PNULL == temp_str)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_GenPartyNumber2 PNULL == temp_str"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_2154_112_2_18_1_58_14_29,(uint8*)"");
        SCI_FREE(temp_wstr);
        return FALSE;
    }
    srand(SCI_GetTickCount());

    do 
    {
        SCI_MEMSET(temp_wstr,0x00,(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1)));
        SCI_MEMSET(temp_str,0x00,(sizeof(uint8)*(MMIFILE_FULL_PATH_MAX_LEN + 1)));
        //产生随机数
        random = rand();

        //文件名以前缀，后面跟以一个随机数字符串，再跟后缀

        //前缀
        if (NULL != prefix_str_ptr)
        {
            sprintf((char *)temp_str,"%s", prefix_str_ptr);
            len = strlen((char*)temp_str);
        }

        sprintf((char *)(temp_str + len),"%05d", random);
        len = strlen((char*)temp_str);

        //后缀
        if (NULL != suffix_str_ptr)
        {
            sprintf((char *)(temp_str + len),"%s", suffix_str_ptr);
            len = strlen((char*)temp_str);
        }

        SCI_ASSERT( len <= MMIFILE_FULL_PATH_MAX_LEN );/*assert verified*/
        MMIAPICOM_StrToWstr(temp_str, temp_wstr );

        MMI_WSTRNCPY( file_name_ptr, MMIFILE_FULL_PATH_MAX_LEN, path_ptr, path_len, path_len );
        //目录
        file_name_len = MIN( path_len, MMIFILE_FULL_PATH_MAX_LEN );

        MMI_WSTRNCPY( file_name_ptr + file_name_len, len, temp_wstr, len, len );


        file_name_len += len;

        //检查是否有同名的文件
        sfs_handle = MMIAPIFMM_FindFirstFile(file_name_ptr,file_name_len, &find_data);
        if(NULL != sfs_handle)
        {
            SFS_FindClose(sfs_handle);

            SCI_MEMSET(file_name_ptr, 0, (file_name_len * sizeof(wchar)) );
            file_name_len = 0;
        }
    }
    while(NULL != sfs_handle);
    
    //创建文件
    sfs_handle = SFS_CreateFile(file_name_ptr, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, NULL, NULL);/*lint !e655 */
    if (NULL != sfs_handle)
    {
        SFS_CloseFile(sfs_handle);
        //SCI_TRACE_LOW:"MMIAPICOM_GetRandFile, success!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_2214_112_2_18_1_58_14_30,(uint8*)"");
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPICOM_GetRandFile, fail!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_2219_112_2_18_1_58_14_31,(uint8*)"");
        result = FALSE;
    }

    *file_name_len_ptr = file_name_len;

    //SCI_FREE(ucs2_str.wstr_ptr);
    //if (PNULL!=temp_wstr)
    {
        SCI_FREE(temp_wstr);
        temp_wstr = PNULL;
    }
    //if (PNULL!=temp_str)
    {
        SCI_FREE(temp_str);
        temp_str = PNULL;
    }
    return result;
}


/*****************************************************************************/
//  Description : 创建一个随机名字的文件
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIAPICOM_CreateRandFileAsyn(
                                        const wchar     *path_ptr,  //in，路径
                                        uint16      path_len,   //in, 双字节单位
                                        uint8       *prefix_str_ptr,    //in，文件名前缀，可以是NULL
                                        uint8       *suffix_str_ptr,    //in，文件名后缀，可以是NULL
                                        wchar       *file_name_ptr,     //out，输出文件名
                                        uint16      *file_name_len_ptr  //out，双字节单位
                                        )
{
    SFS_HANDLE      sfs_handle = NULL;
    // SFS_FIND_DATA_T find_data = {0};
    uint16          random = 0;
    //MMI_STRING_T    ucs2_str = {0};
    uint16          file_name_len = 0;
    //uint16            len = 0;
    MMIFILE_ERROR_E  result = SFS_ERROR_NONE;
    wchar           *temp_wstr = PNULL;
    uint8           *temp_str = PNULL;
    uint16          len = 0; 

    if (PNULL == path_ptr || 0 == path_len || PNULL == file_name_ptr || PNULL == file_name_len_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_CreateRandFileAsyn PNULL == path_ptr || 0 == path_len || PNULL == file_name_ptr || PNULL == file_name_len_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_2266_112_2_18_1_58_14_32,(uint8*)"");
        return SFS_ERROR_INVALID_PARAM;
    }

    temp_wstr = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1));

    if (PNULL == temp_wstr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_CreateRandFileAsyn PNULL == temp_wstr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_2274_112_2_18_1_58_14_33,(uint8*)"");
        return SFS_ERROR_INVALID_PARAM;
    }
    temp_str = SCI_ALLOCA(sizeof(uint8)*(MMIFILE_FULL_PATH_MAX_LEN + 1));

    if (PNULL == temp_str)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_CreateRandFileAsyn PNULL == temp_str"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_2281_112_2_18_1_58_14_34,(uint8*)"");
        SCI_FREE(temp_wstr);
        return SFS_ERROR_INVALID_PARAM;
    }
    srand(SCI_GetTickCount());

    do 
    {
        SCI_MEMSET(temp_wstr,0x00,(sizeof(wchar)*(MMIFILE_FULL_PATH_MAX_LEN + 1)));
        SCI_MEMSET(temp_str,0x00,(sizeof(uint8)*(MMIFILE_FULL_PATH_MAX_LEN + 1)));
        //产生随机数
        random = rand();

        //文件名以前缀，后面跟以一个随机数字符串，再跟后缀

        //前缀
        if (NULL != prefix_str_ptr)
        {
            sprintf((char *)temp_str,"%s", prefix_str_ptr);
            len = strlen((char*)temp_str);
        }

        sprintf((char *)(temp_str + len),"%05d", random);
        len = strlen((char*)temp_str);

        //后缀
        if (NULL != suffix_str_ptr)
        {
            sprintf((char *)(temp_str + len),"%s", suffix_str_ptr);
            len = strlen((char*)temp_str);
        }

        SCI_ASSERT( len <= MMIFILE_FULL_PATH_MAX_LEN );/*assert verified*/
        MMIAPICOM_StrToWstr(temp_str, temp_wstr );

        MMI_WSTRNCPY( file_name_ptr, MMIFILE_FULL_PATH_MAX_LEN, path_ptr, path_len, path_len );
        //目录
        file_name_len = MIN( path_len, MMIFILE_FULL_PATH_MAX_LEN );
        file_name_ptr[file_name_len] = '\\';

        MMI_WSTRNCPY( file_name_ptr + file_name_len+1, len, temp_wstr, len, len );


        file_name_len += len +1;
		
		//创建文件
		result = MMIAPIFMM_CreateFileAsyn(file_name_ptr, 
			SFS_MODE_WRITE|SFS_MODE_CREATE_NEW, /*lint !e655*/
			NULL, 
			NULL,
			0,
			0,
			&sfs_handle);	
		//sfs_handle = SFS_CreateFile(file_name_ptr, SFS_MODE_CREATE_ALWAYS, NULL, NULL);
		if (NULL != sfs_handle)
		{
			SFS_CloseFile(sfs_handle);
			//SCI_TRACE_LOW:"MMIAPICOM_GetRandFile, success!!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_2336_112_2_18_1_58_14_35,(uint8*)"");
			break;
			//result = TRUE;
		}
    }
    while(SFS_ERROR_HAS_EXIST == result);
    
    *file_name_len_ptr = file_name_len;

    //SCI_FREE(ucs2_str.wstr_ptr);
    //if (PNULL!=temp_wstr)
    {
        SCI_FREE(temp_wstr);
        temp_wstr = PNULL;
    }
    //if (PNULL!=temp_str)
    {
        SCI_FREE(temp_str);
        temp_str = PNULL;
    }
    return result;
}

#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
//  Description : caf malloc
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void* CAFMalloc( 
                      uint32 size,
                      const char *file,
                      uint32 line
                      )
{
    return (void*)MMKMNG_MemMalloc( size );
}

/*****************************************************************************/
//  Description : caf free
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void CAFFree(
                   void* mem_ptr
                   )
{
    MMKMNG_MemFree( (uint8*)mem_ptr );
}

/*****************************************************************************/
//  Description : htonl
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFHtonl( uint32 host_long )
{
    return htonl( host_long );
}

/*****************************************************************************/
//  Description : htons
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint16 CAFHtons( uint16 host_short )
{
    return htons( host_short );
}

/*****************************************************************************/
//  Description : htonl
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 CAFNtohl( uint32 net_long )
{
    return ntohl( net_long );
}

/*****************************************************************************/
//  Description : htons
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL uint16 CAFNtohs( uint16 net_short )
{
    return ntohs( net_short );
}

/*****************************************************************************/
//  Description : inet_aton
//  Global resource dependence : 
//  Author:James.Zhang
//  Note: 仅仅是为了去warning
/*****************************************************************************/
LOCAL int CAFINetAton( 
                      const char *cp,
                      uint32* pin
                      )
{
#ifdef MMI_GPRS_SUPPORT
    return inet_aton( (char*)cp, pin );
#else
    return 0;
#endif
}

/*****************************************************************************/
//  Description : inet_ntoa
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
LOCAL char * CAFINetNtoa(uint32 pin)
{
#ifdef MMI_GPRS_SUPPORT
    return inet_ntoa(pin);
#else
    return PNULL;
#endif
}
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:将wstr trace输出
/*****************************************************************************/
PUBLIC void MMIAPICOM_WstrTraceOut(
                                   const wchar* src,
                                   size_t count
                                   )
{
    char* str = SCI_ALLOC_APP( count + 1 );

    SCI_MEMSET( str, 0, (count + 1) );

    MMI_WSTRNTOSTR((uint8*)str, count, src, count, count );

    //SCI_TRACE_LOW:"MMIAPICOM_WstrTraceOut str is %s\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_3169_112_2_18_1_58_16_39,(uint8*)"s", str );

    SCI_FREE( str );
}

#define COM_REV_BIAS '\\' //"\x00\x5c" // "\" 反斜线
#define COM_POS_BIAS '/'   //"\x00\x2f" // "/" 正斜线

/*****************************************************************************/
//  Description : get the full path according to the full file name
//  Global resource dependence : 
//  Author:Xiaoqing.lu
//  Note: 本接口的作用是获取文件名（包含全路径）中的路径名
//  Example: file_name_ptr[d:\com\test\test.exe],输出参数file_path_ptr[d:\com\test]
/*****************************************************************************/
BOOLEAN MMIAPICOM_GetFilePath(
                              const wchar *file_name_ptr, // [IN]
                              uint16 file_name_len,  // [IN]
                              wchar *file_path_ptr, // [OUT]
                              uint16 *file_path_len  // [OUT]
                              )
{
    BOOLEAN result = FALSE;
    int16 i = 0;

    if (file_path_ptr == PNULL || file_path_len == PNULL|| file_name_ptr == PNULL || file_name_len == 0)
    {
        return result;
    }
    
    for (i = (uint16)(file_name_len - 1); i >= 0; i--)
    {
        // 判断字符是否为正斜线或者反斜线
        if ( file_name_ptr[i] == COM_REV_BIAS 
            || file_name_ptr[i] == COM_POS_BIAS )
        {
            MMIAPICOM_Wstrncpy( file_path_ptr, file_name_ptr, i );
            //SCI_MEMCPY(file_path_ptr, file_name_ptr, i);

            *file_path_len = i;
            result = TRUE;
            break;
        }
    }

    return result;
}

/*****************************************************************************/
// Description : get valid file name 
// Global resource dependence : 
// Author: jian.ma
// Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPICOM_GetValidFileName(
										  BOOLEAN	is_complete_name,	//in:名字后面是否还有字符,比如后缀等
										  wchar		*file_name,			//in
										  uint32	file_len,			//in
										  wchar		*new_file_name,		//out
										  uint32	new_file_len		//in
										  )
{
	BOOLEAN		result = FALSE;
	uint32		i = 0;
    uint32		dest_name_len = 0;
    BOOLEAN		need_skip = TRUE;
	
	if(PNULL == file_name || PNULL == new_file_name)
	{
		return FALSE;
	}
    for (i=0; i<file_len; i++)
    {
        if (' ' != file_name[i] && '.' != file_name[i] || (!need_skip))
        {
			//remove first ' ' and '.'
			need_skip = FALSE;
            new_file_name[dest_name_len] = file_name[i];
			dest_name_len++;
			if(new_file_len < dest_name_len)
			{
				break;
			}
			if('\\' == file_name[i])
			{
				need_skip = TRUE;
			}
        }
    }
    if (dest_name_len)
    {
        result = TRUE;
		if (is_complete_name)
		{
			//remove ' ' and '.' from tail
			for (i=0; i<file_len; i++)
			{
				if ((' ' == file_name[file_len-i-1]) || ('.' == file_name[file_len-i-1]))
				{
					if(dest_name_len > 0)
					{     
						  dest_name_len --;
                          new_file_name[dest_name_len] = 0;
					}
					else
					{
						//new file name len = 0
						break;
					}
				}
				else
				{
					//no ' ' and '.'  in tail
					break;
				}
			}
		}		
    }
	
    return result;
}

/*****************************************************************************/
//  Description : get the full path according to the full file name
//  Global resource dependence : 
//  Author:Xiaoqing.lu
//  Note: 
//  Example: file_name_ptr[d:\com\test\test.exe],输出参数file_path_ptr[test.exe]
/*****************************************************************************/
BOOLEAN MMIAPICOM_GetFileName(
                              const uint16 *src_name_ptr, // [IN]
                              uint16 src_name_len,  // [IN]
                              uint16 *des_path_ptr, // [OUT]
                              uint16 *des_path_len  // [OUT]
                              )
{
    BOOLEAN result = FALSE;
    int16 i = 0;

    if (des_path_ptr == PNULL || des_path_len == PNULL || src_name_ptr == PNULL || src_name_len == 0)
    {
        return result;
    }
    
    for (i = (uint16)(src_name_len - 1); i >= 0; i--)
    {
        if ( src_name_ptr[i] == COM_POS_BIAS 
            || src_name_ptr[i] == COM_REV_BIAS )
        {
            MMIAPICOM_Wstrncpy( des_path_ptr, &src_name_ptr[i+1], (src_name_len - i - 1) );
            //SCI_MEMCPY(des_path_ptr, &src_name_ptr[i+1], (src_name_len - i - 1));
            *des_path_len = (uint16)(src_name_len - i - 1);
            result = TRUE;
            break;
        }
    }

    return result;
}

// "."
//#define COM_MIF_DOT "\x00\x2e\x00\x00"  // "."
// "\"
//#define COM_MIF_SLASH "\x00\x5c\x00\x00" // "\"
/*****************************************************************************/
//  Description : combine pathname with two name of dir_ptr and filename
//  Global resource dependence : 
//  Author:Xiaoqing.lu
//  Note: 
/*****************************************************************************/
BOOLEAN MMIAPICOM_CombinePath(
                              const uint16     *dir_ptr,           //in
                              uint16           dir_len,            //in, 双字节为单位
                              const uint16     *file_name_ptr,     //in, can be NULL
                              uint16           file_name_len,      //in, 双字节为单位
                              BOOLEAN          is_extname,         //in, 是否为扩展名
                              uint16           *full_path_ptr,     //out
                              uint16           *full_path_len_ptr  //out, 双字节为单位
                              )
{
    wchar       dot_char   = MMICOM_SUFFIX_DOT;
    wchar       slash_char = '\\';
    
    //uint16      *wchar_ptr = NULL;
    uint32      j = 0;
    
    if (0 == file_name_len || PNULL == file_name_ptr ||file_name_len > CAF_FILENAME_MAX_LEN)
    {
        *full_path_len_ptr = 0;
        return FALSE;
    }
    if (PNULL == full_path_ptr || PNULL == full_path_len_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPICOM_CombinePath PNULL == full_path_ptr || PNULL == full_path_len_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_3354_112_2_18_1_58_16_40,(uint8*)"");
        return FALSE;
    }
    
    //copy dir to full path
    if (dir_len > 0)
    {
        // 根路径不需要添加 '\'
        MMI_WSTRNCPY( full_path_ptr + j, dir_len, dir_ptr, dir_len, dir_len );
        
        j = j + dir_len;
    }

    if (dir_len > 0 && !is_extname)
    {
        // 根路径不需要添加 '\'
        //wchar_ptr = (uint16 *)(full_path_ptr+j);
        
        MMI_WSTRNCPY((full_path_ptr + j), 1, &slash_char, 1, 1 );
        j = j + 1;
    }
    else if (dir_len > 0 && is_extname)
    {
        MMI_WSTRNCPY((full_path_ptr + j), 1, &dot_char, 1, 1 );
        j = j + 1;
    }
    
    
    //copy file name to full path
    MMI_WSTRNCPY( full_path_ptr + j, file_name_len, file_name_ptr, file_name_len, file_name_len );
    
    j = j + file_name_len;
    
    *full_path_len_ptr = (uint16)j;
    
    SCI_ASSERT(0 < *full_path_len_ptr && CAF_FILENAME_MAX_LEN >= *full_path_len_ptr);/*assert verified*/
    
    return TRUE;
}

#ifdef MMI_DEADLOOP_CHECK_ENABLE

/*****************************************************************************/
//  Description : start deadloop check
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_StartDeadLoopCheck(
                                         uint32 expired_time
                                         )
{
    if (PNULL == s_deadloop_check_timer)
    {
        s_deadloop_check_timer = SCI_CreateTimer("MMI Deadloop Check Timer",
            DeadloopCheckTimerExpired,1,expired_time,SCI_AUTO_ACTIVATE);    
    }
}

/*****************************************************************************/
//  Description : end deadloop check
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_EndDeadLoopCheck(void)
{
    if (PNULL != s_deadloop_check_timer)
    {
	    SCI_DeleteTimer(s_deadloop_check_timer);
        s_deadloop_check_timer = PNULL;
    }
}

/*****************************************************************************/
//  Description : print deadloop check
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_PrintDeadLoopCheck(
                                         char *file,    //文件名
                                         uint32 line    //行号
                                         )
{
    s_deadloop_file = file;
    s_deadloop_line = line;
}

/**********************************************************************
//    Description:
//      the function to process the mmi dead loop timer expire
//    Global resource dependence :
//    Author: bin.ji
//    Note:
***********************************************************************/
LOCAL void DeadloopCheckTimerExpired(
                                uint32 lparam   //the function's input parameter
                                )
{
    if (PNULL != s_deadloop_file)
    {
        SCI_PASSERT(0, ("maybe deadloop! file = %s, line = %d", s_deadloop_file, s_deadloop_line)); /*assert to do*/
    }
}
/**********************************************************************
//    Description: the function to judge whether increase or decrease
//    Global resource dependence :
//    Author: jian.ma
//    Note:when *# align Left/Right，Left * decrease and right # increase;
//         when *# align up/down，up * increase and down # decrease;
***********************************************************************/
PUBLIC MMICOM_RETURN_OPTION_E MMIAPICOM_IsIncrease(uint32 key)
{
    MMICOM_RETURN_OPTION_E result = MMICOM_RETURN_NONE;
    
    switch(key) 
    {
    case MSG_KEYREPEAT_STAR:
    case MSG_APP_STAR:
#ifdef PLATFORM_SC6600R
        result = MMICOM_RETURN_INCREASE;    //*# align up/down
#else
        result = MMICOM_RETURN_DECREASE;    //*# align Left/Right
#endif    	
        break;
        
    case MSG_KEYREPEAT_HASH:
    case MSG_APP_HASH:
#ifdef PLATFORM_SC6600R
        result = MMICOM_RETURN_DECREASE;    //*# align up/down
#else
        result = MMICOM_RETURN_INCREASE;    //*# align Left/Right
#endif
        break;
        
    case MSG_KEYREPEAT_UP:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_APP_UP:
    case MSG_APP_UPSIDE:
        result = MMICOM_RETURN_INCREASE;
        break;
        
    case MSG_KEYREPEAT_DOWN:
    case MSG_KEYREPEAT_DOWNSIDE:
    case MSG_APP_DOWN:
    case MSG_APP_DOWNSIDE:
        result = MMICOM_RETURN_DECREASE;
        break;
        
    default:
        result = MMICOM_RETURN_NONE;
        break;
    }
    //SCI_TRACE_LOW:"MMICOM: key = 0x%x result= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_3500_112_2_18_1_58_16_41,(uint8*)"dd",key,result);
    return result;
}

#endif

#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
// 	Description : encapsulation of wstr to gb
//	Global resource dependence : none
//  Author: wei.zhou
//	Note: 
/*****************************************************************************/
LOCAL uint16 MMI_ConvertUCS2GB( const wchar *ucs_ptr, uint16 ucslength, uint8 *gb_ptr)
{
    return  GUI_WstrToGB(gb_ptr,ucs_ptr,ucslength /*ISHellCOM_Wstrlen(ucs_ptr)*/);
}

/*****************************************************************************/
// 	Description : encapsulation of gb to wstr
//	Global resource dependence : none
//  Author: wei.zhou
//	Note: 
/*****************************************************************************/
LOCAL uint16 MMI_ConvertGB2UCS(const uint8 *gb_ptr, uint16 length, wchar *ucs_ptr)
{
    return  GUI_GBToWstr(ucs_ptr,gb_ptr,length);
}

/*****************************************************************************/
// 	Description : encapsulation wstr to UTF8
//	Global resource dependence : 
//  Author: wei.zhou
//	Note:
/*****************************************************************************/
LOCAL uint16 MMI_ConvertUCS2B2UTF8( const wchar *ucs2b_ptr, uint16 ucs2b_len, uint8 *utf8_ptr, uint16 utf8_buf_len)
{
    return  GUI_WstrToUTF8(utf8_ptr,utf8_buf_len,ucs2b_ptr,ucs2b_len /*ISHellCOM_Wstrlen(ucs2b_ptr)*/);
}

/*****************************************************************************/
// 	Description : encapsulation UTF8 to wstr 
//	Global resource dependence : 
//  Author: wei.zhou
//	Note:
/*****************************************************************************/
LOCAL uint16 MMI_ConvertUTF82UCS2B( const uint8 *utf8_ptr, uint16 utf8_len, wchar *ucs2b_ptr, uint16 ucs2b_buf_len)
{
    return  GUI_UTF8ToWstr(ucs2b_ptr,ucs2b_buf_len/sizeof(wchar),utf8_ptr,utf8_len /*strlen(utf8_ptr)*/ );
}

/******************************************************************************/
/*! \brief convert ucs big endian to ucs little endian
 *  \param[out]		ucs2l_ptr		ucs2 little endian string pointer
 *  \param[in]		ucs2l_buf_len	ucs2 little endian string buffer len
 *  \param[in]		ucs2b_ptr		ucs2 big endian string pongiter
 *  \param[in]		ucs2b_len		ucs2 big endian string len
 *  \author  wei.zhou
 *  \return  uint16
 */
/******************************************************************************/
LOCAL uint16 MMI_ConvertUCS2BE2UCS2LE( const uint8 *ucs2b_ptr, uint16 ucs2b_len, uint8 *ucs2l_ptr, uint16 ucs2l_buf_len )
{
    return  GUI_UCS2B2UCS2L(ucs2l_ptr,ucs2l_buf_len,ucs2b_ptr,2*ucs2b_len /*ISHellCOM_Wstrlen(ucs2b_ptr)*/);
}

/******************************************************************************/
/*! \brief convert ucs little endian to ucs big endian
 *  \param[out]		ucs2b_ptr		ucs2 big endian string pointer
 *  \param[in]		ucs2b_buf_len	ucs2 big endian string buffer len
 *  \param[in]		ucs2l_ptr		ucs2 little endian string pongiter
 *  \param[in]		ucs2l_len		ucs2 little endian string len
 *  \author  wei.zhou
 *  \return  uint16
 */
/******************************************************************************/
LOCAL uint16 MMI_ConvertUCS2LE2UCS2BE( const uint8 *ucs2l_ptr, uint16 ucs2l_len, uint8 *ucs2b_ptr, uint16 ucs2b_buf_len )
{
    return  GUI_UCS2L2UCS2B(ucs2b_ptr,ucs2b_buf_len,ucs2l_ptr,(2*ucs2l_len));
}
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: (!!!) this function return free space the unit is K bytes not byte
//  return: return space size by K bytes
/*****************************************************************************/
PUBLIC uint32  MMI_GetFreeSpace(MMIFILE_DEVICE_E file_dev)
{
    uint32      free_space =   0;
    uint32      lWord      =   0;
    uint32      hWord      =   0;
    wchar*      dev_path_ptr = PNULL;
    uint16      dev_len = 0;

    dev_len = MMIAPIFMM_GetDevicePathLen(file_dev);
    dev_path_ptr =  MMIAPIFMM_GetDevicePath(file_dev);
    MMIAPIFMM_GetDeviceFreeSpace(
      dev_path_ptr,
      dev_len,
      &hWord,
      &lWord
      );

    free_space = ((hWord << 22) | (lWord >> 10));
#ifndef WIN32
    //free_space = 
    //SCI_TRACE_LOW:"MMI_GetFreeSpace: free_space = %d K bytes"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_3594_112_2_18_1_58_16_42,(uint8*)"d", free_space);
    return (free_space);
#else
        return (20*1024*1024);
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:zhaohui
//  Note: get user agent
//  return: 
/*****************************************************************************/
PUBLIC char* MMIAPICOM_GetUserAgent(void)
{
#ifdef CSC_XML_SUPPORT
    char * user_agent_ptr = (char *)MMIAPICUS_GetDefaultUAFromCSC();

    if (PNULL != user_agent_ptr)
    {
        return user_agent_ptr;
    }
#endif

    return (char*)MMI_DEFAULT_UA;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:kelly.li
//  Note: get user agent profile info
//  return: function caller should not free the buffer
/*****************************************************************************/
PUBLIC char* MMIAPICOM_GetUserAgentProfile(void)
{
#ifdef CSC_XML_SUPPORT
    char * user_agent_profile_ptr = (char *)MMIAPICUS_GetDefaultUAProfileFromCSC();

    if (PNULL != user_agent_profile_ptr)
    {
        return user_agent_profile_ptr;
    }
#endif

    return (char*)MMI_CUSTOMER_UA_PROFILE_FIELD;
}

/*****************************************************************************/
//     Description : convert win rect
//    Global resource dependence : 
//  Author:bin.ji
//    Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMI_ConvertWinRect( 
                              MMICOM_WINPOS_CONVERT_E convert_type,
                              MMI_HANDLE_T   win_handle,
                              GUI_RECT_T rect
                              )
{
    // BOOLEAN result = FALSE;
    GUI_RECT_T out_rect = {0};
    GUI_RECT_T win_rect = {0};
    // GUI_LCD_DEV_INFO lcd_dev_info = {0};
    // int16 offset_x = 0;
    // int16 offset_y = 0;
    
    if (MMK_GetWinRect(win_handle, &win_rect))
    {
        switch (convert_type)
        {
        case MMICOM_WINPOS_WIN2DISP:
        case MMICOM_WINPOS_WIN2LCD:
            out_rect.left = rect.left + win_rect.left;
            out_rect.top = rect.top + win_rect.top;
            out_rect.right = rect.right + win_rect.left;
            out_rect.bottom = rect.bottom + win_rect.top;
            break;

        case MMICOM_WINPOS_DISP2WIN:
        case MMICOM_WINPOS_LCD2WIN:
            out_rect.left = rect.left - win_rect.left;
            out_rect.top = rect.top - win_rect.top;
            out_rect.right = rect.right - win_rect.left;
            out_rect.bottom = rect.bottom - win_rect.top;            
            break;
/*
        case MMICOM_WINPOS_WIN2LCD:
            MMK_GetWinLcdDevInfo(win_handle, &lcd_dev_info);
            UILAYER_GetLayerPosition(&lcd_dev_info, &offset_x, &offset_y);
            out_rect.left = rect.left + win_rect.left + offset_x;
            out_rect.top = rect.top + win_rect.top + offset_y;
            out_rect.right = rect.right + win_rect.left + offset_x;
            out_rect.bottom = rect.bottom + win_rect.top + offset_y;            
            break;
            
        case MMICOM_WINPOS_LCD2WIN:
            MMK_GetWinLcdDevInfo(win_handle, &lcd_dev_info);
            UILAYER_GetLayerPosition(&lcd_dev_info, &offset_x, &offset_y);
            out_rect.left = rect.left - win_rect.left - offset_x;
            out_rect.top = rect.top - win_rect.top - offset_y;
            out_rect.right = rect.right - win_rect.left - offset_x;
            out_rect.bottom = rect.bottom - win_rect.top - offset_y;            
            break;
            */
            
        default:
            break;
        }
    }
    return out_rect;
}

/*****************************************************************************/
//     Description : convert win point
//    Global resource dependence : 
//  Author:bin.ji
//    Note:
/*****************************************************************************/
PUBLIC GUI_POINT_T MMI_ConvertWinPoint( 
                              MMICOM_WINPOS_CONVERT_E convert_type,
                              MMI_HANDLE_T   win_handle,
                              GUI_POINT_T point
                              )
{
    // BOOLEAN result = FALSE;
    GUI_POINT_T out_point = {0};
    GUI_RECT_T win_rect = {0};
    // GUI_LCD_DEV_INFO lcd_dev_info = {0};
    // int16 offset_x = 0;
    // int16 offset_y = 0;

    if (MMK_GetWinRect(win_handle, &win_rect))
    {
        switch (convert_type)
        {
        case MMICOM_WINPOS_WIN2DISP:
        case MMICOM_WINPOS_WIN2LCD:
            out_point.x = point.x + win_rect.left;
            out_point.y = point.y + win_rect.top;
            break;

        case MMICOM_WINPOS_DISP2WIN:
        case MMICOM_WINPOS_LCD2WIN:
            out_point.x = point.x - win_rect.left;
            out_point.y = point.y - win_rect.top;
            break;

            /*
        case MMICOM_WINPOS_WIN2LCD:
            MMK_GetWinLcdDevInfo(win_handle, &lcd_dev_info);
            UILAYER_GetLayerPosition(&lcd_dev_info, &offset_x, &offset_y);
            out_point.x = point.x + win_rect.left + offset_x;
            out_point.y = point.y + win_rect.top + offset_y;
            break;

        case MMICOM_WINPOS_LCD2WIN:
            MMK_GetWinLcdDevInfo(win_handle, &lcd_dev_info);
            UILAYER_GetLayerPosition(&lcd_dev_info, &offset_x, &offset_y);
            out_point.x = point.x - win_rect.left - offset_x;
            out_point.y = point.y - win_rect.top - offset_y;
            break;
            */
            
        default:
            break;
        }
    }
    return out_point;
}

/*****************************************************************************/
// Description: The MMIAPICOM_bcdInbcdHead function number2 in number1 from head. 
// Parameters:  number1: bcd number string to cmp 
//              number2:bcd number string to cmp 
// Return:      TRUE:number2 in number1 from head; FALSE: number2 not in number1 from head
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_bcdInbcdHead(const MMIPB_BCD_NUMBER_T *number1, MMIPB_BCD_NUMBER_T *number2)
{
	BOOLEAN result = TRUE;
    uint16  i = 0;
	uint8   bcd_val = 0;
	uint8   bcd_set = 0;

	if(PNULL == number1 || PNULL == number2)
	{
		//parameter error
        return FALSE;
	}
	if(number1->number_len < number2->number_len)
	{
		//bcd_numset large than bcd_number or number type not same
		return FALSE;
	}
	for(i = 0; i < (number2->number_len << 1); i++)
	{
		 bcd_val = (i & 1) ? ((number1->number[i/2] &0xF0) >> 4) :number1->number[i/2] &0x0F;
		 bcd_set = (i & 1) ? ((number2->number[i/2] &0xF0 )>> 4) : number2->number[i/2] &0x0F;
         if(bcd_set == 0x0F)
         {
             //number end
             break;

         }
         if(bcd_val != bcd_set)
		 {
             result = FALSE;
             break;
         }

	}
	return result;
}


/*****************************************************************************/
// Description: The MMIAPICOM_bcdsstr function returns a pointer to the first occurrence of 
//              bcd_strset in bcd_string. 
// Parameters:  bcd_number: bcd number string to search 
//              bcd_numset:bcd number string to search for 
// Return:      TRUE: find bcd_numset in bcd_number; FALSE:not find bcd_numset in bcd_number
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_bcdsstr(const MMIPB_BCD_NUMBER_T *bcd_number, MMIPB_BCD_NUMBER_T *bcd_numset)
{
	BOOLEAN result = FALSE;
    	uint16  i = 0;
	uint16  numset_index = 0;
	uint16  num_index = 0;
	uint8   bcd_val = 0;
	uint8   bcd_set = 0;
	if(PNULL == bcd_number || PNULL == bcd_numset)
	{
		//parameter error
        return result;
	}
	if((bcd_number->number_len < bcd_numset->number_len))
	{
		//bcd_numset large than bcd_number or number type not same
		return result;
	}
	for(i = 0; (num_index < bcd_number->number_len*2) && (numset_index < bcd_numset->number_len*2); )
	{
		 bcd_val = (num_index & 1) ? ((bcd_number->number[num_index/2] &0xF0) >> 4) : bcd_number->number[num_index/2] &0x0F;
		 bcd_set = (numset_index & 1) ? ((bcd_numset->number[numset_index/2] &0xF0 )>> 4) : bcd_numset->number[numset_index/2] &0x0F;
         if(bcd_val == bcd_set&&bcd_set != 0x0F)/***&&bcd_set != 0x0F add by liuxiaohua for NEWMS110234 at 20110804*****/
		 {
			 //compare next bcd bytes
			 num_index++;
			 numset_index++;
		 }
		 else if(bcd_set == 0x0F)
		 {
			 //number end
        //      numset_index++;/***change by liuxiaohua for NEWMS110234 at 20110804*****/
			  break;
 		 }
		 else if(bcd_val == 0x0F)
		 {
			 //cmp end
			  break;
 		 }
		 else
		 {
			 //compare from bcd_numset head
			 i++;
			 num_index = i;
			 numset_index = 0;
		 }
	}
	if(bcd_numset->number_len > 0)
	{
		uint8 bcd_num_len = (((bcd_numset->number[bcd_numset->number_len-1])&0xF0) == 0xF0) ? (bcd_numset->number_len*2 -1):(bcd_numset->number_len*2);

   	 	if(numset_index ==  bcd_num_len)
    		{
       			result = TRUE;
     		}
	}
	return result;
}
/*****************************************************************************/
//  Description : using bin search to find the record in list
//  Global resource dependence : 
//  Parameter: search_info:(IN) search_info->start_pos:the start index to search;
//                              search_info->end_pos:the end index of search;
//                              search_info->b_is_backward:search from ahead or not;  
//             func_t:(IN)      func_t->func: compared function,this is a callback function,
//                                            be defined by every caller;
//                              func_t->compare_data_2:(it can be null) the data(string or value) to be compare with list
//                              func_t->list_ptr:(it can be null)search list
//             pos:             if find,it is the postion of searched record; 
//                              if not find, it is the postion of the first larger to func_t->compare_data_2  
//  Note: 
//  return : 0: find this record in func_t->list_ptr, !=0 not find in func_t->list_ptr      
/*****************************************************************************/
PUBLIC int MMIAPICOM_BinSearch(MMI_BIN_SEARCH_INFO_T *search_info, //[IN]
                           BIN_COMPARE_FUNC func, //[IN]
                           uint32 *pos, //[OUT]
                           void *list//[IN]
                           )
{
	BOOLEAN         is_in_list  = FALSE;    // 记录是否列表中有完全一样的姓名
	uint32	        ret_pos         = 0;        // 记录找到的合适位置
    int32          low_pos	    = 0;
    int32          mid_pos	    = 0;        // 二分法的中间位置
   	int32          high_pos	= 0;
	int 	        cmp_result  = 0;  // 两个字符串的比较结果

    if(PNULL == search_info || (NULL == func) || (NULL == pos))
    {
        //SCI_TRACE_LOW:"[mmi_common.c] MMIAPICOM_BinSearch search_info 0x%x  func 0x%x !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_3890_112_2_18_1_58_17_43,(uint8*)"dd", search_info, func);
        return FALSE;
    } 
	low_pos	    = search_info->start_pos;        // 二分法的前一个位置
	high_pos	= search_info->end_pos;        // 二分法的后一个位置
	//func = func_t->func;
	// 二分法查找
	//SCI_TRACE_LOW("MMIPB_BinSearch low_pos %d, high_pos %d",low_pos,high_pos);
	while (low_pos <= high_pos)	// 满足二分法的条件
	{
	    // 获得mid_pos
	    mid_pos = ((low_pos + high_pos) >> 1); 
        // 将两个字符串进行比较//
        cmp_result = (*func)(mid_pos,search_info->compare_base_data, list);
        // 根据比较的结果调整查找的范围
        if (cmp_result < 0)
      	{
          	low_pos = (mid_pos + 1);
      	}
      	else if (cmp_result > 0)
      	{
			if(mid_pos == 0)
			{
				break;
 			}
          	high_pos = mid_pos -1;
        }
        else // begin (0 == cmp_result)
        {
            // 表示列表中有完全一样的记录。
            // 记录目前的位置，并标记已经找到这样的记录。
        	is_in_list  = TRUE;
        	ret_pos     = mid_pos;
            
            // 查找操作
            // 查找时，需要根据往前找，还是往后找，得到合适的查找范围  
            if (MMICOM_SEARCH_LAST_EQUAL == search_info->search_type)
        	{
                low_pos = (mid_pos + 1);
            }
            else if(MMICOM_SEARCH_FIRST_EQUAL == search_info->search_type)
            {
            	// 很有可能不是列表中满足条件的最前面一条记录，需要继续寻找
                if(mid_pos == 0)
				{
					break;
				}
				high_pos = (mid_pos - 1);	
            }
            else
            {
                //找到任意一个相等的就退出
                break;
            }

        } // end (0 == cmp_result)
    } // end of while

    if (!is_in_list)
    {
        if(cmp_result < 0)
        {
            ret_pos = low_pos;
        }
        else
        {
            ret_pos = mid_pos;
        }
    }
	else
	{
		cmp_result = 0;
	}
    *pos = ret_pos;
	//SCI_TRACE_LOW("MMIPB_BinSearch is_in_list %d, mid_pos %d high pos %d",is_in_list,mid_pos, high_pos);
	//SCI_TRACE_LOW:"[mmi_common.c] MMIAPICOM_BinSearch cmp_result %d, ret_pos %d is_in_list %d mid_pos %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_3963_112_2_18_1_58_17_44,(uint8*)"dddd",cmp_result,ret_pos, is_in_list, mid_pos);
    return cmp_result;    
}

/*****************************************************************************/
//     Description : put dword
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
LOCAL void utilPutDword(uint8 *p, uint32 value)
{
    if (PNULL == p)
    {
        //buffer为空
        return;
    }
  *p++ = value & 255;
  *p++ = (value >> 8) & 255;
  *p++ = (value >> 16) & 255;
  *p = (value >> 24) & 255;
}

/*****************************************************************************/
//     Description : 将原始图像数据输出为bmp格式图片
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPICOM_WriteBMPFile(const wchar *u16filename, uint32 w, uint32 h, uint8 *pix, uint32 ColorDepth)
{
    //uint8 writeBuffer[512 * 10 * 3];
    uint8 *writeBuffer = PNULL;
    //uint32 fsz = sizeof(bmpheader) + w*h*3; 
    //FILE *fp = fopen(fileName,"wb");
    MMIFILE_HANDLE fp = 0;
    //uint8 *b = writeBuffer;
    uint8 *b = PNULL;
    uint32 sizeX = w;
    uint32 sizeY = h;
    uint32 x = 0, y = 0,z=0;
    uint32 write_size = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;    
    //int nLineBytes = ((nWidth*nBitCnt+31)>>5)<<2;
    uint32 fsz = 0; 
    uint32 nLineBytes = 0;
    const uint32 nBitCnt = 24;
    uint32 mod_count=0;
    uint16 writecount=WRITE_BMP_COUNT;
    SCI_TRACE_LOW("MMIAPICOM_WriteBMPFile entry !!!");
    if (PNULL == pix || PNULL == u16filename || w==0 || h==0)
    {
        //buffer为空
        return FALSE;
    }
    fp = MMIAPIFMM_CreateFile(u16filename, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
    
    if(SFS_INVALID_HANDLE == fp)
    {
        //SCI_TRACE_LOW:"MMIAPIPTSCR_WriteBMPFile MMIAPIFMM_CreateFile fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_4018_112_2_18_1_58_17_45,(uint8*)"");
        return FALSE;
    }
#if 0   //输出原始数据
    file_ret = MMIAPIFMM_WriteFile(fp, pix, w*h*(ColorDepth>>3), &write_size, PNULL);
    MMIAPIFMM_CloseFile(fp);
    return TRUE;
#endif

    SCI_MEMSET(&BMPheader, 0, sizeof(BMPheader));
    
    BMPheader.ident[0] = 'B';
    BMPheader.ident[1] = 'M';

    nLineBytes = ((w*nBitCnt+31)>>5)<<2;
    fsz = sizeof(BMPheader) + nLineBytes*h; 
    utilPutDword(BMPheader.filesize, fsz);
    utilPutDword(BMPheader.dataoffset, 0x36);
    utilPutDword(BMPheader.headersize, 0x28);
    utilPutDword(BMPheader.width, w);
//    utilPutDword(bmpheader.height, h);
    utilPutDword(BMPheader.height, h);
    utilPutDword(BMPheader.planes, 1);
    utilPutDword(BMPheader.bitsperpixel, 24);
    utilPutDword(BMPheader.datasize, 3*w*h);
    
//    fwrite(&bmpheader, 1, sizeof(bmpheader), fp);
    file_ret = MMIAPIFMM_WriteFile(fp, &BMPheader, sizeof(BMPheader), &write_size, PNULL);
	if(SFS_ERROR_NONE != file_ret)
	{
	    MMIAPIFMM_CloseFile(fp);
		return FALSE;
	}
    if (nLineBytes > WRITE_BMPBUFFER_SIZE)
    {
        //SCI_TRACE_LOW:"MMIAPIPTSCR_WriteBMPFile WriteBMPFile bmp is to large! w = %d, nLineBytes = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_4053_112_2_18_1_58_17_46,(uint8*)"dd", w, nLineBytes);
        MMIAPIFMM_CloseFile(fp);
        return FALSE;
    }

    if(ColorDepth == 16 && (h >= WRITE_BMP_COUNT))
    {
        writeBuffer = SCI_ALLOCA(nLineBytes * h/WRITE_BMP_COUNT);
        mod_count=h % WRITE_BMP_COUNT;
        if(mod_count != 0)
        {
            writecount=WRITE_BMP_COUNT+1;//有时候 WRITE_BMP_COUNT 不能完全写完 ，需要把剩余的多写一次  ,例如h为241.
        }
    }
    else
    {
        writeBuffer = SCI_ALLOCA(nLineBytes );
    }
    
    b = writeBuffer;
    if (PNULL == writeBuffer)
    {
        //SCI_TRACE_LOW:"MMIAPIPTSCR_WriteBMPFile WriteBMPFile alloc fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_4075_112_2_18_1_58_17_47,(uint8*)"");
        MMIAPIFMM_CloseFile(fp);
        return FALSE;
    }
    //  switch(systemColorDepth) 
    switch (ColorDepth)
    {
    case 16:
        {
        if(h >= WRITE_BMP_COUNT)
        {
            uint16 *p = (uint16 *)(pix+(w)*(h-1)*2); // skip first black line
            uint8 *per_nline=writeBuffer;
            uint16 cur_writerow=sizeY/WRITE_BMP_COUNT;
            uint32 offset=0;
            for(z=0;z<writecount;z++)
            {
                per_nline=writeBuffer;
                if(z == writecount-1 && mod_count != 0)
                {
                    cur_writerow = mod_count;//写最后还没有写完的几行 
                }
                for( y = 0; y < cur_writerow; y++) 
                {
                    b=per_nline;
                    for( x = 0; x < sizeX; x++) 
                    {
                        uint16 v = *p++;
                        *b++ = ((v >> systemBlueShift) & 0x01f) << 3; // B      
                        *b++ = ((v >> systemGreenShift) & 0x001f) << 3; // G 
                        *b++ = ((v >> systemRedShift) & 0x001f) << 3; // R
                    }
                    per_nline+=nLineBytes;
                    p -= 2*(w);            
                }

                offset=nLineBytes * cur_writerow;
                file_ret = MMIAPIFMM_WriteFile(fp, writeBuffer, offset, &write_size, PNULL);

                if(SFS_ERROR_NONE != file_ret)
                {
                SCI_FREE(writeBuffer);
                MMIAPIFMM_CloseFile(fp);
                SCI_TRACE_LOW("MMIAPICOM_WriteBMPFile exit,file_ret:%d !!!",file_ret); 
                return FALSE;
                }   

                }

            }
        else
            {
                uint16 *p = (uint16 *)(pix+(w)*(h-1)*2); // skip first black line
                for( y = 0; y < sizeY; y++) {
                    for( x = 0; x < sizeX; x++) {
                    uint16 v = *p++;
                    
                    *b++ = ((v >> systemBlueShift) & 0x01f) << 3; // B      
                    *b++ = ((v >> systemGreenShift) & 0x001f) << 3; // G 
                    *b++ = ((v >> systemRedShift) & 0x001f) << 3; // R
                }
                p -= 2*(w);
//                fwrite(writeBuffer, 1, 3*w, fp);
                file_ret = MMIAPIFMM_WriteFile(fp, writeBuffer, nLineBytes/*3*w*/, &write_size, PNULL);
	            if(SFS_ERROR_NONE != file_ret)
	            {
                    SCI_FREE(writeBuffer);
                    MMIAPIFMM_CloseFile(fp);
		            return FALSE;
	            }                
                b = writeBuffer;
                }
            } 
        }
        break;
    case 24:
        {
            uint8 *pixU8 = (uint8 *)pix+3*w*(h-1);
            for( y = 0; y < sizeY; y++) {
                for( x = 0; x < sizeX; x++) {
                        *b++ = *pixU8++; // B
                        *b++ = *pixU8++; // G
                        *b++ = *pixU8++; // R                         
/*
                    if(systemRedShift > systemBlueShift) {
                        *b++ = *pixU8++; // B
                        *b++ = *pixU8++; // G
                        *b++ = *pixU8++; // R           
                    } else {
                        uint32 red = *pixU8++;
                        uint32 green = *pixU8++;
                        uint32 blue = *pixU8++;
                        
                        *b++ = blue;
                        *b++ = green;
                        *b++ = red;
                    }
                    */
                }
                pixU8 -= 2*3*w;
//                fwrite(writeBuffer, 1, 3*w, fp);
                file_ret = MMIAPIFMM_WriteFile(fp, writeBuffer, nLineBytes, &write_size, PNULL);
	            if(SFS_ERROR_NONE != file_ret)
	            {
                    SCI_FREE(writeBuffer);
                    MMIAPIFMM_CloseFile(fp);
		            return FALSE;
	            }                
                b = writeBuffer;
            }
        }
        break;
    case 32:
        {
            uint32 *pixU32 = (uint32 *)(pix+4*w*(h-1));
            for( y = 0; y < sizeY; y++) {
                for( x = 0; x < sizeX; x++) {
                    uint32 v = *pixU32++;
                    
                    *b++ = ((v >> color32systemBlueShift) & 0x00ff); // B     
                    *b++ = ((v >> color32systemGreenShift) & 0x00ff); // G
                    *b++ = ((v >> color32systemRedShift) & 0x00ff); // R
                }
                pixU32 -= 2*w;                
//                fwrite(writeBuffer, 1, 3*w, fp);
                file_ret = MMIAPIFMM_WriteFile(fp, writeBuffer, nLineBytes, &write_size, PNULL);
	            if(SFS_ERROR_NONE != file_ret)
	            {
                    SCI_FREE(writeBuffer);
                    MMIAPIFMM_CloseFile(fp);
		            return FALSE;
	            }
                
                b = writeBuffer;
            }
        }
        break;
    default:
        SCI_FREE(writeBuffer);
        MMIAPIFMM_CloseFile(fp);
        return FALSE;
    }
    SCI_FREE(writeBuffer);
    MMIAPIFMM_CloseFile(fp);
    SCI_TRACE_LOW("MMIAPICOM_WriteBMPFile exit !!!"); 
    return TRUE;
}

#ifdef MMI_DEBUGLAYER_SUPPORT

/*****************************************************************************/
//  Description : create debug layer
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_CreateDebugLayer(void)
{
    UILAYER_CREATE_T create_info = {0}; 
    GUI_RECT_T main_rect = MMITHEME_GetFullScreenRect(); 
    UILAYER_APPEND_BLT_T append_layer = {0};

//    if (  (UILAYER_NULL_HANDLE == s_debug_layer.block_id)
//        ||(UILAYER_MAIN_HANDLE == s_debug_layer.block_id))
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType((GUI_LCD_DEV_INFO *)s_debug_layer))
    {
        create_info.lcd_id          = LCD_ID_0;
        create_info.owner_handle    = VIRTUAL_WIN_ID;   //MAIN_IDLE_WIN_ID;
        create_info.offset_x        = 0;
        create_info.offset_y        = 0;
        create_info.width           = (main_rect.right+1-main_rect.left);
        create_info.height          = (main_rect.bottom+1-main_rect.top);
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = TRUE;   
        
        UILAYER_CreateLayer(&create_info, &s_debug_layer);
            
        UILAYER_SetLayerColorKey(&s_debug_layer, TRUE, UILAYER_TRANSPARENT_COLOR);
    }
    
    if (UILAYER_IsLayerActive(&s_debug_layer))
    {
        UILAYER_Clear(&s_debug_layer);
        append_layer.lcd_dev_info = s_debug_layer;
        append_layer.layer_level = UILAYER_LEVEL_HIGH;
        UILAYER_AppendBltLayer(&append_layer);  
    }
}

/*****************************************************************************/
//  Description : get debug layer handle
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
LOCAL const GUI_LCD_DEV_INFO *GetDebugLayer(void)
{
    return &s_debug_layer;
}

/*****************************************************************************/
//  Description : output debug text
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_OutputDebugText(
                           uint32 line_index,
                           const MMI_STRING_T *str_ptr
                           )
{
    GUI_RECT_T rect = {0};
    GUISTR_STATE_T temp_state = GUISTR_STATE_NONE;//GUISTR_STATE_INVERSE;
    GUISTR_STYLE_T temp_style = {0};
    const GUI_LCD_DEV_INFO *debug_layer_ptr = GetDebugLayer();
        
    if (line_index >= MMI_DEBUGTEXT_MAX_LINE_NUM)
    {
        return;
    }
    rect = s_debug_text_rect[line_index];
    if (PNULL == debug_layer_ptr || PNULL == str_ptr)
    {
        return;
    }
//    if (  (UILAYER_NULL_HANDLE == debug_layer_ptr->block_id)
//        ||(UILAYER_MAIN_HANDLE == debug_layer_ptr->block_id))
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType((GUI_LCD_DEV_INFO *)debug_layer_ptr))
    {
        return;
    }
    temp_style.font = MMI_DEBUGTEXT_FONT;
    temp_style.font_color = MMI_DEBUGTEXT_FONT_COLOR;
   
    LCD_FillRect(debug_layer_ptr, rect, MMI_DEBUGTEXT_BG_COLOR);
//    LCD_DrawRect(debug_layer_ptr, rect, MMI_DEBUGTEXT_BORDER_COLOR);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)debug_layer_ptr,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)str_ptr,
        &temp_style,
        temp_state,
        GUISTR_TEXT_DIR_AUTO
        );
}

#endif
#ifndef WRE_SUPPORT
PUBLIC int32 wre_in_camera_scan_mode(void)
{
    return 0;
}
PUBLIC void wre_camera_snap_callback(uint32 ptr)
{
    ptr = ptr;
    return;
}
#endif

/*****************************************************************************/
//  Description : Get the audio priority by audio type.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_PRI_T MMIAPICOM_GetAudioPri(char *audio_type_str)
{
    uint32 i = 0;
    MMISRVAUD_PRI_T pri = 0;

    //SCI_TRACE_LOW:"MMIAPICOM_GetAudioPri entry: audio_type_str=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_4336_112_2_18_1_58_18_48,(uint8*)"d", audio_type_str);
    
    if(PNULL == audio_type_str)
    {
        return MMISRVAUD_PRI_NORMAL;
    }
    while(s_aud_pri_table[i].audio_type_str != PNULL)
    {
        if(strcmp(audio_type_str, s_aud_pri_table[i].audio_type_str) == 0)
        {
            pri = s_aud_pri_table[i].pri;
            break;
        }
        i++;
    }
    
    //SCI_TRACE_LOW:"MMIAPICOM_GetAudioPri exit: pri=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_COMMON_4352_112_2_18_1_58_18_49,(uint8*)"d", pri);
    
    return pri;
}


/*****************************************************************************/
//  Description : get data and time string.
//  Global resource dependence : 
//  Author:baokun.yin
//  Note: yearmonday_hourminsec
/*****************************************************************************/
PUBLIC wchar *MMIAPICOM_GetDataTimeString(wchar *datatime_str,//OUT
                                          uint16 str_max_len
                                          )
{
    SCI_DATE_T sys_date = {0};
    SCI_TIME_T sys_time = {0};
    char datatime_char[20] = {0};
    
    if(datatime_str != PNULL)
    {
        TM_GetSysDate(&sys_date);
        TM_GetSysTime(&sys_time);  
        
        //生成文件名   
        sprintf(datatime_char, 
            "%04d%02d%02d_%02d%02d%02d",
            sys_date.year,
            sys_date.mon,
            sys_date.mday, 
            sys_time.hour,
            sys_time.min, 
            sys_time.sec);
        if(strlen(datatime_char) < str_max_len)
        {
            MMI_STRNTOWSTR(
                datatime_str,
                str_max_len,
                (uint8 *)datatime_char,
                20,
                strlen(datatime_char)
                );
        }
    }

    return datatime_str;
}


/*****************************************************************************/
// 	Description : convert qwertykey msg to typicalkey msg
//	Global resource dependence : 
//  Author: aoke.hu
//	Note:   
/*****************************************************************************/
PUBLIC MMI_MESSAGE_ID_E MMIAPICOM_ConvertKeyMsg(MMI_MESSAGE_ID_E msg_id)
{
    if (MMK_IsKeyMsg( msg_id))
    {
        uint8 key_code = msg_id & 0xff;

        key_code = MMIDEFAULT_ConvertQwertyKeyToTypicalKey(key_code);

        msg_id = (msg_id & 0xff00) | key_code;
    }

    return msg_id;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:juan.wu
//  Note: The following code can change a string to MD5 
/*****************************************************************************/
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

LOCAL void Encode (unsigned char *output, uint32 *input, unsigned int len)
{
  unsigned int i, j;

  for (i = 0, j = 0; j < len; i++, j += 4) {
      output[j] = (unsigned char)(input[i] & 0xff);
      output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);    /*lint !e661 */
      output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);   /*lint !e661 !e662 */
      output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);   /*lint !e661 !e662 */
  }
}

LOCAL void MD5_memcpy (char * output, char * input, unsigned int len)
{
  unsigned int i;

  for (i = 0; i < len; i++)
 		output[i] = input[i];
}

/* Note: Replace "for loop" with standard memset if possible.
 */
LOCAL void MD5_memset (char * output, int value, unsigned int len)
{
  unsigned int i;

  for (i = 0; i < len; i++)
 ((char *)output)[i] = (char)value;
}

LOCAL void Decode (uint32 *output, unsigned char *input, unsigned int len)
{
  unsigned int i, j;

  for (i = 0, j = 0; j < len; i++, j += 4)
 output[i] = ((uint32)input[j]) | (((uint32)input[j+1]) << 8) |   /*lint !e661 */
   (((uint32)input[j+2]) << 16) | (((uint32)input[j+3]) << 24);   /*lint !e661 !e662 */
}

LOCAL void MD5Transform (uint32 state[4], unsigned char block[64])
{
  uint32 a = state[0], b = state[1], c = state[2], d = state[3], x[16];

  Decode (x, block, 64);

  /* Round 1 */
  FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
  FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
  FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
  FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
  FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
  FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
  FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
  FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
  FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
  FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
  FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
  FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
  FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
  FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
  FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
  FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

 /* Round 2 */
  GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
  GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
  GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
  GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
  GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
  GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
  GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
  GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
  GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
  GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
  GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
  GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
  GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
  GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
  GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
  GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

  /* Round 3 */
  HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
  HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
  HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
  HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
  HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
  HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
  HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
  HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
  HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
  HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
  HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
  HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
  HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
  HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
  HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
  HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

  /* Round 4 */
  II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
  II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
  II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
  II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
  II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
  II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
  II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
  II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
  II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
  II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
  II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
  II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
  II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
  II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
  II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
  II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;

  /* Zeroize sensitive information.*/
  MD5_memset ((char *)x, 0, sizeof (x));
}

LOCAL void MD5Init (MD5_CONTEXT *context)
{
  context->count[0] = context->count[1] = 0;
  context->state[0] = 0x67452301;
  context->state[1] = 0xefcdab89;
  context->state[2] = 0x98badcfe;
  context->state[3] = 0x10325476;
}
LOCAL void MD5Update (MD5_CONTEXT *context, unsigned char *input, unsigned int inputLen)
{
  unsigned int i, index, partLen;
  index = (unsigned int)((context->count[0] >> 3) & 0x3F);
  if ((context->count[0] += ((uint32)inputLen << 3))
   < ((uint32)inputLen << 3))
 context->count[1]++;
  context->count[1] += ((uint32)inputLen >> 29);

  partLen = 64 - index;

  if (inputLen >= partLen) {
 MD5_memcpy
   ((char *)&context->buffer[index], (char *)input, partLen);
 MD5Transform (context->state, context->buffer);

 for (i = partLen; i + 63 < inputLen; i += 64)
   MD5Transform (context->state, &input[i]);

 index = 0;
  }
  else
 i = 0;

  /* Buffer remaining input */
  MD5_memcpy
 ((char *)&context->buffer[index], (char *)&input[i],
  inputLen-i);
}

void FileMD5Update (MD5_CONTEXT *context, MMIFILE_HANDLE fd, unsigned int offset,unsigned int dataLen)
{
  unsigned int i, index, partLen;
  uint32 tmpbuflen = 512*1024;
  uint32 read_bytes = 0, readed_bytes = 0, offset_size = 0, have_read_size = 0, left_bytes = 0;
  uint8 *input = NULL;
  
  index = (unsigned int)((context->count[0] >> 3) & 0x3F);
  if ((context->count[0] += ((uint32)dataLen << 3))< ((uint32)dataLen << 3))
	context->count[1]++;
  context->count[1] += ((uint32)dataLen >> 29);

  partLen = 64 - index;
SCI_TRACE_LOW("parlen:%d", partLen);
  /*read file*/
	offset_size = offset;
       left_bytes = dataLen;
	read_bytes = MIN(tmpbuflen, left_bytes);
	
	input = SCI_ALLOC_APPZ(read_bytes+10);
       SCI_ASSERT(NULL != input);
	   
	MMIAPIFMM_SetFilePointer(fd ,offset_size ,MMIFILE_SEEK_BEGIN);
	if(SFS_ERROR_NONE ==MMIAPIFMM_ReadFile(fd, input , read_bytes,&readed_bytes, PNULL))
	{
		SCI_TRACE_LOW("[baidu] file md5 update. readfile ok. readbyte:%d", readed_bytes);
	}
	have_read_size = readed_bytes;
	//MMIAPIFMM_CloseFile(fd);
  
  if (dataLen >= partLen) 
  {
	 MD5_memcpy
	   ((char *)&context->buffer[index], (char *)input, partLen);
	 MD5Transform (context->state, context->buffer);
        
	 for (i = partLen; i + 63 < readed_bytes; i += 64)
	 {
		MD5Transform (context->state, &input[i]);
	 }
        while(have_read_size < dataLen)
        {
		offset_size += i;
		left_bytes   -= i;
		if(i < readed_bytes)
		{
			have_read_size -= (readed_bytes - i);
		}
	       SCI_TRACE_LOW("have_read_size:%d \n", have_read_size);
		   
		read_bytes = MIN(tmpbuflen, left_bytes);  
        	MMIAPIFMM_SetFilePointer(fd ,offset_size ,MMIFILE_SEEK_BEGIN);
	       if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(fd, input , read_bytes,&readed_bytes, PNULL))
	       {
	       	SCI_TRACE_LOW("[baidu] file md5 update. readfile ok. readbyte:%d", readed_bytes);
	       }
		have_read_size += readed_bytes;
		//MMIAPIFMM_CloseFile(fd);
		//offset_size += readed_bytes;
		//left_bytes -= readed_bytes;
		SCI_TRACE_LOW("have_read_size:%d, offset_size:%d, left_bytes:%d \n", have_read_size, offset_size, left_bytes);
		 for (i=0; i + 63 < readed_bytes; i += 64)
		 {
			MD5Transform (context->state, &input[i]);
		 } 
    	}
	 index = 0;
	 left_bytes   -= i;
  }
  else
 i = 0;

  /* Buffer remaining input */
  MD5_memcpy
 ((char *)&context->buffer[index], (char *)&input[i],
  left_bytes);
  SCI_FREE(input);
}
/* MD5 finalization. Ends an MD5 message-digest operation, writing the
  the message digest and zeroizing the context.
 */
/* message digest */
/* context */
LOCAL void MD5Final (unsigned char digest[16], MD5_CONTEXT *context)
{
  unsigned char bits[8];
  unsigned int index, padLen;

  /* Save number of bits */
  Encode (bits, context->count, 8);

  /* Pad out to 56 mod 64. */
  index = (unsigned int)((context->count[0] >> 3) & 0x3f);
  padLen = (index < 56) ? (56 - index) : (120 - index);
  MD5Update (context, PADDING, padLen);

  /* Append length (before padding) */
  MD5Update (context, bits, 8);

  /* Store state in digest */
  Encode (digest, context->state, 16);

  /* Zeroize sensitive information.
*/
  MD5_memset ((char *)context, 0, sizeof (*context));
}
/*****************************************************************************/
// 	Description : Convert the stream to MD5 context, use MD5 arithmetic
//	Global resource dependence : 
//  Author:juan.wu
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIAPICOM_ProduceSgnMD5Str(uint8 *secreat, int32 scrlen, uint8 *sign)
{
    MD5_CONTEXT context;
    
    MD5Init (&context);
    MD5Update (&context, secreat, scrlen);
    MD5Final (sign, &context);

    return 0;
}
/**********************MD5 CHANGE END***********************************/
void Baidu_MD5Init (MD5_CONTEXT *context)
{
	MD5Init(context);
}

void Baidu_MD5Update (MD5_CONTEXT *context, unsigned char *input, unsigned int inputLen)
{
	MD5Update(context, input, inputLen);
}
void Baidu_MD5Final (unsigned char digest[16], MD5_CONTEXT *context)
{
	MD5Final(digest, context);
}

#ifdef _SW_ZDT_PRODUCT_
//转码函数及log 函数的实现

static uint16  Bit10( uint16 n)
{
    uint16 ret=1;
    while( n>0 )
    {
        ret *= 10;
        n--;
    }
    return ret;
}

uint32  MMIAPICOM_StrToInt(uint8* str, uint16 len)
{
    uint32 ret=0;
    uint16 k=0;
    
    for( ; k<len; k++)
    {
        ret += ((uint8)(*(str + len-k-1)) - 0x30)*Bit10(k);
    }
    return ret;
}
uint16  MMIAPICOM_IntToStr(uint32 value, uint8* buf, uint16 buf_len)
{
    int i = 0, j = 0;
    int semi_len = 0;
    int offset = 0;
    uint8 tmp_chr = 0x00;
    
    SCI_ASSERT(buf != PNULL);
    if(buf_len < 2)
    {
        return 0;
    }
    
    if (value == 0)
    {
        buf[i] = (uint8)(0 + '0');
        buf[i + 1] = 0;
        return 1;
    }
    while (value != 0)
    {
        buf[i] = (uint8)((value % 10) + '0');
        value /= 10;
        i++;
        if (i >= buf_len)
            break;
    }
    if (i < buf_len)
        buf[i] = 0;
    semi_len = i / 2;
    offset = i % 2 - 1;
    for (j = 0; j < semi_len; j++)
    {
        tmp_chr = buf[j];
        buf[j] = buf[2 * semi_len + offset - j];
        buf[2 * semi_len + offset - j] = tmp_chr;
    }
    return i;
}

uint16 MMIAPI_UCS8_To_UCS16(
                           char*  src_ptr, //[IN]  String的源指针
                           uint16  src_len, //[IN]  源String的长度，byte数
                           wchar*  dest_ptr, //[OUT] String的目标指针
                           uint16  dest_len
                           )
{
    uint16  i=0,  j=0;
    wchar high = 0;
    wchar low = 0;
    uint16 len = 0;
    if(src_len == 0)
    {
        SCI_TRACE_LOW("ERROR: MMIAPI_UCS8_To_UCS16 src_len == 0");
        return 0;
    }
    
    // 转换成ucs2
    for(i = 0; i < src_len; i++)
    {
        j = i%2;
        if(j == 0)
        {
            high = src_ptr[i] << 8;
        }
        else
        {
            low = src_ptr[i];
            if(len < dest_len)
            {
                dest_ptr[len++] = high + low;
            }
            else
            {
                break;
            }
        }
        
    }
    
    return len;
}

uint16 MMIAPI_UCS16_To_UCS8(
                           wchar*  src_ptr, //[IN]  String的源指针
                           uint16  src_len, //[IN]  源String的长度，byte数
                           char*  dest_ptr, //[OUT] String的目标指针
                           uint16 dest_en
                           )
{
    uint16  i=0,  j=0;
    if(src_len == 0)
    {
        SCI_TRACE_LOW("ERROR: MMIAPI_UCS16_To_UCS8 src_len == 0");
        return 0;
    }
    
    // 转换成ucs2
    for(i = 0; i < src_len; i++)
    {
        if(j < dest_en - 1)
        {
            dest_ptr[j++] = src_ptr[i]>>8;
            dest_ptr[j++] = src_ptr[i]&0x00FF;
        }
        else
        {
            break;
        }
    }
    
    return j;
}


BOOLEAN  MMIAPICOM_IsAllAscii(
                             uint8*  src_ptr, //[IN]  String的源指针
                             uint16  src_len //[IN]  源String的长度，byte数
                             )
{
	uint8 tmp;
	uint16 i;
	for(i = 0; i < src_len; i++)
	{
		tmp = src_ptr[i];
		if( tmp >= 0x80 )
		{
		    return FALSE;
		}
	}
	return TRUE;
}

uint16 MMISMS_BCD_To_ASC(uint8* dst, uint8* src, uint16 len)
{
    uint16 i=0,j=0,k=0;
    uint16 reflen = 0;
    if( len == 0 )
    {
        SCI_TRACE_LOW("MMIAPICOM_BCD_To_ASC  len == 0");
        return 0;
    }
    for(i = 0; i < len; i++)
    {
        j = 2*i;
        dst[j]   = (uint8)(src[i] & 0x0F) + 0x30;
        dst[j + 1] = (uint8)(src[i] >> 4) + 0x30;
        reflen = j + 2;
    }
    if((dst[j + 1] -  0x30) == 0x0F)
    {
        reflen = j+1;
    }
    return reflen;
}


uint16 MMISMS_Gb2312_to_Ucs2(
                             uint8*  src_ptr, //[IN]  String的源指针
                             uint16  src_len, //[IN]  源String的长度，byte数
                             uint8*  dest_ptr, //[OUT] String的目标指针
                             uint16 dest_len
                             )
{
    uint16* ucs2_ptr       = PNULL;
    uint32  relen          = 0;
    uint16  i              = 0;
	uint16  j              = 0;
    uint16 len             = 0;
    if(src_len == 0)
    {
        SCI_TRACE_LOW("SMS ERROR: MMISMS_Gb2312_to_Ucs2 src_len == 0");
        return 0;
    }
    
    // 转换成ucs2
    ucs2_ptr = (uint16*)SCI_ALLOC_APP(src_len<<1);
    
    relen = gb2312_to_ucs2(
        src_ptr,
        src_len,
        ucs2_ptr,
        src_len<<1);    
    
	j = 0;
    for(i = 0; i < relen; i++)
    {
        if(j < (dest_len - 1))
        {
        	if(ucs2_ptr[i] != 0)
        	{
        		dest_ptr[j++]   = (uint8)((ucs2_ptr[i] & 0xFF00) >> 8);
        		dest_ptr[j++] = (uint8)(ucs2_ptr[i] & 0x00FF);
        	}
        }
    }
    len = j;
    
    SCI_FREE(ucs2_ptr);
    return len;
}

uint16 MMISMS_Str16_to_Int(
                           uint8*  src_ptr, //[IN]  String的源指针
                           uint16  src_len, //[IN]  源String的长度，byte数
                           uint8*  dest_ptr //[OUT] String的目标指针
                           )
{
    uint16  i,  j;
    uint16 high = 0;
    uint16 low = 0;
    uint16 len = 0;
    if(src_len == 0)
    {
        SCI_TRACE_LOW("SMS ERROR: MMISMS_Str16_to_Int src_len == 0");
        return 0;
    }
    
    // 转换成ucs2
    for(i = 0; i < src_len; i++)
    {
        j = i%2;
        if(j == 0)
        {
            if(src_ptr[i] >= 0x30 &&  src_ptr[i] < 0x3a)
            {
                high = (src_ptr[i] - 0x30) * 16;
            }
            else if(src_ptr[i] >= 0x41 && src_ptr[i] <= 0x46)
            {
                high = (src_ptr[i] - 0x41 + 10) * 16;
            }
            else if(src_ptr[i] >= 0x61 && src_ptr[i] <= 0x66)
            {
                high = (src_ptr[i] - 0x61 + 10) * 16;
            }
        }
        else
        {
            if(src_ptr[i] >= 0x30 &&  src_ptr[i] < 0x3a)
            {
                low = src_ptr[i] - 0x30;
            }
            else if(src_ptr[i] >= 0x41 && src_ptr[i] <= 0x46)
            {
                low = src_ptr[i] - 0x41 + 10;
            }
            else if(src_ptr[i] >= 0x61 && src_ptr[i] <= 0x66)
            {
                low = src_ptr[i] - 0x61 + 10;
            }
            dest_ptr[i/2] = high + low;
        }
        
    }
    
    return src_len/2;
}

uint16 MMISMS_Int_to_Str16(  
                           uint8 * value, 
                           uint16    val_len,
                           uint8 * buf, 
                           uint16  buf_len
                           )
{
    int i = 0, j = 0, k = 0,n = 0;
    uint8 tmpval = 0;
    int offset = 0;
    uint8 t_val;
    uint8 tmp_chr = 0x00;
    uint8 tmp_buf[2]; 
    
    SCI_ASSERT(buf != PNULL);
    SCI_ASSERT(buf_len >= 2);
    for(j = 0; j < val_len; j++)
    {
        i = 0;
        tmpval = value[j];
        if (tmpval == 0)
        {
            tmp_buf[0] = (uint8)(0 + '0');
            tmp_buf[1] = (uint8)'0';
        }
        while (tmpval != 0)
        {
            t_val = (tmpval % 16);
            if(t_val >= 0 &&  t_val < 10)
                tmp_buf[i] = (uint8)(t_val + '0');
            else
                tmp_buf[i] = (uint8)(t_val + 'A' - 10);
            i++;
            tmpval /= 16;
            if(tmpval == 0)
                tmp_buf[i] = (uint8)'0';		
            if (i >= 2)
                break;
        }
        
        buf[n++] = tmp_buf[1];
        buf[n++] = tmp_buf[0];
        if (n >= buf_len)
            break;
    }
    
    return n;
}

uint8 MMIPB_AscToBCD(uint8 asc)
{	
	uint8 bcd;
        if(asc >= '0' &&  asc <= '9')
        {
            bcd = (asc- '0');
        }
        else if(asc == '*' )
        {
            bcd = DIALBCD_STAR;
        }
        else if(asc == '#' )
        {
            bcd = DIALBCD_HASH;
        }
        else if(asc == 'P' || asc == 'p' )
        {
            bcd = DIALBCD_PAUSE;
        }
        else if(asc == 'W' || asc == 'w' )
        {
            bcd = DIALBCD_WILD;
        }
	 else
	 {
	     bcd = 0;
	 }
	 return bcd;
}

uint16 MMIPB_Str8ToBCD(
                           uint8*  src_ptr, //[IN]  String的源指针
                           uint16  src_len, //[IN]  源String的长度，byte数
                           uint8*  dest_ptr //[OUT] String的目标指针
                           )
{
    uint16  i=0,  j=0,k=0;
    uint16 high = 0;
    uint16 low = 0;
    uint16 len = 0;
    if(src_len == 0)
    {
        SCI_TRACE_LOW("SMS ERROR: MMISMS_Str16_to_Int src_len == 0");
        return 0;
    }

	// modified  by bao  T080409 加上*#P的处理
	k = src_len/2 ;
	for( i=0; i<k; i++ )
	{
		j = i*2;
		dest_ptr[i] = (MMIPB_AscToBCD(src_ptr[j+1]) << 4 ) + MMIPB_AscToBCD(src_ptr[j]);
	}
	if( (src_len%2)==1 )
		dest_ptr[i] = 0xF0 + MMIPB_AscToBCD(src_ptr[i*2]);

    return (src_len+1)/2;
}
#endif
