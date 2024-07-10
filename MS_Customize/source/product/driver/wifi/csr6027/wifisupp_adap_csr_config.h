#ifndef __WIFISUPP_ADAP_CSR_CONFIG_H__
#define __WIFISUPP_ADAP_CSR_CONFIG_H__

#include "csr_types.h"



/* mib */
//extern CsrBool CsrWifiGetCustomMibData (int chipVersion, CsrUint8 **pp_mib, int *p_len);

/* board profile */
extern CsrBool CsrWifiGetCustomBoardProfileData (int chipVersion, CsrUint8 **pp_boardprf, int *p_len);

/* firmware patch */
extern CsrInt32 CsrWifiFirmwarePatchAcquire(CsrUint32 fwBuild, CsrUint32 *length, CsrUint8 **firmware);

/* ptest firmware */
CsrInt32 CsrWifiFirmwarePtestAcquire(CsrUint32 chipVersion, CsrUint32 *length, CsrUint8 **firmware);

#endif


