#ifndef __LEBAO_WIN_ORDER_H
#define __LEBAO_WIN_ORDER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
*      INCLUDES
*********************/

/*********************
*      DEFINES
*********************/

/**********************
*      TYPEDEFS
**********************/
typedef enum
{
	LEBAO_ORDER_OPT_NODE_NULL,
	LEBAO_ORDER_OPT_NODE_PRIVACY,
	LEBAO_ORDER_OPT_NODE_OPEN,
	LEBAO_ORDER_OPT_NODE_LOGIN,
	LEBAO_ORDER_OPT_NODE_INTEREST,
	LEBAO_ORDER_OPT_NODE_ONEKEY,
	LEBAO_ORDER_OPT_NODE_IVR,
	LEBAO_ORDER_OPT_NODE_CAPTCHA,
	LEBAO_ORDER_OPT_NODE_RELOGIN,
	LEBAO_ORDER_OPT_NODE_12530,
} lebao_order_opt_node_type_t;
/**********************
* GLOBAL PROTOTYPES
**********************/
void lebao_order_open(void* data);
void lebao_order_close(void* data);

#if defined(LEBAO_FEATURE_PHONE_STYLE)
typedef int(*lebao_order_callback_t)(void* data);

void lebao_order_open_confirm(const int fromPageId, const int id, lebao_order_callback_t left, lebao_order_callback_t right);
int	 lebao_order_is_cmcc(void);

int  lebao_order_need_filter_sms(void);
int  lebao_order_set_captcha(const char* captcha);
int  lebao_order_can_send_sms(void);
int  lebao_order_can_login_sms(int mode);
int  lebao_order_send_login_sms(const int from, const int mode);
int  lebao_order_need_login_sms(void);

int	 lebao_order_can_onekey(void);
int	 lebao_order_open_onekey(void);
int	 lebao_order_post_captcha(void);

void lebao_vip_open(void* data);
void lebao_vip_close(void* data);
#endif

/**********************
*      MACROS
**********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__LEBAO_WIN_ORDER_H*/