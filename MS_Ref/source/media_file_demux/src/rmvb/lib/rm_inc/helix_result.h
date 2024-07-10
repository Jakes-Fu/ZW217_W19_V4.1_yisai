

#ifndef HELIX_RESULT_H
#define HELIX_RESULT_H

#include "helix_types.h"

#ifdef __cplusplus
extern "C" {
#endif  /* #ifdef __cplusplus */

/* Definition of HX_RESULT */
#ifndef HX_RESULT
typedef INT32 HX_RESULT;
#endif /* #ifndef HX_RESULT */

/* FACILITY_ITF defintion */
#ifndef FACILITY_ITF
#define FACILITY_ITF 4
#endif

/* General error definition macro */
#ifndef MAKE_HX_FACILITY_RESULT
#define MAKE_HX_FACILITY_RESULT(sev,fac,code) \
    ((HX_RESULT) (((UINT32)(sev) << 31) | ((UINT32)(fac)<<16) | ((UINT32)(code))))
#endif /* #ifndef MAKE_HX_FACILITY_RESULT */

/* Error definition macros with fac == FACILITY_ITF */
#ifndef MAKE_HX_RESULT
#define MAKE_HX_RESULT(sev,fac,code) MAKE_HX_FACILITY_RESULT(sev, FACILITY_ITF, ((fac << 6) | (code)))
#endif /* #ifndef MAKE_HX_RESULT */

#define SS_GLO      0  /* General errors                             */
#define SS_NET      1  /* Networking errors                          */
#define SS_FIL      2  /* File errors                                */
#define SS_DEC      8  /* Decoder errors                             */
#define SS_DPR     63  /* Deprecated errors                          */

#define HXR_NOTIMPL                     MAKE_HX_FACILITY_RESULT(1,0,0x4001)   /* 80004001 */
#define HXR_OUTOFMEMORY                 MAKE_HX_FACILITY_RESULT(1,7,0x000e)   /* 8007000e */
#define HXR_INVALID_PARAMETER           MAKE_HX_FACILITY_RESULT(1,7,0x0057)   /* 80070057 */
#define HXR_NOINTERFACE                 MAKE_HX_FACILITY_RESULT(1,0,0x4002)   /* 80004002 */
#define HXR_POINTER                     MAKE_HX_FACILITY_RESULT(1,0,0x4003)   /* 80004003 */
#define HXR_FAIL                        MAKE_HX_FACILITY_RESULT(1,0,0x4005)   /* 80004005 */
#define HXR_ACCESSDENIED                MAKE_HX_FACILITY_RESULT(1,7,0x0005)   /* 80070005 */
#define HXR_OK                          MAKE_HX_FACILITY_RESULT(0,0,0)        /* 00000000 */

#define HXR_INVALID_VERSION             MAKE_HX_RESULT(1,SS_GLO,5)            /* 80040005 */
#define HXR_UNEXPECTED                  MAKE_HX_RESULT(1,SS_GLO,9)            /* 80040009 */
#define HXR_UNSUPPORTED_AUDIO           MAKE_HX_RESULT(1,SS_GLO,15)           /* 8004000f */
#define HXR_NOT_SUPPORTED               MAKE_HX_RESULT(1,SS_GLO,33)           /* 80040021 */

#define HXR_NO_DATA                     MAKE_HX_RESULT(0,SS_NET,2)            /* 00040042 */

#define HXR_AT_END                      MAKE_HX_RESULT(0,SS_FIL,0)            /* 00040080 */
#define HXR_INVALID_FILE                MAKE_HX_RESULT(1,SS_FIL,1)            /* 80040081 */
#define HXR_CORRUPT_FILE                MAKE_HX_RESULT(1,SS_FIL,17)           /* 80040091 */
#define HXR_READ_ERROR                  MAKE_HX_RESULT(1,SS_FIL,18)           /* 80040092 */

#define HXR_BAD_FORMAT                  MAKE_HX_RESULT(1,SS_DPR,1)            /* 80040fc1 */

#define HXR_DEC_NOT_FOUND               MAKE_HX_RESULT(1,SS_DEC,1)            /* 80040201 */

/* Define success and failure macros */
#define HX_SUCCEEDED(status) (((UINT32) (status) >> 31) == 0)
#define HX_FAILED(status)    (((UINT32) (status) >> 31) != 0)

#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus */

#endif /* #ifndef HELIX_RESULT_H */
