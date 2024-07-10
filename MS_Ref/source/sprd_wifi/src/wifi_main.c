/*
* Copyright (c) 2020, Unisoc Communications Inc. All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "wifi_main.h"
#include "cfw.h"

static uint32 s_wifi_scan_thread_id = SCI_INVALID_BLOCK_ID;
static UAL_WIFI_CALLBACK s_wifi_cb = PNULL;
static uint32 s_wifi_signal_code = 0;//for debug

void wifi_main_init(UAL_WIFI_CALLBACK wifi_callback)
{
    if (SCI_INVALID_BLOCK_ID == s_wifi_scan_thread_id)
    {
        s_wifi_scan_thread_id = SCI_CreateThread("T_WIFI_SCAN", "Q_WIFI_SCAN", wifi_main, 0, NULL, 4096,
                                            20, 200, SCI_PREEMPT, SCI_AUTO_START);
    }

	if (PNULL != wifi_callback)
	{
        s_wifi_cb = wifi_callback;
	}

    return;
}

uint32 wifi_main_get_thread_id(void)
{
    return s_wifi_scan_thread_id;
}

void wifi_main_send_msg_up(uint32 msg_id, uint32 body_size, void* body_ptr)
{
    UAL_WIFI_MSG_T wifi_msg = {0};

    //call the mmi callback
    if (s_wifi_cb)
    {
        wifi_msg.msg_id = msg_id;
        wifi_msg.body_size = body_size;
        wifi_msg.body_ptr = body_ptr;
        s_wifi_cb(&wifi_msg);
    }

	return;
}

void wifi_main_scan_start(WIFI_SCAN_CMD_T *param_ptr)
{
    CFW_WIFISCAN_REQ wifiscan_param = {0};

    //SCI_TRACE_LOW("wifi_main_scan_start");

    if (param_ptr)
    {
        wifiscan_param.wswitch = param_ptr->scan_flag;//?
        wifiscan_param.nChNum = param_ptr->nChNum;//?
        wifiscan_param.nMaxNodeNum = param_ptr->nMaxNodeNum;//?
        SCI_MEMCPY(&wifiscan_param.nCh[0],&param_ptr->nCh[0],13*sizeof(uint16));//?

        SCI_TRACE_LOW("wifi_main_scan_start 0x%x, %d, %d. 0x%x 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x, 0x%x 0x%x 0x%x 0x%x, 0x%x",\
		wifiscan_param.wswitch,wifiscan_param.nChNum,wifiscan_param.nMaxNodeNum,\
		wifiscan_param.nCh[0],wifiscan_param.nCh[1],wifiscan_param.nCh[2],wifiscan_param.nCh[3],\
		wifiscan_param.nCh[4],wifiscan_param.nCh[5],wifiscan_param.nCh[6],wifiscan_param.nCh[7],\
		wifiscan_param.nCh[8],wifiscan_param.nCh[9],wifiscan_param.nCh[10],wifiscan_param.nCh[11],\
		wifiscan_param.nCh[12]);

        CFW_WifiScanStart(wifiscan_param,0,0);//UTI?  SIM_Index?
    }

    return;
}

void wifi_main_scan_stop(void)
{
    CFW_WifiScanStop(0,0);//UTI?  SIM_Index?
    return;
}

int wifi_main_is_lte_busy(void)
{
	CFW_NW_STATUS_INFO LteStatus = {0};

	if (0 == CFW_GprsGetstatus(&LteStatus,0))
	{
		return LteStatus.ConnMode;
	}

    return 0;
}

void wifi_main_open(void)
{
    wifi_main_send_msg_up(WIFI_SUPP_EVENT_OPEN_OK,0,PNULL);
    return;
}

void wifi_main_close(void)
{
    wifi_main_send_msg_up(WIFI_SUPP_EVENT_CLOSE_CNF,0,PNULL);
    return;
}

void wifi_main_scan(void *argv)
{
    WIFI_SCAN_CMD_T *scan_para_ptr = (WIFI_SCAN_CMD_T *)argv;

	if (scan_para_ptr)
	{
	    if (scan_para_ptr->scan_flag)
	    {
	        wifi_main_scan_start(scan_para_ptr);

            wifi_main_send_msg_up(WIFI_SUPP_EVENT_START_SCAN_OK,0,PNULL);
	    }
        else
        {
            wifi_main_scan_stop();

            wifi_main_send_msg_up(WIFI_SUPP_EVENT_STOP_SCAN_OK,0,PNULL);
        }
	}

	return;
}

void wifi_main_scan_handle_rsp(void *argv)
{
	WIFISCAN_AP_RESULT *wifi_scan_rsp_ptr = (WIFISCAN_AP_RESULT*)argv;
	C_F_W_WIFISCAN_AP_RESULT *ptemp = PNULL;
	uint16 i = 0;

    SCI_TRACE_LOW("wifi_main_scan_handle_rsp");

    if ((0 != wifi_scan_rsp_ptr->NumAP) 
		&& wifi_scan_rsp_ptr->p_wifiscan_result)
    {
        for(i=0;i<wifi_scan_rsp_ptr->NumAP;i++)
        {
            ptemp = wifi_scan_rsp_ptr->p_wifiscan_result + i;

            SCI_TRACE_LOW("wifi_main_scan_handle_rsp bssid: 0x%x 0x%x, chn=%d, rssi=%d, %d, %d, 0x%x 0x%x 0x%x 0x%x",\
            	ptemp->bssid_low,ptemp->bssid_high,ptemp->channel,ptemp->rssival,ptemp->reserved,\
            	ptemp->ssidlen,ptemp->ssid[0],ptemp->ssid[1],ptemp->ssid[2],ptemp->ssid[3]);

            wifi_sta_set_scan_results(ptemp);

			wifi_main_send_msg_up(WIFI_SUPP_EVENT_SCAN_AP_INFO_IND,sizeof(CFW_WIFISCAN_AP_RESULT),ptemp);
        }

		SCI_FREE(wifi_scan_rsp_ptr->p_wifiscan_result);
		wifi_scan_rsp_ptr->p_wifiscan_result = PNULL;
    }

	wifi_main_send_msg_up(WIFI_SUPP_EVENT_SCAN_AP_INFO_CNF,0,PNULL);

    return;
}

void wifi_main(uint32 argc, void *argv)
{
	WIFI_SIGNAL_T *sig_ptr = SCI_NULL;
	uint32 sys_tick1,sys_tick2 = 0;

    while(1)
    {
		sig_ptr = (WIFI_SIGNAL_T *)SCI_GetSignal(s_wifi_scan_thread_id);
		SCI_ASSERT(NULL != sig_ptr);/*assert verified*/

		sys_tick1 = SCI_GetTickCount();
        s_wifi_signal_code = sig_ptr->SignalCode;
		SCI_TRACE_LOW("wifi_main enter 0x%x, %d",s_wifi_signal_code,sys_tick1);

        switch(s_wifi_signal_code)
        {
            case WIFI_MAIN_OPEN_CMD:
            {
				wifi_main_open();
				break;
            }

            case WIFI_MAIN_CLOSE_CMD:
            {
				wifi_main_close();
				break;
            }

            case WIFI_MAIN_SCAN_CMD:
			{
				//send the command to the ipc
				wifi_main_scan(sig_ptr->SigP);

				//wifi_main_scan_stop();
				break;
			}

            case WIFI_MAIN_SCAN_START_RSP:
			{
				//Recv the response from the MN_WIFI 

				break;
			}

            case WIFI_MAIN_SCAN_RSP:
			{
				//Recv the response from the MN_WIFI
				wifi_main_scan_handle_rsp(sig_ptr->SigP);
				break;
			}

            case WIFI_MAIN_SCAN_STOP_RSP:
			{
				//Recv the response from the MN_WIFI 

				break;
			}

            case WIFI_MAIN_SENSETIVITY_TEST_ON:
            {
				wifi_npi_sensetivity_test_on();
                break;
            }

            case WIFI_MAIN_SENSETIVITY_TEST:
            {
				wifi_npi_sensetivity_test(sig_ptr->SigP);
                break;
            }

            case WIFI_MAIN_SENSETIVITY_TEST_OFF:
            {
				wifi_npi_sensetivity_test_off();
                break;
            }

            case WIFI_MAIN_SENSETIVITY_TEST_RESULT:
            {
				wifi_npi_sensetivity_get_result();
                break;
            }

			default:
				break;
        }

        if (sig_ptr->SigP)
        {
            SCI_FREE(sig_ptr->SigP);
			sig_ptr->SigP = PNULL;
        }
        if (sig_ptr)
        {
            SCI_FREE(sig_ptr);
			sig_ptr = PNULL;
        }

		sys_tick2 = SCI_GetTickCount();
		SCI_TRACE_LOW("wifi_main exit %d",sys_tick2);
    }

    return;
}

