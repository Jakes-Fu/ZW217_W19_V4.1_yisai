/**************************************************************************
 digitpinyin_api.c

     Description:

     Author:      ke wen (kewen@komoxo.com)
     Date Time:   11/14/2013 3:40:23 PM

     Copyright (C) 2000-2013 Beijing Komoxo Technology Ltd.
     All rights reserved.

 **************************************************************************/

#include "IIMEstd.h"
#include <string.h>

#include "kmxime.h"

typedef struct tagETHIOPICLEADCHAR
{
    int num;
    IIME_WCHAR value[8];
}ETHIOPICLEADCHAR;

/* 
 * Amharic
 */

static const ETHIOPICLEADCHAR Amharic_Symbol[] = 
{
    /* 0 */ {2, {' ', '0'}},
    /* 1 */ {7, {0x1362, 0x1363, 0x1364, 0x1365, '?', '!', '1'}}, 
    /* * */ {6, {0x1362, 0x1363, 0x1364, 0x1365, '?', '!'}}
};

static const ETHIOPICLEADCHAR Amharic_Leader[] = 
{
    {3, {0x1200, 0x1208, 0x1218}},
    {4, {0x1228, 0x1230, 0x1238, 0x1240}},
    {4, {0x1260, 0x1270, 0x1278, 0x1290}},
    {3, {0x1298, 0x12A0, 0x12A8}}, 
    {3, {0x12C8, 0x12D8, 0x12E0}},
    {4, {0x12E8, 0x12F0, 0x1300, 0x1308}},
    {3, {0x1320, 0x1328, 0x1330}},
    {4, {0x1340, 0x1348, 0x1350, 0x1268}},
};

static void FillRowCandidate_Amharic(IIME_WCHAR c1, unsigned short *pCandidate, unsigned short *pLength)
{
    int i;

    memset(pCandidate, 0, sizeof(IIME_WCHAR) * MAX_ETHIOPIC_CANDIDATE);
    for (i = 0; i < 7; i++)
        pCandidate[i] = (IIME_WCHAR)(c1 + i);

    switch (c1)
    {
    case 0x1200:
    //case 0x12A0:
    //case 0x12A8:
    case 0x12C8:
    case 0x12E8:
    //case 0x1300:
    //case 0x1308:
    case 0x1330:
    case 0x1350:
    case 0x1368:
    case 0x1340:
    case 0x1268:
        *pLength = 7;
        break;
    case 0x1240:
        pCandidate[7] = 0x124B;
        *pLength = 8;
        break;
    //case 0x1340:
    //    pThis->m_pCandidate[7] = 0x133F;
    //    pThis->m_nCand = 8;
    //    break;
    case 0x1308:
        pCandidate[7] = 0x1313;
        *pLength = 8;
        break;
    case 0x12A8:
        pCandidate[7] = 0x12B3;
        *pLength = 8;
        break;
    default:
        pCandidate[7] = (IIME_WCHAR)(c1 + 7);
        *pLength = 8;
        break;
    }

}

/* 
 * Tigre
 */

static const ETHIOPICLEADCHAR Tigre_Leader[] = 
{
    {4, {0x1200, 0x1208, 0x1210, 0x1218}},
    {5, {0x1228, 0x1230, 0x1238, 0x1240, 0x1248}},
    {5, {0x1260, 0x1270, 0x1278, 0x1080, 0x1288}},
    {4, {0x1290, 0x12A0, 0x12A8, 0x12B0}}, 
    {4, {0x12C8, 0x12D0, 0x12D8, 0x12E0}},
    {5, {0x12E8, 0x12F0, 0x1300, 0x1308, 0x1310}},
    {3, {0x1320, 0x1328, 0x1330}},
    {3, {0x1338, 0x1348, 0x1350}},
};

static void FillRowCandidate_Tigre(IIME_WCHAR c1, unsigned short *pCandidate, unsigned short *pLength)
{
    int i;

    memset(pCandidate, 0, sizeof(IIME_WCHAR) * MAX_ETHIOPIC_CANDIDATE);

    pCandidate[0] = c1;

    switch (c1)
    {
    case 0x1248:
    case 0x1288:
    case 0x12B0:
    case 0x1310:
        *pLength = 5;
        for (i = 0; i < 4; i++)
            pCandidate[i + 1] = (unsigned short)(c1 + i + 2);
        break;
    default:
        *pLength = 7;
        for (i = 0; i < 6; i++)
            pCandidate[i + 1] = (unsigned short)(c1 + i + 1);
        break;
    }
}

/* 
 * Tigrinya
 */

static const ETHIOPICLEADCHAR Tigrinya_Leader[] = 
{
    {5, {0x1200, 0x1208, 0x1210, 0x1218, 0x1220}},
    {7, {0x1228, 0x1230, 0x1238, 0x1240, 0x1248, 0x1250, 0x1258}},
    {6, {0x1260, 0x1268, 0x1270, 0x1278, 0x1280, 0x1288}},
    {7, {0x1290, 0x1298, 0x12A0, 0x12A8, 0x12B0, 0x12B8, 0x12C0}}, 
    {4, {0x12C8, 0x12D0, 0x12D8, 0x12E0}},
    {5, {0x12E8, 0x12F0, 0x1300, 0x1308, 0x1310}},
    {5, {0x1320, 0x1328, 0x1330, 0x1330, 0x1338}},
    {3, {0x1340, 0x1348, 0x1350}},
};

static void FillRowCandidate_Tigrinya(IIME_WCHAR c1, unsigned short *pCandidate, unsigned short *pLength)
{
    int i;

    memset(pCandidate, 0, sizeof(IIME_WCHAR) * MAX_ETHIOPIC_CANDIDATE);

    pCandidate[0] = c1;

    switch (c1)
    {
    case 0x1248:
    case 0x1258:
    case 0x1288:
    case 0x12B0:
    case 0x12C0:
    case 0x1310:
        *pLength = 5;
        for (i = 0; i < 4; i++)
            pCandidate[i + 1] = (unsigned short)(c1 + i + 2);
        break;
    default:
        *pLength = 7;
        for (i = 0; i < 6; i++)
            pCandidate[i + 1] = (unsigned short)(c1 + i + 1);
        break;
    }
}

int kmx_ethiopic_query_leader(int               lang,     
                              unsigned short    key,
                              unsigned short    *string,
                              unsigned short    *length)
{
    int i;
    const ETHIOPICLEADCHAR *pLeader = 0;

    if (key < 2 || key > 9)
        return 0;

    switch (lang)
    {
    case IIME_LANG_AMHARIC:
        pLeader = Amharic_Leader + (key - 2);
        break;
    case IIME_LANG_TIGRE:
        pLeader = Tigre_Leader + (key - 2);
        break;
    case IIME_LANG_TIGRINYA:
        pLeader = Tigrinya_Leader + (key - 2);
        break;
    default:
        return 0;
    }

    memset(string, 0, sizeof(IIME_WCHAR) * MAX_ETHIOPIC_CANDIDATE);

    *length = (unsigned short)pLeader->num;
    for (i = 0; i < pLeader->num; i++)
    {
        string[i] = pLeader->value[i];
    }

    return 1;
}

int kmx_ethiopic_query_multimap(int             lang,
                                unsigned short  leader,
                                unsigned short  *string,
                                unsigned short  *length)
{
    if (leader < 0x1200 || leader > 0x11478)
        return 0;
    if (leader % 8)     /* leader char must have 0x0 or 0x8 tailer*/
        return 0;

    switch (lang)
    {
    case IIME_LANG_AMHARIC:
        FillRowCandidate_Amharic(leader, string, length);
        break;
    case IIME_LANG_TIGRE:
        FillRowCandidate_Tigre(leader, string, length);
        break;
    case IIME_LANG_TIGRINYA:
        FillRowCandidate_Tigrinya(leader, string, length);
        break;
    default:
        return 0;
    }
    
    return 1;
}

int kmx_ethiopic_query_symbol(int               lang,     
                              unsigned short    key,
                              unsigned short    *string,
                              unsigned short    *length)
{
    int i;
    const ETHIOPICLEADCHAR *pLeader = 0;

    if (key == 0)
        pLeader = Amharic_Symbol;
    else if (key == 1)
        pLeader = Amharic_Symbol + 1;
    else if (key == 10)
        pLeader = Amharic_Symbol + 2;
    else
        return 0;

    memset(string, 0, sizeof(IIME_WCHAR) * MAX_ETHIOPIC_CANDIDATE);
    *length = (unsigned short)pLeader->num;

    for (i = 0; i < pLeader->num; i++)
        string[i] = pLeader->value[i];

    return 1;
}
