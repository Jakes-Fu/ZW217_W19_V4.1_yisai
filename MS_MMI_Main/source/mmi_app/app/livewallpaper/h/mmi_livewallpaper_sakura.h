#ifndef _MMI_LIVEWALLPAPER_SAKURA_H_
#define _MMI_LIVEWALLPAPER_SAKURA_H_

#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "mmilivewallpaper_export.h"
		
typedef struct
{
    MMI_IMAGE_ID_T    bg_id;    // ����
    MMI_IMAGE_ID_T    fg_id;    // ǰ��
    uint16    petal_color;    // sakura��petaͼƬ����ɫ
    uint16    petal_type;    // ��ǰƮ�����������0:sakura    1: petal    2:���,���߶�����
    BOOLEAN    need_pollen;    // �Ƿ���Ҫpollen(����)Ч��
    BOOLEAN    need_moon;    // �Ƿ���Ҫ����
}WALLPAPER_SAKURA_NV_T;

#ifdef   __cplusplus
    }
#endif


#endif //_MMI_LIVEWALLPAPER_SAKURA_H_

