
#ifndef LWM2MCLIENT_H_
#define LWM2MCLIENT_H_

#include "liblwm2m.h"
#include "internals.h"
#if 0 //modify by unisoc
#include <sys/stat.h>
#else
#include "os_api.h"
#endif

#ifdef LWM2M_WITH_LOGS
#define LINUX_MT_DM_DIR "/tmp/mt_dm_log"
#define LOG_M(FMT, ...) SCI_TraceLow("[lwm2m]%s:[%s: %d] " FMT "\r\n", lwm2m_getcurtime(), __FUNCTION__, __LINE__ , __VA_ARGS__)
#define LOG_S(STR)      LOG_M("%s",STR)
#if 0 //modify by unisoc
#define WRITE_LOG_START \
    do { \
        char path[60]; \
        mkdir(LINUX_MT_DM_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); \
        sprintf(path,"%s%s%s%s",LINUX_MT_DM_DIR,"/",lwm2m_getcurtime(),".log"); \
        freopen(path,"w",stderr); \
    }while(0)

#define WRITE_LOG_FLUSH fflush(stderr)
#define WRITE_LOG_EXIT fclose(stderr)
#else
#define WRITE_LOG_START
#define WRITE_LOG_FLUSH
#define WRITE_LOG_EXIT

#endif

#else
#define LOG_M(FMT, ...)
#define LOG_S(STR)
#define WRITE_LOG_START
#define WRITE_LOG_FLUSH
#define WRITE_LOG_EXIT
#endif

#define TEST_OBJECT_ID 31024

lwm2m_object_t * get_object_device(void);
void free_object_device(lwm2m_object_t * objectP);
lwm2m_object_t * get_server_object(int serverId,int lifetime);
void free_server_object(lwm2m_object_t * object);
lwm2m_object_t * get_security_object(int serverId,const char* serverUri);
void free_security_object(lwm2m_object_t * objectP);

lwm2m_object_t * get_object_dm(void);
void free_object_dm(lwm2m_object_t * objectP);


char * get_server_uri(lwm2m_object_t * objectP, uint16_t secObjInstID);


#endif /* LWM2MCLIENT_H_ */
