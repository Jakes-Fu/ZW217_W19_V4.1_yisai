#ifndef _MMI_LIVEWALLPAPER_SAKURA_H_
#define _MMI_LIVEWALLPAPER_SAKURA_H_

#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "mmilivewallpaper_export.h"
		
typedef struct
{
    MMI_IMAGE_ID_T    bg_id;    // 背景
    MMI_IMAGE_ID_T    fg_id;    // 前景
    uint16    petal_color;    // sakura和peta图片的颜色
    uint16    petal_type;    // 当前飘动物体的类型0:sakura    1: petal    2:随机,两者都包含
    BOOLEAN    need_pollen;    // 是否需要pollen(花粉)效果
    BOOLEAN    need_moon;    // 是否需要月亮
}WALLPAPER_SAKURA_NV_T;

#ifdef   __cplusplus
    }
#endif


#endif //_MMI_LIVEWALLPAPER_SAKURA_H_

