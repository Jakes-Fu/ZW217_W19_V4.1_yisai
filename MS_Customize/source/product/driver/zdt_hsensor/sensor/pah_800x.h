#ifndef __pah_800x_h__
#define __pah_800x_h__

// pah
#include "pah_unit.h"
#include "pah_800x_config.h"


//============================================================================
// Definition
//============================================================================
#define TOUCH_TRALSE_VALUE      (-1)


//============================================================================
// Type Definition
//============================================================================
typedef struct
{
    uint16_t    t_weight_normal_mode;
    uint16_t    t_weight_fast_mode;
    uint16_t    t_min;
    uint16_t    t_max;

    uint16_t    raw_outer_high_bound;
    uint16_t    raw_inner_high_target;
    uint16_t    raw_inner_low_target;
    uint16_t    raw_outer_low_bound;

    uint16_t    convergence_count;
} pah_800x_ppg_convergence;

typedef struct
{
    uint16_t                    ppg_initial_exposure[PAH_800x_MAX_CH_NUM];

    bool                        enable_ppg_convergence[PAH_800x_MAX_CH_NUM];
    pah_800x_ppg_convergence    ppg_convergence[PAH_800x_MAX_CH_NUM];
    bool                        enable_ppg_reconvergence_if_out_of_bounds[PAH_800x_MAX_CH_NUM];

} pah_800x_config;

typedef struct
{
    bool        is_enabled;

    bool        is_converging[PAH_800x_MAX_CH_NUM];
    
    bool        ppg_ch_enabled[PAH_800x_MAX_CH_NUM];
    uint16_t    cur_converging_count[PAH_800x_MAX_CH_NUM];
} pah_800x_ppg_state;

typedef struct
{
    bool        has_data;
    uint32_t    data[PAH_800x_MAX_CH_NUM];


    // Avaiable only if has_data is true
    bool        has_overflow;

    uint8_t     overflow_num;

    // Avaiable only if has_data is true
    uint16_t    exposure_time[PAH_800x_MAX_CH_NUM];
    uint16_t    adc[PAH_800x_MAX_CH_NUM];
    uint16_t    frame_period[PAH_800x_MAX_CH_NUM];

} pah_800x_ppg_data;

typedef struct
{
    bool        is_enabled;  
    
    int8_t        touch_flag;

} pah_800x_touch_state;

typedef struct
{
    pah_reg_arr     startup_init;
    pah_reg_arr     ppg_enable;
    pah_reg_arr     touch_enable;
    
} pah_800x_reg_setting;

typedef struct
{
    // Config
    //pah_plat            plat;
    pah_800x_config         config;

    // Power state
    bool                    is_powered;

    // PPG state
    pah_800x_ppg_state      ppg_state;
    pah_800x_ppg_data       ppg_result;

    // TOUCH state
    pah_800x_touch_state    touch_state;
    int8_t                  touch_flag;
    uint16_t				touch_value;
    // Other
    pah_800x_reg_setting    reg_setting;

} pah_800x;


//============================================================================
// Public function
//============================================================================
#ifdef __cplusplus
extern  "C" {
#endif 


// Info
uint16_t    pah_800x_get_i2c_default_slave_addr(void);
void        pah_800x_get_default_config(pah_800x_config *config);
pah_ret     pah_800x_get_product_id( uint16_t *product_id);    

// Init
pah_ret     pah_800x_init(pah_800x *state, const pah_800x_config *config);
pah_ret     pah_800x_deinit(pah_800x *state);

// Device init
pah_ret     pah_800x_device_init(pah_800x *state, const pah_800x_reg_setting *reg_setting);

// Config
//pah_ret     pah_800x_set_ppg_convergence(pah_800x *state,uint8_t ch_num, const pah_800x_ppg_convergence *ppg_convergence);

// Power
pah_ret     pah_800x_power_toggle(pah_800x *state, bool enable);

// Basic verification
pah_ret     pah_800x_verify_product_id(pah_800x *state, uint16_t *product_id);
void 		pah_800x_i2c_burst_read_verify(void);


// PPG
pah_ret     pah_800x_ppg_toggle(pah_800x *state, bool enable);
pah_ret     pah_800x_ppg_get_data(pah_800x *state, pah_800x_ppg_data *ppg_data);

// TOUCH
pah_ret     pah_800x_touch_toggle(pah_800x *state, bool enable);
pah_ret     pah_800x_get_touch_flag(pah_800x *state, int8_t *touch_flag);
pah_ret     pah_800x_get_touch_value(pah_800x *state, uint16_t *touch_value);
pah_ret     pah_800x_update_touch_setting(pah_800x *state);

// Task
pah_ret     pah_800x_task(pah_800x *state);



#ifdef __cplusplus
}
#endif

#endif  // header guard
