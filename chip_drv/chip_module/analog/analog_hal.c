/******************************************************************************
 ** File Name:      analog_cfg.c                                              *
 ** Author:         Jeff.Li                                                   *
 ** DATE:           24/02/2009                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Analog interface.                                         *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 24/02/2009     Jeff.Li          Create.                                   *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "doidleonpage.h"
#include "analog_drvapi.h"
#include "analog_phy.h"
#include "chg_drvapi.h"
/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#define DEBUG_ANA
#define OPEN_TP_CODEC 1
#define CLOSE_TP_CODEC 0
#ifdef DEBUG_ANA
#define ANA_PRINT   SCI_TRACE_LOW
#else
#define ANA_PRINT( _format_string )
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         MACOR Definitions                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL ANA_DEV_T_PTR s_ana_dev_ptr = PNULL;
LOCAL ANA_VBAT_T    s_ana_vbat    = {0};
PUBLIC uint32 audio_codec_enclose = 0;
PUBLIC uint32 TP_enclose = 0;
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    To get statistic vbat
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
LOCAL uint32 _ANA_GetStatisticVbat (void)
{
    uint32 i = 0;
    uint32 statis_vbat;
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();
    
    statis_vbat = p_chgmng_info->bat_cur_vol/10;

    //SCI_DisableIRQ();

    s_ana_vbat.vbat_tab[s_ana_vbat.vbat_tab_index++] = statis_vbat;

    if (ANA_VBAT_VALUE_NUM == s_ana_vbat.vbat_tab_index)
    {
        s_ana_vbat.vbat_tab_index = 0;
    }

    // if the table is full Calculate Statistic Vbat, or only return the current vbat
    if (0 != s_ana_vbat.vbat_tab[s_ana_vbat.vbat_tab_index])
    {
        statis_vbat = 0;

        for (i = 0; i < ANA_VBAT_VALUE_NUM; i++)
        {
            statis_vbat += s_ana_vbat.vbat_tab[i];
        }

        statis_vbat /= ANA_VBAT_VALUE_NUM;
    }

    s_ana_vbat.statis_vbat = statis_vbat;

    //SCI_RestoreIRQ();

    return statis_vbat;

}

/*****************************************************************************/
//  Description:    Get device current max value according to current vbat
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
LOCAL __inline uint32 _ANA_GetDevCurrentMaxValue (
    uint32          cur_vbat,
    ANA_SW_T_PTR    sw_tab_ptr,
    uint32         *cur_index_ptr
)
{
    uint32  index        = 0;
    BOOLEAN is_FindIndex = SCI_FALSE;
    uint32 pre_index = * cur_index_ptr;


    // search the voltage region from the switch table according to the current voltage
    for (index = 0; (index < ANA_SW_INDEX_MAX) && (ANA_INVALID_VALUE != sw_tab_ptr[index].min_vbat_value); index++)
    {
        if ( (sw_tab_ptr[index].min_vbat_value <= cur_vbat) && (sw_tab_ptr[index].max_vbat_value > cur_vbat))
        {
            is_FindIndex = SCI_TRUE;
            break;
        }
    }

    if (is_FindIndex)
    {
        if (ANA_SW_INDEX_MAX > *cur_index_ptr)
        {
            if (*cur_index_ptr < index)         // from low to high
            {
                *cur_index_ptr = index;
            }
            else if ( (*cur_index_ptr > index) && (sw_tab_ptr[*cur_index_ptr].min_vbat_value > cur_vbat)) // from high to low
            {
                *cur_index_ptr = index;
            }
            else
            {
                // do nothing
            }
        }
        else
        {
            *cur_index_ptr = 0;
        }
    }
    else
    {
        *cur_index_ptr = 0;
    }


    //SCI_TRACE_LOW:"_ANA_GetDevCurrentMaxValue: old_index = %d, new_index = %d, max_level = %d, vbat = %d, time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ANALOG_HAL_155_112_2_17_23_2_32_24,(uint8*)"ddddd",pre_index,*cur_index_ptr,sw_tab_ptr[*cur_index_ptr].max_level,cur_vbat,SCI_GetTickCount());
    return sw_tab_ptr[*cur_index_ptr].max_level;

}

/*****************************************************************************/
//  Description:    Get Device Real value
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
LOCAL __inline uint32 _ANA_GetDevRealValue (ANA_DEV_ID_E dev_id, uint32 value)
{
    uint32 max_val = s_ana_dev_ptr[dev_id].max_real_value;

    if (NULL != s_ana_dev_ptr[dev_id].switch_tab_ptr)
    {
        // Get max abstract value according to the current vbat
        max_val = _ANA_GetDevCurrentMaxValue (s_ana_vbat.statis_vbat, s_ana_dev_ptr[dev_id].switch_tab_ptr, &s_ana_dev_ptr[dev_id].switch_tab_index);

        // calculate the max real value by max abstract value
        max_val = max_val * s_ana_dev_ptr[dev_id].max_real_value /  ANA_DEV_VAL_MAX;
    }

    // calculate the current real value by max real value
    return (value * max_val / ANA_DEV_VAL_MAX);
}

/*****************************************************************************/
//  Description:    Set analog value for analog device
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
LOCAL __inline void _ANA_SetDevRealValue (ANA_DEV_ID_E dev_id, uint32 real_value)
{

    // if no function, don't need to call
    if (NULL != s_ana_dev_ptr[dev_id].set_value_func)
    {
        // if the setting register value is more large than max register value, adjust the setting value
        if (real_value > s_ana_dev_ptr[dev_id].max_real_value)
        {
            real_value = s_ana_dev_ptr[dev_id].max_real_value;
        }

        // call set value function
        s_ana_dev_ptr[dev_id].set_value_func (real_value);

    }
}

/*****************************************************************************/
//  Description:    Check if device ID is valida
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
LOCAL __inline BOOLEAN _ANA_CheckDevID (ANA_DEV_ID_E dev_id)
{
    SCI_ASSERT (dev_id < ANA_DEV_ID_MAX);/*assert verified*/

    if (s_ana_dev_ptr[dev_id].id == dev_id)
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }
}

/*****************************************************************************/
//  Description:    Called by OnIdle Timer
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
LOCAL uint32  _ANA_DoIdleCallBack (uint32 param)
{
    LOCAL uint32 cnt = 0;
    ANA_DEV_ID_E id = 0;

    if (0 == (cnt&ANA_VBAT_STATIS_PERIOD))
    {
        _ANA_GetStatisticVbat();

        for (id = 0; id < ANA_DEV_ID_MAX; id++)
        {
            if (NULL != s_ana_dev_ptr[id].switch_tab_ptr)
            {
                ANA_SetDevValule (id, s_ana_dev_ptr[id].cur_value);
            }
        }
    }

    cnt++;

    return 0;
}


/*****************************************************************************/
//  Description:    Initialize analog module
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void ANA_Init (void)
{
    /*Init necessary registers*/
    ANA_PHY_Init();
    
    s_ana_dev_ptr = ANA_GetDevCfgTab();

    if (PNULL != s_ana_dev_ptr)
    {
    #if !defined(PLATFORM_UWS6121E)
        _ANA_GetStatisticVbat(); 
        DoIdle_RegisterCallback ( (DOIDLE_CALLBACK_FUNC) _ANA_DoIdleCallBack);
    #endif
    }
    else
    {
        //ANA_PRINT:"ANA_SetDevValule: can't get analog device table point !! Fail !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ANALOG_HAL_262_112_2_17_23_2_32_25,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description:    Set analog value for analog device
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC void ANA_SetDevValule (ANA_DEV_ID_E dev_id, uint32 value)
{
    uint32 real_val = 0;
    uint32 tick = SCI_GetTickCount();

    if (PNULL != s_ana_dev_ptr)
    {
        SCI_ASSERT (_ANA_CheckDevID (dev_id));/*assert verified*/
        SCI_ASSERT (value <= ANA_DEV_VAL_MAX);/*assert verified*/

        //SCI_DisableIRQ();

        if ((s_ana_dev_ptr[dev_id].cur_value != value)||(value==0))//(value==0) to solve the case that "stop" can't work immediately during vibration
        {
            real_val = _ANA_GetDevRealValue (dev_id, value);
            _ANA_SetDevRealValue (dev_id, real_val);

            s_ana_dev_ptr[dev_id].cur_value = value;
        }

        //SCI_RestoreIRQ();

        //ANA_PRINT:"ANA_SetDevValule: id = %d, value = %d, real_value = %d, time = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ANALOG_HAL_291_112_2_17_23_2_32_26,(uint8*)"dddd", dev_id, value, real_val, SCI_GetTickCount() - tick);
    }
    else
    {
        //ANA_PRINT:"ANA_SetDevValule: Not Init !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ANALOG_HAL_295_112_2_17_23_2_32_27,(uint8*)"");
    }

}

/*****************************************************************************/
//  Description:    Select Fucntion for analog device
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void ANA_SelectDevFunc (ANA_DEV_ID_E dev_id, uint32 value)
{
    ANA_PHY_SelectDevFunc(dev_id, value);
}

/*****************************************************************************/
//  Description:    Set Params for analog device
//  Author:         
//  Note:
/*****************************************************************************/
PUBLIC void ANA_SetDevParams (ANA_DEV_ID_E dev_id, ANA_DEV_PARAM_T* param)
{
    ANA_PHY_SetDevParam(dev_id, param);
}

/*****************************************************************************/
//  Description:   Set TP and Audio Codec
//  Author:    Tianyu.Yang  
//  Note: HW problem
/*****************************************************************************/
PUBLIC void ANA_DecisionCodecTpFunc (ANA_TP_AUDIO_DEV_ID dev_id, uint32 value)
{
	SCI_DisableIRQ();
	SCI_DisableFIQ();
	switch(dev_id)
	{
		case ANA_DEV_ID_TP:
		{
			if(value)
			{
				ANA_PHY_SetCodecTpFunc(OPEN_TP_CODEC);
				TP_enclose = 0;
			}
			else
			{
				TP_enclose = 1;
				if(audio_codec_enclose)
				{
					ANA_PHY_SetCodecTpFunc(CLOSE_TP_CODEC);
				}
			}
		}
		break;

		case ANA_DEV_ID_AUDIO:
		{
			if(value)
			{
				ANA_PHY_SetCodecTpFunc(OPEN_TP_CODEC);
				audio_codec_enclose = 0;
			}
			else
			{
				audio_codec_enclose = 1;
				if(TP_enclose)
				{
					ANA_PHY_SetCodecTpFunc(CLOSE_TP_CODEC);
				}
			}
		}
		break;

		default:
			SCI_PASSERT (0,("Error Set TP and Audio Codec!!!"));   /*assert verified*/
			break;
	}
	SCI_RestoreFIQ();
	SCI_RestoreIRQ();
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}

#endif  // End of analog_drv.c
