
/*
 * update_data.h
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#ifndef UPDATE_DATA_H
#define UPDATE_DATA_H

#include "imedemo.h"

int Read_Update_Data(unsigned char* buf, int offset, int len);
void exec_load_update_data(HWND parent);

#endif /* UPDATE_DATA_H */
