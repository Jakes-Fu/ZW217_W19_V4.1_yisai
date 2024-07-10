/***************************************************************************
** File Name:      atest_r8.h                                            *
** Author:                                                                 *
** Date:           5/09/2010                                                   *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to define the R8 .           *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                             *
** 5/09/2010      shuyan.hu         Modularize
****************************************************************************/


/**------------------------------------------------------------------------*
**                         Include Files                                   *
**-------------------------------------------------------------------------*/
#ifndef _ATEST_R8_H
#define _ATEST_R8_H

#include "sci_types.h"
#include "mmisms_set.h"

#include "mmismscb_wintab.h"
#include "atest_signal.h"


#ifdef WIN32
/**-----------------------------------------------------------------------------------*
 **                               Constants and  Variables                            *
 **-----------------------------------------------------------------------------------*/

#define  R8_MAX_NUM       612
#define  R8_CHARACTER_NUM         100
#define  R8_TUKISH_S    104
#define  R8_SPANISH_S    86
#define  R8_PORTUGUES_S   96
#define  R8_TURKISH_ARR   120
#define  R8_SPANISH_ARR    104
#define  R8_PORTUGUES_ARR  125
#define  R8_FILELINE    200
#define  R8_LINETEM     100
#define  MAX_R8_PTAH   260

typedef enum _LANG_MUA{
	R8_TURKISH = 1,
	R8_SPANISH,
	R8_PORTUGUES, 
	R8_MAXLAN
}LANG_MUA;



/**-----------------------------------------------------------------------*
 **                                Function Prototypes                    *
 **-----------------------------------------------------------------------*/

PUBLIC void atest_r8();
PUBLIC void Atest_R8GetConfigPath(ATEST_APP_DATA_T *r8_signal_ptr);


#endif //WIN32

#endif //_ATEST_R8_H
