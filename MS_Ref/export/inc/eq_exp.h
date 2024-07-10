/******************************************************************************
** File Name:      eq_exp.h                                            
** Author:         shujing.dong                                              
** DATE:           07/26/2006                                                
** Copyright:      2006 Spreatrum, Incoporated. All Rights Reserved.         
** Description:    This file defines the basic operation interfaces 
**                 of eq express plugger.                   
******************************************************************************

******************************************************************************
**                        Edit History                                       
**  -----------------------------------------------------------------------  
** DATE           NAME             DESCRIPTION                               
** 7/26/2006      shujing.dong     Create.                                   
******************************************************************************/
 /*! \file eq_exp.h 
  *  \author Shujing.Dong 
  *  \date July,26 2006
  *  \brief This file defines the basic operation interfaces of eq express plugger. 
  */ 
  
#ifndef _EQ_EXP_H_
#define _EQ_EXP_H_
/**---------------------------------------------------------------------------**
**                         Dependencies                                      **
**---------------------------------------------------------------------------**/
#include "apm_codec.h"
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
/*! \def EQ_EXP_PARA_COUNT
 *  define the count of eq express para
 */
 #define EQ_EXP_PARA_COUNT 2	
 





/*! \enum EQ_EXP_MODE_E
 *  \brief eq mode type. define the value of invalid mode used in only set samplerate para.
 */
typedef enum
{    
    EQ_EXP_MODE_BASS    = 0,
    EQ_EXP_MODE_TREBLE      ,   // 1
    EQ_EXP_MODE_VOCAL       ,   // 2
    EQ_EXP_MODE_POWER       ,   // 3
    EQ_EXP_MODE_POP         ,   // 4
    EQ_EXP_MODE_ROCK        ,   // 5
    EQ_EXP_MODE_CLASSIC     ,   // 6
    EQ_EXP_MODE_SPEAKER_LCF ,   // 7
    EQ_EXP_MODE_PASS        ,   // 8
    EQ_EXP_MODE_MAX         ,   // 9
    EQ_EXP_MODE_INVALID = 0xFF 
}EQ_EXP_MODE_E;	

/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/
/*! \struct EQ_EXP_PARA_CONTENT_T
 *  \brief define the structure of eq express.
 *  \brief uiSamplerate samplerate of source data
 *  \brief uiModeNo no. of eq express mode (0--7)
 */
typedef struct
{
    uint32 uiSamplerate; 
    EQ_EXP_MODE_E uiModeNo;
}EQ_EXP_PARA_T;
/**---------------------------------------------------------------------------**
 **                         Global Variables                                  **
 **---------------------------------------------------------------------------**/
	
	
/**---------------------------------------------------------------------------**
 **                         Constant Variables                                **
 **---------------------------------------------------------------------------**/


/**---------------------------------------------------------------------------**
 **                          Function Declare                                 **
 **---------------------------------------------------------------------------**/
/*****************************************************************************/
//! \fn PUBLIC HAUDIOEXP EQ_EXP_RegExpPlugger(
//!    void
//!    ) 
//! \param void no para
//! \return return express handle 
//! \brief  Description:  This function is to register eq express plugger.
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC HAUDIOEXP EQ_EXP_RegExpPlugger( 
    void
    );
/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/ 
#ifdef __cplusplus
}
#endif 

#endif //end of _TEST_H

//end of file







