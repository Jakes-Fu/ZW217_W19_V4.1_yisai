/****************************************************************************************/
/*  Copyright (c) 2004-2011 NXP Software. All rights are reserved.                      */
/*  Reproduction in whole or in part is prohibited without the prior                    */
/*  written consent of the copyright owner.                                             */
/*                                                                                      */
/*  This software and any compilation or derivative thereof is and                      */
/*  shall remain the proprietary information of NXP Software and is                     */
/*  highly confidential in nature. Any and all use hereof is restricted                 */
/*  and is subject to the terms and conditions set forth in the                         */
/*  software license agreement concluded with NXP Software.                             */
/*                                                                                      */
/*  Under no circumstances is this software or any derivative thereof                   */
/*  to be combined with any Open Source Software in any way or                          */
/*  licensed under any Open License Terms without the express prior                     */
/*  written  permission of NXP Software.                                                */
/*                                                                                      */
/*  For the purpose of this clause, the term Open Source Software means                 */
/*  any software that is licensed under Open License Terms. Open                        */
/*  License Terms means terms in any license that require as a                          */
/*  condition of use, modification and/or distribution of a work                        */
/*                                                                                      */
/*           1. the making available of source code or other materials                  */
/*              preferred for modification, or                                          */
/*                                                                                      */
/*           2. the granting of permission for creating derivative                      */
/*              works, or                                                               */
/*                                                                                      */
/*           3. the reproduction of certain notices or license terms                    */
/*              in derivative works or accompanying documentation, or                   */
/*                                                                                      */
/*           4. the granting of a royalty-free license to any party                     */
/*              under Intellectual Property Rights                                      */
/*                                                                                      */
/*  regarding the work and/or any work that contains, is combined with,                 */
/*  requires or otherwise is based on the work.                                         */
/*                                                                                      */
/*  This software is provided for ease of recompilation only.                           */
/*  Modification and reverse engineering of this software are strictly                  */
/*  prohibited.                                                                         */
/*                                                                                      */
/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/*     Project::                                                                        */
/*     $Author: beq07720 $*/
/*     $Revision: 3977 $*/
/*     $Date: 2011-03-03 13:55:55 +0100 (Thu, 03 Mar 2011) $*/
/*                                                                                      */
/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/*  Header file for the application layer interface of the LVWM module                  */
/*                                                                                      */
/*  This files includes all definitions, types, structures and function prototypes      */
/*  required by the calling layer. All other types, structures and functions are        */
/*  private.                                                                            */
/*                                                                                      */
/****************************************************************************************/

#ifndef __LVWM_H__
#define __LVWM_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/****************************************************************************************/
/*                                                                                      */
/*  Includes                                                                            */
/*                                                                                      */
/****************************************************************************************/
#include "LVC_Types.h"


/****************************************************************************************/
/*                                                                                      */
/*  Definitions                                                                         */
/*                                                                                      */
/****************************************************************************************/
/* General */
#define LVWM_MAX_MODE                           0xF

#define LVWM_MIN_AVL_TARGET_LEVEL_LIN           0x0

#define LVWM_MIN_AVL_MINGAINLIN                 0x0

#define LVWM_MIN_AVL_ATTACK                     0x0

#define LVWM_MIN_AVL_RELEASE                    0x0

#define LVWM_MIN_AVL_LIMIT_MAXOUTPUTLIN         0x0

#define LVWM_MIN_SPDETECT_THRESHOLD             0x0

#define LVWM_GAIN_0dB                           0x0200

/*----------------------------------------------------------------------*/
/* Constant declarations                                                */
/*----------------------------------------------------------------------*/
#define LVWM_MODE_AVL           0x0001
#define LVWM_MODE_SPDETECT      0x0002
#define LVWM_MODE_LIMITER       0x0004
#define LVWM_MODE_EXT_SPDETECT  0x0008

/* Memory table*/

/****************************************************************************************/
/*                                                                                      */
/*  Types                                                                               */
/*                                                                                      */
/****************************************************************************************/
/* Instance handle */
typedef void *LVWM_Handle_t;

/* Status return values */
typedef enum
{
    LVWM_SUCCESS            = 0,                       /* Successful return from a routine */
    LVWM_ALIGNMENTERROR     = 1,                       /* Memory alignment error */
    LVWM_NULLADDRESS        = 2,                       /* NULL allocation address */
    LVWM_OUTOFRANGE         = 3,                       /* Out of range parameter */
    LVWM_INVALIDNUMSAMPLES  = 4,                       /* Invalid number of samples */
    LVWM_RETURNSTATUS_DUMMY = LVM_MAXENUM
}LVWM_ReturnStatus_en_ ;
typedef LVM_UINT32 LVWM_ReturnStatus_en;


/****************************************************************************************/
/*                                                                                      */
/*  Structures                                                                          */
/*                                                                                      */
/****************************************************************************************/

/* Control Parameter structure */
typedef struct
{
    /* General parameters */
    LVM_Mode_en                 OperatingMode;          /* Operating mode */

    /* Parameters for LVWM */
    LVM_UINT16                  mode;
    LVM_INT16                   AVL_Target_level_lin;
    LVM_INT16                   AVL_MinGainLin;
    LVM_INT16                   AVL_MaxGainLin;
    LVM_INT16                   AVL_Attack;
    LVM_INT16                   AVL_Release;
    LVM_INT16                   AVL_Limit_MaxOutputLin;
    LVM_INT16                   SpDetect_Threshold;

} LVWM_ControlParams_st;

/* Instance Parameter structure */
typedef struct
{
    /* General */
    LVM_Fs_en                   SampleRate;             /* sample rate */

} LVWM_InstanceParams_st;

/* Status Parameter structure */
typedef struct
{
    /* Parameters for LVWM */
    LVM_INT16                   AVL_Gain;

} LVWM_StatusParams_st;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LVWM_H__ */

/* End of file */
