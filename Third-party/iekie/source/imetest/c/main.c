
#include <stdio.h>
#include <string.h>

#include "IIMEk2api.h"

const IIME_IME_INFO_PTR IIME_input_method_config[] =
{
    &IIME_ldb_pinyin_cizu,
    &IIME_ldb_stroke_gb2312,
    &IIME_ldb_english,
    0
};

static int IIME_GetTextWidth(const unsigned short* text, int length)
{
    const unsigned short *p = text;
    int width = 0;
    while (*p){
        width += 16;
        p++;
    }

    return width;
}

static int IIME_imm_user_data[16 * 1024 / 4];
static int IIME_imm_user_data_size = 16 *1024;

int main(int argc, char* argv[])
{
	IIME_RESULT result;
	IIME_HIMM IIME_imm;
    IIME_IMM_PARAMS   ip;
    IIME_USER_PARAMS  up;


    memset(&ip, 0, sizeof(ip));
    ip.nMaxPYChoice  = 20;
    ip.nLableWidth   = 8;
    ip.nWinWidth     = 240;
    ip.pGetTextWidthFunc = IIME_GetTextWidth;

    memset(&up, 0, sizeof(up));
    up.pUserCache = (char*)IIME_imm_user_data;
    up.iUserCacheSize = IIME_imm_user_data_size;


    IIME_imm = IIME_IMM_Initialize(0, &ip, &up);
	result = IIME_IMM_SetInputMethod(IIME_imm, IM_PINYIN);

    return result;
}

