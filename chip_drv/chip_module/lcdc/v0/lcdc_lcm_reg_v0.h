/******************************************************************************
 ** File Name:    lcdc_lcm_reg_v0.h                                           *
 ** Author:       Jianping.Wang                                               *
 ** DATE:         01/21/2006                                                  *
 ** Copyright:    2006 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 01/21/2006    Jianping.Wang   Create.                                     *
 ******************************************************************************/
#ifndef _LCDC_LCM_REG_V0_H_
#define _LCDC_LCM_REG_V0_H_
/*----------------------------------------------------------------------------*
 **                          Dependencies                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                          Compiler Flag                                    *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
**                               Macro Define                                **
**----------------------------------------------------------------------------*/


#define LCM_PARAMETER0                  (LCM_REG_BASE + 0x0000)
#define LCM_RSTN                        (LCM_REG_BASE + 0x0004)
#define LCM_DATANUMBER                  (LCM_REG_BASE + 0x0028) // 0x60001c68              
#define LCM_PARAMETER1                  (LCM_REG_BASE + 0x000c)  //LCM_MODESEL 
#define LCM_IFMODE                      (LCM_REG_BASE + 0x0010)
#define LCM_RDDATA                      (LCM_REG_BASE + 0x0014)
#define LCM_STATUS                      (LCM_REG_BASE + 0x0018)
#define LCM_RGBMODE                     (LCM_REG_BASE + 0x001c)
#define LCM_SELPIN                      (LCM_REG_BASE + 0x0020)
#define LCM_RD_MODE                   (LCM_REG_BASE + 0x0024)
#define LCM_TRANS                        (LCM_REG_BASE + 0x0028)
#define LCM_BUS_RATIO                   (LCM_REG_BASE + 0x002C)

#define LCM_CS0_BASE_ADDR               (LCM_CS_BASE_ADDR + 0x0000)
#define LCM_CS1_BASE_ADDR               (LCM_CS_BASE_ADDR + 0x0100)
#define LCM_CS2_BASE_ADDR               (LCM_CS_BASE_ADDR + 0x0200)
#define LCM_CS3_BASE_ADDR               (LCM_CS_BASE_ADDR + 0x0300)
#define LCM_CS4_BASE_ADDR               (LCM_CS_BASE_ADDR + 0x0400)
#define LCM_CS5_BASE_ADDR               (LCM_CS_BASE_ADDR + 0x0500)

/**---------------------------------------------------------------------------*
**                               Data Prototypt                              **
**----------------------------------------------------------------------------*/
typedef union _lcm_parameter0_tag
{
    struct _lcm_paramter0_map
    {
#if defined(CHIP_ENDIAN_LITTLE) && !defined (_BIG_ENDIAN)
        volatile  uint   whpw           :4;
        volatile  uint   wlpw           :4;
        volatile  uint   wcss           :2;
        volatile  uint   rhpw           :4;
        volatile  uint   rlpw           :4;
        volatile  uint   rcss           :2;
        volatile  uint   reserved       :12;//Reserved
#else
        volatile  uint   reserved       :12;//Reserved
        volatile  uint   rcss           :2;
        volatile  uint   rlpw           :4;
        volatile  uint   rhpw           :4;
        volatile  uint   wcss           :2;
        volatile  uint   wlpw           :4;
        volatile  uint   whpw           :4;
#endif
    } mBits;
    volatile  uint   dwValue;
} LCM_PARAMTER0_U;

typedef union _lcm_parameter1_tag
{
    struct _lcm_paramter1_map
    {
#if defined(CHIP_ENDIAN_LITTLE) && !defined (_BIG_ENDIAN)
        volatile  uint   whpw           :4;
        volatile  uint   wlpw           :4;
        volatile  uint   wcss           :2;
        volatile  uint   rhpw           :4;
        volatile  uint   rlpw           :4;
        volatile  uint   rcss           :2;
        volatile  uint   reserved       :12;//Reserved
#else
        volatile  uint   reserved       :12;//Reserved
        volatile  uint   rcss           :2;
        volatile  uint   rlpw           :4;
        volatile  uint   rhpw           :4;
        volatile  uint   wcss           :2;
        volatile  uint   wlpw           :4;
        volatile  uint   whpw           :4;
#endif
    } mBits;
    volatile  uint   dwValue;
} LCM_PARAMTER1_U;

typedef union _lcm_rstn_tag
{
    struct _lcm_rstn_map
    {
#if defined(CHIP_ENDIAN_LITTLE) && !defined (_BIG_ENDIAN)
        volatile  uint   rstn           :1;
        volatile  uint   reserved       :31;//Reserved
#else
        volatile  uint   reserved       :31;//Reserved
        volatile  uint   rstn           :1;
#endif
    } mBits;
    volatile  uint   dwValue;
} LCM_RSTN_U;

typedef union _lcm_modesel_tag
{
    struct _lcm_modesel_map
    {
#if defined(CHIP_ENDIAN_LITTLE) && !defined (_BIG_ENDIAN)
        volatile  uint   modesel0       :1;
        volatile  uint   modesel1       :1;
        volatile  uint   modesel2       :1;
        volatile  uint   modesel3       :1;
        volatile  uint   modesel4       :1;
        volatile  uint   modesel5       :1;
        volatile  uint   reserved       :26;//Reserved
#else
        volatile  uint   reserved       :26;//Reserved
        volatile  uint   modesel5       :1;
        volatile  uint   modesel4       :1;
        volatile  uint   modesel3       :1;
        volatile  uint   modesel2       :1;
        volatile  uint   modesel1       :1;
        volatile  uint   modesel0       :1;
#endif
    } mBits;
    volatile  uint   dwValue;
} LCM_MODESEL_U;

typedef union _lcm_rddata_tag
{
    struct _lcm_rddata_map
    {
#if defined(CHIP_ENDIAN_LITTLE) && !defined (_BIG_ENDIAN)
        volatile  uint   rd_data        :18;
        volatile  uint   reserved       :14;//Reserved
#else
        volatile  uint   reserved       :14;//Reserved
        volatile  uint   rd_data        :18;
#endif
    } mBits;
    volatile  uint   dValue;
} LCM_RDDATA_U;

typedef union _lcm_status_tag
{
    struct _lcm_status_map
    {
#if defined(CHIP_ENDIAN_LITTLE) && !defined (_BIG_ENDIAN)
        volatile  uint   busy           :1;
        volatile  uint   fifo_full      :1;
        volatile  uint   fifo_empty     :1;
        volatile  uint   reserved       :29;//Reserved
#else
        volatile  uint   reserved       :29;//Reserved
        volatile  uint   fifo_empty     :1;
        volatile  uint   fifo_full      :1;
        volatile  uint   busy           :1;
#endif
    } mBits;
    volatile  uint   dValue;
} LCM_STATUS_U;

typedef union _lcm_rgbmode_tag
{
    struct _lcm_rgbmode_map
    {
#if defined(CHIP_ENDIAN_LITTLE) && !defined (_BIG_ENDIAN)
        volatile  uint   rgbmode0       :2;
        volatile  uint   rgbmode1       :2;
        volatile  uint   rgbmode2       :2;
        volatile  uint   rgbmode3       :2;
        volatile  uint   rgbmode4       :2;
        volatile  uint   rgbmode5       :2;
        volatile  uint   reserved       :20;//Reserved
#else
        volatile  uint   reserved       :20;//Reserved
        volatile  uint   rgbmode5       :2;
        volatile  uint   rgbmode4       :2;
        volatile  uint   rgbmode3       :2;
        volatile  uint   rgbmode2       :2;
        volatile  uint   rgbmode1       :2;
        volatile  uint   rgbmode0       :2;
#endif
    } mBits;
    volatile  uint   dwValue;
} LCM_RGBMODE_U;

typedef union _lcm_selpin_tag
{
    struct _lcm_selpin_map
    {
#if defined(CHIP_ENDIAN_LITTLE) && !defined (_BIG_ENDIAN)
        volatile  uint   cd1_cs5        :1;//0:cs5;1:cd1
        volatile  uint   cd2_cs4        :1;//0:cs4;1:cd2
        volatile  uint   cd3_cs3        :1;//0:cs3;1:cd3
        volatile  uint   cd4_data16     :1;//0:data16;1:cd4
        volatile  uint   cd5_data17     :1;//0:data17;1:cd5
        volatile  uint   cd1_data11     :1;//0:data11;1:cd1
        volatile  uint   cd2_data12     :1;//0:data12;1:cd2
        volatile  uint   cd3_data13     :1;//0:data13;1:cd3
        volatile  uint   cd4_data14     :1;//0:data14;1:cd4
        volatile  uint   cd5_data15     :1;//0:data15;1:cd5
        volatile  uint   reserved       :22;//Reserved
#else
        volatile  uint   reserved       :22;//Reserved
        volatile  uint   cd5_data15     :1;//0:data15;1:cd5
        volatile  uint   cd4_data14     :1;//0:data14;1:cd4
        volatile  uint   cd3_data13     :1;//0:data13;1:cd3
        volatile  uint   cd2_data12     :1;//0:data12;1:cd2
        volatile  uint   cd1_data11     :1;//0:data11;1:cd1
        volatile  uint   cd5_data17     :1;//0:data17;1:cd5
        volatile  uint   cd4_data16     :1;//0:data16;1:cd4
        volatile  uint   cd3_cs3        :1;//0:cs3;1:cd3
        volatile  uint   cd2_cs4        :1;//0:cs4;1:cd2
        volatile  uint   cd1_cs5        :1;//0:cs5;1:cd1
#endif
    } mBits;
    volatile  uint   dwValue;
} LCM_SELPIN_U;

typedef struct lcm_dev_reg_tag
{
    volatile LCM_PARAMTER0_U    para0;
    volatile LCM_PARAMTER1_U    para1;
    volatile LCM_RSTN_U         rstn;
    volatile LCM_MODESEL_U      mode_sel;
    volatile LCM_RDDATA_U       rd_data;
    volatile LCM_STATUS_U       status;
    volatile LCM_RGBMODE_U      rgb_mode;
    volatile LCM_SELPIN_U       sel_pin;
} LCM_DEV_REG_T;



/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End




