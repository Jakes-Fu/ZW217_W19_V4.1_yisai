#pragma once
#include "stdafx.h"
#include "HostProtocol.h"

CHostProtocol::CHostProtocol(void)
{
	m_nLastErrNo = RSP_ACK;

	m_strFile.Empty();
	m_pFile = NULL;
	m_nFileSizes = 0;

}

CHostProtocol::~CHostProtocol(void)
{

	m_strFile.Empty();

	if(m_pFile)
	{
		delete []m_pFile;
		m_pFile = NULL;
	}
}


/** 
* @brief               <p> ����һ������������С </p>
* @param int nDataLen  <p> ���е����ݴ�С </p>
* @return value int    <p> ���Ĵ�С </p>
*/
int CHostProtocol::Calc_Packet_Size(int nDataLen)
{
	int nSizes = 0;
	
	nSizes += PACKET_PACKETHEAD_SZIE;
	nSizes += PACKET_DATAHEAD_SIZE;
	nSizes += nDataLen;

	return nSizes;
}

/** 
* @brief                    <p> ��װconnect���� </p>
* @param LPBYTE pDestBuff   <p> Ŀ�껺���� </p>
* @param int nBuffLen       <p> Ŀ�껺�����Ĵ�С </p>
* @param value int          <p> �������е����ݴ�С </p>
*/
int CHostProtocol::Assemble_Patket_Connect(LPBYTE pDestBuff, int &nBuffLen)
{
	T_Packet PktConnect;
	int nSizes = 0;
	int nDataLen = 0;

	/// �������ݰ��Ĵ�С
	nSizes = Calc_Packet_Size(nDataLen);

	if(nSizes > nBuffLen || pDestBuff == NULL)
	{
		if(pDestBuff)
		{
			delete []pDestBuff;
			pDestBuff = NULL;
		}

        nBuffLen = nSizes;
		pDestBuff = new BYTE[nSizes + 1];

		if(!pDestBuff)
		{
			::AfxMessageBox(_T("Assemble_Patket_Connect: memory allocation failure!"));
			return nSizes;
		}
	}

	memset((void *)&PktConnect, 0x00, sizeof(T_Packet));

	PktConnect.PkgHeader.ucTag     = PACKET_FLAG_TAG;
	PktConnect.PkgHeader.nDataSize = PACKET_DATAHEAD_SIZE + nDataLen;
	PktConnect.PkgHeader.ucFlowID  = PACKET_FLAG_FLOWID;
    PktConnect.PkgHeader.wReserved = 0x00;

	PktConnect.PkgPdl.dwCmdType  = CMD_CONNECT;
	PktConnect.PkgPdl.dwDataAddr = 0x00;
	PktConnect.PkgPdl.dwDataSize = 0;
	PktConnect.PkgPdl.pData      = NULL;
	PktConnect.PkgPdl.nPackSize  = nDataLen;

	/// ��װ����
	Assemble_Patket(PktConnect, pDestBuff, nSizes);

	return nSizes;
}


/** 
* @brief                  <p> ��װһ�����ݰ� </p>
* @param int nPacketNo    <p> ���ݰ���� </p>
* @param LPBYTE pDestBuff <p> ������ݰ���Ŀ�껺���� </p>
* @param int nDestBuffLen <p> Ŀ�껺�����Ĵ�С </p>
* @return value int       <p> ���ݰ��Ĵ�С </p>
*/
int CHostProtocol::Assemble_Packet_Data(int nPacketNo, LPBYTE pDestBuff, int nDestBuffLen)
{
	T_Packet PktConnect;
	int nSizes = 0;
	int nDataLen = 0;

	LPBYTE pData = new BYTE[MAX_PACKET_SIZE];         //< ��Ű��е�����

	if(!pData)
	{
		::AfxMessageBox(_T("Assemble_Packet_Data: memory allocation failure!"));
		return nSizes;
	}
	
	/// ��ȡ�ð��е����ݼ���С
    nDataLen = Get_Packet_FDL1(nPacketNo, pData, MAX_PACKET_SIZE);

	/// �������ݰ��Ĵ�С
	nSizes = Calc_Packet_Size(nDataLen);

	if(nSizes > nDestBuffLen || pDestBuff == NULL)
	{
		if(pDestBuff)
		{
			delete []pDestBuff;
			pDestBuff = NULL;
		}

		nDestBuffLen = nSizes;
		pDestBuff = new BYTE[nSizes + 1];

		if(!pDestBuff)
		{
			::AfxMessageBox(_T("Assemble_Packet_Data: memory allocation failure!"));
			return nSizes;
		}
	}

	memset((void *)&PktConnect, 0x00, sizeof(T_Packet));

	PktConnect.PkgHeader.ucTag     = PACKET_FLAG_TAG; 
	PktConnect.PkgHeader.nDataSize = nDataLen + PACKET_DATAHEAD_SIZE;   //< ���ݴ�С
	PktConnect.PkgHeader.ucFlowID  = PACKET_FLAG_FLOWID;
	PktConnect.PkgHeader.wReserved = 0x00;

	PktConnect.PkgPdl.dwCmdType  = CMD_MID_DATA;                        //< �м����ݰ�
	PktConnect.PkgPdl.dwDataAddr = nPacketNo;                           //< ���ݰ������
	PktConnect.PkgPdl.dwDataSize = nDataLen;
	PktConnect.PkgPdl.pData      = pData;                               //< �������ݴ�ŵĻ�����
	PktConnect.PkgPdl.nPackSize  = nDataLen;                            //< �������ݵĴ�С

	/// ��װ����
	Assemble_Patket(PktConnect, pDestBuff, nSizes);

	/// �ͷ��ڴ�
	delete []pData;
	pData = NULL;

	return nSizes;
}



/** 
* @brief                  <p> ��װ��ʼ���ݰ� </p>
* @param LPBYTE pDestBuff <p> ������ݰ���Ŀ�껺���� </p>
* @param int nDestBuffLen <p> Ŀ�껺�����Ĵ�С </p>
* @return value int       <p> ���ݰ��Ĵ�С </p>
*/
int CHostProtocol::Assemble_Patket_Start(LPBYTE pDestBuff, int &nBuffLen)
{
	T_Packet PktConnect;

	char FileFlag[5] = {0x50, 0x44, 0x4c, 0x31, 0x00};      //< "PDL1"
	int nDataLen = 5;
    int nSizes = 0;
	
	/// �������ݰ��Ĵ�С
	nSizes = Calc_Packet_Size(nDataLen);

	if(nSizes > nBuffLen || pDestBuff == NULL)
	{
		if(pDestBuff)
		{
			delete []pDestBuff;
			pDestBuff = NULL;
		}

		nBuffLen = nSizes;
		pDestBuff = new BYTE[nSizes + 1];

		if(!pDestBuff)
		{
			::AfxMessageBox(_T("Assemble_Packet_Data: memory allocation failure!"));
			return nSizes;
		}
	}

	memset((void *)&PktConnect, 0x00, sizeof(T_Packet));

	PktConnect.PkgHeader.ucTag     = PACKET_FLAG_TAG;
	PktConnect.PkgHeader.nDataSize = PACKET_DATAHEAD_SIZE + nDataLen;
	PktConnect.PkgHeader.ucFlowID  = PACKET_FLAG_FLOWID;
	PktConnect.PkgHeader.wReserved = 0x00;

	PktConnect.PkgPdl.dwCmdType    = CMD_START_DATA;             //< START_DATA
	PktConnect.PkgPdl.dwDataAddr   = (DWORD)FDL1_ADDRESS;        //< FDL1 �ļ�����д��ַ
	PktConnect.PkgPdl.dwDataSize   = m_nFileSizes;               //< FDL1�ļ��Ĵ�С
	PktConnect.PkgPdl.pData        = (BYTE *)&FileFlag;          //< ���ݻ�����
	PktConnect.PkgPdl.nPackSize    = nDataLen;                   //< ���ݴ�С

	/// ��װ����
	Assemble_Patket(PktConnect, pDestBuff, nSizes);

	return nSizes;
}


/** 
* @brief                  <p> ��װ�������ݰ� </p>
* @param LPBYTE pDestBuff <p> ������ݰ���Ŀ�껺���� </p>
* @param int nDestBuffLen <p> Ŀ�껺�����Ĵ�С </p>
* @return value int       <p> ���ݰ��Ĵ�С </p>
*/
int CHostProtocol::Assemble_Patket_End(LPBYTE pDestBuff, int &nBuffLen)
{
	T_Packet PktConnect;

	char Data[4] = {0x1c, 0x3c, 0x6e, 0x06};
	int nDataLen = 4;
	int nSizes = 0;

	/// �������ݰ��Ĵ�С
	nSizes = Calc_Packet_Size(nDataLen);

	if(nSizes > nBuffLen || pDestBuff == NULL)
	{
		if(pDestBuff)
		{
			delete []pDestBuff;
			pDestBuff = NULL;
		}

		nBuffLen = nSizes;
		pDestBuff = new BYTE[nSizes + 1];

		if(!pDestBuff)
		{
			::AfxMessageBox(_T("Assemble_Patket_End: memory allocation failure!"));
			return nSizes;
		}
	}

	memset((void *)&PktConnect, 0x00, sizeof(T_Packet));

	PktConnect.PkgHeader.ucTag     = PACKET_FLAG_TAG;
	PktConnect.PkgHeader.nDataSize = PACKET_DATAHEAD_SIZE + nDataLen;
	PktConnect.PkgHeader.ucFlowID  = PACKET_FLAG_FLOWID;
	PktConnect.PkgHeader.wReserved = 0x00;

	PktConnect.PkgPdl.dwCmdType    = CMD_END_DATA;               //< END_DATA
	PktConnect.PkgPdl.dwDataAddr   = 0;                          //< ��д��ַ
	PktConnect.PkgPdl.dwDataSize   = 0;                          //< FDL1�ļ��Ĵ�С
	PktConnect.PkgPdl.pData        = (BYTE *)&Data;              //< ���ݻ�����
	PktConnect.PkgPdl.nPackSize    = nDataLen;                   //< ���ݴ�С

	/// ��װ����
	Assemble_Patket(PktConnect, pDestBuff, nSizes);

	return nSizes;
}


/** 
* @brief                  <p> ��װִ�����ݰ� </p>
* @param LPBYTE pDestBuff <p> ������ݰ���Ŀ�껺���� </p>
* @param int nDestBuffLen <p> Ŀ�껺�����Ĵ�С </p>
* @return value int       <p> ���ݰ��Ĵ�С </p>
*/
int CHostProtocol::Assemble_Patket_Execute(LPBYTE pDestBuff, int &nBuffLen)
{
	T_Packet PktConnect;

	int nDataLen = 0;
	int nSizes = 0;

	/// �������ݰ��Ĵ�С
	nSizes = Calc_Packet_Size(nDataLen);

	if(nSizes > nBuffLen || pDestBuff == NULL)
	{
		if(pDestBuff)
		{
			delete []pDestBuff;
			pDestBuff = NULL;
		}

		nBuffLen = nSizes;
		pDestBuff = new BYTE[nSizes + 1];

		if(!pDestBuff)
		{
			::AfxMessageBox(_T("Assemble_Patket_Execute: memory allocation failure!"));
			return nSizes;
		}
	}

	memset((void *)&PktConnect, 0x00, sizeof(T_Packet));

	PktConnect.PkgHeader.ucTag     = PACKET_FLAG_TAG;
	PktConnect.PkgHeader.nDataSize = PACKET_DATAHEAD_SIZE + nDataLen;
	PktConnect.PkgHeader.ucFlowID  = PACKET_FLAG_FLOWID;
	PktConnect.PkgHeader.wReserved = 0x00;

	PktConnect.PkgPdl.dwCmdType    = CMD_EXEC_DATA;              //< END_DATA
	PktConnect.PkgPdl.dwDataAddr   = 0;                          //< ��д��ַ
	PktConnect.PkgPdl.dwDataSize   = 0;                          //< FDL1�ļ��Ĵ�С
	PktConnect.PkgPdl.pData        = NULL;                       //< ���ݻ�����
	PktConnect.PkgPdl.nPackSize    = nDataLen;                   //< ���ݴ�С

	/// ��װ����
	Assemble_Patket(PktConnect, pDestBuff, nSizes);

	return nSizes;
}

/** 
* @brief                     <p> ��װ���ְ� </p>
* @param T_Packet& sPacket   <p> ���ݰ��Ļ�����Ϣ </p>
* @param LPBYTE pDestBuff    <p> ������ݵ�Ŀ�껺���� </p>
* @param int nBuffLen        <p> ���ݰ��Ĵ�С </p>
* @return value int          <p> ���ݰ��Ĵ�С </p> 
*/
int CHostProtocol::Assemble_Patket(T_Packet& sPacket, LPBYTE pDestBuff, int nBuffLen)
{
    LPBYTE pDst = pDestBuff;

	memcpy(pDst,(BYTE *)&sPacket.PkgHeader.ucTag, sizeof(sPacket.PkgHeader.ucTag));
	pDst += sizeof(sPacket.PkgHeader.ucTag);

	memcpy(pDst,(BYTE *)&sPacket.PkgHeader.nDataSize, sizeof(sPacket.PkgHeader.nDataSize));
	pDst += sizeof(sPacket.PkgHeader.nDataSize);

	memcpy(pDst,(BYTE *)&sPacket.PkgHeader.ucFlowID, sizeof(sPacket.PkgHeader.ucFlowID));
	pDst += sizeof(sPacket.PkgHeader.ucFlowID);

	memcpy(pDst,(BYTE *)&sPacket.PkgHeader.wReserved, sizeof(sPacket.PkgHeader.wReserved));
	pDst += sizeof(sPacket.PkgHeader.wReserved);

	memcpy(pDst,(BYTE *)&sPacket.PkgPdl.dwCmdType, sizeof(sPacket.PkgPdl.dwCmdType));
	pDst += sizeof(sPacket.PkgPdl.dwCmdType);

	memcpy(pDst,(BYTE *)&sPacket.PkgPdl.dwDataAddr, sizeof(sPacket.PkgPdl.dwDataAddr));
	pDst += sizeof(sPacket.PkgPdl.dwDataAddr);

	memcpy(pDst,(BYTE *)&sPacket.PkgPdl.dwDataSize, sizeof(sPacket.PkgPdl.dwDataSize));
	pDst += sizeof(sPacket.PkgPdl.dwDataSize);

	if(sPacket.PkgPdl.pData)
	{
		memcpy(pDst, sPacket.PkgPdl.pData, sPacket.PkgPdl.nPackSize);
		pDst += sPacket.PkgPdl.nPackSize;
	}

	return nBuffLen;
}

/** 
* @brief                    <p> ��������Ӧ���� </p>
* @param LPBYTE pDataBuff   <p> Դ���ݻ����� </p>
* @param int nDataSize      <p> �������е����ݴ�С </p>
* @param value BOOL         <p> Ӧ��OK������TRUE�����򣬷���FALSE </p>
*/
BOOL CHostProtocol:: Decode_Response(LPBYTE pDataBuff, int nDataSize)
{
	if(nDataSize < PACKET_MIN_SIEZ)
	{
		return FALSE;
	}

    T_Packet sPacket;

    sPacket.PkgHeader.ucTag     = *pDataBuff;
	sPacket.PkgHeader.nDataSize = *((int *)(pDataBuff + 1));
	sPacket.PkgHeader.ucFlowID  = *(pDataBuff + 5);
	sPacket.PkgHeader.wReserved = *((WORD *)(pDataBuff + 6));

	sPacket.PkgPdl.dwCmdType = *((DWORD *)(pDataBuff + 8));

    if(sPacket.PkgHeader.ucTag == PACKET_FLAG_TAG \
		&& sPacket.PkgHeader.ucFlowID == PACKET_FLAG_FLOWID \
		&& sPacket.PkgPdl.dwCmdType == RSP_ACK)
	{
		return TRUE;
	}

	m_nLastErrNo = sPacket.PkgPdl.dwCmdType;

	return FALSE;
}


/** 
* @brief      <p> ���һ�εĴ�����Ϣ </p>
*/
CString CHostProtocol::GetLastErrInfo()
{
	int nErrCount = sizeof(ErrInfo) / sizeof(TCHAR *);

	if(m_nLastErrNo < nErrCount)
	{
        return ErrInfo[m_nLastErrNo];
	}

	return _T("");
}


/** 
* @brief   <p> ����FDL1�ļ�·���� </p>
*/
void CHostProtocol::SetFDL1Path(CString strFDL1File)
{
	m_strFile = strFDL1File;
}


/** 
* @brief      <p> ���㷢�͵����ݰ��� </p>
*/
int CHostProtocol::CountPackets()
{
	int nPacketCount = 0;

	if(m_strFile.IsEmpty())
	{
		return nPacketCount;
	}

	CFile sFile;

	if( !sFile.Open(m_strFile,CFile::modeRead) )
	{
		::AfxMessageBox(_T("Open file Fail"));
		return nPacketCount;
	}

	if(m_pFile)
	{
		delete []m_pFile;
		m_pFile = NULL;

		m_nFileSizes = 0;
	}

	m_nFileSizes = (int)sFile.GetLength();

    m_pFile = new BYTE[m_nFileSizes + 1];

	if(m_pFile)
	{
		sFile.Read(m_pFile, m_nFileSizes);
		nPacketCount = (m_nFileSizes - 1) / MAX_PACKET_SIZE + 1;
	}
	else
	{
		::AfxMessageBox(_T("Open file Fail!"));
	}

	sFile.Close();


	return nPacketCount;
}


/** 
* @brief                      <p> ��ȡһ�����ݰ������� <p>
* @param int nPacketNo        <p> ���ݰ������ </p>
* @param LPBYTE pDestBuff     <p> ������ݰ���Ŀ�껺���� </p>
* @param int nDestBuffLen     <p> Ŀ�껺�����Ĵ�С </p>
* @return value int           <p> ���ݰ��Ĵ�С </p>
*/
int CHostProtocol::Get_Packet_FDL1(int nPacketNo, LPBYTE pDestBuff, int nDestBuffLen)
{
	int nPackSizes = 0;                                     //< ���ݰ��Ĵ�С
	int nPactetOffset = 0;                                  //< ���ݵĿ�ʼƫ��
	int nPacketNums = CountPackets();                       //< ���͵��ܰ���

	if(nPacketNums == 0)
	{
		return nPackSizes;
	}

	if(nPacketNo < 0 || nPacketNo >= nPacketNums)
	{
        return nPackSizes;
	}

	/// �������ݰ��Ĵ�С
	if(nPacketNo < nPacketNums - 1)
	{
		nPackSizes = MAX_PACKET_SIZE;
	}
	else
	{
		nPackSizes = m_nFileSizes - nPacketNo * MAX_PACKET_SIZE; 
	}

	/// ��������FDL1�ܵı��˵�ַ
    nPactetOffset = nPacketNo * MAX_PACKET_SIZE; 

	if(nDestBuffLen < nPackSizes)
	{
		delete []pDestBuff;
		pDestBuff = NULL;
	}

	if(!pDestBuff)
	{
		pDestBuff = new BYTE[nPackSizes + 1];
	}

	memcpy(pDestBuff, (LPBYTE)(m_pFile + nPactetOffset), nPackSizes);

	return nPackSizes;
}


