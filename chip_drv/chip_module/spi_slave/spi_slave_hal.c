/******************************************************************************
 ** File Name:      SPI_SLAVE_hal.c                                                *
 ** Author:         liuhao                                                   *
 ** DATE:           12/20/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the hal layer of SPI_SLAVE device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/20/2010     liuhao     Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "spi_slave_drvapi.h"
#include "spi_slave_phy.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"

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
    uint32 reference;
    SPI_SLAVE_PHY_FUN *phy_fun;
} SPI_SLAVE_BUS;

#define SPI_SLAVE_GetMutex(mutex)  {\
        uint32 ret = SCI_SUCCESS;\
        if( NULL == mutex )\
        {\
            return ERR_SPI_SLAVE_NO_MUTEX;\
        }\
        if(SCI_InThreadContext())\
        {\
            ret = SCI_GetMutex(mutex, SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0);\
        }\
        SCI_ASSERT( ret == SCI_SUCCESS );/*assert verified*/\
    }

#define SPI_SLAVE_PutMutex(mutex)  {\
        uint32 ret = SCI_SUCCESS;\
        if( NULL == mutex )\
        {\
            return ERR_SPI_SLAVE_NO_MUTEX;\
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
LOCAL SPI_SLAVE_DEV __spi_slave_dev = {3, 32, PNULL, PNULL}; //mode 3, tx_bit_len 32
LOCAL SPI_SLAVE_BUS __spi_slave_bus = {NULL, 0, PNULL};

/**---------------------------------------------------------------------------*
 **                            Global Variables
 **---------------------------------------------------------------------------*/
extern SPI_SLAVE_PHY_FUN spi_slave_phy_fun;

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
LOCAL __inline void SPI_SLAVE_Bus_Init (void)
{
    SPI_SLAVE_PRINT ("[SPI_SLAVE DRV:]SPI_SLAVE_Bus_Init");

    /*config bus property*/
    __spi_slave_bus.mutex = SCI_CreateMutex ("SPI SLAVE SYNC MUTEX", SCI_INHERIT);
    SCI_PASSERT ( (NULL!= __spi_slave_bus.mutex), ("SPI SLAVE Great MUTEX fail!"));/*assert verified*/
    __spi_slave_bus.reference = 1;
    /*config bus method*/
    __spi_slave_bus.phy_fun = &spi_slave_phy_fun;

    /*disable deep sleep*/
    SCI_SPI_EnableDeepSleep (SCI_FALSE);
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
LOCAL uint32 SPI_SLAVE_Bus_Exit (void)
{
    uint32 status_val = 0;

    SPI_SLAVE_PRINT ("[SPI_SLAVE DRV:]SPI_SLAVE_Bus_Exit");

    if (NULL != __spi_slave_bus.mutex)
    {
        status_val = SCI_DeleteMutex (__spi_slave_bus.mutex);
        //SCI_PASSERT((0 != status_val),("SPI SLAVE delete MUTEX fail!"));
        __spi_slave_bus.mutex = NULL;
    }

    __spi_slave_bus.reference =  0;
    __spi_slave_bus.phy_fun = NULL;

    SCI_SPI_EnableDeepSleep (SCI_TRUE);
    return 0;
}

/*********************************************************************************************************
** Function name: SPI_SLAVE_HAL_Open
** Descriptions: spi_slave bus open function
** input parameters:
**      dev - pointer of spi_slave slave dev
**
**
** output parameters: NONE
** Returned value:
*********************************************************************************************************/
PUBLIC void SPI_SLAVE_HAL_Open (SPI_SLAVE_DEV *dev)
{
    /*if dev is null, then we use the default value*/
    if (NULL != dev)
    {
        SCI_ASSERT ( ( (4 > dev->mode) && (0 <= dev->mode)));/*assert verified*/
        /*set the parameters for spi_slave slave device*/
        __spi_slave_dev.mode = dev->mode;
        __spi_slave_dev.tx_bit_length = dev->tx_bit_length;
        __spi_slave_dev.tx_callback = dev->tx_callback;
        __spi_slave_dev.rx_callback = dev->rx_callback;
    }

    SPI_SLAVE_PRINT ("[SPI_SLAVE DRV:]SPI_SLAVE_HAL_Open:mode=%d, bit_len=%d", __spi_slave_dev.mode, __spi_slave_dev.tx_bit_length);

    /*first open, than init spi_slave controller*/
    if (0 == __spi_slave_bus.reference)
    {
        SPI_SLAVE_PRINT ("[SPI_SLAVE DRV:]SPI_SLAVE_HAL_Open:init");
        SPI_SLAVE_Bus_Init ();
        __spi_slave_bus.phy_fun->init (__spi_slave_dev.mode, __spi_slave_dev.tx_bit_length, __spi_slave_dev.tx_callback, __spi_slave_dev.rx_callback);
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
PUBLIC void SPI_SLAVE_HAL_Close (void)
{
    SPI_SLAVE_PRINT ("[SPI_SLAVE DRV:]SPI_SLAVE_HAL_Close");

    if (1 == __spi_slave_bus.reference)
    {
        __spi_slave_bus.phy_fun->exit();
        SPI_SLAVE_Bus_Exit(); /*lint !e527 comfirmed by windy.wang*/
    }

    /*release the logic_id*/
    __spi_slave_dev.mode = 3;
    __spi_slave_dev.tx_bit_length = 32;
    __spi_slave_dev.tx_callback = PNULL;
    __spi_slave_dev.rx_callback = PNULL;
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
PUBLIC uint32 SPI_SLAVE_HAL_Read (uint8 *buffer_ptr, uint32 len)
{
    uint32 ret_val;
    uint32 dma_len;
    uint32 remain_size;
    uint32 addr_offset = ( ( (uint32) buffer_ptr) % 32);

    SCI_ASSERT (NULL != buffer_ptr);/*assert verified*/
    SCI_ASSERT (0 < len);/*assert verified*/

    SPI_SLAVE_PRINT ("[SPI_SLAVE DRV:]SPI_SLAVE_HAL_Read:buffer_ptr=%x, len=%d", buffer_ptr, len);

    /*get spi bus*/
    SPI_SLAVE_GetMutex (__spi_slave_bus.mutex);/*assert verified*/

    do
    {
        ret_val = 0;

        if (len  < 64)
        {
            /*read data*/
            ret_val = __spi_slave_bus.phy_fun->rw (buffer_ptr, len, 0);

            //ret_val = __spi_slave_bus.phy_fun->read (buffer_ptr, len);
            if (ERR_SPI_SLAVE_NONE != ret_val)
            {
                break;
            }
        }
        else
        {
            if (__spi_slave_dev.rx_callback != PNULL)
            {
                __spi_slave_bus.phy_fun->dma_rw (buffer_ptr, len, 0);
                ret_val = len;
                break;
            }

            __spi_slave_bus.phy_fun->rst_fifo();

            if (addr_offset > 0)
            {
                addr_offset = 32 - addr_offset;
                /*read data*/
                ret_val = __spi_slave_bus.phy_fun->read (buffer_ptr, addr_offset);

                if (ERR_SPI_SLAVE_NONE != ret_val)
                {
                    break;
                }

                len -= addr_offset;
                buffer_ptr += addr_offset;
            }

            remain_size = len & (64 - 1);
            dma_len     = len - remain_size;
            ret_val = __spi_slave_bus.phy_fun->dma_r (buffer_ptr, dma_len);

            if (ERR_SPI_SLAVE_NONE != ret_val)
            {
                break;
            }

            if (remain_size > 0)
            {
                buffer_ptr += dma_len;
                ret_val = __spi_slave_bus.phy_fun->read (buffer_ptr, remain_size);

                if (ERR_SPI_SLAVE_NONE != ret_val)
                {
                    break;
                }
            }
        }

        ret_val = len;
    }
    while (0);

    /*release spi bus*/
    SPI_SLAVE_PutMutex (__spi_slave_bus.mutex);/*assert verified*/

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
PUBLIC uint32 SPI_SLAVE_HAL_Write (uint8 *buffer_ptr, uint32 len)
{
    uint32 ret_val;

    SCI_ASSERT (NULL != buffer_ptr);/*assert verified*/
    SCI_ASSERT (0 < len);/*assert verified*/

    SPI_SLAVE_PRINT ("[SPI_SLAVE DRV:]SPI_SLAVE_HAL_Write:buffer_ptr=%x, len=%d", buffer_ptr, len);

    /*get spi bus*/
    SPI_SLAVE_GetMutex (__spi_slave_bus.mutex);/*assert verified*/

    do
    {
        ret_val = 0;

        if (len  < 64)
        {
            /*write spi_slave slave device's data*/
            ret_val = __spi_slave_bus.phy_fun->rw (buffer_ptr, len, 1);

            //ret_val = __spi_slave_bus.phy_fun->write (buffer_ptr, len);
            if (ERR_SPI_SLAVE_NONE != ret_val)
            {
                break;
            }
        }
        else
        {
            if (__spi_slave_dev.tx_callback != PNULL)
            {
                __spi_slave_bus.phy_fun->dma_rw (buffer_ptr, len, 1);
                ret_val = len;
                break;
            }

            ret_val = __spi_slave_bus.phy_fun->dma_w (buffer_ptr, len);

            if (ERR_SPI_SLAVE_NONE != ret_val)
            {
                break;
            }
        }

        ret_val = len;
    }
    while (0);

    /*release spi bus*/
    SPI_SLAVE_PutMutex (__spi_slave_bus.mutex);/*assert verified*/

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
PUBLIC uint32 SPI_SLAVE_HAL_Ioctl (uint32 cmd, uint32 *arg)
{
    uint32 ret_val = 0;

    SCI_ASSERT (NULL != arg);/*assert verified*/

    SPI_SLAVE_PRINT ("[SPI_SLAVE DRV:]SPI_SLAVE_HAL_Ioctl: cmd=%x, arg=%d", cmd, *arg);

    switch (cmd)
    {
        case SPI_SLAVE_CTL_G_MODE:
            break;
        case SPI_SLAVE_CTL_S_MODE:
            break;
        case SPI_SLAVE_CTL_G_BITLEN:
            break;
        case SPI_SLAVE_CTL_S_BITLEN:
            break;
        case SPI_SLAVE_CTL_S_TIMEOUT:
            __spi_slave_bus.phy_fun->time_out (*arg);
            break;
        default:
            break;
    }

    return  ret_val;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/*  End Of File */
