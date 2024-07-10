// pah
#include "pah800x_factory_test.h"
#include "pah8009_factory_reg.h"

//============================================================================
// Static Global Variable
//============================================================================
static pah_800x_test_t _factory_state;
static pah_verify_config_t	g_config_table[test_type_num] =
{
	//factory_tesst_num=0
	{
              {
        		{
        			BRIGHTNESS_GLODEN_CHA_ENABLE,
        			BRIGHTNESS_GLODEN_CHA_MIN_ENABLE,
        			BRIGHTNESS_GLODEN_CHA_MAX_ENABLE,
        			BRIGHTNESS_GLODEN_CHA_MIN,
        			BRIGHTNESS_GLODEN_CHA_MAX
        		},
        		{
        			BRIGHTNESS_GLODEN_CHB_ENABLE,
        			BRIGHTNESS_GLODEN_CHB_MIN_ENABLE, 
        			BRIGHTNESS_GLODEN_CHB_MAX_ENABLE,
        			BRIGHTNESS_GLODEN_CHB_MIN,
        			BRIGHTNESS_GLODEN_CHB_MAX
        		},
        		{
        			BRIGHTNESS_GLODEN_CHC_ENABLE,
        			BRIGHTNESS_GLODEN_CHC_MIN_ENABLE,
        			BRIGHTNESS_GLODEN_CHC_MAX_ENABLE,
        			BRIGHTNESS_GLODEN_CHC_MIN,
        			BRIGHTNESS_GLODEN_CHC_MAX
        		}
        	},
		{
			{
				PAH_800x_REG_DEVICE_INIT_FACTORY,
				PAH_ARRAY_SIZE(PAH_800x_REG_DEVICE_INIT_FACTORY)
			},
			{
				PAH_800x_REG_BRIGHTNESS_TEST_FACTORY,
				PAH_ARRAY_SIZE(PAH_800x_REG_BRIGHTNESS_TEST_FACTORY)
			}
		}
	},

	//factory_tesst_num=1
	{
		{
        		{
        			BRIGHTNESS_TARGET_CHA_ENABLE,
        			BRIGHTNESS_TARGET_CHA_MIN_ENABLE, BRIGHTNESS_TARGET_CHA_MAX_ENABLE,
        			BRIGHTNESS_TARGET_CHA_MIN, BRIGHTNESS_TARGET_CHA_MAX
        		},
        		
        		{
        			BRIGHTNESS_TARGET_CHB_ENABLE,
        			BRIGHTNESS_TARGET_CHB_MIN_ENABLE, BRIGHTNESS_TARGET_CHB_MAX_ENABLE,
        			BRIGHTNESS_TARGET_CHB_MIN, BRIGHTNESS_TARGET_CHB_MAX
        		},
        		
        		{
        			BRIGHTNESS_TARGET_CHC_ENABLE,
        			BRIGHTNESS_TARGET_CHC_MIN_ENABLE, BRIGHTNESS_TARGET_CHC_MAX_ENABLE,
        			BRIGHTNESS_TARGET_CHC_MIN, BRIGHTNESS_TARGET_CHC_MAX
        		}
        	},
		{
			
			{
				PAH_800x_REG_DEVICE_INIT_FACTORY,
				PAH_ARRAY_SIZE(PAH_800x_REG_DEVICE_INIT_FACTORY)
			},
			
			{
				PAH_800x_REG_BRIGHTNESS_TEST_FACTORY,
				PAH_ARRAY_SIZE(PAH_800x_REG_BRIGHTNESS_TEST_FACTORY)
			}
		}
	},

	//factory_tesst_num=2
	{
		{
        		{
        			LEAK_LIGHT_TARGET_CHA_ENABLE,
        			LEAK_LIGHT_TARGET_CHA_MIN_ENABLE, LEAK_LIGHT_TARGET_CHA_MAX_ENABLE,
        			LEAK_LIGHT_TARGET_CHA_MIN, LEAK_LIGHT_TARGET_CHA_MAX
        		},
        		
        		{
        			LEAK_LIGHT_TARGET_CHB_ENABLE,
        			LEAK_LIGHT_TARGET_CHB_MIN_ENABLE, LEAK_LIGHT_TARGET_CHB_MAX_ENABLE,
        			LEAK_LIGHT_TARGET_CHB_MIN, LEAK_LIGHT_TARGET_CHB_MAX
        		},
        		
        		{
        			LEAK_LIGHT_TARGET_CHC_ENABLE,
        			LEAK_LIGHT_TARGET_CHC_MIN_ENABLE, LEAK_LIGHT_TARGET_CHC_MAX_ENABLE,
        			LEAK_LIGHT_TARGET_CHC_MIN, LEAK_LIGHT_TARGET_CHC_MAX
        		}
        	},
		{
			
			{
				PAH_800x_REG_DEVICE_INIT_FACTORY,
				PAH_ARRAY_SIZE(PAH_800x_REG_DEVICE_INIT_FACTORY)
			},
			
			{
				PAH_800x_REG_LEAK_LIGHT_TEST_FACTORY,
				PAH_ARRAY_SIZE(PAH_800x_REG_LEAK_LIGHT_TEST_FACTORY)
			}
		}
	},

	//factory_tesst_num=3
	{
            {
                0
            },
		
		{
			
			{
				PAH_800x_REG_DEVICE_INIT_FACTORY,
				PAH_ARRAY_SIZE(PAH_800x_REG_DEVICE_INIT_FACTORY)
			},
			
			{
				PAH_800x_REG_LEAK_LIGHT_TEST_FACTORY,
				PAH_ARRAY_SIZE(PAH_800x_REG_LEAK_LIGHT_TEST_FACTORY)
			}
		}
	}

};

//============================================================================
// Private function
//============================================================================
static void pah_sensor_factory_mode_init(factory_test_type test_type, bool expo_adjust_en, uint16_t expo_a, uint16_t expo_b, uint16_t expo_c);
static void pah_sensor_factory_mode_start(void);
static void pah_sensor_factory_mode_stop(void);
static void pah_sensor_factory_mode_task(void);
static void pah_sensor_factory_mode_verify_result(void);

static pah_ret _pah_800x_hal_comm_write(uint8_t addr, const uint8_t *data, size_t write_size);
static pah_ret _pah_800x_hal_comm_read(uint8_t addr, uint8_t *data, size_t read_size);
static pah_ret _comm_write(uint8_t addr, uint8_t data);

static pah_ret _pah_800x_hal_write_reg_arr(const pah_reg_arr *reg_arr);
static pah_ret _pah_800x_hal_write_reg_array(const uint8_t reg_array[][2], size_t length);
static pah_ret _pah_800x_hal_write_suspend_state(bool suspend);
static pah_ret _pah_800x_hal_write_interrupt_clear(void);
static pah_ret _pah_800x_hal_write_ch_exposure(uint16_t expo_a, uint16_t expo_b, uint16_t expo_c, uint8_t ch_num);
static pah_ret _pah_800x_hal_tg_toggle(bool enable);
static pah_ret _pah_800x_hal_update_flag(void);

static pah_ret _pah_800x_hal_read_product_id(uint16_t *product_id);
static pah_ret _pah_800x_hal_read_ppg_interrupt(uint8_t *ppg_interrupt);
static pah_ret _pah_800x_hal_read_ppg_overflow(uint8_t *ppg_overflow, uint8_t *overflow_num);
static pah_ret _pah_800x_hal_read_ppg_adc(uint16_t *ppg_adc, uint8_t ch_num);
static pah_ret _pah_800x_hal_check_enable_channel(pah_800x_test_t *factory_state, const pah_reg_arr *reg_arr);


//============================================================================
// Public function
//============================================================================
void factory_test_mode(factory_test_type test_type, bool expo_adjust_en, uint16_t expo_a, uint16_t expo_b, uint16_t expo_c)
{
	LOG_PRINT("test_type=%d\n{\n factory_test_brightness_golden_sample=0\n "	\
			  "factory_test_brightness_target_sample=1\n factory_test_leak_light_target_sample=2\n "	\
			  "factory_test_power_noise=3\n factory_test_INT_high=4\n factory_test_INT_low=5\n}\n", test_type);

	pah_sensor_factory_mode_init(test_type, expo_adjust_en, expo_a, expo_b, expo_c);

	pah_sensor_factory_mode_start();

    pah_delay_ms(25);

    if (test_type < factory_test_power_noise)
    {
    	pah_sensor_factory_mode_task();
    	pah_sensor_factory_mode_verify_result();
        pah_sensor_factory_mode_stop();
    }
    else
    {
    	return;
    }

    pah_delay_ms(500);

    return;

}


//============================================================================
// Private function 
//============================================================================
static void pah_sensor_factory_mode_init(factory_test_type test_type, bool expo_adjust_en, uint16_t expo_a, uint16_t expo_b, uint16_t expo_c)
{
    uint16_t product_id = 0;
    int ch_index=0;
    uint8_t ch_num=0;
    DEBUG_PRINT("%s(%d)\n", __func__, test_type);

    memset(&_factory_state, 0, sizeof(_factory_state));

    _pah_800x_hal_read_product_id(&product_id);
    DEBUG_PRINT("product_id = 0x%04X\n", product_id);
    if (product_id != PAH_800x_PRODUCT_ID)
    {
    	DEBUG_PRINT("Failed to verify product_id\n");
    }
/**/
   	for (ch_index=0; ch_index < 3; ch_index++)
   	{
   		_factory_state.verify_config.ch_spec[ch_index] = g_config_table[test_type].ch_spec[ch_index];
   	}

   	_factory_state.verify_config.test_regs = g_config_table[test_type].test_regs;
    _pah_800x_hal_write_reg_arr(&_factory_state.verify_config.test_regs.init_reg_arr);
    _pah_800x_hal_write_reg_arr(&_factory_state.verify_config.test_regs.test_reg_arr);
   	_pah_800x_hal_check_enable_channel(&_factory_state, &_factory_state.verify_config.test_regs.test_reg_arr);
/**/
	if (expo_adjust_en)
	{
        for(ch_num=0; ch_num<PAH_800x_MAX_CH_NUM; ch_num++)
		{
        	_pah_800x_hal_write_ch_exposure(expo_a, expo_b, expo_c, ch_num);
		}
        _pah_800x_hal_update_flag();
	}

	_pah_800x_hal_write_interrupt_clear();

//	PAH_REG_ARR_SETUP(_factory_state.verify_config.test_regs.init_reg_arr, PAH_800x_REG_DEVICE_INIT_FACTORY);
//	_pah_800x_hal_write_reg_arr(&_factory_state.verify_config.test_regs.init_reg_arr);
}

static void pah_sensor_factory_mode_start(void)
{
	DEBUG_PRINT("%s\n", __func__);

    _pah_800x_hal_write_suspend_state(false);
    _pah_800x_hal_tg_toggle(true);

}

static void pah_sensor_factory_mode_stop(void)
{
	DEBUG_PRINT("%s\n", __func__);

    _pah_800x_hal_tg_toggle(false);
    _pah_800x_hal_write_suspend_state(true);

}

static void pah_sensor_factory_mode_task(void)
{
    uint8_t ch_num=0; 
    uint8_t ppg_interrupt = 0;
    bool has_ppg_interrupt = false;
    uint8_t ppg_overflow = 0;
    uint8_t overflow_num = 0;
	DEBUG_PRINT("%s\n", __func__);
    _pah_800x_hal_read_ppg_interrupt(&ppg_interrupt);
    _pah_800x_hal_read_ppg_overflow(&ppg_overflow, &overflow_num);

    has_ppg_interrupt = (ppg_interrupt > 0);
    if (has_ppg_interrupt > 0)
    {
        for (ch_num=0; ch_num < PAH_800x_MAX_CH_NUM; ch_num++)
        {
            if (_factory_state.test_ch_enabled[ch_num])
            {
            	_pah_800x_hal_read_ppg_adc(&_factory_state.test_result.adc[ch_num], ch_num);
            }
        }
    }

    //clear_interrupt(state, has_ppg_interrupt, state->ppg_result.has_overflow);

}

static void pah_sensor_factory_mode_verify_result(void)
{
    int i=0;
	DEBUG_PRINT("%s\n", __func__);
	LOG_PRINT("=============== Verify_Result ===============\n"	\
			  "{\n result=1 : pass\n result=0 : fail\n}\n");

	for (i=0; i<PAH_800x_MAX_CH_NUM; ++i)
	{
		if (_factory_state.verify_config.ch_spec[i].verify_enabled
			&& _factory_state.verify_config.ch_spec[i].min_enabled
			&& _factory_state.verify_config.ch_spec[i].max_enabled)
		{
			if (_factory_state.test_result.adc[i] < _factory_state.verify_config.ch_spec[i].min
				|| _factory_state.test_result.adc[i] > _factory_state.verify_config.ch_spec[i].max)
			{
				_factory_state.verify_result.ch_result[i].is_passed = false;
			}
			else
			{
				_factory_state.verify_result.ch_result[i].is_passed = true;
			}
			LOG_PRINT("ch[%d] : (min, value, max)=(%d, %d, %d), result=%d \n",\
						i, _factory_state.verify_config.ch_spec[i].min, \
						_factory_state.test_result.adc[i], \
						_factory_state.verify_config.ch_spec[i].max, \
						_factory_state.verify_result.ch_result[i].is_passed);
		}
		else if (_factory_state.verify_config.ch_spec[i].verify_enabled
				 && _factory_state.verify_config.ch_spec[i].min_enabled)
		{
			if (_factory_state.test_result.adc[i] < _factory_state.verify_config.ch_spec[i].min)
			{
				_factory_state.verify_result.ch_result[i].is_passed = false;
			}
			else
			{
				_factory_state.verify_result.ch_result[i].is_passed = true;
			}
			LOG_PRINT("ch[%d] : (min, value)=(%d, %d), result=%d \n",\
						i, _factory_state.verify_config.ch_spec[i].min, \
						_factory_state.test_result.adc[i], \
						_factory_state.verify_result.ch_result[i].is_passed);
		}
		else if (_factory_state.verify_config.ch_spec[i].verify_enabled
				&& _factory_state.verify_config.ch_spec[i].max_enabled)
		{
			if (_factory_state.test_result.adc[i] > _factory_state.verify_config.ch_spec[i].max)
			{
				_factory_state.verify_result.ch_result[i].is_passed = false;
			}
			else
			{
				_factory_state.verify_result.ch_result[i].is_passed = true;
			}
			LOG_PRINT("ch[%d] : (value, max)=(%d, %d), result=%d \n",\
						i, _factory_state.test_result.adc[i], \
						_factory_state.verify_config.ch_spec[i].max, \
						_factory_state.verify_result.ch_result[i].is_passed);
		}
	}
}

bool pah_sensor_get_verify_result(uint8_t idx,char * out_str)
{
    bool res = false;
    int i=idx;
    
    if(out_str == NULL)
    {
        return res;
    }
    if( i<PAH_800x_MAX_CH_NUM)
    {
    	if (_factory_state.verify_config.ch_spec[i].verify_enabled
    		&& _factory_state.verify_config.ch_spec[i].min_enabled
    		&& _factory_state.verify_config.ch_spec[i].max_enabled)
    	{
    		if (_factory_state.test_result.adc[i] < _factory_state.verify_config.ch_spec[i].min
    			|| _factory_state.test_result.adc[i] > _factory_state.verify_config.ch_spec[i].max)
    		{
    			res = false;
    		}
    		else
    		{
    			res = true;
    		}
    		sprintf(out_str,"ch[%d] : (min, v, max)=(%d, %d, %d),%d",\
    					i, _factory_state.verify_config.ch_spec[i].min, \
    					_factory_state.test_result.adc[i], \
    					_factory_state.verify_config.ch_spec[i].max, \
    					res);
    	}
    	else if (_factory_state.verify_config.ch_spec[i].verify_enabled
    			 && _factory_state.verify_config.ch_spec[i].min_enabled)
    	{
    		if (_factory_state.test_result.adc[i] < _factory_state.verify_config.ch_spec[i].min)
    		{
    			res = false;
    		}
    		else
    		{
    			res = true;
    		}
    		sprintf(out_str,"ch[%d] : (min, value)=(%d, %d),%d",\
    					i, _factory_state.verify_config.ch_spec[i].min, \
    					_factory_state.test_result.adc[i], \
    					res);
    	}
    	else if (_factory_state.verify_config.ch_spec[i].verify_enabled
    			&& _factory_state.verify_config.ch_spec[i].max_enabled)
    	{
    		if (_factory_state.test_result.adc[i] > _factory_state.verify_config.ch_spec[i].max)
    		{
    			res = false;
    		}
    		else
    		{
    			res = true;
    		}
    		sprintf(out_str,"ch[%d] : (value, max)=(%d, %d),%d",\
    					i, _factory_state.test_result.adc[i], \
    					_factory_state.verify_config.ch_spec[i].max, \
    					res);
    	}
    }
    return res;
}

//============================================================================
// Private function (HAL)
//============================================================================
static pah_ret _pah_800x_hal_comm_write(uint8_t addr, const uint8_t *data, size_t write_size)
{
    //return plat->comm.write(&plat->comm, addr, data, write_size);
    //return plat->comm.write(addr, data, write_size);
    return pah_comm_i2c_write(addr, data, write_size);
}

static pah_ret _pah_800x_hal_comm_read(uint8_t addr, uint8_t *data, size_t read_size)
{

    //return plat->comm.read(&plat->comm, addr, data, read_size);
    return pah_comm_i2c_read(addr, data, read_size);
}

static pah_ret _comm_write(uint8_t addr, uint8_t data)
{
    //return pah_800x_hal_comm_write( addr, &data, 1);
    return _pah_800x_hal_comm_write(addr, &data, 1);
}

#define _comm_read(addr, data)                     _pah_800x_hal_comm_read(addr, data, 1)
#define _comm_burst_read(addr, data, read_size)    _pah_800x_hal_comm_read(addr, data, read_size)


static pah_ret _pah_800x_hal_write_reg_arr(const pah_reg_arr *reg_arr)
{
    return _pah_800x_hal_write_reg_array(reg_arr->reg_arr, reg_arr->reg_arr_len);
}

static pah_ret _pah_800x_hal_write_reg_array(const uint8_t reg_array[][2], size_t length)
{
    size_t i = 0;
    uint8_t reg_value = 0;
    for (i = 0; i < length; ++i)
    {
        //for trim bit
        if(reg_array[i][0] == 0x14)
        {
            _comm_read(0x14, &reg_value);
            reg_value = (reg_value & 0x1F) | (reg_array[i][1] & 0xE0);
            _comm_write(reg_array[i][0], reg_value);
        }
        else
        	_comm_write(reg_array[i][0], reg_array[i][1]);
    }
    return PAH_RET_SUCCESS;
}

static pah_ret _pah_800x_hal_write_suspend_state(bool suspend)
{
    uint8_t data = 0;
    DEBUG_PRINT("%s(%d)\n", __func__, suspend);
    if (suspend)
    {
        _comm_read(PAH_800x_ADDR_E8, &data);
        PAH_SET_BIT(data, PAH_800x_ADDR_E8_CMD_SUSPEND_MODE_BIT);
        _comm_write(PAH_800x_ADDR_E8, data);
    }
    else
    {
        _comm_read(PAH_800x_ADDR_E8, &data);
        PAH_CLEAR_BIT(data, PAH_800x_ADDR_E8_CMD_SUSPEND_MODE_BIT);
        _comm_write(PAH_800x_ADDR_E8, data);

    }
    DEBUG_PRINT("%s(%d) end\n", __func__, suspend);

    return PAH_RET_SUCCESS;
}

static pah_ret _pah_800x_hal_write_interrupt_clear(void)
{
    uint8_t data = 0;
    // DR interrupt
    _comm_read(PAH_800x_ADDR_20, &data);
    //PAH_CLEAR_BIT(data, PAH_800x_ADDR_20_BIT_DR_FLAG);
    _comm_write(PAH_800x_ADDR_20, (data|0x04));	//SD-revise-20191205
    return PAH_RET_SUCCESS;
}

static pah_ret _pah_800x_hal_write_ch_exposure(uint16_t expo_a, uint16_t expo_b, uint16_t expo_c, uint8_t ch_num)
{
    //uint8_t data = 0;

    uint8_t a_exposure_low = PAH_UINT16_LOW(expo_a);
    uint8_t a_exposure_high = PAH_UINT16_HIGH(expo_a);
    uint8_t b_exposure_low = PAH_UINT16_LOW(expo_b);
    uint8_t b_exposure_high = PAH_UINT16_HIGH(expo_b);
    uint8_t c_exposure_low = PAH_UINT16_LOW(expo_c);
    uint8_t c_exposure_high = PAH_UINT16_HIGH(expo_c);
    
    DEBUG_PRINT("%s(%d, %d, %d, %d)\n", __func__, expo_a, expo_b, expo_c, ch_num);
    
    if(ch_num == PAH_800x_CH_NUM_A)
    {
        _comm_write(PAH_800x_ADDR_17, a_exposure_low);
        _comm_write(PAH_800x_ADDR_18, a_exposure_high);
    }
    else if(ch_num == PAH_800x_CH_NUM_B)
    {
        _comm_write(PAH_800x_ADDR_19, b_exposure_low);
        _comm_write(PAH_800x_ADDR_1A, b_exposure_high);
    }
    else if(ch_num == PAH_800x_CH_NUM_C)
    {
        _comm_write(PAH_800x_ADDR_1B, c_exposure_low);
        _comm_write(PAH_800x_ADDR_1C, c_exposure_high);
    }

    return PAH_RET_SUCCESS;
}

static pah_ret _pah_800x_hal_tg_toggle(bool enable)
{
    uint8_t data = 0;
    DEBUG_PRINT("%s(%d)\n", __func__, enable);
    _comm_read( PAH_800x_ADDR_07_TG, &data);
    if(enable)
        PAH_SET_BIT(data, PAH_800x_ADDR_07_TG_BIT_EN);
    else
        PAH_CLEAR_BIT(data, PAH_800x_ADDR_07_TG_BIT_EN);
    _comm_write(PAH_800x_ADDR_07_TG, data);

	return PAH_RET_SUCCESS;
}

static pah_ret _pah_800x_hal_update_flag(void)
{
     _comm_write(PAH_800x_ADDR_EB, PAH_800x_ADDR_E8_UPDATE_FLAG);

     return PAH_RET_SUCCESS;
}

static pah_ret _pah_800x_hal_read_product_id(uint16_t *product_id)
{
    return _comm_burst_read(PAH_800x_ADDR_00_PRODUCT_ID, (uint8_t *)product_id,2);
}

static pah_ret _pah_800x_hal_read_ppg_interrupt(uint8_t *ppg_interrupt)
{
    uint8_t data = 0;
    _comm_read(PAH_800x_ADDR_20, &data);
    if (ppg_interrupt)
        *ppg_interrupt = PAH_CHECK_BIT(data, PAH_800x_ADDR_20_BIT_DR_FLAG);

    return PAH_RET_SUCCESS;
}

static pah_ret _pah_800x_hal_read_ppg_overflow(uint8_t *ppg_overflow, uint8_t *overflow_num)
{
    uint8_t data = 0;
    _comm_read( PAH_800x_ADDR_20, &data);
    if (ppg_overflow)
    {
        *ppg_overflow = PAH_CHECK_BIT(data, PAH_800x_ADDR_20_BIT_OVERFLOW_FLAG);
        *overflow_num = (data&0x70)>>4;
    }
    return PAH_RET_SUCCESS;
}

static pah_ret _pah_800x_hal_read_ppg_adc(uint16_t *ppg_adc, uint8_t ch_num)
{
    uint16_t data = 0;

    if(ch_num == PAH_800x_CH_NUM_A)
        _comm_burst_read(PAH_800x_ADDR_2E, (uint8_t*)&data, 2);
    else if(ch_num == PAH_800x_CH_NUM_B)
        _comm_burst_read(PAH_800x_ADDR_39, (uint8_t*)&data, 2);
    else if(ch_num == PAH_800x_CH_NUM_C)
        _comm_burst_read(PAH_800x_ADDR_44, (uint8_t*)&data, 2);

    //*ppg_adc = data >> 2;
    *ppg_adc = data >> 2;

    return PAH_RET_SUCCESS;
}

static pah_ret _pah_800x_hal_check_enable_channel(pah_800x_test_t *factory_state, const pah_reg_arr *reg_arr)
{
    int i=0;
    for(i=0;i<reg_arr->reg_arr_len;i++)
    {
        if(reg_arr->reg_arr[i][0] == PAH_800x_ADDR_11_EnCh)
        {
            uint8_t data = reg_arr->reg_arr[i][1];
            factory_state->test_ch_enabled[0] = PAH_CHECK_BIT(data, 0); // CH A
            factory_state->test_ch_enabled[1] = PAH_CHECK_BIT(data, 1); // CH B
            factory_state->test_ch_enabled[2] = PAH_CHECK_BIT(data, 2); // CH C
        }
    }

		return PAH_RET_SUCCESS;
}
