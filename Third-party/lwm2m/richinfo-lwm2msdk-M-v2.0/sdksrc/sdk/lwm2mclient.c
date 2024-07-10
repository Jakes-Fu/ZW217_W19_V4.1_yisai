
#include "lwm2mclient.h"
#include "liblwm2m.h"
#include "commandline.h"
#ifdef WITH_TINYDTLS
#include "dtlsconnection.h"
#else
#include "connection.h"
#endif

#include <string.h>
#include <stdlib.h>
#if 0 //modify by unisoc
#include <unistd.h>
#endif
#include <stdio.h>
#include <ctype.h>
#if 0 //modify by unisoc
#include <sys/select.h>
#include <sys/types.h>
#endif
#include <sys/socket.h>
#if 0 //modify by unisoc
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#endif
#include <errno.h>
#include <signal.h>

#include "j_aes.h"
#include "j_base64.h"

