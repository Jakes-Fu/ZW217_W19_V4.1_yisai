
#include "zfb_app.h"
#include "alipay_vendor.h"
#include "wifi_drv.h"
#include "Bt_abs.h"
#include "Bt_cfg.h"
#include "bt_abs.h"
#include "production_test.h"
#include "mmi_filemgr.h"

void alipay_print_log(ALIPAY_log_level level, const char *format, ...)
{
    int len = 0;
    char buf[1024] = {0};
    va_list arg;
    va_start(arg, format);
    #ifdef WIN32
    len = vsprintf(buf,format, arg);    
   #else
    len = vsnprintf(buf, 1023,format, arg);    
    #endif
    if(len > 0)
    {
        ZFB_AlipayTraceExt(buf,len);
    }
    va_end(arg);
}
prd_type_e alipay_get_prd_type(void)
{
    return prd_type_qzwt;
}

void alipay_binding_process_notify(int status)
{
    ZFBAPP_TRACE("alipay_binding_process_notify status=%d",status);
    ZFBPay_BindingStart();
    return;
}

extern void alipay_task_sleep(uint32_t ms)
{
    SCI_Sleep(ms);
    return;
}

