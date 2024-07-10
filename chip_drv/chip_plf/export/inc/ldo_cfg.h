#ifndef _LDO_CFG_H_
#define _LDO_CFG_H_


#define LDO_INVALID_REG 0xFFFFFFFF
#define LDO_INVALID_BIT     0xFFFFFFFF

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
typedef struct
{
    LDO_ID_E id;
    uint32 bp_reg;
    uint32 bp;
    int32 ref;
} LDO_CTL_T, * LDO_CTL_PTR;
#else
typedef struct
{
    LDO_ID_E id;
    uint32 bp_reg;
    uint32 bp;
    uint32 bp_rst_reg;
    uint32 bp_rst;
    uint32 level_reg_b0;
    uint32 b0;
    uint32 b0_rst;
    uint32 level_reg_b1;
    uint32 b1;
    uint32 b1_rst;
    uint32 level_reg_b2;
    uint32 b2;
    uint32 b2_rst;
    uint32 valid_time;
    uint32 init_level;
    int32 ref;
} LDO_CTL_T, * LDO_CTL_PTR;
#endif

typedef struct
{
    SLP_LDO_E id;
    uint32 ldo_reg;
    uint32 mask;
    SLP_BIT_DEF_E value;
    BOOLEAN valid;
    uint32 reserved;
} SLP_LDO_CTL_T, * SLP_LDO_CTL_PTR;

typedef struct
{
    CHIP_TYPE_E chip_type;
    LDO_CTL_PTR ldo_ctl;
} LDO_CFG_T, * LDO_CFG_PTR;

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define VDD1V1_DEFAULT    1100
#define VDD1V2_DEFAULT    1200
#define VDD1V5_DEFAULT    1500
#define VDD1V8_DEFAULT    1800
#define VDD2V0_DEFAULT    2000
#define VDD2V7_DEFAULT    2700
#define VDD2V8_DEFAULT    2800
#define VDD3V0_DEFAULT    3000
#define VDD3V1_DEFAULT    3100
#define VDD3V3_DEFAULT    3300
typedef struct
{
	LDO_ID_E id;
	CAL_LDO_GROUP_E ldo_group;
	uint32 cali_chn;      // ldo trim control
	ADC_E  adc_ch;        // ldo adc channel
	uint32 step;          // 10mV per step
	
	uint32 reg;           // voltage adjust register
	uint32 mask_bit;
	uint32 shift_bit;
	
	uint32 default_vol;   
	uint32 default_gear;
	uint32 vol_min;       // output minimum voltage uint: mV
	uint32 vol_max;       // output maximum voltage uint: mV
	uint32 gear_min;      // min gear
	uint32 gear_max;      // max gear
	uint32 current_vol;
	
	BOOLEAN default_pd;   // power down status
	BOOLEAN if_cali_en;   // ldo calibration enable flag
	BOOLEAN cali_done;    // calibration status: TRUE: calibrated, FALSE: didn't calibrate
}CAL_LDO_T, * CAL_LDO_PTR;
#endif

PUBLIC LDO_CTL_PTR Ldo_Get_Cfg (void);
PUBLIC SLP_LDO_CTL_PTR Slp_Ldo_Get_Cfg (void);

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
PUBLIC CAL_LDO_PTR Cal_Ldo_Get_Cfg (void);
PUBLIC uint32 Cal_Ldo_Get_Tab_Size (void);
#endif

#endif // _LDO_CFG_H_
