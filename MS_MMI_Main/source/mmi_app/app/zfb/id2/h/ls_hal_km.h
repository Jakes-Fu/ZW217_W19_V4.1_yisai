/*
 * Copyright (C) 2016-2019 Alibaba Group Holding Limited
 */

#ifndef _LS_HAL_KM_H_
#define _LS_HAL_KM_H_

#ifdef WIN32
#include "alipay_iot_type.h"
#else
#include <stdint.h>
#endif

#define LS_HAL_READ     1
#define LS_HAL_WRITE    2

/*
 * get device imei
 *
 * note: if imei is NULL and *len is 0, return -1
 *        and set *len to the real length of the imei
 *
 * param: out:  imei: device imei should be no longer than 100 byte
 *        in_out: len: device imei length
 *
 * return: 0: success
 *         -1: fail
 *
*/
int ls_hal_get_dev_imei(uint8_t *imei, uint32_t *len);

/*
 * get device mac address
 *
 * note: if mac is NULL and *len is 0, return -1
 *        and set *len to the real length of the mac address
 *
 * param: out:  mac: device mac address should be no longer than 100 byte
 *        in_out: len: device mac address length
 *
 * return: 0: success
 *         -1: fail
 *
*/
int ls_hal_get_dev_mac(uint8_t *mac, uint32_t *len);

/*
 * get device serial number
 *
 * note: if sn is NULL and *len is 0, return -1
 *        and set *len to the real length of the serial number
 *
 * param: out:  sn: device serial number should be no longer than 100 byte
 *        in_out: len: device serial number length
 *
 * return: 0: success
 *         -1: fail
 *
*/
int ls_hal_get_dev_sn(uint8_t *sn, uint32_t *len);

/*
 * get device unique id
 *
 * note: if dev_id is NULL and *id_len is 0, return -1
 *        and set *id_len to the real length of the id
 *
 * param: out:  dev_id: device uinque id should be a string no longer than 100 byte
 *        in_out: id_len: device uinque id length
 *
 * return: 0: success
 *         -1: fail
 *
*/
int ls_hal_get_dev_id(uint8_t *dev_id, uint32_t *id_len);

/*
 * get device model
 *
 * note: if model is NULL and *len is 0, return -1
 *        and set *len to the real length of the device model
 *
 * param: out:  model: device model should be no longer than 100 byte
 *        in_out: len: device model length
 *
 * return: 0: success
 *         -1: fail
 *
*/
int ls_hal_get_model(uint8_t *model, uint32_t *len);

/*
 * get product key
 *
 * note: if product_key is NULL and *len is 0, return -1
 *        and set *len to the real length of the device product_key
 *
 * param: out:  product_key: device product key should be no longer than 100 byte
 *        in_out: len: device model length
 *
 * return: 0: success
 *         -1: fail
 *
*/
int ls_hal_get_product_key(uint8_t *product_key, uint32_t *len);


/*
 * open the reserved partition that user has read and write permission
 *
 * note: if not support file system, return 0 directly
 *
 * parametr: in: flag: LS_HAL_READ: user only has read permission
 *                     LS_HAL_WRITE: user only has write permission
 *                     LS_HAL_READ | LS_HAL_WRITE: user has read and write permission
 *
 * return: the new file descriptor: success
 *         -1: fail
*/
int ls_hal_open_rsvd_part(int flag);

/*
 * write reserved  partition
 *
 * parametr: in: fd: file handle, can be ignored if no file system
 *               offset: the offset of the reserved partition
 *               data: the data need to write
 *               data_len: the length of the data need to write
 *
 * return: 0: success
 *         -1: fail
*/
int ls_hal_write_rsvd_part(int fd, uint32_t offset, void *data, uint32_t data_len);

/*
 * read reserved partition
 *
 * parametr: in:  fd: file handle, can be ignored if no file system
 *                offset: the offset of the reserved partition
 *                read_len: the length of the data need to read
 *           out: buffer: the data read from the reserved part

 * return: 0: success
           -1: fail
 */
int ls_hal_read_rsvd_part(int fd, uint32_t offset, void *buffer, uint32_t read_len);

/*
 * close the file descriptor of reserved partition, if no file system support, return 0 directly
 *
 * parametr: in: fd: file descriptor of the reserved part

 * return: 0: success
           -1: fail
 */

int ls_hal_close_rsvd_part(int fd);

#endif /* _LS_HAL_KM_H_ */

