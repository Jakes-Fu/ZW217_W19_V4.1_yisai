//
//	Utils.h
//
#ifndef __UTILS_API_H
#define	__UTILS_API_H

#include "_Types.h"

#ifdef	__cplusplus
extern "C"
{
#endif
#if 0
typedef enum
{
	EMachineInfo_Serial,	//机器唯一序列号,字符串
	EMachineInfo_Signal,	//信号量,0-100的整数
	EMachineInfo_Battery,	//信号量,BATTERYINFO	结构
} MachineInfoEnum;
#else
typedef enum 
{
	EMachine_Serial,
	EMachine_OS,
	EMachine_Lang,
	EMachine_BatteryStrength,
	EMachine_SignalStrength,
}MachineInfoEnum;
#endif
//#define   _TESTVER					1
#define	MAX_SIGNAL_VALUE	(100)
#define	MAX_BATTERY_VALUE	(100)

typedef struct tagBATTERYINFO	BATTERYINFO;
struct tagBATTERYINFO
{
	_INT	m_nStatus;	//0表示正常状态, 1表示充电状态
	_INT	m_nValue;	//电量0-100的整数
};

extern _BOOLEAN GetMachineInfo(MachineInfoEnum nIndex, _PVOID pBuffer, _INT nSize);
extern _DWORD GetDiskSpace(_CONST _WCHAR * strDiskPath);			//***2009-05-27 : 以KB为单位
extern _BOOLEAN GetScreenRect(_RECT* rect);

//extern _INT  GetScreenWidth(_VOID);
//extern _INT  GetScreenHeight(_VOID);

extern _VOID ExitApplication(_INT nCode);

extern  _VOID GetDevVer(_WCHAR* pString, _INT nLen);
extern _BOOLEAN InstallApp(_CONST _WCHAR * strDiskPath);
extern _BOOLEAN LoadInstallFileExtension(_WCHAR * strDiskPath, _INT nSize);

#ifdef	__cplusplus
}
#endif

#endif	//__UTILS_H
