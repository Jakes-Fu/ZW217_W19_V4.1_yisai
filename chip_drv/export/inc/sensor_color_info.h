#ifndef _SENSOR_COLOR_INFO_H_
#define _SENSOR_COLOR_INFO_H_
#ifdef   __cplusplus
extern   "C"
{
#endif

typedef enum
{
    SENSOR_GAMMA_LEVEL_0 = 0,
    SENSOR_GAMMA_LEVEL_1 ,
    SENSOR_GAMMA_LEVEL_2 ,
    SENSOR_GAMMA_LEVEL_3 ,
    SENSOR_GAMMA_LEVEL_4 ,
    SENSOR_GAMMA_LEVEL_5 ,
    SENSOR_GAMMA_LEVEL_6 ,
    SENSOR_GAMMA_LEVEL_MAX
} SENSOR_GAMMA_LEVEL;


typedef enum
{
    SENSOR_SCENE_MODE_AUTO,
    SENSOR_SCENE_MODE_LANDSCAPE,
    SENSOR_SCENE_MODE_PEOPLE,
    SENSOR_SCENE_MODE_NIGHT,
    SENSOR_SCENE_MODE_PEOPLE_NIGHT,
    SENSOR_SCENE_MODE_MICRO,
    SENSOR_SCENE_MODE_MOVING,
    SENSOR_SCENE_MODE_BACKLIGHT,
    SENSOR_SCENE_MODE_MAX

} SENSOR_SCENE_MODE;


typedef enum
{
    SENSOR_CCE_NORMAL = 0,
    SENSOR_CCE_BLACKWHITE,
    SENSOR_CCE_RED,
    SENSOR_CCE_GREEN,
    SENSOR_CCE_BLUE,
    SENSOR_CCE_YELLOW,
    SENSOR_CCE_SEPIA,
    SENSOR_CCE_NEGATIVE,
    SENSOR_CCE_LANDSCAPE,
    SENSOR_CCE_PEOPLE,
    SENSOR_CCE_NIGHTMODE,

    SENSOR_CCE_MAX
} SENSOR_CCE_SETTING;

typedef enum
{
    SENSOR_SET_PARAM_SIZE_2048X1536 = 0, //QXGA
    SENSOR_SET_PARAM_SIZE_1600X1200,        //UXGA
    SENSOR_SET_PARAM_SIZE_1280X1024,        //SXGA
    SENSOR_SET_PARAM_SIZE_1280X960,     //SXGA-
    SENSOR_SET_PARAM_SIZE_1024X768,     //XGA
    SENSOR_SET_PARAM_SIZE_800X600,      //SVGA
    SENSOR_SET_PARAM_SIZE_640X512,      //QSXGA
    SENSOR_SET_PARAM_SIZE_640X480,      //VGA
    SENSOR_SET_PARAM_SIZE_352X288,      //CIF
    SENSOR_SET_PARAM_SIZE_320X240,      //QCIF
    SENSOR_SET_PARAM_SIZE_160X120,      //QQVGA

    SENSOR_SET_PARAM_SIZE_RESERVE       //RESERVE

} SENSOR_SET_PARAM_SIZE;

typedef struct
{
    SENSOR_GAMMA_LEVEL  sensor_gmma_level;
    const uint32        *gamma_value;

} SENSOR_GAMMA_TABLE_T;

typedef struct
{
    SENSOR_CCE_SETTING  ensor_cce_setting;
    const int32         *sensor_cce_matrix;

} SENSOR_CCE_SETTING_TABLE_T;


typedef struct  _SENSOR_len_setting_t
{
    SENSOR_SET_PARAM_SIZE imgsize;
    const int32           *sensor_len_setting_ptr;
} SENSOR_LEN_SETTING_T;


typedef struct
{
    int32        c_x; // reference center x
    int32        c_y; // reference center y
    uint32       width; // reference width
    uint32       height; // reference height
    uint8        rate; // reference rate: pixel_value(0,0)/pixel_value(c_x,c_y)=rate/256
} SENSOR_LNC_REF_PARAMS_T;

typedef struct
{
    int32       c_x; //CENTER_X
    int32       c_y; //CENTER_Y
    int32      alfa_r; //ALPHAR
    int32      alfa_g; //ALPHAG
    int32      alfa_b; //ALPHAB
    int32       alfa_shift; //ALPHA_SHIFT
    int32       center_sqr; //CENTER_SQR
} SENSOR_LNC_SETTINGS_T;


typedef struct _SENSOR_CFA_SETTING_T
{
    SENSOR_SET_PARAM_SIZE imagesize;
    const uint32 *pcfa_ptr;

} SENSOR_CFA_SETTING_T;

typedef struct _sensor_table_table_t_
{
    const SENSOR_GAMMA_TABLE_T *gamma_ptr;

} SENSOR_GAMMA_TABLE_FULL_T;

//hansen sun@one SENSOR table for one sensor
typedef struct _sensor_color_tab_info_t
{
    const uint32                     *target_luminance_ptr;
    const SENSOR_GAMMA_TABLE_FULL_T  *cap_gamma_ptr;
    const SENSOR_CCE_SETTING_TABLE_T *cap_cce_ptr;
    const int32                      *cap_denoise_ptr;
    const SENSOR_CFA_SETTING_T       *cap_cfa_ptr;
    const SENSOR_GAMMA_TABLE_FULL_T  *pre_gamma_ptr;
    const SENSOR_CCE_SETTING_TABLE_T *pre_cce_ptr;
    const int32                      *pre_denoise_ptr;
    const SENSOR_CFA_SETTING_T       *pre_cfa_ptr;
    const SENSOR_LEN_SETTING_T       *lens_ptr;
    const uint32                     *blc_ptr;
    const uint32                     *op_ptr;

} SENSOR_COLOR_TAB_INFO_T;



#ifdef   __cplusplus
}
#endif
#endif








