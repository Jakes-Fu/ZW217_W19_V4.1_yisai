#ifdef HERO_ENGINE_HMGS_SUPPORT

#ifndef __HCCALL_FILE_ADAPTER_H__
#define __HCCALL_FILE_ADAPTER_H__

#include "HcDataType.h"


#define PO_RDONLY				0//Open for read only, FS_READ_ONLY
#define PO_WRONLY				1//Opens an empty file for writing. If the given file exists, its contents are destroyed.
#define PO_RDWR					2//Opens for writing and Reading; creates the file first if it doesn�t exist.    

#define PSEEK_SET				0// offset from begining of file
#define PSEEK_CUR				1// offset from current file pointer
#define PSEEK_END				2// offset from end of file

#define TIHO_MOBILENUMBER_PATH "hmgs4\\"
#define TIHO_MOBILENUMBER_DIR  "hmgs4"
//#define TOO_MODE

///////////////////////////////////////////
//// �����ʵ����Ŀ�������Ӧ�Ŀ��� /////
///////////////////////////////////////////
// ����汾���ÿ���
//#define FIRM_HMGS_MIN_ROM

// ȫ�����ÿ���
//#define FIRM_HMGS_FULL_ROM

// ���ذ汾����
//#define FIRM_HMGS_TCARD
///////////////////////////////////////////



typedef  unsigned short     uint16;      //�з���16bit����
typedef  unsigned char      uint8;        //�޷���8bit����
typedef  signed char          int8;        //�з���8bit����
typedef  signed short         int16;       //�з���16bit����
#ifndef NULL
#define NULL  0
#endif
#ifndef FALSE
#define FALSE       0
#endif
#ifndef TRUE
#define TRUE        1
#endif

#ifndef wchar
typedef uint16 wchar;
#endif

typedef unsigned char		BOOLEAN;

#ifndef BOOL
#define BOOL        BOOLEAN
#endif

#ifndef HR_SUCCESS
#define HR_SUCCESS  0    //�ɹ�
#endif

#ifndef HR_FAILED
#define HR_FAILED   -1    //ʧ��
#endif

//int hr_str_wstrlen(char * txt);

//proting
void hmgs_read_file(char * filename,void* buffer,unsigned long int* size );
void * hmgs_malloc(int s);
void hmgs_free(void *address);

// [inkleak.zhao] log ����
#ifndef HERO_INCOMECALL_APP_LOGOPEN
//#define HERO_INCOMECALL_APP_LOGOPEN
#endif
void hccall_printf(const char *format, ...);

 //���
int hmgs_init(void);
void get_hmgs_buff(char* number,unsigned char* strbuf);

void hmgs_deinit(void);
int GetLocation_ex( char *area, int size, char *tel );

#endif //__HCCALL_FILE_ADAPTER_H__
#endif // FEATURE_SUPPORT_HERO_HCCALL
