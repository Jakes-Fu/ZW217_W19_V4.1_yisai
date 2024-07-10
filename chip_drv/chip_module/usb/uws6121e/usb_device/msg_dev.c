/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
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


#include "msg_dev.h"
//#include "block_device.h"
//#include "tflash_block_device.h"
#include "osi_compiler.h"
//#include "drv_emmc.h"
//#include "vfs.h"
#include <stdlib.h>
#include <stdio.h>
//#include <sys/errno.h>
#include "usbservice_api.h"
#include "priority_system.h"
//#include "memdev.h"
#include "bsd.h"
#include "umss_common.h"

#define MSG_DEV_THREAD_STACK_SIZE (8 * 1024)
#define MSG_DEV_THREAD_PRIORITY  32 //(PRI_USB_SERVICE-5) //63
#define MSG_DATA_BUF_MAX (512)
#define MSG_IN_DATA_BUF_MAX (4096)
#define TCARD_SECTOR_SIZE (512)
#define TCARD_TX_SECTOR_ONCE_COUNT (8)

BLOCK_ID   ums_task_id = 0xffff;
uint32 write10_length = 0;
uint32 read10_length = 0;


enum msg_state
{
    x_idle = 0,
    x_data,
    x_csw,
};
	
typedef struct msg_dev_priv
{
    UMSS_DEV_T * umssDev;
    usbXfer_t *tx_xfer;
    usbXfer_t *rx_xfer;
    usbMsgDev_t *msg;
    msgRxData_t *dummy_data;
    //uint32_t LogicBlocAddr[UMSS_MAX_DISK_NUM];
	//uint32 tx_DataResidue;
    enum msg_state tx_state;
    enum msg_state rx_state;
	uint8 udisk_num;
	bool tx_sector_done;
    bool rx_sector_done;
	void *xfer_buf;

} msgDevPriv_t;

msgDevPriv_t msgDev;

struct ReqSenseData
{
	uint8_t RespCode;
	uint8_t Obs;
	uint8_t SenseKey;
	uint8_t Information[4];
	uint8_t AddSenseLen;
	uint8_t CmdSpecInfo[4];
	uint8_t AddSenseCode;
	uint8_t AddSnsCodeQlfr;
	uint8_t FldReplUnitCode;
	uint8_t Sksv;
	uint8_t SenseKeySpec[2];
};
struct InquiryData
{
	uint8_t PerDevType;
	uint8_t Rem;
	uint8_t Version;
	uint8_t RespDataFormat;
	uint8_t AddLen;
	uint8_t Resvered[3];
	char VendInfo[UMSS_SCSI_VENDER_STR_LEN];
	char ProductIden[UMSS_SCSI_PRODUCT_ID_STR_LEN];
	char ProductRevision[4];
};
struct FormatCapac
{
	uint32_t CapListHeader;
	uint32_t BlockNumber;
	uint32_t BlockLen;
	uint32_t BlockNumber1;
	uint32_t BlockLen1;
};
struct CapacData
{
	uint32_t LBA;	   // logic block address
	uint32_t BlockLen; //Block length
};

static bool prvMsgRxStart(msgDevPriv_t *p)
{
	int r;
    //USB_LOG_TRACE("msg prepare rx buffer\n");
    r = udcEpQueue(p->msg->func->controller, p->msg->epout, p->rx_xfer);
    if (r != 0)
    {
        USB_LOG_TRACE("msg rx start fail %d\n", r);
        return false;
    }
    return true;
}
struct ModeSenseData
{
	uint8_t MDLen;
	uint8_t MediumTyp;
	uint8_t DevSpec;
	uint8_t BlockDescLen;
};

static bool prvReqSense(msgDevPriv_t *p, const msgRxData_t *d)
{
	struct ReqSenseData *r;
    int result;

    uint8_t Information[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t CmdSpecInfo[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t SenseKeySpec[2] = {0x00, 0x00};
	p->tx_state = x_data;
    r = p->tx_xfer->buf;
    //uint32_t critical = osiEnterCritical();
    r->RespCode = 0x70;
    r->Obs = 0x00;
    r->SenseKey = 0x05; //0x05:ILLEGAL REQUEST; 0x02:NOT READY;
    r->AddSenseLen = 0x0A;
    r->AddSenseCode = 0x20;
    r->AddSnsCodeQlfr = 0x00;
    r->FldReplUnitCode = 0x00;
    r->Sksv = 0x00;
    memcpy(r->Information, Information, sizeof(Information));
    memcpy(r->CmdSpecInfo, CmdSpecInfo, sizeof(CmdSpecInfo));
    memcpy(r->SenseKeySpec, SenseKeySpec, sizeof(SenseKeySpec));
    p->tx_xfer->length = sizeof(struct ReqSenseData);
    //osiExitCritical(critical);

    result = udcEpQueue(p->msg->func->controller, p->msg->epin, p->tx_xfer);
    if (result < 0)
    {
        USB_LOG_TRACE("msg %x data fail %d\n", d->Param[0], result);
    }
    return result == 0;
}
static bool prvInquiryData(msgDevPriv_t *p, const msgRxData_t *d)
{
    uint8_t Resvered[3] = {0x00, 0x00, 0x00};
	struct InquiryData *r;
	int result;
    char VendInfo[8] = {'U', 'n', 'i', 's', 'o', 'c', ' ', ' '};
    char ProductIden[16] = {'S', 't', 'o', 'r', 'a', 'g', 'e', ' ', 'D', 'e', 'v', 'i', 'c', 'e', ' ', ' '};
    char ProductRevision[4] = {'1', '.', '0', '0'};

	//char *VendInfo = (char *)p->umssDev[d->Lun].vendor;
    //char *ProductIden= (char *)p->umssDev[d->Lun].product;
	p->tx_state = x_data;
    r = p->tx_xfer->buf;
    //uint32_t critical = osiEnterCritical();
    r->PerDevType = 0x00; //Direct storage device, eg U disk:00h      CD:05h
    r->Rem = 0x80;        //Removable device
    r->Version = 0x00;
    r->RespDataFormat = 0x01;
    r->AddLen = 0x1f;
    memcpy(r->Resvered, Resvered, sizeof(Resvered));
    memcpy(r->VendInfo, VendInfo, UMSS_SCSI_VENDER_STR_LEN);
    memcpy(r->ProductIden, ProductIden, UMSS_SCSI_PRODUCT_ID_STR_LEN);
    memcpy(r->ProductRevision, ProductRevision, sizeof(ProductRevision));
    p->tx_xfer->length = sizeof(struct InquiryData);
    //osiExitCritical(critical);

    result = udcEpQueue(p->msg->func->controller, p->msg->epin, p->tx_xfer);
    if (result < 0)
    {
        USB_LOG_TRACE("msg %x data fail %d\n", d->Param[0], result);
    }
    return result == 0;
}

static bool prvRDFormatCapacitiesData(msgDevPriv_t *p, const msgRxData_t *d)
{
    struct FormatCapac *r = p->tx_xfer->buf;
	int result;
    p->tx_state = x_data;
	
	if(d->Lun >= p->udisk_num)
		return FALSE;
	
    //uint32_t critical = osiEnterCritical();
    r->CapListHeader = cpu_to_be32( 0x00000010);//0x10000000;
    r->BlockNumber = cpu_to_be32(p->umssDev[d->Lun].page_number);//0x00ccda01;
    r->BlockLen = cpu_to_be32(p->umssDev[d->Lun].page_size | 0x02000000);//0x00020002;
    r->BlockNumber1 = cpu_to_be32(p->umssDev[d->Lun].page_number);//0x00ccda01;
    r->BlockLen1 = cpu_to_be32(p->umssDev[d->Lun].page_size);//0x00020000;
    p->tx_xfer->length = sizeof(struct FormatCapac);
    //osiExitCritical(critical);
    USB_LOG_TRACE("msg BlockNumber: 0x%x,blocklen=0x%x,BlockNumber1=0x%x,BlockLen1=0x%x\n", r->BlockNumber,r->BlockLen,r->BlockNumber1,r->BlockLen1);

    result = udcEpQueue(p->msg->func->controller, p->msg->epin, p->tx_xfer);
    if (result < 0)
    {
        USB_LOG_TRACE("msg %x data fail %d\n", d->Param[0], result);
    }
    return result == 0;
}
static bool prvRDCapacityData(msgDevPriv_t *p, const msgRxData_t *d)
{
	int result;
    struct CapacData *r = p->tx_xfer->buf;
	
	if(d->Lun >= p->udisk_num)
		return FALSE;
	
	p->tx_state = x_data;
    //uint32_t critical = osiEnterCritical();
    r->LBA = cpu_to_be32(p->umssDev[d->Lun].page_number - 1);//p->LogicBlocAddr[d->Lun]; 
    // r->LBA = 0xffcbda01;
    r->BlockLen = cpu_to_be32(p->umssDev[d->Lun].page_size);//0x00020000; //512
    //USB_LOG_TRACE("msg LBA is 0x%x,blocklen=0x%x\n", r->LBA,r->BlockLen);
    p->tx_xfer->length = sizeof(struct CapacData);
    //osiExitCritical(critical);
    result = udcEpQueue(p->msg->func->controller, p->msg->epin, p->tx_xfer);
    if (result < 0)
    {
        USB_LOG_TRACE("msg %x data fail %d\n", d->Param[0], result);
    }
    return result == 0;
}

static bool prvModeSense6Data(msgDevPriv_t *p, const msgRxData_t *d)
{
	int result;
    struct ModeSenseData *r = p->tx_xfer->buf;
	p->tx_state = x_data;
    //uint32_t critical = osiEnterCritical();
    r->MDLen = 0x03;
    r->MediumTyp = 0x00;
    r->DevSpec = 0x00;
    r->BlockDescLen = 0x00;
    p->tx_xfer->length = sizeof(struct ModeSenseData);
    //osiExitCritical(critical);

    result = udcEpQueue(p->msg->func->controller, p->msg->epin, p->tx_xfer);
    if (result < 0)
    {
        USB_LOG_TRACE("msg %x data fail %d\n", d->Param[0], result);
    }
    return result == 0;
}

static bool prvRead10Data(msgDevPriv_t *p, const msgRxData_t *d)
{    
	static int tx_count = 0;
	int current_count = 0;
	int result;
	BSD_STATUS		status = BSD_ERROR;
	uint32 sector_num = (d->Param[7] << 8) + d->Param[8];
    uint32 LBA = (d->Param[2] << 24) + (d->Param[3] << 16) + (d->Param[4] << 8) + d->Param[5];
    int total_count = 0;
	uint32 page_size = 0;
	//uint32 tx_once_count = 0;
	
	if(d->Lun >= p->udisk_num)
		return FALSE;
	
    p->tx_state = x_data;
    p->tx_sector_done = false;
	page_size = p->umssDev[d->Lun].page_size;

	if(page_size)
	{
			//p->tx_DataResidue = 0;
			total_count = d->XferLen / page_size;
	}
	else
	{
		USB_LOG_TRACE("p->umssDev[%d].page_size is not set!\n",d->Lun);
		return FALSE;
	}
	
	current_count = OSI_MIN(uint32 ,(total_count-tx_count), MSG_IN_DATA_BUF_MAX / page_size);

    USB_LOG_TRACE("MSG prvRead10Data[%d] current_count is %d, total_count is %d,LBA is 0x%x2,tx_count=%d\n",d->Lun, current_count,total_count, LBA,tx_count);
    //count = OSI_MIN(unsigned, (total_count - tx_count), (TCARD_TX_SECTOR_ONCE_COUNT * 1));
	#if 0
	int tx_once_count;
	static int rcount = 0;
    rcount = blockDeviceRead(p->bdev, (uint64_t)(LBA + tx_count), count, p->tx_xfer->buf);
	if (rcount != count)
    {
        USB_LOG_TRACE("MSG blockDeviceRead fail %d\n", rcount);
        return 0;
    }
	tx_once_count = OSI_MIN(unsigned, TCARD_TX_SECTOR_ONCE_COUNT, rcount);
    tx_count += tx_once_count;
    //uint32_t critical = osiEnterCritical();
    p->tx_xfer->length = tx_once_count * TCARD_SECTOR_SIZE;
    //osiExitCritical(critical);
	#else //  // to do!!!!!!!!!!!!!!
	
	 if (current_count) {
		   status = BSD_Read(p->umssDev[d->Lun].dev_handle,
						 LBA+tx_count, \
						 current_count,\
						 (uint8 *)p->tx_xfer->buf); 

	   }
	
	   if(BSD_SUCCESS != status)
	   {
	   	   USB_LOG_TRACE("BSD_Read[%s] fail %d\n", p->umssDev[d->Lun].dev_name,status);
		   return FALSE;
	   }
	   tx_count += current_count;
	   p->tx_xfer->length = current_count * page_size;

#endif

    if (tx_count == total_count)
    {
        tx_count = 0;
        p->tx_sector_done = true;
    }
	USB_LOG_TRACE( "MSG read10 %d, total_count is %d, tx_count is %d\n", p->tx_sector_done, total_count, tx_count);
    result = udcEpQueue(p->msg->func->controller, p->msg->epin, p->tx_xfer);
    if (result < 0)
    {
        USB_LOG_TRACE( "msg %x data fail %d\n", d->Param[0], result);
    }
    return result == 0;
}

static int prvWrite10Data(msgDevPriv_t *p, const msgRxData_t *d)
{
	static int count = 0;
	static uint32_t rx_count = 0;
    uint32_t LBA = p->dummy_data->Signature;
	uint8_t Lun = p->dummy_data->Lun;
	//uint32_t LBA = (d->Param[2] << 24) + (d->Param[3] << 16) + (d->Param[4] << 8) + d->Param[5];
    uint32_t total_count = 0;
    uint32_t page_size = 0;
    //static uint32_t cnt = 0;
	BSD_STATUS		status = BSD_SUCCESS;
	//uint32 sector_num = (d->Param[7] << 8) + d->Param[8];
    p->rx_sector_done = false;
	
	//if(d->Lun >= p->udisk_num)
		//return 0;

	page_size = p->umssDev[Lun].page_size;
	
	if(page_size)
		total_count = p->rx_xfer->length /page_size;
	else
	{
		USB_LOG_TRACE("p->umssDev[%d].page_size is not set!\n",Lun);
		return 0;
	}
	
	USB_LOG_TRACE("MSG prvWrite10Data[%d] LBA is 0x%x, rx count is %d, total_count is %d, p->dummy_data->XferLen is 0x%x\n",Lun, LBA, rx_count, total_count, p->dummy_data->XferLen);
#if 0

    do
    {
        count = OSI_MIN(unsigned, (total_count - cnt), TCARD_TX_SECTOR_ONCE_COUNT);
		
        cnt = blockDeviceWrite(p->bdev, (uint64_t)(LBA + rx_count), count, (const void *)p->rx_xfer->buf);


        rx_count += cnt;
    } while (total_count - cnt);
#else	 // to do!!!!!!!!!!!!!!
do{
	count = OSI_MIN(unsigned, (total_count - count), 8/*tx_once_sector_num*/);
	if (count) {
		status = BSD_Write(p->umssDev[Lun].dev_handle,
			          LBA + rx_count, \
			          count,\
			          (uint8 *)(const void *)p->rx_xfer->buf); 
	}
	
    if(BSD_SUCCESS != status)
    {
    	 USB_LOG_TRACE("BSD_Write[%s] fail %d\n", p->umssDev[Lun].dev_name,status);
         return 0;
    }
	rx_count += count;
}while(total_count - count);
#endif

    p->dummy_data->XferLen -= p->rx_xfer->length;
    //cnt = 0;
    if (!p->dummy_data->XferLen)
    {
        p->rx_sector_done = true;
        rx_count = 0;
    }
	USB_LOG_TRACE("MSG write10 %d, total_count is %d, rx_count is %d\n", p->rx_sector_done, total_count, rx_count);

    return p->rx_xfer->length;
}

static bool prvCSWRespPhase(msgDevPriv_t *p, const msgRxData_t *d, unsigned length)
{
	int result;
    //uint32_t critical = osiEnterCritical();
    msgCSWData_t *r = p->tx_xfer->buf;
	p->tx_state = x_csw;
    r->Signature = CSW;
    r->Tag = d->Tag;
    r->CSWDataResidue = d->XferLen - length;
    if (MSG_PREVENTALLWMDMRMVL == d->Param[0])
        r->CSWStatus = d->Param[4]; //resp for MSG_PREVENTALLWMDMRMVL
    else
        r->CSWStatus = 0x00;
    p->tx_xfer->length = sizeof(msgCSWData_t);
    //osiExitCritical(critical);

    result = udcEpQueue(p->msg->func->controller, p->msg->epin, p->tx_xfer);
    if (result < 0)
    {
        USB_LOG_TRACE("msg resp fail %d\n", result);
    }
    return result == 0;
}

static bool prvProcessCommand(msgDevPriv_t *p, const msgRxData_t *d, unsigned length)
{
    bool res, result = false;
	static uint32 write_length = 0;
    if ((p->tx_state != x_csw) && (p->rx_state != x_csw))
    {
        USB_LOG_TRACE("msg rx command code 0x%x, xferlen is 0x%x,tag is 0x%x\n", d->Param[0], d->XferLen, d->Tag);
        switch (d->Param[0])
        {
        case MSG_REQUESTSENSE:
            res = prvReqSense(p, d);
            if (!res)
            {
                USB_LOG_TRACE("msg requestsense resp %x error\n", d->Param[0]);
                return false;
            }
            break;
        case MSG_INQUIRY:
            res = prvInquiryData(p, d);
            if (!res)
            {
                USB_LOG_TRACE("msg data resp %x error\n", d->Param[0]);
                return false;
            }
            break;
        case MSG_READFORMATCAPACITIES:
            res = prvRDFormatCapacitiesData(p, d);
            if (!res)
            {
                USB_LOG_TRACE("msg data resp %x error\n", d->Param[0]);
                return false;
            }
            break;
        case MSG_READCAPACITY:
            res = prvRDCapacityData(p, d);
            if (!res)
            {
                USB_LOG_TRACE( "msg data resp %x error\n", d->Param[0]);
                return false;
            }
            break;
        case MSG_MODESENSE6:
            res = prvModeSense6Data(p, d);
            if (!res)
            {
                USB_LOG_TRACE("msg data resp %x error\n", d->Param[0]);
                return false;
            }
            break;
        case MSG_READ10:
            res = prvRead10Data(p, d);
            if (!res)
            {
                USB_LOG_TRACE( "msg read data resp %x error\n", d->Param[0]);
                return false;
            }
            break;
        case MSG_WRITE10:
            p->rx_state = x_data;
			length = d->XferLen;
            p->dummy_data->Tag = d->Tag;
            p->dummy_data->XferLen = d->XferLen;
			p->dummy_data->Lun = d->Lun;
            p->dummy_data->Signature = (d->Param[2] << 24) + (d->Param[3] << 16) + (d->Param[4] << 8) + d->Param[5]; //for use the unified structure;
            USB_LOG_TRACE("msg write10 data start!\n");
            break;
        case MSG_TESTUNITREADY:
        case MSG_PREVENTALLWMDMRMVL:
            res = prvCSWRespPhase(p, d, d->XferLen);
            if (!res)
            {
                USB_LOG_TRACE("msg resp code %x error\n", d->Param[0]);
                return false;
            }
            break;
        default:
            break;
        }
    }
    else
    {
        /* CSW RESP */
        if (p->rx_state == x_csw)
        {
            USB_LOG_TRACE("msg write data done and start csw resp:xferlen is 0x%x,tag is 0x%x\n", length, p->dummy_data->Tag);
            result = prvCSWRespPhase(p, p->dummy_data, length);
			write_length = 0;
        }
        else
        {
            //if (d->Param[0] == MSG_READ10)
                //length = d->XferLen;
            result = prvCSWRespPhase(p, d, length);
        }
        if (!result)
        {
            USB_LOG_TRACE( "msg resp code %x error\n", d->Param[0]);
            return false;
        }
    }
    return true;
}

void SendMsgToUMSThread(USB_UMS_MSG_E sig,uint32 sig_param)
{
	xSignalHeader signal;
	signal = SCI_ALLOC (sizeof (*signal));
	signal->SignalCode = sig;
	signal->SignalSize = sizeof (*signal);
	signal->Sender = SCI_IdentifyThread();
	SCI_SendSignal (signal, ums_task_id);
}

static void prvMsgRxWork(void *param)
{
    msgDevPriv_t *p = (msgDevPriv_t *)param;
    const usbXfer_t *x = p->rx_xfer;
    const msgRxData_t *d = (const msgRxData_t *)x->buf;
	int status;
	bool r;

    if (x->status < 0)
    {
        USB_LOG_TRACE("msg rx complete fail 0x%x\n", x->status);
		return;
    }

    if (p->rx_state == x_idle)
    {
        //USB_LOG_TRACE("prvMsgRxWork write request\n");
        r = prvProcessCommand(p, d, x->actual);
        if (!r)
        {
            USB_LOG_TRACE("msg rx parse fail\n");
        }
    }
	else if (p->rx_state == x_data)
    {
        //USB_LOG_TRACE("msg rx write10 data start,d->XferLen = 0x%x,x->length= 0x%x\n", d->XferLen, x->length);
		
        status = prvWrite10Data(p, d);
        if (!status)
        {
            USB_LOG_TRACE("msg write data fail\n");
        }
		write10_length += x->actual;
        if (p->rx_sector_done)
        {
            p->rx_state = x_csw;
            r = prvProcessCommand(p, d, write10_length/*x->actual*/);
            if (!r)
            {
                USB_LOG_TRACE("msg rx parse fail\n");
            }
			write10_length=0;
            p->rx_state = x_idle;
        }
    }

    if (!prvMsgRxStart(p))
    {
        USB_LOG_TRACE("msg rx restart fail\n");
    }

}

static void prvMsgTxWork(void *param)
{
    msgDevPriv_t *p = (msgDevPriv_t *)param;
    const usbXfer_t *x = p->rx_xfer;
    const msgRxData_t *d = (const msgRxData_t *)x->buf;
	bool r;
    if (p->tx_state == x_data)
    {
    	if(d->Param[0] == MSG_READ10)
    	{
			read10_length += p->tx_xfer->actual;
		}
        if ((d->Param[0] == MSG_READ10) && !(p->tx_sector_done))
        {
            p->tx_state = x_data;
            //USB_LOG_TRACE("msg tx read10 data tx continue\n");
            prvRead10Data(p, d);
        }
        else
        {
            p->tx_state = x_csw;
            USB_LOG_TRACE("msg tx read10 data done and send csw resp\n");
            r = prvProcessCommand(p, d, read10_length);
            if (!r)
            {
                USB_LOG_TRACE( "msg data_resp pahase fail\n");
            }
			read10_length = 0;
        }
    }
    else if (p->tx_state == x_csw)
    {
        p->tx_state = x_idle;
        USB_LOG_TRACE("msg tx state %d", p->tx_state);
    }
}

static void prvMsgTxComplete(usbEp_t *ep, struct usb_xfer *xfer)
{
	msgDevPriv_t *p;
    USB_LOG_TRACE("msg tx complete %d/%d\n", xfer->status, xfer->length);
    if (xfer->status < 0)
    {
        USB_LOG_TRACE(0x1000a040, "msg tx complete fail %d", xfer->status);
    }

    if (xfer->status != -ECANCELED)
    {
        p = (msgDevPriv_t *)xfer->param;
		//prvMsgTxWork(p);
		SendMsgToUMSThread(USB_UMS_TX_COMPLET,p);
    }
}

static void prvMsgRxComplete(usbEp_t *ep, struct usb_xfer *xfer)
{
	msgDevPriv_t *p;
    USB_LOG_TRACE( "msg rx complete %d/%d\n", xfer->status, xfer->actual);
	
    if (xfer->status == 0/*!= -ECANCELED*/)
    {
        p = (msgDevPriv_t *)xfer->param;
		//prvMsgRxWork(p);
		SendMsgToUMSThread(USB_UMS_RX_COMPLET,p);
    }
}

LOCAL void _usbMsg_SrvThread (uint32 argc, void *argv)
{
    xSignalHeader    signal;
	msgDevPriv_t *p = (msgDevPriv_t *)argv;
	BLOCK_ID tid = SCI_IdentifyThread();

    for(;;)
    {
        signal = SCI_GetSignal (tid);

        switch(signal->SignalCode)
        {
            case USB_UMS_TX_COMPLET:
				prvMsgTxWork(p);

            break;
            case USB_UMS_RX_COMPLET:
				prvMsgRxWork(p);

            break;
            default:
                SCI_PASSERT (0,("Error msg!!!"));   /*assert verified*/
            break;
        }

        SCI_FREE(signal);
        signal = NULL;
    }
}

LOCAL void _usbMsg_print_diskInfo (msgDevPriv_t *p)
{
	uint8 i = 0;
	for(i=0;i<p->udisk_num;i++)
	{
		USB_LOG_TRACE("[%d][%s]page_size:%d,page_number=%d,state:%d,bsd_handle:0x%x\n",i,p->umssDev[i].dev_name,p->umssDev[i].page_size,p->umssDev[i].page_number,p->umssDev[i].dev_state,p->umssDev[i].dev_handle);

	}
}

static msgDevPriv_t *prvCreate(usbMsgDev_t *msg)
{
	uintptr_t aligned;
	bool result = false;
	uint8 i = 0;
	uint32 block_count;
    const unsigned alloc_size = /*sizeof(msgDevPriv_t) + */	CONFIG_CACHE_LINE_SIZE + MSG_DATA_BUF_MAX + MSG_IN_DATA_BUF_MAX;
	msgDevPriv_t *p = &msgDev;
	uintptr_t xfer_buffer = (uint8 *)calloc(1, alloc_size);

    if (xfer_buffer == NULL){
		USB_ERR_TRACE("%s calloc fail\n",__FUNCTION__);
        return NULL;
    }
	
	SCI_MEMSET(p, 0, sizeof(msgDevPriv_t));
	p->xfer_buf = (void *)xfer_buffer;
	USB_LOG_TRACE("%s enter\n",__FUNCTION__);

    aligned = OSI_ALIGN_UP((uintptr_t)xfer_buffer, CONFIG_CACHE_LINE_SIZE);
    
    do
    {

		p->umssDev = umss_GetActiveDiskDev();
		if(p->umssDev == NULL){
			p->umssDev = umss_CustomerConfig();
			if(p->umssDev == NULL)
			{
				USB_LOG_TRACE("msg failed to find block device\n");
				free(p->xfer_buf);
				return NULL;
			}
			p->udisk_num = umss_GetActiveDiskNum();
			_usbMsg_print_diskInfo(p);
		}
		
		USB_LOG_TRACE("%s umssDev get done\n",__FUNCTION__);
		if(ums_task_id == 0xffff){
			ums_task_id  = SCI_CreateThread("UMSS_SRV",
										"Q_UMSS_SRV",
										_usbMsg_SrvThread ,
										1,
										&msgDev,
										MSG_DEV_THREAD_STACK_SIZE ,
										24,
										MSG_DEV_THREAD_PRIORITY , //higher priority for higher speed?
										SCI_PREEMPT,
										SCI_AUTO_START);
		}
        p->tx_xfer = udcXferAlloc(msg->func->controller);
        if (!p->tx_xfer)
            break;

        p->rx_xfer = udcXferAlloc(msg->func->controller);
        if (!p->rx_xfer)
            break;

        p->dummy_data = (msgRxData_t *)calloc(1, sizeof(msgRxData_t));
        if (!p->dummy_data)
            break;

        result = true;
    } while (0);

    if (!result)
    {
    	if(p->rx_xfer)
        	udcXferFree(msg->func->controller, p->rx_xfer);
		if(p->tx_xfer)
        	udcXferFree(msg->func->controller, p->tx_xfer);
		if(p->dummy_data)
        	free(p->dummy_data);
		if(p->xfer_buf)
        	free(p->xfer_buf);
        return NULL;
    }
	
    p->rx_xfer->buf = (void *)aligned;
    p->rx_xfer->length = MSG_DATA_BUF_MAX;
    p->rx_xfer->param = p;
    p->rx_xfer->complete = prvMsgRxComplete;

    p->tx_xfer->buf = (void *)(aligned + MSG_DATA_BUF_MAX);
    p->tx_xfer->param = p;
    p->tx_xfer->complete = prvMsgTxComplete;

    p->msg = msg;
    return p;
}

static void prvDelete(usbMsgDev_t *msg, msgDevPriv_t *p)
{
    udcXferFree(msg->func->controller, p->rx_xfer);
    udcXferFree(msg->func->controller, p->tx_xfer);
	umss_CustomerDeconfig();
    free(p->dummy_data);
    free(p->xfer_buf);
}

bool usbMsgDevBind(usbMsgDev_t *msg)
{
	msgDevPriv_t *prv;
	
    if (msg == NULL)
        return false;

    prv = prvCreate(msg);
    if (prv == NULL)
        return false;

    msg->priv = (void *)prv;
    return true;
}

void usbMsgDevUnbind(usbMsgDev_t *msg)
{
	msgDevPriv_t *prv;
    if (msg == NULL || msg->priv == NULL)
        return;

    prv = (msgDevPriv_t *)msg->priv;
    msg->priv = NULL;
    prvDelete(msg, prv);
}

bool usbMsgDevStart(usbMsgDev_t *msg)
{
	msgDevPriv_t *prv;

    if (msg == NULL || msg->priv == NULL)
        return false;

    prv = (msgDevPriv_t *)msg->priv;
    return prvMsgRxStart(prv);
}

void usbMsgDevStop(usbMsgDev_t *msg)
{
    if (msg == NULL || msg->priv == NULL)
        return;

    udcEpDequeueAll(msg->func->controller, msg->epout);
    udcEpDequeueAll(msg->func->controller, msg->epin);
}

