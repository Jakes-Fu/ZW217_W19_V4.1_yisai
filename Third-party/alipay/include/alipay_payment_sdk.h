#ifndef _ALIPAY_PAYMENT_SDK_H_
#define _ALIPAY_PAYMENT_SDK_H_
#include "alipay_common.h"



#if defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

/***
 * 预初始化接口，开机后网络功能和系统时间正常之后就可以先调用以准备环境，只有本接口返回RV_OK之后才可以使用sdk的其它接口
 * 开机后空闲时即可调用，避免等待时间过长
 * @return  RV_OK    初始化成功，可以进行支付宝业务接口调用
 *          若返回值为其他，则说明初始化失败，需要重新初始化。可能的失败原因如下：
 * --------------------------------------------------------------
 *          RV_NETWORK_ERROR   网络错误
 *          RV_UNKNOWN  未知错误
 */
retval_e alipay_pre_init(void);

/***
 * 获取当前的本地绑定状态，即判断当前设备是否已经有用户的绑定数据（无论服务端是否已解绑）
 * @return  true    已经有绑定数据
 *          false   无绑定数据
 */
bool alipay_get_bind_status(void);

/***
 * 获取绑定码
 * @param [out]result       绑定码字符串，长度根据厂商传入的设备型号（model）变化，型号最长20时，绑定码最长大约90个字符
 * @param [out]len_result   绑定码字符串的长度，传入时值应当是result buffer的长度
 * @return  RV_OK  获取绑定码成功，可以使用result输出的绑定码进行绑定
 *          若返回值为其他，则说明绑定码获取失败，需要重新获取。可能的失败原因如下：
 * --------------------------------------------------------------
 *          RV_WRONG_PARAM  输入参数不合法
 *          RV_SE_INFO_ERROR  内部错误
 *          RV_JS_ERROR  Json操作错误
 *          RV_SERVER_FAIL_ERROR  服务端返回错误
 *          RV_NETWORK_ERROR  网络错误
 *          RV_BUF_TOO_SHORT  result buffer长度不足
 *          RV_UNKNOWN  未知错误
 */
retval_e  alipay_get_binding_code(char* result, int *len_result);

/***
 * 轮询绑定结果
 * @return  RV_OK  绑定成功，数据已保存
 *          RV_PROCESSING  绑定过程进行中，请继续轮询绑定结果;
 *                         建议的轮询间隔为2s，超时时间建议为60s;超时后仍然返回processing，即可认为绑定失败
 *          RV_NETWORK_ERROR  网络错误  --此时建议重新查询
 *          若返回值为其他，则说明绑定失败，需要重新获取绑定码绑定。可能的失败原因如下：
 * --------------------------------------------------------------
 *          RV_NOT_INPROCESSING  未在绑定流程中（还没有调用alipay_get_binding_code获取绑定码）
 *			RV_SE_SAVE_ERROR  内部错误
 *          RV_JS_ERROR  Json操作错误
 *          RV_BINDING_DATA_FORMAT_ERROR  绑定数据格式错误
 *          RV_WRITE_ERROR  数据保存失败
 *          RV_SERVER_FAIL_ERROR  服务端返回错误
 *          RV_UNKNOWN  未知错误
 */
retval_e alipay_query_binding_result(void);

/***
 * 获取支付码
 * @param [out]paycode      支付码字符串(支付码为长度18或19的数字字符串，条形码编码和显示时应注意做好不同长度的兼容)
 * @param [out]len_paycode  支付码字符串的长度，传入时值应当是paycode buffer的长度
 * @return  RV_OK  获取支付码成功，可以使用paycode输出的支付码进行支付
 *          若返回值为其他，则说明支付码获取失败，需要重新获取。可能的失败原因如下：
 * -------------------------------------------------------------------
 *          RV_WRONG_PARAM  输入参数不合法
 *          RV_NOT_FOUND  找不到用户数据，可能为数据损坏或尚未绑定
 *          RV_READ_ERROR  数据读取失败
 *          RV_SE_GENCODE_ERROR  内部错误
 *          RV_BUF_TOO_SHORT  paycode buffer长度不足
 */
retval_e alipay_get_paycode(uint8_t *paycode, uint32_t *len_paycode);
/***
 * 获取收钱码，用法及返回值状态同alipay_get_paycode()
 * 收钱码为长度40或41的字符串，含有字母和数字
 */
retval_e alipay_get_collectioncode(uint8_t *code, uint32_t *len_code);

/***
 * 删除本地的用户绑定数据（无论服务端是否已解绑）
 * @return  RV_OK         删除成功
 *          RV_DEL_ERROR  删除失败。建议重试3次，如果还是失败，可能是存储器故障
 *			RV_SE_RESET_ERROR  内部错误。建议重试3次，如果还是失败，可能是存储器故障
 */
retval_e alipay_unbinding(void);

/***
 * 从Flash读取存储的logon_ID
 * @param [out] id 存放数据的指针
 * @param [out] len_id数据大小，最大读取不超过传入这个值的数据大小
 * @return  RV_OK：读取成功
 *          RV_NOT_FOUND：未发现该存储项
 *          RV_READ_ERROR  数据读取失败
 */
retval_e alipay_get_logon_ID(uint8_t *id, uint32_t *len_id);

/***
 * 从Flash读取存储的nick_name
 * @param [out] nick_name 存放数据的指针
 * @param [out] len_nick_name数据大小，最大读取不超过传入这个值的数据大小
 * @return  RV_OK：读取成功
 *          RV_NOT_FOUND：未发现该存储项
 *          RV_READ_ERROR  数据读取失败
 */
retval_e alipay_get_nick_name(uint8_t *nick_name, uint32_t *len_nick_name);

/***
 * 获取支付码支付结果
 * @param [out]status      支付码支付结果，定义见alipay_payment_status_e，可能的结果：
                           ALIPAY_PAYMENT_STATUS_SUCCESS  支付成功，此时bitmap_amount有金额输出
                           ALIPAY_PAYMENT_STATUS_NOTHING  没有查到交易，请继续使用本接口查询
                           付款码大约90秒过期，建议查询60秒后仍然没有结果，可以息屏或刷新付款码后重新查询
                           --------------------------其它输出均表示支付已经失败---------------------------
                           ALIPAY_PAYMENT_STATUS_DISABLED_BY_USER  付款码支付功能已经被用户关闭
                           ALIPAY_PAYMENT_STATUS_UNBIND_BY_USER  用户已在手机端解绑本设备
 * @param [out]bitmap_amount  支付金额的bitmap，二维数组，直接从左至右逐bit点阵输出即可（黑色bit为1，白色为0）
 * @return  RV_OK  查询接口调用成功，此时会输出status和bitmap_amount
 *          若返回值为其他，则说明查询接口调用失败。可能的失败原因如下：
 * -------------------------------------------------------------------
 *          RV_IO_ERROR 获取设备本地信息失败
 *          RV_WRONG_PARAM  输入参数不合法
 *          RV_NOT_INPROCESSING 当前没有获取支付码，无法查询支付结果
 *          RV_READ_ERROR  本地数据读取失败，可能是数据损坏或已解绑
            -----------以下错误建议重试------------
 *          RV_UNKNOWN  未知错误
 *          RV_JS_ERROR  json操作异常-可能是内存不足
 *          RV_NETWORK_ERROR  网络异常
 */
retval_e alipay_query_order_result(alipay_payment_status_e *status, char bitmap_amount[32][14]);

/***
 * 主扫时解析扫到的收钱码
 * @param [in]code      摄像头扫描到的商家收钱码
 * @param [out]seller_name      商家名字
 * @param [in]len_seller_name_buff      seller_name buffer的长度，建议>=100字节
 * @param [out]amount      如果收钱码是预设有交易金额的，将会在此字段输出，后续直接展示本金额，无需用户输入。禁止改动本金额
 * @param [in]len_amount_buff  amount buffer的长度，建议>=10字节
 * @return  RV_OK  解析接口调用成功，此时会输出seller_name和amount
 *          若返回值为其他，则说明接口调用失败。可能的失败原因如下：
 * -------------------------------------------------------------------
 *          RV_IO_ERROR 获取设备本地信息失败
 *          RV_WRONG_PARAM  参数不合法
 *          RV_READ_ERROR  本地数据读取失败，可能是数据损坏或已解绑
 *          RV_JS_ERROR  json操作异常-可能是内存不足
 *          RV_NETWORK_ERROR  网络异常 -->提示：网络异常，请重试
 *          RV_SERVER_FAIL_ERROR 码值无法解析 -->提示：二维码无法识别
 *          RV_UNKNOWN  未知错误
 */
retval_e alipay_scan_code_parse(char* code, char* seller_name, uint32_t len_seller_name_buff, char* amount, uint32_t len_amount_buff);

/***
 * 发起主扫支付。注意：alipay_scan_code_parse成功之后才可以调用
 * @param [in]amount  支付金额。字符串格式，从范围0.01-1000元，小数点后最多保留2位小数
 * @param [out]status      主扫支付结果，定义见alipay_payment_status_e，可能的结果：
                           ALIPAY_PAYMENT_STATUS_SUCCESS  支付成功
                           ALIPAY_PAYMENT_STATUS_FAIL     支付失败，此时res_fail有失败原因输出，根据输出进行报错
                           ALIPAY_PAYMENT_STATUS_UNKNOWN  结果未知，请用户与商家确认结果
 * @param [out]res_fail    主扫支付失败原因，定义见alipay_scan_pay_fail_res_e，可能的结果：
                           ALIPAY_FAIL_RES_AMOUNT_LIMIT       超出设置的支付额度 - 报错文案：超出设置的支付额度
                           ALIPAY_FAIL_RES_BALANCE_NOT_ENOUGH 余额不足               - 报错文案：零花钱不足，请存入
                           ALIPAY_FAIL_RES_COMMON_FAIL        其它通用的失败case - 报错文案：支付失败
 * @return  RV_OK  支付接口调用成功，此时会输出status和res_fail
 *          若返回值为其他，则说明接口调用失败。可能的失败原因如下：
 * -------------------------------------------------------------------
 *          RV_IO_ERROR 获取设备本地信息失败
 *          RV_WRONG_PARAM  输入参数不合法
 *          RV_NOT_FOUND  未发现绑定数据，设备还没有绑定
 *          RV_SE_GENCODE_ERROR 内部错误
 *          RV_UNKNOWN  未知错误
 *          RV_JS_ERROR  json操作异常-可能是内存不足
 *          RV_NETWORK_ERROR  网络异常 -->提示：网络异常，请向商家确认交易结果
 */
retval_e alipay_scan_create_payment(const char *amount, alipay_payment_status_e* status, alipay_scan_pay_fail_res_e* res_fail);



/***
 * 与服务端同步时间(同步结果可能因为网络原因失败，不需要处理)(可以不用或空闲时调用)
 * @return  RV_OK         成功
 *          其他           失败
 */
retval_e alipay_try_sync_time_network(void);

/***
 * 打断付款码支付结果query接口的执行，让接口立即返回
 * 注意：只允许用来打断alipay_query_order_result接口，请勿在其它接口执行时调用
 */
void alipay_query_flow_break(void);


#if defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
