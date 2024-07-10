/******************************************************************************
 ** File Name:      kpd_phy.c                                                 *
 ** Author:         zhigang.peng                                             *
 ** DATE:           07/10/2018                                             *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/10/2018     zhigang.peng    Create.                                   *
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
#include "adi_hal_internal.h"
#include "kpd_phy_8910.h"
#include "kpd_drvapi.h"

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

#define KEYPAD_DEBOUNCE_TIME  14  // 20ms
#define KEYPAD_INTERVER_TIME 10  // 17

typedef struct
{
    BOOLEAN OnUpEvt;
    BOOLEAN OnPressEvt;
    BOOLEAN OnDownEvt;	
} KPD_STATUS_EVENT_T;

typedef uint32 HAL_KEY_MATRIX_T[2];

LOCAL KPD_STATUS_EVENT_T  keypad_status;
LOCAL HAL_KEY_MATRIX_T    g_previousKeys  ;
LOCAL BOOLEAN                g_previousOnOff ;

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
// KPD device handle
LOCAL DEVICE_HANDLE s_dev_kpd = SCI_NULL;
LOCAL ISR_EXE_T KPD_ISR_Handler (uint32 int_num);
LOCAL void HandleKey (uint8 button, HAL_KEY_STATE_T  int_bitnum);
LOCAL uint16 GetVirtualKey (uint16 key);
LOCAL void KPD_GetMatrixMask (uint32 *col_map,uint32 *row_map);


/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
LOCAL uint32 Key_TickDelay(uint32 ticks)
{
	//uint32 i=ticks << 4;
	//for (;i>0;i--);
	OS_TickDelay(ticks);
       return ticks;
}

// scan the keypad register to fill a BOOLean vector saying if
// each key is pressed or not
LOCAL void hal_KeySingleScan(HAL_KEY_MATRIX_T key)
{
    key[0] = ((hwp_keypad->KP_DATA_L) & (KEYPAD_KP_DATA_L_MASK)) >> (KEYPAD_KP_DATA_L_SHIFT);
    key[1] = ((hwp_keypad->KP_DATA_H) & (KEYPAD_KP_DATA_H_MASK)) >> (KEYPAD_KP_DATA_H_SHIFT);
}



// ============================================================================
// hal_KeyResetIrq
// ----------------------------------------------------------------------------
/// This function clears the keypad interrupt status. It should only be called
/// to avoid unwanted Irq. In normal usage, Irq clearing is done automatically
/// by the interrupt driver.
// ============================================================================
PUBLIC void hal_KeyResetIrq(void)
{
    hwp_keypad->KP_IRQ_CLR = KEYPAD_KP_IRQ_CLR;
}

// ============================================================================
// hal_KeyIrqSetMask
// ----------------------------------------------------------------------------
/// This function controls under which conditions the keypad interrupt will
/// be generated. It can be when a key is pressed, when it is held down and
/// when it is released. Use the function #hal_KeyIrqSetHandler() to configure
/// which user function will be called in case of a keypad interruption.
///
/// @param mask Defines which event will generate a call to the user
/// callback function. See the documentation of the type for details.
// ============================================================================
PUBLIC void hal_KeyIrqSetMask(void)
{
    // Keep track of the event generation configuration.
    keypad_status.OnUpEvt      = TRUE;
    keypad_status.OnPressEvt = FALSE;
    keypad_status.OnDownEvt    = TRUE;

    if (FALSE == keypad_status.OnUpEvt &&
            FALSE == keypad_status.OnPressEvt &&
            FALSE == keypad_status.OnDownEvt)
    {
        // Disable all the interruptions for the keypad.
        KPD_Close();
    }
    else
    {
        // Enables the IRQ for the keypad.
        hwp_keypad->KP_IRQ_MASK = KEYPAD_KP_EVT0_IRQ_MASK  // key up or down
                                  | KEYPAD_KP_EVT1_IRQ_MASK   // all key released
                                  | (keypad_status.OnPressEvt?KEYPAD_KP_ITV_IRQ_MASK:0);
        // Interrupt on key kept pressed state...
        // We might avoid useless IRQ that way
    }
}


// Call the user handler with the given status for each key
// whose bit is set to '1' in the keymatrix
LOCAL void hal_KeyCallHandler(HAL_KEY_MATRIX_T keys, HAL_KEY_STATE_T state)
{
    uint32 i;

    // low keys
        for (i=0; i<LOW_KEY_NB ; i++)
        {
            if ((keys[0] & (1<<i)) != 0)
            {
                // ith key held pressed
                HandleKey(i, state);
            }
        }

        // high keys
        for (i=0; i<HIGH_KEY_NB ; i++)
        {
            if ((keys[1] & (1<<i)) != 0)
            {
                // ith key held pressed
                HandleKey((i+LOW_KEY_NB), state);
            }
        }   
}


/*****************************************************************************/
//  Description:    This function initialize keypad.
//                  1. Enable keypad device.
//                  2. Enable keypad interrupt.
//  Author:         zhigang.peng
//    Note:
/*****************************************************************************/
PUBLIC uint32 KPD_Init (void)
{
    uint32  status    = 0;
    uint32 col_bit_map,row_bit_map;
    uint32	debounceTime=KEYPAD_DEBOUNCE_TIME;
    uint32	kpItvTime=KEYPAD_INTERVER_TIME;
	
    // Disable Keypad interrupt.
    CHIPDRV_DisableIRQINT (TB_KPD_INT);
    mon_Event(0x90000006);
    SCI_TraceLow("KPD_Init line=%d ",__LINE__);

   // SCI_ASSERT (DEVICE_Find ("KEYPAD", &s_dev_kpd) == CLOCK_SUCCESS); /*assert verified*/    
   // DEVICE_Open(s_dev_kpd); 
    /* Soft Reset */
  //  DEVICE_Reset(s_dev_kpd); 
  
    keypad_status.OnDownEvt=FALSE;
    keypad_status.OnPressEvt=FALSE;
    keypad_status.OnUpEvt=FALSE;

   // Initialize the previous state.
    g_previousKeys[0] = 0;
    g_previousKeys[1] = 0;
    g_previousOnOff   = FALSE;  //hal_KeyOnOffStateGet();

    KPD_GetMatrixMask (&col_bit_map,&row_bit_map);
    hwp_keypad->KP_CTRL = 0;
    Key_TickDelay(2);
    status = ISR_RegHandler(TB_KPD_INT, KPD_ISR_Handler);
    if (TB_SUCCESS == status)
    {
        // Enable Keypad interrupt.
        CHIPDRV_EnableIRQINT (TB_KPD_INT);
    }

    hwp_keypad->KP_CTRL = KEYPAD_KP_DBN_TIME(debounceTime)
                          | KEYPAD_KP_ITV_TIME(kpItvTime)
                          | KEYPAD_KP_EN | KEYPAD_KP_IN_MASK(row_bit_map)
                          | KEYPAD_KP_OUT_MASK(col_bit_map);

    // clear current IRQ
    hwp_keypad->KP_IRQ_CLR = KEYPAD_KP_IRQ_CLR;

    hal_KeyIrqSetMask();

    return status;
}


/*****************************************************************************/
//  Description:    This function close keypad.
//                  1. Disable keypad device.
//                  2. Disable keypad interrupt.
//  Author:         zhigang.peng
//    Note:
/*****************************************************************************/
PUBLIC void KPD_Close (void)
{
    // First step : Disable Keypad interrupt.
    CHIPDRV_DisableIRQINT (TB_KPD_INT);

    // Disable Keypad device.
    hwp_keypad->KP_CTRL = 0;
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
   // interrupt status
    uint32 status;
    // Keys pressed a the time of the interrupt
    HAL_KEY_MATRIX_T scannedKeys;

    // Variables to store the state of the different keys
    HAL_KEY_MATRIX_T downKeys;
    HAL_KEY_MATRIX_T upKeys;
    HAL_KEY_MATRIX_T pressedKeys;

    status = hwp_keypad->KP_IRQ_CAUSE &
             (KEYPAD_KP_EVT0_IRQ_CAUSE|KEYPAD_KP_EVT1_IRQ_CAUSE|KEYPAD_KP_ITV_IRQ_CAUSE) ;

    // Clear IRQ
    hwp_keypad->KP_IRQ_CLR = KEYPAD_KP_IRQ_CLR;


    // Key IRQ received, return *status* to the user handler
    // (That is down, up, or held pressed)

    // All keys are released
    if ((status & KEYPAD_KP_EVT1_IRQ_CAUSE) && keypad_status.OnUpEvt)
    {
        pressedKeys[0] = 0;
        pressedKeys[1] = 0;
        // All previously pressed keys are released
        hal_KeyCallHandler(g_previousKeys, HAL_KEY_UP);

        // Keep track of the pressed buttons.
        g_previousKeys[0] = pressedKeys[0];
        g_previousKeys[1] = pressedKeys[1];
    }

    else

    {
        // Key up or down
        if (status &  KEYPAD_KP_EVT0_IRQ_CAUSE)
        {

            hal_KeySingleScan(scannedKeys);
            // Send the ***pressed events*** only if user requested it.
            if (keypad_status.OnPressEvt)
            {
                // Pressed key detection
                // Those are the one previously pressed and still pressed now
                pressedKeys[0] = (scannedKeys[0] & g_previousKeys[0]);
                pressedKeys[1] = (scannedKeys[1] & g_previousKeys[1]);

                hal_KeyCallHandler(pressedKeys, HAL_KEY_PRESSED);
            }

            // If the pressed buttons changed.
            if (scannedKeys[0] != g_previousKeys[0] ||
                    scannedKeys[1] != g_previousKeys[1] )
            {
                // Send the ***down events*** only if user requested it.
                if (keypad_status.OnDownEvt)
                {
                    // Key that are now pressed but weren't before
                    downKeys[0] = scannedKeys[0] & ~g_previousKeys[0];
                    downKeys[1] = scannedKeys[1] & ~g_previousKeys[1];

                    // low and high keys
                    hal_KeyCallHandler(downKeys, HAL_KEY_DOWN);

                }

                // Send the ***up events*** only if user requested it.
                if (keypad_status.OnUpEvt)
                {
                    // Keys that are now unpressed but were pressed before
                    upKeys[0] = ~scannedKeys[0] & g_previousKeys[0];
                    upKeys[1] = ~scannedKeys[1] & g_previousKeys[1];

                    // low and high keys
                    hal_KeyCallHandler(upKeys, HAL_KEY_UP);

                }

                // Keep track of the pressed buttons.
                g_previousKeys[0] = scannedKeys[0];
                g_previousKeys[1] = scannedKeys[1];
            }
        }
        else
        {
            // There are only already pressed keys
            // still pressed, so no change in pressed keys
            // and no need for a new scan
            if ((status & KEYPAD_KP_ITV_IRQ_CAUSE) && keypad_status.OnPressEvt)
            {
                hal_KeyCallHandler(g_previousKeys, HAL_KEY_PRESSED);
            }
        }
    }

    return 0;
}


/*****************************************************************************/
//  Description:    This function handle one key.
//  Author:         zhigang.peng
//    Note:
/*****************************************************************************/
LOCAL void HandleKey (uint8 button, HAL_KEY_STATE_T  int_bitnum)
{
    TB_MSG  kpd_msg;

    kpd_msg.wparam  = GetVirtualKey (button);
    kpd_msg.lparam = TB_NULL;

    switch (int_bitnum)
    {
        case HAL_KEY_DOWN:
            kpd_msg.message = TB_KPD_PRESSED;
            ISR_Callback (TB_KPD_INT, &kpd_msg);
            break;

        case HAL_KEY_UP:
            kpd_msg.message = TB_KPD_RELEASED;
            ISR_Callback (TB_KPD_INT, &kpd_msg);
            break;

        case HAL_KEY_PRESSED:
            // ToDo , How to deal with key_pressed  ????
            break;

        default:
            break;
    }
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

    if(key==255)
    {
	return  SCI_VK_POWER;
    }
    else
    {
        //  Notice 8910 ROW and COL are reverse to 6531 platform
        kaymap_ptr = KPD_GetKeyMap (&keymap_size);
        SCI_ASSERT (key < keymap_size);
        return (kaymap_ptr[key]);
    }
}

/*****************************************************************************/
//  Description:    check pbutton's state
//  Global resource dependence:
//  Author:         Jimmy.Jia
//  Note:           return SCI_TRUE if pressed, else return SCI_FALSE
/*****************************************************************************/
PUBLIC uint32 CheckPowerButtonState (void)
{
    uint32 end = SCI_GetTickCount() + 5;      //must after 4ms,then we can get the real EICDATA,this set to 3ms because the value of SCI_GetTickCount() has 1ms error
    uint32 ret=0;

    EIC_HAL_GetValue(EICA_PBINT);
    while (SCI_GetTickCount() < end) {};

#if 0   
    ret=  (EIC_HAL_GetValue(EICA_PBINT))?1:0;
#else    /*2720m used by 2720, the data is high when power_key up,low when power_key down*/
    ret=  (EIC_HAL_GetValue(EICA_PBINT))?0:1;
#endif
    SCI_TraceLow("KPD CheckPowerButtonState=%d ",ret);	
    return ret;
}

/*****************************************************************************/
//  Description:    check if pbutton is released
//  Global resource dependence:
//  Author:         Jimmy.Jia
//  Note:           return SCI_TRUE if released, else return SCI_FALSE
/****************************************************************************/
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
//  Description:    check pbutton's state
//  Global resource dependence:
//  Author:         Jimmy.Jia
//  Note:           return SCI_TRUE if pressed, else return SCI_FALSE
/*****************************************************************************/
PUBLIC uint32 CheckPowerButton2State (void)
{
    uint32 end = SCI_GetTickCount() + 2;
    uint32 ret=0;

    EIC_HAL_GetValue(EIC_PBINT2);
    while (SCI_GetTickCount() < end) {};

#if 0   
    ret=  (EIC_HAL_GetValue(EICA_PBINT))?1:0;
#else    /*2720m used by 2720, the data is high when power_key up,low when power_key down*/
    ret=  (EIC_HAL_GetValue(EIC_PBINT2))?0:1;
#endif
    SCI_TraceLow("KPD CheckPowerButtonState=%d ",ret);	
    return ret;
}

/*****************************************************************************/
//  Description:    check if pbutton is released
//  Global resource dependence:
//  Author:         Jimmy.Jia
//  Note:           return SCI_TRUE if released, else return SCI_FALSE
/****************************************************************************/
PUBLIC uint32  IsPowerButton2Released (void)
{
    if (SCI_FALSE == CheckPowerButton2State ())
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

        if (SCI_FALSE == CheckPowerButton2State ())
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
    *col_map = 0x1F;
    *row_map = 0x1F;	// 0xFF
    return;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}

#endif  // End of kpd_phy_v0.c
