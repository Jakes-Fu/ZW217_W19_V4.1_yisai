#ifndef PRC_MOCOR_PLTFM_H
#define PRC_MOCOR_PLTFM_H
#include "rpc_verify.h"
#define RPC_VERIFYAP(PKT) verify_ApSide(PKT)
#define RPC_VERIFYCP(PKT) verify_CpSide(PKT)
#define RPC_VERIFYCPRESULT(RST) verify_Cp_result(RST)
#endif