MINCPATH      = $(PARSER_PATH)
ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH   += DAPS/export/inc/tcpip6
MCFLAG_OPT += -DIPV6_SUPPORT
else
MINCPATH   += DAPS/export/inc/tcpip
endif
MINCPATH += external/libc/h \
			external/libc
			
MINCPATH += Third-party/alipay/alimbedtls/include/
MINCPATH += Third-party/alipay/alimbedtls/include/alimbedtls
MSRCPATH += Third-party/alipay/alimbedtls/src

SOURCES += aes.c
SOURCES += aesni.c
SOURCES += arc4.c
SOURCES += asn1parse.c
SOURCES += asn1write.c
SOURCES += base64.c
SOURCES += bignum.c
SOURCES += blowfish.c
SOURCES += camellia.c
SOURCES += ccm.c
SOURCES += certs.c
SOURCES += cipher.c
SOURCES += cipher_wrap.c
SOURCES += cmac.c
SOURCES += ctr_drbg.c
SOURCES += debug.c
SOURCES += des.c
SOURCES += dhm.c
SOURCES += ecdh.c
SOURCES += ecdsa.c
SOURCES += ecjpake.c
SOURCES += ecp.c
SOURCES += ecp_curves.c
SOURCES += entropy.c
SOURCES += entropy_poll.c
SOURCES += error.c
SOURCES += gcm.c
SOURCES += havege.c
SOURCES += hmac_drbg.c
SOURCES += md.c
SOURCES += md2.c
SOURCES += md4.c
SOURCES += md5.c
SOURCES += md_wrap.c
SOURCES += memory_buffer_alloc.c
SOURCES += net_sockets.c
SOURCES += oid.c
SOURCES += padlock.c
SOURCES += pem.c
SOURCES += pk.c
SOURCES += pkcs11.c
SOURCES += pkcs12.c
SOURCES += pkcs5.c
SOURCES += pkparse.c
SOURCES += pkwrite.c
SOURCES += pk_wrap.c
SOURCES += platform.c
SOURCES += ripemd160.c
SOURCES += rsa.c
SOURCES += sha1.c
SOURCES += sha256.c
SOURCES += sha512.c
SOURCES += ssl_cache.c
SOURCES += ssl_ciphersuites.c
SOURCES += ssl_cli.c
SOURCES += ssl_cookie.c
SOURCES += ssl_srv.c
SOURCES += ssl_ticket.c
SOURCES += ssl_tls.c
SOURCES += threading.c
SOURCES += timing.c
SOURCES += version.c
SOURCES += version_features.c
SOURCES += x509.c
SOURCES += x509write_crt.c
SOURCES += x509write_csr.c
SOURCES += x509_create.c
SOURCES += x509_crl.c
SOURCES += x509_crt.c
SOURCES += x509_csr.c
SOURCES += xtea.c
SOURCES += ca.c
SOURCES += HAL_UDP_threadx.c
SOURCES += HAL_TCP_threadx.c

