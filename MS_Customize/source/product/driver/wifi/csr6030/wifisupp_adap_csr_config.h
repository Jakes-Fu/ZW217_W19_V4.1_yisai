#ifndef __WIFISUPP_ADAP_CSR_CONFIG_H__
#define __WIFISUPP_ADAP_CSR_CONFIG_H__

#include "csr_types.h"

/*
 * =====================================================================
 *              A10
 * =====================================================================
 *  ChipVer     0x3a22
 *  RomBuildId  1089
 * ===================================================================== 
 */
typedef struct {
    CsrUint8 *data;
    int len;
} CsrOctectData;

typedef struct {
    /* chip info */
    CsrUint16 chipVer;
    int romBuildId;

    CsrOctectData mib;
    CsrOctectData boardProfile;
    CsrOctectData firmwarePatch;
    CsrOctectData ptestFirmware;
} CsrConfigData;

/* mib */
CsrBool CsrWifiGetCustomMibData (int chipVersion, CsrUint8 **pp_mib, int *p_len);

/* board profile */
CsrBool CsrWifiGetCustomBoardProfileData (int chipVersion, CsrUint8 **pp_boardprf, int *p_len);

/* firmware patch */
CsrInt32 CsrWifiFirmwarePatchAcquire(CsrUint32 fwBuild, CsrUint32 *length, CsrUint8 **firmware);

/* ptest firmware */
CsrInt32 CsrWifiFirmwarePtestAcquire(CsrUint32 chipVersion, CsrUint32 *length, CsrUint8 **firmware);

#endif
