/* Copyright (C) 2022 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _MSG_DEV_H_
#define _MSG_DEV_H_

//#include "usb_device_uws6121e.h"
#include "usb_composite_device.h"
#include <stdint.h>
#include <stddef.h>

typedef struct msgRxData
{
    uint32_t Signature;
    uint32_t Tag;
    uint32_t XferLen;
    uint8_t Dir;
    uint8_t Lun;
    uint8_t CBLen;
    uint8_t Param[16]; //max 6 ?
} msgRxData_t;  //CBW

struct msgCSWData
{
    uint32_t Signature;
    uint32_t Tag;
    uint32_t CSWDataResidue;
    uint8_t CSWStatus;
} __attribute__((packed));

typedef struct msgCSWData msgCSWData_t;
/* DataType */
#define CBW 0x43425355
#define CSW 0x53425355

#define MSG_REQUESTSENSE 0x03
#define MSG_INQUIRY 0x12
#define MSG_READFORMATCAPACITIES 0x23
#define MSG_READCAPACITY 0x25
#define MSG_MODESENSE6 0x1A
#define MSG_READ10 0x28
#define MSG_WRITE10 0x2A
#define MSG_TESTUNITREADY 0x00
#define MSG_GETEVENTSTATUSNOTIFI 0x4A
#define MSG_GETCONFIGURATION 0x46
#define MSG_PREVENTALLWMDMRMVL 0x1E

typedef struct usb_msg_dev
{
    usbEp_t *epin;
    usbEp_t *epout;
    copsFunc_t *func;
    void *priv;
    uint32_t sdio_type;
} usbMsgDev_t;

typedef enum {
    USB_UMS_TX_COMPLET, 
    USB_UMS_RX_COMPLET,
    USB_UMS_MAX_MSG
} USB_UMS_MSG_E;

/**
 * \brief bind msg device
 */
bool usbMsgDevBind(usbMsgDev_t *msg);

/**
 * \brief unbind msg device
 */
void usbMsgDevUnbind(usbMsgDev_t *msg);

/**
 * \brief msg device start schedule
 */
bool usbMsgDevStart(usbMsgDev_t *msg);

/**
 * \brief msg device shutdown
 */
void usbMsgDevStop(usbMsgDev_t *msg);

#endif
