#ifndef _MMICAMERA_MANAGER_H
#define _MMICAMERA_MANAGER_H

#include "sci_types.h"
#include "mmk_type.h"
#include "dal_dcamera.h"

typedef enum
{	
    MMICAMAERA_PHOTO_SIZE_176X220,
        MMICAMAERA_PHOTO_SIZE_220X176,
        MMICAMAERA_PHOTO_SIZE_240X320,
        MMICAMAERA_PHOTO_SIZE_240X400,
        MMICAMAERA_PHOTO_SIZE_400X240,
        MMICAMAERA_PHOTO_SIZE_QVGA,
        MMICAMAERA_PHOTO_SIZE_VGA,
        MMICAMAERA_PHOTO_SIZE_1MP,
        MMICAMAERA_PHOTO_SIZE_2MP,
        MMICAMAERA_PHOTO_SIZE_3MP,
        MMICAMAERA_PHOTO_SIZE_MAX
}MMICamera_PHOTO_SIZE_E;

/*****************************************************************************/
// 	Description : start to run the flow of camera
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_OpenDC(void);

/*****************************************************************************/
// 	Description : start photo preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_StartDCPreview(void);

/*****************************************************************************/
// 	Description : stop photo preview
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_StopDCPreview(void);

/*****************************************************************************/
// 	Description : close dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_CloseDC(void);

/*****************************************************************************/
// 	Description : snap shot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_SnapShot(DCAMERA_CALL_BACK CaptureCallBack);

/*****************************************************************************/
// 	Description : set preview rect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_SetPreviewRect(GUI_RECT_T rect);

/*****************************************************************************/
// 	Description : set preview rect
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMICamera_GetPreviewRect(void);

/*****************************************************************************/
// 	Description : set snapshot photo size
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_SetPhotoSize(MMICamera_PHOTO_SIZE_E size);

/*****************************************************************************/
// 	Description : get preview param
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_InitDCPreviewParam(void);

/*****************************************************************************/
// 	Description : set preview rect
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICamera_SetFullScreenPreview(BOOLEAN full_screen_mode);
#endif


