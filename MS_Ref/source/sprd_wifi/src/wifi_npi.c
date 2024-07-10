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

#include "wifi_api.h"
#include "cfw.h"

static CFW_WIFISENTEST_RES s_wifi_sentest_res = {0};

int iwnpi_cmd_handler(int argc, char **argv, char *iwnpi_ret)
{
    return 0;
}

int wifi_eut_hdlr(char *diag_cmd, char *at_rsp)
{
    return 0;
}

int wifi_sigtest_hdlr(char *at_cmd, char *at_rsp)
{
    return 0;
}

void wifi_npi_sensetivity_test_on(void)
{
    SCI_TRACE_LOW("wifi_npi_sensetivity_test_on");

	CFW_WifiSentestOn();
}

void wifi_npi_sensetivity_test(void *argv)
{
    int ret = -1;
    uint32 result = 0;
    CFW_WIFISENTEST_PARAM sentest_cfg;
	C_F_W_WIFISENTEST_PARAM *wifi_scan_rsp_ptr = (C_F_W_WIFISENTEST_PARAM*)argv;

    SCI_TRACE_LOW("wifi_npi_sensetivity_test");

    SCI_MEMSET(&sentest_cfg, 0, sizeof(CFW_WIFISENTEST_PARAM));
    if (wifi_scan_rsp_ptr)
    {
        sentest_cfg.mode = wifi_scan_rsp_ptr->mode;
        sentest_cfg.ch_id = wifi_scan_rsp_ptr->ch_id;
        sentest_cfg.loop_time = wifi_scan_rsp_ptr->loop_time;
        sentest_cfg.max_loop = wifi_scan_rsp_ptr->max_loop;
        sentest_cfg.resvered = wifi_scan_rsp_ptr->resvered;

        result = CFW_WifiSentestSetParam(&sentest_cfg);
        if (0 != result)
        {
           SCI_TRACE_LOW("wifi_npi_sensetivity_test fail");
        }
    }

	return;
}

void wifi_npi_sensetivity_test_off(void)
{
    SCI_TRACE_LOW("wifi_npi_sensetivity_test_off");

	CFW_WifiSentestOff();
}

void wifi_npi_sensetivity_get_result(void)
{
    uint32 result = 0;

    SCI_MEMSET(&s_wifi_sentest_res, 0, sizeof(CFW_WIFISENTEST_RES));

	result = CFW_WifiSentestGetSingleRes(&s_wifi_sentest_res);

    SCI_TRACE_LOW("wifi_npi_sensetivity_get_result 0x%x, %d %d %d %d, 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",\
		result,s_wifi_sentest_res.mode,s_wifi_sentest_res.ch_id,s_wifi_sentest_res.loop_cnt,s_wifi_sentest_res.max_loop,\
		s_wifi_sentest_res.result[0],s_wifi_sentest_res.result[1],s_wifi_sentest_res.result[2],s_wifi_sentest_res.result[3],s_wifi_sentest_res.result[4],\
		s_wifi_sentest_res.result[5],s_wifi_sentest_res.result[6],s_wifi_sentest_res.result[7],s_wifi_sentest_res.result[8],s_wifi_sentest_res.result[9]);
	
    return;
}

C_F_W_WIFISENTEST_RES wifi_npi_sensetivity_read_result(void)
{
    uint8 i = 0;
    C_F_W_WIFISENTEST_RES ret = {0};

    ret.mode = s_wifi_sentest_res.mode;
    ret.ch_id = s_wifi_sentest_res.ch_id;
    ret.loop_cnt = s_wifi_sentest_res.loop_cnt;
    ret.max_loop = s_wifi_sentest_res.max_loop;
    for(i=0;i<AT_WIFI_SENTEST_MAX_CNT;i++)
    {
        ret.result[i] = s_wifi_sentest_res.result[i];
    }
    ret.resvered = s_wifi_sentest_res.resvered;

    return ret;
}

