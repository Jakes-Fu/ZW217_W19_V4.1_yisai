#ifndef __ZDT_HYHL_API_H__
#define __ZDT_HYHL_API_H__

#include "zdt_common.h"
#include "zdt_tcp.h"

typedef struct _HYHL_APP_t {
    ZDT_TCP_INTERFACE_T * m_hyhl_tcp_reg_interface;
    ZDT_TCP_INTERFACE_T * m_hyhl_tcp_land_interface;
    uint8                       m_hyhl_reg_timer_id;
    uint8                       m_hyhl_land_timer_id;
    uint8                       m_hyhl_heart_timer_id;
    uint8                       m_hyhl_1a_timer_id;
    BOOLEAN         m_hyhl_net_is_open;
    BOOLEAN         m_hyhl_net_is_init;
    BOOLEAN         m_hyhl_is_first_reg;
    BOOLEAN         m_hyhl_is_reg;
    BOOLEAN         m_hyhl_allow_rereg;
    BOOLEAN         m_hyhl_is_land;
    uint32           m_hyhl_reg_ip;
    uint8          m_hyhl_reg_err_num;
    uint16         m_hyhl_reg_port;
    uint16         m_hyhl_rsp_err_num;
    BOOLEAN         m_hyhl_test_stop;

#ifdef ZDT_PLAT_HYHL_SOS_CALL
    uint16 m_hyhl_cur_sos_idx;
#endif
#ifdef ZDT_PLAT_HYHL_WHITE_LIST
    uint16 m_hyhl_cur_whitelist_idx;
#endif
}HYHL_APP_T;

extern HYHL_APP_T  g_hyhl_app;

#endif
