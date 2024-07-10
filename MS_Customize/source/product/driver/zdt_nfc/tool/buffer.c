#include "buffer.h"

struct buf_entry buffers[BUFFER_NUM];  //总的动态内存空间
struct list_head buffers_list;         //buf链表  动态内存空间
struct list_head nci_rx_list;          //nci_rx链表
struct list_head nci_tx_list;          //nci_tx链表
struct list_head uart_rx_list;         //uart_rx链表
struct list_head uart_tx_list;         //uart_tx链表

//初始化buffer
void init_buffer(void) {
    uint8_t i;
    struct buf_entry * pbuf;
    INIT_LIST_HEAD(&buffers_list);     //初始化buf表头
    INIT_LIST_HEAD(&nci_rx_list);      //初始化nci_rx表头
    INIT_LIST_HEAD(&nci_tx_list);      //初始化nci_tx表头
    INIT_LIST_HEAD(&uart_rx_list);     //初始化uart_rx表头
    INIT_LIST_HEAD(&uart_tx_list);     //初始化uart_tx表头
    for(i = 0; i < BUFFER_NUM; i++) { //初始化动态内存池
        pbuf = &buffers[i];
        list_add_tail(&pbuf->list, &buffers_list);
    }
}
//获取buffer
struct buf_entry *get_a_buf(void) {
    struct buf_entry *pbuf;
    if(list_empty(&buffers_list)) {
        MYLOG(DEBUG, "get buffer fail, no buffer available");
        return NULL;
    }
    pbuf = list_first_entry(&buffers_list, struct buf_entry, list);
    pbuf->len = 0;
    list_del(&pbuf->list);
    return pbuf;
}
//从中断获取buffer
struct buf_entry *get_a_buf_in_isr(void) {
    struct buf_entry *pbuf;
    if(buffers_list.next == &buffers_list) {
        MYLOG(DEBUG, "get buffer fail, no buffer available.");
        return NULL;
    }
    pbuf = list_first_entry(&buffers_list, struct buf_entry, list);
    pbuf->len = 0;
    pbuf->list.next->prev = pbuf->list.prev;
    pbuf->list.prev->next = pbuf->list.next;
    return pbuf;
}
//释放buf
void free_a_buf(struct buf_entry ** p_pbuf) {
    if(*p_pbuf == NULL) {
        MYLOG(DEBUG, "free NULL buffer? pbuf=%p", *p_pbuf);
        return;
    }
    list_move_tail(&(*p_pbuf)->list, &buffers_list);
    *p_pbuf = NULL;
}
//释放packet
void free_packet(struct list_head * pkt_scatter_list) {
    struct buf_entry *pbuf;
    while(!list_empty(pkt_scatter_list)) {
        pbuf = list_first_entry(pkt_scatter_list, struct buf_entry, list);
        #if 0
        for(i = 0; i < BUFFER_BLOCK_SIZE; i++) {
            printf("%bx ", pbuf->block[i]);
        }
        printf("\r\n");
        #endif
        list_move_tail(&pbuf->list, &buffers_list);
    }
    #if 0
    printf("after free: ");
    list_for_each(lh, &buffers_list) {
        printf("-> %p", lh);
    }
    printf("\r\n");
    #endif
}

//buffer数据存入list
ErrorStatus put_buffer_to_list(uint8_t *buf, uint16_t len, struct list_head  *list) {
    struct buf_entry * pbuf;
    pbuf  = get_a_buf();
    if(pbuf) {
        pbuf->len = len;
        memcpy(pbuf->block, buf, len);
        list_add_tail(&pbuf->list, list);
        return SUCCESS;
    }
    return ERROR;
}

//从list中获取数据,  数据处理完记得释放：free_a_buf(&pbuf);
struct buf_entry * get_buffer_from_list(struct list_head  *list) {
    if(!list_empty(list)) {
        struct buf_entry * pbuf = list_get_head(list);
        return pbuf;
    }
    return NULL;
}

//测试
void test_buffer(void) {
    struct buf_entry *pbuf;
    struct list_head  plist;
    MYLOG(DEBUG, "test buffer start.");
    INIT_LIST_HEAD(&plist);
    //init_buffer();
    if(put_buffer_to_list("hello world!", 13, &plist) == SUCCESS) {
        pbuf = get_buffer_from_list(&plist);
        if(pbuf != NULL) {
            MYLOG(DEBUG, "test buffer : %s", pbuf->block);
            free_a_buf(&pbuf);
        }
    }
    //while(1);
}
