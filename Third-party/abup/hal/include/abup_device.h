#ifndef _ABUP_DEVICE_H_
#define _ABUP_DEVICE_H_

abup_extern  abup_int32 abup_sim_ready(void);

abup_extern abup_int32 abup_get_device_mid(abup_char* mid);

abup_extern abup_char* abup_get_device_version(void);

abup_extern abup_char* abup_get_device_oem(void);

abup_extern abup_char* abup_get_device_model(void);

abup_extern abup_char* abup_get_device_product_id(void);

abup_extern abup_char* abup_get_device_product_sec(void);

abup_extern abup_char* abup_get_device_type(void);

abup_extern abup_char* abup_get_device_platform(void);

abup_extern abup_char* abup_get_device_network_type(void);

abup_extern void abup_get_device_time(abup_time_struct* time);
abup_extern abup_uint32 abup_get_utc_time_sec(abup_time_struct* time);


abup_extern abup_uint8 abup_get_device_battery_level(void);

abup_extern void abup_device_start_reboot(void);

abup_extern void* abup_memalloc(abup_int32 size);
abup_extern void abup_memfree(void *p);
abup_extern void* abup_memalloc_ext(abup_int32 size);
abup_extern void abup_memfree_ext(void *p);

abup_extern void abup_get_device_data_point1(abup_char** name,abup_char** value);
abup_extern void abup_get_device_data_point2(abup_char* name,abup_char* value);
abup_extern void abup_get_device_data_point3(abup_char* name,abup_char* value);
abup_extern void abup_get_device_data_point4(abup_char* name,abup_char* value);
abup_extern void abup_get_device_data_point5(abup_char* name,abup_char* value);

abup_extern void abup_process_status_cb(abup_int32 status,abup_uint8 fota_status);
abup_extern void abup_em_set_mode(ABUP_BOOL mode);
abup_extern ABUP_BOOL abup_em_mode(void);
abup_extern void abup_em_set_mid(abup_char* mid);
abup_extern void abup_em_get_mid(abup_char* mid);
abup_uint32 abup_net_separate_size(void);

#endif
