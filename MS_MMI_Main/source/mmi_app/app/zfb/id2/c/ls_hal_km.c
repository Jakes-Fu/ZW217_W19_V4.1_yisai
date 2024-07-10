/**
 * Copyright (C) 2017 The YunOS Project. All rights reserved.
 */

#include "ls_osa.h"
#include "ls_hal.h"
#include "plat_comm.h"
#include "zfb_app.h"
#include "wifi_drv.h"
#include "Bt_abs.h"
#include "Bt_cfg.h"
#include "bt_abs.h"
#include "production_test.h"
#include "mmi_filemgr.h"

#define FLASH_BLOCK_LEN 2048

#define DEV_IMEI_LEN       15
#define DEV_MAC_LEN       17
#define DEV_MODEL_LEN      10
#define PRODUCT_KEY_LEN    11
#define DEV_ID_LEN         68

#define FILE_FD_DEFAULT_VAL 1310720;

int g_zfb_rsvd_file_fd = 0;
static uint8 s_zfb_rsvd_need_save = 0;

static int ls_rand(void)
{
    static int ali_rand_offset = 12345;
    int radom = 0;
    srand(SCI_GetTickCount()+ali_rand_offset);
    ali_rand_offset++;
    radom = rand();
#ifdef ALIPAY_DEBUG
    SCI_TRACE_LOW("ls_rand radom=%d",radom);
#endif
    return radom;
}

int ls_hal_get_dev_imei(uint8_t *imei, uint32_t *len)
{
    char imei_buf[100] = {0};
    uint16 imei_len = 0;
    if (imei == NULL ||  len == NULL) {
        LS_HAL_LOG("invalid input args\n");
        return -1;
    }
    if (*len < DEV_IMEI_LEN) {
        if (*len != 0) {
            LS_HAL_LOG("short buffer id len is %d\n", *len);
        }
        *len = DEV_IMEI_LEN;
        return -1;
    }
    imei_len = MMIZFB_Net_GetIMEI(0,imei_buf);
    *len = imei_len;
    SCI_MEMCPY(imei,imei_buf,imei_len);
#ifdef ALIPAY_DEBUG
    LS_HAL_LOG("len=%d,%s",*len,imei_buf);
#endif
    return 0;
}

//Ä¬ÈÏFF:FF:FF:FF:FF:FF  Ëæ»ú: 8C:9B:76:DA:45:40
int ls_hal_get_dev_mac(uint8_t *mac, uint32_t *len)
{
    int i = 0;
    NVITEM_ERROR_E  status  = NVERR_NONE;
    BT_NV_PARAM    nv_param;
    uint8 mac_addr[BT_ADDRESS_SIZE] = {0};
    char mac_buf[100] = {0};
    uint16 mac_len = 0;
    if (mac == NULL ||  len == NULL) {
        LS_HAL_LOG("invalid input args\n");
        return -1;
    }

    if (*len < DEV_MAC_LEN) {
        if (*len != 0) {
            LS_HAL_LOG("short buffer id len is %d\n", *len);
        }
        *len = DEV_MAC_LEN;
        return -1;
    }
    
    status = EFS_NvitemRead(NV_BT_CONFIG, sizeof(BT_NV_PARAM), (uint8*)&nv_param);
    if(NVERR_NONE != status)
    {
        uint32_t cur_radom = ls_rand();
        mac_addr[0] = 0x8C;
        mac_addr[1] = 0x9B;
        mac_addr[2] = 0x76;
        mac_addr[3] = 0xDA;
        mac_addr[4] = (cur_radom>>8) & 0x000000FF;
        mac_addr[5] = cur_radom & 0x000000FF;
        SCI_MEMCPY(nv_param.bd_addr.addr,mac_addr,BT_ADDRESS_SIZE);
        status = NVITEM_UpdateCali(NV_BT_CONFIG, sizeof(nv_param), (uint8*)&nv_param);
    }
    else
    {
        for(i = 0 ; i < BT_ADDRESS_SIZE; i++)
        {
            if(nv_param.bd_addr.addr[i] != 0xFF)
            {
                break;
            }
        }
        if(i == BT_ADDRESS_SIZE)
        {
            uint32_t cur_radom = ls_rand();
            mac_addr[0] = 0x8C;
            mac_addr[1] = 0x9B;
            mac_addr[2] = 0x76;
            mac_addr[3] = 0xDA;
            mac_addr[4] = (cur_radom>>8) & 0x000000FF;
            mac_addr[5] = cur_radom & 0x000000FF;
            SCI_MEMCPY(nv_param.bd_addr.addr,mac_addr,BT_ADDRESS_SIZE);
            status = NVITEM_UpdateCali(NV_BT_CONFIG, sizeof(nv_param), (uint8*)&nv_param);
        }
        else
        {
            SCI_MEMCPY(mac_addr,(uint8*)&(nv_param.bd_addr.addr),  BT_ADDRESS_SIZE);
        }
    }
    
    sprintf(mac_buf,"%02X:%02X:%02X:%02X:%02X:%02X", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    *len = strlen(mac_buf);
    SCI_MEMCPY(mac,mac_buf,(*len));
#ifdef ALIPAY_DEBUG
    LS_HAL_LOG("len=%d,%s",*len,mac_buf);
#endif
    return 0;
}

int ls_hal_get_dev_sn(uint8_t *sn, uint32_t *len)
{
    PRODUCTION_TEST_INFO_T s_mmieng_product = {0};
    char*           temp_ptr = PNULL;
    uint16          temp_len = 0;
    uint8 text_buf[101] = {0};
    if (sn == NULL || len == NULL) {
        LS_HAL_LOG("invalid input args\n");
        return -1;
    }
    //LS_HAL_LOG("in len is %d\n", *len);

#ifdef WIN32
    *len = 10;
    SCI_MEMCPY(sn,"YS8910FFZW",10);
    return 0;
#else
    if (SCI_SUCCESS == SCI_GetProductionTestInfo(&s_mmieng_product))
    {
        temp_len = strlen(s_mmieng_product.serial_num);
        if(temp_len > 0)
        {
            if(temp_len > 48)
            {
                temp_len = 48;
            }
            *len = temp_len;
            SCI_MEMCPY(sn,s_mmieng_product.serial_num,temp_len);
#ifdef ALIPAY_DEBUG
            LS_HAL_LOG("len=%d,sn=%s",*len,sn);
#endif
            return 0;
        }
        else
        {
            *len = 10;
            SCI_MEMCPY(sn,"YS8910FFZW",10);
#ifdef ALIPAY_DEBUG
            LS_HAL_LOG("len=%d,sn=%s",*len,sn);
#endif
            return 0;
        }
    }
    else
    {
        *len = 0;
    }
#endif
    //LS_HAL_LOG("no factory");
    return -1;
}

int ls_hal_get_model(uint8_t *model, uint32_t *len)
{
    char *dev_model = "ECELL_8910";

    if (model == NULL ||  len == NULL) {
        LS_HAL_LOG("invalid input args\n");
        return -1;
    }

    if (*len < DEV_MODEL_LEN) {
        if (*len != 0) {
            LS_HAL_LOG("short buffer id len is %d\n", *len);
        }
        *len = DEV_MODEL_LEN;
        return -1;
    }

    SCI_MEMCPY(model, dev_model, DEV_MODEL_LEN);
    *len = DEV_MODEL_LEN;
#ifdef ALIPAY_DEBUG
    LS_HAL_LOG("len=%d,model=%s",*len,model);
#endif
    return 0;
}

int ls_hal_get_product_key(uint8_t *product_key, uint32_t *len)
{
    char *dev_pk = "EJMZkxFuSKX";

    if (product_key == NULL || len == NULL) {
        LS_HAL_LOG("invalid input args\n");
        return -1;
    }

    if (*len < PRODUCT_KEY_LEN) {
        if (*len != 0) {
            LS_HAL_LOG("short buffer id len is %d\n", *len);
        }
        *len = PRODUCT_KEY_LEN;
        return -1;
    }

    SCI_MEMCPY(product_key, dev_pk, PRODUCT_KEY_LEN);
    *len = PRODUCT_KEY_LEN;
#ifdef ALIPAY_DEBUG
    LS_HAL_LOG("len=%d,key=%s",*len,product_key);
#endif
    return 0;
}

int ls_hal_get_dev_id(uint8_t *dev_id, uint32_t *id_len)
{
    char id_buf[100] = {0};
    uint32_t tmp_len = 48;
    char imei_buf[16] = {0};
    uint16 imei_len = 0;
    if (dev_id == NULL ||  id_len == NULL) {
        LS_HAL_LOG("invalid input args\n");
        return -1;
    }
    if (*id_len < DEV_ID_LEN) {
        if (*id_len != 0) {
            LS_HAL_LOG("short buffer id len is %d\n", *id_len);
        }
        *id_len = DEV_ID_LEN;
        return -1;
    }
    ls_hal_get_dev_sn(id_buf,&tmp_len);
    imei_len = MMIZFB_Net_GetIMEI(0,imei_buf);
    SCI_MEMCPY(id_buf+tmp_len,imei_buf,imei_len);
    tmp_len += imei_len;
    SCI_MEMCPY(dev_id,id_buf,tmp_len);
    *id_len = tmp_len;
#ifdef ALIPAY_DEBUG
    LS_HAL_LOG("len=%d,%s",tmp_len,id_buf);
#endif
    return 0;
}

int ls_hal_open_rsvd_part(int flag)
{
    int fd = -1;
    g_zfb_rsvd_file_fd = ls_rand();
    fd = g_zfb_rsvd_file_fd;
#ifdef ALIPAY_DEBUG
    LS_HAL_LOG("open flag=%d,fd=%d",flag,fd);
#endif
    return fd;
}

int ls_hal_write_rsvd_part(int fd, uint32_t offset, void *data, uint32_t data_len)
{
    //LS_HAL_LOG("write fd=%d,offset=%d,data_len=%d",fd,offset,data_len);
    if(data == NULL || data_len == 0)
    {
        return 0;
    }
    if((offset+data_len) > MAX_ZFB_RSVD_SIZE)
    {
        LS_HAL_LOG("fail over len",fd,offset,data_len);
        return -1;
    }
    if(s_zfb_rsvd_need_save == 0)
    {
        s_zfb_rsvd_need_save = 1;
    }
    SCI_MEMCPY(zfb_DB_Rsvd_Rec.rsvd_data+offset,data,data_len);
    return 0;
}

int ls_hal_read_rsvd_part(int fd, uint32_t offset, void *buffer, uint32_t read_len)
{
    //LS_HAL_LOG("read fd=%d, offset=%d,data_len=%d",fd,offset,read_len);
    if(buffer == NULL || read_len == 0)
    {
        return 0;
    }
    if((offset+read_len) > MAX_ZFB_RSVD_SIZE)
    {
        LS_HAL_LOG("fail over len",fd,offset,read_len);
        return -1;
    }
    SCI_MEMCPY(buffer,zfb_DB_Rsvd_Rec.rsvd_data+offset,read_len);
    return 0;
}

int ls_hal_close_rsvd_part(int fd)
{
    //LS_HAL_LOG("fd = %d",fd);
    if(s_zfb_rsvd_need_save)
    {
        s_zfb_rsvd_need_save = 0;
        ZFB_DB_RSVD_ListModify();
    }
    return 0;
}

