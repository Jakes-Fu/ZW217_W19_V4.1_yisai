/*******************************************************************************
 ** FileName:      temp_8800.c                                             *
 ** Author:        jibin                                       *
 ** Date:          2006/7/24                                                   *
 ** CopyRight:     2006, Spreadtrum, Incoporated, All right reserved            *
 ** Description:   define mmidc extern interface                                       *
 *******************************************************************************
 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME                   DESCRIPTION                          *
 ** 2006/7/24      jibin      Created                          *
 *******************************************************************************/

#define TEMP_8800_C

#ifdef __cplusplus
	extern "C"
	{
#endif

#include "std_header.h"
#include "sci_types.h"
#include "os_api.h"
//#include "ffs.h"
#include "mmk_type.h"
#include "mmifmm_export.h"
#include "mn_type.h"
#include "scm_drv.h"

                      
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/




void COMMON_GetMMIOutProductVersionInfo(void)
{
}

//void SD_GetStatus(void)
//{
//}

void KPD_SetGreenKeyPressed(void)
{
}

void KPD_GetGreenKeyPressed(void)
{
}


PUBLIC void MPLAYER_Close (void){}

PUBLIC void MPLAYER_SetDisplayParam (void){}

PUBLIC void MPLAYER_Play (void){}

PUBLIC void MPLAYER_Resume (void){}

PUBLIC void MPLAYER_Pause (void){}

PUBLIC void MPLAYER_Stop (void){}

PUBLIC void MPLAYER_Open (void){}

PUBLIC void MPLAYER_SetFrameEndNotify (void){}

PUBLIC void MPLAYER_SetEndNotify (void){}

PUBLIC void MPLAYER_GetMovieInfo (void){}

#ifdef __cplusplus
    }
#endif