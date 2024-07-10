/******************************************************************************
 ** File Name:      apm_codec.h                                              
 ** Author:         shujing.dong                                              
 ** DATE:           11/15/2005                                                
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         
 ** Description:    This file defines the basic operation interfaces  
 **                  and data structure of apm_codec
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       
 **  -----------------------------------------------------------------------  
 ** DATE           NAME             DESCRIPTION                               
 ** 11/15/2005     shujing.dong     Create.                                   
 ******************************************************************************/
  /*! \file apm_codec.h
  *  \author Shujing.Dong 
  *  \date Nov 15, 2005
  *  \brief This file defines the basic operation interfaces and data structure 
  *         of audio plugger - codec - manage. 
  */ 
#ifndef _APM_CODEC_H
#define _APM_CODEC_H
/**---------------------------------------------------------------------------**
 **                         Dependencies                                      **
 **---------------------------------------------------------------------------**/
#include "audio_api.h"
#include "asm.h"
#include "apm_gen.h"


/**---------------------------------------------------------------------------**
 **                        Debugging Flag                                     **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------**/
#ifdef __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------**
 **                         MACRO Definations                                 **
 **---------------------------------------------------------------------------**/

	
/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/
 /*----------------------------------------------------------------------------*/
/*! Request:
 *  Requestion is launched by audio service layer to inform audio codec to do something.
 */
/*----------------------------------------------------------------------------*/
 /*----------------------------------------------------------------------------*/
/*! CODEC TASK MSG UNION:
 */
/*----------------------------------------------------------------------------*/
/*! \enum EXT_OPE_CM_OPE_E
 *  \brief CM operation process list in APM xtend Operation .
 */
typedef enum
{
	APM_EXT_ENTER,/*!<enter collision.*/
	APM_EXT_EXIT, /*!<exit collision.*/
	APM_EXT_SCI_NULL  /*!<neither enter, nor exit.*/
}EXT_OPE_CM_OPE_E;


/*! \enum DES_FLAG_E
 *  \brief description type list.
 *            determine get codec_des_info or codec_ext_ope_des_info
 */
typedef enum 
{
	CODEC_DES,          /*!<codec description.*/     
	CODEC_EXT_OPE_DES   /*!<codec extend operation description.*/ 
}DES_FLAG_E;

/*! \enum AUDIO_CODEC_VB_CONTROL_E
 *  \brief determine which control vb, DSP or ARM or neither.
 */
typedef enum
{
    VB_CONTROL_BY_DSP = 0,/*!<by dsp.*/ 
    VB_CONTROL_BY_ARM,    /*!<by arm.*/ 
    VB_CONTROL_MAX        /*!<neither.*/ 
}AUDIO_CODEC_VB_CONTROL_E;

/*! \typedef CONSTRUCT_CALLBACK_PFUNC
 *  \brief Audio construct callback function prototype.
 *  \return return operation result. \sa AUDIO_RESULT_E
 */
typedef AUDIO_RESULT_E (* CONSTRUCT_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *   /*!<the audio object ptr.*/
    );

/*! \typedef DECONSTRUCT_CALLBACK_PFUNC
 *  \brief Audio deconstruct callback function prototype.
 *  \return return operation result. \sa AUDIO_RESULT_E
 */
typedef AUDIO_RESULT_E (* DECONSTRUCT_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *   /*!<the audio object ptr.*/
    );

/*! \typedef PLAY_CONSTRUCT_CALLBACK_PFUNC
 *  \brief Audio play construct callback function prototype.
 *  \return return operation result. \sa AUDIO_RESULT_E
 */
typedef AUDIO_RESULT_E (* PLAY_CONSTRUCT_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *              /*!<the audio object ptr.*/
    );

/*! \typedef STOP_DECONSTRUCT_CALLBACK_PFUNC
 *  \brief Audio stop deconstruct callback function prototype.
 *  \return return operation result. \sa AUDIO_RESULT_E
 */
typedef AUDIO_RESULT_E (* STOP_DECONSTRUCT_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *              /*!<the audio object ptr.*/
    );

/*! \typedef RESUME_CONSTRUCT_CALLBACK_PFUNC
 *  \brief Audio resume construct callback function prototype.
 *  \return return operation result. \sa AUDIO_RESULT_E
 */
typedef AUDIO_RESULT_E (* RESUME_CONSTRUCT_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *              /*!<the audio object ptr.*/
    );

/*! \typedef PAUSE_DECONSTRUCT_CALLBACK_PFUNC
 *  \brief Audio pause deconstruct callback function prototype.
 *  \return return operation result. \sa AUDIO_RESULT_E
 */
typedef AUDIO_RESULT_E (* PAUSE_DECONSTRUCT_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *              /*!<the audio object ptr.*/
    );

/*! \typedef PLAY_CALLBACK_PFUNC
 *  \brief Audio play callback function prototype.
 *  \return return operation result. \sa AUDIO_RESULT_E
 */
typedef AUDIO_RESULT_E (* PLAY_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *              /*!<the audio object ptr.*/
    );
    
/*! \typedef STOP_CALLBACK_PFUNC
 *  \brief Audio stop callback function prototype.
 *  \return return operation result. \sa AUDIO_RESULT_E
 */    
typedef AUDIO_RESULT_E (* STOP_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *              /*!<the audio object ptr.*/
    );
    
/*! \typedef PAUSE_CALLBACK_PFUNC
 *  \brief Audio pause callback function prototype.
 *  \return return operation result. \sa AUDIO_RESULT_E
 */
typedef AUDIO_RESULT_E (* PAUSE_CALLBACK_PFUNC)(
  AUDIO_OBJECT_T *              /*!<the audio object ptr.*/
    );
    
/*! \typedef RESUME_CALLBACK_PFUNC
 *  \brief Audio resume callback function prototype.
 *  \return return operation result. \sa AUDIO_RESULT_E
 */
typedef AUDIO_RESULT_E (* RESUME_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *              /*!<the audio object ptr.*/
    );
/*! \typedef SEEK_CALLBACK_PFUNC
 *  \brief Audio seek callback function prototype.
 *  \return return operation result. \sa AUDIO_RESULT_E
 */
typedef AUDIO_RESULT_E (* SEEK_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *,              /*!<the audio object ptr.*/
    uint32               /*!<.*/ 
    );

/*! \typedef SEND_STREAM_DATA_CALLBACK_PFUNC
 *  \brief Audio send stream data callback function prototype.
 *  \return return operation result. \sa AUDIO_RESULT_E
 */
typedef AUDIO_RESULT_E (* SEND_STREAM_DATA_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *,              /*!<the audio object ptr.*/
    AUDIO_STREAM_CURSRCDATA_INFO_T *tpStreamCurSrcDataInfo /*!<the stream data info.*/ 
    );

/*! \typedef PLAY_CALLBACK_PFUNC
 *  \brief Audio set speed callback function prototype.
 *  \return return operation result. \sa AUDIO_RESULT_E
 */
typedef AUDIO_RESULT_E (* SET_SPEED_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T * ,             /*!<the audio object ptr.*/
    int32               /*!<the seek offset.*/ 
    );

/*! \typedef SET_DIGITAL_GAIN_CALLBACK_PFUNC
 *  \brief Audio set digital gain callback function prototype.
 *  \return return operation result. \sa AUDIO_RESULT_E
 */
typedef AUDIO_RESULT_E (* SET_DIGITAL_GAIN_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *,              /*!<the audio object ptr.*/
    uint32               /*!<the new gain value.*/ 
    );

/*! \typedef GET_DIGITAL_GAIN_CALLBACK_PFUNC
 *  \brief Audio get digital gain callback function prototype.
 *  \return return current digital gain. \sa uint32
 */
typedef uint32 (* GET_DIGITAL_GAIN_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *              /*!<the audio object ptr.*/
    );

/*! \typedef GET_OUTPUT_SAMPLERATE_CALLBACK_PFUNC
 *  \brief Audio get samplerate callback function prototype.
 *  \return return samplerate. \sa uint32
 */
typedef uint32 (* GET_OUTPUT_SAMPLERATE_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *              /*!<the audio object ptr.*/
    );
    
/*! \typedef SET_OUTPUT_SAMPLERATE_CALLBACK_PFUNC
 *  \brief Audio set codec output samplerate callback function prototype.
 *  \       if the data processing is not related with the output samplerate
 *  \       this interface can be null.
 *  \       for example, the samplerate of pcm data in midi codec is related to
 *  \       the output samplerate.
 *  \return return result. \sa AUDIO_RESULT_E
 */
typedef AUDIO_RESULT_E (* SET_OUTPUT_SAMPLERATE_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *              /*!<the audio object ptr.*/
    );
    
/*! \typedef GET_FILE_INFO_CALLBACK_PFUNC
 *  \brief Audio get fileInfo callback function prototype.
 *  \return return operation result. \sa AUDIO_RESULT_E
 */
typedef AUDIO_RESULT_E (* IDENTIFY_FORMAT_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *              /*!<the audio object ptr.*/
    );

/*! \typedef GET_PLAYING_INFO_CALLBACK_PFUNC
 *  \brief Audio get playing info callback function prototype.
 *  \return return playing info struct. \sa AUDIO_PLAYINFO_T
 */    
typedef AUDIO_RESULT_E (* GET_PLAYING_INFO_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *              /*!<the audio object ptr.*/
    );

/*! \typedef EXT_OPE_CALLBACK_PFUNC
 *  \brief Audio execute extend operation callback function prototype.
 *  \return  return operation result. \sa AUDIO_RESULT_E
 */   
typedef uint32 (* EXT_OPE_CALLBACK_PFUNC)(
    uint32,             /*!<number of para.*/
    void *              /*!<content of para.*/
    );

/*! \struct AUDIO_CODEC_GEN_OPERATION_T
 *  \brief save the address of every genOperation.
 */
typedef struct AUDIO_Codec_Gen_Operation_struct
{
    CONSTRUCT_CALLBACK_PFUNC pConstruct;         /*!<construct.*/
    DECONSTRUCT_CALLBACK_PFUNC pDeconstruct;       /*!<deconstruct.*/
    PLAY_CONSTRUCT_CALLBACK_PFUNC pPlayConstruct;     /*!<play construct.*/
    STOP_DECONSTRUCT_CALLBACK_PFUNC pStopDeconstruct;   /*!<stop deconstruct.*/
    RESUME_CONSTRUCT_CALLBACK_PFUNC pResumeConstruct;   /*!<resume construct.*/
    PAUSE_DECONSTRUCT_CALLBACK_PFUNC pPauseDeconstruct;  /*!<pause deconstruct.*/
    PLAY_CALLBACK_PFUNC pPlay;              /*!<play.*/
    STOP_CALLBACK_PFUNC pStop;              /*!<stop.*/
    PAUSE_CALLBACK_PFUNC pPause;             /*!<pause.*/
    RESUME_CALLBACK_PFUNC pResume;            /*!<resume.*/
    SEEK_CALLBACK_PFUNC pSeek;              /*!<seek.*/
    SEND_STREAM_DATA_CALLBACK_PFUNC pSendStreamData;    /*!<send Stream Data.*/
    SET_SPEED_CALLBACK_PFUNC pSetSpeed;          /*!<set speed.*/
    SET_DIGITAL_GAIN_CALLBACK_PFUNC pSetDigitalGain;    /*!<set digital gain.*/
    GET_DIGITAL_GAIN_CALLBACK_PFUNC pGetDigitalGain;    /*!<get digital gain.*/
    IDENTIFY_FORMAT_CALLBACK_PFUNC pIdentifyAudioFormat; /*!<verify audio format.*/
    GET_PLAYING_INFO_CALLBACK_PFUNC pGetPlayingInfo;    /*!<get play info.*/
    GET_OUTPUT_SAMPLERATE_CALLBACK_PFUNC pGetOutputSampleRate;     /*!<get samplerate.*/
    SET_OUTPUT_SAMPLERATE_CALLBACK_PFUNC pSetOutputSampleRate;     /*!<set samplerate.*/
    SEEK_CALLBACK_PFUNC pTimeSeek;              /*!<seek the specified time.*/
}AUDIO_CODEC_GEN_OPERATION_T;

/*! \struct AUDIO_CODEC_ONE_EXT_OPERATION_T
 *  \brief store the content of extend operation
 */
typedef struct AUDIO_Codec_One_Ext_Operation_struct
{
	EXT_OPE_CALLBACK_PFUNC uiExtOpePointer;     /*!<store address of every extend operation.*/ 
	uint16 *pusExtOpeSymbol;                    /*!<symbol of extend operation.*/ 
	EXT_OPE_CM_OPE_E  eExtOpeCmOpe;             /*!<if enter collision handle.*/  
	struct AUDIO_Codec_One_Ext_Operation_struct *ptNextExtOperation;
	                                            /*!<point to next extend operation struct .*/
}AUDIO_CODEC_ONE_EXT_OPERATION_T;

/*! \struct AUDIO_CODEC_EXT_OPERATION_T
 *  \brief store the content of extend operation list
 */
typedef struct AUDIO_Codec_Ext_Operation_struct
{
	struct AUDIO_Codec_One_Ext_Operation_struct  *ptExtFirstOpe;
	                        /*!<store address of every extend operation info.*/
	uint16 *pusExtOpeDescribe;  /*!<describe info of all extend operations.*/
}AUDIO_CODEC_EXT_OPERATION_T;

/*! \struct AUDIO_CODEC_EXPRESS_T
 *  \brief store all express handles included in certain codec
 */
typedef struct AUDIO_CODEC_EXPRESS_struct
{
    HAUDIOEXP hExpHandle;                        /*!<audio express handle.*/ 
    struct    AUDIO_CODEC_EXPRESS_struct *ptNext;/*!<point to next express handle.*/ 
}AUDIO_CODEC_EXPRESS_T;

/*! \struct AUDIO_CODEC_T
 *  \brief store a codec info.
 */
typedef struct AUDIO_Codec_struct
{    
    uint16 *pusCodecName;     /*!<codec name.*/ 
    uint16 *pusCodecDescribe; /*!<describe info of codec.*/   
    uint32 uiTrackOnPoint;    /*!<the length of the data after decoding, when the length of data in buffer is bigger than this open vb*/
    HCOLLISION hCollisionHandle; /*!<collision handle.*/ 
    BOOLEAN bVerifyAudioFormat; /*!<if the handle can verify format.*/ 
    struct AUDIO_Codec_Ext_CfgInfo_struct tExtCfgInfo;  /*!<external config info.*/ 
    struct AUDIO_Codec_Gen_Operation_struct tGenOperation;  /*!<general operations.*/ 
    struct AUDIO_Codec_Ext_Operation_struct tExtOperation;  /*!<extend operations.*/        
}AUDIO_CODEC_T; 

/*! \struct AUDIO_CODEC_T
 *  \brief store one codec node info.
 */
typedef struct AUDIO_CodecHandle_struct
{
    HAUDIOCODEC  hCodecHandle;        /*!<codec handle.*/ 
    struct AUDIO_Codec_struct tCodec; /*!<codec info.*/        
    struct AUDIO_CODEC_EXPRESS_struct  *tExp;     /*!<express pointer.*/ 
    struct AUDIO_CodecHandle_struct *ptPreHandle; /*!<previous codec node.*/ 
    struct AUDIO_CodecHandle_struct *ptNextHandle;/*!<next codec node.*/ 
}AUDIO_CODECHANDLE_T;

/*! \struct AUDIO_CODECHANDLE_HEAD_T, *HAUDIOCODECHEAD
 *  \brief store the header pointed to the codec list.
 */
typedef struct AUDIO_CodecHandle_Head_struct
{
    struct AUDIO_CodecHandle_struct *ptFirstHandle;  /*!<point to the first codec node.*/  
}AUDIO_CODECHANDLE_HEAD_T, *HAUDIOCODECHEAD;

/**---------------------------------------------------------------------------**
 **                         Global Variables                                  **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
 **                         Constant Variables                                **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
 **                         Function Prototypes                               **
 **---------------------------------------------------------------------------**/
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_PM_InitCodecPluggerManage( 
//!    void
//!    ) 
//! \param void no para
//! \return return operation result
//! \brief  Description:  This function is to init Codec Plugger Manage.
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_PM_InitCodecPluggerManage(
    void
    );
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_PM_ReleaseCodecPluggerManage( 
//!    void
//!    ) 
//! \param void no para
//! \return return operation result
//! \brief  Description:  This function is to release Codec Plugger Manage.
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_PM_ReleaseCodecPluggerManage(
    void
    );
    
/*****************************************************************************/
//! \fn PUBLIC HAUDIOCODEC AUDIO_PM_RegCodecPlugger(
//!    AUDIO_CODEC_T *ptAudioCodec 
//!    )
//! \param ptAudioCodec codec info will be registered
//! \return if right, return codec handle
//!         otherwise, return invalid handle
//! \brief  Description:  This function is to reg a codec plugger 
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC HAUDIOCODEC AUDIO_PM_RegCodecPlugger(
    AUDIO_CODEC_T *ptAudioCodec 
    );
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_PM_UnRegCodecPlugger(
//!    HAUDIOCODEC hCodecHandle
//!    )
//! \param hCodecHandle codec handle will be unregistered
//! \return return operation result \sa AUDIO_RESULT_E
//! \brief  Description:  This function is to unreg a codec plugger 
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_PM_UnRegCodecPlugger(
    HAUDIOCODEC hCodecHandle
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_PM_AddCodecExpress(
//!    HAUDIOCODEC hCodecHandle, 
//!    HAUDIOEXP hExpHandle
//!    )
//! \param hCodecHandle codec handle
//! \param hExpHandle     express handle
//! \return if right, return AUDIO_NO_ERROR
//!            otherwise, return error reason
//!           \sa AUDIO_RESULT_E  
//! \brief  Description:  This function is to add  one express  to codec
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_PM_AddCodecExpress(
    HAUDIOCODEC hCodecHandle, 
    HAUDIOEXP hExpHandle
    );				                

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_PM_DeleteCodecExpress(
//!    HAUDIOCODEC hCodecHandle, 
//!    HAUDIOEXP hExpHandle
//!    )
//! \param hCodecHandle codec handle
//! \param hExpHandle     express handle
//! \return if right, return AUDIO_NO_ERROR
//!            otherwise, return error reason 
//! \brief  Description:  This function is to delete one express  from codec
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_PM_DeleteCodecExpress(
    HAUDIOCODEC hCodecHandle, 
    HAUDIOEXP hExpHandle
    );
        
/*****************************************************************************/
//! \fn PUBLIC HAUDIOCODEC  AUDIO_PM_GetFirstCodecHandle(
//!	void
//!	)
//! \param void no para
//! \return if exist return codec handle  
//!           otherwise return INVALID_HANDLE
//!           \sa HAUDIOCODEC
//! \brief  Description:  This function is to get first codecHandle
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC HAUDIOCODEC  AUDIO_PM_GetFirstCodecHandle(
	void
	);
	
/*****************************************************************************/
//! \fn PUBLIC HAUDIOCODEC  AUDIO_PM_GetNextCodecHandle(
//! 		HAUDIOCODEC hCodecHandle
//!		)
//! \param hCodecHandle codec handle
//! \return if exist return codec handle  
//!           otherwise  return INVALID_HANDLE
//!           \sa HAUDIOCODEC
//! \brief  Description:  This function is to get next codecHandle
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC HAUDIOCODEC  AUDIO_PM_GetNextCodecHandle(
	HAUDIOCODEC hCodecHandle
	);
	
/*****************************************************************************/
//! \fn PUBLIC AUDIO_CODEC_GEN_OPERATION_T * AUDIO_PM_GetCodecGenOpe(
//!	HAUDIOCODEC hCodecHandle
//!	)
//! \param hCodecHandle codec handle
//! \return if exist return the address of general operations of the Codec 
//!           otherwise  return SCI_NULL
//!           \sa AUDIO_CODEC_GEN_OPERATION_T *
//! \brief  Description:  This function is to  get all General Operations of the Codec
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_CODEC_GEN_OPERATION_T * AUDIO_PM_GetCodecGenOpe(
	HAUDIOCODEC hCodecHandle
	);
	
/*****************************************************************************/
//! \fn PUBLIC HCOLLISION AUDIO_PM_GetCollision(
//!	HAUDIOCODEC hCodecHandle
//!	)
//! \param hCodecHandle codec handle
//! \return if exist return the collision handle 
//!           otherwise  return INVALID_HANDLE
//!           \sa HCOLLISION
//! \brief  Description:  This function is to get collision
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC HCOLLISION AUDIO_PM_GetCollision(
	HAUDIOCODEC hCodecHandle
	);	

/*****************************************************************************/
//! \fn PUBLIC HAUDIOEXP AUDIO_PM_GetFirstCodecExpressHandle(
//!    HAUDIOCODEC hCodecHandle
//!    )      
//! \param hCodecHandle codec handle
//! \return if exist return express handle 
//!           otherwise return INVALID_HANDLE
//!           \sa  HAUDIOEXP
//! \brief  Description:  This function is to get next codec express handle
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC HAUDIOEXP AUDIO_PM_GetFirstCodecExpressHandle(
    HAUDIOCODEC hCodecHandle
    );
    
/*****************************************************************************/
//! \fn PUBLIC HAUDIOEXP AUDIO_PM_GetNextCodecExpressHandle(
//!	HAUDIOCODEC hCodecHandle, 
//!	HAUDIOEXP hExpHandle
//!  )   
//! \param hCodecHandle codec handle
//! \param hExpHandle    express handle
//! \return if exist return express handle 
//!           otherwise return SCI_NULL
//!           \sa  HAUDIOEXP
//! \brief  Description:  This function is to get next codec express handle
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC HAUDIOEXP AUDIO_PM_GetNextCodecExpressHandle(
	HAUDIOCODEC hCodecHandle, 
	HAUDIOEXP hExpHandle
	);	
	
/*****************************************************************************/
//! \fn PUBLIC const uint16 * AUDIO_PM_GetCodecName(
//!	HAUDIOCODEC hCodecHandle
//!	)
//! \param hCodecHandle codec handle
//! \return return codec name  \sa const uint16 * 
//! \brief  Description:  This function is to  get codec name from codec handle
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing
/*****************************************************************************/
PUBLIC const uint16 * AUDIO_PM_GetCodecName(
	HAUDIOCODEC hCodecHandle
	);
	
/*****************************************************************************/
//! \fn PUBLIC const HAUDIOCODEC AUDIO_PM_GetCodecHandle(
//!	const uint16 *pusCodecName
//!	)
//! \param pusCodecName codec name
//! \return return codec handle info  \sa HAUDIOCODEC
//! \brief  Description:  This function is to get codec handle from codec name 
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing
/*****************************************************************************/
PUBLIC HAUDIOCODEC AUDIO_PM_GetCodecHandle(
	const uint16 *pusCodecName
	);
	
/*****************************************************************************/
//! \fn PUBLIC const uint16 * AUDIO_PM_GetCodecDes(
//!	HAUDIOCODEC hCodecHandle, 
//!	uint16 *pusCodecName, 
//!	DES_FLAG_E eDesFlag
//!	)	
//! \param hCodecHandle codec handle
//! \param pusCodecName codec name
//! \param eDesFlag  determine if codec description or codec 
//!                            extend operation description
//! \return return description info  \sa const uint16 * 
//! \brief  Description:  This function is to get codec description 
//! \author Author:  Shujing.Dong 
//! \note   Note:can get description info  through codec handle or 
//!                  codec name, but the priority of codec handle is higher 
//!                  than that of codec name
/*****************************************************************************/
PUBLIC const uint16 * AUDIO_PM_GetCodecDes(
	HAUDIOCODEC hCodecHandle, 
	uint16 *pusCodecName, 
	DES_FLAG_E eDesFlag
	);
	
/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIO_PM_GetTrackOnPoint( 
//!	HAUDIOCODEC hCodecHandle
//!	)	
//! \param hCodecHandle codec handle
//! \return return trace on point  \sa uint32
//! \brief  Description:  This function is to get trace on point.
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC uint32 AUDIO_PM_GetTrackOnPoint( 
	HAUDIOCODEC hCodecHandle      
	);

/*****************************************************************************/
//! \fn PUBLIC AUDIO_CODEC_EXT_CFGINFO_T * AUDIO_PM_GetCodecExtCfgInfo( 
//!	HAUDIOCODEC hCodecHandle
//!	)	
//! \param hCodecHandle codec handle
//! \return if exist return the address of external config info of the Codec 
//!           otherwise  return SCI_NULL
//!           \sa AUDIO_CODEC_EXT_CFGINFO_T *
//! \brief  Description:  This function is to get external config info of the codec.
//! \author Author:  Binggo.Zhou 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_CODEC_EXT_CFGINFO_T * AUDIO_PM_GetCodecExtCfgInfo( 
	HAUDIOCODEC hCodecHandle      
	);

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_PM_ExeCodecExtOpe(
//!	HAUDIO hAudioHandle, 
//!	uint16 *pusExtOpeSymbol, 
//!	uint32 uiArgc, 
//!	void *pvArgv,
//! void *result
//!	)	
//! \param hAudioHandle audio handle
//! \param pusExtOpeSymbol symbol of extend operation 
//! \param uiArgc number of extend operation
//! \param pvArgv para of extend operation
//! \param pvResult the real return value of execute an extend operation.
//! \return return operation result \sa AUDIO_RESULT_E
//! \brief  Description:  This function is to exe Codec Extend Operation
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_PM_ExeCodecExtOpe(
	HAUDIO hAudioHandle, 
	uint16 *pusExtOpeSymbol, 
	uint32 uiArgc, 
	void  *pvArgv,
	uint32 *pvResult
	);

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_PM_AddOneExtOpe(
//!	HAUDIOCODEC hCodecHandle,  
//!	AUDIO_CODEC_ONE_EXT_OPERATION_T *sur_one_ext_operation_ptr
//!	)	
//! \param hCodecHandle codec handle 
//! \param sur_one_ext_operation_ptr extOpe info 
//! \return return operation result \sa AUDIO_RESULT_E
//! \brief  Description:  This function is to add one extend operation info to 
//!                              the codec node 
//! \author Author:  Shujing.Dong 
//! \note   Note: input para sur_one_ext_operation_ptr->ptNextExtOperation can 
//!                    not be set.
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_PM_AddOneExtOpe(
    HAUDIOCODEC hCodecHandle,  
    AUDIO_CODEC_ONE_EXT_OPERATION_T *sur_one_ext_operation_ptr
    );	
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_PM_IsCodecIncludeExp(
//!	HAUDIOCODEC hCodecHandle,  
//!	HAUDIOEXP hExpHandle
//!	)	
//! \param hCodecHandle codec handle 
//! \param hExpHandle express handle
//! \return return operation result \sa BOOLEAN
//! \brief  Description:  This function is to judge if an express is included by a codec 
//! \author Author:  Shujing.Dong 
//! \note   Note: 
/*****************************************************************************/
PUBLIC BOOLEAN AUDIO_PM_IsCodecIncludeExp(
    HAUDIOCODEC hCodecHandle,  
    HAUDIOEXP hExpHandle
    );   

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_PM_CanVerifyFormat(
//!	HAUDIOCODEC hCodecHandle
//!	)	
//! \param hCodecHandle codec handle 
//! \return return operation result \sa BOOLEAN
//! \brief  Description:  This function is to judge if the codec can verify format.
//! \author Author:  Shujing.Dong 
//! \note   Note: 
/*****************************************************************************/
PUBLIC BOOLEAN AUDIO_PM_CanVerifyFormat(
    HAUDIOCODEC hCodecHandle
    );
/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/ 
#ifdef __cplusplus
}
#endif

#endif  // _APM_CODEC_H

// End of apm_codec.h
