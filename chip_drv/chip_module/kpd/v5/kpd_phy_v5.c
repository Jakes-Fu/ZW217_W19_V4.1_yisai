/******************************************************************************
 ** File Name:      kpd_phy_v5.c                                                 *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           09/10/2002                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/10/2002     Xueliang.Wang    Create.                                   *
 ** 11/07/2003     Xueliang.Wang    Add some trace to debug.                  *
 ** 19/11/2004     Benjamin.Wang   Replace GetVirtualKey function with new hashtable function*
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "dal_keypad.h"
#include "chip_plf_export.h"
#include "clock_drvapi.h"
#include "kpd_reg_v5.h"
#include "kpd_drvapi.h"
#include "../kpd_phy.h"
#include "eic_hal.h"
/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif


LOCAL KpdReadMode keypad_read_mode = KPD_NORMAL_INTERRUPT;//0: normal, 1: sync read

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
// KPD device handle
LOCAL DEVICE_HANDLE s_dev_kpd = SCI_NULL;

LOCAL ISR_EXE_T KPD_ISR_Handler (uint32 int_num);
//LOCAL void KeypadHisr (uint32, void *);
LOCAL void  KeypadHisrFunc (uint32 i, void *d);
LOCAL void ChangeState (KPD_KEY_T *key_ptr);
LOCAL void HandleKey (KPD_INT_TYPE_E  int_bitnum);
LOCAL uint16 GetVirtualKey (uint16 key);
LOCAL uint16 GetKeycode (KPD_NUM_E key_num);
LOCAL BOOLEAN CheckFourKeyCross (void);
LOCAL void KPD_GetMatrixMask (uint32 *col_map,uint32 *row_map);


INPUT_BUFFER_INIT (int32,64)


/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function initialize keypad.
//                  1. Enable keypad device.
//                  2. Enable keypad interrupt.
//  Author:         Xueliang.Wang
//    Note:
/*****************************************************************************/
PUBLIC uint32 KPD_Init (void)
{
    uint32  status    = 0;
    uint32 col_bit_map,row_bit_map;
    kpd_s   *kpd_ptr = (kpd_s *) (KPD_BASE);

#if defined( MODEM_PLATFORM) && defined(HW_ALPHA_CUSTOMER)
    return status;	
#endif

    keypad_read_mode = KPD_NORMAL_INTERRUPT;
    // Disable Keypad interrupt.
    CHIPDRV_DisableIRQINT (TB_KPD_INT);

    SCI_ASSERT (DEVICE_Find ("KEYPAD", &s_dev_kpd) == CLOCK_SUCCESS); /*assert verified*/    
    DEVICE_Open(s_dev_kpd); 
    /* Soft Reset */
    DEVICE_Reset(s_dev_kpd); 

    // interrupt clear register changed
    kpd_ptr->int_clr = KPD_INT_ALL;
    // Set speed of scaning keypad.
    kpd_ptr->clk_divide_cnt = CFG_CLK_DIV & KPDCLK0_CLK_DIV0 ;

    KPD_GetMatrixMask (&col_bit_map,&row_bit_map);

#ifdef PLATFORM_SC6530
    col_bit_map >>= KPD_FIRST_COL_EN;
    row_bit_map >>= KPD_FIRST_ROW_EN;
#endif

    col_bit_map <<= KPD_COL_EN_SHIFT;
    row_bit_map <<= KPD_ROW_EN_SHIFT;

    // keypad enable
    kpd_ptr->ctl &= ~BIT_1;//disable keypad sleep mode

#ifdef FPGA_VERIFICATION
	kpd_ptr->ctl |= ((col_bit_map & KPD_COL_EN_MSK) |
                     (row_bit_map & KPD_ROW_EN_MSK));
#else
    kpd_ptr->ctl |= (0x4 | (col_bit_map & KPD_COL_EN_MSK) |
                     (row_bit_map & KPD_ROW_EN_MSK));
#endif
    kpd_ptr->debounce_cnt = 0x10;  //Default=0x80

    // keypad 1 press INT enable
    kpd_ptr->int_en = KPD_INT_ALL;

    // Config Keypad.
    // Set polarity.
    kpd_ptr->polarity = CFG_ROW_POLARITY | CFG_COL_POLARITY;

    status = ISR_RegHandler_Ex (TB_KPD_INT, KPD_ISR_Handler, KeypadHisrFunc, CHIPDRV_HISR_PRIO_2, NULL);

    if (TB_SUCCESS == status)
    {
        // Enable Keypad interrupt.
        CHIPDRV_EnableIRQINT (TB_KPD_INT);
    }

    kpd_ptr->ctl |= 0x01;  //enable kpd module

    return status;
}

/*****************************************************************************/
//  Description:    This function loopthrough, if these is interruption, put it in the buffer, and then
//                       determine whether the key is ture, if it is true, sent to the callback function for
//                       further processing.
//  Author:          steve.zhan
//  Note:           Get some keystatus using polling method
/*****************************************************************************/
PUBLIC uint32 KPD_SyncGetKey (void)
{
#define READ_REG_DELAY (0xfffff)
    int32 pollingCnt = 1;//Only polling once

    KPD_INT_TYPE_E  int_bitnum = KPD_PRESS_INT_RESERVED;

    while (threadReadBuffer ( (int32 *) (&int_bitnum)))  //if the buffer is empty
    {
        KPD_ISR_Handler (TB_KPD_INT); //read data from keypad controler

        pollingCnt--;

        if (pollingCnt <= 0)
        {
            break;
        }
    }

    if (KPD_PRESS_INT_RESERVED != int_bitnum)
    {
        HandleKey (int_bitnum);
    }
    else
    {
        if (!threadReadBuffer ( (int32 *) (&int_bitnum)))
        {
            HandleKey (int_bitnum);
        }
        else
        {
            {
                //If can't get the value, will delay and then polling the value.
                int32 delay = READ_REG_DELAY;

                while (delay -- > 1) {}
            }

        }
    }

    return 0;
}

/*****************************************************************************/
//  Description:    This function set the current read mode:Sync mode, Or async mode.
//  Author:           steve.zhan
//  Note:              Default value is normal mode(using interrupt is async mode)
/*****************************************************************************/
PUBLIC uint32 KPD_SetKpdReadMode (KpdReadMode mode)
{
    keypad_read_mode = mode;

    if (keypad_read_mode == KPD_SYNC_READ)
    {
        CHIPDRV_DisableIRQINT (TB_KPD_INT);//Notice.
    }
    else
    {
        CHIPDRV_EnableIRQINT (TB_KPD_INT); //Notice.
    }

    return 0;
}

/*****************************************************************************/
//  Description:    This function return  the current read mode:Sync mode, Or async mode.
//  Author:           steve.zhan
//  Note:
/*****************************************************************************/
PUBLIC KpdReadMode KPD_GetKpdReadMode()
{
    return keypad_read_mode;
}

/*****************************************************************************/
//  Description:    This function close keypad.
//                  1. Disable keypad device.
//                  2. Disable keypad interrupt.
//  Author:         Xueliang.Wang
//    Note:
/*****************************************************************************/
PUBLIC void KPD_Close (void)
{
    kpd_s   *kpd_ptr = (kpd_s *) (KPD_BASE);

    // First step : Disable Keypad interrupt.
    CHIPDRV_DisableIRQINT (TB_KPD_INT);

    // Disable Keypad device.
    kpd_ptr->ctl &= ~ (BIT_0);
}


/*****************************************************************************/
//  Description:    This function is keypad 's HISR.
//                  1. THE priority is higher than normal task.
//                  2. Is not real ISR.
//  Author:         steve.zhan
//  Note:
/*****************************************************************************/
LOCAL void  KeypadHisrFunc (uint32 i, void *d)
{
    KPD_INT_TYPE_E  int_bitnum = 0;

    while (!threadReadBuffer ( (int32 *) (&int_bitnum)))
    {
        HandleKey (int_bitnum);
    }
}

/*****************************************************************************/
//  Description:    This function is keypad 's ISR handle.
//  This  handle will read hardware register status and active the Hisr.
//
//  Author:  steve.zhan
//  Note:
/*****************************************************************************/
LOCAL ISR_EXE_T KPD_ISR_Handler (uint32 int_num)
{
    kpd_s    *kpd_ptr = (kpd_s *) (KPD_BASE);
    uint32     s_reg_int_status;

    s_reg_int_status = kpd_ptr->int_raw_status;
    kpd_ptr->int_clr |= KPD_INT_ALL;

    //press int
    if (s_reg_int_status & KPD_PRESS_INT0)
    {
        if (!CheckFourKeyCross())
        {
            IsrWriteBuffer (KPD_PRESS1_INT);
        }
    }

    if (s_reg_int_status & KPD_PRESS_INT1)
    {
        if (!CheckFourKeyCross())
        {
            IsrWriteBuffer (KPD_PRESS2_INT);
        }
    }

    if (s_reg_int_status & KPD_PRESS_INT2)
    {
        if (!CheckFourKeyCross())
        {
            IsrWriteBuffer (KPD_PRESS3_INT);
        }
    }

#if (MAX_MUL_KEY_NUM == 4)

    if (s_reg_int_status & KPD_PRESS_INT3)
    {
        if (!CheckFourKeyCross())
        {
            IsrWriteBuffer (KPD_PRESS4_INT);
        }
    }

#endif

    //release int:
    if (s_reg_int_status & KPD_RELEASE_INT0)
    {
        IsrWriteBuffer (KPD_RELEASE1_INT);
    }

    if (s_reg_int_status & KPD_RELEASE_INT1)
    {
        IsrWriteBuffer (KPD_RELEASE2_INT);

    }

    if (s_reg_int_status & KPD_RELEASE_INT2)
    {
        IsrWriteBuffer (KPD_RELEASE3_INT);
    }

#if (MAX_MUL_KEY_NUM == 4)

    if (s_reg_int_status & KPD_RELEASE_INT3)
    {
        IsrWriteBuffer (KPD_RELEASE4_INT);
    }

#endif

    return CALL_HISR;
}

/*****************************************************************************/
//  Description: This function check whether four key cross is happened
//  Author:         yong.zou
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckFourKeyCross (void)
{
    uint32 s_reg_status;
    kpd_s *kpd_ptr = (kpd_s *) (KPD_BASE);

    s_reg_status = kpd_ptr->key_status;

    if (s_reg_status & BIT_3)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description:    This function handle one key.
//  Author:         Xueliang.Wang
//    Note:
/*****************************************************************************/
LOCAL void HandleKey (KPD_INT_TYPE_E  int_bitnum)
{
    KPD_KEY_T   *key_ptr = NULL;
    kpd_s          *kpd_ptr = (kpd_s *) (KPD_BASE);
    KPD_KEY_T KPD_KEY;
    key_ptr = &KPD_KEY;

    switch (int_bitnum)
    {
        case KPD_PRESS1_INT:
            //get key code from KPD_KEY_STATUS  register
            key_ptr->key_code = GetKeycode (KPD_NUM_1);
            key_ptr->state = TB_KPD_PRESSED;
            ChangeState (key_ptr);
            break;
        case KPD_PRESS2_INT:
            key_ptr->key_code = GetKeycode (KPD_NUM_2);
            key_ptr->state = TB_KPD_PRESSED;
            ChangeState (key_ptr);
            break;
        case KPD_PRESS3_INT:
            key_ptr->key_code = GetKeycode (KPD_NUM_3);
            key_ptr->state = TB_KPD_PRESSED;
            ChangeState (key_ptr);
            break;
        case KPD_PRESS4_INT:
            key_ptr->key_code = GetKeycode (KPD_NUM_4);
            key_ptr->state = TB_KPD_PRESSED;
            ChangeState (key_ptr);
            break;

        case KPD_RELEASE1_INT:
            key_ptr->key_code = GetKeycode (KPD_NUM_1);
            key_ptr->state = TB_KPD_RELEASED;
            ChangeState (key_ptr);
            break;
        case KPD_RELEASE2_INT:
            key_ptr->key_code = GetKeycode (KPD_NUM_2);
            key_ptr->state = TB_KPD_RELEASED;
            ChangeState (key_ptr);
            break;

        case KPD_RELEASE3_INT:
            key_ptr->key_code = GetKeycode (KPD_NUM_3);
            key_ptr->state = TB_KPD_RELEASED;
            ChangeState (key_ptr);
            break;
        case KPD_RELEASE4_INT:
            key_ptr->key_code = GetKeycode (KPD_NUM_4);
            key_ptr->state = TB_KPD_RELEASED;
            ChangeState (key_ptr);
            break;
        case KPD_LONGPRESS1_INT:
            kpd_ptr->int_clr |= BIT_8;
            break;
        case KPD_LONGPRESS2_INT:
            kpd_ptr->int_clr |= BIT_9;
            break;
        case KPD_LONGPRESS3_INT:
            kpd_ptr->int_clr |= BIT_10;
            break;
        case KPD_LONGPRESS4_INT:
            kpd_ptr->int_clr |= BIT_11;
            break;
        default:
            break;
    }
}

/*****************************************************************************/
//  Description:    This function change the state of the key, and call
//                  the related function.
//  Author:         Xueliang.Wang
//    Note:
/*****************************************************************************/

LOCAL void ChangeState (KPD_KEY_T *key_ptr)
{
    TB_MSG  kpd_msg;

    // Change state of the key according to it's current state.
    if (TB_KPD_RELEASED == key_ptr->state)
    {
        //set key message ,include key state and key's virtualkey
        kpd_msg.message = TB_KPD_RELEASED;
        kpd_msg.wparam  = GetVirtualKey (key_ptr->key_code);
        //KPDDRV_PRINT:"KPD_DRIVER: Key code %d is UP!\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KPD_PHY_V5_420_112_2_17_23_5_4_1038,(uint8*)"d", key_ptr->key_code);
    }
    else if (TB_KPD_PRESSED == key_ptr->state)
    {
        //set key message ,include key state and key's virtualkey
        kpd_msg.message = TB_KPD_PRESSED;
        kpd_msg.wparam  = GetVirtualKey (key_ptr->key_code);

    }

    kpd_msg.lparam = TB_NULL;
    // Call related function to handle this message.
    ISR_Callback (TB_KPD_INT, &kpd_msg);
}

/*****************************************************************************/
//  Description:    This function change the state of the key, and call
//                  the related function.
//  Author:         Benjamin.Wang
//    Note:
/*****************************************************************************/
LOCAL uint16 GetVirtualKey (uint16 key)
{
    uint32 keymap_size;
    uint16 *kaymap_ptr;

    kaymap_ptr = KPD_GetKeyMap (&keymap_size);

    SCI_ASSERT (key < keymap_size);/*assert verified*/

    return (kaymap_ptr[key]);
}

/*****************************************************************************/
//  Description: This function get key_code;
//  Author:         yong.zou
//    Note:
/*****************************************************************************/
LOCAL uint16 GetKeycode (KPD_NUM_E key_num)
{
    kpd_s      *kpd_ptr = (kpd_s *) (KPD_BASE);
    uint16      key_code = MAX_SCAN_KEY_CODE;

    SCI_ASSERT (key_num < KPD_NUM_MAX);/*assert verified*/

    /*lint -e{656} confirmed by xuepeng*/
    switch (key_num)
    {
        case KPD_NUM_1:
            key_code = ( ((kpd_ptr->key_status & KPD1_COL_CNT) >> ((key_num-KPD_NUM_1) << 3)) * KPD_ROW_NUM) +
                       ( (kpd_ptr->key_status & KPD1_ROW_CNT) >> (((key_num-KPD_NUM_1) << 3) + 4));
            break;
        case KPD_NUM_2:
            key_code = ( ((kpd_ptr->key_status & KPD2_COL_CNT) >> ((key_num-KPD_NUM_1) << 3)) * KPD_ROW_NUM) +
                       ( (kpd_ptr->key_status & KPD2_ROW_CNT) >> (((key_num-KPD_NUM_1) << 3) + 4));
            break;
        case KPD_NUM_3:
            key_code = ( ((kpd_ptr->key_status & KPD3_COL_CNT) >> ((key_num-KPD_NUM_1) << 3)) * KPD_ROW_NUM) +
                       ( (kpd_ptr->key_status & KPD3_ROW_CNT) >> (((key_num-KPD_NUM_1) << 3) + 4));
            break;
        case KPD_NUM_4:
            key_code = ( ((kpd_ptr->key_status & KPD4_COL_CNT) >> ((key_num-KPD_NUM_1) << 3)) * KPD_ROW_NUM) +
                       ( (kpd_ptr->key_status & KPD4_ROW_CNT) >> (((key_num-KPD_NUM_1) << 3) + 4));
            break;
        default:
            break;

    }

    return key_code;
}

/*****************************************************************************/
//  Description:    check pbutton's state
//  Global resource dependence:
//  Author:         Jimmy.Jia
//  Note:           return SCI_TRUE if pressed, else return SCI_FALSE
/*****************************************************************************/
PUBLIC uint32 CheckPowerButtonState (void)
{
#if defined(FPGA_SUPPORT_SC6530) || defined(FPGA_SUPPORT_SC6531) || defined(FPGA_SUPPORT_SC6531EFM) || defined(FPGA_SUPPORT_UIX8910)
    return 0;
#else

#if defined(CHIP_VER_6531)
    uint32 end = SCI_GetTickCount() + 2;

    EIC_HAL_GetValue(19);
    while (SCI_GetTickCount() < end) {};
   
    return ((EIC_HAL_GetValue(19))?0:1);
#elif defined(CHIP_VER_6531EFM)
    uint32 end = SCI_GetTickCount() + 2;

    EIC_HAL_GetValue(EICA_PBINT);
    while (SCI_GetTickCount() < end) {};
   
    return ((EIC_HAL_GetValue(EICA_PBINT))?1:0);
#else
    return EIC_HAL_GetValue(19);
#endif

#endif
}

/*****************************************************************************/
//  Description:    check if pbutton is released
//  Global resource dependence:
//  Author:         Jimmy.Jia
//  Note:           return SCI_TRUE if released, else return SCI_FALSE
/*****************************************************************************/
PUBLIC uint32  IsPowerButtonReleased (void)
{
    if (SCI_FALSE == CheckPowerButtonState ())
    {
        if (SCI_InThreadContext())
        {
            SCI_Sleep (30);
        }
        else
        {
            uint32 end = SCI_GetTickCount() + 30;

            while (SCI_GetTickCount() < end) {};
        }

        if (SCI_FALSE == CheckPowerButtonState ())
        {
            return SCI_TRUE;
        }
    }

    return SCI_FALSE;
}

/*****************************************************************************/
//  Description: Get col and row enable map for initialize setting.
//  Author:         mingwei.zhang
//    Note:
/*****************************************************************************/
void KPD_GetMatrixMask (uint32 *col_map,uint32 *row_map)
{
    uint32 keymap_size;
    uint16 *kaymap_ptr;
    uint32 col_bit_map = 0,row_bit_map = 0;
    uint32 i;

    //keymap_size = sizeof(keymap)/sizeof(keymap[0]);
    kaymap_ptr = KPD_GetKeyMap (&keymap_size);
    SCI_ASSERT (keymap_size <= (KPD_COL_NUM*KPD_ROW_NUM)); /*assert verified*/

    for (i = 0; i < keymap_size; i++)
    {
        if (kaymap_ptr[i] != SCI_VK_INVALID_KEY)
        {
            col_bit_map |= (1 << (i / KPD_ROW_NUM));
            row_bit_map |= (1 << (i % KPD_ROW_NUM));
        }
    }

#ifdef DSP_IQ_DEBUG
	col_bit_map = 0x7;
#endif
    *col_map = col_bit_map;
    *row_map = row_bit_map;

    return;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}

#endif  // End of kpd_phy_v0.c
