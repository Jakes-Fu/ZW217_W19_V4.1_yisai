// Command.h: interface for the CCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMAND_H__578B1F4B_7357_4268_A3C4_52B3D675DC17__INCLUDED_)
#define AFX_COMMAND_H__578B1F4B_7357_4268_A3C4_52B3D675DC17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Diag_win32.h"
#include "DiagChan.h"
#include "typedef.h"

typedef enum
{
	GET_AUDIO_MODE_COUNT,
	GET_AUDIO_MODE_NAME,
	GET_AUDIO_DATA_FROM_RAM,
	SET_AUDIO_DATA_TO_RAM,
	SET_AUDIO_MODE_NAME,
	SET_AUDIO_DATA_TO_FLASH,
	GET_AUDIO_DATA_FROM_FLASH,
	GET_ARM_VOLUME_MODE_COUNT,
	GET_ARM_VOLUME_MODE_NAME,
	GET_ARM_VOLUME_DATA_FROM_RAM,
	SET_ARM_VOLUME_DATA_TO_RAM,
	SET_ARM_VOLUME_DATA_TO_FLASH,
	GET_ARM_VOLUME_DATA_FROM_FLASH
}AUDIO_CMD_E;

typedef enum
{
	GET_ENHA_MODE_COUNT,
	GET_ENHA_DATA_FROM_RAM,
	SET_ENHA_DATA_TO_RAM,
	GET_ENHA_DATA_FROM_FLASH,
	SET_ENHA_DATA_TO_FLASH
}EQ_G_CMD_E;

typedef enum
{
	GET_LVVE_MODE_COUNT,
	GET_LVVE_MODE_NAME,
	GET_LVVE_DATA_FROM_RAM,
	SET_LVVE_DATA_TO_RAM,
	GET_LVVE_DATA_FROM_FLASH,
	SET_LVVE_DATA_TO_FLASH

}LVVE_CMD_E;

typedef enum
{
	TYPE_EQ,
	TYPE_TUN
};

typedef enum
{
	GET_AUDIO_DSP_MODE_COUNT,
	GET_AUDIO_DSP_MODE_NAME,
	GET_AUDIO_DSP_DATA_FROM_RAM,
	SET_AUDIO_DSP_DATA_TO_RAM,
	GET_AUDIO_DSP_CONTROL_MODE,
	SET_AUDIO_DSP_DATA_TO_FLASH,
	GET_AUDIO_DSP_DATA_FROM_FLASH,
};

typedef enum
{
	LVVE_NO_RESERVE,
		LVVE_RESERVE,
		LVVE_JUNIPER,
		LVVE_SINGLE_MIC,
		LVVE_DUAL_MIC
}LVVE_TYPE_E;


class CCommand : public IProtocolObserver
{
public:
	CCommand();
	virtual ~CCommand();

public:
    BOOL Connect( LPCSTR lpszAddr,DWORD dwPort );
	BOOL Connect( DWORD dwPort,DWORD dwBaudrate );
    BOOL Disconnect();
    
    // Read audio mode count from phone
    BOOL GetModeCount( int& nModeCount );

    // Read audio mode name from phone
    BOOL GetModeName( int nModeIndex,  CString& strModeName );

    // Read audio structure from phone
    BOOL GetModeData( int nModeIndex, int nType,AUDIO_STRUCT& audio_data );

    // Write audio structure to phone
    BOOL SetModeData( int nModeIndex,CString strModeName,AUDIO_STRUCT& audio_data );
    
    // Write audio structure to running nv
    BOOL SetModeData2Flash( int nModeIndex,CString strModeName,AUDIO_STRUCT& audio_data );

    // Set volume of phone
    BOOL SetVolume( int nVolume );

    // Set speaker status
    BOOL SetSpeaker( int nSpeaker );

    // Set mute status
    BOOL SetMute( int nMute );

    // Make a phone call
    BOOL MakeCall( CString strPhoneNumber );

    BOOL Hangup();
	
	//open mix mode
	BOOL OpenMixMode();

	//close arm log
	BOOL CloseArmLog();

	BOOL CloseDeepSleep();

    // Return audio data size received in last GetAudioData
    int GetAudioDataSize(){ return m_nAudioDataSize; }

    void SetAudioDataSize( int nSize ){ m_nAudioDataSize = nSize; }
	
	//teana hu 2010.06.08
	// Read eq count from phone
    BOOL GetEQCount( int& nEQCount );

    // Read eq structure from phone
    BOOL GetEQData( int nEQIndex,AUDIO_EQ_STRUCT_T& eq_data, int nPos = 0 ); //nPos = 0 : ram; 1: flash

    // Write eq structure to phone
    BOOL SetEQData( int nEQIndex,AUDIO_EQ_STRUCT_T& eq_data, int nPos = 0 );

	// Return audio data size received in last GetAudioData
    int GetEQDataSize(){ return m_nEQDataSize; }

	void SetEQDataSize( int nSize ){ m_nEQDataSize = nSize; }
	//
	
	//teana hu 2011.02.16
	// Read audio mode count from phone
    BOOL GetVolumeModeCount( int& nModeCount );

	BOOL GetVolumeModeName( int nModeIndex,  CString& strModeName );
	
    // Read volume structure from phone
    BOOL GetVolumeData( int nIndex, int nType, VOLUME_STRUCT& volume_data );

    // Write volume structure to phone
    BOOL SetVolumeData( int nIndex, int nType, CString strModeName, VOLUME_STRUCT& volume_data);

	// Return volume data size received in last GetAudioData
    int GetVolumeDataSize(){ return m_nVolumeDataSize; }

	void SetVolumeDataSize( int nSize ){ m_nVolumeDataSize = nSize; }
	//

	//teana hu 2011.05.09
	BOOL GetVolumeSerailCount(int& nCount);
	//
	
	//teana hu 2011.03.22
	BOOL GetEQTunModeCount( int& nModeCount, int nType);

	BOOL GetEQGData( int nIndex, int nPos, EQ_STRUCT_G_T& eq_data);

	BOOL GetTunData( int nIndex, int nPos, TUN_STRUCT_G_T& tun_data);

	BOOL SetEQGData( int nIndex, int nPos, EQ_STRUCT_G_T& eq_data, int nEQModeSelType);

	BOOL SetTunData( int nIndex, int nPos, TUN_STRUCT_G_T& tun_data, int nEQModeSelType);

	BOOL SetVolumePara( int nVolumeIndex, int nApptype );
	//

	//teana hu 2011.04.14
	BOOL GetLVVEModeCount( int& nModeCount );
	
	BOOL GetLVVEModeName( int nModeIndex,  CString& strModeName );
	
    BOOL GetLVVEData( int nIndex, int nType, LVVE_CTRL_PARAM_NV_T& lvve_data );
	
    BOOL SetLVVEData( int nIndex, int nType, CString strModeName, LVVE_CTRL_PARAM_NV_T& lvve_data);

    int GetLVVEDataSize(){ return m_nLVVEDataSize; }
	
	void SetLVVEDataSize( int nSize ){ m_nLVVEDataSize = nSize; }

	void SetLVVEType(DWORD dwLvveType){m_dwLVVEType = dwLvveType;}

	//

	//teana hu 2012.01.31
	BOOL GetDspModeCount( int& nModeCount );
	
	BOOL GetDspModeName( int nModeIndex,  CString& strModeName );
	
    BOOL GetDspData( int nIndex, int nType, AUDIO_NV_DSP_MODE_STRUCT_T& dsp_data );
	
    BOOL SetDspData( int nIndex, int nType, CString strModeName, AUDIO_NV_DSP_MODE_STRUCT_T& dsp_data);

	BOOL GetDspCodecInfo(short& nDspCpdecInfo);
	
    int GetDspDataSize(){ return m_nDspModeSize; }
	
	void SetDspDataSize( int nSize ){ m_nDspModeSize = nSize; }
	//

	//teana hu 2011.04.22
	BOOL SetAudMode(CString strModeName);
	//

	//teana hu 2011.05.09
	void CopyVolumeStructFrom8ToMax(VOLUME_STRUCT &volume_data_16, VOLUME_STRUCT_MIN &volume_data_8);
	void CopyVolumeStructFromMaxTo8(VOLUME_STRUCT_MIN &volume_data_8, VOLUME_STRUCT &Svolume_data_16);
	//
	void CopyVolumeStructFrom1ToMax(VOLUME_STRUCT &volume_data_16, VOLUME_STRUCT_ONE &volume_data_1);
	void CopyVolumeStructFromMaxTo1(VOLUME_STRUCT_ONE &volume_data_1, VOLUME_STRUCT &Svolume_data_16);
	
	//teana hu 2011.06.07
	void CopyLVVEFromMinToMax(LVVE_CTRL_PARAM_NV_T &lvve_data, LVVE_CTRL_PARAM_NV_MIN_T &lvve_data_min);
	void CopyLVVEFromMaxToMin(LVVE_CTRL_PARAM_NV_MIN_T &lvve_data_min, LVVE_CTRL_PARAM_NV_T &lvve_data);
	BOOL LVVEIsMaxOrMin();
	//

	//teana hu 2012.01.20
	void CopyVolumeStructFrom6ToMax(VOLUME_STRUCT &volume_data_16, VOLUME_STRUCT_SIX &volume_data_6);
	void CopyVolumeStructFromMaxTo6(VOLUME_STRUCT_SIX &volume_data_6, VOLUME_STRUCT &volume_data_16);
	void CopyVolumeStructFrom16ToMax(VOLUME_STRUCT &volume_data, VOLUME_STRUCT_SIXTEEN &volume_data_16);
	void CopyVolumeStructFromMaxTo16(VOLUME_STRUCT_SIXTEEN &volume_data_16, VOLUME_STRUCT &volume_data);
	//


	//teana hu 2012.01.06
	void CopyLVVEFromNoReserveToMax(LVVE_CTRL_PARAM_NV_T &lvve_data, LVVE_CTRL_PARAM_NV_MIN_T &lvve_data_min);
	void CopyLVVEFromMaxToNoReserve(LVVE_CTRL_PARAM_NV_MIN_T &lvve_data_min, LVVE_CTRL_PARAM_NV_T &lvve_data);
	void CopyLVVEFromReserveToMax(LVVE_CTRL_PARAM_NV_T &lvve_data, LVVE_CTRL_PARAM_NV_1_T &lvve_data_min);
	void CopyLVVEFromMaxToReserve(LVVE_CTRL_PARAM_NV_1_T &lvve_data_min, LVVE_CTRL_PARAM_NV_T &lvve_data);
	void CopyLVVEFromJuniperToMax(LVVE_CTRL_PARAM_NV_T &lvve_data, LVVE_CTRL_PARAM_NV_2_T &lvve_data_min);
	void CopyLVVEFromMaxToJuniper(LVVE_CTRL_PARAM_NV_2_T &lvve_data_min, LVVE_CTRL_PARAM_NV_T &lvve_data);
	void CopyLVVEFromSingleMicToMax(LVVE_CTRL_PARAM_NV_T &lvve_data, LVVE_CTRL_PARAM_NV_3_T &lvve_data_min);
	void CopyLVVEFromMaxToSingleMic(LVVE_CTRL_PARAM_NV_3_T &lvve_data_min, LVVE_CTRL_PARAM_NV_T &lvve_data);
	
	DWORD GetLVVEType();
	//


public:
	virtual int OnChannelEvent( unsigned int event,void* lpEventData );
	virtual int OnChannelData(LPVOID lpData, ULONG reserved );

protected:
    BOOL ConnectToCS();

    BOOL Bin2ASCII( 
        const BYTE* pBuffer,//source buffer
        BYTE* pResultBuffer,//dest buffer
        unsigned long ulSize //source buffer length
        );

    BOOL ASCII2Bin(
        const BYTE* pBuffer,//source buffer
        BYTE* pResultBuffer,//dest buffer
        unsigned long ulSize //source buffer length
        );
    
    void EndianConvertion(
        WORD* pBuffer, //buffer to  endian
        unsigned long iSize //buffer size
        );
        
    void Log( LPCTSTR lpszInfo,BOOL bErrMsg = FALSE );

	BOOL SendATCmd( char* lpszATCmd );
	BOOL ExeDiagCmd( BYTE type,BYTE subtype,void* lpData,DWORD dwLen,DWORD dwTimeout,PRT_BUFF** lppBuff );

	BOOL QueryEndian();

	void ConvertLvveEndian( LVVE_CTRL_PARAM_NV_T& lvve_data );

	//teana hu 2012.01.30
	void ConvertArmStructLongEndian(VOLUME_STRUCT_SIX& volume_data);
	//
	

    
protected:
    //CDiag m_Diag;
    //HANDLE  m_hRcvCSDataThread;
    //DWORD m_dwRcvThreadId;
	IProtocolChannel* m_pChannel;

    //different context, the phone will return different string to identify 
    // the action result is  successful or not
    CString m_strSucessFlag;

	//teana hu 2012.02.21
	CString m_strHTCSucFlag;
	//

    CString m_strReturn;

    //to synchronize the thread which send AT command and the thread which check the AT command result. 
    HANDLE m_hEventGetResult;
    //AT command execute successful or not
    BOOL m_bCmdExecuteFail;
    
    BOOL m_bConnected;

    // How many bytes received in last 
    // GetModeData function
    // This member will be used to decide
    // phone software version.Diffrent phone
    // software defines different audiodata
    // size.
    int m_nAudioDataSize;

	//teana hu 2010.06.10
	int m_nEQDataSize;

	int m_nEndian;
	CHANNEL_ATTRIBUTE m_ca;
	
	//teana hu 2011.02.16
	int m_nVolumeDataSize;

	//teana hu 2011.03.22
	int m_nEQGGDataSize;
	int m_nTundataSize;

	//teana hu 2011.04.14
	int m_nLVVEDataSize;

	//teana hu 2011.06.07
	//BOOL m_bLVVEMin;

	//teana hu 2012.01.06
	DWORD m_dwLVVEType;


	//teana hu 2012.01.31
	int m_nDspModeSize;
	
	//teana hu 2012.04.06
	BOOL m_bDspCodec;

};

#endif // !defined(AFX_COMMAND_H__578B1F4B_7357_4268_A3C4_52B3D675DC17__INCLUDED_)
