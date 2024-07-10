
/*
 * kmx_ime.c
 *
 * Implementation of KOMOXO input method user interface.
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "resource.h"
#include "kmx_ime.h"
#include "gui.h"
/*
 * CSTAR input method private headers
 */

#include "IIMEk2api.h"
#include "kmxime.h"
#include "kmx_all_ldb.h"

typedef struct _tagKMX_LDB_INFO
{
    int             id;
    const wchar_t*  name;
    const void**     ldb;
} KMX_LDB_TAG;

int FindUserCacheByIM(int iMethod, const IIME_USER_PARAMS *pUserTotal, IIME_USER_PARAMS *pUserIM);

static KMX_IME_ENGINE engine;

/**************************************************************************************************
 *
 * pinyin 
 *
 **************************************************************************************************/
#if defined(CS_PINYIN) || defined(CS_PINYIN_MINI)
KMX_LDB_TAG pinyin_ldb_tag[] = 
{
#if defined(PY_GB2312_LEVEL1_20K_DIGIT)
    {IM_PINYIN, L"kmx_ldb_pinyin_gb2312", &kmx_ldb_pinyin_gb2312},
#elif defined(PY_GB2312_LEVEL2_20K_DIGIT)
    {IM_PINYIN, L"kmx_ldb_pinyin_gb2312", &kmx_ldb_pinyin_gb2312},
#elif defined(PY_GB2312_LEVEL2_10K_DIGIT)
    {IM_PINYIN, L"kmx_ldb_pinyin_gb2312", &kmx_ldb_pinyin_gb2312},
#elif defined(PY_GB2312_LEVEL1_10K_DIGIT)
    {IM_PINYIN, L"kmx_ldb_pinyin_gb2312", &kmx_ldb_pinyin_gb2312},
#endif

#if defined(PY_GBK_LEVEL1_20K_DIGIT)
    {IM_PINYIN, L"kmx_ldb_pinyin_gbk", &kmx_ldb_pinyin_gbk},
#endif

#if defined(PY_GB18030_LEVEL1_20K_DIGIT)
    {IM_PINYIN, L"kmx_ldb_pinyin_gb18030", &kmx_ldb_pinyin_gb18030},
#endif

#if defined(PY_BIG5_LEVEL1_20K_DIGIT)
    {IM_PINYIN, L"kmx_ldb_pinyin_big5", &kmx_ldb_pinyin_big5},
#endif

#if defined(PY_GB2312_LEVEL1_20K_SURETYPE)
    {IM_PINYIN, L"kmx_ldb_pinyin_gb2312_suretype", &kmx_ldb_pinyin_gb2312_suretype},
#endif

#if defined(PY_GB18030_LEVEL1_20K_SURETYPE)
    {IM_PINYIN, L"kmx_ldb_pinyin_gb18030_suretype", &kmx_ldb_pinyin_gb18030_suretype},
#endif

#if defined(PY_BIG5_LEVEL1_20K_SURETYPE)
    {IM_PINYIN, L"kmx_ldb_pinyin_big5_suretype", &kmx_ldb_pinyin_big5_suretype},
#endif

#if defined(DZPY_GB2312_LEVEL0_20K_QWERTY)
    {IM_PINYIN, L"kmx_ldb_pinyin_danzi", &kmx_ldb_pinyin_danzi},
#endif

#if defined(DZPY_GB2312_LEVEL0_0K_QWERTY) || defined(DZPY_GB2312_LEVEL0_5K_QWERTY) || defined(DZPY_GB2312_LEVEL0_10K_QWERTY)
    {IM_PINYIN, L"kmx_ldb_pinyin_danzi", &kmx_ldb_pinyin_danzi},
#endif

#if defined(DZPY_GBK_LEVEL0_20K_QWERTY)
    {IM_PINYIN, L"kmx_ldb_pinyin_danzi", &kmx_ldb_pinyin_danzi},
#endif

    {0}
};

static BOOL init_pinyin_engine(int index)
{
    const void *ldb;
    IIME_IMM_OPTIONS *imeoptiondata;
    IIME_USER_PARAMS user_params_total, user_params_pinyin;
    int imeoptiondata_size;

    KMX_IME_ENGINE_INIT init_info;

    ldb = *(pinyin_ldb_tag[index].ldb);

    imeoptiondata = gui_load_ime_options(&imeoptiondata_size);

    user_params_total.pUserCache = gui_load_ime_userdata(&user_params_total.iUserCacheSize);
    if (FindUserCacheByIM(pinyin_ldb_tag[index].id, &user_params_total, &user_params_pinyin))
    {
        init_info.user_data = user_params_pinyin.pUserCache;
        init_info.user_data_size = user_params_pinyin.iUserCacheSize;
    }
    else
    {
        init_info.user_data = 0;
        init_info.user_data_size = 0;
    }

    init_info.database = ldb; 
    init_info.flags = imeoptiondata->dwPYFuzzy;

    if (kmx_pinyin_initialize(&engine, &init_info))
        return TRUE;
    else
        return FALSE;
}

static void on_init_pinyin_dialog(HWND hwnd)
{
    int i;
    HWND hwndCombo;

    for (i = 0; pinyin_ldb_tag[i].id; i++)
    {
        const wchar_t *str;
        hwndCombo = GetDlgItem(hwnd, IDC_COMBO_PINYIN_LDB);
        str = pinyin_ldb_tag[i].name;
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)str);
    }
    SendDlgItemMessage(hwnd, IDC_COMBO_PINYIN_LDB, CB_SETCURSEL, 0, 0);

    if (init_pinyin_engine(0))
    {
        /* init IDC_COMBO_PINYIN combo */
        hwndCombo = GetDlgItem(hwnd, IDC_COMBO_PINYIN);
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_digit_pinyin_match");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_pinyin_match");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_pinyin_match_1");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_pinyin_update_cache");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_pinyin_check_association");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_pinyin_query_association");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_pinyin_query_pinyin");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_pinyin_query_pinyin_dyz");

        SendDlgItemMessage(hwnd, IDC_COMBO_PINYIN, CB_SETCURSEL, 0, 0);
    }
}


static void test_pinyin_digit_match(HWND hwnd)
{
    unsigned short index, aux;
    BOOL bTranslated;

    TCHAR wszInput[64];
    char szInput[64];

    unsigned short pinyin_string_length;

    KMX_PINYIN_COMPOSITION pinyin_composition;
    KMX_PINYIN_MATCH_RESULT pinyin_match_result;
    KMX_PINYIN_WORD_MATCH_RESULT word_match_result;

    GetDlgItemText(hwnd, IDC_EDIT_PINYIN_INPUT, wszInput, 64);
    WideCharToMultiByte(CP_ACP, 0, wszInput, -1, szInput, 64, NULL, NULL);

    index = (unsigned short)GetDlgItemInt(hwnd, IDC_EDIT_PINYIN_INDEX, &bTranslated, FALSE);
    aux   = (unsigned short)GetDlgItemInt(hwnd, IDC_EDIT_PINYIN_AUX, &bTranslated, FALSE);
    if (!bTranslated)
        aux = KMX_PINYIN_MATCH_INDEX_NONE;

    pinyin_string_length = (unsigned short)strlen(szInput);
    if (kmx_digit_pinyin_match(&engine, szInput, pinyin_string_length, 
                                aux, index, 
                                &pinyin_composition, 
                                &pinyin_match_result, 
                                &word_match_result))
    {
        int i, len;
        wchar_t out[512];
        wchar_t temp[64];

        wcscpy(out, L"Composition: ");
        len = pinyin_composition.length;
        MultiByteToWideChar(CP_ACP, 0, pinyin_composition.string, len, temp, len);
        temp[len] = 0;
        wcscat(out, temp);
        wcscat(out, L"\r\n");

        wcscat(out, L"Pinyin Result: ");
        for (i = 0; i < pinyin_match_result.count; i++)
        {
            KMX_CHAR_PINYIN pinyin = pinyin_match_result.table[i];
            len = pinyin.length;
            MultiByteToWideChar(CP_ACP, 0, pinyin.string, len, temp, len);
            temp[len] = 0;
            wcscat(out, temp);
            wcscat(out, L" ");
        }
        wcscat(out, L"\r\n");

        wcscat(out, L"Word Result: ");
        for (i = 0; i < word_match_result.count; i++)
        {
            KMX_PINYIN_CANDIDATE candidate = word_match_result.table[i];
            wcsncat(out, candidate.string, candidate.length);
            wsprintf(temp, L"(%d) ", candidate.match_length);
            wcscat(out, temp);
        }
        wcscat(out, L"\r\n");

        if (word_match_result.more_available)
            wcscat(out, L"...");

        SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, out);
    }
    else
        SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, L"");

}

static void test_pinyin_match(HWND hwnd)
{
    unsigned short index;
    BOOL bTranslated;

    unsigned short pinyin_string_length;
    wchar_t wszInput[64];
    char    szInput[64];

    KMX_PINYIN_WORD_MATCH_RESULT word_match_result;

    GetDlgItemText(hwnd, IDC_EDIT_PINYIN_INPUT, wszInput, 64);
    WideCharToMultiByte(CP_ACP, 0, wszInput, -1, szInput, 64, NULL, NULL);

    index = (unsigned short)GetDlgItemInt(hwnd, IDC_EDIT_PINYIN_INDEX, &bTranslated, FALSE);

    pinyin_string_length = (unsigned short)strlen(szInput);
    if (kmx_pinyin_match(&engine, szInput, pinyin_string_length, index, &word_match_result))
    {
        int i;
        wchar_t out[512];
        wchar_t temp[64];
       
        wcscpy(out, L"Word Result: ");
        for (i = 0; i < word_match_result.count; i++)
        {
            KMX_PINYIN_CANDIDATE candidate = word_match_result.table[i];
            wcsncat(out, candidate.string, candidate.length);
            wsprintf(temp, L"(%d) ", candidate.match_length);
            wcscat(out, temp);
        }
        wcscat(out, L"\r\n");

        if (word_match_result.more_available)
            wcscat(out, L"...");

        SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, out);

    }
    else
        SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, L"");

}

static void test_pinyin_match_1(HWND hwnd)
{
    unsigned short index;
    BOOL bTranslated;

    unsigned short pinyin_string_length;
    wchar_t wszInput[64];
    char    szInput[64];

    KMX_PINYIN_WORD_MATCH_RESULT word_match_result;

    GetDlgItemText(hwnd, IDC_EDIT_PINYIN_INPUT, wszInput, 64);
    WideCharToMultiByte(CP_ACP, 0, wszInput, -1, szInput, 64, NULL, NULL);

    index = (unsigned short)GetDlgItemInt(hwnd, IDC_EDIT_PINYIN_INDEX, &bTranslated, FALSE);

    pinyin_string_length = (unsigned short)strlen(szInput);
    if (kmx_pinyin_match_1(&engine, szInput, pinyin_string_length, index, &word_match_result))
    {
        int i;
        wchar_t out[512];
        wchar_t temp[64];
       
        wcscpy(out, L"Word Result: ");
        for (i = 0; i < word_match_result.count; i++)
        {
            KMX_PINYIN_CANDIDATE candidate = word_match_result.table[i];
            wcsncat(out, candidate.string, candidate.length);
            wsprintf(temp, L"(%d) ", candidate.match_length);
            wcscat(out, temp);
        }
        wcscat(out, L"\r\n");

        if (word_match_result.more_available)
            wcscat(out, L"...");

        SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, out);

    }
    else
        SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, L"");

}

static void test_pinyin_update_cache(HWND hwnd)
{
    wchar_t wszInput[64];
    unsigned short len;

    len = (unsigned short)GetDlgItemText(hwnd, IDC_EDIT_PINYIN_INPUT, wszInput, 64);

    if (kmx_pinyin_update_cache(&engine, wszInput, len))
        SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, L"Add phrase successed.");
    else
        SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, L"Add phrase failed.");

}

static void test_pinyin_check_asso(HWND hwnd)
{
    wchar_t wszInput[64];
    int len;

    len = GetDlgItemText(hwnd, IDC_EDIT_PINYIN_INPUT, wszInput, 64);
    if (len > 0)
    {
        int i, n;
        wchar_t out[512] = L"\0";
        wchar_t buffer[32 * 5] = L"\0";
        wchar_t char_code = wszInput[len - 1];
        n = kmx_pinyin_check_association(&engine, char_code, buffer, 32);
        if (n > 0)
        {
            wchar_t *p = buffer;
            for (i = 0; i < n; i++)
            {
                wcscat(out, p);
                wcscat(out, L" ");

                p += wcslen(p) + 1;
            }

            SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, out);
        }
        else
            SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, L"");
    }
    else
        SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, L"");
}

static void test_pinyin_query_asso(HWND hwnd)
{
    wchar_t wszInput[64];
    int len;
    int index = SendDlgItemMessage(hwnd, IDC_COMBO_PINYIN_LDB, CB_GETCURSEL, 0, 0);
    
    len = GetDlgItemText(hwnd, IDC_EDIT_PINYIN_INPUT, wszInput, 64);
    if (len > 0)
    {
        int n;
        int number = 32;
        unsigned short buffer[32];
        wchar_t out[512] = L"\0";
        n = kmx_chinese_query_association(*(pinyin_ldb_tag[index].ldb), wszInput[len - 1], buffer, number);
        if (n > 0)
        {
            wcsncpy(out, buffer, n);
            SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, out);
        }
        else 
            SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, L"failed");
    }
    else
        SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, L"");
}

static void test_pinyin_query_pinyin(HWND hwnd)
{
    wchar_t wszInput[64];
    int len;

    int index = SendDlgItemMessage(hwnd, IDC_COMBO_PINYIN_LDB, CB_GETCURSEL, 0, 0);

    len = GetDlgItemText(hwnd, IDC_EDIT_PINYIN_INPUT, wszInput, 64);
    if (len > 0)
    {
        int result;
        KMX_CHAR_PINYIN pinyin_buffer;
        wchar_t out[512] = L"\0";
        result = kmx_chinese_query_pinyin(*(pinyin_ldb_tag[index].ldb), wszInput[len - 1], &pinyin_buffer);
        //result = kmx_chinese_query_pinyin(kmx_ldb_py_gbk_c2_10k, wszInput[len - 1], &pinyin_buffer);
        if (result != 0)
        {
            int n;
            for (n = 0; n < pinyin_buffer.length; n++)
            {
                out[n] = pinyin_buffer.string[n];
            }
            out[n] = 0;
            SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, out);
        }
        else 
            SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, L"failed");
    }
    else
        SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, L"");
}

static void test_pinyin_query_pinyin_dyz(HWND hwnd)
{
    wchar_t wszInput[64];
    int len;

    int index = SendDlgItemMessage(hwnd, IDC_COMBO_PINYIN_LDB, CB_GETCURSEL, 0, 0);

    len = GetDlgItemText(hwnd, IDC_EDIT_PINYIN_INPUT, wszInput, 64);
    if (len > 0)
    {
        int result;
        KMX_CHAR_PINYIN pinyin_buffer[8];
        wchar_t out[512] = L"\0";
        result = kmx_chinese_query_pinyin_dyz(*(pinyin_ldb_tag[index].ldb), wszInput[len - 1], pinyin_buffer, 8);
        if (result != 0)
        {
            int i, n, m;
            m = 0;
            for (i = 0; i < result; i++)
            {
                for (n = 0; n < pinyin_buffer[i].length; n++)
                {
                    out[m++] = pinyin_buffer[i].string[n];
                }
                out[m++] = '\r';
                out[m++] = '\n';
            }
            SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, out);
        }
        else 
            SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, L"failed");
    }
    else
        SetDlgItemText(hwnd, IDC_EDIT_PINYIN_OUTPUT, L"");
}

static void on_test_pinyin(HWND hwnd)
{
    int index = SendDlgItemMessage(hwnd, IDC_COMBO_PINYIN, CB_GETCURSEL, 0, 0);
    switch (index)
    {
    case 0:
        test_pinyin_digit_match(hwnd);
        break;
    case 1:
        test_pinyin_match(hwnd);
        break;
    case 2:
        test_pinyin_match_1(hwnd);
        break;
    case 3:
        test_pinyin_update_cache(hwnd);
        break;
    case 4:
        test_pinyin_check_asso(hwnd);
        break;
    case 5:
        test_pinyin_query_asso(hwnd);
        break;
    case 6:
        test_pinyin_query_pinyin(hwnd);
        break;
    case 7:
        test_pinyin_query_pinyin_dyz(hwnd);
        break;
    }
}


static BOOL CALLBACK kmx_pinyin_dialog_proc(
    HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_INITDIALOG:
            on_init_pinyin_dialog(hwnd);
            return TRUE;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                    EndDialog(hwnd, 0);
                    return TRUE;
                case IDCANCEL:
                    EndDialog(hwnd, 0);
                    return TRUE;

                case IDC_BTN_PINYIN:
                    on_test_pinyin(hwnd);
                    return TRUE;
                case IDC_COMBO_PINYIN_LDB:
                    if (HIWORD(wParam) == CBN_SELCHANGE)
                    {
                        int index = SendDlgItemMessage(hwnd, IDC_COMBO_PINYIN_LDB, CB_GETCURSEL, 0, 0);
                        init_pinyin_engine(index);
                        return TRUE;
                    }
                    return FALSE;
            }
            break;
    }

    return FALSE;
}

void execute_pinyin_test_dialog(HWND parent)
{
    DialogBox(g_instance, MAKEINTRESOURCE(ID_KMX_PINYIN_DIALOG), parent, kmx_pinyin_dialog_proc);
}

#else
void execute_pinyin_test_dialog(HWND parent)
{
    return;
}
#endif

/**************************************************************************************************
 *
 * zhuyin 
 *
 **************************************************************************************************/
#if defined(CS_ZHUYIN) || defined(CS_ZHUYIN_MINI)
static KMX_LDB_TAG zhuyin_ldb_tag[] = 
{
#if defined(ZY_GB2312_LEVEL1_20K)
    {IM_ZHUYIN_MT, L"kmx_ldb_zhuyin_gb2312", &kmx_ldb_zhuyin_gb2312},
#endif
#if defined(ZY_GB18030_LEVEL1_20K)
    {IM_ZHUYIN_MT, L"kmx_ldb_zhuyin_gb18030", &kmx_ldb_zhuyin_gb18030},
#endif
#if defined(ZY_BIG5_LEVEL1_20K)
    {IM_ZHUYIN_MT, L"kmx_ldb_zhuyin_big5", &kmx_ldb_zhuyin_big5},
#endif
#if defined(ZY_BIG5NORMAL_LEVEL1_20K)
    {IM_ZHUYIN_MT, L"kmx_ldb_zhuyin_big5normal", &kmx_ldb_zhuyin_big5normal},
#endif
#if defined(ZY_BIG5HKSCS_LEVEL1_20K)
    {IM_ZHUYIN_MT, L"kmx_ldb_zhuyin_big5hkscs", &kmx_ldb_zhuyin_big5hkscs},
#endif
#if defined(DZZY_BIG5NORMAL_LEVEL0_20K)
    {IM_ZHUYIN_MT, L"kmx_ldb_zhuyin_danzi", &kmx_ldb_zhuyin_danzi},
#endif
    {0}
};

static BOOL init_zhuyin_engine(int index)
{
    const void *ldb;
    IIME_USER_PARAMS user_params_total, user_params_pinyin;

    KMX_IME_ENGINE_INIT init_info;

    ldb = *(zhuyin_ldb_tag[index].ldb);

    user_params_total.pUserCache = gui_load_ime_userdata(&user_params_total.iUserCacheSize);
    if (FindUserCacheByIM(zhuyin_ldb_tag[index].id, &user_params_total, &user_params_pinyin))
    {
        init_info.user_data = user_params_pinyin.pUserCache;
        init_info.user_data_size = user_params_pinyin.iUserCacheSize;
    }
    else
    {
        init_info.user_data = 0;
        init_info.user_data_size = 0;
    }

    init_info.database = ldb;
    init_info.flags = 0;

    if (kmx_zhuyin_initialize(&engine, &init_info))
        return TRUE;
    else
        return FALSE;
}

static void on_init_zhuyin_dialog(HWND hwnd)
{
    int i;
    HWND hwndCombo;

    for (i = 0; zhuyin_ldb_tag[i].id; i++)
    {
        const wchar_t *str;
        hwndCombo = GetDlgItem(hwnd, IDC_COMBO_ZHUYIN_LDB);
        str = zhuyin_ldb_tag[i].name;
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)str);
    }
    SendDlgItemMessage(hwnd, IDC_COMBO_ZHUYIN_LDB, CB_SETCURSEL, 0, 0);

    if (init_zhuyin_engine(0))
    {
        /* init IDC_COMBO_PINYIN combo */
        hwndCombo = GetDlgItem(hwnd, IDC_COMBO_ZHUYIN);
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_digit_to_zhuyin");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_zhuyin_match");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_zhuyin_update_cache");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_zhuyin_check_association");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_chinese_query_association");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_chinese_query_zhuyin");

        SendDlgItemMessage(hwnd, IDC_COMBO_ZHUYIN, CB_SETCURSEL, 0, 0);
    }
}

static void test_digit_to_zhuyin(HWND hwnd)
{
    wchar_t wszInput[64];
    char    szInput[64];
    unsigned short len;

    wchar_t zhuyin_string[256];
    unsigned short     nZhuyin;

    len = (unsigned short)GetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_INPUT, wszInput, 64);
    if (len == 0)
    {
        SetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_OUTPUT, L"");
        return;
    }
    WideCharToMultiByte(CP_ACP, 0, wszInput, -1, szInput, 64, NULL, NULL);

    if (kmx_digit_to_zhuyin(&engine, szInput, len, zhuyin_string, &nZhuyin))
    {
        int i;
        wchar_t out[512] = L"\0";
        wchar_t *p = zhuyin_string;
        for (i = 0; i < nZhuyin; i++)
        {
            wcscat(out, p);
            p += wcslen(p) + 1;
            wcscat(out, L"\r\n");
        }

        SetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_OUTPUT, out);
    }
    else
        SetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_OUTPUT, L"Failed!");

}

static void test_zhuyin_match(HWND hwnd)
{
    wchar_t wszInput[64];
    unsigned short len, index;
    KMX_ZHUYIN_WORD_MATCH_RESULT word_match_result;

    len = (unsigned short)GetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_INPUT, wszInput, 64);
    if (len == 0)
    {
        SetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_OUTPUT, L"");
        return;
    }

    index = (unsigned short)GetDlgItemInt(hwnd, IDC_EDIT_ZHUYIN_INDEX, NULL, FALSE);

    if (kmx_zhuyin_match(&engine, wszInput, len, index, &word_match_result))
    {
        wchar_t out[512] = L"\0";
        wchar_t temp[64];
        int i;

        for (i = 0; i < word_match_result.count; i++)
        {
            KMX_ZHUYIN_CANDIDATE candidate = word_match_result.table[i];
            wcsncat(out, candidate.string, candidate.length);
            wsprintf(temp, L"(%d) ", candidate.match_length);
            wcscat(out, temp);
        }

        wcscat(out, L"\r\n");
        if (word_match_result.more_available)
            wcscat(out, L"...");

        SetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_OUTPUT, out);
    }
    else
        SetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_OUTPUT, L"Failed!");
}

static void test_zhuyin_update_cache(HWND hwnd)
{
    wchar_t wszInput[64];
    unsigned short len;

    len = (unsigned short)GetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_INPUT, wszInput, 64);

    if (kmx_zhuyin_update_cache(&engine, wszInput, len))
        SetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_OUTPUT, L"Add phrase successed.");
    else
        SetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_OUTPUT, L"Add phrase failed.");
}

static void test_zhuyin_check_asso(HWND hwnd)
{
    wchar_t wszInput[64];
    int len;

    len = GetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_INPUT, wszInput, 64);
    if (len > 0)
    {
        int i, n;
        wchar_t out[512] = L"\0";
        wchar_t buffer[32 * 5] = L"\0";
        wchar_t char_code = wszInput[len - 1];
        n = kmx_zhuyin_check_association(&engine, char_code, buffer, 32);
        if (n > 0)
        {
            wchar_t *p = buffer;
            for (i = 0; i < n; i++)
            {
                wcscat(out, p);
                wcscat(out, L" ");

                p += wcslen(p) + 1;
            }

            SetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_OUTPUT, out);
        }
        else
            SetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_OUTPUT, L"");
    }
    else
        SetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_OUTPUT, L"");
}
static void test_zhuyin_query_asso(HWND hwnd)
{
    wchar_t wszInput[64];
    int len;

    int index = SendDlgItemMessage(hwnd, IDC_COMBO_ZHUYIN_LDB, CB_GETCURSEL, 0, 0);

    len = GetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_INPUT, wszInput, 64);
    if (len > 0)
    {
        int n;
        int number = 32;
        unsigned short buffer[32];
        wchar_t out[512] = L"\0";
        n = kmx_chinese_query_association(*(zhuyin_ldb_tag[index].ldb), wszInput[len - 1], buffer, number);
        if (n > 0)
        {
            wcsncpy(out, buffer, n);
            SetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_OUTPUT, out);
        }
        else 
            SetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_OUTPUT, L"failed");
    }
    else
        SetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_OUTPUT, L"");
}

static void test_query_zhuyin(HWND hwnd)
{
    wchar_t wszInput[64];
    int len;

    int index = SendDlgItemMessage(hwnd, IDC_COMBO_ZHUYIN_LDB, CB_GETCURSEL, 0, 0);

    len = GetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_INPUT, wszInput, 64);
    if (len > 0)
    {
        int result;
        KMX_CHAR_ZHUYIN zhuyin_buffer;
        wchar_t out[512] = L"\0";
        result = kmx_chinese_query_zhuyin(*(zhuyin_ldb_tag[index].ldb), wszInput[len - 1], &zhuyin_buffer);
        if (result != 0)
        {
            wcsncpy(out, zhuyin_buffer.string, zhuyin_buffer.length);
            SetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_OUTPUT, out);
        }
        else 
            SetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_OUTPUT, L"failed");
    }
    else
        SetDlgItemText(hwnd, IDC_EDIT_ZHUYIN_OUTPUT, L"");
}

static void on_test_zhuyin(HWND hwnd)
{
    int index = SendDlgItemMessage(hwnd, IDC_COMBO_ZHUYIN, CB_GETCURSEL, 0, 0);
    switch (index)
    {
    case 0:
        test_digit_to_zhuyin(hwnd);
        break;
    case 1:
        test_zhuyin_match(hwnd);
        break;
    case 2:
        test_zhuyin_update_cache(hwnd);
        break;
    case 3:
        test_zhuyin_check_asso(hwnd);
        break;
    case 4:
        test_zhuyin_query_asso(hwnd);
        break;
    case 5:
        test_query_zhuyin(hwnd);
        break;
    }

}

static BOOL CALLBACK kmx_zhuyin_dialog_proc(
    HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_INITDIALOG:
            on_init_zhuyin_dialog(hwnd);
            return TRUE;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                    EndDialog(hwnd, 0);
                    return TRUE;
                case IDCANCEL:
                    EndDialog(hwnd, 0);
                    return TRUE;

                case IDC_BTN_ZHUYIN:
                    on_test_zhuyin(hwnd);
                    return TRUE;
                case IDC_COMBO_ZHUYIN_LDB:
                    if (HIWORD(wParam) == CBN_SELCHANGE)
                    {
                        int index = SendDlgItemMessage(hwnd, IDC_COMBO_ZHUYIN_LDB, CB_GETCURSEL, 0, 0);
                        init_zhuyin_engine(index);
                        return TRUE;
                    }
                    return FALSE;
            }
            break;
    }

    return FALSE;
}

void execute_zhuyin_test_dialog(HWND parent)
{
    DialogBox(g_instance, MAKEINTRESOURCE(ID_KMX_ZHUYIN_DIALOG), parent, kmx_zhuyin_dialog_proc);
}

#else
void execute_zhuyin_test_dialog(HWND parent)
{
    return;
}
#endif

/***************************************************************************************
 *
 * Latin test
 *
 ***************************************************************************************/

static KMX_LDB_TAG latin_ldb_tag[] = 
{
#if defined(KMX_LDB_ENGLISH)
    {IM_ENGLISH, L"kmx_ldb_english", &kmx_ldb_english},
#endif
#if defined(KMX_LDB_ENGLISH_SURETYPE)
    {IM_ENGLISH, L"kmx_ldb_english_suertype", &kmx_ldb_english_suretype},
#endif
#if defined(KMX_LDB_ARMENIAN)
    {IM_ARMENIAN, L"kmx_ldb_armenian", &kmx_ldb_armenian},
#endif
#if defined(KMX_LDB_ARABIC)
    {IM_ARABIC, L"kmx_ldb_arabic", &kmx_ldb_arabic},
#endif
#if defined(KMX_LDB_BENGALI)
    {IM_BENGALI, L"kmx_ldb_bengali", &kmx_ldb_bengali},
#endif
#if defined(KMX_LDB_BULGARIAN)
    {IM_BULGARIAN, L"kmx_ldb_bulgarian", &kmx_ldb_bulgarian},
#endif
#if defined(KMX_LDB_CZECH)
    {IM_CZECH, L"kmx_ldb_czech", &kmx_ldb_czech},
#endif
#if defined(KMX_LDB_DANISH)
    {IM_DANISH, L"kmx_ldb_danish", &kmx_ldb_danish},
#endif
#if defined(KMX_LDB_DUTCH)
    {IM_DUTCH, L"kmx_ldb_dutch", &kmx_ldb_dutch},
#endif
#if defined(KMX_LDB_FINNISH)
    {IM_FINNISH, L"kmx_ldb_finnish", &kmx_ldb_finnish},
#endif
#if defined(KMX_LDB_FRENCH)
    {IM_FRENCH, L"kmx_ldb_french", &kmx_ldb_french},
#endif
#if defined(KMX_LDB_GERMAN)
    {IM_GERMAN, L"kmx_ldb_german", &kmx_ldb_german},
#endif
#if defined(KMX_LDB_GREEK)
    {IM_GREEK, L"kmx_ldb_greek", &kmx_ldb_greek},
#endif
#if defined(KMX_LDB_HEBREW)
    {IM_HEBREW, L"kmx_ldb_hebrew", &kmx_ldb_hebrew},
#endif
#if defined(KMX_LDB_HINDI)
    {IM_HINDI, L"kmx_ldb_hindi", &kmx_ldb_hindi},
#endif
#if defined(KMX_LDB_HINGLISH)
    {IM_HINGLISH, L"kmx_ldb_hinglish", &kmx_ldb_hinglish},
#endif
#if defined(KMX_LDB_NEPALI)
    {IM_NEPALI, L"kmx_ldb_nepali", &kmx_ldb_nepali},
#endif
#if defined(KMX_LDB_SINHALA)
    {IM_SINHALA, L"kmx_ldb_sinhala", &kmx_ldb_sinhala},
#endif
//daodejing
#if defined(KMX_LDB_BODO)
    {IM_BODO, L"kmx_ldb_bodo", &kmx_ldb_bodo},
#endif
#if defined(KMX_LDB_DOGRI)
    {IM_DOGRI, L"kmx_ldb_dogri", &kmx_ldb_dogri},
#endif
#if defined(KMX_LDB_SANSKRIT)
    {IM_SANSKRIT, L"kmx_ldb_sanskrit", &kmx_ldb_sanskrit},
#endif
#if defined(KMX_LDB_KASHMIRI)
    {IM_KASHMIRI, L"kmx_ldb_kashmiri", &kmx_ldb_kashmiri},
#endif
#if defined(KMX_LDB_KONKANI)
    {IM_KONKANI, L"kmx_ldb_konkani", &kmx_ldb_konkani},
#endif
#if defined(KMX_LDB_MAITHILI)
    {IM_MAITHILI, L"kmx_ldb_maithili", &kmx_ldb_maithili},
#endif
#if defined(KMX_LDB_MANIPURI)
    {IM_MANIPURI, L"kmx_ldb_manipuri", &kmx_ldb_manipuri},
#endif
#if defined(KMX_LDB_SANTALI)
    {IM_SANTALI, L"kmx_ldb_santali", &kmx_ldb_santali},
#endif
#if defined(KMX_LDB_SOMALI)
    {IM_SOMALI, L"kmx_ldb_somali", &kmx_ldb_somali},
#endif
#if defined(KMX_LDB_SINDHI)
    {IM_SINDHI, L"kmx_ldb_sindhi", &kmx_ldb_sindhi},
#endif
#if defined(KMX_LDB_HUNGARIAN)
    {IM_HUNGARIAN, L"kmx_ldb_hungarian", &kmx_ldb_hungarian},
#endif
#if defined(KMX_LDB_INDONESIAN)
    {IM_INDONESIAN, L"kmx_ldb_indonesian", &kmx_ldb_indonesian},
#endif
#if defined(KMX_LDB_ITALIAN)
    {IM_ITALIAN, L"kmx_ldb_italian", &kmx_ldb_italian},
#endif
#if defined(KMX_LDB_LITHUANIAN)
    {IM_LITHUANIAN, L"kmx_ldb_lithuanian", &kmx_ldb_lithuanian},
#endif
#if defined(KMX_LDB_MALAY)
    {IM_MALAY, L"kmx_ldb_malay", &kmx_ldb_malay},
#endif
#if defined(KMX_LDB_MYANMAR)
    {IM_MALAY, L"kmx_ldb_myanmar", &kmx_ldb_myanmar},
#endif
#if defined(KMX_LDB_NORWEGIAN)
    {IM_NORWEGIAN, L"kmx_ldb_norwegian", &kmx_ldb_norwegian},
#endif
#if defined(KMX_LDB_PERSIAN)
    {IM_PERSIAN, L"kmx_ldb_persian", &kmx_ldb_persian},
#endif
#if defined(KMX_LDB_POLISH)
    {IM_POLISH, L"kmx_ldb_polish", &kmx_ldb_polish},
#endif
#if defined(KMX_LDB_PORTUGUESE)
    {IM_PORTUGUESE, L"kmx_ldb_portuguese", &kmx_ldb_portuguese},
#endif
#if defined(KMX_LDB_ROMANIAN)
    {IM_ROMANIAN, L"kmx_ldb_romanian", &kmx_ldb_romanian},
#endif
#if defined(KMX_LDB_RUSSIAN)
    {IM_RUSSIAN, L"kmx_ldb_russian", &kmx_ldb_russian},
#endif
#if defined(KMX_LDB_SLOVAK)
    {IM_SLOVAK, L"kmx_ldb_slovak", &kmx_ldb_slovak},
#endif
#if defined(KMX_LDB_SLOVENIAN)
    {IM_SLOVENIAN, L"kmx_ldb_slovenian", &kmx_ldb_slovenian},
#endif
#if defined(KMX_LDB_SPANISH)
    {IM_SPANISH, L"kmx_ldb_spanish", &kmx_ldb_spanish},
#endif
#if defined(KMX_LDB_SWAHILI)
    {IM_SWAHILI, L"kmx_ldb_swahili", &kmx_ldb_swahili},
#endif
#if defined(KMX_LDB_SWEDISH)
    {IM_SWEDISH, L"kmx_ldb_swedish", &kmx_ldb_swedish},
#endif
#if defined(KMX_LDB_THAI)
    {IM_THAI, L"kmx_ldb_thai", &kmx_ldb_thai},
#endif
#if defined(KMX_LDB_TURKISH)
    {IM_TURKISH, L"kmx_ldb_turkish", &kmx_ldb_turkish},
#endif
#if defined(KMX_LDB_UKRAINIAN)
    {IM_UKRAINIAN, L"kmx_ldb_ukrainian", &kmx_ldb_ukrainian},
#endif
#if defined(KMX_LDB_VIETNAMESE)
    {IM_VIETNAMESE, L"kmx_ldb_vietnamese", &kmx_ldb_vietnamese},
#endif
#if defined(KMX_LDB_URDU)
    {IM_URDU, L"kmx_ldb_urdu", &kmx_ldb_urdu},
#endif
    0
};

static BOOL init_latin_engine(int index)
{
    const void *ldb;
    IIME_USER_PARAMS user_params_total, user_params_english;

    KMX_IME_ENGINE_INIT init_info = {0};


    ldb = *(latin_ldb_tag[index].ldb);

    user_params_total.pUserCache = gui_load_ime_userdata(&user_params_total.iUserCacheSize);
    if (FindUserCacheByIM(latin_ldb_tag[index].id, &user_params_total, &user_params_english))
    {
        init_info.user_data = user_params_english.pUserCache;
        init_info.user_data_size = user_params_english.iUserCacheSize;
    }
    else
    {
        init_info.user_data = 0;
        init_info.user_data_size = 0;
    }

    init_info.database = ldb;
    init_info.flags = 0;

    if (kmx_latin_initialize(&engine, &init_info))
        return TRUE;
    else
        return FALSE;

}

static void on_init_latin_dialog(HWND hwnd)
{
    int i;
    HWND hwndCombo;

    for (i = 0; latin_ldb_tag[i].id; i++)
    {
        const wchar_t *str;
        hwndCombo = GetDlgItem(hwnd, IDC_COMBO_LATIN_LDB);
        str = latin_ldb_tag[i].name;
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)str);
    }
    SendDlgItemMessage(hwnd, IDC_COMBO_LATIN_LDB, CB_SETCURSEL, 0, 0);

    if (init_latin_engine(0))
    {
        /* init IDC_COMBO_PINYIN combo */
        hwndCombo = GetDlgItem(hwnd, IDC_COMBO_LATIN);
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_latin_match");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_latin_query_association");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_latin_update_cache");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_latin_query_key_map");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_latin_query_multitap");
        SendDlgItemMessage(hwnd, IDC_COMBO_LATIN, CB_SETCURSEL, 0, 0);

        hwndCombo = GetDlgItem(hwnd, IDC_COMBO_LATIN_FLAGS);
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"KMX_LATIN_LOWER_CASE");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"KMX_LATIN_UPPER_CASE");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"KMX_LATIN_INITIAL_CASE");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"KMX_LATIN_NUMBER_FIRST");
        SendDlgItemMessage(hwnd, IDC_COMBO_LATIN_FLAGS, CB_SETCURSEL, 0, 0);

    }
}

static void test_latin_match(HWND hwnd)
{
    int nLen, iFlags, result;
    wchar_t wszInput[KMX_MAX_LATIN_WORD_LENGTH];
    char input_string[KMX_MAX_LATIN_WORD_LENGTH];
    wchar_t wszContext[KMX_MAX_LATIN_WORD_LENGTH];

    KMX_INPUT_CONTEXT input_context;

    unsigned short input_string_length;
    unsigned short match_flags = 0;
    unsigned short match_index;
    KMX_LATIN_MATCH_RESULT match_result;

    nLen = GetDlgItemText(hwnd, IDC_EDIT_LATIN_INPUT, wszInput, KMX_MAX_LATIN_WORD_LENGTH);
    if (nLen == 0)
    {
        SetDlgItemText(hwnd, IDC_EDIT_LATIN_OUTPUT, L"");
        return;
    }

    WideCharToMultiByte(CP_ACP, 0, wszInput, -1, input_string, nLen, NULL, NULL);
    input_string_length = (unsigned short)nLen;

    iFlags = SendDlgItemMessage(hwnd, IDC_COMBO_LATIN_FLAGS, CB_GETCURSEL, 0, 0);
    switch (iFlags)
    {
    case 0:
        match_flags = KMX_LATIN_LOWER_CASE;
        break;
    case 1:
        match_flags = KMX_LATIN_UPPER_CASE;
        break;
    case 2:
        match_flags = KMX_LATIN_INITIAL_CASE;
        break;
    case 3:
        match_flags = KMX_LATIN_NUMBER_FIRST;
        break;
    }

    match_index = (unsigned short)GetDlgItemInt(hwnd, IDC_EDIT_LATIN_INDEX, NULL, FALSE);

    nLen = GetDlgItemText(hwnd, IDC_EDIT_LATIN_CONTEXT, wszContext, KMX_MAX_LATIN_WORD_LENGTH);
    if (nLen == 0)
    {
        memset(&input_context, 0, sizeof(KMX_INPUT_CONTEXT));
    }
    else
    {
        input_context.text_before = wszContext;
        input_context.text_before_length = (unsigned short)nLen;
        input_context.text_after = NULL;
        input_context.text_after_length = 0;
    }

    result = kmx_latin_match(&engine, &input_context, input_string, input_string_length, match_flags, match_index, &match_result);
    if (result)
    {
        int i;
        wchar_t out[512] = L"\0";
        for (i = 0; i < match_result.count; i++)
        {
            KMX_LATIN_CANDIDATE cand = match_result.table[i];
            wcsncat(out, cand.string, cand.length);
            wcscat(out, L"\r\n");
        }

        if (match_result.more_available)
            wcscat(out, L"...");

        SetDlgItemText(hwnd, IDC_EDIT_LATIN_OUTPUT, out);
    }
    else
        SetDlgItemText(hwnd, IDC_EDIT_LATIN_OUTPUT, L"Match failed.");
}

static void test_latin_query_asso(HWND hwnd)
{
    int result, iFlags;

    wchar_t input_string[KMX_MAX_LATIN_WORD_LENGTH];

    unsigned short nLen;
    unsigned short match_flags = 0;
    unsigned short match_index;
    KMX_LATIN_MATCH_RESULT match_result;

    nLen = (unsigned short)GetDlgItemText(hwnd, IDC_EDIT_LATIN_INPUT, input_string, KMX_MAX_LATIN_WORD_LENGTH);
    if (nLen == 0)
    {
        SetDlgItemText(hwnd, IDC_EDIT_LATIN_OUTPUT, L"");
        return;
    }

    iFlags = SendDlgItemMessage(hwnd, IDC_COMBO_LATIN_FLAGS, CB_GETCURSEL, 0, 0);
    switch (iFlags)
    {
    case 0:
        match_flags = KMX_LATIN_LOWER_CASE;
        break;
    case 1:
        match_flags = KMX_LATIN_UPPER_CASE;
        break;
    case 2:
        match_flags = KMX_LATIN_INITIAL_CASE;
        break;
    case 3:
        match_flags = KMX_LATIN_NUMBER_FIRST;
        break;
    }

    match_index = (unsigned short)GetDlgItemInt(hwnd, IDC_EDIT_LATIN_INDEX, NULL, FALSE);

    result = kmx_latin_query_association(&engine, input_string, nLen, match_flags, match_index, &match_result);
    if (result)
    {
        int i;
        wchar_t out[512] = L"\0";
        for (i = 0; i < match_result.count; i++)
        {
            KMX_LATIN_CANDIDATE cand = match_result.table[i];
            wcsncat(out, cand.string, cand.length);
            wcscat(out, L"\r\n");
        }

        if (match_result.more_available)
            wcscat(out, L"...");

        SetDlgItemText(hwnd, IDC_EDIT_LATIN_OUTPUT, out);
    }
    else
        SetDlgItemText(hwnd, IDC_EDIT_LATIN_OUTPUT, L"Query association failed.");
}
static void test_latin_update_cache(HWND hwnd)
{
    int result;
    unsigned short nLen;
    wchar_t input_string[KMX_MAX_LATIN_WORD_LENGTH];

    nLen = (unsigned short)GetDlgItemText(hwnd, IDC_EDIT_LATIN_INPUT, input_string, KMX_MAX_LATIN_WORD_LENGTH);
    if (nLen == 0)
    {
        SetDlgItemText(hwnd, IDC_EDIT_LATIN_OUTPUT, L"");
        return;
    }

    result = kmx_latin_update_cache(&engine, input_string, nLen);
    if (result)
    {
        SetDlgItemText(hwnd, IDC_EDIT_LATIN_OUTPUT, L"Update cache successed.");
    }
    else
    {
        SetDlgItemText(hwnd, IDC_EDIT_LATIN_OUTPUT, L"Update cache failed.");
    }
}
static void test_latin_query_key_map(HWND hwnd)
{
    int nLen, result, index;
    wchar_t input_string[KMX_MAX_LATIN_WORD_LENGTH];
    wchar_t key_code[2];

    nLen = GetDlgItemText(hwnd, IDC_EDIT_LATIN_INPUT, input_string, KMX_MAX_LATIN_WORD_LENGTH);
    if (nLen != 1)
    {
        SetDlgItemText(hwnd, IDC_EDIT_LATIN_OUTPUT, L"");
        return;
    }

    index = SendDlgItemMessage(hwnd, IDC_COMBO_LATIN_LDB, CB_GETCURSEL, 0, 0);
    result = kmx_latin_query_key_map(*(latin_ldb_tag[index].ldb), input_string[0], key_code);
    if (result)
    {
        key_code[0] = (wchar_t)(key_code[0] + '0');
        key_code[1] = 0;
        SetDlgItemText(hwnd, IDC_EDIT_LATIN_OUTPUT, key_code);
    }
    else
    {
        SetDlgItemText(hwnd, IDC_EDIT_LATIN_OUTPUT, L"Query key map failed.");
    }
}
static void test_latin_query_multitap(HWND hwnd)
{
    int nLen, result, iFlags;
    wchar_t input_string[KMX_MAX_LATIN_WORD_LENGTH];
    const unsigned short *multitype_string;
    int index;

    unsigned short key;
    unsigned short match_flags = 0;
    unsigned short length;

    nLen = GetDlgItemText(hwnd, IDC_EDIT_LATIN_INPUT, input_string, KMX_MAX_LATIN_WORD_LENGTH);
    if (nLen != 1)
    {
        SetDlgItemText(hwnd, IDC_EDIT_LATIN_OUTPUT, L"");
        return;
    }

    key = (unsigned short)(input_string[0] - '0');

    iFlags = SendDlgItemMessage(hwnd, IDC_COMBO_LATIN_FLAGS, CB_GETCURSEL, 0, 0);
    switch (iFlags)
    {
    case 0:
        match_flags = KMX_LATIN_LOWER_CASE;
        break;
    case 1:
        match_flags = KMX_LATIN_UPPER_CASE;
        break;
    case 2:
        match_flags = KMX_LATIN_INITIAL_CASE;
        break;
    case 3:
        match_flags = KMX_LATIN_NUMBER_FIRST;
        break;
    }

    index = SendDlgItemMessage(hwnd, IDC_COMBO_LATIN_LDB, CB_GETCURSEL, 0, 0);
    result = kmx_latin_query_multitap(*(latin_ldb_tag[index].ldb), key, match_flags, &multitype_string, &length);
    if (result)
    {
        wchar_t out[512] = L"\0";
        wcsncpy(out, multitype_string, length);
        SetDlgItemText(hwnd, IDC_EDIT_LATIN_OUTPUT, out);
    }
    else
        SetDlgItemText(hwnd, IDC_EDIT_LATIN_OUTPUT, L"Query key map failed.");
}

static void on_test_latin(HWND hwnd)
{
    int index = SendDlgItemMessage(hwnd, IDC_COMBO_LATIN, CB_GETCURSEL, 0, 0);
    switch (index)
    {
    case 0:
        test_latin_match(hwnd);
        break;
    case 1:
        test_latin_query_asso(hwnd);
        break;
    case 2:
        test_latin_update_cache(hwnd);
        break;
    case 3:
        test_latin_query_key_map(hwnd);
        break;
    case 4:
        test_latin_query_multitap(hwnd);
        break;
    }
}

static BOOL CALLBACK kmx_latin_dialog_proc(
    HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_INITDIALOG:
            on_init_latin_dialog(hwnd);
            return TRUE;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                    EndDialog(hwnd, 0);
                    return TRUE;
                case IDCANCEL:
                    EndDialog(hwnd, 0);
                    return TRUE;

                case IDC_BTN_LATIN:
                    on_test_latin(hwnd);
                    return TRUE;
                case IDC_COMBO_LATIN_LDB:
                    if (HIWORD(wParam) == CBN_SELCHANGE)
                    {
                        int index = SendDlgItemMessage(hwnd, IDC_COMBO_LATIN_LDB, CB_GETCURSEL, 0, 0);
                        init_latin_engine(index);
                        return TRUE;
                    }
                    return FALSE;
            }
            break;
    }

    return FALSE;
}

void execute_latin_test_dialog(HWND parent)
{
    DialogBox(g_instance, MAKEINTRESOURCE(ID_KMX_LATIN_DIALOG), parent, kmx_latin_dialog_proc);;
}

/*********************************************************************************************
 *
 * stroke 
 *
 ********************************************************************************************/

#if defined(KMX_LDB_STROKE_GB2312) || defined(KMX_LDB_STROKE_GBK) || defined(KMX_LDB_STROKE_GB18030) || defined(KMX_LDB_STROKE_BIG5) || defined(KMX_LDB_STROKE_BIG5HKSCS)

KMX_LDB_TAG stroke_ldb_tag[] = 
{
#if defined(KMX_LDB_STROKE_GB2312)
    {IM_STROKE, L"kmx_ldb_stroke_gb2312", &kmx_ldb_stroke_gb2312},
#endif
#if defined(KMX_LDB_STROKE_GBK)
    {IM_STROKE, L"kmx_ldb_stroke_gbk", &kmx_ldb_stroke_gbk},
#endif
#if defined(KMX_LDB_STROKE_GB18030)
    {IM_STROKE, L"kmx_ldb_stroke_gb18030", &kmx_ldb_stroke_gb18030},
#endif
#if defined(KMX_LDB_STROKE_BIG5)
    {IM_STROKE, L"kmx_ldb_stroke_big5", &kmx_ldb_stroke_big5},
#endif
#if defined(KMX_LDB_STROKE_BIG5HKSCS)
    {IM_STROKE, L"kmx_ldb_stroke_big5hkscs", &kmx_ldb_stroke_big5hkscs},
#endif
    {0}
};

static BOOL init_stroke_engine(int index)
{
    IIME_USER_PARAMS user_params_total, user_params_stroke;
    const void *ldb;
    KMX_IME_ENGINE_INIT init_info;

    ldb = *(stroke_ldb_tag[index].ldb);
    user_params_total.pUserCache = gui_load_ime_userdata(&user_params_total.iUserCacheSize);
    if (FindUserCacheByIM(stroke_ldb_tag[index].id, &user_params_total, &user_params_stroke))
    {
        init_info.user_data = user_params_stroke.pUserCache;
        init_info.user_data_size = user_params_stroke.iUserCacheSize;
    }
    else
    {
        init_info.user_data = 0;
        init_info.user_data_size = 0;
    }

    init_info.database = ldb;
    init_info.flags = 0;

    if (kmx_stroke_initialize(&engine, &init_info))
        return TRUE;
    else
        return FALSE;
 }

static void on_init_stroke_dialog(HWND hwnd)
{
    HWND hwndCombo;
    int i;

    for (i = 0; stroke_ldb_tag[i].id; i++)
    {
        const wchar_t *str;
    
        hwndCombo = GetDlgItem(hwnd, IDC_COMBO_STROKE_LDB);
        str = stroke_ldb_tag[i].name;
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)str);
    }
    SendDlgItemMessage(hwnd, IDC_COMBO_STROKE_LDB, CB_SETCURSEL, 0, 0);

    if (init_stroke_engine(0))
    {
        /* init IDC_COMBO_STROKE combo */
        hwndCombo = GetDlgItem(hwnd, IDC_COMBO_STROKE);
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_stroke_match");
        SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCSTR)L"kmx_stroke_update_cache");
        SendDlgItemMessage(hwnd, IDC_COMBO_STROKE, CB_SETCURSEL, 0, 0);
    }
}

static void test_stroke_match(HWND hwnd)
{
    int nLen, status;
    wchar_t wszInput[KMX_MAX_STROKE_INPUT_LENGTH];
    char input_string[KMX_MAX_STROKE_INPUT_LENGTH];

    KMX_STROKE_MATCH_RESULT match_result;
    unsigned short input_string_length, match_index;

    nLen = GetDlgItemText(hwnd, IDC_EDIT_STROKE_INPUT, wszInput, KMX_MAX_STROKE_INPUT_LENGTH);
    if (nLen == 0)
    {
        SetDlgItemText(hwnd, IDC_EDIT_STROKE_OUTPUT, L"");
        return;
    }

    WideCharToMultiByte(CP_ACP, 0, wszInput, -1, input_string, nLen, NULL, NULL);

    input_string_length = (unsigned short)nLen;
    match_index = (unsigned short)GetDlgItemInt(hwnd, IDC_EDIT_STROKE_INDEX, NULL, FALSE);

    status = kmx_stroke_match(&engine, (const char*)input_string, input_string_length, match_index, &match_result);
    if (status != 0)
    {
        int i;
        wchar_t text[512];

        wsprintf(text, L"%d", match_result.count);

        for (i = 0; i < match_result.count; i++)
        {
            wcscat(text, L" ");
            wcscat(text, match_result.table[i].string);
        }

        if (match_result.more_available)
            wcscat(text, L" ...");

        wcscat(text, L"\r\n");

        SetDlgItemText(hwnd, IDC_EDIT_STROKE_OUTPUT, text);
    }
    else
    {
        SetDlgItemText(hwnd, IDC_EDIT_STROKE_OUTPUT, L"Match stroke failed.");
    }
}

static void test_stroke_update_cache(HWND hwnd)
{
    int result;
    unsigned short nLen;
    wchar_t wszInput[KMX_MAX_STROKE_INPUT_LENGTH];

    nLen = (unsigned short)GetDlgItemText(hwnd, IDC_EDIT_STROKE_INPUT, wszInput, KMX_MAX_STROKE_INPUT_LENGTH);
    if (nLen == 0)
    {
        SetDlgItemText(hwnd, IDC_EDIT_STROKE_OUTPUT, L"");
        return;
    }

    result = kmx_stroke_update_cache(&engine, wszInput, nLen);
    if (result)
    {
        SetDlgItemText(hwnd, IDC_EDIT_STROKE_OUTPUT, L"Update stroke cache successed.");
    }
    else
    {
        SetDlgItemText(hwnd, IDC_EDIT_STROKE_OUTPUT, L"Update stroke cache failed.");
    }
}

static void on_test_stroke(HWND hwnd)
{
    int index = SendDlgItemMessage(hwnd, IDC_COMBO_STROKE, CB_GETCURSEL, 0, 0);
    switch (index)
    {
    case 0:
        test_stroke_match(hwnd);
        break;
    case 1:
        test_stroke_update_cache(hwnd);
        break;
    }
}

static BOOL CALLBACK kmx_stroke_dialog_proc(
    HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_INITDIALOG:
            on_init_stroke_dialog(hwnd);
            return TRUE;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                    EndDialog(hwnd, 0);
                    return TRUE;
                case IDCANCEL:
                    EndDialog(hwnd, 0);
                    return TRUE;

                case IDC_BTN_STROKE:
                    on_test_stroke(hwnd);
                    return TRUE;
                case IDC_COMBO_STROKE_LDB:
                    if (HIWORD(wParam) == CBN_SELCHANGE)
                    {
                        int index = SendDlgItemMessage(hwnd, IDC_COMBO_STROKE_LDB, CB_GETCURSEL, 0, 0);
                        init_stroke_engine(index);
                        return TRUE;
                    }
                    return FALSE;
            }
            break;
    }

    return FALSE;
}

void execute_stroke_test_dialog(HWND parent)
{
    DialogBox(g_instance, MAKEINTRESOURCE(ID_KMX_STROKE_DIALOG), parent, kmx_stroke_dialog_proc);
}

#else
void execute_stroke_test_dialog(HWND parent)
{
    return;
}
#endif
# if defined(INTERFACE_08_ONLY)
#define MIN_CACHE_BLOCK	(2* 1024)

static int SpliteCache(void)
{
    IIME_BOOL bHasPinyin, bHasZhuyin;
    int i, nBlock;

    nBlock = 0;
    i = 0;
    bHasPinyin = 0;
    bHasZhuyin = 0;
    while(ilME_input_method_config[i])
    {
        switch (ilME_input_method_config[i]->input_method)
        {
        case IM_PINYIN_DZ:
            break;
        case IM_PINYIN:
        case IM_PINYIN_SURETYPE:
        case IM_PINYIN_FULLKEY:
            if (!bHasPinyin)
            {
                nBlock += 2;
                bHasPinyin = 1;
            }
            break;
        case IM_ZHUYIN_MT:
        case IM_ZHUYIN_FULLKEY:
            if (!bHasZhuyin)
            {
                nBlock += 2;
                bHasZhuyin = 1;
            }
            break;
        default:
           nBlock += 1;
        }
        i++;
    }

    return nBlock;
}

int FindUserCacheByIM(int iMethod, const IIME_USER_PARAMS *pUserTotal, IIME_USER_PARAMS *pUserIM)
{
    IIME_BOOL bHasPinyin, bHasZhuyin;
    int iBlockPinyin, iBlockZhuyin;

    int i, iBlock, nBlock, szBlock;

    pUserIM->iUserCacheSize = 0;
    pUserIM->pUserCache = 0;

    nBlock = SpliteCache();
    if (nBlock)
        szBlock = (pUserTotal->iUserCacheSize / nBlock) / MIN_CACHE_BLOCK * MIN_CACHE_BLOCK;
    else
        szBlock = 0;

    if (szBlock == 0)
        return 0;

//#ifdef _MSC_VER
//    wsprintf(out, L"iMethod = %d, nBlock = %d, szBlock = %d\n", iMethod, nBlock, szBlock);
//    OutputDebugString(out);
//#endif

    i = 0;
    iBlock = 0;
    bHasPinyin = 0;
    bHasZhuyin = 0;
    iBlockPinyin = -1;
    iBlockZhuyin = -1;

    if (iMethod == IM_PINYIN_SURETYPE || iMethod == IM_PINYIN_FULLKEY)
        iMethod = IM_PINYIN;
    if (iMethod == IM_ZHUYIN_FULLKEY)
        iMethod = IM_ZHUYIN_MT;

    while(ilME_input_method_config[i])
    {
        int orgBlock = iBlock;
//#ifdef _MSC_VER
//        wsprintf(out, L"ilME_input_method_config[%d]->input_method = %d\n", i, ilME_input_method_config[i]->input_method);
//        OutputDebugString(out);
//#endif
        switch (ilME_input_method_config[i]->input_method)
        {
        case IM_PINYIN_DZ:
            break;
        case IM_PINYIN:
        case IM_PINYIN_SURETYPE:
        case IM_PINYIN_FULLKEY:
            if (!bHasPinyin)
            {
                iBlockPinyin = iBlock;
                iBlock += 2;
                bHasPinyin = 1;
            }
            break;
        case IM_ZHUYIN_MT:
        case IM_ZHUYIN_FULLKEY:
            if (!bHasZhuyin)
            {
                iBlockZhuyin = iBlock;
                iBlock += 2;
                bHasZhuyin = 1;
            }
            break;
        default:
           iBlock += 1;
        }

        if (iMethod == ilME_input_method_config[i]->input_method)
        {
            iBlock = orgBlock;
            break;
        }

        i++;
    }

    switch(iMethod)
    {
    case IM_PINYIN_DZ:
        pUserIM->pUserCache = 0;
        pUserIM->iUserCacheSize = 0;
    case IM_PINYIN:
    case IM_PINYIN_SURETYPE:
    case IM_PINYIN_FULLKEY:
        pUserIM->pUserCache = pUserTotal->pUserCache + iBlockPinyin * szBlock;
        pUserIM->iUserCacheSize = 2 * szBlock;
        break;
    case IM_ZHUYIN:
    case IM_ZHUYIN_MT:
    case IM_ZHUYIN_FULLKEY:
        pUserIM->pUserCache = pUserTotal->pUserCache + iBlockZhuyin * szBlock;
        pUserIM->iUserCacheSize = 2 * szBlock;
        break;

    default:
        pUserIM->pUserCache = pUserTotal->pUserCache + iBlock * szBlock;
        pUserIM->iUserCacheSize = szBlock;
    }
//#ifdef _MSC_VER
//    wsprintf(out, L"iBlock = %d, iBlockPinyin = %d, iBlockZhuyin = %d\n", iBlock, iBlockPinyin, iBlockZhuyin);
//    OutputDebugString(out);
//    wsprintf(out, L"pCache = 0x%08x, szCache = 0x%08x\n", pUserIM->pUserCache, pUserIM->iUserCacheSize);
//    OutputDebugString(out);
//    wsprintf(out, L"pTotalCache = 0x%08x, szTotalCache = 0x%08x\n", pUserTotal->pUserCache, pUserTotal->iUserCacheSize);
//    OutputDebugString(out);
//#endif
    return 1;
}
#endif
