#ifndef _ALIPAY_VENDOR_H_
#define _ALIPAY_VENDOR_H_
#ifndef WIN32
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#endif
#include "alipay_common.h"

typedef enum {
    // for device name now
    ALIPAY_ITEM_SHARE_KEY   = 0x01 << 0,
    // encrypted seed from server side
    ALIPAY_ITEM_SEED        = 0x01 << 1,
    // time diff between iot client and server
    ALIPAY_ITEM_TIMEDIFF    = 0x01 << 2,
    // nick name for user
    ALIPAY_ITEM_NICKNAME    = 0x01 << 3,
    // logon ID for user
    ALIPAY_ITEM_LOGONID     = 0x01 << 4,
    // logon uid for user
    ALIPAY_ITEM_UID         = 0x01 << 5,
    // logon Tid for user
    ALIPAY_ITEM_TID         = 0x01 << 6,
    // binding flag, see binding_status_e for value
    ALIPAY_ITEM_BINDFLAG    = 0x01 << 7,
    ALIPAY_ITEM_DEVICE_NAME    = 0x01 << 8,
} ALIPAY_storage_item;
	

#include "cJSON.h"
#define ALI_JSON cJSON

#define ALI_JSON_CreateObject()    cJSON_CreateObject()
#define ALI_JSON_CreateArray()     cJSON_CreateArray()
#define ALI_JSON_CreateStringArray(strings,counts)    cJSON_CreateStringArray(strings,counts)
#define ALI_JSON_GetObjectItem(object,name)              cJSON_GetObjectItem(object,name)
#define ALI_JSON_GetArrayItem(array,item)    cJSON_GetArrayItem(array,item)

#define ALI_JSON_AddNullToObject(object,name)		cJSON_AddItemToObject(object, name, cJSON_CreateNull())
#define ALI_JSON_AddTrueToObject(object,name)		cJSON_AddItemToObject(object, name, cJSON_CreateTrue())
#define ALI_JSON_AddFalseToObject(object,name)		cJSON_AddItemToObject(object, name, cJSON_CreateFalse())
#define ALI_JSON_AddBoolToObject(object,name,b)		cJSON_AddItemToObject(object, name, cJSON_CreateBool(b))
#define ALI_JSON_AddNumberToObject(object,name,n)	cJSON_AddItemToObject(object, name, cJSON_CreateNumber(n))
#define ALI_JSON_AddStringToObject(object,name,s)	cJSON_AddItemToObject(object, name, cJSON_CreateString(s))
#define ALI_JSON_AddItemToObject(object,name,item)	cJSON_AddItemToObject(object, name, item)
#define ALI_JSON_AddItemToArray(arr,item)	        cJSON_AddItemToArray(arr,item)

#define ALI_JSON_PrintUnformatted(obj)		cJSON_PrintUnformatted(obj)
#define ALI_JSON_Delete(obj)				cJSON_Delete(obj)
#define ALI_JSON_free(P)    				cJSON_free(P)
#define ALI_JSON_Parse(p)                   cJSON_Parse(p)

#define ALI_JSON_IsObject(p) cJSON_IsObject(p)
#define ALI_JSON_IsString(p) cJSON_IsString(p)

#define ALI_JSON_False	cJSON_False
#define ALI_JSON_True	cJSON_True

#if defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

/////////////////////////////////////////////
////��ȡ�豸mac��ַ(��ð�ŷָ��16����mac��ַ�ַ���)
////Ҫ�������ԡ�\0����β�ҳ��Ȳ�����'\0'
/////////////////////////////////////////////
extern retval_e alipay_get_mac(uint8_t* buf_did, uint32_t *len_did);

/////////////////////////////////////////////
////��ȡ�豸SN(����ӡˢ���豸�ϵ��豸���к�)
////���Ȳ�����49���ַ���ֻ�ܰ�����Сд��ĸ�����֡��»���
////Ҫ�������ԡ�\0����β�ҳ��Ȳ�����'\0'
////��ȡʧ�ܷ���RV_IO_ERROR
/////////////////////////////////////////////
extern retval_e alipay_get_sn(char *out, size_t *len);


//��ȡ�豸IMEI(�ַ�����ʽ)
extern retval_e alipay_get_imei(char *out, size_t *len);     // �豸imei

//��ȡ�豸�ͺ�(�ַ������,���Ȼ�Ӱ�����ĳ��ȣ�����20֮��. ��ʽ�ο�����
extern retval_e alipay_get_model(char *out, size_t *len);        // �豸�ͺ�

extern retval_e alipay_get_product_key(char *out, size_t *len); 

/////////////////////////////////////////////
////��ȫ�洢:��ȡ
////�TΪ��ֹ�洢�ƻ�����Ҫ����У���
/// ����ֵ��
/// RV_OK����ȡ����
/// RV_READ_ERROR����ȡʧ�Q
/// RV_NOT_FOUND����Ӧ�����Ҳ���
/////////////////////////////////////////////
 extern retval_e alipay_storage_read(ALIPAY_storage_item item,
                                   uint8_t *buf_content, uint32_t *len_content);

/////////////////////////////////////////////
////��ȫ�洢:д��
////�T ����ʵ���豸��ؼӽ��ܣ���д�뵽A�豸
////�����ݣ���ʹ������B�豸��Ҳ�޷�����ʹ��
////����Ϊ��ֹ�洢�ƻ�����Ҫ����У���
/// ����ֵ��
/// RV_OK��д�����
/// RV_WRITE_ERROR��д��ʧ�Q
/////////////////////////////////////////////
 extern retval_e alipay_storage_write(ALIPAY_storage_item item,
                                    const uint8_t *buf_content, uint32_t len_content);

/////////////////////////////////////////////
////��ȫ�洢:ɾ��(��Ҫ֧��һ��ɾ�����item,
////�BALIPAY_ITEM_PRIVATE_KEY | ALIPAY_ITEM_SEED)
/// ����ֵ��
/// RV_OK��ɾ������
/// RV_DEL_ERROR��ɾ��ʧ�Q
/////////////////////////////////////////////
 extern retval_e alipay_storage_delete(uint32_t items);


/////////////////////////////////////////////
////��ȫ�洢:�ж��Ƿ�����ļ�
/// ����ֵ��
/// RV_OK�����
/// RV_NOT_FOUND��������
/////////////////////////////////////////////
extern retval_e alipay_storage_exists(ALIPAY_storage_item item);

extern void *alipay_iot_malloc(size_t size);

/////////////////////////////////////////////
////��ȡ��ǰϵͳunixʱ�������λs
/////////////////////////////////////////////
extern uint32_t alipay_get_timestamp(void);

/////////////////////////////////////////////
////����ʵ�ֵ�֪ͨ��Ϣ���սӿڣ��˽ӿڱ�����֮��˵�����Թرհ���չʾ����ʼ��ʾ�󶨽���?
/// statusΪ����ֵ�İٷֱȣ���ʱֻ��70һ�����ȱ�
/////////////////////////////////////////////
extern void alipay_binding_process_notify(int status);

/////////////////////////////////////////////
////����ʵ�ֵ��豸���Ͷ���ӿ�
/// ����ֵΪ �豸����
///    prd_type_watch=0, //�����ֱ�
///    prd_type_b6t,   //�����ֻ�
///    prd_type_card,  //���˿�Ƭ
///    prd_type_qzwt,  //��ͯ�ֱ�
///    prd_type_qzb6t, //��ͯ�ֻ�
///    prd_type_qzcrd, //��ͯ��Ƭ
/////////////////////////////////////////////
extern prd_type_e alipay_get_prd_type(void);
// {
//     return prd_type_watch;
// }

/////////////////////////////////////////////
////��־��Ϣ����ӿ�
/////////////////////////////////////////////
typedef enum {
    ALIPAY_LEVEL_FATAL = 0x01,
    ALIPAY_LEVEL_ERRO,
    ALIPAY_LEVEL_WARN,
    ALIPAY_LEVEL_INFO,
    ALIPAY_LEVEL_DBG,
} ALIPAY_log_level;

///////////////////////////////////////////////
// ֻ�����һЩ�ؼ���trace log, ���ڷ�������ʱ���ٶ�??
///////////////////////////////////////////////
void alipay_print_log(ALIPAY_log_level level, const char *format, ...);

///////////////////////////////////////////////
// ms��˯�ߺ�??
///////////////////////////////////////////////
extern void alipay_task_sleep(uint32_t ms);


#if defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
