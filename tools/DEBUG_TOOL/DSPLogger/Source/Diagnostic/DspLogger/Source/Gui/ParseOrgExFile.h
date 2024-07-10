// ParseOrgExFile.h: interface for the ParseOrgExFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSEORGEXFILE_H__298A4B6D_403B_40C2_BEC0_5BEC3F0A07DE__INCLUDED_)
#define AFX_PARSEORGEXFILE_H__298A4B6D_403B_40C2_BEC0_5BEC3F0A07DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Export type definitions
typedef struct _ORG_HEADER_T_
{
	BYTE	magic[4];   // "DSPF"
	BYTE 	version[4];
	BYTE 	Endian;	
	BYTE 	Reserved[7];
	_ORG_HEADER_T_(){
		magic[0]='D';
		magic[1]='S';
		magic[2]='P';
		magic[3]='F';
		version[0]='0';
		version[1]='0';
		version[2]='0';
		version[3]='1';
		Endian=0;
		memset(Reserved,'\0',7);
	}	
} ORG_HEADER, *PORG_HEADER;
/*
struct DSP_ORG_PACKAGE
{
	DWORD dwLen;		// 8+len(lpData)
	DWORD dwPCTime;		// 18:51:25:122 -->0x02DA3CC4
	BYTE* lpData;		// buffer data
};
*/
/*
struct DSP_ORG_HEADER
{
	DWORD dwLen;		// 8+len(lpData)
	DWORD dwPCTime;		// 18:51:25:122 -->0x02DA3CC4
};

struct DSP_ORG_PACKAGE
{
	DSP_ORG_HEADER header;
	BYTE* data;				// Package data
};
*/

struct  TIME_VAL 
{
	DWORD  dwSec;         /* seconds */
	DWORD  dwMicroSec;    /* microseconds */
};
struct DSP_ORG_HEADER
{
	BYTE	magic[4];		// "ORGD"
	DWORD	dwLen;			// sizeof(DSP_ORG_HEADER)+len(lpData)
	TIME_VAL clock_time;
	DSP_ORG_HEADER (){
		memset(this,0,sizeof(DSP_ORG_HEADER));
		magic[0]='O';
		magic[1]='R';
		magic[2]='G';
		magic[3]='D';
	}
};
struct DSP_ORG_PACKAGE
{
    DSP_ORG_HEADER header;
	BYTE* lpData;		        // buffer data
};


class ParseOrgExFile  
{
public:
	ParseOrgExFile();
	virtual ~ParseOrgExFile();
public:
	BOOL	IsOrgExtendFile(LPCTSTR lpOrgFile);
	BOOL	PlaybackOrgEx(LPCTSTR lpOrgFile,LPVOID lpPara);

private:
	BOOL	LoadFile(LPCTSTR lpszFileName,LPBYTE *ppBuf, DWORD* pSize);
	BOOL	LitEdnToBigEdn( LPDWORD pwLitEdn, int nNum);
	DWORD	ConvEdn_DWORD( DWORD dwEdn );

};

#endif // !defined(AFX_PARSEORGEXFILE_H__298A4B6D_403B_40C2_BEC0_5BEC3F0A07DE__INCLUDED_)
