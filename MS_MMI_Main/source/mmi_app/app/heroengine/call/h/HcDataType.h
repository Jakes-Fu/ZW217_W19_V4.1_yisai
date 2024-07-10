#ifdef HERO_ENGINE_HMGS_SUPPORT

#ifndef _PIM_DATA_TYPE_H
#define _PIM_DATA_TYPE_H

//#include "sci_types.h"
//#include "dsm_datatype.h" 
#ifndef NULL
#define NULL (void*) 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

typedef unsigned char		U8;
typedef unsigned short		U16;
typedef unsigned int		U32;

typedef char		S8;
typedef short		S16;
typedef int		S32;

#if 1
typedef  unsigned short	uint16;
typedef  unsigned long int	uint32;
typedef  long int			int32; 
typedef  unsigned char		uint8; 
typedef  signed char		int8; 
typedef  signed short		int16;
#endif

typedef void (*FuncPtr) (void);

//#include "hr_base.h" 

#define HCNUMBER_LEN 	  65
#define HCSTRLOCATION_LEN 128

//#define DEBUG_FILE_TRACE
#define HCCALL_DEFAULT_AUTOUPDATE	(0)//0-Close; 1-one month; 2-two month; 3-three month
//#define __HCCALL_DATABASE_FIRM__

#define HC_FIELDVALUE_MAX_LEN	32

#ifdef DEBUG_FILE_TRACE
void FilePrintf(const char *pcFormat, ...);
#define FILE_TRACE    FilePrintf
#else
#ifdef WIN32
#define FILE_TRACE
#else
#define FILE_TRACE(...)
#endif
#endif

enum{
    HC_MENU_DSM_GAME_CENTER,
    HC_STR_GLOBAL_OK,
    HC_IMG_GLOBAL_OK,
    HC_STR_GLOBAL_BACK,
    HC_IMG_GLOBAL_BACK,
    HC_KEY_EVENT_UP,
    HC_MOD_MMI
};

enum{
    HC_DRIVER_SYSTEM,
    HC_DRIVER_PUBLIC,
    HC_DRIVER_CARD
};

typedef enum HC_DLD_EVT_E
{
	HC_DLD_FINDNEW,
	HC_DLD_ISNEW,
	HC_DLD_CONERROR,
	HC_DLD_NETERROR,
	HC_DLD_SERVERERROR
}HC_DLD_EVT;

#endif //_PIM_DATA_TYPE_H
#endif //FEATURE_SUPPORT_HERO_HCCALL