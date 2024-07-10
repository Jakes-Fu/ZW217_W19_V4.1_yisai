#ifndef __pah800x_factory_test_h__
#define __pah800x_factory_test_h__

// pah
//#include "pah_800x.h"
#include "pah_unit.h"
#include "pah_800x_config.h"


//============================================================================
// Definition
//============================================================================

/*---------Brightness Test Golden Option---------*/
#define BRIGHTNESS_GLODEN_CHA_ENABLE		true
#define BRIGHTNESS_GLODEN_CHA_MIN_ENABLE	true
#define BRIGHTNESS_GLODEN_CHA_MAX_ENABLE	true
#define BRIGHTNESS_GLODEN_CHA_MIN			7600
#define BRIGHTNESS_GLODEN_CHA_MAX			8400
#define BRIGHTNESS_GLODEN_CHB_ENABLE		true
#define BRIGHTNESS_GLODEN_CHB_MIN_ENABLE	true
#define BRIGHTNESS_GLODEN_CHB_MAX_ENABLE	true
#define BRIGHTNESS_GLODEN_CHB_MIN			7600
#define BRIGHTNESS_GLODEN_CHB_MAX			8400
#define BRIGHTNESS_GLODEN_CHC_ENABLE		true
#define BRIGHTNESS_GLODEN_CHC_MIN_ENABLE	true
#define BRIGHTNESS_GLODEN_CHC_MAX_ENABLE	true
#define BRIGHTNESS_GLODEN_CHC_MIN			7600
#define BRIGHTNESS_GLODEN_CHC_MAX			8400

/*---------Brightness Test Target Option---------*/
#define BRIGHTNESS_TARGET_CHA_ENABLE		true
#define BRIGHTNESS_TARGET_CHA_MIN_ENABLE	true
#define BRIGHTNESS_TARGET_CHA_MAX_ENABLE	true
#define BRIGHTNESS_TARGET_CHA_MIN			2400
#define BRIGHTNESS_TARGET_CHA_MAX			15200
#define BRIGHTNESS_TARGET_CHB_ENABLE		true
#define BRIGHTNESS_TARGET_CHB_MIN_ENABLE	true
#define BRIGHTNESS_TARGET_CHB_MAX_ENABLE	true
#define BRIGHTNESS_TARGET_CHB_MIN			2400
#define BRIGHTNESS_TARGET_CHB_MAX			15200
#define BRIGHTNESS_TARGET_CHC_ENABLE		true
#define BRIGHTNESS_TARGET_CHC_MIN_ENABLE	true
#define BRIGHTNESS_TARGET_CHC_MAX_ENABLE	true
#define BRIGHTNESS_TARGET_CHC_MIN			4000
#define BRIGHTNESS_TARGET_CHC_MAX			12000

/*---------Brightness Test Target Option---------*/
#define LEAK_LIGHT_TARGET_CHA_ENABLE		true
#define LEAK_LIGHT_TARGET_CHA_MIN_ENABLE	false
#define LEAK_LIGHT_TARGET_CHA_MAX_ENABLE	true
#define LEAK_LIGHT_TARGET_CHA_MIN			0
#define LEAK_LIGHT_TARGET_CHA_MAX			160
#define LEAK_LIGHT_TARGET_CHB_ENABLE		true
#define LEAK_LIGHT_TARGET_CHB_MIN_ENABLE	false
#define LEAK_LIGHT_TARGET_CHB_MAX_ENABLE	true
#define LEAK_LIGHT_TARGET_CHB_MIN			0
#define LEAK_LIGHT_TARGET_CHB_MAX			160
#define LEAK_LIGHT_TARGET_CHC_ENABLE		true
#define LEAK_LIGHT_TARGET_CHC_MIN_ENABLE	false
#define LEAK_LIGHT_TARGET_CHC_MAX_ENABLE	true
#define LEAK_LIGHT_TARGET_CHC_MIN			0
#define LEAK_LIGHT_TARGET_CHC_MAX			160


//============================================================================
// Type Definition
//============================================================================
typedef enum
{
	factory_test_brightness_golden_sample,
	factory_test_brightness_target_sample,
	factory_test_leak_light_target_sample,
	factory_test_power_noise,
	factory_test_INT_high,
	factory_test_INT_low,
	test_type_num,

} factory_test_type;

typedef struct
{
	bool		verify_enabled;

	bool		min_enabled;
	bool		max_enabled;

	uint16_t    min;
    uint16_t    max;

} pah_ch_verify_spec_t;

typedef struct
{
    pah_reg_arr     init_reg_arr;
    pah_reg_arr		test_reg_arr;

} pah_verify_reg_t;

typedef struct
{
	pah_ch_verify_spec_t	ch_spec[PAH_800x_MAX_CH_NUM];

	pah_verify_reg_t 		test_regs;

} pah_verify_config_t;

typedef struct
{
    bool        has_data;

    // Avaiable only if has_data is true
    bool        has_overflow;

    uint8_t     overflow_num;

    // Avaiable only if has_data is true
    uint16_t    exposure_time[PAH_800x_MAX_CH_NUM];
    uint16_t    adc[PAH_800x_MAX_CH_NUM];

} pah_test_result_t;

typedef struct
{
    uint16_t	adc;

    bool		is_passed;

} pah_ch_verify_result_t;

typedef struct
{
    pah_ch_verify_result_t		ch_result[PAH_800x_MAX_CH_NUM];

} pah_verify_result_t;

typedef struct
{
    // Config
    //pah_plat            plat;
	pah_verify_config_t		verify_config;

    bool					test_ch_enabled[PAH_800x_MAX_CH_NUM];

    // PPG state
    pah_test_result_t		test_result;

    // verify
    pah_verify_result_t		verify_result;

} pah_800x_test_t;


//============================================================================
// Public function
//============================================================================
void factory_test_mode(factory_test_type test_type, bool expo_adjust_en, uint16_t expo_a, uint16_t expo_b, uint16_t expo_c);


#endif  // header guard
