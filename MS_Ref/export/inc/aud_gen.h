/******************************************************************************
 ** File Name:      aud_gen.h                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           12/11/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    General types and macro definition.*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/11/2005     Benjamin.Wang     Create.                                  *
 ******************************************************************************/

 /*! \file aud_gen.h
  *  \author Benjamin Wang
  *  \date Nov 12, 2005
  *  \brief General types and macro definition of audio service..
  */

#ifndef _AUD_GEN_H
#define _AUD_GEN_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
#define DEBUG_AUDIO

#ifdef DEBUG_AUDIO
    #ifndef AUDIO_PRINT
    #define AUDIO_PRINT( _format_string )   SCI_TRACE_LOW _format_string
    #endif
#else
    #ifndef AUDIO_PRINT
    #define AUDIO_PRINT( _format_string ) 
    #endif
#endif

#ifndef AUDIO_ASSERT
#define AUDIO_ASSERT SCI_PASSERT
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/
/*! \def IGNORE_PARAM 
 *  \brief Used to ignore unused parameters.*/
#define IGNORE_PARAM(param) (param = param)

/*! \def IGNORED_AREA 
 *  \brief Used to ignore no cared handle area.*/
#define IGNORED_AREA (0)

/*! \def AUDIO_CODEC_DEFAULT_PRIORITY \brief Codec's default priority. It is recommended.*/
#define AUDIO_CODEC_DEFAULT_PRIORITY (31)

/*! \def AUDIO_CODEC_DEFAULT_PRIORITY 
 *  \brief Before the audio task is created, we use this value to fill in task_id variable.*/
#define AUDIO_DEFAULT_TASK_ID (0xFFFFFFFF)

/*! \def ANSI2UINT16 
 *  \brief Convert ANSI string character to uint16 character.
 *  \note this operation can't be used to convert GB Chinese character to UNICODE character!
 */
#define ANSI2UINT16(quote) (L##quote) 

/*! \def AUDIO_MODULE 
 *  \brief Assigned audio module ID.
 */
#define AUDIO_MODULE (0xA1)

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
/*! \enum AUDIO_HandleType_enum \brief Audio module's main handle types' definition.*/
enum AUDIO_HandleType_enum
{
    INVALID_HANDLETYPE = 0,     /*!< Invalid handle type.*/
    AUDIO_HANDLE,   /*!<1. Audio Handle.*/
    PLUGGER_HANDLE, /*!<2. Plugger Handle.*/
    COLLISION_HANDLE, /*!<3. Collision Handle.*/
    DEVICE_HANDLE /*!<4. Audio Device Handle*/
};

/*! \enum AUDIO_PluggerHandle_SubHandleType_enum \brief Sub handle types' definition.*/
enum AUDIO_PluggerHandle_SubHandleType_enum
{
    INVALID_SUBHANDLETYPE = 0, /*!< Invalid handle type.*/
    CODEC_HANDLE, /*!<1. Codec Handle.*/
    EXPRESS_HANDLE /*!<2. Expression Handle.*/
};

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _AUD_GEN_H

// End of aud_gen.h

