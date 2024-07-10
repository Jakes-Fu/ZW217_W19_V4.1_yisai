#pragma once
#include "Global.h"

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

#define PACKET_MIN_SIEZ				(12)
#define PACKET_FLAG_TAG				(0xAE)
#define PACKET_FLAG_FLOWID			(0xFF)
#define PACKET_PACKETHEAD_SZIE		(8)                 //< 包头的大小
#define PACKET_DATAHEAD_SIZE		(12)                //< 数据头的大小
#define FDL1_ADDRESS				(0x008000C0)
#define MAX_HOST_DATA_PACKET_SIZE	(2048)              //< 一个包中的数据大小

#define MAX_HOST_PACKAGE_LEN		(0x10000)	

#pragma pack(push, 1)
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
	 DWORD dwDataSize;    //< 数据大小 或 开始包的下载文件大小
	 BYTE  *pData;        //< 数据
}T_PDL_Data_Packet, *pT_PDL_Data_Packet;


typedef struct T_Packet_Tag
{
     T_Packet_Header   PkgHeader;
     T_PDL_Data_Packet PkgPdl;
	 T_Packet_Tag()
	 {
		 memset(this,0x0,sizeof(T_Packet));
	 }
}T_Packet, *pT_Packet;
#pragma pack(pop)

#define DeclareHostHeader(_PkgHeader, _DataLen)     \
	_PkgHeader.ucTag     = PACKET_FLAG_TAG;\
	_PkgHeader.nDataSize = PACKET_DATAHEAD_SIZE+_DataLen;\
	_PkgHeader.ucFlowID  = PACKET_FLAG_FLOWID;\
	_PkgHeader.wReserved = 0x00;


#define DeclareHostData(_PkgData, _Cmd,_Addr,_Size,_pData)    \
	_PkgData.dwCmdType  = _Cmd;\
	_PkgData.dwDataAddr = _Addr;\
	_PkgData.dwDataSize = _Size;\
	_PkgData.pData      = _pData;


class CHostProtocol
{
public:
	CHostProtocol(void);
	virtual ~CHostProtocol(void);
	/// 设置FDL1的文件信息
	BOOL SetFdlFileInfo(const BMFileInfo* pBMFileInfo);

    /// 组装connect命令
	BOOL Assemble_Packet_Connect(LPBYTE& pDestBuff, int& nBuffLen); 

	/// 组装开始数据包
	BOOL Assemble_Packet_Start(LPBYTE& pDestBuff, int& nBuffLen);

	/// 组装一个数据包
	BOOL Assemble_Packet_Data(int nPacketNo, LPBYTE& pDestBuff, int& nDestBuffLen); 

	/// 组装结束数据包
    BOOL Assemble_Packet_End(LPBYTE& pDestBuff, int& nBuffLen);

	/// 组装执行数据包
	BOOL Assemble_Packet_Execute(LPBYTE& pDestBuff, int &nBuffLen); 

	/// 解码应答数据
	BOOL Decode_Response(LPBYTE pDataBuff, int nDataSize);  

	/// 计算文件发送的包数
    int CountPackets();


private:
	/// 计算一个完整包的大小
	int Calc_Packet_Size(int nDataLen);

	/// 获取一个数据包中包含的数据
	BOOL Get_Packet_FDL1(int nPacketNo, LPBYTE& pDestBuff, int& nDestBuffLen);

	/// 组装数据包
    void Assemble_Packet(const T_Packet& sPacket, LPBYTE pDestBuff, int nBuffLen);

	LPVOID	GetMemory(DWORD dwSize);
	void    FreeMemory(LPBYTE& lpMemBlock);
	BOOL	CheckPackageMemory();

private:
	LPBYTE	m_pFileBuf;                  //< 保存FDL1文件数据缓冲区
	DWORD   m_dwFileSize;                //< 保存FDL1文件大小
	DWORD	m_dwAddress;                 //< 烧写地址
	DWORD   m_dwMaxLength;
	LPBYTE	m_pPackageBuf;               //< host协议包缓存

	LPBYTE			m_pRingBuf;			 // Internal buffer 
	unsigned int	m_nDataSize;		 // Internal data size
};
