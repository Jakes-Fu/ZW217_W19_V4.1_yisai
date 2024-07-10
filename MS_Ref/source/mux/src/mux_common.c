/******************************************************************************
 ** File Name:      mux_recv.c                                      *
 ** Author:         yayan.xu                                             *
 ** DATE:           07/10/2008                                              *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:      The file defines public function for MUX module*
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



/*default DCL parameter according to 27.010*/
LOCAL MUX_PN_DATA_T g_mux_default_dlc_para  =
{
    0x00, 0x07, 0x0A, 0x1F, 0x00, 0x03, 0x02,0,0/*lint !e542*/
};
/*default DCL service according to 27.010*/



/*following reserved CRC table*/
const uint8         crctable[256]           =
{
    0x00, 0x91, 0xE3, 0x72, 0x07, 0x96, 0xE4, 0x75, 
    0x0E, 0x9F, 0xED, 0x7C, 0x09, 0x98, 0xEA, 0x7B, 
    0x1C, 0x8D, 0xFF, 0x6E, 0x1B, 0x8A, 0xF8, 0x69, 
    0x12, 0x83, 0xF1, 0x60, 0x15, 0x84, 0xF6, 0x67, 
    0x38, 0xA9, 0xDB, 0x4A, 0x3F, 0xAE, 0xDC, 0x4D, 
    0x36, 0xA7, 0xD5, 0x44, 0x31, 0xA0, 0xD2, 0x43, 
    0x24, 0xB5, 0xC7, 0x56, 0x23, 0xB2, 0xC0, 0x51, 
    0x2A, 0xBB, 0xC9, 0x58, 0x2D, 0xBC, 0xCE, 0x5F, 
    0x70, 0xE1, 0x93, 0x02, 0x77, 0xE6, 0x94, 0x05, 
    0x7E, 0xEF, 0x9D, 0x0C, 0x79, 0xE8, 0x9A, 0x0B, 
    0x6C, 0xFD, 0x8F, 0x1E, 0x6B, 0xFA, 0x88, 0x19, 
    0x62, 0xF3, 0x81, 0x10, 0x65, 0xF4, 0x86, 0x17, 
    0x48, 0xD9, 0xAB, 0x3A, 0x4F, 0xDE, 0xAC, 0x3D, 
    0x46, 0xD7, 0xA5, 0x34, 0x41, 0xD0, 0xA2, 0x33, 
    0x54, 0xC5, 0xB7, 0x26, 0x53, 0xC2, 0xB0, 0x21, 
    0x5A, 0xCB, 0xB9, 0x28, 0x5D, 0xCC, 0xBE, 0x2F, 
    0xE0, 0x71, 0x03, 0x92, 0xE7, 0x76, 0x04, 0x95, 
    0xEE, 0x7F, 0x0D, 0x9C, 0xE9, 0x78, 0x0A, 0x9B, 
    0xFC, 0x6D, 0x1F, 0x8E, 0xFB, 0x6A, 0x18, 0x89, 
    0xF2, 0x63, 0x11, 0x80, 0xF5, 0x64, 0x16, 0x87, 
    0xD8, 0x49, 0x3B, 0xAA, 0xDF, 0x4E, 0x3C, 0xAD, 
    0xD6, 0x47, 0x35, 0xA4, 0xD1, 0x40, 0x32, 0xA3, 
    0xC4, 0x55, 0x27, 0xB6, 0xC3, 0x52, 0x20, 0xB1, 
    0xCA, 0x5B, 0x29, 0xB8, 0xCD, 0x5C, 0x2E, 0xBF, 
    0x90, 0x01, 0x73, 0xE2, 0x97, 0x06, 0x74, 0xE5, 
    0x9E, 0x0F, 0x7D, 0xEC, 0x99, 0x08, 0x7A, 0xEB, 
    0x8C, 0x1D, 0x6F, 0xFE, 0x8B, 0x1A, 0x68, 0xF9, 
    0x82, 0x13, 0x61, 0xF0, 0x85, 0x14, 0x66, 0xF7, 
    0xA8, 0x39, 0x4B, 0xDA, 0xAF, 0x3E, 0x4C, 0xDD, 
    0xA6, 0x37, 0x45, 0xD4, 0xA1, 0x30, 0x42, 0xD3, 
    0xB4, 0x25, 0x57, 0xC6, 0xB3, 0x22, 0x50, 0xC1, 
    0xBA, 0x2B, 0x59, 0xC8, 0xBD, 0x2C, 0x5E, 0xCF
};
/*****************************************************************************/
// Description :    this function is used to check fcs , when a mux frame was received
// Global resource dependence :
// Author :         From Amoi
// DATE :         2007.7.25
// Note :           
/*****************************************************************************/
uint32 MUX_Crc_Check(uint8 *data, uint32 length, uint8 check_sum)
{
    uint8   fcs = 0xff;

    while (length--)
    {
        fcs = crctable[fcs ^ *data++];
    }
    fcs = crctable[fcs ^ check_sum];
    //MUX_TRACE_LOW(("MUX fcs : %x\n", fcs));
    //MUX_TRACE_LOW(("MUX fcs : %x\n", check_sum)); 
    if (fcs == 0xcf)
    {
        /*CRC_VALID) */
        //MUX_TRACE_LOW(("MUX_Crc_Check: CRC check OK\n"));
        return 0;
    }
    else
    {
        //MUX_TRACE_WARNING:"MUX MUX_Crc_Check: CRC check failed\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_COMMON_85_112_2_18_1_49_17_16,(uint8*)"");
        return 1;
    }
}
/*****************************************************************************/
// Description :    this function is used to calculate fcs , when a mux frame is ready to be sent
// Global resource dependence :
// Author :         from amoi
// DATE :         2007.7.25
// Note :           
/*****************************************************************************/
uint8 MUX_Crc_Calc(uint8 *data, uint32 length)
{
    uint8   fcs = 0xff;

    while (length--)
    {
        fcs = crctable[fcs ^ *data++];
    }

    return 0xff - fcs;
}


/*****************************************************************************/
// Description :    flush a specific link id info include DTI info
// Global resource dependence :  DLC  manager
// Author :        yayan.xu  
// DATE :         10th July.08
// Note :       
/*****************************************************************************/
MUX_RETURN_E MUX_FlushDlci(uint8 link_id)
{
    MUX_ASSERT(link_id < MUX_MAX_LINK_NUM);/*assert verified*/

    g_mux_dlc_info[link_id].dlci = MUX_DLC_LINK_INVALID;

    MUX_DlcSetPara(&(g_mux_dlc_info[link_id].param), &g_mux_default_dlc_para);  

    g_mux_dlc_info[link_id].rx_buf.end_point = 0;
    g_mux_dlc_info[link_id].rx_buf.start_point = 0;
    g_mux_dlc_info[link_id].rx_buf.size = 0;
    /*Maybe exist memory leak rick, determinatede by caller*/
    g_mux_dlc_info[link_id].rx_buf.data_ptr = NULL;

    g_mux_dlc_info[link_id].used_state = MUX_DLC_STATE_DISCONNECT;

    /*event_flag. every link use 2 bit, one for open, another for close.*/
    g_mux_dlc_info[link_id].open_wait_event = (MUX_OPEN_WAIT_EVENT) << (2 * link_id);
    g_mux_dlc_info[link_id].close_wait_event = (MUX_CLOSE_WAIT_EVENT) << (2 * link_id);

    /*When support MSC message, there shall be modify for v24_status*/
    MUX_MEMSET(&(g_mux_dlc_info[link_id].v24_status), 0x00, sizeof(MUX_V24_SIGNAL_T));
    /*default value is MUX_Default_ATC_Callback*/
    g_mux_dlc_info[link_id].common_recv_func = g_mux_common_recv_func;

    if (g_mux_dlc_info[link_id].mux_plus_timer != NULL)
    {
  	MUX_DELETE_TIMER(g_mux_dlc_info[link_id].mux_plus_timer);
        g_mux_dlc_info[link_id].mux_plus_timer = NULL;
        
    }

    if (g_mux_dlc_info[link_id].work_mode != MUX_DLC_DATA_MODE)
    {
        g_mux_dlc_info[link_id].work_mode = MUX_DLC_AT_MODE;
    }
    g_mux_dlc_info[link_id].entity.entity_recv_func = NULL;
    if(link_id < MUX_COM1_LINK_NUM)	//MS00145432 
    {
	    g_mux_dlc_info[link_id].from = MUX_1; 
    }
    else
    {
	    g_mux_dlc_info[link_id].from = MUX_2;     
    }

    g_mux_dlc_info[link_id].cid = 0xff;
#ifdef MUX_PARSE_SNC_AFTER_DLC_CREATED
    g_mux_dlc_info[link_id].service = 0;
    g_mux_dlc_info[link_id].codec = 0;
#endif

    return MUX_RET_SUCCESS;
}

/*****************************************************************************/
// Description :    flush a specific link id info include DTI info
// Global resource dependence :  DLC  manager
// Author :        yinqiu.hu  
// DATE : 
// Note :       
/*****************************************************************************/
MUX_RETURN_E MUX_ResetDlci(uint8 link_id)
{
    MUX_ASSERT(link_id < MUX_MAX_LINK_NUM);/*assert verified*/

    g_mux_dlc_info[link_id].dlci = MUX_DLC_LINK_INVALID;

    MUX_DlcSetPara(&(g_mux_dlc_info[link_id].param), &g_mux_default_dlc_para);  

    g_mux_dlc_info[link_id].rx_buf.end_point = 0;
    g_mux_dlc_info[link_id].rx_buf.start_point = 0;
    g_mux_dlc_info[link_id].rx_buf.size = 0;
    /*Maybe exist memory leak rick, determinatede by caller*/
    g_mux_dlc_info[link_id].rx_buf.data_ptr = NULL;

    g_mux_dlc_info[link_id].used_state = MUX_DLC_STATE_DISCONNECT;

    /*event_flag. every link use 2 bit, one for open, another for close.*/
    g_mux_dlc_info[link_id].open_wait_event = (MUX_OPEN_WAIT_EVENT) << (2 * link_id);
    g_mux_dlc_info[link_id].close_wait_event = (MUX_CLOSE_WAIT_EVENT) << (2 * link_id);

    /*When support MSC message, there shall be modify for v24_status*/
    MUX_MEMSET(&(g_mux_dlc_info[link_id].v24_status), 0x00, sizeof(MUX_V24_SIGNAL_T));
    /*default value is MUX_Default_ATC_Callback*/
    g_mux_dlc_info[link_id].common_recv_func = g_mux_common_recv_func;

    if (g_mux_dlc_info[link_id].mux_plus_timer != NULL)
    {
  	MUX_DELETE_TIMER(g_mux_dlc_info[link_id].mux_plus_timer);
        g_mux_dlc_info[link_id].mux_plus_timer = NULL;
        
    }

    g_mux_dlc_info[link_id].work_mode = MUX_DLC_AT_MODE;
    g_mux_dlc_info[link_id].entity.entity_recv_func = NULL;
    if(link_id < MUX_COM1_LINK_NUM)	//MS00145432 
    {
	    g_mux_dlc_info[link_id].from = MUX_1; 
    }
    else
    {
	    g_mux_dlc_info[link_id].from = MUX_2;     
    }

    g_mux_dlc_info[link_id].cid = 0xff;
    g_mux_dlc_info[link_id].service = MUX_DLC_SERVICE_NULL;
    g_mux_dlc_info[link_id].codec = MUX_DLC_CODEC_NULL;

    return MUX_RET_SUCCESS;
}

/*****************************************************************************/
// Description :    Set dcl parameters
// Global resource dependence :  DLC  manager
// Author :        yayan.xu   
// DATE :         18th Jan.08
// Note :       
/*****************************************************************************/
MUX_RETURN_E MUX_DlcSetPara(MUX_PN_DATA_T *para_ptr, MUX_PN_DATA_T *new_para_ptr)
{
    MUX_ASSERT(PNULL != para_ptr);  /*assert verified*/ 
    MUX_ASSERT(PNULL != new_para_ptr);/*assert verified*/   

    /*Because size of  MUX_PN_DATA_T is NOT integer times than 4 byte, 
       So NOT COPY directly!!!!!!*/
    para_ptr ->credit_flow = new_para_ptr ->credit_flow;
    para_ptr ->frame_type = new_para_ptr ->frame_type;

    para_ptr->res2 = new_para_ptr->res2;
    para_ptr ->prior = new_para_ptr ->prior;        

    para_ptr ->ack_timer = new_para_ptr ->ack_timer;    

    para_ptr ->frame_sizel = new_para_ptr ->frame_sizel;    
    para_ptr ->frame_sizeh = new_para_ptr ->frame_sizeh;    
    para_ptr ->max_nbrof_retrans = new_para_ptr ->frame_sizeh;
    para_ptr->credits = new_para_ptr->credits;

    return MUX_RET_SUCCESS;
}

/*****************************************************************************/
// Description :    default ATC callback function
// Global resource dependence :  DLC  manager
// Author :        yayan.xu   
// DATE :         18th July.08
// Note :       
/*****************************************************************************/
void MUX_Default_ATC_Callback(uint8 link_id, uint32 len, uint8 *data_ptr)
{
    //MUX_TRACE_WARNING:"MUX: MUX_Default_ATC_Callback in link_id = %d, len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_COMMON_188_112_2_18_1_49_18_17,(uint8*)"dd", link_id, len);
    MUX_TraceData(data_ptr, len);
    MUX_ASSERT(FALSE);/*assert verified*/
}

/*****************************************************************************/
// Description :    find linkid by DLCI
// Global resource dependence :  DLC  manager
// Author :        yayan.xu   
// DATE :         18th July.08
// Note :       
/*****************************************************************************/
uint8 MUX_Get_Linkid(uint8 dlci,  uint8  from)
{
    uint8   loop;
    for (loop = 0; loop < MUX_MAX_LINK_NUM; loop++)
    {
        if ((g_mux_dlc_info[loop].used_state != MUX_DLC_STATE_DISCONNECT)
            && (dlci == g_mux_dlc_info[loop].dlci) && (from  == g_mux_dlc_info[loop].from))
        {
            return loop;
        }
    }
    //MUX_TRACE_WARNING:"MUX_Get_Linkid, 0XFF"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_COMMON_210_112_2_18_1_49_18_18,(uint8*)"");
    return MUX_DLC_LINK_INVALID;
}

uint8 MUX_Get_AllLinkid(uint8 dlci,  uint8  from)
{
    uint8   loop;
    for (loop = 0; loop < MUX_MAX_LINK_NUM; loop++)
    {
        if ((dlci == g_mux_dlc_info[loop].dlci) && (from  == g_mux_dlc_info[loop].from))
        {
            return loop;
        }
    }
    //MUX_TRACE_WARNING:"MUX_Get_Linkid, 0XFF"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_COMMON_210_112_2_18_1_49_18_18,(uint8*)"");
    return MUX_DLC_LINK_INVALID;
}
        
/*****************************************************************************/
// Description :    this function is used to confirm that the dlci being opreated is valid
// Global resource dependence :
// Author :         from Amoi
// DATE :         2007.7.25
// Note :           
/*****************************************************************************/
uint8 MUX_Valid_Dlci(uint8 dlci)
{
    /*In TS27.010, valid range is 0-63*/
    if (dlci < MUX_MAX_DLC_NUM)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
// Description :    this function is used to confirm that the dlci being opreated is valid
// Global resource dependence :
// Author :         from Amoi
// DATE :         2007.7.25
// Note :           
/*****************************************************************************/
MUX_RETURN_E MUX_Get_NewLinkid(uint8 *link_id, uint8 from)
{
    uint8   loop, loop_start, loop_end ;

    *link_id = MUX_DLC_LINK_INVALID;

    if(MUX_2 == from)
    {
        loop_start = MUX_COM1_LINK_NUM;
        loop_end  = MUX_MAX_LINK_NUM;
    }
    else
    {
         loop_start = 0;
         loop_end  = MUX_COM1_LINK_NUM;
    }
    
    for (loop = loop_start; loop < loop_end; loop++)
    {
        if (MUX_DLC_STATE_DISCONNECT == g_mux_dlc_info[loop].used_state)
        {
            *link_id = loop;
             MUX_TRACE_LOW(("MUX_Get_NewLinkid = %d",  loop));	
            return MUX_RET_SUCCESS;
        }
    }
    //MUX_TRACE_WARNING:"MUX_Get_NewLinkid  fails"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_COMMON_263_112_2_18_1_49_18_19,(uint8*)"");
    return MUX_RET_FAILURE;
}


/*****************************************************************************/
// Description :    this function is used to print data filed
// Global resource dependence :
// Author :         from mark
// DATE :         2007.7.25
// Note :           
/*****************************************************************************/
void MUX_TraceData(unsigned char *data, uint32 datalen)
{
//#ifdef _MUX_DEBUG_ //del@yayan.xu
    uint32  i = 0, n = 0;

    //SCI_TRACE_LOW:"MUX_TraceDatat datalen = %d \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_COMMON_278_112_2_18_1_49_18_20,(uint8*)"d", datalen);

    while (i < datalen)
    {
        n = datalen - i;// + 8;
        if (n > 8)
        {
            //SCI_TRACE_LOW:"0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, \r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_COMMON_285_112_2_18_1_49_18_21,(uint8*)"dddddddd", data[i], data[i + 1], data[i + 2], data[i + 3], data[i + 4], data[i + 5], data[i + 6], data[i + 7]);
            i += 8;
        }
        else
        {
            while (n > 0)
            {
                //SCI_TRACE_LOW:"0x%02x, \r\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_COMMON_292_112_2_18_1_49_18_22,(uint8*)"d", data[i]);
                i++;
                n--;
            }
            break;
        }
    }
//#endif/*_MUX_DEBUG_*/ del@yayan.xu
}

/*****************************************************************************/
// Description :    this function is used to init MUX global variable.
// Global resource dependence :
// Author :         yayan.xu
// DATE :         2007.8.8
// Note :           
/*****************************************************************************/
PUBLIC void MUX_Init(void)
{
    uint8   i;

    /*init MUX recv task*/
    MUX_RecvTaskInit();

    for (i = 0; i < MUX_MAX_LINK_NUM; i++)
    {
        MUX_MEMSET(&g_mux_data_common_recv_func[i], 0x00, sizeof(MUX_ENTITY_T));
    }


    return ;
}
/*****************************************************************************/
// Description :    this function is used to assemble AT+CMUX parameter.
// Global resource dependence :
// Author :         yayan.xu
// DATE :         2007.8.28
// Note :           
/*****************************************************************************/
MUX_RETURN_E MUX_Assemble_CMUX_CMD(MUX_SYS_PARA_T *para, char *output_ptr, uint8 *output_len, uint8 times)
{
    *output_len = 0;

    if (PNULL == output_ptr)
    {
        //MUX_TRACE_WARNING:"MUX_Assemble_CMUX_CMD: outptr is null pointer\r"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_COMMON_333_112_2_18_1_49_18_23,(uint8*)"");
        return MUX_RET_FAILURE;
    }


    /*Check para, rule refer MUX_SYS_PARA_T structure defination.*/
    if (PNULL != para)
    {
        /*work_mode use enum struct, Not need check*/
        /*subset use enum struct, Not need check*/
        /*port_speed use enum struct, Not need check*/
        if ((para->N1 > 32768) || (para->N1 < 1))
        {
            // maximum frame size, range: 1-1- 32768 default Value : 31
            para->N1 = 31;
        }

        if (para->T1 < 10)//acknowledgement timer in units of ten milliseconds. Default: 10  (100ms)
        {
            /*value below 10, maybe happen timeout easily*/
            para->T1 = 10;
        }

        if (para->N2 > 100)
        {
            //maximum number of re-transmissions,range:(0-100);default:3.
            para->N2 = 3;
        }

        if (para->T2 < 2)
        {
            /*range:2-255, default: 30 (300ms)*/
            para->T2 = 30;
        }

        if (para->T3 < 1)
        {
            /*Range: 1-255, default :10*/
            para->T3 = 10;
        }

        if ((para->K > 7) || (para->K < 1))
        {
            // Range:1-7,default:2
            para->K = 2;
        }
    }
    /*Retry 3 times: 
     First according to customer setting
     Seconde basic mode preferred.
     Third advance mode.
    */
    switch (times)
    {
        case 3:
            if (PNULL == para)
            {
                MUX_STRCPY(output_ptr, "AT+CMUX=0\r");
            }
            else
            {
                MUX_SPRINTF(output_ptr, "AT+CMUX=%d,%d,%d,%d,%d,%d,%d,%d,%d\r", para->mux_mode, para->subset, para->port_speed, para->N1, para->T1, para->N2, para->T2, para->T3, para->K);
            }
            break;
        case 1:
            MUX_STRCPY(output_ptr, "AT+CMUX=1\r");
            break;            
        case 2:
        default:
            MUX_STRCPY(output_ptr, "AT+CMUX=0\r");
            break;
    }
    *output_len += MUX_STRLEN(output_ptr); 

    return MUX_RET_SUCCESS;
}
