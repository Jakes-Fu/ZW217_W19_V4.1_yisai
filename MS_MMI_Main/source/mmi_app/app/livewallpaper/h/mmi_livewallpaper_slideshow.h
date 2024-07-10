#ifndef _MMI_LIVEWALLPAPER_SLIDESHOW_H_
#define _MMI_LIVEWALLPAPER_SLIDESHOW_H_

#include "mmilivewallpaper_export.h"
#include "mmiset_export.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/


typedef struct
{
  //  uint32             effect_type;
    uint32             slide_period;
    uint16             last_play_index;
}WALLPAPER_SLIDESHOW_NV_T;

/*---------------------------------------------------------------------------*/
/*                          PUBLIC FUNCTION DECLARE                          */
/*---------------------------------------------------------------------------*/
#ifdef UI_P3D_SUPPORT
/*****************************************************************************/
//  Description :play  slideshow effect
//  Global resource dependence : 
//  Author:xiaohua.liu
//  Note:
/*****************************************************************************/
PUBLIC void MMILIEVEWALLPAPER_PlaySlideShowEffect(MMI_WIN_ID_T win_id);
#endif
/*****************************************************************************/
//  Description : when photo decode fail.whether reset wallpaper to default type
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMILiveWallpaper_NeedResetWallpaper(uint16 wallpaper_id);


#endif //_MMI_LIVEWALLPAPER_SLIDESHOW_H_
