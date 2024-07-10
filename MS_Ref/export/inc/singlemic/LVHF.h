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
/*     $Author: beq07720 $*/
/*     $Revision: 2154 $*/
/*     $Date: 2010-09-20 17:07:38 +0200 (Mon, 20 Sep 2010) $*/
/*                                                                                      */
/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/*  Header file for the application layer interface of the LVHF module                  */
/*                                                                                      */
/*  This files includes all definitions, types, structures and function prototypes      */
/*  required by the calling layer. All other types, structures and functions are        */
/*  private.                                                                            */
/*                                                                                      */
/****************************************************************************************/

#ifndef __LVHF_H__
#define __LVHF_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****************************************************************************************/
/*                                                                                      */
/*  Includes                                                                            */
/*                                                                                      */
/****************************************************************************************/

#include "LVC_lvcommon.h"

/****************************************************************************************/
/*                                                                                      */
/*  Definitions                                                                         */
/*                                                                                      */
/****************************************************************************************/
#define LVHF_STATUS_FAR_SPEAKS               1
#define LVHF_STATUS_FAR_STARTS               2
#define LVHF_STATUS_MIC_SPEAKS               3
#define LVHF_STATUS_CLIP                     4
#define LVHF_STATUS_PATHCHANGE               5

#define LVHF_NLMS_MAX_TAPS_LB                200 /* Maximum length of NLMS_LB (should be multiple of 8) */
#define LVHF_NLMS_MAX_TAPS_HB                136 /* Maximum length of NLMS_HB (should be multiple of 8) */


/****************************************************************************************/
/*                                                                                      */
/*  Types                                                                               */
/*                                                                                      */
/****************************************************************************************/

/* Instance handle */
typedef void *LVHF_Handle_t;

/* Status return values */
typedef enum
{
    LVHF_SUCCESS            = 0,                       /* Successful return from a routine */
    LVHF_ALIGNMENTERROR     = 1,                       /* Memory alignment error */
    LVHF_NULLADDRESS        = 2,                       /* NULL allocation address */
    LVHF_OUTOFRANGE         = 3,                       /* Out of range parameter */
    LVHF_INVALIDNUMSAMPLES  = 4,                       /* Invalid number of samples */
    LVHF_RETURNSTATUS_DUMMY = LVM_MAXENUM
} LVHF_ReturnStatus_en_;
typedef LVM_UINT32 LVHF_ReturnStatus_en;


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

    /* LVHF - Internal mode parameters */
    LVM_UINT16                  Mode;                    // [0, 128]   default  0xBF = FULL ON, 64=mute, 128=NLMS_preset
    LVM_UINT16                  TuningMode;

    /* LVHF - Scaling parameters */
    LVM_INT16                   InputGain;               // [0, 32767] default = 8192 = 1.0
    LVM_INT16                   OutputGain;              // [0, 32767] default = 2048 = 1.0

    /* LVHF - PRE-NLMS parameters */
    LVM_INT16                   NLMS_limit;              // [-24, 0]dB default = 0

    /* LVHF - NLMS parameters */
    LVM_UINT16                  NLMS_LB_taps;            // [0, 200]   default     64
    LVM_INT16                   NLMS_LB_two_alpha;       // [0, 32767] default   1000 = 0.25
    LVM_INT16                   NLMS_LB_erl;             // [0, 32767] default    128 = less than 0 dB
    LVM_UINT16                  NLMS_HB_taps;            // [0, 200]   default     64
    LVM_INT16                   NLMS_HB_two_alpha;       // [0, 32767] default   1000 = 0.25
    LVM_INT16                   NLMS_HB_erl;             // [0, 32767] default    128 = less than 0 dB
    LVM_INT16                   NLMS_preset_coefs;       // [0, 2]     default      1 = yes, nonzero
    LVM_INT16                   NLMS_offset;             // [0, 32767] default    300 = see above and try to change it!

    /* LVHF - DENS parameters */
    LVM_INT16                   DENS_tail_alpha;         // [0, 32767] default  25395 = 0.775
    LVM_INT16                   DENS_tail_portion;       // [0, 32767] default  29491 = 0.9
    LVM_INT16                   DENS_gamma_e_high;       // [0, 32767] default   2047 = 1.999
    LVM_INT16                   DENS_gamma_e_dt;         // [0, 32767] default   1024 = 1.0
    LVM_INT16                   DENS_gamma_e_low;        // [0, 32767] default   1024 = 1.0
    LVM_INT16                   DENS_gamma_e_alpha;      // [0, 32767] default  24000 = 0.7325
    LVM_INT16                   DENS_gamma_n;
    LVM_INT16                   DENS_limit_ns;           // [0, 32767] default   9830 = 0.3
    LVM_INT16                   DENS_CNI_Gain;           // [0, 32767] default  32686 = 1.995
    LVM_INT16                   DENS_NL_atten;           // [0, 2048] default   2048 = 1.0
    LVM_INT16                   DENS_spdet_near;
    LVM_INT16                   DENS_spdet_act;
    LVM_INT16                   DENS_NFE_blocksize;

    /* LVHF - Speech Detector parameters */
    LVM_INT16                   SPDET_far;
    LVM_INT16                   SPDET_mic;
    LVM_INT16                   SPDET_x_clip;

    /* LVHF - Path Change Detector parameters */
    LVM_INT16                   PCD_threshold;
    LVM_UINT16                  PCD_taps;
    LVM_INT16                   PCD_erl;
    LVM_INT16                   PCD_minimum_erl;
    LVM_INT16                   PCD_erl_step;
    LVM_INT16                   PCD_gamma_e_rescue;

} LVHF_ControlParams_st;



/* Instance Parameter structure */
typedef struct
{
    /* General */

    LVM_Fs_en                   SampleRate;             /* sample rate */

    /* Parameters for HF */

} LVHF_InstanceParams_st;


/* Status Parameter structure */
typedef struct
{
    /* General */

    LVM_INT16                   gammae;                /* value of multiplicator factor of echo suppression */
    LVM_UINT16                  status;                /* Bit mask describing current state of HF */
    LVM_INT32                   *pNLMS_LB_Coefs;       /* Pointer to NLMS lowband filter coefficients */
    LVM_INT32                   *pNLMS_HB_Coefs;       /* Pointer to NLMS lowband filter coefficients */
    LVM_INT32                   *pPCD_Coefs;           /* Pointer to PCD filter coefficients */

    /* Parameters for HF */

} LVHF_StatusParams_st;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif      /* __LVHF_H__ */

/* End of file */
