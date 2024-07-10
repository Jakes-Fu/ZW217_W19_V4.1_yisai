/******************************************************************************
 ** File Name:      FDL_main.c                                                 *
 ** Author:         weihua.wang                                                  *
 ** DATE:           27/06/2005                                               *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "fdl_stdio.h"
#include "fdl_command.h"
#include "fdl_conf.h"
#include "fdl_main.h"
#include "cmd_def.h"
#include "packet.h"
#include "dl_engine.h"
#include "sio_drv.h"
//#include "sdram_init.h"
#include "usb_boot.h"
//#include "chip_init.h"
#include "virtual_com.h"
#include "fdl_channel.h"
//#include "bl_mmu.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
/*lint -e551 for symbol:"stack_space" */
LOCAL unsigned char stack_space[STACK_SIZE]= {0};
uint32 s_intel_m18_serial = 0;

const unsigned long stack_bottom = ( (unsigned long) &stack_space + STACK_SIZE);

static const char VERSION_STR[] = {"Spreadtrum Boot Block version 1.1"};

/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
static void error (void)
{
    //sio_putstr("The first FDL failed!\r\n");
    for (;;) /*Do nothing*/;
}

int main (void)
{
    PACKET_T         *packet;
    char ch;

    usb_boot (1);
	  s_intel_m18_serial = 0;
    FDL_PacketInit();

    FDL_DlInit();
    FDL_DlReg (BSL_CMD_CONNECT,    FDL1_SysConnect,  0);
    FDL_DlReg (BSL_CMD_START_DATA, FDL1_DataStart,   0);
    FDL_DlReg (BSL_CMD_MIDST_DATA, FDL1_DataMidst,   0);
    FDL_DlReg (BSL_CMD_EXEC_DATA,  FDL1_DataExec,    0);
    FDL_DlReg (BSL_CMD_END_DATA,   FDL1_DataEnd,     0);
    FDL_DlReg (BSL_SET_BAUDRATE,   FDL1_SetBaudrate, 0);

    for ( ; ; )
    {
        ch = gFdlUsedChannel->GetChar (gFdlUsedChannel);

        if (0x7e == ch)
        {
            break;
        }
    }


    //when got the signal of 0x7e ,send response to pc
    packet = FDL_MallocPacket();
    packet->packet_body.type = BSL_REP_VER;
    packet->packet_body.size = sizeof (VERSION_STR);
    FDL_memcpy (packet->packet_body.content, VERSION_STR, sizeof (VERSION_STR));
    FDL_SendPacket (packet);
    FDL_FreePacket (packet);

    FDL_DlEntry (DL_STAGE_NONE);

    error();
    return 1;
}
