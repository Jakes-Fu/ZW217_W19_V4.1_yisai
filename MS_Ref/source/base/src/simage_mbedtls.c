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

//#include "simage.h"
#include "crc32.h"
//#include "mbedtls/pk.h"
//#include "sha256.h"
//#include "mbedtls/asn1.h"
//#include "mbedtls/asn1write.h"
//#include "mbedtls/ecdsa.h"
//#include "osi_api.h"
//#include "osi_log.h"
#ifdef CONFIG_MBEDTLS_SUPPORT_SM2
//#include "mbedtls/ecdsa_sm2.h"
#endif
#include "rsa.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

bool rsaVerify(unsigned pubkey_n_bytes, unsigned pubkey_e,
               const void *pubkey_n, unsigned pubkey_padding,
               const void *sig_data, unsigned sig_bytes,
               const void *hash, unsigned hash_bytes)
{
    mbedtls_pk_context ctx;
    mbedtls_pk_init(&ctx);

    const mbedtls_pk_info_t *pk_info = mbedtls_pk_info_from_type(MBEDTLS_PK_RSA);
    int ret = mbedtls_pk_setup(&ctx, pk_info);
    if (ret != 0)
        goto failed;

    mbedtls_rsa_context *rsa = mbedtls_pk_rsa(ctx);
    int padding = (pubkey_padding == SIMAGE_PKATYPE_RSA) ? MBEDTLS_RSA_PKCS_V15 : MBEDTLS_RSA_PKCS_V21;
    mbedtls_rsa_init(rsa, padding, MBEDTLS_MD_SHA256);

    ret = mbedtls_mpi_read_binary(&rsa->N, (const unsigned char *)pubkey_n, pubkey_n_bytes);
    if (ret != 0)
        goto failed;
    ret = mbedtls_mpi_read_binary(&rsa->E, (const unsigned char *)&pubkey_e, 4);
    if (ret != 0)
        goto failed;

    rsa->len = (mbedtls_mpi_bitlen(&rsa->N) + 7) >> 3;

    ret = mbedtls_pk_verify(&ctx, MBEDTLS_MD_SHA256,
                            (const unsigned char *)hash, 0,
                            (const unsigned char *)sig_data, sig_bytes);
    if (ret != 0)
        goto failed;

    mbedtls_pk_free(&ctx);
    return true;

failed:
    mbedtls_pk_free(&ctx);
    return false;
}

bool eccVerify(unsigned curve, const void *ecp_x, unsigned xbytes,
               const void *ecp_y, unsigned ybytes,
               const void *sig_r, unsigned rbytes,
               const void *sig_s, unsigned sbytes,
               const void *hash, unsigned hash_bytes)
{
#ifdef CONFIG_MBEDTLS_SUPPORT_ECP
    mbedtls_pk_context ctx;
    mbedtls_pk_init(&ctx);

    mbedtls_mpi r, s;
    mbedtls_mpi_init(&r);
    mbedtls_mpi_init(&s);

    const mbedtls_pk_info_t *pk_info = mbedtls_pk_info_from_type(MBEDTLS_PK_ECKEY);
    int ret = mbedtls_pk_setup(&ctx, pk_info);
    if (ret != 0)
        goto failed;

    mbedtls_ecp_keypair *ecp = mbedtls_pk_ec(ctx);
    mbedtls_ecp_group_init(&ecp->grp);

    mbedtls_ecp_group_id grp_id;
    switch (curve)
    {
    case SIMAGE_ECC_CURVE_secp192k1:
        grp_id = MBEDTLS_ECP_DP_SECP192K1;
        break;
    case SIMAGE_ECC_CURVE_secp192r1:
        grp_id = MBEDTLS_ECP_DP_SECP192R1;
        break;
    case SIMAGE_ECC_CURVE_secp224k1:
        grp_id = MBEDTLS_ECP_DP_SECP224K1;
        break;
    case SIMAGE_ECC_CURVE_secp224r1:
        grp_id = MBEDTLS_ECP_DP_SECP224R1;
        break;
    case SIMAGE_ECC_CURVE_secp256k1:
        grp_id = MBEDTLS_ECP_DP_SECP256K1;
        break;
    case SIMAGE_ECC_CURVE_secp256r1:
        grp_id = MBEDTLS_ECP_DP_SECP256R1;
        break;
    case SIMAGE_ECC_CURVE_secp384r1:
        grp_id = MBEDTLS_ECP_DP_SECP384R1;
        break;
    case SIMAGE_ECC_CURVE_secp521r1:
        grp_id = MBEDTLS_ECP_DP_SECP521R1;
        break;
    default:
        goto failed;
    }

    const mbedtls_ecp_curve_info *curve_info = mbedtls_ecp_curve_info_from_grp_id(grp_id);
    if (curve_info == NULL)
        goto failed;

    unsigned mpibytes = (curve_info->bit_size + 7) / 8;
    if (xbytes < mpibytes || ybytes < mpibytes || rbytes < mpibytes || sbytes < mpibytes)
        goto failed;

    ret = mbedtls_ecp_group_load(&ecp->grp, grp_id);
    if (ret != 0)
        goto failed;

    ret = mbedtls_mpi_read_binary(&ecp->Q.X, (const unsigned char *)ecp_x, mpibytes);
    if (ret != 0)
        goto failed;

    ret = mbedtls_mpi_read_binary(&ecp->Q.Y, (const unsigned char *)ecp_y, mpibytes);
    if (ret != 0)
        goto failed;

    mbedtls_mpi_lset(&ecp->Q.Z, 1);

    // It is tricky to read signature from simage data structure
    ret = mbedtls_mpi_read_binary(&r, (const unsigned char *)sig_r, mpibytes);
    if (ret != 0)
        goto failed;

    ret = mbedtls_mpi_read_binary(&s, (const unsigned char *)sig_s, mpibytes);
    if (ret != 0)
        goto failed;

    unsigned char buf[MBEDTLS_ECDSA_MAX_LEN];
    unsigned char *p = buf + sizeof(buf);
    size_t len = 0;

    ret = mbedtls_asn1_write_mpi(&p, buf, &s);
    if (ret < 0)
        goto failed;
    len += ret;

    ret = mbedtls_asn1_write_mpi(&p, buf, &r);
    if (ret < 0)
        goto failed;
    len += ret;

    ret = mbedtls_asn1_write_len(&p, buf, len);
    if (ret < 0)
        goto failed;
    len += ret;

    ret = mbedtls_asn1_write_tag(&p, buf, MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE);
    if (ret < 0)
        goto failed;
    len += ret;

    ret = mbedtls_pk_verify(&ctx, MBEDTLS_MD_SHA256,
                            (const unsigned char *)hash, 0,
                            (const unsigned char *)p, len);
    if (ret != 0)
        goto failed;

    mbedtls_mpi_free(&r);
    mbedtls_mpi_free(&s);
    mbedtls_pk_free(&ctx);
    return true;

failed:
    mbedtls_mpi_free(&r);
    mbedtls_mpi_free(&s);
    mbedtls_pk_free(&ctx);
#endif

    return false;
}

bool sm2Verify(const void *ecp_x, const void *ecp_y, const void *sig_r, const void *sig_s, const void *hash, unsigned hashbytes)
{
#ifdef CONFIG_MBEDTLS_SUPPORT_SM2
    mbedtls_ecdsa_context ctx;
    mbedtls_ecdsa_init(&ctx);

    mbedtls_mpi r, s;
    mbedtls_mpi_init(&r);
    mbedtls_mpi_init(&s);

    unsigned mpibytes = 256 / 8;
    mbedtls_ecp_keypair *ecp = &ctx;

    int ret = sm2_param_load(&ecp->grp);
    if (ret != 0)
        goto failed;

    ret = mbedtls_mpi_read_binary(&ecp->Q.X, (const unsigned char *)ecp_x, mpibytes);
    if (ret != 0)
        goto failed;

    ret = mbedtls_mpi_read_binary(&ecp->Q.Y, (const unsigned char *)ecp_y, mpibytes);
    if (ret != 0)
        goto failed;

    mbedtls_mpi_lset(&ecp->Q.Z, 1);

    // It is tricky to read signature from simage data structure
    ret = mbedtls_mpi_read_binary(&r, (const unsigned char *)sig_r, mpibytes);
    if (ret != 0)
        goto failed;

    ret = mbedtls_mpi_read_binary(&s, (const unsigned char *)sig_s, mpibytes);
    if (ret != 0)
        goto failed;

    ret = mbedtls_ecdsa_sm2_verify(&ecp->grp, hash, hashbytes,
                                   &ecp->Q, &r, &s);

    if (ret != 0)
        goto failed;

    mbedtls_mpi_free(&r);
    mbedtls_mpi_free(&s);
    mbedtls_ecdsa_free(&ctx);

    return true;

failed:
    mbedtls_mpi_free(&r);
    mbedtls_mpi_free(&s);
    mbedtls_ecdsa_free(&ctx);
#endif

    return false;
}
