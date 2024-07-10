/*release the bus method*/
/******************************************************************************
 ** File Name:      SPI_hal.c                                                *
 ** Author:         liuhao                                                   *
 ** DATE:           12/20/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the hal layer of SPI device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/20/2010     liuhao           Create.                                   *
 ** 05/30/2011     wenjun.shi     Add SPI_HAL_Read_Async(), 
 **                                   SPI_HAL_Write_Async().                  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "spi_drvapi.h"
#include "spi_phy.h"

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
typedef struct
{
    SCI_MUTEX_PTR mutex;
    uint32 current_mode;
    uint32 current_bit_len;
    uint32 current_freq;
    uint32 current_pin_gid;
    uint32 reference;
    SPI_PHY_FUN *phy_fun;
} SPI_BUS;


#define SPI_BOUNDRY_NUM 32

#define SPI_GetMutex(mutex)  {\
        uint32 ret = SCI_SUCCESS;\
        if( NULL == mutex )\
        {\
            return ERR_SPI_NO_MUTEX;\
        }\
        if(SCI_InThreadContext())\
        {\
            ret = SCI_GetMutex(mutex, SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0);\
        }\
        SCI_ASSERT( ret == SCI_SUCCESS );/*assert verified*/\
    }

#define SPI_PutMutex(mutex)  {\
        uint32 ret = SCI_SUCCESS;\
        if( NULL == mutex )\
        {\
            return ERR_SPI_NO_MUTEX;\
        }\
        if(SCI_InThreadContext())\
        {\
            ret = SCI_PutMutex( mutex );\
        }\
        SCI_ASSERT( ret == SCI_SUCCESS );/*assert verified*/\
    }
/**---------------------------------------------------------------------------*
 **                            Local Variables
 **---------------------------------------------------------------------------*/
LOCAL SPI_DEV __spi_dev[SPI_ID_MAX];

/**---------------------------------------------------------------------------*
 **                            Global Variables
 **---------------------------------------------------------------------------*/
SPI_BUS __spi_bus[SPI_BUS_MAX];

extern const SPI_PHY_CFG __spi_phy_cfg[SPI_ID_MAX];
extern PUBLIC SPI_RW_CTX_T s_spi_dma_ctx;
extern PUBLIC _spi_callback spi_tx_callback;
extern PUBLIC _spi_callback spi_rx_callback;
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
LOCAL void SPI_Bus_CFG (uint32 logic_id)
{
    uint32 bus_id = __spi_phy_cfg[logic_id].phy_id;
    /*config bus method*/
    __spi_bus[bus_id].phy_fun = __spi_phy_cfg[logic_id].phy_fun;
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
LOCAL uint32 SPI_Bus_Init (uint32 logic_id)
{
    uint32 bus_id = __spi_phy_cfg[logic_id].phy_id;
    uint32 pin_gid = __spi_phy_cfg[logic_id].pin_gid;
    
    SPI_PRINT(("[SPI DRV:]SPI_Bus_Init"));
    
    /*config bus property*/
    __spi_bus[bus_id].mutex = SCI_CreateMutex ("SPI SYNC MUTEX", SCI_INHERIT);
    SCI_PASSERT ( (NULL!= __spi_bus[bus_id].mutex), ("SPI Great MUTEX fail!"));/*assert verified*/
    __spi_bus[bus_id].current_mode = __spi_dev[logic_id].mode;
    __spi_bus[bus_id].current_bit_len = __spi_dev[logic_id].tx_bit_length;
    __spi_bus[bus_id].current_freq = __spi_dev[logic_id].freq;
    __spi_bus[bus_id].current_pin_gid = pin_gid;
    /*config bus method*/
    SPI_Bus_CFG (logic_id);
    
    /*select pin group*/
    if (PNULL != __spi_bus[bus_id].phy_fun->pingrp_select)
    {
        __spi_bus[bus_id].phy_fun->pingrp_select (bus_id, pin_gid);
    }

    /*disable deep sleep*/
    //SCI_SPI_EnableDeepSleep (SCI_FALSE);
    return 0;
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
LOCAL uint32 SPI_Bus_Exit (uint32 bus_id)
{
    uint32 status_val = 0;
    uint32 i = 0;
    __spi_bus[bus_id].reference =  0;

    if (NULL != __spi_bus[bus_id].mutex)
    {
        status_val = SCI_DeleteMutex (__spi_bus[bus_id].mutex);
        //SCI_PASSERT((0 != status_val),("SPI delete MUTEX fail!"));
        __spi_bus[bus_id].mutex = NULL;
    }

    SPI_PRINT(("[SPI DRV:]SPI_Bus_Exit: status=%d", status_val));
    /*release the bus method*/
    __spi_bus[bus_id].phy_fun = NULL;

    /*check if enable sleep*/
    for (i=0; i<SPI_BUS_MAX; i++)
    {
        if (0 != __spi_bus[i].reference)
        {
            return 0;
        }
    }
    //SCI_SPI_EnableDeepSleep (SCI_TRUE);
    
    return 0;
}

/*********************************************************************************************************
** Function name: SPI_HAL_Open
** Descriptions: spi bus open function
** input parameters:
**      dev - pointer of spi slave dev
**
**
** output parameters: NONE
** Returned value:
*********************************************************************************************************/
PUBLIC int32 SPI_HAL_Open (SPI_DEV *dev)
{
    uint32 logic_id;
    uint32 bus_id;
    uint32 cs_id;
#ifdef PLATFORM_SC8800G
    uint32 pad_name = SPI_PAD_MAX;
#endif
    SCI_ASSERT (NULL != dev);/*assert verified*/
    /*check if use the wrong id number*/
    logic_id = dev->id;
    SCI_ASSERT (SPI_ID_MAX > dev->id);/*assert verified*/

    /*now we should get the spi bus id and the controller's cs id*/
    bus_id = __spi_phy_cfg[logic_id].phy_id;
    cs_id = __spi_phy_cfg[logic_id].cs_id;

    /*set the parameters for spi slave device*/
    __spi_dev[logic_id].id = logic_id;
    __spi_dev[logic_id].mode = dev->mode;
    __spi_dev[logic_id].tx_bit_length = dev->tx_bit_length;
    __spi_dev[logic_id].freq = dev->freq;
    __spi_dev[logic_id].spi_cs_cb = dev->spi_cs_cb;
    __spi_dev[logic_id].spi_tx_cb = dev->spi_tx_cb;
    __spi_dev[logic_id].spi_rx_cb = dev->spi_rx_cb;
       
     SPI_PRINT(("[SPI DRV:]SPI_HAL_Open:logic_id=%d, mode=%d, bit_len=%d, freq=%d", logic_id, dev->mode, dev->tx_bit_length, dev->freq));

    /*first open, than init spi controller*/
    if (0 == __spi_bus[bus_id].reference)
    {
        SPI_Bus_Init (logic_id);
        __spi_bus[bus_id].phy_fun->init (bus_id);
        __spi_bus[bus_id].phy_fun->cfg (bus_id, __spi_dev[logic_id].mode, __spi_dev[logic_id].tx_bit_length, __spi_dev[logic_id].freq);
    }
#ifdef PLATFORM_SC8800G
    /* set cs pad to output '0' to reduce power consumption. 
     * however, it's better to be done in APP layer by calling 
     * SPI_HAL_Ioctl(,SPI_CTL_S_PAD ,). So the following is removed 
     * in PLATFORM_SC6530.
    */
    if ( PNULL != __spi_bus[bus_id].phy_fun->set_pad )
    {
        /* to do: add get_pad_name instead*/
        if (cs_id == 0)
        {
            pad_name = PAD_SPI0_0_CS0;
        }
        else if (cs_id == 1)
        {
            pad_name = PAD_SPI0_0_CS1;
        }
        __spi_bus[bus_id].phy_fun->set_pad(pad_name, SPI_PAD_SPI, 0);
    }
#endif
    __spi_bus[bus_id].reference++;

    return  0;
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
PUBLIC int32 SPI_HAL_Close (uint32 logic_id)
{
    uint32 bus_id;
    uint32 cs_id;
#ifdef PLATFORM_SC8800G
    uint32 pad_name = SPI_PAD_MAX;
#endif   
    /*logic_id shouldn't larger than SPI_ID_MAX*/
    SCI_ASSERT (SPI_ID_MAX > logic_id);/*assert verified*/

    cs_id = __spi_phy_cfg[logic_id].cs_id;
    /*this logic_id is already closed*/
    if (0 == __spi_dev[logic_id].freq)
    {
        return -1;
    }
    
    bus_id = __spi_phy_cfg[logic_id].phy_id;
    
#ifdef PLATFORM_SC8800G
    //set cs_x pad to gpio, output '0'
    if ( PNULL != __spi_bus[bus_id].phy_fun->set_pad )
    {
        if (cs_id == 0)
        {
            pad_name = PAD_SPI0_0_CS0;
        }
        else if (cs_id == 1)
        {
            pad_name = PAD_SPI0_0_CS1;
        }
        __spi_bus[bus_id].phy_fun->set_pad(pad_name, SPI_PAD_GPIO, 0);
    }
#endif
    
    if (1 == __spi_bus[bus_id].reference)
    {
        __spi_bus[bus_id].phy_fun->deinit(bus_id);
        SPI_Bus_Exit (bus_id);
    }
    else
    {
        __spi_bus[bus_id].reference--;
    }

    /*release the logic_id*/
    __spi_dev[logic_id].id = 0;
    __spi_dev[logic_id].mode = 0;
    __spi_dev[logic_id].tx_bit_length = 0;
    __spi_dev[logic_id].freq = 0;
    __spi_dev[logic_id].spi_cs_cb = PNULL;
    __spi_dev[logic_id].spi_tx_cb = PNULL;
    __spi_dev[logic_id].spi_rx_cb = PNULL;
    SPI_PRINT(("[SPI DRV:]SPI_HAL_Close: logic_id=%d, ref=%d", logic_id, __spi_bus[bus_id].reference));

    return  0;
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
PUBLIC uint32 SPI_HAL_Read (uint32 logic_id, uint8 *buffer_ptr, uint32 len)
{
    uint32 ret_val;
    uint32 bus_id;
    uint32 err_code;
    uint32 spi_bitlen;
    uint32 max_receive_byte;
    //uint32 time_out_cnt;

    /*logic_id shouldn't larger than SPI_ID_MAX*/
    SCI_ASSERT (SPI_ID_MAX > logic_id);/*assert verified*/
    SCI_ASSERT (NULL != buffer_ptr);/*assert verified*/
    SCI_ASSERT (0 < len);/*assert verified*/
    /*the logic_id has destroied*/
    SCI_ASSERT (0 != __spi_dev[logic_id].freq);/*assert verified*/

    SPI_PRINT(("[SPI DRV:]SPI_HAL_Read:logic_id=%d, buffer_ptr=%x, len=%d", logic_id, buffer_ptr, len));
    bus_id = __spi_phy_cfg[logic_id].phy_id;
    spi_bitlen = __spi_bus[bus_id].current_bit_len;

    /*in receive only mode, receive num should be multiple of SPI_THRHOLD_CNT's byte*/
    max_receive_byte = ((SPI_MAX_RECEIVE_WORD - (SPI_MAX_RECEIVE_WORD % ( SPI_THRHOLD_CNT )))*(spi_bitlen/BITLEN_8));
    
    do
    {
        err_code = 0;
        /*read data*/

        /*spi polling mode*/
        ret_val = __spi_bus[bus_id].phy_fun->read (bus_id, buffer_ptr, len);
        
        if (ERR_SPI_NONE != ret_val)
        {
            break;
        }
        err_code = len;
    }
    while (0);

    return  err_code;
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
PUBLIC uint32 SPI_HAL_Write (uint32 logic_id, uint8 *buffer_ptr, uint32 len)
{
    uint32 ret_val;
    uint32 bus_id;
    uint32 err_code;
    uint32 spi_bitlen;
    
    /*logic_id shouldn't larger than SPI_ID_MAX*/
    SCI_ASSERT (SPI_ID_MAX > logic_id);/*assert verified*/
    SCI_ASSERT (NULL != buffer_ptr);/*assert verified*/
    SCI_ASSERT (0 < len);/*assert verified*/
    /*the logic_id has destroied*/
    SCI_ASSERT (0 != __spi_dev[logic_id].freq);/*assert verified*/

    SPI_PRINT(("[SPI DRV:]SPI_HAL_Write:logic_id=%d, buffer_ptr=%x, len=%d", logic_id, buffer_ptr, len));

    bus_id = __spi_phy_cfg[logic_id].phy_id;
    spi_bitlen = __spi_bus[bus_id].current_bit_len;
    
    do
    {
        err_code = 0;
        /*write spi slave device's data*/

        /*spi polling mode*/
        ret_val = __spi_bus[bus_id].phy_fun->write (bus_id, buffer_ptr, len);
        if (ERR_SPI_NONE != ret_val)
        {
            break;
        }
        err_code = len;
    }
    while (0);

    return  err_code;
}


PUBLIC uint32 SPI_HAL_LCDRead (uint32 logic_id, uint32 *buffer_ptr, uint32 data_len, uint32 dummy_len)
{
    uint32 err_code=0;

#if  defined(MAINLCM_INTERFACE_SPI) || defined(SUBLCM_INTERFACE_SPI)
    uint32 ret_val;
    uint32 bus_id;
    uint32 spi_bitlen;

    /*logic_id shouldn't larger than SPI_ID_MAX*/
    SCI_ASSERT (SPI_ID_MAX > logic_id);/*assert verified*/
    SCI_ASSERT (NULL != buffer_ptr);/*assert verified*/
    SCI_ASSERT (0 < data_len);/*assert verified*/
    /*the logic_id has destroied*/
    SCI_ASSERT (0 != __spi_dev[logic_id].freq);/*assert verified*/

    SPI_PRINT (("[SPI DRV:]SPI_HAL_LCDRead:logic_id=%d, buffer_ptr=%x, len=%d", logic_id, buffer_ptr, data_len));

    bus_id = __spi_phy_cfg[logic_id].phy_id;
    err_code =  __spi_phy_cfg[logic_id].phy_fun->lcd_read(bus_id, buffer_ptr, data_len, dummy_len);
#endif

    return  err_code;
}

PUBLIC uint32 SPI_HAL_LCDWrite(uint32 logic_id, uint32 *buffer_ptr, uint32 data_len, uint32 dummy_len)
{
    uint32 err_code=0;
#if  defined(MAINLCM_INTERFACE_SPI) || defined(SUBLCM_INTERFACE_SPI)
    uint32 ret_val;
    uint32 bus_id;
    uint32 spi_bitlen;
    
    /*logic_id shouldn't larger than SPI_ID_MAX*/
    SCI_ASSERT (SPI_ID_MAX > logic_id);/*assert verified*/
    SCI_ASSERT (NULL != buffer_ptr);/*assert verified*/
    SCI_ASSERT (0 < data_len);/*assert verified*/
    /*the logic_id has destroied*/
    SCI_ASSERT (0 != __spi_dev[logic_id].freq);/*assert verified*/

    SPI_PRINT (("[SPI DRV:]SPI_HAL_LCDWrite:logic_id=%d, buffer_ptr=%x, len=%d", logic_id, buffer_ptr, data_len));

    bus_id = __spi_phy_cfg[logic_id].phy_id;
    err_code =  __spi_phy_cfg[logic_id].phy_fun->lcd_write(bus_id, buffer_ptr, data_len, dummy_len);
#endif

    return  err_code;
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
PUBLIC uint32 SPI_HAL_Ioctl (uint32 logic_id, uint32 cmd, uint32 *arg)
{
    uint32 ret_val = 0;
    uint32 bus_id;
    uint32 cs_id;
    uint32 pin_gid;
    _spi_callback spi_cs_callback = PNULL;
    
    /*logic_id shouldn't larger than SPI_ID_MAX*/
    SCI_ASSERT (SPI_ID_MAX > logic_id);/*assert verified*/
    SCI_ASSERT (NULL != arg);/*assert verified*/

    bus_id = __spi_phy_cfg[logic_id].phy_id;
    cs_id = __spi_phy_cfg[logic_id].cs_id;
    pin_gid = __spi_phy_cfg[logic_id].pin_gid;
    spi_cs_callback = __spi_dev[logic_id].spi_cs_cb;
    
    SPI_PRINT(("[SPI DRV:]SPI_HAL_Ioctl: logic_id=%d, cmd=%x, arg=%d", logic_id, cmd, *arg));

    switch (cmd)
    {
        case SPI_CTL_G_MODE:
            *arg = __spi_bus[bus_id].current_mode;
            break;
        case SPI_CTL_S_MODE:
            ret_val = __spi_bus[bus_id].phy_fun->set_mode (bus_id, *arg);
            break;
        case SPI_CTL_G_BITLEN:
            *arg = __spi_bus[bus_id].current_bit_len;
            break;
        case SPI_CTL_S_BITLEN:
            ret_val = __spi_bus[bus_id].phy_fun->set_tx_bit_len (bus_id, *arg);
            __spi_bus[bus_id].current_bit_len = *arg;
            break;
        case SPI_CTL_G_FREQ:
            *arg = __spi_bus[bus_id].current_freq;
            break;
        case SPI_CTL_S_FREQ:
            ret_val = __spi_bus[bus_id].phy_fun->set_freq (bus_id, *arg);
            break;
        case SPI_CTL_S_CS:
            if (*arg == SPI_CS_HIGH)
            {
                /*selet pin group*/
                if ((__spi_bus[bus_id].current_pin_gid != pin_gid) && (PNULL != __spi_bus[bus_id].phy_fun->pingrp_select))
                {
                    __spi_bus[bus_id].phy_fun->pingrp_select (bus_id, pin_gid);
                    __spi_bus[bus_id].current_pin_gid = pin_gid;
                }
                cs_id = 0xf;
                ret_val = __spi_bus[bus_id].phy_fun->set_cs (bus_id, cs_id, *arg);
                /*release spi bus*/
                SPI_PutMutex (__spi_bus[bus_id].mutex);
            }
            else
            {
                /*get spi bus*/
                SPI_GetMutex (__spi_bus[bus_id].mutex);
                /*selet pin group*/
                if ((__spi_bus[bus_id].current_pin_gid != pin_gid) && (PNULL != __spi_bus[bus_id].phy_fun->pingrp_select))
                {
                    __spi_bus[bus_id].phy_fun->pingrp_select (bus_id, pin_gid);
                    __spi_bus[bus_id].current_pin_gid = pin_gid;
                }
                
                if ( PNULL != spi_cs_callback)
                {
                    spi_cs_callback (&logic_id);
                }
                /*set spi controller*/
                if ( (__spi_bus[bus_id].current_freq != __spi_dev[logic_id].freq) ||
                        (__spi_bus[bus_id].current_bit_len != __spi_dev[logic_id].tx_bit_length) ||
                        (__spi_bus[bus_id].current_mode != __spi_dev[logic_id].mode))
                {
                    SPI_PRINT(("[SPI DRV:]SPI_HAL_Read:reinit"));
                    __spi_bus[bus_id].phy_fun->cfg (bus_id, __spi_dev[logic_id].mode, __spi_dev[logic_id].tx_bit_length, __spi_dev[logic_id].freq);
                    __spi_bus[bus_id].current_freq = __spi_dev[logic_id].freq;
                    __spi_bus[bus_id].current_bit_len = __spi_dev[logic_id].tx_bit_length;
                    __spi_bus[bus_id].current_mode = __spi_dev[logic_id].mode;
                }
                ret_val = __spi_bus[bus_id].phy_fun->set_cs (bus_id, cs_id, *arg);
            }
            break;
        case SPI_CTL_S_MUTEX:
            if (*arg == SPI_MUTEX_PUT)
            {
                /*release spi bus*/
                SPI_PutMutex (__spi_bus[bus_id].mutex);
            }
            else
            {
                /*get spi bus*/
                SPI_GetMutex (__spi_bus[bus_id].mutex);
            }
            break;
        case SPI_CTL_S_CS_NO_MUTEX:
            if (*arg == SPI_CS_HIGH)
            {
                /*selet pin group*/
                if ((__spi_bus[bus_id].current_pin_gid != pin_gid) && (PNULL != __spi_bus[bus_id].phy_fun->pingrp_select))
                {
                    __spi_bus[bus_id].phy_fun->pingrp_select (bus_id, pin_gid);
                    __spi_bus[bus_id].current_pin_gid = pin_gid;
                }
                // cs_id = 0xf; //
                ret_val = __spi_bus[bus_id].phy_fun->set_cs (bus_id, cs_id, *arg);
            }
            else
            {
                /*selet pin group*/
                if ((__spi_bus[bus_id].current_pin_gid != pin_gid) && (PNULL != __spi_bus[bus_id].phy_fun->pingrp_select))
                {
                    __spi_bus[bus_id].phy_fun->pingrp_select (bus_id, pin_gid);
                    __spi_bus[bus_id].current_pin_gid = pin_gid;
                }
                /*set spi controller*/
                if ( (__spi_bus[bus_id].current_freq != __spi_dev[logic_id].freq) ||
                        (__spi_bus[bus_id].current_bit_len != __spi_dev[logic_id].tx_bit_length) ||
                        (__spi_bus[bus_id].current_mode != __spi_dev[logic_id].mode))
                {
                    SPI_PRINT(("[SPI DRV:]SPI_HAL_Read:reinit"));
                    __spi_bus[bus_id].phy_fun->cfg (bus_id, __spi_dev[logic_id].mode, __spi_dev[logic_id].tx_bit_length, __spi_dev[logic_id].freq);
                    __spi_bus[bus_id].current_freq = __spi_dev[logic_id].freq;
                    __spi_bus[bus_id].current_bit_len = __spi_dev[logic_id].tx_bit_length;
                    __spi_bus[bus_id].current_mode = __spi_dev[logic_id].mode;
                }
                ret_val = __spi_bus[bus_id].phy_fun->set_cs (bus_id, cs_id, *arg);
            }
            break;			
        case SPI_CTL_S_PAD:   
            if ( ((SPI_PAD_CFG_T*)arg)->pad_fun == SPI_PAD_SPI)
            {
                /* Param 'output ' does not make sense when 'pad_fun == SPI_PAD_SPI' */
                ret_val = __spi_bus[bus_id].phy_fun->set_pad( ((SPI_PAD_CFG_T*)arg)->pad_name, SPI_PAD_SPI, ((SPI_PAD_CFG_T*)arg)->output );
            }
            else
            {
                ret_val = __spi_bus[bus_id].phy_fun->set_pad( ((SPI_PAD_CFG_T*)arg)->pad_name, SPI_PAD_GPIO, ((SPI_PAD_CFG_T*)arg)->output );
            }
            break;
#if  defined(MAINLCM_INTERFACE_SPI) || defined(SUBLCM_INTERFACE_SPI)
    	case SPI_CTL_S_CDLEVEL:
            {
                #if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
                if((logic_id == SPI0_0_CS0) ||
                   (logic_id == SPI1_0_CS0))
                #else
                if((logic_id == SPI0_0_CS0) ||
                   (logic_id == SPI0_0_CS1) ||
                   (logic_id == SPI0_1_CS0) ||
                   (logic_id == SPI0_1_CS1) ||
                   (logic_id == SPI0_2_CS0) ||
                   (logic_id == SPI0_2_CS1) ||
                   (logic_id == SPI1_0_CS0) ||
                   (logic_id == SPI1_0_CS1) ||
                   (logic_id == SPI1_1_CS0) ||
                   (logic_id == SPI1_1_CS1) ||
                   (logic_id == SPI1_2_CS0) ||
                   (logic_id == SPI1_2_CS1) )
                #endif
                {
                    if(*arg)
                        *arg = SPI_CS_HIGH;
                    else
                        *arg = SPI_CS_LOW;
                    SPI_HAL_Ioctl(logic_id, SPI_CTL_S_CS_NO_MUTEX, arg);
                }
                else
                {
        	        __spi_bus[bus_id].phy_fun->set_cd_level(bus_id, *arg);
                }
    	    }
    	   break;
    	   
    	case SPI_CTL_G_DATAPORT:
    	    __spi_bus[bus_id].phy_fun->get_data_port(bus_id, arg);
    	   break;	

    	case SPI_CTL_S_LCDWORKMODE:
    	    __spi_bus[bus_id].phy_fun->set_lcd_workmode(bus_id, *arg);	    	
    	    break;

    	case SPI_CTL_S_TRANSMODE:
    	    __spi_bus[bus_id].phy_fun->set_trans_mode(bus_id, *arg);   	
    	    break;			
#endif
        default:
            break;
    }

    return  ret_val;
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
PUBLIC uint32 SPI_HAL_RW (uint32 logic_id, uint8 *din_buffer_ptr, uint8 *dout_buffer_ptr, uint32 len)
{
    uint32 ret_val;
    uint32 bus_id;
    uint32 err_code;

    /*logic_id shouldn't larger than SPI_ID_MAX*/
    SCI_ASSERT (SPI_ID_MAX > logic_id);/*assert verified*/
    SCI_ASSERT (NULL != din_buffer_ptr);/*assert verified*/
    SCI_ASSERT (NULL != dout_buffer_ptr);/*assert verified*/
    SCI_ASSERT (0 < len);/*assert verified*/
    /*the logic_id has destroied*/
    SCI_ASSERT (0 != __spi_dev[logic_id].freq);/*assert verified*/

    bus_id = __spi_phy_cfg[logic_id].phy_id;

    /*set spi controller*/
    if ( (__spi_bus[bus_id].current_freq != __spi_dev[logic_id].freq) ||
            (__spi_bus[bus_id].current_bit_len != __spi_dev[logic_id].tx_bit_length) ||
            (__spi_bus[bus_id].current_mode != __spi_dev[logic_id].mode))
    {
        __spi_bus[bus_id].phy_fun->cfg (bus_id, __spi_dev[logic_id].mode, __spi_dev[logic_id].tx_bit_length, __spi_dev[logic_id].freq);
        __spi_bus[bus_id].current_freq = __spi_dev[logic_id].freq;
        __spi_bus[bus_id].current_bit_len = __spi_dev[logic_id].tx_bit_length;
        __spi_bus[bus_id].current_mode = __spi_dev[logic_id].mode;
    }

    do
    {
        err_code = 0;

        /*write spi slave device's data and stop bus*/
        ret_val = __spi_bus[bus_id].phy_fun->rw (bus_id, din_buffer_ptr, dout_buffer_ptr, len);
        if (ERR_SPI_NONE != ret_val)
        {
            break;
        }

        err_code = len;
    }
    while (0);

    return  err_code;
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
PUBLIC uint32 SPI_HAL_Read_Async (uint32 logic_id, uint8 *buffer_ptr, uint32 len, _spi_callback rx_cb)
{
    uint32 ret_val;
    uint32 bus_id;
    uint32 err_code;
    uint32 dma_len;
    uint32 remain_size;
    uint32 addr_offset = SPI_BOUNDRY_NUM - (((uint32) buffer_ptr) % SPI_BOUNDRY_NUM);
    uint32 spi_bitlen;
    //uint32 time_out_cnt;

    /*logic_id shouldn't larger than SPI_ID_MAX*/
    SCI_ASSERT (SPI_ID_MAX > logic_id);/*assert verified*/
    SCI_ASSERT (NULL != buffer_ptr);/*assert verified*/
    SCI_ASSERT (0 < len);/*assert verified*/
    /*the logic_id has destroied*/
    SCI_ASSERT (0 != __spi_dev[logic_id].freq);/*assert verified*/

    SPI_PRINT(("[SPI DRV:]SPI_HAL_Read_Async:logic_id=%d, buffer_ptr=%x, len=%d", logic_id, buffer_ptr, len));
    bus_id = __spi_phy_cfg[logic_id].phy_id;
    spi_bitlen = __spi_bus[bus_id].current_bit_len;
    spi_rx_callback = rx_cb;
    SCI_ASSERT(PNULL != spi_rx_callback);   /*assert to do*/  
    do
    {
        err_code = 0;
        /*small amount of data*/
        if ((len  < (((SPI_THRHOLD_CNT) *(spi_bitlen/BITLEN_8)) + SPI_BOUNDRY_NUM)) || (PNULL == __spi_bus[bus_id].phy_fun->dma_rw_int))
        {
            /*read data*/
            /*spi interrput mode*/
            ret_val = __spi_bus[bus_id].phy_fun->read_int (bus_id, buffer_ptr, len);
            
            if (ERR_SPI_NONE != ret_val)
            {
                break;
            }
        }
        /*large amount of data, guarantee at least one dma transfer*/
        else
        {
            /*keep dma read addr. alignment*/
            if (addr_offset < SPI_BOUNDRY_NUM && addr_offset > 0)
            {
                /*read data*/
                ret_val = __spi_bus[bus_id].phy_fun->read (bus_id, buffer_ptr, addr_offset);
                if (ERR_SPI_NONE != ret_val)
                {
                    break;
                }

                len -= addr_offset;
                buffer_ptr += addr_offset;
            }

            remain_size = len % ((SPI_THRHOLD_CNT) *(spi_bitlen/BITLEN_8));
            dma_len     = len - remain_size;

            /*spi dma interrupt mode*/
            s_spi_dma_ctx.spi_rw_remain_size = remain_size;
            s_spi_dma_ctx.spi_rw_rx_buf_ptr = buffer_ptr + dma_len;
            ret_val = __spi_bus[bus_id].phy_fun->dma_rw_int (bus_id, buffer_ptr, PNULL,  dma_len);
            if (ERR_SPI_NONE != ret_val)
            {
                break;
            }
        } /*large amount of data*/
        
        err_code = len;
    }
    while (0);

    return  err_code;
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
PUBLIC uint32 SPI_HAL_Write_Async (uint32 logic_id, uint8 *buffer_ptr, uint32 len, _spi_callback tx_cb)
{
    uint32 ret_val;
    uint32 bus_id;
    uint32 err_code;
    uint32 dma_len;
    uint32 remain_size;
    //uint32 addr_offset = 4 - (((uint32) buffer_ptr) % 4);
    uint32 spi_bitlen;
    //uint32 time_out_cnt;
    
    /*logic_id shouldn't larger than SPI_ID_MAX*/
    SCI_ASSERT (SPI_ID_MAX > logic_id);/*assert verified*/
    SCI_ASSERT (NULL != buffer_ptr);/*assert verified*/
    SCI_ASSERT (0 < len);/*assert verified*/
    /*the logic_id has destroied*/
    SCI_ASSERT (0 != __spi_dev[logic_id].freq);/*assert verified*/

    SPI_PRINT(("[SPI DRV:]SPI_HAL_Write_Async:logic_id=%d, buffer_ptr=%x, len=%d", logic_id, buffer_ptr, len));

    bus_id = __spi_phy_cfg[logic_id].phy_id;
    spi_bitlen = __spi_bus[bus_id].current_bit_len;
    spi_tx_callback = tx_cb;
    SCI_ASSERT(PNULL != spi_tx_callback);   /*assert to do*/  
    do
    {
        err_code = 0;
        /*small amount of data*/
        if ((len < (((SPI_THRHOLD_CNT) *(spi_bitlen/BITLEN_8))))  || (PNULL == __spi_bus[bus_id].phy_fun->dma_rw_int))
        {
            /*write spi slave device's data*/
            /*spi interrupt mode*/
            ret_val = __spi_bus[bus_id].phy_fun->write_int (bus_id, buffer_ptr, len);

            if (ERR_SPI_NONE != ret_val)
            {
                break;
            }
        }
        /*large amount of data, guarantee at least one dma transfer*/
        else
        {
            remain_size = len % ((SPI_THRHOLD_CNT) *(spi_bitlen/BITLEN_8));
            dma_len     = len - remain_size;
            /*spi dma interrupt mode*/
            s_spi_dma_ctx.spi_rw_remain_size = remain_size;
            s_spi_dma_ctx.spi_rw_tx_buf_ptr = buffer_ptr + dma_len;
            ret_val = __spi_bus[bus_id].phy_fun->dma_rw_int (bus_id, PNULL, buffer_ptr, dma_len);
            
            if (ERR_SPI_NONE != ret_val)
            {
                break;
            }

        }/*large amount of data*/

        err_code = len;
    }
    while (0);

    return  err_code;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/*  End Of File */
