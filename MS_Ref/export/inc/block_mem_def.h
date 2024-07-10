/******************************************************************************
 ** File Name:      block_mem.h                                               *
 ** Author:         Richard Yang                                              *
 ** DATE:           05/09/2005                                                *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 large block table for different applications.             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/09/2005     Richard.Yang     Create.                                   *
 ** 13/03/2007     Daniel.ding      Trim.                                     *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifndef _BLOCK_MEM_DEF_H_
#define _BLOCK_MEM_DEF_H_

#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#define BLOCK_MEM_POOL_REF_DC_SIZE_ZOOM     (1600*1024)
#define BLOCK_MEM_POOL_REF_DC_SIZE_NO_ZOOM  (641*1024)
#define BLOCK_MEM_POOL_REF_DV_SIZE          (600*1024)

#define BLOCK_MEM_POOL_REF_CAMERA_SIZE      (1100*1024)

#define BLOCK_MEM_POOL_REF_MP4DEC_SIZE      (970*1024)
#define BLOCK_MEM_POOL_REF_MP4ENC_SIZE 	    (500*1024)
//--define only for  MP4/DV module on Himalaya7810.
#define BLOCK_MEM_POOL_REF_MP4_DEMUX_SIZE   (320*1024)
#define BLOCK_MEM_POOL_REF_MP4H263_DEC_SIZE (440*1024)
#define BLOCK_MEM_POOL_REF_MP4ENC_SIZE_7810 (64*1024)
//--define only for MJPEG software decoder on Himalaya7810 .
#define BLOCK_MEM_POOL_REF_JPEGDEC_SIZE     (300*1024)

//所有内存ID都在这儿定义，The IDs must be different each other .
typedef enum
{
    BLOCK_MEM_POOL_REF = 0,	    //From 0 to 255 for driver and middleware .
    BLOCK_MEM_POOL_ISP,	        //isp buffer ID	
    BLOCK_MEM_POOL_MP4DEC,      //mp4/mjpg decodeing buffer ID	
    //--define only for  MP4/DV module on Himalaya7810 platform.
    BLOCK_MEM_POOL_MP4_DEMUX,
    BLOCK_MEM_POOL_MP4H263_DEC,
    //--
    BLOCK_MEM_POOL_MP4ENC,      //mp4/mjpg encodeing buffer ID	
    BLOCK_MEM_POOL_JPEGDEC,     //jpeg decoding buffer ID
    BLOCK_MEM_POOL_SFS,         //file system buffer ID
    BLOCK_MEM_POOL_REF_DC,      //previewing display and capturing fifo buffer ID
    BLOCK_MEM_POOL_REF_DV,
    BLOCK_MEM_POOL_DC,
    //you can add other pool id for driver ;
    BLOCK_MEM_POOL_AVIDEC,
    BLOCK_MEM_POOL_MV_RES,

    BLOCK_MEM_POOL_MMI = 256,   //From 256 to 1024 for mmi and other third party application  .

    BLOCK_MEM_POOL_MAX = 1024   //Max id is 1024 ;
}BLOCK_MEM_ID_E;

typedef enum
{
    NOT_USE = 0 ,
    ALLOC_BY_SELF,
    ALLOC_BY_OTHERS,
    ERROR_STATUS
}BLOCK_MEM_STATUS_E;

typedef enum
{
    BORROW_FROM_CURRENT_BLOCK,
    GIVEBACK_TO_CURRENT_BLOCK
}BLOCK_MEM_RELATION_STATUS_E;

//结构定义拿到block_mem.c
// typedef BOOLEAN (*BL_CallBack)(BLOCK_MEM_ID_E block_id, BLOCK_MEM_RELATION_STATUS_E		relation_info);
// 
// typedef struct BLOCK_MEM_tag
// {
//     BLOCK_MEM_ID_E      block_id;
//     //uint32           	block_offset;      //block相对BLOCK_POOL的起始地址
//     uint32           	block_num;         //block的大小，单位是BLOCK_POOL_SIZE
//     BLOCK_MEM_STATUS_E  block_mem_flag;    //表明这个block是否已经分配
// 	//BL_CallBack			block_callback_ptr; //Block mem回调函数
// }BLOCK_MEM_T;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Allocate a pool for special applications according as the id  
//	Global resource dependence: 
//  Author: Daiel.ding
//	Note:
/*****************************************************************************/
#ifdef WIN32
#define BL_FILE_NAME	__FILE__
#define BL_FILE_LINE	__LINE__
#else
#define BL_FILE_NAME	__MODULE__
#define BL_FILE_LINE	__LINE__
#endif

#define BL_Malloc(id) BL_MallocEx(id, BL_FILE_NAME, BL_FILE_LINE)

void *BL_MallocEx(
                  BLOCK_MEM_ID_E id,                          
                  const char     *file,
                  uint32         line
                  );

/*****************************************************************************/
//  Description:   Free the apllocated memory room .
//	Global resource dependence: 
//  Author: Daniel.ding
//	Note:
/*****************************************************************************/
void BL_Free(BLOCK_MEM_ID_E id);

/*****************************************************************************/
//  Description:    Get special block used status ....
//	Global resource dependence: 
//  Author:Daniel.ding
//	Note:
/*****************************************************************************/
BLOCK_MEM_STATUS_E BL_GetStatus(BLOCK_MEM_ID_E id);

/*****************************************************************************/
//  Description: Return whether the block is used or not, regardless other block
//	Global resource dependence: 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
BLOCK_MEM_STATUS_E BL_GetStatusOnly(BLOCK_MEM_ID_E id);

/*****************************************************************************/
//  Description:    Get special block size ....
//	Global resource dependence: 
//  Author:Daniel.ding
//	Note:
/*****************************************************************************/
uint32 BL_GetSize(BLOCK_MEM_ID_E id);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
