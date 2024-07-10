#ifndef BOOTPARAM_DEF_H
#define BOOTPARAM_DEF_H

typedef struct _OS_INFO
{
    char            name[24];
    char            description[48];
    char            version[24];
    unsigned short  offset;  /* in sectors */
    unsigned short  size;    /* in sectors */
} OS_INFO, *POS_INFO;

typedef struct _BOOT_PARAM
{
    unsigned char   magic[4];
    unsigned short  size;
    unsigned short  total_size;
    long            time_stamp;
    unsigned char   trace_on;
    unsigned char   reserved;
    unsigned char   current_os;
    unsigned char   num_of_os;
    unsigned short  os_info_size;
    unsigned short  os_offset;
} BOOT_PARAM, *PBOOT_PARAM;

#endif /* BOOTPARAM_DEF_H */