#ifndef _GENERAL_PARAM_H_
#define _GENERAL_PARAM_H_

#define ON_RUN_PRESS				(WM_USER+1)
#define ON_ABORT_RELEASE			(WM_USER+2)
#define ON_MSG_RESPONSE				(WM_USER+3)

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

#define COLOR_IDLE      RGB(100, 150, 200)  
#define COLOR_WORKING   RGB(255, 255, 000)
#define COLOR_PASS      RGB(000, 128, 000)
#define COLOR_FAIL      RGB(255, 000, 000)
#define COLOR_PGREEN    RGB(003, 253, 027)
#define COLOR_ORANGE    RGB(254, 168, 001)
#define COLOR_GREEN     RGB(000, 255, 000)
#define COLOR_GRAY      RGB(190, 190, 190)



#define RFT_SUCCESS     0
#define RFT_FAIL	    1

const int MAX_BUFFLEN						= 1024;
const int MAX_PATH_LEN						= 1024;
const unsigned int	TOTAL_THREAD			= 10;
const unsigned char CONTROL_FLAG_START		= 0x00;
const unsigned char CONTROL_FLAG_RUNNING	= 0x01;
const unsigned char CONTROL_FLAG_COMPLETE	= 0x02;
const unsigned char CONTROL_FLAG_CANCLE		= 0x03;
const unsigned char CONTROL_FLAG_FAIL		= 0x04;
const unsigned char CONTROL_FLAG_PASS		= 0x05;
const unsigned char CONTROL_FLAG_EXIT		= 0x06;
const unsigned char CONTROL_FLAG_TIMEOUT	= 0x07;

const unsigned char MSG_FLAG_GREED			= 0x00;
const unsigned char MSG_FLAG_RED			= 0x01;

typedef enum
{
	MSGFMT_DEFTEXT = -1,
	MSGFMT_REDTEXT = 0x00000001,
	MSGFMT_GRNTEXT = 0x00000002,
	MSGFMT_BUETEXT = 0x00000004,
	MSGFMT_BIGBOLD = 0x00001000
}COLOR;

typedef enum
{
	Role_Write_Tosql,
	Role_Query_Fromsql,
	Role_Export_ToXls,
	Role_Statistics_Fromsql
}ROLETYPE;

typedef enum
{
	TEST_FACTORYDOWNLOAD,
	TEST_WRITESN,
	TEST_CFT,
	TEST_ANTENNA,
	TEST_MMI,
	TEST_IMEI,
	TEST_UPGRADE,
	TEST_PACKAGE,
	TEST_STATISTICS
}STATIONTYPE;


typedef enum
{
	RESULT_FLAG_NONE,
	RESULT_FLAG_PASS,
	RESULT_FLAG_FAIL,
	RESULT_FLAG_WORKING,
	RESULT_FLAG_TIMEOUT,
}STATUSTYPE;

typedef enum{
	Save_Mdb,
	save_Sql,
	save_MySql,
	Save_None,
}SAVEIMEITYPE;


typedef enum{
	PROJECT_GENERAL,
	PROJECT_SHARK_WG,
	PROJECT_SHARK_TG
}PROJECTTYPE;



typedef struct{
	HWND				hwnd;
	DWORD				dwThreadId;
	HANDLE				hStartEvent;
	HANDLE				hThreadHandle;
	int					m_nTimeOut;

	int					nSnLength;
	unsigned char		nCkLastState;
	unsigned char		nSaveMode;
	unsigned char		nStationMode;
	unsigned char		nPreStationMode;
	BOOL				bConnectSql;

	int					*m_nCount;
	int					*m_nPassCount;
	int					*m_nOnePass;
	int					*m_nRePass;

	unsigned int		m_nPass;
	unsigned int		m_nTotal;
	unsigned int		uiErrorCode;
	unsigned char		byControlFlag;
	unsigned char		nWorkMode;
	
	
	TCHAR				*sSqlServer;
	TCHAR				*sSqlSheet;
	TCHAR				*sSqlPid;
	TCHAR				*sSqlPwd;
	TCHAR				*sMySqlDriver;
	
	TCHAR				*sDebugMsg;
	int					*nId;
	TCHAR				*sSn;
	TCHAR				*sProject;
	TCHAR				*sOrderNum; //wei.song 20140828
	TCHAR				*sInformation;
	TCHAR				*sStatusType;
	TCHAR				*sPreStation;
	TCHAR				*sCurStation;
	TCHAR				*sAddress;
	TCHAR				*sDateTime;
	TCHAR				*sElapseTime;
	TCHAR				*sErrType;
	TCHAR				*sErrCode;
	TCHAR				*sErrMessage;
	TCHAR				*sSwVersion;
	TCHAR				*sToolVersion;
	TCHAR				*sTime;

	TCHAR				*sAccount;
	TCHAR				*sPassword;
	TCHAR				*sPrivilege;
	
}THREADPARAM, *PTHREADPARAM;


#endif


