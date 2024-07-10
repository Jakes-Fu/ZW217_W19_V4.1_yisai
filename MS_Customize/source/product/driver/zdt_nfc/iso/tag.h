#ifndef _TAGX_H
#define _TAGX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MyInclude.h"

typedef struct _TAG4_CC {
    uint16_t CCLEN;              //000f CCLEN (15 bytes)
    uint8_t  MappingVersion;     //20   Mapping Version 2.0
    uint16_t MLe;                //003B MLe (49 bytes); Maximum R-APDU data size
    uint16_t MLc;                //0034 MLc (52 bytes); Maximum C-APDU data size
    uint8_t  T;                  //04   T field of the NDEF File Control TLV
    uint8_t  L;                  //06h  L field of the NDEF File Control TLV
    uint8_t  V[64];              //     V field of the NDEF File Control TLV:
                                 //E104 File Identifier
                                 //0032 Maximum NDEF size (50 bytes)
                                 //00   NDEF file read access condition; read access without any security
                                 //     NDEF file write access condition; write access without any security
} TAG4_CC;

typedef struct _TAG4_NDEF {
    uint16_t NLEN;               //000E ³¤¶È

    uint8_t MB;
    uint8_t ME;
    uint8_t CF;
    uint8_t SR;
    uint8_t IL;
    uint8_t TNF;

    uint8_t TypeLength;
    uint8_t PayLoadLength[4];
    uint8_t IDLength;
    uint8_t Type[10];
    uint8_t ID[10];
    uint8_t PayLoad[64];
} TAG4_NDEF;

void tag1test(void);
void tag2test(void);
void tag3test(void);
void tag4test(void);
void tag5test(void);

#ifdef __cplusplus
}
#endif
#endif
