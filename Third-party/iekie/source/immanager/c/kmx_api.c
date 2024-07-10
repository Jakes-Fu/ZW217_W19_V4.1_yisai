
/**************************************************************************
 digitpinyin_api.c

     Description:

     Author:      ke wen (kewen@komoxo.com)
     Date Time:   11/23/2007 3:40:23 PM

     Copyright (C) 2000-2005 Beijing Komoxo Technology Ltd.
     All rights reserved.

 **************************************************************************/

#include "../utli/IIMEstd.h"
#include <string.h>

#include "../inc/kmxime.h"
/*#include "inc/IIME_activestate.h"*/
#if 0//ZHMCH	无此文件
#include "core/digitpinyin/digitpinyinconst.h"
#include "core/digitpinyin/IIMEdpycore.h"
#include "core/digitpinyin/hanzicodemap.h"
#endif

#include "../inc/IIMEk2api.h"
/*****************************************************************************
 *
 * pinyin api
 *
 *****************************************************************************/

/* static functions declare */
static unsigned short CalcMatchLength(const char* digit_string, unsigned short digit_length, int input_length);

#if defined(NOLLEC_LENOVO_SPLIT) || defined(NOLLEC_LENOVO_SPLIT_1) || defined(NOLLEC_LENOVO_SPLIT_2)
static void pinyin_TranslateSplit(char *pBuffer);
#endif

typedef struct tagPinyin_History
{
    int                 is_digit;
    IIME_HCORE            hCore;
    unsigned int        flags;
    IIMEDPYCORE_PARAMS    user_cache;
    const KMX_PINYIN_DB *pdb;

    char            digit_string[64];
    unsigned short  digit_string_length;
    unsigned short  pinyin_match_index;

    char            pinyin_string[64];
    CALLBACK_READ_UPDATE_DATA pfnReadUpdateData;
}Pinyin_History;

#if defined(KMX_SP)
static int s_output_number = 0;
#endif

static int switch_pinyin_method(Pinyin_History *pinyin_history, int is_digit)
{
    if (is_digit)
    {
        int iMethod;
        pinyin_history->is_digit = 1;
        iMethod = IM_PINYIN;

        pinyin_history->hCore = IIMEDPYCORE_Initialize(&(pinyin_history->user_cache),
                                                     iMethod, 
                                                     pinyin_history->pdb, 
                                                     pinyin_history->hCore, 
                                                     pinyin_history->pfnReadUpdateData);

        if (pinyin_history->hCore){
            IIME_IMM_OPTIONS option = {0};
            option.dwPYFuzzy = pinyin_history->flags;
            IIMEDPYCORE_SetOptions(pinyin_history->hCore, &option);
            return 1;
        }
        else
            return 0;
    }
    else{
        pinyin_history->is_digit = 0;

        pinyin_history->hCore = IIMEDPYCORE_Initialize(&(pinyin_history->user_cache), 
                                                     IM_PINYIN_FULLKEY, 
                                                     pinyin_history->pdb, 
                                                     pinyin_history->hCore, 
                                                     pinyin_history->pfnReadUpdateData);

        if (pinyin_history->hCore){
            IIME_IMM_OPTIONS option = {0};
            option.dwPYFuzzy = pinyin_history->flags;
            IIMEDPYCORE_SetOptions(pinyin_history->hCore, &option);
            return 1;
        }
        else
            return 0;
    }
}

#if defined(NOLLEC_LENOVO_SPLIT_1)
static void pinyin_TranslateSplit(char* pBuffer)
{
    int bFirst = 1;
    char *pScan;
    char pTemp[KMX_MAX_PINYIN_COMPOSITION];

    memcpy(pTemp, pBuffer, KMX_MAX_PINYIN_COMPOSITION);
    memset(pBuffer, 0, KMX_MAX_PINYIN_COMPOSITION);

    pScan = pTemp;

    while (*pScan)
    {
        if (*pScan >= 'A' && *pScan <= 'Z')
        {
            if (bFirst)
            {
                *pBuffer++ = *pScan++ - 'A' + 'a';
                bFirst = 0;
            }
            else
            {
                *pBuffer++ = '\'';
                *pBuffer++ = *pScan++ - 'A' + 'a';
            }
        }
        else if (*pScan == ' ')
        {
            *pBuffer++ = *pScan++;
            *pBuffer++ = '\'';
            bFirst = 1;
        }
        else
        {
            *pBuffer++ = *pScan++;
        }
    }

    if (*(pBuffer-1) == '\'')
        *(pBuffer-1) = 0;

}
#elif defined(NOLLEC_LENOVO_SPLIT_2)
static void pinyin_TranslateSplit(char* pBuffer)
{
    int bFirst = 1;
    char *pScan;
    char pTemp[KMX_MAX_PINYIN_COMPOSITION];

    memcpy(pTemp, pBuffer, KMX_MAX_PINYIN_COMPOSITION);
    memset(pBuffer, 0, KMX_MAX_PINYIN_COMPOSITION);

    pScan = pTemp;

    while (*pScan)
    {
        if (*pScan >= 'A' && *pScan <= 'Z')
        {
            if (bFirst)
            {
                *pBuffer++ = *pScan++ - 'A' + 'a';
                bFirst = 0;
            }
            else
            {
                *pBuffer++ = '\'';
                *pBuffer++ = *pScan++ - 'A' + 'a';
            }
        }
        else if (*pScan == ' ')
        {
            *pBuffer++ = *pScan++;
            bFirst = 1;
        }
        else
        {
            *pBuffer++ = *pScan++;
        }
    }
}
#elif defined(NOLLEC_LENOVO_SPLIT)
static void pinyin_TranslateSplit(char* pBuffer)
{
    int bFirst = 1;
    char *pScan;
    char pTemp[KMX_MAX_PINYIN_COMPOSITION];

    memcpy(pTemp, pBuffer, KMX_MAX_PINYIN_COMPOSITION);
    memset(pBuffer, 0, KMX_MAX_PINYIN_COMPOSITION);

    pScan = pTemp;

    while (*pScan)
    {
        if (*pScan >= 'A' && *pScan <= 'Z')
        {
            if (bFirst)
            {
                *pBuffer++ = (char)(*pScan++ - 'A' + 'a');
                bFirst = 0;
            }
            else
            {
                *pBuffer++ = '\'';
                *pBuffer++ = (char)(*pScan++ - 'A' + 'a');
            }
        }
        else if (*pScan == ' ')
        {
            *pBuffer++ = '\'';
            *pBuffer++ = *pScan++;
            bFirst = 1;
        }
        else
        {
            *pBuffer++ = *pScan++;
        }
    }
}
#endif

int kmx_pinyin_initialize(
    KMX_IME_ENGINE*                 engine,
    const KMX_IME_ENGINE_INIT*      init_info)
{
	Pinyin_History *pinyin_history = IIME_NULL;

#ifdef SELFALLOC_PRODUCT
    IIME_MRESET(IIME_NULL);
#endif

	pinyin_history = IIME_MALLOC(22 * 1024); /*sizeof(Pinyin_History));*/
    memset(pinyin_history, 0, sizeof(Pinyin_History));

    pinyin_history->is_digit = 1;
    pinyin_history->user_cache.pCache = init_info->user_data;
    pinyin_history->user_cache.szCache = init_info->user_data_size;
    pinyin_history->flags = init_info->flags;
    pinyin_history->pdb = (const KMX_PINYIN_DB *)(init_info->database);

    pinyin_history->hCore = (char*)pinyin_history + sizeof(Pinyin_History);

    if (switch_pinyin_method(pinyin_history, 1)){
        engine->engine = (int*)pinyin_history;
        return 1;
    }
    else
        return 0;
}

int kmx_pinyin_set_update_callback(
    KMX_IME_ENGINE*                 engine,
    CALLBACK_READ_UPDATE_DATA       pfnReadUpdateData)
{
    Pinyin_History *pinyin_history = (Pinyin_History*)(engine->engine);

    pinyin_history->pfnReadUpdateData = pfnReadUpdateData;

    if (switch_pinyin_method(pinyin_history, 1)){
        return 1;
    }

    return 0;
}

static unsigned short CalcMatchLength(const char* digit_string, unsigned short digit_length, int input_length)
{
    int i = 0;
    while(input_length > 0 && i < digit_length)
    {
        if(digit_string[i] != ' ')
            input_length--;

        i++;
    }

    if (i < digit_length && digit_string[i] == ' ')
        i++;

    return (unsigned short)i;
}

/* clear page information */
static void ClearHistory(Pinyin_History *pinyin_history)
{
    memset(pinyin_history->digit_string, 0, 64);
    pinyin_history->digit_string_length = 0;
    pinyin_history->pinyin_match_index = 0;
    memset(pinyin_history->pinyin_string, 0, 64);
}

int kmx_digit_pinyin_match(KMX_IME_ENGINE*                 engine,
                           const char*                     digit_string,
                           unsigned short                  digit_string_length,
                           unsigned short                  pinyin_match_index,
                           unsigned short                  word_match_index,
                           KMX_PINYIN_COMPOSITION*         pinyin_composition,
                           KMX_PINYIN_MATCH_RESULT*        pinyin_match_result,
                           KMX_PINYIN_WORD_MATCH_RESULT*   word_match_result)
{
    return kmx_digit_pinyin_match_length(engine, digit_string, 
                                         digit_string_length,
                                         pinyin_match_index,
                                         word_match_index,
                                         MAX_LEX_LENGTH,
                                         pinyin_composition,
                                         pinyin_match_result,
                                         word_match_result);
}

int kmx_digit_pinyin_match_length(
    KMX_IME_ENGINE*                 engine,
    const char*                     digit_string,
    unsigned short                  digit_string_length,
    unsigned short                  pinyin_match_index,
    unsigned short                  word_match_index,
    unsigned short                  max_match_word_length,
    KMX_PINYIN_COMPOSITION*         pinyin_composition,
    KMX_PINYIN_MATCH_RESULT*        pinyin_match_result,
    KMX_PINYIN_WORD_MATCH_RESULT*   word_match_result)
{
    int i, count;
    IIME_BOOL bResetEngine = IIME_FALSE;
	Pinyin_History *pinyin_history;

    Pinyin_Choice pinyin_choice;
	Pinyin_String pAssertPinyin[MAX_LEX_LENGTH + 1];
    IIME_WCHAR *p;
    char *pComposition;

    int history_digit_num;

    memset(&pinyin_choice, 0, sizeof(Pinyin_Choice));
    memset(pAssertPinyin, 0, sizeof(Pinyin_String) * (MAX_LEX_LENGTH + 1));

    pinyin_history = (Pinyin_History*)(engine->engine);
    if (!pinyin_history->is_digit)
    {
        if (switch_pinyin_method(pinyin_history, 1))
            ClearHistory(pinyin_history);
        else
            return 0;
    }

    for (i = 0; i < digit_string_length - 1; i++) /* duplicate space is not allow */
    {
        if (digit_string[i] == ' ' && digit_string[i + 1] == ' '){
            ClearHistory(pinyin_history);
            return 0;
        }
    }
        
    history_digit_num = pinyin_history->digit_string_length;

    /* add code_string */
    if (history_digit_num == digit_string_length && 
		strncmp(pinyin_history->digit_string, digit_string, history_digit_num) == 0 ) /* same input digit*/
    {
        bResetEngine = IIME_FALSE;
    }
    else if ((pinyin_history->pdb->nFormat & KMX_DB_FLAG_COMPACT) == 0 &&               
             history_digit_num > 0 && 
             history_digit_num < digit_string_length && 
		     strncmp(pinyin_history->digit_string, digit_string, history_digit_num) == 0)    /* append input*/
    {
        bResetEngine = IIME_TRUE;
        ClearHistory(pinyin_history);
        for(i = 0; i < digit_string_length - history_digit_num; i++)
        {
          if (IIMEDPYCORE_AppendCode(pinyin_history->hCore, digit_string[i + history_digit_num], max_match_word_length) == 0)
		  {
              ClearHistory(pinyin_history);
              return 0;
		  }
        }
    }
    else                                                                                    /* new input*/
    {
        IIME_WCHAR wszCodeString[MAX_PREEDIT_LENGTH];
        memset(wszCodeString, 0, sizeof(IIME_WCHAR) * MAX_PREEDIT_LENGTH);

        bResetEngine = IIME_TRUE;
        ClearHistory(pinyin_history);
        for (i = 0; i < digit_string_length; i++)
            wszCodeString[i] = digit_string[i];

        if (IIMEDPYCORE_SetCodeString(pinyin_history->hCore, wszCodeString, max_match_word_length) == 0)
        {
            ClearHistory(pinyin_history);
			/*pinyin_history->digit_string_length = 0;*/
            return 0;
		}
    }

    /* set restrict */
    if (IIME_ERROR_SUCCESS != IIMEDPYCORE_GetPinyinString(pinyin_history->hCore, &pinyin_choice, pAssertPinyin))
	{
        ClearHistory(pinyin_history);
		/*pinyin_history->digit_string_length = 0;*/

        return 0;
	}

    if ( (bResetEngine && pinyin_match_index != KMX_PINYIN_MATCH_INDEX_NONE) ||
         (!bResetEngine && pinyin_match_index != pinyin_history->pinyin_match_index) )
    {
		if (pinyin_match_index == KMX_PINYIN_MATCH_INDEX_NONE)
		{
            if (IIMEDPYCORE_SetRestrict(pinyin_history->hCore, NULL, MAX_LEX_LENGTH) == 0)
            {
                ClearHistory(pinyin_history);
                return 0;
            }
            else
            {
                IIMEDPYCORE_GetPinyinString(pinyin_history->hCore, &pinyin_choice, pAssertPinyin);
            }
		}
        else if (pinyin_match_index < pinyin_choice.m_nChoice)
        {
            if (IIMEDPYCORE_SetRestrict(pinyin_history->hCore, pinyin_choice.m_pChoice[pinyin_match_index], MAX_LEX_LENGTH) == 0)
            {
                ClearHistory(pinyin_history);
                return 0;
            }
            else
            {
                IIMEDPYCORE_GetPinyinString(pinyin_history->hCore, &pinyin_choice, pAssertPinyin);
            }
        }
        else
		{
            ClearHistory(pinyin_history);
            return 0;
		}

        ClearHistory(pinyin_history);
    }

    /* Get Candidate */
    if (!IIMEDPYCORE_JumpToPos(pinyin_history->hCore, word_match_index))
	{
        ClearHistory(pinyin_history);
        return 0;
	}

    word_match_result->count = 0;
    word_match_result->more_available = 0;
    while(word_match_result->count < KMX_MAX_WORD_MATCH + 1)
    {
        IIME_WCHAR wszCandidate[MAX_LEX_LENGTH + 2];
        unsigned short nLen, nInputLen;
        KMX_PINYIN_CANDIDATE *pCandidate = word_match_result->table + word_match_result->count;
        int result = IIMEDPYCORE_GetNextCandidate(pinyin_history->hCore, wszCandidate, &nLen, &nInputLen);

        if (result >= 0)
        {
            if (word_match_result->count == KMX_MAX_WORD_MATCH)
            {
                word_match_result->more_available = 1;
                break;
            }
            else
            {
                pCandidate->length = nLen;
                pCandidate->match_length = CalcMatchLength(digit_string, digit_string_length, nInputLen);
                memset(pCandidate->string, 0, sizeof(IIME_WCHAR) * KMX_MAX_PINYIN_WORD_LENGTH);
                memcpy(pCandidate->string, wszCandidate, sizeof(IIME_WCHAR) * nLen);
                word_match_result->count++;
            }
        }
        else
            break;
    }

    /* fill pinyin_composition */
    if (pinyin_composition)
    {
        int star_prefix = 0;

        memset(pinyin_composition, 0, sizeof(KMX_PINYIN_COMPOSITION));
        count = 0;
        pComposition = pinyin_composition->string;
#if defined(LENOVO_SPECIAL)
		if ( (pinyin_match_index == KMX_PINYIN_MATCH_INDEX_NONE) && *(pAssertPinyin[0]))
        {
            *(pComposition++) = '*';
            star_prefix = 1;
        }
#endif
        if (pAssertPinyin[0][0] == 0)
            p = pinyin_choice.m_pChoice[0];
        else
            p = pAssertPinyin[count];
        while (*p)
        {
            int pos;
            *(pComposition++) = (char)(*p + 'A' - 'a');
            p++;
            while(*p)
                *(pComposition++) = (char)(*(p++));

            pos = strlen(pinyin_composition->string) - star_prefix;
            if (pos < digit_string_length && digit_string[pos] == ' ')
                *(pComposition++) = ' ';

            p = pAssertPinyin[++count];
        }
#if defined(NOLLEC_LENOVO_SPLIT) || defined(NOLLEC_LENOVO_SPLIT_1) || defined(NOLLEC_LENOVO_SPLIT_2)
        pinyin_TranslateSplit(pinyin_composition->string);
#endif
        pinyin_composition->length =(unsigned short)strlen(pinyin_composition->string);
    }

    /* fill pinyin_match_result */
    if (pinyin_match_result)
    {
        memset(pinyin_match_result, 0, sizeof(KMX_PINYIN_MATCH_RESULT));
        count = 0;

        pinyin_match_result->count = (unsigned short)pinyin_choice.m_nChoice;
        while (count < pinyin_match_result->count)
        {
            Pinyin_String *pinyin = pinyin_choice.m_pChoice + count;
            KMX_CHAR_PINYIN *pinyin_char = pinyin_match_result->table + count;

            p = pinyin[0];
            i = 0;
            while(*p)
            {
                pinyin_char->string[i] =(char)(*p);
                i++;
                p++;
            }

            pinyin_char->length = (unsigned short)i;
            count++;
        }
    }

    /* save the history */
    memcpy(pinyin_history->digit_string, digit_string, digit_string_length);
    pinyin_history->digit_string_length = digit_string_length;
    pinyin_history->pinyin_match_index = pinyin_match_index;

    return 1;
}

int kmx_pinyin_match_length(
    KMX_IME_ENGINE*                 engine,
    const char*                     pinyin_string,
    unsigned short                  pinyin_string_length,
    unsigned short                  word_match_index,
    unsigned short                  max_match_word_length,
    KMX_PINYIN_WORD_MATCH_RESULT*   word_match_result)
{
    int i;
    IIME_WCHAR wszCodeString[MAX_PREEDIT_LENGTH + 2];
    char CodeString[MAX_PREEDIT_LENGTH + 2];

	Pinyin_History *pinyin_history;

    pinyin_history = (Pinyin_History*)(engine->engine);
    if (pinyin_history->is_digit){
        if (switch_pinyin_method(pinyin_history, 0)){
            ClearHistory(pinyin_history);
        }
        else
            return 0;
    }
    
    if (pinyin_string_length > MAX_PREEDIT_LENGTH)
        return 0;

    memset(wszCodeString, 0, (MAX_PREEDIT_LENGTH + 2) * sizeof(IIME_WCHAR));
    memset(CodeString, 0, (MAX_PREEDIT_LENGTH + 2) * sizeof(char));

    for (i = 0; i < pinyin_string_length; i++)
    {
        wszCodeString[i] = pinyin_string[i];
        CodeString[i] = pinyin_string[i];
    }

    if (strcmp(pinyin_history->pinyin_string, CodeString) != 0)
    {
        if (!IIMEDPYCORE_SetCodeString(pinyin_history->hCore, wszCodeString, max_match_word_length))
        {
		    memset(pinyin_history->pinyin_string, 0, 64);
            return 0;
        }
    }

    /* Get Candidate */
    if (!IIMEDPYCORE_JumpToPos(pinyin_history->hCore, word_match_index))
	{
		memset(pinyin_history->pinyin_string, 0, 64);
        return 0;
	}

    word_match_result->count = 0;
    word_match_result->more_available = 0;
    while(word_match_result->count < KMX_MAX_WORD_MATCH + 1)
    {
        IIME_WCHAR wszCandidate[MAX_LEX_LENGTH + 2];
        unsigned short nLen, nInputLen;
        KMX_PINYIN_CANDIDATE *pCandidate = word_match_result->table + word_match_result->count;
        int result = IIMEDPYCORE_GetNextCandidate(pinyin_history->hCore, wszCandidate, &nLen, &nInputLen);

        if (result >= 0)
        {
            if (word_match_result->count == KMX_MAX_WORD_MATCH)
            {
                word_match_result->more_available = 1;
                break;
            }
            else
            {
                pCandidate->length = nLen;
                pCandidate->match_length = nInputLen;
                memset(pCandidate->string, 0, sizeof(IIME_WCHAR) * KMX_MAX_PINYIN_WORD_LENGTH);
                memcpy(pCandidate->string, wszCandidate, sizeof(IIME_WCHAR) * nLen);
                word_match_result->count++;
            }
        }
        else
            break;
    }

    /* save the history */
	memset(pinyin_history->pinyin_string, 0, 64);
    memcpy(pinyin_history->pinyin_string, pinyin_string, pinyin_string_length);

    return 1;
}

int kmx_pinyin_match(
    KMX_IME_ENGINE*                 engine,
    const char*                     pinyin_string,
    unsigned short                  pinyin_string_length,
    unsigned short                  word_match_index,
    KMX_PINYIN_WORD_MATCH_RESULT*   word_match_result)
{
    return kmx_pinyin_match_length(engine,
                                   pinyin_string,
                                   pinyin_string_length,
                                    word_match_index,
                                    MAX_LEX_LENGTH,
                                    word_match_result);
}

int kmx_pinyin_match_1(
    KMX_IME_ENGINE*                 engine,
    const char*                     pinyin_string,
    unsigned short                  pinyin_string_length,
    unsigned short                  word_match_index,
    KMX_PINYIN_WORD_MATCH_RESULT*   word_match_result)
{
    int i;
    IIME_WCHAR wszCodeString[MAX_PREEDIT_LENGTH];
	Pinyin_History *pinyin_history;

    pinyin_history = (Pinyin_History*)(engine->engine);
    if (pinyin_history->is_digit){
        if (switch_pinyin_method(pinyin_history, 0)){
            ClearHistory(pinyin_history);
        }
        else
            return 0;
    }

    memset(wszCodeString, 0, MAX_PREEDIT_LENGTH);
    for (i = 0; i < pinyin_string_length; i++)
        wszCodeString[i] = pinyin_string[i];

    if (strcmp(pinyin_history->pinyin_string, pinyin_string) != 0)
    {
        if (!IIMEDPYCORE_SetCodeString(pinyin_history->hCore, wszCodeString, 0xFFFF))
        {
		    memset(pinyin_history->pinyin_string, 0, 64);
            return 0;
        }
    }

    /* Get Candidate */
    if (!IIMEDPYCORE_JumpToPos(pinyin_history->hCore, word_match_index))
	{
		memset(pinyin_history->pinyin_string, 0, 64);
        return 0;
	}

    word_match_result->count = 0;
    word_match_result->more_available = 0;
    while(word_match_result->count < KMX_MAX_WORD_MATCH + 1)
    {
        IIME_WCHAR wszCandidate[MAX_LEX_LENGTH + 2];
        unsigned short nLen, nInputLen;
        KMX_PINYIN_CANDIDATE *pCandidate = word_match_result->table + word_match_result->count;
        int result = IIMEDPYCORE_GetNextCandidate(pinyin_history->hCore, wszCandidate, &nLen, &nInputLen);

        if (result >= 0)
        {
            if (word_match_result->count == KMX_MAX_WORD_MATCH)
            {
                word_match_result->more_available = 1;
                break;
            }
            else
            {
                pCandidate->length = nLen;
                pCandidate->match_length = nInputLen;
                memset(pCandidate->string, 0, sizeof(IIME_WCHAR) * KMX_MAX_PINYIN_WORD_LENGTH);
                memcpy(pCandidate->string, wszCandidate, sizeof(IIME_WCHAR) * nLen);
                word_match_result->count++;
            }
        }
        else
            break;
    }

    /* save the history */
	memset(pinyin_history->pinyin_string, 0, 64);
    memcpy(pinyin_history->pinyin_string, pinyin_string, pinyin_string_length);

    return 1;
}

int kmx_pinyin_update_cache(
    KMX_IME_ENGINE*                 engine,
    const unsigned short*           string,
    unsigned short                  length)
{
    int i;
    unsigned short pyCode[MAX_LEX_LENGTH + 8];
    Pinyin_History *pinyin_history;

#if defined(KMX_SP)
    s_output_number += length;
    if (s_output_number > 30)
    {
        IIME_IMM_SetOutputNumber(s_output_number);
        s_output_number = 0;
    }
#endif

    if (length > MAX_LEX_LENGTH)
        return 0;

    pinyin_history = (Pinyin_History*)(engine->engine);
    ClearHistory(pinyin_history);

    memset(pyCode, 0, sizeof(unsigned short) * MAX_LEX_LENGTH);

    for (i = 0; i < length; i++){
        if (HanziCodeMap_Uni2HZCode(pinyin_history->pdb, string[i], pyCode + i) == 0)
            return 0;
    }

    IIMEDPYCORE_AddPhrase(pinyin_history->hCore, pyCode, length);

    return 1;
}

int kmx_pinyin_check_association(
    KMX_IME_ENGINE*                 engine,
    unsigned short                  char_code,
    unsigned short*                 buffer,
    int                             number)
{
    int n = number;
    Pinyin_History *pinyin_history = (Pinyin_History *)(engine->engine);
    ClearHistory(pinyin_history);

    if(IIMEDPYCORE_CheckAsso(pinyin_history->hCore, char_code, buffer, &n) == IIME_ERROR_SUCCESS)
        return n;
    else
        return 0;
}

/*****************************************************************************
 *
 * zhuyin api
 *
 *****************************************************************************/
typedef struct tag_zhuyin_history
{
    IIME_HCORE            hCore;
    IIMEDPYCORE_PARAMS    user_cache;
    const KMX_PINYIN_DB *pdb;

    unsigned short zhuyin_string[64];
    unsigned short zhuyin_length;
    unsigned short is_danzi;
}Zhuyin_History;

/*Zhuyin_History zhuyin_history;*/

int kmx_zhuyin_initialize(
    KMX_IME_ENGINE*                 engine,
    const KMX_IME_ENGINE_INIT*      init_info)
{
	Zhuyin_History *zhuyin_history = 0;

#ifdef SELFALLOC_PRODUCT
    IIME_MRESET(IIME_NULL);
#endif

	zhuyin_history = IIME_MALLOC(16 * 1024); /*sizeof(Zhuyin_History));*/
    memset(zhuyin_history, 0, sizeof(Zhuyin_History));

    zhuyin_history->user_cache.pCache  = init_info->user_data;
    zhuyin_history->user_cache.szCache = init_info->user_data_size;
    zhuyin_history->pdb = (const KMX_PINYIN_DB*)init_info->database;
    zhuyin_history->hCore = (char*)zhuyin_history + sizeof(Zhuyin_History);

    zhuyin_history->hCore = IIMEDPYCORE_Initialize(&(zhuyin_history->user_cache),
                                                 IM_ZHUYIN_FULLKEY, 
                                                 zhuyin_history->pdb, 
                                                 zhuyin_history->hCore,
                                                 0);
    if (zhuyin_history->hCore)
    {
        IIME_IMM_OPTIONS option = {0};
        option.dwZhuyinSymbolAsCand = 1;

        IIMEDPYCORE_SetOptions(zhuyin_history->hCore, &option);
        engine->engine = (int*)zhuyin_history;
        return 1;
    }
    else
    {
        return 0;
    }
}

#define IS_TONE(x) (x == 0X02C9 || x == 0x02CA || x == 0x02C7 || x == 0x02CB || x == 0x02D9)

int kmx_zhuyin_match_length(
    KMX_IME_ENGINE*                 engine,
    const unsigned short*           zhuyin_string,
    unsigned short                  zhuyin_string_length,
    unsigned short                  word_match_index,
    unsigned short                  max_match_word_length,
    KMX_ZHUYIN_WORD_MATCH_RESULT*   word_match_result)
{
    int i;
    IIME_BOOL bMatch = IIME_TRUE;
	Zhuyin_History *zhuyin_history;
    unsigned short wszCodeString[MAX_PREEDIT_LENGTH];

    zhuyin_history = (Zhuyin_History*)(engine->engine);
    
    /* judge the dup tune*/
    {
        if (zhuyin_string_length > 0)
            if ( IS_TONE(zhuyin_string[0]) )
            {
                zhuyin_history->zhuyin_length = 0;
                return 0;
            }

        if (zhuyin_string_length > 1)
        {
            for (i = 0; i < zhuyin_string_length - 1; i++)
            {
                if (IS_TONE(zhuyin_string[i] ) && IS_TONE(zhuyin_string[i + 1]) )
                {
                    zhuyin_history->zhuyin_length = 0;
                    return 0;
                }
            }
        }
    }

    if (zhuyin_history->zhuyin_length == zhuyin_string_length)
    {
        for (i = 0; i < zhuyin_string_length; i++)
        {
            if (zhuyin_history->zhuyin_string[i] != zhuyin_string[i])
            {
                bMatch = IIME_FALSE;
                break;
            }
        }
    }
    else
        bMatch = IIME_FALSE;
    
    if (!bMatch)
    {
        unsigned int max_lex_len;
        memset(wszCodeString, 0, sizeof(unsigned short) * MAX_PREEDIT_LENGTH);
        for (i = 0; i < zhuyin_string_length; i++)
            wszCodeString[i] = zhuyin_string[i];

        max_lex_len = max_match_word_length;

/*
#if defined(LENOVO_SPECIAL)
        if (IIMEDPYCORE_SetCodeString(zhuyin_history->hCore, wszCodeString, max_lex_len) != 1)
#else
*/
        if (IIMEDPYCORE_SetCodeString(zhuyin_history->hCore, wszCodeString, max_lex_len) == 0)
/*
#endif
*/
        {
            zhuyin_history->zhuyin_length = 0;
            return 0;
        }
    }

    /* Get Candidate */
    if (!IIMEDPYCORE_JumpToPos(zhuyin_history->hCore, word_match_index))
	{
		zhuyin_history->zhuyin_length = 0;
        return 0;
	}

    word_match_result->count = 0;
    word_match_result->more_available = 0;
    while(word_match_result->count < KMX_MAX_WORD_MATCH + 1)
    {
        IIME_WCHAR wszCandidate[MAX_LEX_LENGTH + 2];
        unsigned short nLen, nInputLen;
        KMX_ZHUYIN_CANDIDATE *pCandidate = word_match_result->table + word_match_result->count;
        int result = IIMEDPYCORE_GetNextCandidate(zhuyin_history->hCore, wszCandidate, &nLen, &nInputLen);

        if (result >= 0)
        {
            if (word_match_result->count == KMX_MAX_WORD_MATCH)
            {
                word_match_result->more_available = 1;
                break;
            }
            else
            {
                pCandidate->length = nLen;
                pCandidate->match_length = nInputLen;
                memset(pCandidate->string, 0, sizeof(IIME_WCHAR) * KMX_MAX_PINYIN_WORD_LENGTH);
                memcpy(pCandidate->string, wszCandidate, sizeof(IIME_WCHAR) * nLen);
                word_match_result->count++;
            }
        }
        else
            break;
    }

	zhuyin_history->zhuyin_length = zhuyin_string_length;
	for (i = 0; i < zhuyin_string_length; i++)
	{
		zhuyin_history->zhuyin_string[i] = zhuyin_string[i];
	}

    return 1;
}


int kmx_zhuyin_match(
    KMX_IME_ENGINE*                 engine,
    const unsigned short*           zhuyin_string,
    unsigned short                  zhuyin_string_length,
    unsigned short                  word_match_index,
    KMX_ZHUYIN_WORD_MATCH_RESULT*   word_match_result)
{
    return kmx_zhuyin_match_length(engine,
                                   zhuyin_string,
                                   zhuyin_string_length,
                                   word_match_index,
                                   MAX_LEX_LENGTH,
                                   word_match_result);
}


static const char BOPOMOFO_DIGIT_MAP[] = "1111222233344455556667777888899999000";

static int FindDupZhuyin(unsigned short *zhuyin_string, unsigned short zhuyin_number, const unsigned short *pPinyin, unsigned short digit_length)
{
    unsigned short *p;
    unsigned short i, j;

    p = zhuyin_string;
    for (i = 0; i < zhuyin_number; i++)
    {
        int matched = 1;
        for (j = 0; j < digit_length; j++)
        {
            if (p[j] != pPinyin[j])
            {
                matched = 0;
                break;
            }
        }

        if (matched)
            return 1;

        p += digit_length + 1;
    }

    return 0;
}


int kmx_digit_to_zhuyin(
    KMX_IME_ENGINE*                 engine,
    const char*                     digit_string,
    unsigned short                  digit_length,
    unsigned short*                 zhuyin_string,
    unsigned short*                 pzhuyin_number)
{
    Zhuyin_History *zhuyin_history;
    const KMX_PINYIN_DB *pdb;
    unsigned short i, nPinyin;
    unsigned short *p;

    zhuyin_history = (Zhuyin_History*)(engine->engine);
    pdb = zhuyin_history->pdb;

    *pzhuyin_number = 0;

    for (i = 0; i < digit_length; i++)
    {
        if (digit_string[i] < '0' || digit_string[i] > '9')
            return 0;
    }

    p = zhuyin_string;
    nPinyin = pdb->nPinyin;
    for (i = 0; i < nPinyin; i++)
    {
        int j;
        const unsigned short *pPinyin = pdb->pPinyinTable + i * (MAX_PINYIN_LEN_ZY + 1) ;/*pFilter->m_pPinyinTable[i];*/
        char pDigit[4];
        for (j = 0; j < 4; j++)
        {
            if (pPinyin[j] == 0)
            {
                pDigit[j] = 0;
                break;
            }
            pDigit[j] = BOPOMOFO_DIGIT_MAP[pPinyin[j] - 0x3105];
        }

        if (strncmp(digit_string, pDigit, digit_length) == 0)
        {
            if (!FindDupZhuyin(zhuyin_string, *pzhuyin_number, pPinyin, digit_length))
            {
                for (j = 0; j < digit_length; j++)
                {
                    *(p++) = pPinyin[j];
                }

                *(p++) = 0;
                
                (*pzhuyin_number)++;
            }
        }
    }

    if (*pzhuyin_number == 0)
        return 0;
    else
        return 1;
}

int kmx_zhuyin_update_cache(
    KMX_IME_ENGINE*                 engine,
    const unsigned short*           string,
    unsigned short                  length)
{
    int i;
    unsigned short pyCode[MAX_LEX_LENGTH + 8];
    Zhuyin_History *zhuyin_history = (Zhuyin_History *)(engine->engine);

    memset(zhuyin_history->zhuyin_string, 0, sizeof(unsigned short)*64);
    zhuyin_history->zhuyin_length = 0;

    if (length > MAX_LEX_LENGTH)
        return 0;

    memset(pyCode, 0, sizeof(unsigned short) * MAX_LEX_LENGTH);

    for (i = 0; i < length; i++)
    {
        if (HanziCodeMap_Uni2HZCode(zhuyin_history->pdb, string[i], pyCode + i) == 0)
            return 0;
    }

    IIMEDPYCORE_AddPhrase(zhuyin_history->hCore, pyCode, length);

    return 1;

}

int kmx_zhuyin_check_association(
    KMX_IME_ENGINE*                 engine,
    unsigned short                  char_code,
    unsigned short*                 buffer,
    int                             number)
{
    int n = number;
    Zhuyin_History *zhuyin_history = (Zhuyin_History *)(engine->engine);

    memset(zhuyin_history->zhuyin_string, 0, sizeof(unsigned short)*64);
    zhuyin_history->zhuyin_length = 0;

    if (IIMEDPYCORE_CheckAsso(zhuyin_history->hCore, char_code, buffer, &n) == IIME_ERROR_SUCCESS)
        return n;
    else
        return 0;
}



/****************************************************************************************
 *
 * Query Association
 *
 ****************************************************************************************/
extern IIME_RESULT   HanziAsso_QueryAsso_ByDB(const KMX_PINYIN_DB *pdb,
                                IIME_WCHAR wContext, 
                                IIME_PWSTR pwszAsso,
                                IIME_INT *pnAsso);

int kmx_chinese_query_association(
    const void*                     database,
    unsigned short                  char_code,
    unsigned short*                 buffer,
    int                             buffer_length)
{
    int number;
    const KMX_PINYIN_DB *pdb = (const KMX_PINYIN_DB *)database;

    number = buffer_length;
    if (HanziAsso_QueryAsso_ByDB(pdb, char_code, buffer, &number) == IIME_ERROR_SUCCESS)
        return number;
    else
        return 0;
}

extern IIME_BOOL QueryPinyinZhuyinByDB(const KMX_PINYIN_DB *pdb, IIME_WCHAR wCode, IIME_WCHAR *wszPinyin, int maxDyz);

int kmx_chinese_query_pinyin(
    const void*                     database,
    unsigned short                  char_code,
    KMX_CHAR_PINYIN*                pinyin)
{
    IIME_BOOL result;
    int i;
    unsigned short wszPinyin[8];
    const KMX_PINYIN_DB *pdb = (const KMX_PINYIN_DB *)database;

    memset(wszPinyin, 0, 16);
    result = QueryPinyinZhuyinByDB(pdb, char_code, wszPinyin, 1);

    if (result)
    {
        i = 0;
        while (wszPinyin[i]){
            pinyin->string[i] = (char)(wszPinyin[i]);
            i++;
        }

        pinyin->length = (unsigned short)i;

        return 1;
    }
    else
        return 0;
}

int kmx_chinese_query_pinyin_dyz(
    const void*                     database,
    unsigned short                  char_code,
    KMX_CHAR_PINYIN*                pinyin,
    int                             maxDyz)
{
    IIME_BOOL result;
    int i;
    unsigned short wszPinyin[128];
    unsigned short *pBuf;
    const KMX_PINYIN_DB *pdb = (const KMX_PINYIN_DB *)database;

    memset(wszPinyin, 0, 128 * 2);
    result = QueryPinyinZhuyinByDB(pdb, char_code, wszPinyin, maxDyz);

    i = 0;
    pBuf = wszPinyin;
    while (*pBuf)
    {
        char *p = pinyin[i].string;
        int n = 0;
        while(*pBuf)
        {
            p[n] = (char)(*pBuf);
            n++;
            pBuf++;
        }
        p[n] = 0;
        pinyin[i].length = (unsigned short)n;

        pBuf++;
        i++;
    }

    return i;
/*
    if (result)
    {
        i = 0;
        while (wszPinyin[i]){
            pinyin->string[i] = (char)(wszPinyin[i]);
            i++;
        }

        pinyin->length = (unsigned short)i;

        return result;
    }
    else
        return 0;
*/
}

int kmx_chinese_query_zhuyin(
    const void*                     database,
    unsigned short                  char_code,
    KMX_CHAR_ZHUYIN*                zhuyin)
{
    IIME_BOOL result;
    int i;
    unsigned short wszPinyin[8];
    const KMX_PINYIN_DB *pdb = (const KMX_PINYIN_DB *)database;

    memset(wszPinyin, 0, 16);

    result = QueryPinyinZhuyinByDB(pdb, char_code, wszPinyin, 1);
    if (result)
    {
        i = 0;
        while (wszPinyin[i]){
            zhuyin->string[i] = wszPinyin[i];
            i++;
        }

        zhuyin->length = (unsigned short)i;

        return 1;
    }
    else
        return 0;

}

#if defined(IIME_MARS_SCRIPT)

#include "core/ldbapi2/kmxime_stroke.h"
#include "core/digitpinyin/digitpinyinconst.h"

static int binary_search_earth(const unsigned short *table, int len, unsigned short value)
{
    if (table != 0 && len > 0)
    {
        int beg, end, mid;

        beg = 0;
        end = len - 1;

        while (beg <= end)
        {
            mid = (beg + end) / 2;
            if (value < table[mid])
                end = mid - 1;
            else if (value > table[mid])
                beg = mid + 1;
            else
                return mid;
        }
    }

    return -1;
}

static void KMX_Earth2Mars(unsigned short *buf, unsigned short buffer_length, const unsigned short* earth, const unsigned short *mars, int table_len)
{
    int i;
    for (i = 0; i < buffer_length; i++)
    {
        unsigned short code = buf[i];
        if (code)
        {
            int pos = binary_search_earth(earth, table_len, code);
            if (pos > 0)
                buf[i] = mars[pos];
        }
    }

    return ;
}


int kmx_chinese_mars_translate(
    const void*                     database,
    unsigned short*                 buffer,
    unsigned short                  buffer_length)
{
    const unsigned short *earth, *mars;
    int len;

    const KMX_PINYIN_DB *pdb = (const KMX_PINYIN_DB *)database;

    if (!pdb)
        return 0;

    if ((pdb->nFormat & KMX_DB_FLAG_GB2312) || (pdb->nFormat & KMX_DB_FLAG_COMPACT))
    {
        earth = earth_gb2312;
        mars = mars_gb2312;
        len = earth_mars_gb2312_length;
    }
    else
    {
        earth = earth_gbk;
        mars = mars_gbk;
        len = earth_mars_gbk_length;
    }
    
    KMX_Earth2Mars(buffer, buffer_length, earth, mars, len);

    return 1;
}

#endif /* IIME_MARS_SCRIPT */
