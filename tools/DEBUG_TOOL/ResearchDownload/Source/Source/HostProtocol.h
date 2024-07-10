#pragma once
#include "afx.h"
//#include <afxtempl.h>
//#include "Global.h"

enum pdl_cmd_type {
	CMD_CONNECT,            //< 准备连接
	CMD_ERASE_FLASH,        //< 擦除某一块flash
	CMD_ERASE_PARTITION,    //< 擦除一个flash mtd分区
	CMD_ERASE_ALL,          //< 擦除所有flash
	CMD_START_DATA,         //< 开始download数据
	CMD_MID_DATA,           //< 中间数据
	CMD_END_DATA,           //< 传输完成
	CMD_EXEC_DATA,          //< 执行该数据，主要是PDL用
	CMD_READ_FLASH,         //< read flash
	CMD_READ_PARTITIONS,    //< read 某一个分区, 可能用来读product info。
	CMD_NORMAL_RESET,       //< reset phone
	CMD_READ_CHIPID,        //< 读chipid
	CMD_SET_BAUDRATE,       //< option命令，对usb转串口没有意义，对真正的串口有效，
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
#define PACKET_PACKETHEAD_SZIE   8                 //< 包头的大小
#define PACKET_DATAHEAD_SIZE     12                //< 数据头的大小
#define FDL1_ADDRESS 0x008000C0
#define MAX_PACKET_SIZE    2048                    //< 一个包的大小


typedef struct T_Packet_Header_Tag
{
	BYTE ucTag;     //< 0xAE
	int  nDataSize; //< data size
	BYTE ucFlowID;  //< 0xFF
	WORD wReserved; //< reserved
}T_Packet_Header, *pT_Packet_Header;


typedef struct T_PDL_Data_Packet_Tag
{
     DWORD dwCmdType;     //< 命令类型
	 DWORD dwDataAddr;    //< 数据地址
	 DWORD dwDataSize;    //< 数据大小
	 BYTE  *pData;        //< 数据
	 int   nPackSize;     //< 一包总的数据大小
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
    /// 组装connect命令
	int Assemble_Patket_Connect(LPBYTE pDestBuff, int& nBuffLen); 

	/// 组装开始数据包
	int Assemble_Patket_Start(LPBYTE pDestBuff, int& nBuffLen);

	/// 组装一个数据包
	int Assemble_Packet_Data(int nPacketNo, LPBYTE pDestBuff, int nDestBuffLen); 

	/// 组装结束数据包
    int Assemble_Patket_End(LPBYTE pDestBuff, int& nBuffLen);

	/// 组装执行数据包
	int Assemble_Patket_Execute(LPBYTE pDestBuff, int &nBuffLen); 

	/// 解码应答数据
	BOOL Decode_Response(LPBYTE pDataBuff, int nDataSize);  

	/// 返回最后一次的错误信息
	CString GetLastErrInfo();  
	
	/// 设置FDL1文件路径名(Full path name)
	void SetFDL1Path(CString strFDL1File);    

	/// 计算文件发送的包数
    int CountPackets();

	/// 计算一个完整包的大小
	int Calc_Packet_Size(int nDataLen);
	////////////////////////////////////////////////////////////////////////
	CHostProtocol(void);
	~CHostProtocol(void);
private:
	/// 获取一个数据包中包含的数据
	int Get_Packet_FDL1(int nPacketNo, LPBYTE pDestBuff, int nDestBuffLen);

	/// 组装数据包
    int Assemble_Patket(T_Packet& sPacket, LPBYTE pDestBuff, int nBuffLen);

private:
	int m_nLastErrNo;                     //< 最后一次的错误号
	CString m_strLastErr;                 //< 错误信息字符串
	CString m_strFile;                    //< 保存当前下发的文件名路径
	BYTE *m_pFile;                        //< 保存文件数据缓冲区
	int m_nFileSizes;                     //< 保存文件大小
};
