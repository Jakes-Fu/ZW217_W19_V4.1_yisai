#ifndef _NVITEMDEF_H
#define _NVITEMDEF_H
#pragma warning(push,3)
#include <string>
#include <vector>
#pragma warning(pop)

using namespace std;

#define MAX_PATHLENGTH   520
#define MAX_NAME_LENGTH                 (0x80)
#define MAX_ITEM_CONTENT                (0x80)
#define MAX_ITEM_DESCPT                 (0x100)
#define INVALID_MODULE_ID               ((INT)0xFFFFFFFF)
#define INVALID_MODULE_ITEM_INDEX       ((INT)0xFFFFFFFF)
#define MAX_TOTAL_ITEM_CONTENT          (0x500000)	//1M
#define DSP_CODE_SIZE                   (0xC000)
#define MAX_DSP_CODE_SIZE               (64*1024*2)	//128k
#define FILE_TOTAL_SIZE					(0x10000)   //64k

//Define Item type
typedef enum _PERMISSION_LEVEL_ENUM 
{ 
    NONE_PERMISSION		= 0,	//Edit X, Read X
	SUPER_PERMISSION	= 1,	//Edit O, Read O
	READ_PERMISSION		= 2		//Edit X, Read O						
}PERMISSION_LEVEL;

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

typedef enum ITEMDES_ENUM
{ 
	CLOSE_INTERVAL_TYPE = 0, // ()
	HALF_CLOSE_INTERVAL_TYPE,//(]
	HALF_OPEN_INTERVAL_TYPE, //[)
	OPEN_INTERVAL_TYPE,
	SET_RANGE_TYPE
		
};

typedef struct _ST_FILE_INFO
{
	int       nFileID; 
	BOOL      bCheckFlag;
	TCHAR     szFilePath[MAX_PATHLENGTH]; 
	_ST_FILE_INFO()
	{
		memset(this,0,sizeof(_ST_FILE_INFO));
	}
}ST_FILE_INFO,*PFILE_INFO_ST;

typedef vector <ST_FILE_INFO>  VEC_FILEINFO;

typedef struct _ST_NVCmdLine
{
	BOOL	bBigEndian;
	BOOL	bWait;
	BOOL	bTail;
	BOOL	bVersion;
	BOOL	bHeaderFile;
	TCHAR	strErrorFile[MAX_PATHLENGTH];
	TCHAR   strImportFile[MAX_PATHLENGTH];
	TCHAR   strDspBinFile[MAX_PATHLENGTH];    
	TCHAR   strFileName[MAX_PATHLENGTH];
	TCHAR   strMergePath[MAX_PATHLENGTH]; 
	TCHAR   strHeaderFile[MAX_PATHLENGTH];
	TCHAR   strFixBinFile[MAX_PATHLENGTH];
	TCHAR   strHashHeader[MAX_PATHLENGTH];
	int     nParaCount;
	int     nMaxParaCount;
	_ST_NVCmdLine()
	{
		memset(this,0,sizeof(_ST_NVCmdLine));
		bBigEndian		= TRUE;
		nMaxParaCount	= 5; 
		bTail			= TRUE;
	}
}ST_NVCmdLine, *P_ST_NVCmdLine;



#endif
