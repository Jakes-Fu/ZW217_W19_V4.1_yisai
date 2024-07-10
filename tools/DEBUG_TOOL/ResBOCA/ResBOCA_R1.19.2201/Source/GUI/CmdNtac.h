#pragma once
#include "NAParserV2.h"

typedef enum
{
	E_EXECL	= 0,
	E_NTAC	= 1	
} NTAC_TYPE_E;

typedef struct _NTAC_Args_T
{
	NTAC_TYPE_E eInType;
	TCHAR szInFile[MAX_PATH];
	NTAC_TYPE_E eOutType;
	TCHAR szOutFile[MAX_PATH];
	_NTAC_Args_T()
	{
		Clear();
		eInType = E_EXECL;
		eOutType = E_NTAC;
	}
	void Clear()
	{
		memset(this,0, sizeof(_NTAC_Args_T));
	}

}NTAC_Args_T,*PNTAC_Args_PTR;

class CCmdNtac
{
public:
	CCmdNtac(void);
	virtual ~CCmdNtac(void);
	BOOL GenNtac(const NTAC_Args_T& tArgs);
private:
	BOOL LoadNtacFile(const NTAC_Args_T& tArgs);
	BOOL SaveNtacFile(const NTAC_Args_T& tArgs);

private:
	CNAParserV2 m_Parser;
};
