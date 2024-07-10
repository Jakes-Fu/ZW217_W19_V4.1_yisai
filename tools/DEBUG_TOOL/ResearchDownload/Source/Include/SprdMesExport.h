//#include "SprdMes.h"
#pragma once

#pragma warning(push, 3)
#include <map>
#include <vector>
#include <string>
#pragma warning(pop)

using std::string;
using std::vector;

#ifdef _SPRD_MES_EXPORT_H
#define SPRD_MES_API extern "C" _declspec(dllexport)
#else 
#define SPRD_MES_API extern "C" _declspec(dllimport)
#endif

#if _MSC_VER < 1600
#define nullptr NULL
#endif

#define MAX_DATE_LENGTH				10
#define MAX_PROJECT_LENGTH			16
#define MAX_TIME_LENGTH				20
#define MAX_SN_LENGTH				24
#define MAX_INFORMATION_LENGTH		255
#define MAX_STATUS_LENGTH			16
#define MAX_ADDRESS_LENGTH			16
#define MAX_ELAPSE_LENGTH			8
#define MAX_ERRTYPE_LENGTH			64
#define MAX_ERRCODE_LENGTH			64
#define MAX_ERRMESSAGE_LENGTH		255
#define MAX_SWVERSION_LENGTH		128
#define MAX_TOOLVERSION_LENGTH		128
#define MAX_DATABASE_LENGTH			16
#define MAX_STATION_LENGTH			16
#define MAX_IP_LENGTH				16
#define MAX_ORDERNUM_LEN			20
#define MAX_IMEI_LENGTH				15
#define MAX_BT_ADD_LENGTH			12
#define MAX_WIFI_ADD_LENGTH			12
#define MAX_USERID_LENGTH			30
#define MAX_BATCH_LENGTH			30

template<typename T>
class MyArray
{
private:
    T* m_point;
    int m_length;
public:
    explicit MyArray()
    {
        m_point = nullptr;
    };
    virtual ~MyArray()
    {
        if(m_point != nullptr)
        {
            delete[] m_point;
            m_point = nullptr;
        }
    };
    virtual void SetLength(int length)
    {
        if(m_point != nullptr)
            delete[] m_point;

        if(length != 0)
            m_point = new T[length];
        m_length = length;
    };
    T& operator[](int index)
    {
        return m_point[index];
    };
    int GetLength()
    {
        return m_length;
    };
};

typedef enum
{
	MES_SUCCESS = 0,			//SQL执行成功
	MES_FAIL,					//SQL执行失败
	MES_DISABLE_FAIL,			//MES离线状态
	MES_UNCONNECT_FAIL,			//SQL未连接
	MES_EOF_FAIL,				//记录集结束
	MES_OPENRS_FAIL,			//打开Recordset失败
	MES_ADDNEW_FAIL,			//添加记录失败
	MES_PUTCOLLECT_FAIL,		//设置列数据失败
	MES_GETCOLLECT_FAIL,		//获得列数据失败
	MES_UPDATE_FAIL,			//更新记录集失败
	MES_REPEATEDRS_FAIL,		//重复的记录
	MES_CHECKDONE_FAIL,			//检查前面是否做过失败
	MES_CHECKOVERRUN_FAIL,		//检查超出失败，比如装箱大于设定值
	MES_CHECKNULLPARAM_FAIL,	//检查输入的参数为空
	MES_NetCode_REPEATEDRS_FAIL,	//网标重复
	MES_SN1_REPEATEDRS_FAIL,		//SN1重复
	MES_SN2_REPEATEDRS_FAIL,		//SN1重复
	MES_IMEI1_REPEATEDRS_FAIL,		//IMEI重复
	MES_IMEI2_REPEATEDRS_FAIL,		//IMEI2重复
	MES_IMEI3_REPEATEDRS_FAIL,		//IMEI3重复
	MES_IMEI4_REPEATEDRS_FAIL,		//IMEI4重复 
	MES_BTMAC_REPEATEDRS_FAIL,		//蓝牙地址重复
	MES_WIFIMAC_REPEATEDRS_FAIL,		//Wifi地址重复
	MES_WRITE_BINDING_FAIL,			//写绑定表失败
	MES_CURSTATION_NOT_EXIST_FAIL,		//检查站位时，发现当前站位不存在
	MES_CURSTATION_PASSED_FAIL,		//当前站位已测试pass
	MES_PRESTATION_NOTPASSED_FAIL,	//前一站未测试pass
	MES_TEST_LOCKED_FAIL,			//该SN已被锁定
	MES_CHECK_LOCKED_FAIL,			//检查MES锁定失败

	MES_RESULT_SUM,					//返回值类型总数
}MES_RESULT;

typedef struct _BATCH_INFO_T
{
	int nBatchID;
	char szBatchName[MAX_BATCH_LENGTH];
	char szProjectName[MAX_PROJECT_LENGTH];
	char szPreSN1[MAX_SN_LENGTH];
	char szPreSN2[MAX_SN_LENGTH];
	int nSN1_Length;
	int nSN2_Length;
	int nStatus;
	char szChipName[MAX_SWVERSION_LENGTH];
	char szSWVer[MAX_SWVERSION_LENGTH];
	char szMiniVer[MAX_SWVERSION_LENGTH];
	char szCustVer[MAX_SWVERSION_LENGTH];
	char szPcbVersion[50];
	char szElecBomName[MAX_SWVERSION_LENGTH];
	char szMechBomName[MAX_SWVERSION_LENGTH];
	char szScheduledDate[MAX_TIME_LENGTH];
	int nScheduledQuantity;
	char szManufactureDate[MAX_TIME_LENGTH];
	int nManufactureQuantity;
    int nSN1Start;
    int nSN1End;
    int nSN2Start;
    int nSN2End;
    int nIMEINumber;
    int nOperator;
	BOOL bFlowControl;
	int nLogLevel;
	int nFactoryID;
	_BATCH_INFO_T()
	{
		ZeroMemory(this, sizeof(*this));
	}
}BATCH_INFO_T, *PBATCH_INFO_T;

typedef struct _BATCH_STATION_CONFIG
{
	char szBatchName[50];
	char szStationName[30];
	int nMaxFailCount;

	_BATCH_STATION_CONFIG()
	{
		ZeroMemory(this,sizeof(*this));
	}
}BATCH_STATION_CONFIG, *PBATCH_STATION_CONFIG;

typedef struct _STATION_RESULT_STATISTIC
{
	//char szBatchName[50];
	//char szSN[50];
	char szStationName[50];
	int nStationID;
	int nLastTestResult;
	//int nConFailCount;
	//BOOL bLocked;

	_STATION_RESULT_STATISTIC()
	{
		ZeroMemory(this,sizeof(*this));
	}
}STATION_RESULT_STATISTIC, *PSTATION_RESULT_STATISTIC;

typedef std::map<int, STATION_RESULT_STATISTIC> MAP_STATION_RESULT, *PMAP_STATION_RESULT;
typedef std::map<int, BATCH_STATION_CONFIG> MAP_BATCH_STATION_CONFIG, *PMAP_BATCH_STATION_CONFIG;

//typedef struct _MES_LOCK_T
//{
//	char szSN[MAX_SN_LENGTH+1];
//	int nLockStationID;
//	//int nStatus;
//	char szReason[200];
//	int nAccountID;
//	char szLockTime[30];
//}MES_LOCK_T;

//typedef struct _TEST_INFO_T
//{
//	char szSN[30];
//	char szBatchName[30];
//	char szStation[20];
//	char szToolsVersion[200];
//	char szBenchMAC[30];
//
//	_TEST_INFO_T()
//	{
//		ZeroMemory(this, sizeof(*this));
//	}
//}TEST_INFO_T, *PTEST_INFO_T;

typedef struct _TEST_RESULT_T
{
	char szSN[30];
	char szBatchName[30];
	char szStation[20];
	char szToolsVersion[200];
	char szBenchMAC[30];
	int nResult;
	int nTestTime;
	char szInformation[500];
	char szErrCode[50];
	char szErrType[50];
	char szErrMsg[200];
	//char szStartTestTime[30];
	//char szEndTestTime[30];
    char szUploadTime[30];
	/*int nTestResultID;*/
	char szTestResultGUID[50];

	_TEST_RESULT_T()
	{
		ZeroMemory(this, sizeof(*this));
	}
}TEST_RESULT_T, *PTEST_RESULT_T;

typedef struct _TEST_VALUE_T
{
	char szSN[MAX_SN_LENGTH+1];
	char szTestItem[200];
	char szSubTestItem[200];
	float fTestValue;
	float fLowSpec;
	float fHighSpec;
	int nResult;
	int nTestTime;
	//char szStartTestTime[30];
	char szEndTestTime[30];
	char szTestInfo[200];
	/*int nTestResultID;*/
	char szTestResultGUID[50];
	//int nTestValueID;

	_TEST_VALUE_T()
	{
		ZeroMemory(this, sizeof(*this));
	}
}TEST_VALUE_T, *PTEST_VALUE_T;

//Write Code, wei.song 20150610
#define CODE_MASK_IMEI1		0x01	//bit0
#define CODE_MASK_IMEI2		0x02	//bit1
#define CODE_MASK_IMEI3		0x04	//bit2
#define CODE_MASK_IMEI4		0x08	//bit3
#define CODE_MASK_BT		0x10	//bit4
#define CODE_MASK_WIFI		0x20	//bit5
#define CODE_MASK_SN1		0x40	//bit6
#define CODE_MASK_SN2		0x80	//bit7
#define CODE_MASK_DL_SN		0x100	//bit8
#define CODE_MASK_NETCODE	0x200	//bit8

typedef struct _Code_Struct
{
	DWORD	dwCodeMask;

	char	szIMEI1[MAX_IMEI_LENGTH+1];
	char	szIMEI2[MAX_IMEI_LENGTH+1];
	char	szIMEI3[MAX_IMEI_LENGTH+1];
	char	szIMEI4[MAX_IMEI_LENGTH+1];
	char	szBT[MAX_BT_ADD_LENGTH+1];
	char	szWIFI[MAX_WIFI_ADD_LENGTH+1];
	char	szSN1[MAX_SN_LENGTH+1];
	char	szSN2[MAX_SN_LENGTH+1];
	char	szDLSN[MAX_SN_LENGTH+1];
	char	szNetCode[50];

	_Code_Struct()
	{
		ZeroMemory(this, sizeof(*this));
	}
}CODE_STRUCT, *PCODE_STRUCT;

typedef struct _CodeBind_Struct
{
    int ID;
    char* BatchName;
    char* SN1;
    char* SN2;
    char* DL_SN;
    char* IMEI1;
    char* IMEI2;
    char* IMEI3;
    char* IMEI4;
    char* BT;
    char* WIFI;
    char* NetCode;
    char* Operator;
    char* BindTime;
    int Status;

    _CodeBind_Struct()
    {
        BatchName = new char[50];
        memset(BatchName, '\0', 50);
        SN1 = new char[30];
        memset(SN1, '\0', 30);
        SN2 = new char[30];
        memset(SN2, '\0', 30);
        DL_SN = new char[30];
        memset(DL_SN, '\0', 30);
        IMEI1 = new char[20];
        memset(IMEI1, '\0', 20);
        IMEI2 = new char[20];
        memset(IMEI2, '\0', 20);
        IMEI3 = new char[20];
        memset(IMEI3, '\0', 20);
        IMEI4 = new char[20];
        memset(IMEI4, '\0', 20);
        BT = new char[15];
        memset(BT, '\0', 15);
        WIFI = new char[15];
        memset(WIFI, '\0', 15);
        NetCode = new char[50];
        memset(NetCode, '\0', 50);
        Operator = new char[50];
        memset(Operator, '\0', 50);
        BindTime = new char[20];
        memset(BindTime, '\0', 20);
    }

    ~_CodeBind_Struct()
    {
        delete[] BatchName;
        delete[] SN1;
        delete[] SN2;
        delete[] DL_SN;
        delete[] IMEI1;
        delete[] IMEI2;
        delete[] IMEI3;
        delete[] IMEI4;
        delete[] BT;
        delete[] WIFI;
        delete[] NetCode;
        delete[] Operator;
        delete[] BindTime;
    }
}CODEBIND_STRUCT, *PCODEBIND_STRUCT;

typedef struct _Account_Struct
{
    int AccountID;
    char* AccountName;
    char* Password;
    char* Privilege;
    byte Status;
	int nFactoryID;
	int nLineID;
	int nBenchID;

    _Account_Struct()
    {
        AccountID = 0;
        AccountName = new char[50];
        memset(AccountName, '\0', 50);
        Password = new char[50];
        memset(Password, '\0', 50);
        Privilege = new char[255];
        memset(Privilege, '\0', 255);
        Status = 0;
		nFactoryID = 0;
		nLineID = 0;
		nBenchID = 0;
    }

    ~_Account_Struct()
    {
        delete[] AccountName;
        delete[] Password;
        delete[] Privilege;
    }
}ACCOUNT_STRUCT, *PACCOUNT_STRUCT;

typedef struct _Privilege_Struct
{
    int ID;
    char* Name;

    _Privilege_Struct()
    {
        ID = 0;
        Name = new char[200];
        memset(Name, '\0', 200);
    }

    ~_Privilege_Struct()
    {
        delete[] Name;
    }
}PRIVILEGE_STRUCT, *PPRIVILEGE_STRUCT;

typedef struct _Factory_Struct
{
    int ID;
    char* Name;
    char* City;
    char* Region;
    char* Country;
    byte Status;

    _Factory_Struct()
    {
        ID = 0;
        Name = new char[200];
        memset(Name, '\0', 200);
        City = new char[100];
        memset(City, '\0', 100);
        Region = new char[50];
        memset(Region, '\0', 50);
        Country = new char[50];
        memset(Country, '\0', 50);
        Status = 0;
    }

    ~_Factory_Struct()
    {
        delete[] Name;
        delete[] City;
        delete[] Region;
        delete[] Country;
    }
}FACTORY_STRUCT, *PFACTORY_STRUCT;

typedef struct _Line_Struct
{
    int ID;
    char* Name;
    byte Status;

    _Line_Struct()
    {
        ID = 0;
        Name = new char[200];
        memset(Name, '\0', 200);
        Status = 0;
    }

    ~_Line_Struct()
    {
        delete[] Name;
    }
}LINE_STRUCT, *PLINE_STRUCT;

typedef struct _Bench_Struct
{
    int ID;
    char* Name;
    char* MAC;
    int LineID;
    int FactoryID;

    _Bench_Struct()
    {
        Name = new char[200];
        memset(Name, '\0', 200);
        MAC = new char[100];
        memset(MAC, '\0', 100);
    }

    ~_Bench_Struct()
    {
        delete[] Name;
        delete[] MAC;
    }
}BENCH_STRUCT, *PBENCH_STRUCT;

typedef struct _Station_Struct
{
    int ID;
    char Name[MAX_STATION_LENGTH];
    byte Status;

    _Station_Struct()
    {
        /*Name = new char[200];
		memset(Name, '\0', 200);*/
        memset(this, '\0', sizeof(*this));
    }

    /*~_Station_Struct()
    {
        delete[] Name;
    }*/
}STATION_STRUCT, *PSTATION_STRUCT;

typedef struct _BSC_Struct
{
    int StationID;
    char* BatchName;
    int MaxFailCount;

    _BSC_Struct()
    {
        BatchName = new char[50];
        memset(BatchName, '\0', 50);
    }

    ~_BSC_Struct()
    {
        delete[] BatchName;
    }
}BSC_STRUCT, *PBSC_STRUCT;

typedef struct _ErrType_Struct
{
    int ID;
    char* Name;
    char* Note;

    _ErrType_Struct()
    {
        Name = new char[50];
        memset(Name, '\0', 50);
        Note = new char[200];
        memset(Note, '\0', 200);
    }

    ~_ErrType_Struct()
    {
        delete[] Name;
        delete[] Note;
    }
}ERRTYPE_STRUCT, *PERRTYPE_STRUCT;


typedef struct _Owner_Struct
{
    int ID;
    char* Name;

    _Owner_Struct()
    {
        Name = new char[50];
        memset(Name, '\0', 50);
    }

    ~_Owner_Struct()
    {
        delete[] Name;
    }
}OWNER_STRUCT, *POWNER_STRUCT;

typedef struct _Repair_Code_Struct
{
    char* RepairCode;
    char* RepairMsg;

    _Repair_Code_Struct()
    {
        RepairCode = new char[100];
        memset(RepairCode, '\0', 100);
        RepairMsg = new char[500];
        memset(RepairMsg, '\0', 500);
    }

    ~_Repair_Code_Struct()
    {
        delete[] RepairCode;
        delete[] RepairMsg;
    }
}REPAIR_CODE_STRUCT, *PREPAIR_CODE_STRUCT;

typedef struct _Repair_Struct
{
    int index;
    char* SN;
    char* BatchName;
    int StationID;
    char* ErrMsg;
    char* ErrAnanlysis;
    char* RepairCode;
    int OwnerID;
    int Ananlysier;
    char* StartTime;
    char* EndTime;

    _Repair_Struct()
    {
        SN = new char[50];
        memset(SN, '\0', 50);
        BatchName = new char[50];
        memset(BatchName, '\0', 50);
        ErrMsg = new char[400];
        memset(ErrMsg, '\0', 400);
        ErrAnanlysis = new char[500];
        memset(ErrAnanlysis, '\0', 500);
        RepairCode = new char[100];
        memset(RepairCode, '\0', 100);
        StartTime = new char[50];
        memset(StartTime, '\0', 50);
        EndTime = new char[50];
        memset(EndTime, '\0', 50);
    }

    ~_Repair_Struct()
    {
        delete[] SN;
        delete[] BatchName;
        delete[] ErrMsg;
        delete[] ErrAnanlysis;
        delete[] StartTime;
        delete[] EndTime;
    }
}REPAIR_STRUCT, *PREPAIR_STRUCT;

typedef struct _ErrInfo_Struct
{
    char* BatchName;
    int StationID;
    char* ErrCode;
    char* ErrType;
    char* ErrMsg;

    _ErrInfo_Struct()
    {
        BatchName = new char[50];
        memset(BatchName, '\0', 50);
        ErrCode = new char[50];
        memset(ErrCode, '\0', 50);
        ErrType = new char[50];
        memset(ErrType, '\0', 50);
        ErrMsg = new char[200];
        memset(ErrMsg, '\0', 200);
    }

    ~_ErrInfo_Struct()
    {
        delete[] BatchName;
        delete[] ErrCode;
        delete[] ErrType;
        delete[] ErrMsg;
    }
}ERRINFO_STRUCT, *PERRINFO_STRUCT;

typedef struct _Package_Struct
{
    int Index;
    char* SN;
    char* BatchName;
    int Status;
    int Result;
    char* PackageTime;

    _Package_Struct()
    {
        SN = new char[25];
        memset(SN, '\0', 25);
        BatchName = new char[50];
        memset(BatchName, '\0', 50);
        PackageTime = new char[20];
        memset(PackageTime, '\0', 20);
    }

    ~_Package_Struct()
    {
        delete[] SN;
        delete[] BatchName;
        delete[] PackageTime;
    }
}PACKAGE_STRUCT, *PPACKAGE_STRUCT;

typedef struct _SMT_Struct
{
    char* BatchName;
    char* BOM;
    char* SerialNumber;
    char* Description;
    int QTY;
    char* Reference;
    char* ManuFacture;
    char* PAndN;
    char* Lot;
    char* DateCode;
    char* Time;

    // Default constructor
    _SMT_Struct()
    {
        BatchName = new char[50];
        memset(BatchName, '\0', 50);
        BOM = new char[200];
        memset(BOM, '\0', 200);
        SerialNumber = new char[30];
        memset(SerialNumber, '\0', 30);
        Description = new char[200];
        memset(Description, '\0', 200);
        Reference = new char[200];
        memset(Reference, '\0', 200);
        ManuFacture = new char[200];
        memset(ManuFacture, '\0', 200);
        PAndN = new char[200];
        memset(PAndN, '\0', 200);
        Lot = new char[200];
        memset(Lot, '\0', 200);
        DateCode = new char[200];
        memset(DateCode, '\0', 200);
        Time = new char[20];
        memset(Time, '\0', 20);
    }

    // Copy constructor
    _SMT_Struct(const _SMT_Struct& other)
    {
        BatchName = new char[50];
        strcpy(BatchName, other.BatchName);
        BOM = new char[200];
        strcpy(BOM, other.BOM);
        SerialNumber = new char[30];
        strcpy(SerialNumber, other.SerialNumber);
        Description = new char[200];
        strcpy(Description, other.Description);
        Reference = new char[200];
        strcpy(Reference, other.Reference);
        ManuFacture = new char[200];
        strcpy(ManuFacture, other.ManuFacture);
        PAndN = new char[200];
        strcpy(PAndN, other.PAndN);
        Lot = new char[200];
        strcpy(Lot, other.Lot);
        DateCode = new char[200];
        strcpy(DateCode, other.DateCode);
        Time = new char[20];
        strcpy(Time, other.Time);
        QTY = other.QTY;
    }

    ~_SMT_Struct()
    {
        delete[] BatchName;
        delete[] BOM;
        delete[] SerialNumber;
        delete[] Description;
        delete[] Reference;
        delete[] ManuFacture;
        delete[] PAndN;
        delete[] Lot;
        delete[] DateCode;
        delete[] Time;
    }

}SMT_STRUCT, *PSMT_STRUCT;


SPRD_MES_API MES_RESULT MES_Create();
SPRD_MES_API BOOL MES_EnableCheck();
SPRD_MES_API MES_RESULT MES_Connect(void);
SPRD_MES_API void MES_Disconnect(void);
SPRD_MES_API void MES_GetLastError(char* szError);
SPRD_MES_API void MES_Release();

SPRD_MES_API void MES_GetCurrentTime(char* szCurrentTime);
SPRD_MES_API void MES_GetCurrentDate(char* szCurrentDate);
SPRD_MES_API MES_RESULT MES_GetServerTime(char* szServerTime);

SPRD_MES_API MES_RESULT MES_GetCurAccountPriv(char* szpriv);

SPRD_MES_API MES_RESULT MES_AddBatch(PBATCH_INFO_T newbatch);
SPRD_MES_API MES_RESULT MES_QueryBatch(MyArray<BATCH_INFO_T>& result, const char* batchname = nullptr);
SPRD_MES_API MES_RESULT MES_ModifyBatch(PBATCH_INFO_T newbatch);
SPRD_MES_API MES_RESULT MES_DeleteBatch(int id);

SPRD_MES_API MES_RESULT MES_BatchSelectUI(char* szBatchName);
SPRD_MES_API MES_RESULT MES_QueryBatchName(char* szBatchName);
SPRD_MES_API MES_RESULT MES_GetBatchName(char* szBatchName);
SPRD_MES_API MES_RESULT MES_GetBatchInfo(char* szBatchName, PBATCH_INFO_T pBatchInfo);

SPRD_MES_API MES_RESULT MES_GetCurrentStationName(char* szStationName);
SPRD_MES_API MES_RESULT MES_CheckPreStation(char* szBatchName, char* szSN);
SPRD_MES_API MES_RESULT MES_CheckAllPreStation(char* szBatchName, char* szSN);

SPRD_MES_API MES_RESULT MES_InitTest(/*char* szSN, char* szBatchName,*/ /*OUT*//*int *nTestResultID,*/ /*OUT*/char* szTestResultGUID);
SPRD_MES_API MES_RESULT MES_WriteTestResult(PTEST_RESULT_T pTestResult);
SPRD_MES_API MES_RESULT MES_WriteTestValue(PTEST_VALUE_T pTestValue, int nLevel = 1);
SPRD_MES_API MES_RESULT MES_UploadMdbData(BOOL bStart);  //离线上传mdb文件中的数据（TestValue）
//SPRD_MES_API int MES_WriteTestResultStatistic(EM_STATION_TYPE emStation, PTEST_RESULT_T pTestResult);
SPRD_MES_API MES_RESULT MES_QueryTestResult(MyArray<TEST_RESULT_T>& result, const char* batchname = "", const char* sn = "");



//Download
SPRD_MES_API MES_RESULT MES_GetDownloadSN(char* szBatchName, char* szDownloadSN);
SPRD_MES_API MES_RESULT MES_ReclaimDownloadSN(char* szBatchName, char* szDownloadSN);  //Must Reclaim DL_SN when download fail

//WriteSN
//SPRD_MES_API int MES_Write
SPRD_MES_API MES_RESULT MES_WriteSN(PCODE_STRUCT pCodeInfo, char* szTestResultGUID);

//WriteIMEI
SPRD_MES_API MES_RESULT MES_GetIMEIbySN(BOOL bSN1, PCODE_STRUCT pCodeInfo);
SPRD_MES_API MES_RESULT MES_WriteIMEIbySN(char* pSN);
SPRD_MES_API MES_RESULT MES_WriteCodeInfo(PCODE_STRUCT pCodeInfo, char* szTestResultGUID);
SPRD_MES_API MES_RESULT MES_GetAssignedCode(PCODE_STRUCT pInput, PCODE_STRUCT pOutput);  //获取待分配的号码，用于写入手机中

//CheckCode
SPRD_MES_API MES_RESULT MES_GetDeviceCode(PCODE_STRUCT pInput, PCODE_STRUCT pOutput);  //获取已经写入手机的号码，用于检查或打印


SPRD_MES_API MES_RESULT MES_GetLastIMEI(char* imei);
SPRD_MES_API MES_RESULT MES_AddCodeBind(const char* batch, const char* startsn, const int snquanlity,
    const char* lastimei, const int imeinum);
SPRD_MES_API MES_RESULT MES_DeleteCodeBind(const char* batch);
SPRD_MES_API MES_RESULT MES_QueryCodeBindInfo(MyArray<CODEBIND_STRUCT>& result, const char* batch = "", const int status = 0);

//Account Management
SPRD_MES_API MES_RESULT MES_QueryPriv(MyArray<PRIVILEGE_STRUCT>& result, int id = -1, const char* name = nullptr);
SPRD_MES_API MES_RESULT MES_AddPriv(const char* name);
SPRD_MES_API MES_RESULT MES_ModifyPriv(int id, const char* name);
SPRD_MES_API MES_RESULT MES_DeletePriv(int id);
SPRD_MES_API MES_RESULT MES_GetPrivNameByID(const char* privid, char* privname);

SPRD_MES_API MES_RESULT MES_QueryAccount(MyArray<ACCOUNT_STRUCT>& result, int id = -1, const char* name = nullptr, int status = -1);
SPRD_MES_API MES_RESULT MES_AddAccount(PACCOUNT_STRUCT newaccount);
SPRD_MES_API MES_RESULT MES_BatchAddAccount(int startid, char* name, char* priv, int count);
SPRD_MES_API MES_RESULT MES_ModifyAccount(PACCOUNT_STRUCT newaccount);
SPRD_MES_API MES_RESULT MES_DeleteAccount(int id);

SPRD_MES_API MES_RESULT MES_QueryFactory(MyArray<FACTORY_STRUCT>& result, 
    int id = -1, 
    const char* name = nullptr, 
    const char* city = nullptr, 
    const char* region = nullptr, 
    const char* country = nullptr, 
    int status = -1);
SPRD_MES_API MES_RESULT MES_AddFactory(PFACTORY_STRUCT newfactory);
SPRD_MES_API MES_RESULT MES_ModifyFactory(PFACTORY_STRUCT newfactory);
SPRD_MES_API MES_RESULT MES_DeleteFactory(int id);

SPRD_MES_API MES_RESULT MES_QueryLine(MyArray<LINE_STRUCT>& result, int id = -1, const char* name = nullptr, int status = -1);
SPRD_MES_API MES_RESULT MES_AddLine(PLINE_STRUCT newline);
SPRD_MES_API MES_RESULT MES_ModifyLine(PLINE_STRUCT newline);
SPRD_MES_API MES_RESULT MES_DeleteLine(int id);

SPRD_MES_API MES_RESULT MES_QueryBench(MyArray<BENCH_STRUCT>& result, 
    int id = -1, 
    const char* name = nullptr, 
    const char* IP = nullptr,
    int lineid = -1, 
    int factoryid = -1);
SPRD_MES_API MES_RESULT MES_AddBench(PBENCH_STRUCT newbench);
SPRD_MES_API MES_RESULT MES_ModifyBench(PBENCH_STRUCT newbench);
SPRD_MES_API MES_RESULT MES_DeleteBench(int id);

SPRD_MES_API MES_RESULT MES_QueryStation(MyArray<STATION_STRUCT>& result, int id = -1, const char* name = nullptr, int status = -1);
SPRD_MES_API MES_RESULT MES_AddStation(PSTATION_STRUCT newstation);
SPRD_MES_API MES_RESULT MES_ModifyStation(PSTATION_STRUCT newstation);
SPRD_MES_API MES_RESULT MES_DeleteStation(int id);
SPRD_MES_API MES_RESULT MES_GetReturnStation(MyArray<STATION_STRUCT>& result, const char* batch, int stationid);

SPRD_MES_API MES_RESULT MES_GetBatchStationConfig(MyArray<BSC_STRUCT>& result, const char* batch);
SPRD_MES_API MES_RESULT MES_SetBatchStationConfig(MyArray<BSC_STRUCT>& result, const char* batch);

SPRD_MES_API MES_RESULT MES_GetErrType(MyArray<ERRTYPE_STRUCT>& result, int id = -1, const char* name = nullptr);

SPRD_MES_API MES_RESULT MES_GetOwner(MyArray<OWNER_STRUCT>& result);
SPRD_MES_API MES_RESULT MES_GetErrInfo(PERRINFO_STRUCT result, const char* SN);

SPRD_MES_API MES_RESULT MES_QueryRepairCode(MyArray<REPAIR_CODE_STRUCT>& result);
SPRD_MES_API MES_RESULT MES_QueryRepairInfo(MyArray<REPAIR_STRUCT>& result, const char* SN);
SPRD_MES_API MES_RESULT MES_WriteRepairInfo(PREPAIR_STRUCT info);
SPRD_MES_API MES_RESULT MES_Unlock(const char* batch, const char* SN, int stationid);

SPRD_MES_API MES_RESULT MES_QueryPackageInfo(MyArray<PACKAGE_STRUCT>& result, 
    const char* batchname = nullptr, 
    const char* SN = nullptr,
    int status = -1,
    const char* starttime = nullptr,
    const char* endtime = nullptr);
SPRD_MES_API MES_RESULT MES_AddPackageInfo(PPACKAGE_STRUCT info);
SPRD_MES_API MES_RESULT MES_DeletePackageInfo(int id);

SPRD_MES_API MES_RESULT MES_QuerySMTInfo(MyArray<SMT_STRUCT> &result, const char* batchname = "", const char* BOM = "");
SPRD_MES_API MES_RESULT MES_AddSMTInfo(PSMT_STRUCT smtinfo);
SPRD_MES_API MES_RESULT MES_SMTDone(const char* batchname);
SPRD_MES_API MES_RESULT MES_CheckSMTDone(const char* batchname);
SPRD_MES_API MES_RESULT MES_ImportBOMInfo(vector<SMT_STRUCT> &bominfo);