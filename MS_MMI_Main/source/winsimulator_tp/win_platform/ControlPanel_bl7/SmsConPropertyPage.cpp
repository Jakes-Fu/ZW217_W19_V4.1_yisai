// SmsConPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "SmsConPropertyPage.h"
#include "ConPanPropertySheet.h"
#include "ConPanLayer.h"
#include "ConPanFunc.h"
#include "ConPan.h"
#include "signals_header.h"
#include "UnicodeConvert.h"
#include "mn_api.h"
#include "ATestPropertyPage.h"  // ATEST MODULE

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_TIMER_SMS_STATUS_REPORT     132
//@ Liu Kai 2004-7-8
// The line below will cause memory leak
//char 			*pszSMSData_long 	= new char[1000];
char     pszSMSData_long[10000] = {0};
uint32	 length_long = 0;
static uint8	s_refer_num = 0xff;
static uint8	s_index_num = 1;
static uint8	s_total_num = 1;
static uint16	s_limit_len = 0;
static uint16	s_split_len = 0;
static uint16	s_data_len = 0;
static uint8*	s_data_ptr = PNULL;
static CALLED_NUMBER_T_s s_dest_addr;
static CALLED_NUMBER_T_s s_sc_addr;

/////////////////////////////////////////////////////////////////////////////
// CSmsConPropertyPage property page

IMPLEMENT_DYNCREATE(CSmsConPropertyPage, CPropertyPage)

CSmsConPropertyPage::CSmsConPropertyPage() : CPropertyPage(CSmsConPropertyPage::IDD)
{
	//{{AFX_DATA_INIT(CSmsConPropertyPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSmsConPropertyPage::~CSmsConPropertyPage()
{
}

void CSmsConPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSmsConPropertyPage)
	DDX_Control(pDX, IDC_RADIO_SMS_DATATYPE, m_wndSMSDataType);
	DDX_Control(pDX, IDC_RADIO2, m_wndSMSBinary);
	DDX_Control(pDX, IDC_RADIO_SMS_SC_STATUS, m_wndSMSScStatus);
	DDX_Control(pDX, IDC_RADIO1, m_wndSmsRevBySME);
	DDX_Control(pDX, IDC_RADIO4, m_wndSmsNotConfirm);
	DDX_Control(pDX, IDC_RADIO5, m_wndSmsReplaceBySC);
	DDX_Control(pDX, IDC_RADIO6, m_wndSmsSendLater);
	DDX_Control(pDX, IDC_RADIO7, m_wndSmsNotSendAgain);
	DDX_Control(pDX, IDC_RADIO8, m_wndSmsInvalidSR);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSmsConPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSmsConPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_SEND_SR, OnButtonSendStatusReport)
	ON_EN_MAXTEXT(IDC_EDIT_SMSSEND, OnMaxtextEditSmssend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_FormatSet, OnBUTTONFormatSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmsConPropertyPage message handlers

void CSmsConPropertyPage::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
    sms_info.DestroyWindow();
	CPropertyPage::PostNcDestroy();
//	delete this;
}
void CSmsConPropertyPage::DealCPMsg(void *pmsg)
{
	ASSERT(pmsg);
// 	static char     pszSMSData_long[1000] = {0};
// 	static uint32	length_long = 0;

    LPCTSTR UNAVAILABLE_INFO = "(not available)";

	xSignalHeaderRec 	*psig 	= (xSignalHeaderRec *) pmsg;

	CStatic 			*pstTip	= (CStatic *) GetDlgItem(IDC_STATIC_TIP1);
		
	switch(psig->SignalCode)
	{
	case MN_RL_REPORT_REQ:
        {
		    MN_RL_REPORT_REQ_T *report_req_ptr = (MN_RL_REPORT_REQ_T *)psig;
		    if (SMS_SOR_NORMAL_EVENT != report_req_ptr->sms_report.sor)
		    {
			    MessageBox("Sorry. Memory capability exceeded.", "Info", MB_ICONINFORMATION);
		        return ;
		    }
			else
			{
				s_index_num++;
				if(s_index_num <= s_total_num)
				{
					MN_RL_DATA_IND_T    *pSMSInd;	
					InnerSCICreateSignal(
									  (xSignalHeaderRec **)&pSMSInd,
									  MN_RL_DATA_IND,
									  sizeof(MN_RL_DATA_IND_T),
									  P_CPANEL);
					if(pSMSInd != PNULL)
					{
						uint8	len = 0;
						pSMSInd->user_head_is_exist = TRUE;
						pSMSInd->header_arr.num = 1;
						if (sms_info.m_formatData.user_header.ref_type == SMS_PORT_8BIT)
						{
							pSMSInd->header_arr.head_arr[0].iei = MN_SMS_CONCAT_8_BIT_REF_NUM;
							pSMSInd->header_arr.head_arr[0].head_union.cancat_8bit.ref_num = s_refer_num;
							pSMSInd->header_arr.head_arr[0].head_union.cancat_8bit.segment_num = s_total_num;
							pSMSInd->header_arr.head_arr[0].head_union.cancat_8bit.seq_num = s_index_num;
						}else
						{
							pSMSInd->header_arr.head_arr[0].iei = MN_SMS_CONCAT_16_BIT_REF_NUM;
							pSMSInd->header_arr.head_arr[0].head_union.cancat_16bit.ref_num = s_refer_num;
							pSMSInd->header_arr.head_arr[0].head_union.cancat_16bit.segment_num = s_total_num;
							pSMSInd->header_arr.head_arr[0].head_union.cancat_16bit.seq_num = s_index_num;
						}
						
						if (sms_info.m_formatData.user_header.is_port_exist)
						{
							pSMSInd->header_arr.num++;
							if (sms_info.m_formatData.user_header.port_type == SMS_PORT_8BIT)
							{
								pSMSInd->header_arr.head_arr[pSMSInd->header_arr.num-1].iei = MN_SMS_APP_PORT_8_BIT_ADDR;
								pSMSInd->header_arr.head_arr[pSMSInd->header_arr.num-1].head_union.port_8bit.org_port = sms_info.m_formatData.user_header.sour_port;
								pSMSInd->header_arr.head_arr[pSMSInd->header_arr.num-1].head_union.port_8bit.des_port = sms_info.m_formatData.user_header.dest_port;
							}else
							{
								pSMSInd->header_arr.head_arr[pSMSInd->header_arr.num-1].iei = MN_SMS_APP_PORT_16_BIT_ADDR;
								pSMSInd->header_arr.head_arr[pSMSInd->header_arr.num-1].head_union.port_16bit.org_port = sms_info.m_formatData.user_header.sour_port;
								pSMSInd->header_arr.head_arr[pSMSInd->header_arr.num-1].head_union.port_16bit.des_port = sms_info.m_formatData.user_header.dest_port;
							}
						}
						memset( &pSMSInd->dcs, 0, sizeof(MN_SMS_DCS_T) );
						// SMS_DELIVER_MSG
						pSMSInd->sms_type = 0;
						
						pSMSInd->dest_addr = s_dest_addr;
						pSMSInd->sc_addr = s_sc_addr;
						pSMSInd->dcs = sms_info.m_formatData.dcs;

						if(s_data_len >= (s_index_num * s_split_len))
						{
							len = (uint8)s_split_len;
						}
						else
						{
							len = s_data_len - (s_index_num - 1) * s_split_len;
						}
						pSMSInd->sms_data.user_valid_data_t.length = len;
						memcpy( pSMSInd->sms_data.user_valid_data_t.user_valid_data_arr, s_data_ptr + (s_index_num - 1) * s_split_len, len );

						CTime tmNow;    
						tmNow = CTime::GetCurrentTime();
						pSMSInd->sms_time.year 	= tmNow.GetYear() % 100;
						pSMSInd->sms_time.month	= tmNow.GetMonth();
						pSMSInd->sms_time.day	= tmNow.GetDay();
						pSMSInd->sms_time.hour	= tmNow.GetHour();
						pSMSInd->sms_time.minute= tmNow.GetMinute();
						pSMSInd->sms_time.second= tmNow.GetSecond();
						pSMSInd->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
    
						SendCPMsgToPSDummy( (xSignalHeaderRec *)pSMSInd );
						pstTip->SetWindowText("sent a short message.");
						UpdateWindow();  
						PlaySendSMSSound();
					}
				}else
				{
					free(s_data_ptr);
				}
			}		
        }
		break;
	case MN_SM_RL_MEM_AVAIL_REQ:
		MessageBox("Memory capability avail.", "Info", MB_ICONINFORMATION);
		MN_RL_REPORT_IND_T *pRlReportInd_ptr;
	            
		InnerSCICreateSignal(
						  (xSignalHeaderRec **)&pRlReportInd_ptr,
						  MN_RL_REPORT_IND,
						  sizeof(MN_RL_REPORT_IND_T),
						  P_CPANEL);

		pRlReportInd_ptr->smi = 1;
		pRlReportInd_ptr->sms_report.sor = SMS_SOR_NORMAL_EVENT;
		pRlReportInd_ptr->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
		
		SendCPMsgToPSDummy((xSignalHeaderRec *)pRlReportInd_ptr);
		break;
	case MN_SMS_REQ:
		switch (m_wndSMSScStatus.GetCheck())
		{
		case 1:
			{
				char 			*pszSMSData 	= NULL;  //current SMS, is used to debug. pszSMSData_long is used to display.
				
				MN_SMS_REQ_T 	*pSmsReqSig 	= (MN_SMS_REQ_T *) psig;
				CEdit 			*pedRcvedSms  	= (CEdit *) GetDlgItem(IDC_EDIT_RCVEDSMS);
	            CEdit 			*pedSC 			= (CEdit *) GetDlgItem(IDC_EDIT_SMS_REMOTE_SC);
	            CEdit 			*pedDest 		= (CEdit *) GetDlgItem(IDC_EDIT_RCVEDFROM2);

				uint8 			*pdata 			= pSmsReqSig->sms_data.user_valid_data_t.user_valid_data_arr;					
				uint8 			length 			= pSmsReqSig->sms_data.user_valid_data_t.length;
				uint8 			*phead 			= pSmsReqSig->sms_data.user_data_head_t.user_header_arr;					
				uint8 			length_head		= pSmsReqSig->sms_data.user_data_head_t.length;
				
			
	            switch ( pSmsReqSig->dcs.alphabet_type )
	            {
	            case MN_SMS_DEFAULT_ALPHABET: // ASCII data
	                {
	                    pszSMSData = new char[ length + 1 ]; // +1 for '\0'
						memset( pszSMSData, 0, length + 1 );
	                    memcpy( pszSMSData, pdata, length );
	                    *( pszSMSData + length ) = 0;
						memcpy( pszSMSData_long + length_long, pdata, length );
						length_long += length;
						*( pszSMSData_long + length_long ) = 0;
	                }
	                break;

	            case MN_SMS_8_BIT_ALPHBET: // binary data
	                {
	                    //pszSMSData = new char[ length * 2 + 1 ]; // *2: 2 characters stand for 1 byte
	                    //for ( int i = 0; i < length; i++ )
	                    //{
	                    //    sprintf( &pszSMSData[i * 2], "%02X", pdata[i] );
						//	sprintf( &pszSMSData_long[i * 2 + length_long], "%02X", pdata[i] );
	                    //}
						//length_long += length;
						pszSMSData = new char[ length + 1 ]; // +1 for '\0'
						memset( pszSMSData, 0, length + 1 );
	                    memcpy( pszSMSData, pdata, length );
	                    *( pszSMSData + length ) = 0;
						memcpy( pszSMSData_long + length_long, pdata, length );
						length_long += length;
						*( pszSMSData_long + length_long ) = 0;
	                }
	                break;

	            case MN_SMS_UCS2_ALPHABET: // unicode data
	                {
	                    // Ensure the 'length' is even
	                    ASSERT( 0 == length % 2 );

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
	                    WCHAR * pwcSMSData = (WCHAR *)pdata; // /2: 2 bytes stand for 1 unicode character
#else
	                    WCHAR * pwcSMSData = new WCHAR[ length / 2 ]; // /2: 2 bytes stand for 1 unicode character
	                    
	                    // NOTE: do not use memcpy() here because of different endian
	                    for ( int i = 0; i < length; i += 2 )
	                    {
	                        *( (char *)pwcSMSData + i )     = *( pdata + i + 1 );
	                        *( (char *)pwcSMSData + i + 1 ) = *( pdata + i );
	                    }

	                    pszSMSData = new char[ length + 1 ]; // +1 for '\0'
	                    memset( pszSMSData, 0, length + 1 );
						memset( pszSMSData_long + length_long, 0, length + 1 );
#endif
						// simulator use little endian,so do not need endian conversion

						pszSMSData = new char[ length + 1 ]; // +1 for '\0'
						memset( pszSMSData, 0, length + 1 );
	                    WideCharToMultiByte(
	                        CP_ACP,
	                        0,
	                        (WCHAR *)pwcSMSData,
	                        length / 2,
	                        pszSMSData,
	                        length,
	                        NULL,
	                        NULL );
						length_long +=WideCharToMultiByte(
	                        CP_ACP,
	                        0,
	                        (WCHAR *)pwcSMSData,
	                        length / 2,
	                        pszSMSData_long + length_long,
	                        length,
	                        NULL,
	                        NULL );
					
	                }
	                break;

	            default: // unknown data
	                {
	                    pszSMSData = new char[ strlen(UNAVAILABLE_INFO) + 1 ];
	                    strcpy( pszSMSData, UNAVAILABLE_INFO );
						strcpy( pszSMSData_long+length_long, UNAVAILABLE_INFO );
						length_long += strlen(UNAVAILABLE_INFO);
	                }
	                break;
	            }
	            // ]

				uint16 			nSCNumLen   = pSmsReqSig->sc_addr.num_len;
	            uint16 			nDestNumLen = pSmsReqSig->dest_addr.num_len;
				unsigned char 	*pszSC   	= new unsigned char[nSCNumLen * 2 + 2];
	            unsigned char 	*pszDest 	= new unsigned char[nDestNumLen * 2 + 2];

				if (MN_NUM_TYPE_INTERNATIONAL == pSmsReqSig->sc_addr.number_type)
				{
					pszSC[0] = '+';
		            GetStrFromCall(&pSmsReqSig->sc_addr,
									&pszSC[1],
									nSCNumLen * 2 + 1);
				}
				else
				{
		            GetStrFromCall(&pSmsReqSig->sc_addr,
									&pszSC[0],
									nSCNumLen * 2 + 1);
				}

	            if (MN_NUM_TYPE_INTERNATIONAL == pSmsReqSig->dest_addr.number_type)
            	{
            		pszDest[0] = '+';
		            GetStrFromCall(&pSmsReqSig->dest_addr,
									&pszDest[1],
									nDestNumLen * 2 + 1);
            	}
	            else
            	{
		            GetStrFromCall(&pSmsReqSig->dest_addr,
									&pszDest[0],
									nDestNumLen * 2 + 1);
            	}

	            CString csSMSData = pszSMSData_long;
	            CString csSMSSC = pszSC;
	            CString csSMSDest = pszDest;

	            if (csSMSSC.IsEmpty())
	            {
	                csSMSSC = UNAVAILABLE_INFO;
	            }

	            if (csSMSDest.IsEmpty())
	            {
	                csSMSDest = UNAVAILABLE_INFO;
	            }

				pedRcvedSms->SetWindowText(csSMSData);
				pedSC->SetWindowText(csSMSSC);
	            pedDest->SetWindowText(csSMSDest);
	            pstTip->SetWindowText("Received a short message.");
				UpdateWindow();
	            PlayRecvSMSSound();

				

                // status report
                if (pSmsReqSig->status_report_is_request)
            	{
            		StartSmsStatusReport();
            	}

	            // 
	            MN_RL_REPORT_IND_T *pRlReportInd_ptr;
	            
				InnerSCICreateSignal(
								  (xSignalHeaderRec **)&pRlReportInd_ptr,
								  MN_RL_REPORT_IND,
								  sizeof(MN_RL_REPORT_IND_T),
								  P_CPANEL);

				pRlReportInd_ptr->smi = 1;
				pRlReportInd_ptr->sms_report.sor = SMS_SOR_NORMAL_EVENT;
				pRlReportInd_ptr->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
	
				SendCPMsgToPSDummy((xSignalHeaderRec *)pRlReportInd_ptr);
				delete [] pszSMSData;
	            pszSMSData = NULL;
				delete [] pszSC;
	            pszSC = NULL;
	            delete [] pszDest;
	            pszDest = NULL;
				if(phead[3] == phead[4])
				{
						length_long = 0;
						memset( pszSMSData_long, 0, 1000);
				}
			}

			           
			break;
			
		case 0:
		    MN_RL_ERROR_IND_T	*pRlErrorInd_ptr;	
			InnerSCICreateSignal(
							  (xSignalHeaderRec **)&pRlErrorInd_ptr,
							  MN_RL_ERROR_IND,
							  sizeof(MN_RL_ERROR_IND_T),
							  P_CPANEL);

			pRlErrorInd_ptr->smi = 1;
			pRlErrorInd_ptr->sms_error = SMS_NO_VALID_SMI;
			pRlErrorInd_ptr->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
		    
			SendCPMsgToPSDummy((xSignalHeaderRec *)pRlErrorInd_ptr);
			break;
			
		default:
			break;
		}
		
        break;
    case ATEST_CP_SEND_PDU_IND: // send automatically the PDU.
        { 
            ATEST_CP_DATA_T *pData = (ATEST_CP_DATA_T*)psig;
            CEdit *pedSmsSend = (CEdit *)GetDlgItem(IDC_EDIT_SMSSEND);
            
            pedSmsSend->SetWindowText("");
            pedSmsSend->SetWindowText(pData->record_data);
            
            m_wndSMSDataType.SetCheck(0);
            m_wndSMSBinary.SetCheck(0);
            UpdateWindow();
            OnButtonSend(); // send the PDU to the MS simulator.
        }
        break;
	default:
		break;	
	}

}

void CSmsConPropertyPage::OnButtonSend() 
{
	// TODO: Add your control notification handler code here
	if (!((CConPanPropertySheet *)GetParent())->IsPowerOn(sys_id))
	{
		MessageBox("Sorry. Mobile is power off.", "Warning", MB_ICONWARNING);
		return ;
	}

    if (!((CConPanPropertySheet *)GetParent())->IsServiceAvailable(sys_id))
	{
		MessageBox("Sorry. No service available.", "Warning", MB_ICONWARNING);
		return ;
	}

    CEdit * pedSmsSend = (CEdit *) GetDlgItem(IDC_EDIT_SMSSEND);
	CEdit * pedSendFrom = (CEdit *) GetDlgItem(IDC_EDIT_SENDFROM);
	CStatic *pstTip = (CStatic *) GetDlgItem(IDC_STATIC_TIP1);


    char strServiceCenter[32] = {0};
    char strfrom[32] = {0};
    char strsms[601] = {0};
	BOOLEAN		is_pdu = FALSE;
   
    GetDlgItemText(IDC_EDIT_SMS_LOCAL_SC, strServiceCenter, sizeof(strServiceCenter));
	if (strlen(strServiceCenter) == 0)
	{
		MessageBox("Sorry, 'service center' can not be empty.",
					"Warning",
					MB_ICONWARNING);
        return;
	}

	GetDlgItemText(IDC_EDIT_SENDFROM,strfrom,32);
	if (strlen(strfrom) == 0)
	{
		MessageBox("Sorry, 'send from' can not be empty.",
					"Warning",
					MB_ICONWARNING);
        return;
	}

	pedSmsSend->GetWindowText(strsms, sizeof(strsms));	
	
	if ( 1 == m_wndSMSDataType.GetCheck() ) // ASCII
	{
        CUnicodeConvert uniConv;

        int nChineseCharCount = uniConv.GetChineseCharCount( strsms );
        if ( nChineseCharCount == 0 ) // English message
        {
            
			sms_info.m_formatData.dcs.alphabet_type = MN_SMS_DEFAULT_ALPHABET;
			s_limit_len = 160;
			s_split_len = 153;
	        s_data_len = strlen( strsms );
			s_data_ptr = (uint8*)malloc(s_data_len);
	        if(s_data_ptr != PNULL)
			{
				memcpy( s_data_ptr, strsms, s_data_len);
			}
        }
        else // Chinese message
        {
            int nUnicodeCount = strlen( strsms ) - nChineseCharCount;
            WCHAR * pwcUnicode = new WCHAR[ nUnicodeCount ];
            
            uniConv.ConvertToUnicode( pwcUnicode,
                                      nUnicodeCount * sizeof(WCHAR),
                                      strsms,
                                      strlen(strsms) );

            sms_info.m_formatData.dcs.alphabet_type = MN_SMS_UCS2_ALPHABET;
			s_limit_len = 140;
			s_split_len = 134;
            s_data_len = nUnicodeCount * sizeof(WCHAR);
			s_data_ptr = (uint8*)malloc(s_data_len);
			if(s_data_ptr != PNULL)
			{
				// simulator use little endian,so do not need endian convertion				
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
				memcpy( s_data_ptr,pwcUnicode,s_data_len );
#else
				for ( int i = 0; i < nUnicodeCount; i++ )
				{
					*(s_data_ptr + i * 2)     = HIBYTE( *(pwcUnicode + i) );
					*(s_data_ptr + i * 2 + 1) = LOBYTE( *(pwcUnicode + i) );
				}
#endif
				
			}
            // NOTE: do not use memcpy() here because of different endian

            delete [] pwcUnicode;
        }
    }
    else if(m_wndSMSBinary.GetCheck())// Binary
    {
        CString strSMS;
        pedSmsSend->GetWindowText( strSMS );

        if ( strSMS.GetLength() % 2 != 0 )
        {
            AfxMessageBox( "Invalid binary data: data length must be even!", MB_ICONSTOP );
            return;
        }
        
        int nByteLength = strSMS.GetLength() / 2;
        BYTE * pbHexByte = new BYTE[ nByteLength ];
        memset( pbHexByte, 0, nByteLength );
        for ( int i = 0; i < nByteLength; i ++ )
        {
            pbHexByte[ i ] = ( BYTE )strtol( strSMS.Mid(i * 2, 2), NULL, 16 );
        }

        sms_info.m_formatData.dcs.alphabet_type = MN_SMS_8_BIT_ALPHBET;
		        
		s_limit_len = 140;
		s_split_len = 134;
	    s_data_len = nByteLength;
		s_data_ptr = (uint8*)malloc(s_data_len);
		if(s_data_ptr != PNULL)
		{
			memcpy( s_data_ptr, pbHexByte, nByteLength );
		}
			
		delete [] pbHexByte;
    }else //pdu
	{
		is_pdu = TRUE;
		
		CString strSMS;
        pedSmsSend->GetWindowText( strSMS );

        if ( strSMS.GetLength() % 2 != 0 )
        {
            AfxMessageBox( "Invalid binary data: data length must be even!", MB_ICONSTOP );
            return;
        }
        
        int nByteLength = strSMS.GetLength() / 2;
        BYTE * pbHexByte = new BYTE[ nByteLength ];
        memset( pbHexByte, 0, nByteLength );
        for ( int i = 0; i < nByteLength; i ++ )
        {
            pbHexByte[ i ] = ( BYTE )strtol( strSMS.Mid(i * 2, 2), NULL, 16 );
        }
			    s_data_len = nByteLength;
		s_data_ptr = (uint8*)malloc(s_data_len);
		if(s_data_ptr != PNULL)
		{
			memcpy( s_data_ptr, pbHexByte, nByteLength );
		}
		delete [] pbHexByte;
	}

	SetStrToCall( &s_dest_addr, (unsigned char *) strfrom );
	SetStrToCall( &s_sc_addr, (unsigned char *) strServiceCenter );

    s_index_num = 1;
	s_refer_num++;
	MN_RL_DATA_IND_T    *pSMSInd;	
	InnerSCICreateSignal(
					  (xSignalHeaderRec **)&pSMSInd,
					  MN_RL_DATA_IND,
					  sizeof(MN_RL_DATA_IND_T),
					  P_CPANEL);
    if(pSMSInd != PNULL)
	{
		if (is_pdu)
		{
			pSMSInd->is_pdu_type = TRUE;
			pSMSInd->pdu.length = s_data_len;
			memcpy(&pSMSInd->pdu.l3_msg, s_data_ptr,s_data_len);
			pSMSInd->sms_type = 0;
			pSMSInd->sc_addr = s_sc_addr;
		}else
		{
			memset( &pSMSInd->dcs, 0, sizeof(MN_SMS_DCS_T) );

			// SMS_DELIVER_MSG
			pSMSInd->sms_type = 0;
			
			pSMSInd->dest_addr = s_dest_addr;
			pSMSInd->sc_addr = s_sc_addr;
			pSMSInd->dcs = sms_info.m_formatData.dcs;
			pSMSInd->pid_present = sms_info.m_formatData.is_pid_exist;
			pSMSInd->pid_e	= sms_info.m_formatData.pid;
			if(s_data_len > s_limit_len)
			{
				s_total_num = s_data_len / s_split_len + (((s_data_len % s_split_len) == 0)? 0:1);
			}
			else
			{
				s_total_num = 1;
			}
			if(1 == s_total_num)
			{
				if (sms_info.m_formatData.user_header.is_port_exist)
				{
					pSMSInd->user_head_is_exist = TRUE;
					pSMSInd->header_arr.num = 1;
					if (sms_info.m_formatData.user_header.port_type == SMS_PORT_8BIT)
					{
						pSMSInd->header_arr.head_arr[0].iei = MN_SMS_APP_PORT_8_BIT_ADDR;
						pSMSInd->header_arr.head_arr[0].head_union.port_8bit.org_port = sms_info.m_formatData.user_header.sour_port;
						pSMSInd->header_arr.head_arr[0].head_union.port_8bit.des_port = sms_info.m_formatData.user_header.dest_port;
					}else
					{
						pSMSInd->header_arr.head_arr[0].iei = MN_SMS_APP_PORT_16_BIT_ADDR;
						pSMSInd->header_arr.head_arr[0].head_union.port_16bit.org_port = sms_info.m_formatData.user_header.sour_port;
						pSMSInd->header_arr.head_arr[0].head_union.port_16bit.des_port = sms_info.m_formatData.user_header.dest_port;
					}
					
//					MNSMS_EncodeUserHead(&pSMSInd->sms_data.user_data_head_t,&header_arr);
					
				}else
				{
					pSMSInd->user_head_is_exist = FALSE;
				}
				
				pSMSInd->sms_data.user_valid_data_t.length = (uint8)s_data_len;
				memcpy( pSMSInd->sms_data.user_valid_data_t.user_valid_data_arr, s_data_ptr, s_data_len );
			}
			else
			{
				uint8	len = 0;
				pSMSInd->user_head_is_exist = TRUE;
				pSMSInd->header_arr.num = 1;
				if (sms_info.m_formatData.user_header.ref_type == SMS_PORT_8BIT)
				{
					pSMSInd->header_arr.head_arr[0].iei = MN_SMS_CONCAT_8_BIT_REF_NUM;
					pSMSInd->header_arr.head_arr[0].head_union.cancat_8bit.ref_num = s_refer_num;
					pSMSInd->header_arr.head_arr[0].head_union.cancat_8bit.segment_num = s_total_num;
					pSMSInd->header_arr.head_arr[0].head_union.cancat_8bit.seq_num = s_index_num;
				}else
				{
					pSMSInd->header_arr.head_arr[0].iei = MN_SMS_CONCAT_16_BIT_REF_NUM;
					pSMSInd->header_arr.head_arr[0].head_union.cancat_16bit.ref_num = s_refer_num;
					pSMSInd->header_arr.head_arr[0].head_union.cancat_16bit.segment_num = s_total_num;
					pSMSInd->header_arr.head_arr[0].head_union.cancat_16bit.seq_num = s_index_num;
				}
				
				if (sms_info.m_formatData.user_header.is_port_exist)
				{
					pSMSInd->header_arr.num++;
					if (sms_info.m_formatData.user_header.port_type == SMS_PORT_8BIT)
					{
						pSMSInd->header_arr.head_arr[pSMSInd->header_arr.num-1].iei = MN_SMS_APP_PORT_8_BIT_ADDR;
						pSMSInd->header_arr.head_arr[pSMSInd->header_arr.num-1].head_union.port_8bit.org_port = sms_info.m_formatData.user_header.sour_port;
						pSMSInd->header_arr.head_arr[pSMSInd->header_arr.num-1].head_union.port_8bit.des_port = sms_info.m_formatData.user_header.dest_port;
					}else
					{
						pSMSInd->header_arr.head_arr[pSMSInd->header_arr.num-1].iei = MN_SMS_APP_PORT_16_BIT_ADDR;
						pSMSInd->header_arr.head_arr[pSMSInd->header_arr.num-1].head_union.port_16bit.org_port = sms_info.m_formatData.user_header.sour_port;
						pSMSInd->header_arr.head_arr[pSMSInd->header_arr.num-1].head_union.port_16bit.des_port = sms_info.m_formatData.user_header.dest_port;
					}
				}

				//MNSMS_EncodeUserHead(&pSMSInd->sms_data.user_data_head_t,&header_arr);

				if(s_data_len >= (s_index_num * s_split_len))
				{
					len = (uint8)s_split_len;
				}
				else
				{
					len = s_data_len - (s_index_num - 1) * s_split_len;
				}
				pSMSInd->sms_data.user_valid_data_t.length = len;
				memcpy( pSMSInd->sms_data.user_valid_data_t.user_valid_data_arr, s_data_ptr, len );
			}
		}

		CTime tmNow;    
		tmNow = CTime::GetCurrentTime();
		pSMSInd->sms_time.year 	= tmNow.GetYear() % 100;
		pSMSInd->sms_time.month	= tmNow.GetMonth();
		pSMSInd->sms_time.day	= tmNow.GetDay();
		pSMSInd->sms_time.hour	= tmNow.GetHour();
		pSMSInd->sms_time.minute= tmNow.GetMinute();
		pSMSInd->sms_time.second= tmNow.GetSecond();
		pSMSInd->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
    
		SendCPMsgToPSDummy( (xSignalHeaderRec *)pSMSInd );
		pstTip->SetWindowText("sent a short message.");
		UpdateWindow();  
		PlaySendSMSSound();
	}
}

void CSmsConPropertyPage::OnButtonSendStatusReport() 
{
	// TODO: Add your control notifiiication handler code here
	if (!((CConPanPropertySheet *)GetParent())->IsPowerOn(sys_id))
	{
		MessageBox("Sorry. Mobile is power off.", "Warning", MB_ICONWARNING);
		return ;
	}

    if (!((CConPanPropertySheet *)GetParent())->IsServiceAvailable(sys_id))
	{
		MessageBox("Sorry. No service available.", "Warning", MB_ICONWARNING);
		return ;
	}

//    CEdit * pedSmsSend = (CEdit *) GetDlgItem(IDC_EDIT_SMSSEND);
	CEdit * pedSendTo = (CEdit *) GetDlgItem(IDC_DEST_ADDR_NUM);
	CEdit * pedScTime = (CEdit *) GetDlgItem(IDC_SC_RECEIVE_TIME);
	CEdit * pedSmeTime = (CEdit *) GetDlgItem(IDC_SME_RECEIVE_TIME);
	CEdit * pedMR= (CEdit *) GetDlgItem(IDC_MR);
	CStatic *pstTip = (CStatic *) GetDlgItem(IDC_STATIC_TIP1);


//    char strServiceCenter[32] = {0};
	char strServiceCenter[32] = {0};
    char strdestaddr[32] = {0};
	char strsctime[14]={0};
	char strsmetime[14]={0};
	char strmr[3]={0};
	MN_SMS_TP_MR_T tp_mr = 0;
 //   char strsms[MN_SMS_USER_DATA_LENGTH + 1] = {0};
    
    GetDlgItemText(IDC_EDIT_SMS_LOCAL_SC, strServiceCenter, sizeof(strServiceCenter));
	if (strlen(strServiceCenter) == 0)
	{
		MessageBox("Sorry, 'service center' can not be empty.",
					"Warning",
					MB_ICONWARNING);
        return;
	}

	GetDlgItemText(IDC_DEST_ADDR_NUM,strdestaddr,32);
	if (strlen(strdestaddr) == 0)
	{
		MessageBox("Sorry, 'dest address' can not be empty.",
					"Warning",
					MB_ICONWARNING);
        return;
	}
	
	GetDlgItemText(IDC_SC_RECEIVE_TIME,strsctime,32);
	if (strlen(strsctime) == 0)
	{
		MessageBox("Sorry, 'sc receive time' can not be empty.",
					"Warning",
					MB_ICONWARNING);
        return;
	}
	else if(strlen(strsctime) != 14)
	{
		MessageBox("Sorry, 'sc receive time' length must be 14 ??year??month??day??hour??minute??second??timezone.",
					"Warning",
					MB_ICONWARNING);
        return;
	}

	GetDlgItemText(IDC_SME_RECEIVE_TIME,strsmetime,32);
	if (strlen(strsmetime) == 0)
	{
		MessageBox("Sorry, 'sme receive time' can not be empty.",
					"Warning",
					MB_ICONWARNING);
        return;
	}
	else if(strlen(strsmetime) != 14)
	{
		MessageBox("Sorry, 'sme receive time' length must be 14 ??year??month??day??hour??minute??second??timezone.",
					"Warning",
					MB_ICONWARNING);
        return;
	}

	GetDlgItemText(IDC_MR,strmr,32);
	SetStrToMR( &tp_mr, (unsigned char *) strmr);
	if (strlen(strmr) == 0)
	{
		MessageBox("Sorry, 'message reference' can not be empty.",
					"Warning",
					MB_ICONWARNING);
        return;
	}
	else if(tp_mr >= 256)
	{
		MessageBox("Sorry, 'message reference' must less than 256.",
					"Warning",
					MB_ICONWARNING);
        return;
	}

//	pedSmsSend->GetWindowText(strsms, sizeof(strsms));	
	
    MN_RL_DATA_IND_T    *pSMSInd;	
	InnerSCICreateSignal(
					  (xSignalHeaderRec **)&pSMSInd,
					  MN_RL_DATA_IND,
					  sizeof(MN_RL_DATA_IND_T),
					  P_CPANEL);

	// SMS_STATUS_REPORT_MSG
	pSMSInd->sms_type = 0x02;
	pSMSInd->smi = 0x01;
	
    memset( &pSMSInd->dcs, 0, sizeof(MN_SMS_DCS_T) );

	SetStrToCall( &pSMSInd->sc_addr, (unsigned char *) strServiceCenter );
	SetStrToCall( &pSMSInd->dest_addr, (unsigned char *) strdestaddr );
	SetStrToTime( &pSMSInd->tp_scts, (unsigned char *) strsctime);
	SetStrToTime( &pSMSInd->tp_dt, (unsigned char *) strsmetime);
	SetStrToMR( &pSMSInd->tp_mr, (unsigned char *) strmr);

	if(1 == m_wndSmsRevBySME.GetCheck())
	{
		pSMSInd->tp_status = MN_SMS_RECEIVED_BY_SME;
	}
	else if(1 == m_wndSmsNotConfirm.GetCheck())
	{
		pSMSInd->tp_status = MN_SMS_NOT_CONFIRM_RECEIVED;
	}
	else if(1 == m_wndSmsReplaceBySC.GetCheck())
	{
		pSMSInd->tp_status = MN_SMS_SM_REPLACED_BY_SC;
	}
	else if(1 == m_wndSmsSendLater.GetCheck())
	{
		pSMSInd->tp_status = MN_SMS_SEND_LATER;
	}
	else if(1 == m_wndSmsNotSendAgain.GetCheck())
	{
		pSMSInd->tp_status = MN_SMS_NOT_SEND_AGAIN;
	}
	else if(1 == m_wndSmsInvalidSR.GetCheck())
	{
		pSMSInd->tp_status = MN_SMS_INVALID_STATUS_REPORT;
	}
//	SetStrToCall( &pSMSInd->sc_addr, (unsigned char *) strServiceCenter );

//	pSMSInd->sms_data.user_data_head_t.length = 0;
	
/*	if ( 1 == m_wndSMSDataType.GetCheck() ) // ASCII
	{
        CUnicodeConvert uniConv;

        int nChineseCharCount = uniConv.GetChineseCharCount( strsms );
        if ( nChineseCharCount == 0 ) // English message
        {
            pSMSInd->dcs.alphabet_type = MN_SMS_DEFAULT_ALPHABET;
	        pSMSInd->sms_data.user_valid_data_t.length = (uint8) strlen( strsms );
	        memcpy( pSMSInd->sms_data.user_valid_data_t.user_valid_data_arr, strsms, strlen(strsms) );
        }
        else // Chinese message
        {
            int nUnicodeCount = strlen( strsms ) - nChineseCharCount;
            WCHAR * pwcUnicode = new WCHAR[ nUnicodeCount ];
            
            uniConv.ConvertToUnicode( pwcUnicode,
                                      nUnicodeCount * sizeof(WCHAR),
                                      strsms,
                                      strlen(strsms) );

            pSMSInd->dcs.alphabet_type = MN_SMS_UCS2_ALPHABET;
            pSMSInd->sms_data.user_valid_data_t.length = nUnicodeCount * sizeof(WCHAR);
            
            // NOTE: do not use memcpy() here because of different endian
            for ( int i = 0; i < nUnicodeCount; i++ )
            {
                pSMSInd->sms_data.user_valid_data_t.user_valid_data_arr[ i * 2 ]     = HIBYTE( *(pwcUnicode + i) );
                pSMSInd->sms_data.user_valid_data_t.user_valid_data_arr[ i * 2 + 1 ] = LOBYTE( *(pwcUnicode + i) );
            }

            delete [] pwcUnicode;
        }
    }
    else // Binary
    {
        pSMSInd->dcs.alphabet_type = MN_SMS_8_BIT_ALPHBET;

        CString strSMS;
        pedSmsSend->GetWindowText( strSMS );

        if ( strSMS.GetLength() % 2 != 0 )
        {
            AfxMessageBox( "Invalid binary data: data length must be even!", MB_ICONSTOP );
            return;
        }
        
        int nByteLength = strSMS.GetLength() / 2;
        BYTE * pbHexByte = new BYTE[ nByteLength ];
        memset( pbHexByte, 0, nByteLength );
        for ( int i = 0; i < nByteLength; i ++ )
        {
            pbHexByte[ i ] = ( BYTE )strtol( strSMS.Mid(i * 2, 2), NULL, 16 );
        }
        
	    pSMSInd->sms_data.user_valid_data_t.length = nByteLength;
	    memcpy( pSMSInd->sms_data.user_valid_data_t.user_valid_data_arr, pbHexByte, nByteLength );
			
		delete [] pbHexByte;
    }*/


    
    pSMSInd->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
	SendCPMsgToPSDummy( (xSignalHeaderRec *)pSMSInd );
    pstTip->SetWindowText("sent a status report.");
    UpdateWindow();  
    PlaySendSMSSound();
}

BOOL CSmsConPropertyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
    SetDlgItemText(IDC_EDIT_SMS_LOCAL_SC,"13033334444");
	SetDlgItemText(IDC_EDIT_SENDFROM,"13612345678");

    CEdit *pedSMSSend = (CEdit *) GetDlgItem(IDC_EDIT_SMSSEND);
    ASSERT(pedSMSSend);
    pedSMSSend->SetLimitText(600);//MN_SMS_USER_DATA_LENGTH);
    
    m_wndSMSDataType.SetCheck(1);
    m_wndSMSScStatus.SetCheck(1);
	m_wndSmsRevBySME.SetCheck(1);

	sms_info.Create(IDD_DIALOG_SMSINFO,this);
	sms_info.ShowWindow(SW_HIDE);

    UpdateWindow();

	//initial sms format data
	sms_info.m_formatData.dcs.msg_is_waiting = FALSE;
	sms_info.m_formatData.dcs.class_is_present = FALSE;
	sms_info.m_formatData.dcs.is_bit_67_set = FALSE;
	sms_info.m_formatData.user_header.ref_type = SMS_CAT_8BIT_REF;
	sms_info.m_formatData.user_header.is_port_exist = FALSE;
	sms_info.m_formatData.is_pid_exist = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSmsConPropertyPage::PlaySendSMSSound()
{
    Beep(2000, 100);
    Sleep(100);
    Beep(2000, 100);
}
void CSmsConPropertyPage::PlayRecvSMSSound()
{
    Beep(2000, 100);
    Sleep(100);
    Beep(2000, 100);
}

void CSmsConPropertyPage::OnMaxtextEditSmssend() 
{
	// TODO: Add your control notification handler code here
    CString csMessage;
    csMessage.Format("Error, Message must be no more than %d characters.", 600);//MN_SMS_USER_DATA_LENGTH);
	MessageBox(csMessage, "Error", MB_ICONSTOP );
}


void CSmsConPropertyPage::StartSmsStatusReport()
{
    SetTimer(ID_TIMER_SMS_STATUS_REPORT, 6000, NULL);
}

void CSmsConPropertyPage::StopSmsStatusReport()
{
    KillTimer(ID_TIMER_SMS_STATUS_REPORT);
}


void CSmsConPropertyPage::OnTimer(UINT nIDEvent) 
{
    StopSmsStatusReport();

    if (nIDEvent == ID_TIMER_SMS_STATUS_REPORT)
    {
        MN_RL_DATA_IND_T 	*pSMSInd;	
        
	    char strServiceCenter[32];
	    char strfrom[32];

	    memset (strServiceCenter, 0, sizeof (strServiceCenter));
	    memset (strfrom, 0, sizeof (strfrom));
	    
	    InnerSCICreateSignal((xSignalHeaderRec **)&pSMSInd,
				      		MN_RL_DATA_IND,
				      		sizeof(MN_RL_DATA_IND_T),
				      		P_CPANEL);
	    
        memset (&pSMSInd->dcs, 0, sizeof (MN_SMS_DCS_T));

		// SMS_STATUS_REPORT_MSG
        pSMSInd->sms_type = 2;

	    GetDlgItemText (IDC_EDIT_SMS_REMOTE_SC, strServiceCenter, sizeof(strServiceCenter));
		if (strlen (strServiceCenter) == 0)
		{
			MessageBox ("Sorry, 'service center' can not be empty.",
						"Warning",
						MB_ICONWARNING);
		}

		GetDlgItemText (IDC_EDIT_RCVEDFROM2,strfrom,32);
		if (strlen (strfrom) == 0)
		{
			MessageBox ("Sorry, 'send from' can not be empty.",
						"Warning",
						MB_ICONWARNING);
		}
		
	    SetStrToCall( &pSMSInd->dest_addr, (unsigned char *) strfrom );
	    SetStrToCall( &pSMSInd->sc_addr, (unsigned char *) strServiceCenter );

	    pSMSInd->sms_data.user_data_head_t.length = 0;
	    
    
        CTime tmNow;    
        tmNow = CTime::GetCurrentTime();
        pSMSInd->sms_time.year 	= tmNow.GetYear() % 100 ;
        pSMSInd->sms_time.month	= tmNow.GetMonth();
        pSMSInd->sms_time.day	= tmNow.GetDay();
        pSMSInd->sms_time.hour	= tmNow.GetHour();
        pSMSInd->sms_time.minute= tmNow.GetMinute();
        pSMSInd->sms_time.second= tmNow.GetSecond();
		pSMSInd->dual_sys = (MN_DUAL_SYS_E)(sys_id -1);
        	

	    SendCPMsgToPSDummy( (xSignalHeaderRec *)pSMSInd );
        UpdateWindow();  
        PlaySendSMSSound();
    }
	    
	CPropertyPage::OnTimer(nIDEvent);
}
void CSmsConPropertyPage::OnBUTTONFormatSet() 
{
	sms_info.CenterWindow();
	sms_info.ShowWindow(SW_SHOW);

	
}
