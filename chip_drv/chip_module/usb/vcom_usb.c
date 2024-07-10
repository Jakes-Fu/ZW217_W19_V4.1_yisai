/******************************************************************************
 ** File Name:    usb.c                                                     *
 ** Author:       weihua.wang                                                 *
 ** DATE:         3/25/2005                                                   *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:  This file is usb driver file .It include all driver funciton*
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 3/25/2005     weihua.wang     Create.                                     *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "clock_drvapi.h"
#include "usb_drv.h"
#include "vcom_usb.h"
#include "ref_outport.h"
#include "Auto_download.h"
#ifdef PLATFORM_SC6800H
#include "v0/usb_reg_v0.h"
//#include "../int/v0/int_reg_v0.h"
#endif
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
#define DATA_OUT            (1)
#define DATA_IN             (0)

#define USB_TX_LIMIT        (0x40 )
#define USB_RX_LIMIT        (0x100)
#define TABLE_MAX_NUM       (0x40 )
#define RESERVED_SIZE       (0x04 )

#define USB_SEND_MAX_TIME   (1000)

#define USB_TX_SUCCESS      (0x0)
#define USB_TX_EMPTY        (0x1) 
#define USB_TX_TIMEOUT      (0x2) 

#define DWORD_ENDIAN_SWAP(DWord) \
    (((DWord & 0x0FF00)<< 8)|((DWord & 0x0FF)<<24) |\
     ((DWord & 0xFF000000)>>24)|((DWord & 0xFF0000)>>8))


PUBLIC BOOLEAN USB_FIFODWordBufEndianSwap (uint32 *src ,uint32 *des ,uint32 len)
{
    memcpy (des, src, len<<2);
    
    return TRUE ;
}

PUBLIC void USB_LDO_Control (BOOLEAN flag)
{
  uint32 ldo_reg_val = 0;
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    if (flag)
    {
	 ldo_reg_val = ANA_REG_GET(ANA_LDO_USB_REG0);
     ldo_reg_val  &= ~LDO_USB_PD;
	 ANA_REG_SET(ANA_LDO_USB_REG0, ldo_reg_val);

    }
    else
    {
	 ldo_reg_val = ANA_REG_GET(ANA_LDO_USB_REG0);
     ldo_reg_val  |= LDO_USB_PD;
	 ANA_REG_SET(ANA_LDO_USB_REG0, ldo_reg_val);
    }
#else
    if (flag)
    {
	 ldo_reg_val = ANA_REG_GET(ANA_LDO_PD_SET1);
     ldo_reg_val  &= ~BIT_4;
	 ANA_REG_SET(ANA_LDO_PD_SET1, ldo_reg_val);

	 ldo_reg_val = ANA_REG_GET(ANA_LDO_PD_RST1);
     ldo_reg_val  |= BIT_4;
	 ANA_REG_SET(ANA_LDO_PD_RST1, ldo_reg_val);
  
    }
    else
    {
	 ldo_reg_val = ANA_REG_GET(ANA_LDO_PD_RST1);
     ldo_reg_val  &= ~BIT_4;
	 ANA_REG_SET(ANA_LDO_PD_RST1, ldo_reg_val);

	 ldo_reg_val = ANA_REG_GET(ANA_LDO_PD_SET1);
     ldo_reg_val  |= BIT_4;
	 ANA_REG_SET(ANA_LDO_PD_SET1, ldo_reg_val);
    }
#endif
}


	
PUBLIC void USB_EPxDataOutCtrl (BOOLEAN ctrl,uint32 ep_id)
{
    USB_EPx_REG_T *ctrl_ep2 = (USB_EPx_REG_T *) USB_EP2_BASE ;
    USB_EPx_REG_T *ctrl_ep4 = (USB_EPx_REG_T *) USB_EP4_BASE ;
    USB_EP0_REG_T *ctrl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;

    switch (ep_id)
    {
        case USB_EP0_OUT:
            ctrl_ep0->ctrl.mBits.buf_ready  = ctrl;
            break;
        case USB_EP2:
            ctrl_ep2->ctrl.mBits.buf_ready  = ctrl;
            break;
        case USB_EP4:
            ctrl_ep4->ctrl.mBits.buf_ready  = ctrl;
            break;
        default:
            break ;
    }
}
PUBLIC void USB_EPxDataInCtrl (BOOLEAN ctrl,uint32 ep_id)
{
    USB_EPx_REG_T *ctrl_ep1 = (USB_EPx_REG_T *) USB_EP1_BASE ;
    USB_EPx_REG_T *ctrl_ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;
    USB_EP0_REG_T *ctrl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;

    switch (ep_id)
    {
        case USB_EP0_IN:
            ctrl_ep0->ctrl.mBits.data_ready = ctrl;
            break;
        case USB_EP1:
            ctrl_ep1->ctrl.mBits.data_ready = ctrl;
            break;
        case USB_EP3:
            ctrl_ep3->ctrl.mBits.data_ready = ctrl;
            break;
        default:
            break ;
    }
}
PUBLIC uint32 USB_FIFODWordEndianSwap (uint32 DWord)
{
#ifdef _LITTLE_ENDIAN_DEBUG
    return DWORD_ENDIAN_SWAP (DWord);
#else
    return DWord;
#endif
}
PUBLIC void USB_SetPllDividor (void)
{
}

PUBLIC void USB_ErrorHandle (USB_ERROR_TYPE_E value)
{
    return;
}

PUBLIC void USB_Disconnect (void)
{
    //Disable USB LDO
    USB_LDO_Control (FALSE);
    //SCI_TRACE_LOW:"USB LDO disable!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,VCOM_USB_151_112_2_17_23_8_1_1486,(uint8*)"");
}

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef struct _usb_TX_buf_tag
{
    uint32  read;
    uint32  write;
    uint8   usb_buf[USB_TX_LIMIT+RESERVED_SIZE];
} USB_TX_buf_T;
typedef struct _usb_RX_buf_tag1
{
    uint32  read;
    uint32  write;
    uint8   usb_buf[USB_RX_LIMIT];
} USB_RX_buf_T;

typedef struct _usb_endp_info
{
    uint16 in_endp;
    uint16 out_endp;
} USB_Endp_T;

typedef struct _vcom_device
{
    uint32     state;
    USB_Endp_T endp_info;
    USB_RX_buf_T  rx_buf;
    USB_TX_buf_T  tx_buf;
} VCOM_DEVICE_T;

DEVICE_HANDLE s_dev_usb = SCI_NULL;

/**----------------------------------------------------------------------------*
**                         Variable Prototype                                 **
**----------------------------------------------------------------------------*/
/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

/*--------------------------- Local Data ------------------------------------*/
__align (4) LOCAL const USB_DEVICE_DESCRIPTOR_T DeviceDescrSerial =
{
    sizeof (USB_DEVICE_DESCRIPTOR_T),
    USB_DEVICE_DESCRIPTOR_TYPE,
    USB_WORD_SWAP_ENDIAN (USB_SPECIFICATION_VERSION), // USB 1.1
    0x00,
    0x00,
    0x00,
    USB_FIFO_MAX_BYTE,
    USB_WORD_SWAP_ENDIAN (USB_SPREADTRUM_VENDER_ID),  // SPREADTRUM VID
    USB_WORD_SWAP_ENDIAN (USB_SPREADTRUM_CHIP_ID), /*lint !e778 confirmed by xuepeng*/
    USB_WORD_SWAP_ENDIAN (0x0001UL),               /*lint !e778 !e572 confirmed by xuepeng*/
    0x00,0x00,0x00,0x01
};

//one interface
#define CONFIG_LEN 0x20
__align (4) LOCAL const uint8 ConfigDescrSerial[] =
{
    0x09,0x02,CONFIG_LEN,0x00,0x01,0x01,0x00,0xc0,0x32,
    0x09,0x04,0x00,0x00,0x02,0xff,0x00,0x00,0x00,
    0x07,0x05,0x83,0x02,0x40,0x00,0x00, //Ep 3 In
    0x07,0x05,0x04,0x02,0x40,0x00,0x00, //Ep 4 OUT
};
__align (4) const uint8 AutoDL_ConfigDescrSerial[] =
{
    0x09,0x02,CONFIG_LEN,0x00,0x01,0x01,0x00,0xc0,0x32,
    0x09,0x04,0x00,0x00,0x02,0xff,0x00,0x00,0x00,
    0x07,0x05,0x83,0x02,0x40,0x00,0x00, //Ep 3 In
    0x07,0x05,0x02,0x02,0x40,0x00,0x00, //Ep 4 OUT
};
#if 0
//two interface
#define CONFIG_LEN 0x37
__align (4) LOCAL const uint8 ConfigDescrSerial[] =
{
    //two interface
    0x09,0x02,0x37,0x00,0x02,0x01,0x00,0xc0,0x32,

    //interface 1
    0x09,0x04,0x00,0x00,0x02,0xff,0x00,0x00,0x00,
    0x07,0x05,0x83,0x02,0x40,0x00,0x00, //Ep 1 In
    0x07,0x05,0x04,0x02,0x40,0x00,0x00, //Ep 2 OUT

    //interface 2
    0x09,0x04,0x01,0x00,0x02,0xff,0x00,0x00,0x00,
    0x07,0x05,0x81,0x02,0x40,0x00,0x00, //Ep 3 In
    0x07,0x05,0x02,0x02,0x40,0x00,0x00, //Ep 4 OUT
};
#endif

__align (4) LOCAL const char VenderRespond[] =
{
    0x00,0x00,0x00,0x00
};

LOCAL uint8 class_buf[16];

/*--------------------------- Global Data -----------------------------------*/
/*--------------------------- External Data ---------------------------------*/
LOCAL VCOM_DEVICE_T usb_dev_ctl;
LOCAL uint8 usb_ep_buf[USB_FIFO_MAX_BYTE];
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL uint32 USB_EPxSendData (uint8 ep_id ,uint32 *pBuf,uint16 len);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL uint32 USB_EPxReceiveData (uint8 ep_id ,uint32 *pBuf,uint16 len); /*lint !e401 */

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN USB_EP0Config (void)
{
    USB_EP0_REG_T *ctl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;

    //Set max packet size is 8 Byte ;
    ctl_ep0->ctrl.mBits.pack_size     = 0x8 ;

    ctl_ep0->int_clr.dwValue          = 0x3fff ;
    ctl_ep0->int_ctrl.mBits.setup_end = 1 ;

    //Set buffer ready ;
    USB_EPxDataOutCtrl (TRUE,USB_EP0_OUT);

    return TRUE;
}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN USB_EP1Config (void)
{
    USB_EPx_REG_T *ctl_ep1 = (USB_EPx_REG_T *) USB_EP1_BASE ;

    ctl_ep1->ctrl.mBits.pack_size        = 0x40   ;
    ctl_ep1->transfer_size.mBits.num     = 0x40 ;

    ctl_ep1->ctrl.mBits.type             = 0 ;    // Bulk endpoint

    ctl_ep1->ctrl.mBits.ep_en            = 1 ;

    ctl_ep1->int_clr.dwValue             = 0x3fff ;
    ctl_ep1->int_ctrl.mBits.transfer_end = 1 ;

    USB_EPxDataInCtrl (FALSE,USB_EP1);   //disable data ready ;

    return TRUE;
}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN USB_EP2Config (void)
{
    USB_EPx_REG_T *ctl_ep2 = (USB_EPx_REG_T *) USB_EP2_BASE ;

    ctl_ep2->ctrl.mBits.pack_size        = 0x40 ;
    ctl_ep2->transfer_size.mBits.num     = 0x40 ;

    ctl_ep2->ctrl.mBits.ep_en            = 1 ;      // Enable endp2

    ctl_ep2->ctrl.mBits.type             = 0 ;       // bulk endpoint

    ctl_ep2->int_clr.dwValue             = 0x3fff ; // Clear all int sts
    ctl_ep2->int_ctrl.mBits.transfer_end = 1 ;      // Enable interrupt

    USB_EPxDataOutCtrl (TRUE,USB_EP2);              //Set buffer ready ;

    return TRUE;
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN USB_EP3Config (void)
{
    USB_EPx_REG_T *ctl_ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;

    ctl_ep3->ctrl.mBits.pack_size        = 0x40 ;
    ctl_ep3->transfer_size.mBits.num     = 0x40 ;

    ctl_ep3->ctrl.mBits.ep_en            = 1 ;

    ctl_ep3->ctrl.mBits.type             = 0 ;     // Bulk endpoint

    ctl_ep3->int_clr.dwValue             = 0x3fff ;
    ctl_ep3->int_ctrl.mBits.transfer_end = 1 ;

    USB_EPxDataInCtrl (FALSE,USB_EP3);             //disable data ready ;

    return TRUE;
}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN USB_EP4Config (void)
{
    USB_EPx_REG_T *ctl_ep4 = (USB_EPx_REG_T *) USB_EP4_BASE ;

    ctl_ep4->ctrl.mBits.pack_size        = 0x40 ;
    ctl_ep4->transfer_size.mBits.num     = 0x40 ;

    ctl_ep4->ctrl.mBits.ep_en            = 1 ;  //Enable ep4

    ctl_ep4->ctrl.mBits.type             = 0 ;  //bulk endpoint

    ctl_ep4->int_clr.dwValue             = 0x3fff;  //Clear
    ctl_ep4->int_ctrl.mBits.transfer_end = 1 ;  //Enable tranaction end int

    USB_EPxDataOutCtrl (TRUE,USB_EP4);          //Set buffer ready ;

    return TRUE;
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL void USB_GetDevDescriptor (uint8 bDirection,uint16 wValue,uint16 wIndex ,uint16 wLen)
{
    volatile USB_HWORD_VALUE_U temp;
    uint16 wResidue = 0;
    uint8 *pData = 0;

    temp.wValue  = wValue;

    switch (temp.bmDescriptor.type)
    {
        case USB_DEVICE_DESCRIPTOR_TYPE:

            if (wLen >= 0x040)
            {
                wResidue = 18;
            }
            else
            {
                wResidue = wLen;
            }

            pData    = (uint8 *) &DeviceDescrSerial;
            break;
        case USB_CONFIGURATION_DESCRIPTOR_TYPE:

            if (wLen >= 0x0ff)
            {
                wResidue = CONFIG_LEN;
            }
            else
            {
                wResidue = wLen;
            }

            if (AutoDL_IsDloaderMode())  //autodownload
            {
                pData = (uint8 *) &AutoDL_ConfigDescrSerial[0]; //vcom

            }
            else
            {
                pData    = (uint8 *) &ConfigDescrSerial;
            }

            break;
        case USB_STRING_DESCRIPTOR_TYPE:
            break;
        default:
            break;
    }

    //Write data to EP in & send data to host  ;
    USB_EPxSendData (USB_EP0_IN, (uint32 *) pData,wResidue);
}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL void USB_VendorCommand (uint8 bDirection,uint16 wValue,uint16 wIndex ,uint16 wLen)
{
    char *pData = 0;

    if (bDirection)
    {
        pData    = (char *) &VenderRespond;
        USB_EPxSendData (USB_EP0_IN, (uint32 *) pData,wLen);
    }
    else
    {
        pData    = (char *) &VenderRespond;
        USB_EPxReceiveData (USB_EP0_OUT, (uint32 *) pData,NULL);
    }
}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL void USB_ClassCommand (uint8 bDirection,uint16 wValue,uint16 wIndex ,uint16 wLen)
{
    USB_EP0_REG_T *ep0_ctrl = (USB_EP0_REG_T *) USB_EP0_BASE ;
    USB_HWORD_VALUE_U temp;


    if (bDirection)
    {
        USB_EPxSendData (USB_EP0_IN, (uint32 *) class_buf,wLen);
    }
    else
    {
        USB_EPxReceiveData (USB_EP0_OUT, (uint32 *) class_buf,wLen);
    }

    if (ep0_ctrl->setup_low.mBits.request == 0x22)
    {
        temp.wValue  = wValue;

        if ( (temp.bmDescriptor.type) != 0) //21 22 01 02(03 00)(02 00)
        {
            //virtual com is opened
            usb_dev_ctl.state = VCOM_DEV_READY;
        }
        else         //21 22 00 00
        {
            //virtual com was enumerated, but not open yet
            usb_dev_ctl.state = VCOM_DEV_PRESENT;
        }
    }

}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
//LOCAL void USB_SetAddress (uint8 bDirection,uint16 wValue,uint16 wIndex ,uint16 wLen)
//{
//  USB_EPxSendData (USB_EP0_IN,(uint32 *)class_buf,NULL);
//}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL void data_proc (uint32 *src ,uint32 *des ,uint32 direct ,uint32 len ,uint32 ep_id)
{
    uint32 i;

    if (direct)
    {
        for (i = 0; i< (len); i++)
        {
            * (des + i) = USB_FIFODWordEndianSwap (REG32 (src));

        }
    }
    else
    {
        //Send datat to FIFO ;
        for (i = 0; i < (len); i++)
        {

            REG32 (des) = USB_FIFODWordEndianSwap (* (src + i));

        }
    }
}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL uint32 USB_EPxSendData (uint8 ep_id ,uint32 *pBuf,uint16 len)
{
    USB_DEV_REG_T *ctrl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;
    uint32 size = ( (len + 3) >>2) & 0x3FF ;
    uint32 old_tick, new_tick;

    old_tick = new_tick = SCI_GetTickCount();

    switch (ep_id)
    {
        case USB_EP0_IN:
            {
                USB_EP0_REG_T *ctl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
                ctl_ep0->ctrl.mBits.pack_size       = len ;
                ctl_ep0->transfer_size_in.mBits.num = len ;

                ctrl_usb->ep_rest.mBits.ep0_in_fifo_rst = TRUE;

                data_proc (pBuf , (uint32 *) USB_EP0_IN_FIFO, DATA_IN,size,USB_EP0_IN);
                ctl_ep0->ctrl.mBits.data_ready      = 1 ;    //Set data ready ;
            }
            break;
        case USB_EP1:
            {
                USB_EPx_REG_T *ctl_ep1 = (USB_EPx_REG_T *) USB_EP1_BASE ;

                ctl_ep1->ctrl.mBits.pack_size    = len ;
                ctl_ep1->transfer_size.mBits.num = len ;

                //Clear fifo
                ctrl_usb->ep_rest.mBits.ep1_fifo_rst = TRUE;

                data_proc (pBuf , (uint32 *) USB_EP1_FIFO, DATA_IN,size,USB_EP1);
                ctl_ep1->ctrl.mBits.data_ready      = 1 ;    //Set data ready ;
                ctl_ep1->int_ctrl.mBits.transfer_end = 1;
                while (! (ctl_ep1->int_sts.mBits.transfer_end))
                {
                    new_tick = SCI_GetTickCount();

                    if ( (new_tick - old_tick) > USB_SEND_MAX_TIME)
                    {   
                        /*return zeor, to indicate upper layer, the transfer is abnormal*/
                        size = 0;
                        break;
                    }
                }

                //clear the int status of ep1's tranfer end
                ctl_ep1->int_clr.mBits.transfer_end = 1;
            }
            break;
        case USB_EP3:
            {
                USB_EPx_REG_T *ctl_ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;
                ctl_ep3->ctrl.mBits.pack_size           = len ;
                ctl_ep3->transfer_size.mBits.num    = len ;

                //Clear fifo
                ctrl_usb->ep_rest.mBits.ep3_fifo_rst = TRUE;

                data_proc (pBuf , (uint32 *) USB_EP3_FIFO, DATA_IN,size,USB_EP3);
		  ctl_ep3->int_ctrl.mBits.transfer_end = 1;
                ctl_ep3->ctrl.mBits.data_ready      = 1 ;    //Set data ready ;

                while (! (ctl_ep3->int_sts.mBits.transfer_end))
                {
                    new_tick = SCI_GetTickCount();

                    if ( (new_tick - old_tick) > USB_SEND_MAX_TIME)
                    {
                        /*return zeor, to indicate upper layer, the transfer is abnormal*/
                        size = 0;
                        break;
                    }
                }

                //clear the int status of ep3's tranfer end
                ctl_ep3->int_clr.mBits.transfer_end = 1;


            }
            break;
        default:
            break ;
    }

    return size;
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL uint32 USB_EPxReceiveData (uint8 ep_id ,uint32 *pBuf,uint16 len)
{
    volatile int size = ( (len + 3) >>2) & 0xFF ;

    switch (ep_id)
    {
        case USB_EP0_OUT:
            {
                USB_EP0_REG_T *ctl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
                data_proc ( (uint32 *) USB_EP0_OUT_FIFO, (uint32 *) pBuf , DATA_OUT,size,USB_EP0_OUT);
                ctl_ep0->ctrl.mBits.buf_ready = 1 ;  //Set buffer ready ;
            }
            break;
        case USB_EP2:
            {
                USB_EPx_REG_T *ctl_ep2 = (USB_EPx_REG_T *) USB_EP2_BASE ;
                data_proc ( (uint32 *) USB_EP2_FIFO, (uint32 *) pBuf , DATA_OUT,size,USB_EP2);
                ctl_ep2->ctrl.mBits.buf_ready = 1 ;  //Set buffer ready ;
            }
            break;
        case USB_EP4:
            {
                USB_EPx_REG_T *ctl_ep4 = (USB_EPx_REG_T *) USB_EP4_BASE ;
                data_proc ( (uint32 *) USB_EP4_FIFO, (uint32 *) pBuf , DATA_OUT,size,USB_EP4);
                ctl_ep4->ctrl.mBits.buf_ready = 1 ;  //Set buffer ready ;
            }
            break;
        default:
            break ;
    }

    return size;
}

/*****************************************************************************/
//  Description:    write to usb inendp fifo without interrupt
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:           called by assert print
//                  param1: endp number
//                  param2: write data buffer
//                  param3: write length
/*****************************************************************************/
LOCAL int32 USB_WritetoEndp (uint8 ep_num,uint8 *write_buf,uint32 send_len)
{
    USB_EPx_REG_T *ctl_epx = NULL ;
    uint32 i;

    if (send_len == 0)
    {
        return USB_TX_EMPTY;
    }

    ctl_epx = (USB_EPx_REG_T *) (USB_EP0_BASE + ep_num * 0x40);

    //Clear transfer end int_en bit
    ctl_epx->int_clr.mBits.transfer_end = 1 ;

    //Epx max packsize is 64 bytes .
    if ( (send_len >> 6) > 0)
    {
        for (i=0; i< (send_len >> 6); i++)
        {
            if(0 == USB_EPxSendData (ep_num , (uint32 *) (write_buf + (i<<6)),USB_FIFO_MAX_BYTE))
            {
                return USB_TX_TIMEOUT;
            }
        }

        if (send_len % (uint32) USB_FIFO_MAX_BYTE)
        {
            if(0 == USB_EPxSendData (ep_num , (uint32 *) (write_buf + (i<<6)), (send_len - (i<<6))))
            {
                return USB_TX_TIMEOUT;
            }
        }

    }
    else
    {
        if(0 == USB_EPxSendData (ep_num , (uint32 *) write_buf,send_len))
        {
            return USB_TX_TIMEOUT;
        }
    }

    return USB_TX_SUCCESS;

}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL void USB_Ep0_handler (void)
{
    USB_EP0_REG_T *ep0_ctrl = (USB_EP0_REG_T *) USB_EP0_BASE ;
    USB_EPx_INT_U status ;

    status.dwValue = ep0_ctrl->int_sts.dwValue ;

    if (status.mBits.setup_end)
    {
        switch (ep0_ctrl->setup_low.mBits.type)
        {
            case USB_REQ_STANDARD:

                switch (ep0_ctrl->setup_low.mBits.recipient)
                {
                    case USB_REC_DEVICE:
                        USB_GetDevDescriptor (\
                                              (ep0_ctrl->setup_low.mBits.direction),\
                                              (ep0_ctrl->setup_low.mBits.value),\
                                              (ep0_ctrl->setup_high.mBits.index),\
                                              (ep0_ctrl->setup_high.mBits.length));
                        break;
                    case USB_REC_INTERFACE:
                        break;
                    default :
                        break;
                }

                break;
            case USB_REQ_CLASS:

                if (USB_REC_INTERFACE == ep0_ctrl->setup_low.mBits.recipient)
                {
                    switch (ep0_ctrl->setup_low.mBits.request)
                    {
                        case 0x20:
                        case 0x21:
                        case 0x22:
                            USB_ClassCommand (\
                                              (ep0_ctrl->setup_low.mBits.direction),\
                                              (ep0_ctrl->setup_low.mBits.value),\
                                              (ep0_ctrl->setup_high.mBits.index),\
                                              (ep0_ctrl->setup_high.mBits.length));
                            break;
                        default :
                            break;
                    }
                }

                break;
            case USB_REQ_VENDOR:

                if (USB_REC_DEVICE == ep0_ctrl->setup_low.mBits.recipient)
                {
                    switch (ep0_ctrl->setup_low.mBits.request)
                    {
                        case 0x01:
                        case 0x03:
                            USB_VendorCommand (\
                                               (ep0_ctrl->setup_low.mBits.direction),\
                                               (ep0_ctrl->setup_low.mBits.value),\
                                               (ep0_ctrl->setup_high.mBits.index),\
                                               (ep0_ctrl->setup_high.mBits.length));
                            break;
                        default :
                            break;
                    }
                }

                break;
            default :
                break;
        }
    }

    ep0_ctrl->int_clr.dwValue =USB_INT_EPx_MASK;

}
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL void USB_Ep1_handler (void)
{
    USB_EPx_REG_T *ctl = (USB_EPx_REG_T *) USB_EP1_BASE ;
    USB_EPx_INT_U status ;

    status.dwValue = ctl->int_sts.dwValue ;

    if (status.mBits.transaction_end)
    {
        ctl->int_clr.mBits.transaction_end = 1;
    }

    ctl->int_clr.dwValue = USB_INT_EPx_MASK; //Clear all interrupt ;
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL void USB_Ep2_handler (void)
{
    USB_EPx_REG_T *ctl_ep2 = (USB_EPx_REG_T *) USB_EP2_BASE ;

    USB_EPx_INT_U status ;
    volatile uint16 num = 0;
    volatile int32 i ;

    status.dwValue = ctl_ep2->int_sts.dwValue ;

    if (status.mBits.transfer_end)
    {

        num = ctl_ep2->ctrl.mBits.p_num ;

        USB_EPxReceiveData (USB_EP2, (uint32 *) usb_ep_buf,num);

        ctl_ep2->ctrl.mBits.buf_ready   = 1;        //Set data ready ;

        for (i=0; i<num; i++)
        {
            usb_dev_ctl.rx_buf.usb_buf[usb_dev_ctl.rx_buf.write] = usb_ep_buf[i];

            usb_dev_ctl.rx_buf.write ++ ;

            if (usb_dev_ctl.rx_buf.write >= USB_RX_LIMIT)
            {
                usb_dev_ctl.rx_buf.write = 0;
            }
        }
    }

    ctl_ep2->int_clr.dwValue = USB_INT_EPx_MASK;    //Clear all interrupt ;
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL void USB_Ep3_handler (void)
{
    USB_EPx_REG_T *ctl_ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;
    USB_EPx_INT_U status ;

    status.dwValue = ctl_ep3->int_sts.dwValue ;
    ctl_ep3->int_clr.dwValue = USB_INT_EPx_MASK; //Clear all interrupt ;
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL void USB_Ep4_handler (void)
{
    USB_EPx_REG_T *ctl_ep4 = (USB_EPx_REG_T *) USB_EP4_BASE ;

    USB_EPx_INT_U status ;
    volatile uint16 num = 0;
    volatile int32 i ;

    status.dwValue = ctl_ep4->int_sts.dwValue ;

    if (status.mBits.transfer_end)
    {

        num = ctl_ep4->ctrl.mBits.p_num ;

        USB_EPxReceiveData (USB_EP4, (uint32 *) usb_ep_buf,num);

        ctl_ep4->ctrl.mBits.buf_ready   = 1;        //Set data ready ;

        for (i=0; i<num; i++)
        {
            usb_dev_ctl.rx_buf.usb_buf[usb_dev_ctl.rx_buf.write] = usb_ep_buf[i];

            usb_dev_ctl.rx_buf.write ++ ;

            if (usb_dev_ctl.rx_buf.write >= USB_RX_LIMIT)
            {
                usb_dev_ctl.rx_buf.write = 0;
            }
        }
    }

    ctl_ep4->int_clr.dwValue = USB_INT_EPx_MASK;    //Clear all interrupt ;
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL void USB_init (void)
{
    USB_DEV_REG_T    *ctl_usb = (USB_DEV_REG_T *) USB_REG_BASE;

    //Disable USB LDO
    USB_LDO_Control (FALSE);

#if defined (PLATFORM_SC6800H)
    //lint -e(516) -e(718) -e(746)
    REG32 (AHB_CTL0 )  |= BIT_2;  /*important, usb clk en should be set ahead of other usb register, esspecially wdgreset assert*/
    REG32 (GR_ANA_CTL) |= BIT_27; //USB_PURES_EN
#elif defined (PLATFORM_SC6530)
    //lint -e(516) -e(718) -e(746)
    CHIP_REG_SET(AHB_EB0_SET, USBD_EB_SET);  /*important, usb clk en should be set ahead of other usb register, esspecially wdgreset assert*/
#ifdef CHIP_VER_6531
    CHIP_REG_OR (APB_MISC_CTL0,USB_1K5_EN);
#else
    CHIP_REG_AND (APB_MISC_CTL0,~BIT_6);
#endif
#ifdef NEED_MODIFY
    REG32 (GR_ANA_CTL) |= BIT_27; //USB_PURES_EN
#endif
#elif defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
	CHIP_REG_SET(AHB_EB0_SET, USBD_EB_SET);
	CHIP_REG_OR (APB_MISC_CTL0,USB_1K5_EN);
#ifdef NEED_MODIFY
    REG32 (GR_ANA_CTL) |= BIT_27; //USB_PURES_EN
#endif
#endif

    // Reset usb device
    USB_SetPllDividor();
    DEVICE_Reset (s_dev_usb);

    OS_TickDelay (300);

    //Set USB to self power;
    ctl_usb->ctrl.mBits.power = TRUE;

    //Enable USB device ;
    ctl_usb->ctrl.mBits.en      = TRUE;

    USB_EP0Config ();
    USB_EP1Config ();
    USB_EP2Config ();
    USB_EP3Config ();
    USB_EP4Config ();

    //Enalbe USB LDO  ;
    USB_LDO_Control (TRUE);
}

/*****************************************************************************/
//  Description:    usb register init and local var init
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
PUBLIC void VCOM_usb_boot (void)
{
    SCI_ASSERT (DEVICE_Find ("USB", &s_dev_usb) == CLOCK_SUCCESS); /*assert verified*/

    /*
    local variable init start
    */
    usb_dev_ctl.state = VCOM_DEV_NO_PRESENT;
    usb_dev_ctl.tx_buf.read  = 0 ;
    usb_dev_ctl.tx_buf.write = 0 ;
    usb_dev_ctl.rx_buf.read  = 0 ;
    usb_dev_ctl.rx_buf.write = 0 ;

    usb_dev_ctl.endp_info.in_endp  = USB_EP3;
    usb_dev_ctl.endp_info.out_endp = USB_EP4;

    //Open usb device
    DEVICE_Open (s_dev_usb);

    USB_init();
}

/*****************************************************************************/
//  Description:    complete the handshake and receive data handle
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:           -1:got none from fifo, else return the received data
/*****************************************************************************/
PUBLIC int32 VCOM_GetChar (void)
{
    USB_DEV_REG_T *ctl         = (USB_DEV_REG_T *) USB_REG_BASE;
    VCOM_DEVICE_T *dev_ptr     = NULL;
    USB_DEV_INT_STS_U status ;
    int32 ch = -1;

    dev_ptr = &usb_dev_ctl;
    ch      = -1;

    do
    {

        if (dev_ptr->rx_buf.read != dev_ptr->rx_buf.write)
        {
            ch = dev_ptr->rx_buf.usb_buf[dev_ptr->rx_buf.read];
            dev_ptr->rx_buf.read++;

            if (dev_ptr->rx_buf.read >= USB_RX_LIMIT)
            {
                dev_ptr->rx_buf.read = 0;
            }

            return ch ;
        }

        //usb int handler
        #if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
        if (REG32(INT1_IRQ_RAW_STS) & (BIT_5))
        #elif 1 //defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A)
        if (INT_PHY_GetIrqIdStatus(TB_USB_INT))    /*ÔÝÊ±´ò×®*/
        #else
        if (REG32(INT_IRQ_RAW_STS) & (1<<TB_USB_INT))
		#endif
        {
            status.dwValue = ctl->int_sts.dwValue ;

            //dispatch handler message according to usb int status ;
            if (status.dwValue  & USB_INT_EPx_MASK)
            {
                if (status.mBits.ep0)
                {
                    USB_Ep0_handler();
                }

                if (status.mBits.ep2)
                {
                    USB_Ep2_handler();
                }

                if (status.mBits.ep4)
                {
                    USB_Ep4_handler();
                }

                if (status.mBits.ep1)
                {
                    USB_Ep1_handler();
                }

                if (status.mBits.ep3)
                {
                    USB_Ep3_handler();
                }
            }
        }

    }
    while (0);

    return ch ;
}

/*****************************************************************************/
//  Description:    pull down usb ldo
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
PUBLIC void VCOM_disconnect (void)
{
    //Disable USB LDO
    USB_LDO_Control (FALSE);
}

/*****************************************************************************/
//  Description:    put char to fifo in loop mode
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :       uint8 nchar: char to be sent
//  Note:           when this func is called, it's indicated that current irs is disable
//                  , no interrupt can happen,
/*****************************************************************************/
PUBLIC void VCOM_PutTxChar (uint8 nchar)
{
    VCOM_DEVICE_T *dev_ptr;
    int32 ret_val;

    dev_ptr = &usb_dev_ctl;

    //if virtual is not ready,then exit with doing nothing
    if (dev_ptr->state != VCOM_DEV_READY)
    {
        return;
    }

    dev_ptr->tx_buf.usb_buf[dev_ptr->tx_buf.write++] = nchar;

    if (dev_ptr->tx_buf.write >= USB_TX_LIMIT)
    {
        ret_val = USB_WritetoEndp (dev_ptr->endp_info.in_endp,
                         dev_ptr->tx_buf.usb_buf,
                         dev_ptr->tx_buf.write);
        dev_ptr->tx_buf.write = 0;
        if(USB_TX_TIMEOUT == ret_val)
        {   /*if usb tx transfer timeout, means usb abnormal or usb cable is plugout forcely
              do usb reinit*/ 
            VCOM_usb_boot();
        }
    }
}

/*****************************************************************************/
//  Description:    put chars to fifo in loop mode
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :       uint8* data_ptr: chars to be sent
//                  int32 len: total data size to be sent
//  Note:           called when dump memory only! to reduce time, 0x7e will be convert here
/*****************************************************************************/
PUBLIC void VCOM_PutTxChars (uint8* data_ptr, int32 len)
{
    int32 i, ret_val;
    VCOM_DEVICE_T *dev_ptr;
    uint8 tempval;

    dev_ptr = &usb_dev_ctl;

    if (dev_ptr->state != VCOM_DEV_READY)
    {
        return ;
    }

    for(i = 0; i < len; i++)
    {
        tempval = *(data_ptr+i);
        if ((0x7E == tempval) || (0x7D == tempval))  
        {
            dev_ptr->tx_buf.usb_buf[dev_ptr->tx_buf.write++] = (0x7D);
            dev_ptr->tx_buf.usb_buf[dev_ptr->tx_buf.write++] = ((tempval ^ 0x20));
        }
        else
        {
            dev_ptr->tx_buf.usb_buf[dev_ptr->tx_buf.write++] = tempval;
        }

        if (dev_ptr->tx_buf.write >= USB_TX_LIMIT)
        {
            ret_val = USB_WritetoEndp (dev_ptr->endp_info.in_endp,
                             dev_ptr->tx_buf.usb_buf,
                             dev_ptr->tx_buf.write);
            
            dev_ptr->tx_buf.write = 0;

            if(USB_TX_TIMEOUT == ret_val)
            {   /*if usb tx transfer timeout, means usb abnormal or usb cable is plugout forcely
                 do usb reinit*/ 
                VCOM_usb_boot();
                return; 
            }
        }
    }
}

/*****************************************************************************/
//  Description:    indicating send char will be finished, remain char should be flushed
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :
//  Note:
/*****************************************************************************/
PUBLIC void VCOM_Flush (void)
{
    VCOM_DEVICE_T *dev_ptr;
    int32 ret_val;

    dev_ptr = &usb_dev_ctl;

    //if virtual is not ready,then exit with doing nothing
    if (dev_ptr->state != VCOM_DEV_READY)
    {
        return;
    }

    if (dev_ptr->tx_buf.write)
    {
        ret_val = USB_WritetoEndp (dev_ptr->endp_info.in_endp,
                         dev_ptr->tx_buf.usb_buf,
                         dev_ptr->tx_buf.write);
        dev_ptr->tx_buf.write = 0;

        if(USB_TX_TIMEOUT == ret_val)
        {   /*if usb tx transfer timeout, means usb abnormal or usb cable is plugout forcely
              do usb reinit*/ 
            VCOM_usb_boot();
            return;  
        }
    }
}

/*****************************************************************************/
//  Description:    get usb virtual com state
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :       none
//  Note: ret value:VCOM_DEV_NO_PRESENT
//                  VCOM_DEV_PRESENT
//                  VCOM_DEV_READY
//                  VCOM_DEV_BUSY
/*****************************************************************************/
PUBLIC uint32 VCOM_GetDevState (void)
{
    return usb_dev_ctl.state;
}

/*****************************************************************************/
//  Description:    set usb virtual com state by external caller
//  Global resource dependence:
//  Author:         weihua.wang
//  Param   :       VCOM_DEV_NO_PRESENT
//                  VCOM_DEV_PRESENT
//                  VCOM_DEV_READY
//                  VCOM_DEV_BUSY
/*****************************************************************************/
PUBLIC void VCOM_SetDevState (uint32 mode)
{
    SCI_ASSERT (mode < VCOM_DEV_MAX);/*assert verified*/

    if (usb_dev_ctl.state == mode)
    {
        //curmode is equal to the mode to be set, return directly
        return;
    }

    //if setting to VCOM_DEV_NO_PRESENT, usb init
    if (VCOM_DEV_NO_PRESENT == mode)
    {
        VCOM_usb_boot();
    }

    usb_dev_ctl.state = mode;
}

PUBLIC void  VCOM_Open()
{
    USB_DEV_REG_T    *ctl_usb = (USB_DEV_REG_T *) USB_REG_BASE;

    if(ctl_usb->ctrl.mBits.en)
    {   /*when watchdog reset, usb resiger will be set to default*/
        usb_dev_ctl.state = VCOM_DEV_READY;
        usb_dev_ctl.tx_buf.read  = 0 ;
        usb_dev_ctl.tx_buf.write = 0 ;
        usb_dev_ctl.rx_buf.read  = 0 ;
        usb_dev_ctl.rx_buf.write = 0 ;
        
        if (AutoDL_IsDloaderMode()) //#ifdef AUTO_DOWNLOAD_SUPPORT
        {
            SCI_ASSERT (DEVICE_Find ("USB", &s_dev_usb) == CLOCK_SUCCESS); /*assert verified*/     
            DEVICE_Open(s_dev_usb);           
            USB_init();
            usb_dev_ctl.endp_info.in_endp  = USB_EP3;
            usb_dev_ctl.endp_info.out_endp = USB_EP2;
	        USB_EP2Config();
	        USB_EP3Config();
            return;
        }
        
        USB_EP4Config();
	    USB_EP3Config();
        usb_dev_ctl.endp_info.in_endp  = USB_EP3;
        usb_dev_ctl.endp_info.out_endp = USB_EP4;
    }
    else
    {
        VCOM_usb_boot();
    }
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
// End

