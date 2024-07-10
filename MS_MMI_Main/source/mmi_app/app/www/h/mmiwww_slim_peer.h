/***************************************************************************
** File Name:      mmiwww_slim_peer.h                                            *
** Author:                                                                 *
** Date:           15/06/2009                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to www browser slim peer define                   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 06/2009        wancan.you        Create
****************************************************************************/
#ifndef  _MMIWWW_SLIM_PEER_H_    
#define  _MMIWWW_SLIM_PEER_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmiwww_internal.h"
#include "config.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define SLIM_FLAG_NONE	0 /* means no bit flag is on */

#define SLIM_STREAM_READ            0x00000001
#define SLIM_STREAM_WRITE           0x00000002
#define SLIM_STREAM_READWRITE   (SLIM_STREAM_READ|SLIM_STREAM_WRITE)
#define SLIM_STREAM_CREATE	        0x00000004
#define SLIM_STREAM_TRUNCATE    0x00000008
#define SLIM_STREAM_EXCLUSIVE   0x00000010

#define SLIM_STAT_IFMT  0x00170000
#define SLIM_STAT_IFREG 0x00100000
#define SLIM_STAT_IFDIR 0x00040000
#define SLIM_STAT_ISUID 0x00004000
#define SLIM_STAT_ISGID 0x00002000
#define SLIM_STAT_ISVTX 0x00001000
#define SLIM_STAT_IRWXU 0x00000700
#define SLIM_STAT_IRUSR 0x00000400
#define SLIM_STAT_IWUSR 0x00000200
#define SLIM_STAT_IXUSR 0x00000100
#define SLIM_STAT_IRWXG 0x00000070
#define SLIM_STAT_IRGRP 0x00000040
#define SLIM_STAT_IWGRP 0x00000020
#define SLIM_STAT_IXGRP 0x00000010
#define SLIM_STAT_IRWXO 0x00000007
#define SLIM_STAT_IROTH 0x00000004
#define SLIM_STAT_IWOTH 0x00000002
#define SLIM_STAT_IXOTH 0x00000001

#define SLIM_IP_ADDR_TYPE_ANY 0
#define SLIM_IP_ADDR_TYPE_V4 4
#define SLIM_IP_ADDR_TYPE_V6 6
#define SLIM_IP_ADDR_LEN_V4 4
#define SLIM_IP_ADDR_LEN_V6 16

#define WAVE_SCREEN_CONTEXT_PRIMARY ((TScreenContext)WAVE_SCREEN_ID_PRIMARY)
#define WAVE_SCREEN_CONTEXT_SECONDARY   ((TScreenContext)WAVE_SCREEN_ID_SECONDARY)
#define WAVE_SCREEN_CONTEXT_PRINTER ((TScreenContext)WAVE_SCREEN_ID_PRINTER)

#define WAVE_SCREEN_CAPS_DIRECTSCREEN           0x00000001
#define WAVE_SCREEN_CAPS_INDIRECTSCREEN         0x00000002
#define WAVE_SCREEN_CAPS_DIRECTHAL                  0x00000004
#define WAVE_SCREEN_CAPS_MEMMAP                     0x00000008
#define WAVE_SCREEN_CAPS_SUPPORT_CURSOR         0x00000010
#define WAVE_SCREEN_CAPS_SYSTEM_CURSOR          0x00000020
#define WAVE_SCREEN_CAPS_STRETCHBLT                 0x00000100
#define WAVE_SCREEN_CAPS_DRAWLINE                   0x00000200
#define WAVE_SCREEN_CAPS_FRAMERECT                  0x00000400
#define WAVE_SCREEN_CAPS_FRAMEPOLYGON           0x00000800
#define WAVE_SCREEN_CAPS_FILLPOLYGON	            0x00001000
#define WAVE_SCREEN_CAPS_FRAMEARC                   0x00002000
#define WAVE_SCREEN_CAPS_FILLARC                        0x00004000
#define WAVE_SCREEN_CAPS_INVALRECT                  0x00008000
#define WAVE_SCREEN_CAPS_LOCKUNLOCKPIXEL        0x00010000
#define WAVE_SCREEN_CAPS_MASK                           0x00ffffff

#ifdef SLIM_CONFIG_DEBUG
#ifdef USE_PEER_DP
#define PEER_DP(arg)  slimDebugPrintfPeer arg
#else
#define PEER_DP(arg)
#endif
#define PEER_ASSERT(test) ((test) ? (void)0 : slimDebugAssertFailPeer(#test, __FILE__, __LINE__, 0, 0))
#define PEER_ASSERT_MSG(test,msg) ((test) ? (void)0 : slimDebugAssertFailPeer(#test, __FILE__, __LINE__, 0, (msg)))
#define PEER_ASSERT_TYPE(var,type) PEER_ASSERT_MSG(sizeof(var) == sizeof(type), "Type mismatch.")
#else
#define PEER_DP(arg) (0)
#define PEER_ASSERT(test) (0)
#define PEER_ASSERT_MSG(test,msg) (0)
#define PEER_ASSERT_TYPE(var,type) (0)
#endif

#define STACK_REMEMBER() stack_remember()
#define STACK_REMIND()  stack_remind()

#define SLIM_MIN(x, y) ((x)<(y)?(x):(y))
#define SLIM_MAX(x, y) ((x)>(y)?(x):(y))
#define SLIM_ABS(n) ((n)>0?(n):-(n))
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
enum
{
    SLIM_SEEK_SET,
    SLIM_SEEK_CUR,
    SLIM_SEEK_END
};

struct slim_ip_addr_
{
    slim_int fType;
    slim_byte fAddr[SLIM_IP_ADDR_LEN_V6];
};
typedef struct slim_ip_addr_ slim_ip_addr;

/* tcp */
enum
{
    SLIM_TCP_OP_DELAY = 0,
    SLIM_TCP_OP_LINGER    = 1,
    SLIM_TCP_OP_KEEPALIVE = 2,
    SLIM_TCP_OP_RCVBUF    = 3,
    SLIM_TCP_OP_SNDBUF    = 4,
    SLIM_TCP_OPTIONS  = 5
};


/* udp */
enum
{
    SLIM_UDP_OP_SNDBUF  = 0,
    SLIM_UDP_OPTIONS    = 1
};

/* Screen structure */
typedef slim_opaque TScreenContext;

enum
{
    WAVE_SCREEN_ID_NULL = 0,
    WAVE_SCREEN_ID_PRIMARY,
    WAVE_SCREEN_ID_SECONDARY,
    WAVE_SCREEN_ID_PRINTER,

    WAVE_SCREEN_ID_MAX
};

struct TFontStyle_
{
    slim_int fFontSize;
    slim_int fFontSizeAdjust;								/* CSS2 */
    slim_byte fFontStretch;									/* CSS2 */
    slim_byte fFontStyle;
    slim_byte fFontVariant;
    slim_byte fFontWeight;
};
typedef struct TFontStyle_ TFontStyle;

enum
{
    SLIM_FONT_STYLE_NORMAL = 0,
    SLIM_FONT_STYLE_ITALIC,
    SLIM_FONT_STYLE_OBLIQUE,
    SLIM_FONT_STYLE_enums
};

enum
{
    SLIM_FONT_WEIGHT_100 = 0,
    SLIM_FONT_WEIGHT_200,
    SLIM_FONT_WEIGHT_300,
    SLIM_FONT_WEIGHT_400,
    SLIM_FONT_WEIGHT_500,
    SLIM_FONT_WEIGHT_600,
    SLIM_FONT_WEIGHT_700,
    SLIM_FONT_WEIGHT_800,
    SLIM_FONT_WEIGHT_900,
    SLIM_FONT_WEIGHT_enums,
    /* aliases */
    SLIM_FONT_WEIGHT_NORMAL = SLIM_FONT_WEIGHT_400,
    SLIM_FONT_WEIGHT_BOLD = SLIM_FONT_WEIGHT_700
};

/*            */
/* font style */
/*            */
enum
{
    SLIM_FONT_SIZE_XXXLARGE = -1,
    SLIM_FONT_SIZE_XXLARGE  = -2,
    SLIM_FONT_SIZE_XLARGE   = -3,
    SLIM_FONT_SIZE_LARGE    = -4,
    SLIM_FONT_SIZE_MEDIUM   = -5,
    SLIM_FONT_SIZE_SMALL    = -6,
    SLIM_FONT_SIZE_XSMALL   = -7,
    SLIM_FONT_SIZE_XXSMALL  = -8,
    SLIM_FONT_SIZE_ENUMS     = 9
};

enum
{
    WAVE_LOCK_SCREEN_PAINT,
    WAVE_LOCK_SCREEN_UPDATE,
    WAVE_LOCK_SCREEN_PROGRESS
};

struct TFontMetrics_
{
    slim_int fAscent;
    slim_int fDescent;
    slim_int fHeight;
    slim_int fMinWidth;
    slim_int fMaxWidth;
};
typedef struct TFontMetrics_ TFontMetrics;

struct TGraphicsCaps_
{
    slim_int fFlag;
    slim_int fWidth;
    slim_int fHeight;
    slim_int fBytesPerPixel;
    slim_int fScanLineAlign;
    slim_int fRowBytes;
    void *fBuffer;
    slim_int fCursorWidth;
    slim_int fCursorHeight;
};
typedef struct TGraphicsCaps_ TGraphicsCaps;

typedef void * TWnd;

/* event structure */
struct TEvent_
{
    slim_int fID;	/* type */
    TWnd fTarget;
    slim_int fX;
    slim_int fY;
    slim_int fKeyCode;
    slim_int fKeyChar;
    slim_int fModifiers;
    slim_opaque fArg;
};

typedef struct TEvent_ TEvent;

/* event ID base constant */
#define SLIM_EVENTBASE_RAW          0x0000
#define SLIM_EVENTBASE_GENERIC      0x1000
#define SLIM_EVENTBASE_EXTEND       0x2000
#define SLIM_EVENTBASE_USER         0x3000

/* raw event ID constant */
enum
{
    SLIM_EVENT_COMMAND = -1,
    SLIM_EVENT_NIL = SLIM_EVENTBASE_RAW,
    SLIM_EVENT_KEYPRESS,
    SLIM_EVENT_KEYRELEASE,
    SLIM_EVENT_MOUSEDOWN,
    SLIM_EVENT_MOUSEUP,
    SLIM_EVENT_MOUSEMOVE,
    SLIM_EVENT_MOUSEWHEEL,
    SLIM_EVENT_CLICK,
    SLIM_EVENT_DBLCLICK,
    SLIM_EVENT_CONTEXTMENU,
    SLIM_EVENT_PAINT
};

#ifdef SLIM_CONFIG_USE_DNS
typedef slim_opaque TDNSPeer;
typedef void (*TDNSWakeUpCallbackProc)(slim_opaque in_callback_aux, slim_int in_ticket_id);
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : slim net peer initlialize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimNetInitializePeer(void);

/*****************************************************************************/
//  Description : slim net peer finalize
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC slim_int slimNetFinalizePeer(void);

/*****************************************************************************/
// Description : Outputs debug(with linefeed)
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void slimDebugPrintfPeer(char *in_format, ...);

/*****************************************************************************/
// Description : Gives notification that assertion failed
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void slimDebugAssertFailPeer(char *in_condition,
                                                                    char *in_filename,
                                                                    int in_lineno,
                                                                    int in_arg,
                                                                    char *in_msg);

/*****************************************************************************/
// Description : Allocates memory
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void *slimMemoryAltMallocPeer(slim_int in_size);

/*****************************************************************************/
// Description : Releases memory
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void slimMemoryAltFreePeer(void *in_p);

/*****************************************************************************/
// Description : Reallocates memory
// Global resource dependence :
// Author: Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void *slimMemoryAltReallocPeer(void *in_p, slim_int in_size);

/*****************************************************************************/
//  Description : combine full path
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC slim_bool slimFile_CombineFullPath(
                                         slim_char  *in_fname,
                                         wchar      *full_path_ptr,     //out
                                         uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                         );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif


