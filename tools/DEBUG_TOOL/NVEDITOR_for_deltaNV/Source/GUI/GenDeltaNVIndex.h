// GenDeltaNVIndex.h: interface for the CGenDeltaNVIndex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENDELTANVINDEX_H__65D661BA_9EBE_4D11_B678_289A3D5BB92D__INCLUDED_)
#define AFX_GENDELTANVINDEX_H__65D661BA_9EBE_4D11_B678_289A3D5BB92D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <MAP>
#include <VECTOR>
#include "pugixml.hpp"
#include <string>
using namespace std;


#define DELTA_NV_FILE_NAME_LENGTH	(20)
#define DELTA_NV_VERSION_LENGTH		(2)
#define DELTA_NV_DATA_LENGTH		(4)

const char	DELTA_INDEX_NAME[] = "DeltaNvIndex";
const TCHAR OPERATOR_INDEX_XML_NAME[] = _T("Index.xml");

enum {
	E_OPERATOR_CFG		= 0,    
	E_OPERATOR_SIM		= 1,    
	E_OPERATOR_PLMN		= 2,   
	E_OPERATOR_COUNT
};

const string agStrPostfix[E_OPERATOR_COUNT] = {
	"_c",
	"_s",
	"_p"
};


typedef struct _DELTA_NV_INFO_T_
{
	_DELTA_NV_INFO_T_()
	{
		Clear();
	}
	unsigned int  nMccMnc;							// high 16 bit:mcc;low 16 bit: mnc; nMccMnc = mcc<<16 | mnc
												    // mnc bit15: 1, three ; 0, two
	unsigned int  agOffset[E_OPERATOR_COUNT];		// E_OPERATOR_CFG ~ E_OPERATOR_PLMN
													// agOffset[0]: Operator_Cfg.nv offset,default is 0xFFFFFFFF
													// agOffset[1]: Operator_Sim.nv offset,default is 0xFFFFFFFF
													// agOffset[2]: Operator_Plmn.nv offset,default is 0xFFFFFFFF
	void Clear()
	{
		memset(this,0xFF,sizeof(_DELTA_NV_INFO_T_));
		nMccMnc = 0;
	}
}DELTA_NV_INFO_T,*PDELTA_NV_INFO_PTR;


typedef std::map<std::string,DWORD> CMapDeltaNV;
typedef std::map<DWORD,DELTA_NV_INFO_T> CMapIndex;


class CGenDeltaNVIndex  
{
public:
	CGenDeltaNVIndex();
	virtual ~CGenDeltaNVIndex();
	BOOL GenDeltaIndex(LPBYTE pDeltaBuf,DWORD& dwSize, LPCTSTR lpIndexFile);

private:
	BOOL ParserDeltaNV(LPBYTE pDeltaBuf,DWORD dwSize,CMapDeltaNV& mapOffsetInfo);
	BOOL ParserIndexXml(LPCTSTR lpXmlFile, CMapDeltaNV& mapOffsetInfo,CMapIndex& mapIndex);
	BOOL GetOperatorOffset(const string strOperator,CMapDeltaNV& mapOffsetInfo,DELTA_NV_INFO_T& tDeltaInfo);

};

#endif // !defined(AFX_GENDELTANVINDEX_H__65D661BA_9EBE_4D11_B678_289A3D5BB92D__INCLUDED_)
