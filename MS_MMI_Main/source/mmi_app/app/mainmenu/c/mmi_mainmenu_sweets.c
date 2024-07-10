/*****************************************************************************
** File Name:      mmi_mainmenu_sweets.c                                     *
** Author:                                                                   *
** Date:                                                                     *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**                                 Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "guimenu.h"
#include "guires.h"
#include "mmi_mainmenu_export.h"
#include "mmi_theme.h"
#include "mmi_common.h"
#include "mmi_modu_main.h"
#include "mmidisplay_data.h"
#include "guistring.h"
#include "mmi_image.h"
#include "mmi_link.h"
#include "mmi_text.h"
#include "mmi_nv.h"
#include "guilcd.h"
#include "mmk_tp.h"
#include "mmitheme_special.h"
#include "mmi_id.h"
#include "mmiset_display.h"
#include "mmi_common.h"
#include "mmk_app.h" 
#include "mmifm_export.h"
#include "guistatusbar.h"
#include "graphics_3d_rotate.h"
#include "mmimp3_export.h"
#include "mmiset_export.h"
#include "mmi_menutable.h"
#ifdef DYNAMIC_MAINMENU_SUPPORT
#include "mmi_mainmenu_synchronize.h"
#endif
#include "mmiudisk_export.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "mmi_resource.h"
#include "mmi_mainmenu_sweets.h"
#include "mmitheme_mainmenu.h"
#include "mmi_mainmenu_data.h"
#include "guictrl_api.h"
#ifdef MENU_SWEETS_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#ifdef MAINLCD_SIZE_320X480
#define SWEETS_ICON_TEXT_WIDTH    71
#define SWEETS_ICON_TEXT_HEIGHT    71
#define EQUATER 80  //直径如若效果不好可-1
#define GLASS_H  50
#define GLASS_W 50
#define SWEETS_PAGE_ICON_NUM 12    //每页item的数目
#define SWEETS_TXT_OFFSET_DOWN_ICON 5  //文字的位置，距离icon底部的距离
#define SWEETS_TITLE_ICON_RECT_V {90, 453, 230, 476}   //竖屏
#define SWEETS_TITLE_ICON_RECT_H {10, 120, 22, 200}    //横屏
#elif defined MAINLCD_SIZE_240X400
#define SWEETS_ICON_TEXT_WIDTH    56
#define SWEETS_ICON_TEXT_HEIGHT    56
#define EQUATER 64  //直径  如若效果不好可-1
#define GLASS_H  40
#define GLASS_W 40
#define SWEETS_PAGE_ICON_NUM 12    //每页item的数目
#define SWEETS_TXT_OFFSET_DOWN_ICON 5  //文字的位置，距离icon底部的距离
#define SWEETS_TITLE_ICON_RECT_V {50, 380, 190, 399}   //竖屏
#define SWEETS_TITLE_ICON_RECT_H {10, 120, 22, 200}    //横屏
#elif defined MAINLCD_SIZE_240X320
#define SWEETS_ICON_TEXT_WIDTH    60
#define SWEETS_ICON_TEXT_HEIGHT    60
#define EQUATER 67  //直径如若效果不好可-1
#define GLASS_H  42
#define GLASS_W 42
#define SWEETS_PAGE_ICON_NUM 7    //每页item的数目
#define SWEETS_TXT_OFFSET_DOWN_ICON 1  //文字的位置，距离icon底部的距离
#define SWEETS_TITLE_ICON_RECT_V {20, 300, 220, 319}   //竖屏
#define SWEETS_TITLE_ICON_RECT_H {90, 220, 230, 239}    //横屏
#else
#define SWEETS_ICON_TEXT_WIDTH    71
#define SWEETS_ICON_TEXT_HEIGHT    71
#define EQUATER 80  //直径
#define GLASS_H  50
#define GLASS_W 50
#define SWEETS_PAGE_ICON_NUM 12    //每页item的数目
#define SWEETS_TXT_OFFSET_DOWN_ICON 5  //文字的位置，距离icon底部的距离
#define SWEETS_TITLE_ICON_RECT_V {90, 453, 230, 476}   //竖屏
#define SWEETS_TITLE_ICON_RECT_H {10, 120, 22, 200}    //横屏
#endif

#define SWEETS_INIT_PAGE_ITEM_NUM 3   //初始化page的数目
#define SWEETS_MOVE_MAX_ANGLE   350  //移动时倾斜的最大角度
#define SWEETS_MOVE_TO_AIM_ANGLE_PIX 100    //变化到目标需要一百个像素
#define SWEETS_MAX_CHANGE_ANGLE 100   //变化角度
#define SWEETS_IDLE_SHAKE_ANGLE    200 //idle时shake的最大角度
#define SWEETS_TP_DOWN_SHAKE_ANGLE  400 //tp down时 shake的最大角度

#define SWEETS_SHAKE_FRAME   70  //shake的总帧数

#define SWEETS_CHANGE_PAGE_TOTAL_FRAME_MAX MMI_MAINSCREEN_WIDTH  //必须被2整除
#define SWEETS_CHANGE_PAGE_ONE_FRAME_PIX (MMI_MAINSCREEN_WIDTH/SWEETS_CHANGE_PAGE_TOTAL_FRAME_MAX)  //必须被2整除

#define SWEETS_TPUP_STEP_PARAM  2  //tp up后跳跃的步长

#define SWEETS_START_MOVE_PIX   3  //移动三个像素开始move

#define SWEETS_STOP_MOVE_NUM    10   //停止三次算停止

#define SWEETS_VELOCITY_PARAM    4  //速度转化为SHAKE的参数

#define SWEETS_ICON_WIDTH   MENUICON_WIDTH    //中间小图标的大小
#define SWEETS_ICON_HEIGHT  MENUICON_HEIGHT

#define SWEETS_SHADOW_WIDTH      SWEETS_ICON_WIDTH     //shadow的大小
#define SWEETS_SHADOW_HEIGHT     (SWEETS_ICON_HEIGHT/4)

#define SWEETS_ENTER_ANIM_FRAME   10  //入场动画帧数

#define SWEETS_MENUTEXT_WIDTH SWEETS_ICON_TEXT_WIDTH
#define SWEETS_MENUTEXT_HEIGHT MENUTEXT_HEIGHT

#define SWEETS_MAX_SHAKE_ANGLE  800     //最大晃动角度

#define SWEETS_MENU_HAVE_ENTER_ANIM //有入场动画
//算法
#define ICON_W SWEETS_ICON_TEXT_WIDTH
#define ICON_H  SWEETS_ICON_TEXT_HEIGHT
#define ICON_NUM SWEETS_PAGE_ICON_NUM

#define DO_LIGHT     //是否打光
#define INANGLE 1200  
#define ALIAS_ANGLE 10

//#define RONS   //上下晃动
#define ROEW     //左右晃动

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
typedef struct
{
    GUI_LCD_DEV_INFO layer_handle;
    uint8  index;//取值为0，1，2 始终以1为中心
    BOOLEAN is_change;
}SWEETS_MEM_PAGE_INFO_T;

typedef enum
{
    IN_POSITON_NONE = 0,//无
    IN_POSITON_LEFT,//向左滑动
    IN_POSITON_RIGHT,//向右滑动
    IN_POSITON_UP,//向上滑动
    IN_POSITON_DOWN,//向下滑动
    IN_POSITON_MAX
}IN_POSITON_DIRECTION_E;


//当前状态
typedef enum
{
    SWEETS_STATE_NONE = 0,//静止
    SWEETS_STATE_ENTER_ANIM,   //入场动画
    SWEETS_STATE_SHAKE,   //晃动
    SWEETS_STATE_ROTATION_MOVE,//移动页面
    SWEETS_STATE_STANDBY_ANIM,  //待机动画
    SWEETS_STATE_MAX
}SWEETS_STATE_E;

typedef struct{
    uint16 * out_icon_layer;//主层
    uint32 * out_shine_layer;//dst层
    uint32 * glass_color_ptr;//光幕层
    uint8 cur_icon_num;//数目
    uint8 next_icon_num;//数目
    uint16 icon_w;//图标宽度
    uint16 icon_h;//图标高度
    uint16 dst_width;//DST宽
    uint16 dst_height;//dst高
    uint32 *current_page_icon_buf;//图标BUF
    uint32 *next_page_icon_buf;//下一页图标buf
    int16  angle; //角度
    int16  * out_current_angle;
    int16 cur_frame;//当前帧
    int16 total_frame;//总帧数
    int16 c_page_offset;  
    IMGREF_POINT_T * icon_location;//图标位置
    SWEETS_STATE_E cur_state; //当前状态
    IN_POSITON_DIRECTION_E in_position;
    BOOLEAN Is_Init;
    uint32 *shadow_buf;
    uint32 *cur_text_buf;
    uint32 *next_text_buf;
    uint16 text_width;
    uint16 text_height;
    uint16 shadow_w;
    uint16 shadow_h;
}SWEETS_INPARAM_T;


//记录所有全局信息的结构
typedef struct
{
    //GUI_LCD_DEV_INFO wallpaper_layer_handle;//bg layer 565 
    GUI_LCD_DEV_INFO dst_layer_handle;//dst layer 888
    GUI_LCD_DEV_INFO icon_bg_layer_handle;//icon bg layer 565   资源
    GUI_LCD_DEV_INFO up_cover_layer_handle; //光     资源
    GUI_LCD_DEV_INFO enter_anim_temp_layer_handle; //入场动画临时handle  888
    GUI_LCD_DEV_INFO text_layer_handle;
    GUI_LCD_DEV_INFO glass_layer_handle; //玻璃窗
    GUI_LCD_DEV_INFO shadow_layer_handle;
    SWEETS_MEM_PAGE_INFO_T mem_page_info[SWEETS_INIT_PAGE_ITEM_NUM];//item层信息
    uint16           total_page_num;  //总共有多少页
    BOOLEAN          is_in_change_state;  //
    SWEETS_STATE_E   cur_state;  //当前状态
    uint16           total_frame;
    uint16           cur_frame;
    int16            cur_angle; //当前算法显示的角度
    int16            aim_angle;  //要变化到的目标angle    
    uint16           stop_move_num; //停止的次数，超过三个认为停止
    IMGREF_POINT_T   *icon_location;
    FIX16_POINT_T * Point_Add_ptr;
    int16            change_angle;
}SWEETS_MENU_INFO_T;

typedef struct  {
    int x;
    int y;
    int z;
}THREE_VECTOR_T;

typedef struct  {
    int32 x;
    int32 y;
    int32 z;
}IMGREF_LONG_3D_POINT_T;

typedef struct  {
    IMGREF_SIZE_T Out_Size;
    IMGREF_SIZE_T map_Size;
    IMGREF_POINT_T * icon_location;
    int equator;
    int16 offset_centre;
    IMGREF_LONG_3D_POINT_T Ball_centre;
    THREE_VECTOR_T vector_ro_axis; //自转轴单位向量
    THREE_VECTOR_T vector_eq_axis;//过球心的零度精度的赤道轴
}BALL_DATA_PARAM_ARGB_T;

/*lint -e770*/
//long形 POINT
typedef struct{
    int x;
    int y;
}IMGREF_LONG_POINT_T;
/*lint -e770*/

typedef struct{
    uint16 * out_icon_layer;
    uint32 * glass_ptr;
    uint32 * out_shine_layer;
    uint16 dst_width;
    uint16 dst_height;
    uint16 radius;
    IMGREF_POINT_T offset;
    IMGREF_3D_POINT_T centre_p;
    uint32 * icon_buffer;
}FILLBALL_INPARAM_T;



typedef struct
{

    MMI_HANDLE_T                win_handle;                             //window handle
    MMI_HANDLE_T                ctrl_handle;

    GUIMENU_STATE_T             menu_state;
    BOOLEAN                     is_active;
    
    uint16                      cur_page_index;                         //default 0,from 0 to max-1
    uint16                      cur_item_index;                         //current selected menu index. in option page style, it is also item index
    uint16                      item_total_num;                         //current total visible item number. in option page style, it is page count.

    MMI_MENU_GROUP_ID_T         cur_group_id;                           //current menu group id
 

    MMITHEME_MAINMENU_COMMON_T  main_menu_theme;                        //main menu theme

    MMIMAINEMENU_ITEM_LIST_T    menu_list_info;                         //menu list information
    uint32                      start_item_index;

    BOOLEAN                     is_move_state;                          //item是否在抖动
    BOOLEAN                     is_moving;                              //item是否被拖动
    BOOLEAN                     is_item_pressed;                        //tp down时，是否在item上
    BOOLEAN                     is_tp_pressed;                          //is tp down?

    
    //CONTINUE_MOVE_TYPE_E 	    continue_move_type;	                    // 连续移动的类型
    uint8                       redrew_timer_id;				        // 滑动时，slide和fling状态下，画图的timer

    BOOLEAN                     is_display_title;                       // 是否显示title

    MMK_TP_SLIDE_E              slide_state;                            //current slide state
    GUI_POINT_T                 slide_start_point;                      //Slide时候使用
    GUI_POINT_T                 slide_pre_point;                        //Slide时候使用

    GUI_LCD_DEV_INFO            titleidx_layer_handle;                  // 页面索引层

    float                       fling_velocity;                         //滑动速度
    GUIMENU_ENTER_ANIM_TYPE_E   enter_anim_type;                        // 进入时的动画类型

    uint16                      pre_msg_loop_page_index;                // 每次消息循环后，focus page index
    uint16                      pre_msg_loop_item_index;                // 每次消息循环后，focus项index
    uint16                      pre_msg_loop_menu_level;                // 每次消息循环后，focus项的level

    uint32			            icon_shake_period;	                    // icon 震动时间间隔
    uint32			            auto_move_period;	                    // 支持连续移动的时间间隔

} MMIMAINMENU_SWEETS_DATA_T;


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL SWEETS_MENU_INFO_T s_sweets_menu_info = {0};//记录所有全局信息的全局结构变量
#ifdef MAINLCD_SIZE_320X480
LOCAL CONST IMGREF_POINT_T s_icon_location_v[SWEETS_PAGE_ICON_NUM]=
{
    //2 3 2 3 2
    {69,33},{175,33},
    {16,116},{122,116},{228,116},
    {69,199},{175,199},
    {16,282},{122,282},{228,282},
    {69,365},{175,365}
};
LOCAL CONST IMGREF_POINT_T s_icon_location_h[SWEETS_PAGE_ICON_NUM]=
{
    //2 3 2 3 2
    {120,22},{280,22},
    {40,75},{200,75},{360,75},
    {120,128},{280,128},
    {40,181},{200,181},{360,181},
    {120,234},{280,234}
};
#elif defined MAINLCD_SIZE_240X400
LOCAL CONST IMGREF_POINT_T s_icon_location_v[SWEETS_PAGE_ICON_NUM]=
{
    //2 3 2 3 2
    {53,31},{131,31},
    {14,100},{92,100},{170,100},
    {53,169},{131,169},
    {14,238},{92,238},{170,238},
    {53,307},{131,307}
};
LOCAL CONST IMGREF_POINT_T s_icon_location_h[SWEETS_PAGE_ICON_NUM]=
{
    //2 3 2 3 2
    {120,22},{280,22},
    {40,75},{200,75},{360,75},
    {120,128},{280,128},
    {40,181},{200,181},{360,181},
    {120,234},{280,234}
};
#elif defined MAINLCD_SIZE_240X320
LOCAL CONST IMGREF_POINT_T s_icon_location_v[SWEETS_PAGE_ICON_NUM]=
{
    //2 3 2 3 2
    {51,34},{129,34},
    {12,124},{90,124},{168,124},
    {51,214},{129,214},

};
LOCAL CONST IMGREF_POINT_T s_icon_location_h[SWEETS_PAGE_ICON_NUM]=
{
    //2 3 2 3 2
    {74,22},{186,22},
    {18,85},{130,85},{242,85},
    {74,148},{186,148},
};
#else
LOCAL CONST IMGREF_POINT_T s_icon_location_v[SWEETS_PAGE_ICON_NUM]=
{
    //2 3 2 3 2
    {69,33},{175,33},
    {16,116},{122,116},{228,116},
    {69,199},{175,199},
    {16,282},{122,282},{228,282},
    {69,365},{175,365}
};
LOCAL CONST IMGREF_POINT_T s_icon_location_h[SWEETS_PAGE_ICON_NUM]=
{
    //2 3 2 3 2
    {120,22},{280,22},
    {40,75},{200,75},{360,75},
    {120,128},{280,128},
    {40,181},{200,181},{360,181},
    {120,234},{280,234}
};
#endif
#if 1//使用分辨率控制


#endif
//int  s_candy_cur_frame=0,s_candy_total_frame=100;
const unsigned short ball_sqrt[1025] = 
{
    0,128,181,221,256,286,313,338,362,384,
        404,424,443,461,478,495,512,527,543,557,
        572,586,600,613,627,640,652,665,677,689,
        701,712,724,735,746,757,768,778,789,799,
        809,819,829,839,849,858,868,877,886,896,
        905,914,923,931,940,949,957,966,974,983,
        991,999,1007,1015,1024,1031,1039,1047,1055,1063,
        1070,1078,1086,1093,1101,1108,1115,1123,1130,1137,
        1144,1152,1159,1166,1173,1180,1187,1193,1200,1207,
        1214,1221,1227,1234,1241,1247,1254,1260,1267,1273,
        1280,1286,1292,1299,1305,1311,1317,1324,1330,1336,
        1342,1348,1354,1360,1366,1372,1378,1384,1390,1396,
        1402,1408,1413,1419,1425,1431,1436,1442,1448,1453,
        1459,1465,1470,1476,1481,1487,1492,1498,1503,1509,
        1514,1519,1525,1530,1536,1541,1546,1551,1557,1562,
        1567,1572,1578,1583,1588,1593,1598,1603,1608,1614,
        1619,1624,1629,1634,1639,1644,1649,1654,1659,1664,
        1668,1673,1678,1683,1688,1693,1698,1702,1707,1712,
        1717,1722,1726,1731,1736,1740,1745,1750,1755,1759,
        1764,1768,1773,1778,1782,1787,1792,1796,1801,1805,
        1810,1814,1819,1823,1828,1832,1837,1841,1846,1850,
        1854,1859,1863,1868,1872,1876,1881,1885,1889,1894,
        1898,1902,1907,1911,1915,1920,1924,1928,1932,1936,
        1941,1945,1949,1953,1958,1962,1966,1970,1974,1978,
        1982,1987,1991,1995,1999,2003,2007,2011,2015,2019,
        2023,2027,2031,2035,2039,2043,2048,2051,2055,2059,
        2063,2067,2071,2075,2079,2083,2087,2091,2095,2099,
        2103,2107,2111,2114,2118,2122,2126,2130,2134,2138,
        2141,2145,2149,2153,2157,2160,2164,2168,2172,2176,
        2179,2183,2187,2191,2194,2198,2202,2205,2209,2213,
        2217,2220,2224,2228,2231,2235,2239,2242,2246,2250,
        2253,2257,2260,2264,2268,2271,2275,2278,2282,2286,
        2289,2293,2296,2300,2304,2307,2311,2314,2318,2321,
        2325,2328,2332,2335,2339,2342,2346,2349,2353,2356,
        2360,2363,2367,2370,2374,2377,2380,2384,2387,2391,
        2394,2398,2401,2404,2408,2411,2415,2418,2421,2425,
        2428,2432,2435,2438,2442,2445,2448,2452,2455,2458,
        2462,2465,2468,2472,2475,2478,2482,2485,2488,2491,
        2495,2498,2501,2505,2508,2511,2514,2518,2521,2524,
        2527,2531,2534,2537,2540,2543,2547,2550,2553,2556,
        2560,2563,2566,2569,2572,2575,2579,2582,2585,2588,
        2591,2594,2598,2601,2604,2607,2610,2613,2616,2620,
        2623,2626,2629,2632,2635,2638,2641,2644,2648,2651,
        2654,2657,2660,2663,2666,2669,2672,2675,2678,2681,
        2684,2688,2691,2694,2697,2700,2703,2706,2709,2712,
        2715,2718,2721,2724,2727,2730,2733,2736,2739,2742,
        2745,2748,2751,2754,2757,2760,2763,2766,2769,2772,
        2774,2777,2780,2783,2786,2789,2792,2795,2798,2801,
        2804,2807,2810,2813,2816,2818,2821,2824,2827,2830,
        2833,2836,2839,2842,2844,2847,2850,2853,2856,2859,
        2862,2865,2867,2870,2873,2876,2879,2882,2884,2887,
        2890,2893,2896,2899,2901,2904,2907,2910,2913,2916,
        2918,2921,2924,2927,2930,2932,2935,2938,2941,2944,
        2946,2949,2952,2955,2957,2960,2963,2966,2968,2971,
        2974,2977,2979,2982,2985,2988,2990,2993,2996,2999,
        3001,3004,3007,3010,3012,3015,3018,3020,3023,3026,
        3029,3031,3034,3037,3039,3042,3045,3047,3050,3053,
        3055,3058,3061,3063,3066,3069,3072,3074,3077,3079,
        3082,3085,3087,3090,3093,3095,3098,3101,3103,3106,
        3109,3111,3114,3117,3119,3122,3124,3127,3130,3132,
        3135,3137,3140,3143,3145,3148,3150,3153,3156,3158,
        3161,3163,3166,3169,3171,3174,3176,3179,3182,3184,
        3187,3189,3192,3194,3197,3200,3202,3205,3207,3210,
        3212,3215,3217,3220,3222,3225,3228,3230,3233,3235,
        3238,3240,3243,3245,3248,3250,3253,3255,3258,3260,
        3263,3265,3268,3270,3273,3275,3278,3280,3283,3285,
        3288,3290,3293,3295,3298,3300,3303,3305,3308,3310,
        3313,3315,3318,3320,3323,3325,3328,3330,3332,3335,
        3337,3340,3342,3345,3347,3350,3352,3354,3357,3359,
        3362,3364,3367,3369,3372,3374,3376,3379,3381,3384,
        3386,3388,3391,3393,3396,3398,3401,3403,3405,3408,
        3410,3413,3415,3417,3420,3422,3425,3427,3429,3432,
        3434,3436,3439,3441,3444,3446,3448,3451,3453,3456,
        3458,3460,3463,3465,3467,3470,3472,3474,3477,3479,
        3481,3484,3486,3489,3491,3493,3496,3498,3500,3503,
        3505,3507,3510,3512,3514,3517,3519,3521,3524,3526,
        3528,3531,3533,3535,3537,3540,3542,3544,3547,3549,
        3551,3554,3556,3558,3561,3563,3565,3567,3570,3572,
        3574,3577,3579,3581,3584,3586,3588,3590,3593,3595,
        3597,3599,3602,3604,3606,3609,3611,3613,3615,3618,
        3620,3622,3624,3627,3629,3631,3633,3636,3638,3640,
        3642,3645,3647,3649,3651,3654,3656,3658,3660,3663,
        3665,3667,3669,3672,3674,3676,3678,3680,3683,3685,
        3687,3689,3692,3694,3696,3698,3700,3703,3705,3707,
        3709,3712,3714,3716,3718,3720,3723,3725,3727,3729,
        3731,3734,3736,3738,3740,3742,3744,3747,3749,3751,
        3753,3755,3758,3760,3762,3764,3766,3768,3771,3773,
        3775,3777,3779,3781,3784,3786,3788,3790,3792,3794,
        3797,3799,3801,3803,3805,3807,3810,3812,3814,3816,
        3818,3820,3822,3825,3827,3829,3831,3833,3835,3837,
        3840,3842,3844,3846,3848,3850,3852,3854,3857,3859,
        3861,3863,3865,3867,3869,3871,3873,3876,3878,3880,
        3882,3884,3886,3888,3890,3892,3895,3897,3899,3901,
        3903,3905,3907,3909,3911,3913,3916,3918,3920,3922,
        3924,3926,3928,3930,3932,3934,3936,3938,3941,3943,
        3945,3947,3949,3951,3953,3955,3957,3959,3961,3963,
        3965,3968,3970,3972,3974,3976,3978,3980,3982,3984,
        3986,3988,3990,3992,3994,3996,3998,4000,4002,4004,
        4007,4009,4011,4013,4015,4017,4019,4021,4023,4025,
        4027,4029,4031,4033,4035,4037,4039,4041,4043,4045,
        4047,4049,4051,4053,4055,4057,4059,4061,4063,4065,
        4067,4069,4071,4073,4075,4077,4079,4081,4083,4085,
        4087,4089,4091,4093,4096
        
};
/*lint -e569*/
//256/sin(i/10)
static const uint16 Reci_Sintable_temp[]={
    146677,146677,73338,48892,36669,29335,24446,20954,18335,16298,14668,13335,12223,11283,10477,9779,9168,8629,8150,
        7721,7335,6986,6668,6378,6113,5868,5643,5434,5240,5059,4891,4733,4586,4447,4316,4193,4077,3967,3862,3763,
        3669,3580,3495,3414,3336,3262,3192,3124,3059,2997,2937,2879,2824,2771,2720,2670,2623,2577,2533,2490,2449,
        2409,2370,2332,2296,2261,2227,2194,2162,2130,2100,2071,2042,2014,1987,1961,1935,1910,1886,1862,1839,1816,
        1794,1773,1752,1731,1711,1692,1673,1654,1636,1618,1601,1584,1567,1551,1535,1519,1504,1488,1474,1459,1445,
        1431,1418,1404,1391,1378,1366,1353,1341,1329,1317,1306,1295,1284,1273,1262,1251,1241,1231,1221,1211,1201,
        1192,1182,1173,1164,1155,1146,1138,1129,1121,1112,1104,1096,1088,1080,1073,1065,1058,1050,1043,1036,1029,
        1022,1015,1008,1002,995,989,982,976,970,964,957,951,946,940,934,928,923,917,912,906,901,896,890,885,880,
        875,870,865,860,856,851,846,842,837,832,828,824,819,815,811,806,802,798,794,790,786,782,778,774,770,766,
        763,759,755,752,748,744,741,737,734,730,727,724,720,717,714,711,707,704,701,698,695,692,689,686,683,680,
        677,674,671,668,666,663,660,657,655,652,649,647,644,642,639,636,634,631,629,626,624,622,619,617,614,612,
        610,608,605,603,601,599,596,594,592,590,588,586,583,581,579,577,575,573,571,569,567,565,563,561,560,558,
        556,554,552,550,548,547,545,543,541,539,538,536,534,533,531,529,528,526,524,523,521,519,518,516,515,513,
        512,510,508,507,505,504,502,501,499,498,497,495,494,492,491,489,488,487,485,484,483,481,480,479,477,476,
        475,473,472,471,470,468,467,466,465,463,462,461,460,458,457,456,455,454,453,451,450,449,448,447,446,445,
        444,443,441,440,439,438,437,436,435,434,433,432,431,430,429,428,427,426,425,424,423,422,421,420,419,418,
        417,416,415,414,413,413,412,411,410,409,408,407,406,405,405,404,403,402,401,400,399,399,398,397,396,395,
        394,394,393,392,391,390,390,389,388,387,387,386,385,384,384,383,382,381,381,380,379,378,378,377,376,376,
        375,374,373,373,372,371,371,370,369,369,368,367,367,366,365,365,364,363,363,362,362,361,360,360,359,358,
        358,357,357,356,355,355,354,354,353,352,352,351,351,350,350,349,348,348,347,347,346,346,345,345,344,343,
        343,342,342,341,341,340,340,339,339,338,338,337,337,336,336,335,335,334,334,333,333,332,332,331,331,330,
        330,329,329,328,328,328,327,327,326,326,325,325,324,324,323,323,323,322,322,321,321,320,320,320,319,319,
        318,318,318,317,317,316,316,316,315,315,314,314,314,313,313,312,312,312,311,311,311,310,310,309,309,309,
        308,308,308,307,307,306,306,306,305,305,305,304,304,304,303,303,303,302,302,302,301,301,301,300,300,300,
        299,299,299,298,298,298,298,297,297,297,296,296,296,295,295,295,295,294,294,294,293,293,293,292,292,292,
        292,291,291,291,291,290,290,290,289,289,289,289,288,288,288,288,287,287,287,287,286,286,286,286,285,285,
        285,285,284,284,284,284,283,283,283,283,282,282,282,282,282,281,281,281,281,280,280,280,280,280,279,279,
        279,279,278,278,278,278,278,277,277,277,277,277,276,276,276,276,276,275,275,275,275,275,274,274,274,274,
        274,274,273,273,273,273,273,272,272,272,272,272,272,271,271,271,271,271,271,270,270,270,270,270,270,269,
        269,269,269,269,269,269,268,268,268,268,268,268,267,267,267,267,267,267,267,266,266,266,266,266,266,266,
        266,265,265,265,265,265,265,265,265,264,264,264,264,264,264,264,264,263,263,263,263,263,263,263,263,263,
        262,262,262,262,262,262,262,262,262,262,261,261,261,261,261,261,261,261,261,261,260,260,260,260,260,260,
        260,260,260,260,260,260,259,259,259,259,259,259,259,259,259,259,259,259,259,258,258,258,258,258,258,258,
        258,258,258,258,258,258,258,258,258,257,257,257,257,257,257,257,257,257,257,257,257,257,257,257,257,257,
        257,257,257,257,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,
        256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,
        256,256,256
};
/*lint -e569*/

static const uint32 Asin_Table_Fix[]={
    0,572,1145,1718,2291,2864,3437,4010,4583,5156,5729,6303,6876,7449,8022,8595,9168,9742,10315,10888,11462,
        12035,12608,13182,13756,14329,14903,15477,16050,16624,17198,17772,18346,18920,19494,20069,20643,21217,
        21792,22367,22941,23516,24091,24666,25241,25816,26391,26966,27542,28117,28693,29269,29845,30421,30997,
        31573,32149,32726,33303,33879,34456,35033,35610,36188,36765,37343,37920,38498,39076,39654,40233,40811,
        41390,41968,42547,43127,43706,44285,44865,45445,46025,46605,47185,47766,48347,48927,49509,50090,50671,
        51253,51835,52417,52999,53582,54165,54748,55331,55914,56498,57082,57666,58250,58835,59420,60005,60590,
        61175,61761,62347,62934,63520,64107,64694,65281,65869,66457,67045,67633,68222,68811,69400,69990,70579,
        71169,71760,72350,72941,73533,74124,74716,75308,75901,76494,77087,77681,78274,78868,79463,80058,80653,
        81248,81844,82440,83037,83634,84231,84829,85427,86025,86624,87223,87822,88422,89022,89623,90224,90825,
        91427,92029,92632,93234,93838,94442,95046,95650,96255,96861,97467,98073,98680,99287,99895,100503,101111,
        101720,102330,102940,103550,104161,104772,105384,105996,106609,107222,107836,108450,109065,109680,110296,
        110912,111529,112146,112764,113382,114001,114621,115240,115861,116482,117104,117726,118348,118972,119595,
        120220,120845,121470,122097,122723,123351,123979,124607,125236,125866,126496,127127,127759,128391,129024,
        129658,130292,130927,131562,132198,132835,133473,134111,134750,135389,136030,136671,137312,137955,138598,
        139242,139886,140531,141177,141824,142472,143120,143769,144419,145069,145721,146373,147026,147679,148334,
        148989,149645,150302,150960,151619,152278,152938,153600,154262,154924,155588,156253,156918,157585,158252,
        158920,159590,160260,160931,161603,162276,162949,163624,164300,164977,165655,166333,167013,167694,168376,
        169058,169742,170427,171113,171800,172488,173177,173868,174559,175251,175945,176640,177335,178032,178730,
        179430,180130,180832,181534,182238,182944,183650,184358,185067,185777,186488,187201,187915,188630,189347,
        190065,190784,191504,192226,192950,193674,194401,195128,195857,196587,197319,198052,198787,199523,200261,
        201000,201741,202483,203227,203972,204719,205468,206218,206970,207723,208478,209235,209994,210754,211516,
        212280,213045,213812,214581,215352,216125,216899,217676,218454,219234,220016,220800,221586,222374,223164,
        223956,224750,225546,226345,227145,227948,228752,229559,230368,231179,231993,232809,233627,234448,235271,
        236096,236924,237754,238586,239421,240259,241099,241942,242788,243636,244487,245340,246196,247056,247917,
        248782,249650,250521,251394,252271,253150,254033,254919,255808,256700,257596,258495,259397,260303,261212,
        262124,263040,263960,264883,265810,266741,267676,268614,269557,270503,271453,272408,273367,274330,275297,
        276268,277244,278225,279210,280200,281194,282194,283198,284207,285222,286241,287266,288296,289332,290373,
        291420,292472,293531,294595,295666,296743,297826,298915,300012,301115,302225,303341,304466,305597,306736,
        307883,309037,310200,311371,312550,313738,314934,316140,317355,318580,319814,321058,322313,323578,324855,
        326142,327441,328752,330075,331411,332759,334122,335497,336888,338293,339713,341150,342603,344072,345560,
        347066,348591,350137,351703,353290,354901,356535,358194,359880,361592,363334,365106,366911,368749,370624,
        372537,374491,376490,378535,380631,382782,384993,387269,389615,392040,394552,397162,399881,402725,405715,
        408874,412235,415845,419769,424106,429027,434862,442462,460800
};
#if 0
IMGREF_POINT_T s_edge_table_fix[300]={0};
uint32 s_src_p_location_table[4000]={0};
int16 s_src_p_z_table[4000]={0};
uint16 s_src_shine_table[4000]={0};
uint16 Data_Num=0;
uint32 P_z_Num=0;
uint8 s_edge_alpha[4000]={0};
int16 s_half_chord_array[150]={0};
#else
uint8* s_sweets_alloc_mem_ptr = PNULL;
IMGREF_POINT_T *s_edge_table_fix = PNULL;
uint32 *s_src_p_location_table = PNULL;
//int16 *s_src_p_z_table = PNULL;
uint16 *s_src_shine_table = PNULL;
uint16 Data_Num=0;
//uint32 P_z_Num=0;
uint8 *s_edge_alpha = PNULL;
int16 s_half_chord_array[150]={0};
#endif
#ifdef DO_LIGHT
IMGREF_3D_POINT_T Light_Point = {550,-280,-1500,0};
IMGREF_3D_POINT_T Light_Point2 = {-400,800,-400,0};
IMGREF_3D_POINT_T Light_Point3 = {1000,600,-500,0};

#define LIGHT_DIST_3 20
//#define LIGHT_DIST_3_POW2 400
#define LIGHT_DIST_2 30
//#define LIGHT_DIST_2_POW2 900
#define LIGHT_DIST 15
//#define LIGHT_DIST_POW2 225
#define LIGHT_DIST_DELTA 5

#define LIGHT_TABLE_LEN 15


#define LIGHTCALC_DIST_DELTA 150
#define LIGHTCALC_DIST_MIN 1520
#define LIGHTCALC_ALPHA_DELTA 150
#define LIGHTCALC_ALPHA_MAX 255

const uint8 EarthAlphaTable[LIGHT_TABLE_LEN]=
{
    226,222,214,202,187,170,152,133,114,96,78,64,51,40,30
        //226,222,214,202,187,170,152,133,114,96,78,64,51,40,30,23,16,12,8,5,4,2,1,1,0,0
};

#endif

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
extern   int s2d_cos(int angle);
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void *MMIMENU_SWEETS_InitMenu(
                                   MMI_HANDLE_T ctrl_handle
                                   );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_DisplayIconBgLayer(
                                     MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                     );
/*****************************************************************************/
// 	Description :销毁
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_DestoryMenu(
                                      void *pm_data_ptr
                                      );

/*****************************************************************************/
//  Description : fullpaint显示
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MMIMENU_SWEETS_DisplayMenu(
                                      MMIMAINMENU_SWEETS_DATA_T     *menu_ctrl_ptr
                                      );
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void MMIMENU_SWEETS_DisplaySpecialMenu(
                                             MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                             );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMENU_SWEETS_HandleUpKey(
                                              MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                              );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMENU_SWEETS_HandleDownKey(
                                                MMIMAINMENU_SWEETS_DATA_T     *menu_ctrl_ptr
                                                );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_HandleLeftKey(
                                           MMIMAINMENU_SWEETS_DATA_T     *menu_ctrl_ptr
                                           );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_HandleRightKey(
                                            MMIMAINMENU_SWEETS_DATA_T     *menu_ctrl_ptr
                                            );


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_HandleNumKey(
                                          uint16 num_index,
                                          MMI_MESSAGE_ID_E msg_id,
                                          MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                          );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MMIMENU_SWEETS_HandleTpDown(
                                       DPARAM param,
                                       MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                       );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_HandleTpUp(
                                        DPARAM param,
                                        MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                        );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MMIMENU_SWEETS_HandleTpMove(
                                       DPARAM param,
                                       MMIMAINMENU_SWEETS_DATA_T*menu_ctrl_ptr
                                       );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_MoveInit(
                                      MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                      );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_MoveDestory(
                                         MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                         );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_HandleMenuOk(
                                          MMIMAINMENU_SWEETS_DATA_T     *menu_ctrl_ptr
                                          );


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_HandleMenuCancel(
                                          MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                          );


/*****************************************************************************/
//  Description : timer处理函数
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMENU_SWEETS_HandleTimer(
                                              MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,
                                              MMI_MESSAGE_ID_E msg_id,		//事件类型
                                              DPARAM           param          //附带的处理参数
                                              );
/*****************************************************************************/
//  Description : 处理LCD switch
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MMIMENU_SWEETS_HandleModifyRect(
                                           MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                           );

/*****************************************************************************/
//  Description : 控件消息处理函数
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMENU_SWEETS_HandleSpecialMsg(
                                                   MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr, 
                                                   MMI_MESSAGE_ID_E msg_id,
                                                   DPARAM               param
                                                   );


/*****************************************************************************/
// Description : 获取菜单总的item
// Global resource dependence : 
//  Author:
// Note:
/*****************************************************************************/
LOCAL uint16 MMIMENU_SWEETS_GetTotalItemNum(
                                            MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr,
                                            uint16 page_index
                                            );
/*****************************************************************************/
//  Description : 画一页的item
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SWEETS_DrawItemPageLayer(
                                       MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,
                                       uint16   page_index,
                                       GUI_LCD_DEV_INFO *lcd_dev_ptr
                                       );
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint16 SWEETS_GetPageItemNum(
                                   MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr,
                                   uint16   page_index
                                   );
/*****************************************************************************/
// Description : 
// Global resource dependence : 
//  Author:
// Note:
/*****************************************************************************/
PUBLIC void Glass_CandyType_Menu(SWEETS_INPARAM_T * inparam_data);

/*****************************************************************************/
//  Description : 释放三页数据
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_ReleaseItemData(void);

/*****************************************************************************/
//  Description : 获取当前状态
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL SWEETS_STATE_E SWEETS_GetAnimCurState(void);
/*****************************************************************************/
//  Description : 设置当前动画的状态
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_SetAnimCurState(SWEETS_STATE_E state);


LOCAL void Glass_Sweets_Get_Convert_Buffer(MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,
                                    uint32* icon_buffer,
                                    uint32 * out_32_buffer,
                                    IMGREF_POINT_T *icon_location,
                                    uint16 icon_num,
                                           uint32 * glass_color_ptr
                                           );
LOCAL void Glass_Sweets_Maginify_In(uint32 * out_buffer,
                                    uint32 * in_color_buffer,
                                    IMGREF_SIZE_T win_size,
                                    IMGREF_SIZE_T img_size,
                                    uint16 cur_frame,
                                    uint16 total_frame,
                                    FIX16_POINT_T *Point_Add_ptr
                                    );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_HandleFullPaintEnd(
                                     MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                     );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint16 SWEETS_GetMenuTotalPageNum(void);

/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL uint32* SWEETS_GetTextBufPtrByPageIndex(
                                              uint16        page_index
                                              );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SWEETS_HandleEnterAnim(
                                          MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr 
                                          );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SWEETS_Calculate(
                                    MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,
                                    uint16 offset,
                                    int16  angle,
                                    IN_POSITON_DIRECTION_E in_position
                                    );

/*****************************************************************************/
// 	Description : handle menu msg function
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMENU_SWEETS_Handle(
                                 void *pm_data_ptr,
                                 MMI_MESSAGE_ID_E   msg_id,  	//message
                                 DPARAM             param       //add data
                                 );


/*****************************************************************************/
//  Description : 获取菜单项的位置信息
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SWEETS_GetPageIdxRect(
                                     GUI_RECT_T *rect_ptr
                                     );


/*****************************************************************************/
//  Description : notify parent control or parent window notify message
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E NotifyParentMsg(
    CAF_HANDLE_T     ctrl_handle,
    MMI_MESSAGE_ID_E msg_id
);


/*****************************************************************************/
//  Description : Sweets Stop Redraw Timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SweetsStopRedrawTimer(
                                 MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : start timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SweetsStartRedrawTimer(
                                  MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                  );

/*****************************************************************************/
//  Description: Set State
//  Global resource dependence: 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetState(
                    uint32 *src_state_ptr, 
                    uint32 aim_state,
                    BOOLEAN is_set
                    );

/*****************************************************************************/
//  Description : get state
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetState(
                       uint32 dst_state,
                       uint32 src_state
                       );


/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_DestoryMenuInfo(
                                             MMIMAINMENU_SWEETS_DATA_T * menu_ctrl_ptr
                                             );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIMENU_SWEETS_InitMenuInfo(
                                       MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                       );
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Sweets Register
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_SweetsRegister(
CTRLMAINMENU_PM_APIS_T *apis
)
{
    if (PNULL == apis)
    {
        return FALSE;
    }

    apis->Initialize = MMIMENU_SWEETS_InitMenu;
    apis->Destory = MMIMENU_SWEETS_DestoryMenu;
    apis->Handle = MMIMENU_SWEETS_Handle;

    return TRUE;
}

/*****************************************************************************/
//  Description : 设置状态
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SetState(
                    uint32 *src_state_ptr, 
                    uint32 aim_state,
                    BOOLEAN is_set
                    )
{
    if (is_set)
    {
        *src_state_ptr |= aim_state;
    }
    else
    {
        *src_state_ptr &= ~aim_state;
    }

    return;
}

/*****************************************************************************/
//  Description : get state
//  Global resource dependence : 
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetState(
                       uint32 dst_state,
                       uint32 src_state
                       )
{
    return (BOOLEAN)( ( dst_state & src_state ) == src_state );
}

/*****************************************************************************/
// 	Description:
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMENU_SWEETS_Handle(
                                 void *pm_data_ptr,
                                 MMI_MESSAGE_ID_E   msg_id,  	//message
                                 DPARAM             param       //add data
                                 )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr = (MMIMAINMENU_SWEETS_DATA_T *)pm_data_ptr;

    if (PNULL == menu_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }


    switch (msg_id)
    {
    case MSG_CTL_OPEN:
		MMITHEME_DestroyMenuItemCache();
        SetState(&menu_ctrl_ptr->menu_state, GUIMENU_STATE_ENTER, TRUE);
        break;

    case MSG_CTL_PAINT:
        MMIMENU_SWEETS_DisplayMenu(menu_ctrl_ptr);
        if (menu_ctrl_ptr->enter_anim_type > GUIMENU_ENTER_ANIM_NONE && menu_ctrl_ptr->enter_anim_type < GUIMENU_ENTER_ANIM_MAX)
        {
#ifdef EFFECT_ZOOM_SUPPORT
            MMIDEFAULT_SetCurWin3DEffect(MMI_WIN_ANIM_ZOOM_IN);
#endif
        }
        break;


    case MSG_CTL_GET_FOCUS:
        // 初始值
        menu_ctrl_ptr->pre_msg_loop_page_index = (uint16)-1;
        menu_ctrl_ptr->pre_msg_loop_item_index = (uint16)-1;
        menu_ctrl_ptr->pre_msg_loop_menu_level = (uint16)-1;
		
		//初始化三页item层
#ifdef UI_MULTILAYER_SUPPORT
        if (UILAYER_IsMultiLayerEnable())
        {
            MMIMENU_SWEETS_HandleSpecialMsg(menu_ctrl_ptr, msg_id, param);
			if(!GetState(menu_ctrl_ptr->menu_state, GUIMENU_STATE_ENTER))
			{
				SWEETS_DisplayIconBgLayer(menu_ctrl_ptr);
				SWEETS_HandleFullPaintEnd(menu_ctrl_ptr);
			}

        }
#endif
        break;

        
    case MSG_CTL_GET_ACTIVE:
        menu_ctrl_ptr->is_active = TRUE;
        break;


    case MSG_CTL_LOSE_FOCUS:        
#ifdef UI_MULTILAYER_SUPPORT
        if (UILAYER_IsMultiLayerEnable())
        {
            MMIMENU_SWEETS_HandleSpecialMsg(menu_ctrl_ptr, msg_id, param);
        }
#endif
        break;


    case MSG_CTL_LOSE_ACTIVE:
    case MSG_CTL_CLOSE:
        if (MSG_CTL_LOSE_ACTIVE == msg_id)
        {
            menu_ctrl_ptr->is_active = FALSE;
        }    
        break;

        
    case MSG_APP_OK:
    case MSG_APP_WEB:    
        MMIMENU_SWEETS_HandleMenuOk(menu_ctrl_ptr);
        break;

    case MSG_NOTIFY_CANCEL:    
    case MSG_APP_CANCEL:
        if (MMK_TP_SLIDE_TP_SCROLL != menu_ctrl_ptr->slide_state && !menu_ctrl_ptr->is_moving)
        { 
            MMIMENU_SWEETS_HandleMenuCancel(menu_ctrl_ptr);
        }
        break;

    case MSG_APP_UP:
    case MSG_KEYREPEAT_UP:
        if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state && !menu_ctrl_ptr->is_move_state)
        {
            MMIMENU_SWEETS_HandleUpKey(menu_ctrl_ptr);
        }
        break;
        
    case MSG_APP_DOWN:
    case MSG_KEYREPEAT_DOWN:
        if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state && !menu_ctrl_ptr->is_move_state)
        {
            MMIMENU_SWEETS_HandleDownKey(menu_ctrl_ptr);
        }
        break;
        
    case MSG_APP_LEFT:
    case MSG_KEYREPEAT_LEFT:
        if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state && !menu_ctrl_ptr->is_move_state)
        {
            MMIMENU_SWEETS_HandleLeftKey(menu_ctrl_ptr);
        }
        break;
        
    case MSG_APP_RIGHT:
    case MSG_KEYREPEAT_RIGHT:
        if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state && !menu_ctrl_ptr->is_move_state)
        {
            MMIMENU_SWEETS_HandleRightKey(menu_ctrl_ptr);
        }
        break;


    case MSG_APP_1:
    case MSG_APP_2:
    case MSG_APP_3:
    case MSG_APP_4:
    case MSG_APP_5:
    case MSG_APP_6:
    case MSG_APP_7:
    case MSG_APP_8:
    case MSG_APP_9:
        if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state && !menu_ctrl_ptr->is_move_state)
        {
            MMIMENU_SWEETS_HandleNumKey(menu_ctrl_ptr->item_total_num, msg_id, menu_ctrl_ptr);
        }
        break;



#ifdef TOUCH_PANEL_SUPPORT
    case MSG_TP_PRESS_DOWN:
        MMIMENU_SWEETS_HandleTpDown(param, menu_ctrl_ptr);
        break;


    case MSG_TP_PRESS_UP:
        MMIMENU_SWEETS_HandleTpUp(param, menu_ctrl_ptr);
        break;


    case MSG_TP_PRESS_MOVE:
        MMIMENU_SWEETS_HandleTpMove(param, menu_ctrl_ptr);
        break;
#endif


    case MSG_TIMER:        
        MMIMENU_SWEETS_HandleTimer(menu_ctrl_ptr, msg_id, param);
        break;

		
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
   
    return (result);
}


/*****************************************************************************/
// 	Description : 销毁一个服务信息结点指针
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void SWEETS_DestroyMenuItemInfo(
                                      MMIMAINMENU_ITEM_INFO_T *item_info_ptr
                                      )
{
    if (PNULL != item_info_ptr)
    {
        SCI_FREE(item_info_ptr);
    }
    
    return;
}

/*****************************************************************************/
// 	Description : Free the struct of MMIMAINMENU_ITEM_INFO_T
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void SWEETS_DestroyMenuItemNodeInfo(
                                          MMI_LINK_NODE_T const * node_ptr
                                          )
{
    MMIMAINMENU_ITEM_INFO_T *item_info_ptr = PNULL;
    
    if (PNULL == node_ptr)
    {
        return;
    }

    item_info_ptr = (MMIMAINMENU_ITEM_INFO_T*)node_ptr->data;
    
    SWEETS_DestroyMenuItemInfo(item_info_ptr);
    
    return;
}
/*****************************************************************************/
// 	Description : 初始化链表
//	Global resource dependence : 
//  Author:
//	Note: 既可以是初始化,也可用于销毁
/*****************************************************************************/
LOCAL void SWEETS_ResetItemListInfo(
                                    MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                    )
{
    SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    
    if (PNULL != menu_ctrl_ptr->menu_list_info.list_head_ptr)
    {
        MMILINK_DestroyLink(menu_ctrl_ptr->menu_list_info.list_head_ptr, MMIMAINMENU_FreeItemNode);
    }
            
    menu_ctrl_ptr->menu_list_info.list_head_ptr = PNULL;
    menu_ctrl_ptr->menu_list_info.item_count = 0;
    menu_ctrl_ptr->start_item_index = 0;

    return;
}

/*****************************************************************************/
// 	Description : 创建一个服务信息结点指针
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL MMIMAINMENU_ITEM_INFO_T *SWEETS_CreateMenuItemNodeInfo(
                                                             const MMIMAINMENU_ITEM_INFO_T *item_info_ptr
                                                             )
{
    MMIMAINMENU_ITEM_INFO_T *new_item_info_ptr = PNULL;
    
    if (item_info_ptr != PNULL)
    {
        new_item_info_ptr = SCI_ALLOC_APP(sizeof(MMIMAINMENU_ITEM_INFO_T));
        if (PNULL != new_item_info_ptr)
        {
            new_item_info_ptr->menu_id = item_info_ptr->menu_id;
            new_item_info_ptr->group_id = item_info_ptr->group_id;
        }
    }
    
    return new_item_info_ptr;
}

/*****************************************************************************/
// 	Description : 增加一个Item
//	Global resource dependence : 
//  Author: 
//  RETRUN:  返回当前item的总数
//	Note:    
/*****************************************************************************/
LOCAL uint32 SWEETS_AddItem(
                            MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr,
                            const MMIMAINMENU_ITEM_INFO_T *item_info_ptr
                            )
{
    MMIMAINMENU_ITEM_INFO_T *new_item_info_ptr = PNULL;
    
    // 参数检查
    SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    
    // 创建一个服务信息节点,这里申请了内存
    new_item_info_ptr = SWEETS_CreateMenuItemNodeInfo(item_info_ptr);
    // 参数检查
    if (PNULL != new_item_info_ptr)
    {
        
        if (PNULL == menu_ctrl_ptr->menu_list_info.list_head_ptr)
        {
            menu_ctrl_ptr->menu_list_info.list_head_ptr = MMILINK_CreateHead((uint32)new_item_info_ptr);
        }
        else
        {
            MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode((uint32)new_item_info_ptr);
            MMILINK_AddNodeBeforeBaseNode(menu_ctrl_ptr->menu_list_info.list_head_ptr, tmp_node_ptr);
        }
        
        menu_ctrl_ptr->menu_list_info.item_count++;
    }
    
    return menu_ctrl_ptr->menu_list_info.item_count;
}

/*****************************************************************************/
// 	Description : get item
//	Global resource dependence : 
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
LOCAL void SWEETS_GetMenuItemByIndex(
                                     MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr, 
                                     uint16 item_index,
                                     CTRLMAINMENU_ITEM_T *item_ptr
                                     )
{
    MMIMAINMENU_ITEM_INFO_T item_info = {0};
    
    if(PNULL == menu_ctrl_ptr)
    {
        return;
    }
    
    MMIMAINMENU_GetItem(&menu_ctrl_ptr->menu_list_info, item_index, &item_info);

    MMIMAINMENU_GetItemData(item_info.menu_id, item_info.group_id, item_ptr);

    return;
}

#ifdef DYNAMIC_MAINMENU_SUPPORT
/*****************************************************************************/
//  Description : 重新加载主菜单
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_ReloadItemToMainMenu( MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr)
{
    if (PNULL != menu_ctrl_ptr)
    {
        // 释放
        MMIMENU_SWEETS_DestoryMenuInfo(menu_ctrl_ptr);    
        MMIMENU_SWEETS_InitMenuInfo(menu_ctrl_ptr);

		SetState(&menu_ctrl_ptr->menu_state, GUIMENU_STATE_ENTER, TRUE);
    }
    
    return;
}
#endif

/*****************************************************************************/
//  Description : 生成菜单数据
//  Global resource dependence :
//  Author:
//  Note:
//       item_list_ptr包括了静态数据&动态数据
/*****************************************************************************/
LOCAL BOOLEAN SWEETS_ReadMenuItemInfo(
                                      MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                      )
{
    uint32 item_index = 0;
    uint16 item_num = 0;
    MMIMAINMENU_ITEM_INFO_T new_item_info = {0};
    MMIMAINEMENU_ITEM_LIST_T *item_list_ptr = PNULL;
    MMI_LINK_NODE_T *node_ptr = PNULL;
    
    if(PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    item_list_ptr = MMIMAINMENU_GetItemList();
    node_ptr = item_list_ptr->list_head_ptr;

    item_num = MIN(item_list_ptr->item_count, MMIMENU_MAIN_MAX_NUM);
    menu_ctrl_ptr->menu_list_info.item_count = 0;
    
    for (item_index = 0; item_index < item_num; item_index++)
    {
        if(0 != ((MMIMAINMENU_ITEM_INFO_T *)node_ptr->data)->menu_id
            #ifdef GO_MENU_ONE_ICON_CHANGE_STYLE
        		&& ID_GOMENU_CHANGE_STYLE != ((MMIMAINMENU_ITEM_INFO_T *)node_ptr->data)->menu_id
            #endif
            #ifdef QBTHEME_SUPPORT
                && ID_MAINMENU_QBTHEMEAPP != ((MMIMAINMENU_ITEM_INFO_T*)node_ptr->data)->menu_id
			    && ID_MAINMENU_QBTHEMEMORE != ((MMIMAINMENU_ITEM_INFO_T*)node_ptr->data)->menu_id
            #endif
          )

        {
            new_item_info.menu_id = ((MMIMAINMENU_ITEM_INFO_T *)node_ptr->data)->menu_id;
            new_item_info.group_id = ((MMIMAINMENU_ITEM_INFO_T *)node_ptr->data)->group_id;
            
            SWEETS_AddItem(menu_ctrl_ptr, &new_item_info);
        }
        
        node_ptr = node_ptr->next_ptr;     
    }

    menu_ctrl_ptr->item_total_num = menu_ctrl_ptr->menu_list_info.item_count;
        
    return TRUE;
}
/*****************************************************************************/
//  Description : create bg layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_Adapt_IN_PARAM(
                                 MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,
                                 SWEETS_INPARAM_T *in_param,
                                 uint16 offset,
                                 int16  angle,
                                 IN_POSITON_DIRECTION_E in_position
                                 )
{
    UILAYER_INFO_T layer_info_ptr = {0};
    //uint16 *bg_ptr = PNULL;
    //uint32 temp_time = 0;
    SWEETS_STATE_E cur_state = SWEETS_STATE_NONE;
    uint16 next_page_index = 0;
    int i = 0;
    UILAYER_INFO_T shadow_info = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};


    if(menu_ctrl_ptr == PNULL||in_param== PNULL)
    {
        return;
    }
    

    GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);
    cur_state = SWEETS_GetAnimCurState();
    
    //各自关心的参数
    in_param->c_page_offset = offset;
    in_param->in_position = in_position;
    
    if(angle>SWEETS_MAX_SHAKE_ANGLE)
    {
        angle = SWEETS_MAX_SHAKE_ANGLE;
    }
    else if(angle<-SWEETS_MAX_SHAKE_ANGLE)
    {
        angle = -SWEETS_MAX_SHAKE_ANGLE;
    }
    
    in_param->angle = angle;
    
    for(i=0;i<SWEETS_INIT_PAGE_ITEM_NUM;i++)
    {
        if(s_sweets_menu_info.mem_page_info[i].index == 1)
        {
            in_param->current_page_icon_buf = (uint32*)UILAYER_GetLayerBufferPtr(&s_sweets_menu_info.mem_page_info[i].layer_handle);
        }
        
        if(IN_POSITON_LEFT == in_param->in_position)
        {
            if(s_sweets_menu_info.mem_page_info[i].index == 2)
            {
                in_param->next_page_icon_buf = (uint32*)UILAYER_GetLayerBufferPtr(&s_sweets_menu_info.mem_page_info[i].layer_handle);
            }
        }
        else if(IN_POSITON_RIGHT== in_param->in_position)
        {
            if(s_sweets_menu_info.mem_page_info[i].index == 0)
            {
                in_param->next_page_icon_buf = (uint32*)UILAYER_GetLayerBufferPtr(&s_sweets_menu_info.mem_page_info[i].layer_handle);
            }
        }
        else
        {
            if(s_sweets_menu_info.mem_page_info[i].index == 2)
            {
                in_param->next_page_icon_buf = (uint32*)UILAYER_GetLayerBufferPtr(&s_sweets_menu_info.mem_page_info[i].layer_handle);
            }
        }
    }
    
    if(IN_POSITON_LEFT == in_param->in_position)
    {
        if(menu_ctrl_ptr->cur_page_index == (s_sweets_menu_info.total_page_num-1))
        {
            next_page_index = 0;
        }
        else
        {
            next_page_index = menu_ctrl_ptr->cur_page_index + 1;
        }
        
    }
    else if(IN_POSITON_RIGHT== in_param->in_position)
    {
        if(menu_ctrl_ptr->cur_page_index == 0)
        {
            next_page_index = s_sweets_menu_info.total_page_num-1;
        }
        else
        {
            next_page_index = menu_ctrl_ptr->cur_page_index-1;
        }
    }
    UILAYER_GetLayerInfo(&s_sweets_menu_info.shadow_layer_handle,&shadow_info);
    in_param->shadow_buf = (uint32*)UILAYER_GetLayerBufferPtr(&s_sweets_menu_info.shadow_layer_handle);
    in_param->shadow_w = shadow_info.layer_width;
    in_param->shadow_h = shadow_info.layer_height;
    in_param->out_current_angle = &s_sweets_menu_info.cur_angle;
    in_param->cur_frame = s_sweets_menu_info.cur_frame;
    in_param->total_frame = s_sweets_menu_info.total_frame;
    in_param->out_icon_layer = (uint16*)UILAYER_GetLayerBufferPtr(&lcd_dev_info);
    in_param->out_shine_layer = (uint32*)UILAYER_GetLayerBufferPtr(&s_sweets_menu_info.dst_layer_handle);
    in_param->cur_icon_num = SWEETS_GetPageItemNum(menu_ctrl_ptr,menu_ctrl_ptr->cur_page_index);
    in_param->next_icon_num = SWEETS_GetPageItemNum(menu_ctrl_ptr,next_page_index);
    in_param->icon_w = SWEETS_ICON_TEXT_WIDTH;
    in_param->icon_h = SWEETS_ICON_TEXT_HEIGHT;
    UILAYER_GetLayerInfo(&s_sweets_menu_info.dst_layer_handle,&layer_info_ptr);
    in_param->dst_width = layer_info_ptr.layer_width;
    in_param->dst_height = layer_info_ptr.layer_height;
    
    in_param->icon_location = s_sweets_menu_info.icon_location;
    in_param->cur_state = s_sweets_menu_info.cur_state;
    
    in_param->cur_text_buf = SWEETS_GetTextBufPtrByPageIndex(menu_ctrl_ptr->cur_page_index);
    in_param->next_text_buf = SWEETS_GetTextBufPtrByPageIndex(next_page_index);
    
    in_param->text_width = SWEETS_MENUTEXT_WIDTH;
    in_param->text_height = SWEETS_MENUTEXT_HEIGHT;
    
    //glass
    in_param->glass_color_ptr = (uint32*)UILAYER_GetLayerBufferPtr(&s_sweets_menu_info.glass_layer_handle);
    
    UILAYER_Clear(&s_sweets_menu_info.dst_layer_handle);
    
}
/*****************************************************************************/
//  Description : create bg layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_DisplayBgToLayer(
                                   MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,
                                   GUI_LCD_DEV_INFO *lcd_dev_ptr
                                   )
{
    //GUI_BG_T            common_bg = {0};
    MMI_IMAGE_ID_T      bg_img_id = menu_ctrl_ptr->main_menu_theme.bg_image;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    GUI_RECT_T          bg_rect = {0};
    GUI_RECT_T          menu_rect = {0};
    
    if(PNULL == menu_ctrl_ptr)
    {
        return;
    }

    GUIAPICTRL_GetRect(menu_ctrl_ptr->ctrl_handle, &menu_rect);
    bg_rect.bottom = menu_rect.bottom;
    bg_rect.right = menu_rect.right;

    if (PNULL == lcd_dev_ptr)
    {
        CTRLMAINMENU_GetlayerInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);
    }
    else
    {
        lcd_dev_info = *lcd_dev_ptr;
    }

#ifdef BG_ON_BOTTOM_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
    if (UILAYER_IsMultiLayerEnable())
    {
        if (MMITHEME_GetBgImgId() == bg_img_id)
        {
            //通常背景
            MMITHEME_DisplayBg(&bg_rect, menu_ctrl_ptr->win_handle, &lcd_dev_info);
        }
        else if (0 != bg_img_id)
        {
            GUIRES_DisplayImg(PNULL,
                              &bg_rect,
                              &bg_rect,
                              menu_ctrl_ptr->win_handle,
                              bg_img_id,
                              (const GUI_LCD_DEV_INFO *)&lcd_dev_info);
        }
        else
        {
            GUI_FillRect(&lcd_dev_info,
                         bg_rect,
                         menu_ctrl_ptr->main_menu_theme.bg_color);
        }
    }
    else
#endif
#endif
    {
        if (0 != bg_img_id)
        {
            GUIRES_DisplayImg(PNULL,
                              &bg_rect,
                              &bg_rect,
                              menu_ctrl_ptr->win_handle,
                              bg_img_id,
                              (const GUI_LCD_DEV_INFO *)&lcd_dev_info);
        }
        else
        {
            GUI_FillRect(&lcd_dev_info,
                         bg_rect,
                         menu_ctrl_ptr->main_menu_theme.bg_color);
        }
    }
}


/*****************************************************************************/
//  Description : create dst layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_CreatDstLayer(
                                MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                )
{   
    uint16              layer_width = 0;
    uint16              layer_height = 0;
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    
    if(PNULL == menu_ctrl_ptr)
    {
        return;
    }

    GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);
    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &layer_width, &layer_height);
    
    s_sweets_menu_info.dst_layer_handle.lcd_id = GUI_MAIN_LCD_ID;
    s_sweets_menu_info.dst_layer_handle.block_id = UILAYER_NULL_HANDLE;
    
    // 创建层
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = layer_width;
    create_info.height = layer_height;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE; 
    create_info.format = UILAYER_MEM_DOUBLE_COPY;
    UILAYER_CreateLayer(
        &create_info,  
        &s_sweets_menu_info.dst_layer_handle
        );
    // 设置color key
    UILAYER_SetLayerColorKey(&s_sweets_menu_info.dst_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
    
    UILAYER_SetPreMultiplyAlpha(&s_sweets_menu_info.dst_layer_handle,TRUE,TRUE);
    
    // 使用color清除层
    UILAYER_Clear(&s_sweets_menu_info.dst_layer_handle);
    
    return;
}


/*****************************************************************************/
//  Description : release dst layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_ReleaseDstLayer(void)
{
    if (UILAYER_IsMultiLayerEnable())    
    {
        UILAYER_RELEASELAYER(&s_sweets_menu_info.dst_layer_handle);
    }
}


/*****************************************************************************/
//  Description : create  layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_CreatEnterAnimLayer(
                                      MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                      )
{   
    uint16              layer_width = 0;
    uint16              layer_height = 0;
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    
    if(PNULL == menu_ctrl_ptr)
    {
        return;
    }
    
    //全屏
    GUIAPICTRL_GetLcdDevInfo(menu_ctrl_ptr->ctrl_handle, &lcd_dev_info);
    GUILCD_GetLogicWidthHeight(lcd_dev_info.lcd_id, &layer_width, &layer_height);
    
    s_sweets_menu_info.enter_anim_temp_layer_handle.lcd_id = GUI_MAIN_LCD_ID;
    s_sweets_menu_info.enter_anim_temp_layer_handle.block_id = UILAYER_NULL_HANDLE;
    
    // 创建层
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = layer_width;
    create_info.height = layer_height;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE; 
    UILAYER_CreateLayer(
        &create_info,  
        &s_sweets_menu_info.enter_anim_temp_layer_handle
        );
    // 设置color key
    UILAYER_SetLayerColorKey(&s_sweets_menu_info.enter_anim_temp_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
    UILAYER_SetPreMultiplyAlpha(&s_sweets_menu_info.enter_anim_temp_layer_handle,TRUE,TRUE);
    // 使用color清除层
    UILAYER_Clear(&s_sweets_menu_info.enter_anim_temp_layer_handle);
    
    return;
}


/*****************************************************************************/
//  Description : release dst layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_ReleaseEnterAnimLayer(
                                        MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                        )
{
    if(PNULL == menu_ctrl_ptr)
    {
        return;
    }
    
    if (UILAYER_IsMultiLayerEnable())    
    {
        UILAYER_RELEASELAYER(&s_sweets_menu_info.enter_anim_temp_layer_handle);/*lint !e506 !e774*/
    }
}


/*****************************************************************************/
//  Description : create ICON BG layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_CreatIconBgLayer(
                                   MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                   )
{   
    //uint16              layer_width = 0;
    //uint16              layer_height = 0;
    UILAYER_CREATE_T    create_info = {0};
    
    if(PNULL == menu_ctrl_ptr)
    {
        return;
    }
    
    s_sweets_menu_info.icon_bg_layer_handle.lcd_id = GUI_MAIN_LCD_ID;
    s_sweets_menu_info.icon_bg_layer_handle.block_id = UILAYER_NULL_HANDLE;
    
    // 创建层
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = SWEETS_ICON_TEXT_WIDTH;
    create_info.height = SWEETS_ICON_TEXT_HEIGHT;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE; 
    UILAYER_CreateLayer(
        &create_info,  
        &s_sweets_menu_info.icon_bg_layer_handle
        );
    // 设置color key
    UILAYER_SetLayerColorKey(&s_sweets_menu_info.icon_bg_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
    // 使用color清除层
    UILAYER_Clear(&s_sweets_menu_info.icon_bg_layer_handle);
    
    return;
}


/*****************************************************************************/
//  Description : release ICON BG layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_ReleaseIconBgLayer(void)
{
    
    if (UILAYER_IsMultiLayerEnable())    
    {
        UILAYER_RELEASELAYER(&s_sweets_menu_info.icon_bg_layer_handle);/*lint !e506 !e774*/
    }
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_DisplayIconBgLayer(
                                     MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                     )
{
    GUI_POINT_T display_point = {0,0};
	
    UILAYER_SetPreMultiplyAlpha(&s_sweets_menu_info.icon_bg_layer_handle,FALSE,FALSE);
    UILAYER_Clear(&s_sweets_menu_info.icon_bg_layer_handle);
    GUIRES_DisplayImg(&display_point, 
        PNULL, 
        PNULL, 
        menu_ctrl_ptr->win_handle, 
        IMAGE_SWEETS_ICON_BG, 
        &s_sweets_menu_info.icon_bg_layer_handle);
    UILAYER_SetPreMultiplyAlpha(&s_sweets_menu_info.icon_bg_layer_handle,TRUE,TRUE);
}


/*****************************************************************************/
//  Description : create ICON BG layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_CreatShadowLayer(
                                   MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                   )
{   
    UILAYER_CREATE_T    create_info = {0};
    
    if(PNULL == menu_ctrl_ptr)
    {
        return;
    }
    
    s_sweets_menu_info.shadow_layer_handle.lcd_id = GUI_MAIN_LCD_ID;
    s_sweets_menu_info.shadow_layer_handle.block_id = UILAYER_NULL_HANDLE;
    
    // 创建层
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    GUIRES_GetImgWidthHeight((uint16*)&create_info.width, (uint16*)&create_info.height, IMAGE_SWEETS_SHADOW,menu_ctrl_ptr->win_handle);
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE; 
    UILAYER_CreateLayer(
        &create_info,  
        &s_sweets_menu_info.shadow_layer_handle
        );
    // 设置color key
    UILAYER_SetLayerColorKey(&s_sweets_menu_info.shadow_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
    // 使用color清除层
    UILAYER_Clear(&s_sweets_menu_info.shadow_layer_handle);
    
    return;
}


/*****************************************************************************/
//  Description : release ICON BG layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_ReleaseShadowLayer(void)
{
    
    if (UILAYER_IsMultiLayerEnable())    
    {
        UILAYER_RELEASELAYER(&s_sweets_menu_info.shadow_layer_handle);/*lint !e506 !e774*/
    }
}


/*****************************************************************************/
//  Description: Draw Shadow of menu item
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_DisplayShadowLayer(
                                     MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                     )
{
    GUI_POINT_T display_point = {0,0};
    
    UILAYER_SetPreMultiplyAlpha(&s_sweets_menu_info.shadow_layer_handle,FALSE,FALSE);
    UILAYER_Clear(&s_sweets_menu_info.shadow_layer_handle);
    GUIRES_DisplayImg(&display_point, 
        PNULL, 
        PNULL, 
        menu_ctrl_ptr->win_handle, 
                        IMAGE_SWEETS_SHADOW, 
        &s_sweets_menu_info.shadow_layer_handle);
    
    UILAYER_SetPreMultiplyAlpha(&s_sweets_menu_info.shadow_layer_handle,TRUE,TRUE);
}


/*****************************************************************************/
//  Description : create ICON BG layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_CreatOneIconLayer(
                                    MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,
                                    GUI_LCD_DEV_INFO *lcd_dev_ptr,
                                    uint16 width,
                                    uint16 height
                                    )
{   
    UILAYER_CREATE_T    create_info = {0};
    
    if(PNULL == menu_ctrl_ptr)
    {
        return;
    }
    
    lcd_dev_ptr->lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_ptr->block_id = UILAYER_NULL_HANDLE;
    
    // 创建层
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = width;
    create_info.height = height;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE; 
    UILAYER_CreateLayer(
        &create_info,  
        lcd_dev_ptr
        );
    // 设置color key
    UILAYER_SetLayerColorKey(lcd_dev_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);
    // 使用color清除层
    UILAYER_Clear(lcd_dev_ptr);
    
    return;
}


/*****************************************************************************/
//  Description : release ICON BG layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_ReleaseOneIconLayer(
                                      MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,
                                      GUI_LCD_DEV_INFO *lcd_dev_ptr
                                      )
{
    if(PNULL == menu_ctrl_ptr)
    {
        return;
    }
    
    if (UILAYER_IsMultiLayerEnable())    
    {
        UILAYER_RELEASELAYER(lcd_dev_ptr);/*lint !e506 !e774*/
    }
}


/*****************************************************************************/
//  Description : create ICON BG layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_CreatIconShadowLayer(
                                       MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,
                                       GUI_LCD_DEV_INFO *lcd_dev_ptr
                                       )
{   
    UILAYER_CREATE_T    create_info = {0};
    
    if(PNULL == menu_ctrl_ptr)
    {
        return;
    }
    
    lcd_dev_ptr->lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_ptr->block_id = UILAYER_NULL_HANDLE;
    
    // 创建层
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = SWEETS_SHADOW_WIDTH;
    create_info.height = SWEETS_SHADOW_HEIGHT;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE; 
    UILAYER_CreateLayer(
        &create_info,  
        lcd_dev_ptr
        );
    // 设置color key
    UILAYER_SetLayerColorKey(lcd_dev_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);
    // 使用color清除层
    UILAYER_Clear(lcd_dev_ptr);
    
    return;
}


/*****************************************************************************/
//  Description : release ICON BG layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_ReleaseIconShadowLayer(
                                         MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,
                                         GUI_LCD_DEV_INFO *lcd_dev_ptr
                                         )
{
    if(PNULL == menu_ctrl_ptr)
    {
        return;
    }
    
    if (UILAYER_IsMultiLayerEnable())    
    {
        UILAYER_RELEASELAYER(lcd_dev_ptr);/*lint !e506 !e774*/
    }
}


/*****************************************************************************/
//  Description : create TEXT
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_CreatTextLayer(
                                 MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                 )
{   
    UILAYER_CREATE_T    create_info = {0};
    
    if(PNULL == menu_ctrl_ptr)
    {
        return;
    }
    
    s_sweets_menu_info.text_layer_handle.lcd_id = GUI_MAIN_LCD_ID;
    s_sweets_menu_info.text_layer_handle.block_id = UILAYER_NULL_HANDLE;
    
    // 创建层
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = SWEETS_MENUTEXT_WIDTH;
    create_info.height = SWEETS_MENUTEXT_HEIGHT*menu_ctrl_ptr->item_total_num;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE; 
    UILAYER_CreateLayer(
        &create_info,  
        &s_sweets_menu_info.text_layer_handle
        );
    // 设置color key
    UILAYER_SetLayerColorKey(&s_sweets_menu_info.text_layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);
    // 使用color清除层
    UILAYER_Clear(&s_sweets_menu_info.text_layer_handle);
    
    return;
}


/*****************************************************************************/
//  Description : release ICON BG layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_ReleaseTextLayer(void)
{
    if (UILAYER_IsMultiLayerEnable())    
    {
        UILAYER_RELEASELAYER(&s_sweets_menu_info.text_layer_handle);/*lint !e506 !e774*/
    }
}


/*****************************************************************************/
//  Description : create TEXT
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_DisplayOneTextToLayer(
                                        MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,
                                        GUI_RECT_T text_rect,
                                        GUI_LCD_DEV_INFO *lcd_dev_ptr,
                                        uint16 item_index
                                        )
{
    CTRLMAINMENU_ITEM_T item_info  = {0}; 
    MMI_STRING_T        item_str = {0};
    GUISTR_STYLE_T      str_style = {0};
    GUISTR_STATE_T      str_state = GUISTR_STATE_ELLIPSIS | GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE;
    GUI_COLOR_T         item_color = 0;

    
    if (PNULL == menu_ctrl_ptr || PNULL == lcd_dev_ptr)
    {
        return;
    }

    SWEETS_GetMenuItemByIndex(menu_ctrl_ptr, item_index, &item_info);
        
    //judge is grayed
    if (MMIAPICOM_IsGrayed(menu_ctrl_ptr->win_handle, item_info.group_id, item_info.menu_id))
    {
        item_color = MMI_DARK_GRAY_COLOR;
    }
    else
    {
        item_color = MMI_WHITE_COLOR;
    }

    if(CTRL_TEXT_ID == item_info.text.type)
    {
        MMI_GetLabelTextByLang(item_info.text.data.text_id, &item_str);	
    }
    else
    {
		item_str.wstr_len = item_info.text.data.str_info.wstr_len;
		item_str.wstr_ptr = item_info.text.data.str_info.wstr_ptr;
    }

    str_style.align = ALIGN_HMIDDLE;
    str_style.font = menu_ctrl_ptr->main_menu_theme.item_font;
    str_style.font_color = item_color;
    
    GUISTR_DrawTextToLCDInRect(lcd_dev_ptr,
        (const GUI_RECT_T*)&text_rect,
        (const GUI_RECT_T*)&text_rect,
        (const MMI_STRING_T*)&item_str, 
        &str_style, 
        str_state,
        GUISTR_TEXT_DIR_AUTO);

    return;   
}


/*****************************************************************************/
//  Description : Draw item text
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_DisplayTextLayer(
                                   MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                   )
{
    GUI_RECT_T          text_rect = {0};
    int i = 0;
    
    UILAYER_SetPreMultiplyAlpha(&s_sweets_menu_info.text_layer_handle,FALSE,FALSE);
    UILAYER_Clear(&s_sweets_menu_info.text_layer_handle);
    
    text_rect.left = 0;
    text_rect.top = 0;
    text_rect.right = text_rect.left + SWEETS_MENUTEXT_WIDTH;
    text_rect.bottom = text_rect.top + SWEETS_MENUTEXT_HEIGHT;
    for(i=0;i<menu_ctrl_ptr->item_total_num;i++)
    {
        SWEETS_DisplayOneTextToLayer(menu_ctrl_ptr,text_rect,&s_sweets_menu_info.text_layer_handle,i);
        
        text_rect.top += SWEETS_MENUTEXT_HEIGHT;
        text_rect.bottom = text_rect.top + SWEETS_MENUTEXT_HEIGHT;
    }
    UILAYER_SetPreMultiplyAlpha(&s_sweets_menu_info.text_layer_handle,TRUE,TRUE);
}


/*****************************************************************************/
//  Description : 获取菜单项的位置信息
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SWEETS_GetPageIdxRect(
                                     GUI_RECT_T *rect_ptr
                                     )
{
    GUI_RECT_T rect = SWEETS_TITLE_ICON_RECT_V;
    GUI_RECT_T h_rect = SWEETS_TITLE_ICON_RECT_H;
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        rect = h_rect;
    }
    
    if (PNULL != rect_ptr)
    {
        *rect_ptr = rect;
        
        return GUI_CheckRectValid(rect);
    }
    else
    {
        return FALSE;
    }
}


/*****************************************************************************/
//  Description : 获取菜单项的位置信息
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T SWEETS_GetIdxRect_With_index(
                                               uint16 index
                                               )
{
    uint16  total_page_num = 0;
    GUI_RECT_T rect = {0};
    GUI_RECT_T index_rect = {0};
    uint16  total_len = 0;
    uint16  one_idx_len = 0;
    
    total_page_num = SWEETS_GetMenuTotalPageNum();
    
    if(0==total_page_num)
    {
        return index_rect;
    }
    
    SWEETS_GetPageIdxRect(&rect);
#if 0
    if(MMITHEME_IsMainScreenLandscape())
    {
        total_len = rect.bottom-rect.top + 1;
        one_idx_len = total_len/total_page_num;
        
        index_rect.left = rect.left;
        index_rect.right = rect.left;
        index_rect.top = rect.top+ index*one_idx_len;
        index_rect.bottom = index_rect.top  + one_idx_len;
    }
    else
#endif
    {
        total_len = rect.right-rect.left + 1;
        one_idx_len = total_len/total_page_num;
        
        index_rect.left = rect.left+ index*one_idx_len;
        index_rect.right = index_rect.left + one_idx_len;
        index_rect.top = rect.top;
        index_rect.bottom = rect.bottom;
    }
    
    
    return index_rect;
}


/*****************************************************************************/
//  Description : create page idx layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_CreatePageIdxLayer(
                                     MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr  //in
                                     )
{   
    GUI_RECT_T          icon_rect = {0};
    UILAYER_CREATE_T    create_info = {0};
    
    if (SWEETS_GetPageIdxRect(&icon_rect))   
    {
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
        create_info.offset_x = icon_rect.left;
        create_info.offset_y = icon_rect.top;
        create_info.width = (icon_rect.right + 1 - icon_rect.left);
        create_info.height = (icon_rect.bottom + 1 - icon_rect.top);
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE;
        create_info.format = UILAYER_MEM_DOUBLE_COPY;
        
        UILAYER_CreateLayer(
            &create_info,  
            &(menu_ctrl_ptr->titleidx_layer_handle)
            );
        
        UILAYER_SetLayerColorKey(&(menu_ctrl_ptr->titleidx_layer_handle), TRUE, UILAYER_TRANSPARENT_COLOR);
        UILAYER_Clear(&(menu_ctrl_ptr->titleidx_layer_handle));
    }
}


/*****************************************************************************/
//  Description : release move layer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_ReleasePageIdxLayer(
                                      MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr  //in
                                      )
{
    // 释放层
    if (UILAYER_IsMultiLayerEnable())    
    {
        UILAYER_RELEASELAYER(&menu_ctrl_ptr->titleidx_layer_handle);   /*lint !e506 !e774*/
    }
    
    return;
}


/*****************************************************************************/
//  Description: 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void SWEETS_DisplayPageIdxLayer(
                                       MMIMAINMENU_SWEETS_DATA_T     *menu_ctrl_ptr,
                                       uint16             cur_page_index,
                                       GUI_LCD_DEV_INFO lcd_dev_info
                                       )
{
    uint16          i = 0;
    uint16          image_width = 0;
    uint16          image_height = 0;
    GUI_RECT_T      image_rect = {0};
    GUI_RECT_T      title_rect = {0};
    GUI_RECT_T      disp_rect = {0};
    MMI_IMAGE_ID_T  title_img_id = IMAGE_NULL;
    uint16  total_page_num = 0;
    
    SCI_ASSERT(PNULL != menu_ctrl_ptr); /*assert verified*/
    
    menu_ctrl_ptr->is_display_title = TRUE; // 显示title
    
    IMG_EnableTransparentColor(TRUE);
    
    UILAYER_SetLayerColorKey(&lcd_dev_info, TRUE, UILAYER_TRANSPARENT_COLOR);
    
    UILAYER_Clear(&lcd_dev_info);
    
    SWEETS_GetPageIdxRect(&title_rect);
#if 0
    //display title image
    GUIRES_DisplayImg(PNULL,
        &title_rect,
        PNULL,
        menu_ctrl_ptr->win_handle,
        IMAGE_SLIDE_MAINMENU_TITLE_BG,
        &lcd_dev_info);
#endif
    total_page_num = SWEETS_GetMenuTotalPageNum();
    
    for (i = 0; i < total_page_num; i++)
    {
        disp_rect = SWEETS_GetIdxRect_With_index(i);
        
        if (i == cur_page_index)
        {
            title_img_id = IMAGE_SWEETS_CHOOSE_ON;
        }
        else
        {
            title_img_id = IMAGE_SWEETS_CHOOSE_OFF;
        }
        
        
        GUIRES_GetImgWidthHeight(&image_width, &image_height, title_img_id, menu_ctrl_ptr->win_handle);
        
        disp_rect = GUI_GetCenterRectExt(disp_rect, image_width, image_height);
        image_rect = GUI_CreateRect(0, 0, image_width - 1, image_height - 1);
        
        //display title image
        GUIRES_DisplayImg(PNULL,
            &disp_rect,
            &image_rect,
            menu_ctrl_ptr->win_handle,
            title_img_id,
            &lcd_dev_info);
    }
    
    IMG_EnableTransparentColor(FALSE);
    
    return;
}


/*****************************************************************************/
//  Description : create page idx layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_CreateGlassLayer(
                                   MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr  //in
                                   )
{   
    UILAYER_CREATE_T    create_info = {0};
    
    
    s_sweets_menu_info.glass_layer_handle.lcd_id = GUI_MAIN_LCD_ID;
    s_sweets_menu_info.glass_layer_handle.block_id = UILAYER_NULL_HANDLE;
    
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    GUIRES_GetImgWidthHeight((uint16*)&create_info.width, (uint16*)&create_info.height,IMAGE_SWEETS_GLASS,menu_ctrl_ptr->win_handle);
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    
    UILAYER_CreateLayer(
        &create_info,  
        &(s_sweets_menu_info.glass_layer_handle)
        );
    
    UILAYER_SetLayerColorKey(&(s_sweets_menu_info.glass_layer_handle), TRUE, UILAYER_TRANSPARENT_COLOR);
    UILAYER_Clear(&(s_sweets_menu_info.glass_layer_handle));
    
}


/*****************************************************************************/
//  Description : release move layer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_ReleaseGlassLayer(void)
{
    // 释放层
    if (UILAYER_IsMultiLayerEnable())    
    {
        UILAYER_RELEASELAYER(&s_sweets_menu_info.glass_layer_handle);   /*lint !e506 !e774*/
    }
    
    return;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_DisplayGlassLayer(
                                    MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                    )
{
    GUI_POINT_T display_point = {0,0};
    UILAYER_SetPreMultiplyAlpha(&s_sweets_menu_info.glass_layer_handle,FALSE,FALSE);
    UILAYER_Clear(&s_sweets_menu_info.glass_layer_handle);
    
    GUIRES_DisplayImg(&display_point, 
        PNULL, 
        PNULL, 
        menu_ctrl_ptr->win_handle, 
        IMAGE_SWEETS_GLASS, 
        &s_sweets_menu_info.glass_layer_handle);

    UILAYER_SetPreMultiplyAlpha(&s_sweets_menu_info.glass_layer_handle,TRUE,TRUE);

}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint16 SWEETS_GetPageItemMaxNum(void)
{
    return SWEETS_PAGE_ICON_NUM;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint16 SWEETS_GetMenuTotalPageNum(void)
{
    return s_sweets_menu_info.total_page_num;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL SWEETS_STATE_E SWEETS_GetAnimCurState(void)
{
    return s_sweets_menu_info.cur_state;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL int16 SWEETS_GetAnimCurAngle(void)
{
    return s_sweets_menu_info.cur_angle;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:默认传进来的都是正的
/*****************************************************************************/
LOCAL void SWEETS_SetAnimAimAngle(int16 aim_angle)
{
    if(aim_angle>SWEETS_MAX_SHAKE_ANGLE)
    {
        aim_angle = SWEETS_MAX_SHAKE_ANGLE;
    }
    else if(aim_angle<-SWEETS_MAX_SHAKE_ANGLE)
    {
        aim_angle = -SWEETS_MAX_SHAKE_ANGLE;
    }
       
    s_sweets_menu_info.aim_angle = abs(aim_angle);   
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL int16 SWEETS_GetAnimAimAngle(void)
{
    return s_sweets_menu_info.aim_angle;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_SetAnimCurAngle(int16 cur_angle)
{
    if(cur_angle>SWEETS_MAX_SHAKE_ANGLE)
    {
        cur_angle = SWEETS_MAX_SHAKE_ANGLE;
    }
    else if(cur_angle<-SWEETS_MAX_SHAKE_ANGLE)
    {
        cur_angle = -SWEETS_MAX_SHAKE_ANGLE;
    }
    
    s_sweets_menu_info.cur_angle = cur_angle;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_SetAnimCurState(SWEETS_STATE_E state)
{
    s_sweets_menu_info.cur_state = state;
    
    if(state == SWEETS_STATE_NONE)
    {
        s_sweets_menu_info.cur_frame = 0;
    }
    else if(state == SWEETS_STATE_ENTER_ANIM)
    {
        s_sweets_menu_info.total_frame = SWEETS_ENTER_ANIM_FRAME;
        s_sweets_menu_info.cur_frame = 0;
    }
    else if(state == SWEETS_STATE_SHAKE)
    {
        s_sweets_menu_info.total_frame = SWEETS_SHAKE_FRAME;
        s_sweets_menu_info.cur_frame = 0;
    }
    else if(state == SWEETS_STATE_ROTATION_MOVE)
    {
        //设置需要变化到的目标角度
        SWEETS_SetAnimAimAngle(SWEETS_MOVE_MAX_ANGLE);
    }   
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_SetChangeState(BOOLEAN is_change)
{
    s_sweets_menu_info.is_in_change_state = is_change;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SWEETS_GetChangeState(void)
{
    return s_sweets_menu_info.is_in_change_state;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint16 SWEETS_GetPageItemNum(
                                   MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr,
                                   uint16   page_index
                                   )
{
    uint16 total_page_num = 0;
    uint16 item_num = 0;
    uint16 max_page_item_num = 0;
    
    total_page_num = SWEETS_GetMenuTotalPageNum();
    max_page_item_num = SWEETS_GetPageItemMaxNum();
    
    if(0 == max_page_item_num)
    {
        SCI_TRACE_LOW("[SWEETS] item num = 0 ,PAGE_INDEX = %d",page_index);
        return 0;
    }
    
    if(page_index == (total_page_num-1))//最后一页
    {
        item_num = menu_ctrl_ptr->item_total_num%max_page_item_num;
    }
    else if(page_index<(total_page_num-1))
    {
        item_num = max_page_item_num;
    }
    else
    {
        SCI_TRACE_LOW("[SWEETS] page_index = %d",page_index);
    }
    
    return item_num;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint16 SWEETS_GetPageItemIndex(
                                     uint16   page_index,
                                     uint16   i
                                     )
{
    uint16 item_index = 0;
    uint16 max_page_item_num = 0;
    uint16 total_page_num = 0;
    
    total_page_num = SWEETS_GetMenuTotalPageNum();
    
    if(page_index>=total_page_num)
    {
        return item_index;
    }
    
    max_page_item_num = SWEETS_GetPageItemMaxNum();
    
    item_index = page_index*max_page_item_num + i;
    
    return item_index;
}


/*****************************************************************************/
//  Description : 画一个item到指定LAYER的指定位置
//  Global resource dependence : 
//  Author: 
//  Note:item_index是相对于总的来说的，不分page页
/*****************************************************************************/
LOCAL BOOLEAN SWEETS_DrawOneItemToLayer(
                                        MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,
                                        GUI_LCD_DEV_INFO *lcd_dev_ptr,//out
                                        uint16*    out_ptr,
                                        uint16        item_index,
                                        GUI_LCD_DEV_INFO* icon_shadow_ptr,
                                        GUI_RECT_T dis_rect
                                        )
{
    CTRLMAINMENU_ITEM_T  item_info  = {0}; 
    GUI_RECT_T          icon_rect = {0};
    MMI_IMAGE_ID_T         img_id = 0;
    uint16 max_page_item_num = 0;
    
    COLOR16* pBgRes = PNULL;
    IMGREF_SIZE_T BgRes_window = {0};
    
    UILAYER_INFO_T icon_bg_layer_info = {0};
    uint32 * icon_buf = PNULL;
    FIX16_POINT_T	offset = {0};
    IMGREF_SIZE_T	up_window = {0}; 
    
    GUI_LCD_DEV_INFO   icon_layer_handle = {0,UILAYER_NULL_HANDLE};
    
    if(PNULL == menu_ctrl_ptr || PNULL == lcd_dev_ptr)
    {
        return FALSE;
    }
    
    max_page_item_num = SWEETS_GetPageItemMaxNum();
    
    SWEETS_GetMenuItemByIndex(menu_ctrl_ptr, item_index, &item_info);
    
    icon_rect = GUI_CreateRect(0, 0,SWEETS_ICON_TEXT_WIDTH, SWEETS_ICON_TEXT_HEIGHT);     
    
    pBgRes = (COLOR16*)UILAYER_GetLayerBufferPtr(&s_sweets_menu_info.icon_bg_layer_handle);
    UILAYER_GetLayerInfo(&s_sweets_menu_info.icon_bg_layer_handle,&icon_bg_layer_info);
    BgRes_window.w = icon_bg_layer_info.layer_width;
    BgRes_window.h = icon_bg_layer_info.layer_height;
    
    SCI_MEMCPY(out_ptr,pBgRes,SWEETS_ICON_TEXT_WIDTH*SWEETS_ICON_TEXT_HEIGHT*sizeof(uint32));
    
    UILAYER_SetPreMultiplyAlpha(&icon_layer_handle,FALSE,FALSE);
    UILAYER_Clear(&icon_layer_handle);
       
    //画icon
    SWEETS_GetMenuItemByIndex(menu_ctrl_ptr, item_index, &item_info);
    switch(item_info.icon.type)
    {
    case CTRL_ICON_ID:
        img_id = item_info.icon.data.icon_id;
        GUIRES_GetImgWidthHeight((uint16*)&up_window.w, (uint16*)&up_window.h, img_id, menu_ctrl_ptr->win_handle);
        SWEETS_CreatOneIconLayer(menu_ctrl_ptr,&icon_layer_handle,up_window.w,up_window.h);
        GUIRES_DisplayImg(PNULL,
            &icon_rect,
            &icon_rect,
            menu_ctrl_ptr->win_handle,
            img_id,
            (const GUI_LCD_DEV_INFO*)&icon_layer_handle);
        break;

    case CTRL_ICON_BUFFER:
        {
        	GUI_POINT_T img_point = {0};
			GUIIMG_BITMAP_T img_map_info = {0};
			GUIIMG_DISPLAYBMP_PARAM_T display_param = {0};
			
			img_point.x = icon_rect.left;
			img_point.y = icon_rect.top;
			
			img_map_info.bit_ptr    = item_info.icon.data.buffer.data;
			img_map_info.img_width  = item_info.icon.data.buffer.width;
			img_map_info.img_height = item_info.icon.data.buffer.height;
			img_map_info.date_type  = GUIIMG_DATE_TYPE_ARGB888;

            SWEETS_CreatOneIconLayer(menu_ctrl_ptr, &icon_layer_handle, img_map_info.img_width, img_map_info.img_height);
			
			display_param.is_horizontal = FALSE;
			display_param.rect_ptr = &icon_rect;
			display_param.point_ptr = &img_point;
			display_param.bitmap_ptr = &img_map_info;
			display_param.lcd_dev_ptr = &icon_layer_handle;
			display_param.is_transparent_mode = TRUE;
			display_param.transparent_color = 0x00;
			GUIIMG_DisplayBmpEx(&display_param);
			//SCI_FREE(img_map_info.bit_ptr);

            up_window.h = item_info.icon.data.buffer.height;
            up_window.w = item_info.icon.data.buffer.width;
        } 
        break;

    case CTRL_ICON_FILE://目前暂不支持
    default:
        break;
    }
        
    UILAYER_SetPreMultiplyAlpha(&icon_layer_handle,TRUE,TRUE);
    
    icon_buf = (uint32*)UILAYER_GetLayerBufferPtr(&icon_layer_handle);
    
    offset.x = (SWEETS_ICON_TEXT_WIDTH- up_window.w)/2;
    offset.y = (SWEETS_ICON_TEXT_HEIGHT-up_window.h)/2;
    
    //算法显示大图标
    S2D_TypeBlending(BgRes_window,out_ptr,S3D_DATA_PAF_RGB888,up_window,offset,icon_buf,S3D_DATA_PAF_RGB888, out_ptr,PNULL);
    SWEETS_ReleaseOneIconLayer(menu_ctrl_ptr,&icon_layer_handle);
       
    return TRUE;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_Change_handle_and_index(SWEETS_MEM_PAGE_INFO_T* page_array_info,
                                          BOOLEAN is_tpmove_right)
{
    int i = 0;
    
    //is_tpmove_right 为true时，则tp向右滑动，为FALSE时，则tp是向左滑动
    if(is_tpmove_right)
    {
        for(i=0;i<SWEETS_INIT_PAGE_ITEM_NUM;i++)
        {
            if((SWEETS_INIT_PAGE_ITEM_NUM-1) == page_array_info[i].index)
            {
                //杀掉该layer,重新创建并初始化
                page_array_info[i].is_change = TRUE;
                page_array_info[i].index = 0;
            }
            else
            {
                page_array_info[i].index++;
            }
        }
        
    }
    else
    {
        for(i=0;i<SWEETS_INIT_PAGE_ITEM_NUM;i++)
        {
            if(0 == page_array_info[i].index)
            {
                //杀掉该layer,重新创建并初始化
                page_array_info[i].is_change = TRUE;
                page_array_info[i].index = SWEETS_INIT_PAGE_ITEM_NUM-1;
            }
            else
            {
                page_array_info[i].index--;
            }
        }
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: kyle.jin
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_UpdateItemLayer(
                                  MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr,
                                  BOOLEAN is_tpmove_right
                                  )
{
    int i = 0;
    uint16 page_index   = 0;
    
    //先切换当前页
    if(is_tpmove_right)
    {
        if(0 == menu_ctrl_ptr->cur_page_index)
        {
            menu_ctrl_ptr->cur_page_index = s_sweets_menu_info.total_page_num-1;
        }
        else
        {
            menu_ctrl_ptr->cur_page_index = menu_ctrl_ptr->cur_page_index-1;
        }
    }
    else
    {
        if((s_sweets_menu_info.total_page_num-1) == menu_ctrl_ptr->cur_page_index)
        {
            menu_ctrl_ptr->cur_page_index = 0;
        }
        else
        {
            menu_ctrl_ptr->cur_page_index = menu_ctrl_ptr->cur_page_index+1;
        }
        
    }
    SWEETS_DisplayPageIdxLayer(menu_ctrl_ptr,menu_ctrl_ptr->cur_page_index,menu_ctrl_ptr->titleidx_layer_handle);
    
    //is_tpmove_right 为true时，则tp向右滑动，为FALSE时，则tp是向左滑动
    SWEETS_Change_handle_and_index(s_sweets_menu_info.mem_page_info,is_tpmove_right);
    
    if(is_tpmove_right)
    {
        //新的PAGE INDEX
        if(0 == menu_ctrl_ptr->cur_page_index)
        {
            page_index = s_sweets_menu_info.total_page_num-1;
        }
        else
        {
            page_index = menu_ctrl_ptr->cur_page_index-1;
        }
    }
    else
    {
        //新的PAGE INDEX
        if((s_sweets_menu_info.total_page_num-1) == menu_ctrl_ptr->cur_page_index)
        {
            page_index = 0;
        }
        else
        {
            page_index = menu_ctrl_ptr->cur_page_index+1;
        }
    }
    
    for(i=0;i<SWEETS_INIT_PAGE_ITEM_NUM;i++)
    {
        if(s_sweets_menu_info.mem_page_info[i].is_change)
        {            
            SWEETS_DrawItemPageLayer(menu_ctrl_ptr,page_index,&(s_sweets_menu_info.mem_page_info[i].layer_handle));
            //重置标志量
            s_sweets_menu_info.mem_page_info[i].is_change = FALSE;
            
            break;
        }
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_CreatOnePageLayer(
                                    MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,
                                    GUI_LCD_DEV_INFO *lcd_dev_ptr
                                    )
{
    uint16              layer_width = 0;
    uint16              layer_height = 0;
    UILAYER_CREATE_T    create_info = {0};
    uint16              page_item_num = 0;
    
    if(PNULL == menu_ctrl_ptr)
    {
        return;
    }
    
    //三块数据申请成一样的，这样换页时不用释放后重新申请，也减少了碎片
    //page_item_num = SWEETS_GetPageItemNum(menu_ctrl_ptr,page_index);
    page_item_num = SWEETS_GetPageItemMaxNum();
    
    layer_width = SWEETS_ICON_TEXT_WIDTH;
    layer_height = SWEETS_ICON_TEXT_HEIGHT*page_item_num;
    
    lcd_dev_ptr->lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_ptr->block_id = UILAYER_NULL_HANDLE;
    
    // 创建层
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = menu_ctrl_ptr->ctrl_handle;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = layer_width;
    create_info.height = layer_height;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE; 
    UILAYER_CreateLayer(
        &create_info,  
        lcd_dev_ptr
        );
    // 设置color key
    UILAYER_SetLayerColorKey(lcd_dev_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);
    // 使用color清除层
    UILAYER_Clear(lcd_dev_ptr);
    
    return;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SWEETS_DrawItemPageLayer(
                                       MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,
                                       uint16   page_index,
                                       GUI_LCD_DEV_INFO *lcd_dev_ptr
                                       )
{
    int        i = 0;
    uint16     item_num = 0;
    uint32     *out_ptr = PNULL;
    uint16     item_index = 0;
    uint16     max_page_item_num = 0;
    GUI_RECT_T dis_rect = {0};
    
    if(PNULL == menu_ctrl_ptr
        ||PNULL == lcd_dev_ptr
        ||!UILAYER_IsLayerActive(lcd_dev_ptr)
        ||(page_index>=s_sweets_menu_info.total_page_num)
        )
    {
        return FALSE;
    }

    max_page_item_num = SWEETS_GetPageItemMaxNum();
    
    item_num = SWEETS_GetPageItemNum(menu_ctrl_ptr,page_index);
    
    if(0 == item_num)
    {
        return FALSE;
    }
   
    //这是该页第一个ITEM的item_index
    item_index = page_index*max_page_item_num;
    
    dis_rect.top =0;
    dis_rect.left = 0;
    dis_rect.right = dis_rect.left + SWEETS_ICON_TEXT_WIDTH;
    dis_rect.bottom = dis_rect.top + SWEETS_ICON_TEXT_HEIGHT;

    
    UILAYER_SetPreMultiplyAlpha(lcd_dev_ptr,TRUE,TRUE);
    out_ptr = (uint32*)UILAYER_GetLayerBufferPtr(lcd_dev_ptr);
    
    for(i=0;i<item_num;i++)
    {

        SWEETS_DrawOneItemToLayer(menu_ctrl_ptr,
                                 (GUI_LCD_DEV_INFO*)lcd_dev_ptr,//out
                                 (uint16*)out_ptr,
                                 item_index,
                                 PNULL,
                                 dis_rect
                                 );
        out_ptr += SWEETS_ICON_TEXT_WIDTH*SWEETS_ICON_TEXT_HEIGHT;
        dis_rect.top += SWEETS_ICON_TEXT_HEIGHT;
        dis_rect.bottom = dis_rect.top + SWEETS_ICON_TEXT_HEIGHT;
        item_index++; 
    }

    out_ptr = PNULL;


    return TRUE;
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SWEETS_CreatItemLayer(
                                    MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                    )
{
    SWEETS_ReleaseItemData();
    
    //创建当前页
    SWEETS_CreatOnePageLayer(menu_ctrl_ptr,&s_sweets_menu_info.mem_page_info[1].layer_handle);
    
    //创建上一页
    SWEETS_CreatOnePageLayer(menu_ctrl_ptr,&s_sweets_menu_info.mem_page_info[0].layer_handle);
    
    //创建下一页
    SWEETS_CreatOnePageLayer(menu_ctrl_ptr,&s_sweets_menu_info.mem_page_info[2].layer_handle);

    return TRUE;
    
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SWEETS_InitCurPageItemData(
                                         MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                         )
{
    //画当前页
    s_sweets_menu_info.mem_page_info[1].index = 1;
    SWEETS_DrawItemPageLayer(menu_ctrl_ptr,menu_ctrl_ptr->cur_page_index,&s_sweets_menu_info.mem_page_info[1].layer_handle);

    return TRUE;
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SWEETS_InitItemData(
                                  MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                  )
{
    //int i=0;
    //BOOLEAN result = TRUE;
    uint16 page_index = 0;
    
    if(UILAYER_IsLayerActive(&s_sweets_menu_info.mem_page_info[0].layer_handle)
        &&UILAYER_IsLayerActive(&s_sweets_menu_info.mem_page_info[1].layer_handle)
        &&UILAYER_IsLayerActive(&s_sweets_menu_info.mem_page_info[2].layer_handle)
        )
    {
        //画当前页
        s_sweets_menu_info.mem_page_info[1].index = 1;
        SWEETS_DrawItemPageLayer(menu_ctrl_ptr,menu_ctrl_ptr->cur_page_index,&s_sweets_menu_info.mem_page_info[1].layer_handle);
        
        //画上一页
        if(0 == menu_ctrl_ptr->cur_page_index)
        {
            page_index = s_sweets_menu_info.total_page_num-1;
        }
        else
        {
            page_index = menu_ctrl_ptr->cur_page_index-1;
        }
        s_sweets_menu_info.mem_page_info[0].index = 0;
        SWEETS_DrawItemPageLayer(menu_ctrl_ptr,page_index,&s_sweets_menu_info.mem_page_info[0].layer_handle);
        
        //画下一页
        if((s_sweets_menu_info.total_page_num-1) == menu_ctrl_ptr->cur_page_index)
        {
            page_index = 0;
        }
        else
        {
            page_index = menu_ctrl_ptr->cur_page_index+1;
        }
        
        s_sweets_menu_info.mem_page_info[2].index = 2;
        SWEETS_DrawItemPageLayer(menu_ctrl_ptr,page_index,&s_sweets_menu_info.mem_page_info[2].layer_handle);
    }
    else
    {
        SWEETS_ReleaseItemData();
        return FALSE;
    }
    
    return TRUE;
}
/*****************************************************************************/
//  Description : release dst layer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_ReleaseItemData(void)
{
    int i=0;
    
    if (UILAYER_IsMultiLayerEnable())    
    {
        for(i=0;i<SWEETS_INIT_PAGE_ITEM_NUM;i++)
        {
            UILAYER_RELEASELAYER(&s_sweets_menu_info.mem_page_info[i].layer_handle);
            s_sweets_menu_info.mem_page_info[i].index = i;
            s_sweets_menu_info.mem_page_info[i].is_change = FALSE;
        }
    }
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_InitMenuTotalPageNum(
                                       MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                       )
{
    uint16 max_page_item_num = 0;
    
    if(PNULL == menu_ctrl_ptr)
    {
        return;
    }
    
    max_page_item_num = SWEETS_GetPageItemMaxNum();
    
    if(0 == max_page_item_num)
    {
        return;
    }
    
    if((menu_ctrl_ptr->item_total_num%max_page_item_num) != 0)
    {
        s_sweets_menu_info.total_page_num =menu_ctrl_ptr->item_total_num/max_page_item_num+1;
    }
    else
    {
        s_sweets_menu_info.total_page_num =menu_ctrl_ptr->item_total_num/max_page_item_num;
    }
}


/*****************************************************************************/
// 	Description : reset
//	Global resource dependence : 
//  Author:
//	Note: 既可以是初始化,也可用于销毁
/*****************************************************************************/
LOCAL void SWEETS_ResetLocalParam(void)
{
    SCI_MEMSET(&s_sweets_menu_info,0,sizeof(s_sweets_menu_info));
}


/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void SWEETS_InitItemRect(MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr)
{
    //uint16 width = 0;
    //uint16 height = 0;
    
    //全屏
    //GUILCD_GetLogicWidthHeight(menu_ctrl_ptr->lcd_dev_info.lcd_id, &width, &height);
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        s_sweets_menu_info.icon_location = (IMGREF_POINT_T*)s_icon_location_h;
    }
    else
    {
        s_sweets_menu_info.icon_location = (IMGREF_POINT_T*)s_icon_location_v;
    }
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_AllocMem(void)
{
    uint32 edge_table_size = 300*sizeof(IMGREF_POINT_T);
    uint32 location_table_size = 4000*sizeof(uint32);
    uint32 z_table_size = 4000*sizeof(int16);
    uint32 shine_table_size = 4000*sizeof(uint16);
    uint32 edge_alpha_size = 4000*sizeof(uint8);
    uint32 all_size = 0;
    uint8* buf_ptr = PNULL;

    
    all_size = edge_table_size+location_table_size+z_table_size+shine_table_size+edge_alpha_size;

    if(PNULL != s_sweets_alloc_mem_ptr)
    {
        SCI_FREE(s_sweets_alloc_mem_ptr);
    }
    
    s_sweets_alloc_mem_ptr = SCI_ALLOCA(all_size);
    if(s_sweets_alloc_mem_ptr == PNULL)
    {
        return;
    }

    SCI_MEMSET(s_sweets_alloc_mem_ptr,0,all_size);
    
    buf_ptr = s_sweets_alloc_mem_ptr;
    
    s_edge_table_fix = (IMGREF_POINT_T *)buf_ptr;
    buf_ptr += edge_table_size;
    s_src_p_location_table = (uint32*)buf_ptr;
    buf_ptr += location_table_size;
    //s_src_p_z_table = (int16*)buf_ptr;
    buf_ptr += z_table_size;
    s_src_shine_table = (uint16*)buf_ptr;
    buf_ptr += shine_table_size;
    s_edge_alpha = (uint8*)buf_ptr;

    
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_ReleaseMem(void)
{
    s_edge_table_fix = PNULL;
    s_src_p_location_table = PNULL;
    //s_src_p_z_table = PNULL;
    s_src_shine_table = PNULL;
    s_edge_alpha = PNULL;

    SCI_FREE(s_sweets_alloc_mem_ptr);
    s_sweets_alloc_mem_ptr = PNULL;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void *MMIMENU_SWEETS_InitMenu(
                                   MMI_HANDLE_T ctrl_handle
                                   )
{ 
    MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr = PNULL;
    
    menu_ctrl_ptr = SCI_ALLOC_APP(sizeof(MMIMAINMENU_SWEETS_DATA_T));

    if (PNULL == menu_ctrl_ptr)
    {
        SCI_TRACE_LOW("MatrixMenuInit Faild");
        return PNULL;
    }

    SCI_MEMSET(menu_ctrl_ptr, 0, sizeof(MMIMAINMENU_SWEETS_DATA_T));

    menu_ctrl_ptr->ctrl_handle = ctrl_handle;
    menu_ctrl_ptr->win_handle = MMK_GetWinHandleByCtrl(ctrl_handle);

    MMIMENU_SWEETS_InitMenuInfo(menu_ctrl_ptr);

    return (void *)menu_ctrl_ptr;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIMENU_SWEETS_InitMenuInfo(
                                       MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                       )
{
    if (PNULL == menu_ctrl_ptr)
    {
        return;
    }
    MMITHEME_DestroyMenuItemCache();
    MMIRESOUT_Init(RES_OUT_MENU, MENU_SWEETS);
        
    SWEETS_ResetLocalParam();

    // 初始化内存中的链表
    SWEETS_ResetItemListInfo(menu_ctrl_ptr);

    //init main menu theme info
    MMITHEME_GetMainMenuTheme(&menu_ctrl_ptr->main_menu_theme);    
    
    // 读取item信息
    SWEETS_ReadMenuItemInfo(menu_ctrl_ptr);

    //初始化页数
    SWEETS_InitMenuTotalPageNum(menu_ctrl_ptr);
    
    //创建dst层
    SWEETS_CreatDstLayer(menu_ctrl_ptr);
    
    //创建item层
    SWEETS_CreatItemLayer(menu_ctrl_ptr);
    
    //index 层
    SWEETS_CreatePageIdxLayer(menu_ctrl_ptr);
    
    //text
    SWEETS_CreatTextLayer(menu_ctrl_ptr);
    
    //icon bg res
    SWEETS_CreatIconBgLayer(menu_ctrl_ptr);

    //glass
    SWEETS_CreateGlassLayer(menu_ctrl_ptr);

    SWEETS_CreatShadowLayer(menu_ctrl_ptr);

    SWEETS_AllocMem();
    
    GUIWIN_CreateStbDyn(MAIN_MAINMENU_WIN_ID); // 动态创建状态栏
    GUIWIN_SetStbState(menu_ctrl_ptr->win_handle, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
    GUIWIN_SetStbBgIsDesplay(menu_ctrl_ptr->win_handle,TRUE);
   
    //在删除动态应用后，将menu_ctrl_ptr->cur_item_index置为0，防止在删除位置显示高亮。
    menu_ctrl_ptr->cur_item_index = 0;
    
    //区域位置初始化
    SWEETS_InitItemRect(menu_ctrl_ptr);

    return;
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_DestoryMenu(
                                      void *pm_data_ptr
                                      )
{
    MMIMAINMENU_SWEETS_DATA_T * menu_ctrl_ptr =(MMIMAINMENU_SWEETS_DATA_T *)pm_data_ptr;

    if(PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }
    
    MMIMENU_SWEETS_DestoryMenuInfo(menu_ctrl_ptr);

    SCI_FREE(pm_data_ptr);
    pm_data_ptr = PNULL;

    return TRUE;
}

/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_DestoryMenuInfo(
                                             MMIMAINMENU_SWEETS_DATA_T * menu_ctrl_ptr
                                             )
{
    if(PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }
    
    //重置
    SWEETS_ResetItemListInfo(menu_ctrl_ptr);
    
    //释放dst层
    SWEETS_ReleaseDstLayer();
    
    //释放item层
    SWEETS_ReleaseItemData();
    
    //index 层
    SWEETS_ReleasePageIdxLayer(menu_ctrl_ptr);
    
    //text
    SWEETS_ReleaseTextLayer();
    
    SWEETS_ReleaseIconBgLayer();

    SWEETS_ReleaseGlassLayer();

    SWEETS_ReleaseShadowLayer();

    SWEETS_ReleaseMem();
    
    SWEETS_ReleaseEnterAnimLayer(menu_ctrl_ptr);
    SCI_FREE(s_sweets_menu_info.Point_Add_ptr);
    s_sweets_menu_info.Point_Add_ptr = PNULL;
    
    menu_ctrl_ptr->cur_item_index = 0;
    menu_ctrl_ptr->cur_page_index = 0;
    
    SWEETS_ResetLocalParam();

    MMIRESOUT_UnInit();

    return TRUE;
}

/*****************************************************************************/
//  Description : 合并层
//  Global resource dependence : 
//  Author: jila
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_BltLayer(GUI_LCD_DEV_INFO *layer_1_ptr,GUI_LCD_DEV_INFO *layer_2_ptr)
{
    UILAYER_APPEND_BLT_T append_layer = {0};
    
    if(PNULL != layer_1_ptr)
    {
        UILAYER_RemoveBltLayer(layer_1_ptr);
        
        append_layer.lcd_dev_info = *layer_1_ptr;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }
    
    if(PNULL != layer_2_ptr)
    {
        UILAYER_RemoveBltLayer(layer_2_ptr);
        
        append_layer.lcd_dev_info = *layer_2_ptr;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }
}
/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL uint32* SWEETS_GetTextBufPtrByPageIndex(
                                              uint16        page_index
                                              )
{
    uint32 *text_ptr = PNULL;
    uint32 *page_text_ptr = PNULL;
    uint32 one_page_size = 0;
    
    text_ptr = (uint32*)UILAYER_GetLayerBufferPtr(&s_sweets_menu_info.text_layer_handle);
    one_page_size = SWEETS_MENUTEXT_WIDTH*SWEETS_MENUTEXT_HEIGHT*SWEETS_PAGE_ICON_NUM;
    page_text_ptr = text_ptr + page_index*one_page_size;
    
    return page_text_ptr;
}
/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T SWEETS_GetItemDisRect(
                                       MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr,
                                       uint16        item_index
                                       )
{
    GUI_RECT_T rect = {0};
    IMGREF_POINT_T point = {0};
    
    point = s_sweets_menu_info.icon_location[item_index]; 
    
    rect.left = point.x;
    rect.right = rect.left + SWEETS_ICON_TEXT_WIDTH;
    rect.top = point.y;
    rect.bottom = rect.top + SWEETS_ICON_TEXT_HEIGHT;
    
    return rect;
}

#ifdef ATEST_SUPPORT
PUBLIC GUI_RECT_T Atest_SWEETS_GetItemDisRect(
                                       void *menu_ctrl_ptr,
                                       uint16        item_index
                                       )
{
	return SWEETS_GetItemDisRect((MMIMAINMENU_SWEETS_DATA_T*)menu_ctrl_ptr, item_index);
}

PUBLIC uint16 Atest_SWEETS_GetPageItemNum(void *menu_ctrl_ptr, uint16   page_index)
{
  return SWEETS_GetPageItemNum((MMIMAINMENU_SWEETS_DATA_T*)menu_ctrl_ptr, page_index);
}
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_DisplayActiveIcon(
                                    MMIMAINMENU_SWEETS_DATA_T     *menu_ctrl_ptr
                                    )
{
    return;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SWEETS_StartEnterAnim(
                                 MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                 )
{
    uint32* icon_buffer = PNULL;
    uint32 * out_32_buffer = PNULL;
    uint32 * glass_color_ptr = PNULL;
    IMGREF_POINT_T *icon_location = PNULL;
    uint16 icon_num = 0;
    
    //创建临时层 888
    SWEETS_CreatEnterAnimLayer(menu_ctrl_ptr);
    
    icon_buffer = (uint32*)UILAYER_GetLayerBufferPtr(&s_sweets_menu_info.mem_page_info[1].layer_handle);
    
    out_32_buffer = (uint32*)UILAYER_GetLayerBufferPtr(&s_sweets_menu_info.enter_anim_temp_layer_handle);
    icon_location = s_sweets_menu_info.icon_location;
    icon_num = SWEETS_GetPageItemNum(menu_ctrl_ptr,menu_ctrl_ptr->cur_page_index);

    glass_color_ptr = (uint32*)UILAYER_GetLayerBufferPtr(&s_sweets_menu_info.glass_layer_handle);

    if(PNULL == icon_buffer||PNULL == glass_color_ptr||PNULL == out_32_buffer)
    {
        SWEETS_ReleaseEnterAnimLayer(menu_ctrl_ptr);
        SCI_TRACE_LOW("[SWEETS] PTR333 IS PNULL   ERROR!!!!!");
        return;
    }
    
    Glass_Sweets_Get_Convert_Buffer(menu_ctrl_ptr,
        icon_buffer,
        out_32_buffer,
        icon_location,
        icon_num,
        glass_color_ptr
        );

    s_sweets_menu_info.Point_Add_ptr = SCI_ALLOCA(3072*sizeof(FIX16_POINT_T));
    
    if(s_sweets_menu_info.Point_Add_ptr == PNULL)
    {
        SCI_TRACE_LOW("[SWEETS] Point_Add_ptr IS PNULL   ERROR!!!!!");
        SWEETS_ReleaseEnterAnimLayer(menu_ctrl_ptr);
        SCI_FREE(s_sweets_menu_info.Point_Add_ptr);
        s_sweets_menu_info.Point_Add_ptr = PNULL;
        return;
    }

		//入场动画
		SWEETS_SetAnimCurState(SWEETS_STATE_ENTER_ANIM);

    //start timer
    SweetsStartRedrawTimer(menu_ctrl_ptr);   
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SWEETS_EndEnterAnim(
                               MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                               )
{
    SWEETS_ReleaseEnterAnimLayer(menu_ctrl_ptr);
    SCI_FREE(s_sweets_menu_info.Point_Add_ptr);
    s_sweets_menu_info.Point_Add_ptr = PNULL;
    
    SWEETS_HandleFullPaintEnd(menu_ctrl_ptr);
    
    SWEETS_SetAnimCurState(SWEETS_STATE_SHAKE);
    SWEETS_SetAnimAimAngle(SWEETS_IDLE_SHAKE_ANGLE);
    SWEETS_SetChangeState(TRUE);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SWEETS_DisplayOnePage(
                                 MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,
                                 uint16   page_index,
                                 GUI_LCD_DEV_INFO *lcd_dev_ptr
                                 )
{
    int16 angle = 0;
    uint16 offset = 0;
    IN_POSITON_DIRECTION_E in_position = 0;
    
    SWEETS_SetAnimCurState(SWEETS_STATE_NONE);
    
    SWEETS_Calculate(menu_ctrl_ptr,offset,angle,in_position);
    //发消息初始化三页的数据
    MMK_PostMsg(menu_ctrl_ptr->ctrl_handle,MSG_MENU_FULL_PAINT_END,PNULL,0);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:kyle jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SWEETS_CheckLayerMem( 
							   MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
							   )
{
    BOOLEAN result = TRUE;
    int i = 0;

    if(!UILAYER_IsLayerActive(&s_sweets_menu_info.dst_layer_handle)
    ||!UILAYER_IsLayerActive(&s_sweets_menu_info.glass_layer_handle)
    ||!UILAYER_IsLayerActive(&s_sweets_menu_info.text_layer_handle)
    ||!UILAYER_IsLayerActive(&s_sweets_menu_info.shadow_layer_handle)
    )
    {
        result = FALSE;
    }
    else
    {
        for(i=0;i<SWEETS_INIT_PAGE_ITEM_NUM;i++)
        {
            if(!UILAYER_IsLayerActive(&s_sweets_menu_info.mem_page_info[i].layer_handle))
            {
				SCI_TRACE_LOW("SWEETS_CheckLayerMem invalid layer i = %d, handle = %d", i, s_sweets_menu_info.mem_page_info[i].layer_handle.block_id);
                result = FALSE;
                break;
            }	
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:kyle jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_CheckLayerMem( 
									   MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
									   )
{
    BOOLEAN result = TRUE;
    
    if(PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }
    
    result = SWEETS_CheckLayerMem(menu_ctrl_ptr);
    
    if (!result)
    {
        //释放缓冲区
        BLKMEM_GarbageClean();

        UILAYER_SetMainLayerMemFormat(UILAYER_MEM_NONE);
        UILAYER_SetLayerMemFormat(&s_sweets_menu_info.dst_layer_handle,UILAYER_MEM_NONE);

        //重新初始化 申请内存
        UILAYER_ActiveAllLayerOfHandle( menu_ctrl_ptr->ctrl_handle);
        //MMIMENU_InitMenuData(menu_ctrl_ptr);

        //再次check
        result = SWEETS_CheckLayerMem(menu_ctrl_ptr);
        
        if (!result)
        {
            MMISET_IDLE_STYLE_E     idle_style = MMIAPISET_GetIdleStyle();
            MMISET_IDLE_SUBSTYLE_E cur_idle_substyle = MMIAPISET_GetIdleSubStyle();
            
            //只有默认系统下可以切换主菜单风格重新进入，其他风格只能退出到IDLE
            if(MMISET_IDLE_STYLE_GRID == idle_style&&MMISET_IDLE_SUBSTYLE_DEFAULT == cur_idle_substyle)
            {
                MMK_CloseWin(menu_ctrl_ptr->win_handle);
                MMIAPISET_SetMainmenuStyle(MMIMAINMENU_MATRIX_E);
                MMIAPIMENU_CreatMainMenu();
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_MEMORY);
                MMK_CloseWin(menu_ctrl_ptr->win_handle);
            }

        }
    }

	return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MMIMENU_SWEETS_DisplayMenu(
                                      MMIMAINMENU_SWEETS_DATA_T     *menu_ctrl_ptr
                                      )
{   
    int i = 0;
    
    if(PNULL == menu_ctrl_ptr)
    {
        return;
    }
    //lose focus 时有回拷动作，不需要画任何东东
    if(!MMK_IsFocusWin(menu_ctrl_ptr->win_handle))
    {
        return;
    }
#ifdef DYNAMIC_MAINMENU_SUPPORT
    if (MMIMENU_DyCheckIsSyn())//check信息有变化
    {
        SWEETS_ReloadItemToMainMenu(menu_ctrl_ptr);
    }
#endif

    if(!MMIMENU_SWEETS_CheckLayerMem(menu_ctrl_ptr))
    {
        return;
    }
    
    if(UILAYER_IsLayerActive(&s_sweets_menu_info.dst_layer_handle))
    {
        SWEETS_BltLayer(&s_sweets_menu_info.dst_layer_handle,&menu_ctrl_ptr->titleidx_layer_handle);
        
        SWEETS_DisplayTextLayer(menu_ctrl_ptr);
        
        SWEETS_DisplayPageIdxLayer(menu_ctrl_ptr,menu_ctrl_ptr->cur_page_index,menu_ctrl_ptr->titleidx_layer_handle);

        SWEETS_DisplayIconBgLayer(menu_ctrl_ptr);

        SWEETS_DisplayGlassLayer(menu_ctrl_ptr);

        SWEETS_DisplayShadowLayer(menu_ctrl_ptr);

        for(i=0;i<SWEETS_INIT_PAGE_ITEM_NUM;i++)
        {
            s_sweets_menu_info.mem_page_info[i].index = i;
            s_sweets_menu_info.mem_page_info[i].is_change = FALSE;
        }
        
        //初始化当前页数据
        SWEETS_InitCurPageItemData(menu_ctrl_ptr);

        //往主层上直接画BG图
        SWEETS_DisplayBgToLayer(menu_ctrl_ptr,PNULL);
			
#if defined(SWEETS_MENU_HAVE_ENTER_ANIM)
        if (GetState(menu_ctrl_ptr->menu_state, GUIMENU_STATE_ENTER))
        {
            SWEETS_StartEnterAnim(menu_ctrl_ptr);
        }
        else
#endif
        {
            //没有入场动画直接画一帧
            SWEETS_DisplayOnePage(menu_ctrl_ptr,menu_ctrl_ptr->cur_page_index,&s_sweets_menu_info.dst_layer_handle);
			//start timer
        }
   
    }
    else
    {
        //往主层上直接画BG图
        //SWEETS_DisplayBgToLayer(menu_ctrl_ptr,PNULL);
    }
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIMENU_SWEETS_DisplaySpecialMenu(
                                             MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                             )
{
    return;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMENU_SWEETS_HandleUpKey(
                                              MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                              )
{
    return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMENU_SWEETS_HandleDownKey(
                                                MMIMAINMENU_SWEETS_DATA_T     *menu_ctrl_ptr
                                                )
{
    return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_HandleLeftKey(
                                           MMIMAINMENU_SWEETS_DATA_T     *menu_ctrl_ptr
                                           )
{
    return TRUE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_HandleRightKey(
                                            MMIMAINMENU_SWEETS_DATA_T     *menu_ctrl_ptr
                                            )
{
    return TRUE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_HandleNumKey(
                                          uint16 num_index,
                                          MMI_MESSAGE_ID_E msg_id,
                                          MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                          )
{
    if(PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }
    
    if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state && !menu_ctrl_ptr->is_move_state)
    {
        MMIMENU_SWEETS_HandleMenuOk(menu_ctrl_ptr);
    }
   
    return TRUE;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIMENU_SWEETS_HandleTpDown(
                                       DPARAM param,
                                       MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                       )
{
    uint16      item_num = 0;
    int         i = 0;
    uint16      item_index = 0;
    GUI_RECT_T  item_rect = {0};
    GUI_POINT_T tp_point = {0};
    
    if(PNULL == menu_ctrl_ptr)
    {
        return;
    }
    
    if(SWEETS_STATE_ENTER_ANIM == SWEETS_GetAnimCurState())
    {
        return;
    }

    //set tp press point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);
    
    menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
    menu_ctrl_ptr->is_tp_pressed = TRUE;
    
    // 增加速度点
    MMK_AddVelocityItem( tp_point.x, tp_point.y );
    
    // 首先设置页面的滑动相关数据
    // 记录起始点
    menu_ctrl_ptr->slide_start_point.x = tp_point.x;
    menu_ctrl_ptr->slide_start_point.y = tp_point.y;
    
    // 记录前一个点
    menu_ctrl_ptr->slide_pre_point.x = tp_point.x;
    menu_ctrl_ptr->slide_pre_point.y = tp_point.y;
    
    item_num = SWEETS_GetPageItemNum(menu_ctrl_ptr, menu_ctrl_ptr->cur_page_index);
    
    for(i = 0; i < item_num; i++)
    {
        item_index = SWEETS_GetPageItemIndex(menu_ctrl_ptr->cur_page_index, i);
        item_rect = SWEETS_GetItemDisRect(menu_ctrl_ptr, i);
        //if press item
        if (GUI_PointIsInRect(tp_point,item_rect))
        {
            menu_ctrl_ptr->is_item_pressed = TRUE;
            
            menu_ctrl_ptr->cur_item_index = item_index;
            
            SWEETS_DisplayActiveIcon(menu_ctrl_ptr);
            
            break;
        }
    }

    return;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIMENU_SWEETS_HandleTpMove(
                                       DPARAM param,
                                       MMIMAINMENU_SWEETS_DATA_T*menu_ctrl_ptr
                                       )
{
    GUI_POINT_T      tp_point = {0};
    
    if(SWEETS_STATE_ENTER_ANIM == SWEETS_GetAnimCurState()||menu_ctrl_ptr->slide_state == MMK_TP_SLIDE_TP_SCROLL)
    {
        return;
    }
    
    //set tp press point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);
    
    // 增加速度点
    MMK_AddVelocityItem(tp_point.x, tp_point.y);
    if(abs(tp_point.x-menu_ctrl_ptr->slide_start_point.x)>4)
    {
        menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_HandleTpUp(
                                        DPARAM param,
                                        MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                        )
{
    BOOLEAN          result = FALSE;
    GUI_POINT_T      tp_point = {0};
    float            fling_velocity_x = 0;
    float            fling_velocity_y = 0;
    uint16           distance   = 0;
    
    if(PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }
    
    if(SWEETS_STATE_ENTER_ANIM == SWEETS_GetAnimCurState())
    {
        return FALSE;
    }
    
    //set tp press point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);
    
    // 增加速度点
    MMK_AddVelocityItem(tp_point.x, tp_point.y);
    #ifdef TOUCH_PANEL_SUPPORT
    MMK_ComputeCurrentVelocity((float*)&fling_velocity_x, (float*)&fling_velocity_y,(float)FLING_MAX_VELOCITY,  (float)FLING_MAX_VELOCITY);
    #endif
    menu_ctrl_ptr->fling_velocity = fling_velocity_x;
    
    result = menu_ctrl_ptr->is_item_pressed;
    menu_ctrl_ptr->is_item_pressed = FALSE;
    menu_ctrl_ptr->is_tp_pressed = FALSE;
    
    distance = abs(menu_ctrl_ptr->slide_pre_point.x - menu_ctrl_ptr->slide_start_point.x);
    
    //已经处于滑动状态
    if (MMK_TP_SLIDE_TP_SCROLL == menu_ctrl_ptr->slide_state)
    {
        //如果滑动速度很快，则直接滑倒下一页
        if(FLING_MIN_VELOCITY <= abs((int32)menu_ctrl_ptr->fling_velocity))
        {
            menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_FLING;
        }
        else//如果滑动速度很慢，则要看滑动的距离
        {            
            if(distance>SWEETS_CHANGE_PAGE_TOTAL_FRAME_MAX/2)
            {
                menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_FLING;
            }
            else
            {
                menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_RECOVER;
            }
        }
        s_sweets_menu_info.cur_frame = 0;
        return FALSE;
    }
    //未处于滑动状态，但速度很快，则直接翻到下一页
    else if(FLING_MIN_VELOCITY <= abs((int32)menu_ctrl_ptr->fling_velocity)
        &&(MMK_TP_SLIDE_TP_SCROLL != menu_ctrl_ptr->slide_state))
    {
        menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_FLING;
        s_sweets_menu_info.cur_frame = 0;
        return FALSE;
    }

    if (result)
    {
        MMIMENU_SWEETS_HandleMenuOk(menu_ctrl_ptr);
    }
    
    return result;   
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_HandleFullPaintEnd(
                                     MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                     )
{
    SweetsStopRedrawTimer(menu_ctrl_ptr);
    //初始化三页item层
    SWEETS_InitItemData(menu_ctrl_ptr);
    //start timer
    SweetsStartRedrawTimer(menu_ctrl_ptr);
}


/*****************************************************************************/
//  Description : Sweets Stop Redraw Timer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SweetsStopRedrawTimer(
                                 MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                 )
{
    if (PNULL != menu_ctrl_ptr && 0 != menu_ctrl_ptr->redrew_timer_id)
    {
        MMK_StopTimer(menu_ctrl_ptr->redrew_timer_id);
        menu_ctrl_ptr->redrew_timer_id = 0;
    }
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMENU_SWEETS_HandleSpecialMsg(
                                                   MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr, 
                                                   MMI_MESSAGE_ID_E msg_id,
                                                   DPARAM param
                                                   )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    
    if(PNULL == menu_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_LOSE_FOCUS:
        SWEETS_ReleaseMem();
        SweetsStopRedrawTimer(menu_ctrl_ptr);
        MMIRESOUT_Pause();
        menu_ctrl_ptr->is_tp_pressed = FALSE;
        menu_ctrl_ptr->slide_state= MMK_TP_SLIDE_NONE;
		SetState(&menu_ctrl_ptr->menu_state, GUIMENU_STATE_ENTER, FALSE);
        break;

    case MSG_CTL_GET_FOCUS:
        SWEETS_AllocMem();
        MMIRESOUT_Resume();
        break;

    case MSG_MENU_FULL_PAINT_END:
        SWEETS_HandleFullPaintEnd(menu_ctrl_ptr);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_MoveInit(
                                      MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                      )
{
    return FALSE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_MoveDestory(
                                         MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                                         )
{
    return FALSE;
}


/*****************************************************************************/
//  Description :Sweets Menu Run Item
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SweetsMenuRunItem(
    MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr,
    uint16 item_index
)
{
    CTRLMAINMENU_ITEM_T item = {0};

    SWEETS_GetMenuItemByIndex(menu_ctrl_ptr, item_index, &item);

    if (
#ifdef DYNAMIC_MAINMENU_SUPPORT
    !MMIMENU_DyRunItem(item.group_id,item.menu_id) && 
#endif
    PNULL != item.call_back_start)
    {
        item.call_back_start(item.param_data1, item.param_data2);
    }

    return;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_HandleMenuOk(
                                          MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                          )
{
    //BOOLEAN is_exist_child = FALSE;

    if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state && 
        !menu_ctrl_ptr->is_moving &&
        !menu_ctrl_ptr->is_move_state)
    {
        MMIMAINMENU_ITEM_INFO_T item = {0};
        BOOLEAN is_grayed = TRUE;

        
        MMIMAINMENU_GetItem(&menu_ctrl_ptr->menu_list_info, menu_ctrl_ptr->cur_item_index, &item);
        is_grayed = MMIMAINMENU_IsItemDisable(item.menu_id);

        if (!is_grayed)
        {
            SweetsMenuRunItem(menu_ctrl_ptr, menu_ctrl_ptr->cur_item_index);
        }
    }
    
    return FALSE;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_SWEETS_HandleMenuCancel(
                                          MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                          )
{
    if(PNULL == menu_ctrl_ptr)
    {
        return FALSE;
    }

    if (MMK_TP_SLIDE_NONE == menu_ctrl_ptr->slide_state && 
        !menu_ctrl_ptr->is_moving)
    {
        NotifyParentMsg(menu_ctrl_ptr->ctrl_handle, MSG_APP_CANCEL);
    }
    
    return TRUE;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SWEETS_Calculate(
                                    MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,
                                    uint16 offset,
                                    int16  angle,
                                    IN_POSITON_DIRECTION_E in_position
                                    )
{
    SWEETS_INPARAM_T calc_in_param = {0};
    GUI_RECT_T update_rect = {0};
    //static uint32 cost_time = 0;
    //uint32 temp_time = 0;
    //int i = 0,j=0;
    //uint32 * dst_buf = PNULL;
    //uint32 * glass_buf = PNULL;
    
	static uint32 whole_time1=0,whole_time2=0,tick_time=0;
    
    whole_time1=SCI_GetTickCount();
    SCI_TRACE_LOW("[SWEETS], SHAKE cost_time ONE frame = %d ms", whole_time1-whole_time2);
    whole_time2=SCI_GetTickCount();
    
    //主层有修改，不能设置成immu
    UILAYER_SetMainLayerImmutable(TRUE);
    UILAYER_SetDirectDraw( FALSE );
    UILAYER_SwitchDoubleMem(&s_sweets_menu_info.dst_layer_handle,TRUE);
    
    //填写算法入参
    SWEETS_Adapt_IN_PARAM(menu_ctrl_ptr,&calc_in_param,offset,angle,in_position);
    // 使用color清除层
    UILAYER_Clear(&s_sweets_menu_info.dst_layer_handle);
    //调用算法函数
    tick_time=SCI_GetTickCount();

    if(calc_in_param.out_shine_layer == PNULL
    ||calc_in_param.current_page_icon_buf == PNULL
    ||calc_in_param.next_page_icon_buf == PNULL
    ||calc_in_param.cur_text_buf == PNULL
    ||calc_in_param.next_text_buf == PNULL
    )
    {
        return FALSE;
    }
    Glass_CandyType_Menu(&calc_in_param);

    SCI_TRACE_LOW("YOON_Calc = %d",SCI_GetTickCount() - tick_time);

    
    SCI_TRACE_LOW("[SWEETS] calc time = %d",SCI_GetTickCount() - whole_time2);
    
    update_rect.bottom = calc_in_param.dst_height-1;
    update_rect.right = calc_in_param.dst_width-1;
    
    MMITHEME_StoreUpdateRect(&s_sweets_menu_info.dst_layer_handle,update_rect);

    return TRUE;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SWEETS_HandleShakeAnim(
                                          MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr 
                                          )
{
    int16 angle = 0;
    int16 aim_angle = 0;
    int16 cur_angle = 0;
    uint16 offset = 0;
    IN_POSITON_DIRECTION_E in_position = 0;
    GUI_POINT_T  point = {0};
    MMI_TP_STATUS_E state = MMI_TP_NONE;
    
    //位移
    if(menu_ctrl_ptr->is_tp_pressed)
    {
        #ifdef TOUCH_PANEL_SUPPORT
        MMK_GetLogicTPMsg(&state, &point);
        #endif
        offset = abs(point.x - menu_ctrl_ptr->slide_start_point.x);
        
        //方向
        if(point.x> menu_ctrl_ptr->slide_start_point.x)
        {
            in_position = IN_POSITON_RIGHT;
        }
        else
        {
            in_position = IN_POSITON_LEFT;
        }
    }
    else
    {
        offset = 0;
        in_position = IN_POSITON_NONE;
    }
    
    if(SWEETS_GetChangeState())
    {
        cur_angle = SWEETS_GetAnimCurAngle();
        aim_angle = SWEETS_GetAnimAimAngle();
        
        //判断当前的角度是否达到目标角度
        if(cur_angle != aim_angle)
        {
            angle = cur_angle + (((aim_angle - cur_angle)*SWEETS_MAX_CHANGE_ANGLE)/abs(aim_angle - cur_angle));
            
            if((angle>=aim_angle&&aim_angle>cur_angle)
                ||(aim_angle>=angle&&cur_angle>aim_angle))
            {
                //则，证明已经到达最大值
                angle = aim_angle;
                SWEETS_SetChangeState(FALSE);
            }
        }
        else
        {
            angle = aim_angle;
            SWEETS_SetChangeState(FALSE);
        }
        
        SWEETS_SetAnimCurState(SWEETS_STATE_ROTATION_MOVE);
        SWEETS_Calculate(menu_ctrl_ptr,offset,angle,in_position);
        SWEETS_SetAnimCurState(SWEETS_STATE_SHAKE);
    }
    else
    {
        //定义设定刚开始shake的角度为最大角度
        angle = SWEETS_GetAnimAimAngle();
        
        if(s_sweets_menu_info.cur_frame>=s_sweets_menu_info.total_frame)
        {
            s_sweets_menu_info.cur_frame = s_sweets_menu_info.total_frame;
        }
        
        SCI_TRACE_LOW("[SWEETS]SWEETS_HandleShakeAnim  angle = %d ",angle);
        
        SWEETS_Calculate(menu_ctrl_ptr,offset,angle,in_position);
        
        s_sweets_menu_info.cur_frame++;
        
    }

    return TRUE;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SWEETS_HandleNoneAnim(
                                         MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                         )
{
    int16 angle = 0;
    uint16 offset = 0;
    IN_POSITON_DIRECTION_E in_position = 0;
    GUI_POINT_T  point = {0};
    MMI_TP_STATUS_E state = MMI_TP_NONE;
    
    angle = SWEETS_GetAnimCurAngle();
    
    if(menu_ctrl_ptr->is_tp_pressed)
    {
        #ifdef TOUCH_PANEL_SUPPORT
        MMK_GetLogicTPMsg(&state, &point);
        #endif
        //位移
        offset = abs(point.x - menu_ctrl_ptr->slide_start_point.x);
        
        //方向
        if(point.x> menu_ctrl_ptr->slide_start_point.x)
        {
            in_position = IN_POSITON_RIGHT;
        }
        else
        {
            in_position = IN_POSITON_LEFT;
        }
    }
    else
    {
        offset = 0;
        in_position = IN_POSITON_NONE;
    }
    
    SCI_TRACE_LOW("[SWEETS]SWEETS_HandleNoneAnim  angle = %d ",angle);
    
    SWEETS_Calculate(menu_ctrl_ptr,offset,angle,in_position);

    return TRUE;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_HandleRMAnim(
                               MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                               )
{
    int16 angle = 0;
    uint16 offset = 0;
    IN_POSITON_DIRECTION_E in_position = 0;
    int16 cur_angle = SWEETS_GetAnimCurAngle();
    int16 aim_angle = SWEETS_GetAnimAimAngle();
    GUI_POINT_T  point = {0};
    MMI_TP_STATUS_E state = MMI_TP_NONE;

    #ifdef TOUCH_PANEL_SUPPORT
    MMK_GetLogicTPMsg(&state, &point);
    #endif
    //判断较前一个点是否有移动
    if(point.x != menu_ctrl_ptr->slide_pre_point.x)
    {
        //位移
        offset = abs(point.x - menu_ctrl_ptr->slide_start_point.x);
        
        //方向
        if(point.x> menu_ctrl_ptr->slide_start_point.x)
        {
            aim_angle = -aim_angle;
            
            in_position = IN_POSITON_RIGHT;
            //滑了一段距离开始往回滑
            if(point.x<menu_ctrl_ptr->slide_pre_point.x)
            {
                aim_angle = -aim_angle;
            }
            
        }
        else
        {
            if(point.x>menu_ctrl_ptr->slide_pre_point.x)
            {
                aim_angle = -aim_angle;
            }
            in_position = IN_POSITON_LEFT;
        }
        
        
        
        //判断当前的角度是否达到目标角度
        if(cur_angle != aim_angle)
        {
            angle = cur_angle + (((aim_angle - cur_angle)*SWEETS_MAX_CHANGE_ANGLE)/abs(aim_angle - cur_angle));
            
            if((angle>=aim_angle&&aim_angle>cur_angle)
                ||(aim_angle>=angle&&cur_angle>aim_angle))
            {
                //则，证明已经到达最大值
                angle = aim_angle;
            }
        }
        else
        {
            angle = aim_angle;
        }
        
        SCI_TRACE_LOW("[SWEETS]SWEETS_HandleRMAnim  angle = %d ",angle);
        
        SWEETS_Calculate(menu_ctrl_ptr,offset,angle,in_position);
        
    }
    else
    {
        return;
    }
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_HandleSlideNoneAnim(
                                      MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                      )
{
    SWEETS_STATE_E cur_state = SWEETS_STATE_NONE;
    
    cur_state = SWEETS_GetAnimCurState();
    
    switch(cur_state)
    {
    case SWEETS_STATE_NONE:
        SWEETS_HandleNoneAnim(menu_ctrl_ptr);
        break;
    case SWEETS_STATE_SHAKE:
        SWEETS_HandleShakeAnim(menu_ctrl_ptr);
        break;
    case SWEETS_STATE_ENTER_ANIM:
        SWEETS_HandleEnterAnim(menu_ctrl_ptr);
        break;
    default:
        //不处理
        break;  
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_HandleSlideFlingAnim(
                                       MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                       )
{
    int16 angle = 0;
    uint16 offset = 0;
    IN_POSITON_DIRECTION_E in_position = 0;
    int16 cur_angle = SWEETS_GetAnimCurAngle();
    uint16           distance   = 0;//距离
    if(s_sweets_menu_info.cur_frame == s_sweets_menu_info.total_frame)
    {
        return;
    }
    
    
    //起始fling的距离
    distance = abs(menu_ctrl_ptr->slide_pre_point.x - menu_ctrl_ptr->slide_start_point.x);
    s_sweets_menu_info.total_frame = SWEETS_CHANGE_PAGE_TOTAL_FRAME_MAX - distance;
    if(s_sweets_menu_info.total_frame > 0)
    {
        
        s_sweets_menu_info.cur_frame += SWEETS_TPUP_STEP_PARAM*s_sweets_menu_info.total_frame/GDI_sqrt(s_sweets_menu_info.total_frame);
    }
    else
    {
        s_sweets_menu_info.total_frame = 0;
    }
    
    
    if(s_sweets_menu_info.cur_frame>=s_sweets_menu_info.total_frame)
    {
        s_sweets_menu_info.cur_frame = s_sweets_menu_info.total_frame;
    }
    
    offset = distance + s_sweets_menu_info.cur_frame;
    
    if(offset>=SWEETS_CHANGE_PAGE_TOTAL_FRAME_MAX)
    {
        offset = SWEETS_CHANGE_PAGE_TOTAL_FRAME_MAX;
    }
    
    //方向
    if(menu_ctrl_ptr->slide_pre_point.x> menu_ctrl_ptr->slide_start_point.x)
    {
        in_position = IN_POSITON_RIGHT;   
        angle = -cur_angle;
    }
    else
    {
        in_position = IN_POSITON_LEFT;
        angle = cur_angle;
    }
    
    SWEETS_Calculate(menu_ctrl_ptr,offset,angle,in_position);
    
    if(s_sweets_menu_info.cur_frame == s_sweets_menu_info.total_frame)
    {
        if(in_position == IN_POSITON_RIGHT)
        {
            SWEETS_UpdateItemLayer(menu_ctrl_ptr, TRUE);
        }
        else
        {
            SWEETS_UpdateItemLayer(menu_ctrl_ptr, FALSE);
        }
    }
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_HandleSlideRecoverAnim(
                                         MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                         )
{
    int16 angle = 0;
    uint16 offset = 0;
    IN_POSITON_DIRECTION_E in_position = 0;
    int16 aim_angle = SWEETS_GetAnimAimAngle();
    uint16           distance   = 0;//距离
    
    //起始fling的距离
    distance = abs(menu_ctrl_ptr->slide_pre_point.x - menu_ctrl_ptr->slide_start_point.x);
    s_sweets_menu_info.total_frame = distance;
    if(s_sweets_menu_info.total_frame > 0)
    {
        
        s_sweets_menu_info.cur_frame += SWEETS_TPUP_STEP_PARAM*s_sweets_menu_info.total_frame/GDI_sqrt(s_sweets_menu_info.total_frame);
    }
    else
    {
        s_sweets_menu_info.total_frame = 0;
    }
    
    if(s_sweets_menu_info.cur_frame>=s_sweets_menu_info.total_frame)
    {
        s_sweets_menu_info.cur_frame = s_sweets_menu_info.total_frame;
    }
    
    offset = distance - s_sweets_menu_info.cur_frame;
    
    if(offset>SWEETS_CHANGE_PAGE_TOTAL_FRAME_MAX)
    {
        offset = 0;
    }
    
    //方向
    if(menu_ctrl_ptr->slide_pre_point.x> menu_ctrl_ptr->slide_start_point.x)
    {
        in_position = IN_POSITON_RIGHT;
        angle = aim_angle;
    }
    else
    {
        in_position = IN_POSITON_LEFT;
        angle = -aim_angle;
    }
    
    SWEETS_Calculate(menu_ctrl_ptr,offset,angle,in_position);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SWEETS_SwitchStateHanlder(
                                     MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                     )
{
    SWEETS_STATE_E cur_state = SWEETS_STATE_NONE;
    
    if(menu_ctrl_ptr->is_tp_pressed)
    {
        cur_state = SWEETS_GetAnimCurState();
        
        switch(cur_state)
        {
        case SWEETS_STATE_NONE:
            SWEETS_HandleNoneAnim(menu_ctrl_ptr);
            break;
        case SWEETS_STATE_SHAKE:
            SWEETS_HandleShakeAnim(menu_ctrl_ptr);
            break;
        case SWEETS_STATE_ROTATION_MOVE:
            SWEETS_HandleRMAnim(menu_ctrl_ptr);
            break;
        default:   
            break;  
        }
        
    }
    else
    {
        switch(menu_ctrl_ptr->slide_state)
        {
        case MMK_TP_SLIDE_NONE:
            SWEETS_HandleSlideNoneAnim(menu_ctrl_ptr);
            break;
        case MMK_TP_SLIDE_FLING:
            SWEETS_HandleSlideFlingAnim(menu_ctrl_ptr);
            break;
        case MMK_TP_SLIDE_RECOVER:
            SWEETS_HandleSlideRecoverAnim(menu_ctrl_ptr);
            break;
        default:   
            break;  
        }
    }
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SWEETS_HandleEnterAnim(
                                          MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr 
                                          )
{
    uint32 * out_buffer = PNULL;
    uint32 * in_color_buffer = PNULL;
    IMGREF_SIZE_T win_size = {0};
 //   FIX16_POINT_T *Point_Add_ptr = PNULL;
    UILAYER_INFO_T layer_info_ptr = {0};
    static uint32 whole_time1=0,whole_time2=0;

    whole_time1=SCI_GetTickCount();
    SCI_TRACE_LOW("[SWEETS], enter cost_time ONE frame = %d ms", whole_time1-whole_time2);
    whole_time2=SCI_GetTickCount();
    
    //主层没有修改，能设置成immu
    UILAYER_SetDirectDraw( TRUE );
    UILAYER_SetMainLayerImmutable(TRUE);
    UILAYER_SwitchDoubleMem(&s_sweets_menu_info.dst_layer_handle,TRUE);
    whole_time1=SCI_GetTickCount();
    s_sweets_menu_info.total_frame = SWEETS_ENTER_ANIM_FRAME;
    
    if(s_sweets_menu_info.cur_frame>=s_sweets_menu_info.total_frame)
    {
        s_sweets_menu_info.cur_frame = s_sweets_menu_info.total_frame;
    }
    in_color_buffer = (uint32*)UILAYER_GetLayerBufferPtr(&s_sweets_menu_info.enter_anim_temp_layer_handle);
    out_buffer = (uint32*)UILAYER_GetLayerBufferPtr(&s_sweets_menu_info.dst_layer_handle);
    UILAYER_GetLayerInfo(&s_sweets_menu_info.dst_layer_handle,&layer_info_ptr);
    win_size.w = layer_info_ptr.layer_width;
    win_size.h = layer_info_ptr.layer_height;
    
    UILAYER_Clear(&s_sweets_menu_info.dst_layer_handle);
    SCI_TRACE_LOW("[SWEETS] ENTER init time = %d",SCI_GetTickCount() - whole_time1);

    whole_time1=SCI_GetTickCount();
    
    if(s_sweets_menu_info.Point_Add_ptr == PNULL
        ||in_color_buffer == PNULL
        ||out_buffer == PNULL
        )
    {
        SCI_TRACE_LOW("[SWEETS] PTR IS PNULL   ERROR!!!!!");
        return FALSE;
    }
    
    Glass_Sweets_Maginify_In(out_buffer,
                   in_color_buffer,
                   win_size,
                   win_size,
                   s_sweets_menu_info.cur_frame,
                    s_sweets_menu_info.total_frame,
                   s_sweets_menu_info.Point_Add_ptr
                   );

    SCI_TRACE_LOW("[SWEETS] ENTER calc time = %d",SCI_GetTickCount() - whole_time1);
    
    s_sweets_menu_info.cur_frame++;

    return TRUE;
}

/*****************************************************************************/
//  Description: start timer
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SweetsStartRedrawTimer(
                            MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr
                            )
{
    if (PNULL != menu_ctrl_ptr && 0 == menu_ctrl_ptr->redrew_timer_id)
    {
        menu_ctrl_ptr->redrew_timer_id = MMK_CreateWinTimer(
            menu_ctrl_ptr->ctrl_handle, 
            MMITHEME_MENU_REDRAW_TIME,
            FALSE);
    }

    return;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIMENU_SWEETS_HandleTimer(
                                              MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,
                                              MMI_MESSAGE_ID_E msg_id,		//事件类型
                                              DPARAM           param          //附带的处理参数
                                              )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    SWEETS_STATE_E cur_state = SWEETS_STATE_NONE;
    
    if(menu_ctrl_ptr->redrew_timer_id == *(uint8*)param)
    {
        //停止timer
        SweetsStopRedrawTimer(menu_ctrl_ptr);
        //开启timer
        SweetsStartRedrawTimer(menu_ctrl_ptr);
        
        //判断是否tp press
        if(menu_ctrl_ptr->is_tp_pressed)
        {
            GUI_POINT_T  point = {0};
            MMI_TP_STATUS_E state = MMI_TP_NONE;

            #ifdef TOUCH_PANEL_SUPPORT
            MMK_GetLogicTPMsg(&state, &point);
            #endif
            //先判断当前动画风格是否为NONE
            //获取当前动画风格
            cur_state = SWEETS_GetAnimCurState();
            
            if(SWEETS_STATE_NONE == cur_state)//现在处在TP down状态
            {
                //判断是否开始move  x方向
                if(abs(point.x - menu_ctrl_ptr->slide_pre_point.x)>SWEETS_START_MOVE_PIX)
                {
                    //设置状态为倾斜 move
                    SWEETS_SetAnimCurState(SWEETS_STATE_ROTATION_MOVE);
                }
                else//没有变化
                {
                    //do nothing
                }
            }
            //再判断当前动画风格是否为MOVE
            //获取当前动画风格
            else if(SWEETS_STATE_ROTATION_MOVE == cur_state)
            {
                //判断较前一个点是否有移动
                if(point.x == menu_ctrl_ptr->slide_pre_point.x)
                {
                    //设置状态为shake
                    s_sweets_menu_info.stop_move_num++;
                    if(s_sweets_menu_info.stop_move_num>=SWEETS_STOP_MOVE_NUM)
                    {
                        SWEETS_SetAnimCurState(SWEETS_STATE_SHAKE);
                        SWEETS_SetAnimAimAngle(SWEETS_TP_DOWN_SHAKE_ANGLE);
                        s_sweets_menu_info.stop_move_num = 0;
                        SWEETS_SetChangeState(TRUE);
                    }
                    
                }
            }           
            //再判断当前动画风格是否为SHAKE
            //获取当前动画风格
            else if(SWEETS_STATE_SHAKE == cur_state)
            {
                //判断较前一个点是否有移动
                if(abs(point.x - menu_ctrl_ptr->slide_pre_point.x)>SWEETS_START_MOVE_PIX)/*lint !e524*//*lint !e747*/
                {
                    //设置状态为倾斜 move
                    SWEETS_SetAnimCurState(SWEETS_STATE_ROTATION_MOVE);
                }
                else if(s_sweets_menu_info.cur_frame == s_sweets_menu_info.total_frame)
                {
                    //设置状态为NONE
                    SWEETS_SetAnimCurState(SWEETS_STATE_NONE);
                    s_sweets_menu_info.cur_frame = 0;
                }
            }
            
            SWEETS_SwitchStateHanlder(menu_ctrl_ptr);
            // 记录前一个点
            menu_ctrl_ptr->slide_pre_point.x = point.x;
            menu_ctrl_ptr->slide_pre_point.y = point.y;
        }
        else//TP没有按下的情况
        {
            //判断当前动画状态  NONE   FLING   RECOVER
            if(menu_ctrl_ptr->slide_state == MMK_TP_SLIDE_FLING)
            {
                if(s_sweets_menu_info.cur_frame >= s_sweets_menu_info.total_frame)
                {
                    //设置状态为shake
                    SWEETS_SetAnimCurState(SWEETS_STATE_SHAKE);
                    if(FLING_MIN_VELOCITY <= abs((int32)menu_ctrl_ptr->fling_velocity))
                    {
                        SWEETS_SetAnimAimAngle((SWEETS_IDLE_SHAKE_ANGLE+abs(menu_ctrl_ptr->fling_velocity)/SWEETS_VELOCITY_PARAM));/*lint !e524*//*lint !e747*/
                    }
                    else
                    {
                        SWEETS_SetAnimAimAngle(SWEETS_IDLE_SHAKE_ANGLE);
                    }
                    
                    menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
                    SWEETS_SetChangeState(TRUE);
                }
            }
            else if(menu_ctrl_ptr->slide_state == MMK_TP_SLIDE_RECOVER)
            {
                if(s_sweets_menu_info.cur_frame >= s_sweets_menu_info.total_frame)
                {
                    //设置状态为shake
                    SWEETS_SetAnimCurState(SWEETS_STATE_SHAKE);
                    SWEETS_SetAnimAimAngle(SWEETS_IDLE_SHAKE_ANGLE);
                    menu_ctrl_ptr->slide_state = MMK_TP_SLIDE_NONE;
                    SWEETS_SetChangeState(TRUE);
                }
            }
            else if(menu_ctrl_ptr->slide_state == MMK_TP_SLIDE_NONE)
            {
                cur_state = SWEETS_GetAnimCurState();
                
                if(SWEETS_STATE_SHAKE == cur_state)
                {
                    if(s_sweets_menu_info.cur_frame == s_sweets_menu_info.total_frame)
                    {
                        //设置状态为NONE
                        SWEETS_SetAnimCurState(SWEETS_STATE_NONE);
                        s_sweets_menu_info.cur_frame = 0;
                    }
                }
                else if(SWEETS_STATE_ENTER_ANIM == cur_state)
                {
                    if(s_sweets_menu_info.cur_frame == s_sweets_menu_info.total_frame)
                    {
                        //设置状态为SHAKE
                        SWEETS_EndEnterAnim(menu_ctrl_ptr);
                    }
                }
                
            }
            
            SWEETS_SwitchStateHanlder(menu_ctrl_ptr);
        } 
        
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MMIMENU_SWEETS_HandleModifyRect(
                                           MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr
                                           )
{
    MMIMENU_SWEETS_DestoryMenuInfo(menu_ctrl_ptr);
    MMIMENU_SWEETS_InitMenuInfo(menu_ctrl_ptr);
}


/*****************************************************************************/
// Description : 
// Global resource dependence : 
//  Author:
// Note:
/*****************************************************************************/
LOCAL uint16 MMIMENU_SWEETS_GetTotalItemNum(
                                            MMIMAINMENU_SWEETS_DATA_T *menu_ctrl_ptr,
                                            uint16 page_index
                                            )
{
    return 0;
}

LOCAL void Glass_Draw_Shadow(uint32 * Shadow_buffer,
						int16 text_w,
						int16 text_h,
						int16 win_w,
						int16 win_h,
						int16 icon_w,
						int16 icon_h,
						uint32 * out_buffer,
						IMGREF_POINT_T first_icon_location,
						IMGREF_POINT_T offset)
{	
    int draw_x=0,draw_y=0;
    int32 i=0,j=0;
    uint32 * dst_ptr=PNULL;
    uint32 * fill_ptr=PNULL;
    
    
    // text_w=103;
    // text_h=39;
    //return;
    if(PNULL == out_buffer)
    {
        return;
    }
    draw_x=	first_icon_location.x+(icon_w-text_w)/2+offset.x;
    draw_y=first_icon_location.y+offset.y+icon_h-text_h/2;
    if((draw_x>=0)&&(draw_x+text_w<win_w)&&(draw_y>=0)&&(draw_y+text_h<win_h))
    {
        for(i=0;i<text_h;i++)
        {     dst_ptr=& out_buffer[(draw_y+i)*win_w+draw_x];
        fill_ptr=&Shadow_buffer[i*text_w];
        for(j=0;j<text_w;j++)
        {        *dst_ptr++=*fill_ptr++;
        // out_buffer[(draw_y+i)*win_w+draw_x+j]=Shadow_buffer[i*text_w+j];
        }
        
        }  
    }
    else
    {
        for(i=0;i<text_h;i++)
        {       
            dst_ptr=& out_buffer[(draw_y+i)*win_w+draw_x];
            fill_ptr=&Shadow_buffer[i*text_w];
            for(j=0;j<text_w;j++)
            {	
                if((draw_x+j>=0)&&(draw_x+j<win_w)&&((draw_y+i)>=0)&&((draw_y+i)<win_h))
                {        *dst_ptr++=*fill_ptr++;
                //out_buffer[(draw_y+i)*win_w+draw_x+j]=Shadow_buffer[i*text_w+j];
                }
                else
                {
                    dst_ptr++;
                    fill_ptr++;
                }
            }
        }
    }
}

LOCAL void Glass_Draw_Txt(uint32 * text_buffer,
						int16 text_w,
						int16 text_h,
						int16 win_w,
						int16 win_h,
						
						int16 icon_h,
						uint32 * out_buffer,
						IMGREF_POINT_T first_icon_location,
						IMGREF_POINT_T offset)
{	
    int draw_x=0,draw_y=0;
    int32 i=0,j=0;
    int16 offset_y= -(SWEETS_TXT_OFFSET_DOWN_ICON);
    uint32 * dst_ptr=PNULL;
    uint32 * fill_ptr=PNULL;
    register uint32 temp1 = 0;
    register uint32 temp2 = 0;
    register uint32 temp3 = 0;
    register uint32 alpha1 = 0;
    register uint32 alpha2 = 0;
    
    
    
    draw_x=	first_icon_location.x+offset.x;
    draw_y=first_icon_location.y+offset.y+icon_h-offset_y;
    if((draw_x>=0)&&(draw_x+text_w<win_w)&&(draw_y>=0)&&(draw_y+text_h<win_h))
    {
        for(i=0;i<text_h;i++)
        {      dst_ptr=& out_buffer[((draw_y)+i)*win_w+(draw_x)];
        fill_ptr=&text_buffer[i*text_w];
        for(j=0;j<text_w;j++)
        {     
            
            temp1 = *fill_ptr++;
            if( 0xff000000 == temp1)
            {
                dst_ptr++;
            }
            else
            {       
                temp2= *dst_ptr;
                if(0xff000000 !=temp2)
                {
                    alpha1 = ((temp1>>24)&0xff);
                    alpha2 = ((temp2>>24)&0xff);
                    
                    temp3 = temp2&0x00ff00ff;
                    temp2 = temp2&0x0000ff00;
                    temp3 = ((temp3*alpha1)>>8)&0x00ff00ff;
                    temp2 = ((temp2*alpha1)>>8)&0x0000ff00;
                    alpha2 = ((alpha2*alpha1)<<16)&0xff000000;
                    
                    temp1 += temp2+temp3;
                    temp1 &= 0x00ffffff;
                    temp1 |= alpha2;
                    
                    *dst_ptr++= temp1;
                }
                else
                {
                    * dst_ptr++=temp1;
                }
            }
            
            
        }
        }
    }
    else
    {
        for(i=0;i<text_h;i++)
        {
            for(j=0;j<text_w;j++)
            {	
                if((draw_x+(int)(j)>=0)&&(draw_x+(int)(j)<win_w)&&((draw_y+(int)(i))>=0)&&((draw_y+(int)(i))<win_h))
                {       /*
                        upColor_R_B=text_buffer[i*text_w+j];
                        alpha=upColor_R_B>>24;
                        if(255==alpha)
                        {
                        out_buffer[(draw_y+i)*win_w+draw_x+j]=upColor_R_B;
                        }
                        else if(0!=alpha)
                        {   
                        downColor_R_B= out_buffer[(draw_y+i)*win_w+draw_x+j];
                        if(0==downColor_R_B)
                        {
                        out_buffer[(draw_y+i)*win_w+draw_x+j]=upColor_R_B;
                        }
                        else
                        {
                        upColor_G=upColor_R_B&0x0000ff00;
                        upColor_R_B=upColor_R_B&0x00ff00ff;
                        
                          downColor_G=downColor_R_B&0x0000ff00;
                          downColor_R_B=downColor_R_B&0x00ff00ff;
                          upColor_R_B=((upColor_R_B*alpha+downColor_R_B*(255-alpha))&0xff00ff00)>>8;
                          upColor_G=((upColor_G*alpha+downColor_G*(255-alpha))&0x00ff0000)>>8;
                          out_buffer[(draw_y+i)*win_w+draw_x+j]=((alpha<<24))|(upColor_R_B)|(upColor_G);
                          }
                          }
                    */
                    temp2 =  out_buffer[(draw_y+i)*win_w+draw_x+j];
                    if( 0xff000000 == temp2)
                    {
                        out_buffer[(draw_y+i)*win_w+draw_x+j]= text_buffer[i*text_w+j];
                    }
                    else
                    {
                        temp1=text_buffer[i*text_w+j];
                        if(0xff000000 !=temp1)
                        {
                            alpha1 = ((temp1>>24)&0xff);
                            alpha2 = ((temp2>>24)&0xff);
                            
                            temp3 = temp2&0x00ff00ff;
                            temp2 = temp2&0x0000ff00;
                            temp3 = ((temp3*alpha1)>>8)&0x00ff00ff;
                            temp2 = ((temp2*alpha1)>>8)&0x0000ff00;
                            alpha2 = ((alpha2*alpha1)<<16)&0xff000000;
                            
                            temp1 += temp2+temp3;
                            temp1 &= 0x00ffffff;
                            temp1 |= alpha2;
                            
                            out_buffer[(draw_y+i)*win_w+draw_x+j] = temp1;
                        }
                    }
                }
            }
        }
    }
}


void Glass_Fill_Ball_Color(FILLBALL_INPARAM_T * fill_in_data_param)
{
    int x=0,data_num=0,x_start=0,x_end=0;
    int32 calc_y=0;
    int32 offset_x=0,offset_y=0;
    //register uint16 * dst_icon_ptr=PNULL;
    register uint32 * src_ptr=PNULL;
    uint32 * glass_ptr=PNULL;
    uint16 * current_shine_ptr=PNULL;
    uint32 * location_ptr=PNULL;
    register uint8 * edge_alpha=0;
    register uint8 c_alpha=0;
    //uint32 Color_up1=0,Color_up2=0,Color_up3=0,Color_down1=0,Color_down2=0,Color_down3=0;
    int16 win_h=0,win_w=0;
    uint16 ptr_offset=0, ptr_back_offset=0;
    uint32 * Dst_Shine_Current =PNULL ;
    
    
    register uint temp1 = 0;
    register uint temp2 = 0;
    register uint temp3 = 0;
    register uint32 alpha1 = 0;
    register uint32 alpha2 = 0;
    
    if(PNULL==fill_in_data_param||PNULL == fill_in_data_param->out_shine_layer)
    {	
        return;
    }
    
    win_w=fill_in_data_param->dst_width;
    win_h=fill_in_data_param->dst_height;
    data_num=0;
    edge_alpha=s_edge_alpha;
    src_ptr=fill_in_data_param->icon_buffer;
    glass_ptr=fill_in_data_param->glass_ptr;
    location_ptr=s_src_p_location_table;
    current_shine_ptr=s_src_shine_table;
    offset_x=fill_in_data_param->offset.x;
    offset_y=fill_in_data_param->offset.y;
    
    while(data_num<=Data_Num)
    {	
        calc_y=s_edge_table_fix[data_num].y+offset_y;
        if((calc_y>0)&&(calc_y<win_h))
        {
            x_start=s_edge_table_fix[data_num].x+offset_x;
            x_end=s_edge_table_fix[data_num+1].x+offset_x;
            if((x_start>=win_w)||(x_end<=0))
            {
                ptr_offset=s_edge_table_fix[data_num+1].x-s_edge_table_fix[data_num].x;
                edge_alpha+=ptr_offset;
                
                location_ptr+=ptr_offset;
                current_shine_ptr+=ptr_offset;
                
            }
            else
            {
                if(x_start<0)
                {
                    ptr_offset=0-x_start;
                    edge_alpha+=ptr_offset;
                    
                    location_ptr+=ptr_offset;
                    current_shine_ptr+=ptr_offset;
                    
                    x_start=0;
                }
                else if(x_end>win_w)
                {
                    ptr_back_offset=x_end-(win_w);
                    x_end=(win_w);
                }
                //dst_icon_ptr=&fill_in_data_param->out_icon_layer[(calc_y)*win_w+x_start];
                
                Dst_Shine_Current=&fill_in_data_param->out_shine_layer[(calc_y)*win_w+x_start];
                
                //第一个像素和最后一个像素消锯齿
                
                for(x=x_start;x<x_end;x++)
                    
                {	 	
                    c_alpha=* edge_alpha ++;
                    
                    
                    if(c_alpha==255)
                    {
                        temp2=src_ptr[(* location_ptr++)];
                        //  Dst_Shine_Current++=src_ptr[(* location_ptr++)];
                        
                    }
                    
                    else
                    {     //temp2 在下temp1在上
                        temp2=*Dst_Shine_Current;
                        temp1=src_ptr[(* location_ptr++)];
                        if(0xff000000==temp1)
                        {
                        }
                        else
                        {
                            
                            temp3 = temp1&0x00ff00ff;
                            temp1 = temp1&0x0000ff00;
                            temp3 = ((temp3*c_alpha)>>8)&0x00ff00ff;
                            temp1 = ((temp1*c_alpha)>>8)&0x0000ff00;
                            temp1=temp1+temp3;
                            
                            alpha1=255-c_alpha;
                            alpha2 = ((temp2>>24)&0xff);
                            
                            temp3 = temp2&0x00ff00ff;
                            temp2 = temp2&0x0000ff00;
                            temp3 = ((temp3*alpha1)>>8)&0x00ff00ff;
                            temp2 = ((temp2*alpha1)>>8)&0x0000ff00;
                            alpha2 = ((alpha2*alpha1)<<16)&0xff000000;
                            
                            temp1 += temp2+temp3;
                            temp1 &= 0x00ffffff;
                            temp1 |= alpha2;
                            temp2=temp1;
                            
                        }
                    }
                    
                    // * Dst_Shine_Current++=temp2;
                    
                    temp1=*current_shine_ptr++;
                    if(0!=temp1)
                    {
                        temp1 = glass_ptr[temp1];
                        
                        if( 0xff000000==temp1)
                        {
                            * Dst_Shine_Current++=temp2;
                        }
                        else
                        {
                            alpha1 = ((temp1>>24)&0xff);
                            alpha2 = ((temp2>>24)&0xff);
                            
                            temp3 = temp2&0x00ff00ff;
                            temp2 = temp2&0x0000ff00;
                            temp3 = ((temp3*alpha1)>>8)&0x00ff00ff;
                            temp2 = ((temp2*alpha1)>>8)&0x0000ff00;
                            alpha2 = ((alpha2*alpha1)<<16)&0xff000000;
                            
                            temp1 += temp2+temp3;
                            temp1 &= 0x00ffffff;
                            temp1 |= alpha2;
                            * Dst_Shine_Current++=temp1;
                        }
                    }
                    else
                    {
                        * Dst_Shine_Current++=temp2;
                    }
                    
                }
                
                edge_alpha+=ptr_back_offset;
                
                location_ptr+=ptr_back_offset;
                
                current_shine_ptr+=ptr_back_offset;
                ptr_back_offset=0;
                        }
                    }
                    else
                    {	ptr_offset=s_edge_table_fix[data_num+1].x-s_edge_table_fix[data_num].x;
                    edge_alpha+=ptr_offset;
                    
                    location_ptr+=ptr_offset;
                    
                    current_shine_ptr+=ptr_offset;
                    }
                    data_num+=2;
        }
        
}



//用与轴线角度消锯齿
void Glass_Creat_ET_Out(BALL_DATA_PARAM_ARGB_T * in_data_param)//control 精度到4096
{
    int x=0,y=0,radius=0,i=0,angle_longi=0;
    int center_x=0,center_y=0,center_z=0;
    int Muti_Ro_X=0,Muti_Ro_Y=0,Muti_Ro_Z=0; 
    int Muti_Eq_X=0,Muti_Eq_Y=0,Muti_Eq_Z=0; 
    int Muti_w_X=0,Muti_w_Y=0,Muti_w_Z=0; 
    int32 equator=0,half_chord=0,calc_z=0;
    int win_width=0,win_height=0,map_width=0,map_height=0;
    int tempSqrt=0,RemainSqrt=0;
    int  Vec_Ro_x=0,Vec_Ro_y=0,Vec_Ro_z=0;//0A的
    int  Vec_Eq_x=0,Vec_Eq_y=0,Vec_Eq_z=0;
    int  Vec_Op_x=0,Vec_Op_y=0,Vec_Op_z=0;
    int  Cross_Pro_x=0,Cross_Pro_y=0,Cross_Pro_z=0;
    int circle_x=0,circle_y=0;
    int glass_x=0,glass_y=0;
    int offset_glass=0;
    int32 ball_centre_x=0,ball_centre_y=0,ball_centre_z=0;
    int Reci_Radius=0,Cos_Angle_Lati=0;
    int Cos_Angle_Longi=0,Sum_Eq=0,Re_Front_Back=0,Cos_Angle_Cross=0;
    int Angle_Cross=0,Pre_Angle_Cross=0;
    BOOLEAN Is_In_Spheroid=FALSE;
    BOOLEAN Pre_Is_In=FALSE;	
    register uint32 * src_p_position=PNULL;
    IMGREF_POINT_T * current_edge_table=PNULL;
    uint16 * current_shine_ptr=PNULL;
    uint16 data_num=0;
    //uint32 p_z_num=0;
    //消锯齿只支持左右晃动时消锯齿。
    
    int16 border_angle= INANGLE/2-ALIAS_ANGLE;
    uint8 * current_edge_alpha=PNULL;
    
    int Sqrt_three=0;
    //antialias end 
    offset_glass=in_data_param->offset_centre;
    center_x=in_data_param->Ball_centre.x>>12;
    center_y=in_data_param->Ball_centre.y>>12;
    center_z=in_data_param->Ball_centre.z>>12;
    ball_centre_x=in_data_param->Ball_centre.x;
    ball_centre_z=in_data_param->Ball_centre.z;
    ball_centre_y=in_data_param->Ball_centre.y;
    if((2*in_data_param->Out_Size.h+abs(in_data_param->Ball_centre.z))<=0)
    {
        return;
    }
    
    equator=in_data_param->equator;
    
    radius=equator/2;
    if(0==radius)
    {
        return;
    }
    
    win_width=in_data_param->Out_Size.w;
    win_height=in_data_param->Out_Size.h;
    map_width=in_data_param->map_Size.w;
    map_height=in_data_param->map_Size.h;
    
    
    Vec_Ro_x=in_data_param->vector_ro_axis.x;
    Vec_Ro_y=in_data_param->vector_ro_axis.y;
    Vec_Ro_z=in_data_param->vector_ro_axis.z;
    Vec_Eq_x=in_data_param->vector_eq_axis.x;
    Vec_Eq_y=in_data_param->vector_eq_axis.y;
    Vec_Eq_z=in_data_param->vector_eq_axis.z;
    //通过自转轴和0度赤道轴的外积去求出东京90度的向量坐标。
    Cross_Pro_x=(Vec_Ro_y*Vec_Eq_z-Vec_Ro_z*Vec_Eq_y)>>12;
    Cross_Pro_y=(Vec_Ro_z*Vec_Eq_x-Vec_Ro_x*Vec_Eq_z)>>12;
    Cross_Pro_z=(Vec_Ro_x*Vec_Eq_y-Vec_Ro_y*Vec_Eq_x)>>12;
    Reci_Radius=4096/(radius);
    src_p_position=s_src_p_location_table;
    //src_p_z=s_src_p_z_table;
    current_edge_table=s_edge_table_fix;
    current_edge_alpha=s_edge_alpha;
    current_shine_ptr=s_src_shine_table;
    Sqrt_three=GDI_sqrt(3*65536);
    while(i<4000)
    {
        i++;
        * current_shine_ptr++=0;
    }
    current_shine_ptr=s_src_shine_table;
    i=0;
    for(y=0;y<2*radius+1;y++)
    {	
        //half_chord=(((radius*s2d_cos(Asin_Table_Fix[abs(radius-y)*Reci_Radius/8]))>>9)>>8);
        half_chord=((radius*s2d_cos(Asin_Table_Fix[abs(radius-y)*Reci_Radius/8]>>9))>>8);
        s_half_chord_array[y]=half_chord;
    }
    for(y=0;y<2*radius+1;y++)
    {	
        
        //Vec_Op_y=(-(equator)/2)+y;
        //Vec_Op_y=Vec_Op_y*Reci_Radius;
        Vec_Op_y=((((center_y-radius+y)*4096-ball_centre_y)*Reci_Radius)>>12);
        tempSqrt = 16777216 - Vec_Op_y*Vec_Op_y;
        //half_chord=(((radius*s2d_cos(Asin_Table_Fix[abs(radius-y)*Reci_Radius/8]))>>9)>>16);
        half_chord=((radius*s2d_cos(Asin_Table_Fix[abs(radius-y)*Reci_Radius/8]>>9))>>16);
        if(0==half_chord)
        {
            continue;
        }
        Muti_Ro_Y=Vec_Op_y*Vec_Ro_y;
        Muti_Eq_Y=Vec_Op_y*Vec_Eq_y;
        Muti_w_Y=Vec_Op_y*Cross_Pro_y;
        Is_In_Spheroid=FALSE;
        Pre_Is_In=FALSE;
        for(x=0;x<2*half_chord+1;x++)
        {	
            
            Vec_Op_x=((((center_x-half_chord+x)*4096-ball_centre_x)*Reci_Radius)>>12);
            RemainSqrt=tempSqrt-Vec_Op_x*Vec_Op_x;
            if(RemainSqrt<0)
            {
                Vec_Op_z=0;
            }
            else
            {
                Vec_Op_z =- (ball_sqrt[RemainSqrt>>14]);
            }
            calc_z=(Vec_Op_z*radius/4096)+center_z;
            
            
            Muti_Eq_X=Vec_Op_x*Vec_Eq_x;
            Muti_Eq_Z=Vec_Op_z*Vec_Eq_z;
            Muti_w_X=Vec_Op_x*Cross_Pro_x;
            Muti_w_Z=Vec_Op_z*Cross_Pro_z;
            
            
            
            Sum_Eq=(Muti_Eq_X+Muti_Eq_Y+Muti_Eq_Z)>>8;
            Re_Front_Back=(Muti_w_X+Muti_w_Y+Muti_w_Z);
            Cos_Angle_Cross=Re_Front_Back>>15;
            if(Cos_Angle_Cross>0)
            {	
                if(Cos_Angle_Cross>512)
                {
                    Cos_Angle_Cross=512;
                }
                
                Angle_Cross=900-(Asin_Table_Fix[Cos_Angle_Cross]>>9);
                
                if(Angle_Cross<INANGLE/2)
                {
                    Is_In_Spheroid=TRUE;
                }
            }
            if((Is_In_Spheroid!=Pre_Is_In))
            {
                current_edge_table->x=x+center_x-half_chord;
                current_edge_table->y=center_y-radius+y;
                
                data_num++;
                current_edge_table++;
                //in_data_param->out_icon_buffer[(center_y-radius+y)*win_width+x+center_x-half_chord+1]=0xffff0000;
                //in_data_param->out_icon_buffer[(center_y-radius+y)*win_width+x+center_x-half_chord]=0xffff0000;
            }
            else if((TRUE==Is_In_Spheroid)&&(x==2*half_chord))
            {
                current_edge_table->x=x+center_x-half_chord+1;
                current_edge_table->y=center_y-radius+y;
                
                data_num++;
                current_edge_table++;
                //in_data_param->out_icon_buffer[(center_y-radius+y)*win_width+x+center_x-half_chord+1]=0xffff0000;
                //in_data_param->out_icon_buffer[(center_y-radius+y)*win_width+x+center_x-half_chord]=0xffff0000;
            }
            Pre_Angle_Cross=Angle_Cross;
            if(Is_In_Spheroid)
            {	
                if(Angle_Cross>border_angle)
                {
                    * current_edge_alpha++=(ALIAS_ANGLE-(Angle_Cross-border_angle))*255/ALIAS_ANGLE;
                    
                }
                else
                {
                    * current_edge_alpha++=255;
                }
                Muti_Ro_X=Vec_Op_x*Vec_Ro_x;
                Muti_Ro_Z=Vec_Op_z*Vec_Ro_z;
                Cos_Angle_Longi=((Muti_Ro_X+Muti_Ro_Y+Muti_Ro_Z)>>15);
                if(Cos_Angle_Longi>0)
                {	
                    if(Cos_Angle_Longi>512)
                    {
                        Cos_Angle_Longi=512;
                    }
                    
                    angle_longi=900-(Asin_Table_Fix[Cos_Angle_Longi]>>9);
                }
                else
                {
                    if(Cos_Angle_Longi<-512)
                    {
                        Cos_Angle_Longi=-512;
                    }
                    
                    angle_longi=900+(Asin_Table_Fix[-Cos_Angle_Longi]>>9);
                }
                if(angle_longi>900)
                {
                    angle_longi=1800-angle_longi;
                }
                Cos_Angle_Lati=(((Sum_Eq)*Reci_Sintable_temp[angle_longi])>>15);
                
                circle_x=((int)(map_width)*65536-s_half_chord_array[y]*(Cos_Angle_Lati))>>17;
                circle_y=((int)(map_height/2)-(radius-y));
                glass_x=	circle_x+(2*GLASS_W-map_width)/2+offset_glass*Sqrt_three/(2*256);
                glass_y=	circle_y+(2*GLASS_H-map_height)/2+offset_glass/2;
                if((glass_x<GLASS_W)&&(glass_y<GLASS_H))
                {
                    * current_shine_ptr++ =glass_y*GLASS_W+glass_x;
                }
                else
                {
                    current_shine_ptr++ ;
                }
                * src_p_position++ =circle_y*map_width+circle_x;
                
                
                //*src_p_z++=calc_z;
                //P_z_Num++;
                //p_z_num++;		
            }
            Pre_Is_In=Is_In_Spheroid;
            Is_In_Spheroid=FALSE;
            
            
                }
        }
        Data_Num=data_num;
        //P_z_Num=p_z_num;
        
}
/*****************************************************************************/
// Description : 
// Global resource dependence : 
//  Author:
// Note:
/*****************************************************************************/
LOCAL void Glass_Get_3D_Points(IMGREF_LONG_3D_POINT_T * Out_ball_cntre,
										int equator,
										int d_alpha,
										IMGREF_POINT_T icon_location)
{	
    IMGREF_LONG_3D_POINT_T Ball_centre={0};
    IMGREF_LONG_POINT_T fix_ro={0};
    IMGREF_LONG_POINT_T fix_temp={0};
#ifdef	ROEW
    Ball_centre.x=icon_location.x;
    Ball_centre.z=0;
    fix_ro.x=Ball_centre.x;
    //fix_ro.y=Ball_centre.z-(int)((float)(equator)/2*0.707);
    fix_ro.y=Ball_centre.z-((equator/2)*s2d_cos_new(INANGLE/2)>>16);/*lint !e504*/
    fix_temp.x=(Ball_centre.x-fix_ro.x);
    fix_temp.y=(Ball_centre.z-fix_ro.y);
    
    Out_ball_cntre->x=(fix_ro.x*4096+fix_temp.x*s2d_cos_new(d_alpha)/16-fix_temp.y*s2d_sin_new(d_alpha)/16);
    Out_ball_cntre->z=(fix_ro.y*4096+fix_temp.x*s2d_sin_new(d_alpha)/16+fix_temp.y*s2d_cos_new(d_alpha)/16);
    Out_ball_cntre->y=(int32)(icon_location.y)*4096;
#endif
#ifdef	RONS
    Ball_centre.x=icon_location.x;
    Ball_centre.y=icon_location.y;
    Ball_centre.z=0;
    fix_ro.x=Ball_centre.y;
    //fix_ro.y=Ball_centre.z-equator/2*0.707;
    fix_ro.y=Ball_centre.z-((equator/2)*s2d_cos_new(INANGLE/2)>>16);
    fix_temp.x=(Ball_centre.y-fix_ro.x);
    fix_temp.y=(Ball_centre.z-fix_ro.y);
    
    Out_ball_cntre->y=(fix_ro.x*4096+fix_temp.x*s2d_cos_new(d_alpha)/16-fix_temp.y*s2d_sin_new(d_alpha)/16);
    Out_ball_cntre->z=(fix_ro.y*4096+fix_temp.x*s2d_sin_new(d_alpha)/16+fix_temp.y*s2d_cos_new(d_alpha)/16);
    Out_ball_cntre->x=Ball_centre.x*4096;
#endif
}
void Glass_Object_Swing(	int16 * angle,
						int frame,
						int total_frame,
						int max_angle)
{    
    int32 act_times=12;
    int32 stop_point_x=0;
    int32 current_x=0;
    int32 amplitude=0;  
    if(max_angle>=900)
    {
        max_angle=900;
    }
    else if(max_angle<=-900)
    {
        max_angle=-900;
    }
    
    if(max_angle>=0)
    {
        stop_point_x=1800*act_times+900;
        
    }
    else
    {
        stop_point_x=1800*act_times-900;
    }
    
    
    amplitude=10*1800*256*7/22*max_angle/(stop_point_x*(s2d_sin_new(stop_point_x)>>8));
    SCI_TRACE_LOW("[SWEETS] amplitude  %d",amplitude);
    current_x=stop_point_x*frame/total_frame;
    * angle=amplitude*(stop_point_x-current_x)*(22)/(10*1800*7)*(s2d_sin_new(stop_point_x-current_x)>>8)/256;
    //* angle=(amplitude*(stop_point_x-current_x)*(3.1415926)/(100*1800)*s2d_sin_new((stop_point_x-current_x))*10/65536);
    SCI_TRACE_LOW("[SWEETS] out_angle  %d",* angle);
    
}
PUBLIC void Glass_CandyType_Menu(SWEETS_INPARAM_T * inparam_data)
{
    int16 current_angle=0,d_angle=0;
    uint32 i=0; 
    static uint32 tick_time1=0;
    IMGREF_POINT_T current_location={0};
    BALL_DATA_PARAM_ARGB_T creat_ball_in_param={0};
    FILLBALL_INPARAM_T  fill_color_in_param={0};
    int16 icon_w=0,icon_h=0;
    int16 win_h=0,win_w=0;
    BOOLEAN Is_in_Screen=FALSE;
    int16 act_radius=0;
    //TEST
    
    IMGREF_SIZE_T bg_window={0};
    
    
    
    //
    if(PNULL==inparam_data||PNULL == inparam_data->out_shine_layer)
    {
        return;
    }
    
    bg_window.w = inparam_data->dst_width;
    bg_window.h = inparam_data->dst_height;
    icon_w=inparam_data->icon_w;
    icon_h=inparam_data->icon_h;
    win_w=inparam_data->dst_width;
    win_h=inparam_data->dst_height;
   // act_radius=(int)((EQUATER)*1.414/4);
    act_radius=(int)(((EQUATER/2)*s2d_sin_new(INANGLE/2))>>16);
    switch(inparam_data->cur_state)
    {
    case SWEETS_STATE_SHAKE:
        if(0==inparam_data->total_frame)
        {
            return;
        }
        
        Glass_Object_Swing(&current_angle,
            inparam_data->cur_frame,
            inparam_data->total_frame,
            inparam_data->angle);
        current_angle=current_angle+1800;
        d_angle=current_angle-1800;
        SCI_TRACE_LOW("[SWEETS] CANDY  %d ,%d, %d, %d",inparam_data->angle,d_angle,inparam_data->cur_frame,inparam_data->total_frame);
        break;
        
        
    case SWEETS_STATE_ROTATION_MOVE:
        current_angle=inparam_data->angle+1800;
        d_angle=current_angle-1800;
        break;
    case SWEETS_STATE_NONE:
        current_angle= 1800;
        d_angle=0;
        break;
    default: 
        current_angle= 1800;
        d_angle=0;
        break;
    }
    creat_ball_in_param.vector_eq_axis.x=s2d_cos_new(current_angle)>>4;
    creat_ball_in_param.vector_eq_axis.y=0;
    creat_ball_in_param.vector_eq_axis.z=s2d_sin_new(current_angle)>>4;
    creat_ball_in_param.vector_ro_axis.x=0;
    creat_ball_in_param.vector_ro_axis.y=-4096;
    creat_ball_in_param.vector_ro_axis.z=0;
    /*
    creat_ball_in_param.vector_eq_axis.x=-4096;
    creat_ball_in_param.vector_eq_axis.y=0;
    creat_ball_in_param.vector_eq_axis.z=0;
    creat_ball_in_param.vector_ro_axis.x=0;
    creat_ball_in_param.vector_ro_axis.y=s2d_cos_new(current_angle)>>4;
    creat_ball_in_param.vector_ro_axis.z=s2d_sin_new(current_angle)>>4;
    */
    creat_ball_in_param.Out_Size.w=inparam_data->dst_width;
    creat_ball_in_param.Out_Size.h=inparam_data->dst_height;
    creat_ball_in_param.equator=EQUATER;
    
    creat_ball_in_param.icon_location=inparam_data->icon_location;
    creat_ball_in_param.map_Size.w=inparam_data->icon_w;
    creat_ball_in_param.map_Size.h=inparam_data->icon_h;
    
    tick_time1=SCI_GetTickCount();
    current_location.x=inparam_data->icon_location[0].x+act_radius;
    current_location.y=inparam_data->icon_location[0].y+act_radius;	
    
    Glass_Get_3D_Points(&creat_ball_in_param.Ball_centre,  EQUATER,  d_angle, current_location);
    creat_ball_in_param.offset_centre=current_location.x-(creat_ball_in_param.Ball_centre.x>>12);
    Glass_Creat_ET_Out(&creat_ball_in_param);
    
    * (inparam_data->out_current_angle )=d_angle;
    SCI_TRACE_LOW("YOON_Creat_ET, cost_time ONE frame = %d ms", SCI_GetTickCount()-tick_time1);
    
    fill_color_in_param.dst_height=inparam_data->dst_height;
    fill_color_in_param.dst_width=inparam_data->dst_width;
    fill_color_in_param.out_icon_layer=(inparam_data->out_icon_layer);
    fill_color_in_param.out_shine_layer=(inparam_data->out_shine_layer);
    fill_color_in_param.radius=EQUATER/2;
    tick_time1=SCI_GetTickCount();
    /*
    for(h=0;h<100;h++)
    {
    for(j=0;j<100;j++)
				{
                inparam_data->out_icon_layer[h*win_w+j]=(inparam_data->glass_color_ptr)[h*icon_w+j];
                }
                }
    */
    //   S2D_Blending( bg_window,  inparam_data->out_icon_layer, up_window ,  offset,inparam_data->glass_color_ptr ,inparam_data->out_icon_layer);
    for(i=0;i<inparam_data->cur_icon_num;i++)
        //for(i=1;i<2;i++)
        
    {	Is_in_Screen=FALSE;
    
    if(IN_POSITON_RIGHT==inparam_data->in_position)
    {
        fill_color_in_param.offset.x=inparam_data->icon_location[i].x-inparam_data->icon_location[0].x+inparam_data->c_page_offset;
        fill_color_in_param.offset.y=inparam_data->icon_location[i].y-inparam_data->icon_location[0].y;
        //if((fill_color_in_param.offset.x+inparam_data->icon_location[0].x-act_radius)<win_w)
        if((fill_color_in_param.offset.x+inparam_data->icon_location[0].x)<win_w)
        {
            Is_in_Screen=TRUE;
        }
    }
    else if(IN_POSITON_LEFT==inparam_data->in_position)
				{
        fill_color_in_param.offset.x=inparam_data->icon_location[i].x-inparam_data->icon_location[0].x-inparam_data->c_page_offset;
        fill_color_in_param.offset.y=inparam_data->icon_location[i].y-inparam_data->icon_location[0].y;
        //if((fill_color_in_param.offset.x+inparam_data->icon_location[0].x+act_radius)>=0)
        if((fill_color_in_param.offset.x+inparam_data->icon_location[0].x+2*act_radius)>=0)
        {
            Is_in_Screen=TRUE;
        }
				}
    else
				{	//默认位置时 当前页显示
        fill_color_in_param.offset.x=inparam_data->icon_location[i].x-inparam_data->icon_location[0].x;
        fill_color_in_param.offset.y=inparam_data->icon_location[i].y-inparam_data->icon_location[0].y;
        Is_in_Screen=TRUE;
				}
    
    if(Is_in_Screen)
				{
        fill_color_in_param.centre_p.x=(creat_ball_in_param.Ball_centre.x>>12);
        fill_color_in_param.centre_p.y=(creat_ball_in_param.Ball_centre.y>>12);
        fill_color_in_param.centre_p.z=(creat_ball_in_param.Ball_centre.z>>12);
        //透明玻璃
        fill_color_in_param.glass_ptr=inparam_data->glass_color_ptr;
        //
        fill_color_in_param.icon_buffer=inparam_data->current_page_icon_buf+i*inparam_data->icon_w*inparam_data->icon_h;
        Glass_Draw_Shadow(inparam_data->shadow_buf, 
            inparam_data->shadow_w, 
            inparam_data->shadow_h, 
            win_w, 
            win_h, 
            icon_w,
            icon_h, 
            inparam_data->out_shine_layer, 
            inparam_data->icon_location[0], 
            fill_color_in_param.offset);
        if(SWEETS_STATE_ENTER_ANIM==inparam_data->cur_state)
        {
            // Glass_Fill_Ball_Color_INIT(&fill_color_in_param);
            Glass_Fill_Ball_Color(&fill_color_in_param);
        }
        else
        {
            Glass_Fill_Ball_Color(&fill_color_in_param);
            
            
        }
        //画图标文字的部分
        
        Glass_Draw_Txt(inparam_data->cur_text_buf+i*inparam_data->text_width*inparam_data->text_height, 
            inparam_data->text_width, 
            inparam_data->text_height, 
            win_w, 
            win_h, 
            icon_h, 
            inparam_data->out_shine_layer, 
            inparam_data->icon_location[0], 
            fill_color_in_param.offset);
        
        
				}
    }
    for(i=0;i<inparam_data->next_icon_num;i++)
    {	Is_in_Screen=FALSE;
    if(IN_POSITON_RIGHT==inparam_data->in_position)
    {
        fill_color_in_param.offset.x=inparam_data->icon_location[i].x-inparam_data->icon_location[0].x+inparam_data->c_page_offset-inparam_data->dst_width;
        fill_color_in_param.offset.y=inparam_data->icon_location[i].y-inparam_data->icon_location[0].y;
        //if((fill_color_in_param.offset.x+inparam_data->icon_location[0].x-act_radius)<win_w)
        if((fill_color_in_param.offset.x+inparam_data->icon_location[0].x)<win_w)
        {
            Is_in_Screen=TRUE;
        }
    }
    else if(IN_POSITON_LEFT==inparam_data->in_position)
				{
        fill_color_in_param.offset.x=inparam_data->icon_location[i].x-inparam_data->icon_location[0].x-inparam_data->c_page_offset+inparam_data->dst_width;
        fill_color_in_param.offset.y=inparam_data->icon_location[i].y-inparam_data->icon_location[0].y;
        //if((fill_color_in_param.offset.x+inparam_data->icon_location[0].x+act_radius)>=0)
        if((fill_color_in_param.offset.x+inparam_data->icon_location[0].x+2*act_radius)>=0)
        {
            Is_in_Screen=TRUE;
        }
				}
    else
				{	//默认位置时 下一页不显示
        fill_color_in_param.offset.x=inparam_data->icon_location[i].x-inparam_data->icon_location[0].x;
        fill_color_in_param.offset.y=inparam_data->icon_location[i].y-inparam_data->icon_location[0].y;
        Is_in_Screen=FALSE;
				}
    if(Is_in_Screen)
				{
        fill_color_in_param.centre_p.x=(creat_ball_in_param.Ball_centre.x>>12);
        fill_color_in_param.centre_p.y=(creat_ball_in_param.Ball_centre.y>>12);
        fill_color_in_param.centre_p.z=(creat_ball_in_param.Ball_centre.z>>12);
        //透明玻璃
        fill_color_in_param.glass_ptr=inparam_data->glass_color_ptr;
        //
        fill_color_in_param.icon_buffer=inparam_data->next_page_icon_buf+i*inparam_data->icon_w*inparam_data->icon_h;
        Glass_Draw_Shadow(inparam_data->shadow_buf, 
            inparam_data->shadow_w, 
            inparam_data->shadow_h, 
            win_w, 
            win_h, 
            icon_w,
            icon_h, 
            inparam_data->out_shine_layer, 
            inparam_data->icon_location[0], 
            fill_color_in_param.offset);
        Glass_Fill_Ball_Color(&fill_color_in_param);
        
        Glass_Draw_Txt(inparam_data->next_text_buf+i*inparam_data->text_width*inparam_data->text_height, 
            inparam_data->text_width, 
            inparam_data->text_height, 
            win_w, 
            win_h, 
            icon_h, 
            inparam_data->out_shine_layer, 
            inparam_data->icon_location[0], 
            fill_color_in_param.offset);
        
				}
    }
    SCI_TRACE_LOW("YOON_Fill_Color, cost_time ONE frame = %d ms", SCI_GetTickCount()-tick_time1);
}
LOCAL void Glass_Sweets_Get_Convert_Buffer(MMIMAINMENU_SWEETS_DATA_T   *menu_ctrl_ptr,uint32* icon_buffer,uint32 * out_32_buffer,IMGREF_POINT_T *icon_location,uint16 icon_num,uint32 * glass_color_ptr)
{
    int16 pre_angle=0;
    SWEETS_INPARAM_T candy_in_param={0};
    UILAYER_INFO_T shadow_info = {0};
    UILAYER_INFO_T dst_info = {0};
    
    //debug
    
    //candy_in_param.out_icon_layer=shine_layer_buffer;
    candy_in_param.out_shine_layer=out_32_buffer;
    candy_in_param.cur_frame=0;
    UILAYER_GetLayerInfo(&s_sweets_menu_info.dst_layer_handle,&dst_info);
    
    candy_in_param.dst_height=dst_info.layer_height;
    candy_in_param.dst_width=dst_info.layer_width;
    candy_in_param.current_page_icon_buf=icon_buffer;
    candy_in_param.next_page_icon_buf=icon_buffer;
    candy_in_param.icon_h=ICON_H;
    candy_in_param.icon_w=ICON_W;
    candy_in_param.icon_location=icon_location;
    candy_in_param.cur_icon_num=icon_num;
    candy_in_param.next_icon_num=0;
    candy_in_param.total_frame=0;
    candy_in_param.cur_state=SWEETS_STATE_ENTER_ANIM;
    candy_in_param.c_page_offset=0;
    candy_in_param.in_position=IN_POSITON_NONE;
    candy_in_param.angle=0;
    candy_in_param.out_current_angle=&pre_angle;
    //  candy_in_param.Is_Init=TRUE;
    candy_in_param.cur_text_buf = SWEETS_GetTextBufPtrByPageIndex(menu_ctrl_ptr->cur_page_index);
    candy_in_param.next_text_buf = SWEETS_GetTextBufPtrByPageIndex(menu_ctrl_ptr->cur_page_index);
    candy_in_param.text_width = SWEETS_MENUTEXT_WIDTH;
    candy_in_param.text_height = SWEETS_MENUTEXT_HEIGHT;
    candy_in_param.glass_color_ptr = glass_color_ptr;
    
    UILAYER_GetLayerInfo(&s_sweets_menu_info.shadow_layer_handle,&shadow_info);
    candy_in_param.shadow_buf = (uint32*)UILAYER_GetLayerBufferPtr(&s_sweets_menu_info.shadow_layer_handle);
    candy_in_param.shadow_w = shadow_info.layer_width;
    candy_in_param.shadow_h = shadow_info.layer_height;
    Glass_CandyType_Menu(&candy_in_param);
    
}
LOCAL void Glass_Sweets_Maginify_In(uint32 * out_buffer,uint32 * in_color_buffer,IMGREF_SIZE_T win_size,IMGREF_SIZE_T img_size,uint16 cur_frame,uint16 total_frame,FIX16_POINT_T *Point_Add_ptr)
{	
	IMGREF_POINT_T window_mid={0};
	IMGREF_POINT_T img_mid={0};
	int32 max_z=4096*3;
	int32 curret_z=0;
	uint8 alpha_para=0;
	int32 SinData=0;
	if((PNULL==in_color_buffer)||(PNULL==out_buffer)||(0==total_frame))
		{

			return;
		}

	SinData=s2d_sin_new(900*cur_frame/total_frame);
	window_mid.x=win_size.w/2;
	window_mid.y=win_size.h/2;
	img_mid.x=img_size.w/2;
	img_mid.y=img_size.h/2;
	curret_z=max_z+(4096-max_z)*SinData/65536;
	alpha_para=255*SinData/65536;
    /*
    for(i=0;i<320*480;i++)
        {
           * out_buffer++= * in_color_buffer++;
        }
    */
	S2d_4_in_one_fast_PAF888_Zerosize(  win_size, 
										  img_size, 
										 window_mid, 
										 img_mid, 
										0, 
										in_color_buffer, 
										out_buffer, 
										 2, 
										curret_z, 
										 alpha_para,							
										 Point_Add_ptr);
										 
    
}


/*****************************************************************************/
//  Description : notify parent control or parent window notify message
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E NotifyParentMsg(
    CAF_HANDLE_T     ctrl_handle,
    MMI_MESSAGE_ID_E msg_id
)
{
    MMI_RESULT_E        result = MMI_RESULT_FALSE;
    MMI_MESSAGE_ID_E    notify_msg = MSG_NOTIFY_START;

    switch (msg_id)
    {
        case MSG_APP_WEB:
            notify_msg = MSG_NOTIFY_MIDSK;
            break;

        case MSG_APP_OK:
            notify_msg = MSG_NOTIFY_OK;
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_TP_PRESS_UP:
            notify_msg = MSG_NOTIFY_PENOK;
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_APP_CANCEL:
            notify_msg = MSG_NOTIFY_CANCEL;
            break;

        default:
            break;
    }

    if (MSG_NOTIFY_START != notify_msg)
    {
        //notify parent control
        result = GUICTRL_PostNotify(ctrl_handle, notify_msg);
    }

    return (result);
}
#endif // #ifdef MENU_SWEETS_SUPPORT