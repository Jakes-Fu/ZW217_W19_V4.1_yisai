/******************************************************************************
 ** File Name:      I2C_phy_v0.c                                                 *
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
#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "timer_drvapi.h"
#include "i2c_reg_v0.h"
#if defined(PLATFORM_SC6600L) || defined(PLATFORM_SC6800H)
#include "pinmap_drvapi.h"
#endif //defined(PLATFORM_SC6600L) || defined(PLATFORM_SC6800H)

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

#define I2C_TIMEOUT_FACTOR  500000  //the critical value is 10000

#define I2C_WAIT_INT                                                  \
    {                                                                     \
        timetick = TIMER_GetSystemCounterReg();                                  \
        while(!(ptr->ctl & I2CCTL_INT))                                   \
        {                                                                 \
            uint32 wait_counter=0x00;   \
            for(wait_counter=0x00; wait_counter<0xff;wait_counter++)    \
            {                                                   \
                wait_counter++;                         \
            }                                                    \
            if((TIMER_GetSystemCounterReg() - timetick) >= g_i2c_timeout)        \
            {                                                             \
                if(ERR_I2C_NONE == ret_value)                             \
                {                                                         \
                    ret_value = ERR_I2C_INT_TIMEOUT;                      \
                }                                                         \
                break;                                                    \
            }                                                             \
            \
        }                                                                 \
        \
    }

#define I2C_WAIT_BUSY                                                 \
    {                                                                     \
        timetick = TIMER_GetSystemCounterReg();                                  \
        while(ptr->cmd & I2CCMD_BUS)                                      \
        {                                                                 \
            if ((TIMER_GetSystemCounterReg() - timetick) >= g_i2c_timeout)       \
            {                                                             \
                if(ERR_I2C_NONE == ret_value)                             \
                {                                                         \
                    ret_value = ERR_I2C_BUSY_TIMEOUT;                     \
                }                                                         \
                break;                                                    \
            }                                                             \
        }                                                                 \
        \
    }

#define I2C_WAIT_ACK                                                  \
    {                                                                     \
        timetick = TIMER_GetSystemCounterReg();                                  \
        while(ptr->cmd & I2CCMD_ACK)                                      \
        {                                                                 \
            if ((TIMER_GetSystemCounterReg() - timetick) >= g_i2c_timeout)       \
            {                                                             \
                if(ERR_I2C_NONE == ret_value)                             \
                {                                                         \
                    ret_value = ERR_I2C_ACK_TIMEOUT;                      \
                }                                                         \
                break;                                                    \
            }                                                             \
        }                                                                 \
    }


#define I2C_CLEAR_INT                                                 \
    {                                                                     \
        I2C_WAIT_BUSY                                                     \
        ptr->cmd |= I2CCMD_INT_ACK; \
    }

/**---------------------------------------------------------------------------*
 **                            Local Variables
 **---------------------------------------------------------------------------*/

#if defined (PLATFORM_SC6800H)
const PM_PINFUNC_T i2c_port1_en[]=
{
    { PIN_SCL_REG1, PIN_FUNC_DEF },
    { PIN_SDA_REG1, PIN_FUNC_DEF },
    {0xffffffff, 0xffffffff}
};
const PM_PINFUNC_T i2c_port1_dis[]=
{
    { PIN_SCL_REG1, PIN_FUNC_3 },
    { PIN_SDA_REG1, PIN_FUNC_3 },
    {0xffffffff, 0xffffffff}
};

const PM_PINFUNC_T i2c_port2_en[]=
{
    { PIN_SCL_REG2, PIN_FUNC_1 },
    { PIN_SDA_REG2, PIN_FUNC_1 },
    {0xffffffff, 0xffffffff}
};
const PM_PINFUNC_T i2c_port2_dis[]=
{
    { PIN_SCL_REG2, PIN_FUNC_3 },
    { PIN_SDA_REG2, PIN_FUNC_3 },
    {0xffffffff, 0xffffffff}
};
const PM_PINFUNC_T i2c1_port1_en[]=
{
    { PIN_SCL_REG3, PIN_FUNC_1 },
    { PIN_SDA_REG3, PIN_FUNC_1 },
    {0xffffffff, 0xffffffff}
};
const PM_PINFUNC_T i2c1_port1_dis[]=
{
    { PIN_SCL_REG3, PIN_FUNC_DEF },
    { PIN_SDA_REG3, PIN_FUNC_DEF },
    {0xffffffff, 0xffffffff}
};

const PM_PINFUNC_T i2c1_port2_en[]=
{
    { PIN_SCL_REG4, PIN_FUNC_1 },
    { PIN_SDA_REG4, PIN_FUNC_1 },
    {0xffffffff, 0xffffffff}
};
const PM_PINFUNC_T i2c1_port2_dis[]=
{
    { PIN_SCL_REG4, PIN_FUNC_DEF },
    { PIN_SDA_REG4, PIN_FUNC_DEF },
    {0xffffffff, 0xffffffff}
};
#endif

/**---------------------------------------------------------------------------*
 **                            Global Variables
 **---------------------------------------------------------------------------*/
PUBLIC volatile uint32     g_i2c_timeout = 2; //unit is ms
extern const I2C_BASE_INFO __i2c_base_info[I2C_BUS_MAX];

/*********************************************************************************************************
** Function name:__I2C_PHY_GetBase
** Descriptions:this fun is used to get base address
*********************************************************************************************************/
LOCAL uint32  __I2C_PHY_GetBase (uint32 phy_id)
{
    int32 i;
    uint32 ret = 0;

    for (i = 0; i < I2C_BUS_MAX; i++)
    {
        if (phy_id == (uint32) __i2c_base_info[i].phy_id)
        {
            ret = (uint32) __i2c_base_info[i].base_addr;
            break;
        }
    }

    SCI_PASSERT ( (0 != ret), ("get I2C controller base address fail!"));/*assert verified*/
    return ret;
}
/*********************************************************************************************************
** Function name:__I2C_PHY_SetSCL
** Descriptions:this fun is used to set freq
*********************************************************************************************************/
LOCAL ERR_I2C_E __I2C_PHY_SetSCL (uint32 phy_id, uint32 freq)
{
    uint32 APB_clk,i2c_dvd;
    volatile I2C_CTL_REG_T *ptr = (volatile I2C_CTL_REG_T *) __I2C_PHY_GetBase (phy_id);
    APB_clk= CHIP_GetAPBClk();
    i2c_dvd=APB_clk/ (4*freq)-1;
    ptr->div0= (uint16) (i2c_dvd & 0xffff);
    ptr->div1= (uint16) (i2c_dvd>>16);
    g_i2c_timeout = I2C_TIMEOUT_FACTOR / (freq);

    if (g_i2c_timeout < 2)
    {
        g_i2c_timeout = 2;
    }

    return ERR_I2C_NONE;
}

/*********************************************************************************************************
** Function name:__I2C_PHY_SetPort
** Descriptions:this fun is used to set pad group
*********************************************************************************************************/
LOCAL ERR_I2C_E __I2C_PHY_SetPort (uint32 phy_id, uint32 port)
{
    /*FIXME this should be replaced by pin map module api*/
#if defined (PLATFORM_SC6800H)
    uint32 i = 0;
    uint32 port1_val;
    uint32 port1_fun;
    uint32 port2_val;
    uint32 port2_fun;

    if(0 == phy_id)
    {
        /*read current functions*/
        port1_fun = (REG32 (i2c_port1_en[0].addr) & 0x30);
        IIC_PRINT (("[IIC DRV:]__I2C_PHY_SetPort:  i2c0, port1_fun=%x", port1_fun));
        port2_fun = (REG32 (i2c_port2_en[0].addr) & 0x30);
        IIC_PRINT (("[IIC DRV:]__I2C_PHY_SetPort: i2c0, port2_fun=%x", port2_fun));

        if (1==port)
        {
            if (i2c_port1_en[0].value != port1_fun)
            {
                /*switch port0 to i2c*/
                for(;;)
                {
                    // Check if search to end
                    if (PM_INVALID_VAL == i2c_port1_en[i].addr)
                    {
                        break;
                    }

                    port1_val = REG32 (i2c_port1_en[i].addr);
                    port1_val &= (~0x30);
                    port1_val |= i2c_port1_en[i].value;
                    IIC_PRINT (("[IIC DRV:]__I2C_PHY_SetPort1: port1_val value=%x",port1_val));
                    REG32 (i2c_port1_en[i].addr) = port1_val;
                    IIC_PRINT (("[IIC DRV:]__I2C_PHY_SetPort1: addr=%x, i2c_port1_en value=%x", i2c_port1_en[i].addr,REG32 (i2c_port1_en[i].addr)));
                    i++;
                }
            }

            i = 0;

            /*switch other port, not i2c*/
            if (i2c_port2_en[0].value == port2_fun)
            {
                for(;;)
                {
                    if (PM_INVALID_VAL == i2c_port2_dis[i].addr)
                    {
                        break;
                    }

                    port2_val = REG32 (i2c_port2_dis[i].addr);
                    port2_val &= (~0x30);
                    port2_val |= i2c_port2_dis[i].value;
                    IIC_PRINT (("[IIC DRV:]__I2C_PHY_SetPort1: port2_val value=%x",port2_val));
                    // Write the value to chip
                    REG32 (i2c_port2_dis[i].addr) = port2_val;
                    i++;
                }
            }
        }
        else if (2 == port)
        {
            if (PIN_FUNC_DEF == port2_fun)
            {
                //SCI_TRACE_LOW:"[IIC DRV:]__I2C_PHY_SetPort: Fatal error, if you use qwerty keypad, i2c fun of multiplex pins are disabled"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,I2C_PHY_V0_293_112_2_17_23_4_51_1032,(uint8*)"");
                return ERR_I2C_DEVICE_NOT_FOUND;
            }
                    
            if (i2c_port2_en[0].value != port2_fun)
            {
                /*switch port1 to i2c*/
                for(;;)
                {
                    // Check if search to end
                    if (PM_INVALID_VAL == i2c_port2_en[i].addr)
                    {
                        break;
                    }

                    port2_val = REG32 (i2c_port2_en[i].addr);
                    port2_val &= (~0x30);
                    port2_val |= i2c_port2_en[i].value;
                    IIC_PRINT (("[IIC DRV:]__I2C_PHY_SetPort2: port2_val value=%x",port2_val));
                    REG32 (i2c_port2_en[i].addr) = port2_val;
                    IIC_PRINT (("[IIC DRV:]__I2C_PHY_SetPort2: addr=%x, i2c_port2_en value=%x", i2c_port2_en[i].addr,REG32 (i2c_port2_en[i].addr)));
                    i++;
                }
            }

            i = 0;

            /*switch other port, not i2c*/
            if (i2c_port1_en[0].value == port1_fun)
            {
                for(;;)
                {
                    if (PM_INVALID_VAL == i2c_port1_dis[i].addr)
                    {
                        break;
                    }

                    port1_val = REG32 (i2c_port1_dis[i].addr);
                    port1_val &= (~0x30);
                    port1_val |= i2c_port1_dis[i].value;
                    IIC_PRINT (("[IIC DRV:]__I2C_PHY_SetPort2: port1_val value=%x",port1_val));
                    // Write the value to chip
                    REG32 (i2c_port1_dis[i].addr) = port1_val;
                    i++;
                }
            }
        }
        else
        {
            SCI_ASSERT (0);/*assert verified*/
        }
    }
    else if(1 == phy_id)
    {
        SCI_ASSERT(SC6800H != CHIP_GetChipType());/*assert verified*///for sc6800h1 chip, only one i2c controller
        
        /*read current functions*/
        port1_fun = (REG32 (i2c1_port1_en[0].addr) & 0x30);
        IIC_PRINT (("[IIC DRV:]__I2C_PHY_SetPort: i2c1, port1_fun=%x", port1_fun));
        port2_fun = (REG32 (i2c1_port2_en[0].addr) & 0x30);
        IIC_PRINT (("[IIC DRV:]__I2C_PHY_SetPort: i2c1, port2_fun=%x", port2_fun));

        if (1==port)
        {
            if (i2c1_port1_en[0].value != port1_fun)
            {
                /*switch port0 to i2c*/
                for(;;)
                {
                    // Check if search to end
                    if (PM_INVALID_VAL == i2c1_port1_en[i].addr)
                    {
                        break;
                    }

                    port1_val = REG32 (i2c1_port1_en[i].addr);
                    port1_val &= (~0x30);
                    port1_val |= i2c1_port1_en[i].value;
                    IIC_PRINT (("[IIC DRV:]__I2C_PHY_Setport1: port1_val value=%x",port1_val));
                    REG32 (i2c1_port1_en[i].addr) = port1_val;
                    IIC_PRINT (("[IIC DRV:]__I2C_PHY_Setport1: addr=%x, i2c1_port1_en value=%x", i2c1_port1_en[i].addr,REG32 (i2c1_port1_en[i].addr)));
                    i++;
                }
            }

            i = 0;

            /*switch other port, not i2c*/
            if (i2c1_port2_en[0].value == port2_fun)
            {
                for(;;)
                {
                    if (PM_INVALID_VAL == i2c1_port2_dis[i].addr)
                    {
                        break;
                    }

                    port2_val = REG32 (i2c1_port2_dis[i].addr);
                    port2_val &= (~0x30);
                    port2_val |= i2c1_port2_dis[i].value;
                    IIC_PRINT (("[IIC DRV:]__I2C_PHY_Setport1: port2_val value=%x",port2_val));
                    // Write the value to chip
                    REG32 (i2c1_port2_dis[i].addr) = port2_val;
                    i++;
                }
            }
        }
        else if (2 == port)
        {
            if (i2c1_port2_en[0].value != port2_fun)
            {
                /*switch port3 to i2c*/
                for(;;)
                {
                    // Check if search to end
                    if (PM_INVALID_VAL == i2c1_port2_en[i].addr)
                    {
                        break;
                    }

                    port2_val = REG32 (i2c1_port2_en[i].addr);
                    port2_val &= (~0x30);
                    port2_val |= i2c1_port2_en[i].value;
                    IIC_PRINT (("[IIC DRV:]__I2C_PHY_Setport2: port2_val value=%x",port2_val));
                    REG32 (i2c1_port2_en[i].addr) = port2_val;
                    IIC_PRINT (("[IIC DRV:]__I2C_PHY_Setport2: addr=%x, i2c1_port2_en value=%x", i2c1_port2_en[i].addr,REG32 (i2c1_port2_en[i].addr)));
                    i++;
                }
            }

            i = 0;

            /*switch other port, not i2c*/
            if (i2c1_port1_en[0].value == port1_fun)
            {
                for(;;)
                {
                    if (PM_INVALID_VAL == i2c1_port1_dis[i].addr)
                    {
                        break;
                    }

                    port1_val = REG32 (i2c1_port1_dis[i].addr);
                    port1_val &= (~0x30);
                    port1_val |= i2c1_port1_dis[i].value;
                    IIC_PRINT (("[IIC DRV:]__I2C_PHY_Setport2: port1_val value=%x",port1_val));
                    // Write the value to chip
                    REG32 (i2c1_port1_dis[i].addr) = port1_val;
                    i++;
                }
            }
        }
        else
        {
            SCI_ASSERT (0);/*assert verified*/
        }
    }

#endif

#if defined (PLATFORM_SC8800G)
    {
        uint32 port_sel;

        /*select port*/
        port_sel = REG32 (GR_PIN_CTL);
        port_sel &= (~ (BIT_3 | BIT_4));

        switch (port)
        {
            case 1:
                port_sel |= (2<<3);
                break;
            case 2:
                port_sel |= (1<<3);
                break;
            case 3:
                port_sel |= (0<<3);
                break;
            case 4:
                port_sel |= (3<<3);
                break;
            default:
                break;
        }

        REG32 (GR_PIN_CTL) = port_sel;
    }
#endif

    return ERR_I2C_NONE;
}

/*********************************************************************************************************
** Function name:I2C_PHY_ControlInit_V0
** Descriptions:this fun is used to init controler
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_ControlInit_V0 (uint32 phy_id, uint32 freq, uint32 port)
{
    volatile I2C_CTL_REG_T *ptr = (volatile I2C_CTL_REG_T *) __I2C_PHY_GetBase (phy_id);

    ptr->rst = BIT_0;//why reset
    ptr->ctl &= ~ (I2CCTL_EN); //you must first disable i2c module then change clock
    ptr->ctl &= ~ (I2CCTL_IE);
    ptr->ctl &= ~ (I2CCTL_CMDBUF_EN);
    __I2C_PHY_SetSCL (phy_id, freq);

    if (I2C_PORT_NUM < port)
    {
        SCI_ASSERT (0);/*assert to do*/
    }

    __I2C_PHY_SetPort (phy_id, port);

    ptr->ctl |=  (I2CCTL_IE | I2CCTL_EN);
    //Clear I2C int
    ptr->cmd |=  (I2CCMD_INT_ACK);
    IIC_PRINT ("[IIC DRV:]I2C_PHY_ControlInit_V0: freq=%d, port=%d", freq, port);
    return ERR_I2C_NONE;
}

/*********************************************************************************************************
** Function name:I2C_PHY_StartBus_V0
** Descriptions:this fun is used to send start cmd
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_StartBus_V0 (uint32 phy_id, uint8 addr, BOOLEAN rw, BOOLEAN ack_en)
{
    uint32 timetick = 0;
    uint32 cmd = 0;
    volatile I2C_CTL_REG_T *ptr = (volatile I2C_CTL_REG_T *) __I2C_PHY_GetBase (phy_id);
    uint32   ret_value = ERR_I2C_NONE;

    if (rw)
    {
        /*read cmd*/
        cmd = ( (uint32) (addr |0x1)) <<8;
    }
    else
    {
        /*write cmd*/
        cmd = ( (uint32) addr) <<8;
    }

    cmd = cmd | I2CCMD_START | I2CCMD_WRITE;
    IIC_PRINT (("[IIC DRV:]I2C_PHY_StartBus_V0: cmd=%x", cmd));
    ptr->cmd = cmd;
    I2C_WAIT_INT
    I2C_CLEAR_INT

    //check ACK
    if (ack_en)
    {
        I2C_WAIT_ACK
    }

    return ERR_I2C_NONE;
}

/*********************************************************************************************************
** Function name:I2C_PHY_WriteBytes_V0
** Descriptions:this fun is used to send write cmd
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_WriteBytes_V0 (uint32 phy_id, uint8 *pCmd, uint32 len, BOOLEAN ack_en, BOOLEAN no_stop)
{
    uint32 timetick = 0;
    uint32 i = 0;
    uint32 cmd = 0;
    volatile I2C_CTL_REG_T *ptr = (volatile I2C_CTL_REG_T *) __I2C_PHY_GetBase (phy_id);
    uint32   ret_value = ERR_I2C_NONE;

    for (i=0; i<len; i++)
    {
        cmd = ( (uint32) pCmd[i]) <<8;
        cmd = cmd | I2CCMD_WRITE ;

        if ( (i== (len-1)) && (!no_stop))
        {
            cmd = cmd | I2CCMD_STOP;
        }

        ptr->cmd = cmd;
        IIC_PRINT (("[IIC DRV:]I2C_PHY_WriteBytes_V0: cmd=%x", cmd));
        I2C_WAIT_INT
        I2C_CLEAR_INT

        //check ACK
        if (ack_en)
        {
            I2C_WAIT_ACK
        }
    }

    return ret_value;
}


/*********************************************************************************************************
** Function name:I2C_PHY_ReadBytes_V0
** Descriptions:this fun is used to send read cmd
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_ReadBytes_V0 (uint32 phy_id, uint8 *pCmd, uint32 len, BOOLEAN ack_en)
{
    uint32 timetick = 0;
    uint32 i = 0;
    uint32 cmd = 0;
    volatile I2C_CTL_REG_T *ptr = (volatile I2C_CTL_REG_T *) __I2C_PHY_GetBase (phy_id);
    uint32   ret_value = ERR_I2C_NONE;

    for (i=0; i<len; i++)
    {
        cmd = I2CCMD_READ; /*FIXME |I2CCMD_TX_ACK;*/

        if (i== (len-1))
        {
            cmd = cmd |I2CCMD_STOP |I2CCMD_TX_ACK;
        }

        ptr->cmd = cmd;
        IIC_PRINT (("[IIC DRV:]I2C_PHY_ReadBytes_V0: cmd=%x", cmd));
        I2C_WAIT_INT
        I2C_CLEAR_INT
        pCmd[i] = (uint8) ( (ptr->cmd) >>8);
    }

    return ret_value;
}

/*********************************************************************************************************
** Function name:I2C_PHY_StopBus_V0
** Descriptions:this fun is used to send stop cmd
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_StopBus_V0 (uint32 phy_id)
{
    uint32 timetick = 0;
    uint32 cmd = 0;
    volatile I2C_CTL_REG_T *ptr = (volatile I2C_CTL_REG_T *) __I2C_PHY_GetBase (phy_id);
    uint32   ret_value = ERR_I2C_NONE;
    cmd = I2CCMD_STOP;
    ptr->cmd = cmd;
    I2C_WAIT_INT
    I2C_CLEAR_INT
    return ERR_I2C_NONE;
}

/*********************************************************************************************************
** Function name:I2C_PHY_SendACK_V0
** Descriptions:this fun is used to send ack
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_SendACK_V0 (uint32 phy_id)
{
    return ERR_I2C_NONE;
}

/*********************************************************************************************************
** Function name:I2C_PHY_GetACK_V0
** Descriptions:this fun is used to get ack
*********************************************************************************************************/
LOCAL ERR_I2C_E I2C_PHY_GetACK_V0 (uint32 phy_id)
{
    return ERR_I2C_NONE;
}

/*this version armcc can not support this method :(*/
/*
PUBLIC I2C_PHY_FUN phy_fun_v0 = {
        .init = I2C_PHY_ControlInit_V0,
        .start = I2C_PHY_StartBus_V0,
        .stop = I2C_PHY_StopBus_V0,
        .read = I2C_PHY_ReadBytes_V0,
        .write = I2C_PHY_WriteBytes_V0,
        .sendack = I2C_PHY_SendACK_V0,
        .getack = I2C_PHY_GetACK_V0,
};*/

PUBLIC I2C_PHY_FUN phy_fun_v0 =
{
    I2C_PHY_ControlInit_V0,
    I2C_PHY_StartBus_V0,
    I2C_PHY_WriteBytes_V0,
    I2C_PHY_ReadBytes_V0,
    I2C_PHY_StopBus_V0,
    I2C_PHY_SendACK_V0,
    I2C_PHY_GetACK_V0
};
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/


#ifdef   __cplusplus
}
#endif

/*  End Of File */
