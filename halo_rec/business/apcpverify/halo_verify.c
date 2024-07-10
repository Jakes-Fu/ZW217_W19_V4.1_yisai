
#include "system_hal.h"
#if(PLAY_ROLE == PLAY_AP)
#include "ap_key.h"
#define CMD APS_CMD
#define R_CMD CPS_CMD
#define ENCODE_N __ap_mn
#define DECODE_N __cp_mn
#define ENCODE_PRIK  __ap_prk

#include "halo_verify_ap.c"

#elif(PLAY_ROLE == PLAY_CP)
#include "cp_key.h"
#define CMD CPS_CMD
#define R_CMD CPS_CMD
#define ENCODE_N __cp_mn
#define DECODE_N __ap_mn
#define ENCODE_PRIK  __cp_prk

#include "halo_verify_cp.c"
#endif