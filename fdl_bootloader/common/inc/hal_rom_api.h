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

#ifndef _HAL_ROM_API_H_
#define _HAL_ROM_API_H_

//#include "osi_compiler.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
//OSI_EXTERN_C_BEGIN

/**
 * \brief parameter for loader entry
 *
 * This is pre-defined loader parameters. For example, when FDL1 can
 * support download from multiple devices, the parameter will indicate
 * the download device. So, FDL1 will known the download device.
 */
enum
{
    ROM_LOADPAR_NORMAL = 0,
    ROM_LOADPAR_DOWNLOAD_UART1 = 0x01,
    ROM_LOADPAR_DOWNLOAD_UART2 = 0x02,
    ROM_LOADPAR_DOWNLOAD_UART3 = 0x03,
    ROM_LOADPAR_DOWNLOAD_UART4 = 0x04,
    ROM_LOADPAR_DOWNLOAD_UART5 = 0x05,
    ROM_LOADPAR_DOWNLOAD_UART6 = 0x06,
    ROM_LOADPAR_DOWNLOAD_USERIAL = 0x08,
    ROM_LOADPAR_FROM_BOOT2 = 0x10,
    ROM_LOADPAR_FROM_SDCARD = 0x11,
};

typedef struct
{
    uint32_t block_start;
    uint32_t block_end;
    unsigned req_size;
    unsigned alloc_size;
    unsigned req_size_max;
    unsigned alloc_size_max;
    unsigned high_mark;
} halRomMemPool_t;

typedef struct
{
    uint8_t *in;
    uint32_t len;
    uint8_t *out;
    uint8_t *key;
    uint8_t *iv;
    bool key_from_kce;
} sym_dec_t;

typedef struct
{
    uint32_t data[8 / 4];
} osiBits64_t, osiBytes8_t;

typedef struct
{
    uint32_t data[16 / 4];
} osiBits128_t, osiBytes16_t;

typedef struct
{
    uint32_t data[32 / 4];
} osiBits256_t, osiBytes32_t;

typedef struct
{
    uint32_t data[64 / 4];
} osiBits512_t, osiBytes64_t;

typedef struct
{
    uint32_t data[80 / 4];
} osiBits640_t, osiBytes80_t;

typedef struct
{
    uint32_t data[128 / 4];
} osiBits1024_t, osiBytes128_t;

typedef struct
{
    uint32_t data[512 / 4];
} osiBits4096_t, osiBytes512_t;
/**
 * \brief signed image header
 *
 * The structure of signed image is:
 * - header
 * - data
 * - private data
 * - key cert
 * - private data cert
 * - debug primary cert
 * - debug devel cert
 * - chain cert
 *
 * Only header is mendatory, all others are optional. The cert names are for
 * bootloader. For other images, most likely, debug certs won't appear.
 */
typedef struct simageHeader
{
    uint32_t magic;                ///< Magic number: SIHR
    osiBytes32_t header_checksum;  ///< Image header checksum. Checksum calculation offset is \p SIMAGE_HEADER_CHECKSUM_OFFSET.
    uint32_t image_size;           ///< Total image size
    uint8_t image_name[64];        ///< Image name. The terminate NUL should be included
    uint16_t format_version;       ///< Image format version.
    uint16_t ver_patch;            ///< Version patch level.
    uint16_t ver_minor;            ///< Minor version
    uint16_t ver_major;            ///< Major version
    uint32_t revision;             ///< Revision information. Usually, it is SVN revision number or the first 4 bytes of git hash.
    uint32_t platform;             ///< Platform ID, 0 for unknown
    uint32_t timestamp;            ///< Creation timestamp, seconds since epoch
    uint32_t header_size;          ///< Header size. Currently, 256B is used, other sizes are reserved for future.
    uint32_t header_flags;         ///< Header flags. Currently, [3:0] is used to indicate checksum method
    osiBytes32_t payload_checksum; ///< Payload (data after header) checksum. Fill zero when there are no payload
    uint32_t data_offset;          ///< Data offset in the image, from the beginning of image.
    uint32_t data_size;            ///< Data size, not including padding. 0 for not exist.
    uint32_t data_flags;           ///< Data flags. [3:0] for encryption type
    uint32_t data_load;            ///< Data load address.
    uint32_t data_load_hi;         ///< Data load address [63:32]
    uint32_t data_entry;           ///< Data entry address.
    uint32_t data_entry_hi;        ///< Data entry address [63:32]
    uint32_t priv_data_offset;     ///< Private data offset in the image, from the beginning of image.
    uint32_t priv_data_size;       ///< Private data size, not including padding. 0 for not exist.
    uint32_t priv_data_flags;      ///< Private data flags, reserved for future.
    uint32_t keycert_offset;       ///< Key cert offset in the image, from the beginning of image.
    uint32_t keycert_size;         ///< Key cert size, 0 for not exist.
    uint32_t privcert_offset;      ///< Private data cert offset in the image, from the beginning of image.
    uint32_t privcert_size;        ///< Private data cert size, 0 for not exist.
    uint32_t debugcert_offset;     ///< Debug cert offset in the image, from the beginning of image
    uint32_t debugcert_size;       ///< Debug cert size, 0 for not exist.
    uint32_t develcert_offset;     ///< Developer debug cert offset in the image, from the beginning of image
    uint32_t develcert_size;       ///< Developer debug cert size, 0 for not exist.
    uint32_t chaincert_offset;     ///< Chain cert offset in the image, from the beginning of image.
    uint32_t chaincert_size;       ///< Chain cert size, 0 for not exist.
    uint32_t zero[3];              ///< Zero padding to 256 bytes.
} simageHeader_t;
bool halRomApiInit(void);

void halRomSetLogPrintf(void *fn);

halRomMemPool_t *halRomGetMemPool(void);
bool halRomSetMemPool(void *mem, unsigned size);
void *halRomMalloc(unsigned size);
void halRomFree(void *ptr);

typedef struct halRomSha256Context halRomSha256Context_t;
unsigned halRomSha256ContextSize(void);
void halRomSha256Init(halRomSha256Context_t *ctx);
void halRomSha256Update(halRomSha256Context_t *ctx, const void *input, unsigned size);
void halRomSha256Final(halRomSha256Context_t *ctx, void *output);
void halRomSha256Calc(const void *input, unsigned size, void *output);

typedef struct halRomSm3Context halRomSm3Context_t;
unsigned halRomSm3ContextSize(void);
void halRomSm3Init(halRomSm3Context_t *ctx);
void halRomSm3Update(halRomSm3Context_t *ctx, const void *input, unsigned size);
void halRomSm3Final(halRomSm3Context_t *ctx, void *output);
void halRomSm3Calc(const void *input, unsigned size, void *output);

bool halRomRsaVerify(unsigned nbytes, unsigned e, const void *n,
                     unsigned padtype, const void *sigdata, unsigned sigbytes,
                     const void *hash, unsigned hashbytes);

bool halRomEccVerify(unsigned curve, const void *ecp_x, unsigned xbytes,
                     const void *ecp_y, unsigned ybytes,
                     const void *sig_r, unsigned rbytes,
                     const void *sig_s, unsigned sbytes,
                     const void *hash, unsigned hashbytes);

bool halRomSm2Verify(const void *ecp_x, const void *ecp_y,
                     const void *sig_r, const void *sig_s,
                     const void *hash, unsigned hashbytes);

bool halRomSm4Decrypt(sym_dec_t *param);
bool halRomAesDecrypt(sym_dec_t *param);

typedef struct simageHeader simageHeader_t;
bool halRomSimageSignVerify(const simageHeader_t *header,
                            const osiBits256_t *rotpk,
                            const osiBits64_t *soc_raw_id,
                            unsigned min_secure_version);
bool halRomSimageDataDecrypt(simageHeader_t *header);
bool halRomSimageHeaderValid(const simageHeader_t *header, unsigned max_size);
bool halRomSimagePayloadValid(const simageHeader_t *header);
bool halRomSimageValid(const simageHeader_t *header, unsigned max_size);

void *halRomGetUserial(void);
void halRomUserialOpen(void *ser);
int halRomUserialGetChar(void *ser);
int halRomUserialPutChar(void *ser, uint8_t ch);
void halRomUserialPoll(void *ser);
void halRomUserialFlush(void *ser);
void *getEfuseContent(void);

//OSI_EXTERN_C_END
#endif
