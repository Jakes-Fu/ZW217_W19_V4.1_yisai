
/*
 * userdata.c
 *
 * Copyright (C) 2006 Beijing Chinese Star Cyber Technology Limited.
 * All rights reserved.
 */

#include "userdata.h"
#include "IIMEk2api.h"
#include "emu_option.h"

#define USERDATA_FILE       L"userdata.dat"
#define IMEOPTION_FILE      L"imeoption.dat"

#define USERDATA_SIZE       (128 * 1024)
#define IMEOPTION_SIZE      (sizeof(CS_IMM_OPTIONS))

static WCHAR data_file_path[MAX_PATH];

static int userdata_buf[USERDATA_SIZE / 4];
static int imeoption_buf[IMEOPTION_SIZE / 4];

void* userdata = userdata_buf;
int userdata_size = USERDATA_SIZE;

void* imeoptiondata = imeoption_buf;
int imeoptiondata_size = IMEOPTION_SIZE;

static void get_app_path(WCHAR* s)
{
    int n;
    
    n = GetModuleFileName(0, s, MAX_PATH);
    while ( (n > 0) && (s[n-1] != L'\\') )
        n--;

    s[n] = 0;
}

static BOOL is_file_exist(const WCHAR* file_name)
{
    HANDLE hFind;
    WIN32_FIND_DATA fd;

    hFind = FindFirstFile(file_name, &fd);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    else
    {
        FindClose(hFind);
        return TRUE;
    }
}

void userdata_init(void)
{
    DWORD temp;
    HANDLE hFile;
    int file_size;
    WCHAR file_name[MAX_PATH];
    CS_IMM_OPTIONS* opt;

    memset(userdata_buf, 0, sizeof(userdata_buf));
    memset(imeoption_buf, 0, sizeof(imeoption_buf));
    opt = (CS_IMM_OPTIONS*)imeoption_buf;
    opt->dwPYFuzzy = 0;
    opt->dwStrokeLayout = CS_STROKE_STANDARD_LAYOUT;
    opt->dwAssoPinyin = CS_ASSOCIATION_ON;
    opt->dwAssoStroke = CS_ASSOCIATION_ON;
    opt->dwPinyinMode = CS_PINYIN_MODE_1;
    opt->dwCandidateMode = CS_CANDIDATE_MODE_1;
    opt->dwMarScript = 1;

    get_app_path(data_file_path);

    lstrcpy(file_name, data_file_path);
    lstrcat(file_name, USERDATA_FILE);
    if (is_file_exist(file_name))
    {
        hFile = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE)
        {
            file_size = GetFileSize(hFile, NULL);
            if (file_size >= userdata_size)
                ReadFile(hFile, userdata_buf, userdata_size, &temp, NULL);
            CloseHandle(hFile);
        }
    }

    lstrcpy(file_name, data_file_path);
    lstrcat(file_name, IMEOPTION_FILE);
    if (is_file_exist(file_name))
    {
        hFile = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE)
        {
            file_size = GetFileSize(hFile, NULL);
            if (file_size >= imeoptiondata_size)
                ReadFile(hFile, imeoption_buf, imeoptiondata_size, &temp, NULL);
            CloseHandle(hFile);
        }
    }

    if (opt->dwStrokeLayout == CS_STROKE_STANDARD_LAYOUT)
        g_stroke_layout = 1;
    else
        g_stroke_layout = 2;

    if (opt->dwPinyinMode == CS_PINYIN_MODE_1)
        g_pinyin_mode = 0;
    else if (opt->dwPinyinMode == CS_PINYIN_MODE_2)
        g_pinyin_mode = 1;
    else if (opt->dwPinyinMode == CS_PINYIN_MODE_3)
        g_pinyin_mode = 2;
    else if (opt->dwPinyinMode == CS_PINYIN_MODE_4)
        g_pinyin_mode = 3;
    else if (opt->dwPinyinMode == CS_PINYIN_MODE_5)
        g_pinyin_mode = 4;
    
    if (opt->dwCandidateMode == CS_CANDIDATE_MODE_1)
        g_candidate_mode = 1;
    else
        g_candidate_mode = 2;
}

void userdata_save(void)
{
    DWORD temp;
    HANDLE hFile;
    WCHAR file_name[MAX_PATH];

    lstrcpy(file_name, data_file_path);
    lstrcat(file_name, USERDATA_FILE);
    hFile = CreateFile(file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        WriteFile(hFile, userdata_buf, userdata_size, &temp, NULL);
        CloseHandle(hFile);
    }

    lstrcpy(file_name, data_file_path);
    lstrcat(file_name, IMEOPTION_FILE);
    hFile = CreateFile(file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        WriteFile(hFile, imeoption_buf, imeoptiondata_size, &temp, NULL);
        CloseHandle(hFile);
    }
}
