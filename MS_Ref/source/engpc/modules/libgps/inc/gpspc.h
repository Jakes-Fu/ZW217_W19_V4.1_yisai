#ifndef __GPSPC_H__
#define __GPSPC_H__

int gnss_at_test(char *buf, char *rsp);
int gnss_diag_test(char *buf, int len, char *rsp, int rsplen);
int set_report_ptr(DYMIC_WRITETOPC_FUNC * write_interface_ptr);
int gnss_bbat_test(char *buf, int len, char *rsp, int rsplen);
int eng_diag_gps_assert_func(char *buf, int len, char *rsp, int rsplen);

#endif