/******************************************************************************
 ** File Name:    ept_drvapi.h                                                   *
 ** Author:       andy.chen                                                   *
 ** DATE:         2010-04-20                                                  *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 2010-04-20    andy.chen       Create.                                     *
 ******************************************************************************/
#ifndef _EPT_DRVAPI_H_
#define _EPT_DRVAPI_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
#define EPT_MODE_GEA1          1
#define EPT_MODE_GEA2          2
#define EPT_MODE_GEA3          3
#define EPT_MODE_KSMI          4
#define EPT_MODE_F8            8
#define EPT_MODE_F9            9

/**----------------------------------------------------------------------------*
**                               Type Define                                  **
**----------------------------------------------------------------------------*/
typedef struct EPT_F8_GRP_TAG
{
    uint8  key[16];     // 16bytes value, no endian issue
    uint32 count;
    uint32 bearer;
    uint32 dir;         // direction: 0,1
    uint32 len;         // in unit of byte
    uint8  *data_in;
    uint8  *data_out;
    struct EPT_F8_GRP_TAG *next_grp;
}
EPT_F8_GRP_T, *EPT_F8_GRP_T_P;

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
//***************************************************************************************
//    Description: GEA cipher function for GEA1/2
//    Global resource dependence :
//    Author:
//    Note:
//      the function is used by ps/gprs/llc to generate/decode ciphered llc frame.
//
//      "key"  : gea1,gea2 have 64bit key
//
//      "input": gea1,gea2 have 32bit input
//
//      example(gea1):
//              Kc    0xac245504a30f9b3e
//              INPUT 0xb208858a
//              DIRECTION 0
//              OUTPUT(16bytes):
//              0xe0,0xf8,0x03,0x7a,0x8d,0x40,0x85,0x26,0x5f,0x1c,0x27,0x50,0x6e,0x28,0xdb,0x63
//
//              gea_get_stream(kc, in_0, 0, 1, 8, stream_out);
//***************************************************************************************
PUBLIC uint gea_get_stream (const uint8 key[8],  // bit00~63 of parameter kc
                            uint32 in_0,    // bit00~31 of parameter input
                            uint32 bDir,    // direction: 0,1
                            uint32 mode,    // 1:gea1, 2:gea2
                            uint32 len,     // in unit of 2bytes
                            uint8 *stream); // output stream pointer

//***************************************************************************************
//    Description: GEA3 cipher function
//    Global resource dependence :
//    Author:
//    Note:
//      the function is used by ps/gprs/llc to generate/decode ciphered llc frame.
//
//      "key"  : gea3 have 64bit key
//
//      "input": gea3 have 32bit input
//
//      example(gea3):
//              Kc    0xEFA8B2229E720C2A
//              INPUT 0x4BDBD5E5
//              DIRECTION 1
//              OUTPUT(16bytes):
//              0x4718A2ADFC90590949DDADAB406EC3B9
//
//              gea3_get_stream(kc, 0x4BDBD5E5, 1, 16, stream_out);
//***************************************************************************************
PUBLIC uint gea3_get_stream (const uint8 key[8], // bit00~63 of parameter kc
                             uint32 in_0,    // bit00~31 of parameter input
                             uint32 bDir,    // direction: 0,1
                             uint32 len,     // in unit of byte
                             uint8 *stream); // output stream pointer

//***************************************************************************************
//    Description: kasumi cipher function
//    Global resource dependence :
//    Author:
//    Note:
//      the function is used by ps/gprs/llc to generate/decode ciphered llc frame.
//
//      "key"  : kasumi have 128bit key
//
//      "input": kasumi have 64bit input
//
//***************************************************************************************
PUBLIC uint kasumi_get_stream (const uint8 key[16],     // bit00~128 of parameter kc
                               const uint8 in[8],        // bit00~64 of parameter input
                               uint32 bDir,        // direction: 0,1
                               uint8 stream[8]);   // 8byte value, no endian issue

//***************************************************************************************
//    Description: F8 cipher function
//    Global resource dependence :
//    Author:
//    Note:
//      the function is used by ps/gprs/llc to generate/decode ciphered llc frame.
//
//      "key"  : F8 have 128bit key
//
//      "input": F8 have 64bit input
//
//***************************************************************************************
PUBLIC void F8_get_stream (EPT_F8_GRP_T *grp_head);

//***************************************************************************************
//    Description: F9 cipher function
//    Global resource dependence :
//    Author:
//    Note:
//      the function is used by ps/gprs/llc to generate/decode ciphered llc frame.
//
//      "key"  : F9 have 128bit key
//
//
//
//***************************************************************************************
PUBLIC uint F9_get_stream (const uint8 key[16],  // bit00~128 of parameter kc
                           uint32 count,
                           uint32 fresh,
                           uint32 bDir,    // direction: 0,1
                           uint32 len,     // in unit of byte
                           uint8 *stream,  // in stream pointer
                           uint8  mac[4]); // 4byte value, no endian issue


#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_EPT_DRVAPI_H_

