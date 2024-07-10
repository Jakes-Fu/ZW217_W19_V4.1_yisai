/******************************************************************************
 ** File Name:      jretarget.h                                               *
 ** Author:         Zhemin.Lin                                                *
 ** Date:           2004/07/19                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    define the debug port                                     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2004/07/19     Zhemin.Lin       Create.                                   *
 ******************************************************************************/
#ifndef JPEG_RETARGET_H
#define JPEG_RETARGET_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "jcodec.h"

#ifdef WIN32
	#define JPEG_PRINTF printf

	#define JPEG_TRACE JPEG_PRINTF

	#define JPEG_ERROR(err_id, arg) \
		g_JpegCodec.error_id = err_id; \
		JPEG_PRINTF("JPEG ERROR: %s, %s, %d\n",(#err_id), __FILE__, __LINE__); \
		JPEG_PRINTF(##arg); \
		JPEG_PRINTF("\n")
		
	#define JPEG_ASSERT(expr) \
		if(!(expr)) { return 0; }
#else
	#define JPEG_PRINTF 

	#define JPEG_TRACE

	#define JPEG_ERROR
		
	#define JPEG_ASSERT

#endif

#endif

