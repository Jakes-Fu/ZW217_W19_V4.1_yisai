#ifndef BT_BLE_ABS_H
#define BT_BLE_ABS_H

#include "sci_types.h"

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

/* Maximum UUID size - 16 bytes, and structure to hold any type of UUID. */
#define UUID_LEN_MAX    16

#ifndef BTGATT_MAX_ATTR_LEN
#define BTGATT_MAX_ATTR_LEN 600
#endif

#ifndef WIN32
typedef struct
{
    uint8 uu[16];
}uuid_t;
#endif

typedef struct
{
    int server_handle;
    uint8 server_uuid[16];
    uint16 service_handle;
    uint16 service_handle_num;
    uint8 service_uuid[16];
    uint16 char_desc_handle;
    uint8 char_desc_uuid[16];
    BOOLEAN res_status;
// ID_STATUS_GATT_SERVER_REG_READ_RES                           //server handle & server uuid & status
// ID_STATUS_GATT_SERVER_REG_WRITE_RES       	            //server handle & server uuid & status
// ID_STATUS_GATT_SERVER_DEREG_WRITE_RES       	            //server handle & server uuid & status
// ID_STATUS_GATT_SERVER_SERVICE_READ_RES                    //server handle & service uuid & service handle & service handle number & status
// ID_STATUS_GATT_SERVER_SERVICE_ADD_WRITE_RES          //server handle & service uuid & service handle & service handle number & status
// ID_STATUS_GATT_SERVER_SERVICE_DEL_WRITE_RES           //server handle & service uuid & service handle & status
// ID_STATUS_GATT_SERVER_CHAR_WRITE_RES       	            //server handle & service handle & char_uuid & char_handle & status
// ID_STATUS_GATT_SERVER_DESC_WRITE_RES       	            //server handle & service handle & char_uuid & char_handle & status
// ID_STATUS_GATT_SERVER_SERVICE_START_WRITE_RES       //server handle & service uuid & service handle & status
// ID_STATUS_GATT_SERVER_SERVICE_STOP_WRITE_RES         //server handle & service uuid & service handle & status
}gatt_server_param_t;

typedef struct
{
    uint8 conn_id;
    uint8 remote_address[6];
    uint16 trans_id;
    uint16 att_handle;
    uint8 read_write_flag;//0:read, 1:write
    uint8 req_value[BTGATT_MAX_ATTR_LEN];
    uint16 req_len;
    BOOLEAN res_status;
// ID_STATUS_GATT_SERVER_INDICAT_NOTIFY_WRITE_RES      //att_handle  & conn_id & status
// ID_STATUS_GATT_SERVER_REQ_RECIEVE_IND       	            //(conn_id & trans_id & att handle & receive write & value & value_length& status) or (conn_id & trans_id & att handle & receive read & status)
// ID_STATUS_GATT_SERVER_RSP_WRITE_RES                          //conn_id & att_handle & status
// ID_STATUS_GATT_SERVER_CONN_READ_RES                         //conn_id & remote addr & status
// ID_STATUS_GATT_SERVER_DISCONN_WRITE_RES                  //conn_id & status
}gatt_conn_param_t;

/******************************************************************************
 * FUNCTION:        app_read_register_gatt_server_id
 *
 * DESCRIPTION:  get server id and server uuid
 *     
 * PARAMETERS: 
 *      None
 *
 * RETURNS:
 *      None
 *
 * MESSAGES:
 *      if successful, stack will send message to ATC task in other ways
 ******************************************************************************/
void app_read_register_gatt_server_id(void);

/******************************************************************************
 * FUNCTION:        app_register_gatt_server
 *
 * DESCRIPTION:  register a server
 *     
 * PARAMETERS:
 *      uuid : gatt server uuid
 *      uuid_len : uuid length
 *
 * RETURNS:
 *      FALSE if failure.
 *
 * MESSAGES:
 *      if successful, stack will send message to ATC task in other ways
 ******************************************************************************/
BOOLEAN app_register_gatt_server(uint8 *uuid, uint16 uuid_len);

/******************************************************************************
 * FUNCTION:        app_deregister_gatt_server
 *
 * DESCRIPTION:  deregister a server
 *     
 * PARAMETERS:
 *      server_if : handle of registered gatt server
 *
 * RETURNS:
 *      FALSE if failure.
 *
 * MESSAGES:
 *      if successful, stack will send message to ATC task in other ways
 ******************************************************************************/
BOOLEAN app_deregister_gatt_server(int server_if);

/******************************************************************************
 * FUNCTION:        app_add_user_service
 *
 * DESCRIPTION:  add a service 
 *     
 * PARAMETERS:
 *      server_if: handle of registered gatt server
 *      uuid : service uuid
 *      uuid_len : service uuid length
 *      handle_num : number of handles of this service .
 *      is_primary : 
 *                          1 : primary service
 *                          0 : not primary service
 *
 * RETURNS:
 *      FALSE if failure.
 *
 * MESSAGES:
 *      if successful, stack will send message to ATC task in other ways
 ******************************************************************************/
BOOLEAN app_add_user_service(int server_if, uint8 *uuid, uint16 uuid_len, uint16 handle_num, uint8 is_primary);

/******************************************************************************
 * FUNCTION:        app_read_user_service
 *
 * DESCRIPTION:  get added service infor
 *     
 * PARAMETERS: 
 *      None
 *
 * RETURNS:
 *      None
 *
 * MESSAGES:
 *      if successful, stack will send message to ATC task in other ways
 ******************************************************************************/
void app_read_user_service(void);

/******************************************************************************
 * FUNCTION:        app_remove_user_service
 *
 * DESCRIPTION:  remove a service
 *     
 * PARAMETERS:
 *      service_handle : handle of added service
 *
 * RETURNS:
 *      FALSE if failure.
 *
 * MESSAGES:
 *      if successful, stack will send message to ATC task in other ways
 ******************************************************************************/
BOOLEAN app_remove_user_service(uint16 service_handle);

/******************************************************************************
 * FUNCTION:        app_add_user_character
 *
 * DESCRIPTION:  add a character in service 
 *     
 * PARAMETERS:
 *      service_handle : handle of added service
 *      uuid : character uuid
 *      uuid_len : character uuid length
 *      property : character property
 *      perm : character permission
 *
 * RETURNS:
 *      FALSE if failure.
 *
 * MESSAGES:
 *      if successful, stack will send message to ATC task in other ways
 ******************************************************************************/
BOOLEAN app_add_user_character(uint16 service_handle, uint8 *uuid, uint16 uuid_len, int property, uint16 perm);

/******************************************************************************
 * FUNCTION:        app_add_user_discriptor
 *
 * DESCRIPTION:  add a character discriptor in service 
 *     
 * PARAMETERS:
 *      service_handle : handle of added service
 *      uuid : character uuid
 *      uuid_len : character uuid length
 *      perm : character permission
 *
 * RETURNS:
 *      FALSE if failure.
 *
 * MESSAGES:
 *      if successful, stack will send message to ATC task in other ways
 ******************************************************************************/
BOOLEAN app_add_user_discriptor(uint16 service_handle, uint8 *uuid, uint16 uuid_len, uint16 perm);

/******************************************************************************
 * FUNCTION:        app_start_user_service
 *
 * DESCRIPTION:  start handle of service
 *     
 * PARAMETERS:
 *      service_handle : handle of added service
 *      transport : 
 *                      0 : LE
 *
 * RETURNS:
 *      FALSE if failure.
 *
 * MESSAGES:
 *      if successful, stack will send message to ATC task in other ways
 ******************************************************************************/
BOOLEAN app_start_user_service( uint16 service_handle, uint8 transport);

/******************************************************************************
 * FUNCTION:        app_stop_user_service
 *
 * DESCRIPTION:  stop handle of service
 *     
 * PARAMETERS:
 *      service_handle : handle of added service
 *
 * RETURNS:
 *      FALSE if failure.
 *
 * MESSAGES:
 *      if successful, stack will send message to ATC task in other ways
 ******************************************************************************/
BOOLEAN app_stop_user_service(uint16 service_handle );

/******************************************************************************
 * FUNCTION:        app_read_user_conn_id
 *
 * DESCRIPTION:  get connect link id
 *     
 * PARAMETERS: 
 *      None
 *
 * RETURNS:
 *      None
 *
 * MESSAGES:
 *      if successful, stack will send message to ATC task in other ways
 ******************************************************************************/
void app_read_user_conn_id(void);

/******************************************************************************
 * FUNCTION:        app_send_indi_or_noti
 *
 * DESCRIPTION:  send indication or notification to client
 *     
 * PARAMETERS:
 *      conn_id: id of current connection
 *      att_handle: handle of attribution
 *      need_confirm: 
 *                              0 : NO
 *                              1 : YES
 *      value: value to send
 *      value_length: value length to send
 *
 * RETURNS:
 *      FALSE if failure.
 *
 * MESSAGES:
 *      if successful, stack will send message to ATC task in other ways
 ******************************************************************************/
BOOLEAN app_send_indi_or_noti(int conn_id, uint16 att_handle, uint8 need_confirm, uint8 *value, uint16 value_length );

/******************************************************************************
 * FUNCTION:        app_send_response
 *
 * DESCRIPTION:  send response to client
 *     
 * PARAMETERS:
 *      conn_id: id of current connection
 *      trans_id: id of current transaction
 *      att_handle: handle of attribution
 *      value: value to send
 *      value_length: value length to send
 *
 * RETURNS:
 *      FALSE if failure.
 *
 * MESSAGES:
 *      if successful, stack will send message to ATC task in other ways
 ******************************************************************************/
BOOLEAN app_send_response(int conn_id, uint32 trans_id, uint16 att_handle, uint8 *value, uint16 value_length );

/******************************************************************************
 * FUNCTION:        app_disconnect_gatt_server
 *
 * DESCRIPTION:  disconnect the link
 *     
 * PARAMETERS:
 *      conn_id: id of current connection
 *
 * RETURNS:
 *      FALSE if failure.
 *
 * MESSAGES:
 *      if successful, stack will send message to ATC task in other ways
 ******************************************************************************/
BOOLEAN app_disconnect_gatt_server(int conn_id);
#endif
