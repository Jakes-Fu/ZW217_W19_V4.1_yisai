#include "sci_types.h"
#include "os_api.h"
#include "sio.h"
#include "sfs.h"

#include "sprd_fts_type.h"
#include "sprd_fts_log.h"

#include "chnl.h"
#include "chnl_sd.h"

#define DEVICE_NAME_MAX_LENGTH  2
#define PATH_PATTER_MAX_LENGTH 32
#define FILE_NAME_MAX_LENGTH 32
#define FILE_FULL_PATH   128

typedef struct _DUMP_TYPE_PATTERN
{
    int type;
    uint16 path_name_pattern[PATH_PATTER_MAX_LENGTH];
    uint16 file_name[FILE_NAME_MAX_LENGTH];
}DUMP_TYPE_PATTERN;

LOCAL DUMP_TYPE_PATTERN s_dump_path[] = {
    {CP_DUMP_LOG, {'E', ':', '\\', 'm', 'o','d','e','m', '_', 'l', 'o', 'g', '\\', '*', '_', 'd', 'u', 'm', 'p', 0}, {'c', 'p', '_', '5', 'm', 'o', 'd', 'e', '.', 'l', 'o', 'g', 0}},
    {CP_DUMP_MEMORY, {'E', ':', '\\', 'm', 'o','d','e','m', '_', 'l', 'o', 'g', '\\', '*', '_', 'd', 'u', 'm', 'p', 0}, {'c', 'p', '_', '5', 'm', 'o', 'd', 'e', '_', 'm', 'e', 'm', 'o', 'r', 'y', '.', 'd', 'm', 'p', 0}},
    {CP_DUMP_MINIDUMP, {'E', ':', '\\', 'm', 'o','d','e','m', '_', 'l', 'o', 'g', '\\', '*', '_', 'd', 'u', 'm', 'p', 0}, {'m', 'i', 'n', 'i', 'd', 'u', 'm', 'p', '.', 'b', 'i', 'n', 0}},
    {CP_DUMP_SBLOCK, {'E', ':', '\\', 'm', 'o','d','e','m', '_', 'l', 'o', 'g', '\\', '*', '_', 'd', 'u', 'm', 'p', 0}, {'s', 'b', 'l', 'o', 'c', 'k',  '.', 'l', 'o', 'g', 0}},
    {CP_DUMP_SBUF, {'E', ':', '\\', 'm', 'o','d','e','m', '_', 'l', 'o', 'g', '\\', '*', '_', 'd', 'u', 'm', 'p', 0}, {'s', 'b', 'u', 'f', '.', 'l', 'o', 'g', 0}},
    {CP_DUMP_SMSG, {'E', ':', '\\', 'm', 'o','d','e','m', '_', 'l', 'o', 'g', '\\', '*', '_', 'd', 'u', 'm', 'p', 0}, {'s', 'm', 's', 'g', '.', 'l', 'o', 'g', 0}}
};

LOCAL uint16 tmp_flag[PATH_PATTER_MAX_LENGTH] = {'_', 'd', 'u', 'm', 'p', '\\', 0};
LOCAL uint16 root_dir[PATH_PATTER_MAX_LENGTH] = {'E', ':', '\\', 'm', 'o','d','e','m', '_', 'l', 'o', 'g', '\\', 0};
LOCAL uint16 device_name[DEVICE_NAME_MAX_LENGTH]  = {'E', 0};
LOCAL uint16 modem_dump_file_name[FILE_FULL_PATH] = {'E', ':', '\\', 'm', 'o','d','e','m', '_', 'l', 'o', 'g', '\\', 'c', 'p', '\\', 'c', 'p', '_', '5', 'm', 'o', 'd', 'e', '_', 'm', 'e', 'm', 'o', 'r', 'y', '.', 'd', 'm', 'p', 0};
//LOCAL uint16 modem_dump_file_name[FILE_FULL_PATH] = {'E',':', '\\', 'u', 'l', 'o', 'g', '\\', 'c','p' ,'.', 'l', 'o', 'g', 0};

#define SFS_HANDLE_INVALIDATE 0
LOCAL SFS_HANDLE s_dumpFile_fd = SFS_HANDLE_INVALIDATE;

LOCAL SCI_TIMER_PTR s_check_timer = NULL;
LOCAL uint32 s_check_bytes_writen = 0;
LOCAL BOOLEAN s_check_finished = TRUE;

PUBLIC void chnl_dump_monitor();

static void NumStrToUCS2(uint8 *num_str, uint16 num_len, uint8 *ucs2_str)
{
    uint16   i = 0;

    if (PNULL == num_str || PNULL == ucs2_str) return;

    for( i = 0; i < num_len; i++ )
    {
        *(uint16 *)&ucs2_str[2*i] = num_str[i];
    }
}

static void NumUCS2ToStr(uint8 *ucs2_str,uint16 num_len, uint8 *num_str )
{
    uint16   i = 0;

    if (PNULL == num_str || PNULL == ucs2_str) return;

    for( i = 0; i < num_len; i++ )
    {
         num_str[i]=*(uint16 *)&ucs2_str[2*i] ;
    }
}

static int getNewFileNo(uint16* full_path){
    SFS_FIND_DATA_T find_data = {0};
    int max=-1;

    SFS_HANDLE f_handle = SFS_FindFirstFile((uint16*)full_path, &find_data);
    if (f_handle != NULL){
        do{
            char str[16]={0};
            NumUCS2ToStr(find_data.name,2,str);
            int no=atoi(str);
            if(max<no){
                max=no;
            }
        }while (SFS_NO_ERROR == SFS_FindNextFile(f_handle, &find_data));
     }else{
        ENG_LOG("ulog getNewFileNo faild");
     }
     
     if (f_handle != NULL){
        SFS_FindClose(f_handle);
     }

      return max+1;

}

static void TraceUCS2String(uint16* str){
    char buff[64]={0};
    int num_len=unistrlength(str);
    int i=0;
	 for( i = 0; i < num_len; i++ ){
            buff[i]=((char*)str)[2*i] ;
    }
     ENG_LOG("UCS:%s",buff);
}

static void getFileName(int type, uint16* filename ){
        uint16 fullpath[FILE_NAME_MAX_LENGTH]={'0'};
        char idbuff[8]={0};
        uint16 idbuff2[8]    ={0};

        if(type < CP_DUMP_LOG || type >= CP_DUM_TOTAL){
            ENG_LOG("%s: error type %d", __FUNCTION__, type);
            return ;
        }

        int no=getNewFileNo(s_dump_path[type].path_name_pattern);
        sprintf(idbuff,"%02d",no);
        NumStrToUCS2(idbuff,strlen(idbuff),idbuff2);
        
        unistrcpy(fullpath, root_dir);
        unistrcat(fullpath,idbuff2);
        unistrcat(fullpath, tmp_flag);
        unistrcat(fullpath,s_dump_path[type].file_name);
        unistrcpy(filename,fullpath);

}

LOCAL void dumpFileUninit()
{
    if(SFS_HANDLE_INVALIDATE <= s_dumpFile_fd){
        SFS_FlushFile(s_dumpFile_fd,0);
        SFS_CloseFile(s_dumpFile_fd);
        s_dumpFile_fd = SFS_HANDLE_INVALIDATE;
    }
}

LOCAL BOOLEAN dumpFileInit(int type)
{
    uint16 format = 2;
    SFS_ERROR_E  result  = SFS_GetDeviceStatus(device_name);
    ENG_LOG("%s: SFS_GetDeviceStatus return: %d", __FUNCTION__, result);

    do{
        if(SFS_ERROR_HAS_EXIST == result){
            result = SFS_RegisterDevice(device_name, &format);
            if(result !=SFS_ERROR_NONE){
                break;
            }
        }
        result  = SFS_GetDeviceStatus(device_name);
        if(SFS_ERROR_NONE != result){
            break;
        }
    }while(0);

    if(SFS_ERROR_NONE != result){
        ENG_LOG("%s: checkDeviceExist ERR: %d", __FUNCTION__, result);
        return FALSE;
     }

    memset(modem_dump_file_name, 0, sizeof(modem_dump_file_name));
    getFileName(type, &modem_dump_file_name);
    ENG_LOG("%s: file name = %s", __FUNCTION__, modem_dump_file_name);

    s_dumpFile_fd = SFS_CreateFile(modem_dump_file_name, SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, NULL, NULL);
    if(SFS_HANDLE_INVALIDATE >= s_dumpFile_fd){
        ENG_LOG("%s: create cp dump file fail!!", __FUNCTION__);
        return FALSE;
    }

    chnl_dump_monitor();

    ENG_LOG("%s: create cp dump file succ!", __FUNCTION__);
    return TRUE;
}

#define DUMP_WRITE_RETRY 60
PUBLIC void chnl_sd_dump2File(int type, uint8* src, int len)
{
    SFS_ERROR_E  result = SFS_NO_ERROR;
    uint32 bytes_to_write = len;
    uint32 bytes_written = 0;
    int retry = DUMP_WRITE_RETRY*5;
    int offset = 0;

    ENG_LOG("%s: len = %d", __FUNCTION__, len);
    if(SFS_HANDLE_INVALIDATE >= s_dumpFile_fd){
        if (!dumpFileInit(type)){
            return ;
        }
    }

    offset = 0;
    do{
        result=SFS_WriteFile(s_dumpFile_fd, src+offset, bytes_to_write, &bytes_written, 0);
        if (SFS_NO_ERROR != result){
            if(retry-- <=0){
                ENG_LOG("%s: error: dump write fail! break", __FUNCTION__);
                break;
            }else{
                SCI_Sleep(500);
                ENG_LOG("%s: error: dump write fail! retry, bytes_written = %d, error = %d",__FUNCTION__, bytes_written, result);
            }
        }else{
            bytes_to_write -= bytes_written;
            offset += bytes_written;
        }

    }while(bytes_to_write > 0);

    s_check_bytes_writen += bytes_written;
    s_check_finished = FALSE;
    ENG_LOG("%s: bytes_written = %d, s_check_bytes_writen = %d", __FUNCTION__, bytes_written, s_check_bytes_writen);

    //SFS_FlushFile(s_dumpFile_fd,0);
    SCI_Sleep(20);

}

#define MAX_DUMP_CP_MEMORY_TIMEOUT (5*1000)
static BLOCK_ID s_engpc_cp_dump_monitor_task = SCI_INVALID_BLOCK_ID;
extern char* engpc_findAssertInfo();
LOCAL void Engpc_CP_DumpMontor( uint32 argc, void* argv )
{
    do{
        SCI_Sleep(MAX_DUMP_CP_MEMORY_TIMEOUT);
        if(s_check_finished){
            ENG_LOG("%s: there is no data written with 5 second, so we think dump memory is finished. s_check_bytes_writen = %d ", __FUNCTION__, s_check_bytes_writen);
            dumpFileUninit();
            SCI_PASSERT(0, ("%s",engpc_findAssertInfo() ));
            break;
        }else{
            s_check_finished = TRUE;
        }
    }while(TRUE);
}

PUBLIC void chnl_dump_monitor()
{
    if( SCI_INVALID_BLOCK_ID == s_engpc_cp_dump_monitor_task )
    {         
        s_engpc_cp_dump_monitor_task = SCI_CreateThread(
            "ENGPC_CP_DUMP_MONITOR_THREAD",
            "ENGPC_CP_DUMP_MONITOR_THREAD",
            Engpc_CP_DumpMontor,
            NULL,
            NULL,
            (1*1024),
            15,
            78,
            SCI_PREEMPT,
            SCI_AUTO_START );
    }
}

