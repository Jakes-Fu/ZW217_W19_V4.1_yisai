
#include "IIMEk2api.h"
#include "emu_option.h"

#include "update_data.h"

#define MAX_UPDATE_FILE_SIZE (10 * 1024)

static unsigned char buf_update[10 * 1024];
static DWORD buf_size = 0;

int Read_Update_Data(unsigned char* buf, int offset, int len)
{
//    offset += 4;
    if (buf_size && offset + len <= (int)buf_size)
    {
        memcpy(buf, buf_update + offset, len);

        return 1;
    }
    else
        return 0;
}

void exec_load_update_data(HWND parent)
{
    OPENFILENAME ofn;
    TCHAR szFile[260];
    HANDLE hf = 0;

    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = parent;
    ofn.lpstrFile = szFile;

    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile) / sizeof(TCHAR);
    ofn.lpstrFilter = L"Bin\0*.bin\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE)
    {
        DWORD BytesRead;
        hf = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hf == INVALID_HANDLE_VALUE)
            return;

        buf_size = GetFileSize(hf, NULL);
        if (buf_size == INVALID_FILE_SIZE || buf_size > MAX_UPDATE_FILE_SIZE)
        {
            buf_size = 0;
            return;
        }

        if (!ReadFile(hf, buf_update, buf_size, &BytesRead, NULL))
        {
            buf_size = 0;
        }

    }

    CloseHandle(hf);
}


