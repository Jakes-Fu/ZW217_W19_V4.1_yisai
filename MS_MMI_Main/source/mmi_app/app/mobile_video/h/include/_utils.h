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
	EMachineInfo_Serial,	//����Ψһ���к�,�ַ���
	EMachineInfo_Signal,	//�ź���,0-100������
	EMachineInfo_Battery,	//�ź���,BATTERYINFO	�ṹ
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
	_INT	m_nStatus;	//0��ʾ����״̬, 1��ʾ���״̬
	_INT	m_nValue;	//����0-100������
};

extern _BOOLEAN GetMachineInfo(MachineInfoEnum nIndex, _PVOID pBuffer, _INT nSize);
extern _DWORD GetDiskSpace(_CONST _WCHAR * strDiskPath);			//***2009-05-27 : ��KBΪ��λ
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
