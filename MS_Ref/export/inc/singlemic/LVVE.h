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
/*     $Author: nxp39536 $*/
/*     $Revision: 6244 $*/
/*     $Date: 2011-09-06 16:51:25 +0200 (Tue, 06 Sep 2011) $*/
/*                                                                                      */
/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/*  Header file for the application layer interface of the LVVE module                 */
/*                                                                                      */
/*  This files includes all definitions, types, structures and function prototypes      */
/*  required by the calling layer. All other types, structures and functions are        */
/*  private.                                                                            */
/*                                                                                      */
/****************************************************************************************/

#ifndef __LVVE_H__
#define __LVVE_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****************************************************************************************/
/*                                                                                      */
/*  Includes                                                                            */
/*                                                                                      */
/****************************************************************************************/

#include "LVC_Types.h"

#include "LVHF.h"
#include "LVVC.h"
#include "LVFENS.h"


#include "LVWM.h"


#include "LVMDRC.h"



#define STACK_CHECK 1024
typedef struct
{
    char array[STACK_CHECK];
} stack_st;











/****************************************************************************************/
/*                                                                                      */
/*  Definitions                                                                         */
/*                                                                                      */
/****************************************************************************************/
/* General */

#define LVVE_MAX_BULK_DELAY                6400           /* Maximum bulk delay in samples */
#define LVVE_MAX_VOL_GAIN_DB                 24           /* Maximimum volume gain in dB */
#define LVVE_MIN_VOL_GAIN_DB                -96           /* Minimum volume gain in dB */
#define LVVE_MAX_HPF_CORNER_HZ             1000           /* Maximimum corner frequency of HPF in Hz */
#define LVVE_MAX_HPF_RX_CORNER_HZ          1500           /* Maximimum corner frequency of Rx HPF in Hz */
#define LVVE_MIN_HPF_CORNER_HZ               50           /* Minimum corner frequency of HPF in Hz */
#define LVVE_MAX_NLPP_LIMIT_DB                0           /* Maximimum limit level in dB */
#define LVVE_MIN_NLPP_LIMIT_DB              -24           /* Minimum limit level in dB */
#define LVVE_MAX_EQ_LENGTH_NB                32           /* Maximimum equaliser length for narrowband configuration */
#define LVVE_MAX_EQ_LENGTH_WB                64           /* Maximimum equaliser length for wideband configuration */
#define LVBDM_MAX_TAPS                      200           /* Max Taps for BDM */
#define LVVE_PCM_FRAME_SIZE                  80           /* frame size to be processed in samples */
#define LVVE_MAX_PCM_FRAME_SIZE             160           /* maximum frame size to be processed in samples */

/****************************************************************************************/
/*                                                                                      */
/*  Types                                                                               */
/*                                                                                      */
/****************************************************************************************/
/* Instance handle */
typedef void *LVVE_Tx_Handle_t;
typedef void *LVVE_Rx_Handle_t;

/* Status return values */
typedef enum
{
    LVVE_SUCCESS                            = 0,                       /* Successful return from a routine */
    LVVE_ALIGNMENTERROR                     = 1,                       /* Memory alignment error */
    LVVE_NULLADDRESS                        = 2,                       /* NULL allocation address */
    LVVE_OUTOFRANGE                         = 3,                       /* Out of range parameter */
    LVVE_INVALIDNUMSAMPLES                  = 4,                       /* Invalid number of samples */
    LVVE_INVALID_ALGORITHM_CONFIGURATION    = 5,                       /* Mutally exclusive algorithms configured ON */
    LVVE_RETURNSTATUS_DUMMY                 = LVM_MAXENUM
}LVVE_ReturnStatus_en_ ;
typedef LVM_UINT32 LVVE_ReturnStatus_en;

/* Tx Operating Mode */
typedef enum
{
    LVVE_TX_MODE_OFF                        = 0,                       /* Tx voice engine is turned off */
    LVVE_TX_MODE_ON                         = 1,                       /* Tx voice in will run */
    LVVE_TX_MODE_BYPASS_MIC0                = 2,                       /* Tx voice engine will run but output will Mic0 signal */
    LVVE_TX_MODE_BYPASS_MIC1                = 3,                       /* Tx voice engine will run but output will Mic1 signal */
    LVVE_TX_MODE_BYPASS_REF                 = 4,                       /* Tx voice engine will run but output will Ref signal */
    LVVE_TX_MODE_DUMMY                      = LVM_MAXENUM
} LVVE_Tx_Mode_en_;
typedef LVM_UINT32 LVVE_Tx_Mode_en;

/* Rx Operating Mode */
typedef enum
{
    LVVE_RX_MODE_OFF                        = 0,                       /* Rx voice engine is turned off */
    LVVE_RX_MODE_ON                         = 1,                       /* Rx voice in will run */
    LVVE_RX_MODE_BYPASS                     = 2,                       /* Rx voice engine will run but output will be ignored */
    LVVE_RX_MODE_DUMMY                      = LVM_MAXENUM
} LVVE_Rx_Mode_en_;
typedef LVM_UINT32 LVVE_Rx_Mode_en;

/****************************************************************************************/
/*                                                                                      */
/*  Module Control Parameters                                                           */
/*                                                                                      */
/****************************************************************************************/

typedef struct
{
    /* General */
    LVM_INT16                     NLPP_Limit;
    LVM_UINT16                    NLPP_HPF_CornerFreq;

} LVNLPP_ControlParams_st;

typedef struct
{
    /* General */
    LVM_UINT16                    EQ_Length;
    LVM_INT16                     *pEQ_Coefs;

} LVEQ_ControlParams_st;

typedef struct
{
    LVM_Mode_en                 OperatingMode;  // /* Operating mode for DRC */
    /* Parameters for compressor */
    LVM_UINT16                  NumKnees; //Max is 5, minimum is 1 for top level [input level 0dBFs]
    LVM_INT16                   CompressorCurveInputLevels[LVMDRC_MAX_KNEES]; //Specified in dBFS [range is -96dB till 0dBFs]
    LVM_INT16                   CompressorCurveOutputLevels[LVMDRC_MAX_KNEES]; //Specified in dBFS
    LVM_INT16                   AttackTime; //specified in  ms
    LVM_INT16                   ReleaseTime; //specified in ms
    LVM_Mode_en                 LimiterOperatingMode;          /* Operating mode for limiter*/
    LVM_INT16                   LimitLevel; //Expressed in dBFS
} LVDRC_ControlParams_st;

/* LMfV Control Structure definition*/
typedef struct
{
    LVM_INT16                  CNG_Volume;     /* CNG Output Volume in dB (-90 to 0 dB)*/
} CNGControl_Parameters_st;



/****************************************************************************************/
/*                                                                                      */
/*  Module Status Parameters                                                            */
/*                                                                                      */
/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/*  Module Instance Parameters                                                          */
/*                                                                                      */
/****************************************************************************************/

typedef struct
{
    /* General */
    LVM_UINT16                    EQ_MaxLength;            /* Max Size of Equaliser */

} LVEQ_InstanceParams_st;

/****************************************************************************************/
/*                                                                                      */
/*  Structures                                                                          */
/*                                                                                      */
/****************************************************************************************/

/* Tx Control Parameter structure */
typedef struct
{
    /* General parameters */
    LVVE_Tx_Mode_en               OperatingMode;          /* Operating mode */

    /* Parameters for VE_Rx */
    LVM_Mode_en                   Mute;


    /* Delay Parameter */
    LVM_Mode_en                   BD_OperatingMode;
    LVM_UINT16                    BulkDelay;

    /* VOL parameters */
    LVM_Mode_en                   VOL_OperatingMode;
    LVM_INT16                     VOL_Gain;

    /* Microphone High Pass Filer */
    LVM_Mode_en                   HPF_OperatingMode;
    LVM_UINT16                    MIC_HPF_CornerFreq;

    /* Hands Free parameters */
    LVHF_ControlParams_st         HF_ControlParams;


    /* Whisper Mode parameters */
    LVWM_ControlParams_st         WM_ControlParams;

    /* Equalizer parameters */
    LVM_Mode_en                   EQ_OperatingMode;
    LVEQ_ControlParams_st         EQ_ControlParams;


    /* DRC parameters */
    LVDRC_ControlParams_st        DRC_ControlParams;
} LVVE_Tx_ControlParams_st;



/* Rx Control Parameter structure */
typedef struct
{
    /* General parameters */
    LVVE_Rx_Mode_en                     OperatingMode;          /* Operating mode */

    /* Parameters for VE_Rx */
    LVM_Mode_en                         Mute;

    /* VOL parameters */
    LVM_Mode_en                         VOL_OperatingMode;
    LVM_INT16                           VOL_Gain;

    /* FENS parameters */
    LVFENS_ControlParams_st             FENS_ControlParams;


    /* NLPP parameters */
    LVM_Mode_en                         NLPP_OperatingMode;
    LVNLPP_ControlParams_st             NLPP_ControlParams;

    /* Voice Clarity parameters */
    LVVC_ControlParams_st               VC_ControlParams;

    /* Equalizer parameters */
    LVM_Mode_en                         EQ_OperatingMode;
    LVEQ_ControlParams_st               EQ_ControlParams;


    /* Microphone High Pass Filer */
    LVM_Mode_en                         HPF_OperatingMode;
    LVM_UINT16                          HPF_CornerFreq;

    /* CNG Parameters*/
    LVM_Mode_en                         CNG_OperatingMode;        /* Operating mode for CNG */
    CNGControl_Parameters_st             CNG_ControlParams;

    /* Whisper Mode parameters */
    LVWM_ControlParams_st               WM_ControlParams;


} LVVE_Rx_ControlParams_st;

/* Tx Instance Parameter structure */
typedef struct
{
    /* Parameters for VE_Tx */

    LVM_Fs_en                   SampleRate;             /* Sample rate */

    /* Delay Parameters */
    LVM_UINT16                  MaxBulkDelay;

    /* Equalizer parameters */
    LVEQ_InstanceParams_st      EQ_InstParams;

} LVVE_Tx_InstanceParams_st;


/* Rx Instance Parameter structure */
typedef struct
{
    /* Parameters for VE_Rx */

    LVM_Fs_en                   SampleRate;             /* Sample rate */

    /* Equalizer parameters */
    LVEQ_InstanceParams_st      EQ_InstParams;

} LVVE_Rx_InstanceParams_st;

/* Tx Status Parameter structure */
typedef struct
{
    /* Hands Free parameters */
    LVHF_StatusParams_st        HF_StatusParams;


    /* Whisper Mode parameters */
    LVWM_StatusParams_st        WM_StatusParams;

    LVM_INT8 dummy;

} LVVE_Tx_StatusParams_st;

/* Rx Status Parameter structure */
typedef struct
{

    /* Voice clarity parameters */
    LVVC_StatusParams_st        VC_StatusParams;

    /* Whisper Mode parameters */
    LVWM_StatusParams_st        WM_StatusParams;

    LVM_INT8 dummy;

} LVVE_Rx_StatusParams_st;

typedef struct
{
    LVM_CHAR                    VersionNumber[64];
} LVVE_VersionInfo;


typedef struct
{
    LVM_ResetType_en ResetType;
} LVVE_Rx_ResetParams_st;

typedef struct
{
    LVM_ResetType_en ResetType;
} LVVE_Tx_ResetParams_st;

/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVVE_Tx_GetMemoryTable                                      */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function is used to obtain the LVVE_Tx module memory requirements to support   */
/*  memory allocation. It can also be used to return the memory base address provided   */
/*  during memory allocation to support freeing of memory when the LVVE_Tx module is no */
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
/*  LVVE_SUCCESS           Succeeded                                                    */
/*  LVVE_NULLADDRESS       When pMemoryTable is NULL                                    */
/*  LVVE_NULLADDRESS       When requesting memory requirements and pInstanceParams      */
/*                          is NULL                                                     */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVVE_Tx_Process function                */
/*                                                                                      */
/****************************************************************************************/
LVVE_ReturnStatus_en LVVE_Tx_GetMemoryTable(LVVE_Tx_Handle_t               hInstance,
                                            LVM_MemoryTable_st             *pMemoryTable,
                                            LVVE_Tx_InstanceParams_st      *pInstanceParams);

/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVVE_Rx_GetMemoryTable                                      */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function is used to obtain the LVVE_Rx module memory requirements to support   */
/*  memory allocation. It can also be used to return the memory base address provided   */
/*  during memory allocation to support freeing of memory when the LVVE_Rx module is no */
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
/*  LVVE_SUCCESS           Succeeded                                                    */
/*  LVVE_NULLADDRESS       When pMemoryTable is NULL                                    */
/*  LVVE_NULLADDRESS       When requesting memory requirements and pInstanceParams      */
/*                          is NULL                                                     */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVVE_Rx_Process function                */
/*                                                                                      */
/****************************************************************************************/
LVVE_ReturnStatus_en LVVE_Rx_GetMemoryTable(LVVE_Rx_Handle_t               hInstance,
                                            LVM_MemoryTable_st             *pMemoryTable,
                                            LVVE_Rx_InstanceParams_st      *pInstanceParams);

/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVVE_Tx_GetInstanceHandle                                   */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function is used to create a LVVE_Tx module instance. It returns the created   */
/*  instance handle through phInstance. All parameters are set to invalid values, the   */
/*  LVVE_Tx_SetControlParameters function must be called with a set of valid control    */
/*  parameters before the LVVE_Tx_Process function can be called.                       */
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
/*  LVVE_SUCCESS           Succeeded                                                    */
/*  LVVE_NULLADDRESS       When phInstance or pMemoryTable or pInstanceParams is NULL   */
/*  LVVE_NULLADDRESS       When one of the memory regions has a NULL pointer            */
/*                                                                                      */
/* NOTES:                                                                               */
/*                                                                                      */
/****************************************************************************************/
LVVE_ReturnStatus_en LVVE_Tx_GetInstanceHandle(LVVE_Tx_Handle_t                *phInstance,
                                               LVM_MemoryTable_st              *pMemoryTable,
                                               LVVE_Tx_InstanceParams_st       *pInstanceParams);

/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVVE_Rx_GetInstanceHandle                                   */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function is used to create a LVVE_Rx module instance. It returns the created   */
/*  instance handle through phInstance. All parameters are set to invalid values, the   */
/*  LVVE_Rx_SetControlParameters function must be called with a set of valid control    */
/*  parameters before the LVVE_Rx_Process function can be called.                       */
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
/*  LVVE_SUCCESS           Succeeded                                                    */
/*  LVVE_NULLADDRESS       When phInstance or pMemoryTable or pInstanceParams is NULL   */
/*  LVVE_NULLADDRESS       When one of the memory regions has a NULL pointer            */
/*                                                                                      */
/* NOTES:                                                                               */
/*                                                                                      */
/****************************************************************************************/
LVVE_ReturnStatus_en LVVE_Rx_GetInstanceHandle(LVVE_Rx_Handle_t                *phInstance,
                                               LVM_MemoryTable_st              *pMemoryTable,
                                               LVVE_Rx_InstanceParams_st       *pInstanceParams);

/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVVE_Tx_GetControlParameters                                */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Request the LVVE_Tx module control parameters. The current parameter set is returned*/
/*  via the parameter pointer.                                                          */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance handle                                             */
/*  pControlParams          Pointer to an empty parameter structure                     */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVVE_SUCCESS           Succeeded                                                    */
/*  LVVE_NULLADDRESS       When hInstance or pControlParams is NULL                     */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVVE_Tx_Process function                */
/*                                                                                      */
/****************************************************************************************/
LVVE_ReturnStatus_en LVVE_Tx_GetControlParameters(   LVVE_Tx_Handle_t           hInstance,
                                                     LVVE_Tx_ControlParams_st   *pControlParams);


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVVE_Rx_GetControlParameters                                */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Request the LVVE_Rx module control parameters. The current parameter set is returned*/
/*  via the parameter pointer.                                                          */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance handle                                             */
/*  pControlParams          Pointer to an empty parameter structure                     */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVVE_SUCCESS           Succeeded                                                    */
/*  LVVE_NULLADDRESS       When hInstance or pControlParams is NULL                     */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVVE_Rx_Process function                */
/*                                                                                      */
/****************************************************************************************/
LVVE_ReturnStatus_en LVVE_Rx_GetControlParameters(   LVVE_Rx_Handle_t           hInstance,
                                                     LVVE_Rx_ControlParams_st   *pControlParams);

/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVVE_Tx_SetControlParameters                                */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Sets or changes the LVVE_Tx module parameters.                                      */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance handle                                             */
/*  pNewParams              Pointer to a parameter structure                            */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVVE_SUCCESS           Succeeded                                                    */
/*  LVVE_NULLADDRESS       When hInstance or pNewParams is NULL                         */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVVE_Tx_Process function                */
/*                                                                                      */
/****************************************************************************************/
LVVE_ReturnStatus_en LVVE_Tx_SetControlParameters(   LVVE_Tx_Handle_t           hInstance,
                                                     LVVE_Tx_ControlParams_st   *pNewParams);

/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVVE_Rx_SetControlParameters                                */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Sets or changes the LVVE_Rx module parameters.                                      */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance handle                                             */
/*  pNewParams              Pointer to a parameter structure                            */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVVE_SUCCESS           Succeeded                                                    */
/*  LVVE_NULLADDRESS       When hInstance or pNewParams is NULL                         */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVVE_Rx_Process function                */
/*                                                                                      */
/****************************************************************************************/
LVVE_ReturnStatus_en LVVE_Rx_SetControlParameters(   LVVE_Rx_Handle_t           hInstance,
                                                     LVVE_Rx_ControlParams_st   *pNewParams);


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVVE_Tx_GetStatusParameters                                 */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Request the LVVE module status parameters. The current parameter set is returned    */
/*  via the parameter pointer.                                                          */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance handle                                             */
/*  pStatusParams           Pointer to an empty parameter structure                     */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVVE_SUCCESS           Succeeded                                                    */
/*  LVVE_NULLADDRESS       When hInstance or pStatusParams is NULL                      */
/*                                                                                      */
/*                                                                                      */
/****************************************************************************************/
LVVE_ReturnStatus_en LVVE_Tx_GetStatusParameters(    LVVE_Tx_Handle_t           hInstance,
                                                     LVVE_Tx_StatusParams_st    *pStatusParams);


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVVE_Tx_ResetInstance                                       */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Resets the LVVE Rx module.                                                          */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance handle                                             */
/*  pResetParams            Reset parameters                                            */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVVE_SUCCESS            Succeeded                                                   */
/*  LVVE_NULLADDRESS        When hInstance or pResetParams are NULL                     */
/*  LVVE_OUTOFRANGE         When incorrect reset parameters are provided                */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVVE_Tx_Process function                */
/*                                                                                      */
/****************************************************************************************/
LVVE_ReturnStatus_en LVVE_Tx_ResetInstance(  LVVE_Tx_Handle_t        hInstance,
                                             LVVE_Tx_ResetParams_st *pResetParams  );


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVVE_Rx_GetStatusParameters                                 */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Request the LVVE module status parameters. The current parameter set is returned    */
/*  via the parameter pointer.                                                          */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance handle                                             */
/*  pStatusParams           Pointer to an empty parameter structure                     */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVVE_SUCCESS           Succeeded                                                    */
/*  LVVE_NULLADDRESS       When hInstance or pStatusParams is NULL                      */
/*                                                                                      */
/*                                                                                      */
/****************************************************************************************/
LVVE_ReturnStatus_en LVVE_Rx_GetStatusParameters(    LVVE_Rx_Handle_t           hInstance,
                                                     LVVE_Rx_StatusParams_st    *pStatusParams);

/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVVE_Tx_Process                                             */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Process function for the LVVE_Tx module.                                            */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance handle                                             */
/*  pInDataMic0             Pointer to the input data (primary microphone)              */
/*  pInDataMic1             Pointer to the input data (secondary microphone)            */
/*  pRefData                Pointer to the reference data                               */
/*  pNoiseData              Pointer to the noise data                                   */
/*  pOutData                Pointer to the output data                                  */
/*  NumSamples              Number of samples in the input buffer                       */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVVE_SUCCESS           Succeeded                                                    */
/*  LVVE_INVALIDNUMSAMPLES NumSamples was larger than the maximum block size            */
/*  LVVE_Tx_ALIGNMENTERROR    When either the input our output buffers are not 32-bit   */
/*                          aligned                                                     */
/*  LVVE_NULLADDRESS       When one of hInstance, pInData or pOutData is NULL           */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1. The input and output buffers must be 32-bit aligned                              */
/*                                                                                      */
/****************************************************************************************/
LVVE_ReturnStatus_en LVVE_Tx_Process(   LVVE_Tx_Handle_t    hInstance,
                                        const LVM_INT16     *pInDataMic0,
                                        const LVM_INT16     *pInDataMic1,
                                        const LVM_INT16     *pRefData,
                                        LVM_INT16           *pNoiseData,
                                        LVM_INT16           *pOutData,
                                        const LVM_UINT16    NumSamples);

/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVVE_Rx_Process                                             */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Process function for the LVVE_Rx module.                                            */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance handle                                             */
/*  pInData                 Pointer to the input data                                   */
/*  pNoiseData              Pointer to the noise buffer (for LVVC only)                 */
/*  pOutData                Pointer to the output data                                  */
/*  NumSamples              Number of samples in the input buffer                       */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVVE_Success           Succeeded                                                    */
/*  LVVE_INVALIDNUMSAMPLES NumSamples was larger than the maximum block size            */
/*  LVVE_ALIGNMENTERROR    When either the input our output buffers are not 32-bit      */
/*                          aligned                                                     */
/*  LVVE_NULLADDRESS       When one of hInstance, pInData or pOutData is NULL           */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1. The input and output buffers must be 32-bit aligned                              */
/*                                                                                      */
/****************************************************************************************/

LVVE_ReturnStatus_en LVVE_Rx_Process(   LVVE_Rx_Handle_t    hInstance,
                                        const LVM_INT16     *pInData,
                                        const LVM_INT16     *pNoiseData,
                                        LVM_INT16           *pOutData,
                                        const LVM_UINT16    NumSamples);


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVVE_Rx_ResetInstance                                       */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Resets the LVVE Rx module.                                                          */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance handle                                             */
/*  pResetParams            Reset parameters                                            */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVVE_SUCCESS            Succeeded                                                   */
/*  LVVE_NULLADDRESS        When hInstance or pResetParams are NULL                     */
/*  LVVE_OUTOFRANGE         When incorrect reset parameters are provided                */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVVE_Rx_Process function                */
/*                                                                                      */
/****************************************************************************************/
LVVE_ReturnStatus_en LVVE_Rx_ResetInstance(  LVVE_Rx_Handle_t        hInstance,
                                             LVVE_Rx_ResetParams_st *pResetParams  );





/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVVE_GetVersionInfo                                         */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  get the version of the current build   .                                            */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  pVersion                version info                                                */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVVE_SUCCESS           Succeeded                                                    */
/*  LVVE_NULLADDRESS       When one of hInstance, pInData or pOutData is NULL           */
/*  LVVE_OUT                                                                            */
/*                                                                                      */
/* NOTES:                                                                               */
/*                                                                                      */
/****************************************************************************************/

LVVE_ReturnStatus_en LVVE_GetVersionInfo(
                                        LVVE_VersionInfo *pVersion
                                        );






#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif      /* __LVVE_H__ */

/* End of file */
