/*****************************************************************************
** File Name:      mmieng_memtester.c                                        *
** Author:                                                                   *
** Date:           6/13/2020                                                 *
** Copyright:                                                                *
** Description:    This file is used to implement the Eng MemTester          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 6/2020         tingting.zheng   Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "mmi_menutable.h"
#include "mmieng_menutable.h"
#include "mmieng_text.h"
#include "window_parse.h"
#include "mmieng_id.h"
#include "mmi_signal_ext.h"
#include "mmk_timer.h"
#include "mmieng_memtester.h"
#ifdef GPS_SUPPORT
//#include "mmigps_api.h"
#endif
#include "guilistbox.h"
#include "guiform.h"
#include "guilabel.h"
#include "guiedit.h"
#include "guilcd.h"
#include "guicommon.h"
#include "guitext.h"
#include "mmidisplay_data.h"
#include "sig_code.h"
#include "mmicom_trace.h"
#include "mmieng_export.h"
#include "guidropdownlist.h"
#include "mmivp_export.h"

/**--------------------------------------------------------------------------*
**                macro declaration                                          *
**---------------------------------------------------------------------------*/

typedef unsigned long ul;
typedef unsigned long long ull;
typedef unsigned long volatile ulv;
      #define rand_ul() rand()

 struct test {
     char *name;
     int (*fp)();
 };

#define UL_ONEBITS 0xffffffff
#define UL_LEN 32
#define CHECKERBOARD1 0x55555555
#define CHECKERBOARD2 0xaaaaaaaa
#define UL_BYTE(x) ((x | x << 8 | x << 16 | x << 24))
#define ONE 0x00000001L

#define ENG_MEMTESTER_RESULT_LEN_MAX    (255)
#define ENG_MEMTESTER_TIME_OUT          (1000)// 1S
#define ENG_MEMTESTER_VIDEO_MAX         (1)
#define ENG_MEMTESTER_DURATION_MAX      (6)
#define ENG_MEMTESTER_MEMSIZE_MAX       (3)
#define ENG_MEMTESTER_BLOCK_SIZE        (10*1024)// TODO:*1024*1024 s_memtester_memsize M
#define ENG_MEMTESTER_TRANSPARENCE      (255)
#define ENG_MEMTESTER_MILLISECONDS_IN_HOUR   (3600000)

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

LOCAL uint32 s_memtester_duration = 0;
LOCAL uint32 s_memtester_memsize = 0;
LOCAL uint32 s_memtester_scan_times = 0;
LOCAL uint32 s_memtester_pass_times = 0;
LOCAL uint32 s_memtester_fail_times = 0;
static uint8 s_memtester_timer_id = 0;
static uint8 s_memtester_duration_timer_id = 0;
LOCAL GUI_LCD_DEV_INFO  s_memtester_vp_layer = {0};
LOCAL VP_HANDLE         s_memtester_vp_handle = 0;
const wchar s_memtester_vp_filepath_wstr[] = {'E',':','\\','V','i','d','e','o','\\','m','e','m','t','e','s','t','.','m','p','4', 0 };//E:\Video\MOV_0000002.3gp
static uint32 droplist_test_duration[ENG_MEMTESTER_DURATION_MAX] =
                                    {
                                        TXT_ENG_MEMTEST_DURATION_24H,
                                        TXT_ENG_MEMTEST_DURATION_36H,
                                        TXT_ENG_MEMTEST_DURATION_48H,
                                        TXT_ENG_MEMTEST_DURATION_72H,
                                        TXT_ENG_MEMTEST_DURATION_96H,
                                        TXT_ENG_MEMTEST_DURATION_128H
                                    };

/*---------------------------------------------------------------------------*/
/*                     ALGOTITHM LOCAL FUNCTION DEFINITION                   */
/*---------------------------------------------------------------------------*/
LOCAL int compare_regions(ulv *bufa, ulv *bufb, size_t count)
{
    int r = 0;
    size_t i,j;
    ulv *p1 = bufa;
    ulv *p2 = bufb;

    for (j = 0; j < 20; j++)
    {
        p1 = bufa;
        p2 = bufb;
        //SCI_Sleep(1);
        for (i = 0; i < count; i++, p1++, p2++)
        {
            if (*p1 != *p2)
            {
                r = -1;
            }
            else
            {
                r = 0;
            }
        }
    }
    return r;
}

LOCAL int test_random_value(ulv *bufa, ulv *bufb, size_t count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    ul j = 0;
    size_t i;

    for (i = 0; i < count; i++)
    {
        *p1++ = *p2++ = rand_ul();
    }
    return compare_regions(bufa, bufb, count);
}

LOCAL int test_xor_comparison(ulv *bufa, ulv *bufb, size_t count)
{
      ulv *p1 = bufa;
      ulv *p2 = bufb;
      size_t i;
      ul q = rand_ul();

      for (i = 0; i < count; i++) {
          *p1++ ^= q;
          *p2++ ^= q;
      }
      return compare_regions(bufa, bufb, count);
}

LOCAL int test_sub_comparison(ulv *bufa, ulv *bufb, size_t count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++)
    {
        *p1++ -= q;
        *p2++ -= q;
    }
    return compare_regions(bufa, bufb, count);
}

LOCAL int test_mul_comparison(ulv *bufa, ulv *bufb, size_t count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++)
    {
        *p1++ *= q;
        *p2++ *= q;
    }
    return compare_regions(bufa, bufb, count);
}

LOCAL int test_div_comparison(ulv *bufa, ulv *bufb, size_t count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++)
    {
        if (!q)
        {
            q++;
        }
        *p1++ /= q;
        *p2++ /= q;
    }
    return compare_regions(bufa, bufb, count);
}

LOCAL int test_or_comparison(ulv *bufa, ulv *bufb, size_t count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++)
    {
        *p1++ |= q;
        *p2++ |= q;
    }
    return compare_regions(bufa, bufb, count);
}

LOCAL int test_and_comparison(ulv *bufa, ulv *bufb, size_t count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++)
    {
        *p1++ &= q;
        *p2++ &= q;
    }
    return compare_regions(bufa, bufb, count);
}

LOCAL int test_seqinc_comparison(ulv *bufa, ulv *bufb, size_t count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;
    ul q = rand_ul();

    for (i = 0; i < count; i++)
    {
        *p1++ = *p2++ = (i + q);
    }
    return compare_regions(bufa, bufb, count);
}

LOCAL int test_solidbits_comparison(ulv *bufa, ulv *bufb, size_t count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    ul q;
    size_t i;

    for (j = 0; j < 64; j++)
    {
        q = (j % 2) == 0 ? UL_ONEBITS : 0;
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        for (i = 0; i < count; i++)
        {
            *p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
        }
        if (compare_regions(bufa, bufb, count))
        {
            return -1;
        }
    }
    return 0;
}

LOCAL int test_checkerboard_comparison(ulv *bufa, ulv *bufb, size_t count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    ul q;
    size_t i;

    for (j = 0; j < 64; j++)
    {
        q = (j % 2) == 0 ? CHECKERBOARD1 : CHECKERBOARD2;
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        for (i = 0; i < count; i++)
        {
            *p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
        }
        if (compare_regions(bufa, bufb, count))
        {
            return -1;
        }
    }
    return 0;
}

LOCAL int test_blockseq_comparison(ulv *bufa, ulv *bufb, size_t count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    size_t i;

    for (j = 0; j < 256; j++)
    {
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        for (i = 0; i < count; i++)
        {
            *p1++ = *p2++ = (ul) UL_BYTE(j);
        }
        if (compare_regions(bufa, bufb, count))
        {
            return -1;
        }
    }
    return 0;
}

LOCAL int test_walkbits0_comparison(ulv *bufa, ulv *bufb, size_t count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    size_t i;

    for (j = 0; j < UL_LEN * 2; j++)
    {
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        for (i = 0; i < count; i++)
        {
            if (j < UL_LEN) /* Walk it up. */
            {
                *p1++ = *p2++ = ONE << j;
            }
            else /* Walk it back down. */
            {
                *p1++ = *p2++ = ONE << (UL_LEN * 2 - j - 1);
            }
        }
        if (compare_regions(bufa, bufb, count))
        {
            return -1;
        }
    }
    return 0;
}

LOCAL int test_walkbits1_comparison(ulv *bufa, ulv *bufb, size_t count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    size_t i;

    for (j = 0; j < UL_LEN * 2; j++)
    {
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;

        for (i = 0; i < count; i++)
        {
            if (j < UL_LEN)/* Walk it up. */
            {
                *p1++ = *p2++ = UL_ONEBITS ^ (ONE << j);
            }
            else/* Walk it back down. */
            {
                *p1++ = *p2++ = UL_ONEBITS ^ (ONE << (UL_LEN * 2 - j - 1));
            }
        }
        if (compare_regions(bufa, bufb, count)) {
        return -1;
        }
    }
    return 0;
}

LOCAL int test_bitspread_comparison(ulv *bufa, ulv *bufb, size_t count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j;
    size_t i;

    for (j = 0; j < UL_LEN * 2; j++)
    {
        p1 = (ulv *) bufa;
        p2 = (ulv *) bufb;
        for (i = 0; i < count; i++)
        {
            if (j < UL_LEN)/* Walk it up. */
            {
                *p1++ = *p2++ = (i % 2 == 0)
                    ? (ONE << j) | (ONE << (j + 2))
                    : UL_ONEBITS ^ ((ONE << j)
                                | (ONE << (j + 2)));
            }
            else/* Walk it back down. */
            {
                *p1++ = *p2++ = (i % 2 == 0)
                    ? (ONE << (UL_LEN * 2 - 1 - j)) | (ONE << (UL_LEN * 2 + 1 - j))
                    : UL_ONEBITS ^ (ONE << (UL_LEN * 2 - 1 - j)
                                | (ONE << (UL_LEN * 2 + 1 - j)));
            }
        }
        if (compare_regions(bufa, bufb, count))
        {
            return -1;
        }
    }
    return 0;
}

LOCAL int test_bitflip_comparison(ulv *bufa, ulv *bufb, size_t count)
{
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    unsigned int j, k;
    ul q;
    size_t i;

    for (k = 0; k < UL_LEN; k++)
    {
        q = ONE << k;
        for (j = 0; j < 8; j++)
        {
            q = ~q;
            p1 = (ulv *) bufa;
            p2 = (ulv *) bufb;
            for (i = 0; i < count; i++)
            {
                *p1++ = *p2++ = (i % 2) == 0 ? q : ~q;
            }
            if (compare_regions(bufa, bufb, count))
            {
                return -1;
            }
        }
    }
    return 0;
}

struct test tests[] = {
    { "Random Value", test_random_value },
    { "Compare XOR", test_xor_comparison },
    { "Compare SUB", test_sub_comparison },
    { "Compare MUL", test_mul_comparison },
    { "Compare DIV",test_div_comparison },
    { "Compare OR", test_or_comparison },
    { "Compare AND", test_and_comparison },
    { "Sequential Increment", test_seqinc_comparison },
    { "Solid Bits", test_solidbits_comparison },
    { "Block Sequential", test_blockseq_comparison },
    { "Checkerboard", test_checkerboard_comparison },
    { "Bit Spread", test_bitspread_comparison },
    { "Bit Flip", test_bitflip_comparison },
    { "Walking Ones", test_walkbits1_comparison },
    { "Walking Zeroes", test_walkbits0_comparison },
    //{ "Bit Flip for Tiger", test_bitflip_comparison_tiger},
    { NULL, NULL }
};


/*---------------------------------------------------------------------------*/
/*                     DISPLAY LOCAL FUNCTION DECLARE                        */
/*---------------------------------------------------------------------------*/

LOCAL BOOLEAN EngMemTester_StartPlayVideo( void );

LOCAL BOOLEAN EngMemTester_StopPlayVideo( void );

LOCAL void EngMemTester_GetConfigInfo( void );

LOCAL void EngMemTester_OpenReportWin(void );

/*---------------------------------------------------------------------------*/
/*                     DISPLAY LOCAL FUNCTION DEFINITION                     */
/*---------------------------------------------------------------------------*/
LOCAL BOOLEAN EngMemTester_TestComparison(ulv *bufa, ulv *bufb, size_t count)
{
    ul i = 0;
    BOOLEAN result = FALSE;
    uint32 fail_count = 0;

    for (i=0;;i++)
    {
        if (!tests[i].name) break;
        if (!tests[i].fp(bufa, bufb, count))
        {
            SCI_TraceLow("EngMemTesterResult %s pass!\n",tests[i].name);
        }
        else
        {
            SCI_TraceLow("EngMemTesterResult %s fail!\n",tests[i].name);
            fail_count ++;
        }
    }
    if( 0 < fail_count)
    {
        result = FALSE;
    }
    else
    {
        result = TRUE;
    }
    return result;
}

LOCAL BOOLEAN EngMemTester_StartMemTest(void)
{
    void *aligned = PNULL;
    size_t wantbytes = 0;
    size_t wantbytes_orig = 0;
    size_t halflen = 0;
    size_t count = 0;
    ulv *bufa  = 0;
    ulv *bufb = 0;
    BOOLEAN app_heap_result = FALSE;
    BOOLEAN base_heap_result = FALSE;
    BOOLEAN const_heap_result = FALSE;

    wantbytes = s_memtester_memsize*ENG_MEMTESTER_BLOCK_SIZE;
    SCI_TraceLow("EngMemTester Enter StartTest");
    //----------------Scan APP HEAP-------------------//
    //--MAX_DYNAMIC_APP_HEAP_SIZE  (20 * 1024 * 1024)-//

    aligned = SCI_ALLOC_APP(wantbytes);
    if (aligned == PNULL) {
        return FALSE;
    }
    SCI_TraceLow("EngMemTester Start scan APP HEAP!\n");

    halflen = wantbytes / 2;
    count = halflen / sizeof(ul);
    bufa = (ulv *) aligned;
    bufb = (ulv *) ((size_t) aligned + halflen);
    SCI_TraceLow("EngMemTester log. halflen = %d, count = %d",halflen,count);

    //set value and compare
    app_heap_result = EngMemTester_TestComparison(bufa, bufb, count);
    SCI_TraceLow("EngMemTester Scan APP HEAP Done!\n");
	if( PNULL != aligned)
	{
		SCI_FREE(aligned);
		aligned = PNULL;
	}

    //----------------Scan BASE HEAP-------------------//
    //--MAX_DYNAMIC_APP_HEAP_SIZE  (20 * 1024 * 1024)-//

    aligned = SCI_ALLOC_BASE(wantbytes);
    if (aligned == PNULL) {
        return FALSE;
    }
    SCI_TraceLow("EngMemTester Start scan BASE HEAP!\n");

    halflen = wantbytes / 2;
    count = halflen / sizeof(ul);
    bufa = (ulv *) aligned;
    bufb = (ulv *) ((size_t) aligned + halflen);
    SCI_TraceLow("EngMemTester log. halflen = %d, count = %d",halflen,count);

    //set value and compare
    base_heap_result = EngMemTester_TestComparison(bufa, bufb, count);
    SCI_TraceLow("EngMemTester Scan BASE HEAP Done!\n");

	if( PNULL != aligned)
	{
		SCI_FREE(aligned);
		aligned = PNULL;
	}

    //----------------Scan CONST HEAP-------------------//
    //----------MAX_STATIC_HEAP_SIZE     (1000*1024)----//

    //wantbytes = 100*1024;
    SCI_TraceLow("EngMemTester wantbytes = %d\n",wantbytes);
    aligned = SCI_ALLOC_CONST(wantbytes);
    if (aligned == PNULL) {
        return FALSE;
    }
    SCI_TraceLow("EngMemTester Start scan CONST HEAP!\n");

    halflen = wantbytes / 2;
    count = halflen / sizeof(ul);
    bufa = (ulv *) aligned;
    bufb = (ulv *) ((size_t) aligned + halflen);
    SCI_TraceLow("EngMemTester log. halflen = %d, count = %d",halflen,count);

    //set value and compare
    const_heap_result = EngMemTester_TestComparison(bufa, bufb, count);
    SCI_TraceLow("EngMemTester Scan CONST HEAP Done!\n"); 

	if( PNULL != aligned)
	{
		SCI_FREE(aligned);
		aligned = PNULL;
	}
    SCI_TraceLow("EngMemTester Test Done!\n");

    if(app_heap_result && base_heap_result && const_heap_result)
    {
        s_memtester_pass_times ++;
    }
    else
    {
        s_memtester_fail_times ++;
    }
    return TRUE;
}


LOCAL void EngMemTester_VpHandleFsmMsg(VP_CALLBACK_T *msg_ptr)
{
    MMI_WIN_ID_T  win_id    = MMIENG_MEM_TESTER_WIN_ID;
    if (PNULL == msg_ptr)
    {
        SCI_TraceLow("EngMemTester_VpHandleFsmMsg :msg_ptr=PNULL");
        return;
    }
    if (PNULL == msg_ptr->para)
    {
        SCI_TraceLow("EngMemTester_VpHandleFsmMsg :msg_ptr->para=PNULL");
        return;
    }
    if(!MMK_IsFocusWin(win_id))
    {
        SCI_TraceLow("cur win losed focus");
        return;
    }
    switch(msg_ptr->msg_id)
    {
        case VP_PLAY_CNF://开始播放
        {
            SCI_TraceLow("EngMemTester VP_PLAY_CNF");
        }
        break;
        case VP_PLAY_END_IND://播放结束
        {
            SCI_TraceLow("EngMemTester VP_PLAY_END_IND");
            SCI_TraceLow("EngMemTester s_memtester_duration_timer_id = %d",s_memtester_duration_timer_id);
            if(0 != s_memtester_duration_timer_id)//duration not time out,continue play
            {
                MMK_PostMsg(MMIENG_MEM_TESTER_WIN_ID, MSG_ENG_MEMTESTER_CONTINUE_PLAY, NULL, NULL);
            }
            else
            {
                MMK_PostMsg(MMIENG_MEM_BUTTON_STOP_ID, MSG_APP_WEB, NULL, NULL);
            }
        }
        break;
        case VP_STOP_IND: //全部停止播放
        {
            SCI_TraceLow("EngMemTester VP_STOP_IND");
        }
        break;
        default:
        {
             //do nothing
        }
        break;
    }
}

LOCAL BOOLEAN EngMemTester_StopPlayVideo( void )
{
    BOOLEAN     result = FALSE;
    if(UILAYER_IsBltLayer(&s_memtester_vp_layer))
    {
        UILAYER_Clear(&s_memtester_vp_layer);
    }

    result = MMIAPIVP_FSMStop(s_memtester_vp_handle);
    SCI_TraceLow("EngMemTester MMIAPIVP_FSMStop result = %d",result);
    GUIBLOCK_ResetMainType();
    MMIAPIVP_FSMRelease(s_memtester_vp_handle);
    s_memtester_vp_handle = 0;
    return result;
}

LOCAL BOOLEAN EngMemTester_StartPlayVideo( void )
{
    BOOLEAN        result         = MMI_RESULT_TRUE;
    UILAYER_CREATE_T     create_info  = {0};
    UILAYER_APPEND_BLT_T append_layer = {0};
    VP_PLAY_PARAM_T*    play_param_ptr = PNULL;
    GUI_RECT_T          display_rect   = {0,0,239,199};

    // 1.Create Fsm
    s_memtester_vp_handle = MMIAPIVP_FSMInit(EngMemTester_VpHandleFsmMsg);

    // 2.Create and blent vp layer
#ifdef UI_MULTILAYER_SUPPORT
    if(UILAYER_IsBltLayer(&s_memtester_vp_layer))
    {
       UILAYER_RELEASELAYER(&s_memtester_vp_layer);
    }

    if (UILAYER_IsMultiLayerEnable())//创建图层
    {
        create_info.lcd_id          = GUI_MAIN_LCD_ID;
        create_info.owner_handle    = MMIENG_MEM_TESTER_WIN_ID;
        create_info.offset_x        = 0;
        create_info.offset_y        = 0;
        create_info.width           = 240;
        create_info.height          = 200;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;
        UILAYER_CreateLayer(&create_info, &s_memtester_vp_layer);
    }
    if(!UILAYER_IsBltLayer(&s_memtester_vp_layer))//加到图层队列中
    {
       append_layer.layer_level  = UILAYER_LEVEL_NORMAL;
       append_layer.lcd_dev_info = s_memtester_vp_layer;
       UILAYER_AppendBltLayer(&append_layer);
    }
#endif
    GUIBLOCK_SetType(ENG_MEMTESTER_TRANSPARENCE, MMIAPIVP_GetTransparenceColor(),GUIBLOCK_TYPE_OSD, (GUI_LCD_DEV_INFO *)MMITHEME_GetDefaultLcdDev());

    // 3.Play Video
    play_param_ptr = (VP_PLAY_PARAM_T*)SCI_ALLOCA(sizeof(VP_PLAY_PARAM_T));
    if (PNULL == play_param_ptr)
    {
        SCI_TraceLow("EngMemTester play_param_ptr = PNULL");
        return MMI_RESULT_FALSE;
    }
    SCI_MEMSET(play_param_ptr,0,sizeof(VP_PLAY_PARAM_T));

    play_param_ptr->dual_sys = 0;
    play_param_ptr->display_param.audioSupport = 1;
    play_param_ptr->display_param.videoSupport = 1;
    play_param_ptr->display_param.b_support_seek = 1;
    play_param_ptr->display_param.disp_mode = DPLAYER_DISP_FULLVIEW;
    play_param_ptr->display_param.RotateAngle = 0;

    play_param_ptr->is_scal_up = FALSE;

    play_param_ptr->display_param.disp_rect.dy = display_rect.bottom - display_rect.top + 1;
    play_param_ptr->display_param.disp_rect.dx = display_rect.right - display_rect.left + 1;
    play_param_ptr->display_param.disp_rect.x  = display_rect.left;
    play_param_ptr->display_param.disp_rect.y  = display_rect.top;

    play_param_ptr->display_param.target_rect.x  = play_param_ptr->display_param.disp_rect.x;
    play_param_ptr->display_param.target_rect.y  = play_param_ptr->display_param.disp_rect.y;
    play_param_ptr->display_param.target_rect.dx = play_param_ptr->display_param.disp_rect.dx;
    play_param_ptr->display_param.target_rect.dy = play_param_ptr->display_param.disp_rect.dy;

    play_param_ptr->video_buf_info.type = 0;
    play_param_ptr->video_buf_info.video_buf_len = 0;
    play_param_ptr->video_buf_info.video_buf_ptr = PNULL;
    play_param_ptr->play_type = VP_PLAY_FROM_FILE;

    play_param_ptr->full_path_len = MMIAPICOM_Wstrlen(s_memtester_vp_filepath_wstr);
    play_param_ptr->full_path_ptr = s_memtester_vp_filepath_wstr;

    play_param_ptr->url_ptr = PNULL;
    play_param_ptr->mode = 0;
    play_param_ptr->is_random_play = FALSE;

    play_param_ptr->vol = MMIAPISET_GetMultimVolume();
    result = MMIAPIVP_FSMPlay(s_memtester_vp_handle,play_param_ptr);
    SCI_TraceLow("EngMemTester :PlayVideo result is %d",result);
    if( PNULL != play_param_ptr )
    {
        SCI_FREE(play_param_ptr);
        play_param_ptr = PNULL;
    }

    return result;
}

LOCAL void EngMemTester_StartButtonCallback( void )
{
    SCI_TraceLow("EngMemTester ENTER Start");
    MMIDEFAULT_AllowTurnOffBackLight(FALSE);
    //get config info
    EngMemTester_GetConfigInfo();
    //play radio first
    if(0 != s_memtester_vp_handle)
    {
        EngMemTester_StopPlayVideo();
    }
    EngMemTester_StartPlayVideo();
    //clean test result
    s_memtester_scan_times = 0;
    s_memtester_pass_times = 0;
    s_memtester_fail_times = 0;
    //start mem test timer
    if (s_memtester_timer_id != 0)
    {
        MMK_StopTimer(s_memtester_timer_id);
        s_memtester_timer_id = 0;
    }
    s_memtester_timer_id = MMK_CreateWinTimer(MMIENG_MEM_TESTER_WIN_ID, ENG_MEMTESTER_TIME_OUT, FALSE);

    //start test duration timer
    if (s_memtester_duration_timer_id != 0)
    {
        MMK_StopTimer(s_memtester_duration_timer_id);
        s_memtester_duration_timer_id = 0;
    }
    s_memtester_duration_timer_id = MMK_CreateWinTimer(MMIENG_MEM_TESTER_WIN_ID, s_memtester_duration, FALSE);

}

LOCAL void EngMemTester_StopButtonCallback(void)
{
    SCI_TraceLow("EngMemTester ENTER Stop");
    MMIDEFAULT_AllowTurnOffBackLight(TRUE);

    //play radio first
    EngMemTester_StopPlayVideo();
    //stop mem test timer
    MMK_PostMsg(MMIENG_MEM_TESTER_WIN_ID, MSG_FULL_PAINT, NULL, NULL);
    if (s_memtester_timer_id != 0)
    {
        MMK_StopTimer(s_memtester_timer_id);
        s_memtester_timer_id = 0;
    }

    //stop test duration timer
    if (s_memtester_duration_timer_id != 0)
    {
        MMK_StopTimer(s_memtester_duration_timer_id);
        s_memtester_duration_timer_id = 0;
    }
}

LOCAL void EngMemTester_ReportButtonCallback(void)
{
    SCI_TraceLow("EngMemTester ENTER Report");
    if(0 == s_memtester_vp_handle)//only responce when stop video
    {
        EngMemTester_OpenReportWin();
    }
}

LOCAL void EngMemTester_GetConfigInfo( void )
{
    uint16          curIdx = 0;
    uint8           play_duration_char[15] = {0};
    uint32          play_duration_hour = 0;
    MMI_STRING_T    play_duration_str = {0};

    SCI_TraceLow("EngMemTester_GetConfigInfo");
    curIdx = GUIDROPDOWNLIST_GetCurItemIndex(MMIENG_MEM_DROPLIST_PLAY_TIMES_ID);
    MMI_GetLabelTextByLang(droplist_test_duration[curIdx],&play_duration_str); //get name string
    MMI_WSTRNTOSTR((uint8*)play_duration_char, 15, play_duration_str.wstr_ptr,play_duration_str.wstr_len,play_duration_str.wstr_len);
    play_duration_hour = (uint32)atoi(play_duration_char);

    s_memtester_duration = play_duration_hour * ENG_MEMTESTER_MILLISECONDS_IN_HOUR;
    s_memtester_memsize = GUIDROPDOWNLIST_GetCurItemIndex(MMIENG_MEM_DROPLIST_MEMSIZE_ID) + 1;
    SCI_TraceLow("MemTester play_duration_hour = %d",play_duration_hour);
    SCI_TraceLow("MemTester s_memtester_memsize = %d",s_memtester_memsize);
}

LOCAL void EngMemTester_SetCtrlParam(MMI_WIN_ID_T win_id)
{
    uint8 i = 0;
    GUI_BG_T    bg_btn	= {0};
    GUI_BG_T    display_bg = {0};
    MMI_STRING_T text_video = {0};
    MMI_STRING_T text_playtimes = {0};
    MMI_STRING_T text_memsize = {0};
    MMI_STRING_T text_start = {0};
    MMI_STRING_T text_report = {0};
    MMI_STRING_T text_stop = {0};
    GUIFORM_CHILD_HEIGHT_T height_info = {0};
    GUIFORM_CHILD_HEIGHT_T form_height = {0};
    GUIFORM_CHILD_WIDTH_T form_width  = {0};
    GUI_FONT_ALL_T        font_info          = {0};

    uint32 droplist_video[ENG_MEMTESTER_VIDEO_MAX] = {TXT_ENG_MEMTEST_VIDEO_NAME};
    uint32 droplist_memsize[ENG_MEMTESTER_MEMSIZE_MAX] =
                                    {
                                        TXT_ENG_MEMTEST_MIMSIZE_1M,
                                        TXT_ENG_MEMTEST_MIMSIZE_2M,
                                        TXT_ENG_MEMTEST_MIMSIZE_3M
                                    };

    display_bg.bg_type = GUI_BG_COLOR;
    display_bg.color   = MMI_WHITE_COLOR;

    //form
    GUIFORM_SetType(MMIENG_FORM_CTRL_ID, GUIFORM_TYPE_TP);
    GUIFORM_SetBg(MMIENG_FORM_CTRL_ID,&display_bg);
    GUIFORM_PermitChildFont(MMIENG_FORM_CTRL_ID,FALSE);

    //set unit
    GUIFORM_SetStyle(MMIENG_MEM_FORM_VIDEO_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIENG_MEM_FORM_PLAY_TIMES_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIENG_MEM_FORM_MEMSIZE_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIENG_MEM_FORM_OPERATE_ID,GUIFORM_STYLE_UNIT);

    //height value
    form_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
    form_height.add_data = 60;
    //width value
    form_width.type = GUIFORM_CHILD_WIDTH_PERCENT;
    form_width.add_data = 65;
    height_info.type = GUIFORM_CHILD_HEIGHT_AUTO;

    //set video form height & width
    GUIFORM_SetChildHeight(MMIENG_FORM_CTRL_ID,MMIENG_MEM_FORM_VIDEO_ID,&form_height);
    GUIFORM_SetChildHeight(MMIENG_MEM_FORM_VIDEO_ID,MMIENG_COMMON_LABEL_CTRL1_ID,&form_height);
    GUIFORM_SetChildHeight(MMIENG_MEM_FORM_VIDEO_ID,MMIENG_MEM_DROPLIST_VIDEO_ID,&form_height);
    CTRLFORM_SetChildWidth(MMIENG_MEM_FORM_VIDEO_ID,MMIENG_MEM_DROPLIST_VIDEO_ID,&form_width);
    GUIFORM_SetChildHeight(MMIENG_MEM_FORM_VIDEO_ID, MMIENG_MEM_DROPLIST_VIDEO_ID, &height_info);

    form_width.add_data = 30;

    //set play times form height & width
    GUIFORM_SetChildHeight(MMIENG_FORM_CTRL_ID,MMIENG_MEM_FORM_PLAY_TIMES_ID,&form_height);
    GUIFORM_SetChildHeight(MMIENG_MEM_FORM_PLAY_TIMES_ID,MMIENG_COMMON_LABEL_CTRL2_ID,&form_height);
    GUIFORM_SetChildHeight(MMIENG_MEM_FORM_PLAY_TIMES_ID,MMIENG_MEM_DROPLIST_PLAY_TIMES_ID,&form_height);
    CTRLFORM_SetChildWidth(MMIENG_MEM_FORM_PLAY_TIMES_ID,MMIENG_MEM_DROPLIST_PLAY_TIMES_ID,&form_width);
    GUIFORM_SetChildHeight(MMIENG_MEM_FORM_PLAY_TIMES_ID, MMIENG_MEM_DROPLIST_PLAY_TIMES_ID, &height_info);

    //set memsize form height & width
    GUIFORM_SetChildHeight(MMIENG_FORM_CTRL_ID,MMIENG_MEM_FORM_MEMSIZE_ID,&form_height);
    GUIFORM_SetChildHeight(MMIENG_MEM_FORM_MEMSIZE_ID,MMIENG_COMMON_LABEL_CTRL3_ID,&form_height);
    GUIFORM_SetChildHeight(MMIENG_MEM_FORM_MEMSIZE_ID,MMIENG_MEM_DROPLIST_MEMSIZE_ID,&form_height);
    CTRLFORM_SetChildWidth(MMIENG_MEM_FORM_MEMSIZE_ID,MMIENG_MEM_DROPLIST_MEMSIZE_ID,&form_width);
    GUIFORM_SetChildHeight(MMIENG_MEM_FORM_MEMSIZE_ID, MMIENG_MEM_DROPLIST_MEMSIZE_ID, &height_info);

    //set operate form height & width
    form_height.add_data = 40;
    GUIFORM_SetStyle(MMIENG_MEM_FORM_OPERATE_ID,GUIFORM_STYLE_NORMAL);
    GUIFORM_SetChildHeight(MMIENG_FORM_CTRL_ID,MMIENG_MEM_FORM_OPERATE_ID,&form_height);
    GUIFORM_SetChildHeight(MMIENG_MEM_FORM_OPERATE_ID,MMIENG_MEM_BUTTON_START_ID,&form_height);
    GUIFORM_SetChildHeight(MMIENG_MEM_FORM_OPERATE_ID,MMIENG_MEM_BUTTON_REPORT_ID,&form_height);
    GUIFORM_SetChildHeight(MMIENG_MEM_FORM_OPERATE_ID,MMIENG_MEM_BUTTON_STOP_ID,&form_height);

    //set video form content
    MMI_GetLabelTextByLang(TXT_ENG_MEMTEST_VIDEO,&text_video);
    GUILABEL_SetText(MMIENG_COMMON_LABEL_CTRL1_ID, &text_video, TRUE);
    GUIDROPDOWNLIST_SetVisible(MMIENG_MEM_DROPLIST_VIDEO_ID,TRUE,TRUE);
    GUIDROPDOWNLIST_SetMaxItem(MMIENG_MEM_DROPLIST_VIDEO_ID,1);
    for(i=0;i<ARR_SIZE(droplist_video);i++)
    {
        GUIDROPDOWNLIST_AppendItemById(MMIENG_MEM_DROPLIST_VIDEO_ID,droplist_video[i]);
    }
    GUIDROPDOWNLIST_SetCurItemIndex(MMIENG_MEM_DROPLIST_VIDEO_ID,0);

    //set test duration form content
    MMI_GetLabelTextByLang(TXT_ENG_MEMTEST_DURATION,&text_playtimes);
    GUILABEL_SetText(MMIENG_COMMON_LABEL_CTRL2_ID, &text_playtimes, TRUE);
    GUIDROPDOWNLIST_SetMaxItem(MMIENG_MEM_DROPLIST_PLAY_TIMES_ID,6);
    for(i=0;i<ARR_SIZE(droplist_test_duration);i++)
    {
        GUIDROPDOWNLIST_AppendItemById(MMIENG_MEM_DROPLIST_PLAY_TIMES_ID,droplist_test_duration[i]);
    }
    GUIDROPDOWNLIST_SetCurItemIndex(MMIENG_MEM_DROPLIST_PLAY_TIMES_ID,0);

    //set memsize form content
    MMI_GetLabelTextByLang(TXT_ENG_MEMTEST_MIMSIZE,&text_memsize);
    GUILABEL_SetText(MMIENG_COMMON_LABEL_CTRL3_ID, &text_memsize, TRUE);
    GUIDROPDOWNLIST_SetMaxItem(MMIENG_MEM_DROPLIST_MEMSIZE_ID,3);
    for(i=0;i<ARR_SIZE(droplist_memsize);i++)
    {
        GUIDROPDOWNLIST_AppendItemById(MMIENG_MEM_DROPLIST_MEMSIZE_ID,droplist_memsize[i]);
    }
    GUIDROPDOWNLIST_SetCurItemIndex(MMIENG_MEM_DROPLIST_MEMSIZE_ID,0);
    MMK_SetAtvCtrl(win_id,MMIENG_MEM_FORM_VIDEO_ID);

    MMI_GetLabelTextByLang(TXT_ENG_MEMTEST_START, &text_start);
    MMI_GetLabelTextByLang(TXT_ENG_MEMTEST_REPORT, &text_report);
    MMI_GetLabelTextByLang(TXT_ENG_MEMTEST_STOP, &text_stop);

    font_info.font  = WATCH_DEFAULT_SMALL_FONT;
    font_info.color = MMI_DARK_BLUE_COLOR;
    bg_btn.bg_type = GUI_BG_IMG;
#ifdef GUIF_SOFTKEY_PDASTYLE
    bg_btn.img_id = IMAGE_PDASOFTKEY_PRESSED_BUTTON;
#else
    bg_btn.img_id = IMAGE_SOFTKEY_BG;
#endif

    GUIBUTTON_SetTextAlign(MMIENG_MEM_BUTTON_START_ID,ALIGN_HVMIDDLE);
    GUIBUTTON_SetText(MMIENG_MEM_BUTTON_START_ID,text_start.wstr_ptr,text_start.wstr_len);
    GUIBUTTON_SetFont(MMIENG_MEM_BUTTON_START_ID,&font_info);
    GUIBUTTON_SetPressedBg(MMIENG_MEM_BUTTON_START_ID, &bg_btn);
    GUIBUTTON_SetCallBackFunc(MMIENG_MEM_BUTTON_START_ID, EngMemTester_StartButtonCallback);//the button get param of TP

    GUIBUTTON_SetTextAlign(MMIENG_MEM_BUTTON_REPORT_ID,ALIGN_HVMIDDLE);
    GUIBUTTON_SetText(MMIENG_MEM_BUTTON_REPORT_ID,text_report.wstr_ptr,text_report.wstr_len);
    GUIBUTTON_SetFont(MMIENG_MEM_BUTTON_REPORT_ID,&font_info);
    GUIBUTTON_SetPressedBg(MMIENG_MEM_BUTTON_REPORT_ID, &bg_btn);
    GUIBUTTON_SetCallBackFunc(MMIENG_MEM_BUTTON_REPORT_ID, EngMemTester_ReportButtonCallback);//the button get param of TP

    GUIBUTTON_SetTextAlign(MMIENG_MEM_BUTTON_STOP_ID,ALIGN_HVMIDDLE);
    GUIBUTTON_SetText(MMIENG_MEM_BUTTON_STOP_ID,text_stop.wstr_ptr,text_stop.wstr_len);
    GUIBUTTON_SetFont(MMIENG_MEM_BUTTON_STOP_ID,&font_info);
    GUIBUTTON_SetPressedBg(MMIENG_MEM_BUTTON_STOP_ID, &bg_btn);
    GUIBUTTON_SetCallBackFunc(MMIENG_MEM_BUTTON_STOP_ID, EngMemTester_StopButtonCallback);//the button get param of TP

}
LOCAL MMI_RESULT_E EngMemTester_ReportWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    char str[ENG_MEMTESTER_RESULT_LEN_MAX] = {""};
    char str_tmp[ENG_MEMTESTER_RESULT_LEN_MAX] = {""};
    wchar wstr[ENG_MEMTESTER_RESULT_LEN_MAX] = {0};
    int length = 0;

    MMI_RESULT_E result = MMI_RESULT_TRUE;
    SCI_TraceLow("EngMemTester EngMemTester_ReportWinHandleMsg");

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            memset(str, 0, sizeof(char)*ENG_MEMTESTER_RESULT_LEN_MAX);
            memset(wstr, 0, sizeof(wchar)*ENG_MEMTESTER_RESULT_LEN_MAX);
            strcat(str, "Total Scan Times:");
            sprintf((char*)str_tmp, "%d",s_memtester_scan_times);
            strcat(str, str_tmp);
            strcat(str, "\n");
            strcat(str, "Pass times:");
            memset(str_tmp, 0, sizeof(char)*ENG_MEMTESTER_RESULT_LEN_MAX);
            sprintf((char*)str_tmp, "%d",s_memtester_pass_times);
            strcat(str, str_tmp);
            strcat(str, "\n");
            strcat(str, "Fail times:");
            memset(str_tmp, 0, sizeof(char)*ENG_MEMTESTER_RESULT_LEN_MAX);
            sprintf((char*)str_tmp, "%d",s_memtester_fail_times);
            strcat(str, str_tmp);
            strcat(str, "\n");
            strcat(str, "Please see detail infomation in armlog.\n");
            strcat(str, "Search Key code: EngMemTester \n");
            length = strlen(str);
            MMI_STRNTOWSTR( wstr, ENG_MEMTESTER_RESULT_LEN_MAX, (uint8*)str, length, length);
            GUITEXT_SetString(MMIENG_MEM_TESTER_REPROT_ID, wstr, length, FALSE);
            MMK_SetAtvCtrl(win_id,MMIENG_MEM_TESTER_REPROT_ID);
        }
        break;
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL:
        {
            MMK_CloseWin(win_id);
        }
        break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return (result);

}

LOCAL MMI_RESULT_E EngMemTester_MainWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id,DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    uint16       cur_selection  = 0;
    MMI_STRING_T itemStr        = {0};
    GUI_BG_T    bg_btn	= {0};
    GUI_FONT_ALL_T font_info          = {0};
    SCI_TraceLow("EngMemTester_MainWinHandleMsg msg_id = 0x%x",msg_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            EngMemTester_SetCtrlParam(win_id);
        }
        break;

        case MSG_APP_WEB:
        case MSG_CTL_OK://start
        {
            if(0 == s_memtester_vp_handle)//only responce when stop video
            {
                EngMemTester_StartButtonCallback();
            }
        }
        break;
        case MSG_KEYDOWN_CANCEL:
        case MSG_CTL_CANCEL://stop
        {
            if(0 != s_memtester_vp_handle)
            {
                EngMemTester_StopButtonCallback();//if video playing then stop
            }
            else
            {
                MMK_CloseWin(win_id);//if video stop,close window
            }
        }
        break;
        case MSG_NOTIFY_GET_ACTIVE:
        {
            bg_btn.bg_type = GUI_BG_IMG;
#ifdef GUIF_SOFTKEY_PDASTYLE
            bg_btn.img_id = IMAGE_PDASOFTKEY_PRESSED_BUTTON;
#else
            bg_btn.img_id = IMAGE_SOFTKEY_BG;
#endif
            font_info.font  = WATCH_DEFAULT_SMALL_FONT;
            font_info.color = MMI_YELLOW_COLOR;
            if(MMIENG_MEM_BUTTON_REPORT_ID == MMK_GetActiveCtrlId(win_id))
            {
                CTRLBUTTON_SetBg(MMIENG_MEM_BUTTON_REPORT_ID,&bg_btn);
                GUIBUTTON_SetFont(MMIENG_MEM_BUTTON_REPORT_ID,&font_info);

                bg_btn.img_id = IMAGE_PDASOFTKEY_GRAYED_BUTTON;
                font_info.color = MMI_DARK_BLUE_COLOR;

                CTRLBUTTON_SetBg(MMIENG_MEM_BUTTON_START_ID,&bg_btn);
                GUIBUTTON_SetFont(MMIENG_MEM_BUTTON_START_ID,&font_info);

                CTRLBUTTON_SetBg(MMIENG_MEM_BUTTON_STOP_ID,&bg_btn);
                GUIBUTTON_SetFont(MMIENG_MEM_BUTTON_STOP_ID,&font_info);
            }
            else if(MMIENG_MEM_BUTTON_START_ID == MMK_GetActiveCtrlId(win_id))
            {
                CTRLBUTTON_SetBg(MMIENG_MEM_BUTTON_START_ID,&bg_btn);
                GUIBUTTON_SetFont(MMIENG_MEM_BUTTON_START_ID,&font_info);

                bg_btn.img_id = IMAGE_PDASOFTKEY_GRAYED_BUTTON;
                font_info.color = MMI_DARK_BLUE_COLOR;

                CTRLBUTTON_SetBg(MMIENG_MEM_BUTTON_REPORT_ID,&bg_btn);
                GUIBUTTON_SetFont(MMIENG_MEM_BUTTON_REPORT_ID,&font_info);

                CTRLBUTTON_SetBg(MMIENG_MEM_BUTTON_STOP_ID,&bg_btn);
                GUIBUTTON_SetFont(MMIENG_MEM_BUTTON_STOP_ID,&font_info);
            }
            else if(MMIENG_MEM_BUTTON_STOP_ID == MMK_GetActiveCtrlId(win_id))
            {
                CTRLBUTTON_SetBg(MMIENG_MEM_BUTTON_STOP_ID,&bg_btn);
                GUIBUTTON_SetFont(MMIENG_MEM_BUTTON_STOP_ID,&font_info);

                bg_btn.img_id = IMAGE_PDASOFTKEY_GRAYED_BUTTON;
                font_info.color = MMI_DARK_BLUE_COLOR;

                CTRLBUTTON_SetBg(MMIENG_MEM_BUTTON_REPORT_ID,&bg_btn);
                GUIBUTTON_SetFont(MMIENG_MEM_BUTTON_REPORT_ID,&font_info);

                CTRLBUTTON_SetBg(MMIENG_MEM_BUTTON_START_ID,&bg_btn);
                GUIBUTTON_SetFont(MMIENG_MEM_BUTTON_START_ID,&font_info);
            }

            MMK_PostMsg(MMIENG_MEM_BUTTON_START_ID, MSG_CTL_PAINT, NULL, NULL);
            MMK_PostMsg(MMIENG_MEM_BUTTON_REPORT_ID, MSG_CTL_PAINT, NULL, NULL);
            MMK_PostMsg(MMIENG_MEM_BUTTON_STOP_ID, MSG_CTL_PAINT, NULL, NULL);
        }
        break;
        case MSG_ENG_MEMTESTER_CONTINUE_PLAY:
        {
            EngMemTester_StopPlayVideo();//stop play first
            EngMemTester_StartPlayVideo();
        }
        break;
        case MSG_TIMER:
            if (*(uint8*)param == s_memtester_timer_id)
            {
                SCI_TraceLow("EngMemTester START mem test");
                EngMemTester_StartMemTest();
                s_memtester_scan_times ++;
                SCI_TraceLow("EngMemTester s_memtester_scan_times = %d",s_memtester_scan_times);
                //start next mem test timer
                if (s_memtester_timer_id != 0)
                {
                    MMK_StopTimer(s_memtester_timer_id);
                    s_memtester_timer_id = 0;
                }
                s_memtester_timer_id = MMK_CreateWinTimer(MMIENG_MEM_TESTER_WIN_ID, ENG_MEMTESTER_TIME_OUT, FALSE);
            }
            if (*(uint8*)param == s_memtester_duration_timer_id)
            {
                SCI_TraceLow("EngMemTester duration time out,stop memtest");
                EngMemTester_StopButtonCallback();
            }
            break;
        case MSG_CLOSE_WINDOW:
            EngMemTester_StopButtonCallback();
            s_memtester_duration = 0;
            s_memtester_memsize = 0;
            s_memtester_scan_times = 0;
            s_memtester_pass_times = 0;
            s_memtester_fail_times = 0;
            break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return (result);
}

// window table for memtester report window
WINDOW_TABLE(MMIENG_MEM_TESTER_REPORT_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)EngMemTester_ReportWinHandleMsg),
    WIN_ID(MMIENG_MEM_TESTER_REPORT_WIN_ID),
    WIN_TITLE(TXT_ENG_MEM_TESTER_REPORT),
    CREATE_TEXT_CTRL(MMIENG_MEM_TESTER_REPROT_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

// window table for memtester main window
WINDOW_TABLE(MMIENG_MEM_TESTER_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_FUNC((uint32)EngMemTester_MainWinHandleMsg),
    WIN_ID(MMIENG_MEM_TESTER_WIN_ID),
    WIN_TITLE(TXT_ENG_MEM_TESTER),

    CREATE_FORM_CTRL( GUIFORM_LAYOUT_ORDER, MMIENG_FORM_CTRL_ID ),
    //Video
    CHILD_FORM_CTRL( TRUE, GUIFORM_LAYOUT_SBS, MMIENG_MEM_FORM_VIDEO_ID, MMIENG_FORM_CTRL_ID ),
        CHILD_LABEL_CTRL( GUILABEL_ALIGN_LEFT, FALSE, MMIENG_COMMON_LABEL_CTRL1_ID, MMIENG_MEM_FORM_VIDEO_ID ),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIENG_MEM_DROPLIST_VIDEO_ID, MMIENG_MEM_FORM_VIDEO_ID ),
    //Play Times
    CHILD_FORM_CTRL( TRUE, GUIFORM_LAYOUT_SBS, MMIENG_MEM_FORM_PLAY_TIMES_ID,MMIENG_FORM_CTRL_ID ),
        CHILD_LABEL_CTRL( GUILABEL_ALIGN_LEFT, FALSE, MMIENG_COMMON_LABEL_CTRL2_ID, MMIENG_MEM_FORM_PLAY_TIMES_ID ),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIENG_MEM_DROPLIST_PLAY_TIMES_ID, MMIENG_MEM_FORM_PLAY_TIMES_ID ),
    //Memory Size(M)
    CHILD_FORM_CTRL( TRUE, GUIFORM_LAYOUT_SBS, MMIENG_MEM_FORM_MEMSIZE_ID,MMIENG_FORM_CTRL_ID ),
        CHILD_LABEL_CTRL( GUILABEL_ALIGN_LEFT, FALSE, MMIENG_COMMON_LABEL_CTRL3_ID, MMIENG_MEM_FORM_MEMSIZE_ID ),
        CHILD_DROPDOWNLIST_CTRL(TRUE, MMIENG_MEM_DROPLIST_MEMSIZE_ID, MMIENG_MEM_FORM_MEMSIZE_ID ),
    //BUTTON START REPORT STOP
    CHILD_FORM_CTRL( TRUE, GUIFORM_LAYOUT_SBS, MMIENG_MEM_FORM_OPERATE_ID, MMIENG_FORM_CTRL_ID ),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_PDASOFTKEY_GRAYED_BUTTON,MMIENG_MEM_BUTTON_START_ID,MMIENG_MEM_FORM_OPERATE_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_PDASOFTKEY_GRAYED_BUTTON,MMIENG_MEM_BUTTON_REPORT_ID,MMIENG_MEM_FORM_OPERATE_ID),
        CHILD_BUTTON_CTRL(TRUE,IMAGE_PDASOFTKEY_GRAYED_BUTTON,MMIENG_MEM_BUTTON_STOP_ID,MMIENG_MEM_FORM_OPERATE_ID),

    WIN_SOFTKEY(TXT_ENG_MEMTEST_START, TXT_NULL, TXT_ENG_MEMTEST_STOP),
    END_WIN
};

/*****************************************************************************/
//  Description : open memtester report window
//  Parameter: [In] None
//             [Out] int32
//             [Return] None
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
LOCAL void EngMemTester_OpenReportWin(void)
{
    MMK_CreateWin((uint32 * )MMIENG_MEM_TESTER_REPORT_TAB, PNULL);
}

/*****************************************************************************/
//  Description : open memtester main window
//  Parameter: [In] None
//             [Out] int32
//             [Return] None
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIENG_OpenMemTeserWin(void)
{
    MMK_CreateWin((uint32 * )MMIENG_MEM_TESTER_TAB, PNULL);
    return 0;
}


