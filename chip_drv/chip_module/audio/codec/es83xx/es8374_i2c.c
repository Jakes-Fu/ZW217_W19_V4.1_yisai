/*
* <es8374_i2c.c> - <This file define the basic hw interfaces of audio codec es8374>
*
* Copyright (c) 2019 Unisoc Communications Inc.
* History
*      <2022/07/21> <rui.huang> <create>
*/

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "os_api.h"
#include "sci_api.h"
#include "sci_types.h"
#include "audio_codec.h"
#include "audio_drvapi.h"

#include "es83xx_i2c.h"

#include "i2c_drvapi.h"

/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/
//#define EXT_DEV_I2C_ADDR (0x10)

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
LOCAL int32 s_i2c_handle = -1;
LOCAL I2C_DEV s_i2c_dev = {//unchecked
    3,          /*logic id, which presentes as a specific i2c bus and the i2c slave device connects to this bus. */
                //uint32 id;
    100*1000,   /*i2c slave device's working freqency */
                //uint32 freq;
    0,          /*Don't care it, i2c driver will calculate correct value and assign it. */
                //uint32 bus;
    0x20,       /*i2c slave device's write address, whose length is 8 bits, not 7bits.*/
                //uint8 slave_addr;
    1,          /*i2c slave device's internal register length */
                //uint8 reg_addr_num;
    1,          /*set to 1, i2c controller will check i2c slave device's ACK signal, otherwise, set it to 0*/
                //uint8 check_ack;
    0           /*set to 1, i2c controller will send restart signal to i2c slave device in read transantion. */
                //uint8 no_stop;
};

typedef struct
{
    uint8 addr;
    uint8 data;
} I2C_REG_T;

//This is an example for ES8374.You can modify them.
LOCAL const I2C_REG_T g_audInitRegList_slave[3][14] =
    {
        {
            //record
            {0x00, 0x3f},
            {0x00, 0x03},
            {0x01, 0xea},
            {0x0f, 0x01}, // slave
            /*{0x6f, 0xa0},
            {0x72, 0x41},
            {0x09, 0x01},
            {0x0c, 0x17},
            {0x0d, 0xa3},
            {0x0e, 0x30},
            {0x0a, 0x8a},
            {0x0b, 0x07},
            {0x09, 0x41},*/
            {0x10, 0x0c}, //i2s 16bit,
            //{0x10, 0x0f}, //pcm 16bit,
            {0x24, 0x28}, //adc set mute
            {0x36, 0x20}, //dac set mute
            {0x12, 0x40},
            {0x13, 0x40},
            {0x00, 0x81}, // IC START

            {0x16, 0x1f},
            {0x17, 0x43},
            {0x14, 0x8a},
            {0x15, 0x62},
        },
        {
            //play
            {0x00, 0x3f},
            {0x00, 0x03},
            {0x01, 0xff},
            {0x0f, 0x01}, // slave
            /*{0x6f, 0xa0},
            {0x72, 0x41},
            {0x09, 0x01},
            {0x0c, 0x17},
            {0x0d, 0xa3},
            {0x0e, 0x30},
            {0x0a, 0x8a},
            {0x0b, 0x07},
            {0x09, 0x41},*/
            {0x11, 0x0c}, //i2s 16bit,
            //{0x11, 0x0f}, //pcm 16bit,
            {0x24, 0x28}, //adc set mute
            {0x36, 0x20}, //dac set mute
            {0x12, 0x40},
            {0x13, 0x40},
            {0x00, 0x80}, // IC START

            {0x16, 0x7b},
            {0x17, 0xa8},
            {0x14, 0x8a},
            {0x15, 0x4c},
        },
        {
            //poc
            {0x00, 0x3f},
            {0x00, 0x03},
            {0x01, 0xff},
            {0x0f, 0x01}, // slave
            /*{0x6f, 0xa0},
            {0x72, 0x41},
            {0x09, 0x01},
            {0x0c, 0x17},
            {0x0d, 0xa3},
            {0x0e, 0x30},
            {0x0a, 0x8a},
            {0x0b, 0x07},
            {0x09, 0x41},*/
            {0x10, 0x0c}, //i2s 16bit,
            {0x11, 0x0c}, //i2s 16bit,
            //{0x10, 0x0f}, //pcm 16bit,
            //{0x11, 0x0f}, //pcm 16bit,
            {0x24, 0x28}, //adc set mute
            {0x36, 0x20}, //dac set mute
            {0x12, 0x40},
            {0x13, 0x40},
            {0x00, 0x80}, // IC START

            {0x17, 0xa8},
            {0x14, 0x8a},
            {0x15, 0x40},
        },
};

//This is an example for ES8374.You can modify them.
LOCAL const I2C_REG_T g_audSwitchRegList_slave[4][14] =
{
    {
//playback + capture
        {0x00, 0x3f},
        {0x00, 0x03},
        {0x01, 0xff},
        {0x0f, 0x01}, // slave
        /*{0x6f, 0xa0},
        {0x72, 0x41},
        {0x09, 0x01},
        {0x0c, 0x17},
        {0x0d, 0xa3},
        {0x0e, 0x30},
        {0x0a, 0x8a},
        {0x0b, 0x07},
        {0x09, 0x41},*/
        {0x10, 0x0c}, //i2s 16bit,
        {0x11, 0x0c}, //i2s 16bit,
        //{0x10, 0x0f}, //pcm 16bit,
        //{0x11, 0x0f}, //pcm 16bit,
        {0x24, 0x28}, //adc set mute
        {0x36, 0x20}, //dac set mute
        {0x12, 0x40},
        {0x13, 0x40},
        {0x00, 0x80}, // IC START

        {0x17, 0xa8},
        {0x14, 0x8a},
        {0x15, 0x40},

    },
    {
//capture + playback
        {0x00, 0x3f},
        {0x00, 0x03},
        {0x01, 0xff},
        {0x0f, 0x01}, // slave
        /*{0x6f, 0xa0},
        {0x72, 0x41},
        {0x09, 0x01},
        {0x0c, 0x17},
        {0x0d, 0xa3},
        {0x0e, 0x30},
        {0x0a, 0x8a},
        {0x0b, 0x07},
        {0x09, 0x41},*/
        {0x10, 0x0c}, //i2s 16bit,
        {0x11, 0x0c}, //i2s 16bit,
        //{0x10, 0x0f}, //pcm 16bit,
        //{0x11, 0x0f}, //pcm 16bit,
        {0x24, 0x28}, //adc set mute
        {0x36, 0x20}, //dac set mute
        {0x12, 0x40},
        {0x13, 0x40},
        {0x00, 0x80}, // IC START

        {0x17, 0xa8},
        {0x14, 0x8a},
        {0x15, 0x40},
    },
    {
//left record
        {0x00, 0x3f},
        {0x00, 0x03},
        {0x01, 0xea},
        {0x0f, 0x01}, // slave
        /*{0x6f, 0xa0},
        {0x72, 0x41},
        {0x09, 0x01},
        {0x0c, 0x17},
        {0x0d, 0xa3},
        {0x0e, 0x30},
        {0x0a, 0x8a},
        {0x0b, 0x07},
        {0x09, 0x41},*/
        {0x10, 0x0c}, //i2s 16bit,
        //{0x10, 0x0f}, //pcm 16bit,
        {0x24, 0x28}, //adc set mute
        {0x36, 0x20}, //dac set mute
        {0x12, 0x40},
        {0x13, 0x40},
        {0x00, 0x81}, // IC START

        {0x16, 0x1f},
        {0x17, 0x43},
        {0x14, 0x8a},
        {0x15, 0x62},
    },
    {
//left play
        {0x00, 0x3f},
        {0x00, 0x03},
        {0x01, 0xff},
        {0x0f, 0x01}, // slave
        /*{0x6f, 0xa0},
        {0x72, 0x41},
        {0x09, 0x01},
        {0x0c, 0x17},
        {0x0d, 0xa3},
        {0x0e, 0x30},
        {0x0a, 0x8a},
        {0x0b, 0x07},
        {0x09, 0x41},*/
        {0x11, 0x0c}, //i2s 16bit,
        //{0x11, 0x0f}, //pcm 16bit,
        {0x24, 0x28}, //adc set mute
        {0x36, 0x20}, //dac set mute
        {0x12, 0x40},
        {0x13, 0x40},
        {0x00, 0x80}, // IC START

        {0x16, 0x7b},
        {0x17, 0xa8},
        {0x14, 0x8a},
        {0x15, 0x4c},
    },
};

LOCAL const I2C_REG_T g_audInitRegList_master[] =
    {
        {0x00, 0x3f},
        {0x00, 0x03},
        {0x01, 0xff},
        {0x6f, 0xa0},
        {0x72, 0x41},
        {0x09, 0x01},
        {0x0c, 0x17},
        {0x0d, 0xa3},
        {0x0e, 0x30},
        {0x0f, 0x81}, //codec slave  {0x0f, 0x01},  //codec master:{0x0f, 0x81}
        {0x0a, 0x8a},
        {0x0b, 0x07},
        {0x09, 0x41},
        {0x10, 0x0c},
        {0x11, 0x0c},
        {0x24, 0x28},
        {0x36, 0x20},
        {0x12, 0x30},
        {0x13, 0x20},
        {0x2c, 0x05},
        {0x2d, 0x05},
        {0x00, 0x80},
        //{0x02, 0x08},  //pll on,noise
        {0x14, 0x0a},
        {0x15, 0x40},

};

//This is an example for ES8374.You can modify them.
//g_audCloseRegList:  register list when closing.
LOCAL const I2C_REG_T g_audCloseRegList[] =
    {
        /*{0x38, 0xc0},
    {0x25, 0xc0},
    {0x24, 0x28},
    {0x36, 0x20},
    {0x37, 0x21},
    {0x1a, 0x08},
    {0x1c, 0x10},
    {0x1d, 0x10},
    {0x1e, 0x40},
    {0x21, 0xd4},
    {0x15, 0xbf},
    {0x14, 0x14},
    {0x09, 0x80},
    {0x01, 0x03},*/

        {0x38, 0xc0},
        {0x25, 0xc0},
        {0x28, 0x1c},
        {0x36, 0x20},
        {0x37, 0x01},
        {0x1a, 0x08},
        {0x1d, 0x10},
        {0x1e, 0x40},
        {0x1c, 0x10},
        {0x24, 0x20},
        {0x22, 0x00},
        {0x21, 0xd4},
        {0x15, 0xbf},
        {0x14, 0x94},
        {0x09, 0x80},
        {0x01, 0x03},

};

//This is an example for ES8374.You can modify them and add lists,such as
//8k,11.025k,12k,16k,22.05k,24k,32k,44.1k,48k.

LOCAL const I2C_REG_T g_aud8kRegList_master[] =
    {
        {0x0b, 0x07}, //mclk:12288000,use pll
        {0x0c, 0x17},
        {0x0d, 0xa3},
        {0x0e, 0x30},
        {0x0f, 0x9d}, //master,BCLKDIV:0x1d:MCLK/48
        {0x05, 0x66}, //adcdiv/dacdiv
        /* {0x06, 0x06}, //use pll,lrckdiv=mclk/lrck
    {0x07, 0x00},
    {0x03, 0x20},*/
        {0x06, 0x06}, //mclk:13000000,not use pll,lrckdiv
        {0x07, 0x59},
        {0x03, 0x20},

};

LOCAL const I2C_REG_T g_aud11kRegList_master[] =
    {
        {0x0b, 0x06}, //mclk:11289600,use pll
        {0x0c, 0x27},
        {0x0d, 0xdc},
        {0x0e, 0x2b},
        {0x0f, 0x98}, //master,BCLKDIV:0x18:MCLK/24
        {0x05, 0x44},
        /* {0x06, 0x04}, //use pll
    {0x07, 0x00},
    {0x03, 0x20},*/
        {0x06, 0x04}, //mclk:13000000,not use pll
        {0x07, 0x9b},
        {0x03, 0x20},
};

LOCAL const I2C_REG_T g_aud12kRegList_master[] =
    {
        {0x0b, 0x07}, //mclk:12288000,use pll
        {0x0c, 0x17},
        {0x0d, 0xa3},
        {0x0e, 0x30},
        {0x0f, 0x98}, //master,BCLKDIV:0x18:MCLK/24
        {0x05, 0x44},
        /*{0x06, 0x04}, //use pll
    {0x07, 0x00},
    {0x03, 0x20},*/
        {0x06, 0x04}, //mclk:13000000,not use pll
        {0x07, 0x3b},
        {0x03, 0x20},
};

LOCAL const I2C_REG_T g_aud16kRegList_master[] =
    {
        {0x0b, 0x07}, //mclk:12288000,use pll
        {0x0c, 0x17},
        {0x0d, 0xa3},
        {0x0e, 0x30},
        {0x0f, 0x95}, //master,BCLKDIV:0x15:MCLK/21
        {0x05, 0x33},
        /*{0x06, 0x03}, //use pll
    {0x07, 0x00},
    {0x03, 0x20},*/
        {0x06, 0x03}, //mclk:13000000,not use pll
        {0x07, 0x2d},
        {0x03, 0x20},
};

LOCAL const I2C_REG_T g_aud22kRegList_master[] =
    {
        {0x0b, 0x06}, //mclk:11289600,use pll
        {0x0c, 0x27},
        {0x0d, 0xdc},
        {0x0e, 0x2b},
        {0x0f, 0x90}, //master,BCLKDIV:0x10:MCLK/16
        {0x05, 0x22},
        /*{0x06, 0x02}, //use pll
    {0x07, 0x00},
    {0x03, 0x20},*/
        {0x06, 0x02}, //mclk:13000000,not use pll
        {0x07, 0x4e},
        {0x03, 0x20},
};

LOCAL const I2C_REG_T g_aud24kRegList_master[] =
    {
        {0x0b, 0x07}, //mclk:12288000,use pll
        {0x0c, 0x17},
        {0x0d, 0xa3},
        {0x0e, 0x30},
        {0x0f, 0x90}, //master,BCLKDIV:0x10:MCLK/16
        {0x05, 0x22},
        /*{0x06, 0x02}, //use pll
    {0x07, 0x00},
    {0x03, 0x20},*/
        {0x06, 0x02}, //mclk:13000000,not use pll
        {0x07, 0x1e},
        {0x03, 0x20},
};

LOCAL const I2C_REG_T g_aud32kRegList_master[] =
    {
        {0x0b, 0x07}, //mclk:12288000,use pll
        {0x0c, 0x17},
        {0x0d, 0xa3},
        {0x0e, 0x30},
        {0x0f, 0x8c}, //master,BCLKDIV:0xc:MCLK/12
        {0x05, 0x11},
        /*{0x06, 0x01}, //use pll
    {0x07, 0x80},
    {0x03, 0x20},*/
        {0x06, 0x01}, //mclk:13000000,not use pll
        {0x07, 0x96},
        {0x03, 0x20},
};

LOCAL const I2C_REG_T g_aud44kRegList_master[] =
    {
        {0x0b, 0x06}, //mclk:11289600,use pll
        {0x0c, 0x27},
        {0x0d, 0xdc},
        {0x0e, 0x2b},
        {0x0f, 0x88}, //master,BCLKDIV:0x8:MCLK/8
        {0x05, 0x11},
        /*{0x06, 0x01}, //use pll
    {0x07, 0x00},
    {0x03, 0x20},*/
        {0x06, 0x01}, //mclk:13000000,not use pll
        {0x07, 0x27},
        {0x03, 0x20},
};

LOCAL const I2C_REG_T g_aud48kRegList_master[] =
    {
        {0x0b, 0x07}, //mclk:12288000,use pll
        {0x0c, 0x17},
        {0x0d, 0xa3},
        {0x0e, 0x30},
        {0x0f, 0x88}, //master,BCLKDIV:0x8:MCLK/8
        {0x05, 0x11},
        /*{0x06, 0x01}, //use pll
    {0x07, 0x00},
    {0x03, 0x20},*/
        {0x06, 0x01}, //mclk:13000000,not use pll
        {0x07, 0x0f},
        {0x03, 0x20},
};

LOCAL const I2C_REG_T g_aud8kRegList_slave[] =
    {
        {0x0b, 0x07},
        {0x0c, 0x17},
        {0x0d, 0xa3},
        {0x0e, 0x30},
        {0x05, 0x66},
        {0x06, 0x06},
        {0x07, 0x00},
        {0x03, 0x20},
};

LOCAL const I2C_REG_T g_aud11kRegList_slave[] =
    {
        {0x0b, 0x06}, //mclk:11289600
        {0x0c, 0x27},
        {0x0d, 0xdc},
        {0x0e, 0x2b},
        {0x05, 0x44},
        {0x06, 0x04},
        {0x07, 0x00},
        {0x03, 0x20},
};

LOCAL const I2C_REG_T g_aud12kRegList_slave[] =
    {
        {0x0b, 0x07}, //mclk:12288000
        {0x0c, 0x17},
        {0x0d, 0xa3},
        {0x0e, 0x30},
        {0x05, 0x44},
        {0x06, 0x04},
        {0x07, 0x00},
        {0x03, 0x20},
};

LOCAL const I2C_REG_T g_aud16kRegList_slave[] =
    {
        {0x0b, 0x07},
        {0x0c, 0x17},
        {0x0d, 0xa3},
        {0x0e, 0x30},
        {0x05, 0x33},
        {0x06, 0x03},
        {0x07, 0x00},
        {0x03, 0x20},
};

LOCAL const I2C_REG_T g_aud22kRegList_slave[] =
    {
        {0x0b, 0x06}, //mclk:11289600
        {0x0c, 0x27},
        {0x0d, 0xdc},
        {0x0e, 0x2b},
        {0x05, 0x22},
        {0x06, 0x02},
        {0x07, 0x00},
        {0x03, 0x20},
};

LOCAL const I2C_REG_T g_aud24kRegList_slave[] =
    {
        {0x0b, 0x07}, //mclk:12288000
        {0x0c, 0x17},
        {0x0d, 0xa3},
        {0x0e, 0x30},
        {0x05, 0x22},
        {0x06, 0x02},
        {0x07, 0x00},
        {0x03, 0x20},
};

LOCAL const I2C_REG_T g_aud32kRegList_slave[] =
    {
        {0x0b, 0x07},
        {0x0c, 0x17},
        {0x0d, 0xa3},
        {0x0e, 0x30},
        {0x05, 0x11},
        {0x06, 0x01},
        {0x07, 0x80},
        {0x03, 0x20},
};

LOCAL const I2C_REG_T g_aud44kRegList_slave[] =
    {
        {0x0b, 0x06},
        {0x0c, 0x27},
        {0x0d, 0xdc},
        {0x0e, 0x2b},
        {0x05, 0x11},
        {0x06, 0x01},
        {0x07, 0x00},
        {0x03, 0x20},
};

LOCAL const I2C_REG_T g_aud48kRegList_slave[] =
    {
        {0x0b, 0x07},
        {0x0c, 0x17},
        {0x0d, 0xa3},
        {0x0e, 0x30},
        {0x05, 0x11},
        {0x06, 0x01},
        {0x07, 0x00},
        {0x03, 0x20},
};

//This is an example for ES8374.(You can modify when recording.)
//main mic:RIN1,LIN1
LOCAL const I2C_REG_T g_audLin1RegList[] =
    {
        {0x21, 0x14},
        {0x24, 0x08},
        {0x28, 0x00},

};

//head mic:Lin2-Rin2,
LOCAL const I2C_REG_T g_audLin2RegList[] =
    {
        {0x21, 0x24},
        {0x24, 0x08},
        //{0x26, 0x4a},
        //{0x28, 0x80},
        {0x28, 0x00},
};

//This is an example for ES8374.(You can modify when playing.)
LOCAL const I2C_REG_T g_audSpkRegList[] =
    {
        {0x1a, 0x18},
        {0x1c, 0x90},
        //{0x1d, 0x00},
        //{0x1f, 0x00},
        //{0x1e, 0xa0},
};

LOCAL const I2C_REG_T g_audMonoRegList[] =
    {
        {0x1a, 0xa0}, //?bit6
                      //{0x1b, 0x1b}, //4//4db
                      //{0x1e, 0x40},
};

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
//keep for debug
LOCAL void _i2c_writeOneReg(uint8 addr, uint8 data)
{
    uint32 err = 0;
    if (s_i2c_handle < 0) {
        AUDDRV_ERROR("i2c is not open");
    }

    err = I2C_HAL_Write(s_i2c_handle, &addr, &data, 1);
    AUDDRV_DEBUG("aud write reg 0x%02x--0x%02x, ret%d", addr, data, err);
    SCI_SLEEP(1);
}

LOCAL void _i2c_writeListReg(const I2C_REG_T *regList, uint16 len)
{
    uint16 regCount;

    for (regCount = 0; regCount < len; regCount++) {
        _i2c_writeOneReg(regList[regCount].addr, regList[regCount].data);

        //SCI_SLEEP(1);
        if (regList[regCount].addr == 0x0 && regList[regCount].data == 0x80) {
            SCI_SLEEP(50);
        }

        if (regList[regCount].addr == 0x0 && regList[regCount].data == 0x3f) {
            SCI_SLEEP(1);
        }
    }
}

LOCAL uint32 __es8374_cfg_master_init(uint32 sampleRate)
{
    uint32 real_sampleRate = 8000;
    //init
    _i2c_writeListReg(g_audInitRegList_master, sizeof(g_audInitRegList_master) / sizeof(I2C_REG_T));

    //samplerate,master
    switch (sampleRate)
    {
    case 8000:
        _i2c_writeListReg(g_aud8kRegList_master, sizeof(g_aud8kRegList_master) / sizeof(I2C_REG_T));
        real_sampleRate = 8000;
        break;
    case 11025:
        _i2c_writeListReg(g_aud11kRegList_master, sizeof(g_aud11kRegList_master) / sizeof(I2C_REG_T));
        real_sampleRate = 11025;
        break;
    case 12000:
        _i2c_writeListReg(g_aud12kRegList_master, sizeof(g_aud12kRegList_master) / sizeof(I2C_REG_T));
        real_sampleRate = 12000;
        break;
    case 16000:
        _i2c_writeListReg(g_aud16kRegList_master, sizeof(g_aud16kRegList_master) / sizeof(I2C_REG_T));
        real_sampleRate = 16000;
        break;
    case 22050:
        _i2c_writeListReg(g_aud22kRegList_master, sizeof(g_aud22kRegList_master) / sizeof(I2C_REG_T));
        real_sampleRate = 22050;
        break;
    case 24000:
        _i2c_writeListReg(g_aud24kRegList_master, sizeof(g_aud24kRegList_master) / sizeof(I2C_REG_T));
        real_sampleRate = 24000;
        break;
    case 32000:
        _i2c_writeListReg(g_aud32kRegList_master, sizeof(g_aud32kRegList_master) / sizeof(I2C_REG_T));
        real_sampleRate = 32000;
        break;
    case 44100:
        _i2c_writeListReg(g_aud44kRegList_master, sizeof(g_aud44kRegList_master) / sizeof(I2C_REG_T));
        real_sampleRate = 44100;
        break;
    case 48000:
        _i2c_writeListReg(g_aud48kRegList_master, sizeof(g_aud48kRegList_master) / sizeof(I2C_REG_T));
        real_sampleRate = 48000;
        break;
    default:
        _i2c_writeListReg(g_aud8kRegList_master, sizeof(g_aud8kRegList_master) / sizeof(I2C_REG_T));
        real_sampleRate = 8000;
        break;
    }
    return real_sampleRate;
}

LOCAL uint32 __es8374_cfg_slave_init(uint32 sampleRate, uint32 mode)
{
    uint32 real_sampleRate = 8000;
    //init
    _i2c_writeListReg(g_audInitRegList_slave[mode], sizeof(g_audInitRegList_slave[mode]) / sizeof(I2C_REG_T));

    //samplerate,slave
    switch (sampleRate)
    {
    case 8000:
        _i2c_writeListReg(g_aud8kRegList_slave, sizeof(g_aud8kRegList_slave) / sizeof(I2C_REG_T));
        real_sampleRate = 8000;
        break;
    case 11025:
        _i2c_writeListReg(g_aud11kRegList_slave, sizeof(g_aud11kRegList_slave) / sizeof(I2C_REG_T));
        real_sampleRate = 11025;
        break;
    case 12000:
        _i2c_writeListReg(g_aud12kRegList_slave, sizeof(g_aud12kRegList_slave) / sizeof(I2C_REG_T));
        real_sampleRate = 12000;
        break;
    case 16000:
        _i2c_writeListReg(g_aud16kRegList_slave, sizeof(g_aud16kRegList_slave) / sizeof(I2C_REG_T));
        real_sampleRate = 16000;
        break;
    case 22050:
        _i2c_writeListReg(g_aud22kRegList_slave, sizeof(g_aud22kRegList_slave) / sizeof(I2C_REG_T));
        real_sampleRate = 22050;
        break;
    case 24000:
        _i2c_writeListReg(g_aud24kRegList_slave, sizeof(g_aud24kRegList_slave) / sizeof(I2C_REG_T));
        real_sampleRate = 24000;
        break;
    case 32000:
        _i2c_writeListReg(g_aud32kRegList_slave, sizeof(g_aud32kRegList_slave) / sizeof(I2C_REG_T));
        real_sampleRate = 32000;
        break;
    case 44100:
        _i2c_writeListReg(g_aud44kRegList_slave, sizeof(g_aud44kRegList_slave) / sizeof(I2C_REG_T));
        real_sampleRate = 44100;
        break;
    case 48000:
        _i2c_writeListReg(g_aud48kRegList_slave, sizeof(g_aud48kRegList_slave) / sizeof(I2C_REG_T));
        real_sampleRate = 48000;
        break;
    default:
        _i2c_writeListReg(g_aud8kRegList_slave, sizeof(g_aud8kRegList_slave) / sizeof(I2C_REG_T));
        real_sampleRate = 8000;
        break;
    }
    return real_sampleRate;
}

LOCAL void __es8374_cfg_slave_switch(uint32 mode)
{
    _i2c_writeListReg(g_audSwitchRegList_slave[mode], sizeof(g_audSwitchRegList_slave[mode]) / sizeof(I2C_REG_T));
}

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*
#define AUDIO_SPK           (0x0001)
#define AUDIO_EAR           (0x0002)
#define AUDIO_HP            (0x0004)
#define AUDIO_MIC           (0x0008)
#define AUDIO_LINEIN        (0x0010)
#define AUDIO_HP_MIC        (0x0020)
*/
PUBLIC uint32 __es83xx_support_device(void)
{
    return AUDIO_SPK | AUDIO_HP | AUDIO_MIC | AUDIO_HP_MIC;
}

PUBLIC void __es83xx_modify_device(uint32 *device, uint32 direct)
{
    uint32 ret_device = 0;
    if (direct == AUDIO_CAPTRUE)
    {
        // capture(ul)
        ret_device = (AUDIO_HP_MIC & (*device)) ? AUDIO_HP_MIC : AUDIO_MIC;
    }
    else
    {
        // playback(dl)
        ret_device = (AUDIO_HP & (*device)) ? AUDIO_HP : AUDIO_SPK;
    }
    AUDDRV_TRACE("set path 0x%x to 0x%x", *device, ret_device);
    *device = ret_device;
}

PUBLIC int32 __es83xx_i2c_init()
{
    if (s_i2c_handle >= 0) {
        AUDDRV_TRACE("i2c handle already opened %d", s_i2c_handle);
        return AUDIO_HAL_SUCCESS;
    }

    s_i2c_handle = I2C_HAL_Open(&s_i2c_dev);
    if (s_i2c_handle > 0) {
        AUDDRV_TRACE("i2c handle %d", s_i2c_handle);
        return AUDIO_HAL_SUCCESS;
    } else {
        AUDDRV_ERROR("i2c handle %d", s_i2c_handle);
        return AUDIO_HAL_ERROR;
    }
}

PUBLIC void __es83xx_i2c_deinit()
{
    AUDDRV_TRACE("i2c handle 0x%x", s_i2c_handle);
    if (s_i2c_handle > 0) {
        I2C_HAL_Close(s_i2c_handle);
        s_i2c_handle = -1;
    }
}

PUBLIC uint32 __es83xx_fs_check(uint32 sampleRate)
{
    switch (sampleRate)
    {
    case 8000:
    case 11025:
    case 12000:
    case 16000:
    case 22050:
    case 24000:
    case 32000:
    case 44100:
    case 48000:
        return sampleRate;
    default:
        return 0;
    }
    return 0;
}

PUBLIC uint32 __es83xx_cfg_init(uint32 sampleRate, uint32 mode, uint8 isSlave)
{
    if (isSlave)
    {
        return __es8374_cfg_slave_init(sampleRate, mode);
    }
    else
    {
        return __es8374_cfg_master_init(sampleRate);
    }
}

PUBLIC void __es83xx_cfg_swicth(uint32 mode, uint8 isSlave)
{
    if (isSlave)
    {
        __es8374_cfg_slave_switch(mode);
    }
//    else
//    {
//        // no need
//        return __es8374_cfg_master_init(sampleRate);
//    }
}

PUBLIC void __es83xx_cfg_close()
{
    _i2c_writeListReg(g_audCloseRegList, sizeof(g_audCloseRegList) / sizeof(I2C_REG_T));
}

PUBLIC void __es83xx_cfg_output(uint16 hpFlag, uint16 dac, uint16 mixgain)
{
    if (dac > 192)
    {
        dac = 192;
    }

    //es8374:REG0X1D/0x1B:0x20-0x2b/0x30-0x3b:-21~-9.5db,0-0xb/0x10-0x1b,-8~4db
    if (mixgain > 0xb && mixgain < 0x10)
    {
        mixgain = 0xb;
    }

    if (hpFlag == 1)
    {
        _i2c_writeListReg(g_audMonoRegList, sizeof(g_audMonoRegList) / sizeof(I2C_REG_T));
        _i2c_writeOneReg(0x1b, mixgain);
        _i2c_writeOneReg(0x1e, 0x40);
    }
    else
    {
        _i2c_writeListReg(g_audSpkRegList, sizeof(g_audSpkRegList) / sizeof(I2C_REG_T));
        _i2c_writeOneReg(0x1d, mixgain);
        _i2c_writeOneReg(0x1f, 0x00);
        _i2c_writeOneReg(0x1e, 0xa0);
    }

    _i2c_writeOneReg(0x36, 0x00);
    _i2c_writeOneReg(0x38, dac);
    _i2c_writeOneReg(0x37, 0x00);
}

PUBLIC void __es83xx_cfg_input(uint16 hpFlag, uint16 adc, uint16 pga)
{
    if (hpFlag == 1)
    {
        _i2c_writeListReg(g_audLin2RegList, sizeof(g_audLin2RegList) / sizeof(I2C_REG_T));
    }
    else
    {
        _i2c_writeListReg(g_audLin1RegList, sizeof(g_audLin1RegList) / sizeof(I2C_REG_T));
    }

    if (pga > 7)
    {
        pga = 7;
    }

    if (adc > 192)
    {
        adc = 192;
    }

    _i2c_writeOneReg(0x22, pga);
    _i2c_writeOneReg(0x25, adc);
}

PUBLIC void __es83xx_cfg_output_mute(uint16 muteFlag)
{
    if (muteFlag == 1)
    {
        _i2c_writeOneReg(0x36, 0x20);
    }
    else
    {
        _i2c_writeOneReg(0x36, 0x00);
    }
}
PUBLIC void __es83xx_cfg_input_mute(uint16 muteFlag)
{
    if (muteFlag == 1)
    {
        _i2c_writeOneReg(0x24, 0x28);
    }
    else
    {
        _i2c_writeOneReg(0x24, 0x8);
    }
}
PUBLIC void __es83xx_cfg_fs(uint32 sampleRate)
{

}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/

// End
