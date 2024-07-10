#ifndef __OD_GAME_RES_ID_DEF_H__
#define __OD_GAME_RES_ID_DEF_H__
#include "od_game_menu.h"
#include "od_game_id.h"
#include "od_game_image.h"
#include "od_game_text.h"
#include "od_game_menutable.h"
#ifdef MAINLCD_LOGIC_ANGLE_90
#define OD_LCD_WIDTH     (MMI_MAINSCREEN_HEIGHT)
#define OD_LCD_HEIGHT    (MMI_MAINSCREEN_WIDTH)
#else
#define OD_LCD_WIDTH     (MMI_MAINSCREEN_WIDTH)
#define OD_LCD_HEIGHT    (MMI_MAINSCREEN_HEIGHT)
#endif


#if defined(MAINLCD_DEV_SIZE_128X128) 
#define __MMI_MAINLCD_128X128__
#elif defined(MAINLCD_DEV_SIZE_128X160)
#define __MMI_MAINLCD_128X160__
#elif defined(MAINLCD_DEV_SIZE_160X128)
#define __MMI_MAINLCD_160X128__
#elif defined(MAINLCD_DEV_SIZE_176X220)
#define __MMI_MAINLCD_176X220__
#elif defined(MAINLCD_DEV_SIZE_220X176)
#define __MMI_MAINLCD_220X176__
#elif defined(MAINLCD_DEV_SIZE_240X320)
#define __MMI_MAINLCD_240X320__
#elif defined(MAINLCD_DEV_SIZE_320X240)
#define __MMI_MAINLCD_320X240__
#elif defined(MAINLCD_DEV_SIZE_240X400)
#define __MMI_MAINLCD_240X400__
#elif defined(MAINLCD_DEV_SIZE_272X480)
#define __MMI_MAINLCD_272X480__
#elif defined(MAINLCD_DEV_SIZE_320X480)
#define __MMI_MAINLCD_320X480__
#endif 
#endif  /* __OD_GAME_RES_ID_DEF_H__ */
