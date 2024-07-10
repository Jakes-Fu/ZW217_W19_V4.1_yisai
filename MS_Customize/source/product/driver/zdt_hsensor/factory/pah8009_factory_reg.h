#ifndef __pah8009_factory_reg_h__
#define __pah8009_factory_reg_h__

// pah
#include "pah800x_factory_test.h"


static const uint8_t PAH_800x_REG_DEVICE_INIT_FACTORY[][2] = {
//  { 0xEE,0x07 }, //SW RESET
    { 0xE8,0x00 },//Close suspend
    { 0xD1,0xD0 },//{R_GPIO_0_IEB, R_GPIO_1_IEB, R_GPIO_0_OEB, R_GPIO_1_OEB, R_GPIO_0_PULLL_EnH, R_GPIO_1_PULLL_EnH, R_GPIO_0_PULLH_EnH, R_GPIO_1_PULLH_EnH};
    { 0xD2,0x00 },//R_GPIO_0_Open_Drain_EnH(0xD2[5]), 1(0x20):GPIO default open-drain mode, 0(0x00):GPIO debug mode
    { 0xD3,0x20 },//GPIO debug select
//	{ 0xD6,0x30 },//{Dummy, R_I2C_SLEW, R_I2C_DRV[1:0], Dummy[3:0]}
//	{ 0xD5,0x44 },//IO-level(I2C), 44:3.3V, 00:1.8V
//	{ 0xB2,0x25 },
	{ 0x55,0x24 },//Soft Reset:04 Hard Reset:24
	{ 0x52,0x12 },//VDDAY=1.63V
//	{ 0xA6,0xF7 },//Cmd_vdday_pwrsv_sel_EnH=0
	{ 0xAE,0x3F },//R_LPO_Cali_mode=1
};

static const uint8_t PAH_800x_REG_BRIGHTNESS_TEST_FACTORY[][2] = {
    { 0x03,0x40 },//Golden report period[7:0] (1T=31.25us)
    { 0x04,0x06 },//Golden report period[15:8]
    { 0x05,0x00 },//Golden report period[19:16]
    { 0x08,0x00 },//A period
    { 0x09,0x08 },//A period
    { 0x0A,0x00 },//B period
    { 0x0B,0x08 },//B period
    { 0x0C,0x00 },//C period
    { 0x0D,0x08 },//C period
    { 0x0E,0x01 },//RDB_FrameNum_A[6:0]
    { 0x0F,0x01 },//RDB_FrameNum_B[6:0]
    { 0x10,0x01 },//RDB_FrameNum_C[6:0]
    { 0x11,0x77 },//{R_OnOff_mode_C_EnH, R_OnOff_mode_B_EnH, R_OnOff_mode_A_EnH, R_ChC_EnH, R_ChB_EnH, R_ChA_EnH}
    { 0x14,0x52 },//{RDB_ChA_congain_sel[1:0], Dummy[0], R_LEDDAC_code0[4:0]}
    { 0x15,0x44 },//{RDB_ChB_congain_sel[1:0], Dummy[0], R_LEDDAC_code1[4:0]}
    { 0x16,0x44 },//{RDB_ChC_congain_sel[1:0], Dummy[0], R_LEDDAC_code2[4:0]}
    { 0x17,0x00 },//RDB_expo_A[7:0]
    { 0x18,0x01 },//RDB_expo_A[15:8]
    { 0x19,0x00 },//RDB_expo_B[7:0]
    { 0x1A,0x01 },//RDB_expo_B[15:8]
    { 0x1B,0x00 },//RDB_expo_C[7:0]
    { 0x1C,0x01 },//RDB_expo_C[15:8]
    { 0x1D,0x2F },//{R_ChA_LED_EnH[2:0], R_ChA_Pixel_EnH[3:0]}, [3]:pixel3, [2]:pixel2, [1]:pixel1, [0]:pixel0, enable high
    { 0x1E,0x4F },//{R_ChB_LED_EnH[2:0], R_ChB_Pixel_EnH[3:0]}, [3]:pixel3, [2]:pixel2, [1]:pixel1, [0]:pixel0, enable high
    { 0x1F,0x1F },//{R_ChC_LED_EnH[2:0], R_ChC_Pixel_EnH[3:0]}, [3]:pixel3, [2]:pixel2, [1]:pixel1, [0]:pixel0, enable high
    { 0x22,0xF7 },//RDB_Int_En[7:0], [2]:touch flag [1]:overflow [0]:data ready
    { 0x26,0x2A },//{R_ADCdata_C_Sel[1:0], R_ADCdata_B_Sel[1:0], R_ADCdata_A_Sel[1:0]}, 0:on, 1:off(需開On-Off), 2:on-off(需開On-Off)
    { 0xEB,0x80 },//update_flag
//  { 0x20,0xF7 },//Clear INT
};

static const uint8_t PAH_800x_REG_LEAK_LIGHT_TEST_FACTORY[][2] = {
    { 0x03,0x40 },//Golden report period[7:0] (1T=31.25us)
    { 0x04,0x06 },//Golden report period[15:8]
    { 0x05,0x00 },//Golden report period[19:16]
    { 0x08,0x00 },//A period
    { 0x09,0x08 },//A period
    { 0x0A,0x00 },//B period
    { 0x0B,0x08 },//B period
    { 0x0C,0x00 },//C period
    { 0x0D,0x08 },//C period
    { 0x0E,0x01 },//RDB_FrameNum_A[6:0]
    { 0x0F,0x01 },//RDB_FrameNum_B[6:0]
    { 0x10,0x01 },//RDB_FrameNum_C[6:0]
    { 0x11,0x77 },//{R_OnOff_mode_C_EnH, R_OnOff_mode_B_EnH, R_OnOff_mode_A_EnH, R_ChC_EnH, R_ChB_EnH, R_ChA_EnH}
    { 0x14,0x12 },//{RDB_ChA_congain_sel[1:0], Dummy[0], R_LEDDAC_code0[4:0]}
    { 0x15,0x0A },//{RDB_ChB_congain_sel[1:0], Dummy[0], R_LEDDAC_code1[4:0]}
    { 0x16,0x8A },//{RDB_ChC_congain_sel[1:0], Dummy[0], R_LEDDAC_code2[4:0]}
    { 0x17,0x00 },//RDB_expo_A[7:0]
    { 0x18,0x02 },//RDB_expo_A[15:8]
    { 0x19,0x00 },//RDB_expo_B[7:0]
    { 0x1A,0x02 },//RDB_expo_B[15:8]
    { 0x1B,0x20 },//RDB_expo_C[7:0]
    { 0x1C,0x00 },//RDB_expo_C[15:8]
    { 0x1D,0x2F },//{R_ChA_LED_EnH[2:0], R_ChA_Pixel_EnH[3:0]}, [3]:pixel3, [2]:pixel2, [1]:pixel1, [0]:pixel0, enable high
    { 0x1E,0x4F },//{R_ChB_LED_EnH[2:0], R_ChB_Pixel_EnH[3:0]}, [3]:pixel3, [2]:pixel2, [1]:pixel1, [0]:pixel0, enable high
    { 0x1F,0x1F },//{R_ChC_LED_EnH[2:0], R_ChC_Pixel_EnH[3:0]}, [3]:pixel3, [2]:pixel2, [1]:pixel1, [0]:pixel0, enable high
    { 0x22,0xF7 },//RDB_Int_En[7:0], [2]:touch flag [1]:overflow [0]:data ready
    { 0x26,0x2A },//{R_ADCdata_C_Sel[1:0], R_ADCdata_B_Sel[1:0], R_ADCdata_A_Sel[1:0]}, 0:on, 1:off(需開On-Off), 2:on-off(需開On-Off)
    { 0xEB,0x80 },//update_flag
//  { 0x20,0xF7 },//Clear INT
};


//============================================================================
// Public function
//============================================================================
void factory_test_mode(factory_test_type test_type, bool expo_adjust_en, uint16_t expo_a, uint16_t expo_b, uint16_t expo_c);


#endif  // header guard
