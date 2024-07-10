
## CFLAG OPT
MCFLAG_OPT   	= --c99 -DOS_USING_ONEOS_SOCKET

## user config && TCP_TEST
MINCPATH += Third-party/oneos/cms/inc \
            Third-party/oneos/cms/src \
            Third-party/oneos/cms/src/test \
            Third-party/oneos \
			external/oneos/inc/kernel

MSRCPATH += Third-party/oneos/cms/src
MSRCPATH += Third-party/oneos/cms/src/test
SOURCES += cms_config.c cms_tcp_test.c cms_mqtt_test.c