MINCPATH      = $(PARSER_PATH)
ifeq ($(strip $(MBEDTLS_SUPPORT)), TRUE)
ifeq ($(strip $(IPVERSION_SUPPORT)), V4_V6)
MINCPATH   += DAPS/export/inc/tcpip6
MCFLAG_OPT += -DIPV6_SUPPORT
else
MINCPATH   += DAPS/export/inc/tcpip
endif

MINCPATH += external/libc/h \
			external/libc
			
ifeq ($(strip $(MBEDTLS_VERSION)), V224)
MINCPATH += Third-party/mbedtls/V224/include/
MINCPATH += Third-party/mbedtls/V224/include/mbedtls
MSRCPATH += Third-party/mbedtls/V224/src

SOURCES +=  aes.c          ctr_drbg.c        md2.c                  platform.c                    ssl_cookie.c 			\
			aesni.c        debug.c           md4.c                  platform_util.c               ssl_msg.c             \
			arc4.c         des.c             md5.c                  poly1305.c                    ssl_srv.c             \
			aria.c         dhm.c             md.c                   psa_crypto.c                  ssl_ticket.c          \
			asn1parse.c    ecdh.c            memory_buffer_alloc.c  psa_crypto_se.c               ssl_tls.c             \
			asn1write.c    ecdsa.c           net_sockets.c          psa_crypto_slot_management.c  threading.c           \
			base64.c       ecjpake.c         nist_kw.c              psa_crypto_storage.c          timing.c              \
			bignum.c       ecp.c             oid.c                  psa_its_file.c                version.c             \
			blowfish.c     ecp_curves.c      padlock.c              ripemd160.c                   version_features.c    \
			camellia.c     entropy.c         pem.c                  rsa.c                         x509.c                \
			ccm.c          entropy_poll.c    pk.c                   rsa_internal.c                x509_create.c         \
			certs.c        error.c           pkcs11.c               sha1.c                        x509_crl.c            \
			chacha20.c     gcm.c             pkcs12.c               sha256.c                      x509_crt.c            \
			chachapoly.c   havege.c          pkcs5.c                sha512.c                      x509_csr.c            \
			cipher.c       hkdf.c            pkparse.c              ssl_cache.c                   x509write_crt.c       \
			cipher_wrap.c  hmac_drbg.c       pk_wrap.c              ssl_ciphersuites.c            x509write_csr.c       \
			cmac.c         mbedtls_import.c  pkwrite.c              ssl_cli.c                     xtea.c                
else
#ifeq ($(strip $(MBEDTLS_VERSION)), V206)
MINCPATH += Third-party/mbedtls/V206/include/mbedtls
MINCPATH += Third-party/mbedtls/V206/include
MSRCPATH += Third-party/mbedtls/V206/src



SOURCES +=  aes.c        ccm.c          ecdh.c          havege.c               oid.c      pkwrite.c           ssl_cli.c           x509.c			\
		aesni.c      certs.c        ecdsa.c         hmac_drbg.c            padlock.c  platform.c          ssl_cookie.c        x509_create.c		\
		arc4.c       cipher.c       ecjpake.c       md2.c                  pem.c      ripemd160.c         ssl_srv.c           x509_crl.c         \
		asn1parse.c  cipher_wrap.c  ecp.c           md4.c                  pk.c       rsa.c               ssl_ticket.c        x509_crt.c         \
		asn1write.c  cmac.c         ecp_curves.c    md5.c                  pkcs11.c   sha1.c              ssl_tls.c           x509_csr.c         \
		base64.c     ctr_drbg.c     entropy.c       md.c                   pkcs12.c   sha256.c            threading.c         x509write_crt.c     \
		bignum.c     debug.c        entropy_poll.c  md_wrap.c              pkcs5.c    sha512.c            timing.c            x509write_csr.c     \
		blowfish.c   des.c          error.c         memory_buffer_alloc.c  pkparse.c  ssl_cache.c         version.c           xtea.c              \
		camellia.c   dhm.c          gcm.c           net_sockets.c          pk_wrap.c  ssl_ciphersuites.c  version_features.c

endif
endif