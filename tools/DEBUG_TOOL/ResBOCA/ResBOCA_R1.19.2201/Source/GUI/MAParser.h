#pragma once

#pragma warning(push,3)
#include <vector>
#pragma warning(pop)

#define ECallCount	10
#define ECallSize	32

#define VMailCount	10
#define VMailSize	32

#define URLCount	1
#define URLSize		128

#define MTVA_MAGIC			0x4156544D// MTVA
#define MTVA_MAX_NUM        0xFFFF
#define MTVA_VER            1

typedef struct _MTVA_FILE_HEAD_T
{
	DWORD	magic;
	DWORD	version;
	DWORD	nCount;
	DWORD	Revered;
	//_MTVA_FILE_HEAD_T()
	//{
	//	memset(this,0,sizeof(_MTVA_FILE_HEAD_T));
	//}
	void Init()
	{
		magic = MTVA_MAGIC;
		version = MTVA_VER;
		nCount = 0;
		Revered = 0;
	}

	_MTVA_FILE_HEAD_T()
	{
		Init();
	}
}MTVA_FILE_HEAD_T;

typedef struct _MTVA_T
{
	//DWORD	nActive;
	DWORD	nMcc;
	DWORD	nMnc;
	CHAR	ECall[ECallCount][ECallSize];
	CHAR	VMail[VMailCount][VMailSize];
	CHAR	Url[URLSize];

	void Init()
	{
		memset(this,0,sizeof(_MTVA_T));
		memcpy(Url,"http://",strlen("http://"));
		nMcc = 0;
		nMnc = 0;
	}

	_MTVA_T()
	{
		Init();
	}

}MTVA_T;

class CMAParser
{
public:
	CMAParser(void);
	~CMAParser(void);
	BOOL DecodeMTVA(LPBYTE pBuf,DWORD dwSize);
	BOOL EncodeMTVA(LPBYTE &pBuf,DWORD &dwSize);

	std::vector<MTVA_T> m_vMTVA;
	MTVA_FILE_HEAD_T m_tHead;

private:
	void Clear();
};
