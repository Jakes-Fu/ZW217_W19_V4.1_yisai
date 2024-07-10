#ifndef __WLDT_CY_H__
#define __WLDT_CY_H__

#include "wldt_cy_api.h"
#include "wldt_cy_net.h"
#include "wldt_cy_func.h"
#include "wldt_cy_db.h"

typedef struct _WLDT_CY_DATA_t
{
    uint16 type;
    void* data_p;
} WLDT_CY_DATA_T;

typedef struct
{
    SIGNAL_VARS
    WLDT_CY_DATA_T * p_data;
} WLDT_CY_DATA_SIG_T;

typedef struct CY_APP_T {
    WLDT_TCP_INTERFACE_T * m_cy_tcp_reg_interface;
// Common
    BOOLEAN         m_cy_net_is_open;
    BOOLEAN         m_cy_net_is_init;
    BOOLEAN         m_cy_net_first_reg;
    BOOLEAN         m_cy_is_reg;
    BOOLEAN         m_cy_allow_rereg;
    BOOLEAN         m_cy_is_land;
    uint32           m_cy_lk_send_time;
    uint32           m_cy_reg_ip;
    uint8          m_cy_reg_err_num;
    uint16         m_cy_reg_port;
    uint16         m_cy_rsp_err_num;
    BOOLEAN         m_cy_test_stop;
//End Common

    uint8                       m_cy_reg_timer_id;
    uint8                       m_cy_land_timer_id;
    uint8                       m_cy_heart_timer_id;
    uint8                       m_cy_find_timer_id;
    uint8                       m_cy_cr_timer_id;
    uint8                       m_cy_pos_timer_id;
    uint8                       m_cy_gj_timer_id;
}CY_APP_T;

PUBLIC BOOLEAN  CY_PostEvtTo_APP(WLDT_APP_SIG_E sig_id, void* p_data, uint16 type);

extern MMI_RESULT_E CY_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param);
extern int CY_AppInit(void);
extern int CY_AppFree(void);
extern int CY_RestorFactory(void);

extern CY_APP_T  g_cy_app;
extern uint8  g_wldt_cy_imei_len;
#endif
