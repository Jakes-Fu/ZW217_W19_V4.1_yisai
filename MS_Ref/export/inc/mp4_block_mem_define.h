/***************************************************************************************
** File Name:      playback_engine.h                                         		 			   *
** DATE:           13/04/2006                                                		   *
** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         		   *
** Description:    																       *
****************************************************************************************

****************************************************************************************
**                         Edit History                                       		   *
**-------------------------------------------------------------------------------------*
** DATE                    DESCRIPTION                               				   *
** 13/04/2006     	       Create.													   *
****************************************************************************************/
#ifndef _MP4_BLOCK_MEM_DEFINE_H
#define _MP4_BLOCK_MEM_DEFINE_H

/**------------------------------------------------------------------------------------*
 **                         Dependencies           			                           *
 **------------------------------------------------------------------------------------*/
#define DWL_FRAMEIN_MAX_SIZE		(512*1024)
#define DWL_FRAMEOUT_MAX_SIZE		(512*1024)
#define DWL_VIDEOBUFFER_SIZE		(256*1024*3)
#define DWL_BITSBUFFER_SIZE			(256*1024*5)

#define EWL_WRITEBUFFER_MAX_SIZE	(400*1024) 
#define EWL_STREAMBUFFER_MAX_SIZE	(256*1024) //Max Image 640*480
#define EWL_SEARCHIMAGE_SIZE		(400*6*64) //Max Image 352*288
#define EWL_INTERNALIMAGE_SIZE		(400*6*64) //Max Image 352*288

#define	MP4DEC_BLOCK_MEMORY_SIZE 	(DWL_FRAMEIN_MAX_SIZE+DWL_FRAMEOUT_MAX_SIZE+\
					DWL_VIDEOBUFFER_SIZE+DWL_BITSBUFFER_SIZE)
					
#define MP4ENC_BLOCK_MEMORY_SIZE	(EWL_WRITEBUFFER_MAX_SIZE+EWL_STREAMBUFFER_MAX_SIZE+\
					EWL_SEARCHIMAGE_SIZE+EWL_INTERNALIMAGE_SIZE)

//@Liangwen 070409 move to isp_reservedspace.h 
//#define JPEGDEC_BLOCK_MEMORY_SIZE 1600*1200*3/2 
/**------------------------------------------------------------------------------------*
 **							Compiler Flag											   *
 **------------------------------------------------------------------------------------*/
#ifdef __cpluslus
	extern "C"
	{
#endif


/**------------------------------------------------------------------------------------*
 **								Compiler Flag										   *
 **------------------------------------------------------------------------------------*/
#ifdef __cplusplus
	}
#endif


#endif//_MP4_BLOCK_MEM_DEFINE_H

//end of mp4_block_mem_define.h
