/******************************************************************************
 ** File Name:      I2C_phy_v5.c                                                 *
 ** Author:         liuhao                                                   *
 ** DATE:           06/28/2010                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
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
#include "timer_drvapi.h"
#include "i2c_reg_v5.h"
#include "pinmap_drvapi.h"

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
#define I2C_CMDBUF_SUPPORT 0
#define I2C_CMDBUF_SIZE 8
static volatile uint32 cmd_buffer_nr = 0;

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
    IIC_PRINT (("[IIC DRV:]__I2C_PHY_SelPort:  phy_id=%x, port=%x", phy_id, port));
#if defined(CHIP_VER_6531)
    if(0 == phy_id)
    {
        if (1==port)
        {
            //select i2c0 port1
            CHIP_REG_AND (APB_PIN_CTL0, ~I2C0_SEL);
         }
        else if (2 == port)
        {
            //select i2c0 port2
            CHIP_REG_OR (APB_PIN_CTL0, I2C0_SEL);
        }
        else
        {
            SCI_ASSERT(0); /*assert to do*/
        }
    }
    else if(1 == phy_id)
    {
        if (1==port)
        {
            //select i2c1 port1
            CHIP_REG_AND (APB_PIN_CTL0, ~I2C1_SEL);
         }
        else if (2 == port)
        {
            //select i2c1 port2
            CHIP_REG_OR (APB_PIN_CTL0, I2C1_SEL);
        }
        else
        {
            SCI_ASSERT(0); /*assert to do*/
        }
    }
    else if(2 == phy_id)
    {
        if (1==port)
        {
            //select i2c2 port1
            CHIP_REG_AND (APB_PIN_CTL0, ~I2C2_SEL);
         }
        else if (2 == port)
        {
            //select i2c2 port2
            CHIP_REG_OR (APB_PIN_CTL0, I2C2_SEL);
        }
        else
        {
            SCI_ASSERT(0); /*assert to do*/
        }
    }
#elif defined(CHIP_VER_6531EFM)
	if(0 == phy_id)
    {
        if (1==port)
        {
            //select i2c0 port1
            //CHIP_REG_AND (APB_PIN_CTL0, ~I2C0_SEL);  //not found this bit
        }
        else
        {
            SCI_ASSERT(0); /*assert to do*/
        }
    }
    else if(1 == phy_id)
    {
        if (1==port)
        {
            //select i2c1 port1
            CHIP_REG_AND (APB_PIN_CTL0, ~I2C1_SEL);
         }
        else if (2 == port)
        {
            //select i2c1 port2
            CHIP_REG_OR (APB_PIN_CTL0, I2C1_SEL);
        }
        else
        {
            SCI_ASSERT(0); /*assert to do*/
        }
    }
#endif
    else
    {
        SCI_ASSERT(0); /*assert to do*/
    }
	
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

#if I2C_CMDBUF_SUPPORT
    ptr->ctl |=  (I2CCTL_CMDBUF_EN);
#endif

    IIC_PRINT (("[IIC DRV:]I2C_PHY_ControlInit_V0: freq=%d, port=%d", freq, port));
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

#if (!I2C_CMDBUF_SUPPORT)
    cmd = I2CCMD_START;
    ptr->cmd = cmd;
    I2C_WAIT_INT
    I2C_CLEAR_INT
#endif

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

#if (I2C_CMDBUF_SUPPORT)
    cmd = cmd | I2CCMD_START | I2CCMD_WRITE;
#else
    cmd = cmd | I2CCMD_WRITE;
#endif

    IIC_PRINT (("[IIC DRV:]I2C_PHY_StartBus_V0: cmd=%x", cmd));

#if (!I2C_CMDBUF_SUPPORT)
    ptr->cmd = cmd;
    I2C_WAIT_INT
    I2C_CLEAR_INT

    //check ACK
    if (ack_en)
    {
        I2C_WAIT_ACK
    }
#else
    ptr->cmd_buf = cmd;
    cmd_buffer_nr++;
#endif

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
#if (I2C_CMDBUF_SUPPORT)	
    uint32 k = 0;
#endif
    uint32 cmd = 0;
    volatile I2C_CTL_REG_T *ptr = (volatile I2C_CTL_REG_T *) __I2C_PHY_GetBase (phy_id);
    uint32   ret_value = ERR_I2C_NONE;
    uint32 flag;

    flag = 0;
    for (i=0; i<len; i++)
    {
        cmd = ( (uint32) pCmd[i]) <<8;
        cmd = cmd | I2CCMD_WRITE ;

#if (I2C_CMDBUF_SUPPORT)
        if ( (i== (len-1)) && (!no_stop))
        {
            cmd = cmd | I2CCMD_STOP;
            flag = 1;
        }
#endif
        IIC_PRINT (("[IIC DRV:]I2C_PHY_WriteBytes_V0: cmd=%x", cmd));

#if (!I2C_CMDBUF_SUPPORT)
        ptr->cmd = cmd;
        I2C_WAIT_INT
        I2C_CLEAR_INT

        //check ACK
        if (ack_en)
        {
            I2C_WAIT_ACK
        }

        if ( (i== (len-1)) && (!no_stop))
        {
            cmd = I2CCMD_STOP;
            ptr->cmd = cmd;
            I2C_WAIT_INT
            I2C_CLEAR_INT
        }

#else
        ptr->cmd_buf = cmd;
        cmd_buffer_nr++;
        SCI_ASSERT(I2C_CMDBUF_SIZE > cmd_buffer_nr);/*assert to do*/
        if(flag)
        {
            ptr->ctl |=  I2CCTL_CMDBUF_EXEC;
            I2C_WAIT_INT
            I2C_CLEAR_INT

            for(k=0;k<cmd_buffer_nr;k++)
            {
                cmd = ptr->cmd_buf;
                //IIC_PRINT (("[IIC DRV:]I2C_PHY_WriteBytes_V0xx: write cmd=%x", cmd));
            }
            cmd_buffer_nr = 0;
        }
#endif

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
    uint32 ret_value = ERR_I2C_NONE;
    uint32 cmd_buf_save_nr1;
#if (I2C_CMDBUF_SUPPORT)	
    uint32 cmd_buf_save_nr2;
    uint32 j = 0;
#endif
    uint32 flag;

    flag = 0;
    cmd_buf_save_nr1 = cmd_buffer_nr;
    for (i=0; i<len; i++)
    {
        cmd = I2CCMD_READ; /*FIXME |I2CCMD_TX_ACK;*/

        if (i== (len-1))
        {
#if (I2C_CMDBUF_SUPPORT)
            cmd = cmd |I2CCMD_STOP |I2CCMD_TX_ACK;
            flag = 1;
#else
            cmd = cmd | I2CCMD_TX_ACK;
#endif

        }

        IIC_PRINT (("[IIC DRV:]I2C_PHY_ReadBytes_V0: cmd=%x", cmd));

#if (!I2C_CMDBUF_SUPPORT)
        ptr->cmd = cmd;
        I2C_WAIT_INT
        I2C_CLEAR_INT
        pCmd[i] = (uint8) ( (ptr->cmd) >>8);

        if (i== (len-1))
        {
            cmd = I2CCMD_STOP;
            ptr->cmd = cmd;
            I2C_WAIT_INT
            I2C_CLEAR_INT
        }
#else
        ptr->cmd_buf = cmd;
        cmd_buffer_nr++;
        SCI_ASSERT(I2C_CMDBUF_SIZE > cmd_buffer_nr);/*assert to do*/
        if(flag)
        {
            cmd_buf_save_nr2 = cmd_buffer_nr;
            ptr->ctl |=  I2CCTL_CMDBUF_EXEC;
            I2C_WAIT_INT
            I2C_CLEAR_INT

            for(j=0;j<cmd_buf_save_nr2;j++)
            {
                cmd = (uint8) ( (ptr->cmd_buf) >>8);
                //IIC_PRINT (("[IIC DRV:]I2C_PHY_WriteBytes_V0xx: read cmd=%x", cmd));
                if(j>=cmd_buf_save_nr1)
                    pCmd[j-cmd_buf_save_nr1] = (uint8)(cmd);
            }
            cmd_buffer_nr = 0;
        }
#endif
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
