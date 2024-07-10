/****************************************************************************************/
/*  Copyright (c) 2004-2010 NXP Software. All rights are reserved.                      */
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
/*     $Author: nxp34438 $ */
/*     $Revision: 4328 $ */
/*     $Date: 2011-03-28 12:45:16 +0200 (Mon, 28 Mar 2011) $ */
/*                                                                                      */
/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/*  Header file for the application layer interface of the LVFENS module                */
/*                                                                                      */
/*  This files includes all definitions, types, structures and function prototypes      */
/*  required by the calling layer. All other types, structures and functions are        */
/*  private.                                                                            */
/*                                                                                      */
/****************************************************************************************/

#ifndef __LVFENS_H__
#define __LVFENS_H__

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
#define LVFENS_MODE_HS         0x0001 /* Switch ON and OFF Horn Suppressor :*/

/****************************************************************************************/
/*                                                                                      */
/*  Types                                                                               */
/*                                                                                      */
/****************************************************************************************/
/* Instance handle */
typedef void *LVFENS_Handle_t;

/* Status return values */
typedef enum
{
    LVFENS_SUCCESS            = 0,                       /* Successful return from a routine */
    LVFENS_ALIGNMENTERROR     = 1,                       /* Memory alignment error */
    LVFENS_NULLADDRESS        = 2,                       /* NULL allocation address */
    LVFENS_OUTOFRANGE         = 3,                       /* Out of range parameter */
    LVFENS_INVALIDNUMSAMPLES  = 4,                       /* Invalid number of samples */
    LVFENS_RETURNSTATUS_DUMMY = LVM_MAXENUM
}LVFENS_ReturnStatus_en_ ;
typedef LVM_UINT32 LVFENS_ReturnStatus_en;


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

    /* Parameters for FENS */

    LVM_INT16                   FENS_limit_NS;          /* FENS Noise Suppression Limit */

    LVM_UINT16                  Mode;
} LVFENS_ControlParams_st;


/* Instance Parameter structure */
typedef struct
{
    /* General */

    LVM_Fs_en                   SampleRate;             /* Sample rate */

    /* Parameters for FENSX */

} LVFENS_InstanceParams_st;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif      /* __LVFENS_H__ */

/* End of file */
