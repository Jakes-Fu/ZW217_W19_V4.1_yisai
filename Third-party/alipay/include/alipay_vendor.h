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
////获取设备mac地址(以冒号分割的16进制mac地址字符串)
////要求内容以‘\0’结尾且长度不包含'\0'
/////////////////////////////////////////////
extern retval_e alipay_get_mac(uint8_t* buf_did, uint32_t *len_did);

/////////////////////////////////////////////
////获取设备SN(厂商印刷在设备上的设备序列号)
////长度不超过49个字符，只能包含大小写字母、数字、下划线
////要求内容以‘\0’结尾且长度不包含'\0'
////获取失败返回RV_IO_ERROR
/////////////////////////////////////////////
extern retval_e alipay_get_sn(char *out, size_t *len);


//获取设备IMEI(字符串格式)
extern retval_e alipay_get_imei(char *out, size_t *len);     // 设备imei

//获取设备型号(字符串格庿,长度会影响绑定码的长度，建议20之内. 格式参考文柿
extern retval_e alipay_get_model(char *out, size_t *len);        // 设备型号

extern retval_e alipay_get_product_key(char *out, size_t *len); 

/////////////////////////////////////////////
////安全存储:读取
////汿为防止存储破坏，需要加上校验便
/// 返回值：
/// RV_OK：读取成刿
/// RV_READ_ERROR：读取失貿
/// RV_NOT_FOUND：对应数据找不到
/////////////////////////////////////////////
 extern retval_e alipay_storage_read(ALIPAY_storage_item item,
                                   uint8_t *buf_content, uint32_t *len_content);

/////////////////////////////////////////////
////安全存储:写入
////汿 必须实现设备相关加解密，即写入到A设备
////的内容，即使拷贝到B设备，也无法正常使用
////此外为防止存储破坏，需要加上校验便
/// 返回值：
/// RV_OK：写入成刿
/// RV_WRITE_ERROR：写入失貿
/////////////////////////////////////////////
 extern retval_e alipay_storage_write(ALIPAY_storage_item item,
                                    const uint8_t *buf_content, uint32_t len_content);

/////////////////////////////////////////////
////安全存储:删除(需要支持一次删除多个item,
////夿ALIPAY_ITEM_PRIVATE_KEY | ALIPAY_ITEM_SEED)
/// 返回值：
/// RV_OK：删除成刿
/// RV_DEL_ERROR：删除失貿
/////////////////////////////////////////////
 extern retval_e alipay_storage_delete(uint32_t items);


/////////////////////////////////////////////
////安全存储:判断是否存在文件
/// 返回值：
/// RV_OK：存嚿
/// RV_NOT_FOUND：不存在
/////////////////////////////////////////////
extern retval_e alipay_storage_exists(ALIPAY_storage_item item);

extern void *alipay_iot_malloc(size_t size);

/////////////////////////////////////////////
////获取当前系统unix时间戳，单位s
/////////////////////////////////////////////
extern uint32_t alipay_get_timestamp(void);

/////////////////////////////////////////////
////厂商实现的通知信息接收接口，此接口被调用之后，说明可以关闭绑定码展示，开始显示绑定进度?
/// status为进度值的百分比，暂时只有70一个进度便
/////////////////////////////////////////////
extern void alipay_binding_process_notify(int status);

/////////////////////////////////////////////
////厂商实现的设备类型定义接口
/// 返回值为 设备类型
///    prd_type_watch=0, //成人手表
///    prd_type_b6t,   //成人手环
///    prd_type_card,  //成人卡片
///    prd_type_qzwt,  //儿童手表
///    prd_type_qzb6t, //儿童手环
///    prd_type_qzcrd, //儿童卡片
/////////////////////////////////////////////
extern prd_type_e alipay_get_prd_type(void);
// {
//     return prd_type_watch;
// }

/////////////////////////////////////////////
////日志信息输出接口
/////////////////////////////////////////////
typedef enum {
    ALIPAY_LEVEL_FATAL = 0x01,
    ALIPAY_LEVEL_ERRO,
    ALIPAY_LEVEL_WARN,
    ALIPAY_LEVEL_INFO,
    ALIPAY_LEVEL_DBG,
} ALIPAY_log_level;

///////////////////////////////////////////////
// 只会输出一些关键的trace log, 便于发现问题时快速定??
///////////////////////////////////////////////
void alipay_print_log(ALIPAY_log_level level, const char *format, ...);

///////////////////////////////////////////////
// ms级睡眠函??
///////////////////////////////////////////////
extern void alipay_task_sleep(uint32_t ms);


#if defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
