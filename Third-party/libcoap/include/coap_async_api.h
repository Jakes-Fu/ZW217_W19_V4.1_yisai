#ifndef _COAP_ASYNC_API_H_
#define _COAP_ASYNC_API_H_
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
//#include <errno.h>
#include <signal.h>
#include <stdbool.h>

#include "coap_config.h"

#include "coap_thread.h"
//#include "base_prv.h"
/*
   "usage: %s [-A type...] [-t type] [-b [num,]size] [-B seconds] [-e text]\n"
   "        [-m method] [-N] [-o file] [-P addr[:port]] [-p port]\n"
   "        [-s duration] [-O num,text] [-T string] [-v num] [-a addr] \n\n"
   "        [-u user] [-k key] URI"
   "    URI can be an absolute or relative coap URI,\n"
   "    -a addr    the local interface address to use\n"
   "    -A type...    accepted media types as comma-separated list of\n"
   "            symbolic or numeric values\n"
   "    -t type        content format for given resource for PUT/POST\n"
   "    -b [num,]size    block size to be used in GET/PUT/POST requests\n"
   "                   (value must be a multiple of 16 not larger than 1024)\n"
   "                   If num is present, the request chain will start at\n"
   "                   block num\n"
   "    -B seconds    break operation after waiting given seconds\n"
   "            (default is 90)\n"
   "    -e text        include text as payload (use percent-encoding for\n"
   "            non-ASCII characters)\n"
   "    -k key        Pre-shared key for the specified user. This argument\n"
   "                   requires DTLS with PSK to be available.\n"
   "    -N        send NON-confirmable message\n"
   "    -p port        listen on specified port\n"
   "    -s duration    subscribe for given duration [s]\n"
   "    -u user        user identity for pre-shared key mode. This argument\n"
   "                   requires DTLS with PSK to be available.\n"
   "    -v num        verbosity level (default: 3)\n"
   "    -O num,text    add option num with contents text to request\n"
   "    -P addr[:port]    use proxy (automatically adds Proxy-Uri option to\n"
   "            request)\n"
   "    -T token    include specified token\n"
   "\n"
   "examples:\n"
   "    coap-client -m get coap://[::1]/\n"
   "    coap-client -m get coap://[::1]/.well-known/core\n"
   "    coap-client -m get -T cafe coap://[::1]/time\n"
   "    echo 1000 | coap-client -m put -T cafe coap://[::1]/time -f -\n"
*/



typedef enum
{
    COAP_STATUS_NONE,
    COAP_STATUS_INIT,
    COAP_STATUS_READY,
    COAP_STATUS_PROCEING,
    COAP_STATUS_DONE
} coap_status_t;

/* CoAP request method codes */
typedef enum
{
    COAP_GET = 1,
    COAP_POST,
    COAP_PUT,
    COAP_DELETE
} coap_method_t;

/**
 * @ingroup COAP
 * Function prototype for COAP incoming data callback function. Called when data
 * arrives to a client
 *
 * @param arg Additional argument to pass to the callback function
 * @param data User data, pointed object, data may not be referenced after callback return,
          NULL is passed when all publish data are delivered
 * @param len Length of data fragment
 * @param num data fragment index
 *
 */

struct coap_client_t;
typedef struct coap_client_t coap_client_t;

typedef void (*coap_incoming_data_cb_t)(coap_client_t *client, const uint8_t *data, uint16_t len, uint8_t num);

//typedef void (*coap_user_cb_t)(coap_client_t *client, coap_method_t method, bool result);


coap_client_t *coap_getCoapClient(void);
coap_status_t coap_client_getStatus(coap_client_t *client);
bool coap_client_setParams(coap_client_t *client, uint8_t *cmdline_param);
bool coap_client_setIndataCbk(coap_client_t *client, coap_incoming_data_cb_t data_cb);

bool coap_async_get(coap_client_t *client, uint8_t *url);
bool coap_async_put(coap_client_t *client, uint8_t *url, uint8_t *payload);
bool coap_async_post(coap_client_t *client, uint8_t *url, uint8_t *payload);
bool coap_async_delete(coap_client_t *client, uint8_t *url);

uint32_t coap_client_payload_size();

//int coap_client_main(int argc, char **argv);

uint8_t coap_client_set_payload(void *data, unsigned int payload_length, unsigned char payload_type);

//void coap_client_set_datahander(coap_incoming_data_cb_t handler, void *param);

#define CMDLINE_NUM (16)
#define DATA_LEN_INV (0)
#define DATA_LEN_MIN (1)
#define DATA_LEN_MAX (65535)
#define DATA_TIMEOUT_MIN (1)
#define DATA_TIMEOUT_DEF (90)
#define DATA_TIMEOUT_MAX (120)
#define DATA_MANUL_LEN_MAX (1024)
#define AT_URL_LENG_MAX (60)
#define AT_CMDLINE_LENG_MAX (128)

#define COAP_HOSTNAME_LEN_MAX (256)
#define COAP_CMDLINE_LEN_MAX (256)
#define COAP_PORT_DEF (5863)
#define COAPS_PORT_DEF (5864)
#define COAP_PORT_MIN (1)
#define COAP_PORT_MAX (65535)

#define COAP_MESSAGE_MAX_SIZE 64 /* maximum size of one block.num message */
#define COAP_BLOCK_MAX_SIZE 64   /* maximum size of block */

typedef struct _coap_data
{
    uint8_t *gBuff[COAP_BLOCK_MAX_SIZE];
    int gNum;
    int gLength;
    int gLen[COAP_BLOCK_MAX_SIZE];    
} COAP_DATA_T;


struct coap_client_t
{
    coap_status_t status;
    //bool autoFree;
    uint8_t *cmdline;
    uint8_t *payload;
    uint32_t payload_len;
    //void *user_task;
    //coap_user_cb_t user_cb;
    //void *user_data;
    coap_incoming_data_cb_t data_cb;
};


PUBLIC void coapProcessReturnData(coap_client_t *client, const unsigned char *data, size_t len, int num);


#endif
