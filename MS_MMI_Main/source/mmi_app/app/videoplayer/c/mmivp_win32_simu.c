/*******************************************************************************
 ** FileName:      mmivp_win32_simu.c                                             *
 ** Author:        applezhang                                       *
 ** Date:          2010/5/25                                                   *
 ** CopyRight:     2010, Spreatrum, Incoporated, All right reserved            *
 ** Description:                                      *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME                   DESCRIPTION                          *
 ** 2010/5/25      apple.zhang             Created                          *
 *******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "std_header.h"
#include "sci_types.h"
#include "guilcd.h"
#include "mmi_appmsg.h"   
#include "dal_player.h"     
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/******************************************************************************/
// Description: Init DPlayer
// Author:
// Input:      void
// Output:     none
// Return:     DPLAYER_RETURN_E
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_Init(void)
{
    SCI_Sleep(1000);
    return DPLAYER_SUCCESS;
}

/******************************************************************************/
// Description: Release DPlayer
// Author:
// Input:      void                        
// Output:     none
// Return:     DPLAYER_RETURN_E
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_Release(void)
{
    SCI_Sleep(1000);
    return DPLAYER_SUCCESS;
}

/******************************************************************************/
// Description: open a movie file, prepare it to play 
// Author:
// Input:     
//                          
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_HANDLE  DPLAYER_Open(
    DPLAYER_OPEN_TYPE_E     open_type,
    int32               argc, 
    void *              argv[])
{
    SCI_Sleep(500);
    return 1;
}

/******************************************************************************/
// Description: stop the player
// Author:
// Input:      
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_Close(DPLAYER_HANDLE   player_handle)
{
    return DPLAYER_SUCCESS;
}

/******************************************************************************/
// Description: play an interval
// Author:
// Input:      
// Output:     none
// Return:     0           successful
//             others      failed
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_Play(DPLAYER_HANDLE    player_handle)
{
    return DPLAYER_SUCCESS;
}

/******************************************************************************/
// Description: stop
// Author:
// Input:      
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_Stop(DPLAYER_HANDLE    player_handle)
{    

    return DPLAYER_SUCCESS;
}

/******************************************************************************/
// Description: pause
// Author:
// Input:      
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E  DPLAYER_Pause(DPLAYER_HANDLE  player_handle)
{
    return DPLAYER_SUCCESS;
}

DPLAYER_RETURN_E  DPLAYER_PauseEx(DPLAYER_HANDLE	player_handle)
{
    return DPLAYER_SUCCESS;
}
/******************************************************************************/
// Description: pause
// Author:
// Input:      
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_Resume(DPLAYER_HANDLE  player_handle)
{
    return DPLAYER_SUCCESS;
}

DPLAYER_RETURN_E DPLAYER_ResumeEx(DPLAYER_HANDLE  player_handle)
{
    return DPLAYER_SUCCESS;
}

/******************************************************************************/
// Description: get the length (duration)
// Author:
// Input:    
// Output:     none
// Return:     <0         failed
//             others     total length of the movie
// Note:       none
/******************************************************************************/
uint32 DPLAYER_GetLength(DPLAYER_HANDLE player_handle)
{
    return 1000;
}

/******************************************************************************/
// Description: set next time to play
// Author:
// Input:      i_time     next play point, milli-second
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_SetTime(
                    DPLAYER_HANDLE  player_handle,uint32 i_time)
{
    return DPLAYER_SUCCESS;
}


/******************************************************************************/
// Description: get current playing time
// Author:
// Input:      
// Output:     none
// Return:     <0         failed
//             others     total length of the movie
// Note:       none
/******************************************************************************/
uint32  DPLAYER_GetTime(DPLAYER_HANDLE  player_handle)
{
     return 0;
}
/******************************************************************************/
// Description: set play rate
// Author:
// Input:      none
// Output:     none
// Return:     <0         failed
//             others     total length of the movie
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_SetPlayRate(DPLAYER_HANDLE     player_handle,
        DPLAYER_PLAY_RATE_E rate)
{
    return DPLAYER_SUCCESS;
}

/******************************************************************************/
// Description: update current frame
// Author:
// Input:      none
// Output:     none
// Return:     <0         failed
//             others     total length of the movie
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_UpdateCurrentFrame(DPLAYER_HANDLE  player_handle)
{
    return DPLAYER_SUCCESS;
}

/******************************************************************************/
// Description: update next frame
// Author:
// Input:      
// Output:     none
// Return:     <0         failed
//             others     total length of the movie
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_UpdateNextFrame(DPLAYER_HANDLE player_handle)
{
    return DPLAYER_SUCCESS;
}


/******************************************************************************/
// Description: get movie info
// Author:     
// Input:      p_info      point to movie information structure
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_GetMovieInfo(DPLAYER_HANDLE    player_handle,
    DPLAYER_MEDIA_INFO_T *p_info)
{
    return DPLAYER_SUCCESS;
}

/******************************************************************************/
// Description: register display notify function
// Author:     
// Input:      pf_notify   point to notify function
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/

DPLAYER_RETURN_E DPLAYER_SetFrameEndNotify(DPLAYER_HANDLE   player_handle,
    void (*pf_notify)(int32 frame_num))
{
    return DPLAYER_SUCCESS;
}

/******************************************************************************/
// Description: set display parameter
// Author:     
// Input:      display_param_t *pParam pointer to display parameter strcture
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_SetDisplayParam(DPLAYER_HANDLE         player_handle,
    DPLAYER_DISPLAY_PARAM_T const *param_ptr)
{
    return DPLAYER_SUCCESS;
}

/******************************************************************************/
// Description: get current frame data for display or other application.
// Input      : 1. dest_frame_ptr, the destin frame offered by APP.
//                 now it's null.
//              2. pf_frm_rdy_callback, if =NULL, it means sync process,otherwise
//                 async process. Now only support sync process.
// Output     : 1. src_frame_ptr, the src frame output from lowlevel.
// Return     : 
// Note       : when dest_frame_ptr is null, the buffer of src_frame_ptr will 
//              allocate by lowlevel, otherwise will use the buffer in 
//              dest_frame_ptr.
/******************************************************************************/
PUBLIC DPLAYER_RETURN_E DPLAYER_GetCurrentFrameData(
					DPLAYER_HANDLE			 player_handle,
                    DPLAYER_IMAGE_DATA_T     *des_frame_ptr,
					DPLAYER_IMAGE_DATA_T     *src_frame_ptr,
					PF_FRM_RDY_CALLBACK      pf_frm_rdy_callback
					)
{
    return DPLAYER_SUCCESS;
}


/******************************************************************************/
// Description: register display end notify function
// Author:     
// Input:      pf_notify   point to notify function
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_SetEndNotify(DPLAYER_HANDLE    player_handle,
    void (*pf_notify)(int32 i_type))
{
    return DPLAYER_SUCCESS;
}

/******************************************************************************/
// Description: DPLAYER_ForceAVSync
// Author:     
// Input:      
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_ForceAVSync(DPLAYER_HANDLE player_handle,
    BOOLEAN     support_skip )
{
    return DPLAYER_SUCCESS;
}

/******************************************************************************/
// Description: DPLAYER_StreamRegistRxDataPath
// Author:     
// Input:      
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_StreamRegistRxDataPath(DPLAYER_HANDLE player_handle, DPLAYER_STRM_GETFRAME_PTR pf_getframe_ptr, DPLAYER_STRM_FREEFRAME_PTR pf_freeframe_ptr)
{
	return DPLAYER_SUCCESS;
}

/******************************************************************************/
// Description: DPLAYER_StreamRxStart
// Author:     
// Input:      
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_StreamRxStart(DPLAYER_HANDLE player_handle, DPLAYER_STRM_POS_T *p_new_pos)
{	
	return DPLAYER_SUCCESS;
}

/******************************************************************************/
// Description: DPLAYER_StreamRxStop
// Author:     
// Input:     
// Output:     none
// Return:     DPLAYER_RETURN_E
// Note:       none
/******************************************************************************/
DPLAYER_RETURN_E DPLAYER_StreamRxStop(DPLAYER_HANDLE player_handle)
{
	
	return DPLAYER_SUCCESS;
}
