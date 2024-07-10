/******************************************************************************
 ** File Name:      dc_param_ctrl.h                                           * 
 ** Author:         Jing.Li                                                   *
 ** DATE:           2008/12/09                                                *
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 *******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 2008/12/09     Jing.Li          Create.                                   *
 ******************************************************************************/

#ifndef DC_PARAM_CTRL_H
#define DC_PARAM_CTRL_H
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "dal_dcamera.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro define                                      *
 **---------------------------------------------------------------------------*/
typedef DCAMERA_RETURN_VALUE_E (*pf_dcamera_param_ctrl_process)(uint32 arg, DCAMERA_PARAM_CTRL_CALL_BACK callback);

/******************************************************************************/
// Description: Get the parameter control process function according to the input command
// Author:     jing.li
// Input:      cmd: the request command
// Output:     none
// Return:     NONE
//             
// Note:       
/******************************************************************************/
PUBLIC pf_dcamera_param_ctrl_process DC_GetParamCtrlFunc(uint32 cmd);

/******************************************************************************/
// Description: get callback fun of param ctrl 
// Author:     Tim.zhu
// Input:      none
// Output:     none
// Return:     the point of callback fun
//             
// Note:       
/******************************************************************************/
PUBLIC DCAMERA_PARAM_CTRL_CALL_BACK DC_GetParamCtrlCallBack(void);

/******************************************************************************/
// Description: notify the param ctrl caller that param_ctrl is done
// Author:     Tim.zhu           
// Note:       
/******************************************************************************/
PUBLIC int DC_ParamCtrlCallback(uint32 is_success);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif



#endif /*DC_PARAM_CTRL_H*/
