/****************************************************************************************/
/*  Copyright (c) 2004-2007 NXP Software. All rights are reserved.                      */
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
/*     $Author: nxp11156 $*/
/*     $Revision: 6149 $*/
/*     $Date: 2011-08-26 11:57:08 +0200 (Fri, 26 Aug 2011) $*/
/*                                                                                      */
/****************************************************************************************/


/****************************************************************************************/
/*                                                                                      */
/*  Header file for the application layer interface of the LVMDRC module                */
/*                                                                                      */
/*  This files includes all definitions, types, structures and function prototypes      */
/*  required by the calling layer. All other types, structures and functions are        */
/*  private.                                                                            */
/*                                                                                      */
/****************************************************************************************/

#ifndef __LVMDRC_H__
#define __LVMDRC_H__

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
#define LVMDRC_MIN_MAXBLOCKSIZE             44      /* Processing block size multiple */
#define LVMDRC_MAXDELAY                     40      /* Maximumm delay :40 * 500u sec = 20ms */
#define LVMDRC_MAX_KNEES                     5      /* Maximum number of knee points */
#define LVMDRC_MAX_NR_COMPRESSORS            6      /* Maximum number of compressors */
#define LVMDRC_MAX_LEVEL_DB                 48      /* Max level for the knees and the limiter */
#define LVMDRC_MIN_LEVEL_DB                -96      /* Min level for the knees and the limiter */


/* Memory table*/
#define LVMDRC_NR_MEMORY_REGIONS             4      /* Number of memory regions */


/* Application specific definition                                                         */
/* For voice applications, a soft limiter is desired to get better speech quality.         */
/* Compared to hard limiter, a soft limiter generates no high-freq. artifacts.             */
/* So, depending on compiler switch, the type of limiter is determined.                    */
/* The selection of limiter can be controled by LimiterMode in the paramter type.          */
/*    - 0 for no limiter, 1 for hard limiter, 2 for soft limiter                           */
/* The current version of the soft limiter (from LVCOMMON) only supports 16-bit foramt     */


/****************************************************************************************/
/*                                                                                      */
/*  Types                                                                               */
/*                                                                                      */
/****************************************************************************************/
/* Instance handle */
typedef void *LVMDRC_Handle_t;

/* Status return values */
typedef enum
{
    LVMDRC_SUCCESS            = 0,                      /* Successful return from a routine */
    LVMDRC_ALIGNMENTERROR     = 1,                      /* Memory alignment error */
    LVMDRC_NULLADDRESS        = 2,                      /* NULL allocation address */
    LVMDRC_OUTOFRANGE         = 3,                      /* Out of range control parameter */
    LVMDRC_INVALIDNUMSAMPLES  = 4,                      /* Invalid number of samples */
    LVMDRC_RETURNSTATUS_DUMMY = LVM_MAXENUM
}LVMDRC_ReturnStatus_en_ ;
typedef LVM_UINT32 LVMDRC_ReturnStatus_en;


/* Side channel source */
typedef enum
{
    LVMDRC_INTERNAL         = 0,
    LVMDRC_EXTERNAL         = 1,
    LVMDRC_SIDESOURCE_DUMMY = LVM_MAXENUM
}LVMDRC_SideSource_en_ ;
typedef LVM_UINT32 LVMDRC_SideSource_en;


/* Side channel control types */
typedef enum
{
    LVMDRC_ABSOLUTE          = 0,
    LVMDRC_PEAK              = 1,
    LVMDRC_MEANPOWER         = 2,
    LVMDRC_SIDECONTROL_DUMMY = LVM_MAXENUM
}LVMDRC_SideControl_en_ ;
typedef LVM_UINT32 LVMDRC_SideControl_en;


/* Time constant type */
typedef enum
{
    LVMDRC_EXPONENTIAL        = 0,
    LVMDRC_LINEAR             = 1,
    LVMDRC_INTEGRATING        = 2,
    LVMDRC_TIMECONSTANT_DUMMY = LVM_MAXENUM
} LVMDRC_TimeConstant_en_;
typedef LVM_UINT32 LVMDRC_TimeConstant_en;


/****************************************************************************************/
/*                                                                                      */
/*  Structures                                                                          */
/*                                                                                      */
/****************************************************************************************/
/* Memory table containing the region definitions */
typedef struct
{
    LVM_MemoryRegion_st         Region[LVMDRC_NR_MEMORY_REGIONS];  /* One definition for each region */
} LVMDRC_MemoryTable_st;


/* Compressor knee definition structure */
typedef struct
{
    LVM_INT16                   InputLevel;             /* Input level in dB */
    LVM_INT16                   OutputLevel;            /* Output level in dB */
} LVMDRC_KneeDefinition_st;


/* Side channel filter structure */
typedef struct
{
    LVM_Fs_en                   SampleRate;             /* Sample rate */
    LVM_Format_en               SourceFormat;           /* Input signal source format */
    void                        *pFilterContext;        /* Pointer to filter specific data */
} LVMDRC_Filter;


/* Compressor parameters */
typedef struct
{
    /* Frequency range control */
    LVM_UINT16                  LowFrequency;           /* Low corner frequency of the frequency band */

    /* Transfer function definition */
    LVM_Mode_en                 CompressorCoreMode;     /* Operating mode */
    LVM_UINT16                  NumberKnees;            /* Number of knee points to use */
    LVMDRC_KneeDefinition_st    Knees[LVMDRC_MAX_KNEES]; /* The knee points on the transfer function */
    LVM_UINT16                  Softness;               /* Kneed softness control */
    LVM_INT16                   MakeupGain;             /* The make-up gain in dB */

    /* Dynamic control */
    LVMDRC_SideSource_en        SideSource;             /* Side channel source signal */
    LVMDRC_SideControl_en       SideChannel;            /* Side channel control method */
    LVMDRC_TimeConstant_en      AttackTCType;           /* Attack time constant type */
    LVM_UINT16                  Attack;                 /* Attack time constant in 100us steps */
    LVMDRC_TimeConstant_en      DecayTCType;            /* Decay time constant type */
    LVM_UINT16                  Decay;                  /* Decay time constant in 100us steps */
    LVM_UINT16                  DelayTime;              /* Side channel delay time in 500us steps */

    /* Limiter control */
    LVM_Mode_en                 Limiter;                /* Limiter mode control */
    LVM_INT16                   LimitLevel;             /* Limiter threshold */

} LVMDRC_CompressorParams_st;


/* Control Parameter structure */
typedef struct
{
    /* General parameters */
    LVM_Mode_en                 OperatingMode;          /* Operating mode */
    LVM_Fs_en                   SampleRate;             /* Sample rate */
    LVM_Format_en               SourceFormat;           /* Input signal source format */

    /* Compressor parameters */
    LVM_UINT16                  NumBands;               /* Number of active compressor bands */
    LVM_Mode_en                 OutputCompressor;       /* Output compressor active */
    LVMDRC_CompressorParams_st  *pCompressorParams;     /* Pointer to an array of compressor parameters */

} LVMDRC_ControlParams_st;


/* Instance Parameter structure */
typedef struct
{
    /* General */
    LVM_UINT16                  MaxBlockSize;           /* Maximum processing block size */

    /* Compressor capabilities */
    LVM_UINT16                  NumCompressors;         /* Maximum number of compressors including the output compressor */
    LVM_UINT16                  MaxDelayTime;           /* Signal delay time in 500us steps */

    /* Side channel callback */
    LVM_Callback                SideChannelFilter;      /* Side channel filter callback */
    void                        *pFilterData;           /* Pointer to the filter data */

} LVMDRC_InstanceParams_st;


/****************************************************************************************/
/*                                                                                      */
/*  Function Prototypes                                                                 */
/*                                                                                      */
/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVMDRC_GetMemoryTable                                       */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function is used to obtain the LVMDRC module memory requirements to support    */
/*  memory allocation. It can also be used to return the memory base address provided   */
/*  during memory allocation to support freeing of memory when the LVMDRC module is no  */
/*  longer required. It is called in two ways:                                          */
/*                                                                                      */
/*  hInstance = NULL                Returns the memory requirements                     */
/*  hInstance = Instance handle     Returns the memory requirements and allocated       */
/*                                  base addresses.                                     */
/*                                                                                      */
/*  When this function is called with hInstance = NULL the memory base address pointers */
/*  will be NULL on return.                                                             */
/*                                                                                      */
/*  When the function is called for freeing memory, hInstance = Instance Handle the     */
/*  memory table returns the allocated memory and base addresses used during            */
/*  initialisation.                                                                     */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance Handle                                             */
/*  pMemoryTable            Pointer to an empty memory table                            */
/*  pInstanceParams         Pointer to the instance parameters                          */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVMDRC_SUCCESS          Succeeded                                                   */
/*  LVMDRC_NULLADDRESS      When pMemoryTable is NULL                                   */
/*  LVMDRC_NULLADDRESS      When requesting memory requirements and pInstanceParams     */
/*                          is NULL                                                     */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVMDRC_Process function                 */
/*                                                                                      */
/****************************************************************************************/
LVMDRC_ReturnStatus_en LVMDRC_GetMemoryTable(LVMDRC_Handle_t           hInstance,
                                             LVMDRC_MemoryTable_st     *pMemoryTable,
                                             LVMDRC_InstanceParams_st  *pInstanceParams);


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVMDRC_GetInstanceHandle                                    */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function is used to create a LVMDRC module instance. It returns the created    */
/*  instance handle through phInstance. All parameters are set to invalid values, the   */
/*  LVMDRC_SetControlParameters function must be called with a set of valid control     */
/*  parameters before the LVMDRC_Process function can be called.                        */
/*                                                                                      */
/*  The memory allocation must be provided by the application by filling in the memory  */
/*  region base addresses in the memory table before calling this function.             */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  phInstance              Pointer to the instance handle                              */
/*  pMemoryTable            Pointer to the memory definition table                      */
/*  pInstanceParams         Pointer to the instance parameters                          */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVMDRC_SUCCESS          Succeeded                                                   */
/*  LVMDRC_NULLADDRESS      When phInstance or pMemoryTable or pInstanceParams is NULL  */
/*  LVMDRC_NULLADDRESS      When one of the memory regions has a NULL pointer           */
/*                                                                                      */
/* NOTES:                                                                               */
/*                                                                                      */
/****************************************************************************************/
LVMDRC_ReturnStatus_en LVMDRC_GetInstanceHandle(LVMDRC_Handle_t            *phInstance,
                                                LVMDRC_MemoryTable_st      *pMemoryTable,
                                                LVMDRC_InstanceParams_st   *pInstanceParams);


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVXX_GetControlParameters                                   */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Request the LVMDRC module control parameters. The current parameter set is returned */
/*  via the parameter pointer.                                                          */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance handle                                             */
/*  pControlParams          Pointer to an empty parameter structure                     */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVMDRC_SUCCESS          Succeeded                                                   */
/*  LVMDRC_NULLADDRESS      When hInstance or pControlParams is NULL                    */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVMDRC_Process function                 */
/*                                                                                      */
/****************************************************************************************/
LVMDRC_ReturnStatus_en LVMDRC_GetControlParameters(LVMDRC_Handle_t           hInstance,
                                                   LVMDRC_ControlParams_st   *pControlParams);


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVMDRC_SetControlParameters                                 */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Sets or changes the LVMDRC module parameters.                                       */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance handle                                             */
/*  pNewControlParams       Pointer to a parameter structure                            */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVMDRC_SUCCESS          Succeeded                                                   */
/*  LVMDRC_NULLADDRESS      When hInstance or pNewControlParams is NULL                 */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVMDRC_Process function                 */
/*                                                                                      */
/****************************************************************************************/
LVMDRC_ReturnStatus_en LVMDRC_SetControlParameters(LVMDRC_Handle_t           hInstance,
                                                   LVMDRC_ControlParams_st   *pNewControlParams);


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVMDRC_Process                                              */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Process function for the LVMDRC module.                                             */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance handle                                             */
/*  pInData                 Pointer to the input data                                   */
/*  pOutData                Pointer to the output data                                  */
/*  NumSamples              Number of samples in the input buffer                       */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVMDRC_SUCCESS          Succeeded                                                   */
/*  LVMDRC_INVALIDNUMSAMPLES NumSamples was larger than the maximum block size          */
/*                                                                                      */
/* NOTES:                                                                               */
/*                                                                                      */
/****************************************************************************************/
LVMDRC_ReturnStatus_en LVMDRC_Process(LVMDRC_Handle_t     hInstance,
                                      const LVM_INT32     *pInData,
                                      LVM_INT32           *pOutData,
                                      const LVM_UINT16    NumSamples);

/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVMDRC_ClearAudioBuffers                                    */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function is used to clear the internal audio buffers of the module.            */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance handle                                             */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVMDRC_SUCCESS          Initialisation succeeded                                    */
/*  LVMDRC_NULLADDRESS      Instance is NULL                                            */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1. This function must not be interrupted by the LVM_Process function                */
/*                                                                                      */
/****************************************************************************************/
LVMDRC_ReturnStatus_en LVMDRC_ClearAudioBuffers(LVMDRC_Handle_t  hInstance);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif      /* __LVMDRC_H__ */

/* End of file */
