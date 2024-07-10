/*****************************************************************************
** File Name:      mmi_parse.h                                               *
** Author:                                                                   *
** Date:           14/01/2007                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define mmi parse function            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2007       bruce.chi           Create
******************************************************************************/

#ifndef MMIPARSE_H
#define MMIPARSE_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiphone_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define PARS_MAX_STR_LEN	41

#define PARS_MAX_SI_NUM		4
#define PARS_PSWD_LEN		4
#define PARS_MAX_LEN_OF_SI 21

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
    MMI_PARSE_NORMAL_CALL_TYPE_E,
    MMI_PARSE_CALL_CLIR_SUPPRESSION_E,
    MMI_PARSE_CALL_CLIR_INVOCATION_E,
    MMI_PARSE_SS_TYPE_E,
    MMI_PARSE_SIM_CTRL_TYPE_E,
    MMI_PARSE_USSD_TYPE_E,
    MMI_PARSE_VIDEO_TYPE_E,
    MMI_PARSE_TYPE_MAX
}MMI_PARSE_TYPE_E;

/* According to GSM spec 02.30 Annex B */
typedef enum parsTypeOfSITag
{
    PARS_DIRECTORY_NUMBER,
    PARS_UUS_REQUIRED,
    PARS_PASSWORD,
    PARS_BASIC_SERVICE_GROUP,
    PARS_NO_REPLY_CONDITION_TIMER,
    PARS_PIN_CODE,

    PARS_UNKNOWN_SI_TYPE,
    PARS_NUM_OF_TYPE_OF_SI = PARS_UNKNOWN_SI_TYPE

} parsTypeOfSIE;

typedef enum parsCtrlCodeTag
{
    PARS_CHANGE_PIN1,
    PARS_CHANGE_PIN2,
    PARS_UNBLOCKING_PIN1,
    PARS_UNBLOCKING_PIN2,
    PARS_IMEI,
    PARS_MANUFACTURE_TEST,
    #ifdef AUTODEMO_SUPPORT
    AUTODEMO_TEST,
    #endif
    PARS_SHOW_VERSION,
    PARS_ACOUSTICS_TEST,
    PARS_RESET_LANGUAGE,
    PARS_POWER_SAVING_ON,
    PARS_POWER_SAVING_OFF,
    PARS_ENGINEER_MENU,
  #ifdef MEDIA_DEMO_SUPPORT  
    PARS_ENGINEER_MEDIA_MENU,
  #endif   
    PARS_ADC_CAL_VALUE,       /* Tungwei Lin for *#308# */
    PARS_RESET_PHONE_CODE,
    PARS_NV_COUNTING,

    PARS_CTRL_CODE_UNDETERMINED,
    PARS_NUM_OF_CTRL_CODE = PARS_CTRL_CODE_UNDETERMINED
}parsCtrlCodeE;

typedef enum parsResultCodeTag
{
    PARS_RESULT_SUCCESS,
    PARS_RESULT_FORMAT_ERROR,
    PARS_RESULT_PIN_CONTRADICT,

    PARS_RESULT_MAX,
}parsErrorCodeE;

typedef struct parsServiceInfoTag
{
    parsTypeOfSIE   siType;
    char            siData[PARS_MAX_LEN_OF_SI];

}   parsSvcInfoS;

typedef enum parsOperationCodeTag
{
    PARS_SS_REGISTRATION = 0,/* "**" */
    PARS_SS_ERASURE,         /* "##" */
    PARS_SS_ACTIVATION,      /* "*"  */
    PARS_SS_DEACTIVATION,    /* "#"  */
    PARS_SS_INTERROGATION,   /* "*#" */

    PARS_OPERATION_CODE_UNDETERMINED,
    PARS_NUM_OF_OPERATION_CODE = PARS_OPERATION_CODE_UNDETERMINED
} parsOptCodeE;


typedef struct parsStrInfoTag
{
    char            inputStr[PARS_MAX_STR_LEN]; /* Store the input string */
    uint8           strLen;    /* The length of this input string         */
    parsOptCodeE    optCode;   /* OperationCode of this SS input string   */
    parsCtrlCodeE   ctrlCode;  /* ContrlCode of this SIM ctrl input string*/
    uint8           numOfSI;   /* How many SIs in this SS input string    */
    uint8           numOfMandatorySI; /* Maximum SIs for this SS input .  */
    /* The parsed each SI string can be  digit string or '*'. */
    parsSvcInfoS    siField[PARS_MAX_SI_NUM];

}   parsStrInfoS;

typedef struct parsInfoStoreTag
{
    /* ussSendAgain for contrl the phase 1 signal      */
    /* procedure of USS.                               */
    BOOLEAN ussSendAgain;
    /* Store the input string for USSD sending again.  */
    char    ussdStr[PARS_MAX_STR_LEN];
    BOOLEAN isOldPswdExist;     /* Password is available. */
    /* Store the net password in this SS input string  */
    char    oldPwd[PARS_PSWD_LEN+1];
    char    newPwd[PARS_PSWD_LEN+1];
    BOOLEAN getChvRspFlag;
}   parsInfoStoreS;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : parse telephone number
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC MMI_PARSE_TYPE_E MMIAPIPARSE_ParseTeleNum(MN_DUAL_SYS_E dual_sys, uint8 *str, uint8 str_len, BOOLEAN is_video_call);

/*****************************************************************************/
// 	Description : init pars string
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPARSE_ParsInitialise( parsStrInfoS* parsStrInfo, BOOLEAN strInitialised , const uint8 *str);

/*****************************************************************************/
// 	Description : this function is used to parse user input string,
//  put user input string value to struct MMI_PUK_PIN_INFO_T 
//	Global resource dependence : 
//  Author: jassmine.meng
//	Note: when use puk change pin, extpin mean puk, pin mean new pin;
//        when use pin change pin, extpin mean old pin, pin mean new pin.
/*****************************************************************************/
PUBLIC parsErrorCodeE MMIAPIPARSE_ParsePukAndPinStr(
                              char              *str,
                              MMI_PIN_INFO_T    *pin_info
                              );

/*****************************************************************************/
// 	Description : process the "*#06#" imei
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPARSE_ProcessImeiStr(
                                   uint8* str_ptr, //[IN] input string pointer
                                   uint16 str_len  //[IN] input string length
                                   );

/*****************************************************************************/
// 	Description : parse dialing number string
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:解析跟sim卡无关的操作
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPARSE_ParseString(uint8 *str, uint8 str_len);

/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/

#endif
