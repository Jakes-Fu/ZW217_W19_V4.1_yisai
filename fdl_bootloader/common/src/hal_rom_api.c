/* Copyright (C) 2017 RDA Technologies Limited and/or its affiliates("RDA").
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

#include "hal_rom_api.h"

typedef struct
{
    unsigned version;  ///< ROM API compatibility version
    unsigned revision; ///< revision of sources

    void *delay_us_fn;
    void *apply_reg_fn;
    void *apply_reglist_fn;
    void (*set_log_printf_fn)(void *fn);
    halRomMemPool_t *(*get_mem_pool_fn)(void);
    bool (*set_mem_pool_fn)(void *mem, unsigned size);
    void *(*malloc_fn)(unsigned size);
    void (*free_fn)(void *ptr);
    void *spi_flash_cmd_fn;
    void *spi_flash_cmd_no_rx_fn;
    void *spi_flash_cmd_only_fn;
    void *spi_flash_cmd_readback_fn;
    void *(*get_efuse_content_fn)(void);
    void *crc32_init_fn;
    void *crc32_update_fn;
    void *crc32_calc_fn;
    unsigned (*sha256_context_size_fn)(void);
    void (*sha256_init_fn)(halRomSha256Context_t *ctx);
    void (*sha256_update_fn)(halRomSha256Context_t *ctx, const void *input, unsigned size);
    void (*sha256_final_fn)(halRomSha256Context_t *ctx, void *output);
    void (*sha256_calc_fn)(const void *input, unsigned size, void *output);
    unsigned (*sm3_context_size_fn)(void);
    void (*sm3_init_fn)(halRomSm3Context_t *ctx);
    void (*sm3_update_fn)(halRomSm3Context_t *ctx, const void *input, unsigned size);
    void (*sm3_final_fn)(halRomSm3Context_t *ctx, void *output);
    void (*sm3_calc_fn)(const void *input, unsigned size, void *output);
    bool (*rsa_verify_fn)(unsigned nbytes, unsigned e, const void *n,
                          unsigned padtype, const void *sigdata, unsigned sigbytes,
                          const void *hash, unsigned hashbytes);
    bool (*ecc_verify_fn)(unsigned curve, const void *ecp_x, unsigned xbytes,
                          const void *ecp_y, unsigned ybytes,
                          const void *sig_r, unsigned rbytes,
                          const void *sig_s, unsigned sbytes,
                          const void *hash, unsigned hashbytes);
    bool (*sm2_verify_fn)(const void *ecp_x, const void *ecp_y,
                          const void *sig_r, const void *sig_s,
                          const void *hash, unsigned hashbytes);
    bool (*sm4_decrypt_fn)(sym_dec_t *param);
    bool (*aes_decrypt_fn)(sym_dec_t *param);
    bool (*simage_verify_fn)(const simageHeader_t *header,
                             const osiBits256_t *rotpk,
                             const osiBits64_t *soc_id,
                             unsigned min_secure_version);
    bool (*simage_data_decrypt_fn)(simageHeader_t *header);
    bool (*simage_header_valid_fn)(const simageHeader_t *header, unsigned max_size);
    bool (*simage_payload_valid_fn)(const simageHeader_t *header);
    bool (*simage_valid_fn)(const simageHeader_t *header, unsigned max_size);
    void *(*get_userial)(void);
    void (*userial_open)(void *ser);
    int (*userial_getchar)(void *ser);
    int (*userial_putchar)(void *ser, uint8_t ch);
    void (*userial_poll)(void *ser);
    void (*userial_flush)(void *ser);
} romApi_t;

#define ROM_API (*(const romApi_t **)0x20)

bool halRomApiInit(void)
{
    return true;
}

void halRomSetLogPrintf(void *fn)
{
    ROM_API->set_log_printf_fn(fn);
}

halRomMemPool_t *halRomGetMemPool(void)
{
    return ROM_API->get_mem_pool_fn();
}

bool halRomSetMemPool(void *mem, unsigned size)
{
    return ROM_API->set_mem_pool_fn(mem, size);
}

void *halRomMalloc(unsigned size)
{
    return ROM_API->malloc_fn(size);
}

void halRomFree(void *ptr)
{
    ROM_API->free_fn(ptr);
}

unsigned halRomSha256ContextSize(void)
{
    return ROM_API->sha256_context_size_fn();
}

void halRomSha256Init(halRomSha256Context_t *ctx)
{
    ROM_API->sha256_init_fn(ctx);
}

void halRomSha256Update(halRomSha256Context_t *ctx, const void *input, unsigned size)
{
    ROM_API->sha256_update_fn(ctx, input, size);
}

void halRomSha256Final(halRomSha256Context_t *ctx, void *output)
{
    ROM_API->sha256_final_fn(ctx, output);
}

void halRomSha256Calc(const void *input, unsigned size, void *output)
{
    ROM_API->sha256_calc_fn(input, size, output);
}

unsigned halRomSm3ContextSize(void)
{
    return ROM_API->sm3_context_size_fn();
}

void halRomSm3Init(halRomSm3Context_t *ctx)
{
    ROM_API->sm3_init_fn(ctx);
}

void halRomSm3Update(halRomSm3Context_t *ctx, const void *input, unsigned size)
{
    ROM_API->sm3_update_fn(ctx, input, size);
}

void halRomSm3Final(halRomSm3Context_t *ctx, void *output)
{
    ROM_API->sm3_final_fn(ctx, output);
}

void halRomSm3Calc(const void *input, unsigned size, void *output)
{
    ROM_API->sm3_calc_fn(input, size, output);
}

bool halRomRsaVerify(unsigned nbytes, unsigned e, const void *n,
                     unsigned padtype, const void *sigdata, unsigned sigbytes,
                     const void *hash, unsigned hashbytes)
{
    return ROM_API->rsa_verify_fn(nbytes, e, n, padtype, sigdata, sigbytes,
                                  hash, hashbytes);
}

bool halRomEccVerify(unsigned curve, const void *ecp_x, unsigned xbytes,
                     const void *ecp_y, unsigned ybytes,
                     const void *sig_r, unsigned rbytes,
                     const void *sig_s, unsigned sbytes,
                     const void *hash, unsigned hashbytes)
{
    return ROM_API->ecc_verify_fn(curve, ecp_x, xbytes, ecp_y, ybytes,
                                  sig_r, rbytes, sig_s, sbytes,
                                  hash, hashbytes);
}

bool halRomSm2Verify(const void *ecp_x, const void *ecp_y,
                     const void *sig_r, const void *sig_s,
                     const void *hash, unsigned hashbytes)
{
    return ROM_API->sm2_verify_fn(ecp_x, ecp_y, sig_r, sig_s,
                                  hash, hashbytes);
}

bool halRomSm4Decrypt(sym_dec_t *param)
{
    return ROM_API->sm4_decrypt_fn(param);
}

bool halRomAesDecrypt(sym_dec_t *param)
{
    return ROM_API->aes_decrypt_fn(param);
}

bool halRomSimageSignVerify(const simageHeader_t *header,
                            const osiBits256_t *rotpk,
                            const osiBits64_t *soc_raw_id,
                            unsigned min_secure_version)
{
    return ROM_API->simage_verify_fn(header, rotpk, soc_raw_id,
                                     min_secure_version);
}

bool halRomSimageDataDecrypt(simageHeader_t *header)
{
    return ROM_API->simage_data_decrypt_fn(header);
}

bool halRomSimageHeaderValid(const simageHeader_t *header, unsigned max_size)
{
    return ROM_API->simage_header_valid_fn(header, max_size);
}

bool halRomSimagePayloadValid(const simageHeader_t *header)
{
    return ROM_API->simage_payload_valid_fn(header);
}

bool halRomSimageValid(const simageHeader_t *header, unsigned max_size)
{
    return ROM_API->simage_valid_fn(header, max_size);
}

void *halRomGetUserial(void)
{
    return ROM_API->get_userial();
}

void halRomUserialOpen(void *ser)
{
    ROM_API->userial_open(ser);
}

int halRomUserialGetChar(void *ser)
{
    return ROM_API->userial_getchar(ser);
}

int halRomUserialPutChar(void *ser, uint8_t ch)
{
    return ROM_API->userial_putchar(ser, ch);
}

void halRomUserialPoll(void *ser)
{
    ROM_API->userial_poll(ser);
}

void halRomUserialFlush(void *ser)
{
    ROM_API->userial_flush(ser);
}

void *getEfuseContent(void)
{
    return ROM_API->get_efuse_content_fn();
}
