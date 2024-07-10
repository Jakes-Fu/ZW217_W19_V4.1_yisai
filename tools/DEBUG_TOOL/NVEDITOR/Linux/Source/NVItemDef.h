#ifndef _NVITEMDEF_H
#define _NVITEMDEF_H
#include "TCHAR.h"
#include <string>
#include <vector>
using namespace std;

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  \
    do \
{\
	if (p != NULL) \
	{\
		delete p; \
		p=NULL;\
    }\
} while(0) 
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)  \
	do \
{\
	if (p != NULL) {\
	delete []p; \
	p=NULL;\
	}\
} while(0) 
#endif

#ifndef FALSE
#define FALSE                   0
#endif

#ifndef TRUE
#define TRUE                    1
#endif

#ifndef _MAX_PATH
#define _MAX_PATH               512
#endif

#ifndef MAX_PATH
#define MAX_PATH                512
#endif
#define MAX_PATHLENGTH  520

#define MAX_NAME_LENGTH                 (0x200)
#define MAX_ITEM_CONTENT                (0x200)
#define INVALID_MODULE_ID               ((INT)0xFFFFFFFF)
#define INVALID_MODULE_ITEM_INDEX		((INT)0xFFFFFFFF)
#define MAX_TOTAL_ITEM_CONTENT          (0x100000)	// 1M
#define DSP_CODE_SIZE                   (0xC000)
#define MAX_DSP_CODE_SIZE               (64*1024*2)	//128k
#define FILE_TOTAL_SIZE					(0x10000)   //64k

//Define Item type
typedef enum ITEMTYPE_ENUM 
{ 
    NONE_ITEM_TYPE,
    CHAR_ITEM_TYPE  , 
    SHORT_ITEM_TYPE , 
    LONG_ITEM_TYPE = 4 , 
    ARRAY_ITEM_TYPE,
    ASTRING_ITEM_TYPE,
    STRUCT_ITEM_TYPE,
    WSTRING_ITEM_TYPE
}ITEMTYPE;

typedef struct _ST_FILE_INFO
{
    UINT	nFileID; 
    BOOL	bCheckFlag;
    TCHAR	szFilePath[MAX_PATH]; 
    _ST_FILE_INFO()
    {
        memset(this,0,sizeof(_ST_FILE_INFO));	
    }
}ST_FILE_INFO,*PFILE_INFO_ST;

typedef vector<ST_FILE_INFO>  VEC_FILEINFO;
typedef vector<int>            CUIntArray;

typedef struct _ST_NVCmdLine
{
    BOOL    bBigEndian;
    BOOL	bWait;
    BOOL	bTail;
    BOOL	bVersion;
    BOOL	bHeaderFile;
    BOOL  bCheckItemName;
    TCHAR	strErrorFile[MAX_PATHLENGTH];
    TCHAR	strImportFile[MAX_PATHLENGTH];
    TCHAR	strDspBinFile[MAX_PATHLENGTH];    
    TCHAR	strFileName[MAX_PATHLENGTH];
    TCHAR	strMergePath[MAX_PATHLENGTH]; 
    TCHAR	strHeaderFile[MAX_PATHLENGTH];
    TCHAR	strFixBinFile[MAX_PATHLENGTH];
    TCHAR	strHashHeader[MAX_PATHLENGTH];
    int		nParaCount;
    int		nMaxParaCount;
    _ST_NVCmdLine()
    {
        memset(this ,0, sizeof(_ST_NVCmdLine));
        bBigEndian		= TRUE;
        nMaxParaCount	= 5; 
        bTail			= TRUE;
	  bCheckItemName = TRUE;
    }
}ST_NVCmdLine, *P_ST_NVCmdLine;



#endif
