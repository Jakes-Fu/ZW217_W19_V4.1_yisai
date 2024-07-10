/*****************************************************************************
** File Name:      mmipic_edit_export.c                                                  *
** Author:                                                                   *
** Date:           01/10/2008                                                *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe pic editor                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2008       Vincent.shang       Create                                  *
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "sci_types.h"
//#include "mmipic_edit_export.h"
//macro replace
//#ifdef MMI_PIC_EDITOR_ENABLE
#ifdef PIC_EDITOR_SUPPORT
#include "mmipic_edit.h"
//enter piceditor disable 3D;exit piceditor enable 3D
#include "mmi_default.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                 *
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
/*****************************************************************************/
//  Description : exit from pic editor
//  Global resource dependence : none
//  Author: vincent.shang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPIC_ExitFromPicEditor(void)
{
    MMIPIC_ExitFromPicEditor();
}
/*****************************************************************************/
//  Description : init pic module
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPIC_InitModule(void)
{
    MMIPIC_InitModule();
}
/*****************************************************************************/
//  Description : open pic editor 
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPIC_OpenPicEditorMainWin(
                                              const wchar    *full_path_ptr,     //in
                                              uint16         full_path_len      //in:字节数
                                              )
{
//enter piceditor disable 3D;exit piceditor enable 3D
    //MMI_Enable3DMMI(FALSE);
#ifdef UI_P3D_SUPPORT
    MMI_Disable3DEffect(MMI_3D_EFFECT_PIC_EDITOR);
#endif
    return (MMIPIC_OpenPicEditorMainWin(full_path_ptr,full_path_len));
}



#else
/*****************************************************************************/
//  Description : exit from pic editor
//  Global resource dependence : none
//  Author: vincent.shang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPIC_ExitFromPicEditor(void)
{
    
}
/*****************************************************************************/
//  Description : init pic module
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPIC_InitModule(void)
{
    
}
/*****************************************************************************/
//  Description : open pic editor 
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPIC_OpenPicEditorMainWin(uint16         *full_path_ptr,     //in
                                              uint16         full_path_len      //in:字节数
                                              )
{
#if 0 
    uint16 *ptr = full_path_ptr;
    uint16 len = full_path_len;
#endif
    return TRUE;
}
#endif
