/*************************************************************************
 ** File Name:      mmijava_nv.c                                              *
 ** Author:         arthur.peng                                          *
 ** Date:           2006/09/15                                          *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/15     arthur.peng      Create.                              *
*************************************************************************/


#ifdef JAVA_SUPPORT_SUN

#include "sci_types.h"
#include "mmijava_nv.h"
#include "mmi_modu_main.h"
#include "mmijava_export.h"
#include "javacall_logging.h"

/*the length of java nv*/
const uint16 mmijava_nv_len[] =
{      

    sizeof(int32),         //MMINV_JAVA_MEDIA_VOLUME 
    sizeof(MMIBROWSER_MIME_CALLBACK_PARAM_T),          
    
};

/*****************************************************************************/
//  Description : register java module nv len and max item
//  Global resource dependence : none
//  Author: arthur.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIJAVA_RegJavaNv(void)
{
    // actually, MMI_RegModuleNv() function shoulde be boolean return type,
    // we can't get any success or fail info from this interface! @arthur
    MMI_RegModuleNv(MMI_MODULE_JAVA, mmijava_nv_len, sizeof(mmijava_nv_len) / sizeof(uint16));
    return TRUE;
}

/*****************************************************************************/
// 	Description : Read Download Manager
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIJAVA_ReadOtaCallbackFile(MMIBROWSER_MIME_CALLBACK_PARAM_T *param_ptr)
{
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;

    javacall_printf(LOG_STR_ENTER("param_ptr = %x"), param_ptr);    
    if(PNULL == param_ptr)
    {
        return 2;
    }

    MMINV_READ(MMINV_JAVA_CALLBACK_PARAM, param_ptr, nv_value);

    if (MN_RETURN_SUCCESS != nv_value)
    {
        MMIBROWSER_MIME_CALLBACK_PARAM_T *temp_param_ptr = PNULL;

        temp_param_ptr = SCI_ALLOCA(sizeof(MMIBROWSER_MIME_CALLBACK_PARAM_T));
        SCI_MEMSET(temp_param_ptr, 0x00, sizeof(MMIBROWSER_MIME_CALLBACK_PARAM_T));

        MMIJAVA_WriteOtaCallbackFile(temp_param_ptr);

        SCI_FREE(temp_param_ptr);

        return 1;   //error
    }
    return 0;       //no error
}

/*****************************************************************************/
// 	Description :  Write Download Manager
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIJAVA_WriteOtaCallbackFile(MMIBROWSER_MIME_CALLBACK_PARAM_T *param_ptr)
{   
    javacall_printf(LOG_STR_ENTER("param_ptr = %x"), param_ptr);    
    if(PNULL == param_ptr)
    {
        return 1;
    }
    MMINV_WRITE(MMINV_JAVA_CALLBACK_PARAM, param_ptr);

    return 0;
}
#endif