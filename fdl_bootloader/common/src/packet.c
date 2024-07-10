#include "string.h"
#include "stdarg.h"

#include "sci_types.h"
#include "packet.h"
#include "fdl_stdio.h"
#include "fdl_main.h"
#include "fdl_crc.h"
#include "sio_drv.h"
#include "usb_boot.h"
#include "virtual_com.h"
#include "fdl_channel.h"

#ifdef defined(PLATFORM_UWS6121E)
extern void rprintf(const char *fmt, ...);
#endif

struct FDL_ChannelHandler *gFdlUsedChannel;

#pragma arm section zidata = "ZIDATA_AREA_1"
static PACKET_T  packet[ PACKET_MAX_NUM ];
#pragma arm section zidata

static PACKET_T *packet_free_list;
static PACKET_T *packet_completed_list;
static PACKET_T *packet_receiving;
extern uint32    s_intel_m18_serial;
extern BOOLEAN g_is_dload_phasecheck;

uint32 g_fdl_pkt_crc_type = 0;

extern void rprintf(const char *fmt, ...);
typedef struct UartPort
{
    unsigned int regBase;
    unsigned int baudRate;
} UartPort_T;

extern FDL_ChannelHandler_T gUart1_DownloadChannel;

void FDL_PacketInit (void)
{
    uint32 i = 0;
	unsigned int temp_base = 0;

    packet_free_list = &packet[0];

    for (i = 0; i < PACKET_MAX_NUM; i++)
    {
        memset (&packet[i], 0, sizeof (PACKET_T));
        packet[i].next   = &packet[i+1];
    }

    packet[PACKET_MAX_NUM-1].next = PNULL;

    packet_completed_list = NULL;
    packet_receiving      = NULL;

	rprintf("FDL_PacketInit\n");
    gFdlUsedChannel = FDL_ChannelGet();

}


PACKET_T *FDL_MallocPacket (void)
{
    PACKET_T   *tmp_ptr = NULL;

    if (NULL != packet_free_list)
    {
        tmp_ptr = packet_free_list;
        packet_free_list = tmp_ptr->next;

        // only clear the packet header
        memset (tmp_ptr, 0, 32);

        tmp_ptr->next       = NULL;
        tmp_ptr->pkt_state  = PKT_NONE;
        tmp_ptr->ack_flag   = 0;
        tmp_ptr->data_size  = 0;
    }

    return tmp_ptr;
}

void FDL_FreePacket (PACKET_T *ptr)
{
    ptr->next        = packet_free_list;
    packet_free_list = ptr;
}

PACKET_T   *FDL_GetPacket (void)
{
    PACKET_T *ptr;

    // waiting for a packet
    while (NULL == packet_completed_list)
    {
        FDL_PacketDoIdle();
    }

    // remove from completed list
    ptr                     = packet_completed_list;
    packet_completed_list   = ptr->next;
    ptr->next               = NULL;
    return ptr;
}

void FDL_PacketDoIdle (void)
{
    //FDL_BOOT_MODE_E          boot_mode;
    unsigned char *pdata      = NULL;
    PACKET_T       *packet_ptr = NULL;
    PACKET_T       *tmp_ptr    = NULL;
    uint32          crc;
    unsigned char   ch;
    int ch1;

    // try get a packet to handle the receive char
    packet_ptr = packet_receiving;

    if (NULL == packet_ptr)
    {
        packet_ptr = FDL_MallocPacket();

        if (NULL != packet_ptr)
        {
            packet_receiving    = packet_ptr;
            packet_ptr->next    = NULL;
        }
        else
        {
            return ;
        }
    }

    pdata  = (unsigned char *) & (packet_ptr->packet_body);

    for( ; ; )
    {
        /*
        * here the reason that we don't call GetChar is
        * that GetChar() isnot exited until got data from
        * outside,FDL_PacketDoIdle() is called also in norflash write and erase
        * so we have to exited when no char recieived here,
        * usb virtual com is the same handle
        */
        ch1 = gFdlUsedChannel->GetSingleChar (NULL);

        if (ch1 == -1)
        {
            return;
        }

        ch = ch1&0xff;

        if (packet_ptr->data_size > MAX_PKT_SIZE)
        {
            packet_receiving = NULL;
            FDL_FreePacket (packet_ptr);
			#ifdef defined(PLATFORM_UWS6121E)
            rprintf("data_size error");
			#endif
            SEND_ERROR_RSP (BSL_REP_VERIFY_ERROR)
            //return;
        }

        // try handle this input data
        switch (packet_ptr->pkt_state)
        {
            case PKT_NONE:

                if (HDLC_FLAG == ch)
                {
                    packet_ptr->pkt_state = PKT_HEAD;
                    packet_ptr->data_size = 0;
                }

                break;
            case PKT_HEAD:

                if (HDLC_FLAG != ch)
                {
                    if (HDLC_ESCAPE == ch)
                    {
                        // Try get the "true" data.
                        //ch = sio_get_char();
                        ch = gFdlUsedChannel->GetChar (NULL);
                        ch = ch ^ HDLC_ESCAPE_MASK;
                    }

                    packet_ptr->pkt_state = PKT_GATHER;
                    * (pdata + packet_ptr->data_size)         = ch;
                    //*(pdata++) = ch;
                    packet_ptr->data_size += 1;

                }

                break;
            case PKT_GATHER:

                if (HDLC_FLAG == ch)
                {
                    packet_ptr->pkt_state = PKT_RECV;

                    // check the packet. CRC should be 0
                    if(TRUE == g_fdl_pkt_crc_type)
                    {
                        crc = crc_16_l_calc((char *)&packet_ptr->packet_body, (uint32)packet_ptr->data_size);
                    }
                    else
                    {
                        crc = frm_chk ( (unsigned short *) &packet_ptr->packet_body, packet_ptr->data_size);
                    }

                    if (0 != crc)
                    {
                        // Verify error, reject this packet.
                        FDL_FreePacket (packet_ptr);
                        packet_receiving = NULL;
                        // if check result failed, notify PC
                        SEND_ERROR_RSP (BSL_REP_VERIFY_ERROR)
                        //sio_trace( "crc failed" );
                        //return ;
                    }
                    else
                    {
                        // Check there are free packet?
		                	if(    (NULL != packet_free_list)
		                    	&& (BSL_CMD_MIDST_DATA == packet_ptr->packet_body.type)
		                    	&& (0 == s_intel_m18_serial)
		                    	&& (g_is_dload_phasecheck == FALSE ))
                        {
                            // send a ACK
                            packet_ptr->ack_flag = 1;
                            FDL_SendAckPacket (BSL_REP_ACK);
                        }

                        // It is a complete packet, move to completed list.
                        packet_ptr->next = NULL;

                        if (NULL == packet_completed_list)
                        {
                            packet_completed_list       = packet_ptr;
                        }
                        else
                        {
                            // added to the tail
                            tmp_ptr = packet_completed_list;

                            while (NULL != tmp_ptr->next)
                            {
                                tmp_ptr = tmp_ptr->next;
                            }

                            tmp_ptr->next = packet_ptr;
                        }

                        //set to null for next transfer
                        packet_receiving = NULL;

                        return ;
                    }
                }
                else
                {
                    if (HDLC_ESCAPE == ch)
                    {
                        //ch = sio_get_char();//gFdlUsedChannel->GetChar(gFdlUsedChannel);
                        ch = gFdlUsedChannel->GetChar (NULL);
                        ch = ch ^ HDLC_ESCAPE_MASK;
                    }

                    * (pdata + packet_ptr->data_size)         = ch;
                    packet_ptr->data_size += 1;
                }

                break;
            default:
                break;
        }
    }
}

/******************************************************************************
 * write_packet
 ******************************************************************************/
LOCAL void FDL_WritePacket (const void *buf, int len)
{
    gFdlUsedChannel->Write (gFdlUsedChannel, buf, len);
}

void FDL_Flush (void)
{
    gFdlUsedChannel->Close(gFdlUsedChannel);
}


LOCAL uint32  FDL_DataProcess (PACKET_T *packet_ptr_src, PACKET_T *packet_ptr_dest)
{
    unsigned short  crc, size;
    int32           write_len;  /*orginal length*/
    int32           send_len;   /*length after encode*/
    int32           i;
    uint8           curval;

    uint8          *des_ptr = NULL;
    uint8          *src_ptr = NULL;

    size = packet_ptr_src->packet_body.size;
    write_len = size + sizeof (unsigned short) + PACKET_HEADER_SIZE;
    src_ptr = (uint8 *) &packet_ptr_src->packet_body;

    packet_ptr_src->packet_body.size = EndianConv_16 (packet_ptr_src->packet_body.size);
    packet_ptr_src->packet_body.type = EndianConv_16 (packet_ptr_src->packet_body.type);

    /*src CRC calculation*/
    if(TRUE == g_fdl_pkt_crc_type)
    {
        crc = crc_16_l_calc((char *)(&(packet_ptr_src->packet_body)), size + PACKET_HEADER_SIZE);
    }
    else
    {
        crc = frm_chk ( (const unsigned short *) (& (packet_ptr_src->packet_body)), size + PACKET_HEADER_SIZE);

        crc = EndianConv_16 (crc);
    }

    packet_ptr_src->packet_body.content[ size ] = (crc >> 8) & 0xFF;
    packet_ptr_src->packet_body.content[ size+1 ] = (crc)    & 0xFF;

    /*******************************************
    *    des data preparation
    ********************************************/

    des_ptr = (uint8 *) &packet_ptr_dest->packet_body;
    /*head flag*/
    * (des_ptr++) = HDLC_FLAG;
    send_len = 1;

    /*middle part process*/
    for (i = 0; i < write_len; i++)
    {
        curval = * (src_ptr + i);

        if ( (HDLC_FLAG == curval) || (HDLC_ESCAPE == curval))
        {
            * (des_ptr++) = HDLC_ESCAPE;
            * (des_ptr++) = ~HDLC_ESCAPE_MASK & curval;
            send_len++;
        }
        else
        {
            * (des_ptr++) = curval;
        }

        send_len++;
    }

    /*end flag*/
    * (des_ptr++) = HDLC_FLAG;
    send_len++;

    return send_len;
}
/******************************************************************************
 * FDL_SendPacket
 ******************************************************************************/
void FDL_SendPacket (PACKET_T *packet_ptr)
{
    int32           send_len;   /*length after encode*/
    PACKET_T       *tmp_packet_ptr = NULL;

    // send a ACK packet to notify PC that we are ready.
    tmp_packet_ptr = FDL_MallocPacket();

    if (NULL == tmp_packet_ptr)
    {

        FDL_FreePacket (packet_receiving);
        tmp_packet_ptr = FDL_MallocPacket();

        send_len = FDL_DataProcess (packet_ptr, tmp_packet_ptr);
        FDL_WritePacket ( (char *) (& (tmp_packet_ptr->packet_body)), send_len);
        FDL_FreePacket (tmp_packet_ptr);

        for( ; ; ) {} //error occurs ...
    }

    send_len = FDL_DataProcess (packet_ptr, tmp_packet_ptr);

    FDL_WritePacket ( (char *) (& (tmp_packet_ptr->packet_body)), send_len);

    FDL_FreePacket (tmp_packet_ptr);

}

/******************************************************************************
 * FDL_SendAckPacket_packet
 ******************************************************************************/
void FDL_SendAckPacket (cmd_pkt_type  pkt_type)
{

    unsigned long ack_packet_src[8];
    unsigned long ack_packet_dst[8];
    PACKET_T *packet_ptr = (PACKET_T *) ack_packet_src;

    int32           send_len;   /*length after encode*/
    PACKET_T       *tmp_packet_ptr = NULL;

    packet_ptr->packet_body.type = pkt_type;
    packet_ptr->packet_body.size = 0;

    tmp_packet_ptr = (PACKET_T *) ack_packet_dst;

    send_len = FDL_DataProcess (packet_ptr, tmp_packet_ptr);
    FDL_WritePacket ( (char *) (& (tmp_packet_ptr->packet_body)), send_len);
	
	FDL_Flush();
}

//#define FDL_DEBUG_LOG
/**********************************************************************************/
//  Description :
//
//    Global resource dependence :
//      sector_size
//  Author:
//      henry.he 2012.4.8
//    Note:
//
/**********************************************************************************/
void FDL_SendErrLog(const char *x_format, ...)
{
#ifdef FDL_DEBUG_LOG

    PACKET_T *packet_ptr = PNULL;

    int32      send_len = 0;
    char       format_str[256] = {0};
    va_list    args;

    va_start(args, x_format);
    send_len = vsprintf(format_str, x_format, args);
    if(send_len%2)
    {
        format_str[send_len++] = 0x20;
    }
    send_len += vsprintf(format_str+send_len, "\r\n", args);
    va_end(args);

    FDL_SendTrace(format_str);

    packet_ptr = FDL_MallocPacket();

    packet_ptr->packet_body.type = BSL_REP_LOG;
    packet_ptr->packet_body.size = send_len;

    FDL_memcpy (packet_ptr->packet_body.content, format_str, send_len);

    FDL_SendPacket (packet_ptr);

    FDL_FreePacket (packet_ptr);

    {
        int i;
        for(i=0; i<2000; i++);
    }

#endif
    return;
}

/**********************************************************************************/
//  Description :
//
//    Global resource dependence :
//      sector_size
//  Author:
//      henry.he 2012.4.8
//    Note:
//
/**********************************************************************************/
void FDL_SendTrace(const char *x_format, ...)
{
#if 0
    int     ret = 0;
    int32   send_len = 0;
    char    format_str[256] = {0};
    va_list args;

    va_start(args, x_format);
    send_len = vsprintf(format_str, x_format, args);
    send_len += vsprintf(format_str+send_len, "\r\n", args);
    va_end(args);

    if(&gUart1Channel != FDL_ChannelGet())
    {
        ret = gUart1Channel.Write (&gUart1Channel, format_str, send_len);
    }

    if(&gUart0Channel != FDL_ChannelGet())
    {
        ret = gUart0Channel.Write (&gUart0Channel, format_str, send_len);
    }

    //FDL_SendTrace("%s %d", __func__, __LINE__);
#endif
    return;
}

