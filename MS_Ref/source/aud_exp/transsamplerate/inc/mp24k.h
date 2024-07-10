#ifndef _MIDI_DECODE_H
#define _MIDI_DECODE_H

//#include <string.h>
//
#ifdef __cplusplus
    extern   "C"
    {
#endif
#define  DEFAULT_RATE   24000 // must 24000 for 6600D
//
//
#define  EQmode          1    // no eq, 1 bass,2 treble, 3 vocal, 4 power, 5 rock...
#define  Karamode        1    // 
#define  Compress
//-----------------------------------------------------------
//
typedef   unsigned char  BYTE_;     /*8 bits*/
typedef   unsigned short WORD_;     /*16 bits*/
typedef   unsigned long  DWORD_;    /*32 bits*/

typedef   signed char    CHAR_;     /*08 bits*/
typedef   signed short   INT_ ;     /*16 bits*/
typedef   signed long    LONG_;     /*32 bits*/

#define PPP

typedef struct _tag_TRANSAM_CONTEXT
{
    DWORD_ playrate;
    INT_ PlayrateStep;
    INT_ PlayScale;
    INT_ Intpoint;
    INT_ Floatpoint ;
    INT_ dataratestep;
    INT_ blocklength;
#ifdef PPP    
    INT_  lword_data;
    INT_  rword_data;
    char  data_rsved_flag;    
#endif
    INT_ mp324k_join_data_l;
    INT_ mp324k_join_data_r; 
} TRANSAM_CONTEXT_T;    
    
void MP324K_InitParam(TRANSAM_CONTEXT_T *ptTransamObj);
unsigned long MP3_Play24k(TRANSAM_CONTEXT_T *ptTransamObj, INT_ * Linbuf, 
                    INT_ * Rinbuf, INT_ inum, DWORD_ irate, INT_ *Loubuf, INT_ *Roubuf);
void MP3_Set_VoiceRate(TRANSAM_CONTEXT_T *ptTransamObj, DWORD_ splay);
#if 0
//
//
unsigned long MP3_Play24k(INT_ * inbuf, INT_ inum,DWORD_ irate, INT_ *oubuf);
//
void MP3_Set_VoiceRate(DWORD_ play);
void MP3_Set_EQmode(WORD_ em);
void MP3_Set_karaokmode(WORD_ km);
void MP3_Set_compress(INT_ targetcdb);
void MP3_Set_Gvolume(INT_ StartcdB,INT_ EndcdB,WORD_ tms);
void MP324K_InitParam(void);

//
void writewave(INT_ *bf, DWORD_ cunt);
#endif

#ifdef __cplusplus
    }
#endif

#endif
