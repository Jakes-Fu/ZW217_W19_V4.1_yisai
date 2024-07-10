/******************************************************************************
 ** File Name:      IMG_PORT.h                                                *
 ** Author:         Zhemin.Lin                                                *
 ** DATE:           2004/04/28                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    interface of transplant                                   *
 ** Note:           None                                                      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2004/04/28     Zhemin.Lin       Create.                                   *
 ** 2004/07/09     Zhemin.Lin       Modified accordint to CR12175.            *
 ******************************************************************************/
 
#ifndef IMG_PORT_H
#define IMG_PORT_H

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "sci_types.h"

#ifdef WIN32
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <crtdbg.h>

    typedef FILE    IMG_FILE;
	#define IMG_PRINT printf
	#define MAX_PATH_LEN	512
//	#include "os_api.h"
    #define SCI_ASSERT(_EXP)   _ASSERTE(_EXP)
#else
	#define MAX_PATH_LEN	64

	#ifdef UNIT_TEST
	    #include <stdio.h>
	    
	    typedef uint32 IMG_FILE;    
		#define IMG_PRINT printf
	#else
		#include "os_api.h"
		#include "sci_log.h"
		#include "efs.h"
		
		typedef SCI_FILE IMG_FILE;
	
		#define IMG_PRINT SCI_TRACE_LOW
	#endif
#endif

#define IMG_ASSERT(a) \
    if (!(a)) { \
        SCI_ASSERT(FALSE);\
        }
        
/*error handle*/
extern uint32 g_IMG_ErrorId;

#define IMG_SET_LASTERROR(errorid) \
    g_IMG_ErrorId = (errorid);\
    if (errorid) \
    { \
        IMG_PRINT("\r\nIMG: current errid is %s, file:%s, line:%d\r\n", #errorid, __FILE__,__LINE__);\
    }

/*mem operation interface*/
void * IMG_AllocMem(uint32 size);

void IMG_xFreeMem(void *ptr);
#define IMG_FreeMem(a)  IMG_xFreeMem((a)); \
						(a) = (void *)0;


/*define file open mode*/
#define IMG_OPENMODE_READONLY   1
#define IMG_OPENMODE_WRITE      2
#define IMG_OPENMODE_CREATE     4

/*file operation interface*/
IMG_FILE* IMG_OpenFile(const char *filename, uint32 option);
uint32 IMG_CloseFile( IMG_FILE *file);
uint32 IMG_SeekFile( IMG_FILE *file, uint32 pos);
uint32 IMG_ReadFile( void *buffer, uint32 size, IMG_FILE *file );
uint32 IMG_WriteFile( const void *buffer, uint32 size, IMG_FILE *file );

/*big endian <--> little endian*/
#ifdef __RES_BIG_ENDIAN
#define UINT16_LE(a)    ((((a)&0xFF)<<8)+(((a)>>8)&0xFF))
#define UINT32_LE(a)    ((((a)>>24)&0xff)+(((a)>>8)&0xff00)+(((a)<<8)&0xff0000)+(((a)<<24)&0xff000000))
#else
#define UINT16_LE(a)    (a)
#define UINT32_LE(a)    (a)
#endif

#define BREAK_POINT(string) \
	{\
		IMG_PRINT("\r\n %s", string);\
		while(1);\
	}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif //IMG_PORT_H
