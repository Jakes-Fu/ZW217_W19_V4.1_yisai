#ifndef __BUFFER_H__
#define __BUFFER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "MyInclude.h"

#define BUFFER_NUM (9)
#define BUFFER_BLOCK_SIZE (256+3)

// #define HEADER_ROOM_SIZE (3)
struct buf_entry {
    struct list_head list;
    // uint8_t header_room[HEADER_ROOM_SIZE];
    uint8_t  block[BUFFER_BLOCK_SIZE];
    uint16_t len;
};

extern struct list_head buffers_list;         //buf链表  动态内存空间
extern struct list_head nci_rx_list;          //nci_rx链表
extern struct list_head nci_tx_list;          //nci_tx链表
extern struct list_head uart_rx_list;         //uart_rx链表
extern struct list_head uart_tx_list;         //uart_tx链表

void init_buffer(void);
struct buf_entry *get_a_buf(void);
struct buf_entry *get_a_buf_in_isr(void);
void free_a_buf(struct buf_entry ** pbuf);
void free_packet(struct list_head * pkt_scatter_list);

ErrorStatus put_buffer_to_list(uint8_t *buf, uint16_t len, struct list_head  *list); //发送数据
struct buf_entry * get_buffer_from_list(struct list_head  *list);                 //接收数据

void test_buffer(void);
#ifdef __cplusplus
}
#endif
#endif /* BUFFER_H */
