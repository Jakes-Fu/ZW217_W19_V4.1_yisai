// Command.cpp: implementation of the CCommand class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "audiotester.h"
#include "Command.h"

#include <WINSOCK2.H>
#include "AudioTesterSheet.h"

#define PPP_PACKET_R 104
#define PPP_PACKET_A 156
#define DIAG_LOG_F 2
#define ARM_LOG_DISABLE 5
#define DIAG_CURRENT_TEST_F 17
#define CURRENT_TEST_DEEP_SLEEP_FLAG_DISABLE 0xC
#define MSG_REG 99
#define MSG_UNREG 100
#define ITC_REQ_TYPE	209
#define ITC_REQ_SUBTYPE 100
#define ITC_REP_SUBTYPE 101

#define ITC_TOOL_NAME_LEN 32
typedef struct tag_ITC_Req_T
{
	DWORD dwReqType;	// Request type
	char szToolName[ITC_TOOL_NAME_LEN];	// target tool name
}ITC_Req_T;

typedef struct tag_ITC_Rep_T
{
	DWORD dwReqType;	// Request type
	DWORD dwRetCode;	// Error code
}ITC_Rep_T;

enum ITC_Req_Type_E
{
	Reg_Tool,
	Unreg_Tool,
	Query_Tool,
	Logel_ClearLog,
	Logel_SaveLog,
	Logel_Rec_keynote,
	Script_Resume,
	Query_Endian
};

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef SIMU_TEST
int g_data_size = 0;
int g_data_from = 0; // 0 means from code,1 means from nvitem.bin
#define MAX_NV_SIZE 65536
BYTE g_lpBuffer[MAX_NV_SIZE];
#define NVID_MODE_COUNT 6
#define NVID_MODE_DATA 7
int g_mode_count = 0;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommand::CCommand()
{
//    m_hRcvCSDataThread = NULL;
    m_hEventGetResult = NULL;
    
    m_bConnected = FALSE;

//    m_dwRcvThreadId = 0;
    m_bCmdExecuteFail = FALSE;

    m_nAudioDataSize = 0;

	//teana hu 2010.06.10
	m_nEQDataSize = 0;
	//

	m_pChannel = NULL;

	m_nEndian = PP_UNKOWN_ENDIAN;

	m_nVolumeDataSize = 0;
	m_nEQGGDataSize = 0;
	m_nTundataSize = 0;

//	m_bLVVEMin = FALSE;
	
	m_dwLVVEType = LVVE_RESERVE;
	m_nDspModeSize = 0;

	//teana hu 2012.02.21
	m_strHTCSucFlag = _T("0"); // 0 is "OK" of HTC
	//

	m_bDspCodec = FALSE;
}

CCommand::~CCommand()
{
    //lint -e1540
    // member will be set to null in Disconnect
}

BOOL CCommand::Connect( LPCSTR lpszAddr,DWORD dwPort )
{
	m_ca.ChannelType = CHANNEL_TYPE_SOCKET;
	m_ca.Socket.dwFlag = SC_TYPE_CLIENT;
	m_ca.Socket.dwIP = ntohl(inet_addr(lpszAddr));
	m_ca.Socket.dwPort = dwPort;

    // 与Channel Server建立连接
	return ConnectToCS();
}

BOOL CCommand::Connect( DWORD dwPort,DWORD dwBaudrate )
{
	m_ca.ChannelType = CHANNEL_TYPE_COM;
	m_ca.Com.dwPortNum = dwPort;
	m_ca.Com.dwBaudRate = dwBaudrate;

	return ConnectToCS();
}

BOOL CCommand::Disconnect()
{
#ifndef SIMU_TEST

    if( m_bConnected )
    {
        // 注销异步消息
        //m_Diag.UnregMsg( PPP_PACKET_A );   
		ExeDiagCmd( MSG_UNREG,PPP_PACKET_A,NULL,0,1000,NULL );
    
        // 断开与Channel Server的连接
        //m_Diag.Close();
		m_pChannel->Close();

        m_bConnected = FALSE;
    }

    CloseHandle( m_hEventGetResult );
    m_hEventGetResult = NULL;
        
    // 结束接收线程
	/*
    PostThreadMessage( m_dwRcvThreadId,WM_QUIT,0,0 );
    CloseHandle( m_hRcvCSDataThread );
    m_hRcvCSDataThread = NULL;
    m_dwRcvThreadId = 0;
	*/
#endif
	m_nEndian = PP_UNKOWN_ENDIAN;
    Log( _T("Disconnect with CS") );
    return TRUE;
}

// Read audio mode count from phone
BOOL CCommand::GetModeCount( int& nModeCount )
{
#ifdef SIMU_TEST
	if( g_data_from )
	{
		// Read from nvitem.bin
		CString strFilter = "NV image files(*.bin)|*.bin|";
		CFileDialog dlg(TRUE,".bin",NULL,OFN_HIDEREADONLY,strFilter,NULL);
		if( IDOK == dlg.DoModal() )
		{
			CFile file;
			BOOL bRet = file.Open( dlg.GetPathName(),CFile::modeRead );
			if( !bRet )
			{
				return FALSE;
			}

			file.Read( g_lpBuffer,MAX_NV_SIZE );
			file.Close();
		}

		// find mode count item
		int i = 4;	// skip nv time stamp
		while( i < MAX_NV_SIZE )
		{
			WORD id = (WORD)(( g_lpBuffer[i] << 8 ) | g_lpBuffer[i+1]);
			if( id == NVID_MODE_COUNT )
			{
				// Found
				nModeCount = ( g_lpBuffer[i+4] << 8 ) | g_lpBuffer[i+5];
				g_mode_count = nModeCount;
				return TRUE;
			}
			else
			{
				// goto next item
				WORD len = (WORD) (( g_lpBuffer[i+2] << 8 ) | g_lpBuffer[i+3]);
				if( len == 0 )
				{
					// Invalid nv file
					return FALSE;
				}
				int n = len % 4;
				if( n > 0 )
				{
					len += 4 - n;
				}
				i += len + 4;
			}
		}
	}
	else
	{
		nModeCount = 9;
	}
    return TRUE;
#endif

    if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor wc;

	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
    
    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+SADM=%d\r\n"), GET_AUDIO_MODE_COUNT);    
    ResetEvent( m_hEventGetResult );

	Log(_T("Read audio_dsp mode count from phone:"),FALSE );

#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif         
	
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
//            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
//            Log( "Command response is " + m_strReturn ,TRUE );
			Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",FALSE ); 
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
#ifdef _UNICODE
		swscanf( m_strReturn,_T("+SADM:%d"),&nModeCount );
#else
		sscanf( (LPSTR)(LPCTSTR)m_strReturn,"+SADM:%d",&nModeCount );
#endif
		if(nModeCount == 0)
		{
			Log(_T("Error! Audio dsp mode count is 0!  (Return:  " + m_strReturn+ ")"),TRUE );  
			return FALSE;
		}
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

// Read audio mode name from phone
BOOL CCommand::GetModeName( int nModeIndex,CString& strModeName )
{
#ifdef SIMU_TEST
	if( g_data_from )
	{
		// data come from nvitem.bin
		
		// find audio mode data item
		int i = 4;	// skip nv time stamp
		while( i < MAX_NV_SIZE )
		{
			WORD id = (WORD)(( g_lpBuffer[i] << 8 ) | g_lpBuffer[i+1]);
			if( id == NVID_MODE_DATA )
			{
				// Found
				break;
			}
			else
			{
				// goto next item
				WORD len = (WORD)(( g_lpBuffer[i+2] << 8 ) | g_lpBuffer[i+3]);
				if( len == 0 )
				{
					// Invalid nv file
					return FALSE;
				}
				int n = len % 4;
				if( n > 0 )
				{
					len += (unsigned short)(4 - n);
				}
				i += len + 4;
			}
		}
		if( i >= MAX_NV_SIZE )
		{
			// Not found
			return FALSE;
		}

		i += 2;
		if( (i + 1) >= MAX_NV_SIZE )
		{
			// Not found
			return FALSE;
		}
		WORD len = (WORD)(( g_lpBuffer[i] << 8 ) | g_lpBuffer[i+1]);
		i += 2;

		g_data_size = len / g_mode_count - 16;
		strModeName = g_lpBuffer+(i+nModeIndex*(16+g_data_size));
	}
	else
	{
		char mode_name[9][12] = { "Headset","Headfree","Handset","Handsfree","MP4HFTP","MP4HFTR","MP4Headset","Record","BTHS" };
		strModeName = mode_name[nModeIndex];
	}
    return TRUE;
#endif

    if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor wc;

	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
    
    m_strSucessFlag = "OK";          

    strATCommand.Format(_T("AT+SADM=%d,%d\r\n"),GET_AUDIO_MODE_NAME,nModeIndex );    
    ResetEvent( m_hEventGetResult );  
	Log(_T("Read audio_dsp mode name from phone:"),FALSE );
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif     
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
        
        // The return string may be '+SADM:0,"ModeName"'
        int nCommaPos = m_strReturn.Find( '"' );
        if( nCommaPos == -1 )
        {
            return FALSE;
        }
        else
        {
            strModeName = m_strReturn.Right( m_strReturn.GetLength() - nCommaPos - 1 );
            strModeName.Remove( '"' );
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

// Read audio structure from phone
BOOL CCommand::GetModeData( int nModeIndex, int nType, AUDIO_STRUCT& audio_data )
{
#ifdef SIMU_TEST
    memset( &audio_data,0,sizeof( AUDIO_STRUCT ) );
	
	if( g_data_from )
	{
		// data come from nvitem.bin
		
		// find audio mode data item
		int i = 4;	// skip nv time stamp
		while( i < MAX_NV_SIZE )
		{
			WORD id = (WORD)(( g_lpBuffer[i] << 8 ) | g_lpBuffer[i+1]);
			if( id == NVID_MODE_DATA )
			{
				// Found
				break;
			}
			else
			{
				// goto next item
				WORD len = (WORD)(( g_lpBuffer[i+2] << 8 ) | g_lpBuffer[i+3]);
				if( len == 0 )
				{
					// Invalid nv file
					return FALSE;
				}
				int n = len % 4;
				if( n > 0 )
				{
					len += (unsigned short)(4 - n);
				}
				i += len + 4;
			}
		}
		if( i >= MAX_NV_SIZE )
		{
			// Not found
			return FALSE;
		}

		i += 4;

		//teana hu 2009.06.02
		if((i+nModeIndex*(16+g_data_size)+16) >= MAX_NV_SIZE || g_data_size >= (MAX_NV_SIZE - (i+nModeIndex*(16+g_data_size)+16)))
		{
			return FALSE;
		}
		//
		memcpy( &audio_data,g_lpBuffer+(i+nModeIndex*(16+g_data_size)+16),g_data_size ); //lint !e662
		
		//teana hu 2010.05.06
		if(pSheet != NULL && pSheet->m_bBigEndian)
		{
			EndianConvertion( (WORD*)&audio_data,sizeof( AUDIO_STRUCT ) / 2 );
		}
		//
	}
	else
	{
		audio_data.dl_device_external = 0;
		audio_data.dl_device_internal = 0x0063;
		audio_data.ul_device_external = 0;
		audio_data.ul_device_internal = 0x1;
		audio_data.dl_pga_gain_h = 0;
		audio_data.dl_pga_gain_l = 0x7;
		audio_data.ul_pga_gain_h = 0;
		audio_data.ul_pga_gain_l = 0x6;
		
		audio_data.sidetone_switch = 0x0;
		audio_data.aec_switch = 0;
		audio_data.volume_mode = 2;
		audio_data.sample_rate = 0x8;
		
		audio_data.dl_dp_gain = 0x1295;
		audio_data.dl_dp_attenu = 0x800;
		
		audio_data.dl_eq_bass_alpha = 9602;
		audio_data.dl_eq_bass_beta = 13254;
		audio_data.dl_eq_bass_gama = 0x0;
		audio_data.dl_eq_bass_gain = 0x1000;
		
		audio_data.dl_eq_mid_alpha = 6044;
		audio_data.dl_eq_mid_beta = 11943;
		audio_data.dl_eq_mid_gama = 0xfb30;
		audio_data.dl_eq_mid_gain = 0x1000;
		
		audio_data.dl_eq_treble_alpha = 3258;
		audio_data.dl_eq_treble_beta = 0xc6fa;
		audio_data.dl_eq_treble_gama = 0x0;
		audio_data.dl_eq_treble_gain = 0x1000;
		
		audio_data.digital_sidetone_gain = 0;
		
		audio_data.ul_dp_gain = 0x3000;
		audio_data.ul_dp_attenu = 0x800;
		
		audio_data.ul_eq_bass_alpha     = 5323;
		audio_data.ul_eq_bass_beta      = 14384;
		audio_data.ul_eq_bass_gama      = 0;
		audio_data.ul_eq_bass_gain      = 0x1000;
		audio_data.ul_eq_treble_alpha   = 13993;
		audio_data.ul_eq_treble_beta    = 0xde90;
		audio_data.ul_eq_treble_gama    = 0;
		audio_data.ul_eq_treble_gain    = 0x1000;
		
		audio_data.dl_eq_switch = 0x10;
		
		audio_data.dl_pop_switch = 1;
		
		audio_data.dl_agc_switch = 0x100;
		
		audio_data.ul_fir_hpf_enable = 1;
		audio_data.aec_enable = 1;
		audio_data.pdelay = 24;
		audio_data.dl_ref_hpf_enable = 0x101;
		
		audio_data.decor_filter_enable = 0;
		audio_data.fir_taps = 32;
		audio_data.aec_frozen = 0;
		audio_data.coeff_frozen = 0;
		audio_data.dt_dect_threshold = 409;
		audio_data.dt_noise_floor = 80;
		audio_data.step_size = 1536;
		audio_data.coeff_norm_shift = 1;
		audio_data.sa_ctrl = 0;
		
		audio_data.send_attenu_in_dt = 2048;
		audio_data.send_attenu_in_rv = 400;
		audio_data.send_threshold = 0x6000;
		audio_data.r2dt_threshold = 0x4000;
		audio_data.s2dt_threshold = 0x2000;
		audio_data.recv_threshold = 0x1000;
		audio_data.bn40 = 10;
		audio_data.sa_ar = 0;
		
		audio_data.ng_select = 0x0101;
		audio_data.alpha_distor = 13993;
		audio_data.beta_distor = 11585;
		audio_data.ul_ng_plk_wpyy_a = 64;
		audio_data.ul_ng_plk_wpyy_n = 16;
		audio_data.ul_ng_plk_holdc = 16;     
		audio_data.ul_ng_plk_att = 1024;
		audio_data.ul_ng_clk_wpyy_a = 256;
		audio_data.ul_ng_clk_wpyy_n = 80;
		audio_data.ul_ng_clk_holdc = 16;
		audio_data.ul_ng_clk_att = 512;   
		audio_data.dl_ng_plk_wpyy_a = 64;
		audio_data.dl_ng_plk_wpyy_n = 16;    
		audio_data.dl_ng_plk_holdc = 16;
		audio_data.dl_ng_plk_att = 4096;   
		audio_data.dl_ng_clk_wpyy_a = 256;
		audio_data.dl_ng_clk_wpyy_n = 80;    
		audio_data.dl_ng_clk_holdc = 16;     
		audio_data.dl_ng_clk_att = 4096;       
		
		audio_data.da_limit = 0;
		audio_data.dl_fir_eq_coeff[0] = 1;
		audio_data.ul_fir_eq_coeff[0] = 1;
		audio_data.st_fir_eq_coeff[0] = 1;
		
		audio_data.audio_three_switch = 1;
	}

    m_nAudioDataSize = g_data_size;

    return TRUE;
#endif

    if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor wc;

	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;

	memset( &audio_data,0,sizeof( AUDIO_STRUCT ) );

    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+SADM=%d,%d\r\n"),nType, nModeIndex );    
    ResetEvent( m_hEventGetResult );
	if(nType == GET_AUDIO_DATA_FROM_RAM)
	{
		Log(_T("Read audio_dsp structrue from ram:"),FALSE );
	}
	else
	{
		Log(_T("Read audio_dsp mode count from flash:"),FALSE );
	}

#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif     
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
        
        // The return string may be '+SADM:0,"ModeName",modedata'
        int nCommaPos = m_strReturn.ReverseFind( ',' );
        if( nCommaPos == -1 )
        {
            return FALSE;
        }
        else
        {
            CString strData;
            strData = m_strReturn.Right( m_strReturn.GetLength() - nCommaPos - 1 );
            // Two bytes represent one byte binary data
            m_nAudioDataSize = strData.GetLength() / 2;
#ifdef _UNICODE
			char szTemp[4096] = {0};
			WideCharToMultiByte(CP_ACP,0,strData.GetBuffer(strData.GetLength()),-1,szTemp,strData.GetLength(),NULL,NULL);
			ASCII2Bin( (BYTE*)szTemp,(BYTE*)&audio_data,strlen(szTemp) );
#else
			ASCII2Bin( (BYTE*)strData.GetBuffer(0),(BYTE*)&audio_data,strData.GetLength() );
#endif
			//teana hu 2010.05.06
			if( PP_BIG_ENDIAN == m_nEndian )
			{
				EndianConvertion( (WORD*)&audio_data,m_nAudioDataSize / 2 );
			}
			//
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

BOOL CCommand::OpenMixMode()
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }
	
	CWaitCursor wc;

	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;

    m_strSucessFlag = "OK";          
    
    strATCommand = _T("AT+SUPS=1\r\n");
	ResetEvent( m_hEventGetResult ); 
	Log(_T("Open mix mode:"),FALSE );
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() ); 		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif   
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",FALSE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",FALSE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

BOOL CCommand::CloseArmLog()
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
	Log(_T("Close arm log:"),FALSE );

	BOOL bRet = ExeDiagCmd( DIAG_LOG_F,ARM_LOG_DISABLE,NULL,0,3000,NULL );
	if( bRet )
	{
		Log( _T("Execute command: close arm log success!"),FALSE );
	}
	else
	{
		Log( _T("Execute command: close arm log failed!"),FALSE );
	}

	return bRet;
}

BOOL CCommand::CloseDeepSleep()
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
	
	Log(_T("Close deep sleep:"),FALSE );

	BOOL bRet = ExeDiagCmd( DIAG_CURRENT_TEST_F,CURRENT_TEST_DEEP_SLEEP_FLAG_DISABLE,NULL,0,3000,NULL );
	if( bRet )
	{
		Log( _T("Execute command: close deep sleep success!"),FALSE );
	}
	else
	{
		Log( _T("Execute command: close deep sleep failed!"),FALSE );
	}

	return bRet;
}

BOOL CCommand::ConnectToCS()
{
#ifndef SIMU_TEST
	if( NULL == m_pChannel )
	{
		if( !CreateDiagChannel( &m_pChannel ) )
		{
			Log( _T( "Create communication channel failed!" ),TRUE );
			return FALSE;
		}
	}

	// 连接过程中显示等待光标
    CWaitCursor waitCursor;
    
    if( CHANNEL_TYPE_SOCKET == m_ca.ChannelType )
	{
		// Set diag type
		m_pChannel->SetProperty( 0,PPI_DIAG_Type,(void*)DIAG_TYPE_NO_HEADER );
		
		// Set endian
		m_pChannel->SetProperty( 0,PPI_Endian,(void*)(PP_LITTLE_ENDIAN << 8 | PP_LITTLE_ENDIAN));
	}
	else
	{
		// Set endian
		m_pChannel->SetProperty( 0,PPI_Endian,(void*)(PP_LITTLE_ENDIAN << 8 | m_nEndian ));
	}

	m_pChannel->AddObserver( this );
    m_bConnected = m_pChannel->Open( &m_ca );

    if ( !m_bConnected )  // 连接失败
    {
        Disconnect();
        Log( _T("Can not connect with CS"),TRUE );
        return FALSE;
    }

    m_hEventGetResult = CreateEvent( NULL, FALSE, FALSE ,NULL );
    
    // 注册异步消息
    //m_Diag.RegMsg( PPP_PACKET_A );
	ExeDiagCmd( MSG_REG,PPP_PACKET_A,NULL,0,1000,NULL );

#endif

	Log( _T("Connect with CS") );
    return TRUE;
}

// Set speaker status
BOOL CCommand::SetSpeaker( int nSpeaker )
{
#ifdef SIMU_TEST
    return TRUE;
#endif
    
    if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor wc;
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
    
    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+SDMUT=%d\r\n"),nSpeaker );    
    ResetEvent( m_hEventGetResult );    
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif     
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

// Set mute status
BOOL CCommand::SetMute( int nMute )
{
#ifdef SIMU_TEST
    return TRUE;
#endif 
    
    if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first." ));
        return FALSE;
    }

    CWaitCursor wc;
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
    
    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+CMUT=%d\r\n"),nMute );    
    ResetEvent( m_hEventGetResult );    
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif       
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE ); 
            Log( "Command response is " + m_strReturn ,TRUE );
			Log( _T("This command can only be executed when phone is on a call,please make call then try again."),TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

// Write audio structure to phone
BOOL CCommand::SetModeData( int nModeIndex,CString strModeName,AUDIO_STRUCT& audio_data )
{
	UNREFERENCED_PARAMETER(nModeIndex);
	//
#ifdef SIMU_TEST
    AUDIO_STRUCT temp_data = audio_data;
    // 0X3456 --->0X5634
	//teana hu 2010.05.06
	if(pSheet != NULL && pSheet->m_bBigEndian)
	{
		EndianConvertion( (WORD*)&temp_data,sizeof( AUDIO_STRUCT ) / 2 );
	}
	//
    
    BYTE* pASCIIData = new BYTE[sizeof(AUDIO_STRUCT)*2+1 ];
    //0X5634--->'5''6''3''4'
    Bin2ASCII((BYTE*)&temp_data,pASCIIData,sizeof(AUDIO_STRUCT));

    ASCII2Bin( pASCIIData,(BYTE*)&temp_data,sizeof( AUDIO_STRUCT )*2 );
	//teana hu 2010.05.06
	if(pSheet != NULL && pSheet->m_bBigEndian)
	{
		EndianConvertion( (WORD*)&temp_data,sizeof( AUDIO_STRUCT ) / 2 );
	}
	//
        
    return TRUE;
#else 

    if( !m_bConnected )
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor wc;
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;

    AUDIO_STRUCT temp_data = audio_data;
    
    m_strSucessFlag = ">";          
    
    strATCommand.Format(_T("AT+SADM=%d,%d\r\n"),SET_AUDIO_DATA_TO_RAM, nModeIndex );    
    ResetEvent( m_hEventGetResult );  
	Log(_T("Write audio_dsp structrue to ram:"),FALSE );
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif      
    
    //synchronize with thread which check the AT command result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    //restore the return string into default AT command successful string
    m_strSucessFlag = "OK";
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    // 0X3456 --->0X5634
	//teana hu 2010.05.06
	if( PP_BIG_ENDIAN == m_nEndian )
	{
		EndianConvertion( (WORD*)&temp_data,sizeof( AUDIO_STRUCT ) / 2 );
	}
	//
    
    BYTE* pASCIIData = new BYTE[sizeof(AUDIO_STRUCT)*2+2 ];
    //0X5634--->'5''6''3''4'
    Bin2ASCII((BYTE*)&temp_data,pASCIIData,sizeof(AUDIO_STRUCT));
    
    //append ctrl+z
    pASCIIData[m_nAudioDataSize*2] = 0X1A;
	pASCIIData[m_nAudioDataSize*2+1] = 0;
    ResetEvent( m_hEventGetResult );       
    //m_Diag.Send( PPP_PACKET_R, 0, pASCIIData,m_nAudioDataSize*2+1 );	  
	SendATCmd( (char*)pASCIIData );
        
    delete [] pASCIIData;
    
    //synchronize with thread which check the AT comamnd result   
    dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log(_T("Failed to Execute command! "),TRUE);
            Log( _T("Command response is ") + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log(_T("Execute command sucessfully! ") );
        }        
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log(_T("Execute Command  Time Out! "),TRUE);
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log(_T("Failed to Execute command! "),TRUE);   
        return FALSE;
    }

	return TRUE;
    
#endif
}

// Write audio structure to running nv
BOOL CCommand::SetModeData2Flash( int nModeIndex,CString strModeName,AUDIO_STRUCT& audio_data )
{
	UNREFERENCED_PARAMETER(nModeIndex);
#ifdef SIMU_TEST
    AUDIO_STRUCT temp_data = audio_data;
    // 0X3456 --->0X5634
	//teana hu 2010.05.06
	if(pSheet != NULL && pSheet->m_bBigEndian)
	{
		EndianConvertion( (WORD*)&temp_data,sizeof( AUDIO_STRUCT ) / 2 );
	}
    
    BYTE* pASCIIData = new BYTE[sizeof(AUDIO_STRUCT)*2+1 ];
    //0X5634--->'5''6''3''4'
    Bin2ASCII((BYTE*)&temp_data,pASCIIData,sizeof(AUDIO_STRUCT));

    ASCII2Bin( pASCIIData,(BYTE*)&temp_data,sizeof( AUDIO_STRUCT )*2 );
	//teana hu 2010.05.06
	if(pSheet != NULL && pSheet->m_bBigEndian)
	{
		EndianConvertion( (WORD*)&temp_data,sizeof( AUDIO_STRUCT ) / 2 );
	}
        
    return TRUE;
#else 

    if( !m_bConnected )
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor wc;
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;

    AUDIO_STRUCT temp_data = audio_data;
    
    m_strSucessFlag = ">";          
    
    strATCommand.Format(_T("AT+SADM=%d,%d\r\n"),SET_AUDIO_DATA_TO_FLASH, nModeIndex );    
    ResetEvent( m_hEventGetResult );  
	Log(_T("Write audio_dsp structrue to flash:"),FALSE );
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif    
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    //restore the return string into default AT command successful string
    m_strSucessFlag = "OK";
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    // 0X3456 --->0X5634
	//teana hu 2010.05.06
	if( PP_BIG_ENDIAN == m_nEndian )
	{
		EndianConvertion( (WORD*)&temp_data,sizeof( AUDIO_STRUCT ) / 2 );
	}
	//
    
    BYTE* pASCIIData = new BYTE[sizeof(AUDIO_STRUCT)*2+2 ];
    //0X5634--->'5''6''3''4'
    Bin2ASCII((BYTE*)&temp_data,pASCIIData,sizeof(AUDIO_STRUCT));
    
    //append ctrl+z
    pASCIIData[m_nAudioDataSize*2] = 0X1A;
	pASCIIData[m_nAudioDataSize*2+1] = 0;
    ResetEvent( m_hEventGetResult );          
    //m_Diag.Send( PPP_PACKET_R, 0, pASCIIData,m_nAudioDataSize*2+1 );	 
	SendATCmd( (char*)pASCIIData );
        
    delete [] pASCIIData;
    
    //synchronize with thread which check the AT comamnd result   
    dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log(_T("Failed to Execute command! "),TRUE);
            Log( _T("Command response is ") + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log(_T("Execute command sucessfully! ") );
        }        
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log(_T("Execute Command  Time Out! "),TRUE);
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log(_T("Failed to Execute command! "),TRUE);   
        return FALSE;
    }

    return TRUE;
#endif
}


//convert binary string to ASCII string. 0X12 0X34 ---> '1''2''3''4'
BOOL CCommand::Bin2ASCII( 
                           const BYTE* pBuffer,//source buffer
                           BYTE* pResultBuffer,//dest buffer
                           unsigned long ulSize //source buffer length
                           )
{
    ASSERT( pBuffer );
    ASSERT( pResultBuffer );
    
    const BYTE* pCurBype = pBuffer;
    const BYTE* pEnd = pBuffer + ulSize;
    BYTE* pCurDestBuf = pResultBuffer;
    int iCount = 0;
    while( pCurBype != pEnd )
    {
        BYTE bCurByte = *pCurBype;        
        unsigned char lHigh = (BYTE)(bCurByte >> 4);
        if( lHigh <= 9)
        {
            *pCurDestBuf = (BYTE)(lHigh + '0');
        }
        else
        {
            *pCurDestBuf = (BYTE)(lHigh -10 + 'A');
        }
        ++pCurDestBuf;
        
        bCurByte = *pCurBype;
        
        unsigned char lLow = (unsigned char)(bCurByte << 4);
        lLow = (unsigned char)(lLow >>4);                
        if( lLow <= 9)
        {
            *pCurDestBuf = (BYTE)(lLow + '0');
        }
        else
        {
            *pCurDestBuf = (BYTE)(lLow -10 + 'A');
        }
        ++pCurDestBuf;
        
        ++pCurBype;
        ++iCount;
    }
//    int iOff = pCurDestBuf - pResultBuffer;
    return TRUE;
}

BOOL CCommand::ASCII2Bin(
               const BYTE* pBuffer,//source buffer
               BYTE* pResultBuffer,//dest buffer
               unsigned long ulSize //source buffer length
               )
{
    ASSERT( pBuffer );
    ASSERT( pResultBuffer );
    
    int iCount = 0;
    BYTE l,h;
    while( (unsigned long)iCount < ulSize )
    {
        if( *pBuffer >= '0' && *pBuffer <= '9' )
        {
            h = (BYTE)(*pBuffer - '0');
        }
        else
        {
            h = (BYTE)(*pBuffer - 'A' + 10);
        }
        h = (BYTE)(h << 4);
        pBuffer++;
        if( *pBuffer >= '0' && *pBuffer <= '9' )
        {
            l = (BYTE)(*pBuffer - '0');
        }
        else
        {
            l = (BYTE)(*pBuffer - 'A' + 10);
        }
        *pResultBuffer = (BYTE)( h  | l );
        pResultBuffer++;
        pBuffer++;
        iCount += 2;
    }     
    return TRUE;
}

//0X12 0X34 --->0X34 0X12
void CCommand::EndianConvertion(
                                  WORD* pBuffer, //buffer to  endian
                                  unsigned long iSize //buffer size
                                  )
{
    ASSERT( pBuffer );    
    WORD* pCurWord = pBuffer;
    WORD* pEndWord = pBuffer + iSize ;
    
    while ( pCurWord != pEndWord ) 
    {
        *pCurWord = (WORD)((*pCurWord << 8 ) | (*pCurWord >>8 ));
        ++pCurWord;
    }
}

void CCommand::Log( LPCTSTR lpszInfo,BOOL bErrMsg)
{
    CAudioTesterSheet* pSheet = (CAudioTesterSheet*)AfxGetMainWnd();
    pSheet->Log( lpszInfo,bErrMsg );
}

// Set volume of phone
BOOL CCommand::SetVolume( int nVolume )
{
    if( !m_bConnected )
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }
    
    CWaitCursor wc;
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
    
    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+VGR=%d\r\n"),nVolume+1 );    
    ResetEvent( m_hEventGetResult );   
	Log(_T("Set volume of phone:"),FALSE );
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif    
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

// Make a phone call
BOOL CCommand::MakeCall( CString strPhoneNumber )
{
    if( !m_bConnected )
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    if( strPhoneNumber.IsEmpty() )
    {
        Log( _T("Phone number is invalid !") ,TRUE );
        return FALSE;
    }
    
    CWaitCursor tempWait;
    
    if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

	CString sATCommand("ATD");  
    sATCommand += strPhoneNumber;
    sATCommand += ";\r\n";
	Log(_T("Make a call:"),FALSE );

#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,sATCommand.GetBuffer(sATCommand.GetLength()),-1,szATCmd,sATCommand.GetLength(),NULL,NULL);
	//BOOL bRet = m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	BOOL bRet = SendATCmd( szATCmd );
#else   
    //BOOL bRet = m_Diag.Send( PPP_PACKET_R, 0, sATCommand.GetBuffer(sATCommand.GetLength()),sATCommand.GetLength() );		
	BOOL bRet = SendATCmd( sATCommand.GetBuffer(sATCommand.GetLength()) );
#endif     
    
    if( bRet )
    {
        Log(_T("msg : cmd ( make call ) send !"));  
    }
    else
    {
        Log( _T("Make call failed !"),TRUE );
    }   

    return bRet;
}

BOOL CCommand::Hangup()
{
    if( !m_bConnected )
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor tempWait;
    
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

    CString sATCommand("ATH\r\n");  
	Log(_T("Hang up a call:"),FALSE );
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,sATCommand.GetBuffer(sATCommand.GetLength()),-1,szATCmd,sATCommand.GetLength(),NULL,NULL);
	//BOOL bRet = m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	BOOL bRet = SendATCmd( szATCmd );
#else   
    //BOOL bRet = m_Diag.Send( PPP_PACKET_R, 0, sATCommand.GetBuffer(sATCommand.GetLength()),sATCommand.GetLength() );		
	BOOL bRet = SendATCmd( sATCommand.GetBuffer(sATCommand.GetLength()) );
#endif      
    
    if( bRet )
    {
        Log(_T("msg : cmd ( Hang up ) send !"));  
    }
    else
    {
        Log( _T("Hang up failed !"),TRUE );
    }   
    
    return bRet;
}

BOOL CCommand::GetEQCount( int& nEQCount )
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

	m_strReturn.Empty();
	nEQCount = 0;
	
    CWaitCursor wc;
    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
    
    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+SPEQPARA=0\r\n") );    
    ResetEvent( m_hEventGetResult );   
	Log(_T("Get EQ mode count from phone:"),FALSE );
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif     
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            //Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            //Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
#ifdef _UNICODE
		swscanf( m_strReturn,_T("+SPEQPARA:%d"),&nEQCount );
#else
		sscanf( (LPSTR)(LPCTSTR)m_strReturn,"+SPEQPARA:%d",&nEQCount );
#endif
		if(nEQCount == 0)
		{
			Log(_T("Error! Eq mode count is 0!  (Return:  " + m_strReturn+ ")"),TRUE );  
			return FALSE;
		}
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

BOOL CCommand::GetEQData( int nEQIndex,AUDIO_EQ_STRUCT_T& eq_data, int nPos )
{
    if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor wc;

	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;

	memset( &eq_data,0,sizeof( AUDIO_EQ_STRUCT_T ) );

    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+SPEQPARA=1,%d,%d\r\n"),nEQIndex,nPos );    //get from ram
    ResetEvent( m_hEventGetResult ); 
	if(nPos == 0)
	{
		Log(_T("Read EQ strcuture from ram:"),FALSE );
	}
	else
	{
		Log(_T("Read EQ strcuture from flash:"),FALSE );
	}
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif     
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",FALSE );  
            Log( "Command response is " + m_strReturn ,FALSE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
        
        // The return string may be '+SPEQPARA:<data>
		int nCommaPos = m_strReturn.Find(_T("+SPEQPARA: "));
        if( nCommaPos == -1 )
        {
            return FALSE;
        }
        else
        {
            CString strData;
			m_strReturn.Delete(0, 11); //delete "+SPEQPARA: "
            strData = m_strReturn;
            // Two bytes represent one byte binary data
            m_nEQDataSize = strData.GetLength() / 2;
#ifdef _UNICODE
			char szTemp[4096] = {0};
			WideCharToMultiByte(CP_ACP,0,strData.GetBuffer(strData.GetLength()),-1,szTemp,strData.GetLength(),NULL,NULL);
			ASCII2Bin( (BYTE*)szTemp,(BYTE*)&eq_data,strlen(szTemp) );
#else
			ASCII2Bin( (BYTE*)strData.GetBuffer(0),(BYTE*)&eq_data,strData.GetLength() );
#endif
			//teana hu 2010.05.06
			if( PP_BIG_ENDIAN == m_nEndian )
			{
				EndianConvertion( (WORD*)&eq_data.eq_mode_control, sizeof(WORD) / 2);
				EndianConvertion( (WORD*)&eq_data.eq_modes, sizeof(EQ_MODE_NV_PARAM_T) * TOTAL_EQ_MODE_NUM  / 2 + EQ_EXTEND_ARRAY_DIM );
			}
			//
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",FALSE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

BOOL CCommand::SetEQData( int nEQIndex, AUDIO_EQ_STRUCT_T& eq_data, int nPos)
{
    if( !m_bConnected )
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor wc;
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
    const int nWaitForDiagDataSecond = 6000;

    AUDIO_EQ_STRUCT_T temp_data = eq_data;

	// 0X3456 --->0X5634
	//teana hu 2010.05.06
	if( PP_BIG_ENDIAN == m_nEndian )
	{
		EndianConvertion( (WORD*)&temp_data.eq_mode_control,sizeof( WORD ) / 2 );
		EndianConvertion( (WORD*)&temp_data.eq_modes,sizeof( EQ_MODE_NV_PARAM_T ) * TOTAL_EQ_MODE_NUM / 2 + EQ_EXTEND_ARRAY_DIM );
	}
	//
    
    BYTE* pASCIIData = new BYTE[sizeof(AUDIO_EQ_STRUCT_T)*2+1 ];
    //0X5634--->'5''6''3''4'
    Bin2ASCII((BYTE*)&temp_data,pASCIIData,m_nEQDataSize);
    
    //append ctrl+z
    //pASCIIData[m_nEQDataSize*2] = 0X1A;
	pASCIIData[m_nEQDataSize*2] = '\0';
    
    m_strSucessFlag = "OK"; 
	if(nPos == 0)
	{
		Log(_T("Write EQ strcuture to ram:"),FALSE );
	}
	else
	{
		Log(_T("Write EQ strcuture to flash:"),FALSE );
	}
    
	CString strAT ;
	strAT.Format(_T("AT+SPEQPARA=2,%d,%d,...\r\n"),nEQIndex, nPos);
	char szATCmd[4096] = {0};
	sprintf(szATCmd, "AT+SPEQPARA=2,%d,%d,\"%s\"\r\n", nEQIndex, nPos, pASCIIData);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd) );
	SendATCmd( szATCmd );

    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strAT+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
		else
		{
            Log("Execute command successfully!  (CMD:  " + strAT+ ")",FALSE );  
		}
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strAT+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strAT+  ")",TRUE );   
        return FALSE;
    }  	  
        
    delete [] pASCIIData; 

    return TRUE;
}

BOOL CCommand::SendATCmd( char* lpszATCmd )
{
	if( NULL == lpszATCmd )
	{
		Log( _T( "AT command can not be NULL!"),TRUE );
		return FALSE;
	}

	DIAG_PACKAGE dp;
	dp.header.type = PPP_PACKET_R;
	dp.header.subtype = 0;
	dp.header.sn = SPECIAL_SN;
	dp.header.len = strlen( lpszATCmd );
	dp.data = lpszATCmd;

	PRT_WRITE_T pwt;
	pwt.nCond = -1;
	pwt.action = PRT_WRITE_no_respond;
	pwt.lpProtocolData = &dp;

	return m_pChannel->Write( &pwt,1 );
}

BOOL CCommand::ExeDiagCmd( BYTE type,BYTE subtype,void* lpData,DWORD dwLen,DWORD dwTimeout,PRT_BUFF** lppBuff )
{
	DIAG_PACKAGE dp;
	dp.header.type = type;
	dp.header.subtype = subtype;
	dp.header.sn = SPECIAL_SN;
	dp.header.len = dwLen;
	dp.data = lpData;

	PRT_WRITE_T pwt;
	pwt.nCond = -1;
	pwt.action = PRT_WRITE_normal;
	pwt.lpProtocolData = &dp;

	m_pChannel->Write( &pwt,1 );

	PRT_BUFF* lpBuff = NULL;
	PRT_READ_T prt;
	prt.nCond = pwt.nCond;
	prt.lpCond = NULL;
	prt.lppBuff = &lpBuff;
	m_pChannel->Read( &prt,1,dwTimeout );
	BOOL bRet = ( lpBuff != NULL );
	if( NULL != lppBuff )
	{
		// Return read buff
		*lppBuff = lpBuff;
	}
	else
	{
		if( lpBuff )
		{
			lpBuff->free_prt( lpBuff );
		}
	}
	return bRet;
}

int CCommand::OnChannelEvent( unsigned int event,void* lpEventData )
{
	if( PP_EVENT_BIG_ENDIAN == event )
	{
		m_nEndian = PP_BIG_ENDIAN;
	}
	else if( PP_EVENT_LITTLE_ENDIAN == event )
	{
		m_nEndian = PP_LITTLE_ENDIAN;
	}

	if( NULL != lpEventData )
	{
		PRT_BUFF* lpBuff = (PRT_BUFF*)lpEventData;
		lpBuff->free_prt( lpBuff );
	}
	return 0;
}

int CCommand::OnChannelData(LPVOID lpData, ULONG /*reserved*/ )
{
	PRT_BUFF* lpBuff = (PRT_BUFF*)lpData;
	DIAG_HEADER* lpHead = (DIAG_HEADER*)lpBuff->lpData;
	if( NULL != lpHead && lpBuff->size > sizeof( DIAG_HEADER ) )
	{
		if( PPP_PACKET_A == lpHead->type )
		{
			unsigned char* pCharResult = new unsigned char[ lpBuff->size ];
			memset( pCharResult,0,lpBuff->size ); 
			memcpy( pCharResult, lpBuff->lpData + sizeof(DIAG_HEADER), lpBuff->size - sizeof( DIAG_HEADER ) );       
			
			CString strValue( pCharResult );
			delete [] pCharResult;
			
			strValue.Remove('\r');
			strValue.Remove('\n');
			CString strValueOri = strValue;
			strValue.MakeUpper();
			strValue.TrimLeft();
			strValue.TrimRight();
			if( strValue.GetLength() > 0 )            
			{
				//tell the send thread the AT command result
				if( -1 != strValue.Find( _T("+SADM") ) )
				{
					m_strReturn = strValueOri;
				}
				else if( strValue == m_strSucessFlag || strValue == m_strHTCSucFlag)
				{                
					m_bCmdExecuteFail = FALSE;                
					SetEvent( m_hEventGetResult );
				}
				else if( -1 != strValue.Find(_T("ERROR"),0 ) || strValue == _T("4\r\n"))//4 is "ERROR" of HTC
				{
					m_bCmdExecuteFail = TRUE;
					m_strReturn = strValueOri;
					SetEvent( m_hEventGetResult );
				}
				else if(-1 != strValue.Find( _T("+SPEQPARA") ))
				{
					m_strReturn = strValueOri;
				}
				else if(-1 != strValue.Find( _T("+SPENHA") ))
				{
					m_strReturn = strValueOri;
				}
				else if(-1 != strValue.Find( _T("+SPADLVVE") ))
				{
					m_strReturn = strValueOri;
				}
				else if(-1 != strValue.Find( _T("+SADMDSP") ))
				{
					m_strReturn = strValueOri;
				}
			}
		}
		else if( ITC_REQ_TYPE == lpHead->type && ITC_REP_SUBTYPE == lpHead->subtype )
		{
			if( lpBuff->size == sizeof( DIAG_HEADER ) + sizeof( ITC_Rep_T ) )
			{
				ITC_Rep_T* lpRep = (ITC_Rep_T*)(lpBuff->lpData + sizeof( DIAG_HEADER ) );
				if( Query_Endian == lpRep->dwReqType )
				{
					m_nEndian = ( lpRep->dwRetCode & 0xFF );
				}
			}
		}
	}

	lpBuff->free_prt( lpBuff );

	return 0;
}

BOOL CCommand::QueryEndian()
{
	if( !m_bConnected )
	{
		// Not connected
		return FALSE;
	}

	if( m_nEndian != PP_UNKOWN_ENDIAN )
	{
		return TRUE;
	}

	BOOL bRet = FALSE;

	if( CHANNEL_TYPE_SOCKET == m_ca.ChannelType )
	{
		// Connected to channelserver
		// Send a command to channelserver to get version of channelserver
		PRT_BUFF* lpBuff = NULL;
		ExeDiagCmd( ITC_REQ_TYPE,0,NULL,0,100,&lpBuff );
		if( NULL == lpBuff )
		{
			// Old version channelserver does not support Query_Endian command
			// Load endian from ini file
			TCHAR szCfgPathName[MAX_PATH];
			::GetModuleFileName( NULL, szCfgPathName, _MAX_PATH );
			LPTSTR pExt = _tcsrchr(szCfgPathName, _T('.'));
			_tcscpy(pExt + 1, _T("ini"));
			m_nEndian = GetPrivateProfileInt( _T("Endian"), _T("Endian"), PP_BIG_ENDIAN, szCfgPathName );
			// Reg PPP_PACKET_A again because the first reg command will be treat as
			// machine info by old version of channelserver
			ExeDiagCmd( MSG_REG,PPP_PACKET_A,NULL,0,1000,NULL );
			return TRUE;
		}
		else
		{
			lpBuff->free_prt( lpBuff );
			lpBuff = NULL;
			ITC_Req_T irt;
			strcpy( irt.szToolName,"AudioTester" );
			irt.dwReqType = Query_Endian;
			
			ExeDiagCmd( ITC_REQ_TYPE,ITC_REQ_SUBTYPE,&irt,sizeof( irt ),3000,&lpBuff );
			if( lpBuff != NULL )
			{
				if( lpBuff->size == sizeof( DIAG_HEADER ) + sizeof( ITC_Rep_T ) )
				{
					ITC_Rep_T* lpRep = (ITC_Rep_T*)(lpBuff->lpData + sizeof( DIAG_HEADER ) );
					if( Query_Endian == lpRep->dwReqType )
					{
						m_nEndian = ( lpRep->dwRetCode & 0xFF );
						bRet = TRUE;
					}
				}
				lpBuff->free_prt( lpBuff );
			}
		}
	}
	else
	{
		// Connect to uart directly
		// Try to get endian from channel first
		DWORD dwEndian;
		m_pChannel->GetProperty( 0,PPI_Endian,&dwEndian );
		dwEndian = dwEndian & 0xFFFF;
		if( PP_UNKOWN_ENDIAN != dwEndian )
		{
			m_nEndian = dwEndian;
			bRet = TRUE;
		}
		else
		{
			// Send a query phone software version command,
			// DiagChan.dll will convert it to a query endian
			// command. Endian will be retrieved in OnChannelEvent
			ExeDiagCmd( 0,0,NULL,0,3000,NULL );
			// Sleep to confirm OnChannelEvent is called.
			Sleep(10);
			if( PP_UNKOWN_ENDIAN != m_nEndian )
			{
				bRet = TRUE;
			}
		}
	}

	if( !bRet )
	{
		Log( _T( "Can not get endian mode from phone." ),TRUE );
	}

	return bRet;
}

BOOL CCommand::GetVolumeModeCount( int& nModeCount )
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor wc;

	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
    
    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+SADM=%d\r\n"),GET_ARM_VOLUME_MODE_COUNT );    
    ResetEvent( m_hEventGetResult );
	Log(_T("Read audio_arm mode count from phone:"),FALSE );

#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif         
	
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
			Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",FALSE ); 
			Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
#ifdef _UNICODE
		swscanf( m_strReturn,_T("+SADM:%d"),&nModeCount );
#else
		sscanf( (LPSTR)(LPCTSTR)m_strReturn,"+SADM:%d",&nModeCount );
#endif
		if(nModeCount == 0)
		{
			Log(_T("Error! Audio arm mode count is 0!  (Return:  " + m_strReturn+ ")"),TRUE );  
			return FALSE;
		}
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",FALSE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",FALSE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

BOOL CCommand::GetVolumeSerailCount(int& nCount)
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }
	
    CWaitCursor wc;
	
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
	
    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
	
    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+SADM=%d,%d\r\n"),GET_ARM_VOLUME_DATA_FROM_RAM, 0 );    
    ResetEvent( m_hEventGetResult );    
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif     
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
        
        // The return string may be '+SADM:0,"ModeName",modedata'
        int nCommaPos = m_strReturn.ReverseFind( ',' );
        if( nCommaPos == -1 )
        {
            return FALSE;
        }
        else
        {
            CString strData;
            strData = m_strReturn.Right( m_strReturn.GetLength() - nCommaPos - 1 );
            // Two bytes represent one byte binary data
            m_nVolumeDataSize = strData.GetLength() / 2;

			if(m_nVolumeDataSize == sizeof(VOLUME_STRUCT_SIXTEEN))
			{
				nCount =  AUDIO_ARM_APP_TYPE_MAX;
			}
			else if(m_nVolumeDataSize == sizeof(VOLUME_STRUCT_MIN))
			{
				nCount = AUDIO_ARM_APP_TYPE_MIN;
			}
			else if( m_nVolumeDataSize == sizeof( VOLUME_STRUCT_ONE ) )
			{
				nCount = AUDIO_ARM_APP_TYPE_ONE;
			}
			else if(m_nVolumeDataSize == sizeof( VOLUME_STRUCT_SIX ))
			{
				nCount = AUDIO_ARM_APP_TYPE_SIX;
			}
			else
			{
				AfxMessageBox(_T("Data Error! Please check arm audio struct(number of app_config_info must be 16, 8, 1, or 2)."));
				return FALSE;
			}
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

BOOL CCommand::GetVolumeData( int nIndex, int nType, VOLUME_STRUCT& volume_data)
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor wc;

	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;

	memset( &volume_data,0,sizeof( VOLUME_STRUCT ) );

    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+SADM=%d,%d\r\n"),nType, nIndex );    
    ResetEvent( m_hEventGetResult );  
	if(nType == GET_ARM_VOLUME_DATA_FROM_RAM)
	{
		Log(_T("Read audio_arm structure from ram:"),FALSE );
	}
	else
	{
		Log(_T("Read audio_arm structure from flash:"),FALSE );
	}
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif     
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
        
        // The return string may be '+SADM:0,"ModeName",modedata'
        int nCommaPos = m_strReturn.ReverseFind( ',' );
        if( nCommaPos == -1 )
        {
            return FALSE;
        }
        else
        {
            CString strData;
            strData = m_strReturn.Right( m_strReturn.GetLength() - nCommaPos - 1 );
            // Two bytes represent one byte binary data
            m_nVolumeDataSize = strData.GetLength() / 2;
			//teana hu 2011.04.12
			if(m_nVolumeDataSize == sizeof(VOLUME_STRUCT_MIN))
			{
				VOLUME_STRUCT_MIN volume_data_8;
				memset(&volume_data_8, 0, sizeof(VOLUME_STRUCT_MIN));
#ifdef _UNICODE
				char szTemp[4096] = {0};
				WideCharToMultiByte(CP_ACP,0,strData.GetBuffer(strData.GetLength()),-1,szTemp,strData.GetLength(),NULL,NULL);
				ASCII2Bin( (BYTE*)szTemp,(BYTE*)&volume_data_8,strlen(szTemp) );
#else
				ASCII2Bin( (BYTE*)strData.GetBuffer(0),(BYTE*)&volume_data_8,strData.GetLength() );
#endif
				//teana hu 2010.05.06
				if( PP_BIG_ENDIAN == m_nEndian )
				{
					EndianConvertion( (WORD*)&volume_data_8,m_nVolumeDataSize / 2 );
				}
				//

				CopyVolumeStructFrom8ToMax(volume_data, volume_data_8);
			}
			else if( m_nVolumeDataSize == sizeof(VOLUME_STRUCT_ONE))
			{
				VOLUME_STRUCT_ONE volume_data_1;
				memset(&volume_data_1, 0, sizeof(VOLUME_STRUCT_ONE));
#ifdef _UNICODE
				char szTemp[4096] = {0};
				WideCharToMultiByte(CP_ACP,0,strData.GetBuffer(strData.GetLength()),-1,szTemp,strData.GetLength(),NULL,NULL);
				ASCII2Bin( (BYTE*)szTemp,(BYTE*)&volume_data_1,strlen(szTemp) );
#else
				ASCII2Bin( (BYTE*)strData.GetBuffer(0),(BYTE*)&volume_data_1,strData.GetLength() );
#endif
				//teana hu 2010.05.06
				if( PP_BIG_ENDIAN == m_nEndian )
				{
					EndianConvertion( (WORD*)&volume_data_1,m_nVolumeDataSize / 2 );
				}
				//

				CopyVolumeStructFrom1ToMax(volume_data, volume_data_1);
			}
			else if(m_nVolumeDataSize == sizeof(VOLUME_STRUCT_SIX))
			{
				VOLUME_STRUCT_SIX volume_data_6;
				memset(&volume_data_6, 0, sizeof(VOLUME_STRUCT_SIX));
#ifdef _UNICODE
				char szTemp[4096] = {0};
				WideCharToMultiByte(CP_ACP,0,strData.GetBuffer(strData.GetLength()),-1,szTemp,strData.GetLength(),NULL,NULL);
				ASCII2Bin( (BYTE*)szTemp,(BYTE*)&volume_data_6,strlen(szTemp) );
#else
				ASCII2Bin( (BYTE*)strData.GetBuffer(0),(BYTE*)&volume_data_6,strData.GetLength() );
#endif
				//teana hu 2010.05.06
				if( PP_BIG_ENDIAN == m_nEndian )
				{
					ConvertArmStructLongEndian( volume_data_6);
				}
				//
				
				CopyVolumeStructFrom6ToMax(volume_data, volume_data_6);
			}
			else if(m_nVolumeDataSize == sizeof(VOLUME_STRUCT_SIXTEEN))
			{
				VOLUME_STRUCT_SIXTEEN volume_data_16;
				memset(&volume_data_16, 0, sizeof(VOLUME_STRUCT_SIXTEEN));
#ifdef _UNICODE
				char szTemp[4096] = {0};
				WideCharToMultiByte(CP_ACP,0,strData.GetBuffer(strData.GetLength()),-1,szTemp,strData.GetLength(),NULL,NULL);
				ASCII2Bin( (BYTE*)szTemp,(BYTE*)&volume_data_16,strlen(szTemp) );
#else
				ASCII2Bin( (BYTE*)strData.GetBuffer(0),(BYTE*)&volume_data_16,strData.GetLength() );
#endif
				//teana hu 2010.05.06
				if( PP_BIG_ENDIAN == m_nEndian )
				{
					EndianConvertion( (WORD*)&volume_data_16,m_nVolumeDataSize / 2 );
				}
				//
				
				CopyVolumeStructFrom16ToMax(volume_data, volume_data_16);
			}
			else
			{
				AfxMessageBox(_T("Data Error! Please check arm audio struct(number of app_config_info must be 1, 2, 8 or 16)."));
				return FALSE;
			}
			//
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

BOOL CCommand::SetVolumeData( int nIndex, int nType, CString strModeName, VOLUME_STRUCT& volume_data)
{
    if( !m_bConnected )
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor wc;
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

	VOLUME_STRUCT_MIN temp_data_8;
	VOLUME_STRUCT_ONE temp_data_1;
	memset(&temp_data_8, 0, sizeof(VOLUME_STRUCT_MIN));
	memset(&temp_data_1, 0, sizeof(VOLUME_STRUCT_ONE));
	//teana hu 2012.01.30
	VOLUME_STRUCT_SIX temp_data_6;
	VOLUME_STRUCT_SIXTEEN temp_data_16;
	memset(&temp_data_6, 0, sizeof(VOLUME_STRUCT_SIX));
	memset(&temp_data_16, 0, sizeof(VOLUME_STRUCT_SIXTEEN));
	//
	UINT nArmSerialCount = AUDIO_ARM_APP_TYPE_MAX;
	CAudioTesterSheet* pSheet = (CAudioTesterSheet*)AfxGetMainWnd();
	if(pSheet != NULL)
	{
		nArmSerialCount = pSheet->m_nArmSerialCount;
	}

    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;

    VOLUME_STRUCT temp_data = volume_data;
    
    m_strSucessFlag = ">";          
    
    strATCommand.Format(_T("AT+SADM=%d,%d\r\n"),nType, nIndex );   
    ResetEvent( m_hEventGetResult ); 
	if(nType == SET_ARM_VOLUME_DATA_TO_RAM)
	{
		Log(_T("Write audio_arm structure to ram:"),FALSE );
	}
	else
	{
		Log(_T("Write audio_arm structure to flash:"),FALSE );
	}

#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif      
    
    //synchronize with thread which check the AT command result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    //restore the return string into default AT command successful string
    m_strSucessFlag = "OK";
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    // 0X3456 --->0X5634
	//teana hu 2010.05.06
	if( AUDIO_ARM_APP_TYPE_MAX == nArmSerialCount )
	{
		CopyVolumeStructFromMaxTo16(temp_data_16, temp_data);
	}
	else if( AUDIO_ARM_APP_TYPE_MIN == nArmSerialCount )
	{
		CopyVolumeStructFromMaxTo8(temp_data_8, temp_data);
	}
	else if( AUDIO_ARM_APP_TYPE_ONE == nArmSerialCount)
	{
		CopyVolumeStructFromMaxTo1(temp_data_1, temp_data);
	}
	else if(AUDIO_ARM_APP_TYPE_SIX == nArmSerialCount)
	{
		CopyVolumeStructFromMaxTo6(temp_data_6, temp_data);
	}

	if( PP_BIG_ENDIAN == m_nEndian )
	{
		if( AUDIO_ARM_APP_TYPE_MAX == nArmSerialCount )
		{
			//CopyVolumeStructFromMaxTo16(temp_data_16, temp_data);
			EndianConvertion( (WORD*)&temp_data_16,sizeof( VOLUME_STRUCT_SIXTEEN ) / 2 );
		}
		else if( AUDIO_ARM_APP_TYPE_MIN == nArmSerialCount )
		{
			//CopyVolumeStructFromMaxTo8(temp_data_8, temp_data);
			EndianConvertion( (WORD*)&temp_data_8,sizeof( VOLUME_STRUCT_MIN ) / 2 );
		}
		else if( AUDIO_ARM_APP_TYPE_ONE == nArmSerialCount)
		{
			//CopyVolumeStructFromMaxTo1(temp_data_1, temp_data);
			EndianConvertion( (WORD*)&temp_data_1,sizeof( VOLUME_STRUCT_MIN ) / 2 );
		}
		else if(AUDIO_ARM_APP_TYPE_SIX == nArmSerialCount)
		{
			//CopyVolumeStructFromMaxTo6(temp_data_6, temp_data);
			ConvertArmStructLongEndian( temp_data_6);
		}
	}
	//

	if(AUDIO_ARM_APP_TYPE_MAX == nArmSerialCount )
	{
		BYTE* pASCIIData = new BYTE[sizeof(VOLUME_STRUCT_SIXTEEN)*2+2 ];
		//0X5634--->'5''6''3''4'
		Bin2ASCII((BYTE*)&temp_data_16,pASCIIData,sizeof(VOLUME_STRUCT_SIXTEEN));
		
		//append ctrl+z
		pASCIIData[m_nVolumeDataSize*2] = 0X1A;
		pASCIIData[m_nVolumeDataSize*2+1] = 0;
		ResetEvent( m_hEventGetResult );       
		//m_Diag.Send( PPP_PACKET_R, 0, pASCIIData,m_nAudioDataSize*2+1 );	  
		SendATCmd( (char*)pASCIIData );
        
		delete [] pASCIIData;
	}
	else if( AUDIO_ARM_APP_TYPE_MIN == nArmSerialCount )
	{
		BYTE* pASCIIData = new BYTE[sizeof(VOLUME_STRUCT_MIN)*2+2 ];
		//0X5634--->'5''6''3''4'
		Bin2ASCII((BYTE*)&temp_data_8,pASCIIData,sizeof(VOLUME_STRUCT_MIN));
		
		//append ctrl+z
		pASCIIData[m_nVolumeDataSize*2] = 0X1A;
		pASCIIData[m_nVolumeDataSize*2+1] = 0;
		ResetEvent( m_hEventGetResult );       
		//m_Diag.Send( PPP_PACKET_R, 0, pASCIIData,m_nAudioDataSize*2+1 );	  
		SendATCmd( (char*)pASCIIData );
        
		delete [] pASCIIData;
	}
	else if(AUDIO_ARM_APP_TYPE_ONE == nArmSerialCount)
	{
		BYTE* pASCIIData = new BYTE[sizeof(VOLUME_STRUCT_ONE)*2+2 ];
		//0X5634--->'5''6''3''4'
		Bin2ASCII((BYTE*)&temp_data_1,pASCIIData,sizeof(VOLUME_STRUCT_ONE));
		
		//append ctrl+z
		pASCIIData[m_nVolumeDataSize*2] = 0X1A;
		pASCIIData[m_nVolumeDataSize*2+1] = 0;
		ResetEvent( m_hEventGetResult );       
		//m_Diag.Send( PPP_PACKET_R, 0, pASCIIData,m_nAudioDataSize*2+1 );	  
		SendATCmd( (char*)pASCIIData );
        
		delete [] pASCIIData;
	}
	else if(AUDIO_ARM_APP_TYPE_SIX == nArmSerialCount)
	{
		BYTE* pASCIIData = new BYTE[sizeof(VOLUME_STRUCT_SIX)*2+2 ];
		//0X5634--->'5''6''3''4'
		Bin2ASCII((BYTE*)&temp_data_6,pASCIIData,sizeof(VOLUME_STRUCT_SIX));
		
		//append ctrl+z
		pASCIIData[m_nVolumeDataSize*2] = 0X1A;
		pASCIIData[m_nVolumeDataSize*2+1] = 0;
		ResetEvent( m_hEventGetResult );       
		//m_Diag.Send( PPP_PACKET_R, 0, pASCIIData,m_nAudioDataSize*2+1 );	  
		SendATCmd( (char*)pASCIIData );
        
		delete [] pASCIIData;
	}
    
    //synchronize with thread which check the AT comamnd result   
    dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log(_T("Failed to Execute command! "),TRUE);
            Log( _T("Command response is ") + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log(_T("Execute command sucessfully! ") );
        }        
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log(_T("Execute Command  Time Out! "),TRUE);
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log(_T("Failed to Execute command! "),TRUE);   
        return FALSE;
    }

    return TRUE;
}

BOOL CCommand::GetVolumeModeName( int nModeIndex,  CString& strModeName )
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor wc;

	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
    
    m_strSucessFlag = "OK";          

    strATCommand.Format(_T("AT+SADM=%d,%d\r\n"),GET_ARM_VOLUME_MODE_NAME, nModeIndex );    
    ResetEvent( m_hEventGetResult );   
	Log(_T("Read audio_arm mode name from phone:"),FALSE );
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif     
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
        
        // The return string may be '+SADM:0,"ModeName"'
        int nCommaPos = m_strReturn.Find( '"' );
        if( nCommaPos == -1 )
        {
            return FALSE;
        }
        else
        {
            strModeName = m_strReturn.Right( m_strReturn.GetLength() - nCommaPos - 1 );
            strModeName.Remove( '"' );
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

BOOL CCommand::GetEQTunModeCount( int& nModeCount, int nType)
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }
	
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
	
	m_strReturn.Empty();
	nModeCount = 0;
	
    CWaitCursor wc;
    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
    
    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+SPENHA=%d,%d\r\n"), nType, GET_ENHA_MODE_COUNT );    
    ResetEvent( m_hEventGetResult ); 
	if(nType == TYPE_EQ)
	{
		Log(_T("Read untunable eq(8800G) mode count name from phone:"),FALSE );
	}
	else
	{
		Log(_T("Read tunable eq(8800G) mode count name from phone:"),FALSE );
	}
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif     
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",FALSE );  
            Log( "Command response is " + m_strReturn ,FALSE );
			//teana hu 2011.05.19
			if(nType == TYPE_TUN)
			{
				Log( _T("Only \"8800G\" support \"Music Mode\" at present. Please make sure telephone support it.") ,TRUE );
			}
			//
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
#ifdef _UNICODE
		swscanf( m_strReturn,_T("+SPENHA:%d"),&nModeCount );
#else
		sscanf( (LPSTR)(LPCTSTR)m_strReturn,"+SPENHA:%d",&nModeCount );
#endif
		if(nModeCount == 0)
		{
			Log(_T("Error! 8800G Eq mode count is 0!  (Return:  " + m_strReturn+ ")"),TRUE );  
			return FALSE;
		}
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",FALSE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",FALSE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

BOOL CCommand::GetEQGData( int nIndex, int nPos, EQ_STRUCT_G_T& eq_data)
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor wc;

	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;

	memset( &eq_data,0,sizeof( EQ_STRUCT_G_T ) );

    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+SPENHA=%d,%d,%d\r\n"),TYPE_EQ, nPos, nIndex );    
    ResetEvent( m_hEventGetResult );  
	if(nPos == GET_ENHA_DATA_FROM_RAM)
	{
		Log(_T("Read untunable eq(8800G) structure from ram:"),FALSE );
	}
	else
	{
		Log(_T("Read untunable eq(8800G) structure from flash:"),FALSE );
	}
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif     
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
        
        // The return string may be '+SADM:0,"ModeName",modedata'
        int nCommaPos = m_strReturn.ReverseFind( ',' );
        if( nCommaPos == -1 )
        {
            return FALSE;
        }
        else
        {
            CString strData;
            strData = m_strReturn.Right( m_strReturn.GetLength() - nCommaPos - 1 );
            // Two bytes represent one byte binary data
            m_nEQGGDataSize = strData.GetLength() / 2;
#ifdef _UNICODE
			char szTemp[4096] = {0};
			WideCharToMultiByte(CP_ACP,0,strData.GetBuffer(strData.GetLength()),-1,szTemp,strData.GetLength(),NULL,NULL);
			ASCII2Bin( (BYTE*)szTemp,(BYTE*)&eq_data,strlen(szTemp) );
#else
			ASCII2Bin( (BYTE*)strData.GetBuffer(0),(BYTE*)&eq_data,strData.GetLength() );
#endif
			//teana hu 2010.05.06
			if( PP_BIG_ENDIAN == m_nEndian )
			{
				EndianConvertion( (WORD*)&eq_data,m_nEQGGDataSize / 2 );
			}
			//
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

BOOL CCommand::GetTunData( int nIndex, int nPos, TUN_STRUCT_G_T& tun_data)
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }
	
    CWaitCursor wc;
	
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
	
    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
	
	memset( &tun_data,0,sizeof( TUN_STRUCT_G_T ) );
	
    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+SPENHA=%d,%d,%d\r\n"),TYPE_TUN, nPos, nIndex );    
    ResetEvent( m_hEventGetResult );  
	if(nPos == GET_ENHA_DATA_FROM_RAM)
	{
		Log(_T("Read tunable eq(8800G) structure from ram:"),FALSE );
	}
	else
	{
		Log(_T("Read tunable eq(8800G) structure from flash:"),FALSE );
	}
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif     
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
        
        // The return string may be '+SADM:0,"ModeName",modedata'
        int nCommaPos = m_strReturn.ReverseFind( ',' );
        if( nCommaPos == -1 )
        {
            return FALSE;
        }
        else
        {
            CString strData;
            strData = m_strReturn.Right( m_strReturn.GetLength() - nCommaPos - 1 );
            // Two bytes represent one byte binary data
            m_nTundataSize = strData.GetLength() / 2;
#ifdef _UNICODE
			char szTemp[4096] = {0};
			WideCharToMultiByte(CP_ACP,0,strData.GetBuffer(strData.GetLength()),-1,szTemp,strData.GetLength(),NULL,NULL);
			ASCII2Bin( (BYTE*)szTemp,(BYTE*)&tun_data,strlen(szTemp) );
#else
			ASCII2Bin( (BYTE*)strData.GetBuffer(0),(BYTE*)&tun_data,strData.GetLength() );
#endif
			//teana hu 2010.05.06
			if( PP_BIG_ENDIAN == m_nEndian )
			{
				EndianConvertion( (WORD*)&tun_data,m_nTundataSize / 2 );
			}
			//
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

BOOL CCommand::SetEQGData( int nIndex, int nPos, EQ_STRUCT_G_T& eq_data, int nEQModeSelType)
{
	if( !m_bConnected )
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor wc;
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;

    EQ_STRUCT_G_T temp_data = eq_data;
    
    m_strSucessFlag = ">";          
    if(nPos == SET_ENHA_DATA_TO_RAM)
	{
		strATCommand.Format(_T("AT+SPENHA=%d,%d,%d,%d\r\n"),TYPE_EQ, nPos, nIndex, nEQModeSelType ); 
	}
	else if(nPos == SET_ENHA_DATA_TO_FLASH)
	{
		strATCommand.Format(_T("AT+SPENHA=%d,%d,%d\r\n"),TYPE_EQ, nPos, nIndex ); 
	}
       
    ResetEvent( m_hEventGetResult ); 
	if(nPos == SET_ENHA_DATA_TO_RAM)
	{
		Log(_T("Write untunable eq(8800G) structure to ram:"),FALSE );
	}
	else
	{
		Log(_T("Write untunable eq(8800G) structure to flash:"),FALSE );
	}
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif      
    
    //synchronize with thread which check the AT command result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    //restore the return string into default AT command successful string
    m_strSucessFlag = "OK";
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    // 0X3456 --->0X5634
	//teana hu 2010.05.06
	if( PP_BIG_ENDIAN == m_nEndian )
	{
		EndianConvertion( (WORD*)&temp_data,sizeof( VOLUME_STRUCT ) / 2 );
	}
	//
    
    BYTE* pASCIIData = new BYTE[sizeof(VOLUME_STRUCT)*2+2 ];
    //0X5634--->'5''6''3''4'
    Bin2ASCII((BYTE*)&temp_data,pASCIIData,sizeof(VOLUME_STRUCT));
    
    //append ctrl+z
    pASCIIData[m_nEQGGDataSize*2] = 0X1A;
	pASCIIData[m_nEQGGDataSize*2+1] = 0;
    ResetEvent( m_hEventGetResult );       
    //m_Diag.Send( PPP_PACKET_R, 0, pASCIIData,m_nAudioDataSize*2+1 );	  
	SendATCmd( (char*)pASCIIData );
        
    delete [] pASCIIData;
    
    //synchronize with thread which check the AT comamnd result   
    dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log(_T("Failed to Execute command! "),TRUE);
            Log( _T("Command response is ") + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log(_T("Execute command sucessfully! ") );
        }        
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log(_T("Execute Command  Time Out! "),TRUE);
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log(_T("Failed to Execute command! "),TRUE);   
        return FALSE;
    }

    return TRUE;
}

BOOL CCommand::SetTunData( int nIndex, int nPos, TUN_STRUCT_G_T& tun_data, int nEQModeSelType)
{
	if( !m_bConnected )
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor wc;
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;

    TUN_STRUCT_G_T temp_data = tun_data;
    
    m_strSucessFlag = ">";          
    if(nPos == SET_ENHA_DATA_TO_RAM)
	{
		strATCommand.Format(_T("AT+SPENHA=%d,%d,%d,%d\r\n"),TYPE_TUN, nPos, nIndex, nEQModeSelType );  
	}
	else if(nPos == SET_ENHA_DATA_TO_FLASH)
	{
		strATCommand.Format(_T("AT+SPENHA=%d,%d,%d\r\n"),TYPE_TUN, nPos, nIndex );  
	}
      
    ResetEvent( m_hEventGetResult );    
	if(nPos == SET_ENHA_DATA_TO_RAM)
	{
		Log(_T("Write tunable eq(8800G) structure to ram:"),FALSE );
	}
	else
	{
		Log(_T("Write tunable eq(8800G) structure to flash:"),FALSE );
	}
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif      
    
    //synchronize with thread which check the AT command result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    //restore the return string into default AT command successful string
    m_strSucessFlag = "OK";
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    // 0X3456 --->0X5634
	//teana hu 2010.05.06
	if( PP_BIG_ENDIAN == m_nEndian )
	{
		EndianConvertion( (WORD*)&temp_data,sizeof( VOLUME_STRUCT ) / 2 );
	}
	//
    
    BYTE* pASCIIData = new BYTE[sizeof(VOLUME_STRUCT)*2+2 ];
    //0X5634--->'5''6''3''4'
    Bin2ASCII((BYTE*)&temp_data,pASCIIData,sizeof(VOLUME_STRUCT));
    
    //append ctrl+z
    pASCIIData[m_nTundataSize*2] = 0X1A;
	pASCIIData[m_nTundataSize*2+1] = 0;
    ResetEvent( m_hEventGetResult );       
    //m_Diag.Send( PPP_PACKET_R, 0, pASCIIData,m_nAudioDataSize*2+1 );	  
	SendATCmd( (char*)pASCIIData );
        
    delete [] pASCIIData;
    
    //synchronize with thread which check the AT comamnd result   
    dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log(_T("Failed to Execute command! "),TRUE);
            Log( _T("Command response is ") + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log(_T("Execute command sucessfully! ") );
        }        
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log(_T("Execute Command  Time Out! "),TRUE);
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log(_T("Failed to Execute command! "),TRUE);   
        return FALSE;
    }

    return TRUE;
}

BOOL CCommand::SetVolumePara( int nVolumeIndex, int nApptype )
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }
	
	CWaitCursor wc;
	
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
	
    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
	
    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+SPAUDVOLTYPE=%d,%d\r\n"), nVolumeIndex, nApptype);
	ResetEvent( m_hEventGetResult ); 
	Log(_T("Write arm volume to phone for music mode:"),FALSE );
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() ); 		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif   
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",FALSE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",FALSE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

BOOL CCommand::GetLVVEModeCount( int& nModeCount )
{
	
    if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }
	
    CWaitCursor wc;
	
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
	
    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
    
    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+SPADLVVE=%d\r\n"), GET_LVVE_MODE_COUNT);    
    ResetEvent( m_hEventGetResult );

	Log(_T("Read NXP mode count from phone:"),FALSE );
	
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif         
	
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
			//            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
			//            Log( "Command response is " + m_strReturn ,TRUE );
			Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE ); 
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
#ifdef _UNICODE
		swscanf( m_strReturn,_T("+SPADLVVE:%d"),&nModeCount );
#else
		sscanf( (LPSTR)(LPCTSTR)m_strReturn,"+SPADLVVE:%d",&nModeCount );
#endif
		if(nModeCount == 0)
		{
			Log(_T("Error! NXP mode count is 0!  (Return:  " + m_strReturn+ ")"),TRUE );  
			return FALSE;
		}
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

BOOL CCommand::GetLVVEModeName( int nModeIndex,  CString& strModeName )
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }
	
    CWaitCursor wc;
	
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
	
    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
    
    m_strSucessFlag = "OK";          
	
    strATCommand.Format(_T("AT+SPADLVVE=%d,%d\r\n"),GET_LVVE_MODE_NAME,nModeIndex );    
    ResetEvent( m_hEventGetResult );  
	Log(_T("Read NXP mode name from phone:"),FALSE );
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif     
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
        
        // The return string may be '+SADM:0,"ModeName"'
        int nCommaPos = m_strReturn.Find( '"' );
        if( nCommaPos == -1 )
        {
            return FALSE;
        }
        else
        {
            strModeName = m_strReturn.Right( m_strReturn.GetLength() - nCommaPos - 1 );
            strModeName.Remove( '"' );
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

void CCommand::ConvertLvveEndian( LVVE_CTRL_PARAM_NV_T& lvve_data)
{
	CONVERT_INT( lvve_data.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode,lvve_data.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode );
	CONVERT_INT( lvve_data.lvve_ctrl_param_t.ControlParams_Rx.Mute,lvve_data.lvve_ctrl_param_t.ControlParams_Rx.Mute );
	CONVERT_INT( lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode,lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode );

	CONVERT_SHORT( lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain,lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain );

	//teana hu 2012.01.09
	CONVERT_INT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams.OperatingMode, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams.OperatingMode);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams.FENS_limit_NS, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams.FENS_limit_NS);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams.Mode, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams.Mode);

	//
	CONVERT_INT( lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode,lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode );
	CONVERT_SHORT( lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams.NLPP_Limit,lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams.NLPP_Limit );
	CONVERT_SHORT( lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams.NLPP_HPF_CornerFreq,lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams.NLPP_HPF_CornerFreq );
	//teana hu 2012.01.09
	CONVERT_INT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.OperatingMode, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.OperatingMode);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Attack, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Attack);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Limit_MaxOutputLin, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Limit_MaxOutputLin);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_MaxGainLin, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_MaxGainLin);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_MinGainLin, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_MinGainLin);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_NominalGain, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_NominalGain);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Release, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Release);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_ReleaseMax, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_ReleaseMax);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Target_level_lin, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Target_level_lin);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.Gain_Lin_Max, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.Gain_Lin_Max);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.mode, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.mode);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.Noise_Sensitivity, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.Noise_Sensitivity);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.SpDetect_Threshold, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.SpDetect_Threshold);
	//

	CONVERT_INT( lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode,lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode );

	CONVERT_SHORT( lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams.EQ_Length,lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams.EQ_Length );
	long temp = long(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams.pEQ_Coefs);
	CONVERT_INT( temp,temp );
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams.pEQ_Coefs = (short*)temp;

	//teana hu 2012.01.09
	CONVERT_INT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.OperatingMode, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.OperatingMode);
	CONVERT_INT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.LimiterOperatingMode, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.LimiterOperatingMode);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.NumKnees, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.NumKnees);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.AttackTime, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.AttackTime);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.ReleaseTime, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.ReleaseTime);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.LimitLevel, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.LimitLevel); 
	for(int i = 0; i < LVMDRC_MAX_KNEES; i++)
	{
		CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.CompressorCurveInputLevels[i], lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.CompressorCurveInputLevels[i]);
	}
	
	for(i = 0; i < LVMDRC_MAX_KNEES; i++)
	{
		CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.CompressorCurveOutputLevels[i], lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.CompressorCurveOutputLevels[i]);
	}
	
	CONVERT_INT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.HPF_OperatingMode, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.HPF_OperatingMode);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.HPF_CornerFreq, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.HPF_CornerFreq);
	CONVERT_INT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.CNG_OperatingMode, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.CNG_OperatingMode);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.CNG_ControlParams.CNG_Volume, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.CNG_ControlParams.CNG_Volume);
	CONVERT_INT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.OperatingMode, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.OperatingMode);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Attack, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Attack);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Limit_MaxOutputLin, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Limit_MaxOutputLin);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_MaxGainLin, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_MaxGainLin);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_MinGainLin, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_MinGainLin);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Release, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Release);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Target_level_lin, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Target_level_lin);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.mode, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.mode);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.SpDetect_Threshold, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.SpDetect_Threshold);
	
	CONVERT_INT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.OperatingMode, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.OperatingMode);
	CONVERT_INT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.LimiterOperatingMode, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.LimiterOperatingMode);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.AttackTime, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.AttackTime);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.LimitLevel, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.LimitLevel);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.NumKnees, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.NumKnees);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.ReleaseTime, lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.ReleaseTime);
	for(i = 0; i < LVMDRC_MAX_KNEES; i++)
	{
		CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.CompressorCurveInputLevels[i], lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.CompressorCurveInputLevels[i]);
	}
	
	for(i = 0; i < LVMDRC_MAX_KNEES; i++)
	{
		CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.CompressorCurveOutputLevels[i], lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.CompressorCurveOutputLevels[i]);
	}
	//

	for( i=0;i<LVVE_EQ_COEF_SIZE;i++ )
	{
		CONVERT_SHORT( lvve_data.lvve_ctrl_param_t.RxEqCoefs[i],lvve_data.lvve_ctrl_param_t.RxEqCoefs[i] );
	}

	CONVERT_INT( lvve_data.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode,lvve_data.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode );
	CONVERT_INT( lvve_data.lvve_ctrl_param_t.ControlParams_Tx.Mute,lvve_data.lvve_ctrl_param_t.ControlParams_Tx.Mute );
	CONVERT_INT( lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode,lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode );
	CONVERT_SHORT( lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay,lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay );
	CONVERT_INT( lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode,lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode );
	CONVERT_SHORT( lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain,lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain );
	CONVERT_INT( lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode,lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode );
	CONVERT_SHORT( lvve_data.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq,lvve_data.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq );

	CONVERT_INT( lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.OperatingMode,lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.OperatingMode );
	WORD* lpTemp = (WORD*)&lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams;
	lpTemp += 2;
	EndianConvertion( lpTemp, sizeof( LVHF_ControlParams_st ) / 2 - 2 );

	//teana hu 2012.01.09
	CONVERT_INT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.OperatingMode, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.OperatingMode);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.CAL_micPowFloorMin, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.CAL_micPowFloorMin);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_AcThreshold, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_AcThreshold);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_EchoAlphaRev, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_EchoAlphaRev);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_EchoGammaHi, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_EchoGammaHi);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_EchoGammaLo, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_EchoGammaLo);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_EchoTailPortion, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_EchoTailPortion);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_GainEta, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_GainEta);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_LostBeamThreshold, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_LostBeamThreshold);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NlAtten, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NlAtten);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NoiseBiasComp, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NoiseBiasComp);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NoiseGainMinN, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NoiseGainMinN);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NoiseGainMinS, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NoiseGainMinS);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NoiseGammaN, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NoiseGammaN);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NoiseGammaS, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NoiseGammaS);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_WbThreshold, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_WbThreshold);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.FSB_ref_gain, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.FSB_ref_gain);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.FSB_taps, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.FSB_taps);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.FSB_twoalpha, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.FSB_twoalpha);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.GSC_erl, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.GSC_erl);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.GSC_offset, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.GSC_offset);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.GSC_taps, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.GSC_taps);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.GSC_twoalpha, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.GSC_twoalpha);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Input_Gain_Mic0, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Input_Gain_Mic0);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Input_Gain_Mic1, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Input_Gain_Mic1);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Mode, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Mode);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Mode2, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Mode2);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.MpThreshold, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.MpThreshold);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_HB_erl, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_HB_erl);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_HB_taps, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_HB_taps);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_HB_twoalpha, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_HB_twoalpha);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_LB_erl, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_LB_erl);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_LB_taps, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_LB_taps);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_LB_twoalpha, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_LB_twoalpha);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_offset, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_offset);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_preset_coefs, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_preset_coefs);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_HB_erl, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_HB_erl);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_HB_taps, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_HB_taps);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_HB_twoalpha, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_HB_twoalpha);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_LB_erl, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_LB_erl);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_LB_taps, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_LB_taps);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_LB_twoalpha, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_LB_twoalpha);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_offset, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_offset);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_preset_coefs, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_preset_coefs);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Output_Gain, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Output_Gain);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.PCD_beta, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.PCD_beta);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.PCD_Threshold, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.PCD_Threshold);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Tuning_mode, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Tuning_mode);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.WgThreshold, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.WgThreshold);
	for(i = 0; i < 8; i++)
	{
		CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.FSB_init_table0[i], lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.FSB_init_table0[i]);
	}
	
	for(i = 0; i < 8; i++)
	{
		CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.FSB_init_table1[i], lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.FSB_init_table1[i]);
	}
	
	CONVERT_INT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.OperatingMode, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.OperatingMode);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Attack, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Attack);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Limit_MaxOutputLin, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Limit_MaxOutputLin);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_MaxGainLin, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_MaxGainLin);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_MinGainLin, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_MinGainLin);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Release, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Release);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Target_level_lin, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Target_level_lin);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.mode, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.mode);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.SpDetect_Threshold, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.SpDetect_Threshold);
	
	CONVERT_INT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.OperatingMode, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.OperatingMode);
	CONVERT_INT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.LimiterOperatingMode, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.LimiterOperatingMode);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.AttackTime, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.AttackTime);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.LimitLevel, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.LimitLevel);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.NumKnees, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.NumKnees);
	CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.ReleaseTime, lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.ReleaseTime);
	for(i = 0; i < LVMDRC_MAX_KNEES; i++)
	{
		CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.CompressorCurveInputLevels[i], lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.CompressorCurveInputLevels[i]);
	}
	
	for(i = 0; i < LVMDRC_MAX_KNEES; i++)
	{
		CONVERT_SHORT(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.CompressorCurveOutputLevels[i], lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.CompressorCurveOutputLevels[i]);
	}

	CONVERT_SHORT( lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams.EQ_Length,lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams.EQ_Length );
	temp = long(lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams.pEQ_Coefs);
	CONVERT_INT( temp,temp );
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams.pEQ_Coefs = (short*)temp;

	for( i=0;i<LVVE_EQ_COEF_SIZE;i++ )
	{
		CONVERT_SHORT( lvve_data.lvve_ctrl_param_t.TxEqCoefs[i],lvve_data.lvve_ctrl_param_t.TxEqCoefs[i] );
	}

	for( i=0;i<LVVE_CONFIG_RESERVE_LEN;i++ )
	{
		CONVERT_SHORT( lvve_data.lvve_reserve[i],lvve_data.lvve_reserve[i] );
	}

}

void CCommand::ConvertArmStructLongEndian(VOLUME_STRUCT_SIX& volume_data)
{
	CONVERT_SHORT( volume_data.midi_opt, volume_data.midi_opt);
	CONVERT_SHORT( volume_data.aud_dev, volume_data.aud_dev);

	for(int i = 0; i < AUDIO_NV_ARM_PARA_RESERVE; i++)
	{
		CONVERT_SHORT( volume_data.reserve[i], volume_data.reserve[i]);
	}

	CONVERT_SHORT( volume_data.dev_path_set.valid_dev_set_count, volume_data.dev_path_set.valid_dev_set_count);
	CONVERT_SHORT( volume_data.dev_path_set.reserve, volume_data.dev_path_set.reserve);
	for(i = 0; i < AUDIO_DEVICE_MODE_PARAM_MAX; i++)
	{
		CONVERT_SHORT( volume_data.dev_path_set.dev_set[i], volume_data.dev_path_set.dev_set[i]);
	}

	CONVERT_SHORT( volume_data.app_config_info_set.valid_app_set_count, volume_data.app_config_info_set.valid_app_set_count);
	CONVERT_SHORT( volume_data.app_config_info_set.valid_agc_input_gain_count, volume_data.app_config_info_set.valid_agc_input_gain_count);
	CONVERT_SHORT( volume_data.app_config_info_set.aud_proc_exp_control[0], volume_data.app_config_info_set.aud_proc_exp_control[0]);
	CONVERT_SHORT( volume_data.app_config_info_set.aud_proc_exp_control[1], volume_data.app_config_info_set.aud_proc_exp_control[1]);

	short eq_switch;
	short agc_input_gain[AUDIO_AGC_INPUG_GAIN_MAX];
	short valid_volume_level_count;
	LONG arm_volume[AUDIO_ARM_VOLUME_LEVEL];
	short reserve[AUDIO_NV_ARM_APP_PARA_RESERVE];

	for(i = 0; i < AUDIO_ARM_APP_TYPE_SIX; i++)
	{
		CONVERT_SHORT( volume_data.app_config_info_set.app_config_info[i].eq_switch, volume_data.app_config_info_set.app_config_info[i].eq_switch);
		CONVERT_SHORT( volume_data.app_config_info_set.app_config_info[i].valid_volume_level_count, volume_data.app_config_info_set.app_config_info[i].valid_volume_level_count);
		for(int j = 0; j < AUDIO_AGC_INPUG_GAIN_MAX; j++)
		{
			CONVERT_SHORT( volume_data.app_config_info_set.app_config_info[i].agc_input_gain[j], volume_data.app_config_info_set.app_config_info[i].agc_input_gain[j]);
		}

		for(j = 0; j < AUDIO_NV_ARM_APP_PARA_RESERVE; j++)
		{
			CONVERT_SHORT( volume_data.app_config_info_set.app_config_info[i].reserve[j], volume_data.app_config_info_set.app_config_info[i].reserve[j]);
		}

		for(j = 0; j < AUDIO_ARM_VOLUME_LEVEL; j++)
		{
			CONVERT_INT( volume_data.app_config_info_set.app_config_info[i].arm_volume[j], volume_data.app_config_info_set.app_config_info[i].arm_volume[j]);
		}
	}


}

BOOL CCommand::GetLVVEData( int nIndex, int nType, LVVE_CTRL_PARAM_NV_T& lvve_data )
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }
	
    CWaitCursor wc;
	
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
	
    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
	
	memset( &lvve_data,0,sizeof( LVVE_CTRL_PARAM_NV_T ) );

	LVVE_CTRL_PARAM_NV_MIN_T lvve_data_min;
	memset(&lvve_data_min,0,sizeof( LVVE_CTRL_PARAM_NV_MIN_T ));

	//teana hu 2012.01.09
	LVVE_CTRL_PARAM_NV_1_T lvve_data_1;
	memset(&lvve_data_1,0,sizeof( LVVE_CTRL_PARAM_NV_1_T ));

	LVVE_CTRL_PARAM_NV_2_T lvve_data_2;
	memset(&lvve_data_2,0,sizeof( LVVE_CTRL_PARAM_NV_2_T ));

	LVVE_CTRL_PARAM_NV_3_T lvve_data_3;
	memset(&lvve_data_3,0,sizeof( LVVE_CTRL_PARAM_NV_3_T ));
	//
	
    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+SPADLVVE=%d,%d\r\n"),nType, nIndex );    
    ResetEvent( m_hEventGetResult ); 
	if(nType == GET_LVVE_DATA_FROM_RAM)
	{
		Log(_T("Read NXP structure from ram:"),FALSE );
	}
	else
	{
		Log(_T("Read NXP structure from flash:"),FALSE );
	}
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif     
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
        
        // The return string may be '+SADM:0,"ModeName",modedata'
        int nCommaPos = m_strReturn.ReverseFind( ',' );
        if( nCommaPos == -1 )
        {
            return FALSE;
        }
        else
        {
            CString strData;
            strData = m_strReturn.Right( m_strReturn.GetLength() - nCommaPos - 1 );
            // Two bytes represent one byte binary data
            m_nLVVEDataSize = strData.GetLength() / 2;
			//teana hu 2012.01.09
			if(m_nLVVEDataSize == sizeof(LVVE_CTRL_PARAM_NV_T))
			{
				m_dwLVVEType = LVVE_DUAL_MIC;
			}
			else if(m_nLVVEDataSize == sizeof(LVVE_CTRL_PARAM_NV_MIN_T))
			{
				m_dwLVVEType = LVVE_NO_RESERVE;
			}
			else if(m_nLVVEDataSize == sizeof(LVVE_CTRL_PARAM_NV_1_T))
			{
				m_dwLVVEType = LVVE_RESERVE;
			}
			else if(m_nLVVEDataSize == sizeof(LVVE_CTRL_PARAM_NV_2_T))
			{
				m_dwLVVEType = LVVE_JUNIPER;
			}
			else if(m_nLVVEDataSize == sizeof(LVVE_CTRL_PARAM_NV_3_T))
			{
				m_dwLVVEType = LVVE_SINGLE_MIC;
			}
			else
			{
				AfxMessageBox(_T("Error! Please check the size of nxp structrue!"));
				return FALSE;
			}

			//
			/*
			if(m_nLVVEDataSize == sizeof(LVVE_CTRL_PARAM_NV_T))
			{
				m_bLVVEMin = FALSE;
			}
			else if(m_nLVVEDataSize == sizeof(LVVE_CTRL_PARAM_NV_MIN_T))
			{
				m_bLVVEMin = TRUE;
			}
			else
			{
				AfxMessageBox(_T("Error! Please check the size of nxp structrue!"));
				return FALSE;
			}
			*/

#ifdef _UNICODE
			char szTemp[4096] = {0};
			WideCharToMultiByte(CP_ACP,0,strData.GetBuffer(strData.GetLength()),-1,szTemp,strData.GetLength(),NULL,NULL);
			//teana hu 2012.01.09
			if(m_dwLVVEType == LVVE_NO_RESERVE)
			{
				ASCII2Bin( (BYTE*)szTemp,(BYTE*)&lvve_data_min,strlen(szTemp) );
				CopyLVVEFromNoReserveToMax(lvve_data, lvve_data_min);
			}
			else if(m_dwLVVEType == LVVE_RESERVE)
			{
				ASCII2Bin( (BYTE*)szTemp,(BYTE*)&lvve_data_1,strlen(szTemp) );
				CopyLVVEFromReserveToMax(lvve_data, lvve_data_1);
			}
			else if(m_dwLVVEType == LVVE_JUNIPER)
			{
				ASCII2Bin( (BYTE*)szTemp,(BYTE*)&lvve_data_2,strlen(szTemp) );
				CopyLVVEFromJuniperToMax(lvve_data, lvve_data_2);
			}
			else if(m_dwLVVEType == LVVE_SINGLE_MIC)
			{
				ASCII2Bin( (BYTE*)szTemp,(BYTE*)&lvve_data_3,strlen(szTemp) );
				CopyLVVEFromSingleMicToMax(lvve_data, lvve_data_3);
			}
			else if(m_dwLVVEType == LVVE_DUAL_MIC)
			{
				ASCII2Bin( (BYTE*)szTemp,(BYTE*)&lvve_data,strlen(szTemp) );
			}
			//
			
			
			/*
			if(m_bLVVEMin)
			{
				ASCII2Bin( (BYTE*)szTemp,(BYTE*)&lvve_data_min,strlen(szTemp) );
				CopyLVVEFromMinToMax(lvve_data, lvve_data_min);
			}
			else
			{
				ASCII2Bin( (BYTE*)szTemp,(BYTE*)&lvve_data,strlen(szTemp) );
			}
			*/
			
#else
			//teana hu 2012.01.09
			if(m_dwLVVEType == LVVE_NO_RESERVE)
			{
				ASCII2Bin( (BYTE*)strData.GetBuffer(0),(BYTE*)&lvve_data_min,strData.GetLength() );
				CopyLVVEFromMinToMax(lvve_data, lvve_data_min);
			}
			else if(m_dwLVVEType == LVVE_RESERVE)
			{
				ASCII2Bin( (BYTE*)strData.GetBuffer(0),(BYTE*)&lvve_data_1,strData.GetLength() );
				CopyLVVEFromReserveToMax(lvve_data, lvve_data_1);
			}
			else if(m_dwLVVEType == LVVE_JUNIPER)
			{
				ASCII2Bin( (BYTE*)strData.GetBuffer(0),(BYTE*)&lvve_data_2,strData.GetLength() );
				CopyLVVEFromJuniperToMax(lvve_data, lvve_data_2);
			}
			else if(m_dwLVVEType == LVVE_SINGLE_MIC)
			{
				ASCII2Bin( (BYTE*)strData.GetBuffer(0),(BYTE*)&lvve_data_3,strData.GetLength() );
				CopyLVVEFromSingleMicToMax(lvve_data, lvve_data_3);
			}
			else if(m_dwLVVEType == LVVE_DUAL_MIC)
			{
				ASCII2Bin( (BYTE*)strData.GetBuffer(0),(BYTE*)&lvve_data,strData.GetLength() );
			}
			//
			/*
			if(m_bLVVEMin)
			{
				ASCII2Bin( (BYTE*)strData.GetBuffer(0),(BYTE*)&lvve_data_min,strData.GetLength() );
				CopyLVVEFromMinToMax(lvve_data, lvve_data_min);
			}
			else
			{
				ASCII2Bin( (BYTE*)strData.GetBuffer(0),(BYTE*)&lvve_data,strData.GetLength() );
			}
			*/
#endif
			//teana hu 2010.05.06
			if( PP_BIG_ENDIAN == m_nEndian )
			{
				//EndianConvertion( (WORD*)&lvve_data,m_nLVVEDataSize / 2 );
				ConvertLvveEndian( lvve_data );
			}
			//
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

BOOL CCommand::SetLVVEData( int nIndex, int nType, CString strModeName, LVVE_CTRL_PARAM_NV_T& lvve_data)
{
	if( !m_bConnected )
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor wc;
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;

    LVVE_CTRL_PARAM_NV_T temp_data = lvve_data;
    
    m_strSucessFlag = ">";          
    
    strATCommand.Format(_T("AT+SPADLVVE=%d,%d\r\n"),nType, nIndex );    
    ResetEvent( m_hEventGetResult ); 
	if(nType == SET_LVVE_DATA_TO_RAM)
	{
		Log(_T("Write NXP structure to ram:"),FALSE );
	}
	else
	{
		Log(_T("Write NXP structure to flash:"),FALSE );
	}
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif      
    
    //synchronize with thread which check the AT command result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    //restore the return string into default AT command successful string
    m_strSucessFlag = "OK";
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 

	//teana hu 2012.01.09
	if(m_dwLVVEType == LVVE_NO_RESERVE)
	{
		LVVE_CTRL_PARAM_NV_MIN_T lvve_data_min;
		memset(&lvve_data_min, 0, sizeof(LVVE_CTRL_PARAM_NV_MIN_T));
		// 0X3456 --->0X5634
		//teana hu 2010.05.06
		if( PP_BIG_ENDIAN == m_nEndian )
		{
			ConvertLvveEndian( temp_data );
		}
		//

		CopyLVVEFromMaxToNoReserve(lvve_data_min, temp_data);
		
		BYTE* pASCIIData = new BYTE[sizeof(LVVE_CTRL_PARAM_NV_MIN_T)*2+2 ];
		//0X5634--->'5''6''3''4'
		Bin2ASCII((BYTE*)&lvve_data_min,pASCIIData,sizeof(LVVE_CTRL_PARAM_NV_MIN_T));
		
		//append ctrl+z
		pASCIIData[m_nLVVEDataSize*2] = 0X1A;
		pASCIIData[m_nLVVEDataSize*2+1] = 0;
		ResetEvent( m_hEventGetResult );       
		//m_Diag.Send( PPP_PACKET_R, 0, pASCIIData,m_nAudioDataSize*2+1 );	  
		SendATCmd( (char*)pASCIIData );
        
		delete [] pASCIIData;
	}
	else if(m_dwLVVEType == LVVE_RESERVE)
	{
		LVVE_CTRL_PARAM_NV_1_T lvve_data_min;
		memset(&lvve_data_min, 0, sizeof(LVVE_CTRL_PARAM_NV_1_T));
		
		// 0X3456 --->0X5634
		//teana hu 2010.05.06
		if( PP_BIG_ENDIAN == m_nEndian )
		{
			ConvertLvveEndian( temp_data );
		}
		//
		CopyLVVEFromMaxToReserve(lvve_data_min, temp_data);
		BYTE* pASCIIData = new BYTE[sizeof(LVVE_CTRL_PARAM_NV_1_T)*2+2 ];
		//0X5634--->'5''6''3''4'
		Bin2ASCII((BYTE*)&lvve_data_min,pASCIIData,sizeof(LVVE_CTRL_PARAM_NV_1_T));
		
		//append ctrl+z
		pASCIIData[m_nLVVEDataSize*2] = 0X1A;
		pASCIIData[m_nLVVEDataSize*2+1] = 0;
		ResetEvent( m_hEventGetResult );       
		//m_Diag.Send( PPP_PACKET_R, 0, pASCIIData,m_nAudioDataSize*2+1 );	  
		SendATCmd( (char*)pASCIIData );
        
		delete [] pASCIIData;
	}
	else if(m_dwLVVEType == LVVE_JUNIPER)
	{
		LVVE_CTRL_PARAM_NV_2_T lvve_data_min;
		memset(&lvve_data_min, 0, sizeof(LVVE_CTRL_PARAM_NV_2_T));
		
		// 0X3456 --->0X5634
		//teana hu 2010.05.06
		if( PP_BIG_ENDIAN == m_nEndian )
		{
			ConvertLvveEndian( temp_data );
		}
		//
		CopyLVVEFromMaxToJuniper(lvve_data_min, temp_data);

		BYTE* pASCIIData = new BYTE[sizeof(LVVE_CTRL_PARAM_NV_2_T)*2+2 ];
		//0X5634--->'5''6''3''4'
		Bin2ASCII((BYTE*)&lvve_data_min,pASCIIData,sizeof(LVVE_CTRL_PARAM_NV_2_T));
		
		//append ctrl+z
		pASCIIData[m_nLVVEDataSize*2] = 0X1A;
		pASCIIData[m_nLVVEDataSize*2+1] = 0;
		ResetEvent( m_hEventGetResult );       
		//m_Diag.Send( PPP_PACKET_R, 0, pASCIIData,m_nAudioDataSize*2+1 );	  
		SendATCmd( (char*)pASCIIData );
        
		delete [] pASCIIData;
	}
	else if(m_dwLVVEType == LVVE_SINGLE_MIC)
	{
		LVVE_CTRL_PARAM_NV_3_T lvve_data_min;
		memset(&lvve_data_min, 0, sizeof(LVVE_CTRL_PARAM_NV_3_T));
		
		// 0X3456 --->0X5634
		//teana hu 2010.05.06
		if( PP_BIG_ENDIAN == m_nEndian )
		{
			ConvertLvveEndian( temp_data );
		}
		//
		CopyLVVEFromMaxToSingleMic(lvve_data_min, temp_data);
		
		BYTE* pASCIIData = new BYTE[sizeof(LVVE_CTRL_PARAM_NV_3_T)*2+2 ];
		//0X5634--->'5''6''3''4'
		Bin2ASCII((BYTE*)&lvve_data_min,pASCIIData,sizeof(LVVE_CTRL_PARAM_NV_3_T));
		
		//append ctrl+z
		pASCIIData[m_nLVVEDataSize*2] = 0X1A;
		pASCIIData[m_nLVVEDataSize*2+1] = 0;
		ResetEvent( m_hEventGetResult );       
		//m_Diag.Send( PPP_PACKET_R, 0, pASCIIData,m_nAudioDataSize*2+1 );	  
		SendATCmd( (char*)pASCIIData );
        
		delete [] pASCIIData;
	}
	else if(m_dwLVVEType == LVVE_DUAL_MIC)
	{
		// 0X3456 --->0X5634
		//teana hu 2010.05.06
		if( PP_BIG_ENDIAN == m_nEndian )
		{
			//EndianConvertion( (WORD*)&temp_data,sizeof( LVVE_CTRL_PARAM_NV_T ) / 2 );
			ConvertLvveEndian( temp_data );
		}
		//
		
		BYTE* pASCIIData = new BYTE[sizeof(LVVE_CTRL_PARAM_NV_T)*2+2 ];
		//0X5634--->'5''6''3''4'
		Bin2ASCII((BYTE*)&temp_data,pASCIIData,sizeof(LVVE_CTRL_PARAM_NV_T));
		
		//append ctrl+z
		pASCIIData[m_nLVVEDataSize*2] = 0X1A;
		pASCIIData[m_nLVVEDataSize*2+1] = 0;
		ResetEvent( m_hEventGetResult );       
		//m_Diag.Send( PPP_PACKET_R, 0, pASCIIData,m_nAudioDataSize*2+1 );	  
		SendATCmd( (char*)pASCIIData );
        
		delete [] pASCIIData;
	}
	//

	/*
	if(m_bLVVEMin)
	{
		LVVE_CTRL_PARAM_NV_MIN_T lvve_data_min;
		memset(&lvve_data_min, 0, sizeof(LVVE_CTRL_PARAM_NV_MIN_T));
		CopyLVVEFromMaxToMin(lvve_data_min, temp_data);
		// 0X3456 --->0X5634
		//teana hu 2010.05.06
		if( PP_BIG_ENDIAN == m_nEndian )
		{
			EndianConvertion( (WORD*)&lvve_data_min,sizeof( LVVE_CTRL_PARAM_NV_MIN_T ) / 2 );
		}
		//
		
		BYTE* pASCIIData = new BYTE[sizeof(LVVE_CTRL_PARAM_NV_MIN_T)*2+2 ];
		//0X5634--->'5''6''3''4'
		Bin2ASCII((BYTE*)&lvve_data_min,pASCIIData,sizeof(LVVE_CTRL_PARAM_NV_MIN_T));
		
		//append ctrl+z
		pASCIIData[m_nLVVEDataSize*2] = 0X1A;
		pASCIIData[m_nLVVEDataSize*2+1] = 0;
		ResetEvent( m_hEventGetResult );       
		//m_Diag.Send( PPP_PACKET_R, 0, pASCIIData,m_nAudioDataSize*2+1 );	  
		SendATCmd( (char*)pASCIIData );
        
		delete [] pASCIIData;
	}
	else
	{
		// 0X3456 --->0X5634
		//teana hu 2010.05.06
		if( PP_BIG_ENDIAN == m_nEndian )
		{
			//EndianConvertion( (WORD*)&temp_data,sizeof( LVVE_CTRL_PARAM_NV_T ) / 2 );
			ConvertLvveEndian( temp_data );
		}
		//
		
		BYTE* pASCIIData = new BYTE[sizeof(LVVE_CTRL_PARAM_NV_T)*2+2 ];
		//0X5634--->'5''6''3''4'
		Bin2ASCII((BYTE*)&temp_data,pASCIIData,sizeof(LVVE_CTRL_PARAM_NV_T));
		
		//append ctrl+z
		pASCIIData[m_nLVVEDataSize*2] = 0X1A;
		pASCIIData[m_nLVVEDataSize*2+1] = 0;
		ResetEvent( m_hEventGetResult );       
		//m_Diag.Send( PPP_PACKET_R, 0, pASCIIData,m_nAudioDataSize*2+1 );	  
		SendATCmd( (char*)pASCIIData );
        
		delete [] pASCIIData;
	}
	*/
    
    //synchronize with thread which check the AT comamnd result   
    dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log(_T("Failed to Execute command! "),TRUE);
            Log( _T("Command response is ") + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log(_T("Execute command sucessfully! ") );
        }        
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log(_T("Execute Command  Time Out! "),TRUE);
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log(_T("Failed to Execute command! "),TRUE);   
        return FALSE;
    }

    return TRUE;
}

BOOL CCommand::SetAudMode(CString strModeName)
{
	if( !m_bConnected )
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }
	
    CWaitCursor wc;
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
	
    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;

	strATCommand.Format( _T("AT+SADM=%d,\"%s\"\r\n"),SET_AUDIO_MODE_NAME, strModeName );
    
    ResetEvent( m_hEventGetResult );  

	//Log(_T("Set audio mode"),FALSE );

#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif     
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    //restore the return string into default AT command successful string
    m_strSucessFlag = "OK";
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }
	
    return TRUE;
}

BOOL CCommand::GetDspModeCount( int& nModeCount )
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }
	
    CWaitCursor wc;
	
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
	
    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
    
    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+SADMDSP=%d\r\n"),GET_AUDIO_DSP_MODE_COUNT );    
    ResetEvent( m_hEventGetResult );
	Log(_T("Read audio_arm mode count from phone:"),FALSE );
	
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif         
	
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
			Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",FALSE ); 
			Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
#ifdef _UNICODE
		swscanf( m_strReturn,_T("+SADMDSP:%d"),&nModeCount );
#else
		sscanf( (LPSTR)(LPCTSTR)m_strReturn,"+SADMDSP:%d",&nModeCount );
#endif
		if(nModeCount == 0)
		{
			Log(_T("Error! Dsp codec mode count is 0!  (Return:  " + m_strReturn+ ")"),TRUE );  
			return FALSE;
		}
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",FALSE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",FALSE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

BOOL CCommand::GetDspModeName( int nModeIndex,  CString& strModeName )
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }
	
    CWaitCursor wc;
	
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
	
    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
    
    m_strSucessFlag = "OK";          
	
    strATCommand.Format(_T("AT+SADMDSP=%d,%d\r\n"),GET_AUDIO_DSP_MODE_NAME, nModeIndex );    
    ResetEvent( m_hEventGetResult );   
	Log(_T("Read audio_arm mode name from phone:"),FALSE );
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif     
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
        
        // The return string may be '+SADM:0,"ModeName"'
        int nCommaPos = m_strReturn.Find( '"' );
        if( nCommaPos == -1 )
        {
            return FALSE;
        }
        else
        {
            strModeName = m_strReturn.Right( m_strReturn.GetLength() - nCommaPos - 1 );
            strModeName.Remove( '"' );
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

BOOL CCommand::GetDspData( int nIndex, int nType, AUDIO_NV_DSP_MODE_STRUCT_T& dsp_data )
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }
	
    CWaitCursor wc;
	
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
	
    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
	
	memset( &dsp_data,0,sizeof( AUDIO_NV_DSP_MODE_STRUCT_T ) );
	
    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+SADMDSP=%d,%d\r\n"),nType, nIndex );    
    ResetEvent( m_hEventGetResult );
	if(nType == GET_AUDIO_DSP_DATA_FROM_RAM)
	{
		Log(_T("Read dsp codec structrue from ram:"),FALSE );
	}
	else
	{
		Log(_T("Read dsp codec mode count from flash:"),FALSE );
	}
	
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif     
    
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
        
        // The return string may be '+SADMDSP:0,"ModeName",modedata'
        int nCommaPos = m_strReturn.ReverseFind( ',' );
        if( nCommaPos == -1 )
        {
            return FALSE;
        }
        else
        {
            CString strData;
            strData = m_strReturn.Right( m_strReturn.GetLength() - nCommaPos - 1 );
            // Two bytes represent one byte binary data
            m_nDspModeSize = strData.GetLength() / 2;
			if(m_nDspModeSize != sizeof(AUDIO_NV_DSP_MODE_STRUCT_T))
			{
				Log(_T("Dsp codec info size error!"),FALSE ); 
				return FALSE;
			}
#ifdef _UNICODE
			char szTemp[4096] = {0};
			WideCharToMultiByte(CP_ACP,0,strData.GetBuffer(strData.GetLength()),-1,szTemp,strData.GetLength(),NULL,NULL);
			ASCII2Bin( (BYTE*)szTemp,(BYTE*)&dsp_data,strlen(szTemp) );
#else
			ASCII2Bin( (BYTE*)strData.GetBuffer(0),(BYTE*)&dsp_data,strData.GetLength() );
#endif
			//teana hu 2010.05.06
			if( PP_BIG_ENDIAN == m_nEndian )
			{
				EndianConvertion( (WORD*)&dsp_data,m_nDspModeSize / 2 );
			}
			//
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

BOOL CCommand::SetDspData( int nIndex, int nType, CString strModeName, AUDIO_NV_DSP_MODE_STRUCT_T& dsp_data)
{
	if( !m_bConnected )
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }

    CWaitCursor wc;
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}

    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;

    AUDIO_NV_DSP_MODE_STRUCT_T temp_data = dsp_data;
    
    m_strSucessFlag = ">";          
    
    strATCommand.Format(_T("AT+SADMDSP=%d,%d\r\n"),nType, nIndex );    
    ResetEvent( m_hEventGetResult ); 
	if(nType == SET_LVVE_DATA_TO_RAM)
	{
		Log(_T("Write dsp codec structure to ram:"),FALSE );
	}
	else
	{
		Log(_T("Write dsp codec structure to flash:"),FALSE );
	}
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif      
    
    //synchronize with thread which check the AT command result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    //restore the return string into default AT command successful string
    m_strSucessFlag = "OK";
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",TRUE );  
            Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",TRUE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",TRUE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 

	// 0X3456 --->0X5634
	//teana hu 2010.05.06
	if( PP_BIG_ENDIAN == m_nEndian )
	{
		EndianConvertion( (WORD*)&temp_data,sizeof( AUDIO_NV_DSP_MODE_STRUCT_T ) / 2 );
	}
	//
	
	BYTE* pASCIIData = new BYTE[sizeof(AUDIO_NV_DSP_MODE_STRUCT_T)*2+2 ];
	//0X5634--->'5''6''3''4'
	Bin2ASCII((BYTE*)&temp_data,pASCIIData,sizeof(AUDIO_NV_DSP_MODE_STRUCT_T));
	
	//append ctrl+z
	pASCIIData[m_nDspModeSize*2] = 0X1A;
	pASCIIData[m_nDspModeSize*2+1] = 0;
	ResetEvent( m_hEventGetResult );       
	//m_Diag.Send( PPP_PACKET_R, 0, pASCIIData,m_nAudioDataSize*2+1 );	  
	SendATCmd( (char*)pASCIIData );
	
	delete [] pASCIIData;
    
    //synchronize with thread which check the AT comamnd result   
    dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
            Log(_T("Failed to Execute command! "),TRUE);
            Log( _T("Command response is ") + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log(_T("Execute command sucessfully! ") );
        }        
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log(_T("Execute Command  Time Out! "),TRUE);
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log(_T("Failed to Execute command! "),TRUE);   
        return FALSE;
    }

    return TRUE;
}

BOOL CCommand::GetDspCodecInfo(short& nDspCpdecInfo)
{
	if( !m_bConnected )//lint !e527
    {
        // not connected with channelserver
        Log( _T("Connect ChannelServer first.") );
        return FALSE;
    }
	
    CWaitCursor wc;
	
	if( !QueryEndian() )
	{
		// Do not know endian mode
		return FALSE;
	}
	
    CString strATCommand;
    const int nWaitForDiagDataSecond = 6000;
    
    m_strSucessFlag = "OK";          
    
    strATCommand.Format(_T("AT+SADMDSP=%d\r\n"),GET_AUDIO_DSP_CONTROL_MODE );    
    ResetEvent( m_hEventGetResult );
	Log(_T("Read audio_arm mode count from phone:"),FALSE );
	
#ifdef _UNICODE
	char szATCmd[4096] = {0};
	WideCharToMultiByte(CP_ACP,0,strATCommand.GetBuffer(strATCommand.GetLength()),-1,szATCmd,strATCommand.GetLength(),NULL,NULL);
	//m_Diag.Send( PPP_PACKET_R, 0, szATCmd, strlen(szATCmd)); 
	SendATCmd( szATCmd );
#else   
    //m_Diag.Send( PPP_PACKET_R, 0, strATCommand.GetBuffer(strATCommand.GetLength()),strATCommand.GetLength() );		
	SendATCmd( strATCommand.GetBuffer(strATCommand.GetLength()) );
#endif         
	
    //synchronize with thread which check the AT commamnd result
    DWORD dwResult = WaitForSingleObject( m_hEventGetResult , nWaitForDiagDataSecond );
    
    if ( WAIT_OBJECT_0 == dwResult )
    {        
        if( m_bCmdExecuteFail )
        {
			Log("Failed to Execute command!  (CMD:  " + strATCommand+ ")",FALSE ); 
			Log( "Command response is " + m_strReturn ,TRUE );
            return FALSE;
        }
        else
        {
            Log("Execute command successfully!  (CMD:  " + strATCommand+ ")",FALSE );  
        }
#ifdef _UNICODE
		swscanf( m_strReturn,_T("+SADMDSP:%d"),&nDspCpdecInfo );
#else
		sscanf( (LPSTR)(LPCTSTR)m_strReturn,"+SADMDSP:%d",&nDspCpdecInfo );
#endif
		if((nDspCpdecInfo & 0X100) >> 8 == 0)
		{
			m_bDspCodec = TRUE;
		}
		else
		{
			m_bDspCodec = FALSE;
		}
		
    }
    else if( WAIT_TIMEOUT == dwResult  )
    {
        Log( "Execute Command  Time Out! (CMD: " + strATCommand+  ")",FALSE );
        return FALSE;
    }
    else if( WAIT_FAILED  == dwResult )
    {
        Log( "Failed to Execute command!  (CMD: " + strATCommand+  ")",FALSE );   
        return FALSE;
    }  
    
    strATCommand.ReleaseBuffer( strATCommand.GetLength() ); 
    
    return TRUE;
}

void CCommand::CopyVolumeStructFrom8ToMax(VOLUME_STRUCT &volume_data_16, VOLUME_STRUCT_MIN &volume_data_8)
{
	memcpy(&volume_data_16.dev_path_set, &volume_data_8.dev_path_set, sizeof(AUDIO_NV_ARM_DEVICE_PATH_T));
	volume_data_16.app_config_info_set.valid_app_set_count = volume_data_8.app_config_info_set.valid_app_set_count;
	volume_data_16.app_config_info_set.valid_agc_input_gain_count = volume_data_8.app_config_info_set.valid_agc_input_gain_count;
	memcpy(volume_data_16.app_config_info_set.aud_proc_exp_control, volume_data_8.app_config_info_set.aud_proc_exp_control, sizeof(short)*2);
	for(int i = 0; i < AUDIO_ARM_APP_TYPE_MIN; i++)
	{
		//memcpy(&volume_data_16.app_config_info_set.app_config_info[i], &volume_data_8.app_config_info_set.app_config_info[i], sizeof(AUDIO_NV_ARM_APP_CONFIG_INFO_T));
		volume_data_16.app_config_info_set.app_config_info[i].eq_switch = volume_data_8.app_config_info_set.app_config_info[i].eq_switch;
		volume_data_16.app_config_info_set.app_config_info[i].valid_volume_level_count = volume_data_8.app_config_info_set.app_config_info[i].valid_volume_level_count;
		memcpy(volume_data_16.app_config_info_set.app_config_info[i].agc_input_gain, volume_data_8.app_config_info_set.app_config_info[i].agc_input_gain, sizeof(short)*AUDIO_AGC_INPUG_GAIN_MAX);
		memcpy(volume_data_16.app_config_info_set.app_config_info[i].arm_volume, volume_data_8.app_config_info_set.app_config_info[i].arm_volume, sizeof(short)*AUDIO_ARM_VOLUME_LEVEL);
		memcpy(volume_data_16.app_config_info_set.app_config_info[i].reserve, volume_data_8.app_config_info_set.app_config_info[i].reserve, sizeof(short)*AUDIO_NV_ARM_APP_PARA_RESERVE);
	}
	volume_data_16.midi_opt = volume_data_8.midi_opt;
	volume_data_16.aud_dev = volume_data_8.aud_dev;
	memcpy(volume_data_16.reserve, volume_data_8.reserve, sizeof(short)*AUDIO_NV_ARM_PARA_RESERVE);
}

void CCommand::CopyVolumeStructFrom1ToMax(VOLUME_STRUCT &volume_data_16, VOLUME_STRUCT_ONE &volume_data_1)
{
	memcpy(&volume_data_16.dev_path_set, &volume_data_1.dev_path_set, sizeof(AUDIO_NV_ARM_DEVICE_PATH_T));
	volume_data_16.app_config_info_set.valid_app_set_count = volume_data_1.app_config_info_set.valid_app_set_count;
	volume_data_16.app_config_info_set.valid_agc_input_gain_count = volume_data_1.app_config_info_set.valid_agc_input_gain_count;
	memcpy(volume_data_16.app_config_info_set.aud_proc_exp_control, volume_data_1.app_config_info_set.aud_proc_exp_control, sizeof(short)*2);
	//memcpy(&volume_data_16.app_config_info_set.app_config_info, &volume_data_1.app_config_info_set.app_config_info, sizeof(AUDIO_NV_ARM_APP_CONFIG_INFO_T));
	volume_data_16.app_config_info_set.app_config_info[0].eq_switch = volume_data_1.app_config_info_set.app_config_info[0].eq_switch;
	volume_data_16.app_config_info_set.app_config_info[0].valid_volume_level_count = volume_data_1.app_config_info_set.app_config_info[0].valid_volume_level_count;
	memcpy(volume_data_16.app_config_info_set.app_config_info[0].agc_input_gain, volume_data_1.app_config_info_set.app_config_info[0].agc_input_gain, sizeof(short)*AUDIO_AGC_INPUG_GAIN_MAX);
	memcpy(volume_data_16.app_config_info_set.app_config_info[0].arm_volume, volume_data_1.app_config_info_set.app_config_info[0].arm_volume, sizeof(short)*AUDIO_ARM_VOLUME_LEVEL);
	memcpy(volume_data_16.app_config_info_set.app_config_info[0].reserve, volume_data_1.app_config_info_set.app_config_info[0].reserve, sizeof(short)*AUDIO_NV_ARM_APP_PARA_RESERVE);
	volume_data_16.midi_opt = volume_data_1.midi_opt;
	volume_data_16.aud_dev = volume_data_1.aud_dev;
	memcpy(volume_data_16.reserve, volume_data_1.reserve, sizeof(short)*AUDIO_NV_ARM_PARA_RESERVE);
}

void CCommand::CopyVolumeStructFromMaxTo8(VOLUME_STRUCT_MIN &volume_data_8, VOLUME_STRUCT &volume_data_16)
{
	memcpy(&volume_data_8.dev_path_set, &volume_data_16.dev_path_set, sizeof(AUDIO_NV_ARM_DEVICE_PATH_T));
	volume_data_8.app_config_info_set.valid_app_set_count = volume_data_16.app_config_info_set.valid_app_set_count;
	volume_data_8.app_config_info_set.valid_agc_input_gain_count = volume_data_16.app_config_info_set.valid_agc_input_gain_count;
	memcpy(volume_data_8.app_config_info_set.aud_proc_exp_control, volume_data_16.app_config_info_set.aud_proc_exp_control, sizeof(short)*2);
	for(int i = 0; i < AUDIO_ARM_APP_TYPE_MIN; i++)
	{
		//memcpy(&volume_data_8.app_config_info_set.app_config_info[i], &volume_data_16.app_config_info_set.app_config_info[i], sizeof(AUDIO_NV_ARM_APP_CONFIG_INFO_T));
		volume_data_8.app_config_info_set.app_config_info[i].eq_switch = volume_data_16.app_config_info_set.app_config_info[i].eq_switch;
		volume_data_8.app_config_info_set.app_config_info[i].valid_volume_level_count = volume_data_16.app_config_info_set.app_config_info[i].valid_volume_level_count;
		memcpy(volume_data_8.app_config_info_set.app_config_info[i].agc_input_gain, volume_data_16.app_config_info_set.app_config_info[i].agc_input_gain, sizeof(short)*AUDIO_AGC_INPUG_GAIN_MAX);
		if(m_bDspCodec)
		{
			memcpy(volume_data_8.app_config_info_set.app_config_info[i].arm_volume, volume_data_16.app_config_info_set.app_config_info[i].arm_volume_l, sizeof(short)*AUDIO_ARM_VOLUME_LEVEL);
		}
		else
		{
			memcpy(volume_data_8.app_config_info_set.app_config_info[i].arm_volume, volume_data_16.app_config_info_set.app_config_info[i].arm_volume, sizeof(short)*AUDIO_ARM_VOLUME_LEVEL);
		}
		
		memcpy(volume_data_8.app_config_info_set.app_config_info[i].reserve, volume_data_16.app_config_info_set.app_config_info[i].reserve, sizeof(short)*AUDIO_NV_ARM_APP_PARA_RESERVE);
	}
	volume_data_8.midi_opt = volume_data_16.midi_opt;
	volume_data_8.aud_dev = volume_data_16.aud_dev;
	memcpy(volume_data_8.reserve, volume_data_16.reserve, sizeof(short)*AUDIO_NV_ARM_PARA_RESERVE);
}

void CCommand::CopyVolumeStructFromMaxTo1(VOLUME_STRUCT_ONE &volume_data_1, VOLUME_STRUCT &volume_data_16)
{
	memcpy(&volume_data_1.dev_path_set, &volume_data_16.dev_path_set, sizeof(AUDIO_NV_ARM_DEVICE_PATH_T));
	volume_data_1.app_config_info_set.valid_app_set_count = volume_data_16.app_config_info_set.valid_app_set_count;
	volume_data_1.app_config_info_set.valid_agc_input_gain_count = volume_data_16.app_config_info_set.valid_agc_input_gain_count;
	memcpy(volume_data_1.app_config_info_set.aud_proc_exp_control, volume_data_16.app_config_info_set.aud_proc_exp_control, sizeof(short)*2);
	//memcpy(&volume_data_1.app_config_info_set.app_config_info, &volume_data_16.app_config_info_set.app_config_info, sizeof(AUDIO_NV_ARM_APP_CONFIG_INFO_T));
	volume_data_1.app_config_info_set.app_config_info[0].eq_switch = volume_data_16.app_config_info_set.app_config_info[0].eq_switch;
	volume_data_1.app_config_info_set.app_config_info[0].valid_volume_level_count = volume_data_16.app_config_info_set.app_config_info[0].valid_volume_level_count;
	memcpy(volume_data_1.app_config_info_set.app_config_info[0].agc_input_gain, volume_data_16.app_config_info_set.app_config_info[0].agc_input_gain, sizeof(short)*AUDIO_AGC_INPUG_GAIN_MAX);
	if(m_bDspCodec)
	{
		memcpy(volume_data_1.app_config_info_set.app_config_info[0].arm_volume, volume_data_16.app_config_info_set.app_config_info[0].arm_volume_l, sizeof(short)*AUDIO_ARM_VOLUME_LEVEL);
	}
	else
	{
		memcpy(volume_data_1.app_config_info_set.app_config_info[0].arm_volume, volume_data_16.app_config_info_set.app_config_info[0].arm_volume, sizeof(short)*AUDIO_ARM_VOLUME_LEVEL);
	}
	
	memcpy(volume_data_1.app_config_info_set.app_config_info[0].reserve, volume_data_16.app_config_info_set.app_config_info[0].reserve, sizeof(short)*AUDIO_NV_ARM_APP_PARA_RESERVE);
	volume_data_1.midi_opt = volume_data_16.midi_opt;
	volume_data_1.aud_dev = volume_data_16.aud_dev;
	memcpy(volume_data_1.reserve, volume_data_16.reserve, sizeof(short)*AUDIO_NV_ARM_PARA_RESERVE);
}
/*
void CCommand::CopyLVVEFromMinToMax(LVVE_CTRL_PARAM_NV_T &lvve_data, LVVE_CTRL_PARAM_NV_MIN_T &lvve_data_min)
{
	memcpy(lvve_data.ucModeName, lvve_data_min.ucModeName, LVVE_MAX_MODE_NAME_LEN);
	memcpy(&lvve_data.lvve_ctrl_param_t, &lvve_data_min.lvve_ctrl_param_t, sizeof(LVVE_CTRL_PARAM_T));
	memset(lvve_data.lvve_reserve, 0, sizeof(short) * LVVE_CONFIG_RESERVE_LEN);
}

void CCommand::CopyLVVEFromMaxToMin(LVVE_CTRL_PARAM_NV_MIN_T &lvve_data_min, LVVE_CTRL_PARAM_NV_T &lvve_data)
{
	memcpy(lvve_data_min.ucModeName, lvve_data.ucModeName, LVVE_MAX_MODE_NAME_LEN);
	memcpy(&lvve_data_min.lvve_ctrl_param_t, &lvve_data.lvve_ctrl_param_t, sizeof(LVVE_CTRL_PARAM_T));
}

BOOL CCommand::LVVEIsMaxOrMin()
{
	return m_bLVVEMin;
}
*/

//teana hu 2012.01.06
void CCommand::CopyLVVEFromNoReserveToMax(LVVE_CTRL_PARAM_NV_T &lvve_data, LVVE_CTRL_PARAM_NV_MIN_T &lvve_data_min)
{
	memcpy(lvve_data.ucModeName, lvve_data_min.ucModeName, LVVE_MAX_MODE_NAME_LEN);
	
	memcpy(lvve_data.lvve_ctrl_param_t.RxEqCoefs, lvve_data_min.lvve_ctrl_param_t.RxEqCoefs, sizeof(short)*LVVE_EQ_COEF_SIZE);
	memcpy(lvve_data.lvve_ctrl_param_t.TxEqCoefs, lvve_data_min.lvve_ctrl_param_t.TxEqCoefs, sizeof(short)*LVVE_EQ_COEF_SIZE);
	memset(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx, 0, sizeof(LVVE_Rx_ControlParams_st));

	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.Mute = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.Mute;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams, sizeof(LVFENS_ControlParams_st));
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams, sizeof(LVNLPP_ControlParams_st));
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams, sizeof(LVVC_ControlParams_st));
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams, sizeof(LVEQ_ControlParams_st));

	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.Mute = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.Mute;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams, sizeof(LVHF_ControlParams_st));
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams, sizeof(LVWM_ControlParams_st));
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams, sizeof(LVEQ_ControlParams_st));

	memset(lvve_data.lvve_reserve, 0, sizeof(short) * LVVE_CONFIG_RESERVE_LEN);
}

void CCommand::CopyLVVEFromMaxToNoReserve(LVVE_CTRL_PARAM_NV_MIN_T &lvve_data_min, LVVE_CTRL_PARAM_NV_T &lvve_data)
{
	memcpy(lvve_data_min.ucModeName, lvve_data.ucModeName, LVVE_MAX_MODE_NAME_LEN);
	
	memcpy(lvve_data_min.lvve_ctrl_param_t.RxEqCoefs, lvve_data.lvve_ctrl_param_t.RxEqCoefs, sizeof(short)*LVVE_EQ_COEF_SIZE);
	memcpy(lvve_data_min.lvve_ctrl_param_t.TxEqCoefs, lvve_data.lvve_ctrl_param_t.TxEqCoefs, sizeof(short)*LVVE_EQ_COEF_SIZE);
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.Mute = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.Mute;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams, sizeof(LVFENS_ControlParams_st));
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams, sizeof(LVNLPP_ControlParams_st));
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams, sizeof(LVVC_ControlParams_st));
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams, sizeof(LVEQ_ControlParams_st));

	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.Mute = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.Mute;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams, sizeof(LVHF_ControlParams_st));
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams, sizeof(LVWM_ControlParams_st));
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams, sizeof(LVEQ_ControlParams_st));

}

void CCommand::CopyLVVEFromReserveToMax(LVVE_CTRL_PARAM_NV_T &lvve_data, LVVE_CTRL_PARAM_NV_1_T &lvve_data_min)
{
	memcpy(lvve_data.ucModeName, lvve_data_min.ucModeName, LVVE_MAX_MODE_NAME_LEN);
	
	memcpy(lvve_data.lvve_ctrl_param_t.RxEqCoefs, lvve_data_min.lvve_ctrl_param_t.RxEqCoefs, sizeof(short)*LVVE_EQ_COEF_SIZE);
	memcpy(lvve_data.lvve_ctrl_param_t.TxEqCoefs, lvve_data_min.lvve_ctrl_param_t.TxEqCoefs, sizeof(short)*LVVE_EQ_COEF_SIZE);
	memset(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx, 0, sizeof(LVVE_Rx_ControlParams_st));

	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.Mute = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.Mute;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams, sizeof(LVFENS_ControlParams_st));
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams, sizeof(LVNLPP_ControlParams_st));
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams, sizeof(LVVC_ControlParams_st));
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams, sizeof(LVEQ_ControlParams_st));
	
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.Mute = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.Mute;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams, sizeof(LVHF_ControlParams_st));
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams, sizeof(LVWM_ControlParams_st));
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams, sizeof(LVEQ_ControlParams_st));

	memcpy(lvve_data.lvve_reserve, lvve_data_min.lvve_reserve, sizeof(short) * LVVE_CONFIG_RESERVE_LEN);
}

void CCommand::CopyLVVEFromMaxToReserve(LVVE_CTRL_PARAM_NV_1_T &lvve_data_min, LVVE_CTRL_PARAM_NV_T &lvve_data)
{
	memcpy(lvve_data_min.ucModeName, lvve_data.ucModeName, LVVE_MAX_MODE_NAME_LEN);
	
	memcpy(lvve_data_min.lvve_ctrl_param_t.RxEqCoefs, lvve_data.lvve_ctrl_param_t.RxEqCoefs, sizeof(short)*LVVE_EQ_COEF_SIZE);
	memcpy(lvve_data_min.lvve_ctrl_param_t.TxEqCoefs, lvve_data.lvve_ctrl_param_t.TxEqCoefs, sizeof(short)*LVVE_EQ_COEF_SIZE);
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.Mute = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.Mute;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams, sizeof(LVFENS_ControlParams_st));
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams, sizeof(LVNLPP_ControlParams_st));
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams, sizeof(LVVC_ControlParams_st));
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams, sizeof(LVEQ_ControlParams_st));

	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.Mute = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.Mute;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams, sizeof(LVHF_ControlParams_st));
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams, sizeof(LVWM_ControlParams_st));
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams, sizeof(LVEQ_ControlParams_st));

	memcpy(lvve_data_min.lvve_reserve, lvve_data.lvve_reserve, sizeof(short) * LVVE_CONFIG_RESERVE_LEN);
}

void CCommand::CopyLVVEFromJuniperToMax(LVVE_CTRL_PARAM_NV_T &lvve_data, LVVE_CTRL_PARAM_NV_2_T &lvve_data_min)
{
	memcpy(lvve_data.ucModeName, lvve_data_min.ucModeName, LVVE_MAX_MODE_NAME_LEN);
	
	memcpy(lvve_data.lvve_ctrl_param_t.RxEqCoefs, lvve_data_min.lvve_ctrl_param_t.RxEqCoefs, sizeof(short)*LVVE_EQ_COEF_SIZE);
	memcpy(lvve_data.lvve_ctrl_param_t.TxEqCoefs, lvve_data_min.lvve_ctrl_param_t.TxEqCoefs, sizeof(short)*LVVE_EQ_COEF_SIZE);
	memset(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx, 0, sizeof(LVVE_Rx_ControlParams_st));

	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.Mute = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.Mute;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams, sizeof(LVNLPP_ControlParams_st));
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams, sizeof(LVEQ_ControlParams_st));
	
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.Mute = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.Mute;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams, sizeof(LVHF_ControlParams_st));
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams, sizeof(LVEQ_ControlParams_st));
	
	memcpy(lvve_data.lvve_reserve, lvve_data_min.lvve_reserve, sizeof(short) * LVVE_CONFIG_RESERVE_LEN);

}

void CCommand::CopyLVVEFromMaxToJuniper(LVVE_CTRL_PARAM_NV_2_T &lvve_data_min, LVVE_CTRL_PARAM_NV_T &lvve_data)
{
	memcpy(lvve_data_min.ucModeName, lvve_data.ucModeName, LVVE_MAX_MODE_NAME_LEN);
	
	memcpy(lvve_data_min.lvve_ctrl_param_t.RxEqCoefs, lvve_data.lvve_ctrl_param_t.RxEqCoefs, sizeof(short)*LVVE_EQ_COEF_SIZE);
	memcpy(lvve_data_min.lvve_ctrl_param_t.TxEqCoefs, lvve_data.lvve_ctrl_param_t.TxEqCoefs, sizeof(short)*LVVE_EQ_COEF_SIZE);
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.Mute = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.Mute;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams, sizeof(LVNLPP_ControlParams_st));
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams, sizeof(LVEQ_ControlParams_st));
	
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.Mute = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.Mute;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams, sizeof(LVHF_ControlParams_st));
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams, sizeof(LVEQ_ControlParams_st));
	
	memcpy(lvve_data_min.lvve_reserve, lvve_data.lvve_reserve, sizeof(short) * LVVE_CONFIG_RESERVE_LEN);
}

void CCommand::CopyLVVEFromSingleMicToMax(LVVE_CTRL_PARAM_NV_T &lvve_data, LVVE_CTRL_PARAM_NV_3_T &lvve_data_min)
{
	memcpy(lvve_data.ucModeName, lvve_data_min.ucModeName, LVVE_MAX_MODE_NAME_LEN);
	
	memcpy(lvve_data.lvve_ctrl_param_t.RxEqCoefs, lvve_data_min.lvve_ctrl_param_t.RxEqCoefs, sizeof(short)*LVVE_EQ_COEF_SIZE);
	memcpy(lvve_data.lvve_ctrl_param_t.TxEqCoefs, lvve_data_min.lvve_ctrl_param_t.TxEqCoefs, sizeof(short)*LVVE_EQ_COEF_SIZE);
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.Mute = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.Mute;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams, sizeof(LVFENS_ControlParams_st));
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams, sizeof(LVNLPP_ControlParams_st));
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams, sizeof(LVVC_ControlParams_st));
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams, sizeof(LVEQ_ControlParams_st));
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.HPF_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.HPF_OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.HPF_CornerFreq = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.HPF_CornerFreq;
	lvve_data.lvve_ctrl_param_t.ControlParams_Rx.CNG_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.CNG_OperatingMode;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx.CNG_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.CNG_ControlParams, sizeof(CNGControl_Parameters_st));
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams, sizeof(LVWM_ControlParams_st));
	
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.Mute = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.Mute;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode;
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams, sizeof(LVHF_ControlParams_st));
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams, sizeof(LVWM_ControlParams_st));
	lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode = lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode;
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams, sizeof(LVEQ_ControlParams_st));
	memcpy(&lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams, &lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams, sizeof(LVDRC_ControlParams_st));
	
	memcpy(lvve_data.lvve_reserve, lvve_data_min.lvve_reserve, sizeof(short) * LVVE_CONFIG_RESERVE_LEN);

}

void CCommand::CopyLVVEFromMaxToSingleMic(LVVE_CTRL_PARAM_NV_3_T &lvve_data_min, LVVE_CTRL_PARAM_NV_T &lvve_data)
{
	memcpy(lvve_data_min.ucModeName, lvve_data.ucModeName, LVVE_MAX_MODE_NAME_LEN);
	
	memcpy(lvve_data_min.lvve_ctrl_param_t.RxEqCoefs, lvve_data.lvve_ctrl_param_t.RxEqCoefs, sizeof(short)*LVVE_EQ_COEF_SIZE);
	memcpy(lvve_data_min.lvve_ctrl_param_t.TxEqCoefs, lvve_data.lvve_ctrl_param_t.TxEqCoefs, sizeof(short)*LVVE_EQ_COEF_SIZE);
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.Mute = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.Mute;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams, sizeof(LVFENS_ControlParams_st));
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams, sizeof(LVNLPP_ControlParams_st));
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams, sizeof(LVVC_ControlParams_st));
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams, sizeof(LVEQ_ControlParams_st));
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.HPF_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.HPF_OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.HPF_CornerFreq = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.HPF_CornerFreq;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.CNG_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.CNG_OperatingMode;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.CNG_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Rx.CNG_ControlParams, sizeof(CNGControl_Parameters_st));
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams, sizeof(LVWM_ControlParams_st));
	
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.Mute = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.Mute;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode;
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams, sizeof(LVHF_ControlParams_st));
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams, sizeof(LVWM_ControlParams_st));
	lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode;
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams, sizeof(LVEQ_ControlParams_st));
	memcpy(&lvve_data_min.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams, &lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams, sizeof(LVDRC_ControlParams_st));
	
	memcpy(lvve_data_min.lvve_reserve, lvve_data.lvve_reserve, sizeof(short) * LVVE_CONFIG_RESERVE_LEN);
}

DWORD CCommand::GetLVVEType()
{
	return m_dwLVVEType;
}

void CCommand::CopyVolumeStructFrom6ToMax(VOLUME_STRUCT &volume_data_16, VOLUME_STRUCT_SIX &volume_data_6)
{
	memcpy(&volume_data_16.dev_path_set, &volume_data_6.dev_path_set, sizeof(AUDIO_NV_ARM_DEVICE_PATH_T));
	volume_data_16.app_config_info_set.valid_app_set_count = volume_data_6.app_config_info_set.valid_app_set_count;
	volume_data_16.app_config_info_set.valid_agc_input_gain_count = volume_data_6.app_config_info_set.valid_agc_input_gain_count;
	memcpy(volume_data_16.app_config_info_set.aud_proc_exp_control, volume_data_6.app_config_info_set.aud_proc_exp_control, sizeof(short)*2);
	for(int i = 0; i < AUDIO_ARM_APP_TYPE_SIX; i++)
	{
		volume_data_16.app_config_info_set.app_config_info[i].eq_switch = volume_data_6.app_config_info_set.app_config_info[i].eq_switch;
		volume_data_16.app_config_info_set.app_config_info[i].valid_volume_level_count = volume_data_6.app_config_info_set.app_config_info[i].valid_volume_level_count;
		memcpy(volume_data_16.app_config_info_set.app_config_info[i].agc_input_gain, volume_data_6.app_config_info_set.app_config_info[i].agc_input_gain, sizeof(short)*AUDIO_AGC_INPUG_GAIN_MAX);
		memcpy(volume_data_16.app_config_info_set.app_config_info[i].arm_volume_l, volume_data_6.app_config_info_set.app_config_info[i].arm_volume, sizeof(LONG)*AUDIO_ARM_VOLUME_LEVEL);
		memcpy(volume_data_16.app_config_info_set.app_config_info[i].reserve, volume_data_6.app_config_info_set.app_config_info[i].reserve, sizeof(short)*AUDIO_NV_ARM_APP_PARA_RESERVE);
	}
	volume_data_16.midi_opt = volume_data_6.midi_opt;
	volume_data_16.aud_dev = volume_data_6.aud_dev;
	memcpy(volume_data_16.reserve, volume_data_6.reserve, sizeof(short)*AUDIO_NV_ARM_PARA_RESERVE);
}

void CCommand::CopyVolumeStructFromMaxTo6(VOLUME_STRUCT_SIX &volume_data_6, VOLUME_STRUCT &volume_data_16)
{
	memcpy(&volume_data_6.dev_path_set, &volume_data_16.dev_path_set, sizeof(AUDIO_NV_ARM_DEVICE_PATH_T));
	volume_data_6.app_config_info_set.valid_app_set_count = volume_data_16.app_config_info_set.valid_app_set_count;
	volume_data_6.app_config_info_set.valid_agc_input_gain_count = volume_data_16.app_config_info_set.valid_agc_input_gain_count;
	memcpy(volume_data_6.app_config_info_set.aud_proc_exp_control, volume_data_16.app_config_info_set.aud_proc_exp_control, sizeof(short)*2);
	for(int i = 0; i < AUDIO_ARM_APP_TYPE_SIX; i++)
	{
		volume_data_6.app_config_info_set.app_config_info[i].eq_switch = volume_data_16.app_config_info_set.app_config_info[i].eq_switch;
		volume_data_6.app_config_info_set.app_config_info[i].valid_volume_level_count = volume_data_16.app_config_info_set.app_config_info[i].valid_volume_level_count;
		memcpy(volume_data_6.app_config_info_set.app_config_info[i].agc_input_gain, volume_data_16.app_config_info_set.app_config_info[i].agc_input_gain, sizeof(short)*AUDIO_AGC_INPUG_GAIN_MAX);
		if(m_bDspCodec)
		{
			memcpy(volume_data_6.app_config_info_set.app_config_info[i].arm_volume, volume_data_16.app_config_info_set.app_config_info[i].arm_volume_l, sizeof(LONG)*AUDIO_ARM_VOLUME_LEVEL);
		}
		else
		{
			memcpy(volume_data_6.app_config_info_set.app_config_info[i].arm_volume, volume_data_16.app_config_info_set.app_config_info[i].arm_volume, sizeof(LONG)*AUDIO_ARM_VOLUME_LEVEL);
		}
		
		memcpy(volume_data_6.app_config_info_set.app_config_info[i].reserve, volume_data_16.app_config_info_set.app_config_info[i].reserve, sizeof(short)*AUDIO_NV_ARM_APP_PARA_RESERVE);
	}
	volume_data_6.midi_opt = volume_data_16.midi_opt;
	volume_data_6.aud_dev = volume_data_16.aud_dev;
	memcpy(volume_data_6.reserve, volume_data_16.reserve, sizeof(short)*AUDIO_NV_ARM_PARA_RESERVE);
}

void CCommand::CopyVolumeStructFrom16ToMax(VOLUME_STRUCT &volume_data, VOLUME_STRUCT_SIXTEEN &volume_data_16)
{
	memcpy(&volume_data.dev_path_set, &volume_data_16.dev_path_set, sizeof(AUDIO_NV_ARM_DEVICE_PATH_T));
	volume_data.app_config_info_set.valid_app_set_count = volume_data_16.app_config_info_set.valid_app_set_count;
	volume_data.app_config_info_set.valid_agc_input_gain_count = volume_data_16.app_config_info_set.valid_agc_input_gain_count;
	memcpy(volume_data.app_config_info_set.aud_proc_exp_control, volume_data_16.app_config_info_set.aud_proc_exp_control, sizeof(short)*2);
	for(int i = 0; i < AUDIO_ARM_APP_TYPE_MAX; i++)
	{
		volume_data.app_config_info_set.app_config_info[i].eq_switch = volume_data_16.app_config_info_set.app_config_info[i].eq_switch;
		volume_data.app_config_info_set.app_config_info[i].valid_volume_level_count = volume_data_16.app_config_info_set.app_config_info[i].valid_volume_level_count;
		memcpy(volume_data.app_config_info_set.app_config_info[i].agc_input_gain, volume_data_16.app_config_info_set.app_config_info[i].agc_input_gain, sizeof(short)*AUDIO_AGC_INPUG_GAIN_MAX);
		memcpy(volume_data.app_config_info_set.app_config_info[i].arm_volume, volume_data_16.app_config_info_set.app_config_info[i].arm_volume, sizeof(short)*AUDIO_ARM_VOLUME_LEVEL);
		memcpy(volume_data.app_config_info_set.app_config_info[i].reserve, volume_data_16.app_config_info_set.app_config_info[i].reserve, sizeof(short)*AUDIO_NV_ARM_APP_PARA_RESERVE);
	}
	volume_data.midi_opt = volume_data_16.midi_opt;
	volume_data.aud_dev = volume_data_16.aud_dev;
	memcpy(volume_data.reserve, volume_data_16.reserve, sizeof(short)*AUDIO_NV_ARM_PARA_RESERVE);
}

void CCommand::CopyVolumeStructFromMaxTo16(VOLUME_STRUCT_SIXTEEN &volume_data_16, VOLUME_STRUCT &volume_data)
{
	memcpy(&volume_data_16.dev_path_set, &volume_data.dev_path_set, sizeof(AUDIO_NV_ARM_DEVICE_PATH_T));
	volume_data_16.app_config_info_set.valid_app_set_count = volume_data.app_config_info_set.valid_app_set_count;
	volume_data_16.app_config_info_set.valid_agc_input_gain_count = volume_data.app_config_info_set.valid_agc_input_gain_count;
	memcpy(volume_data_16.app_config_info_set.aud_proc_exp_control, volume_data.app_config_info_set.aud_proc_exp_control, sizeof(short)*2);
	for(int i = 0; i < AUDIO_ARM_APP_TYPE_MAX; i++)
	{
		volume_data_16.app_config_info_set.app_config_info[i].eq_switch = volume_data.app_config_info_set.app_config_info[i].eq_switch;
		volume_data_16.app_config_info_set.app_config_info[i].valid_volume_level_count = volume_data.app_config_info_set.app_config_info[i].valid_volume_level_count;
		memcpy(volume_data_16.app_config_info_set.app_config_info[i].agc_input_gain, volume_data.app_config_info_set.app_config_info[i].agc_input_gain, sizeof(short)*AUDIO_AGC_INPUG_GAIN_MAX);
		if(m_bDspCodec)
		{
			memcpy(volume_data_16.app_config_info_set.app_config_info[i].arm_volume, volume_data.app_config_info_set.app_config_info[i].arm_volume_l, sizeof(short)*AUDIO_ARM_VOLUME_LEVEL);
		}
		else
		{
			memcpy(volume_data_16.app_config_info_set.app_config_info[i].arm_volume, volume_data.app_config_info_set.app_config_info[i].arm_volume, sizeof(short)*AUDIO_ARM_VOLUME_LEVEL);
		}
		
		memcpy(volume_data_16.app_config_info_set.app_config_info[i].reserve, volume_data.app_config_info_set.app_config_info[i].reserve, sizeof(short)*AUDIO_NV_ARM_APP_PARA_RESERVE);
	}
	volume_data_16.midi_opt = volume_data.midi_opt;
	volume_data_16.aud_dev = volume_data.aud_dev;
	memcpy(volume_data_16.reserve, volume_data.reserve, sizeof(short)*AUDIO_NV_ARM_PARA_RESERVE);
}