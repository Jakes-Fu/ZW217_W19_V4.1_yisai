

#ifndef _CHALLENGE_H_
#define _CHALLENGE_H_

#include "helix_types.h"
#include "helix_utils.h"
#include "rm_memory.h"



/*
 * The original Challange structure and methods. Used in the
 * Old PNA and as the first round in the new RTSP Challenge.
 */
struct Challenge
{
    BYTE text[33];
    BYTE response[33];
};

struct Challenge* CreateChallenge(INT32 k1,
                                  INT32 k2,
                                  BYTE* k3,
                                  BYTE* k4);

struct Challenge* CreateChallengeFromPool(INT32 k1,
                                          INT32 k2,
                                          BYTE* k3,
                                          BYTE* k4,
                                          rm_malloc_func_ptr fpMalloc,
                                          void* pMemoryPool);
    
BYTE* ChallengeResponse1(BYTE* k1, BYTE* k2,
                         INT32 k3, INT32 k4,
                         struct Challenge* ch);
BYTE* ChallengeResponse2(BYTE* k1, BYTE* k2,
                         INT32 k3, INT32 k4,
                         struct Challenge* ch);
    


/*
 * The new RTSP Challenge structure and methods
 */
struct RealChallenge
{
    BYTE challenge[33];
    BYTE response[41];
    BYTE trap[9];
};


struct RealChallenge* CreateRealChallenge();
struct RealChallenge* CreateRealChallengeFromPool(rm_malloc_func_ptr fpMalloc,
                                                  void* pMemoryPool);
BYTE* RealChallengeResponse1(BYTE* k1, BYTE* k2,
                             INT32 k3, INT32 k4,
                             struct RealChallenge* rch);
BYTE* RealChallengeResponse2(BYTE* k1, BYTE* k2,
                             INT32 k3, INT32 k4,
                             struct RealChallenge* rch);


void CalcCompanyIDKey(const char* companyID,
                      const char* starttime,
                      const char* guid,
                      const char* challenge,
                      const char* copyright,
                      UCHAR*      outputKey);

INT32 BinTo64(const BYTE* pInBuf, INT32 len, char* pOutBuf);


/* Support constants and values */
extern const INT32 G2_BETA_EXPIRATION;

extern const INT32 RC_MAGIC1;
extern const INT32 RC_MAGIC2;
extern const INT32 RC_MAGIC3;
extern const INT32 RC_MAGIC4;

extern const unsigned char pRCMagic1[];
extern const unsigned char pRCMagic2[];


// This UUID was added in the 5.0 player to allow us to identify
// players from Progressive Networks.  Other companies licensing this
// code must be assigned a different UUID.
#define HX_COMPANY_ID "92c4d14a-fa51-4bcb-8a67-7ac286f0ff7e"
#define HX_COMPANY_ID_KEY_SIZE  16


extern const unsigned char HX_MAGIC_TXT_1[];
extern const char pMagic2[];



#endif /*_CHALLENGE_H_*/
