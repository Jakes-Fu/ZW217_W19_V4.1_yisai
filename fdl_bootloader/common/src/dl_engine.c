#include "dl_engine.h"
#include "packet.h"
#include "fdl_stdio.h"
#include <string.h>
#include "sio_drv.h"

typedef struct _DL_STATUS
{
    DL_STAGE stage;
    int      data_verify;   /* Record the result of the latest file download
                             * operation. */
} DL_STATUS, *PDL_STATUS;

typedef struct _CMDPROC_TAB
{
    CMDPROC    proc;
    void      *arg;
} CMDPROC_TAB, *PCMDPROC_TAB;

#pragma arm section zidata = "ZIDATA_AREA_3"
CMDPROC_TAB g_proctab[BSL_CMD_TYPE_MAX - BSL_CMD_TYPE_MIN] = {0, 0};/*lint !e656*/
#pragma arm section zidata

#define IS_VALID_CMD(cmd)           ((cmd >= BSL_CMD_TYPE_MIN) && (cmd < BSL_CMD_TYPE_MAX))
#define CMD_IND(cmd)                ((cmd) - BSL_CMD_TYPE_MIN)
#define CMD_PROC(cmd)               (g_proctab[CMD_IND(cmd)].proc)
#define CMD_ARG(cmd)                (g_proctab[CMD_IND(cmd)].arg)
#define CALL_PROC(cmd, packet)      (CMD_PROC(cmd)((packet), CMD_ARG(cmd)))

unsigned char FDL_DlInit (void)
{
    memset (g_proctab, 0, sizeof (g_proctab));
    return 1;
}

unsigned char FDL_DlReg (CMD_TYPE cmd, CMDPROC proc, void *arg)
{
    if (!IS_VALID_CMD (cmd))
    {
        return 0;
    }

    CMD_PROC (cmd) = proc; /*lint !e656*/
    CMD_ARG (cmd) = arg;   /*lint !e656*/
    return 1;
}


int FDL_DlEntry (DL_STAGE start)
{
    cmd_pkt_type pkt_type;
    PACKET_T    *packet_ptr;

    rprintf("FDL_DlEntry\n");

    for ( ; ; )
    {
        //receive packet
        packet_ptr = FDL_GetPacket();

        pkt_type = (cmd_pkt_type) (packet_ptr->packet_body.type);

        //SIO_TRACE("FDL_DlEntry  pkt_type =0x%x !\r\n", pkt_type);
        pkt_type = (cmd_pkt_type)(EndianConv_16((unsigned short)pkt_type));
        packet_ptr->packet_body.size = EndianConv_16(packet_ptr->packet_body.size);
        //rprintf("GetPacket: type 0x%x, size %d\n", pkt_type, packet_ptr->packet_body.size);

        CALL_PROC(pkt_type, packet_ptr);

        FDL_FreePacket (packet_ptr);
    }

    // return BSL_PHONE_SUCCEED;
}



