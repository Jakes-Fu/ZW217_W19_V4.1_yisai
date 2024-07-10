/******************************************************************************
** File Name:      ratecontrol_exp.h                                           
** Author:         shujing.dong                                              
** DATE:           09/17/2009                                               
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.         
** Description:    This file defines the basic operation interfaces 
**                 of rate control express plugger.                   
******************************************************************************

******************************************************************************
**                        Edit History                                       
**  -----------------------------------------------------------------------  
** DATE           NAME             DESCRIPTION                               
** 09/17/2009     shujing.dong     Create.                                   
******************************************************************************/
/*! \file ratecontrol_exp.h 
 *  \author Shujing.Dong 
 *  \date Sep,17 2009
 *  \brief This file defines the basic operation interfaces of rate control express plugger. 
 */ 
  
#ifndef _RATECONTROL_EXP_H_
#define _RATECONTROL_EXP_H_
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
/*! \enum RATECONTROL_EXP_RATE_E
 *  \brief rate control
 */
typedef enum
{
    RATECONTROL_EXP_RATE_NORMAL,    //normal rate
    RATECONTROL_EXP_RATE_SLOWER,         //slower rate
    RATECONTROL_EXP_RATE_SLOWEST,  //slowest rate
    RATECONTROL_EXP_RATE_FASTER,      //faster rate
    RATECONTROL_EXP_RATE_FASTEST, //fastest rate
    RATECONTROL_EXP_RATE_MAX
}RATECONTROL_EXP_RATE_E;		


/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/
/*! \struct RATECONTROL_EXP_PARA_T
 *  \brief define the structure of ratecontrol express.
 */
typedef struct
{
    RATECONTROL_EXP_RATE_E eRateControl;
}RATECONTROL_EXP_PARA_T;
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
//! \fn PUBLIC HAUDIOEXP RATECONTROL_EXP_RegExpPlugger(
//!    void
//!    ) 
//! \param void no para
//! \return return express handle 
//! \brief  Description:  This function is to register RATECONTROL express plugger.
//! \author Author:  Shujing.Dong 
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC HAUDIOEXP RATECONTROL_EXP_RegExpPlugger( 
    void
    );


/**---------------------------------------------------------------------------**
 **                         NOTE                                              **
 **---------------------------------------------------------------------------**/
 
//    NOTE   1
// SET PARA :
// argc: it's optional;
// argv: it's address of one variable, it's structure is RATECONTROL_EXP_PARA_T;

//    NOTE   2
//the name of ratecontrol exp:      ANSI2UINT16("RATECONTROL_EXP")



/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/ 
#ifdef __cplusplus
}
#endif 

#endif //end of _TEST_H

//end of file







