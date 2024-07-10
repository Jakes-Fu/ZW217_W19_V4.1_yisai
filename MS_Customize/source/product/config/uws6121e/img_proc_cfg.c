/*******************************************************************************
 ** FileName:    img_proc_cfg.c
 ** Author:        Tim.zhu
 ** Date:          2011/12/26
 ** CopyRight:   2011, Spreatrum, Incoporated, All right reserved
 ** Description:
 *******************************************************************************

 *******************************************************************************
 **                        Edit History
 ** -------------------------------------------------------------------------- *
 ** DATE              NAME                   DESCRIPTION
 ** 2011/12/26     Tim.zhu                 Created
 *******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "img_alg.h"

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
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         extend Variables and function                     *
 **---------------------------------------------------------------------------*/
extern IMG_ALG_INFO_T s_face_dect_info_tab;
//extern IMG_ALG_ALG_INFO_T s_panorama_info_tab;
extern IMG_ALG_INFO_T s_ninone_info_tab;
extern IMG_ALG_INFO_T s_hdr_info_tab;
extern IMG_ALG_INFO_T g_fisheye_info;
extern IMG_ALG_INFO_T s_lomo_info_tab;
extern IMG_ALG_INFO_T s_xray_info_tab;
extern IMG_ALG_INFO_T s_infrared_info_tab;
extern IMG_ALG_INFO_T s_pencilsketch_info_tab;
extern IMG_ALG_INFO_T s_frame_info;
extern IMG_ALG_INFO_T s_exposal_info_tab;
extern IMG_ALG_INFO_T s_negative_info_tab;

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

LOCAL IMG_ALG_EXP_INFO_T s_img_alg_exp_info_tab[]=
{
    {IMG_PROC_FACE_DECT, (IMG_ALG_INFO_T_PTR)&s_face_dect_info_tab},
//    {IMG_PROC_PANORAMA, (IMG_ALG_INFO_T_PTR)&s_panorama_info_tab},
    {IMG_PROC_INFRARED, (IMG_ALG_INFO_T_PTR)&s_infrared_info_tab},
    {IMG_PROC_XRAY, (IMG_ALG_INFO_T_PTR)&s_xray_info_tab},
    {IMG_PROC_LOMO, (IMG_ALG_INFO_T_PTR)&s_lomo_info_tab},
    {IMG_PROC_FISHEYE, (IMG_ALG_INFO_T_PTR)&g_fisheye_info},
    {IMG_PROC_HDR, (IMG_ALG_INFO_T_PTR)&s_hdr_info_tab},
    {IMG_PROC_NINONE, (IMG_ALG_INFO_T_PTR)&s_ninone_info_tab},
    {IMG_PROC_BLACK_WHITE_DRAWING, (IMG_ALG_INFO_T_PTR)&s_pencilsketch_info_tab},
    {IMG_PROC_PHOTO_FRAME, (IMG_ALG_INFO_T_PTR)&s_frame_info},
    {IMG_PROC_EXPOSAL, (IMG_ALG_INFO_T_PTR)&s_exposal_info_tab},
    {IMG_PROC_NEGATIVE, (IMG_ALG_INFO_T_PTR)&s_negative_info_tab},
    {IMG_PROC_CMD_MAX, NULL}
};

LOCAL IMG_PROC_IOCTL_FUNC_TAB_T s_img_proc_ioctl_fun_tab[]=
{
    {PNULL, IMGPROC_SetProcImgDstBuf},
    {PNULL, PNULL},
};

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description: get img proc alg exp info
// Global resource dependence:
// Author: Tim.zhu
/*****************************************************************************/
PUBLIC IMG_ALG_EXP_INFO_T_PTR IMG_ALG_GetAlgExpInfo(void)
{
    IMG_ALG_EXP_INFO_T_PTR exp_info_ptr=NULL;

    exp_info_ptr=(IMG_ALG_EXP_INFO_T_PTR)&s_img_alg_exp_info_tab;

    return exp_info_ptr;
}

/*****************************************************************************/
// Description: get img proc ioctl fun
// Global resource dependence:
// Author: Tim.zhu
/*****************************************************************************/
PUBLIC IMG_PROC_IOCTL_FUNC_TAB_T_PTR IMG_PROC_GetIoCtlFun(void)
{
    IMG_PROC_IOCTL_FUNC_TAB_T_PTR ioctl_fun_ptr=NULL;

    ioctl_fun_ptr=(IMG_PROC_IOCTL_FUNC_TAB_T_PTR)&s_img_proc_ioctl_fun_tab[0];

    return ioctl_fun_ptr;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
/*lint -restore */
#ifdef   __cplusplus
}
#endif
