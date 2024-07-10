#pragma once
#include "afx.h"
//#include <afxtempl.h>
//#include "Global.h"

enum pdl_cmd_type {
	CMD_CONNECT,            //< ׼������
	CMD_ERASE_FLASH,        //< ����ĳһ��flash
	CMD_ERASE_PARTITION,    //< ����һ��flash mtd����
	CMD_ERASE_ALL,          //< ��������flash
	CMD_START_DATA,         //< ��ʼdownload����
	CMD_MID_DATA,           //< �м�����
	CMD_END_DATA,           //< �������
	CMD_EXEC_DATA,          //< ִ�и����ݣ���Ҫ��PDL��
	CMD_READ_FLASH,         //< read flash
	CMD_READ_PARTITIONS,    //< read ĳһ������, ����������product info��
	CMD_NORMAL_RESET,       //< reset phone
	CMD_READ_CHIPID,        //< ��chipid
	CMD_SET_BAUDRATE,       //< option�����usbת����û�����壬�������Ĵ�����Ч��
};


enum pdl_rsp {
	RSP_ACK,
	
	/// from PC command
	RSP_INVALID_CMD,
	RSP_UNKNOWN_CMD,
	RSP_INVALID_ADDR,
	RSP_INVALID_BAUDRATE,
	RSP_INVALD_PARTITION,
	RSP_SIZE_ERROR,
	RSP_WAIT_TIMEOUT,
	
	/// from phone
	RSP_VERIFY_ERROR,
	RSP_CHECKSUM_ERROR,
	RSP_OPERATION_FAILED,
	
	/// phone internal
	RSP_DEVICE_ERROR, //DDR,NAND init errors
	RSP_NO_MEMORY
};

static TCHAR *ErrInfo[] = {
    _T("INVALID_CMD"),
	_T("UNKNOWN_CMD"),
	_T("INVALID_ADDR"),
	_T("INVALID_BAUDRATE"),
	_T("INVALD_PARTITION"),
	_T("SIZE_ERROR"),
	_T("WAIT_TIMEOUT"),
	_T("VERIFY_ERROR"),
	_T("CHECKSUM_ERROR"),
	_T("OPERATION_FAILED"),
	_T("DEVICE_ERROR"),
	_T("NO_MEMORY"),
};

#define PACKET_MIN_SIEZ    12
#define PACKET_FLAG_TAG    0xAE
#define PACKET_FLAG_FLOWID 0xFF
#define PACKET_PACKETHEAD_SZIE   8                 //< ��ͷ�Ĵ�С
#define PACKET_DATAHEAD_SIZE     12                //< ����ͷ�Ĵ�С
#define FDL1_ADDRESS 0x008000C0
#define MAX_PACKET_SIZE    2048                    //< һ�����Ĵ�С


typedef struct T_Packet_Header_Tag
{
	BYTE ucTag;     //< 0xAE
	int  nDataSize; //< data size
	BYTE ucFlowID;  //< 0xFF
	WORD wReserved; //< reserved
}T_Packet_Header, *pT_Packet_Header;


typedef struct T_PDL_Data_Packet_Tag
{
     DWORD dwCmdType;     //< ��������
	 DWORD dwDataAddr;    //< ���ݵ�ַ
	 DWORD dwDataSize;    //< ���ݴ�С
	 BYTE  *pData;        //< ����
	 int   nPackSize;     //< һ���ܵ����ݴ�С
}T_PDL_Data_Packet, *pT_PDL_Data_Packet;


typedef struct T_Packet_Tag
{
     T_Packet_Header   PkgHeader;
     T_PDL_Data_Packet PkgPdl;
}T_Packet, *pT_Packet;


class CHostProtocol :
	public CObject
{
public:
    /// ��װconnect����
	int Assemble_Patket_Connect(LPBYTE pDestBuff, int& nBuffLen); 

	/// ��װ��ʼ���ݰ�
	int Assemble_Patket_Start(LPBYTE pDestBuff, int& nBuffLen);

	/// ��װһ�����ݰ�
	int Assemble_Packet_Data(int nPacketNo, LPBYTE pDestBuff, int nDestBuffLen); 

	/// ��װ�������ݰ�
    int Assemble_Patket_End(LPBYTE pDestBuff, int& nBuffLen);

	/// ��װִ�����ݰ�
	int Assemble_Patket_Execute(LPBYTE pDestBuff, int &nBuffLen); 

	/// ����Ӧ������
	BOOL Decode_Response(LPBYTE pDataBuff, int nDataSize);  

	/// �������һ�εĴ�����Ϣ
	CString GetLastErrInfo();  
	
	/// ����FDL1�ļ�·����(Full path name)
	void SetFDL1Path(CString strFDL1File);    

	/// �����ļ����͵İ���
    int CountPackets();

	/// ����һ���������Ĵ�С
	int Calc_Packet_Size(int nDataLen);
	////////////////////////////////////////////////////////////////////////
	CHostProtocol(void);
	~CHostProtocol(void);
private:
	/// ��ȡһ�����ݰ��а���������
	int Get_Packet_FDL1(int nPacketNo, LPBYTE pDestBuff, int nDestBuffLen);

	/// ��װ���ݰ�
    int Assemble_Patket(T_Packet& sPacket, LPBYTE pDestBuff, int nBuffLen);

private:
	int m_nLastErrNo;                     //< ���һ�εĴ����
	CString m_strLastErr;                 //< ������Ϣ�ַ���
	CString m_strFile;                    //< ���浱ǰ�·����ļ���·��
	BYTE *m_pFile;                        //< �����ļ����ݻ�����
	int m_nFileSizes;                     //< �����ļ���С
};
