#include "sci_types.h"
#include "fdl_command.h"
#include "fdl_main.h"
#include "fdl_stdio.h"
#include "fdl_conf.h"
#include "packet.h"
#include "sio_drv.h"
//#include "sio_api.h"
#include "usb_boot.h"

extern void JumpToTarget (unsigned long addr);

typedef struct _DL_FILE_STATUS
{
    unsigned long start_address;
    unsigned long total_size;
    unsigned long recv_size;
    unsigned long next_address;
} DL_FILE_STATUS, *PDL_FILE_STATUS;

static DL_FILE_STATUS g_file;


int FDL1_SysConnect (PACKET_T *packet, void *arg)
{
    FDL_SendAckPacket (BSL_REP_ACK);
    return 1;
}

int FDL1_DataStart (PACKET_T *packet, void *arg)
{
    //unsigned long *ptr = (unsigned long *) packet->packet_body.content;

    unsigned long *data = (unsigned long *) (packet->packet_body.content);
    unsigned long start_addr = EndianConv_32 (*data);
    unsigned long file_size = EndianConv_32 (* (data + 1));

    if ( (start_addr < MEMORY_START) || (start_addr >= MEMORY_START + MEMORY_SIZE))
    {
        FDL_SendAckPacket (BSL_REP_DOWN_DEST_ERROR);
        return 0;
    }

    if ( (start_addr + file_size) > (MEMORY_START + MEMORY_SIZE))
    {
        FDL_SendAckPacket (BSL_REP_DOWN_SIZE_ERROR);
        return 0;
    }

    g_file.start_address = start_addr;
    g_file.total_size = file_size;
    g_file.recv_size = 0;
    g_file.next_address = start_addr;

    FDL_memset ( (void *) start_addr, 0, file_size);

    if (!packet->ack_flag)
    {
        packet->ack_flag = 1;
        FDL_SendAckPacket (BSL_REP_ACK);
    }

    return 1;
}

int FDL1_DataMidst (PACKET_T *packet, void *arg)
{
    unsigned short data_len = packet->packet_body.size;

    if ( (g_file.recv_size + data_len) > g_file.total_size)
    {
        FDL_SendAckPacket (BSL_REP_DOWN_SIZE_ERROR);
        return 0;
    }

    FDL_memcpy ( (void *) g_file.next_address, (const void *) (packet->packet_body.content), data_len);
    g_file.next_address += data_len;
    g_file.recv_size += data_len;

    if (!packet->ack_flag)
    {
        packet->ack_flag = 1;
        FDL_SendAckPacket (BSL_REP_ACK);
    }

    return 1;
}

int FDL1_DataEnd (PACKET_T *packet, void *arg)
{
    if (!packet->ack_flag)
    {
        packet->ack_flag = 1;
        FDL_SendAckPacket (BSL_REP_ACK);
    }

    return 1;
}

int FDL1_DataExec (PACKET_T *packet, void *arg)
{

    //BSL_REP_ACK is sent in the begin of FDL2

    JumpToTarget (g_file.start_address);

    return 0;
}

int FDL1_SetBaudrate (PACKET_T *packet, void *arg)
{
    unsigned long baudrate = * (unsigned long *) (packet->packet_body.content);
    //int           i;
    baudrate = EndianConv_32 (baudrate);

    if (!packet->ack_flag)
    {
        packet->ack_flag = 1;
        FDL_SendAckPacket (BSL_REP_ACK);
    }

    gFdlUsedChannel->SetBaudrate (gFdlUsedChannel, baudrate);
    return 0;

}
