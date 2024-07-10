// pah
#include "pah_800x.h"


//============================================================================
// Private function
//============================================================================
static bool sensor_has_enabled(pah_800x *state);

static pah_ret ppg_convergence_toggle(pah_800x *state, bool enable, uint8_t ch_num);

static pah_ret ppg_reset_exposure(pah_800x *state, uint8_t ch_num);

//static pah_ret touch_process(pah_800x *state, uint8_t *touch_flag);
static pah_ret ppg_process(pah_800x *state, pah_800x_ppg_data *ppg_result);
static pah_ret clear_interrupt(pah_800x *state, bool has_ppg_interrupt, bool has_overflow);

static pah_ret ppg_converging(pah_800x *state, uint32_t ppg_value, uint8_t ch_num);
static uint16_t select_new_ppg_exposure(pah_800x *state, uint16_t old_ppg_exposure, uint16_t ppg_value, uint8_t ch_num);

static bool is_touched(pah_800x *state);
static bool is_not_touched(pah_800x *state);

static bool ppg_value_is_out_of_outer_bounds(pah_800x *state, uint32_t ppg_data, uint8_t ch_num);
static bool ppg_value_is_out_of_inner_bounds(pah_800x *state, uint16_t ppg_data, uint8_t ch_num);

static pah_ret pah_800x_sw_reset(pah_800x *state);
/////////////////////////////////////////////////////////////////////////////////////////////

//// comm

static pah_ret pah_800x_hal_comm_write(uint8_t addr, const uint8_t *data, size_t write_size);
static pah_ret pah_800x_hal_comm_read(uint8_t addr, uint8_t *data, size_t read_size);

    
//// write
static pah_ret pah_800x_hal_write_reg_arr(const pah_reg_arr *reg_arr);
static pah_ret pah_800x_hal_write_reg_array(const uint8_t reg_array[][2], size_t length);
static pah_ret pah_800x_hal_write_suspend_state(bool suspend);
static pah_ret pah_800x_hal_write_interrupt_clear(void);
static pah_ret pah_800x_hal_write_interrupt_clear_v2(bool int_status_DR_FLAG_b);
static pah_ret pah_800x_hal_write_ppg_overflow_clear(void);
static pah_ret pah_800x_hal_write_exposure(uint16_t exposure, uint8_t ch_num);
static pah_ret pah_800x_hal_tg_toggle(bool enable);
static pah_ret pah_800x_hal_update_flag(void);
static pah_ret pah_800x_hal_write_sw_reset(void);
static pah_ret pah_800x_hal_reset_gpio_state(void);
//// read
static pah_ret pah_800x_hal_read_product_id(uint16_t *product_id);
static pah_ret pah_800x_hal_read_ppg_interrupt(uint8_t *ppg_interrupt);
static pah_ret pah_800x_hal_read_ppg_sync(uint32_t *ppg_data, uint8_t ch_num);
static pah_ret pah_800x_hal_read_ppg_overflow(uint8_t *ppg_overflow, uint8_t *overflow_num);
static pah_ret pah_800x_hal_read_ppg_exposure(uint16_t *als_exposure, uint8_t ch_num);
static pah_ret pah_800x_hal_touch_get_touch_flag(int8_t *touch_flag);
static pah_ret pah_800x_hal_touch_get_touch_info(int8_t *touch_flag,uint16_t *touch_value);


static pah_ret pah_800x_hal_check_enable_channel(pah_800x *state, const pah_reg_arr *reg_arr);
static pah_ret comm_write(uint8_t addr, uint8_t data);
static pah_ret pah_800x_hal_read_ppg_adc(uint16_t *ppg_adc, uint8_t ch_num);
static pah_ret pah_800x_hal_read_ppg_frame_period(uint16_t *ppg_frame_period, uint8_t ch_num);


//============================================================================
// Public function
//============================================================================
uint16_t pah_800x_get_i2c_default_slave_addr(void)
{
    return PAH_800x_DEFAULT_I2C_SLAVE_ID_1;
}

void pah_800x_get_default_config(pah_800x_config *config)
{
    int ch=0;
    memset(config, 0, sizeof(*config));    

    config->enable_ppg_convergence[0] = PAH_800x_DEFAULT_ENABLE_PPG_CONVERGENCE;
    config->enable_ppg_convergence[1] = PAH_800x_DEFAULT_ENABLE_PPG_CONVERGENCE;
    config->enable_ppg_convergence[2] = false;

    config->enable_ppg_reconvergence_if_out_of_bounds[0] = PAH_800x_DEFAULT_ENABLE_PPG_RECONVERGENCE_IF_OUT_OF_BOUNDS;
    config->enable_ppg_reconvergence_if_out_of_bounds[1] = PAH_800x_DEFAULT_ENABLE_PPG_RECONVERGENCE_IF_OUT_OF_BOUNDS;
    config->enable_ppg_reconvergence_if_out_of_bounds[2] = false;
    
    for(ch=0;ch<PAH_800x_MAX_CH_NUM;ch++)
    {
        config->ppg_convergence[ch].t_weight_normal_mode = PAH_800x_DEFAULT_PPG_CONVERGENCE_T_WEIGHT_NORMAL_MODE;
        config->ppg_convergence[ch].t_weight_fast_mode = PAH_800x_DEFAULT_PPG_CONVERGENCE_T_WEIGHT_FAST_MODE;
        config->ppg_convergence[ch].t_min = PAH_800x_DEFAULT_PPG_CONVERGENCE_T_MIN;
        config->ppg_convergence[ch].t_max = PAH_800x_DEFAULT_PPG_CONVERGENCE_T_MAX;
        config->ppg_convergence[ch].raw_outer_high_bound = PAH_800x_DEFAULT_PPG_CONVERGENCE_RAW_OUTER_HIGH_BOUND;
        config->ppg_convergence[ch].raw_inner_high_target = PAH_800x_DEFAULT_PPG_CONVERGENCE_RAW_INNER_HIGH_TARGET;
        config->ppg_convergence[ch].raw_inner_low_target = PAH_800x_DEFAULT_PPG_CONVERGENCE_RAW_INNER_LOW_TARGET;
        config->ppg_convergence[ch].raw_outer_low_bound = PAH_800x_DEFAULT_PPG_CONVERGENCE_RAW_OUTER_LOW_BOUND;
        config->ppg_convergence[ch].convergence_count = PAH_800x_DEFAULT_PPG_CONVERGENCE_COUNT;
        config->ppg_initial_exposure[ch] = PAH_800x_DEFAULT_PPG_INITIAL_EXPOSURE;
    }   

}

pah_ret pah_800x_get_product_id(uint16_t *product_id)
{
    return pah_800x_hal_read_product_id(product_id);
}

pah_ret pah_800x_init(pah_800x *state, const pah_800x_config *config)
{
    int ch=0;
    memset(state, 0, sizeof(*state));
    
   // state->plat     = *plat;
    state->config   = *config;
    for(ch=0;ch<PAH_800x_MAX_CH_NUM;ch++)
    {
        DEBUG_PRINT("enable_ppg_convergence_ch[%d] = %d\n", ch, config->enable_ppg_convergence[ch]);
        if (config->enable_ppg_convergence[ch])
        {
            DEBUG_PRINT("(t_weight_normal_mode, t_weight_fast_mode) = (%d, %d)\n",
                        config->ppg_convergence[ch].t_weight_normal_mode,
                        config->ppg_convergence[ch].t_weight_fast_mode);
            DEBUG_PRINT("(t_min, t_max) = (%d, %d)\n",
                        config->ppg_convergence[ch].t_min,
                        config->ppg_convergence[ch].t_max);
            DEBUG_PRINT("(raw_outer_high_bound, raw_inner_high_target) = (%d, %d)\n",
                        config->ppg_convergence[ch].raw_outer_high_bound,
                        config->ppg_convergence[ch].raw_inner_high_target);
            DEBUG_PRINT("(raw_inner_low_target, raw_outer_low_bound) = (%d, %d)\n",
                        config->ppg_convergence[ch].raw_inner_low_target,
                        config->ppg_convergence[ch].raw_outer_low_bound);
        }
    }

    return PAH_RET_SUCCESS;
}

pah_ret pah_800x_deinit(pah_800x *state)
{
    pah_800x_ppg_toggle(state, false);
    pah_800x_touch_toggle(state, false);
    pah_800x_power_toggle(state, false);
    return PAH_RET_SUCCESS;
}

pah_ret pah_800x_device_init(pah_800x *state, const pah_800x_reg_setting *reg_setting)
{
    uint16_t product_id;
    DEBUG_PRINT("%s\n", __func__);

    state->reg_setting = *reg_setting;

    pah_800x_hal_write_suspend_state(false);
    pah_800x_hal_write_sw_reset();
    pah_delay_ms(2);
    pah_800x_power_toggle(state, true);
    
    // read id, delay for reg bank reset
    pah_800x_hal_read_product_id(&product_id);
    pah_800x_hal_write_reg_arr(&state->reg_setting.startup_init);
    
    return PAH_RET_SUCCESS;
}

pah_ret pah_800x_power_toggle(pah_800x *state, bool enable)
{
    DEBUG_PRINT("%s(%d, %d)\n", __func__, state->is_powered, enable);

    
    if (enable)
    {
        if (state->is_powered == enable)
        {
            return PAH_RET_SUCCESS;
        }
        pah_800x_hal_write_suspend_state(false);
    }
    else
    {
        state->ppg_state.is_enabled = false;
        state->touch_state.is_enabled = false;
        
        pah_800x_hal_write_suspend_state(false);
        pah_800x_hal_write_sw_reset();
        pah_delay_ms(2);
        pah_800x_hal_write_suspend_state(true);
        pah_800x_hal_reset_gpio_state();
    }

    state->is_powered = enable;
    state->touch_state.touch_flag = TOUCH_TRALSE_VALUE;

    return PAH_RET_SUCCESS;
}

pah_ret pah_800x_verify_product_id(pah_800x *state, uint16_t *product_id)
{
    return pah_800x_hal_read_product_id(product_id);
}

pah_ret pah_800x_ppg_toggle(pah_800x *state, bool enable)
{
    uint8_t ch_num=0;
    DEBUG_PRINT("%s(%d, %d)\n", __func__, state->ppg_state.is_enabled, enable);
    if (state->ppg_state.is_enabled == enable)
        return PAH_RET_SUCCESS;
    //pah_800x_hal_write_als_toggle( enable);
    if (enable)
    {
        //load ppg setting
        //comm_write(0xEB,0x80);
//        for(uint8_t ch_num=0;ch_num < PAH_800x_MAX_CH_NUM;ch_num++)
//        {
//            ppg_reset_exposure(state,ch_num);
//            pah_800x_hal_update_flag();
//        }
       
        pah_800x_hal_write_reg_arr(&state->reg_setting.ppg_enable);

        pah_800x_hal_check_enable_channel(state, &state->reg_setting.ppg_enable);
        //DEBUG_PRINT("ch enable %d %d %d \n",state->ppg_state.ppg_ch_enabled[0],state->ppg_state.ppg_ch_enabled[1],state->ppg_state.ppg_ch_enabled[2]);
        pah_800x_hal_update_flag();              

        pah_800x_hal_write_interrupt_clear();
        if(!state->touch_state.is_enabled)
            pah_800x_hal_tg_toggle(true);
    }
    else
    {
        if(!state->touch_state.is_enabled)
            pah_800x_hal_tg_toggle(false);
    }
    
    
    for(ch_num=0;ch_num < PAH_800x_MAX_CH_NUM;ch_num++)
    {
        state->ppg_state.is_converging[ch_num] = enable;
        state->ppg_state.cur_converging_count[ch_num] = 0;
    }
    
    state->ppg_state.is_enabled = enable;
    return PAH_RET_SUCCESS;
}

pah_ret pah_800x_ppg_get_data(pah_800x *state, pah_800x_ppg_data *ppg_data)
{
    *ppg_data = state->ppg_result;
    return PAH_RET_SUCCESS;
}

pah_ret pah_800x_update_touch_setting(pah_800x *state)
{
    pah_800x_hal_write_reg_arr(&state->reg_setting.touch_enable);
    pah_800x_hal_update_flag();
    
    return PAH_RET_SUCCESS;
}

pah_ret pah_800x_touch_toggle(pah_800x *state, bool enable)
{
    DEBUG_PRINT("%s(%d, %d)\n", __func__, state->touch_state.is_enabled, enable);
    if (state->touch_state.is_enabled == enable)
        return PAH_RET_SUCCESS;

    if (enable)
    {
        //load touch setting
        pah_800x_hal_write_reg_arr(&state->reg_setting.touch_enable);
        pah_800x_hal_update_flag();
        pah_800x_hal_tg_toggle(true);
    }
    else
        pah_800x_hal_tg_toggle(false);
    state->touch_state.is_enabled = enable;
        
    return PAH_RET_SUCCESS;
}

pah_ret pah_800x_get_touch_flag(pah_800x *state, int8_t *touch_flag)
{
    *touch_flag = state->touch_flag;
    return PAH_RET_SUCCESS;
}
pah_ret pah_800x_get_touch_value(pah_800x *state, uint16_t *touch_value)
{
    *touch_value = state->touch_value;
    return PAH_RET_SUCCESS;
}


pah_ret pah_800x_task(pah_800x *state)
{
    uint8_t ppg_interrupt = 0;
    bool has_ppg_interrupt = false;
    memset(&state->ppg_result, 0, sizeof(state->ppg_result));
    state->touch_flag = -1;
    //memset(&state->touch_result, 0, sizeof(state->touch_result));
    
    //touch_process(state, &state->touch_flag);
    pah_800x_hal_touch_get_touch_flag( &state->touch_flag);
    //pah_800x_hal_touch_get_touch_info( &state->touch_flag,&state->touch_value);

    pah_800x_hal_read_ppg_interrupt(&ppg_interrupt);
    
    has_ppg_interrupt = (ppg_interrupt > 0);
    //DEBUG_PRINT("%s(%d)(%d)\n", __func__, ppg_interrupt,state->touch_flag);
    if (has_ppg_interrupt > 0)
    {
        ppg_process(state, &state->ppg_result);
    }

    clear_interrupt(state, has_ppg_interrupt, state->ppg_result.has_overflow);
    
    return PAH_RET_SUCCESS;
}


//============================================================================
// Private function
//============================================================================
static bool sensor_has_enabled(pah_800x *state)
{
    return state->ppg_state.is_enabled
        || state->touch_state.is_enabled;
}

static pah_ret ppg_convergence_toggle(pah_800x *state, bool enable, uint8_t ch_num)
{
    //DEBUG_PRINT("%s(%d, %d, %d)\n", __func__, state->config.enable_ppg_convergence[ch_num], enable, ch_num);

    if (ch_num >= PAH_800x_MAX_CH_NUM)
        return PAH_RET_INVALID_ARGUMENT;

    if (!state->config.enable_ppg_convergence[ch_num])
        return PAH_RET_SUCCESS;
   
    if (enable && state->ppg_state.cur_converging_count[ch_num] < state->config.ppg_convergence[ch_num].convergence_count)
    {
        state->ppg_state.cur_converging_count[ch_num]++;
        return PAH_RET_SUCCESS;
    }
    else
	{
         state->ppg_state.cur_converging_count[ch_num] = 0;
	}
    state->ppg_state.is_converging[ch_num] = enable;

    return PAH_RET_SUCCESS;
}

static pah_ret ppg_reset_exposure(pah_800x *state, uint8_t ch_num)
{
    //DEBUG_PRINT("%s(%d)\n", __func__, state->config.ppg_initial_exposure[ch_num]);
    if (!state->config.enable_ppg_convergence[ch_num])
        return PAH_RET_SUCCESS;

    return pah_800x_hal_write_exposure( state->config.ppg_initial_exposure[ch_num],ch_num);
}
/*static pah_ret touch_process(pah_800x *state, uint8_t *touch_flag)
{

    pah_800x_touch_get_touch_flag( touch_flag);
    
    return PAH_RET_SUCCESS;
}*/


static pah_ret ppg_process(pah_800x *state, pah_800x_ppg_data *ppg_result)
{
    uint32_t ppg_value[PAH_800x_MAX_CH_NUM] = {0};
    uint8_t ppg_overflow = 0;
    uint8_t overflow_num = 0;
    uint16_t ppg_exposure = 0;
    uint16_t ppg_adc = 0;
    uint16_t ppg_frame_period = 0;
    uint8_t ch_num=0;
    
    if (!state->ppg_state.is_enabled)
        return PAH_RET_SUCCESS;

    for(ch_num=0; ch_num < PAH_800x_MAX_CH_NUM; ch_num++)
    {
        if(state->ppg_state.ppg_ch_enabled[ch_num])
        {
            pah_800x_hal_read_ppg_sync(&ppg_value[ch_num], ch_num);
            pah_800x_hal_read_ppg_exposure(&ppg_exposure, ch_num);
            pah_800x_hal_read_ppg_frame_period(&ppg_frame_period,ch_num);
            pah_800x_hal_read_ppg_adc(&ppg_adc,ch_num);
            ppg_converging(state, ppg_adc, ch_num);//SD-revise-20191205
            
            ppg_result->data[ch_num] = ppg_value[ch_num];
            ppg_result->exposure_time[ch_num] = ppg_exposure;
            ppg_result->frame_period[ch_num] = ppg_frame_period;
            ppg_result->adc[ch_num] = ppg_adc;
        }        
    }    
    pah_800x_hal_read_ppg_overflow(&ppg_overflow,&overflow_num);
    ppg_result->has_data = true;   
    ppg_result->has_overflow = (ppg_overflow > 0);   
    ppg_result->overflow_num = overflow_num;
    
    return PAH_RET_SUCCESS;
}

static pah_ret clear_interrupt(pah_800x *state, bool has_ppg_interrupt, bool has_overflow)
{

    //pah_800x_hal_write_interrupt_clear();            
    pah_800x_hal_write_interrupt_clear_v2(has_ppg_interrupt); 
    return PAH_RET_SUCCESS;
}

static pah_ret ppg_converging(pah_800x *state, uint32_t ppg_adc, uint8_t ch_num)
{
    if (ch_num >= PAH_800x_MAX_CH_NUM)
        return PAH_RET_INVALID_ARGUMENT;

    if (!state->config.enable_ppg_convergence[ch_num])
        return PAH_RET_SUCCESS;

    if (state->config.enable_ppg_reconvergence_if_out_of_bounds[ch_num]
        && !state->ppg_state.is_converging[ch_num]
        && is_touched(state)
        && ppg_value_is_out_of_outer_bounds(state, ppg_adc, ch_num))
    {
        ppg_convergence_toggle(state, true, ch_num);
    }
    else
    {
    	state->ppg_state.cur_converging_count[ch_num] = 0;
    }

    if (!state->ppg_state.is_converging[ch_num])
        return PAH_RET_SUCCESS;
   
    if (ppg_value_is_out_of_inner_bounds(state, ppg_adc, ch_num))
    {
        uint16_t ppg_exposure = 0;
        uint16_t new_ppg_exposure = 0;

        pah_800x_hal_read_ppg_exposure(&ppg_exposure, ch_num);
        //DEBUG_PRINT("ppg converging[%d] exposure time %d ppg_value %d \n",ch_num, ppg_exposure,ppg_value);
        new_ppg_exposure = select_new_ppg_exposure(state, ppg_exposure, ppg_adc, ch_num);

        if (new_ppg_exposure > state->config.ppg_convergence[ch_num].t_max)
        {
            new_ppg_exposure = state->config.ppg_convergence[ch_num].t_max;
            ppg_convergence_toggle(state, false, ch_num);
            
            DEBUG_PRINT("ppg converging complete: (exposure, t_max) = (%d, %d)\n", new_ppg_exposure, state->config.ppg_convergence[ch_num].t_max);
        }
        else if (new_ppg_exposure < state->config.ppg_convergence[ch_num].t_min)
        {
            new_ppg_exposure = state->config.ppg_convergence[ch_num].t_min;
            ppg_convergence_toggle(state, false, ch_num);
            
            DEBUG_PRINT("ppg converging complete: (exposure, t_min) = (%d, %d)\n", new_ppg_exposure, state->config.ppg_convergence[ch_num].t_min);
        }

        pah_800x_hal_write_exposure(new_ppg_exposure, ch_num);
        pah_800x_hal_update_flag();      
        
    }
    else
    {
        ppg_convergence_toggle(state, false, ch_num);
        
        DEBUG_PRINT("ppg converging complete: (low_target, als_lux_sync, high_target) = (%d, %d, %d)\n",
                    state->config.ppg_convergence[ch_num].raw_inner_low_target, ppg_adc,
                    state->config.ppg_convergence[ch_num].raw_inner_high_target);
    }

    return PAH_RET_SUCCESS;
}

static uint16_t select_new_ppg_exposure(pah_800x *state, uint16_t old_ppg_exposure, uint16_t ppg_value, uint8_t ch_num)
{
    int32_t new_ppg_exposure = (int32_t)old_ppg_exposure;

    if (ppg_value < state->config.ppg_convergence[ch_num].raw_outer_low_bound)
        new_ppg_exposure += state->config.ppg_convergence[ch_num].t_weight_fast_mode;
    else if (ppg_value < state->config.ppg_convergence[ch_num].raw_inner_low_target)
        new_ppg_exposure += state->config.ppg_convergence[ch_num].t_weight_normal_mode;
    else if (ppg_value > state->config.ppg_convergence[ch_num].raw_outer_high_bound)
        new_ppg_exposure -= state->config.ppg_convergence[ch_num].t_weight_fast_mode;
    else if (ppg_value > state->config.ppg_convergence[ch_num].raw_inner_high_target)
        new_ppg_exposure -= state->config.ppg_convergence[ch_num].t_weight_normal_mode;

    return new_ppg_exposure;
}

static bool is_touched(pah_800x *state)
{
    return  state->touch_flag == 1;
}

static bool is_not_touched(pah_800x *state)
{
    return state->touch_flag == 0;
}

static bool ppg_value_is_out_of_outer_bounds(pah_800x *state, uint32_t ppg_data, uint8_t ch_num)
{
    return (ppg_data < state->config.ppg_convergence[ch_num].raw_outer_low_bound)
        || (ppg_data > state->config.ppg_convergence[ch_num].raw_outer_high_bound);
}

static bool ppg_value_is_out_of_inner_bounds(pah_800x *state, uint16_t ppg_data, uint8_t ch_num)
{
    return (ppg_data < state->config.ppg_convergence[ch_num].raw_inner_low_target)
        || (ppg_data > state->config.ppg_convergence[ch_num].raw_inner_high_target);
}

static pah_ret pah_800x_sw_reset(pah_800x *state)
{
    //set sw reset should not in suspend mode
    if (state->is_powered == false)
        return PAH_RET_FAILED;

    return pah_800x_hal_write_sw_reset();
}
////////////////////////////////////////////////////////////////////////////////////////////



//===========================================================================
// Private function
//===========================================================================
static pah_ret pah_800x_hal_comm_write(uint8_t addr, const uint8_t *data, size_t write_size)
{
    //return plat->comm.write(&plat->comm, addr, data, write_size);
    //return plat->comm.write(addr, data, write_size);
    return pah_comm_i2c_write(addr, data, write_size);
}

static pah_ret pah_800x_hal_comm_read(uint8_t addr, uint8_t *data, size_t read_size)
{

    //return plat->comm.read(&plat->comm, addr, data, read_size);
    return pah_comm_i2c_read(addr, data, read_size);
}

static pah_ret comm_write(uint8_t addr, uint8_t data)
{
    //return pah_800x_hal_comm_write( addr, &data, 1);
    return pah_800x_hal_comm_write(addr, &data, 1);
}
#define comm_read(addr, data)                     pah_800x_hal_comm_read(addr, data, 1)
#define comm_burst_read(addr, data, read_size)    pah_800x_hal_comm_read(addr, data, read_size)


//===========================================================================
// Private function (HAL)
//===========================================================================
static pah_ret pah_800x_hal_write_reg_arr(const pah_reg_arr *reg_arr)
{
    return pah_800x_hal_write_reg_array(reg_arr->reg_arr, reg_arr->reg_arr_len);
}

static pah_ret pah_800x_hal_write_reg_array(const uint8_t reg_array[][2], size_t length)
{
    size_t i = 0;
    uint8_t reg_value = 0;
    for (i = 0; i < length; ++i)
    {
        //for trim bit
        if (reg_array[i][0] == 0x14)
        {
            comm_read(0x14, &reg_value);
            reg_value = (reg_value & 0x1F) | (reg_array[i][1] & 0xE0);
            comm_write(reg_array[i][0], reg_value);
        }
        else
            comm_write(reg_array[i][0], reg_array[i][1]);
    }
    return PAH_RET_SUCCESS;
}

static pah_ret pah_800x_hal_write_suspend_state(bool suspend)
{
    uint8_t data = 0;
    DEBUG_PRINT("%s(%d)\n", __func__, suspend);
    if (suspend)
    {
        pah_800x_hal_write_interrupt_clear();
        comm_read(PAH_800x_ADDR_E8, &data);
        PAH_SET_BIT(data, PAH_800x_ADDR_E8_CMD_SUSPEND_MODE_BIT);
        comm_write(PAH_800x_ADDR_E8, data);
    }
    else
    {
        comm_read(PAH_800x_ADDR_E8, &data);
        PAH_CLEAR_BIT(data, PAH_800x_ADDR_E8_CMD_SUSPEND_MODE_BIT);
        comm_write(PAH_800x_ADDR_E8, data);

    }
    DEBUG_PRINT("%s(%d) end\n", __func__, suspend);

    return PAH_RET_SUCCESS;
}

static pah_ret pah_800x_hal_write_interrupt_clear(void)
{
    uint8_t data = 0;
    // DR interrupt
    comm_read(PAH_800x_ADDR_20, &data);
    //PAH_CLEAR_BIT(data, PAH_800x_ADDR_20_BIT_DR_FLAG);
    comm_write(PAH_800x_ADDR_20, (data|0x04));    //SD-revise-20191205
    return PAH_RET_SUCCESS;
}
static pah_ret pah_800x_hal_write_interrupt_clear_v2(bool int_status_DR_FLAG_b)
{
    uint8_t data = 0;
    bool int_status_DR_FLAG = 0;
    // DR interrupt
    comm_read(PAH_800x_ADDR_20, &data);
    int_status_DR_FLAG = PAH_CHECK_BIT(data, PAH_800x_ADDR_20_BIT_DR_FLAG) ;
    
    if(int_status_DR_FLAG_b==int_status_DR_FLAG)
    {
        comm_write(PAH_800x_ADDR_20, (data|0x04));	//SD-revise-20191205
    }else if((int_status_DR_FLAG_b)==0 &&(int_status_DR_FLAG==1))
    {
        comm_write(PAH_800x_ADDR_20, ((data|0x04)&0xFE));	//SD-revise-20191205
    }
    return PAH_RET_SUCCESS;
}


static pah_ret pah_800x_hal_write_ppg_overflow_clear(void)
{
    uint8_t data = 0;
    // Clear ppg_overflow flag
    comm_read(PAH_800x_ADDR_20, &data);
    PAH_SET_BIT(data, PAH_800x_ADDR_20_BIT_OVERFLOW_FLAG);
    comm_write( PAH_800x_ADDR_20, data);
    // Recover
    PAH_CLEAR_BIT(data, PAH_800x_ADDR_20_BIT_OVERFLOW_FLAG);
    comm_write(PAH_800x_ADDR_20, data);
    return PAH_RET_SUCCESS;
}

static pah_ret pah_800x_hal_write_exposure(uint16_t exposure, uint8_t ch_num)
{
    //uint8_t data = 0;
    uint8_t exposure_low = PAH_UINT16_LOW(exposure);
    uint8_t exposure_high = PAH_UINT16_HIGH(exposure);

    if(ch_num == PAH_800x_CH_NUM_A)
    {
        comm_write(PAH_800x_ADDR_17, exposure_low);
        comm_write(PAH_800x_ADDR_18, exposure_high);
    }
    else if(ch_num == PAH_800x_CH_NUM_B)
    {
        comm_write(PAH_800x_ADDR_19, exposure_low);
        comm_write(PAH_800x_ADDR_1A, exposure_high);
    }
    else if(ch_num == PAH_800x_CH_NUM_C)
    {
        comm_write(PAH_800x_ADDR_1B, exposure_low);
        comm_write(PAH_800x_ADDR_1C, exposure_high);
    }

    return PAH_RET_SUCCESS;
}

static pah_ret pah_800x_hal_read_product_id(uint16_t *product_id)
{
    return comm_burst_read(PAH_800x_ADDR_00_PRODUCT_ID, (uint8_t *)product_id,2);
}

static pah_ret pah_800x_hal_read_ppg_interrupt(uint8_t *ppg_interrupt)
{
    uint8_t data = 0;
    comm_read(PAH_800x_ADDR_20, &data);
    if (ppg_interrupt)
        *ppg_interrupt = PAH_CHECK_BIT(data, PAH_800x_ADDR_20_BIT_DR_FLAG);

    return PAH_RET_SUCCESS;
}

static pah_ret pah_800x_hal_read_ppg_sync(uint32_t *ppg_data, uint8_t ch_num)
{
    if(ch_num == PAH_800x_CH_NUM_A)
         comm_burst_read(PAH_800x_ADDR_27_RAWDATA_A_SYNC, (uint8_t*)ppg_data, 3);
    else if(ch_num == PAH_800x_CH_NUM_B)
         comm_burst_read(PAH_800x_ADDR_32_RAWDATA_B_SYNC, (uint8_t*)ppg_data, 3);
    else if(ch_num == PAH_800x_CH_NUM_C)
         comm_burst_read(PAH_800x_ADDR_3D_RAWDATA_C_SYNC, (uint8_t*)ppg_data, 3);
    return PAH_RET_SUCCESS; 
}

static pah_ret pah_800x_hal_read_ppg_overflow(uint8_t *ppg_overflow, uint8_t *overflow_num)
{
    uint8_t data = 0;
    comm_read( PAH_800x_ADDR_20, &data);
    if (ppg_overflow)
    {
        *ppg_overflow = PAH_CHECK_BIT(data, PAH_800x_ADDR_20_BIT_OVERFLOW_FLAG);
        *overflow_num = (data&0x70)>>4;
    }
    return PAH_RET_SUCCESS;
}

static pah_ret pah_800x_hal_read_ppg_exposure(uint16_t *ppg_exposure, uint8_t ch_num)
{
    if(ch_num == PAH_800x_CH_NUM_A)
        comm_burst_read(PAH_800x_ADDR_2A, (uint8_t*)ppg_exposure, 2);
    else if(ch_num == PAH_800x_CH_NUM_B)
        comm_burst_read(PAH_800x_ADDR_35, (uint8_t*)ppg_exposure, 2);
    else if(ch_num == PAH_800x_CH_NUM_C)
        comm_burst_read(PAH_800x_ADDR_40, (uint8_t*)ppg_exposure, 2);

    return PAH_RET_SUCCESS;
}

static pah_ret pah_800x_hal_read_ppg_frame_period( uint16_t *ppg_frame_period, uint8_t ch_num)
{
    if(ch_num == PAH_800x_CH_NUM_A)
        comm_burst_read(PAH_800x_ADDR_2C, (uint8_t*)ppg_frame_period, 2);
    else if(ch_num == PAH_800x_CH_NUM_B)
        comm_burst_read(PAH_800x_ADDR_37, (uint8_t*)ppg_frame_period, 2);
    else if(ch_num == PAH_800x_CH_NUM_C)
        comm_burst_read(PAH_800x_ADDR_42, (uint8_t*)ppg_frame_period, 2);

    return PAH_RET_SUCCESS;
}

static pah_ret pah_800x_hal_read_ppg_adc(uint16_t *ppg_adc, uint8_t ch_num)
{
    uint16_t data = 0;

    if(ch_num == PAH_800x_CH_NUM_A)
        comm_burst_read(PAH_800x_ADDR_2E, (uint8_t*)&data, 2);
    else if(ch_num == PAH_800x_CH_NUM_B)
        comm_burst_read(PAH_800x_ADDR_39, (uint8_t*)&data, 2);
    else if(ch_num == PAH_800x_CH_NUM_C)
        comm_burst_read(PAH_800x_ADDR_44, (uint8_t*)&data, 2);
    
    //*ppg_adc = data >> 2;
    *ppg_adc = data >> 2;

    return PAH_RET_SUCCESS;
}

static pah_ret pah_800x_hal_touch_get_touch_flag(int8_t *touch_flag)
{
    uint8_t     data = 0;
    uint16_t    data1 = 0;
//    comm_read(PAH_800x_ADDR_20, &data);
    comm_burst_read(PAH_800x_ADDR_44, (uint8_t*)&data1, 2);
    comm_read(PAH_800x_ADDR_20, &data);
     if (touch_flag)
        *touch_flag = PAH_CHECK_BIT(data, PAH_800x_ADDR_20_BIT_TOUCH_FLAG);

    return PAH_RET_SUCCESS;
}
static pah_ret pah_800x_hal_touch_get_touch_info(int8_t *touch_flag,uint16_t *touch_value)
{
    uint8_t 	data = 0;
    uint16_t	data1 = 0;
//    comm_read(PAH_800x_ADDR_20, &data);
    comm_burst_read(PAH_800x_ADDR_44, (uint8_t*)&data1, 2);
    *touch_value =data1>>2;
    comm_read(PAH_800x_ADDR_20, &data);
     if (touch_flag)
        *touch_flag = PAH_CHECK_BIT(data, PAH_800x_ADDR_20_BIT_TOUCH_FLAG);

    return PAH_RET_SUCCESS;
}

static pah_ret pah_800x_hal_tg_toggle(bool enable)
{
    uint8_t data = 0;
    comm_read( PAH_800x_ADDR_07_TG, &data);
    if(enable)
        PAH_SET_BIT(data, PAH_800x_ADDR_07_TG_BIT_EN);
    else
        PAH_CLEAR_BIT(data, PAH_800x_ADDR_07_TG_BIT_EN);
    comm_write(PAH_800x_ADDR_07_TG, data);
    
    return PAH_RET_SUCCESS;
}

static pah_ret pah_800x_hal_update_flag(void)
{   
     comm_write(PAH_800x_ADDR_EB, PAH_800x_ADDR_E8_UPDATE_FLAG);

     return PAH_RET_SUCCESS;
}

static pah_ret pah_800x_hal_write_sw_reset(void)
{
    comm_write(PAH_800x_ADDR_EE, PAH_800x_ADDR_EE_SW_RESET);

    return PAH_RET_SUCCESS;
}

static pah_ret pah_800x_hal_check_enable_channel(pah_800x *state, const pah_reg_arr *reg_arr)
{
    int i=0;
    
    for(i=0;i<reg_arr->reg_arr_len;i++)
    {
        if(reg_arr->reg_arr[i][0] == PAH_800x_ADDR_11_EnCh)
        {
            uint8_t data = reg_arr->reg_arr[i][1];
            state->ppg_state.ppg_ch_enabled[0] = PAH_CHECK_BIT(data, 0); // CH A
            state->ppg_state.ppg_ch_enabled[1] = PAH_CHECK_BIT(data, 1); // CH B
            state->ppg_state.ppg_ch_enabled[2] = PAH_CHECK_BIT(data, 2); // CH C
        }
    }
        
        return PAH_RET_SUCCESS;
}


static pah_ret pah_800x_hal_reset_gpio_state(void)
{
    uint8_t data = 0;

    comm_read(PAH_800x_ADDR_D2, &data);
    PAH_SET_BIT(data, PAH_800x_ADDR_D2_OD_EnH);
    comm_write(PAH_800x_ADDR_D2, data);

    return PAH_RET_SUCCESS;
}


void pah_800x_i2c_burst_read_verify(void)
{	
	uint8_t burst_read_test[4];

	comm_write(0x4A, 0xAB); // write 0xAB to register 0x4A 
	comm_write(0x4C, 0xCD); // write 0xCD to register 0x4C 
	pah_delay_ms(2); // delay for 1ms 

	comm_burst_read(0x4A, burst_read_test, 4); // burst read 4 bytes from register 0x4A, store data into test[] 
	DEBUG_PRINT("(%s) Addr [0x4A]: \"0x%02X\", Addr [0x4C]: \"0x%02X\" \n", __func__, burst_read_test[0], burst_read_test[2]);
	if(burst_read_test[0] != 0xAB || burst_read_test[2] != 0xCD)
	{
		DEBUG_PRINT("(%s) i2c burst read verify fail \n", __func__);
	}
	else
	{
		DEBUG_PRINT("(%s) i2c burst read verify success! \n", __func__);
	}
}


