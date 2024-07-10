/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
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
#include "liblwm2m_config.h"

#ifdef CONFIG_LWM2M_ONENET_SUPPORT

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "osi_log.h"
#include "mipl_config.h"

#define MIPL_ENABLE_LOG 1

#define NBCFG_USE_BIGENDIAN 1

#define NBCFG_SUPPORT_VERSION 0x01
static const uint8_t MiplVersionUnsupportList[] = {
    0x00,
};

#define NBCFG_HeaderSize (0x03)
#define NBCFG_MAKE_U16(p) (miplMakeU16(p))

#define NBCFG_GetVersion(data) ((data >> 4) & 0x0F)
#define NBCFG_GetCC(data) (data & 0x0F)

#define NBCFG_GetConfigId(data) (mipl_cfgid_t)(data & 0x0F)
#define NBCFG_GetConfigFlag(data) (data & 0x80)
#define NBCFG_GetConfigSize(data) (NBCFG_MAKE_U16(data))

#define NBCFG_Getconfig_LogEnabled(data) ((data >> 7) & 0x01)
#define NBCFG_Getconfig_LogExtoutput(data) ((data >> 6) & 0x01)
#define NBCFG_Getconfig_LogType(data) ((data >> 4) & 0x03)
#define NBCFG_Getconfig_LogOutputlevel(data) (data & 0x0F)
#define NBCFG_GetConfig_Istcount(data) ((data >> 4) & 0x0F)

#define NBCFG_Getconfig_BSEnabled(data) ((data >> 7) & 0x01)
#define NBCFG_Getconfig_DTLSEnabled(data) ((data >> 6) & 0x01)

#define NBCFG_GetByteHi(p) (((uint8_t)(*p) >> 4) & 0x0F)
#define NBCFG_GetByteLow(p) ((uint8_t)(*p) & 0x0F)

#define NBCFG_DEFAULT_BOOTSTRAP ("183.230.40.39")
#define NBCFG_DEFAULT_MTU (1024)

static int mipl_checkBigendian()
{
    union {
        unsigned long int i;
        unsigned char s[4];
    } c;

    c.i = 0x12345678;
    return (0x12 == c.s[0]);
}

static uint16_t miplMakeU16(uint8_t *p)
{
    uint16_t data16;
    uint8_t *pdata8 = (uint8_t *)&data16;

#if NBCFG_USE_BIGENDIAN
    if (mipl_checkBigendian())
    {
        pdata8[0] = *((uint8_t *)p);
        pdata8[1] = *((uint8_t *)p + 1);
    }
    else
    {
        pdata8[1] = *((uint8_t *)p);
        pdata8[0] = *((uint8_t *)p + 1);
    }
#else
    if (mipl_checkBigendian())
    {
        pdata8[1] = *((uint8_t *)p + 0);
        pdata8[0] = *((uint8_t *)p + 1);
    }
    else
    {
        pdata8[0] = *((uint8_t *)p + 0);
        pdata8[1] = *((uint8_t *)p + 1);
    }
#endif

    return data16;
}

static bool miplCheckSum(uint8_t *config, uint16_t size, uint32_t checksum)
{
    return true;
}

static bool miplInitParser(mipl_cfg_context_t *context, uint8_t *config, uint16_t size)
{
    OSI_LOGI(0x10007bf6, "MIPL# >>Config Init");
    OSI_LOGI(0x10007bf7, "MIPL# ----------------");

    return true;
}

static bool miplSysParser(mipl_cfg_context_t *context, uint8_t *config, uint16_t size)
{
    mipl_cfg_sys_t *cfgSys = &context->cfgSys;
    uint8_t *cfg_ptr = config;
    cfgSys->log_enabled = NBCFG_Getconfig_LogEnabled(*cfg_ptr);
    cfgSys->log_ext_output = NBCFG_Getconfig_LogExtoutput(*cfg_ptr);
    cfgSys->log_output_type = NBCFG_Getconfig_LogType(*cfg_ptr);
    cfgSys->log_output_level = NBCFG_Getconfig_LogOutputlevel(*cfg_ptr);
    cfgSys->log_buffer_size = NBCFG_MAKE_U16(cfg_ptr + 1);
    cfg_ptr += 3;
    cfg_ptr += sizeof(uint8_t); //reserve

    cfgSys->user_data.len = NBCFG_MAKE_U16(cfg_ptr);
    cfg_ptr += 2;
    cfgSys->user_data.data = NULL;
    if (cfgSys->user_data.len > 0)
    {
        cfgSys->user_data.data = cfg_ptr;
    }
    cfg_ptr += cfgSys->user_data.len;

    OSI_LOGI(0x10007bf8, "MIPL# >>Config Sys");
    OSI_LOGI(0x10007bf9, "MIPL# Log Enabled:%u", cfgSys->log_enabled);
    OSI_LOGI(0x10007bfa, "MIPL# Log Extend Output Type:%u", cfgSys->log_ext_output);
    OSI_LOGI(0x10007bfb, "MIPL# Log Output Type:%u", cfgSys->log_output_type);
    OSI_LOGI(0x10007bfc, "MIPL# Log Output Level:%u", cfgSys->log_output_level);
    OSI_LOGI(0x10007bfd, "MIPL# Log Buffer Size:%u", cfgSys->log_buffer_size);
    OSI_LOGI(0x10007bfe, "MIPL# Userdata Len:%u", cfgSys->user_data.len);
    OSI_LOGI(0x10007bf7, "MIPL# ----------------");

    return true;
}

static bool miplNetParser(mipl_cfg_context_t *context, uint8_t *config, uint16_t size)
{
    uint8_t *cfg_ptr = config;
    mipl_cfg_net_t *cfgNet = &context->cfgNet;
    cfgNet->mtu = NBCFG_MAKE_U16(cfg_ptr);
    cfg_ptr += sizeof(uint16_t);
    cfgNet->linktype = NBCFG_GetByteHi(cfg_ptr);
    cfgNet->bandtype = NBCFG_GetByteLow(cfg_ptr);
    cfg_ptr += sizeof(uint8_t);
    cfgNet->bs_enabled = NBCFG_Getconfig_BSEnabled(*cfg_ptr);
    cfgNet->dtls_enabled = NBCFG_Getconfig_DTLSEnabled(*cfg_ptr);
    cfg_ptr += sizeof(uint8_t); //reserve

    cfgNet->apn.len = NBCFG_MAKE_U16(cfg_ptr);
    cfg_ptr += sizeof(uint16_t);
    cfgNet->apn.data = (uint8_t *)cfg_ptr;
    cfg_ptr += cfgNet->apn.len;

    cfgNet->username.len = NBCFG_MAKE_U16(cfg_ptr);
    cfg_ptr += sizeof(uint16_t);
    cfgNet->username.data = (uint8_t *)cfg_ptr;
    cfg_ptr += cfgNet->username.len;

    cfgNet->password.len = NBCFG_MAKE_U16(cfg_ptr);
    cfg_ptr += sizeof(uint16_t);
    cfgNet->password.data = (uint8_t *)cfg_ptr;
    cfg_ptr += cfgNet->password.len;

    cfgNet->host.len = NBCFG_MAKE_U16(cfg_ptr);
    cfg_ptr += sizeof(uint16_t);
    cfgNet->host.data = (uint8_t *)cfg_ptr;
    cfg_ptr += cfgNet->host.len;

    cfgNet->user_data.len = NBCFG_MAKE_U16(cfg_ptr);
    cfg_ptr += sizeof(uint16_t);
    cfgNet->user_data.data = (uint8_t *)cfg_ptr;
    cfg_ptr += cfgNet->user_data.len;

    OSI_LOGI(0x10007bff, "MIPL# >>Config Net");
    OSI_LOGI(0x10007c00, "MIPL# Net Mtu:%u", cfgNet->mtu);
    OSI_LOGI(0x10007c01, "MIPL# Net Linktype:%u", cfgNet->linktype);
    OSI_LOGI(0x10007c02, "MIPL# Net Bandtype:%u", cfgNet->bandtype);
    OSI_LOGI(0x10007c03, "MIPL# Net bs_enabled:%u", cfgNet->bs_enabled);
    OSI_LOGI(0x10007c04, "MIPL# Net dtls_enabled:%u", cfgNet->dtls_enabled);
    if (cfgNet->username.len > 0)
    {
        OSI_LOGXI(OSI_LOGPAR_S, 0x10007c05, "MIPL# Net Username:%s", cfgNet->username.data);
    }
    if (cfgNet->password.len > 0)
    {
        OSI_LOGXI(OSI_LOGPAR_S, 0x10007c06, "MIPL# Net Password:%s", cfgNet->password.data);
    }
    if (cfgNet->apn.len > 0)
    {
        OSI_LOGXI(OSI_LOGPAR_S, 0x10007c07, "MIPL# Net Apn:%s", cfgNet->apn.data);
    }
    if (cfgNet->host.len > 0)
    {
        OSI_LOGXI(OSI_LOGPAR_S, 0x10007c08, "MIPL# Net Host:%s", cfgNet->host.data);
    }

    OSI_LOGI(0x10007bfe, "MIPL# Userdata Len:%u", cfgNet->user_data.len);
    OSI_LOGI(0x10007bf7, "MIPL# ----------------");

    return true;
}

static void mipl_config_default(mipl_cfg_context_t *context)
{
    mipl_cfg_net_t *cfgNet = &context->cfgNet;
    mipl_cfg_sys_t *cfgSys = &context->cfgSys;

#if MIPL_ENABLE_LOG
    cfgSys->log_enabled = true;
#else
    cfgSys->log_enabled = false;
#endif
    cfgSys->log_ext_output = 0;
    cfgSys->log_output_type = 0;
    cfgSys->log_output_level = 3;
    cfgSys->log_buffer_size = 200;
    cfgSys->user_data.len = 0;
    cfgSys->user_data.data = NULL;

    cfgNet->mtu = NBCFG_DEFAULT_MTU;
    cfgNet->linktype = 0;
    cfgNet->bandtype = 0;
    cfgNet->apn.len = 0;
    cfgNet->apn.data = NULL;
    cfgNet->username.len = 0;
    cfgNet->username.data = NULL;
    cfgNet->password.len = 0;
    cfgNet->password.data = NULL;
    cfgNet->bs_enabled = true;
    cfgNet->dtls_enabled = true;
    cfgNet->host.len = (uint16_t)strlen(NBCFG_DEFAULT_BOOTSTRAP);
    cfgNet->host.data = (uint8_t *)NBCFG_DEFAULT_BOOTSTRAP;
    cfgNet->user_data.len = 0;
    cfgNet->user_data.data = NULL;

    OSI_LOGI(0x10007c09, "MIPL# >>Default Config Sys");
    OSI_LOGI(0x10007bf9, "MIPL# Log Enabled:%u", cfgSys->log_enabled);
    OSI_LOGI(0x10007bfa, "MIPL# Log Extend Output Type:%u", cfgSys->log_ext_output);
    OSI_LOGI(0x10007bfb, "MIPL# Log Output Type:%u", cfgSys->log_output_type);
    OSI_LOGI(0x10007bfc, "MIPL# Log Output Level:%u", cfgSys->log_output_level);
    OSI_LOGI(0x10007bfd, "MIPL# Log Buffer Size:%u", cfgSys->log_buffer_size);
    OSI_LOGI(0x10007bfe, "MIPL# Userdata Len:%u", cfgSys->user_data.len);

    OSI_LOGI(0x10007c0a, "MIPL# >>Default Config Net");
    OSI_LOGI(0x10007c00, "MIPL# Net Mtu:%u", cfgNet->mtu);
    OSI_LOGI(0x10007c01, "MIPL# Net Linktype:%u", cfgNet->linktype);
    OSI_LOGI(0x10007c02, "MIPL# Net Bandtype:%u", cfgNet->bandtype);

    if (cfgNet->username.len > 0)
    {
        OSI_LOGI(0x10007c05, "MIPL# Net Username:%s", cfgNet->username.data);
    }
    if (cfgNet->password.len > 0)
    {
        OSI_LOGI(0x10007c06, "MIPL# Net Password:%s", cfgNet->password.data);
    }
    if (cfgNet->apn.len > 0)
    {
        OSI_LOGI(0x10007c07, "MIPL# Net Apn:%s", cfgNet->apn.data);
    }
    if (cfgNet->host.len > 0)
    {
        OSI_LOGI(0x10007c08, "MIPL# Net Host:%s", cfgNet->host.data);
    }

    OSI_LOGI(0x10007bfe, "MIPL# Userdata Len:%u", cfgNet->user_data.len);
}

bool mipl_config_init(mipl_cfg_context_t *config_context, void *config_ptr, uint16_t config_len)
{
    int loop;
    uint8_t *cfg_ptr = (uint8_t *)config_ptr;
    uint32_t checksum = 0;
    mipl_cfg_header_t *cfgHeader = NULL;

    if (config_context == NULL)
    {
        return false;
    }

    if (config_ptr == NULL || config_len == 0)
    {
        mipl_config_default(config_context);
        return true;
    }

    cfgHeader = &(config_context->cfgHeader);
    cfgHeader->version = NBCFG_GetVersion(*cfg_ptr);
    cfgHeader->cfgcount = NBCFG_GetCC(*cfg_ptr);
    cfgHeader->bytes = NBCFG_GetConfigSize(cfg_ptr + sizeof(uint8_t));
    cfg_ptr += NBCFG_HeaderSize;

    OSI_LOGI(0x10007c0b, "MIPL# Config Header Info:");
    OSI_LOGI(0x10007c0c, "MIPL# Version:[0x%02x]", cfgHeader->version);
    OSI_LOGI(0x10007c0d, "MIPL# Version Support:[0x%02x]", NBCFG_SUPPORT_VERSION);
    OSI_LOGI(0x10007c0e, "MIPL# Count:%d", cfgHeader->cfgcount);
    OSI_LOGI(0x10007c0f, "MIPL# Size:%d bytes", cfgHeader->bytes);
    OSI_LOGI(0x10007bf7, "MIPL# ----------------");

    if (cfgHeader->version != NBCFG_SUPPORT_VERSION)
    {
        int i = 0;
        for (i = 0; i < sizeof(MiplVersionUnsupportList) / sizeof(uint8_t); i++)
        {
            if (MiplVersionUnsupportList[i] == cfgHeader->version)
            {
                OSI_LOGI(0x10007c10, "MIPL# ERROR:config init failed. Don't support version.");
                return false;
            }
        }
    }

    if (cfgHeader->bytes != config_len)
    {
        OSI_LOGI(0x10007c11, "MIPL# ERROR:config init failed.config len invalid.(%u vs %u)", cfgHeader->bytes, config_len);
        return false;
    }

    if (!miplCheckSum((uint8_t *)config_ptr, config_len, checksum))
    {
        OSI_LOGI(0x10007c12, "MIPL# ERROR:config init failed from checksum.");
        return false;
    }

    for (loop = 0; loop < cfgHeader->cfgcount; loop++)
    {
        uint8_t item_header = *(uint8_t *)cfg_ptr;
        mipl_cfgid_t cfgid = NBCFG_GetConfigId(item_header);
        uint16_t cfg_size = NBCFG_MAKE_U16(cfg_ptr + sizeof(uint8_t));
        uint8_t item_headlen = sizeof(uint8_t) + sizeof(cfg_size);

        uint8_t result = 0;
        switch (cfgid)
        {
        case mipl_cfgid_init:
        {
            result = miplInitParser(config_context, cfg_ptr + item_headlen, cfg_size - item_headlen);
        }
        break;
        case mipl_cfgid_net:
        {
            result = miplNetParser(config_context, cfg_ptr + item_headlen, cfg_size - item_headlen);
        }
        break;
        case mipl_cfgid_sys:
        {
            result = miplSysParser(config_context, cfg_ptr + item_headlen, cfg_size - item_headlen);
        }
        break;
        }

        if (!result)
        {
            OSI_LOGI(0x10007c13, "MIPL# ERROR:config parser failed, cfgid=%d", cfgid);
            return false;
        }

        cfg_ptr += cfg_size;
    }

    OSI_LOGI(0x10007c14, "MIPL# The config file parse finish.");
    return true;
}

#endif
