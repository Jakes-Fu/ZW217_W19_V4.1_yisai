// AudioTesterSheet.cpp : implementation file
//

#include "stdafx.h"
#include "audiotester.h"
#include "AudioTesterSheet.h"

#include "AudioFileOpenDlg.h"
#include "nvvaluefile.h"

#include "AboutDlg.h"
#include <afxpriv.h>

#include "XAboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAudioTesterSheet

IMPLEMENT_DYNAMIC(CAudioTesterSheet, CPropertySheet)

CAudioTesterSheet::CAudioTesterSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    AddPage( &m_SettingsPage );
    m_AudioData.lpAudioMode = NULL;
    m_bLoad = FALSE;
    
    m_structFileHead.m_fFileVersion = 0;
	//teana hu 2010.06.10
	m_structFileHead.m_nEQDataSize = 0;
	//
    
    m_bExtendData = FALSE;
    m_bRocky = FALSE;
	m_bAudio3 = FALSE;

	//teana hu 2010.06.08
	m_EQData.eq_count = 0;
	m_EQData.lpEQMode = NULL;
	m_EQData.eq_count = 0;
	m_bLoadEQ = FALSE;
	//
	
	//teana hu 2011.02.16
	m_VolumeData.mode_count = 0;
	m_VolumeData.lpVolumeMode = NULL;
	m_bLoadVolume = FALSE;
	//

	//teana hu 2011.03.22
	m_eqData_g.lpEQMode = NULL;
	m_eqData_g.eq_count = 0;
	m_tunData_g.lpTunMode = NULL;
	m_tunData_g.tun_count = 0;
	m_bLoadENHA = FALSE;
	//
	
	//teana hu 2011.04.18
	m_LVVEPara.lp_lvve_ctrl_nv_params= NULL;
	m_bLoadLVVE = FALSE;
	//

	//teana hu 2011.05.09
	m_nArmSerialCount = AUDIO_ARM_APP_TYPE_MAX;
	//
	
	//teana hu 2011.06.07
	//m_bLVVEMin = FALSE;
	//
	//teana hu 2012.01.09
	m_dwLvveType = LVVE_RESERVE;
	//

	//teana hu 2012.01.31
	m_DspModePara.nModeCount = 0;
	m_DspModePara.lp_audio_nv_dsp_mode_info = NULL;
	m_bLoadDspCodec = FALSE;
	m_nVolumeLevel_6530 = 0;
	m_nDspCodecInfo = 0;
	m_bDspCodec = FALSE;
	//

	//teana hu 2012.05.02
	m_bExportOldNv = FALSE;
	//

	//teana hu 2012.06.14
	m_bExportOldEQNv = FALSE;
	m_bSerialPara = FALSE;
	//
}

CAudioTesterSheet::CAudioTesterSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CAudioTesterSheet::~CAudioTesterSheet()
{
    ClearAllModes();
}


BEGIN_MESSAGE_MAP(CAudioTesterSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CAudioTesterSheet)
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAudioTesterSheet message handlers
BOOL CAudioTesterSheet::LoadAudioModes(DWORD dwType)
{
    int nModeCount;
    m_bLoad = m_Cmd.GetModeCount( nModeCount );
    if( !m_bLoad )
    {
        // Can not read mode count from phone
        return FALSE;
    }

	//teana hu 2010.07.01
	int nType = 2;
	switch(dwType) {
	case MS_MODE:
		nType = 2;
		break;
	case FLASH_MODE:
		nType = 6;
		break;
	default:
		break;
	}
	//

	int nVolumeType = 2;
	switch(dwType) {
	case MS_MODE:
		nVolumeType = 9;
		break;
	case FLASH_MODE:
		nVolumeType = 12;
		break;
	default:
		break;
	}

	//teana hu 2011.04.18
	int nLVVEType = 2;
	switch(dwType) {
	case MS_MODE:
		nLVVEType = 2;
		break;
	case FLASH_MODE:
		nLVVEType = 4;
		break;
	default:
		break;
	}
	//
	
	//teana hu 2012.01.31
	int nDspType = 2;
	switch(dwType) {
	case MS_MODE:
		nDspType = 2;
		break;
	case FLASH_MODE:
		nDspType = 6;
		break;
	default:
		break;
	}
	//



    m_AudioData.mode_count = nModeCount;
    m_AudioData.lpAudioMode = new AUDIO_MODE[ nModeCount ];

	//teana hu 2011.02.16
	int nVolumeModeCount = 0;
	m_bLoadVolume = m_Cmd.GetVolumeModeCount( nVolumeModeCount );
	if(m_bLoadVolume)
	{
		int nSerailCount = 0;
		m_bLoadVolume = m_Cmd.GetVolumeSerailCount(nSerailCount);
		if(nSerailCount == 0)
		{
			m_bLoadVolume = FALSE;
		}
		if(m_bLoadVolume)
		{
			m_nArmSerialCount = nSerailCount;
			m_VolumeData.mode_count = nVolumeModeCount;
			m_VolumeData.lpVolumeMode = new VOLUME_MODE[ nVolumeModeCount ];
			for(int i = 0; i < nVolumeModeCount; i++)
			{
				CString strModeName;
				m_bLoadVolume = m_Cmd.GetVolumeModeName( i,strModeName );
				if( !m_bLoadVolume )
				{
					break;
				}
				
#ifdef _UNICODE
				WideCharToMultiByte(CP_ACP,0,strModeName.GetBuffer(strModeName.GetLength()),-1,(char *)m_VolumeData.lpVolumeMode[i].mode_name,strModeName.GetLength(),NULL,NULL);
				m_VolumeData.lpVolumeMode[i].mode_name[strModeName.GetLength()] = '\0';
#else
				strcpy( (char*)m_VolumeData.lpVolumeMode[i].mode_name,strModeName );
#endif  
				//teana hu 2011.02.16
				ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[i].volume_data,sizeof( VOLUME_STRUCT ) );
				//
				m_bLoadVolume = m_Cmd.GetVolumeData(i, nVolumeType, m_VolumeData.lpVolumeMode[i].volume_data);
				if( !m_bLoadVolume )
				{
					break;
				}
			}
		}
	}
	//


	//teana hu 2011.04.18
	int nLVVEModeCount = 0;
	m_bLoadLVVE = m_Cmd.GetLVVEModeCount( nLVVEModeCount );
	if(m_bLoadLVVE)
	{
		
		m_LVVEPara.mode_count = nLVVEModeCount;
		m_LVVEPara.lp_lvve_ctrl_nv_params = new LVVE_CTRL_PARAM_NV_T[ nLVVEModeCount ];
		for(int i = 0; i < nLVVEModeCount; i++)
		{
			CString strModeName;
			m_bLoadLVVE = m_Cmd.GetLVVEModeName( i,strModeName );
			if( !m_bLoadLVVE )
			{
				break;
			}
			
#ifdef _UNICODE
			WideCharToMultiByte(CP_ACP,0,strModeName.GetBuffer(strModeName.GetLength()),-1,(char *)m_LVVEPara.lp_lvve_ctrl_nv_params[i].ucModeName ,strModeName.GetLength(),NULL,NULL);
			m_LVVEPara.lp_lvve_ctrl_nv_params[i].ucModeName[strModeName.GetLength()] = '\0';
#else
			strcpy( (char*)m_LVVEPara.lp_lvve_ctrl_nv_params[i].ucModeName,strModeName );
#endif  
			ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[i].lvve_ctrl_param_t,sizeof( LVVE_CTRL_PARAM_T ) );
			ZeroMemory((LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[i].lvve_reserve, sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
			m_bLoadLVVE = m_Cmd.GetLVVEData(i, nLVVEType, m_LVVEPara.lp_lvve_ctrl_nv_params[i]);
			if( !m_bLoadLVVE )
			{
				break;
			}
			//teana hu 2011.06.07
			//m_bLVVEMin = m_Cmd.LVVEIsMaxOrMin();
			//
			//teana hu 2012.01.09
			m_dwLvveType = m_Cmd.GetLVVEType();
			//
		}
	}
	//

	//teana hu 2012.01.31
	int nDspModeCount = 0;
	m_bLoadDspCodec = m_Cmd.GetDspModeCount( nDspModeCount );
	if(m_bLoadDspCodec)
	{	
		m_DspModePara.nModeCount = nDspModeCount;
		m_DspModePara.lp_audio_nv_dsp_mode_info = new AUDIO_NV_DSP_MODE_INFO_T[nDspModeCount];
		for(int i = 0; i < nDspModeCount; i++)
		{
			CString strModeName;
			m_bLoadDspCodec = m_Cmd.GetDspModeName( i,strModeName );
			if( !m_bLoadDspCodec )
			{
				break;
			}
			
#ifdef _UNICODE
			WideCharToMultiByte(CP_ACP,0,strModeName.GetBuffer(strModeName.GetLength()),-1,(char *)m_DspModePara.lp_audio_nv_dsp_mode_info[i].ucModeName,strModeName.GetLength(),NULL,NULL);
			m_DspModePara.lp_audio_nv_dsp_mode_info[i].ucModeName[strModeName.GetLength()] = '\0';
#else
			strcpy( (char*)m_DspModePara.lp_audio_nv_dsp_mode_info[i].ucModeName,strModeName );
#endif  
			ZeroMemory( (LPVOID)&m_DspModePara.lp_audio_nv_dsp_mode_info[i].tAudioNvDspModeStruct,sizeof( AUDIO_NV_DSP_MODE_STRUCT_T ) );
			m_bLoadDspCodec = m_Cmd.GetDspData(i, nDspType, m_DspModePara.lp_audio_nv_dsp_mode_info[i].tAudioNvDspModeStruct);
			if( !m_bLoadDspCodec)
			{
				break;
			}
		}

		m_bLoadDspCodec = m_Cmd.GetDspCodecInfo(m_nDspCodecInfo);
		if(m_bLoadDspCodec)
		{
			if((m_nDspCodecInfo & 0x100) >> 8 == 0)
			{
				m_bDspCodec = TRUE;
			}
			else
			{
				m_bDspCodec = FALSE;
			}
			m_nVolumeLevel_6530 = (m_nDspCodecInfo & 0x3e00) >> 9;
		}
	}
	//

    for( int i=0;i<nModeCount;i++ )
    {
        CString strModeName;
        m_bLoad = m_Cmd.GetModeName( i,strModeName );
        if( !m_bLoad )
        {
            break;
        }

#ifdef _UNICODE
		WideCharToMultiByte(CP_ACP,0,strModeName.GetBuffer(strModeName.GetLength()),-1,(char *)m_AudioData.lpAudioMode[i].mode_name,strModeName.GetLength(),NULL,NULL);
		m_AudioData.lpAudioMode[i].mode_name[strModeName.GetLength()] = '\0';
#else
		strcpy( (char*)m_AudioData.lpAudioMode[i].mode_name,strModeName );
#endif  

		ZeroMemory( (LPVOID)&m_AudioData.lpAudioMode[i].audio_data,sizeof( AUDIO_STRUCT ) );

	    m_bLoad = m_Cmd.GetModeData( i,nType, m_AudioData.lpAudioMode[i].audio_data );
        if( !m_bLoad )
        {
            break;
        }

		if( i == 0 )
		{
			// Decide audio data version
			DecideAudioVersion();
		}

        CAudioModePage* pPage = new CAudioModePage;
        pPage->SetTitle( strModeName );
        
        pPage->SetAudioData( m_AudioData.lpAudioMode[i].audio_data,this );

		//teana hu 2011.02.21
		BOOL bSmartPhone = FALSE;
		if(m_bLoadVolume)
		{
			
			if((m_VolumeData.lpVolumeMode[0].volume_data.reserve[40] & 0xff) == 0x2)
			{
				bSmartPhone = TRUE;
			}
			else
			{
				bSmartPhone = FALSE;
			}

			for(int j = 0; j < nVolumeModeCount; j++)
			{
				if(strcmp((const char *) m_VolumeData.lpVolumeMode[j].mode_name, (const char *)m_AudioData.lpAudioMode[i].mode_name) == 0)
				{
					pPage->SetVolumeData( m_VolumeData.lpVolumeMode[j].volume_data, this);
					break;
				}
			}
		}
		pPage->SetSmartPhoneFlag(bSmartPhone);
		//
		//teana hu 2011.04.18
		if(m_bLoadLVVE)
		{
			for(int j = 0; j < nLVVEModeCount; j++)
			{
				if(strcmp((const char *) m_LVVEPara.lp_lvve_ctrl_nv_params[j].ucModeName, (const char *)m_AudioData.lpAudioMode[i].mode_name) == 0)
				{
					pPage->m_bExistLVVE = TRUE;
					pPage->SetLVVEData( m_LVVEPara.lp_lvve_ctrl_nv_params[j]);
					break;
				}
			}
		}
		//

		//teana hu 2012.01.31
		if(m_bLoadDspCodec)
		{
			for(int j = 0; j < nDspModeCount; j++)
			{
				if(strcmp((const char *) m_DspModePara.lp_audio_nv_dsp_mode_info[j].ucModeName, (const char *)m_AudioData.lpAudioMode[i].mode_name) == 0)
				{
					pPage->m_bExistDspCodec = TRUE;
					pPage->SetDspCodecFlag(m_bDspCodec);
					pPage->SetDspData( m_DspModePara.lp_audio_nv_dsp_mode_info[j].tAudioNvDspModeStruct, m_nVolumeLevel_6530);
					break;
				}
			}
		}
		
		//
		
        m_arrAudioModePages.Add( pPage );

    }

    if( i == nModeCount )
    {
        // All success

        for( i=0;i<nModeCount;i++ )
        {
            AddPage( m_arrAudioModePages[i] );
        }
        UpdatePageTitle();
    }
    else
    {
        // Some commands failed
        for( i=0;i<m_arrAudioModePages.GetSize();i++ )
        {
            CAudioModePage* pPage = m_arrAudioModePages[i];
            delete pPage;
        }
        
        m_arrAudioModePages.RemoveAll();
        m_bLoad = FALSE;
    }

    return m_bLoad;
}

BOOL CAudioTesterSheet::LoadArmAudioModes(DWORD dwType)
{
	if( !ConfirmLoadMusic() )
    {
        //return m_bLoadENHA;
		return FALSE;
    }
	
	int nVolumeType = 2;
	switch(dwType) {
	case MS_MODE:
		nVolumeType = 9;
		break;
	case FLASH_MODE:
		nVolumeType = 12;
		break;
	default:
		break;
	}

	//teana hu 2012.02.10
	m_bLoadDspCodec = m_Cmd.GetDspCodecInfo(m_nDspCodecInfo);
	if(m_bLoadDspCodec)
	{
		if((m_nDspCodecInfo & 0x100) >> 8 == 0)
		{
			m_bDspCodec = TRUE;
		}
		else
		{
			m_bDspCodec = FALSE;
		}
		m_nVolumeLevel_6530 = (m_nDspCodecInfo & 0x3e00) >> 9;
	}
	
	//

	//teana hu 2011.02.16
	int nVolumeModeCount = 0;
	m_bLoadVolume = m_Cmd.GetVolumeModeCount( nVolumeModeCount );
	if(m_bLoadVolume)
	{
		int nSerailCount = 0;
		m_bLoadVolume = m_Cmd.GetVolumeSerailCount(nSerailCount);
		if(nSerailCount == 0)
		{
			m_bLoadVolume = FALSE;
		}
		if(m_bLoadVolume)
		{
			m_nArmSerialCount = nSerailCount;
			
			m_VolumeData.mode_count = nVolumeModeCount;
			m_VolumeData.lpVolumeMode = new VOLUME_MODE[ nVolumeModeCount ];
			for(int i = 0; i < nVolumeModeCount; i++)
			{
				CString strModeName;
				m_bLoadVolume = m_Cmd.GetVolumeModeName( i,strModeName );
				if( !m_bLoadVolume )
				{
					break;
				}
				
#ifdef _UNICODE
				WideCharToMultiByte(CP_ACP,0,strModeName.GetBuffer(strModeName.GetLength()),-1,(char *)m_VolumeData.lpVolumeMode[i].mode_name,strModeName.GetLength(),NULL,NULL);
				m_VolumeData.lpVolumeMode[i].mode_name[strModeName.GetLength()] = '\0';
#else
				strcpy( (char*)m_VolumeData.lpVolumeMode[i].mode_name,strModeName );
#endif  
				//teana hu 2011.02.16
				ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[i].volume_data,sizeof( VOLUME_STRUCT ) );
				//
				m_bLoadVolume = m_Cmd.GetVolumeData(i, nVolumeType, m_VolumeData.lpVolumeMode[i].volume_data);
				if( !m_bLoadVolume )
				{
					break;
				}

				//teana hu 2012.06.14
				if(strcmp((const char *)m_VolumeData.lpVolumeMode[i].mode_name, "Handsfree") == 0)
				{
					
					int n = (m_VolumeData.lpVolumeMode[i].volume_data.app_config_info_set.aud_proc_exp_control[1] & 0x8000) >>15;
					if(n == 0)
					{
						m_bSerialPara = FALSE;
					}
					else
					{
						m_bSerialPara = TRUE;
						m_bExportOldEQNv = FALSE;
					}
				}
			//
				
				CMusicModePage* pPage = new CMusicModePage;
				pPage->SetTitle( strModeName );

				pPage->SetDspCodecFlag(m_bDspCodec);
				
				pPage->SetValue( m_VolumeData.lpVolumeMode[i].volume_data);
				
				pPage->SetPageIndex(i);
				
				m_arrMusicModePages.Add( pPage );
			}
			
			if( i == nVolumeModeCount )
			{
				// All success
				
				for( i=0;i<nVolumeModeCount;i++ )
				{
					AddPage( m_arrMusicModePages[i] );
				}
				UpdateMusicPageTitle();
			}
			else
			{
				// Some commands failed
				for( i=0;i<m_arrMusicModePages.GetSize();i++ )
				{
					CMusicModePage* pPage = m_arrMusicModePages[i];
					delete pPage;
				}
				
				m_arrMusicModePages.RemoveAll();
				m_bLoadVolume = FALSE;
			}
		}
	}
	//

    return m_bLoadVolume;
}

BOOL CAudioTesterSheet::LoadEQModes(int nPos)
{
    int nEQCount;
    m_bLoadEQ = m_Cmd.GetEQCount( nEQCount );
    if( !m_bLoadEQ )
    {
        // Can not read mode count from phone
        return FALSE;
    }

	if( nEQCount == 0 )
	{
		// No EQ count returned
		m_bLoadEQ = 0;
		return FALSE;
	}

    m_EQData.eq_count = nEQCount;
	if(m_EQData.lpEQMode == NULL)
	{
		m_EQData.lpEQMode = new AUDIO_EQ_STRUCT_T[ nEQCount ];
	}

    for( int i=0;i<nEQCount;i++ )
    {
		ZeroMemory( (LPVOID)&m_EQData.lpEQMode[i],sizeof( AUDIO_EQ_STRUCT_T ) );

	    m_bLoadEQ = m_Cmd.GetEQData( i+1,m_EQData.lpEQMode[i], nPos );
        if( !m_bLoadEQ )
        {
            break;
        }
    }

	if( m_bLoadEQ )
	{
		if( sizeof( AUDIO_EQ_STRUCT_T ) == m_Cmd.GetEQDataSize() )
		{
			m_EQData.eq_platform = eq_platform_all;
		}
		else
		{
			m_EQData.eq_platform = eq_platform_6800_only;
		}
	}

    return m_bLoadEQ;
}

BOOL CAudioTesterSheet::SetMixModes()
{

 	m_Cmd.OpenMixMode();
 
 	if(!m_Cmd.CloseArmLog() || !m_Cmd.CloseDeepSleep())
 	{
 		Log( _T("Please set it manually in the phone!"),FALSE );
 	}
	
	return TRUE;
}

BOOL CAudioTesterSheet::OpenMixMode()
{
	return m_Cmd.OpenMixMode();
}

void CAudioTesterSheet::ClearAllModes()
{
    for( int i=0;i<m_arrAudioModePages.GetSize();i++ )
    {
        CAudioModePage* pPage = m_arrAudioModePages[i];
        RemovePage( pPage );
        delete pPage;
    }

    if( m_AudioData.lpAudioMode )
    {
        delete m_AudioData.lpAudioMode;
        m_AudioData.lpAudioMode = NULL;
    }

	//teana hu 2011.02.16
	if(m_VolumeData.lpVolumeMode != NULL)
	{
		delete m_VolumeData.lpVolumeMode;
		m_VolumeData.lpVolumeMode = NULL;
		m_VolumeData.mode_count = 0;
	}
	m_bLoadVolume = FALSE;
	//

    m_arrAudioModePages.RemoveAll();

    m_bLoad = FALSE;

	//teana hu 2010.06.08
	if(m_EQData.lpEQMode != NULL)
	{
		delete m_EQData.lpEQMode;
		m_EQData.lpEQMode = NULL;
		m_EQData.eq_count = 0;
	}
	m_bLoadEQ = FALSE;
	//

	//teana hu 2012.01.31
	if(m_DspModePara.lp_audio_nv_dsp_mode_info != NULL)
	{
		delete m_DspModePara.lp_audio_nv_dsp_mode_info;
		m_DspModePara.lp_audio_nv_dsp_mode_info = NULL;
		m_DspModePara.nModeCount = 0;
	}

	m_bLoadDspCodec = FALSE;
	//
}

void CAudioTesterSheet::ClearAllMusicModes()
{
	for( int i=0;i<m_arrMusicModePages.GetSize();i++ )
    {
        CMusicModePage* pPage = m_arrMusicModePages[i];
        RemovePage( pPage );
        delete pPage;
    }
	
	
	if(m_VolumeData.lpVolumeMode != NULL)
	{
		delete m_VolumeData.lpVolumeMode;
		m_VolumeData.lpVolumeMode = NULL;
	}

	m_bLoadVolume = FALSE;
	
    m_arrMusicModePages.RemoveAll();
	
	if(m_eqData_g.lpEQMode != NULL)
	{
		delete m_eqData_g.lpEQMode;
		m_eqData_g.lpEQMode = NULL;
	}

	if(m_tunData_g.lpTunMode != NULL)
	{
		delete m_tunData_g.lpTunMode;
		m_tunData_g.lpTunMode = NULL;
	}
	m_bLoadENHA = FALSE;
	
		//teana hu 2011.04.18
	if(m_LVVEPara.lp_lvve_ctrl_nv_params != NULL)
	{
		delete m_LVVEPara.lp_lvve_ctrl_nv_params;
		m_LVVEPara.lp_lvve_ctrl_nv_params = NULL;
	}
	m_bLoadLVVE = FALSE;
	//
}

BOOL CAudioTesterSheet::IsSurpportEnha(DWORD dwType)
{
	int nCount = 0;
	m_bLoadENHA = m_Cmd.GetEQTunModeCount(nCount, dwType);
	return m_bLoadENHA;

}

void CAudioTesterSheet::ChangeFont(CWnd* pWnd, CFont* pFont)
{
    CRect windowRect;
    
    // grab old and new text metrics
    TEXTMETRIC tmOld, tmNew;
    CDC * pDC = pWnd->GetDC();
    CFont * pSavedFont = pDC->SelectObject(pWnd->GetFont());
    pDC->GetTextMetrics(&tmOld);
    pDC->SelectObject(pFont);
    pDC->GetTextMetrics(&tmNew);
    pDC->SelectObject(pSavedFont);
    pWnd->ReleaseDC(pDC);
    
    long oldHeight = tmOld.tmHeight+tmOld.tmExternalLeading;
    long newHeight = tmNew.tmHeight+tmNew.tmExternalLeading;
    
    // calculate new dialog window rectangle
    CRect clientRect, newClientRect, newWindowRect;
    
    pWnd->GetWindowRect(windowRect);
    pWnd->GetClientRect(clientRect);
    long xDiff = windowRect.Width() - clientRect.Width();
    long yDiff = windowRect.Height() - clientRect.Height();
    
    newClientRect.left = newClientRect.top = 0;
    newClientRect.right = clientRect.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
    newClientRect.bottom = clientRect.bottom * newHeight / oldHeight;
    
    newWindowRect.left = windowRect.left - 
        (newClientRect.right - clientRect.right)/2;
    newWindowRect.top = windowRect.top -
        (newClientRect.bottom - clientRect.bottom)/2;
    newWindowRect.right = newWindowRect.left + newClientRect.right + xDiff;
    newWindowRect.bottom = newWindowRect.top + newClientRect.bottom + yDiff;
    
    pWnd->MoveWindow(newWindowRect);
    pWnd->SetFont(pFont);
    
    // iterate through and move all child windows and change their font.
    CWnd* pChildWnd = pWnd->GetWindow(GW_CHILD);
    
    while (pChildWnd)
    {
        pChildWnd->SetFont(pFont);
        pChildWnd->GetWindowRect(windowRect);
        CString strClass;
        ::GetClassName(pChildWnd->m_hWnd, strClass.GetBufferSetLength(32), 31);
        strClass.MakeUpper();
        if(strClass==_T("COMBOBOX"))
        {
            CRect rect;
            pChildWnd->SendMessage(CB_GETDROPPEDCONTROLRECT,0,(LPARAM) &rect);
            windowRect.right = rect.right;
            windowRect.bottom = rect.bottom;
        }
        
        pWnd->ScreenToClient(windowRect);
        windowRect.left = windowRect.left * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
        windowRect.right = windowRect.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
        windowRect.top = windowRect.top * newHeight / oldHeight;
        windowRect.bottom = windowRect.bottom * newHeight / oldHeight;
        pChildWnd->MoveWindow(windowRect);
        
        
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
    }
}

BOOL CAudioTesterSheet::OnInitDialog() 
{
	CPropertySheet::OnInitDialog();

	//teana hu 2012.02.20
	CTabCtrl *pCtrl = GetTabControl();
	pCtrl->ModifyStyle(TCS_MULTILINE, 0);
	//

	//teana hu 2012.01.16
	CMenu *pSysMenu = GetSystemMenu(FALSE);
	if(pSysMenu != NULL)
	{
		pSysMenu->AppendMenu(MF_STRING, SC_MINIMIZE);
		ModifyStyle(0, WS_MINIMIZEBOX);
	}
	//

    m_psh.dwFlags |= PSH_NOAPPLYNOW;
    //m_psh.dwFlags &= ~PSH_HASHELP;
    
    CPropertyPage* pPage = GetActivePage ();
    ASSERT (pPage);
    
    // Get the font information from the dialog template
    CDialogTemplate dlgtemp;
    VERIFY (dlgtemp.Load (pPage->m_psp.pszTemplate));
    CString strFace;
    WORD	wSize;
    VERIFY (dlgtemp.GetFont (strFace, wSize));
    if (m_Font.m_hObject)
    {
        VERIFY (m_Font.DeleteObject ());
    }
    VERIFY (m_Font.CreatePointFont (wSize*10, strFace));
    
    ChangeFont( this,&m_Font );	

    // change the font for each page
    for (int iCntr = 0; iCntr < GetPageCount (); iCntr++)
    {
        VERIFY (SetActivePage (iCntr));
        CPropertyPage* pActivePage = GetActivePage ();
        ASSERT (pActivePage);
        ChangeFont (pActivePage, &m_Font);
    }
    
    VERIFY (SetActivePage (pPage));

    CString strTitle;
    strTitle.LoadString( IDS_EXIT );
    GetDlgItem( IDOK )->SetWindowText( strTitle );
    strTitle.LoadString( IDS_CLEAR_LOG );
    GetDlgItem( IDCANCEL )->SetWindowText( strTitle );
//    GetDlgItem( IDHELP )->ShowWindow( SW_HIDE );
    strTitle.LoadString( IDS_STRING_ABOUT );
    GetDlgItem( IDHELP )->SetWindowText( strTitle );
    GetDlgItem( ID_APPLY_NOW )->ShowWindow( SW_HIDE );

    AddLogWindow();
    
	m_CtrlStatus.InitCtrlStatus();
	
	Log( _T("Suggestion: A best resolution is: 1280*1024") );

	//teana hu 2012.05.02
	TCHAR szCfgPathName[MAX_PATH];
	::GetModuleFileName( NULL, szCfgPathName, _MAX_PATH );
	LPTSTR pExt = _tcsrchr(szCfgPathName, _T('.'));
	_tcscpy(pExt + 1, _T("ini"));
	m_bExportOldNv = GetPrivateProfileInt( _T("NV"), _T("ExportOldNVMode"), 0, szCfgPathName );
	m_bExportOldEQNv = GetPrivateProfileInt( _T("NV"), _T("ExportOldEQNVMode"), 0, szCfgPathName );
	//
	
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAudioTesterSheet::Save()
{
    CString strFilter;
    strFilter.LoadString( IDS_FILTER );
    CFileDialog dlg( FALSE,
                        _T(".aud"),
                        NULL,
                        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                        strFilter,
                        this );
    if( IDOK != dlg.DoModal() )
    {
        return FALSE;
    }

    CString strFileName = dlg.GetFileName();

    GetFileHeadInfo();

    //Refresh audio data
    FetchValue( m_AudioData );
    m_structFileHead.m_nAudioDataSize = m_Cmd.GetAudioDataSize();
	//teana hu 2010.06.10
	m_structFileHead.m_nEQDataSize = m_Cmd.GetEQDataSize();
	//

	//teana hu 2011.02.17
	m_structFileHead.m_nVolumeDataSize = m_Cmd.GetVolumeDataSize();
	//
    
    CFile fSaveFile( strFileName, CFile::modeWrite|CFile::modeCreate );
    CArchive ofFile( &fSaveFile, CArchive::store );

    //write file head
    ofFile << m_structFileHead.m_fFileVersion;
    ofFile << m_structFileHead.m_Datetime;
    ofFile << m_structFileHead.m_strProducer;
    ofFile << m_structFileHead.m_nAudioDataSize;
	//teana hu 2010.06.10
	if(m_bLoadEQ)
	{
		ofFile << m_structFileHead.m_nEQDataSize;
	}
	//

	//teana hu 2011.02.17
	if(m_bLoadVolume)
	{
		ofFile << m_structFileHead.m_nVolumeDataSize;
	}
	//

    ofFile << m_AudioData.mode_count;

	for( int j=0;j<m_AudioData.mode_count;j++ )
	{
		ofFile.Write( m_AudioData.lpAudioMode+j, m_structFileHead.m_nAudioDataSize + MODE_NAME_LEN);
	}

    int nSize = m_arrAudioModePages.GetSize();
    
    //ofFile << nSize;
    
    for( int i=0;i<nSize;i++ )
    {
        // Some special data should be saved
        m_arrAudioModePages[i]->WriteToFile( ofFile );
    }
	
	//teana hu 2010.06.10
	if(m_bLoadEQ)
	{
		ofFile << m_EQData.eq_count;
		for( int j=0;j<m_EQData.eq_count;j++ )
		{
			ofFile.Write( m_EQData.lpEQMode+j, m_structFileHead.m_nEQDataSize );
		}
	}
	//

	//teana hu 2011.02.17
	if(m_bLoadVolume)
	{
		FetchVolumeValue(m_VolumeData);
	}
	if(m_bLoadVolume)
	{
		ofFile << m_VolumeData.mode_count;

		for( int j=0;j<m_VolumeData.mode_count;j++ )
		{
			ofFile.Write( m_VolumeData.lpVolumeMode+j, m_structFileHead.m_nVolumeDataSize + MODE_NAME_LEN);
		}
	}
	//
    
    ofFile.Close();
    fSaveFile.Close();

    return TRUE;
}

void CAudioTesterSheet::GetFileHeadInfo()
{  
	//teana hu 2010.06.10
	if(m_bLoadEQ && !m_bLoadVolume)
	{
		m_structFileHead.m_fFileVersion = (float)3.5;
	}
	else if(m_bLoadVolume && !m_bLoadEQ)
	{
		m_structFileHead.m_fFileVersion = (float)4.0;
	}
	else if(m_bLoadVolume && m_bLoadEQ)
	{
		m_structFileHead.m_fFileVersion = (float)4.1;
	}
	else
	{
		m_structFileHead.m_fFileVersion = (float)3.1;
	}
    char chBuffer[MAX_COMPUTERNAME_LENGTH + 1 ];
    DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
#ifdef _UNICODE
	TCHAR szTmp[MAX_COMPUTERNAME_LENGTH * 2 + 1] = {0};
    GetComputerName(szTmp,&dwSize);
	WideCharToMultiByte(CP_ACP,0,szTmp,-1,chBuffer,MAX_COMPUTERNAME_LENGTH + 1,NULL,NULL);
#else
	GetComputerName(chBuffer,&dwSize);
#endif

    m_structFileHead.m_strProducer = chBuffer;    
    m_structFileHead.m_Datetime = CTime::GetCurrentTime();

    m_structFileHead.m_nAudioDataSize = 0;
}

BOOL CAudioTesterSheet::Load()
{
    if( !ConfirmLoad() )
    {
        return m_bLoad;
    }

    CString strFilter;
    strFilter.LoadString( IDS_FILTER );
    CAudioFileOpenDlg dlg(TRUE,_T(".aud"),NULL,OFN_HIDEREADONLY,strFilter,this);
    if( IDOK == dlg.DoModal() )
    {
        CWaitCursor temp;
        CString strFileName = dlg.GetFileName();  
		
		
        CFile fReadFile(strFileName,CFile::modeRead );
        CArchive ofFile( &fReadFile,CArchive::load );
        //read file header
        ofFile >> m_structFileHead.m_fFileVersion;
        ofFile >> m_structFileHead.m_Datetime;
        ofFile >> m_structFileHead.m_strProducer;

		DWORD dwReadSizeAudio = 0;

		DWORD dwReadSizeEQ = 0;

		DWORD dwReadSizeVolume = 0;

        if( m_structFileHead.m_fFileVersion == 2.0 )
        {
            // Old version
            m_structFileHead.m_nAudioDataSize = EXTEND_VERSION_SIZE;
			dwReadSizeAudio = EXTEND_VERSION_SIZE + MODE_NAME_LEN;
			//teana hu 2010.06.10
			m_bLoadEQ = FALSE;
			//
        }
        else if( m_structFileHead.m_fFileVersion == 3.0 )
        {
            ofFile >> m_structFileHead.m_nAudioDataSize;
			dwReadSizeAudio = ROCKY_VERSION_SIZE + MODE_NAME_LEN;
			//teana hu 2010.06.10
			m_bLoadEQ = FALSE;
			//
        }
		else if( m_structFileHead.m_fFileVersion > 3.0 &&  m_structFileHead.m_fFileVersion != 3.5)
		{
			// 3.1 and later version
			ofFile >> m_structFileHead.m_nAudioDataSize;
			dwReadSizeAudio = m_structFileHead.m_nAudioDataSize + MODE_NAME_LEN;
			//teana hu 2010.06.10
			m_bLoadEQ = FALSE;
			//
		}
		else if(m_structFileHead.m_fFileVersion == 3.5)
		{
			ofFile >> m_structFileHead.m_nAudioDataSize;
			ofFile >> m_structFileHead.m_nEQDataSize;
			dwReadSizeAudio = m_structFileHead.m_nAudioDataSize + MODE_NAME_LEN;
			dwReadSizeEQ = m_structFileHead.m_nEQDataSize;
			m_bLoadEQ = TRUE;
			m_bLoadVolume = FALSE;
		}
		else if(m_structFileHead.m_fFileVersion == 4.0)
		{
			ofFile >> m_structFileHead.m_nAudioDataSize;
			ofFile >> m_structFileHead.m_nVolumeDataSize;
			dwReadSizeAudio = m_structFileHead.m_nAudioDataSize + MODE_NAME_LEN;
			dwReadSizeVolume = m_structFileHead.m_nVolumeDataSize;
			m_bLoadEQ = FALSE;
			m_bLoadVolume = TRUE;
		}
		else if(m_structFileHead.m_fFileVersion == 4.1)
		{
			ofFile >> m_structFileHead.m_nAudioDataSize;
			ofFile >> m_structFileHead.m_nEQDataSize;
			ofFile >> m_structFileHead.m_nVolumeDataSize;
			dwReadSizeAudio = m_structFileHead.m_nAudioDataSize + MODE_NAME_LEN;
			dwReadSizeEQ = m_structFileHead.m_nEQDataSize;
			dwReadSizeVolume = m_structFileHead.m_nVolumeDataSize;
			m_bLoadEQ = TRUE;
			m_bLoadVolume = TRUE;
		}
        else
        {
            // Invalid or unsurpported file version
            m_bLoad = FALSE;
            return m_bLoad;
        }

        ofFile >> m_AudioData.mode_count;
        m_AudioData.lpAudioMode = new AUDIO_MODE[m_AudioData.mode_count];
		for( int j=0;j<m_AudioData.mode_count;j++ )
		{
			ZeroMemory( (LPVOID)&m_AudioData.lpAudioMode[j].audio_data,sizeof( AUDIO_STRUCT ) );
			ofFile.Read( m_AudioData.lpAudioMode+j,dwReadSizeAudio );
		}

        // Decide audio data version
        m_Cmd.SetAudioDataSize( m_structFileHead.m_nAudioDataSize );
        DecideAudioVersion();

		//teana hu 2010.03.10
		if(m_bLoadEQ)
		{
			ofFile >> m_EQData.eq_count;
			m_EQData.lpEQMode = new AUDIO_EQ_STRUCT_T[m_EQData.eq_count];
			for( j=0;j<m_EQData.eq_count;j++ )
			{
				ZeroMemory( (LPVOID)&m_EQData.lpEQMode[j],sizeof( AUDIO_EQ_STRUCT_T ) );
				ofFile.Read( m_EQData.lpEQMode+j,dwReadSizeEQ );
			}
		}
		//

		//teana hu 2011.02.17
		if(m_bLoadVolume)
		{
			ofFile >> m_VolumeData.mode_count;

			m_VolumeData.lpVolumeMode = new VOLUME_MODE[m_VolumeData.mode_count];
			for( j=0;j<m_VolumeData.mode_count;j++ )
			{
				ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[j].volume_data,sizeof( VOLUME_STRUCT ) );
				ofFile.Read( m_VolumeData.lpVolumeMode+j,dwReadSizeVolume );
			}
		}
		//

        for( int i=0;i<m_AudioData.mode_count;i++ )
        {
            CAudioModePage* pPage = new CAudioModePage;
			CString strTitle(m_AudioData.lpAudioMode[i].mode_name);
            pPage->SetTitle( strTitle );
            pPage->SetAudioData( m_AudioData.lpAudioMode[i].audio_data,this );
			//teana hu 2011.02.21
			BOOL bSmartPhone = FALSE;
			if(m_bLoadVolume)
			{
				
				if((m_VolumeData.lpVolumeMode[0].volume_data.reserve[40] & 0xff) == 0x2)
				{
					bSmartPhone = TRUE;
				}
				else
				{
					bSmartPhone = FALSE;
				}
				for(int j = 0; j < m_AudioData.mode_count; j++)
				{
					if(strcmp((const char *) m_VolumeData.lpVolumeMode[j].mode_name, (const char *)m_AudioData.lpAudioMode[i].mode_name) == 0)
					{
						pPage->SetVolumeData( m_VolumeData.lpVolumeMode[j].volume_data, this);
						break;
					}
				}
			}
			pPage->SetSmartPhoneFlag(bSmartPhone);
			//
			m_arrAudioModePages.Add( pPage );
            AddPage( pPage );

            // Some special data need to be loaded
            pPage->LoadFromFile( ofFile );
        }

        ofFile.Close();
        fReadFile.Close();

        UpdatePageTitle();
        
        m_bLoad = TRUE;
    }

    return m_bLoad;
}

void CAudioTesterSheet::UpdatePageTitle()
{
    int nPage = GetPageCount();
    CTabCtrl* pTab = GetTabControl();
    CString strText;
    TCITEM ti;
    ti.mask = TCIF_TEXT;
//    int nCount = pTab->GetItemCount();
    
    for( int i=1;i<nPage;i++ )
    {
        m_arrAudioModePages[i-1]->GetTitle( strText );
        ti.pszText = strText.GetBuffer( 0 );
        pTab->SetItem( i,&ti );
    }
}

void CAudioTesterSheet::UpdateMusicPageTitle()
{
    int nPage = GetPageCount();
    CTabCtrl* pTab = GetTabControl();
    CString strText;
    TCITEM ti;
    ti.mask = TCIF_TEXT;
	//    int nCount = pTab->GetItemCount();
    
    for( int i=1;i<nPage;i++ )
    {
        m_arrMusicModePages[i-1]->GetTitle( strText );
        ti.pszText = strText.GetBuffer( 0 );
        pTab->SetItem( i,&ti );
    }
}

void Export(AUDIO_STRUCT& audio_date,std::vector<NVVALUE_TABLE_ENTRY>& lpvValues,
            BOOL bExtend,BOOL bRocky,BOOL bThree )
{
    NVVALUE_TABLE_ENTRY temp;

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_DA_device_internal" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_device_internal;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_DA_device_external" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_device_external;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_AD_device_internal" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_device_internal;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_AD_device_external" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_device_external;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"sidetone_switch" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.sidetone_switch;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"aec_switch" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.aec_switch;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"volume_mode" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.volume_mode;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_PGA_gain_l" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_pga_gain_l;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_PGA_gain_h" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_pga_gain_h;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_PGA_gain_l" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_pga_gain_l;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_PGA_gain_h" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_pga_gain_h;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"Sample_rate" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.sample_rate;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_DP_gain" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_dp_gain;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_DP_attenu" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_dp_attenu;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_EQ_bass_alpha" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_eq_bass_alpha;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_EQ_bass_beta" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_eq_bass_beta;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_EQ_bass_gama" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_eq_bass_gama;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_EQ_bass_gain" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_eq_bass_gain;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_EQ_mid_alpha" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_eq_mid_alpha;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_EQ_mid_beta" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_eq_mid_beta;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_EQ_mid_gama" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_eq_mid_gama;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_EQ_mid_gain" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_eq_mid_gain;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_EQ_treble_alpha" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_eq_treble_alpha;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_EQ_treble_beta" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_eq_treble_beta;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_EQ_treble_gama" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_eq_treble_gama;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_EQ_treble_gain" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_eq_treble_gain;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"digital_sidetone_gain" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.digital_sidetone_gain;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_DP_gain" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_dp_gain;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_DP_attenu" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_dp_attenu;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_EQ_bass_alpha" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_eq_bass_alpha;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_EQ_bass_beta" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_eq_bass_beta;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_EQ_bass_gama" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_eq_bass_gama;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_EQ_bass_gain" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_eq_bass_gain;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_EQ_treble_alpha" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_eq_treble_alpha;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_EQ_treble_beta" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_eq_treble_beta;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_EQ_treble_gama" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_eq_treble_gama;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_EQ_treble_gain" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_eq_treble_gain;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_POP_switch" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_pop_switch;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_AGC_switch" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_agc_switch;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_EQ_AGC_switch" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_eq_agc_switch;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"DP_switch" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dp_switch;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_EQ_switch" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_eq_switch;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"d1_AGC_Rsv0" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.d1_agc_rsv0;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"d1_AGC_Rsv1" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.d1_agc_rsv1;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_EQ_switch" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_eq_switch;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_DA_pop_switch" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_da_pop_switch;
    lpvValues.push_back( temp );
    
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_FIR_HPF_enable" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_fir_hpf_enable;
    lpvValues.push_back( temp );
   
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"aec_enable" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.aec_enable;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"pdelay" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.pdelay;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_ref_HPF_enable" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_ref_hpf_enable;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"decor_filter_enable" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.decor_filter_enable;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"fir_taps" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.fir_taps;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"Aec_frozen" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.aec_frozen;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"coeff_frozen" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.coeff_frozen;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"DT_dect_threshold" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dt_dect_threshold;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"Dt_noise_floor" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dt_noise_floor;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"step_size" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.step_size;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"coeff_norm_shift" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.coeff_norm_shift;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"SA_ctrl" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.sa_ctrl;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"send-attenu_in_dt" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.send_attenu_in_dt;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"send_attenu_in_rv" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.send_attenu_in_rv;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"send_threshold" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.send_threshold;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"r2dt_threshold" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.r2dt_threshold;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"s2dt_threshold" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.s2dt_threshold;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"recv_threshold" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.recv_threshold;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"Bn40" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.bn40;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"Sa_AR" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.sa_ar;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ng_select" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ng_select;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"alpha_distor" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.alpha_distor;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"beta_distor" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.beta_distor;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_ng_plk_wPyy_a" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_ng_plk_wpyy_a;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_ng_plk_wPyy_n" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_ng_plk_wpyy_n;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_ng_plk_holdc" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_ng_plk_holdc;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_ng_plk_ATT" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_ng_plk_att;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_ng_clk_wPyy_a" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_ng_clk_wpyy_a;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_ng_clk_wPyy_n" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_ng_clk_wpyy_n;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_ng_clk_holdc" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_ng_clk_holdc;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"ul_ng_clk_ATT" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.ul_ng_clk_att;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_ng_plk_wPyy_a" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_ng_plk_wpyy_a;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_ng_plk_wPyy_n" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_ng_plk_wpyy_n;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_ng_plk_holdc" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_ng_plk_holdc;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_ng_plk_ATT" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_ng_plk_att;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_ng_clk_wPyy_a" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_ng_clk_wpyy_a;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_ng_clk_wPyy_n" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_ng_clk_wpyy_n;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_ng_clk_holdc" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_ng_clk_holdc;
    lpvValues.push_back( temp );
    
    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"dl_ng_clk_ATT" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.dl_ng_clk_att;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"DA_limit" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.da_limit;
    lpvValues.push_back( temp );

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"reserved" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Value = audio_date.reserved1;
    lpvValues.push_back( temp );

    if( bExtend )
    {
        int offset = 0;
        for( int i = 0;i<DL_FIR_EQ_COUNT;i++ )
        {
            memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
            sprintf( temp.Name,"extend\\extend[%d]",i+offset );
            temp.Type = SHORT_ITEM_TYPE;
            temp.Value = audio_date.dl_fir_eq_coeff[i];
            lpvValues.push_back( temp );
        }
        offset += i;
        
        for( i = 0;i<UL_FIR_EQ_COUNT;i++ )
        {
            memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
            sprintf( temp.Name,"extend\\extend[%d]",i+offset );
            temp.Type = SHORT_ITEM_TYPE;
            temp.Value = audio_date.ul_fir_eq_coeff[i];
            lpvValues.push_back( temp );
        }
        offset += i;
        
        for( i = 0;i<ST_FIR_EQ_COUNT;i++ )
        {
            memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
            sprintf( temp.Name,"extend\\extend[%d]",i+offset );
            temp.Type = SHORT_ITEM_TYPE;
            temp.Value = audio_date.st_fir_eq_coeff[i];
            lpvValues.push_back( temp );
        }
        offset += i;

        LPWORD lpExtend = audio_date.st_fir_eq_coeff + ST_FIR_EQ_COUNT;
        for( i=0;i<EXTEND_DATA_SIZE;i++ )
        {
            memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
            sprintf( temp.Name,"extend\\extend[%d]",i+offset );
            temp.Type = SHORT_ITEM_TYPE;		
            temp.Value = lpExtend[i];//lint !e661 !e662
            lpvValues.push_back( temp );
        }
        
        if( bRocky )
        {
            // The last 20 words are for dsp volume and arm volume 
            // arm volume,10 words
            for( i=0;i<10;i++ )
            {
                memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
                sprintf( temp.Name,"arm_volume\\arm_volume[%d]",i );
                temp.Type = SHORT_ITEM_TYPE;
                temp.Value = audio_date.arm_volume[i];
                lpvValues.push_back( temp );
            }
            
            // dsp volume,10 words
            for( i=0;i<10;i++ )
            {
                memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
                sprintf( temp.Name,"dsp_volume\\dsp_volume[%d]",i );
                temp.Type = SHORT_ITEM_TYPE;
                temp.Value = audio_date.dsp_volume[i];
                lpvValues.push_back( temp );
            }
        }
    }

	if( bThree )
	{
		int nCount = ( AUDIO_VERSION_3_SIZE - ROCKY_VERSION_SIZE ) / 2;
        LPWORD lpExtend = audio_date.dsp_volume + DSP_VOLUME_SIZE;

		for( int i=0;i<nCount;i++ )
		{
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
            sprintf( temp.Name,"extend2\\extend2[%d]",i );
            temp.Type = SHORT_ITEM_TYPE;
            temp.Value = lpExtend[i];//lint !e661 !e662
            lpvValues.push_back( temp );
		}
	}
}

void ExportEQ(AUDIO_EQ_STRUCT_T& eq_date,BOOL bExtend,std::vector<NVVALUE_TABLE_ENTRY>& lpvValues)
{
	NVVALUE_TABLE_ENTRY temp;

    memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"eqControl" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = eq_date.eq_mode_control;
    lpvValues.push_back( temp );

	//eq mode 1
	for(int i = 0; i < TOTAL_EQ_MODE_NUM; i ++)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\lcf\\f1_g0", i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_date.eq_modes[i].lcf_filter.f1_g0;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\lcf\\f1_g1", i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_date.eq_modes[i].lcf_filter.f1_g1;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\lcf\\f1_fp", i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_date.eq_modes[i].lcf_filter.f1_fp;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\lcf\\f2_g0",i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_date.eq_modes[i].lcf_filter.f2_g0;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\lcf\\f2_g1", i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_date.eq_modes[i].lcf_filter.f2_g1;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\lcf\\f2_fp", i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_date.eq_modes[i].lcf_filter.f2_fp;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\agc_in_gain", i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_date.eq_modes[i].agc_in_gain;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\eq_1\\bass_f0", i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_date.eq_modes[i].eq_bass.bass_f0;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\eq_1\\bass_q", i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_date.eq_modes[i].eq_bass.bass_q;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\eq_1\\bass_boost", i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_date.eq_modes[i].eq_bass.bass_boostdB;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\eq_1\\bass_gain", i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_date.eq_modes[i].eq_bass.bass_gaindB;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\eq_2\\treble_f0", i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_date.eq_modes[i].eq_treble.treble_f0;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\eq_2\\treble_q", i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_date.eq_modes[i].eq_treble.treble_q;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\eq_2\\treble_boost", i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_date.eq_modes[i].eq_treble.treble_boostdB;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\eq_2\\treble_gain" , i+1);
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_date.eq_modes[i].eq_treble.treble_gaindB;
		lpvValues.push_back( temp );
	}

	if( bExtend )
	{
		for( i=0;i<EQ_EXTEND_ARRAY_DIM;i++ )
		{
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			sprintf( temp.Name,"extend_array\\extend_array[%d]" , i);
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = eq_date.extend_array[i];
			lpvValues.push_back( temp );
		}
	}	
}


void ExportVolume(VOLUME_STRUCT& volume_date,std::vector<NVVALUE_TABLE_ENTRY>& lpvValues, UINT nArmSerialCount )
{
	NVVALUE_TABLE_ENTRY temp;

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
	strcpy( temp.Name,"dev_path_set\\valid_dev_set_count");
	temp.Type = SHORT_ITEM_TYPE;
	temp.Reserved = volume_date.dev_path_set.valid_dev_set_count;
	lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
	strcpy( temp.Name,"dev_path_set\\reserve");
	temp.Type = SHORT_ITEM_TYPE;
	temp.Reserved = volume_date.dev_path_set.reserve;
	lpvValues.push_back( temp );

	for(int i = 0; i < AUDIO_DEVICE_MODE_PARAM_MAX; i++)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"dev_path_set\\dev_set\\dev_set[%d]", i );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = volume_date.dev_path_set.dev_set[i];
		lpvValues.push_back( temp );
	}
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
	strcpy( temp.Name,"app_config_info_set\\valid_app_set_count");
	temp.Type = SHORT_ITEM_TYPE;
	temp.Reserved = volume_date.app_config_info_set.valid_app_set_count;
	lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
	strcpy( temp.Name,"app_config_info_set\\valid_agc_input_gain_count");
	temp.Type = SHORT_ITEM_TYPE;
	temp.Reserved = volume_date.app_config_info_set.valid_agc_input_gain_count;
	lpvValues.push_back( temp );

	for(i = 0; i < 2; i++)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"app_config_info_set\\aud_proc_exp_control\\aud_proc_exp_control[%d]", i);
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = volume_date.app_config_info_set.aud_proc_exp_control[i];
		lpvValues.push_back( temp );
	}

	int nCount = nArmSerialCount;

	for(i = 0; i < nCount; i++)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"app_config_info_set\\app_config_info\\app_config_info[%d]\\eq_switch", i );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = volume_date.app_config_info_set.app_config_info[i].eq_switch;
		lpvValues.push_back( temp );

		for(int j =0; j < AUDIO_AGC_INPUG_GAIN_MAX; j++)
		{
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			sprintf( temp.Name,"app_config_info_set\\app_config_info\\app_config_info[%d]\\agc_input_gain\\agc_input_gain[%d]", i, j );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = volume_date.app_config_info_set.app_config_info[i].agc_input_gain[j];
			lpvValues.push_back( temp );
		}

		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"app_config_info_set\\app_config_info\\app_config_info[%d]\\valid_volume_level_count", i );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = volume_date.app_config_info_set.app_config_info[i].valid_volume_level_count;
		lpvValues.push_back( temp );

		for( j =0; j < AUDIO_ARM_VOLUME_LEVEL; j++)
		{
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			sprintf( temp.Name,"app_config_info_set\\app_config_info\\app_config_info[%d]\\arm_volume\\arm_volume[%d]", i, j );
			
			//teana hu 202.01.30
			if(nCount == AUDIO_ARM_APP_TYPE_SIX)
			{
				temp.Type = LONG_ITEM_TYPE;
				temp.Value = volume_date.app_config_info_set.app_config_info[i].arm_volume_l[j];
			}
			else
			{
				temp.Type = SHORT_ITEM_TYPE;
				temp.Reserved = volume_date.app_config_info_set.app_config_info[i].arm_volume[j];
			}
			//
			lpvValues.push_back( temp );
		}

		for(j =0; j < AUDIO_NV_ARM_APP_PARA_RESERVE; j++)
		{
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			sprintf( temp.Name,"app_config_info_set\\app_config_info\\app_config_info[%d]\\reserve\\reserve[%d]", i, j );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = volume_date.app_config_info_set.app_config_info[i].reserve[j];
			lpvValues.push_back( temp );
		}
	}

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
	strcpy( temp.Name,"midi_opt" );
	temp.Type = SHORT_ITEM_TYPE;
	temp.Reserved = volume_date.midi_opt;
	lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
	strcpy( temp.Name,"aud_dev" );
	temp.Type = SHORT_ITEM_TYPE;
	temp.Reserved = volume_date.aud_dev;
	lpvValues.push_back( temp );

	for(i = 0; i < AUDIO_NV_ARM_PARA_RESERVE; i++)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"reserve\\reserve[%d]", i );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = volume_date.reserve[i];
		lpvValues.push_back( temp );
	}
}

void ExportDspCodec(AUDIO_NV_DSP_MODE_STRUCT_T& dsp_data,std::vector<NVVALUE_TABLE_ENTRY>& lpvValues )
{
	NVVALUE_TABLE_ENTRY temp;
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
	strcpy( temp.Name,"dac_set");
	temp.Type = SHORT_ITEM_TYPE;
	temp.Reserved = dsp_data.dac_set;
	lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
	strcpy( temp.Name,"adc_set");
	temp.Type = SHORT_ITEM_TYPE;
	temp.Reserved = dsp_data.adc_set;
	lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
	strcpy( temp.Name,"path_set");
	temp.Type = SHORT_ITEM_TYPE;
	temp.Reserved = dsp_data.path_set;
	lpvValues.push_back( temp );

	for(int i = 0; i < AUDIO_NV_DSP_VOL_CFG_NUM; i++)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"vol_cfg0\\vol_cfg0[%d]", i);
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = dsp_data.vol_cfg0[i];
		lpvValues.push_back( temp );
	}

	for(i = 0; i < AUDIO_NV_DSP_VOL_CFG_NUM; i++)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"vol_cfg1\\vol_cfg1[%d]", i);
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = dsp_data.vol_cfg1[i];
		lpvValues.push_back( temp );
	}

	for(i = 0; i < AUDIO_NV_DSP_VOL_CFG_NUM; i++)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"vol_cfg2\\vol_cfg2[%d]", i);
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = dsp_data.vol_cfg2[i];
		lpvValues.push_back( temp );
	}

	for(i = 0; i < AUDIO_NV_DSP_MODE_STRUCT_RESERVE; i++)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"resv\\resv[%d]", i);
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = dsp_data.reserve[i];
		lpvValues.push_back( temp );
	}
}

void ExportENHA_EQ(EQ_STRUCT_G_T& eq_data,std::vector<NVVALUE_TABLE_ENTRY>& lpvValues)
{
	NVVALUE_TABLE_ENTRY temp;

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"eq_control" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = eq_data.eq_control;
    lpvValues.push_back( temp );
	
	//eq mode
	for(int i = 0; i < EQ_MODE_G_NUM; i ++)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\agc_in_gain",i+1);
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_data.eq_mode[i].agc_in_gain;
		lpvValues.push_back( temp );

		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\band_control",i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_data.eq_mode[i].band_control;
		lpvValues.push_back( temp );

		for(int j = 0; j < EQ_BAND_G_NUM; j++)
		{
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			sprintf( temp.Name,"eq_mode_%d\\eq_band_%d\\fo", i+1, j+1 );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = eq_data.eq_mode[i].eq_band[j].f0;
			lpvValues.push_back( temp );
			
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			sprintf( temp.Name,"eq_mode_%d\\eq_band_%d\\q", i+1, j+1 );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = eq_data.eq_mode[i].eq_band[j].q;
			lpvValues.push_back( temp );

			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			sprintf( temp.Name,"eq_mode_%d\\eq_band_%d\\boostdB", i+1, j+1 );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = eq_data.eq_mode[i].eq_band[j].boostdB;
			lpvValues.push_back( temp );

			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			sprintf( temp.Name,"eq_mode_%d\\eq_band_%d\\gaindB", i+1, j+1 );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = eq_data.eq_mode[i].eq_band[j].gaindB;
			lpvValues.push_back( temp );
		}
	}

	//extendArray
	for(i = 0; i < EQ_EXTEND_G; i ++)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"extendArray\\extendArray[%d]", i);
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = eq_data.extend[i];
		lpvValues.push_back( temp );
	}

}

void ExportENHA_Tun(TUN_STRUCT_G_T& tun_data,std::vector<NVVALUE_TABLE_ENTRY>& lpvValues)
{
	NVVALUE_TABLE_ENTRY temp;
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"eq_control" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = tun_data.eq_control;
    lpvValues.push_back( temp );
	
	//fo_array
	for(int i = 0; i < BAND_F0_NUM; i ++)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"fo_array\\fo_band_%d",i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = tun_data.f0_band[i];
		lpvValues.push_back( temp );
	}

	//q_array
	for( i = 0; i < BAND_F0_NUM; i ++)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"q_array\\q_band_%d",i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = tun_data.q[i];
		lpvValues.push_back( temp );
	}

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"level_n" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = tun_data.level_n;
    lpvValues.push_back( temp );

	//eq mode
	for( i = 0; i < EQ_MODE_G_NUM; i ++)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\agc_in_gain",i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = tun_data.eq_mode[i].agc_in_gain;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"eq_mode_%d\\band_control",i+1 );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = tun_data.eq_mode[i].band_control;
		lpvValues.push_back( temp );
		
		for(int j = 0; j < BOOSTDB_DEFAULT_BAND_NUM; j++)
		{
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			sprintf( temp.Name,"eq_mode_%d\\boostdBDefault\\boostdB_default_band_%d", i+1, j+1 );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = tun_data.eq_mode[i].boostdB_default_band[j];
			lpvValues.push_back( temp );
		}
		
		for(j = 0; j < BOOSTDB_CURRENT_BAND_NUM; j++)
		{
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			sprintf( temp.Name,"eq_mode_%d\\boostdBCurrent\\boostdB_current_band_%d", i+1, j+1 );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = tun_data.eq_mode[i].boostdB_current_band[j];
			lpvValues.push_back( temp );
		}
	}
	
	//extendArray
	for(i = 0; i < EQ_EXTEND_GX; i ++)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"extendArray\\extendArray[%d]", i);
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = tun_data.extend[i];
		lpvValues.push_back( temp );
	}
	
}

void ExportLVVE(LVVE_CTRL_PARAM_NV_T& lvve_data,std::vector<NVVALUE_TABLE_ENTRY>& lpvValues, DWORD dwLvveType)
{
	NVVALUE_TABLE_ENTRY temp;
	
	//RX
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\OperatingMode" );
    temp.Type = LONG_ITEM_TYPE;
    temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.OperatingMode;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\Mute" );
    temp.Type = LONG_ITEM_TYPE;
    temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.Mute;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\VOL_OperatingMode" );
    temp.Type = LONG_ITEM_TYPE;
    temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_OperatingMode;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\VOL_Gain" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VOL_Gain;
    lpvValues.push_back( temp );

	if(dwLvveType != LVVE_JUNIPER)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\FENS_ControlParams\\OperatingMode" );
		temp.Type = LONG_ITEM_TYPE;
		temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams.OperatingMode;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\FENS_ControlParams\\FENS_limit_NS" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams.FENS_limit_NS;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\FENS_ControlParams\\Mode" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.FENS_ControlParams.Mode;
		lpvValues.push_back( temp );
	}
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\NLPP_OperatingMode" );
    temp.Type = LONG_ITEM_TYPE;
    temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_OperatingMode;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\NLPP_ControlParams\\NLPP_Limit" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams.NLPP_Limit;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\NLPP_ControlParams\\NLPP_HPF_CornerFreq" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NLPP_ControlParams.NLPP_HPF_CornerFreq;
    lpvValues.push_back( temp );

	if(dwLvveType != LVVE_JUNIPER)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\VC_ControlParams\\OperatingMode" );
		temp.Type = LONG_ITEM_TYPE;
		temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.OperatingMode;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\VC_ControlParams\\mode" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.mode;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\VC_ControlParams\\Gain_Lin_Max" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.Gain_Lin_Max;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\VC_ControlParams\\Noise_Sensitivity" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.Noise_Sensitivity;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\VC_ControlParams\\AVL_Target_level_lin" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Target_level_lin;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\VC_ControlParams\\AVL_MinGainLin" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_MinGainLin;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\VC_ControlParams\\AVL_MaxGainLin" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_MaxGainLin;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\VC_ControlParams\\AVL_NominalGain" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_NominalGain;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\VC_ControlParams\\AVL_Attack" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Attack;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\VC_ControlParams\\AVL_Release" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Release;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\VC_ControlParams\\AVL_ReleaseMax" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_ReleaseMax;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\VC_ControlParams\\AVL_Limit_MaxOutputLin" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.AVL_Limit_MaxOutputLin;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\VC_ControlParams\\SpDetect_Threshold" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.VC_ControlParams.SpDetect_Threshold;
		lpvValues.push_back( temp );
	}

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\EQ_OperatingMode" );
    temp.Type = LONG_ITEM_TYPE;
    temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_OperatingMode;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\EQ_ControlParams\\EQ_Length" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams.EQ_Length;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\EQ_ControlParams\\pEQ_Coefs" );
    temp.Type = LONG_ITEM_TYPE;
    temp.Value = 0;
    lpvValues.push_back( temp );

	if(dwLvveType != LVVE_NO_RESERVE && dwLvveType != LVVE_RESERVE && dwLvveType != LVVE_JUNIPER)
	{
		if(dwLvveType != LVVE_SINGLE_MIC)
		{
			//drc
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\DRC_ControlParams\\OperatingMode" );
			temp.Type = LONG_ITEM_TYPE;
			temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.OperatingMode;
			lpvValues.push_back( temp );
			
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\DRC_ControlParams\\NumKnees" );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.NumKnees;
			lpvValues.push_back( temp );
			
			for(int i = 0; i < LVMDRC_MAX_KNEES; i++)
			{
				memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
				sprintf( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\DRC_ControlParams\\CompressorCurveInputLevels\\CompressorCurveInputLevels[%d]", i );
				temp.Type = SHORT_ITEM_TYPE;
				temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.CompressorCurveInputLevels[i];
				lpvValues.push_back( temp );
			}
			
			for(i = 0; i < LVMDRC_MAX_KNEES; i++)
			{
				memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
				sprintf( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\DRC_ControlParams\\CompressorCurveOutputLevels\\CompressorCurveOutputLevels[%d]", i );
				temp.Type = SHORT_ITEM_TYPE;
				temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.CompressorCurveOutputLevels[i];
				lpvValues.push_back( temp );
			}
			
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\DRC_ControlParams\\AttackTime" );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.AttackTime;
			lpvValues.push_back( temp );
			
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\DRC_ControlParams\\ReleaseTime" );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.ReleaseTime;
			lpvValues.push_back( temp );
			
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\DRC_ControlParams\\LimiterOperatingMode" );
			temp.Type = LONG_ITEM_TYPE;
			temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.LimiterOperatingMode;
			lpvValues.push_back( temp );
			
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\DRC_ControlParams\\LimitLevel" );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.DRC_ControlParams.LimitLevel;
			lpvValues.push_back( temp );
		}
		
		//HPF CNG
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\HPF_OperatingMode" );
		temp.Type = LONG_ITEM_TYPE;
		temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.HPF_OperatingMode;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\HPF_CornerFreq" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.HPF_CornerFreq;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\CNG_OperatingMode" );
		temp.Type = LONG_ITEM_TYPE;
		temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.CNG_OperatingMode;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\CNG_ControlParams\\CNG_Volume" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.CNG_ControlParams.CNG_Volume;
		lpvValues.push_back( temp );
		
		//wm
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\WM_ControlParams\\OperatingMode" );
		temp.Type = LONG_ITEM_TYPE;
		temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.OperatingMode;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\WM_ControlParams\\mode" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.mode;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\WM_ControlParams\\AVL_Target_level_lin" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Target_level_lin;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\WM_ControlParams\\AVL_MinGainLin" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_MinGainLin;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\WM_ControlParams\\AVL_MaxGainLin" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_MaxGainLin;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\WM_ControlParams\\AVL_Attack" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Attack;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\WM_ControlParams\\AVL_Release" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Release;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\WM_ControlParams\\AVL_Limit_MaxOutputLin" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.AVL_Limit_MaxOutputLin;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\WM_ControlParams\\SpDetect_Threshold" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.WM_ControlParams.SpDetect_Threshold;
		lpvValues.push_back( temp );
		
		if(dwLvveType != LVVE_SINGLE_MIC)
		{
			//NG
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\NG_ControlParams\\OperatingMode" );
			temp.Type = LONG_ITEM_TYPE;
			temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.OperatingMode;
			lpvValues.push_back( temp );
			
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\NG_ControlParams\\NumKnees" );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.NumKnees;
			lpvValues.push_back( temp );
			
			for(int i = 0; i < LVMDRC_MAX_KNEES; i++)
			{
				memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
				sprintf( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\NG_ControlParams\\CompressorCurveInputLevels\\CompressorCurveInputLevels[%d]", i );
				temp.Type = SHORT_ITEM_TYPE;
				temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.CompressorCurveInputLevels[i];
				lpvValues.push_back( temp );
			}
			
			for(i = 0; i < LVMDRC_MAX_KNEES; i++)
			{
				memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
				sprintf( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\NG_ControlParams\\CompressorCurveOutputLevels\\CompressorCurveOutputLevels[%d]", i );
				temp.Type = SHORT_ITEM_TYPE;
				temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.CompressorCurveOutputLevels[i];
				lpvValues.push_back( temp );
			}
			
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\NG_ControlParams\\AttackTime" );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.AttackTime;
			lpvValues.push_back( temp );
			
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\NG_ControlParams\\ReleaseTime" );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.ReleaseTime;
			lpvValues.push_back( temp );
			
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\NG_ControlParams\\LimiterOperatingMode" );
			temp.Type = LONG_ITEM_TYPE;
			temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.LimiterOperatingMode;
			lpvValues.push_back( temp );
			
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Rx\\NG_ControlParams\\LimitLevel" );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Rx.NG_ControlParams.LimitLevel;
			lpvValues.push_back( temp );
		}
	}

	//RxEqCoefs
	for(int i = 0; i < lvve_data.lvve_ctrl_param_t.ControlParams_Rx.EQ_ControlParams.EQ_Length; i ++)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"lvve_ctrl_params\\RxEqCoefs\\RxEqCoefs[%d]",i );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.RxEqCoefs[i];
		lpvValues.push_back( temp );
	}

	//TX
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\OperatingMode" );
    temp.Type = LONG_ITEM_TYPE;
    temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.OperatingMode;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\Mute" );
    temp.Type = LONG_ITEM_TYPE;
    temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.Mute;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\BD_OperatingMode" );
    temp.Type = LONG_ITEM_TYPE;
    temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BD_OperatingMode;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\BulkDelay" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.BulkDelay;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\VOL_OperatingMode" );
    temp.Type = LONG_ITEM_TYPE;
    temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_OperatingMode;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\VOL_Gain" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.VOL_Gain;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HPF_OperatingMode" );
    temp.Type = LONG_ITEM_TYPE;
    temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HPF_OperatingMode;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\MIC_HPF_CornerFreq" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.MIC_HPF_CornerFreq;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\OperatingMode" );
    temp.Type = LONG_ITEM_TYPE;
    temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.OperatingMode;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\Mode" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.Mode;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\TuningMode" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.TuningMode;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\InputGain" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.InputGain;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\OutputGain" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.OutputGain;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\NLMS_limit" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_limit;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\NLMS_LB_taps" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_LB_taps;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\NLMS_LB_two_alpha" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_LB_two_alpha;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\NLMS_LB_erl" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_LB_erl;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\NLMS_HB_taps" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_HB_taps;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\NLMS_HB_two_alpha" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_HB_two_alpha;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\NLMS_HB_erl" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_HB_erl;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\NLMS_preset_coefs" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_preset_coefs;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\NLMS_offset" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.NLMS_offset;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\DENS_tail_alpha" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_tail_alpha;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\DENS_tail_portion" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_tail_portion;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\DENS_gamma_e_high" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_gamma_e_high;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\DENS_gamma_e_dt" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_gamma_e_dt;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\DENS_gamma_e_low" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_gamma_e_low;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\DENS_gamma_e_alpha" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_gamma_e_alpha;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\DENS_gamma_n" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_gamma_n;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\DENS_limit_ns" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_limit_ns;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\DENS_CNI_Gain" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_CNI_Gain;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\DENS_NL_atten" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_NL_atten;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\DENS_spdet_near" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_spdet_near;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\DENS_spdet_act" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_spdet_act;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\DENS_NFE_blocksize" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.DENS_NFE_blocksize;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\SPDET_far" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.SPDET_far;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\SPDET_mic" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.SPDET_mic;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\SPDET_x_clip" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.SPDET_x_clip;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\PCD_threshold" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_threshold;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\PCD_taps" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_taps;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\PCD_erl" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_erl;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\PCD_minimum_erl" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_minimum_erl;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\PCD_erl_step" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_erl_step;
    lpvValues.push_back( temp );
	
	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\HF_ControlParams\\PCD_gamma_e_rescue" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.HF_ControlParams.PCD_gamma_e_rescue;
    lpvValues.push_back( temp );

	if(dwLvveType == LVVE_DUAL_MIC)
	{
		//nv
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\OperatingMode" );
		temp.Type = LONG_ITEM_TYPE;
		temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.OperatingMode;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\Mode" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Mode;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\Mode2" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Mode2;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\Tuning_mode" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Tuning_mode;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\Input_Gain_Mic0" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Input_Gain_Mic0;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\Input_Gain_Mic1" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Input_Gain_Mic1;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\Output_Gain" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.Output_Gain;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\NLMS0_LB_taps" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_LB_taps;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\NLMS0_LB_twoalpha" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_LB_twoalpha;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\NLMS0_LB_erl" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_LB_erl;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\NLMS0_HB_taps" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_HB_taps;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\NLMS0_HB_twoalpha" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_HB_twoalpha;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\NLMS0_HB_erl" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_HB_erl;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\NLMS0_preset_coefs" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_preset_coefs;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\NLMS0_offset" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS0_offset;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\NLMS1_LB_taps" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_LB_taps;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\NLMS1_LB_twoalpha" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_LB_twoalpha;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\NLMS1_LB_erl" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_LB_erl;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\NLMS1_HB_taps" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_HB_taps;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\NLMS1_HB_twoalpha" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_HB_twoalpha;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\NLMS1_HB_erl" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_HB_erl;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\NLMS1_preset_coefs" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_preset_coefs;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\NLMS1_offset" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.NLMS1_offset;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\CAL_micPowFloorMin" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.CAL_micPowFloorMin;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\WgThreshold" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.WgThreshold;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\MpThreshold" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.MpThreshold;
		lpvValues.push_back( temp );
		
		for(i = 0; i < 8; i++)
		{
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			sprintf( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\FSB_init_table0\\FSB_init_table0[%d]", i );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.FSB_init_table0[i];
			lpvValues.push_back( temp );
		}
		
		for(i = 0; i < 8; i++)
		{
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			sprintf( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\FSB_init_table1\\FSB_init_table1[%d]", i );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.FSB_init_table1[i];
			lpvValues.push_back( temp );
		}
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\FSB_taps" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.FSB_taps;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\FSB_twoalpha" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.FSB_twoalpha;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\FSB_ref_gain" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.FSB_ref_gain;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\GSC_taps" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.GSC_taps;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\GSC_twoalpha" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.GSC_twoalpha;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\GSC_erl" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.GSC_erl;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\GSC_offset" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.GSC_offset;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\DNNS_EchoGammaHi" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_EchoGammaHi;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\DNNS_EchoGammaLo" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_EchoGammaLo;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\DNNS_EchoAlphaRev" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_EchoAlphaRev;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\DNNS_EchoTailPortion" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_EchoTailPortion;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\DNNS_NlAtten" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NlAtten;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\DNNS_NoiseGammaS" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NoiseGammaS;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\DNNS_NoiseGammaN" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NoiseGammaN;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\DNNS_NoiseGainMinS" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NoiseGainMinS;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\DNNS_NoiseGainMinN" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NoiseGainMinN;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\DNNS_NoiseBiasComp" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_NoiseBiasComp;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\DNNS_GainEta" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_GainEta;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\DNNS_AcThreshold" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_AcThreshold;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\DNNS_WbThreshold" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_WbThreshold;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\DNNS_LostBeamThreshold" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.DNNS_LostBeamThreshold;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\PCD_beta" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.PCD_beta;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\NV_ControlParams\\PCD_Threshold" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.NV_ControlParams.PCD_Threshold;
		lpvValues.push_back( temp );
		//
	}




	if(dwLvveType != LVVE_JUNIPER)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\WM_ControlParams\\OperatingMode" );
		temp.Type = LONG_ITEM_TYPE;
		temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.OperatingMode;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\WM_ControlParams\\mode" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.mode;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\WM_ControlParams\\AVL_Target_level_lin" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Target_level_lin;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\WM_ControlParams\\AVL_MinGainLin" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_MinGainLin;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\WM_ControlParams\\AVL_MaxGainLin" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_MaxGainLin;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\WM_ControlParams\\AVL_Attack" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Attack;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\WM_ControlParams\\AVL_Release" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Release;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\WM_ControlParams\\AVL_Limit_MaxOutputLin" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.AVL_Limit_MaxOutputLin;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\WM_ControlParams\\SpDetect_Threshold" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.WM_ControlParams.SpDetect_Threshold;
		lpvValues.push_back( temp );
	}

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\EQ_OperatingMode" );
    temp.Type = LONG_ITEM_TYPE;
    temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_OperatingMode;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\EQ_ControlParams\\EQ_Length" );
    temp.Type = SHORT_ITEM_TYPE;
    temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams.EQ_Length;
    lpvValues.push_back( temp );

	memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
    strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\EQ_ControlParams\\pEQ_Coefs" );
    temp.Type = LONG_ITEM_TYPE;
    temp.Value = 0;
    lpvValues.push_back( temp );

	if(dwLvveType == LVVE_SINGLE_MIC || dwLvveType == LVVE_DUAL_MIC)
	{
		//drc
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\DRC_ControlParams\\OperatingMode" );
		temp.Type = LONG_ITEM_TYPE;
		temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.OperatingMode;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\DRC_ControlParams\\NumKnees" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.NumKnees;
		lpvValues.push_back( temp );
		
		for(i = 0; i < LVMDRC_MAX_KNEES; i++)
		{
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			sprintf( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\DRC_ControlParams\\CompressorCurveInputLevels\\CompressorCurveInputLevels[%d]", i );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.CompressorCurveInputLevels[i];
			lpvValues.push_back( temp );
		}
		
		for(i = 0; i < LVMDRC_MAX_KNEES; i++)
		{
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			sprintf( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\DRC_ControlParams\\CompressorCurveOutputLevels\\CompressorCurveOutputLevels[%d]", i );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.CompressorCurveOutputLevels[i];
			lpvValues.push_back( temp );
		}
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\DRC_ControlParams\\AttackTime" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.AttackTime;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\DRC_ControlParams\\ReleaseTime" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.ReleaseTime;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\DRC_ControlParams\\LimiterOperatingMode" );
		temp.Type = LONG_ITEM_TYPE;
		temp.Value = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.LimiterOperatingMode;
		lpvValues.push_back( temp );
		
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		strcpy( temp.Name,"lvve_ctrl_params\\ControlParams_Tx\\DRC_ControlParams\\LimitLevel" );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.ControlParams_Tx.DRC_ControlParams.LimitLevel;
		lpvValues.push_back( temp );
	}

	//TxEqCoefs
	for(i = 0; i < lvve_data.lvve_ctrl_param_t.ControlParams_Tx.EQ_ControlParams.EQ_Length; i ++)
	{
		memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
		sprintf( temp.Name,"lvve_ctrl_params\\TxEqCoefs\\RxEqCoefs[%d]",i );
		temp.Type = SHORT_ITEM_TYPE;
		temp.Reserved = lvve_data.lvve_ctrl_param_t.TxEqCoefs[i];
		lpvValues.push_back( temp );
	}

	if(dwLvveType != LVVE_NO_RESERVE)
	{
		//reserve
		for(i = 0; i < LVVE_CONFIG_RESERVE_LEN; i ++)
		{
			memset( &temp,0,NVVALUE_TABLE_ENTRY_SIZE );
			sprintf( temp.Name,"lvve_reserve\\lvve_reserve[%d]",i );
			temp.Type = SHORT_ITEM_TYPE;
			temp.Reserved = lvve_data.lvve_reserve[i];
			lpvValues.push_back( temp );
		}
	}	
}

/*
void CAudioTesterSheet::Export()
{
    CString strFilter;
    strFilter.LoadString( IDS_FILTER_NVV );
    CFileDialog dlg( FALSE,
        _T(".nv"),
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        strFilter,
        this );
    if( IDOK != dlg.DoModal() )
    {
        return;
    }
    
    FetchValue( m_AudioData );

    std::vector<NVVALUE_SECTION_ENTRY> vSections;
    std::vector<NVVALUE_TABLE_ENTRY> vValues;
    CStringArray arrDirs;

    int nValueSize = 0;
    int nDirOffset = 0;

    // Get value of every mode
    for( int i=0;i<m_AudioData.mode_count;i++ )
    {
        ::Export( m_AudioData.lpAudioMode[i].audio_data,vValues,m_bExtendData,m_bRocky,m_bAudio3 );

        CString strMode = "audio\\audio\\";
        strMode += m_AudioData.lpAudioMode[i].mode_name;
        strMode += "\\AudioStructure";
        arrDirs.Add( strMode );
        
        NVVALUE_SECTION_ENTRY section;
        section.OffsetOfValueTable = (WORD)(sizeof (NVVALUE_FILE_HEADER) + nValueSize * NVVALUE_TABLE_ENTRY_SIZE + m_AudioData.mode_count * NVVALUE_SECTION_ENTRY_SIZE);
        section.LongOffsetOfValueTable = sizeof (NVVALUE_FILE_HEADER) + nValueSize * NVVALUE_TABLE_ENTRY_SIZE + m_AudioData.mode_count * NVVALUE_SECTION_ENTRY_SIZE;
        section.NumberOfValueEntries = (WORD)(vValues.size() - nValueSize);
        nValueSize = vValues.size();
        section.OffsetOfDirectoryTable = (WORD)(sizeof (NVVALUE_FILE_HEADER) + nDirOffset  + m_AudioData.mode_count * NVVALUE_SECTION_ENTRY_SIZE);
        section.LongOffsetOfDirectoryTable = sizeof (NVVALUE_FILE_HEADER) + nDirOffset + m_AudioData.mode_count * NVVALUE_SECTION_ENTRY_SIZE;
        section.NumberOfDirectories = 1;

        nDirOffset += strMode.GetLength() + 1;

        vSections.push_back( section );        
    }

    // Modify the offset of dir table and value table
    for( i=0;i<m_AudioData.mode_count;i++ )
    {
        vSections[i].LongOffsetOfDirectoryTable += nValueSize * NVVALUE_TABLE_ENTRY_SIZE;
    }

    CFile fSaveFile( dlg.GetFileName(), CFile::modeWrite|CFile::modeCreate );
    // Write file head.
    NVVALUE_FILE_HEADER header;  
    ZeroMemory( &header, sizeof header ); 
    header.Magic = NVVALUE_FILE_SIGNATURE;
    header.Version = VERSION_2;
    header.TimeDateStamp = m_structFileHead.m_Datetime.GetTime();     
    memcpy( header.Machine,
        m_structFileHead.m_strProducer.GetBuffer( MAX_MACHINE_NAME_LEN ),
        m_structFileHead.m_strProducer.GetLength() );
    header.Machine[m_structFileHead.m_strProducer.GetLength()] = '\0';       
    header.NumberOfSections = vSections.size();             
    fSaveFile.Write( &header, sizeof (header) );  
    
    // Write section table
    for( UINT j=0;j<vSections.size();j++ )
    {
        fSaveFile.Write( &(vSections[j]),NVVALUE_SECTION_ENTRY_SIZE );
    }

    // Write nv value table
    for( j=0;j<vValues.size();j++ )
    {
        fSaveFile.Write( &(vValues[j]),NVVALUE_TABLE_ENTRY_SIZE );
    }

    // Write directory table
    for( i=0;i<arrDirs.GetSize();i++ )
    {
        fSaveFile.Write( arrDirs[i].GetBuffer(0),arrDirs[i].GetLength()+1 );
    }

    fSaveFile.Close();
}*/

void CAudioTesterSheet::Export( BOOL bCallDataOnly )
{
    CString strFilter;
    strFilter.LoadString( IDS_FILTER_NVV );
    CFileDialog dlg( FALSE,
        _T(".nv"),
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        strFilter,
        this );
    if( IDOK != dlg.DoModal() )
    {
        return;
    }

	CStdioFile fSaveFile( dlg.GetFileName(), CFile::modeCreate|CFile::modeWrite|CFile::typeText );
    // Write file verion.
	//teana hu 2012.04.10
	CString strVersion;
	CXVerInfo verInfo;
	m_strVersion = verInfo.GetSPRDVersionString();
	strVersion.Format(_T("// Export by version: %s\n\n"), m_strVersion);
	fSaveFile.WriteString( strVersion ); 
	//

	//teana hu 2010.06.10
	
	if(m_bLoadEQ && !m_bLoadVolume)
	{
		strVersion = _T("// Spreadtrum AudioTester export file version 0x0300\n\n");
	}
	else if(!m_bLoadEQ && m_bLoadVolume)
	{
		strVersion = _T("// Spreadtrum AudioTester export file version 0x0400\n\n");
	}
	else if(m_bLoadEQ && m_bLoadVolume)
	{
		strVersion = _T("// Spreadtrum AudioTester export file version 0x0410\n\n");
	}
	else
	{
		strVersion = _T("// Spreadtrum AudioTester export file version 0x0200\n\n");
	}

    fSaveFile.WriteString( strVersion );  

	//teana hu 2012.06.04
	if(m_bLoadENHA)
	{
		strVersion = _T("// Support ENHA\n\n");
	}
	//

	if( AUDIO_ARM_APP_TYPE_MIN == m_nArmSerialCount )
	{
		strVersion = _T("// audio_arm 8 serial\n\n");
	}
	else if( AUDIO_ARM_APP_TYPE_ONE == m_nArmSerialCount )
	{
		strVersion = _T("// audio_arm 1 serial\n\n");
	}
	else if(AUDIO_ARM_APP_TYPE_SIX == m_nArmSerialCount)
	{
		strVersion = _T("// audio_arm 6 serial\n\n");
	}
	
	fSaveFile.WriteString( strVersion );

	//teana hu 2012.01.11
	if(m_dwLvveType == LVVE_NO_RESERVE)
	{
		strVersion = _T("// nxp structrue no reserve\n\n");
	}
	else if(m_dwLvveType == LVVE_RESERVE)
	{
		strVersion = _T("// nxp structrue reserve\n\n");
	}
	else if(m_dwLvveType == LVVE_JUNIPER)
	{
		strVersion = _T("// nxp structrue juniper\n\n");
	}
	else if(m_dwLvveType == LVVE_SINGLE_MIC)
	{
		strVersion = _T("// nxp structrue single mic\n\n");
	}
	else
	{
		strVersion = _T("// nxp structrue dual mic\n\n");
	}
	fSaveFile.WriteString( strVersion );
	//
    
    FetchValue( m_AudioData );

    std::vector<NVVALUE_TABLE_ENTRY> vValues;

    // Get value of every mode
    for( int i=0;i<m_AudioData.mode_count;i++ )
    {
        ::Export( m_AudioData.lpAudioMode[i].audio_data,vValues,m_bExtendData,m_bRocky,m_bAudio3 );

        CString strMode = "audio\\audio\\";
        strMode += m_AudioData.lpAudioMode[i].mode_name;
        strMode += "\\AudioStructure";
        
		CString strContent;
		for(int j = 0; j < vValues.size(); j++)
		{
#ifdef _UNICODE
			TCHAR szName[1024] = {0};
			MultiByteToWideChar(CP_ACP,0,vValues[j].Name,-1,szName,1024);
			strContent.Format(_T("%s\\%s=0x%x\n"), strMode, szName, (WORD)vValues[j].Value);
#else
			strContent.Format(_T("%s\\%s=0x%x\n"), strMode, vValues[j].Name, (WORD)vValues[j].Value);
#endif
			fSaveFile.WriteString( strContent); 
		}
    
        vValues.clear();  
    }

	if( !bCallDataOnly )
	{
		//teana hu 2010.06.10
		if(m_bLoadEQ)
		{
			for( i=0;i<m_EQData.eq_count;i++ )
			{
				::ExportEQ( m_EQData.lpEQMode[i],( eq_platform_all == m_EQData.eq_platform ),vValues );
				
				CString strMode = "audio\\";
				strMode += m_EQData.lpEQMode[i].ucParaSetName;
				
				CString strContent;
				for(int j = 0; j < vValues.size(); j++)
				{
#ifdef _UNICODE
					TCHAR szName[1024] = {0};
					MultiByteToWideChar(CP_ACP,0,vValues[j].Name,-1,szName,1024);
					strContent.Format(_T("%s\\%s=0x%x\n"), strMode, szName, vValues[j].Reserved);
#else
					strContent.Format(_T("%s\\%s=0x%x\n"), strMode, vValues[j].Name, vValues[j].Reserved);
#endif
					fSaveFile.WriteString( strContent); 
				}
				
				vValues.clear();  
			}
		}
		//

		//teana hu 2011.02.18
		if(m_bLoadVolume)
		{
			for( int i=0;i<m_VolumeData.mode_count;i++ )
			{
				::ExportVolume( m_VolumeData.lpVolumeMode[i].volume_data,vValues, m_nArmSerialCount );
				
				CString strMode;
				if(m_bExportOldNv)
				{
					strMode = "audio\\audio_arm\\";
				}
				else
				{
					strMode = "audio_arm\\audio_arm\\";
				}
				
				strMode += m_VolumeData.lpVolumeMode[i].mode_name;
				strMode += "\\AudioStructure";
				
				CString strContent;
				for(int j = 0; j < vValues.size(); j++)
				{
#ifdef _UNICODE
					TCHAR szName[1024] = {0};
					MultiByteToWideChar(CP_ACP,0,vValues[j].Name,-1,szName,1024);
					if(vValues[j].Type == LONG_ITEM_TYPE)
					{
						strContent.Format(_T("%s\\%s=0x%x\n"), strMode, szName, (DWORD)vValues[j].Value);
					}
					else if(vValues[j].Type == SHORT_ITEM_TYPE)
					{
						strContent.Format(_T("%s\\%s=0x%x\n"), strMode, szName, (WORD)vValues[j].Reserved);
					}
#else
					if(vValues[j].Type == LONG_ITEM_TYPE)
					{
						strContent.Format(_T("%s\\%s=0x%x\n"), strMode, vValues[j].Name, (DWORD)vValues[j].Value);
					}
					else if(vValues[j].Type == SHORT_ITEM_TYPE)
					{
						strContent.Format(_T("%s\\%s=0x%x\n"), strMode, vValues[j].Name, (WORD)vValues[j].Reserved);
					}
#endif
					fSaveFile.WriteString( strContent); 
				}
				vValues.clear();
			}
		}
		//
	}

	//teana hu 2011.04.19
	if(m_bLoadLVVE)
	{
		for( int i=0;i<m_LVVEPara.mode_count;i++ )
		{
			::ExportLVVE( m_LVVEPara.lp_lvve_ctrl_nv_params[i],vValues, m_dwLvveType );

			CString strMode;
			if(m_bExportOldNv)
			{
				strMode = "audio\\lvve_params\\";
			}
			else
			{
				strMode = "lvve_params\\lvve_params\\";
			}
			
			strMode += m_LVVEPara.lp_lvve_ctrl_nv_params[i].ucModeName;
			
			CString strContent;
			for(int j = 0; j < vValues.size(); j++)
			{
#ifdef _UNICODE
				TCHAR szName[1024] = {0};
				MultiByteToWideChar(CP_ACP,0,vValues[j].Name,-1,szName,1024);
				if(vValues[j].Type == LONG_ITEM_TYPE)
				{
					strContent.Format(_T("%s\\%s=0x%x\n"), strMode, szName, (DWORD)vValues[j].Value);
				}
				else if(vValues[j].Type == SHORT_ITEM_TYPE)
				{
					strContent.Format(_T("%s\\%s=0x%x\n"), strMode, szName, (WORD)vValues[j].Reserved);
				}
#else
				if(vValues[j].Type == LONG_ITEM_TYPE)
				{
					strContent.Format(_T("%s\\%s=0x%x\n"), strMode, vValues[j].Name, (DWORD)vValues[j].Value);
				}
				else if(vValues[j].Type == SHORT_ITEM_TYPE)
				{
					strContent.Format(_T("%s\\%s=0x%x\n"), strMode, vValues[j].Name, (WORD)vValues[j].Reserved);
				}
#endif
				fSaveFile.WriteString( strContent); 
			}
			vValues.clear();
		}
	}
	//

	//teana hu 2012.01.31
	if(m_bLoadDspCodec)
	{
		CString strContent;

		strContent.Format(_T("//audio_dsp_codec\\dsp_codec_info=0x%0x\n"), m_nDspCodecInfo);

		fSaveFile.WriteString( strContent); 

		for( int i=0;i<m_DspModePara.nModeCount;i++ )
		{
			::ExportDspCodec( m_DspModePara.lp_audio_nv_dsp_mode_info[i].tAudioNvDspModeStruct,vValues);
			
			CString strMode = "audio_dsp_codec\\dsp_codec_config\\";
			strMode += m_DspModePara.lp_audio_nv_dsp_mode_info[i].ucModeName;
			strMode += "\\AudioStructure";
			
			CString strContent;
			for(int j = 0; j < vValues.size(); j++)
			{
#ifdef _UNICODE
				TCHAR szName[1024] = {0};
				MultiByteToWideChar(CP_ACP,0,vValues[j].Name,-1,szName,1024);
				strContent.Format(_T("%s\\%s=0x%x\n"), strMode, szName, (WORD)vValues[j].Reserved);
#else
				strContent.Format(_T("%s\\%s=0x%x\n"), strMode, vValues[j].Name, (WORD)vValues[j].Reserved);
#endif
				fSaveFile.WriteString( strContent); 
			}
				vValues.clear();
		}
	}
	//

    fSaveFile.Close();
}

BOOL CAudioTesterSheet::Import()
{
    CString strFilter;
    strFilter.LoadString( IDS_FILTER_NVV );
    CAudioFileOpenDlg dlg(TRUE,_T(".nv"),NULL,OFN_HIDEREADONLY,strFilter,this);
	BOOL bImport = FALSE;
    if( IDOK == dlg.DoModal() )
    {
        CWaitCursor temp;
        CString strFileName = dlg.GetFileName(); 	

		FILE *fpRead = NULL;
#ifdef _UNICODE
		fpRead = _wfopen(strFileName, _T("r"));
#else
		fpRead = fopen(strFileName, "r");
#endif		
		if(fpRead == NULL)
		{
            return FALSE;
		}

		BOOL bUsedMusic = FALSE;
		char szLine[1024] = {0};

		for(int i = 0; i < 10; i++)
		{
			fgets(szLine, 1024, fpRead);
			if(strstr(szLine, "music") != NULL)
			{
				bUsedMusic = TRUE;
				break;
			}
		}	

		fclose(fpRead);
		fpRead = NULL;

		if(bUsedMusic)
		{
			if(m_bLoad)
			{
				ClearAllModes();
			}
			
			if(!ConfirmLoadMusic())
			{
				return FALSE;
			}
			bImport = ImportMusic(strFileName);
		}
		else
		{
			if(m_bLoadENHA)
			{
				ClearAllMusicModes();
			}
			
			if(!ConfirmLoad())
			{
				return FALSE;
			}
			bImport = ImportAudio(strFileName);
		}
	}

	return bImport;

}

BOOL CAudioTesterSheet::ImportAudio(CString strFileName)
{
	char szLine[1024] = {0};
	char szLastModeName[128] = {0};
	char szTmp[128] = {0};
	char szModeName[128] = {0};
	short wValue = 0;
	LONG lValue = 0;
	DWORD dwSize = 0;
	BYTE byteAudioStruct[sizeof(AUDIO_STRUCT)] = {0};
	int nEQLenth = sizeof(short)+sizeof(EQ_MODE_NV_PARAM_T)*TOTAL_EQ_MODE_NUM;
	BYTE byteEQStruct[sizeof(short)+sizeof(EQ_MODE_NV_PARAM_T)*TOTAL_EQ_MODE_NUM] = {0};
	BYTE byteVolumeStruct_8[sizeof(VOLUME_STRUCT_MIN)] = {0};
	BYTE byteVolumeStruct_1[sizeof(VOLUME_STRUCT_ONE)] = {0};
	//teana hu 2012.01.30
	BYTE byteVolumeStruct_6[sizeof(VOLUME_STRUCT_SIX)] = {0};
	BYTE byteVolumeStruct_16[sizeof(VOLUME_STRUCT_SIXTEEN)] = {0};
	VOLUME_STRUCT_SIX volume_data_6;
	VOLUME_STRUCT_SIXTEEN volume_data_16;
	//
	BYTE byteLvveStruct[sizeof(LVVE_CTRL_PARAM_T)+sizeof(short)*LVVE_CONFIG_RESERVE_LEN] = {0};
	BYTE byteLvveStructMin[sizeof(LVVE_CTRL_PARAM_1_T)] = {0};
	//teana hu 2012.01.12
	BYTE byteLvveStructReserve[sizeof(LVVE_CTRL_PARAM_1_T)+sizeof(short)*LVVE_CONFIG_RESERVE_LEN] = {0};
	BYTE byteLvveStructJuniper[sizeof(LVVE_CTRL_PARAM_2_T)+sizeof(short)*LVVE_CONFIG_RESERVE_LEN] = {0};
	BYTE byteLvveStructSingleMic[sizeof(LVVE_CTRL_PARAM_3_T)+sizeof(short)*LVVE_CONFIG_RESERVE_LEN] = {0};
	//
	int nLvveOffset[336] = {0};//
	int nLvveOffsetMin[146] = {0};//190+146
	//teana hu 2012.01.11
	int nLvveOffsetReserve[210] = {0};//210+126
	int nLvveOffsetJuniper[185] = {0};//185+151
	int nLvveOffsetSingleMic[239] = {0};//239+97
	//
	BOOL bFirst = TRUE;
	VOLUME_STRUCT_MIN volume_data_8;
	VOLUME_STRUCT_ONE volume_data_1;

	//teana hu 2012.02.01
	BYTE byteDspCodecStruct[sizeof(AUDIO_NV_DSP_MODE_STRUCT_T)] = {0};
	char szLastDspCodecName[128] = {0};
	char szDspCodecName[128] = {0};
	//
	
	//teana hu 2010.06.10
	char szLastEQName[128] = {0};
	char szEQName[128] = {0};
	
	//teana hu 2011.02.18
	char szLastVolumeName[128] = {0};
	char szVolumeName[128] = {0};
	//

	//teana hu 2011.04.19
	char szLastLvveName[128] = {0};
	char szLvveName[128] = {0};
	//

	for(int k = 0; k < 210; k++)
	{
		if(k == 0 || k== 1 || k==2 || k==4 || k==7
			|| k == 10 || k== 23 || k==25 || k==58 || k==59
			|| k == 60 || k== 62 || k==64 || k==66 || k==102
			|| k == 111 || k== 113 || k == 3 || k == 24 || k == 61
			|| k == 63 || k == 65 || k == 101 || k == 112)
		{
			nLvveOffsetReserve[k] = 4;
		}
		else
		{
			nLvveOffsetReserve[k] = 2;
		}
	}
	
	for(k = 0; k < 146; k++)
	{
		if(k == 0 || k== 1 || k==2 || k==4 || k==7
			|| k == 10 || k== 23 || k==25 || k==58 || k==59
			|| k == 60 || k== 62 || k==64 || k==66 || k==102
			|| k == 111 || k== 113 || k == 3 || k == 24 || k == 61
			|| k == 63 || k == 65 || k == 101 || k == 112)
		{
			nLvveOffsetMin[k] = 4;
		}
		else
		{
			nLvveOffsetMin[k] = 2;
		}
	}

	//teana hu 2012.01.12
	for(k = 0; k < 185; k++)
	{
		if(k == 0 || k== 1 || k==2 || k==3 || k==4
			|| k == 7 || k== 42 || k==43 || k==44 || k==45
			|| k == 46 || k== 47 || k==48 || k==49 || k==50
			|| k == 85 || k== 86)
		{
			nLvveOffsetJuniper[k] = 4;
		}
		else
		{
			nLvveOffsetJuniper[k] = 2;
		}
	}

	for(k = 0; k < 239; k++)
	{
		if(k == 0 || k== 1 || k==2 || k==3 || k==4
			|| k == 7 || k== 10 || k==23 || k==26 || k==27
			|| k == 28 || k== 29 || k==30 || k==71 || k==72
			|| k == 73 || k== 74 || k==75 || k==76 || k==77
			|| k==78 || k==79 || k==114 || k==115 || k==124
			|| k==127 || k==140 || k== 141 || k==142)
		{
			nLvveOffsetSingleMic[k] = 4;
		}
		else if(k == 25 || k == 126)
		{
			nLvveOffsetSingleMic[k] = 6;
		}
		else
		{
			nLvveOffsetSingleMic[k] = 2;
		}
	}

	for(k = 0; k < 336; k++)
	{
		if(k == 0 || k== 1 || k==2 || k==3 || k==4
			|| k == 7 || k== 10 || k==23 || k==26 || k==39
			|| k == 40 || k== 41 || k==42 || k==43 || k==44
			|| k == 45 || k== 46 || k==55 || k==68 || k==69
			/*|| k==102*/ || k==103 || k==104 || k==105 || k==106
			|| k==107 || k==108 || k== 109 || k==110 || k==111
			|| k==146 || k==147 || k==212 || k==221 || k==224
			|| k== 237 || k==238 /*|| k==335*/ || k==70 || k==239)
		{
			nLvveOffset[k] = 4;
		}
		else if(k == 25 || k == 223)
		{
			nLvveOffset[k] = 6;
		}
		else
		{
			nLvveOffset[k] = 2;
		}
	}
	//

	int nArmOffset[180] = {0};

	for(k =0; k < 180; k++)
	{
		if(k== 24 || k == 25
			|| k == 26 || k == 27 || k == 28 || k == 29
			|| k == 30 || k == 31 || k == 32 || k == 33  
			|| k == 34 || k == 35 || k == 36 || k == 37
			|| k == 38 || k == 39 || k == 40 || k == 41
			|| k == 42 || k == 43 || k == 44 || k == 45
			|| k == 46 || k == 47 || k == 48 || k == 49
			|| k == 50 || k == 51 || k == 52 || k == 53 
			|| k == 54
			|| k == 74 || k == 75 || k == 76 || k== 77 || k == 78
			|| k == 79 || k == 80 || k == 81 || k == 82
			|| k == 83 || k == 84 || k == 85 || k == 86
			|| k == 87 || k == 88 || k == 89 || k == 90
			|| k == 91 || k == 92 || k == 93 || k == 94
			|| k == 95 || k == 96 || k == 97 || k == 98
			|| k == 99 || k == 100 || k == 101 || k == 102
			|| k == 103 || k == 104 || k == 63 || k == 113)
		{
			nArmOffset[k] = 4;
		}
		else
		{
			nArmOffset[k] = 2;
		}
	}

	int nIndexArm = 0;
	
	FILE *fpRead = NULL;
#ifdef _UNICODE
	fpRead = _wfopen(strFileName, _T("r"));
#else
	fpRead = fopen(strFileName, "r");
#endif		
	if(fpRead == NULL)
	{
		m_bLoad = FALSE;
		return m_bLoad;
	}
	
	m_bExtendData = FALSE;
	m_bRocky = FALSE;
	m_bAudio3 = FALSE;
	m_AudioData.mode_count = 0;
	m_EQData.eq_count = 0;
	m_EQData.eq_platform = eq_platform_6800_only;
	m_VolumeData.mode_count = 0;
	m_LVVEPara.mode_count = 0;

	m_nArmSerialCount = AUDIO_ARM_APP_TYPE_MAX;	
	
	//get mode count
	while(!feof(fpRead))
	{
		if(fgets(szLine, 1024, fpRead) != NULL)
		{
			if(strstr(szLine, "ModeName\\") != NULL)
			{
				continue;
			}

			//teana hu 2012.06.04
			if(strstr(szLine, "// Support ENHA") != NULL)
			{
				m_bLoadENHA = TRUE;
			}
			//

			if(strstr(szLine, "// audio_arm 8 serial") != NULL)
			{
				m_nArmSerialCount = AUDIO_ARM_APP_TYPE_MIN;
			}
			else if( strstr( szLine, "// audio_arm 1 serial") != NULL)
			{
				m_nArmSerialCount = AUDIO_ARM_APP_TYPE_ONE;
			}
			else if( strstr( szLine, "// audio_arm 6 serial") != NULL)
			{
				m_nArmSerialCount = AUDIO_ARM_APP_TYPE_SIX;
			}

			//teana hu 2012.01.11
			if(strstr(szLine, "// nxp structrue no reserve") != NULL)
			{
				m_dwLvveType = LVVE_NO_RESERVE;
			}
			else if(strstr(szLine, "// nxp structrue reserve") != NULL)
			{
				m_dwLvveType = LVVE_RESERVE;
			}
			else if(strstr(szLine, "// nxp structrue juniper") != NULL)
			{
				m_dwLvveType = LVVE_JUNIPER;
			}
			else if(strstr(szLine, "// nxp structrue single mic") != NULL)
			{
				m_dwLvveType = LVVE_SINGLE_MIC;
			}
			else if(strstr(szLine, "// nxp structrue dual mic") != NULL)
			{
				m_dwLvveType = LVVE_DUAL_MIC;
			}
			//

			m_Cmd.SetLVVEType(m_dwLvveType);

			if(strstr(szLine, "audio\\audio\\") != NULL)
			{
				sscanf(szLine, "audio\\audio\\%[a-zA-Z0-9_]%*[\]\\AudioStructure\\", szModeName);
				if(strcmp(szModeName, szLastModeName) != 0)
				{
					memset(&szLastModeName, 0, 128);
					strcpy(szLastModeName, szModeName);
					m_AudioData.mode_count ++;
				}
				m_bLoad = TRUE;
			}
			else if(strstr(szLine, "audio\\EQ_") != NULL)
			{
				CString strTmp = szLine;
				int nFind = strTmp.Find(_T("\\"));
				if(nFind != -1)
				{
					strTmp.Delete(0, nFind + 1);
					nFind = strTmp.Find(_T("\\"));
					if(nFind != -1)
					{
#ifdef _UNICODE
						int nLen = (strTmp.Left(nFind)).GetLength();
						WideCharToMultiByte(CP_ACP,0,strTmp.Left(nFind),-1,szEQName,nLen*2,NULL,NULL);
#else
						strcpy(szEQName, strTmp.Left(nFind));
#endif
					}
				}
				if(strcmp(szEQName, szLastEQName) != 0)
				{
					memset(&szLastEQName, 0, 128);
					strcpy(szLastEQName, szEQName);
					m_EQData.eq_count ++;
				}
				if( strstr( szLine,"extend_array" ) )
				{
					m_EQData.eq_platform = eq_platform_all;
				}
				m_bLoadEQ = TRUE;
			}
			else if(strstr(szLine, "audio\\audio_arm\\") != NULL || strstr(szLine, "audio_arm\\audio_arm\\") != NULL)
			{
				if(strstr(szLine, "audio\\audio_arm\\") != NULL)
				{
					sscanf(szLine, "audio\\audio_arm\\%[a-zA-Z0-9_]%*[\]\\AudioStructure\\", szVolumeName);
				}
				else
				{
					sscanf(szLine, "audio_arm\\audio_arm\\%[a-zA-Z0-9_]%*[\]\\AudioStructure\\", szVolumeName);
				}
				
				if(strcmp(szVolumeName, szLastVolumeName) != 0)
				{
					memset(&szLastVolumeName, 0, 128);
					strcpy(szLastVolumeName, szVolumeName);
					m_VolumeData.mode_count ++;
				}
				m_bLoadVolume = TRUE;
			}
 			else if(strstr(szLine, "audio\\lvve_params\\") != NULL || strstr(szLine, "lvve_params\\lvve_params\\") != NULL)
 			{
				if(strstr(szLine, "audio\\lvve_params\\") != NULL)
				{
					sscanf(szLine, "audio\\lvve_params\\%[a-zA-Z0-9_]%*[\]\\lvve_ctrl_params\\", szLvveName);
				}
				else
				{
					sscanf(szLine, "lvve_params\\lvve_params\\%[a-zA-Z0-9_]%*[\]\\lvve_ctrl_params\\", szLvveName);
				}
 				
 				if(strcmp(szLvveName, szLastLvveName) != 0)
 				{
 					memset(&szLastLvveName, 0, 128);
 					strcpy(szLastLvveName, szLvveName);
 					m_LVVEPara.mode_count ++;
 				}
				m_bLoadLVVE = TRUE;
 			}
			else if(strstr(szLine, "audio_dsp_codec\\dsp_codec_config\\") != NULL)
			{
				sscanf(szLine, "audio_dsp_codec\\dsp_codec_config\\%[a-zA-Z0-9_]%*[\]\\AudioStructure\\", szDspCodecName);
				if(strcmp(szDspCodecName, szLastDspCodecName) != 0)
				{
					memset(&szLastDspCodecName, 0, 128);
					strcpy(szLastDspCodecName, szDspCodecName);
					m_DspModePara.nModeCount ++;
				}
				m_bLoadDspCodec = TRUE;
			}
			else if(strstr(szLine, "//audio_dsp_codec\\dsp_codec_info") != NULL)
			{
				memset(szTmp, 0, 128);
				LPSTR lpFind = strrchr(szLine, '=');
				if(lpFind != NULL)
				{
					strcpy(szTmp, lpFind+1);
				}
				if(strstr(szTmp, "0x") != NULL)
				{
					char* pEnd = NULL;
					m_nDspCodecInfo = (WORD)strtol(szTmp, &pEnd, 16);
				}
				else
				{
					m_nDspCodecInfo = atoi(szTmp);
				}

				if((m_nDspCodecInfo & 0x100) >> 8 == 0)
				{
					m_bDspCodec = TRUE;
				}
				else
				{
					m_bDspCodec = FALSE;
				}
				m_nVolumeLevel_6530 = (m_nDspCodecInfo & 0x3e00) >> 9;
			}
		}
	}
	
	m_AudioData.lpAudioMode = new AUDIO_MODE[m_AudioData.mode_count];
	if(m_bLoadEQ)
	{
		m_EQData.lpEQMode = new AUDIO_EQ_STRUCT_T[m_EQData.eq_count];
	}
	
	if(m_bLoadVolume)
	{
		m_VolumeData.lpVolumeMode = new VOLUME_MODE[m_VolumeData.mode_count];
	}

	if(m_bLoadLVVE)
	{
		m_LVVEPara.lp_lvve_ctrl_nv_params = new LVVE_CTRL_PARAM_NV_T[m_LVVEPara.mode_count];
	}

	if(m_bLoadDspCodec)
	{
		m_DspModePara.lp_audio_nv_dsp_mode_info = new AUDIO_NV_DSP_MODE_INFO_T[m_DspModePara.nModeCount];
	}
	
	fseek(fpRead, 0, SEEK_SET);
	int nIndex = 0;
	BOOL bLastAudio = FALSE;
	BOOL bLastEQ = FALSE;
	BOOL bLastVolume = FALSE;
	BOOL bLastLvve = FALSE;
	int nOffsetIndex = 0;
	
	while(!feof(fpRead))
	{
		if(fgets(szLine, 1024, fpRead) != NULL)
		{
			if(strstr(szLine, "ModeName\\") != NULL)
			{
				continue;
			}
			LPSTR lpFind = strstr(szLine, "audio\\audio\\");
			if(lpFind != NULL)
			{
				sscanf(szLine, "audio\\audio\\%[a-zA-Z0-9_]%*[\]\\AudioStructure\\", szModeName);
				memset(szTmp, 0, 128);
				LPSTR lpFind = strrchr(szLine, '=');
				if(lpFind != NULL)
				{
					strcpy(szTmp, lpFind+1);
				}
				if(strstr(szTmp, "0x") != NULL || strstr(szTmp, "0X") != NULL)
				{
					char* pEnd = NULL;
					wValue = (WORD)strtol(szTmp, &pEnd, 16);
				}
				else
				{
					wValue = atoi(szTmp);
				}
				
				if(!m_bExtendData &&(strstr(szLine, "\\Extend\\") != NULL || strstr(szLine, "\\extend\\") != NULL) && (strstr(szLine, "\\Extend2\\") == NULL || strstr(szLine, "\\extend2\\") != NULL))
				{
					m_bExtendData = TRUE;
				}
				else if(!m_bRocky && (strstr(szLine, "\\arm_volume\\") != NULL || strstr(szLine, "\\dsp_volume\\") != NULL))
				{
					m_bRocky = TRUE;
				}
				else if(!m_bAudio3 && (strstr(szLine, "\\Extend2\\") != NULL || strstr(szLine, "\\extend2\\") != NULL) )
				{
					m_bAudio3 = TRUE;
				}
				
				if(strcmp(szModeName, szLastModeName) != 0)
				{
					if(!bFirst)
					{
						ZeroMemory( (LPVOID)&m_AudioData.lpAudioMode[nIndex].audio_data,sizeof( AUDIO_STRUCT ) );
						memcpy((LPVOID)&m_AudioData.lpAudioMode[nIndex].audio_data, &byteAudioStruct, sizeof(AUDIO_STRUCT));
						ZeroMemory(&byteAudioStruct, sizeof(AUDIO_STRUCT));
						memcpy(m_AudioData.lpAudioMode[nIndex].mode_name, szLastModeName, strlen(szLastModeName)+1);
						dwSize = 0;
						nIndex ++;
					}
					else
					{
						
						bFirst = FALSE;
					}
					memcpy(byteAudioStruct + dwSize, &wValue, sizeof(WORD));
					dwSize += sizeof(WORD);
					memset(&szLastModeName, 0, 128);
					strcpy(szLastModeName, szModeName);
				}
				else
				{
					memcpy(byteAudioStruct + dwSize, &wValue, sizeof(WORD));
					dwSize += sizeof(WORD);
				}
				continue;
			}
			//teana hu 2010.06.10
			if(m_bLoadEQ)
			{
				lpFind = strstr(szLine, "audio\\EQ_");
				if(lpFind != NULL)
				{
					if(!bLastAudio)
					{
						bLastAudio = TRUE;
						bFirst = TRUE;
						//for last audio mode
						ZeroMemory( (LPVOID)&m_AudioData.lpAudioMode[nIndex].audio_data,sizeof( AUDIO_STRUCT ) );
						memcpy((LPVOID)&m_AudioData.lpAudioMode[nIndex].audio_data, &byteAudioStruct, sizeof(AUDIO_STRUCT));
						ZeroMemory(&byteAudioStruct, sizeof(AUDIO_STRUCT));
						memcpy(m_AudioData.lpAudioMode[nIndex].mode_name, szModeName, strlen(szModeName)+1);
						dwSize = 0;
						nIndex = 0;
						
					}
					
					CString strTmp = szLine;
					int nFind = strTmp.Find(_T("\\"));
					if(nFind != -1)
					{
						strTmp.Delete(0, nFind + 1);
						nFind = strTmp.Find(_T("\\"));
						if(nFind != -1)
						{
#ifdef _UNICODE
							int nLen = (strTmp.Left(nFind)).GetLength();
							WideCharToMultiByte(CP_ACP,0,strTmp.Left(nFind),-1,szEQName,nLen*2,NULL,NULL);
#else
							strcpy(szEQName, strTmp.Left(nFind));
#endif
						}
					}
					
					memset(szTmp, 0, 128);
					LPSTR lpFind = strrchr(szLine, '=');
					if(lpFind != NULL)
					{
						strcpy(szTmp, lpFind+1);
					}
					
					if(strstr(szTmp, "0x") != NULL || strstr(szTmp, "0X") != NULL)
					{
						char* pEnd = NULL;
						wValue = (WORD)strtol(szTmp, &pEnd, 16);
					}
					else
					{
						wValue = atoi(szTmp);
					}
					
					if(strcmp(szEQName, szLastEQName) != 0)
					{
						if(!bFirst)
						{
							ZeroMemory( (LPVOID)&m_EQData.lpEQMode[nIndex],sizeof( AUDIO_EQ_STRUCT_T ) );
							memcpy((LPVOID)&m_EQData.lpEQMode[nIndex].eq_mode_control, &byteEQStruct, sizeof(short));
							memcpy((LPVOID)&m_EQData.lpEQMode[nIndex].eq_modes, byteEQStruct+sizeof(short), sizeof(EQ_MODE_NV_PARAM_T)*TOTAL_EQ_MODE_NUM + EQ_EXTEND_ARRAY_DIM * sizeof(short));
							ZeroMemory(&byteEQStruct, sizeof(nEQLenth));
							memcpy(m_EQData.lpEQMode[nIndex].ucParaSetName, szLastEQName, strlen(szLastEQName)+1);
							dwSize = 0;
							nIndex ++;
						}
						else
						{
							bFirst = FALSE;
						}
						memcpy(byteEQStruct + dwSize, &wValue, sizeof(WORD));
						dwSize += sizeof(WORD);
						memset(&szLastEQName, 0, 128);
						strcpy(szLastEQName, szEQName);
					}
					else
					{
						memcpy(byteEQStruct + dwSize, &wValue, sizeof(WORD));
						dwSize += sizeof(WORD);
					}
					continue;
				}
				//
			}	
			if(m_bLoadVolume)
			{
				lpFind = strstr(szLine, "audio\\audio_arm\\");
				LPSTR lpFind1 = strstr(szLine, "audio_arm\\audio_arm\\");

				if(lpFind != NULL || lpFind1 != NULL)
				{
					if(m_bLoadEQ && !bLastEQ)
					{
						bLastEQ = TRUE;
						bFirst = TRUE;
						//for last eq mode
						ZeroMemory( (LPVOID)&m_EQData.lpEQMode[nIndex],sizeof( AUDIO_EQ_STRUCT_T ) );
						memcpy((LPVOID)&m_EQData.lpEQMode[nIndex].eq_mode_control, &byteEQStruct, sizeof(short));
						memcpy((LPVOID)&m_EQData.lpEQMode[nIndex].eq_modes, byteEQStruct+sizeof(short), sizeof(EQ_MODE_NV_PARAM_T)*TOTAL_EQ_MODE_NUM);
						ZeroMemory(&byteEQStruct, sizeof(nEQLenth));
						memcpy(m_EQData.lpEQMode[nIndex].ucParaSetName, szLastEQName, strlen(szLastEQName)+1);
						dwSize = 0;
						nIndex = 0;
					}
					else if(!bLastAudio)
					{
						bLastAudio = TRUE;
						bFirst = TRUE;
						//for last audio mode
						ZeroMemory( (LPVOID)&m_AudioData.lpAudioMode[nIndex].audio_data,sizeof( AUDIO_STRUCT ) );
						memcpy((LPVOID)&m_AudioData.lpAudioMode[nIndex].audio_data, &byteAudioStruct, sizeof(AUDIO_STRUCT));
						ZeroMemory(&byteAudioStruct, sizeof(AUDIO_STRUCT));
						memcpy(m_AudioData.lpAudioMode[nIndex].mode_name, szModeName, strlen(szModeName)+1);
						dwSize = 0;
						nIndex = 0;
					}
					if(lpFind != NULL)
					{
						sscanf(szLine, "audio\\audio_arm\\%[a-zA-Z0-9_]%*[\]\\AudioStructure\\", szVolumeName);
					}
					else
					{
						sscanf(szLine, "audio_arm\\audio_arm\\%[a-zA-Z0-9_]%*[\]\\AudioStructure\\", szVolumeName);
					}
					
					memset(szTmp, 0, 128);
					LPSTR lpFind = strrchr(szLine, '=');
					if(lpFind != NULL)
					{
						strcpy(szTmp, lpFind+1);
					}
					
					if(strstr(szTmp, "0x") != NULL || strstr(szTmp, "0X") != NULL)
					{
						char* pEnd = NULL;
						lValue = (LONG)strtol(szTmp, &pEnd, 16);
					}
					else
					{
						lValue = atoi(szTmp);
					}
					
					if(strcmp(szVolumeName, szLastVolumeName) != 0)
					{
						if(!bFirst)
						{
							if( AUDIO_ARM_APP_TYPE_MAX == m_nArmSerialCount)
							{
								ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
								ZeroMemory((LPVOID)&volume_data_16, sizeof(VOLUME_STRUCT_SIXTEEN));
								memcpy((LPVOID)&volume_data_16, &byteVolumeStruct_16, sizeof(VOLUME_STRUCT_SIXTEEN));
								ZeroMemory(&byteVolumeStruct_16, sizeof(VOLUME_STRUCT_SIXTEEN));
								m_Cmd.CopyVolumeStructFrom16ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_16);
								memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
							}
							else if( AUDIO_ARM_APP_TYPE_MIN == m_nArmSerialCount )
							{
								ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
								ZeroMemory((LPVOID)&volume_data_8, sizeof(VOLUME_STRUCT_MIN));
								memcpy((LPVOID)&volume_data_8, &byteVolumeStruct_8, sizeof(VOLUME_STRUCT_MIN));
								ZeroMemory(&byteVolumeStruct_8, sizeof(VOLUME_STRUCT_MIN));
								m_Cmd.CopyVolumeStructFrom8ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_8);
								memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
							}
							else if( AUDIO_ARM_APP_TYPE_ONE == m_nArmSerialCount )
							{
								ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
								ZeroMemory((LPVOID)&volume_data_1, sizeof(VOLUME_STRUCT_ONE));
								memcpy((LPVOID)&volume_data_1, &byteVolumeStruct_1, sizeof(VOLUME_STRUCT_ONE));
								ZeroMemory(&byteVolumeStruct_1, sizeof(VOLUME_STRUCT_ONE));
								m_Cmd.CopyVolumeStructFrom1ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_1);
								memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
							}
							else if(AUDIO_ARM_APP_TYPE_SIX == m_nArmSerialCount)
							{
								ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
								ZeroMemory((LPVOID)&volume_data_6, sizeof(VOLUME_STRUCT_SIX));
								memcpy((LPVOID)&volume_data_6, &byteVolumeStruct_6, sizeof(VOLUME_STRUCT_SIX));
								ZeroMemory(&byteVolumeStruct_6, sizeof(VOLUME_STRUCT_SIX));
								m_Cmd.CopyVolumeStructFrom6ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_6);
								memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
							}
							dwSize = 0;
							nIndex ++;
							nIndexArm = 0;
						}
						else
						{	
							bFirst = FALSE;
						}
						if( AUDIO_ARM_APP_TYPE_MAX == m_nArmSerialCount )
						{
							memcpy(byteVolumeStruct_16 + dwSize, &lValue, sizeof(WORD));
							dwSize += sizeof(WORD);
						}
						else if( AUDIO_ARM_APP_TYPE_MIN == m_nArmSerialCount )
						{
							memcpy(byteVolumeStruct_8 + dwSize, &lValue, sizeof(WORD));
							dwSize += sizeof(WORD);
						}
						else if(AUDIO_ARM_APP_TYPE_ONE == m_nArmSerialCount)
						{
							memcpy(byteVolumeStruct_1 + dwSize, &lValue, sizeof(WORD));
							dwSize += sizeof(WORD);
						}
						else if(AUDIO_ARM_APP_TYPE_SIX == m_nArmSerialCount)
						{
							memcpy(byteVolumeStruct_6 + dwSize, &lValue, nArmOffset[nIndexArm]);
							dwSize += nArmOffset[nIndexArm];
							nIndexArm ++;
						}
						
						memset(&szLastVolumeName, 0, 128);
						strcpy(szLastVolumeName, szVolumeName);
					}
					else
					{
						if(AUDIO_ARM_APP_TYPE_MAX == m_nArmSerialCount)
						{
							memcpy(byteVolumeStruct_16 + dwSize, &lValue, sizeof(WORD));
							dwSize += sizeof(WORD);
						}
						else if( AUDIO_ARM_APP_TYPE_MIN == m_nArmSerialCount )
						{
							memcpy(byteVolumeStruct_8 + dwSize, &lValue, sizeof(WORD));
							dwSize += sizeof(WORD);
						}
						else if(AUDIO_ARM_APP_TYPE_ONE == m_nArmSerialCount)
						{
							memcpy(byteVolumeStruct_1 + dwSize, &lValue, sizeof(WORD));
							dwSize += sizeof(WORD);
						}
						else if(AUDIO_ARM_APP_TYPE_SIX == m_nArmSerialCount)
						{
							memcpy(byteVolumeStruct_6 + dwSize, &lValue, nArmOffset[nIndexArm]);
							dwSize += nArmOffset[nIndexArm];
							nIndexArm ++;
						}
						
					}
				}
			}

			if(m_bLoadLVVE)
			{
				lpFind = strstr(szLine, "audio\\lvve_params\\");
				LPSTR lpFind1 = strstr(szLine, "lvve_params\\lvve_params\\");
				if(lpFind != NULL || lpFind1 != NULL )
				{
					if(m_bLoadVolume && !bLastVolume)
					{
						bLastVolume = TRUE;
						bFirst = TRUE;
						//for last volume mode
						if(AUDIO_ARM_APP_TYPE_MAX == m_nArmSerialCount)
						{
							ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
							ZeroMemory((LPVOID)&volume_data_16, sizeof(VOLUME_STRUCT_SIXTEEN));
							memcpy((LPVOID)&volume_data_16, &byteVolumeStruct_16, sizeof(VOLUME_STRUCT_SIXTEEN));
							ZeroMemory(&byteVolumeStruct_16, sizeof(VOLUME_STRUCT_SIXTEEN));
							m_Cmd.CopyVolumeStructFrom16ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_16);
							memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
						}
						else if( AUDIO_ARM_APP_TYPE_MIN == m_nArmSerialCount )
						{
							ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
							ZeroMemory((LPVOID)&volume_data_8, sizeof(VOLUME_STRUCT_MIN));
							memcpy((LPVOID)&volume_data_8, &byteVolumeStruct_8, sizeof(VOLUME_STRUCT_MIN));
							ZeroMemory(&byteVolumeStruct_8, sizeof(VOLUME_STRUCT_MIN));
							m_Cmd.CopyVolumeStructFrom8ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_8);
							memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
						}
						else if(AUDIO_ARM_APP_TYPE_ONE == m_nArmSerialCount)
						{
							ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
							ZeroMemory((LPVOID)&volume_data_1, sizeof(VOLUME_STRUCT_ONE));
							memcpy((LPVOID)&volume_data_1, &byteVolumeStruct_1, sizeof(VOLUME_STRUCT_ONE));
							ZeroMemory(&byteVolumeStruct_1, sizeof(VOLUME_STRUCT_ONE));
							m_Cmd.CopyVolumeStructFrom1ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_1);
							memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
						}
						else if(AUDIO_ARM_APP_TYPE_SIX == m_nArmSerialCount)
						{
							ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
							ZeroMemory((LPVOID)&volume_data_6, sizeof(VOLUME_STRUCT_SIX));
							memcpy((LPVOID)&volume_data_6, &byteVolumeStruct_6, sizeof(VOLUME_STRUCT_SIX));
							ZeroMemory(&byteVolumeStruct_6, sizeof(VOLUME_STRUCT_SIX));
							m_Cmd.CopyVolumeStructFrom6ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_6);
							memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
						}
						dwSize = 0;
						nIndex = 0;
					}
					else if(m_bLoadEQ && !bLastEQ)
					{
						bLastEQ = TRUE;
						bFirst = TRUE;
						//for last eq mode
						ZeroMemory( (LPVOID)&m_EQData.lpEQMode[nIndex],sizeof( AUDIO_EQ_STRUCT_T ) );
						memcpy((LPVOID)&m_EQData.lpEQMode[nIndex].eq_mode_control, &byteEQStruct, sizeof(short));
						memcpy((LPVOID)&m_EQData.lpEQMode[nIndex].eq_modes, byteEQStruct+sizeof(short), sizeof(EQ_MODE_NV_PARAM_T)*TOTAL_EQ_MODE_NUM);
						ZeroMemory(&byteEQStruct, sizeof(nEQLenth));
						memcpy(m_EQData.lpEQMode[nIndex].ucParaSetName, szLastEQName, strlen(szLastEQName)+1);
						dwSize = 0;
						nIndex = 0;
					}
					else if(!bLastAudio)
					{
						bLastAudio = TRUE;
						bFirst = TRUE;
						//for last audio mode
						ZeroMemory( (LPVOID)&m_AudioData.lpAudioMode[nIndex].audio_data,sizeof( AUDIO_STRUCT ) );
						memcpy((LPVOID)&m_AudioData.lpAudioMode[nIndex].audio_data, &byteAudioStruct, sizeof(AUDIO_STRUCT));
						ZeroMemory(&byteAudioStruct, sizeof(AUDIO_STRUCT));
						memcpy(m_AudioData.lpAudioMode[nIndex].mode_name, szModeName, strlen(szModeName)+1);
						dwSize = 0;
						nIndex = 0;
					}
					if(lpFind != NULL)
					{
						sscanf(szLine, "audio\\lvve_params\\%[a-zA-Z0-9_]%*[\]\\", szLvveName);
					}
					else
					{
						sscanf(szLine, "lvve_params\\lvve_params\\%[a-zA-Z0-9_]%*[\]\\", szLvveName);
					}
					
					memset(szTmp, 0, 128);
					LPSTR lpFind = strrchr(szLine, '=');
					if(lpFind != NULL)
					{
						strcpy(szTmp, lpFind+1);
					}

					if(strstr(szTmp, "0x") != NULL || strstr(szTmp, "0X") != NULL)
					{
						char* pEnd = NULL;
						lValue = (LONG)strtol(szTmp, &pEnd, 16);
					}
					else
					{
						lValue = atol(szTmp);
					}
					
					if(strcmp(szLvveName, szLastLvveName) != 0)
					{
						if(!bFirst)
						{
							//teana hu 2012.01.12
							if(m_dwLvveType == LVVE_NO_RESERVE)
							{
								ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t,sizeof( LVVE_CTRL_PARAM_T ) );
								ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
								LVVE_CTRL_PARAM_NV_MIN_T lvve_ctrl_param_nv_t = {0};
								memcpy((LPVOID)&lvve_ctrl_param_nv_t.lvve_ctrl_param_t, &byteLvveStructMin, sizeof(LVVE_CTRL_PARAM_1_T));
								m_Cmd.CopyLVVEFromNoReserveToMax(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex], lvve_ctrl_param_nv_t);
								ZeroMemory(&byteLvveStructMin, sizeof(LVVE_CTRL_PARAM_1_T));
								memcpy(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].ucModeName, szLastLvveName, strlen(szLastLvveName)+1);
							}
							else if(m_dwLvveType == LVVE_RESERVE)
							{
								ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t,sizeof( LVVE_CTRL_PARAM_T ) );
								ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
								LVVE_CTRL_PARAM_NV_1_T lvve_ctrl_param_nv_t = {0};
								memcpy((LPVOID)&lvve_ctrl_param_nv_t.lvve_ctrl_param_t, &byteLvveStructReserve, sizeof(LVVE_CTRL_PARAM_1_T));
								m_Cmd.CopyLVVEFromReserveToMax(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex], lvve_ctrl_param_nv_t);
								memcpy((LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, byteLvveStructReserve+sizeof(LVVE_CTRL_PARAM_1_T), sizeof(short)*LVVE_CONFIG_RESERVE_LEN );
								ZeroMemory(&byteLvveStructReserve, sizeof(LVVE_CTRL_PARAM_1_T) + sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
								memcpy(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].ucModeName, szLastLvveName, strlen(szLastLvveName)+1);
							}
							else if(m_dwLvveType == LVVE_JUNIPER)
							{
								ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t,sizeof( LVVE_CTRL_PARAM_T ) );
								ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
								LVVE_CTRL_PARAM_NV_2_T lvve_ctrl_param_nv_t = {0};
								memcpy((LPVOID)&lvve_ctrl_param_nv_t.lvve_ctrl_param_t, &byteLvveStructJuniper, sizeof(LVVE_CTRL_PARAM_2_T));
								m_Cmd.CopyLVVEFromJuniperToMax(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex], lvve_ctrl_param_nv_t);
								memcpy((LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, byteLvveStructJuniper+sizeof(LVVE_CTRL_PARAM_2_T), sizeof(short)*LVVE_CONFIG_RESERVE_LEN );
								ZeroMemory(&byteLvveStructJuniper, sizeof(LVVE_CTRL_PARAM_2_T) + sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
								memcpy(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].ucModeName, szLastLvveName, strlen(szLastLvveName)+1);
							}
							else if(m_dwLvveType == LVVE_SINGLE_MIC)
							{
								ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t,sizeof( LVVE_CTRL_PARAM_T ) );
								ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
								LVVE_CTRL_PARAM_NV_3_T lvve_ctrl_param_nv_t = {0};
								memcpy((LPVOID)&lvve_ctrl_param_nv_t.lvve_ctrl_param_t, &byteLvveStructSingleMic, sizeof(LVVE_CTRL_PARAM_3_T));
								m_Cmd.CopyLVVEFromSingleMicToMax(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex], lvve_ctrl_param_nv_t);
								memcpy((LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, byteLvveStructSingleMic+sizeof(LVVE_CTRL_PARAM_3_T), sizeof(short)*LVVE_CONFIG_RESERVE_LEN );
								ZeroMemory(&byteLvveStructSingleMic, sizeof(LVVE_CTRL_PARAM_3_T) + sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
								memcpy(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].ucModeName, szLastLvveName, strlen(szLastLvveName)+1);
							}
							else
							{
								ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t,sizeof( LVVE_CTRL_PARAM_T ) );
								ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
								memcpy((LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t, &byteLvveStruct, sizeof(LVVE_CTRL_PARAM_T));
								memcpy((LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, byteLvveStruct+sizeof(LVVE_CTRL_PARAM_T), sizeof(short)*LVVE_CONFIG_RESERVE_LEN );
								ZeroMemory(&byteLvveStruct, sizeof(LVVE_CTRL_PARAM_T) + sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
								memcpy(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].ucModeName, szLastLvveName, strlen(szLastLvveName)+1);
							}
							//
							dwSize = 0;
							nIndex ++;
							nOffsetIndex = 0;
						}
						else
						{	
							bFirst = FALSE;
						}

						//teana hu 2012.01.12
						if(m_dwLvveType == LVVE_NO_RESERVE)
						{
							memcpy(byteLvveStructMin + dwSize, &lValue, nLvveOffsetMin[nOffsetIndex]);
							dwSize += nLvveOffsetMin[nOffsetIndex];
						}
						else if(m_dwLvveType == LVVE_RESERVE)
						{
							memcpy(byteLvveStructReserve + dwSize, &lValue, nLvveOffsetReserve[nOffsetIndex]);
							dwSize += nLvveOffsetReserve[nOffsetIndex];
						}
						else if(m_dwLvveType == LVVE_JUNIPER)
						{
							memcpy(byteLvveStructJuniper + dwSize, &lValue, nLvveOffsetJuniper[nOffsetIndex]);
							dwSize += nLvveOffsetJuniper[nOffsetIndex];
						}
						else if(m_dwLvveType == LVVE_SINGLE_MIC)
						{
							memcpy(byteLvveStructSingleMic + dwSize, &lValue, nLvveOffsetSingleMic[nOffsetIndex]);
							dwSize += nLvveOffsetSingleMic[nOffsetIndex];
						}
						else
						{
							memcpy(byteLvveStruct + dwSize, &lValue, nLvveOffset[nOffsetIndex]);
							dwSize += nLvveOffset[nOffsetIndex];
						}
						//
						
						nOffsetIndex ++;
						memset(&szLastLvveName, 0, 128);
						strcpy(szLastLvveName, szLvveName);
					}
					else
					{
						//teana hu 2012.01.12
						if(m_dwLvveType == LVVE_NO_RESERVE)
						{
							memcpy(byteLvveStructMin + dwSize, &lValue, nLvveOffsetMin[nOffsetIndex]);
							dwSize += nLvveOffsetMin[nOffsetIndex];
						}
						else if(m_dwLvveType == LVVE_RESERVE)
						{
							memcpy(byteLvveStructReserve + dwSize, &lValue, nLvveOffsetReserve[nOffsetIndex]);
							dwSize += nLvveOffsetReserve[nOffsetIndex];
						}
						else if(m_dwLvveType == LVVE_JUNIPER)
						{
							memcpy(byteLvveStructJuniper + dwSize, &lValue, nLvveOffsetJuniper[nOffsetIndex]);
							dwSize += nLvveOffsetJuniper[nOffsetIndex];
						}
						else if(m_dwLvveType == LVVE_SINGLE_MIC)
						{
							memcpy(byteLvveStructSingleMic + dwSize, &lValue, nLvveOffsetSingleMic[nOffsetIndex]);
							dwSize += nLvveOffsetSingleMic[nOffsetIndex];
						}
						else
						{
							memcpy(byteLvveStruct + dwSize, &lValue, nLvveOffset[nOffsetIndex]);
							dwSize += nLvveOffset[nOffsetIndex];
						}
						//
						nOffsetIndex ++;
					}
				}
			}
			if(m_bLoadDspCodec)
			{
				lpFind = strstr(szLine, "audio_dsp_codec\\dsp_codec_config\\");
				if(lpFind != NULL)
				{
					if(m_bLoadLVVE && !bLastLvve)
					{
						bLastLvve = TRUE;
						bFirst = TRUE;
						//teana hu 2012.01.12
						if(m_dwLvveType == LVVE_NO_RESERVE)
						{
							ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t,sizeof( LVVE_CTRL_PARAM_T ) );
							ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
							LVVE_CTRL_PARAM_NV_MIN_T lvve_ctrl_param_nv_t = {0};
							memcpy((LPVOID)&lvve_ctrl_param_nv_t.lvve_ctrl_param_t, &byteLvveStructMin, sizeof(LVVE_CTRL_PARAM_1_T));
							m_Cmd.CopyLVVEFromNoReserveToMax(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex], lvve_ctrl_param_nv_t);
							ZeroMemory(&byteLvveStructMin, sizeof(LVVE_CTRL_PARAM_1_T));
							memcpy(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].ucModeName, szLastLvveName, strlen(szLastLvveName)+1);
						}
						else if(m_dwLvveType == LVVE_RESERVE)
						{
							ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t,sizeof( LVVE_CTRL_PARAM_T ) );
							ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
							LVVE_CTRL_PARAM_NV_1_T lvve_ctrl_param_nv_t = {0};
							memcpy((LPVOID)&lvve_ctrl_param_nv_t.lvve_ctrl_param_t, &byteLvveStructReserve, sizeof(LVVE_CTRL_PARAM_1_T));
							m_Cmd.CopyLVVEFromReserveToMax(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex], lvve_ctrl_param_nv_t);
							memcpy((LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, byteLvveStructReserve+sizeof(LVVE_CTRL_PARAM_1_T), sizeof(short)*LVVE_CONFIG_RESERVE_LEN );
							ZeroMemory(&byteLvveStructReserve, sizeof(LVVE_CTRL_PARAM_1_T) + sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
							memcpy(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].ucModeName, szLastLvveName, strlen(szLastLvveName)+1);
						}
						else if(m_dwLvveType == LVVE_JUNIPER)
						{
							ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t,sizeof( LVVE_CTRL_PARAM_T ) );
							ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
							LVVE_CTRL_PARAM_NV_2_T lvve_ctrl_param_nv_t = {0};
							memcpy((LPVOID)&lvve_ctrl_param_nv_t.lvve_ctrl_param_t, &byteLvveStructJuniper, sizeof(LVVE_CTRL_PARAM_2_T));
							m_Cmd.CopyLVVEFromJuniperToMax(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex], lvve_ctrl_param_nv_t);
							memcpy((LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, byteLvveStructJuniper+sizeof(LVVE_CTRL_PARAM_2_T), sizeof(short)*LVVE_CONFIG_RESERVE_LEN );
							ZeroMemory(&byteLvveStructJuniper, sizeof(LVVE_CTRL_PARAM_2_T) + sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
							memcpy(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].ucModeName, szLastLvveName, strlen(szLastLvveName)+1);
						}
						else if(m_dwLvveType == LVVE_SINGLE_MIC)
						{
							ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t,sizeof( LVVE_CTRL_PARAM_T ) );
							ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
							LVVE_CTRL_PARAM_NV_3_T lvve_ctrl_param_nv_t = {0};
							memcpy((LPVOID)&lvve_ctrl_param_nv_t.lvve_ctrl_param_t, &byteLvveStructSingleMic, sizeof(LVVE_CTRL_PARAM_3_T));
							m_Cmd.CopyLVVEFromSingleMicToMax(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex], lvve_ctrl_param_nv_t);
							memcpy((LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, byteLvveStructSingleMic+sizeof(LVVE_CTRL_PARAM_3_T), sizeof(short)*LVVE_CONFIG_RESERVE_LEN );
							ZeroMemory(&byteLvveStructSingleMic, sizeof(LVVE_CTRL_PARAM_3_T) + sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
							memcpy(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].ucModeName, szLastLvveName, strlen(szLastLvveName)+1);
						}
						else
						{
							ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t,sizeof( LVVE_CTRL_PARAM_T ) );
							ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
							memcpy((LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t, &byteLvveStruct, sizeof(LVVE_CTRL_PARAM_T));
							memcpy((LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, byteLvveStruct+sizeof(LVVE_CTRL_PARAM_T), sizeof(short)*LVVE_CONFIG_RESERVE_LEN );
							ZeroMemory(&byteLvveStruct, sizeof(LVVE_CTRL_PARAM_T) + sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
							memcpy(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].ucModeName, szLastLvveName, strlen(szLastLvveName)+1);
						}
							//
						dwSize = 0;
						nIndex = 0;
					}
					else if(m_bLoadVolume && !bLastVolume)
					{
						bLastVolume = TRUE;
						bFirst = TRUE;
						//for last volume mode
						if(AUDIO_ARM_APP_TYPE_MAX == m_nArmSerialCount)
						{
							ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
							ZeroMemory((LPVOID)&volume_data_16, sizeof(VOLUME_STRUCT_SIXTEEN));
							memcpy((LPVOID)&volume_data_16, &byteVolumeStruct_16, sizeof(VOLUME_STRUCT_SIXTEEN));
							ZeroMemory(&byteVolumeStruct_16, sizeof(VOLUME_STRUCT_SIXTEEN));
							m_Cmd.CopyVolumeStructFrom16ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_16);
							memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
						}
						else if( AUDIO_ARM_APP_TYPE_MIN == m_nArmSerialCount )
						{
							ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
							ZeroMemory((LPVOID)&volume_data_8, sizeof(VOLUME_STRUCT_MIN));
							memcpy((LPVOID)&volume_data_8, &byteVolumeStruct_8, sizeof(VOLUME_STRUCT_MIN));
							ZeroMemory(&byteVolumeStruct_8, sizeof(VOLUME_STRUCT_MIN));
							m_Cmd.CopyVolumeStructFrom8ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_8);
							memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
						}
						else if(AUDIO_ARM_APP_TYPE_ONE == m_nArmSerialCount)
						{
							ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
							ZeroMemory((LPVOID)&volume_data_1, sizeof(VOLUME_STRUCT_ONE));
							memcpy((LPVOID)&volume_data_1, &byteVolumeStruct_1, sizeof(VOLUME_STRUCT_ONE));
							ZeroMemory(&byteVolumeStruct_1, sizeof(VOLUME_STRUCT_ONE));
							m_Cmd.CopyVolumeStructFrom1ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_1);
							memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
						}
						else if(AUDIO_ARM_APP_TYPE_SIX == m_nArmSerialCount)
						{
							ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
							ZeroMemory((LPVOID)&volume_data_6, sizeof(VOLUME_STRUCT_SIX));
							memcpy((LPVOID)&volume_data_6, &byteVolumeStruct_6, sizeof(VOLUME_STRUCT_SIX));
							ZeroMemory(&byteVolumeStruct_6, sizeof(VOLUME_STRUCT_SIX));
							m_Cmd.CopyVolumeStructFrom6ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_6);
							memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
						}
						dwSize = 0;
						nIndex = 0;
					}
					else if(m_bLoadEQ && !bLastEQ)
					{
						bLastEQ = TRUE;
						bFirst = TRUE;
						//for last eq mode
						ZeroMemory( (LPVOID)&m_EQData.lpEQMode[nIndex],sizeof( AUDIO_EQ_STRUCT_T ) );
						memcpy((LPVOID)&m_EQData.lpEQMode[nIndex].eq_mode_control, &byteEQStruct, sizeof(short));
						memcpy((LPVOID)&m_EQData.lpEQMode[nIndex].eq_modes, byteEQStruct+sizeof(short), sizeof(EQ_MODE_NV_PARAM_T)*TOTAL_EQ_MODE_NUM);
						ZeroMemory(&byteEQStruct, sizeof(nEQLenth));
						memcpy(m_EQData.lpEQMode[nIndex].ucParaSetName, szLastEQName, strlen(szLastEQName)+1);
						dwSize = 0;
						nIndex = 0;
					}
					else if(!bLastAudio)
					{
						bLastAudio = TRUE;
						bFirst = TRUE;
						//for last audio mode
						ZeroMemory( (LPVOID)&m_AudioData.lpAudioMode[nIndex].audio_data,sizeof( AUDIO_STRUCT ) );
						memcpy((LPVOID)&m_AudioData.lpAudioMode[nIndex].audio_data, &byteAudioStruct, sizeof(AUDIO_STRUCT));
						ZeroMemory(&byteAudioStruct, sizeof(AUDIO_STRUCT));
						memcpy(m_AudioData.lpAudioMode[nIndex].mode_name, szModeName, strlen(szModeName)+1);
						dwSize = 0;
						nIndex = 0;
					}
					sscanf(szLine, "audio_dsp_codec\\dsp_codec_config\\%[a-zA-Z0-9_]%*[\]\\", szDspCodecName);
					memset(szTmp, 0, 128);
					LPSTR lpFind = strrchr(szLine, '=');
					if(lpFind != NULL)
					{
						strcpy(szTmp, lpFind+1);
					}
					
					if(strstr(szTmp, "0x") != NULL || strstr(szTmp, "0X") != NULL)
					{
						char* pEnd = NULL;
						lValue = (LONG)strtol(szTmp, &pEnd, 16);
					}
					else
					{
						lValue = atol(szTmp);
					}
					
					if(strcmp(szDspCodecName, szLastDspCodecName) != 0)
					{
						if(!bFirst)
						{
							ZeroMemory( (LPVOID)&m_DspModePara.lp_audio_nv_dsp_mode_info[nIndex].tAudioNvDspModeStruct,sizeof( AUDIO_NV_DSP_MODE_STRUCT_T ) );
							memcpy((LPVOID)&m_DspModePara.lp_audio_nv_dsp_mode_info[nIndex].tAudioNvDspModeStruct, &byteDspCodecStruct, sizeof(AUDIO_NV_DSP_MODE_STRUCT_T));
							ZeroMemory(&byteDspCodecStruct, sizeof(AUDIO_NV_DSP_MODE_STRUCT_T));
							memcpy(m_DspModePara.lp_audio_nv_dsp_mode_info[nIndex].ucModeName, szLastDspCodecName, strlen(szLastDspCodecName)+1);
							dwSize = 0;
							nIndex ++;
						}
						else
						{	
							bFirst = FALSE;
						}
						memcpy(byteDspCodecStruct + dwSize, &lValue, sizeof(WORD));
						dwSize += sizeof(WORD);
					
						memset(&szLastDspCodecName, 0, 128);
						strcpy(szLastDspCodecName, szDspCodecName);
					}
					else
					{
						memcpy(byteDspCodecStruct + dwSize, &lValue, sizeof(WORD));
						dwSize += sizeof(WORD);
					}
				}
			}
			memset(szLine, 0, 1024);
			}
		}
		
		//for last mode
		if(m_bLoadEQ && !m_bLoadVolume && !m_bLoadLVVE && !m_bLoadDspCodec)
		{
			ZeroMemory( (LPVOID)&m_EQData.lpEQMode[nIndex],sizeof( AUDIO_EQ_STRUCT_T ) );
			memcpy((LPVOID)&m_EQData.lpEQMode[nIndex].eq_mode_control, &byteEQStruct, sizeof(short));
			memcpy((LPVOID)&m_EQData.lpEQMode[nIndex].eq_modes, byteEQStruct+sizeof(short), sizeof(EQ_MODE_NV_PARAM_T)*TOTAL_EQ_MODE_NUM);
			ZeroMemory(&byteEQStruct, sizeof(nEQLenth));
			memcpy(m_EQData.lpEQMode[nIndex].ucParaSetName, szEQName, strlen(szEQName)+1);
		}
		else if(m_bLoadVolume && !m_bLoadLVVE && !m_bLoadDspCodec)
		{
			if( AUDIO_ARM_APP_TYPE_MAX == m_nArmSerialCount )
			{
				ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
				ZeroMemory((LPVOID)&volume_data_16, sizeof(VOLUME_STRUCT_SIXTEEN));
				memcpy((LPVOID)&volume_data_16, &byteVolumeStruct_16, sizeof(VOLUME_STRUCT_SIXTEEN));
				ZeroMemory(&byteVolumeStruct_16, sizeof(VOLUME_STRUCT_SIXTEEN));
				m_Cmd.CopyVolumeStructFrom16ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_16);
				memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
			}
			else if( AUDIO_ARM_APP_TYPE_MIN == m_nArmSerialCount )
			{
				ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
				ZeroMemory((LPVOID)&volume_data_8, sizeof(VOLUME_STRUCT_MIN));
				memcpy((LPVOID)&volume_data_8, &byteVolumeStruct_8, sizeof(VOLUME_STRUCT_MIN));
				ZeroMemory(&byteVolumeStruct_8, sizeof(VOLUME_STRUCT_MIN));
				m_Cmd.CopyVolumeStructFrom8ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_8);
				memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
			}
			else if( AUDIO_ARM_APP_TYPE_ONE == m_nArmSerialCount)
			{
				ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
				ZeroMemory((LPVOID)&volume_data_1, sizeof(VOLUME_STRUCT_ONE));
				memcpy((LPVOID)&volume_data_1, &byteVolumeStruct_8, sizeof(VOLUME_STRUCT_ONE));
				ZeroMemory(&byteVolumeStruct_1, sizeof(VOLUME_STRUCT_ONE));
				m_Cmd.CopyVolumeStructFrom1ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_1);
				memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
			}
			else if( AUDIO_ARM_APP_TYPE_SIX == m_nArmSerialCount)
			{
				ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
				ZeroMemory((LPVOID)&volume_data_6, sizeof(VOLUME_STRUCT_SIX));
				memcpy((LPVOID)&volume_data_6, &byteVolumeStruct_6, sizeof(VOLUME_STRUCT_SIX));
				ZeroMemory(&byteVolumeStruct_6, sizeof(VOLUME_STRUCT_SIX));
				m_Cmd.CopyVolumeStructFrom6ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_6);
				memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
			}
		}
		else if(m_bLoadLVVE && !m_bLoadDspCodec)
		{
			//teana hu 2012.01.12
			if(m_dwLvveType == LVVE_NO_RESERVE)
			{
				ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t,sizeof( LVVE_CTRL_PARAM_T ) );
				ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
				LVVE_CTRL_PARAM_NV_MIN_T lvve_ctrl_param_nv_t = {0};
				memcpy((LPVOID)&lvve_ctrl_param_nv_t.lvve_ctrl_param_t, &byteLvveStructMin, sizeof(LVVE_CTRL_PARAM_1_T));
				m_Cmd.CopyLVVEFromNoReserveToMax(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex], lvve_ctrl_param_nv_t);
				ZeroMemory(&byteLvveStructMin, sizeof(LVVE_CTRL_PARAM_1_T));
				memcpy(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].ucModeName, szLastLvveName, strlen(szLastLvveName)+1);
			}
			else if(m_dwLvveType == LVVE_RESERVE)
			{
				ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t,sizeof( LVVE_CTRL_PARAM_T ) );
				ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
				LVVE_CTRL_PARAM_NV_1_T lvve_ctrl_param_nv_t = {0};
				memcpy((LPVOID)&lvve_ctrl_param_nv_t.lvve_ctrl_param_t, &byteLvveStructReserve, sizeof(LVVE_CTRL_PARAM_1_T));
				m_Cmd.CopyLVVEFromReserveToMax(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex], lvve_ctrl_param_nv_t);
				memcpy((LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, byteLvveStructReserve+sizeof(LVVE_CTRL_PARAM_1_T), sizeof(short)*LVVE_CONFIG_RESERVE_LEN );
				ZeroMemory(&byteLvveStructReserve, sizeof(LVVE_CTRL_PARAM_1_T) + sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
				memcpy(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].ucModeName, szLastLvveName, strlen(szLastLvveName)+1);
			}
			else if(m_dwLvveType == LVVE_JUNIPER)
			{
				ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t,sizeof( LVVE_CTRL_PARAM_T ) );
				ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
				LVVE_CTRL_PARAM_NV_2_T lvve_ctrl_param_nv_t = {0};
				memcpy((LPVOID)&lvve_ctrl_param_nv_t.lvve_ctrl_param_t, &byteLvveStructJuniper, sizeof(LVVE_CTRL_PARAM_2_T));
				m_Cmd.CopyLVVEFromJuniperToMax(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex], lvve_ctrl_param_nv_t);
				memcpy((LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, byteLvveStructJuniper+sizeof(LVVE_CTRL_PARAM_2_T), sizeof(short)*LVVE_CONFIG_RESERVE_LEN );
				ZeroMemory(&byteLvveStructJuniper, sizeof(LVVE_CTRL_PARAM_2_T) + sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
				memcpy(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].ucModeName, szLastLvveName, strlen(szLastLvveName)+1);
			}
			else if(m_dwLvveType == LVVE_SINGLE_MIC)
			{
				ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t,sizeof( LVVE_CTRL_PARAM_T ) );
				ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
				LVVE_CTRL_PARAM_NV_3_T lvve_ctrl_param_nv_t = {0};
				memcpy((LPVOID)&lvve_ctrl_param_nv_t.lvve_ctrl_param_t, &byteLvveStructSingleMic, sizeof(LVVE_CTRL_PARAM_3_T));
				m_Cmd.CopyLVVEFromSingleMicToMax(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex], lvve_ctrl_param_nv_t);
				memcpy((LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, byteLvveStructSingleMic+sizeof(LVVE_CTRL_PARAM_3_T), sizeof(short)*LVVE_CONFIG_RESERVE_LEN );
				ZeroMemory(&byteLvveStructSingleMic, sizeof(LVVE_CTRL_PARAM_3_T) + sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
				memcpy(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].ucModeName, szLastLvveName, strlen(szLastLvveName)+1);
			}
			else
			{
				ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t,sizeof( LVVE_CTRL_PARAM_T ) );
				ZeroMemory( (LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
				memcpy((LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_ctrl_param_t, &byteLvveStruct, sizeof(LVVE_CTRL_PARAM_T));
				memcpy((LPVOID)&m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].lvve_reserve, byteLvveStruct+sizeof(LVVE_CTRL_PARAM_T), sizeof(short)*LVVE_CONFIG_RESERVE_LEN );
				ZeroMemory(&byteLvveStruct, sizeof(LVVE_CTRL_PARAM_T) + sizeof(short)*LVVE_CONFIG_RESERVE_LEN);
				memcpy(m_LVVEPara.lp_lvve_ctrl_nv_params[nIndex].ucModeName, szLastLvveName, strlen(szLastLvveName)+1);
			}
			//
		}
		else if(m_bLoadDspCodec)
		{
			ZeroMemory( (LPVOID)&m_DspModePara.lp_audio_nv_dsp_mode_info[nIndex].tAudioNvDspModeStruct,sizeof( AUDIO_NV_DSP_MODE_STRUCT_T ) );
			memcpy((LPVOID)&m_DspModePara.lp_audio_nv_dsp_mode_info[nIndex].tAudioNvDspModeStruct, &byteDspCodecStruct, sizeof(AUDIO_NV_DSP_MODE_STRUCT_T));
			ZeroMemory(&byteDspCodecStruct, sizeof(AUDIO_NV_DSP_MODE_STRUCT_T));
			memcpy(m_DspModePara.lp_audio_nv_dsp_mode_info[nIndex].ucModeName, szLastDspCodecName, strlen(szLastDspCodecName)+1);
		}
		else
		{
			ZeroMemory( (LPVOID)&m_AudioData.lpAudioMode[nIndex].audio_data,sizeof( AUDIO_STRUCT ) );
			memcpy((LPVOID)&m_AudioData.lpAudioMode[nIndex].audio_data, &byteAudioStruct, sizeof(AUDIO_STRUCT));
			ZeroMemory(&byteAudioStruct, sizeof(AUDIO_STRUCT));
			memcpy(m_AudioData.lpAudioMode[nIndex].mode_name, szModeName, strlen(szModeName)+1);
		}
		
        //m_Cmd.SetAudioDataSize( 740 );
		m_Cmd.SetAudioDataSize( sizeof(AUDIO_STRUCT) );

		if(m_bLoadEQ)
		{
			m_Cmd.SetEQDataSize( sizeof(AUDIO_EQ_STRUCT_T) );
		}
		
        for( int i=0;i<m_AudioData.mode_count;i++ )
        {
            CAudioModePage* pPage = new CAudioModePage;
           
			CString strTitle(m_AudioData.lpAudioMode[i].mode_name);
            pPage->SetTitle( strTitle );
            pPage->SetAudioData( m_AudioData.lpAudioMode[i].audio_data,this );
			//teana hu 2011.02.21
			BOOL bSmartPhone = FALSE;
			if(m_bLoadVolume)
			{
				
				if((m_VolumeData.lpVolumeMode[0].volume_data.reserve[40] & 0xff) == 0x2)
				{
					bSmartPhone = TRUE;
				}
				else
				{
					bSmartPhone = FALSE;
				}

				for(int j = 0; j < m_VolumeData.mode_count; j++)
				{
					if(strcmp((const char *) m_VolumeData.lpVolumeMode[j].mode_name, (const char *)m_AudioData.lpAudioMode[i].mode_name) == 0)
					{
						pPage->SetVolumeData( m_VolumeData.lpVolumeMode[j].volume_data, this);
						break;
					}
				}
				
				if( AUDIO_ARM_APP_TYPE_MAX == m_nArmSerialCount )
				{
					m_Cmd.SetVolumeDataSize( sizeof(VOLUME_STRUCT_SIXTEEN));
				}
				else if( AUDIO_ARM_APP_TYPE_MIN == m_nArmSerialCount )
				{
					m_Cmd.SetVolumeDataSize( sizeof(VOLUME_STRUCT_MIN));
				}
				else if( AUDIO_ARM_APP_TYPE_ONE == m_nArmSerialCount)
				{
					m_Cmd.SetVolumeDataSize( sizeof(VOLUME_STRUCT_ONE));
				}
				else if( AUDIO_ARM_APP_TYPE_SIX == m_nArmSerialCount)
				{
					m_Cmd.SetVolumeDataSize( sizeof(VOLUME_STRUCT_SIX));
				}
			}
			pPage->SetSmartPhoneFlag(bSmartPhone);
			//

			//teana hu 2011.04.19
			if(m_bLoadLVVE)
			{
				for(int j = 0; j < m_LVVEPara.mode_count; j++)
				{
					if(strcmp((const char *) m_LVVEPara.lp_lvve_ctrl_nv_params[j].ucModeName, (const char *)m_AudioData.lpAudioMode[i].mode_name) == 0)
					{
						pPage->m_bExistLVVE = TRUE;
						pPage->SetLVVEData(m_LVVEPara.lp_lvve_ctrl_nv_params[j]);
						break;
					}
				}
				//teana hu 2012.01.12
				if(m_dwLvveType == LVVE_NO_RESERVE)
				{
					m_Cmd.SetLVVEDataSize( sizeof(LVVE_CTRL_PARAM_NV_MIN_T) );
				}
				else if(m_dwLvveType == LVVE_RESERVE)
				{
					m_Cmd.SetLVVEDataSize( sizeof(LVVE_CTRL_PARAM_NV_1_T) );
				}
				else if(m_dwLvveType == LVVE_JUNIPER)
				{
					m_Cmd.SetLVVEDataSize( sizeof(LVVE_CTRL_PARAM_NV_2_T) );
				}
				else if(m_dwLvveType == LVVE_SINGLE_MIC)
				{
					m_Cmd.SetLVVEDataSize( sizeof(LVVE_CTRL_PARAM_NV_3_T) );
				}
				else
				{
					m_Cmd.SetLVVEDataSize( sizeof(LVVE_CTRL_PARAM_NV_T) );
				}
				//
			}
			//

			//teana hu 2012.02.01
			if(m_bLoadDspCodec)
			{
				for(int j = 0; j < m_DspModePara.nModeCount; j++)
				{
					if(strcmp((const char *) m_DspModePara.lp_audio_nv_dsp_mode_info[j].ucModeName, (const char *)m_AudioData.lpAudioMode[i].mode_name) == 0)
					{
						pPage->m_bExistDspCodec = TRUE;
						pPage->SetDspCodecFlag(m_bDspCodec);
						pPage->SetDspData(m_DspModePara.lp_audio_nv_dsp_mode_info[j].tAudioNvDspModeStruct, m_nVolumeLevel_6530);
						break;
					}
				}

				m_Cmd.SetDspDataSize(sizeof(AUDIO_NV_DSP_MODE_STRUCT_T));
			}
			//
			 m_arrAudioModePages.Add( pPage );
            AddPage( pPage );
			
            // Some special data need to be loaded
			// pPage->LoadFromFile( ofFile );
        }
		
        if(fpRead != NULL)
		{
			fclose(fpRead);
			fpRead = NULL;
		}
		
        UpdatePageTitle();
        
        m_bLoad = TRUE;
		
		return m_bLoad;
}

void CAudioTesterSheet::ExportMusic()
{
	CString strFilter;
    strFilter.LoadString( IDS_FILTER_NVV );
    CFileDialog dlg( FALSE,
        _T(".nv"),
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        strFilter,
        this );
    if( IDOK != dlg.DoModal() )
    {
        return;
    }

	CStdioFile fSaveFile( dlg.GetFileName(), CFile::modeCreate|CFile::modeWrite|CFile::typeText );
    // Write file verion.
	CString strVersion;
	CXVerInfo verInfo;

	strVersion = _T("// Spreadtrum AudioTester export file for music version 0x0100\n\n");

    fSaveFile.WriteString( strVersion );  

	//teana hu 2010.06.10
	m_strVersion = verInfo.GetSPRDVersionString();
	strVersion.Format(_T("// Export by version: %s\n\n"), m_strVersion);
	fSaveFile.WriteString( strVersion ); 
	//

	if( AUDIO_ARM_APP_TYPE_MIN == m_nArmSerialCount )
	{
		strVersion = _T("// audio_arm 8 serial\n\n");
	}
	else if( AUDIO_ARM_APP_TYPE_ONE == m_nArmSerialCount )
	{
		strVersion = _T("// audio_arm 1 serial\n\n");
	}
	else if(AUDIO_ARM_APP_TYPE_SIX == m_nArmSerialCount)
	{
		strVersion = _T("// audio_arm 6 serial\n\n");
	}
	else
	{
		strVersion = _T("");
	}

    fSaveFile.WriteString( strVersion ); 

	//teana hu 2012.02.10
	if(m_bDspCodec)
	{
		strVersion = _T("// dsp codec 0\n\n");
	}
	else
	{
		strVersion = _T("// dsp codec 1\n\n");
	}
	fSaveFile.WriteString( strVersion ); 

	//

	if(m_bLoadVolume && m_bLoadENHA)
	{
		FetchMusicValue( m_VolumeData, m_eqData_g, m_tunData_g );
	}

    std::vector<NVVALUE_TABLE_ENTRY> vValues;

	if(m_bLoadVolume)
	{
		for( int i=0;i<m_VolumeData.mode_count;i++ )
		{
			::ExportVolume( m_VolumeData.lpVolumeMode[i].volume_data,vValues,m_nArmSerialCount  );
			
			CString strMode;
			if(m_bExportOldNv)
			{
				strMode = "audio\\audio_arm\\";
			}
			else
			{
				strMode = "audio_arm\\audio_arm\\";
			}
			
			strMode += m_VolumeData.lpVolumeMode[i].mode_name;
			strMode += "\\AudioStructure";
			
			CString strContent;
			for(int j = 0; j < vValues.size(); j++)
			{
#ifdef _UNICODE
				TCHAR szName[1024] = {0};
				MultiByteToWideChar(CP_ACP,0,vValues[j].Name,-1,szName,1024);
				if(vValues[j].Type == LONG_ITEM_TYPE)
				{
					strContent.Format(_T("%s\\%s=0x%x\n"), strMode, szName, (DWORD)vValues[j].Value);
				}
				else if(vValues[j].Type == SHORT_ITEM_TYPE)
				{
					strContent.Format(_T("%s\\%s=0x%x\n"), strMode, szName, (WORD)vValues[j].Reserved);
				}
#else
				if(vValues[j].Type == LONG_ITEM_TYPE)
				{
					strContent.Format(_T("%s\\%s=0x%x\n"), strMode, vValues[j].Name, (DWORD)vValues[j].Value);
				}
				else if(vValues[j].Type == SHORT_ITEM_TYPE)
				{
					strContent.Format(_T("%s\\%s=0x%x\n"), strMode, vValues[j].Name, (WORD)vValues[j].Reserved);
				}
#endif
				fSaveFile.WriteString( strContent); 
			}
			vValues.clear();
		}
	}

	if(m_bLoadENHA)
	{
		for( int i=0;i<m_eqData_g.eq_count;i++ )
		{
			::ExportENHA_EQ( m_eqData_g.lpEQMode[i],vValues );

			//teana hu 2012.06.14
			CString strMode = _T("");
			if(m_bExportOldEQNv)
			{
				strMode = "audio\\";	
			}
			else
			{
				strMode = "audio_eq\\untunableEQ\\";
			}
			strMode += m_eqData_g.lpEQMode[i].eq_set_name;
			//
			
			CString strContent;
			for(int j = 0; j < vValues.size(); j++)
			{
#ifdef _UNICODE
				TCHAR szName[1024] = {0};
				MultiByteToWideChar(CP_ACP,0,vValues[j].Name,-1,szName,1024);
				strContent.Format(_T("%s\\%s=0x%x\n"), strMode, szName, (WORD)vValues[j].Reserved);
#else
				strContent.Format(_T("%s\\%s=0x%x\n"), strMode, vValues[j].Name, (WORD)vValues[j].Reserved);
#endif
				fSaveFile.WriteString( strContent); 
			}
			vValues.clear();
		}

		for( i=0;i<m_tunData_g.tun_count;i++ )
		{
			::ExportENHA_Tun( m_tunData_g.lpTunMode[i],vValues );
			
			//teana hu 2012.06.14
			CString strMode = _T("");
			if(m_bExportOldEQNv)
			{
				strMode = "audio\\";
			}
			else
			{
				strMode = "audio_eq\\tunableEQ\\";
			}
			strMode += m_tunData_g.lpTunMode[i].eq_set_name;
			//
			
			CString strContent;
			for(int j = 0; j < vValues.size(); j++)
			{
#ifdef _UNICODE
				TCHAR szName[1024] = {0};
				MultiByteToWideChar(CP_ACP,0,vValues[j].Name,-1,szName,1024);
				strContent.Format(_T("%s\\%s=0x%x\n"), strMode, szName, (WORD)vValues[j].Reserved);
#else
				strContent.Format(_T("%s\\%s=0x%x\n"), strMode, vValues[j].Name, (WORD)vValues[j].Reserved);
#endif
				fSaveFile.WriteString( strContent); 
			}
			vValues.clear();
		}
	}

    fSaveFile.Close();
}

BOOL CAudioTesterSheet::ImportMusic(CString strFileName)
{
	char szLine[1024] = {0};
	char szTmp[128] = {0};
	short wValue = 0;
	LONG lValue = 0;
	DWORD dwSize = 0;
	BYTE byteVolumeStruct[sizeof(VOLUME_STRUCT)] = {0};
	BYTE byteVolumeStruct_8[sizeof(VOLUME_STRUCT_MIN)] = {0};
	BYTE byteVolumeStruct_1[sizeof(VOLUME_STRUCT_ONE)] = {0};
	BYTE byteEQStruct[sizeof(EQ_STRUCT_G_T) - NAME_MAX_LEN] = {0};
	BYTE byteTunStruct[sizeof(TUN_STRUCT_G_T) - NAME_MAX_LEN] = {0};
	BOOL bFirst = TRUE;
	VOLUME_STRUCT_MIN volume_data_8;
	VOLUME_STRUCT_ONE volume_data_1;

	//teana hu 2012.01.30
	BYTE byteVolumeStruct_6[sizeof(VOLUME_STRUCT_SIX)] = {0};
	BYTE byteVolumeStruct_16[sizeof(VOLUME_STRUCT_SIXTEEN)] = {0};
	VOLUME_STRUCT_SIX volume_data_6;
	VOLUME_STRUCT_SIXTEEN volume_data_16;
	//
	
	char szLastEQName[128] = {0};
	char szEQName[128] = {0};
	
	char szLastVolumeName[128] = {0};
	char szVolumeName[128] = {0};
	
	char szLastTunName[128] = {0};
	char szTunName[128] = {0};

	int nArmOffset[180] = {0};

	for(int k =0; k < 180; k++)
	{
		if(k== 24 || k == 25
			|| k == 26 || k == 27 || k == 28 || k == 29
			|| k == 30 || k == 31 || k == 32 || k == 33  
			|| k == 34 || k == 35 || k == 36 || k == 37
			|| k == 38 || k == 39 || k == 40 || k == 41
			|| k == 42 || k == 43 || k == 44 || k == 45
			|| k == 46 || k == 47 || k == 48 || k == 49
			|| k == 50 || k == 51 || k == 52 || k == 53
			|| k == 54
			|| k == 74 || k == 75 || k == 76 || k== 77 || k == 78
			|| k == 79 || k == 80 || k == 81 || k == 82
			|| k == 83 || k == 84 || k == 85 || k == 86
			|| k == 87 || k == 88 || k == 89 || k == 90
			|| k == 91 || k == 92 || k == 93 || k == 94
			|| k == 95 || k == 96 || k == 97 || k == 98
			|| k == 99 || k == 100 || k == 101 || k == 102
			|| k == 103 || k == 104 || k == 63 || k == 113)
		{
			nArmOffset[k] = 4;
		}
		else
		{
			nArmOffset[k] = 2;
		}
	}
	
	
	FILE *fpRead = NULL;
#ifdef _UNICODE
	fpRead = _wfopen(strFileName, _T("r"));
#else
	fpRead = fopen(strFileName, "r");
#endif		
	if(fpRead == NULL)
	{
		m_bLoadVolume = FALSE;
		m_bLoadENHA = FALSE;
		return FALSE;
	}
	
	m_VolumeData.mode_count = 0;
	m_eqData_g.eq_count = 0;
	m_tunData_g.tun_count = 0;
	m_nArmSerialCount = AUDIO_ARM_APP_TYPE_MAX;
	m_nDspCodecInfo = 1;
	
	//get mode count
	while(!feof(fpRead))
	{
		if(fgets(szLine, 1024, fpRead) != NULL)
		{
			if(strstr(szLine, "ModeName\\") != NULL)
			{
				continue;
			}

			//teana hu 2011.05.10
			if(strstr(szLine, "// audio_arm 8 serial") != NULL)
			{
				m_nArmSerialCount = AUDIO_ARM_APP_TYPE_MIN;
			}
			else if( strstr(szLine, "// audio_arm 1 serial") != NULL)
			{
				m_nArmSerialCount = AUDIO_ARM_APP_TYPE_ONE;
			}
			else if(strstr(szLine, "// audio_arm 6 serial") != NULL)
			{
				m_nArmSerialCount = AUDIO_ARM_APP_TYPE_SIX;
			}
			//

			//teana hu 2012.02.10
			if(strstr(szLine, "// dsp codec 0") != NULL)
			{
				m_nDspCodecInfo = 0;
				if((m_nDspCodecInfo & 0x100) >> 8 == 0)
				{
					m_bDspCodec = TRUE;
				}
				else
				{
					m_bDspCodec = FALSE;
				}
			}
			//

			if(strstr(szLine, "audio\\audio_arm\\") != NULL || strstr(szLine, "audio_arm\\audio_arm\\") != NULL)
			{
				if(strstr(szLine, "audio\\audio_arm\\") != NULL)
				{
					sscanf(szLine, "audio\\audio_arm\\%[a-zA-Z0-9_]%*[\]\\AudioStructure\\", szVolumeName);
				}
				else
				{
					sscanf(szLine, "audio_arm\\audio_arm\\%[a-zA-Z0-9_]%*[\]\\AudioStructure\\", szVolumeName);
				}
				
				if(strcmp(szVolumeName, szLastVolumeName) != 0)
				{
					memset(&szLastVolumeName, 0, 128);
					strcpy(szLastVolumeName, szVolumeName);
					m_VolumeData.mode_count ++;
				}
				//m_bLoadVolume = TRUE;
			}
			else if(strstr(szLine, "audio\\EQ_H") != NULL || strstr(szLine, "audio_eq\\untunableEQ") != NULL)
			{
				CString strTmp = szLine;
				//teana hu 2012.06.14
				if(strstr(szLine, "audio_eq\\untunableEQ") != NULL)
				{
					int nFind = strTmp.Find(_T("\\"));
					if(nFind != -1)
					{
						strTmp.Delete(0, nFind + 1);
					}
				}
				//

				int nFind = strTmp.Find(_T("\\"));
				if(nFind != -1)
				{
					strTmp.Delete(0, nFind + 1);
					nFind = strTmp.Find(_T("\\"));
					if(nFind != -1)
					{
#ifdef _UNICODE
						int nLen = (strTmp.Left(nFind)).GetLength();
						WideCharToMultiByte(CP_ACP,0,strTmp.Left(nFind),-1,szEQName,nLen*2,NULL,NULL);
#else
						strcpy(szEQName, strTmp.Left(nFind));
#endif
					}
				}
				if(strcmp(szEQName, szLastEQName) != 0)
				{
					memset(&szLastEQName, 0, 128);
					strcpy(szLastEQName, szEQName);
					m_eqData_g.eq_count ++;
				}
			}
			else if(strstr(szLine, "audio\\EQ_T") != NULL || strstr(szLine, "audio_eq\\tunableEQ") != NULL)
			{
				CString strTmp = szLine;
				//teana hu 2012.06.14
				if(strstr(szLine, "audio_eq\\tunableEQ") != NULL)
				{
					int nFind = strTmp.Find(_T("\\"));
					if(nFind != -1)
					{
						strTmp.Delete(0, nFind + 1);
					}
				}
				//
				int nFind = strTmp.Find(_T("\\"));
				if(nFind != -1)
				{
					strTmp.Delete(0, nFind + 1);
					nFind = strTmp.Find(_T("\\"));
					if(nFind != -1)
					{
#ifdef _UNICODE
						int nLen = (strTmp.Left(nFind)).GetLength();
						WideCharToMultiByte(CP_ACP,0,strTmp.Left(nFind),-1,szTunName,nLen*2,NULL,NULL);
#else
						strcpy(szTunName, strTmp.Left(nFind));
#endif
					}
				}
				if(strcmp(szTunName, szLastTunName) != 0)
				{
					memset(&szLastTunName, 0, 128);
					strcpy(szLastTunName, szTunName);
					m_tunData_g.tun_count ++;
				}
				//m_bLoadENHA = TRUE;
			}
		}
	}
	
	
	m_VolumeData.lpVolumeMode = new VOLUME_MODE[m_VolumeData.mode_count];
	m_eqData_g.lpEQMode = new EQ_STRUCT_G_T[m_eqData_g.eq_count];
	m_tunData_g.lpTunMode = new TUN_STRUCT_G_T[m_tunData_g.tun_count];
	
	fseek(fpRead, 0, SEEK_SET);
	int nIndex = 0;
	BOOL bLastVolume = FALSE;
	BOOL bLastEQ = FALSE;
	int nIndexArm = 0;
	
	while(!feof(fpRead))
	{
		if(fgets(szLine, 1024, fpRead) != NULL)
		{
			if(strstr(szLine, "ModeName\\") != NULL)
			{
				continue;
			}

			LPSTR lpFind = strstr(szLine, "audio\\audio_arm\\");
			LPSTR lpFind1 = strstr(szLine, "audio_arm\\audio_arm\\");
			if(lpFind != NULL || lpFind1 != NULL)
			{
				if(lpFind != NULL)
				{
					sscanf(szLine, "audio\\audio_arm\\%[a-zA-Z0-9_]%*[\]\\AudioStructure\\", szVolumeName);
				}
				else
				{
					sscanf(szLine, "audio_arm\\audio_arm\\%[a-zA-Z0-9_]%*[\]\\AudioStructure\\", szVolumeName);
				}
				
				memset(szTmp, 0, 128);
				LPSTR lpFind = strrchr(szLine, '=');
				if(lpFind != NULL)
				{
					strcpy(szTmp, lpFind+1);
				}
				
				if(strstr(szTmp, "0x") != NULL || strstr(szTmp, "0X") != NULL)
				{
					char* pEnd = NULL;
					lValue = (LONG)strtol(szTmp, &pEnd, 16);
				}
				else
				{
					lValue = atoi(szTmp);
				}
				
				if(strcmp(szVolumeName, szLastVolumeName) != 0)
				{
					if(!bFirst)
					{
						if( AUDIO_ARM_APP_TYPE_MAX == m_nArmSerialCount)
						{
							ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
							ZeroMemory((LPVOID)&volume_data_16, sizeof(VOLUME_STRUCT_SIXTEEN));
							memcpy((LPVOID)&volume_data_16, &byteVolumeStruct_16, sizeof(VOLUME_STRUCT_SIXTEEN));
							ZeroMemory(&byteVolumeStruct_16, sizeof(VOLUME_STRUCT_SIXTEEN));
							m_Cmd.CopyVolumeStructFrom16ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_16);
							memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
						}
						else if( AUDIO_ARM_APP_TYPE_MIN == m_nArmSerialCount )
						{
							ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
							ZeroMemory((LPVOID)&volume_data_8, sizeof(VOLUME_STRUCT_MIN));
							memcpy((LPVOID)&volume_data_8, &byteVolumeStruct_8, sizeof(VOLUME_STRUCT_MIN));
							ZeroMemory(&byteVolumeStruct_8, sizeof(VOLUME_STRUCT_MIN));
							m_Cmd.CopyVolumeStructFrom8ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_8);
							memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
						}
						else if( AUDIO_ARM_APP_TYPE_ONE == m_nArmSerialCount )
						{
							ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
							ZeroMemory((LPVOID)&volume_data_1, sizeof(VOLUME_STRUCT_ONE));
							memcpy((LPVOID)&volume_data_1, &byteVolumeStruct_1, sizeof(VOLUME_STRUCT_ONE));
							ZeroMemory(&byteVolumeStruct_1, sizeof(VOLUME_STRUCT_ONE));
							m_Cmd.CopyVolumeStructFrom1ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_1);
							memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
						}
						else if(AUDIO_ARM_APP_TYPE_SIX == m_nArmSerialCount)
						{
							ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
							ZeroMemory((LPVOID)&volume_data_6, sizeof(VOLUME_STRUCT_SIX));
							memcpy((LPVOID)&volume_data_6, &byteVolumeStruct_6, sizeof(VOLUME_STRUCT_SIX));
							ZeroMemory(&byteVolumeStruct_6, sizeof(VOLUME_STRUCT_SIX));
							m_Cmd.CopyVolumeStructFrom6ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_6);
							memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
						}
						dwSize = 0;
						nIndexArm = 0;
						nIndex ++;
					}
					else
					{	
						bFirst = FALSE;
					}
					if( AUDIO_ARM_APP_TYPE_MAX == m_nArmSerialCount )
					{
						memcpy(byteVolumeStruct_16 + dwSize, &lValue, sizeof(WORD));
						dwSize += sizeof(WORD);
					}
					else if( AUDIO_ARM_APP_TYPE_MIN == m_nArmSerialCount )
					{
						memcpy(byteVolumeStruct_8 + dwSize, &lValue, sizeof(WORD));
						dwSize += sizeof(WORD);
					}
					else if(AUDIO_ARM_APP_TYPE_ONE == m_nArmSerialCount)
					{
						memcpy(byteVolumeStruct_1 + dwSize, &lValue, sizeof(WORD));
						dwSize += sizeof(WORD);
					}
					else if(AUDIO_ARM_APP_TYPE_SIX == m_nArmSerialCount)
					{
						memcpy(byteVolumeStruct_6 + dwSize, &lValue, nArmOffset[nIndexArm]);
						dwSize += nArmOffset[nIndexArm];
						nIndexArm ++;
					}
					
					
					memset(&szLastVolumeName, 0, 128);
					strcpy(szLastVolumeName, szVolumeName);
				}
				else
				{
					if( AUDIO_ARM_APP_TYPE_MAX == m_nArmSerialCount )
					{
						memcpy(byteVolumeStruct_16 + dwSize, &lValue, sizeof(WORD));
						dwSize += sizeof(WORD);
					}
					else if( AUDIO_ARM_APP_TYPE_MIN == m_nArmSerialCount )
					{
						memcpy(byteVolumeStruct_8 + dwSize, &lValue, sizeof(WORD));
						dwSize += sizeof(WORD);
					}
					else if(AUDIO_ARM_APP_TYPE_ONE == m_nArmSerialCount)
					{
						memcpy(byteVolumeStruct_1 + dwSize, &lValue, sizeof(WORD));
						dwSize += sizeof(WORD);
					}
					else if(AUDIO_ARM_APP_TYPE_SIX == m_nArmSerialCount)
					{
						memcpy(byteVolumeStruct_6 + dwSize, &lValue, nArmOffset[nIndexArm]);
						dwSize += nArmOffset[nIndexArm];
						nIndexArm ++;
					}
					
				}
			}
			else if(strstr(szLine, "audio\\EQ_H") != NULL || strstr(szLine, "audio_eq\\untunableEQ") != NULL)
			{
				lpFind = strstr(szLine, "audio\\EQ_H" );
				lpFind1 = strstr(szLine, "audio_eq\\untunableEQ");
				if(lpFind != NULL || lpFind1 != NULL)
				{
					if(!bLastVolume)
					{
						bLastVolume = TRUE;
						bFirst = TRUE;
						//for last audio mode
						if( AUDIO_ARM_APP_TYPE_MAX == m_nArmSerialCount)
						{
							ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
							ZeroMemory((LPVOID)&volume_data_16, sizeof(VOLUME_STRUCT_SIXTEEN));
							memcpy((LPVOID)&volume_data_16, &byteVolumeStruct_16, sizeof(VOLUME_STRUCT_SIXTEEN));
							ZeroMemory(&byteVolumeStruct_16, sizeof(VOLUME_STRUCT_SIXTEEN));
							m_Cmd.CopyVolumeStructFrom16ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_16);
							memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
						}
						else if( AUDIO_ARM_APP_TYPE_MIN == m_nArmSerialCount )
						{
							ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
							ZeroMemory((LPVOID)&volume_data_8, sizeof(VOLUME_STRUCT_MIN));
							memcpy((LPVOID)&volume_data_8, &byteVolumeStruct_8, sizeof(VOLUME_STRUCT_MIN));
							ZeroMemory(&byteVolumeStruct_8, sizeof(VOLUME_STRUCT_MIN));
							m_Cmd.CopyVolumeStructFrom8ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_8);
							memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
						}
						else if( AUDIO_ARM_APP_TYPE_ONE == m_nArmSerialCount )
						{
							ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
							ZeroMemory((LPVOID)&volume_data_1, sizeof(VOLUME_STRUCT_ONE));
							memcpy((LPVOID)&volume_data_1, &byteVolumeStruct_1, sizeof(VOLUME_STRUCT_ONE));
							ZeroMemory(&byteVolumeStruct_1, sizeof(VOLUME_STRUCT_ONE));
							m_Cmd.CopyVolumeStructFrom1ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_1);
							memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
						}
						else if(AUDIO_ARM_APP_TYPE_SIX == m_nArmSerialCount)
						{
							ZeroMemory( (LPVOID)&m_VolumeData.lpVolumeMode[nIndex].volume_data,sizeof( VOLUME_STRUCT ) );
							ZeroMemory((LPVOID)&volume_data_6, sizeof(VOLUME_STRUCT_SIX));
							memcpy((LPVOID)&volume_data_6, &byteVolumeStruct_6, sizeof(VOLUME_STRUCT_SIX));
							ZeroMemory(&byteVolumeStruct_6, sizeof(VOLUME_STRUCT_SIX));
							m_Cmd.CopyVolumeStructFrom6ToMax(m_VolumeData.lpVolumeMode[nIndex].volume_data, volume_data_6);
							memcpy(m_VolumeData.lpVolumeMode[nIndex].mode_name, szLastVolumeName, strlen(szLastVolumeName)+1);
						}
						dwSize = 0;
						nIndex = 0;						
					}
					
					CString strTmp = szLine;
					//teana hu 2012.06.14
					if(lpFind1 != NULL)
					{
						int nFind = strTmp.Find(_T("\\"));
						if(nFind != -1)
						{
							strTmp.Delete(0, nFind + 1);
						}
					}
					//
					
					int nFind = strTmp.Find(_T("\\"));
					if(nFind != -1)
					{
						strTmp.Delete(0, nFind + 1);
						nFind = strTmp.Find(_T("\\"));
						if(nFind != -1)
						{
#ifdef _UNICODE
							int nLen = (strTmp.Left(nFind)).GetLength();
							WideCharToMultiByte(CP_ACP,0,strTmp.Left(nFind),-1,szEQName,nLen*2,NULL,NULL);
#else
							strcpy(szEQName, strTmp.Left(nFind));
#endif
						}
					}

					
					memset(szTmp, 0, 128);
					LPSTR lpFind = strrchr(szLine, '=');
					if(lpFind != NULL)
					{
						strcpy(szTmp, lpFind+1);
					}
					
					if(strstr(szTmp, "0x") != NULL || strstr(szTmp, "0X") != NULL)
					{
						char* pEnd = NULL;
						wValue = (WORD)strtol(szTmp, &pEnd, 16);
					}
					else
					{
						wValue = atoi(szTmp);
					}
					
					if(strcmp(szEQName, szLastEQName) != 0)
					{
						if(!bFirst)
						{
							ZeroMemory( (LPVOID)&m_eqData_g.lpEQMode[nIndex],sizeof( EQ_STRUCT_G_T) );
							memcpy((LPVOID)(&(m_eqData_g.lpEQMode[nIndex].eq_control)), &byteEQStruct, sizeof(EQ_STRUCT_G_T) - NAME_MAX_LEN);
							ZeroMemory(&byteEQStruct, sizeof(EQ_STRUCT_G_T) - NAME_MAX_LEN);
							memcpy(m_eqData_g.lpEQMode[nIndex].eq_set_name, szLastEQName, strlen(szLastEQName)+1);
							dwSize = 0;
							nIndex ++;
						}
						else
						{
							bFirst = FALSE;
						}
						memcpy(byteEQStruct + dwSize, &wValue, sizeof(WORD));
						dwSize += sizeof(WORD);
						memset(&szLastEQName, 0, 128);
						strcpy(szLastEQName, szEQName);
					}
					else
					{
						memcpy(byteEQStruct + dwSize, &wValue, sizeof(WORD));
						dwSize += sizeof(WORD);
					}
					continue;
				}
			}
			else if(strstr(szLine, "audio\\EQ_T") != NULL || strstr(szLine, "audio_eq\\tunableEQ") != NULL)
			{
				lpFind = strstr(szLine, "audio\\EQ_T");
				lpFind1 = strstr(szLine, "audio_eq\\tunableEQ");

				if(lpFind != NULL || lpFind1 != NULL)
				{
					if(!bLastEQ)
					{
						bLastEQ = TRUE;
						bFirst = TRUE;
						//for last eq mode
						ZeroMemory( (LPVOID)&m_eqData_g.lpEQMode[nIndex],sizeof( EQ_STRUCT_G_T));
						memcpy((LPVOID)(&(m_eqData_g.lpEQMode[nIndex].eq_control)), &byteEQStruct, sizeof(EQ_STRUCT_G_T) - NAME_MAX_LEN);
						ZeroMemory(&byteEQStruct, sizeof(EQ_STRUCT_G_T) - NAME_MAX_LEN);
						memcpy(m_eqData_g.lpEQMode[nIndex].eq_set_name, szLastEQName, strlen(szLastEQName)+1);
						dwSize = 0;
						nIndex = 0;
					}
					
					CString strTmp = szLine;
					//teana hu 2012.06.14
					if(lpFind1 != NULL)
					{
						int nFind = strTmp.Find(_T("\\"));
						if(nFind != -1)
						{
							strTmp.Delete(0, nFind + 1);
						}
					}
					//
					int nFind = strTmp.Find(_T("\\"));
					if(nFind != -1)
					{
						strTmp.Delete(0, nFind + 1);
						nFind = strTmp.Find(_T("\\"));
						if(nFind != -1)
						{
#ifdef _UNICODE
							int nLen = (strTmp.Left(nFind)).GetLength();
							WideCharToMultiByte(CP_ACP,0,strTmp.Left(nFind),-1,szEQName,nLen*2,NULL,NULL);
#else
							strcpy(szEQName, strTmp.Left(nFind));
#endif
						}
					}
					
					memset(szTmp, 0, 128);
					LPSTR lpFind = strrchr(szLine, '=');
					if(lpFind != NULL)
					{
						strcpy(szTmp, lpFind+1);
					}
					
					if(strstr(szTmp, "0x") != NULL || strstr(szTmp, "0X") != NULL)
					{
						char* pEnd = NULL;
						wValue = (WORD)strtol(szTmp, &pEnd, 16);
					}
					else
					{
						wValue = atoi(szTmp);
					}
					
					if(strcmp(szTunName, szLastTunName) != 0)
					{
						if(!bFirst)
						{
							ZeroMemory( (LPVOID)&m_tunData_g.lpTunMode[nIndex],sizeof( TUN_STRUCT_G_T));
							memcpy((LPVOID)(&(m_tunData_g.lpTunMode[nIndex].eq_control)), &byteTunStruct, sizeof(TUN_STRUCT_G_T) - NAME_MAX_LEN);
							ZeroMemory(&byteTunStruct, sizeof(TUN_STRUCT_G_T) - NAME_MAX_LEN);
							memcpy(m_tunData_g.lpTunMode[nIndex].eq_set_name, szLastTunName, strlen(szLastTunName)+1);
							dwSize = 0;
							nIndex ++;
						}
						else
						{
							bFirst = FALSE;
						}
						memcpy(byteTunStruct + dwSize, &wValue, sizeof(WORD));
						dwSize += sizeof(WORD);
						memset(&szLastTunName, 0, 128);
						strcpy(szLastTunName, szTunName);
					}
					else
					{
						memcpy(byteTunStruct + dwSize, &wValue, sizeof(WORD));
						dwSize += sizeof(WORD);
					}
					continue;
				}
			}	
				memset(szLine, 0, 1024);
			}
		}
		
		//for last mode
		ZeroMemory( (LPVOID)&m_tunData_g.lpTunMode[nIndex],sizeof( TUN_STRUCT_G_T));
		memcpy((LPVOID)(&(m_tunData_g.lpTunMode[nIndex].eq_control )), &byteTunStruct, sizeof(TUN_STRUCT_G_T) - NAME_MAX_LEN);
		ZeroMemory(&byteTunStruct, sizeof(TUN_STRUCT_G_T) - NAME_MAX_LEN);
		memcpy(m_tunData_g.lpTunMode[nIndex].eq_set_name, szLastTunName, strlen(szLastTunName)+1);
		
		
        //m_Cmd.SetAudioDataSize( 740 );
		
        for( int i=0;i<m_VolumeData.mode_count;i++ )
        {
            CMusicModePage* pPage = new CMusicModePage;
            
			CString strTitle(m_VolumeData.lpVolumeMode[i].mode_name);
            pPage->SetTitle( strTitle );
			pPage->SetDspCodecFlag(m_bDspCodec);
            pPage->SetValue( m_VolumeData.lpVolumeMode[i].volume_data);
			if( AUDIO_ARM_APP_TYPE_MAX == m_nArmSerialCount )
			{
				m_Cmd.SetVolumeDataSize( sizeof(VOLUME_STRUCT_SIXTEEN));
			}
			else if( AUDIO_ARM_APP_TYPE_MIN == m_nArmSerialCount )
			{
				m_Cmd.SetVolumeDataSize( sizeof(VOLUME_STRUCT_MIN));
			}
			else if( AUDIO_ARM_APP_TYPE_ONE == m_nArmSerialCount)
			{
				m_Cmd.SetVolumeDataSize( sizeof(VOLUME_STRUCT_ONE));
			}
			else if( AUDIO_ARM_APP_TYPE_SIX == m_nArmSerialCount)
			{
				m_Cmd.SetVolumeDataSize( sizeof(VOLUME_STRUCT_SIX));
			}
			pPage->SetPageIndex(i);
			//teana hu 2012.06.14
			if(strcmp((const char *)m_VolumeData.lpVolumeMode[i].mode_name, "Handsfree") == 0)
			{
				
				int n = (m_VolumeData.lpVolumeMode[i].volume_data.app_config_info_set.aud_proc_exp_control[1] & 0x8000) >>15;
				if(n == 0)
				{
					m_bSerialPara = FALSE;
				}
				else
				{
					m_bSerialPara = TRUE;
					m_bExportOldEQNv = FALSE;
				}
			}
			//
			for(int j = 0; j < m_eqData_g.eq_count; j++)
			{
				if(strstr((const char *) m_eqData_g.lpEQMode[j].eq_set_name, (const char *)m_VolumeData.lpVolumeMode[i].mode_name) != NULL)
				{
					pPage->SetEQValue( m_eqData_g.lpEQMode[j]);
					pPage->SetEQIndex(j);
					break;
				}
			}
			pPage->SetTunValue( m_tunData_g.lpTunMode[0]);
			pPage->SetTunIndex(0);
			m_arrMusicModePages.Add( pPage );
            AddPage( pPage );
        }
		
        if(fpRead != NULL)
		{
			fclose(fpRead);
			fpRead = NULL;
		}
		
        UpdateMusicPageTitle();
        
        m_bLoadVolume = TRUE;
		m_bLoadENHA = TRUE;
		
		return TRUE;
}

void CAudioTesterSheet::FetchValue( AUDIO_DATA& audio_data )
{
    audio_data.mode_count = m_arrAudioModePages.GetSize();
    
    for( int i=0;i<audio_data.mode_count;i++ )
    {
        m_arrAudioModePages[i]->FetchValue( audio_data.lpAudioMode[i] );
    }

	if(m_bLoadVolume)
	{
		FetchVolumeValue( m_VolumeData );
	}

	if(m_bLoadLVVE)
	{
		FetchLVVEValue( m_LVVEPara );
	}

	if(m_bLoadDspCodec)
	{
		FetchDspvalue(m_DspModePara);
	}
}

void CAudioTesterSheet::FetchMusicValue(VOLUME_DATA& volume_data , EQ_DATA_G& eqData, TUN_DATA_G& tunData)
{
//	eqData.eq_count = m_arrMusicModePages.GetSize();
//	tunData.tun_count = 1;

	volume_data.mode_count = m_arrMusicModePages.GetSize();
    
	if(!m_bSerialPara)
	{
		for( int i=0;i<eqData.eq_count;i++ )
		{
			m_arrMusicModePages[i]->FetchENHAValue( eqData.lpEQMode[i], tunData.lpTunMode[0] );
		}
	}
	else
	{
		for( int i=0;i<volume_data.mode_count;i++ )
		{
			int nIndex = -1;
			m_arrMusicModePages[i]->FetchParaSetIndex(nIndex);
			if(nIndex != -1)
			{
				m_arrMusicModePages[i]->FetchENHAValue( eqData.lpEQMode[nIndex], tunData.lpTunMode[0] );
			}
		}

	}

    for(int i=0;i<volume_data.mode_count;i++ )
    {
		m_arrMusicModePages[i]->FetchValue( volume_data.lpVolumeMode[i].volume_data);
    }
}

void CAudioTesterSheet::FetchVolumeValue( VOLUME_DATA& volume_data )
{  
	int nCount = m_arrAudioModePages.GetSize();
    for( int i=0;i<volume_data.mode_count;i++ )
    {
		for(int j = 0; j < nCount; j++)
		{
			CString strText;
			m_arrAudioModePages[j]->GetTitle( strText );
#ifdef _UNICODE
			char szTmp[16] = {0};
			WideCharToMultiByte(CP_ACP,0,strText,-1,szTmp,16,NULL,NULL);
			if(strcmp(szTmp, (const char *)volume_data.lpVolumeMode[i].mode_name) == 0)
			{
				m_arrAudioModePages[j]->FetchVolumeValue( volume_data.lpVolumeMode[i] );
				break;
			}
#else
			if(strcmp(strText, (const char *)volume_data.lpVolumeMode[i].mode_name) == 0)
			{
				m_arrAudioModePages[j]->FetchVolumeValue( volume_data.lpVolumeMode[i] );
				break;
			}
#endif
		}
    }
}

void CAudioTesterSheet::FetchLVVEValue( LVVE_CTRL_PARAM_NV_LIST_T& lvve_data )
{
	int nCount = m_arrAudioModePages.GetSize();
    for( int i=0;i<lvve_data.mode_count;i++ )
    {
		for(int j = 0; j < nCount; j++)
		{
			CString strText;
			m_arrAudioModePages[j]->GetTitle( strText );
#ifdef _UNICODE
			char szTmp[16] = {0};
			WideCharToMultiByte(CP_ACP,0,strText,-1,szTmp,16,NULL,NULL);
			if(strcmp(szTmp, (const char *)lvve_data.lp_lvve_ctrl_nv_params[i].ucModeName) == 0)
			{
				m_arrAudioModePages[j]->FetchLVVEData( lvve_data.lp_lvve_ctrl_nv_params[i] );
				break;
			}
#else
			if(strcmp(strText, (const char *)lvve_data.lp_lvve_ctrl_nv_params[i].ucModeName) == 0)
			{
				m_arrAudioModePages[j]->FetchLVVEData( lvve_data.lp_lvve_ctrl_nv_params[i] );
				break;
			}
#endif
		}
    }
}

void CAudioTesterSheet::FetchDspvalue(AUDIO_NV_DSP_MODE_LIST_T& dsp_data)
{
	int nCount = m_arrAudioModePages.GetSize();
    for( int i=0;i<dsp_data.nModeCount;i++ )
    {
		for(int j = 0; j < nCount; j++)
		{
			CString strText;
			m_arrAudioModePages[j]->GetTitle( strText );
#ifdef _UNICODE
			char szTmp[16] = {0};
			WideCharToMultiByte(CP_ACP,0,strText,-1,szTmp,16,NULL,NULL);
			if(strcmp(szTmp, (const char *)dsp_data.lp_audio_nv_dsp_mode_info[i].ucModeName) == 0)
			{
				m_arrAudioModePages[j]->FetchDspData( dsp_data.lp_audio_nv_dsp_mode_info[i].tAudioNvDspModeStruct );
				break;
			}
#else
			if(strcmp(strText, (const char *)dsp_data.lp_audio_nv_dsp_mode_info[i].ucModeName) == 0)
			{
				m_arrAudioModePages[j]->FetchDspData( dsp_data.lp_audio_nv_dsp_mode_info[i].tAudioNvDspModeStruct );
				break;
			}
#endif
		}
    }
}

BOOL CAudioTesterSheet::ConfirmLoad(CString strText, BOOL bClear)
{
    if( m_bLoad )
    {
		CString strFormat;
		strFormat.LoadString(IDS_LOAD_AUDIO_MODE_AGAIN);
		CString strPrompt;
		strPrompt.Format( strFormat, strText );
        int nRet = AfxMessageBox( strPrompt, MB_YESNO );
        if( IDYES == nRet )
        {
            // Remove all loaded mode pages
			if(bClear)
			{
				ClearAllModes();
			}
            m_bLoad = FALSE;
        }
    }

    return !m_bLoad;
}

BOOL CAudioTesterSheet::ConfirmLoadMusic(CString strText, BOOL bClear)
{
	if( m_bLoadENHA )
	{
		CString strFormat;
		strFormat.LoadString(IDS_LOAD_MUSIC_MODE_AGAIN);
		CString strPrompt;
		strPrompt.Format( strFormat, strText );
		int nRet = AfxMessageBox( strPrompt, MB_YESNO );
		if( IDYES == nRet )
		{
			// Remove all loaded mode pages
			if(bClear)
			{
				ClearAllMusicModes();
			}
			m_bLoadENHA = FALSE;
		}
	}
	
	return !m_bLoadENHA;
	
}

BOOL CAudioTesterSheet::Connect( CString& strAddr,DWORD dwPort )
{
#ifdef _UNICODE
	char szAddr[20] = {0};
	WideCharToMultiByte(CP_ACP,0,strAddr.GetBuffer(0),-1,szAddr,20,NULL,NULL);
    return m_Cmd.Connect( szAddr,dwPort );
#else
	return m_Cmd.Connect( strAddr.GetBuffer(0),dwPort );
#endif
}

BOOL CAudioTesterSheet::ConnectUart( DWORD dwPort,DWORD dwBaudrate )
{
	return m_Cmd.Connect( dwPort,dwBaudrate );
}
 
BOOL CAudioTesterSheet::Disconnect()
{
    return m_Cmd.Disconnect();
}
BOOL CAudioTesterSheet::SetVolume( int nVolume )
{
    return m_Cmd.SetVolume( nVolume );
}

BOOL CAudioTesterSheet::SetSpeaker( int nSpeaker )
{
    return m_Cmd.SetSpeaker( nSpeaker );
}

BOOL CAudioTesterSheet::SetMute( int nMute )
{
    return m_Cmd.SetMute( nMute );
}

BOOL CAudioTesterSheet::MakeCall( CString strPhoneNumber )
{
    return m_Cmd.MakeCall( strPhoneNumber );
}

BOOL CAudioTesterSheet::Hangup()
{
    return m_Cmd.Hangup();
}

BOOL CAudioTesterSheet::WriteParaToPhone( int nIndex )
{
    CAudioModePage* pPage = m_arrAudioModePages[nIndex];
    if(m_bLoadVolume)
	{
		for(int i = 0; i < m_VolumeData.mode_count; i++)
		{
			if(strcmp((const char *)m_VolumeData.lpVolumeMode[i].mode_name, (const char *)m_AudioData.lpAudioMode[nIndex].mode_name) == 0)
			{
				if(pPage != NULL)
				{
					m_Cmd.SetVolumeData(i, SET_ARM_VOLUME_DATA_TO_RAM, m_VolumeData.lpVolumeMode[i].mode_name,m_VolumeData.lpVolumeMode[i].volume_data);
					break;
				}
			}
		}
	}
	 //teana hu 2011.04.18
	 if(m_bLoadLVVE)
	 {
		 for(int i = 0; i < m_LVVEPara.mode_count; i++)
		 {
			 if(strcmp((const char *)m_LVVEPara.lp_lvve_ctrl_nv_params[i].ucModeName, (const char *)m_AudioData.lpAudioMode[nIndex].mode_name) == 0)
			 {
				 if(pPage != NULL)
				 {
					 m_Cmd.SetLVVEData(i, SET_LVVE_DATA_TO_RAM, m_LVVEPara.lp_lvve_ctrl_nv_params[i].ucModeName,m_LVVEPara.lp_lvve_ctrl_nv_params[i]);
					 break;
				 }
			 }
		 }
	 }
	 //

	 //teana hu 2012.01.31
	 if(m_bLoadDspCodec)
	 {
		 for(int i = 0; i < m_DspModePara.nModeCount; i++)
		 {
			 if(strcmp((const char *)m_DspModePara.lp_audio_nv_dsp_mode_info[i].ucModeName, (const char *)m_AudioData.lpAudioMode[nIndex].mode_name) == 0)
			 {
				 if(pPage != NULL)
				 {
					 m_Cmd.SetDspData(i, SET_AUDIO_DSP_DATA_TO_RAM, m_DspModePara.lp_audio_nv_dsp_mode_info[i].ucModeName,m_DspModePara.lp_audio_nv_dsp_mode_info[i].tAudioNvDspModeStruct);
					 break;
				 }
			 }
		 }
	 }
	 //

    BOOL bRet = m_Cmd.SetModeData( nIndex,m_AudioData.lpAudioMode[nIndex].mode_name,m_AudioData.lpAudioMode[nIndex].audio_data );
	if(!bRet)
	{
		return FALSE;
	}
	bRet = m_Cmd.SetAudMode(m_AudioData.lpAudioMode[nIndex].mode_name);

	return bRet;
	
}

BOOL CAudioTesterSheet::WriteParaToFlash( int nIndex )
{
    CAudioModePage* pPage = m_arrAudioModePages[nIndex];

	if(m_bLoadVolume)
	{
		for(int i = 0; i < m_VolumeData.mode_count; i++)
		{
			if(strcmp((const char *)m_VolumeData.lpVolumeMode[i].mode_name, (const char *)m_AudioData.lpAudioMode[nIndex].mode_name) == 0)
			{
				if(pPage != NULL )
				{
					m_Cmd.SetVolumeData(i, SET_ARM_VOLUME_DATA_TO_FLASH, m_VolumeData.lpVolumeMode[i].mode_name,m_VolumeData.lpVolumeMode[i].volume_data);
					break;
				}
			}
		}
	}
	//teana hu 2011.04.18
	if(m_bLoadLVVE)
	{
		for(int i = 0; i < m_LVVEPara.mode_count; i++)
		{
			if(strcmp((const char *)m_LVVEPara.lp_lvve_ctrl_nv_params[i].ucModeName, (const char *)m_AudioData.lpAudioMode[nIndex].mode_name) == 0)
			{
				if(pPage != NULL)
				{
					m_Cmd.SetLVVEData(i, SET_LVVE_DATA_TO_FLASH, m_LVVEPara.lp_lvve_ctrl_nv_params[i].ucModeName,m_LVVEPara.lp_lvve_ctrl_nv_params[i]);
					break;
				}
			}
		}
	 }
	//

	//teana hu 2012.01.31
	if(m_bLoadDspCodec)
	{
		for(int i = 0; i < m_DspModePara.nModeCount; i++)
		{
			if(strcmp((const char *)m_DspModePara.lp_audio_nv_dsp_mode_info[i].ucModeName, (const char *)m_AudioData.lpAudioMode[nIndex].mode_name) == 0)
			{
				if(pPage != NULL)
				{
					m_Cmd.SetDspData(i, SET_AUDIO_DSP_DATA_TO_FLASH, m_DspModePara.lp_audio_nv_dsp_mode_info[i].ucModeName,m_DspModePara.lp_audio_nv_dsp_mode_info[i].tAudioNvDspModeStruct);
					break;
				}
			}
		}
	}
	 //
    
    return m_Cmd.SetModeData2Flash( nIndex,m_AudioData.lpAudioMode[nIndex].mode_name,m_AudioData.lpAudioMode[nIndex].audio_data );
}

BOOL CAudioTesterSheet::WriteVolumeParam(int nIndex, int nType)
{

	BOOL bRet = m_Cmd.SetVolumeData(nIndex, nType, m_VolumeData.lpVolumeMode[nIndex].mode_name,m_VolumeData.lpVolumeMode[nIndex].volume_data);
	if(!bRet)
	{
		return FALSE;
	}
	if(nType == SET_ARM_VOLUME_DATA_TO_RAM)
	{
		bRet = m_Cmd.SetAudMode(m_VolumeData.lpVolumeMode[nIndex].mode_name);
	}

	return bRet;
}

BOOL CAudioTesterSheet::GetVolumeParam(int nIndex, int nType)
{
	return m_Cmd.GetVolumeData(nIndex, nType, m_VolumeData.lpVolumeMode[nIndex].volume_data);
}

BOOL CAudioTesterSheet::WriteDspCodecParam(int nIndex, int nType)
{
	BOOL bRet = m_Cmd.SetDspData(nIndex, nType, m_DspModePara.lp_audio_nv_dsp_mode_info[nIndex].ucModeName,m_DspModePara.lp_audio_nv_dsp_mode_info[nIndex].tAudioNvDspModeStruct);
	if(!bRet)
	{
		return FALSE;
	}
}

BOOL CAudioTesterSheet::LoadDspCodecParam(int nIndex, int nType)
{
	return m_Cmd.GetDspData(nIndex, nType, m_DspModePara.lp_audio_nv_dsp_mode_info[nIndex].tAudioNvDspModeStruct);
}

BOOL CAudioTesterSheet::WriteEQParaToPhone( int nIndex )
{
	return m_Cmd.SetEQData( nIndex+1, m_EQData.lpEQMode[nIndex] );
}

BOOL CAudioTesterSheet::WriteEQParaToFlash( int nIndex )
{
	return m_Cmd.SetEQData( nIndex+1, m_EQData.lpEQMode[nIndex], 1);
}

BOOL CAudioTesterSheet::WriteAllParaToPhone()
{
    BOOL bRet = FALSE;
    for( int i=0;i<m_arrAudioModePages.GetSize();i++ )
    {
        bRet = WriteParaToPhone( i );
        if( !bRet )
        {
            break;
        }
    }

    return bRet;
}

void CAudioTesterSheet::AddLogWindow()
{
    const int OFFSET_V = 10;
    const int SPACE_V  = 95;
    
    CRect rcSheet;
    GetWindowRect( &rcSheet );
    SetWindowPos( NULL, 0, 0, rcSheet.Width(), rcSheet.Height() + SPACE_V, SWP_NOMOVE );
    
    CRect rcButton;
    int ids[] = { IDOK, IDCANCEL, ID_APPLY_NOW, IDHELP };
    for ( int i = 0; i < sizeof(ids)/sizeof(int); i++ )
    {
        GetDlgItem( ids[i] )->GetWindowRect( &rcButton );
        ScreenToClient( &rcButton );
        GetDlgItem( ids[i] )->SetWindowPos( NULL, rcButton.left, rcButton.top + SPACE_V, 0, 0, SWP_NOSIZE );
    }
    
    CRect rcPage;
    GetTabControl()->GetWindowRect( &rcPage );
    CRect rcLog( rcPage.left, rcPage.bottom + OFFSET_V, rcPage.right, rcPage.bottom + SPACE_V );
    ScreenToClient( &rcLog );
    m_wndLog.Create( WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL|LBS_OWNERDRAWVARIABLE, rcLog, this, IDC_LOG );
    m_wndLog.SetFont( &m_Font );
    m_wndLog.SetHorizontalExtent( 1000 );
}


BOOL CAudioTesterSheet::OnCommand(WPARAM wParam, LPARAM lParam) 
{
    if ( IDCANCEL == wParam )
    {
        // Cancel button has been treated as "Clear Log"
        ClearLog();
        return TRUE;
    }
    else if( IDHELP == wParam )
    {
        // Show about dialog
        CXAboutDlg dlg;
		dlg.SetProductName(_T("AudioTester"));
        dlg.DoModal();
        return TRUE;
    }
    
	return CPropertySheet::OnCommand(wParam, lParam);
}

void CAudioTesterSheet::ClearLog()
{
    m_wndLog.ResetContent();
    m_wndLog.ClearString();
}

void CAudioTesterSheet::Log( LPCTSTR lpszInfo,BOOL bErrMsg /*= FALSE */)
{
    CColorListBox::ITEM_FACE temp;
    temp.m_strShowString = lpszInfo;
    if( bErrMsg )
    {
        temp.m_color = ERROR_COLOR;
    }
    m_wndLog.AddStringEx( temp );
    m_wndLog.SetTopIndex( m_wndLog.GetCount() - 1 );
    if( !bErrMsg )
    {
         m_wndLog.SetFocus();
        m_wndLog.SetCurSel (m_wndLog.GetCount() - 1);
    }
}

void CAudioTesterSheet::DecideAudioVersion()
{
    // Because audio data size in all modes are same
    // so we get the last result
    int nSize = m_Cmd.GetAudioDataSize();
    if( nSize == EXTEND_VERSION_SIZE )
    {
        m_bExtendData = TRUE;
        m_bRocky = FALSE;
		m_bAudio3 = FALSE;
    }
    else if( nSize == ROCKY_VERSION_SIZE )
    {
        m_bRocky = TRUE;
        m_bExtendData = TRUE;
		m_bAudio3 = FALSE;
    }
	else if( nSize == AUDIO_VERSION_3_SIZE )
	{
		m_bRocky = TRUE;
		m_bExtendData = TRUE;
		m_bAudio3 = TRUE;
	}
    else
    {
        m_bExtendData = FALSE;
        m_bRocky = FALSE;
		m_bAudio3 = FALSE;
    }
}

BOOL CAudioTesterSheet::WriteDefaultFirToPhone( int nIndex )
{
    CAudioModePage* pPage = m_arrAudioModePages[nIndex];
    if(!pPage->FetchValue( m_AudioData.lpAudioMode[nIndex] ))
	{
		return FALSE;
	}
    pPage->SetDefaultFir( m_AudioData.lpAudioMode[nIndex] ); 

	BOOL bRet = m_Cmd.SetModeData( nIndex,m_AudioData.lpAudioMode[nIndex].mode_name,m_AudioData.lpAudioMode[nIndex].audio_data );  
	if(!bRet)
	{
		return FALSE;
	}
	bRet = m_Cmd.SetAudMode(m_AudioData.lpAudioMode[nIndex].mode_name);

	return bRet;
}

BOOL CAudioTesterSheet::WriteDefaultUlFirToPhone( int nIndex )
{
    CAudioModePage* pPage = m_arrAudioModePages[nIndex];
    if(!pPage->FetchValue( m_AudioData.lpAudioMode[nIndex] ))
	{
		return FALSE;
	}
    pPage->SetDefaultUlFir( m_AudioData.lpAudioMode[nIndex] ); 
    
    return m_Cmd.SetModeData( nIndex,m_AudioData.lpAudioMode[nIndex].mode_name,m_AudioData.lpAudioMode[nIndex].audio_data );
}

BOOL CAudioTesterSheet::LoadParaFromPhone(int nIndex,BOOL bConfirm )
{
	CString strName = m_AudioData.lpAudioMode[nIndex].mode_name;
	if( bConfirm )
	{
		if( !ConfirmLoad(strName, FALSE) )
		{
			return m_bLoad;
		}
	}
	m_bLoad = m_Cmd.GetModeData( nIndex, GET_AUDIO_DATA_FROM_RAM, m_AudioData.lpAudioMode[nIndex].audio_data );
	m_arrAudioModePages[nIndex]->SetAudioData(m_AudioData.lpAudioMode[nIndex].audio_data , this);
	//teana hu 2011.02.16
	BOOL bSmartPhone = FALSE;
	if(m_bLoadVolume)
	{
//		CAudioModePage* pPage = m_arrAudioModePages[nIndex];
		
		if((m_VolumeData.lpVolumeMode[0].volume_data.reserve[40] & 0xff) == 0x2)
		{
			bSmartPhone = TRUE;
		}
		else
		{
			bSmartPhone = FALSE;
		}
		for(int i = 0; i < m_VolumeData.mode_count; i++)
		{
			if(strcmp((const char *)m_VolumeData.lpVolumeMode[i].mode_name, (const char *)m_AudioData.lpAudioMode[nIndex].mode_name) == 0)
			{
				m_bLoadVolume = m_Cmd.GetVolumeData(i, GET_ARM_VOLUME_DATA_FROM_RAM, m_VolumeData.lpVolumeMode[i].volume_data);
				if(m_bLoadVolume)
				{
					m_arrAudioModePages[nIndex]->SetVolumeData(m_VolumeData.lpVolumeMode[i].volume_data , this);
				}
				break;
			}
		}
	}
	m_arrAudioModePages[nIndex]->SetSmartPhoneFlag(bSmartPhone);

	//
	//teana hu 2011.04.18
	if(m_bLoadLVVE)
	{
//		CAudioModePage* pPage = m_arrAudioModePages[nIndex];
		for(int i = 0; i < m_LVVEPara.mode_count; i++)
		{
			if(strcmp((const char *)m_LVVEPara.lp_lvve_ctrl_nv_params[i].ucModeName, (const char *)m_AudioData.lpAudioMode[nIndex].mode_name) == 0)
			{
				m_bLoadLVVE = m_Cmd.GetLVVEData(i, GET_LVVE_DATA_FROM_RAM, m_LVVEPara.lp_lvve_ctrl_nv_params[i]);
				if(m_bLoadLVVE)
				{
					//teana hu 2011.06.07
					//m_bLVVEMin = m_Cmd.LVVEIsMaxOrMin();
					//
					//teana hu 2012.01.09
					m_dwLvveType = m_Cmd.GetLVVEType();
					//
					m_arrAudioModePages[nIndex]->m_bExistLVVE = TRUE;
					m_arrAudioModePages[nIndex]->SetLVVEData(m_LVVEPara.lp_lvve_ctrl_nv_params[i]);
				}
				break;
			}
		}
	}
	
	//

	//teana hu 2012.01.31
	if(m_bLoadDspCodec)
	{
		for(int i = 0; i < m_DspModePara.nModeCount; i++)
		{
			if(strcmp((const char *)m_DspModePara.lp_audio_nv_dsp_mode_info[i].ucModeName, (const char *)m_AudioData.lpAudioMode[nIndex].mode_name) == 0)
			{
				m_bLoadDspCodec = m_Cmd.GetDspData(i, GET_AUDIO_DSP_DATA_FROM_RAM, m_DspModePara.lp_audio_nv_dsp_mode_info[i].tAudioNvDspModeStruct);
				if(m_bLoadDspCodec)
				{
					m_arrAudioModePages[nIndex]->m_bExistDspCodec = TRUE;
					m_arrAudioModePages[nIndex]->SetDspData(m_DspModePara.lp_audio_nv_dsp_mode_info[i].tAudioNvDspModeStruct, m_nVolumeLevel_6530);
					m_arrAudioModePages[nIndex]->SetDspCodecFlag(m_bDspCodec);

				}
				break;
			}
		}
	}
	//
	m_arrAudioModePages[nIndex]->UpdateData(FALSE);
	return m_bLoad;
}

BOOL CAudioTesterSheet::LoadParaFromFlash(int nIndex,BOOL bConfirm )
{	
	CString strName = m_AudioData.lpAudioMode[nIndex].mode_name;
	if( !bConfirm )
	{
		if( !ConfirmLoad(strName, FALSE) )
		{
			return m_bLoad;
		}
	}
	m_bLoad = m_Cmd.GetModeData( nIndex, GET_AUDIO_DATA_FROM_FLASH, m_AudioData.lpAudioMode[nIndex].audio_data );
	m_arrAudioModePages[nIndex]->SetAudioData(m_AudioData.lpAudioMode[nIndex].audio_data , this);
	//teana hu 2011.02.16
	BOOL bSmartPhone = FALSE;
	if(m_bLoadVolume)
	{
//		CAudioModePage* pPage = m_arrAudioModePages[nIndex];
		
		if((m_VolumeData.lpVolumeMode[0].volume_data.reserve[40] & 0xff) == 0x2)
		{
			bSmartPhone = TRUE;
		}
		else
		{
			bSmartPhone = FALSE;
		}

		for(int i = 0; i < m_VolumeData.mode_count; i++)
		{
			if(strcmp((const char *)m_VolumeData.lpVolumeMode[i].mode_name, (const char *)m_AudioData.lpAudioMode[nIndex].mode_name) == 0)
			{
				m_bLoadVolume = m_Cmd.GetVolumeData(i, GET_ARM_VOLUME_DATA_FROM_FLASH, m_VolumeData.lpVolumeMode[i].volume_data);
				if(m_bLoadVolume)
				{
					m_arrAudioModePages[nIndex]->SetVolumeData(m_VolumeData.lpVolumeMode[i].volume_data , this);
				}
				break;
			}
		}
	}
	m_arrAudioModePages[nIndex]->SetSmartPhoneFlag(bSmartPhone);

	//
	
	//teana hu 2011.04.18
	if(m_bLoadLVVE)
	{
//		CAudioModePage* pPage = m_arrAudioModePages[nIndex];
		for(int i = 0; i < m_LVVEPara.mode_count; i++)
		{
			if(strcmp((const char *)m_LVVEPara.lp_lvve_ctrl_nv_params[i].ucModeName, (const char *)m_AudioData.lpAudioMode[nIndex].mode_name) == 0)
			{
				m_bLoadLVVE = m_Cmd.GetLVVEData(i, GET_LVVE_DATA_FROM_FLASH, m_LVVEPara.lp_lvve_ctrl_nv_params[i]);
				if(m_bLoadLVVE)
				{
					//teana hu 2011.06.07
					//m_bLVVEMin = m_Cmd.LVVEIsMaxOrMin();
					//
					//teana hu 2012.01.09
					m_dwLvveType = m_Cmd.GetLVVEType();
					//
					m_arrAudioModePages[nIndex]->m_bExistLVVE = TRUE;
					m_arrAudioModePages[nIndex]->SetLVVEData(m_LVVEPara.lp_lvve_ctrl_nv_params[i]);
				}
				break;
			}
		}
	}
	//
	//teana hu 2012.01.31
	if(m_bLoadDspCodec)
	{
		for(int i = 0; i < m_DspModePara.nModeCount; i++)
		{
			if(strcmp((const char *)m_DspModePara.lp_audio_nv_dsp_mode_info[i].ucModeName, (const char *)m_AudioData.lpAudioMode[nIndex].mode_name) == 0)
			{
				m_bLoadDspCodec = m_Cmd.GetDspData(i, GET_AUDIO_DSP_DATA_FROM_FLASH, m_DspModePara.lp_audio_nv_dsp_mode_info[i].tAudioNvDspModeStruct);
				if(m_bLoadDspCodec)
				{
					m_arrAudioModePages[nIndex]->m_bExistDspCodec = TRUE;
					m_arrAudioModePages[nIndex]->SetDspData(m_DspModePara.lp_audio_nv_dsp_mode_info[i].tAudioNvDspModeStruct, m_nVolumeLevel_6530);
					m_arrAudioModePages[nIndex]->SetDspCodecFlag(m_bDspCodec);
				}
				break;
			}
		}
	}
	//
	m_arrAudioModePages[nIndex]->UpdateData(FALSE);
	return m_bLoad;
}

BOOL CAudioTesterSheet::LoadENHAModes(int nType, int nPos)
{
	int nCount = 0;
	m_bLoadENHA = m_Cmd.GetEQTunModeCount(nCount, nType);
	if(!m_bLoadENHA)
	{
		return FALSE;
	}

	if(nCount == 0)
	{
		m_bLoadENHA = FALSE;
		return FALSE;
	}

	if(nType == TYPE_EQ)
	{
		if(m_eqData_g.lpEQMode == NULL)
		{
			m_eqData_g.eq_count = nCount;
			m_eqData_g.lpEQMode = new EQ_STRUCT_G_T[nCount];
		}
		for( int i=0;i<nCount;i++ )
		{
			ZeroMemory( (LPVOID)&m_eqData_g.lpEQMode[i],sizeof( EQ_STRUCT_G_T ) );
			
			m_bLoadENHA = m_Cmd.GetEQGData(i+1, nPos, m_eqData_g.lpEQMode[i]);
			if( !m_bLoadENHA )
			{
				break;
			}
		}
		if(m_bLoadENHA)
		{
			for(i = 0; i < m_arrMusicModePages.GetSize(); i++)
			{
				CMusicModePage *pPage = (CMusicModePage *)m_arrMusicModePages.GetAt(i);
				for(int j = 0; j < nCount; j++)
				{
					CString strTitle = _T("");
					pPage->GetTitle(strTitle);
					char szTitle[NAME_MAX_LEN] = {0};
#ifdef _UNICODE
					WideCharToMultiByte(CP_ACP,0,strTitle,-1,szTitle,strTitle.GetLength(),NULL,NULL);
#else
					strcpy( szTitle,strTitle );
#endif  
					if(strstr((const char *)m_eqData_g.lpEQMode[j].eq_set_name, szTitle) != NULL)
					{
						pPage->SetEQValue(m_eqData_g.lpEQMode[j]);
						pPage->SetEQIndex(j);
						break;
					}
				}
			}
		}
		
	}
	else if(nType == TYPE_TUN)
	{
		if(m_tunData_g.lpTunMode == NULL)
		{
			m_tunData_g.tun_count = nCount;
			m_tunData_g.lpTunMode = new TUN_STRUCT_G_T[nCount];
		}
		for( int i=0;i<nCount;i++ )
		{
			ZeroMemory( (LPVOID)&m_tunData_g.lpTunMode[i],sizeof( TUN_STRUCT_G_T ) );
			
			m_bLoadENHA = m_Cmd.GetTunData(i+1, nPos, m_tunData_g.lpTunMode[i]);
			if( !m_bLoadENHA )
			{
				break;
			}
		}
		
		if(m_bLoadENHA)
		{
			for(i = 0; i < m_arrMusicModePages.GetSize(); i++)
			{
				CMusicModePage *pPage = (CMusicModePage *)m_arrMusicModePages.GetAt(i);
				pPage->SetTunValue(m_tunData_g.lpTunMode[0]);
				pPage->SetTunIndex(0);
			}
		}
	}

	return m_bLoadENHA;
}


BOOL CAudioTesterSheet::WriteENHAParaToPhone( int nIndex, int nType, int nPos, int nEQModeSelType)
{
	if(nType == TYPE_EQ)
	{
		return m_Cmd.SetEQGData( nIndex+1, nPos, m_eqData_g.lpEQMode[nIndex], nEQModeSelType);
	}
	else if(nType == TYPE_TUN)
	{
		return m_Cmd.SetTunData( nIndex+1, nPos, m_tunData_g.lpTunMode[nIndex], nEQModeSelType);
	}
	return FALSE;
}

BOOL CAudioTesterSheet::SetVolumePara(int nVolumeIndex, int nAppType)
{
	return m_Cmd.SetVolumePara(nVolumeIndex, nAppType);
}

int CAudioTesterSheet::GetActiveVolumePageIndex()
{
	int nIndex = GetActiveIndex();
	if( -1 == nIndex )
	{
		// No active index
		return -1;
	}

	if( 0 == nIndex )
	{
		// Page 0 is the setting page,no audio data
		return -1;
	}

	// Audio data page index begins from 1, Audio mode data index begins from 0
	nIndex -= 1;

	for( int i=0;i<m_VolumeData.mode_count;i++ )
	{
		if( !strcmp( (const char*)m_VolumeData.lpVolumeMode[i].mode_name,(const char*)m_AudioData.lpAudioMode[nIndex].mode_name ) )
		{
			return i;
		}
	}

	return -1;
}

BOOL CAudioTesterSheet::FetchValue( UINT nIndex )
{
	if( nIndex >= m_AudioData.mode_count )
	{
		return FALSE;
	}

    CAudioModePage* pPage = m_arrAudioModePages[nIndex];

	if( !pPage->FetchValue( m_AudioData.lpAudioMode[nIndex] ) )
	{
		return FALSE;
	}

	if(m_bLoadVolume)
	{
		for(int i = 0; i < m_VolumeData.mode_count; i++)
		{
			if(strcmp((const char *)m_VolumeData.lpVolumeMode[i].mode_name, (const char *)m_AudioData.lpAudioMode[nIndex].mode_name) == 0)
			{
				if(!pPage->FetchVolumeValue(m_VolumeData.lpVolumeMode[i]))
				{
					return FALSE;
				}
				else
				{
					break;
				}
			}
		}
	}

	 if(m_bLoadLVVE)
	 {
		 for(int i = 0; i < m_LVVEPara.mode_count; i++)
		 {
			 if(strcmp((const char *)m_LVVEPara.lp_lvve_ctrl_nv_params[i].ucModeName, (const char *)m_AudioData.lpAudioMode[nIndex].mode_name) == 0)
			 {
				 if(pPage != NULL)
				 {
					 pPage->FetchLVVEData(m_LVVEPara.lp_lvve_ctrl_nv_params[i]);
					 break;
				 }
			 }
		 }
	 }

	 //teana hu 2012.01.31
	 if(m_bLoadDspCodec)
	 {
		 for(int i = 0; i < m_DspModePara.nModeCount; i++)
		 {
			 if(strcmp((const char *)m_DspModePara.lp_audio_nv_dsp_mode_info[i].ucModeName, (const char *)m_AudioData.lpAudioMode[nIndex].mode_name) == 0)
			 {
				 if(pPage != NULL)
				 {
					 pPage->FetchDspData(m_DspModePara.lp_audio_nv_dsp_mode_info[i].tAudioNvDspModeStruct);
				 }
			 }
		 }
	 }
	 //

	 return TRUE;
}

UINT CAudioTesterSheet::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//return CPropertySheet::OnNcHitTest(point);
	CRect rc;
	GetClientRect(&rc);

	ClientToScreen(&rc);
	return rc.PtInRect(point) ? HTCAPTION : CPropertySheet::OnNcHitTest(point);
}
