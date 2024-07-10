/*
* <extpa_drv_aw87390.c> - <This file define the basic hw interfaces of extpa which is controled by i2c （aw87390）>
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

#include "i2c_drvapi.h"

/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
LOCAL uint16 pa_gain = 0;
LOCAL int32 s_i2c_handle_pa = -1;
LOCAL I2C_DEV s_i2c_dev_pa = {
    6,          /*logic id, which presentes as a specific i2c bus and the i2c slave device connects to this bus. */
                //uint32 id;
    400*1000,   /*i2c slave device's working freqency */
                //uint32 freq;
    0x2,          /*Don't care it, i2c driver will calculate correct value and assign it. */
                //uint32 bus;
    0xB0,       /*i2c slave device's write address, whose length is 8 bits, not 7bits.*/
                //uint8 slave_addr;
    1,          /*i2c slave device's internal register length */
                //uint8 reg_addr_num;
    1,          /*set to 1, i2c controller will check i2c slave device's ACK signal, otherwise, set it to 0*/
                //uint8 check_ack;
    0           /*set to 1, i2c controller will send restart signal to i2c slave device in read transantion. */
                //uint8 no_stop;
};

LOCAL int32 __aw87390_i2c_init()
{
    if (s_i2c_handle_pa >= 0) {
        AUDDRV_TRACE("i2c handle already opened %d", s_i2c_handle_pa);
        return AUDIO_HAL_SUCCESS;
    }

    s_i2c_handle_pa = I2C_HAL_Open(&s_i2c_dev_pa);
    if (s_i2c_handle_pa >= 0) {
        AUDDRV_TRACE("i2c handle %d", s_i2c_handle_pa);
        return AUDIO_HAL_SUCCESS;
    } else {
        AUDDRV_ERROR("i2c handle %d", s_i2c_handle_pa);
        return AUDIO_HAL_ERROR;
    }
}

LOCAL void __aw87390_i2c_deinit()
{
    AUDDRV_TRACE("i2c handle 0x%x", s_i2c_handle_pa);
    if (s_i2c_handle_pa >= 0) {
        I2C_HAL_Close(s_i2c_handle_pa);
        s_i2c_handle_pa = -1;
    }
}

LOCAL void _i2c_writeOneReg(uint8 addr, uint8 data)
{
    uint32 err = 0;

    if (s_i2c_handle_pa < 0) {
        AUDDRV_ERROR("i2c is not open");
        return;
    }

    err = I2C_HAL_Write(s_i2c_handle_pa, &addr, &data, 1);
    AUDDRV_DEBUG("aud pa write reg 0x%02x,0x%02x,%d", addr, data, err);

//    SCI_SLEEP(1);
}



LOCAL void __aw87390_pa_init()
{
    _i2c_writeOneReg(0x67, 0x03);
    _i2c_writeOneReg(0x02, 0x07);
    _i2c_writeOneReg(0x02, 0x00);
    _i2c_writeOneReg(0x02, 0x00); //rcv
    //_i2c_writeOneReg(0x02, 0x0c); //modectrl:EN_2X,EN_SPK,classd
    //_i2c_writeOneReg(0x02, 0x0e); //modectrl:EN_2X,EN_SPK,EN_LG,classd
    _i2c_writeOneReg(0x03, 0x08); //CPOVP:8v
    _i2c_writeOneReg(0x04, 0x05);
    _i2c_writeOneReg(0x05, pa_gain); //GAIN:9db
    _i2c_writeOneReg(0x06, 0x09); //AGC3_PRO
    _i2c_writeOneReg(0x07, 0x4e); //AGC3
    _i2c_writeOneReg(0x08, 0x08); //AGC2_PRO
    _i2c_writeOneReg(0x09, 0x08); //AGC2_PRO
    _i2c_writeOneReg(0x0a, 0x3a); //AGC1
    _i2c_writeOneReg(0x61, 0xb3);
    _i2c_writeOneReg(0x62, 0x24);
    _i2c_writeOneReg(0x63, 0x05); //ADPVTH
    _i2c_writeOneReg(0x64, 0x48);
    _i2c_writeOneReg(0x65, 0x17);
    _i2c_writeOneReg(0x79, 0x7a);
    _i2c_writeOneReg(0x7a, 0x6c);
    _i2c_writeOneReg(0x78, 0x80);
    _i2c_writeOneReg(0x66, 0x38);
    _i2c_writeOneReg(0x76, 0x00);
    _i2c_writeOneReg(0x78, 0x00);
    _i2c_writeOneReg(0x68, 0x1b);
    _i2c_writeOneReg(0x69, 0x5b);
    _i2c_writeOneReg(0x70, 0x1d);
    _i2c_writeOneReg(0x71, 0x10);
    _i2c_writeOneReg(0x72, 0xb4);
    _i2c_writeOneReg(0x73, 0x4f);
    _i2c_writeOneReg(0x74, 0x24);
    _i2c_writeOneReg(0x75, 0x02);
    _i2c_writeOneReg(0x01, 0x07); //sysctrl:EN_PA,EN_CP,EN_CCPA
    _i2c_writeOneReg(0xff, 0x00);
}

LOCAL void __aw87390_pa_close()
{
    _i2c_writeOneReg(0x01, 0x00); //sysctrl:EN_PA,EN_CP,EN_CCPA
    /*_i2c_writeOneReg(0x02, 0x00); //modectrl:EN_2X,EN_SPK,classd
    _i2c_writeOneReg(0x03, 0x00); //CPOVP:8v
    _i2c_writeOneReg(0x05, 0x00); //GAIN:18db
    _i2c_writeOneReg(0x06, 0x00); //AGC3_PRO
    _i2c_writeOneReg(0x07, 0x4e); //AGC3
    _i2c_writeOneReg(0x08, 0x00); //AGC2_PRO
    _i2c_writeOneReg(0x09, 0x00); //AGC2
    _i2c_writeOneReg(0x0a, 0x3a); //AGC1
    _i2c_writeOneReg(0x63, 0x05); //ADPVTH*/
}


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
PUBLIC int32 __extpa_enable(uint8 oc)
{
    int32 ret;

    AUDDRV_DEBUG("open %d", oc);

    if (s_i2c_handle_pa < 0) {
        ret = __aw87390_i2c_init();
        if (AUDIO_HAL_SUCCESS != ret) {
            AUDDRV_ERROR("i2c init failed");
            return AUDIO_HAL_ERROR;
        }
    }

    if (oc) {
        __aw87390_pa_init();
    } else {
        __aw87390_pa_close();
    }
    return AUDIO_HAL_SUCCESS;
}

PUBLIC int32 __extpa_gain_set(uint32 gain)
{
    int32 ret;

    AUDDRV_DEBUG("set 0x%x", gain);

    if (s_i2c_handle_pa < 0) {
        ret = __aw87390_i2c_init();
        if (AUDIO_HAL_SUCCESS != ret) {
            AUDDRV_ERROR("i2c init failed");
            return AUDIO_HAL_ERROR;
        }
    }

    pa_gain = gain;
    _i2c_writeOneReg(0x05, gain); //GAIN:9db
    return AUDIO_HAL_SUCCESS;
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/

// End
