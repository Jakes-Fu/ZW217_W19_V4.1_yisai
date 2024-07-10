
MINCPATH      = MS_Ref/source/simlock/inc \
								chip_drv/export/inc \
								MS_Ref/source/simlock/library/include \
								chip_drv/export/inc/outdated \
								BASE/l4/export/inc
								

MSRCPATH      = MS_Ref/source/simlock/src
MSRCPATH     += MS_Ref/source/simlock/library/source

SOURCES       = simlock_serv.c simlock.c simlock_storage.c simlock_aescrypt.c simlock_prod.c sha2.c sha1.c aes.c rsa.c bignum.c 



