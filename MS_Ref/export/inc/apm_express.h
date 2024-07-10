/******************************************************************************
 ** File Name:      apm_express.h                                              
 ** Author:         shujing.dong                                              
 ** DATE:           11/15/2005                                                
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         
 ** Description:    This file defines the basic operation interfaces  
 **                  and data structure of apm_express                     
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       
 **  -----------------------------------------------------------------------  
 ** DATE           NAME             DESCRIPTION                               
 ** 11/15/2005     shujing.dong     Create.                                   
 ******************************************************************************/
  /*! \file apm_express.h
  *  \author Shujing.Dong 
  *  \date Nov 15, 2005
  *  \brief This file defines the basic operation interfaces and data structure 
  *          of audio plugger - express - manage. 
  */ 
#ifndef _APM_EXPRESS_H
#define _APM_EXPRESS_H
/**---------------------------------------------------------------------------**
 **                         Dependencies                                      **
 **---------------------------------------------------------------------------**/
#include "apm_gen.h"
#include "asm.h"
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
/*! \typedef EXP_CALLBACK_PFUNC
 *  \brief Audio express callback function prototype.
 *  \return void no return. 
 */  
typedef AUDIO_RESULT_E (*EXP_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T *, /*!<audio object info.*/
    int16 *,   /*!<input left channel data.*/
    int16 *,   /*!<input right channel data.*/
    uint32,    /*!<length of input data.*/
    int16 *,   /*!<output left channel data.*/
    int16 *,   /*!<output right channel data.*/
    uint32 *   /*!<length of output data.*/
    );
    
/*! \typedef EXP_SET_PARA_CALLBACK_PFUNC
 *  \brief Audio express set parameter callback function prototype.
 *  \return void no return. 
 */  
typedef AUDIO_RESULT_E (*EXP_SET_PARA_CALLBACK_PFUNC)(
    uint32,   /*!<number of parameters.*/
    void *    /*!<content of parameters.*/
    );    

/*! \typedef EXP_GET_PARA_CALLBACK_PFUNC
 *  \brief Audio express set parameter callback function prototype.
 *  \return void no return. 
 */  
typedef AUDIO_RESULT_E (*EXP_GET_PARA_CALLBACK_PFUNC)(
    uint32,   /*!<number of parameters.*/
    void *    /*!<content of parameters.*/
    ); 

/*! \typedef EXP_SET_PARA_CALLBACK_PFUNC
 *  \brief Audio express set parameter callback function prototype.
 *  \return void no return. 
 */  
typedef AUDIO_RESULT_E (*EXP_INIT_PARA_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T * /*!<audio object info.*/
    );    

/*! \typedef EXP_DEINIT_PARA_CALLBACK_PFUNC
 *  \brief Audio express deinit callback function prototype.
 *  \return void no return. 
 */  
typedef AUDIO_RESULT_E (*EXP_DEINIT_PARA_CALLBACK_PFUNC)(
    AUDIO_OBJECT_T * /*!<audio object info.*/
    );    


/*! \struct AUDIO_EXPRESS_T
 *  \brief express info.
 */
typedef struct AUDIO_Express_struct
{     
	uint16 *pusExpName;  /*!<express name.*/
	uint16 *pusExpDescribe;/*!<express description.*/
	SCI_MUTEX_PTR  mutex;/*!<mutex of process/set_para/init_para/deinit_para.*/
	BOOLEAN resX;
	BOOLEAN res0;
	uint16 res1;
	EXP_CALLBACK_PFUNC process;	/*!<process of express.*/
	EXP_SET_PARA_CALLBACK_PFUNC set_para; /*!<set parameters*/
	EXP_GET_PARA_CALLBACK_PFUNC get_para; /*!<set parameters*/
    EXP_INIT_PARA_CALLBACK_PFUNC init_para; /*!<init parameters*/
    EXP_DEINIT_PARA_CALLBACK_PFUNC deinit_para; /*!<deinit parameters*/
}AUDIO_EXPRESS_T;

/*! \struct AUDIO_EXPHANDLE_T
 *  \brief one express node info.
 */
typedef struct AUDIO_ExpressHandle_struct
{
	HAUDIOEXP hExpHandle;/*!<express handle.*/
	struct AUDIO_Express_struct tExp;/*!<express info.*/	
	struct AUDIO_ExpressHandle_struct *ptPreHandle;/*!<previous express node.*/
	struct AUDIO_ExpressHandle_struct *ptNextHandle;/*!<next express node.*/
}AUDIO_EXPHANDLE_T;

/*! \struct AUDIO_EXPHANDLE_HEAD_T, *HAUDIOEXPHEAD
 *  \brief header of express list.
 */
typedef struct AUDIO_ExpressHandle_Head_struct
{
	struct AUDIO_ExpressHandle_struct *ptFirstHandle;/*!<point to the first express node.*/ 
}AUDIO_EXPHANDLE_HEAD_T, *HAUDIOEXPHEAD;


/**---------------------------------------------------------------------------**
 **                         Global Variables                                  **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
 **                         Constant Variables                                **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
 **                         Function Prototypes                               **
 **---------------------------------------------------------------------------**/
/**---------------------------------------------------------------------------**
 **                         Function Define                                   **
 **---------------------------------------------------------------------------**/
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_PM_InitExpressPluggerManage(
//!    void
//!    ) 
//! \param void no para
//! \return return operation result
//! \brief  Description:  This function is to init Express Plugger Manage.
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_PM_InitExpressPluggerManage(
    void
    );
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_PM_ReleaseExpressPluggerManage( 
//!    void
//!    ) 
//! \param void no para
//! \return return operation result
//! \brief  Description:  This function is to release Express Plugger Manage.
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_PM_ReleaseExpressPluggerManage(
    void
    );
    
/*****************************************************************************/
//! \fn PUBLIC HAUDIOEXP AUDIO_PM_RegExpressPlugger(
//!    AUDIO_EXPRESS_T *ptAudioExp
//!    )
//! \param ptAudioExp express info will be registered
//! \return return express handle
//! \brief  Description:  This function is to reg a express plugger 
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC HAUDIOEXP AUDIO_PM_RegExpressPlugger(
    AUDIO_EXPRESS_T *ptAudioExp
    );
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_PM_UnRegExpressPlugger(
//!    HAUDIOEXP hExpHandle
//!    )
//! \param hExpHandle express handle will be unregistered
//! \return return operation result \sa AUDIO_RESULT_E
//! \brief  Description:  This function is to unreg a express plugger 
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_PM_UnRegExpressPlugger(
    HAUDIOEXP hExpHandle
    );
    
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_PM_ExeExpOpe(
//!    AUDIO_OBJECT_T *ptAudioObject,
//!    HAUDIOEXP hExpHandle, 
//!    int16* psSrcLeftData,
//!    int16* psSrcRightData,
//!    uint32 uiSrcCount, 
//!    int16* psDestLeftData, 
//!    int16* psDestRightData, 
//!    uint32* puiDestCount
//!	)	
//! \param ptAudioObject audio object
//! \param hExpHandle express handle
//! \param psSrcLeftData input left channel data.
//! \param psSrcRightData input right channel data.
//! \param psSrcRightData length of input data.
//! \param psDestLeftData output left channel data.
//! \param psDestRightData output right channel data.
//! \param puiDestCount length of output data.
//! \return return operation result \sa AUDIO_RESULT_E
//! \brief  Description:  This function is to exe express operation
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_PM_ExeExpOpe(
    AUDIO_OBJECT_T *ptAudioObject,
    HAUDIOEXP hExpHandle,  
    int16* psSrcLeftData,
    int16* psSrcRightData,
    uint32 uiSrcCount, 
    int16* psDestLeftData, 
    int16* psDestRightData, 
    uint32* puiDestCount
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_PM_ExeExpSetParaOpe(
//!    HAUDIOEXP hExpHandle,
//!    uint16 *pusExpName,
//!    uint32 argc,
//!    void *argv
//!	)	
//! \param hExpHandle express handle
//! \param pusExpName express name
//! \param argc number of parameters.
//! \param argv content of parameters.
//! \return return operation result \sa AUDIO_RESULT_E
//! \brief  Description:  This function is to exe express set parameter operation
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_PM_ExeExpSetParaOpe(
	HAUDIOEXP hExpHandle, 
	uint16 *pusExpName,  
    uint32 argc,   
    void *argv    
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_PM_ExeExpGetParaOpe(
//!    HAUDIOEXP hExpHandle,
//!    uint16 *pusExpName,
//!    uint32 argc,
//!    void *argv
//!	)	
//! \param hExpHandle express handle
//! \param pusExpName express name
//! \param argc number of parameters.
//! \param argv content of parameters.
//! \return return operation result \sa AUDIO_RESULT_E
//! \brief  Description:  This function is to exe express set parameter operation
//! \author Author:  cherry.liu
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_PM_ExeExpGetParaOpe(
    HAUDIOEXP hExpHandle, 
    uint16 *pusExpName,  
    uint32 argc,   
    void *argv    
    );


/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_PM_ExeExpInitParaOpe(
//!    HAUDIOEXP hExpHandle,
//!    uint16 *pusExpName,
//!    AUDIO_OBJECT_T *ptAudioObject  
//!	)	
//! \param hExpHandle express handle
//! \param pusExpName express name
//! \param ptAudioObject audio object
//! \return return operation result \sa AUDIO_RESULT_E
//! \brief  Description:  This function is to exe express init parameter operation
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_PM_ExeExpInitParaOpe(
    HAUDIOEXP hExpHandle, 
    uint16 *pusExpName,  
    AUDIO_OBJECT_T *ptAudioObject  
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDIO_PM_ExeExpDeInitParaOpe(
//!    HAUDIOEXP hExpHandle,
//!    uint16 *pusExpName,
//!    AUDIO_OBJECT_T *ptAudioObject  
//!	)	
//! \param hExpHandle express handle
//! \param pusExpName express name
//! \param ptAudioObject audio object
//! \return return operation result \sa AUDIO_RESULT_E
//! \brief  Description:  This function is to exe express de-init parameter operation
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDIO_RESULT_E AUDIO_PM_ExeExpDeInitParaOpe(
    HAUDIOEXP hExpHandle, 
    uint16 *pusExpName,  
    AUDIO_OBJECT_T *ptAudioObject  
    );

/*****************************************************************************/
//! \fn PUBLIC HAUDIOEXP AUDIO_PM_GetFirstExpHandle(
//!    void
//!    )   
//! \param void no void
//! \return if exist, return the handle, otherwise return INVALID_HANDLE \sa  HAUDIOEXP
//! \brief  Description:  This function is to get first express handle
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC HAUDIOEXP AUDIO_PM_GetFirstExpHandle(
    void
    );
    
/*****************************************************************************/
//! \fn PUBLIC HAUDIOEXP AUDIO_PM_GetNextExpHandle(
//!    HAUDIOEXP hExpHandle
//!    )   
//! \param hExpHandle express handle
//! \return if exist, return the handle, otherwise return INVALID_HANDLE \sa  HAUDIOEXP
//! \brief  Description:  This function is to get next express handle
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC HAUDIOEXP  AUDIO_PM_GetNextExpHandle(
    HAUDIOEXP hExpHandle
    );
    
/*****************************************************************************/
//! \fn PUBLIC const uint16 * AUDIO_PM_GetExpName(
//!	HAUDIOEXP hExpHandle
//!	)
//! \param hExpHandle express handle
//! \return return express name  \sa const uint16 * 
//! \brief  Description:  This function is to  get express name from express handle
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing
/*****************************************************************************/
PUBLIC const uint16 * AUDIO_PM_GetExpName(
	HAUDIOEXP hExpHandle
	);

/*****************************************************************************/
//! \fn PUBLIC const HAUDIOEXP AUDIO_PM_GetExpHandle(
//!	const uint16 *pusExpName
//!	)
//! \param pusExpName express name
//! \return SUCCESS: return express handle  
//!         FAIL: return INVALID_HANDLE
//!         \sa HAUDIOEXP
//! \brief  Description:  This function is to get express handle from express name 
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing
/*****************************************************************************/
PUBLIC HAUDIOEXP AUDIO_PM_GetExpHandle(
	const uint16 *pusExpName
	);
		
/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/ 
#ifdef __cplusplus
}
#endif

#endif  // _APM_EXPRESS_H

// End of apm_express.h
