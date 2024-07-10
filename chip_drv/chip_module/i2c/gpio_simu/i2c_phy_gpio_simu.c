/******************************************************************************
 ** File Name:      I2C_phy_gpio_simu.c                                                 *
 ** Author:         liuhao                                                   *
 ** DATE:           06/28/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the physical layer of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/28/2010     liuhao     Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "os_api.h"
#include "chip_plf_export.h"
#include "gpio_drvapi.h"
#include "gpio_prod_cfg.h"
#include "gpio_prod_api.h"
/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/
#ifdef ZDT_NFC_SUPPORT
#define I2C_DELAY_UNIT   2
#else
#define I2C_DELAY_UNIT   8
#endif

/**---------------------------------------------------------------------------*
 **                            Local Variables
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                            Global Variables
 **---------------------------------------------------------------------------*/
extern const I2C_GPIO_INFO __i2c_gpio_info[I2C_BUS_MAX];

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/
/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL uint32  __I2C_PHY_GetGPIO (uint32 phy_id, uint32 *sda, uint32 *scl)
{
    int32 i = 0;
    uint32 ret = 1;
    GPIO_CFG_INFO_T_PTR phy_sda_pin_ptr = 0;
    GPIO_CFG_INFO_T_PTR phy_scl_pin_ptr = 0;
    
    for (i = 0; i < I2C_BUS_MAX; i++)
    {
        phy_sda_pin_ptr = GPIO_PROD_GetCfgInfo (__i2c_gpio_info[i].sda_pin);
        phy_scl_pin_ptr = GPIO_PROD_GetCfgInfo (__i2c_gpio_info[i].scl_pin);
        if ((phy_id == (uint32) __i2c_gpio_info[i].phy_id) &&
            phy_sda_pin_ptr && phy_scl_pin_ptr)
        {
            *sda = phy_sda_pin_ptr->gpio_num;
            *scl = phy_scl_pin_ptr->gpio_num;
            ret = 0;
            break;
        }
    }

    SCI_PASSERT ( (0 == ret), ("get I2C controller gpio pins fail!"));/*assert verified*/
    return ret;
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL __inline void __I2C_SET_SDA (uint32 gpio_id, uint32 value)
{
    GPIO_SetDirection (gpio_id, 1);
    GPIO_SetValue (gpio_id, value);
    return;
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL __inline void  __I2C_SET_SCL (uint32 gpio_id, uint32 value)
{
    GPIO_SetValue (gpio_id, value);
    return;
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL __inline uint32 __I2C_GET_SDA (uint32  gpio_id)
{
    GPIO_SetDirection (gpio_id, 0);
    return GPIO_GetValue (gpio_id);
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL void __i2c_delay (uint32 us)
{
    while (us--)
    {
        ;
    }
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL void __I2C_RESTART_BIT (uint32 sda, uint32 scl)
{
    //SCI_TRACE_LOW ("[IIC DRV:]__I2C_RESTART_BIT");

    /*start condition*/
    __I2C_SET_SDA (sda, 1);
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SCL (scl, 1);
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SDA (sda, 0);
    __i2c_delay (I2C_DELAY_UNIT);
    //__I2C_SET_SCL (scl, 0);
    //__i2c_delay (I2C_DELAY_UNIT);
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL void __I2C_START_BIT (uint32 sda, uint32 scl)
{
    /*scl:1, sda:1*/
    __i2c_delay (I2C_DELAY_UNIT);
    /*start condition*/
    __I2C_SET_SDA (sda, 0);
    __i2c_delay (I2C_DELAY_UNIT);
    /*scl:1, sda:0*/
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL void __I2C_STOP_BIT (uint32 sda, uint32 scl)
{
    /*scl:0, sda:1*/
    /*stop condition*/
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SDA (sda, 0);
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SCL (scl, 1);
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SDA (sda, 1);
    __i2c_delay (I2C_DELAY_UNIT);
    /*scl:1, sda:1*/
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL void __I2C_SEND_BYTE (uint8 *data, uint32 sda, uint32 scl)
{
    uint32 i;

    /*from start: scl:1, sda:0*/
    /*repeat write:scl:0, sda:1*/
    for (i=0; i<8; i++)
    {
        __i2c_delay (I2C_DELAY_UNIT);
        /*prepare date, from MSB to LSB*/
        __I2C_SET_SCL (scl, 0);
        __i2c_delay (I2C_DELAY_UNIT);

        if ( (*data) & (1<< (7-i)))
        {
            __I2C_SET_SDA (sda, 1);
        }
        else
        {
            __I2C_SET_SDA (sda, 0);
        }

        /*sample data*/
        __i2c_delay (I2C_DELAY_UNIT);
        __I2C_SET_SCL (scl, 1);
        __i2c_delay (I2C_DELAY_UNIT);
        __I2C_SET_SCL (scl, 0);
    }

    /*release sda line*/
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SDA (sda, 1);
    __i2c_delay (I2C_DELAY_UNIT);
    /*scl:0, sda:1*/
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL uint8 __I2C_RECEIVE_BYTE (uint32 sda, uint32 scl)
{
    /*from write:scl:0, sda:1*/
    /*from send ack:scl:0, sda:0*/
    uint8 val = 0;
    uint32 i;
    /*release sda line
    __I2C_SET_SDA (sda, 1);*/

    for (i=0; i<8; i++)
    {
        /*i2c slave device will send a bit, from MSB to LSB*/
        __i2c_delay (I2C_DELAY_UNIT);
        __I2C_SET_SCL (scl, 0);
        __i2c_delay (I2C_DELAY_UNIT);
        __I2C_SET_SCL (scl, 1);
        /*this should be do early?*/
        GPIO_SetDirection (sda, 0);
        __i2c_delay (I2C_DELAY_UNIT);

        if (__I2C_GET_SDA (sda))
        {
            val |= (1<< (7-i));
        }

        __i2c_delay (I2C_DELAY_UNIT);
        __I2C_SET_SCL (scl, 0);
    }

    /*scl:0, sda:input*/
    return val;
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL uint32 __I2C_RECEIVE_ACK (uint32 sda, uint32 scl)
{
    uint32 nack;
    /*from write: scl:0, sda:1*/
    __i2c_delay (I2C_DELAY_UNIT);
    GPIO_SetDirection (sda, 0);
    /*get ack or nack*/
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SCL (scl, 0);
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SCL (scl, 1);
    __i2c_delay (I2C_DELAY_UNIT);
    nack = __I2C_GET_SDA (sda);
    __I2C_SET_SCL (scl, 0);
    __i2c_delay (I2C_DELAY_UNIT);
    /*release sda line does we need this?
    GPIO_SetDirection (sda, 1);
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SCL (scl, 0);
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SDA (sda, 1);
    __i2c_delay (I2C_DELAY_UNIT);*/
    /*return 1: ack*/
    return (nack == 0) ?1:0;
    /*scl:0, sda:1*/
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL void __I2C_SEND_ACK (uint32 sda, uint32 scl)
{
    /*scl:0, sda:input*/
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SCL (scl, 0);
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SDA (sda, 0);
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SCL (scl, 1);
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SCL (scl, 0);
    GPIO_SetDirection (sda, 0);
    __i2c_delay (I2C_DELAY_UNIT);
    /*scl:0, sda:0*/
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL void __I2C_SEND_NACK (uint32 sda, uint32 scl)
{
    /*scl:0, sda:input*/
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SCL (scl, 0);
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SDA (sda, 1);
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SCL (scl, 1);
    __i2c_delay (I2C_DELAY_UNIT);
    __I2C_SET_SCL (scl, 0);
    __i2c_delay (I2C_DELAY_UNIT);
    /*scl:0, sda:1*/
}
/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_ControlInit_V1 (uint32 phy_id, uint32 freq, uint32 port)
{
    uint32 sda = 0;
    uint32 scl = 0;
    __I2C_PHY_GetGPIO (phy_id, &sda, &scl);
    GPIO_Enable (sda);
    GPIO_Enable (scl);
    GPIO_SetDirection (scl, 1);
    /*at begaining, scl and sda line high*/
    __I2C_SET_SDA (sda, 1);
    __I2C_SET_SCL (scl, 1);
    //SCI_TRACE_LOW ("[IIC DRV:]I2C_PHY_ControlInit_V1: sda=%d, scl=%d", sda, scl);
    return ERR_I2C_NONE;
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_StartBus_V1 (uint32 phy_id, uint8 addr, BOOLEAN rw, BOOLEAN ack_en)
{
    uint32 sda = 0;
    uint32 scl = 0;
    uint8 cmd;
    __I2C_PHY_GetGPIO (phy_id, &sda, &scl);

    if (rw)
    {
        /*read cmd*/
        cmd = addr |0x1;
    }
    else
    {
        /*write cmd*/
        cmd = addr;
    }

    __I2C_START_BIT (sda, scl);
    __I2C_SEND_BYTE (&cmd, sda, scl);
    //SCI_TRACE_LOW ("[IIC DRV:]I2C_PHY_StartBus_V1: cmd=%x", cmd);

    //check ACK
    if (ack_en)
    {
        __I2C_RECEIVE_ACK (sda, scl);
    }

    return ERR_I2C_NONE;
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_WriteBytes_V1 (uint32 phy_id, uint8 *pCmd, uint32 len, BOOLEAN ack_en, BOOLEAN no_stop)
{
    uint32 sda = 0;
    uint32 scl = 0;
    uint32 i = 0;
    uint8 cmd = 0;
    __I2C_PHY_GetGPIO (phy_id, &sda, &scl);

    for (i=0; i<len; i++)
    {
        cmd = pCmd[i];
        __I2C_SEND_BYTE (&cmd, sda, scl);
        //SCI_TRACE_LOW ("[IIC DRV:]I2C_PHY_WriteBytes_V1: cmd=%x", cmd);

        //check ACK
        if (ack_en)
        {
            __I2C_RECEIVE_ACK (sda, scl);
        }

        if (i== (len-1))
        {
            if (1 == no_stop)
            {
                __I2C_RESTART_BIT (sda, scl);
            }
            else if (0 == no_stop)
            {
                __I2C_STOP_BIT (sda, scl);
            }
        }
    }

    return ERR_I2C_NONE;
}


/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_ReadBytes_V1 (uint32 phy_id, uint8 *pCmd, uint32 len, BOOLEAN ack_en)
{
    uint32 sda = 0;
    uint32 scl = 0;
    uint32 i = 0;
    __I2C_PHY_GetGPIO (phy_id, &sda, &scl);

    for (i=0; i<len; i++)
    {
        pCmd[i] = __I2C_RECEIVE_BYTE (sda, scl);
        //SCI_TRACE_LOW ("[IIC DRV:]I2C_PHY_ReadBytes_V1: cmd=%x", pCmd[i]);

        if (i== (len-1))
        {
            __I2C_SEND_NACK (sda, scl);
            __I2C_STOP_BIT (sda, scl);
        }
        else
        {
            __I2C_SEND_ACK (sda, scl);
        }
    }

    return ERR_I2C_NONE;
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_StopBus_V1 (uint32 phy_id)
{
    uint32 sda = 0;
    uint32 scl = 0;
    __I2C_PHY_GetGPIO (phy_id, &sda, &scl);
    __I2C_STOP_BIT (sda, scl);
    return ERR_I2C_NONE;
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_SendACK_V1 (uint32 phy_id)
{
    uint32 sda = 0;
    uint32 scl = 0;
    __I2C_PHY_GetGPIO (phy_id, &sda, &scl);
    __I2C_SEND_ACK (sda, scl);
    return ERR_I2C_NONE;
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_GetACK_V1 (uint32 phy_id)
{
    uint32 sda = 0;
    uint32 scl = 0;
    __I2C_PHY_GetGPIO (phy_id, &sda, &scl);
    __I2C_RECEIVE_ACK (sda, scl);
    return ERR_I2C_NONE;
}

#ifdef ZDT_NFC_SUPPORT
PUBLIC uint32 I2C_PHY_ReadBytes_NFC_V1(uint32 phy_id, uint8 addr,uint8 *pCmd, uint32 max_len)
{
    uint32 sda;
    uint32 scl;
    uint8 cmd;
    uint32 len = 0;
    uint32 i = 0;
    __I2C_PHY_GetGPIO (phy_id, &sda, &scl);

    cmd = addr |0x1;
    __I2C_START_BIT (sda, scl);
    __I2C_SEND_BYTE (&cmd, sda, scl);
    __I2C_RECEIVE_ACK (sda, scl);

    for (i=0; i<3; i++)
    {
        pCmd[i] = __I2C_RECEIVE_BYTE (sda, scl);
        __I2C_SEND_ACK (sda, scl);
    }
    if(GPIO_GetNFCDWL()) {  //boot mode
        len = max_len - 3;
    } else {
        len = pCmd[2];
    }

    
    for (i=0; i<len; i++)
    {
        if(!GPIO_GetNFCIRQ()) {
            break;
        }
        pCmd[i+3] = __I2C_RECEIVE_BYTE (sda, scl);

        if (i== (len-1))
        {
            __I2C_SEND_NACK (sda, scl);
        }
        else
        {
            __I2C_SEND_ACK (sda, scl);
        }
    }
    __I2C_STOP_BIT (sda, scl);
    return (len+3);
}
#endif

/*this version armcc can not support this method :(*/
/*
PUBLIC I2C_PHY_FUN phy_fun_v0 = {
        .init = I2C_PHY_ControlInit_V1,
        .start = I2C_PHY_StartBus_V1,
        .stop = I2C_PHY_StopBus_V1,
        .read = I2C_PHY_ReadBytes_V1,
        .write = I2C_PHY_WriteBytes_V1,
        .sendack = I2C_PHY_SendACK_V1,
        .getack = I2C_PHY_GetACK_V1,
};*/

PUBLIC I2C_PHY_FUN phy_fun_v1 =
{
    I2C_PHY_ControlInit_V1,
    I2C_PHY_StartBus_V1,
    I2C_PHY_WriteBytes_V1,
    I2C_PHY_ReadBytes_V1,
    I2C_PHY_StopBus_V1,
    I2C_PHY_SendACK_V1,
    I2C_PHY_GetACK_V1
};
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/


#ifdef   __cplusplus
}
#endif

/*  End Of File */
