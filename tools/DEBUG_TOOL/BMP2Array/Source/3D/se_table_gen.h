/******************************************************************************
 ** File Name:      se_table_gen.h                                                                              *
 ** Author:           Shan.He                                                                                            *
 ** DATE:             2009-6-9                                                                                         *
 ** Copyright:       2008 Spreatrum, Incoporated. All Rights Reserved.                                  *
 ** Description:     scaling up algorithm                                                                   *
 ** Note:                                                     *
 *****************************************************************************/

#ifndef _SE_TABLE_GEN_H_
#define _SE_TABLE_GEN_H_

#include "os_api.h"


#ifdef __cplusplus
extern "C" 
{
#endif
    
/******************************************************************************
// Description: create table of distort move special effect
// Author:		shan.he
//              uWidth:	icon width
//              uHeight: icon height
// Output:		
// Return:		none
// Note:		the table will be output to the file named "SE_DISTORT_MOVE_table"
******************************************************************************/
PUBLIC BOOLEAN CreateDistortMoveTable(int32 uWidth, int32 uHeight);

/******************************************************************************
// Description: create table of desktop rotation special effect
// Author:		shan.he
//              uWidth:	desktop width
//              uHeight: desktop height
// Output:		
// Return:		none
// Note:		the table will be output to the file named "SE_DESKTOP_ROTATE_table"
******************************************************************************/
PUBLIC BOOLEAN CreateRotDesktopTable(int32 uWidth, int32 uHeight);

/******************************************************************************
// Description: create table of slide move special effect
// Author:		shan.he
//              uWidth:	icon width
//              uHeight: icon height
// Output:		
// Return:		none
// Note:		the table will be output to the file named "SE_SLIDE_MOVE_table"
******************************************************************************/
PUBLIC BOOLEAN CreateSlideMoveTable(int32 uWidth, int32 uHeight);

/******************************************************************************
// Description: create file for micros
// Author:		shan.he
//              uIconWidth:	icon width
//              uIconHeight: icon height
//              uDesktopWidth: desktop width
//              uDesktopHeight: desktop height
// Output:		
// Return:		none
// Note:		the table will be output to the file named "SE_DISTORT_MOVE_table"
******************************************************************************/
PUBLIC BOOLEAN CreateMicros(int32 uIconWidth, int32 uIconHeight, int32 uDesktopWidth, int32 uDesktopHeight);

PUBLIC void SetTempPath(char * pPath);

#ifdef __cplusplus
}
#endif 

#endif