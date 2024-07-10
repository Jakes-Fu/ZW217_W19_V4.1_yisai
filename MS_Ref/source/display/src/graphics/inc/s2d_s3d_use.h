
/******************************************************************************
 ** File Name:      s2d_s3d_use.h                                              *
 ** Author:                                                                   *
 ** DATE:           2010-11-22                                                 *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    image rotate functions                                    *
 ** Note:                                                      				  *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                             *                                                            *
 *****************************************************************************/
#ifndef _S3D_S3D_USE_H_
#define _S3D_S3D_USE_H_

#include "sci_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

PUBLIC  int s2d_sin(int angle);
PUBLIC  int s2d_cos(int angle);
PUBLIC int s2d_arcsin(int32 sin_value);
#ifdef __cplusplus
}
#endif
#endif
