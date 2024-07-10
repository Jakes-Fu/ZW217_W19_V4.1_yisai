/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/
#ifndef _AT_CMD_MIPL_CONFIG_H_
#define _AT_CMD_MIPL_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum enum_mipl_cfgid
{
    mipl_cfgid_init = 0x01,
    mipl_cfgid_net = 0x02,
    mipl_cfgid_sys = 0x03
} mipl_cfgid_t;

typedef struct st_mipl_cfg_init mipl_cfg_init_t;
typedef struct st_mipl_cfg_net mipl_cfg_net_t;
typedef struct st_mipl_cfg_sys mipl_cfg_sys_t;
typedef struct st_mipl_cfg_vdata mipl_cfg_data_t;
typedef struct st_mipl_cfg_header mipl_cfg_header_t;

struct st_mipl_cfg_vdata
{
    uint16_t len;
    uint8_t *data;
};

struct st_mipl_cfg_header
{
    uint8_t version;
    uint8_t cfgcount;
    uint16_t bytes;
};

struct st_mipl_cfg_init
{
    uint8_t *data;
};

struct st_mipl_cfg_net
{
    uint16_t mtu;
    uint8_t linktype;
    uint8_t bandtype;
    uint8_t bs_enabled;
    uint8_t dtls_enabled;

    mipl_cfg_data_t apn;
    mipl_cfg_data_t username;
    mipl_cfg_data_t password;
    mipl_cfg_data_t host;
    mipl_cfg_data_t user_data;
};

struct st_mipl_cfg_sys
{
    bool log_enabled;
    uint8_t log_ext_output;
    uint8_t log_output_type;
    uint8_t log_output_level;
    uint16_t log_buffer_size;
    mipl_cfg_data_t user_data;
};

struct st_mipl_cfg_context
{
    mipl_cfg_header_t cfgHeader;
    mipl_cfg_init_t cfgInit;
    mipl_cfg_net_t cfgNet;
    mipl_cfg_sys_t cfgSys;
};

typedef struct st_mipl_cfg_context mipl_cfg_context_t;

bool mipl_config_init(mipl_cfg_context_t *config_context, void *config_ptr, uint16_t config_len);

#ifdef __cplusplus
};
#endif

#endif
