/* gzip.h -- common declarations for all gzip modules
 * Copyright (C) 1992-1993 Jean-loup Gailly.
 * This is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License, see the file COPYING.
 */
#ifndef THEME_GZIP_EXPORT_H
#define THEME_GZIP_EXPORT_H

#include "os_api.h"

/* Message id should be unique */
#define THEME_UNZIP_SCODE_PROCESS         (0x90DE)
#define THEME_UNZIP_SCODE_FAIL            (0x90DF)

typedef struct{
    uint16 *iname;           /* compressed file */
    uint16 *p_base;          /* Base dir of output */
    uint16 *p_dir;           /* dir */
}THEME_ENTRY_PARAM_T;

typedef struct{
    uint16 process;    /* 0 ~ 100 */ 
    THEME_ENTRY_PARAM_T *p_param;
}THEME_UNZIP_SIG_CTENT_T;

typedef struct{
    SIGNAL_VARS
    THEME_UNZIP_SIG_CTENT_T sig_content;
}THEME_UNZIP_SIG_T;

uint32         theme_unzip(const uint16 *iname, const uint16 *p_base, const uint16 *p_dir);
void           theme_clean(THEME_UNZIP_SIG_CTENT_T *p_sig_ctent);

#endif
