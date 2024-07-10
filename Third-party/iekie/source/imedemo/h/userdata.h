
/*
 * userdata.h
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#ifndef USERDATA_H
#define USERDATA_H

#include "imedemo.h"

extern void* userdata;
extern int userdata_size;

extern void* imeoptiondata;
extern int imeoptiondata_size;

void userdata_init(void);
void userdata_save(void);

#endif /* USERDATA_H */
