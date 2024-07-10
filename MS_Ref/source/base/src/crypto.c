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

#include "sha256.h"
#include "sm3.h"
#include <string.h>
#include "sci_types.h"
#include "os_api.h"
typedef unsigned char		bool;

#define false 0
#define true 1
typedef struct
{
    uint32 data[32 / 4];
} osiBits256_t, osiBytes32_t;
#if 0
typedef struct mbedtls_sha256_context
{
    uint8_t isClone;
    uint8_t is224;
    uint8_t isStarted;
    void *shactx;
}
mbedtls_sha256_context;
struct sha256Context
{
    mbedtls_sha256_context ctx;
};
typedef struct sha256Context sha256Context_t;

void mbedtls_sha256_init( mbedtls_sha256_context *ctx )
{
    //SHA256_VALIDATE( ctx != NULL );
    memset( ctx, 0, sizeof( mbedtls_sha256_context ) );
    ctx->shactx = malloc(sizeof(sprd_crypto_context_t));
    if(NULL != ctx->shactx)
        memset(ctx->shactx, 0, sizeof(sprd_crypto_context_t));
}
void mbedtls_platform_zeroize( void *buf, size_t len )
{
    //MBEDTLS_INTERNAL_VALIDATE( len == 0 || buf != NULL );

    if( len > 0 )
        memset( buf, 0, len );
}
void mbedtls_sha256_free( mbedtls_sha256_context *ctx )
{
    if( ctx == NULL )
        return;
    if(0 == ctx->isClone ){
       sprdHashFree((sprd_crypto_context_t*)ctx->shactx);
    }
    if(NULL != ctx->shactx){
        free(ctx->shactx);
        ctx->shactx = NULL;
    }
    mbedtls_platform_zeroize( ctx, sizeof( mbedtls_sha256_context ) );
}
/*
 * SHA-256 context setup
 */
int mbedtls_sha256_starts_ret( mbedtls_sha256_context *ctx, int is224 )
{
    //SHA256_VALIDATE_RET( ctx != NULL );
    //SHA256_VALIDATE_RET( is224 == 0 || is224 == 1 );

    sprd_crypto_err_t ret = SPRD_CRYPTO_ERROR;
    if(1 == ctx->isStarted){
        mbedtls_sha256_free(ctx);
        mbedtls_sha256_init(ctx);
    }
    ctx->isStarted = 1;
    if( is224 == 0 )
    {
        /* SHA-256 */
        ctx->is224 = 0;
        ret = sprdHashInit(SPRD_CRYPTO_HASH_SHA256, (sprd_crypto_context_t*)ctx->shactx);
    }
    else
    {
        /* SHA-224 */
        ctx->is224 = 1;
        ret = sprdHashInit(SPRD_CRYPTO_HASH_SHA224, (sprd_crypto_context_t*)ctx->shactx);
    }
    return( ret );
}

/*
 * SHA-256 process buffer
 */
int mbedtls_sha256_update_ret( mbedtls_sha256_context *ctx,
                               const unsigned char *input,
                               size_t ilen )
{
    //SHA256_VALIDATE_RET( ctx != NULL );
    //SHA256_VALIDATE_RET( ilen != 0);
    //SHA256_VALIDATE_RET( input != NULL );

    return sprdHashUpdate((sprd_crypto_context_t*)ctx->shactx, (uint8_t *)input, (uint32_t)ilen);
}


/*
 * SHA-256 final digest
 */
int mbedtls_sha256_finish_ret( mbedtls_sha256_context *ctx,
                               unsigned char output[32] )
{
    //SHA256_VALIDATE_RET( ctx != NULL );
    //SHA256_VALIDATE_RET( (unsigned char *)output != NULL );

    sprd_crypto_err_t ret = SPRD_CRYPTO_ERROR;
    if(NULL == ctx->shactx)
        return ret;
    return sprdHashFinalwithoutFree((sprd_crypto_context_t*)ctx->shactx, output);

}
unsigned sha256ContextSize(void)
{
    return sizeof(sha256Context_t);
}
void sha256Init(sha256Context_t *ctx)
{
    mbedtls_sha256_init(&ctx->ctx);
    mbedtls_sha256_starts_ret(&ctx->ctx, 0);
}
bool sha256Update(sha256Context_t *ctx, const void *p, size_t size)
{
    if (size == 0)
        return true;
    if (p == NULL)
        return false;

    return mbedtls_sha256_update_ret(&ctx->ctx, (const unsigned char *)p, size) == 0;
}
bool sha256Update(sha256Context_t *ctx, const void *p, size_t size)
{
    if (size == 0)
        return true;
    if (p == NULL)
        return false;

    return mbedtls_sha256_update_ret(&ctx->ctx, (const unsigned char *)p, size) == 0;
}
#endif
bool sha256Calc(const void *input, size_t ilen, void *output)
{
    sha256_context *ctx = (sha256_context *)SCI_ALLOC_APP(sizeof(sha256_context));
    sha256_starts(ctx);
    sha256_update(ctx, input, ilen);
    sha256_finish(ctx, output);
    return 1;
}

bool sha256Check(const void *p, size_t size, const void *expected)
{
   osiBits256_t hash;
    if (p == NULL || expected == NULL)
        return false;

    
    if (!sha256Calc(p, size, &hash))
        return false;
    return memcmp(expected, &hash, sizeof(hash)) == 0;
}

bool sm3Calc(const void *input, unsigned ilen, void *output)
{
    SM3(input, ilen, output);
    return true;
}

bool sm3Check(const void *p, unsigned size, const void *expected)
{
osiBits256_t hash;
    if (p == NULL || expected == NULL)
        return false;
	
   SM3(p, size, (unsigned char *)(&hash));
     
    return memcmp(expected, &hash, sizeof(hash)) == 0;
}
