#include "SerialCommand.h"

static UartData uartdata;   //�ļ��ڲ�����
void uart_receive_data(uint8_t data) {
    uartdata.buf[uartdata.len++] = data;
    if(uartdata.len > 15) {
        uartdata.len = 0;
        uartdata.flag = 0;
        return;
    }
    if(uartdata.len > 4) {
        if((uartdata.buf[uartdata.len - 1] == 0xff) && (uartdata.buf[uartdata.len - 2] == 0xff)) {
            uartdata.flag = 1;
        }
    }
}

void uart_send_data(uint8_t *ptr, uint8_t len) {
    uint8_t buf[16], i;
    uint8_t tmp_buf[3] = {0xff,0xff,0};
    zdt_nfc_uart_write(tmp_buf,2);
    len = uart_tx_transfer_funct(ptr, len, buf);
    zdt_nfc_uart_write(buf,len);
    zdt_nfc_uart_write(tmp_buf,2);
    MYENTER(DEBUG);
}

//����buff�������ת�壬����֡ͷ֡β������ֵΪ�����򳤶�
uint8_t uart_tx_transfer_funct(uint8_t *p_buff, uint8_t send_len, uint8_t *p_out) {
    uint8_t len_temp = 0; //ת�������ݳ���
    uint8_t i = 0;
    for(i = 0; i < send_len; i++) {
        if(p_buff[i] == 0xff) {
            p_out[len_temp] = 0xfe;
            len_temp++;
            p_out[len_temp] = 0x01;
        } else if(p_buff[i] == 0xfe) {
            p_out[len_temp] = 0xfe;
            len_temp++;
            p_out[len_temp] = 0x00;
        } else {
            p_out[len_temp] = p_buff[i];
        }
        len_temp++;
    }
    return len_temp;
}

//����buff��ת�壬��ȥ֡ͷ֡β������ֵΪ�����򳤶�
uint16_t uart_rx_transfer_funct(uint16_t len) {
    uint16_t len_temp = 0; //ת�������ݳ���
    uint16_t i = 0;
    uint8_t sign_flag = 0; //Ϊ0��δ��⵽֡ͷ��Ϊ1���⵽һ��ff,Ϊ2���⵽2��ff,Ϊ3���⵽֡β��һ��ff��Ϊ4���⵽֡β��2��ff
    if(uartdata.len > 0) {
        for(i = 0; i <= len; i++) {
            switch(sign_flag) {
                case 0:                 //δ��⵽֡ͷ
                    if(uartdata.buf[i] == 0xff) {
                        sign_flag = 1;
                    } else {
                        sign_flag = 0;
                    }
                    break;
                case 1:                 //��⵽֡ͷ��һ��ff
                    if(uartdata.buf[i] == 0xff) {
                        sign_flag = 2;
                        len_temp = 0;
                    } else {
                        sign_flag = 0;
                    }
                    break;
                case 2:                 //��⵽֡ͷ��δ��⵽֡β
                    if(uartdata.buf[i] == 0xff) {
                        if(len_temp == 0) {
                            sign_flag = 2;
                            len_temp = 0;
                        } else {
                            sign_flag = 3;
                        }
                    } else {
                        sign_flag = 2;
                        if(uartdata.buf[i] == 0xfe) {
                            if(uartdata.buf[i + 1] == 0x01) { //fe 01ת��Ϊff
                                uartdata.buf[len_temp] = 0xff;
                            } else if(uartdata.buf[i + 1] == 0x00) { //fe 00ת��Ϊfe
                                uartdata.buf[len_temp] = 0xfe;
                            } else {
                                return 0;
                            }
                            i++;
                        } else {
                            uartdata.buf[len_temp] = uartdata.buf[i];
                        }
                        len_temp++;
                    }
                    break;
                case 3:                 //��⵽����֡β
                    return len_temp;
            }
        }
    }
    return 0;
}

uint16_t uart0_rx_proc(void) {
    uint16_t ret = 0;
    if(uartdata.flag == 1) {
        //MYHEXS(DEBUG, uartrx_buff, uartrx_cnt, "ǰ��");
        ret = uart_rx_transfer_funct(uartdata.len);
        //MYHEXS(DEBUG, uartrx_buff, ret, "��");
        //MYLOG(DEBUG, "uart0_rx_proc %d %d", uartrx_cnt, ret);
        uartdata.flag = 0;
        uartdata.len  = 0;
    }
    return ret;
}

uint8_t Uart_Mission_Proc(void) {
    uint8_t send_buff[5] = {0x11, 0x00, 0x01, 0x00}; //�ɹ���Ӧ
    uint16_t ret = uart0_rx_proc();
    if(ret) {
        send_buff[0] = uartdata.buf[0];
        switch(uartdata.buf[0]) {
            case 0x11:
                uart_send_data(send_buff, 4);
                NFCC_Rest(0);   //��λ������������ģʽ
                SCI_Sleep(100);
                gtPollingParam.POLLING_ENABLE   = 1; //����ģʽ
                gtListencardParam.LISTEN_ENABLE = 0;
                return 1;
            case 0x12:
                uart_send_data(send_buff, 4);
                NFCC_Rest(0);   //��λ������������ģʽ
                SCI_Sleep(100);
                gtPollingParam.POLLING_ENABLE   = 0;
                gtListencardParam.LISTEN_ENABLE = 1; //��ģ��ģʽ
                return 1;
            case 0xef:
                send_buff[1] = 0x02;
                uart_send_data(send_buff, 4);
                SCI_Sleep(100);
                //NFC_Firmware_Update();//�̼�����
                update_proc_from_code();            //��ʼ�����̼�
                NFCC_Init();          //NFC��ʼ��
                return 1;
            default:
                break;
        }
    }
    return 0;
}
