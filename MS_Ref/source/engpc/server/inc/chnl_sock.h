#ifndef _CHNL_SOCK_H_
#define _CHNL_SOCK_H_

#include "chnl.h"//Bug 1635670

PUBLIC int pc_sock_read_data(CHNL_DATA_TYPE type, char* buff, int len);
PUBLIC int pc_sock_write_data(CHNL_DATA_TYPE type, char* buff, int len);
PUBLIC void chnl_sock_init(int type);


#endif