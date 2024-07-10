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

// AUTO GENERATED

#ifndef _RPC_A2C_PAR_H_
#define _RPC_A2C_PAR_H_

#define TAG_rpcHelloOnCP 0x38ba7a57
struct IPAR_rpcHelloOnCP
{
    uint32_t n;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_rpcHelloOnCP));

struct OPAR_rpcHelloOnCP
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_rpcHelloOnCP));

#define TAG_rpcPeerTagSupported 0x16be2d30
struct IPAR_rpcPeerTagSupported
{
    uint32_t tag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_rpcPeerTagSupported));

struct OPAR_rpcPeerTagSupported
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_rpcPeerTagSupported));

#define TAG_rpcPeerTagCount 0xc206373a
struct OPAR_rpcPeerTagCount
{
    int result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_rpcPeerTagCount));

#define TAG_rpcGetPeerTags 0x36271b1c
struct IPAR_rpcGetPeerTags
{
    uint32_t offset;
    uint32_t count;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_rpcGetPeerTags));

struct OPAR_rpcGetPeerTags
{
    int result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_rpcGetPeerTags));


#endif // _RPC_A2C_PAR_H_
// AUTO GENERATED END
