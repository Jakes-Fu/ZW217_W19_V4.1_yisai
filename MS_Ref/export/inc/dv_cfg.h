/******************************************************************************
 ** File Name:      dv_cfg.h                                                  *
 ** Author:                                                                   *
 ** Date:           2012/1/15                                                 *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    API definition for dv configure.                          * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2012/1/15                       Create.                                   *
 ******************************************************************************/
#ifndef DV_CFG_H
#define DV_CFG_H

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "media_file_demux.h"
#include "video_codec_adptor.h"


/******************************************************************************/
// Description: Find out one demux obj from the register table.
// Author:     
// Input:      p_demux_main_type,the main name for this demuxer,e.g. "3gp","mp4".
// Output:     none.
// Return:     PNULL,  failed to find out one demux obj.
//             others, find out one matched demux obj.
// Note:       none
/******************************************************************************/
PUBLIC FILE_DEMUX_OBJ_T* ExtFileDemux_FindObj(const char *p_demux_main_type);


/******************************************************************************/
// Description: Find out one decoder obj from the register table.
// Author:     
// Input:      p_codec_name,the  name for this decoder,e.g. "mpeg4","h263".
// Output:     none.
// Return:     PNULL,  failed to find out one decoder obj.
//             others, find out one matched decoder obj.
// Note:       none
/******************************************************************************/
PUBLIC VCODEC_ADP_DEC_OBJ_T* ExtVideoDec_FindObj(const char *p_codec_name);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     							   *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    }
#endif

#endif /*#ifndef MEDIA_FILE_DEMUX_H*/ 
