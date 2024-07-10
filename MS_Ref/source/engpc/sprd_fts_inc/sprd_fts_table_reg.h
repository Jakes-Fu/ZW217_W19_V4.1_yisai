#include "reg_type.h"

//#include "ftmode.h"

#include "gpspc.h"

#if 0
#include "bt_eut.h"

#include "storage_autotest.h"

#if defined(SPRD_LOAD_FIXNV_SUPPORT)
#include "read_fixed_nv.h"
#endif

#if defined(SPRD_NVITEM_F_SUPPORT)
#include "nvitem_f.h"
#endif

#if defined(CONFIG_WIFI_STA_AT)
#include "wifi_eut.h"
#endif

#include "charge_autotest.h"

#include "deepsleep_autotest.h"

#include "spref.h"

#include "chip_id_check.h"

#if defined(SPRD_MARLIN2_INTEG_SUPPORT)
#include "wcn_assert.h"
#endif

#include "audio4ap.h"

#include "efusereaduid.h"

// libfactoryreset header
#include "libfactoryreset.h"

#include "tfcardtest.h"

#include "msensor_apitest.h"

#include "wificoup.h"

#include "coupletest.h"

#include "efusecheck.h"

#include "simcard_autotest.h"

#include "backupcali.h"

#include "udb.h"

#include "reboot.h"

#include "task_monitor.h"

#if defined(UTEST_SUPPORT)
#include "engpc_utest.h"
#endif
/*--------------------File system operation-------------------*/
//#include "filesystem_atcmd.h"

#if defined(UAL_FACTORY_TEST_SUPPORT)
#include "ual_autotest_factory_test_ble.h"
#endif

#endif

/*
register diag callback like this:

DIAG_REG(type, subtype, eng_diag_func)
DIAG_REG_LOW(type, subtype, diag_ap_cmd, also_need_to_cp, eng_diag_func)
DIAG_REG_FULL(type, subtype, diag_ap_cmd, also_need_to_cp, eng_diag_func, eng_set_writeinterface_func, eng_cmd_match)

register at callback like this:

AT_REG(at_cmd, eng_linuxcmd_func)

*/

extern int eng_diag_ap_assert_func(char *buf, int len, char *rsp, int rsplen);

extern int eng_at_wificonn_func(char *req, char *rsp);
extern int eng_at_wifidisconn_func(char *req, char *rsp);
extern int eng_at_testping_func(char *req, char *rsp);
extern int eng_at_getip_func(char *req, char *rsp);
extern int eng_at_meminfo_func(char *req, char *rsp);
extern int test_eng_diag_wifi_test(char *buf, int len, char *rsp, int rsplen);
extern int eng_at_createfile_func(char *req, char *rsp);
extern int eng_at_statfile_func(char *req, char *rsp);
extern int eng_at_set_testpoint(char *req, char *rsp);
extern int eng_at_subsystem_info(char *req, char *rsp);

#if defined(USH_SUPPORT)
extern int eng_at_ush_info(char *req, char *rsp);
#endif

#if 1
TABLE_DIAG_AT_REG_BEGIN(s_table_reg_diag_at)
DIAG_REG(0x5, 0x8, eng_diag_ap_assert_func)

#include "gpspc.tl"

TABLE_DIAG_AT_REG_END()

#else
TABLE_DIAG_AT_REG_BEGIN(s_table_reg_diag_at)
DIAG_REG(0x5, 0x8, eng_diag_ap_assert_func)
AT_REG("AT+WIFICONN", eng_at_wificonn_func)
AT_REG("AT+WIFIDISCONN", eng_at_wifidisconn_func)
AT_REG("AT+SPGETIP", eng_at_getip_func)
AT_REG("at+ping", eng_at_testping_func)
AT_REG("at+mallocinfo", eng_at_meminfo_func)
AT_REG("at+createfile", eng_at_createfile_func)
AT_REG("at+statfile", eng_at_statfile_func)
AT_REG("AT+SETTESTPOINT=", eng_at_set_testpoint)
AT_REG("AT+SUBSYSTEM=", eng_at_subsystem_info)

#if defined(USH_SUPPORT)
AT_REG("AT+USH=", eng_at_ush_info)
#endif

DIAG_REG(0x0D, 0xAE, test_eng_diag_wifi_test)
// libftmode register here
#include "ftmode.tl"

#include "gpspc.tl"

#include "bt_eut.tl"

// libreadfixednv register here
#if defined(SPRD_LOAD_FIXNV_SUPPORT)
#include "read_fixed_nv.tl"
#endif

// libnvitemf register here
#if defined(SPRD_NVITEM_F_SUPPORT)
#include "nvitem_f.tl"
#endif
// libwifieut register here
#if defined(CONFIG_WIFI_STA_AT)
#include "wifi_eut.tl"
#endif

//libcharge_autotest register here
#include "charge_autotest.tl"

#include "deepsleep_autotest.tl"

//libspref register here
#include "spref.tl"

//libwcnassert register here
#if defined(SPRD_MARLIN2_INTEG_SUPPORT)
#include "wcn_assert.tl"
#endif

//libaudio4ap register here
#include "audio4ap.tl"

//efuse read uid register here
#include "efusereaduid.tl"

// libfactoryreset register here
#include "libfactoryreset.tl"

#include "tfcardtest.tl"

//msensor_apitest register here
#include "msensor_apitest.tl"

//storage_autotest register here
#include "storage_autotest.tl"

#include "wificoup.tl"

#include "coupletest.tl"

//efusecheck register here
#include "efusecheck.tl"
//chip id register here
#include "chip_id_check.tl"

//simcard_autotest register here
#include "simcard_autotest.tl"

#include "backupcali.tl"

//udb_util register here
#include "udb.tl"

/*---------------File system operation--------------*/
#include "filesystem_atcmd.tl"

// libreboot register here
#include "reboot.tl"
#if defined(UAL_FACTORY_TEST_SUPPORT)
#include "ual_autotest_factorytest.tl"
#endif

#if defined(UTEST_SUPPORT)
#include "engpc_utest.tl"
#endif


TABLE_DIAG_AT_REG_END()
#endif
