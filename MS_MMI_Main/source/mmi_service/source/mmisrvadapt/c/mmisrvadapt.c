#include "sci_types.h"
#include "mmi_appmsg.h"
#include "mmk_msg.h"
#include "mmk_app.h"

/*****************************************************************************/
//  Description:  Send a BT msg to system's focus win.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_SendBtMsg(uint32 msg_data)
{
    MMK_PostMsg(MMK_GetFocusWinHandle(), MSG_SRVAUD_BT_EVENT, (DPARAM)msg_data, sizeof(uint32));
}
