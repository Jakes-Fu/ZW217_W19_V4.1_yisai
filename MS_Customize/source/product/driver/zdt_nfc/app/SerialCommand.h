#ifndef _SERIAL_COMMAND_H
#define _SERIAL_COMMAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MyInclude.h"

typedef struct {
    uint8_t  flag;
    uint8_t  len;
    uint8_t  buf[2000];//2000  16
} UartData;

void uart_receive_data(uint8_t data);
void uart_send_data(uint8_t *ptr, uint8_t len);

uint8_t  Uart_Mission_Proc(void);
uint16_t uart_rx_transfer_funct(uint16_t len);
uint8_t  uart_tx_transfer_funct(uint8_t *p_buff, uint8_t send_len,uint8_t *p_out );

#ifdef __cplusplus
}
#endif
#endif
