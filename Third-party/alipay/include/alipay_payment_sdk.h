#ifndef _ALIPAY_PAYMENT_SDK_H_
#define _ALIPAY_PAYMENT_SDK_H_
#include "alipay_common.h"



#if defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

/***
 * Ԥ��ʼ���ӿڣ����������繦�ܺ�ϵͳʱ������֮��Ϳ����ȵ�����׼��������ֻ�б��ӿڷ���RV_OK֮��ſ���ʹ��sdk�������ӿ�
 * ���������ʱ���ɵ��ã�����ȴ�ʱ�����
 * @return  RV_OK    ��ʼ���ɹ������Խ���֧����ҵ��ӿڵ���
 *          ������ֵΪ��������˵����ʼ��ʧ�ܣ���Ҫ���³�ʼ�������ܵ�ʧ��ԭ�����£�
 * --------------------------------------------------------------
 *          RV_NETWORK_ERROR   �������
 *          RV_UNKNOWN  δ֪����
 */
retval_e alipay_pre_init(void);

/***
 * ��ȡ��ǰ�ı��ذ�״̬�����жϵ�ǰ�豸�Ƿ��Ѿ����û��İ����ݣ����۷�����Ƿ��ѽ��
 * @return  true    �Ѿ��а�����
 *          false   �ް�����
 */
bool alipay_get_bind_status(void);

/***
 * ��ȡ����
 * @param [out]result       �����ַ��������ȸ��ݳ��̴�����豸�ͺţ�model���仯���ͺ��20ʱ���������Լ90���ַ�
 * @param [out]len_result   �����ַ����ĳ��ȣ�����ʱֵӦ����result buffer�ĳ���
 * @return  RV_OK  ��ȡ����ɹ�������ʹ��result����İ�����а�
 *          ������ֵΪ��������˵�������ȡʧ�ܣ���Ҫ���»�ȡ�����ܵ�ʧ��ԭ�����£�
 * --------------------------------------------------------------
 *          RV_WRONG_PARAM  ����������Ϸ�
 *          RV_SE_INFO_ERROR  �ڲ�����
 *          RV_JS_ERROR  Json��������
 *          RV_SERVER_FAIL_ERROR  ����˷��ش���
 *          RV_NETWORK_ERROR  �������
 *          RV_BUF_TOO_SHORT  result buffer���Ȳ���
 *          RV_UNKNOWN  δ֪����
 */
retval_e  alipay_get_binding_code(char* result, int *len_result);

/***
 * ��ѯ�󶨽��
 * @return  RV_OK  �󶨳ɹ��������ѱ���
 *          RV_PROCESSING  �󶨹��̽����У��������ѯ�󶨽��;
 *                         �������ѯ���Ϊ2s����ʱʱ�佨��Ϊ60s;��ʱ����Ȼ����processing��������Ϊ��ʧ��
 *          RV_NETWORK_ERROR  �������  --��ʱ�������²�ѯ
 *          ������ֵΪ��������˵����ʧ�ܣ���Ҫ���»�ȡ����󶨡����ܵ�ʧ��ԭ�����£�
 * --------------------------------------------------------------
 *          RV_NOT_INPROCESSING  δ�ڰ������У���û�е���alipay_get_binding_code��ȡ���룩
 *			RV_SE_SAVE_ERROR  �ڲ�����
 *          RV_JS_ERROR  Json��������
 *          RV_BINDING_DATA_FORMAT_ERROR  �����ݸ�ʽ����
 *          RV_WRITE_ERROR  ���ݱ���ʧ��
 *          RV_SERVER_FAIL_ERROR  ����˷��ش���
 *          RV_UNKNOWN  δ֪����
 */
retval_e alipay_query_binding_result(void);

/***
 * ��ȡ֧����
 * @param [out]paycode      ֧�����ַ���(֧����Ϊ����18��19�������ַ�����������������ʾʱӦע�����ò�ͬ���ȵļ���)
 * @param [out]len_paycode  ֧�����ַ����ĳ��ȣ�����ʱֵӦ����paycode buffer�ĳ���
 * @return  RV_OK  ��ȡ֧����ɹ�������ʹ��paycode�����֧�������֧��
 *          ������ֵΪ��������˵��֧�����ȡʧ�ܣ���Ҫ���»�ȡ�����ܵ�ʧ��ԭ�����£�
 * -------------------------------------------------------------------
 *          RV_WRONG_PARAM  ����������Ϸ�
 *          RV_NOT_FOUND  �Ҳ����û����ݣ�����Ϊ�����𻵻���δ��
 *          RV_READ_ERROR  ���ݶ�ȡʧ��
 *          RV_SE_GENCODE_ERROR  �ڲ�����
 *          RV_BUF_TOO_SHORT  paycode buffer���Ȳ���
 */
retval_e alipay_get_paycode(uint8_t *paycode, uint32_t *len_paycode);
/***
 * ��ȡ��Ǯ�룬�÷�������ֵ״̬ͬalipay_get_paycode()
 * ��Ǯ��Ϊ����40��41���ַ�����������ĸ������
 */
retval_e alipay_get_collectioncode(uint8_t *code, uint32_t *len_code);

/***
 * ɾ�����ص��û������ݣ����۷�����Ƿ��ѽ��
 * @return  RV_OK         ɾ���ɹ�
 *          RV_DEL_ERROR  ɾ��ʧ�ܡ���������3�Σ��������ʧ�ܣ������Ǵ洢������
 *			RV_SE_RESET_ERROR  �ڲ����󡣽�������3�Σ��������ʧ�ܣ������Ǵ洢������
 */
retval_e alipay_unbinding(void);

/***
 * ��Flash��ȡ�洢��logon_ID
 * @param [out] id ������ݵ�ָ��
 * @param [out] len_id���ݴ�С������ȡ�������������ֵ�����ݴ�С
 * @return  RV_OK����ȡ�ɹ�
 *          RV_NOT_FOUND��δ���ָô洢��
 *          RV_READ_ERROR  ���ݶ�ȡʧ��
 */
retval_e alipay_get_logon_ID(uint8_t *id, uint32_t *len_id);

/***
 * ��Flash��ȡ�洢��nick_name
 * @param [out] nick_name ������ݵ�ָ��
 * @param [out] len_nick_name���ݴ�С������ȡ�������������ֵ�����ݴ�С
 * @return  RV_OK����ȡ�ɹ�
 *          RV_NOT_FOUND��δ���ָô洢��
 *          RV_READ_ERROR  ���ݶ�ȡʧ��
 */
retval_e alipay_get_nick_name(uint8_t *nick_name, uint32_t *len_nick_name);

/***
 * ��ȡ֧����֧�����
 * @param [out]status      ֧����֧������������alipay_payment_status_e�����ܵĽ����
                           ALIPAY_PAYMENT_STATUS_SUCCESS  ֧���ɹ�����ʱbitmap_amount�н�����
                           ALIPAY_PAYMENT_STATUS_NOTHING  û�в鵽���ף������ʹ�ñ��ӿڲ�ѯ
                           �������Լ90����ڣ������ѯ60�����Ȼû�н��������Ϣ����ˢ�¸���������²�ѯ
                           --------------------------�����������ʾ֧���Ѿ�ʧ��---------------------------
                           ALIPAY_PAYMENT_STATUS_DISABLED_BY_USER  ������֧�������Ѿ����û��ر�
                           ALIPAY_PAYMENT_STATUS_UNBIND_BY_USER  �û������ֻ��˽���豸
 * @param [out]bitmap_amount  ֧������bitmap����ά���飬ֱ�Ӵ���������bit����������ɣ���ɫbitΪ1����ɫΪ0��
 * @return  RV_OK  ��ѯ�ӿڵ��óɹ�����ʱ�����status��bitmap_amount
 *          ������ֵΪ��������˵����ѯ�ӿڵ���ʧ�ܡ����ܵ�ʧ��ԭ�����£�
 * -------------------------------------------------------------------
 *          RV_IO_ERROR ��ȡ�豸������Ϣʧ��
 *          RV_WRONG_PARAM  ����������Ϸ�
 *          RV_NOT_INPROCESSING ��ǰû�л�ȡ֧���룬�޷���ѯ֧�����
 *          RV_READ_ERROR  �������ݶ�ȡʧ�ܣ������������𻵻��ѽ��
            -----------���´���������------------
 *          RV_UNKNOWN  δ֪����
 *          RV_JS_ERROR  json�����쳣-�������ڴ治��
 *          RV_NETWORK_ERROR  �����쳣
 */
retval_e alipay_query_order_result(alipay_payment_status_e *status, char bitmap_amount[32][14]);

/***
 * ��ɨʱ����ɨ������Ǯ��
 * @param [in]code      ����ͷɨ�赽���̼���Ǯ��
 * @param [out]seller_name      �̼�����
 * @param [in]len_seller_name_buff      seller_name buffer�ĳ��ȣ�����>=100�ֽ�
 * @param [out]amount      �����Ǯ����Ԥ���н��׽��ģ������ڴ��ֶ����������ֱ��չʾ���������û����롣��ֹ�Ķ������
 * @param [in]len_amount_buff  amount buffer�ĳ��ȣ�����>=10�ֽ�
 * @return  RV_OK  �����ӿڵ��óɹ�����ʱ�����seller_name��amount
 *          ������ֵΪ��������˵���ӿڵ���ʧ�ܡ����ܵ�ʧ��ԭ�����£�
 * -------------------------------------------------------------------
 *          RV_IO_ERROR ��ȡ�豸������Ϣʧ��
 *          RV_WRONG_PARAM  �������Ϸ�
 *          RV_READ_ERROR  �������ݶ�ȡʧ�ܣ������������𻵻��ѽ��
 *          RV_JS_ERROR  json�����쳣-�������ڴ治��
 *          RV_NETWORK_ERROR  �����쳣 -->��ʾ�������쳣��������
 *          RV_SERVER_FAIL_ERROR ��ֵ�޷����� -->��ʾ����ά���޷�ʶ��
 *          RV_UNKNOWN  δ֪����
 */
retval_e alipay_scan_code_parse(char* code, char* seller_name, uint32_t len_seller_name_buff, char* amount, uint32_t len_amount_buff);

/***
 * ������ɨ֧����ע�⣺alipay_scan_code_parse�ɹ�֮��ſ��Ե���
 * @param [in]amount  ֧�����ַ�����ʽ���ӷ�Χ0.01-1000Ԫ��С�������ౣ��2λС��
 * @param [out]status      ��ɨ֧������������alipay_payment_status_e�����ܵĽ����
                           ALIPAY_PAYMENT_STATUS_SUCCESS  ֧���ɹ�
                           ALIPAY_PAYMENT_STATUS_FAIL     ֧��ʧ�ܣ���ʱres_fail��ʧ��ԭ�����������������б���
                           ALIPAY_PAYMENT_STATUS_UNKNOWN  ���δ֪�����û����̼�ȷ�Ͻ��
 * @param [out]res_fail    ��ɨ֧��ʧ��ԭ�򣬶����alipay_scan_pay_fail_res_e�����ܵĽ����
                           ALIPAY_FAIL_RES_AMOUNT_LIMIT       �������õ�֧����� - �����İ����������õ�֧�����
                           ALIPAY_FAIL_RES_BALANCE_NOT_ENOUGH ����               - �����İ����㻨Ǯ���㣬�����
                           ALIPAY_FAIL_RES_COMMON_FAIL        ����ͨ�õ�ʧ��case - �����İ���֧��ʧ��
 * @return  RV_OK  ֧���ӿڵ��óɹ�����ʱ�����status��res_fail
 *          ������ֵΪ��������˵���ӿڵ���ʧ�ܡ����ܵ�ʧ��ԭ�����£�
 * -------------------------------------------------------------------
 *          RV_IO_ERROR ��ȡ�豸������Ϣʧ��
 *          RV_WRONG_PARAM  ����������Ϸ�
 *          RV_NOT_FOUND  δ���ְ����ݣ��豸��û�а�
 *          RV_SE_GENCODE_ERROR �ڲ�����
 *          RV_UNKNOWN  δ֪����
 *          RV_JS_ERROR  json�����쳣-�������ڴ治��
 *          RV_NETWORK_ERROR  �����쳣 -->��ʾ�������쳣�������̼�ȷ�Ͻ��׽��
 */
retval_e alipay_scan_create_payment(const char *amount, alipay_payment_status_e* status, alipay_scan_pay_fail_res_e* res_fail);



/***
 * ������ͬ��ʱ��(ͬ�����������Ϊ����ԭ��ʧ�ܣ�����Ҫ����)(���Բ��û����ʱ����)
 * @return  RV_OK         �ɹ�
 *          ����           ʧ��
 */
retval_e alipay_try_sync_time_network(void);

/***
 * ��ϸ�����֧�����query�ӿڵ�ִ�У��ýӿ���������
 * ע�⣺ֻ�����������alipay_query_order_result�ӿڣ������������ӿ�ִ��ʱ����
 */
void alipay_query_flow_break(void);


#if defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
