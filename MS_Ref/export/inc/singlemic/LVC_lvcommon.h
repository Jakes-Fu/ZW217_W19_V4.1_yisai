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
/*     $Revision: 6595 $*/
/*     $Date: 2011-09-29 12:42:05 +0200 (Thu, 29 Sep 2011) $*/
/*                                                                                      */
/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/*  Header file for the application layer interface of the lvcommon module              */
/*                                                                                      */
/*  This files includes all definitions, types, structures and function prototypes      */
/*  required by the calling layer. All other types, structures and functions are        */
/*  private.                                                                            */
/*                                                                                      */
/****************************************************************************************/

#ifndef __lvcommon_H__
#define __lvcommon_H__

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
#define NCH_IN                2     /* max nr of microphone input channels      */
#define NCH_OUT               1     /* max nr of output channels                */
#define NREFS                 1     /* max nr of ref DNNS input channels        */
#define LC_SUBBANDS           2     /* nr of subbands: low band, high band      */

#define FRAME                80     /* 80 samples per block                     */
#define FRAME_WB            160     /* 160 samples per block wideband           */

#define NF_NB                65     /* nr of bins spectral magnitude            */
#define NF_WB               129     /* nr of bins spectral magnitude wideband   */
#define FFT_P               128     /* fft size                                 */
#define FFT_P_WB            256     /* fft size wideband                        */
#define OLA                  48     /* overlap size                             */
#define OLA_WB               96     /* overlap size wideband                    */
#define NF_COMPR             26     /* nr of bins compressed spectral magnitude */
#define NF_COMPR_WB          34     /* nr of bins compressed spectral magnitude wideband */

#define QMF_NBANKS_MAX       4      /* maximum number of ANA or SYN filter banks allowed */

#define CONFIG_HANDSET       0
#define CONFIG_SPEAKERPHONE  1

#define MEMREDMODE_OFF       0
#define MEMREDMODE_ON        1

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif      /* __lvcommon_H__ */

/* End of file */
