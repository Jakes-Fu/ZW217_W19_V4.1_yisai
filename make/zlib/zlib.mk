MCFLAG_OPT    += -DZ_PREFIX
MINCPATH      = Third-party/zlib/source/inc
MINCPATH      += Third-party/zlib/include
MSRCPATH      = Third-party/zlib/source/c
SOURCES       = adler32.c \
                compress.c \
                crc32.c \
                deflate.c \
                infback.c \
                inffast.c \
                inflate.c \
                inftrees.c \
                trees.c \
                uncompr.c \
                zutil.c  
