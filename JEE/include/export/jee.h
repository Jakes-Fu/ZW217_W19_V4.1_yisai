#ifndef _JEE_H
#define _JEE_H
#define JS_ART_RUNTIME_ERROR (-1)
#define JS_ART_APPINFO_ERROR (-2)
#define JS_ART_COMPATIBLE_ERROR (-3)
int jee_start_app(char* guid,char* para);
int jee_exit_app();
int jee_get_app_guid(char* buff, int len);
#endif /* _JEE_H */
