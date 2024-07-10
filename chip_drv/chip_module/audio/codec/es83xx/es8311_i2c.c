/*
* <es8311_i2c.c> - <This file define the basic hw interfaces of audio codec es8311>
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
//#define EXT_DEV_I2C_ADDR (0x18)

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
LOCAL int32 s_i2c_handle = -1;
LOCAL I2C_DEV s_i2c_dev = {
    5,          /*logic id, which presentes as a specific i2c bus and the i2c slave device connects to this bus. */
                //uint32 id;
    400*1000,   /*i2c slave device's working freqency */
                //uint32 freq;
    0x2,          /*Don't care it, i2c driver will calculate correct value and assign it. */
                //uint32 bus;
    0x30,       /*i2c slave device's write address, whose length is 8 bits, not 7bits.*/
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
    uint16 gain;
    uint16 mute;
} CODEC_GAIN_T;
LOCAL CODEC_GAIN_T codec_adc={0};
LOCAL CODEC_GAIN_T codec_dac={0};

typedef struct
{
    uint8 addr;
    uint8 data;
} I2C_REG_T;

LOCAL uint8 s_cur_dac_spk_mode = 0;

//This is an example for ES8311.You can modify them.
static const I2C_REG_T g_audInitRegList[] =
    {
        {0x45, 0x00}, //BCLK/LRCK pull off
        {0x01, 0x30},
        {0x02, 0x00}, //MCLK prediv:6-1  mult:1,8K 0xa0
        {0x03, 0x10}, //single speed
        {0x16, 0x24}, //ADC
        {0x04, 0x10}, //DAC_OSR
        {0x05, 0x00}, //DIV_ADCCLK DIV_DACCLK
        {0x06, 0x15}, //DIV_BCK:4,codecÎªmasterÊ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
        {0x07, 0x05}, //256LRCK=MCLK  slave:inactive
        {0x08, 0xff}, //256LRCK=MCLK  slave:inactive
        {0x09, 0x0c}, //SDP_IN:IIS 16BIT:0XC,PCM 16BIT:0XF
        {0x0a, 0x0c}, //SDP_OUT:IIS 16BIT0XC,PCM 16BIT:0XF

        {0x0b, 0x00}, //Power UP STAGE
        {0x0c, 0x00}, //Power UP STAGE
        {0x10, 0x03}, //AVDD:3V3
        //{0x10, 0x7f}, //AVDD:1V8
        {0x11, 0x7b}, //internal use
        {0x13, 0x00},
};

//slave
/*      {0x00, 0x80}, //Slave  Mode,0xc0:Master Mode:0x80,start up
        {0x0d, 0x01}, //START UP VMID,power ctrl
        {0x01, 0x3f}, //MCLKï¿½ï¿½Ê±ï¿½ï¿½
        //{0x01,0xBF};//BCLKï¿½ï¿½Ê±ï¿½ï¿½,ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½BCLKÖ»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ñ¡ï¿½ï¿½ï¿½ï¿½MCLK

        {0x0e, 0x02}, //PGA/ADC ENABLE
        //{0x15, 0x40}, //adc vc ramp rate,fade in and fade out
        {0x15, 0x10}, //adc vc ramp rate,fade in and fade out
        {0x1b, 0x0a}, //adc hpf coeff
        {0x1c, 0x6a}, //adc eq bypass
        {0x37, 0x08}, //dac vc ramp rate,fade in and fade out:0x48
        {0x44, 0x08},

        //{0x17, 0x00}, //ADC:-95.5DB
        //{0x32, 0x00}, //DAC:-95.5DB
*/

//master
/*
        {0x00, 0xc0}, //Slave  Mode,0xc0:Master Mode
        {0x0d, 0x01}, //START UP VMID
        {0x01, 0x3f}, //MCLKï¿½ï¿½Ê±ï¿½ï¿½
        //{0x01,0xBF};//BCLKï¿½ï¿½Ê±ï¿½ï¿½,ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½BCLKÖ»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ñ¡ï¿½ï¿½ï¿½ï¿½MCLK

        {0x0e, 0x02},
        //{0x0f, 0x44}, //power mode set
        {0x15, 0x10},
        {0x1b, 0x05},
        {0x1c, 0x65},
        {0x37, 0x08}, //ADC
        {0x44, 0x08},
*/

//This is an example for ES8311.You can modify them.
//g_audCloseRegList:  register list when closing.
static const I2C_REG_T g_audCloseRegList[] =
    {
        {0x32, 0x00}, //DAC VOLUME:-95.5DB
        {0x17, 0x00}, //ADC VOLUME:-95.5DB
        {0x0E, 0xFF}, //POWER DOWN ANALOG PGA/ADC
        {0x12, 0x02}, //POWER DOWN DAC
        {0x14, 0x00}, //NO INPUT,PGA:0DB
        {0x0D, 0xFA}, //POWER DOWN CIRCUITS
        {0x15, 0x00}, //ADC
        {0x37, 0x08}, //DAC EQ BYPASS
        {0x02, 0x00}, //pre_div mult 0x10
        {0x00, 0x00},
        {0x00, 0x1F}, //reset
        {0x01, 0x30},
        {0x01, 0x00}, //clk off
        {0x45, 0x00},
        {0x0D, 0xFC}, //vmid power down //open by bao 20230914
        //{0x02, 0x00}, ////open by bao 20230914

};

//This is an example for ES8311.You can modify them and add lists,such as
//8k,11.025k,12k,16k,22.05k,24k,32k,44.1k,48k.

static const I2C_REG_T g_aud8kRegList[] =
{
    {0x02, 0x00}, //MCLK prediv:6-1  mult:1,8K   MCLK*MUTE_PRE/FS >= 256
    {0x06, 0x15}, //DIV_BCK:codecÎªmasterÊ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
    {0x07, 0x00}, //1536LRCK=MCLK
    {0x08, 0x8f}, //1536LRCK=MCLK
};

static const I2C_REG_T g_aud11kRegList[] =
{
    {0x02, 0x00}, //MCLK prediv:4-1  mult:1,11.025K,12K
    {0x06, 0x0f}, //DIV_BCK:codecÎªmasterÊ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
    {0x07, 0x00}, //1024LRCK=MCLK
    {0x08, 0x5f}, //1024LRCK=MCLK
};

static const I2C_REG_T g_aud12kRegList[] =
{
    {0x02, 0x00}, //MCLK prediv:4-1  mult:1,11.025K,12K
    {0x06, 0x0f}, //DIV_BCK:codecÎªmasterÊ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
    {0x07, 0x00}, //1024LRCK=MCLK
    {0x08, 0x5f}, //1024LRCK=MCLK
};

static const I2C_REG_T g_aud16kRegList[] =
{
    {0x02, 0x00}, //MCLK prediv:3-1  mult:1,16K
    {0x06, 0x0b}, //DIV_BCK:codecÎªmasterÊ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
    {0x07, 0x00}, //768LRCK=MCLK
    {0x08, 0x47}, //768LRCK=MCLK
};

static const I2C_REG_T g_aud22kRegList[] =
{
    {0x02, 0x00}, //MCLK prediv:2-1  mult:1,22.05K,24K
    {0x06, 0x07}, //DIV_BCK:codecÎªmasterÊ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
    {0x07, 0x00}, //512LRCK=MCLK
    {0x08, 0x2f}, //512LRCK=MCLK
};

static const I2C_REG_T g_aud24kRegList[] =
{
    {0x02, 0x00}, //MCLK prediv:2-1  mult:1,22.05K,24K
    {0x06, 0x07}, //DIV_BCK:codecÎªmasterÊ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
    {0x07, 0x00}, //512LRCK=MCLK
    {0x08, 0x2f}, //512LRCK=MCLK
};

static const I2C_REG_T g_aud32kRegList[] =
{
    {0x02, 0x00}, //MCLK prediv:3-1  mult:2,32K
    {0x06, 0x05}, //DIV_BCK:codecÎªmasterÊ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
    {0x07, 0x00}, //256LRCK=MCLK
    {0x08, 0x23}, //256LRCK=MCLK
};

static const I2C_REG_T g_aud44kRegList[] =
{
    {0x02, 0x00}, //MCLK prediv:1-1  mult:1,44.1K,48K
    {0x06, 0x03}, //DIV_BCK:codecÎªmasterÊ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
    {0x07, 0x00}, //256LRCK=MCLK
    {0x08, 0x17}, //256LRCK=MCLK
};

static const I2C_REG_T g_aud48kRegList[] =
{
    {0x02, 0x00}, //MCLK prediv:1-1  mult:1,44.1K,48K
    {0x06, 0x03}, //DIV_BCK:codecÎªmasterÊ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
    {0x07, 0x00}, //256LRCK=MCLK
    {0x08, 0x17}, //256LRCK=MCLK
};

//This is an example for ES8311.(You can modify when recording.)
//main mic:RIN1,LIN1
/*static const I2C_REG_T g_audLin1RegList[] =
    {
        {0x14, 0x1a}, //Ñ¡ï¿½ï¿½CH1ï¿½ï¿½ï¿½ï¿½+30DB GAIN
        {0x17, 0xbf}, //ADC:0DB
};

//This is an example for ES8311.(You can modify when playing.)
static const I2C_REG_T g_audSpkRegList[] =
    {
        {0x12, 0x28}, //enable DAC
        {0x13, 0x00},
        {0x32, 0xbf}, //DAC volume:0DB
};

static const I2C_REG_T g_audMonoRegList[] =
    {
        {0x12, 0x28}, //enable DAC
        {0x13, 0x10}, //HP OUT
        {0x32, 0xbf}, //DAC volume:0DB
};*/


/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
//keep for debug
LOCAL void _i2c_writeOneReg(uint8 addr, uint8 data)
{
    uint32 err = 0;

    if (s_i2c_handle < 0) {
        AUDDRV_ERROR("i2c is not open");
        return;
    }

    err = I2C_HAL_Write(s_i2c_handle, &addr, &data, 1);
    //AUDDRV_DEBUG("aud write reg 0x%02x,0x%02x,%d", addr, data, err);
//    SCI_SLEEP(1);
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
    if (s_i2c_handle >= 0) {
        AUDDRV_TRACE("i2c handle %d", s_i2c_handle);
        return AUDIO_HAL_SUCCESS;
    } else {
        AUDDRV_ERROR("i2c handle %d", s_i2c_handle);
        return AUDIO_HAL_ERROR;
    }
}

PUBLIC void __es83xx_i2c_deinit()
{
    //AUDDRV_TRACE("i2c handle 0x%x", s_i2c_handle);
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
// EQ audio param ini. by bao. wuxx add. 20231118
#if 0
void __es83xx_EQ_DAC_Initialize(void)
{
        //----------LRCK Frequency  : 48000
        //----------EQ1 Mode        : EQ1_BASS
        //----------EQ1 Frequency   : 5~10
        //----------EQ1 BandStopBW  : 2;   BandStopBW  : 10      _i2c_writeOneReg(0x31,0x60);//mute DAC
      _i2c_writeOneReg(0x37,0x08);//close eq
      _i2c_writeOneReg(0x38,0x00);
      _i2c_writeOneReg(0x39,0xF7);
      _i2c_writeOneReg(0x3A,0xFD);
      _i2c_writeOneReg(0x3B,0xFF);
      _i2c_writeOneReg(0x3C,0x22);
      _i2c_writeOneReg(0x3D,0x7D);
      _i2c_writeOneReg(0x3E,0x3D);
      _i2c_writeOneReg(0x3F,0xFF);
      _i2c_writeOneReg(0x40,0x01);
      _i2c_writeOneReg(0x41,0x22);
      _i2c_writeOneReg(0x42,0x16);
      _i2c_writeOneReg(0x43,0xE8);
      _i2c_writeOneReg(0x37,0x00);//open eq
      _i2c_writeOneReg(0x31,0x00);//ummute DAC

}
#endif

//************************Parameter Setting************************//
//----------LRCK Frequency  : 48000
//----------EQ1 Mode        : EQ1_TREBLE
//----------EQ1 Frequency   : 2.000000e-01~10
//----------EQ1 BandStopBW  : 2;   BandStopBW  : 10
void __es83xx_EQ_DAC_Initialize(void)
{
      _i2c_writeOneReg(0x31,0x60);//mute DAC
      _i2c_writeOneReg(0x37,0x08);//close eq
      _i2c_writeOneReg(0x38,0x00);
      _i2c_writeOneReg(0x39,0x00);
      _i2c_writeOneReg(0x3A,0x0C);
      _i2c_writeOneReg(0x3B,0xA7);
      _i2c_writeOneReg(0x3C,0x21);
      _i2c_writeOneReg(0x3D,0x08);
      _i2c_writeOneReg(0x3E,0x4E);
      _i2c_writeOneReg(0x3F,0xF8);
      _i2c_writeOneReg(0x40,0x01);
      _i2c_writeOneReg(0x41,0x54);
      _i2c_writeOneReg(0x42,0x27);
      _i2c_writeOneReg(0x43,0x6F);
      _i2c_writeOneReg(0x37,0x00);//open eq
      _i2c_writeOneReg(0x31,0x00);//ummute DAC
}

void __es83xx_EQ_ADC_Initialize(void)
{
  _i2c_writeOneReg(0x1C,0x6A);
  _i2c_writeOneReg(0x1D,0x08);
  _i2c_writeOneReg(0x1E,0x74);
  _i2c_writeOneReg(0x1F,0xF9);
  _i2c_writeOneReg(0x20,0xFF);
  _i2c_writeOneReg(0x21,0x00);
  _i2c_writeOneReg(0x22,0x3A);
  _i2c_writeOneReg(0x23,0x9B);
  _i2c_writeOneReg(0x24,0x8D);
  _i2c_writeOneReg(0x25,0x22);
  _i2c_writeOneReg(0x26,0x3B);
  _i2c_writeOneReg(0x27,0x5D);
  _i2c_writeOneReg(0x28,0x7C);
  _i2c_writeOneReg(0x29,0x06);
  _i2c_writeOneReg(0x2A,0x64);
  _i2c_writeOneReg(0x2B,0x75);
  _i2c_writeOneReg(0x2C,0xDF);
  _i2c_writeOneReg(0x2D,0x08);
  _i2c_writeOneReg(0x2E,0x74);
  _i2c_writeOneReg(0x2F,0xF9);
  _i2c_writeOneReg(0x30,0xFF);
  _i2c_writeOneReg(0x1C,0x2A);
}

void __es83xx_ALC_Initialize(void)
{
      _i2c_writeOneReg(0x18,0x82);
      _i2c_writeOneReg(0x19,0xFB);
}

PUBLIC uint32 __es83xx_cfg_init(uint32 sampleRate, uint32 mode, uint8 isSlave)
{
    uint32 real_sample_rate = sampleRate;
    
    //SCI_TRACE_LOW("[es8311]__es83xx_cfg_init sampleRate=%d,mode=%d,isSlave=%d",sampleRate,mode,isSlave);
    
    //init
    _i2c_writeListReg(g_audInitRegList, sizeof(g_audInitRegList) / sizeof(I2C_REG_T));
    if (isSlave)
    {
        _i2c_writeOneReg(0x00, 0x80);
    }
    else
    {
        _i2c_writeOneReg(0x00, 0xc0);
    }

    _i2c_writeOneReg(0x0d, 0x01); //START UP VMID,power ctrl
    _i2c_writeOneReg(0x01, 0x3f); //MCLK,BCLK,ADC D/A,DAC D/A on

    if (mode == 0) //record
    {
        _i2c_writeOneReg(0x09, 0x4c); //SDP_IN:IIS 16BIT,mute
        //_i2c_writeOneReg(0x16, 0x24); //ADC
        _i2c_writeOneReg(0x0e, 0x02); //PGA/ADC ENABLE
        //_i2c_writeOneReg(0x0f, 0x84);
        _i2c_writeOneReg(0x0f, 0x44);

        _i2c_writeOneReg(0x15, 0x10); //adc vc ramp rate,fade in and fade out
        _i2c_writeOneReg(0x1b, 0x05); //adc hpf coeff
        _i2c_writeOneReg(0x1c, 0x65); //adc eq bypass
        //_i2c_writeOneReg(0x0f, 0x01);
    }
    else if (mode == 1)
    {
        _i2c_writeOneReg(0x0a, 0x4c); //SDP_OUT:IIS 16BIT,mute
        _i2c_writeOneReg(0x0e, 0x62);
        //_i2c_writeOneReg(0x0f, 0x7b);
        _i2c_writeOneReg(0x0f, 0x44);

        //_i2c_writeOneReg(0x04, 0x10); //DAC_OSR
        _i2c_writeOneReg(0x37, 0x00); //dac vc ramp rate,fade in and fade out:0x48
        //_i2c_writeOneReg(0x0f, 0x01);
		// EQ audio param ini. by bao. wuxx add. 20231118
    }
    else if (mode == 2)
    {
        //_i2c_writeOneReg(0x16, 0x24);
        //_i2c_writeOneReg(0x04, 0x10);
        _i2c_writeOneReg(0x0e, 0x02);
        _i2c_writeOneReg(0x0f, 0x44);

        _i2c_writeOneReg(0x15, 0x10);
        _i2c_writeOneReg(0x1b, 0x05);
        _i2c_writeOneReg(0x1c, 0x65);
        _i2c_writeOneReg(0x37, 0x00);
    }

    //_i2c_writeOneReg(0x44, 0x08);

    //samplerate
    switch (sampleRate)
    {
    case 8000:
        _i2c_writeListReg(g_aud8kRegList, sizeof(g_aud8kRegList) / sizeof(I2C_REG_T));
        break;
    case 11025:
        _i2c_writeListReg(g_aud11kRegList, sizeof(g_aud11kRegList) / sizeof(I2C_REG_T));
        break;
    case 12000:
        _i2c_writeListReg(g_aud12kRegList, sizeof(g_aud12kRegList) / sizeof(I2C_REG_T));
        break;
    case 16000:
        _i2c_writeListReg(g_aud16kRegList, sizeof(g_aud16kRegList) / sizeof(I2C_REG_T));
        break;
    case 22050:
        _i2c_writeListReg(g_aud22kRegList, sizeof(g_aud22kRegList) / sizeof(I2C_REG_T));
        break;
    case 24000:
        _i2c_writeListReg(g_aud24kRegList, sizeof(g_aud24kRegList) / sizeof(I2C_REG_T));
        break;
    case 32000:
        _i2c_writeListReg(g_aud32kRegList, sizeof(g_aud32kRegList) / sizeof(I2C_REG_T));
        break;
    case 44100:
        _i2c_writeListReg(g_aud44kRegList, sizeof(g_aud44kRegList) / sizeof(I2C_REG_T));
        break;
    case 48000:
        _i2c_writeListReg(g_aud48kRegList, sizeof(g_aud48kRegList) / sizeof(I2C_REG_T));
        break;
    default:
        _i2c_writeListReg(g_aud8kRegList, sizeof(g_aud8kRegList) / sizeof(I2C_REG_T));
        real_sample_rate = 0;
        break;
    }
    
    //__es83xx_EQ_ADC_Initialize();
    __es83xx_EQ_DAC_Initialize();
    __es83xx_ALC_Initialize();
    
    codec_dac.mute = 0;
    codec_dac.gain = 0;
    codec_adc.mute = 0;
    codec_adc.gain = 0;

    return real_sample_rate;
}

PUBLIC void __es83xx_cfg_fs(uint32 sampleRate)
{
    //SCI_TRACE_LOW("[es8311]__es83xx_cfg_fs sampleRate=%d",sampleRate);
    _i2c_writeOneReg(0x44, 0x08);

    //samplerate
    switch (sampleRate)
    {
    case 8000:
        _i2c_writeListReg(g_aud8kRegList, sizeof(g_aud8kRegList) / sizeof(I2C_REG_T));
        break;
    case 11025:
        _i2c_writeListReg(g_aud11kRegList, sizeof(g_aud11kRegList) / sizeof(I2C_REG_T));
        break;
    case 12000:
        _i2c_writeListReg(g_aud12kRegList, sizeof(g_aud12kRegList) / sizeof(I2C_REG_T));
        break;
    case 16000:
        _i2c_writeListReg(g_aud16kRegList, sizeof(g_aud16kRegList) / sizeof(I2C_REG_T));
        break;
    case 22050:
        _i2c_writeListReg(g_aud22kRegList, sizeof(g_aud22kRegList) / sizeof(I2C_REG_T));
        break;
    case 24000:
        _i2c_writeListReg(g_aud24kRegList, sizeof(g_aud24kRegList) / sizeof(I2C_REG_T));
        break;
    case 32000:
        _i2c_writeListReg(g_aud32kRegList, sizeof(g_aud32kRegList) / sizeof(I2C_REG_T));
        break;
    case 44100:
        _i2c_writeListReg(g_aud44kRegList, sizeof(g_aud44kRegList) / sizeof(I2C_REG_T));
        break;
    case 48000:
        _i2c_writeListReg(g_aud48kRegList, sizeof(g_aud48kRegList) / sizeof(I2C_REG_T));
        break;
    default:
        _i2c_writeListReg(g_aud8kRegList, sizeof(g_aud8kRegList) / sizeof(I2C_REG_T));
        break;
    }
}

PUBLIC void __es83xx_cfg_swicth(uint32 mode, uint8 isSlave)
{
    //switch
    //SCI_TRACE_LOW("[es8311]__es83xx_cfg_swicth mode=%d,isSlave=%d",mode,isSlave);
    _i2c_writeListReg(g_audInitRegList, sizeof(g_audInitRegList) / sizeof(I2C_REG_T));

    if (isSlave)
    {
        _i2c_writeOneReg(0x00, 0x80);
    }
    else
    {
        _i2c_writeOneReg(0x00, 0xc0);
    }

    _i2c_writeOneReg(0x0d, 0x01); //START UP VMID,power ctrl
    _i2c_writeOneReg(0x01, 0x3f); //MCLKï¿½ï¿½Ê±ï¿½ï¿½

    if (mode == 2) //record
    {
        _i2c_writeOneReg(0x09, 0x4c); //SDP_IN:IIS 16BIT,mute
        //_i2c_writeOneReg(0x16, 0x24); //ADC
        _i2c_writeOneReg(0x0e, 0x02); //PGA/ADC ENABLE
        //_i2c_writeOneReg(0x0f, 0x84);
        _i2c_writeOneReg(0x0f, 0x44);

        _i2c_writeOneReg(0x15, 0x10); //adc vc ramp rate,fade in and fade out
        _i2c_writeOneReg(0x1b, 0x05); //adc hpf coeff
        _i2c_writeOneReg(0x1c, 0x65); //adc eq bypass
        _i2c_writeOneReg(0x0f, 0x01);
    }
    else if (mode == 3) //playback
    {
        _i2c_writeOneReg(0x0a, 0x4c); //SDP_OUT:IIS 16BIT,mute
        _i2c_writeOneReg(0x0e, 0x62);
        //_i2c_writeOneReg(0x0f, 0x7b);
        _i2c_writeOneReg(0x0f, 0x44);

        //_i2c_writeOneReg(0x04, 0x10); //DAC_OSR
        _i2c_writeOneReg(0x37, 0x00); //dac vc ramp rate,fade in and fade out:0x48
        _i2c_writeOneReg(0x0f, 0x01);
    }
    else if (mode == 0 || mode == 1) //record or playback to poc
    {
        //_i2c_writeOneReg(0x16, 0x24);
        //_i2c_writeOneReg(0x04, 0x10);
        _i2c_writeOneReg(0x0e, 0x02);
        _i2c_writeOneReg(0x0f, 0x44);

        _i2c_writeOneReg(0x15, 0x10);
        _i2c_writeOneReg(0x1b, 0x05);
        _i2c_writeOneReg(0x1c, 0x65);
        _i2c_writeOneReg(0x37, 0x00);
    }

    _i2c_writeOneReg(0x44, 0x08);

}

PUBLIC void __es83xx_cfg_close()
{
    _i2c_writeListReg(g_audCloseRegList, sizeof(g_audCloseRegList) / sizeof(I2C_REG_T));
}

PUBLIC void __es83xx_cfg_spk_out_mod(uint8 out_mode)
{
    s_cur_dac_spk_mode = out_mode;
}

/*
DAC µÄÊý×ÖÒôÁ¿×îºÃ²»Òª³¬¹ý 191 £¨¶ÔÓ¦0X32¼Ä´æÆ÷µÄÖµ²»Òª³¬¹ý0XBF £©£¬   
ÔÚÆ½Ì¨Âú·ùÊý×Ö·ù¶ÈÊä³öµÄÊ±ºò£¬ 32¼Ä´æÆ÷×öÕýÔöÒæ»áµ¼ÖÂ½Ø·ù¡£
*/
PUBLIC void __es83xx_cfg_output(uint16 hpFlag, uint16 dac, uint16 mixgain)
{
    if(s_cur_dac_spk_mode > 0)
    {
        dac += 12;
    }
    if (dac > 191) // old: 255 
    {
        dac = 191;
    }
    codec_dac.gain = dac;
    //PA
    /*if (mixgain > 0x12 || mixgain < 0x0)
    {
        mixgain = 0x0;
    }*/

    if (hpFlag == 1)
    {
        _i2c_writeOneReg(0x13, 0x10);
    }
    else
    {
        _i2c_writeOneReg(0x13, 0x00);
        //prvPaWriteOneReg(0x05, mixgain); //GAIN:9db
    }

    _i2c_writeOneReg(0x31, 0x00);
    _i2c_writeOneReg(0x12, 0x00);
    if (!codec_dac.mute) {
        _i2c_writeOneReg(0x32, dac);  //DAC:0xbf:0DB,0-0xff:-95.5-32db
    }
    _i2c_writeOneReg(0x37, 0x00); //DAC RAMPRATE
}

/*
0x14 3:0 ADC PGA gain
0 ¨C 0dB
1 ¨C 3dB
2 ¨C 6dB
3 ¨C 9dB
4 ¨C 12dB
5 ¨C 15dB
6 ¨C 18dB
7 ¨C 21dB
8 ¨C 24dB
9 ¨C 27dB
10 ¨C 30dB
*/
/*
7:0 ADC volume
0x00 ¨C -95.5dB (default)
0x01 ¨C -90.5dB
¡­ 0.5dB/step
0xBE ¨C -0.5dB
0xBF ¨C 0dB
0xC0 ¨C +0.5dB
¡­
0xFF ¨C +32dB
When ALC is on, ADC_VOLUME = MAXGAIN
*/
PUBLIC void __es83xx_cfg_input(uint16 hpFlag, uint16 adc, uint16 pga)
{
    #if 0
        _i2c_writeOneReg(0x14, 0x17); //30db,0-10:0db-30db
        adc = 0xE4;
        codec_adc.gain = adc;

        if (!codec_adc.mute) {
            _i2c_writeOneReg(0x17, adc); //0-0xff:-95.5-32db
        }
    #else
    uint16 pga_reg = ((1 << 4) | pga);
    _i2c_writeOneReg(0x14, pga_reg);
    //_i2c_writeOneReg(0x14, 0x1a); //30db,0-10:0db-30db

    if (adc > 255)
    {
        adc = 255;
    }
    codec_adc.gain = adc;

    if (!codec_adc.mute) {
        _i2c_writeOneReg(0x17, adc); //0-0xff:-95.5-32db
    }
    #endif
}

/*
DAC µÄÊý×ÖÒôÁ¿×îºÃ²»Òª³¬¹ý 191 £¨¶ÔÓ¦0X32¼Ä´æÆ÷µÄÖµ²»Òª³¬¹ý0XBF £©£¬   ÔÚÆ½Ì¨Âú·ùÊý×Ö·ù¶ÈÊä³öµÄÊ±ºò£¬ 32¼Ä´æÆ÷×öÕýÔöÒæ»áµ¼ÖÂ½Ø·ù¡£
*/
PUBLIC void __es83xx_cfg_output_mute(uint16 muteFlag)
{
    if (muteFlag == 1)
    {
        _i2c_writeOneReg(0x12, 0x02); //POWER DOWN DAC
        _i2c_writeOneReg(0x31, 0x60); //DAC DSM MUTE
        _i2c_writeOneReg(0x32, 0x00); //DAC:-95.5DB
        _i2c_writeOneReg(0x37, 0x00); //DAC RAMPRATE:0
        codec_dac.mute = 1;
    }
    else
    {
        _i2c_writeOneReg(0x31, 0x00);
        _i2c_writeOneReg(0x12, 0x00);
        //_i2c_writeOneReg(0x32, 0xbf); //DAC:0DB
        _i2c_writeOneReg(0x32, codec_dac.gain); //DAC:0DB
        _i2c_writeOneReg(0x37, 0x40); //DAC RAMPRATE
        codec_dac.mute = 0;
    }

}
PUBLIC void __es83xx_cfg_input_mute(uint16 muteFlag)
{
    if (muteFlag == 1)
    {
        _i2c_writeOneReg(0x17, 0x00); //ADC:-95.5DB
        codec_adc.mute = 1;
    }
    else
    {
        _i2c_writeOneReg(0x17, codec_adc.gain); //0-0xff:-95.5-32db
        //_i2c_writeOneReg(0x17, 0xbf); //ADC:0DB
        codec_adc.mute = 0;
    }
}
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/

// End
