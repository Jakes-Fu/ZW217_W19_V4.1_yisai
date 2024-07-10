/******************************************************************************
 ** File Name:      mux_ctl.c                                      *
 ** Author:         yayan.xu                                             *
 ** DATE:           07/10/2008                                              *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:      The file defines MUX DTI behavior*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/10/2008     yayan.xu     Create.                                   *
 ******************************************************************************/
#include "ms_ref_mux_trc.h"
#include "mux_all.h"
#include "mux_api.h"
#include "mux_driver.h"

MUX_ENTITY_T    g_mux_data_common_recv_func[MUX_MAX_LINK_NUM]   =
{
  0
};

//#ifdef _MUX_MODEM_  /*cr250844*/
/*****************************************************************************/
// Description :    external module call this interface to query link if according to callback function
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      10th July.08
// Note :           
/*****************************************************************************/
PUBLIC MUX_RETURN_E MUX_Set_Atc_CallbackFunc(uint8 link_id, MUX_CALLBACK at_recv_func)
{
    uint8   i;

    MUX_INFO_T *mux_ptr = PNULL;



    for( i = 0; i < MAX_MUX_NUM; i++)
    {
         mux_ptr = &g_mux_sys_info[i];
         if(MUX_STATUS_ON != mux_ptr->is_start)
         {         
            g_mux_dlc_info[mux_ptr->ctl_link].common_recv_func = at_recv_func;
            g_mux_common_recv_func = at_recv_func;

         }
    }

     /*when link_id = 0xff, set a common callback for all link.*/
    if (MUX_DLC_LINK_INVALID == link_id)
    {
        g_mux_common_recv_func = at_recv_func;

        /*Check all connected link*/
        for (i = 0; i < MUX_MAX_LINK_NUM; i++)
        {
            if (g_mux_dlc_info[i].used_state != MUX_DLC_STATE_DISCONNECT)
            {
                g_mux_dlc_info[i].common_recv_func = g_mux_common_recv_func;
            }
        }
        return MUX_RET_SUCCESS;
    }

    /*check link id validity*/
    if (link_id >= MUX_MAX_LINK_NUM)
    {
        //MUX_TRACE_WARNING:"MUX_Set_Atc_CallbackFunc fail due to link id(%d) execed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_API_67_112_2_18_1_49_16_0,(uint8*)"d", link_id);
        return MUX_RET_LINK_NUM_ERR;
    }

    if (MUX_DLC_STATE_DISCONNECT == g_mux_dlc_info[link_id].used_state)
    {
        //MUX_TRACE_WARNING:"MUX_Set_Atc_CallbackFunc fail due to status error in link %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_API_73_112_2_18_1_49_16_1,(uint8*)"d", link_id);
        return MUX_RET_DLC_STATUS_ERR;
    }

    g_mux_dlc_info[link_id].common_recv_func = at_recv_func;

    return MUX_RET_SUCCESS;
}
//#endif/*#ifdef _MUX_MODEM_*/  /*cr250844*/

/*****************************************************************************/
// Description :    external module call this interface to destroy DTI when data transfer terminated
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      10th July.08
// Note :           
/*****************************************************************************/
PUBLIC MUX_RETURN_E MUX_DTI_Destroy(uint8 link_id)
{
    /*check link id validity*/
    if (link_id >= MUX_MAX_LINK_NUM)
    {
        //MUX_TRACE_WARNING:"MUX_DTI_Destroy fail due to link id(%d) execed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_API_93_112_2_18_1_49_16_2,(uint8*)"d", link_id);
        return MUX_RET_LINK_NUM_ERR;
    }

#ifdef _STATIC_LINK_SET_ /*MS00130031*/

      return MUX_RET_SUCCESS;
#endif/*#ifdef _STATIC_LINK_SET_*/

    MUX_MEMSET(&g_mux_dlc_info[link_id].entity, 0, sizeof(MUX_ENTITY_T));


    /*It means the link exist data mode once DTI Destroy*/
    g_mux_dlc_info[link_id].work_mode = MUX_DLC_AT_MODE;

#if 0 // MS00226768
    if (g_mux_dlc_info[link_id].mux_plus_timer != NULL)
    {
        /*the timer only used in data mode, shall create when Create DTI
           Delete when Destroy DTI*/
        MUX_DELETE_TIMER(g_mux_dlc_info[link_id].mux_plus_timer);
        g_mux_dlc_info[link_id].mux_plus_timer = NULL;
    }
#endif 
    return MUX_RET_SUCCESS;
}
/*****************************************************************************/
// Description :    external module call this interface to query link if according to callback function
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      10th July.08
// Note :           
/*****************************************************************************/

PUBLIC MUX_RETURN_E MUX_DTI_Query_Entity(uint8 link_id, MUX_ENTITY_T *entity)
{
    /*check link id validity*/
    if (link_id >= MUX_MAX_LINK_NUM)
    {
        //MUX_TRACE_WARNING:"MUX_DTI_Query_Entity fail due to link id(%d) execed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_API_132_112_2_18_1_49_17_3,(uint8*)"d", link_id);
        return MUX_RET_LINK_NUM_ERR;
    }

    entity = &g_mux_dlc_info[link_id].entity;


    return MUX_RET_SUCCESS;
}

/*****************************************************************************/
// Description :    external module call this interface to create DTI when enter data mode
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      10th July.08
// Note :           
/*****************************************************************************/
PUBLIC MUX_RETURN_E MUX_DTI_Create(uint8 link_id, MUX_ENTITY_T entity_info)
{

    MUX_INFO_T *mux_ptr;

    uint8 mux_index = g_mux_dlc_info[link_id].from;

    MUX_ASSERT(mux_index < MAX_MUX_NUM);/*assert verified*/

    mux_ptr = &g_mux_sys_info[mux_index];
    
    /*check link id validity*/
    if (link_id >= MUX_MAX_LINK_NUM)
    {
        //MUX_TRACE_WARNING:"MUX_DTI_Create fail due to link id(%d) execed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_API_161_112_2_18_1_49_17_4,(uint8*)"d", link_id);
        return MUX_RET_LINK_NUM_ERR;
    }

    if (MUX_STATUS_ON != mux_ptr->is_start)
    {
        if(mux_ptr->ctl_link != link_id)//modify by yayanxu 20090619
        {
#ifdef _STATIC_LINK_SET_    
            /*Record DTI info,when DLCI be cretaed, copy to g_mux_dlc_info.entity*/
            MUX_MEMCPY(&g_mux_data_common_recv_func[link_id], &entity_info, sizeof(MUX_ENTITY_T));

            if(link_id >=MUX_COM1_LINK_NUM) 
               g_mux_dlc_info[link_id].work_mode = MUX_DLC_DATA_MODE; 
            return MUX_RET_SUCCESS;

#else/*_STATIC_LINK_SET_*/
            /*MUX not startup,ONLY allow LINK 0*/
            //MUX_TRACE_WARNING:"MUX_DTI_Create fail due to status error in link %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_API_179_112_2_18_1_49_17_5,(uint8*)"d", link_id);
            return MUX_RET_DLC_STATUS_ERR;
#endif/*_STATIC_LINK_SET_*/
        }
    }
    else /*MUX is working*/
    {
        if (g_mux_dlc_info[link_id].used_state == MUX_DLC_STATE_DISCONNECT)
        {
            //MUX_TRACE_WARNING:"MUX_DTI_Create fail due to status error in link %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_API_188_112_2_18_1_49_17_6,(uint8*)"d", link_id);
            return MUX_RET_DLC_STATUS_ERR;
        }
    }

    MUX_MEMCPY(&g_mux_dlc_info[link_id].entity, &entity_info, sizeof(MUX_ENTITY_T));

    /*It means the link enter data mode once DTI Create*/
    g_mux_dlc_info[link_id].work_mode = MUX_DLC_DATA_MODE;

#if 0 //MS00226768
    if (NULL == g_mux_dlc_info[link_id].mux_plus_timer)
    {
        g_mux_dlc_info[link_id].mux_plus_timer = MUX_CREATE_TIMER("MUX PLUS TIMER", (TIMER_FUN) MUX_Check_ThreePlus, link_id, MUX_MODE_CHANGE_DELAY, MUX_NO_ACTIVATE);
    }
#endif

    return MUX_RET_SUCCESS;
}


/*****************************************************************************/
// Description :    external module call this interface to send user data to peer end
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      10th July.08
// Note :           
/*****************************************************************************/
PUBLIC MUX_RETURN_E MUX_Write(uint8 *data_ptr, uint32 data_length, uint8 link_id)
{
    MUX_RETURN_E    res;
    MUX_INFO_T *mux_ptr;

    uint8 mux_index = g_mux_dlc_info[link_id].from;

    MUX_ASSERT(mux_index < MAX_MUX_NUM);/*assert verified*/

    mux_ptr = &g_mux_sys_info[mux_index];



    if (MUX_STATUS_ON != mux_ptr->is_start)
    {            
	if(mux_ptr->TE_wait_OK)//true
	{
             mux_ptr->TE_wait_OK	= FALSE;  
	      mux_ptr->is_start = MUX_STATUS_ON;
             //MUX_TRACE_WARNING:"MUX set ON after send OK"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_API_233_112_2_18_1_49_17_7,(uint8*)"");
	}
        /*MUX Don't startup*/
        MUX_Data_Tx(data_ptr, data_length, g_mux_dlc_info[link_id].from);
        return MUX_RET_SUCCESS;
    }

    /*check link id validity*/
    if (link_id >= MUX_MAX_LINK_NUM)
    {
        //MUX_TRACE_WARNING:"MUX_Write fail due to link id(%d) execed, data_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_API_243_112_2_18_1_49_17_8,(uint8*)"dd", link_id, data_length);
        return MUX_RET_LINK_NUM_ERR;
    }

    if (MUX_DLC_STATE_FLOW_STOPPED == g_mux_dlc_info[mux_ptr->ctl_link].used_state) //yayan.xu modify
    {
        /*Link 0 is in flow control status, means all links be related.*/
        //MUX_TRACE_WARNING:"MUX_Write: Flow stopped on all channels, return zero link id %d, data_len = %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_API_250_112_2_18_1_49_17_9,(uint8*)"dd", link_id, data_length);
        return MUX_RET_FAILURE;
    }
    else if (MUX_DLC_STATE_FLOW_STOPPED == g_mux_dlc_info[link_id].used_state)
    {
        //MUX_TRACE_WARNING:"MUX_Write: Flow stopped, return zero link id %d, data_len = %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_API_255_112_2_18_1_49_17_10,(uint8*)"dd", link_id, data_length);
        return MUX_RET_FAILURE;
    }

    if (g_mux_dlc_info[link_id].used_state != MUX_DLC_STATE_CONNECT)
    {
        //MUX_TRACE_WARNING:"MUX_Write fail due to status error in link id %d,data_len = %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_API_261_112_2_18_1_49_17_11,(uint8*)"dd", link_id, data_length);
        return MUX_RET_DLC_STATUS_ERR;
    }

    if (MUX_CTRL_CHAN == g_mux_dlc_info[link_id].dlci)
    {
        /*Forbid User data in DLCI 0 */
        //MUX_TRACE_WARNING:"MUX_Write fail due to forbid user data in DLCI 0, data_len = %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_API_268_112_2_18_1_49_17_12,(uint8*)"d", data_length);
        return MUX_RET_FAILURE;
    }
    res = MUX_Send_UIH(mux_ptr, data_ptr, data_length,\
                                g_mux_dlc_info[link_id].dlci);

    return res;
}
/*****************************************************************************/
// Description :    external module call this interface to get link id info,
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      21th July.08
// Note :           Just only for PPP.
/*****************************************************************************/
MUX_DLC_T * MUX_GetPPPPort(uint8 link_id)
{
    if (link_id >= MUX_MAX_LINK_NUM)
    {
        return PNULL;
    }

    return &g_mux_dlc_info[link_id];
}

/*****************************************************************************/
//Description: 2 Dimension array, In every row the first number is service type defined in atc, 
//                  and the second number is dlc number. If there's no dlc channel coupled with 
//                  specific service, then just set oxff on the second number in the row
//                  DLC10  <=> VT Data
//                  DLC11  <=> PPP1 Data
//                  DLC12 <=> PPP2 Data
//                  DLC5  <=> PPP3 Data
//                  DLC6  <=> PPP4 Data
/*****************************************************************************/
#ifdef _STATIC_LINK_SET_
LOCAL uint8 g_mux_service2linkid[MUX_MAX_DLC_NUM]   =
{
    0, 1, 2, 3, 4, 5, 6


};

/*****************************************************************************/
// Description :   This function is called by ATC module, which couples and transforms series of
//            transactions in differenct  origianl link_id channel to static and specific link_id channel
// Global resource dependence : none
// Author :    Andrew.Young  
// DATE :      21th July.2011
// Note :   only used by ATC module        
/*****************************************************************************/
uint8 MUX_GetLinkid(MUX_SERVICE_E type)
{

   uint8 dlci = g_mux_service2linkid[type];
   uint8 ret = 0xFF;

   //MUX_TRACE_WARNING:"MUX_Linkid_Couple type:%d, link id %d"

        
       //Modem, only use MUX_1;
       ret = MUX_Get_Linkid(dlci, MUX_1);
       
       if(MUX_DLC_LINK_INVALID == ret )
       {
         MUX_TRACE_WARNING(("MUX_GetLinkid fail!"));
             //ret = link_id;
       }
       //MUX_TRACE_WARNING:"MUX_Linkid_Couple return link id %d"
        return ret;

}
#endif/*_STATIC_LINK_SET_*/
