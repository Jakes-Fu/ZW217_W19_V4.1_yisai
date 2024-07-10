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

#include "os_api.h"
#include "wifi_api.h"
#include "wifi_main.h"

#define WIFI_STA_MAX_NUM 60
static struct wifi_bss_info sta_bss_info[WIFI_STA_MAX_NUM] = {0};
static struct wifi_scan_res sta_scan_res = {0};
static uint8 wifi_state = 0;// 0 closed; 1 enabled

extern C_F_W_WIFISENTEST_RES wifi_npi_sensetivity_read_result(void);

void wifi_sta_test_init(void)
{
    wifi_main_init(NULL);
    return;
}


void wifi_sta_init(UAL_WIFI_CALLBACK wifi_callback)
{
    wifi_main_init(wifi_callback);
    return;
}


int wifi_sta_open(void)
{
    WIFI_SIGNAL_T *sig_ptr = SCI_NULL;
    uint32 wifi_thread_id = SCI_INVALID_BLOCK_ID;

    wifi_thread_id = wifi_main_get_thread_id();
    if (SCI_INVALID_BLOCK_ID == wifi_thread_id)
    {
        return -1;
    }

    SCI_TRACE_LOW("wifi_sta_open");

    SCI_MEMSET(&sta_bss_info[0],0,WIFI_STA_MAX_NUM*sizeof(struct wifi_bss_info));
	sta_scan_res.nr_bss = 0;
	sta_scan_res.bsses = sta_bss_info;

	sig_ptr = (WIFI_SIGNAL_T *)SCI_ALLOC(sizeof(WIFI_SIGNAL_T));
	if (sig_ptr)
	{
        sig_ptr->SigP = PNULL;
     	sig_ptr->SignalCode = WIFI_MAIN_OPEN_CMD;
    	sig_ptr->SignalSize = 0;
    	sig_ptr->Sender = SCI_IdentifyThread();
        SCI_SendSignal((void *)sig_ptr, wifi_thread_id);
	}

    wifi_state = 1;
	return 0;
}


int wifi_sta_close(void)
{
    WIFI_SIGNAL_T *sig_ptr = SCI_NULL;
    uint32 wifi_thread_id = SCI_INVALID_BLOCK_ID;

    wifi_thread_id = wifi_main_get_thread_id();
    if (SCI_INVALID_BLOCK_ID == wifi_thread_id)
    {
        return -1;
    }

    SCI_TRACE_LOW("wifi_sta_close");

    SCI_MEMSET(&sta_bss_info[0],0,WIFI_STA_MAX_NUM*sizeof(struct wifi_bss_info));
	sta_scan_res.nr_bss = 0;
	sta_scan_res.bsses = sta_bss_info;

	sig_ptr = (WIFI_SIGNAL_T *)SCI_ALLOC(sizeof(WIFI_SIGNAL_T));
	if (sig_ptr)
	{
        sig_ptr->SigP = PNULL;
     	sig_ptr->SignalCode = WIFI_MAIN_CLOSE_CMD;
    	sig_ptr->SignalSize = 0;
    	sig_ptr->Sender = SCI_IdentifyThread();
        SCI_SendSignal((void *)sig_ptr, wifi_thread_id);
	}

    wifi_state = 0;
    return 0;
}


int wifi_sta_is_open(void)
{
    return wifi_state;
}


int wifi_sta_scan_debug_test(unsigned char scan_on_flag)
{
    WIFISCAN_REQ temp_test_req = {0};

	temp_test_req.wswitch = 1;
	temp_test_req.nChNum = 5;
	temp_test_req.nMaxNodeNum = 20;
	temp_test_req.nCh[0] = 3;
	temp_test_req.nCh[1] = 6;
	temp_test_req.nCh[2] = 8;
	temp_test_req.nCh[3] = 9;
	temp_test_req.nCh[4] = 10;
	temp_test_req.nCh[5] = 0;
	temp_test_req.nCh[6] = 0;
	temp_test_req.nCh[7] = 0;
	temp_test_req.nCh[8] = 0;
	temp_test_req.nCh[9] = 0;
	temp_test_req.nCh[10] = 0;
	temp_test_req.nCh[11] = 0;
	temp_test_req.nCh[12] = 0;

    wifi_sta_scan_start(temp_test_req);

	return 0;
}


int wifi_sta_scan_all_chn_start(void)
{
    WIFISCAN_REQ temp_test_req = {0};

    temp_test_req.wswitch = 1;
    temp_test_req.nChNum = 13;
    temp_test_req.nMaxNodeNum = 30;
    temp_test_req.nCh[0] = 1;
    temp_test_req.nCh[1] = 2;
    temp_test_req.nCh[2] = 3;
    temp_test_req.nCh[3] = 4;
    temp_test_req.nCh[4] = 5;
    temp_test_req.nCh[5] = 6;
    temp_test_req.nCh[6] = 7;
    temp_test_req.nCh[7] = 8;
    temp_test_req.nCh[8] = 9;
    temp_test_req.nCh[9] = 10;
    temp_test_req.nCh[10] = 11;
    temp_test_req.nCh[11] = 12;
    temp_test_req.nCh[12] = 13;

    wifi_sta_scan_start(temp_test_req);

    return 0;
}


int wifi_sta_scan_start(WIFISCAN_REQ scan_req)
{
    WIFI_SIGNAL_T *sig_ptr = SCI_NULL;
    WIFI_SCAN_CMD_T *scan_ptr = SCI_NULL;
    uint32 wifi_thread_id = SCI_INVALID_BLOCK_ID;
    int ret = -1;

    wifi_thread_id = wifi_main_get_thread_id();
    if (SCI_INVALID_BLOCK_ID == wifi_thread_id)
    {
        return ret;
    }

	SCI_TRACE_LOW("wifi_sta_scan_start %d",scan_req.wswitch);

    SCI_MEMSET(&sta_bss_info[0],0,WIFI_STA_MAX_NUM*sizeof(struct wifi_bss_info));
	sta_scan_res.nr_bss = 0;
	sta_scan_res.bsses = sta_bss_info;

	sig_ptr = (WIFI_SIGNAL_T *)SCI_ALLOC(sizeof(WIFI_SIGNAL_T));
	if (sig_ptr)
	{
     	sig_ptr->SignalCode = WIFI_MAIN_SCAN_CMD;
    	sig_ptr->SignalSize = sizeof(WIFI_SIGNAL_T);
    	sig_ptr->Sender = SCI_IdentifyThread();

		scan_ptr = SCI_ALLOC(sizeof(WIFI_SCAN_CMD_T));
		if (scan_ptr)
		{
		    scan_ptr->scan_flag = (uint8)scan_req.wswitch;
			scan_ptr->nChNum = (uint16)scan_req.nChNum;
			scan_ptr->nMaxNodeNum = (uint16)scan_req.nMaxNodeNum;
			SCI_MEMCPY(&scan_ptr->nCh[0], &scan_req.nCh[0], 13*sizeof(uint16));

            sig_ptr->SigP = scan_ptr;
            SCI_SendSignal((void *)sig_ptr, wifi_thread_id);
		}
	}

	return ret;
}


int wifi_sta_scan_stop(void)
{
    WIFI_SIGNAL_T *sig_ptr = SCI_NULL;
    WIFI_SCAN_CMD_T *scan_ptr = SCI_NULL;
    uint32 wifi_thread_id = SCI_INVALID_BLOCK_ID;
    int ret = -1;

    wifi_thread_id = wifi_main_get_thread_id();
    if (SCI_INVALID_BLOCK_ID == wifi_thread_id)
    {
        return ret;
    }
    SCI_TRACE_LOW("wifi_sta_scan_stop");

	sig_ptr = (WIFI_SIGNAL_T *)SCI_ALLOC(sizeof(WIFI_SIGNAL_T));
	if (sig_ptr)
	{
     	sig_ptr->SignalCode = WIFI_MAIN_SCAN_CMD;
    	sig_ptr->SignalSize = sizeof(WIFI_SIGNAL_T);
    	sig_ptr->Sender = SCI_IdentifyThread();

        scan_ptr = SCI_ALLOC(sizeof(WIFI_SCAN_CMD_T));
        if (scan_ptr)
        {
            scan_ptr->scan_flag = 0;

            sig_ptr->SigP = scan_ptr;
            SCI_SendSignal((void *)sig_ptr, wifi_thread_id);
        }
    }

    return ret;
}


void wifi_sta_set_scan_results(C_F_W_WIFISCAN_AP_RESULT *scan_data)
{
    if (PNULL == scan_data)
    {
        return;
    }

    if (sta_scan_res.nr_bss < WIFI_STA_MAX_NUM)
    {
        SCI_MEMCPY(&sta_scan_res.bsses[sta_scan_res.nr_bss].ssid[0],scan_data->ssid,WIFI_MAX_SSID_LEN);
        sta_scan_res.bsses[sta_scan_res.nr_bss].ssid_len = scan_data->ssidlen;

		sta_scan_res.bsses[sta_scan_res.nr_bss].bssid[0] = (char)scan_data->bssid_low;
		sta_scan_res.bsses[sta_scan_res.nr_bss].bssid[1] = (char)(scan_data->bssid_low >> 8);
		sta_scan_res.bsses[sta_scan_res.nr_bss].bssid[2] = (char)(scan_data->bssid_low >> 16);
		sta_scan_res.bsses[sta_scan_res.nr_bss].bssid[3] = (char)(scan_data->bssid_low >> 24);
		sta_scan_res.bsses[sta_scan_res.nr_bss].bssid[4] = (char)scan_data->bssid_high;
		sta_scan_res.bsses[sta_scan_res.nr_bss].bssid[5] = (char)(scan_data->bssid_high >> 8);

		sta_scan_res.bsses[sta_scan_res.nr_bss].channel = scan_data->channel;

		sta_scan_res.bsses[sta_scan_res.nr_bss].rssi = scan_data->rssival;

        sta_scan_res.nr_bss++;
    }

    return;
}


struct wifi_scan_res *wifi_sta_scan_results(void)
{
#if 0//dummy debug
    sta_scan_res.nr_bss = 3;
    
    sta_scan_res.bsses[0].bssid[0] = 1;
    sta_scan_res.bsses[0].bssid[1] = 2;
    sta_scan_res.bsses[0].bssid[2] = 3;
    sta_scan_res.bsses[0].bssid[3] = 4;
    sta_scan_res.bsses[0].bssid[4] = 5;
    sta_scan_res.bsses[0].bssid[5] = 6;
    sta_scan_res.bsses[0].rssi = 70;
    
    sta_scan_res.bsses[1].bssid[0] = 7;
    sta_scan_res.bsses[1].bssid[1] = 8;
    sta_scan_res.bsses[1].bssid[2] = 9;
    sta_scan_res.bsses[1].bssid[3] = 1;
    sta_scan_res.bsses[1].bssid[4] = 3;
    sta_scan_res.bsses[1].bssid[5] = 4;
    sta_scan_res.bsses[1].rssi = 80;
    
    sta_scan_res.bsses[2].bssid[0] = 5;
    sta_scan_res.bsses[2].bssid[1] = 5;
    sta_scan_res.bsses[2].bssid[2] = 4;
    sta_scan_res.bsses[2].bssid[3] = 4;
    sta_scan_res.bsses[2].bssid[4] = 1;
    sta_scan_res.bsses[2].bssid[5] = 1;
    sta_scan_res.bsses[2].rssi = -65;
#endif

    return &sta_scan_res;
}

int wifi_sta_scan_is_lte_busy(void)
{
    return wifi_main_is_lte_busy();
}

int wifi_sensetivity_test_on(void)
{
	WIFI_SIGNAL_T *sig_ptr = SCI_NULL;
	uint32 wifi_thread_id = SCI_INVALID_BLOCK_ID;

	wifi_thread_id = wifi_main_get_thread_id();
	if (SCI_INVALID_BLOCK_ID == wifi_thread_id)
	{
		return -1;
	}

	SCI_TRACE_LOW("wifi_sensetivity_test_on");

	sig_ptr = (WIFI_SIGNAL_T *)SCI_ALLOC(sizeof(WIFI_SIGNAL_T));
	if (sig_ptr)
	{
		sig_ptr->SigP = PNULL;
		sig_ptr->SignalCode = WIFI_MAIN_SENSETIVITY_TEST_ON;
		sig_ptr->SignalSize = 0;
		sig_ptr->Sender = SCI_IdentifyThread();
		SCI_SendSignal((void *)sig_ptr, wifi_thread_id);
	}

	return 0;
}


int wifi_sensetivity_test(C_F_W_WIFISENTEST_PARAM param)
{
	WIFI_SIGNAL_T *sig_ptr = SCI_NULL;
	uint32 wifi_thread_id = SCI_INVALID_BLOCK_ID;
	C_F_W_WIFISENTEST_PARAM *p_param = PNULL;

	wifi_thread_id = wifi_main_get_thread_id();
	if (SCI_INVALID_BLOCK_ID == wifi_thread_id)
	{
		return -1;
	}

	SCI_TRACE_LOW("wifi_sensetivity_test mode=%d ch=%d loop_time=%d max_loop=%d",param.mode,param.ch_id,param.loop_time,param.max_loop);

	sig_ptr = (WIFI_SIGNAL_T *)SCI_ALLOC(sizeof(WIFI_SIGNAL_T));
	if (sig_ptr)
	{
		sig_ptr->SignalCode = WIFI_MAIN_SENSETIVITY_TEST;
		sig_ptr->SignalSize = 0;
		sig_ptr->Sender = SCI_IdentifyThread();

		p_param = SCI_ALLOC(sizeof(C_F_W_WIFISENTEST_PARAM));
		if (p_param)
		{
            SCI_MEMCPY(p_param,&param,sizeof(C_F_W_WIFISENTEST_PARAM));
			if (p_param->max_loop > AT_WIFI_SENTEST_MAX_CNT)
			{
                p_param->max_loop = AT_WIFI_SENTEST_MAX_CNT;
            }

            sig_ptr->SigP = p_param;
            SCI_SendSignal((void *)sig_ptr, wifi_thread_id);
		}
	}

	return 0;
}

int wifi_sensetivity_test_off(void)
{
	WIFI_SIGNAL_T *sig_ptr = SCI_NULL;
	uint32 wifi_thread_id = SCI_INVALID_BLOCK_ID;

	wifi_thread_id = wifi_main_get_thread_id();
	if (SCI_INVALID_BLOCK_ID == wifi_thread_id)
	{
		return -1;
	}

	SCI_TRACE_LOW("wifi_sensetivity_test_off");

	sig_ptr = (WIFI_SIGNAL_T *)SCI_ALLOC(sizeof(WIFI_SIGNAL_T));
	if (sig_ptr)
	{
		sig_ptr->SigP = PNULL;
		sig_ptr->SignalCode = WIFI_MAIN_SENSETIVITY_TEST_OFF;
		sig_ptr->SignalSize = 0;
		sig_ptr->Sender = SCI_IdentifyThread();
		SCI_SendSignal((void *)sig_ptr, wifi_thread_id);
	}

	return 0;
}

int wifi_sensetivity_get_result(void)
{
	WIFI_SIGNAL_T *sig_ptr = SCI_NULL;
	uint32 wifi_thread_id = SCI_INVALID_BLOCK_ID;

	wifi_thread_id = wifi_main_get_thread_id();
	if (SCI_INVALID_BLOCK_ID == wifi_thread_id)
	{
		return -1;
	}

	SCI_TRACE_LOW("wifi_sensetivity_get_result");

	sig_ptr = (WIFI_SIGNAL_T *)SCI_ALLOC(sizeof(WIFI_SIGNAL_T));
	if (sig_ptr)
	{
		sig_ptr->SigP = PNULL;
		sig_ptr->SignalCode = WIFI_MAIN_SENSETIVITY_TEST_RESULT;
		sig_ptr->SignalSize = 0;
		sig_ptr->Sender = SCI_IdentifyThread();
		SCI_SendSignal((void *)sig_ptr, wifi_thread_id);
	}

	return 0;
}

C_F_W_WIFISENTEST_RES wifi_sensetivity_read_result(void)
{
    return wifi_npi_sensetivity_read_result();
}

