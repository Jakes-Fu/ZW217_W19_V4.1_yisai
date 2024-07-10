#ifndef _SENSOR_AWBAE_CFG_H_
#define _SENSOR_AWBAE_CFG_H_
#ifdef   __cplusplus
extern   "C"
{
#endif

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef struct sensor_awb_lightsource_para_struct_tag
{
    int8    cb;
    int8    cr;
    uint8   r_gain;
    uint8   g_gain;
    uint8   b_gain;
    uint8   thr_chrom_sum;
} SENSOR_AWB_LIGHTSOURCE_PARA_T;


typedef enum awbc_mode_tag/* AWB manual mode gain options*/
{
    AWBC_AUTO = 0,
    AWBC_MANUAL_2800K,
    AWBC_MANUAL_4000K,
    AWBC_MANUAL_4150K,
    AWBC_MANUAL_4400K,
    AWBC_MANUAL_6500K,
    AWBC_MANUAL_7000K,
    AWBC_MANUAL_DEFAULT = 128,
    AWBC_MANUAL_MAX     = 255
} SENSOR_AWBC_MODE_E;

typedef struct awb_manual_mode_gain_struct_tag
{
    SENSOR_AWBC_MODE_E manual_mode;
    uint8              r_gain;
    uint8              gr_gain;
    uint8              gb_gain;
    uint8              b_gain;
} SENSOR_AWB_MANUAL_MODE_GAIN_T;


typedef struct _sensor_awb_win_t_
{
    const int32 *ctc_win_ptr;
    const int32 *ctc_param_ptr;


} SENSOR_AWB_WIN_T;


typedef struct _sensor_awb_info_t_
{
    const uint8  *max_gain_ptr;
    const uint8  *min_gain_ptr;
    const uint8  *initial_r_gain_ptr;
    const uint8  *initial_b_gain_ptr;
    const uint32 *wp_highlight_ptr;
    const uint32 *wp_lowlight_ptr;
    const int32  *simple_win_ptr_ptr;

    const SENSOR_AWB_LIGHTSOURCE_PARA_T *awb_light_source_ptr;
    const SENSOR_AWB_MANUAL_MODE_GAIN_T *awb_manual_gain_ptr;
    const SENSOR_AWB_WIN_T            *awb_advanced_win_ptr;

} SENSOR_AWB_INFO_T;


typedef enum
{
    SENSOR_AE_MEA_AEVERAGE,
    SENSOR_AE_MEA_CENTRE_WEIGHT,
    SENSOR_AE_MEA_CENTRE,

    SENSOR_AE_MEA_MAX = 128

} SENSOR_AE_MEA_TYPE;


typedef struct _sensor_ae_weights_t
{

    SENSOR_AE_MEA_TYPE type_index;
    const uint8 *weidhts_ptr;

} SENSOR_AE_WEIDHTS_T;


typedef struct sensor_ae_value_t_
{

    const uint16 *ae_shutter_ptr;
    const uint16 *ae_gain_ptr;

} SENSOR_AE_table_T;

typedef struct _sensor_ae_info_t_
{
    const uint32 *target_luminance_ptr;
    const int32  *max_normal_index_ptr;
    const int32  *max_night_index_ptr;
    const int32  *normal_intial_index_ptr;
    const int32  *night_initial_index_ptr;
    const int32  *wdead_zone_ptr;

    const SENSOR_AE_WEIDHTS_T *ae_weights_ptr;
    const SENSOR_AE_table_T *ae_table_ptr;

} SENSOR_AE_INFO_T;

#ifdef   __cplusplus
}
#endif
#endif

