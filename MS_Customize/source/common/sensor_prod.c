/******************************************************************************
 ** File Name:     sensor_prod.c                                              *
 ** Description:                                                              *
 **    This file contains common test sensor inteface                         *
 ** Author:         Jianping.wang                                             *
 ** DATE:           06/02/2011                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/02/2011     Jianping.wang        Create.                               *
 ******************************************************************************/
#include "ms_customize_trc.h"
#include "diag.h"
#include "cmddef.h"
#include "sensor_drv.h"
#include "dal_dcamera.h"
#include "dal_display.h"
#include "sensor_cfg.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
#define TOOL_SENSOR_OP_HANDLED           1
#define MSG_UNIT_SIZE                    2 // 2 bytest

#define SENSOR_ID_OFFSET                 0
#define SENSOR_WRITE_OP_DATA_LEN         (2*MSG_UNIT_SIZE)

//read register of sensor
#define SENSOR_REG_SUM                   1
#define SENSOR_READ_REG_ADDRESS_OFFSET   2 
//

//write register of sensor
#define SENSOR_WRITE_REG_ADDRESS_OFFSET  2
#define SENSOR_WRITE_REG_VALUE_OFFSET    (SENSOR_WRITE_REG_ADDRESS_OFFSET+1)
//

//write VSYNC 
#define SENSOR_VSYNC_OFFSET              1
//

//write VSYNC 
#define SENSOR_HSYNC_OFFSET              1
//
#define SENSOR_RESOLUTION_TABLE_SUM      4

#define TOOL_SENSOR_OP_SUCCESS           0
#define TOOL_SENSOR_OP_FAIL_TABLE_NUM    1
#define TOOL_SENSOR_OP_FAIL_SENSOR_ID    2 
#define TOOL_SENSOR_OP_FAIL_READ_SUM     3
#define TOOL_SENSOR_OP_FAIL_WRITE_SUM    4 
#define TOOL_SENSOR_OP_FAIL_TABLE_LEN    5 
/**--------------------------------------------------------------------------*
 **                         LOCAL VARIABLE                                   *
 **--------------------------------------------------------------------------*/
LOCAL SENSOR_INFO_T* s_sensor_table_ptr = PNULL;
//LOCAL SENSOR_REG_TAB_INFO_T_PTR s_sensor_resolution_ptr[SENSOR_MODE_MAX]={0};


/**--------------------------------------------------------------------------*
 **                         LOCAL INTERFACE                                  *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//Description:  check if find sensor 
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL uint32 _CheckSensor( 
                               SENSOR_ID_E sensor_id
                              )
{       
    SENSOR_ID_E cur_id = SENSOR_ID_MAX;
    
    if(sensor_id > SENSOR_SUB)
    {
        //SCI_TRACE_LOW:"[sensor_prod.c:_check_msg: sensor_id error,sensor_id = %d]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_PROD_80_112_2_18_0_26_19_24,(uint8*)"d",sensor_id);
        return 0;
    }

    cur_id = Sensor_GetCurId();

    if(cur_id != sensor_id)
    {
        //SCI_TRACE_LOW:"[sensor_prod.c: sensor id error!]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_PROD_88_112_2_18_0_26_19_25,(uint8*)"");
        return 0;
    }
    
    Sensor_GetTable(sensor_id,&s_sensor_table_ptr);
    if(PNULL == s_sensor_table_ptr)
    {
        //SCI_TRACE_LOW:"[sensor_prod.c:_check_msg:set sensor info table fail]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_PROD_95_112_2_18_0_26_19_26,(uint8*)"");
        return 0;
    }       

    return 1;
    
}


/*****************************************************************************/
//Description:  read register of sensor
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL uint32 _ReadSensorReg( 
                                   uint8 **dest_ptr,       // Pointer of the response message.
                                   uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
                                   const uint8 *src_ptr,   // Pointer of the input message. 
                                   uint16 src_len          // Size of the source buffer in uint8.
                                  )
{        
	uint16  len;                    
    uint16 *src_temp_ptr = PNULL;  
    uint8  *dest_temp_ptr = PNULL;
    uint16 *temp_ptr = PNULL;
    uint32  read_sum = 0;
    uint16  reg_value = 0;
    SENSOR_ID_E sensor_id = SENSOR_MAIN;      
        
    src_temp_ptr = (uint16*)(src_ptr+sizeof(MSG_HEAD_T));
    
    sensor_id = (SENSOR_ID_E)src_temp_ptr[SENSOR_ID_OFFSET];
    read_sum  = src_temp_ptr[SENSOR_REG_SUM];

    len = sizeof(MSG_HEAD_T)+MSG_UNIT_SIZE+MSG_UNIT_SIZE+MSG_UNIT_SIZE*read_sum;    
    dest_temp_ptr = (uint8*)SCI_ALLOCA(len);
    
    if(PNULL == dest_temp_ptr)
    {
        return TOOL_SENSOR_OP_HANDLED;
    }
    
    *dest_ptr = dest_temp_ptr;
    *dest_len_ptr = len;    
    
    SCI_MEMCPY(dest_temp_ptr,src_ptr,sizeof(MSG_HEAD_T));
    ((MSG_HEAD_T*)dest_temp_ptr)->len = len;
    
    dest_temp_ptr += sizeof(MSG_HEAD_T);
    temp_ptr = (uint16*)dest_temp_ptr;
    *temp_ptr++ = sensor_id;

    if((0 == read_sum) || (1 != _CheckSensor(sensor_id)))
    {
        if(0 == read_sum)
        {
            *temp_ptr++ = TOOL_SENSOR_OP_FAIL_READ_SUM;//fail
        }
        else
        {
            *temp_ptr++ = TOOL_SENSOR_OP_FAIL_SENSOR_ID;//fail
        }
        return TOOL_SENSOR_OP_HANDLED;
    }
    *temp_ptr++ = TOOL_SENSOR_OP_SUCCESS;//success   
    
    reg_value = Sensor_ReadReg(src_temp_ptr[SENSOR_READ_REG_ADDRESS_OFFSET]);
    *temp_ptr++ = reg_value;
    
    if(read_sum>1)
    {
        uint32 i = 0;
        uint32 j = SENSOR_READ_REG_ADDRESS_OFFSET+1; 
        read_sum -= 1;
        for( i=0; i< read_sum ; i++)
        {
            *temp_ptr++ = Sensor_ReadReg(src_temp_ptr[j++]);
        }
    }
    
    return TOOL_SENSOR_OP_HANDLED;
    
}
/*****************************************************************************/
//Description:  write register of sensor
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL uint32 _WriteSensorReg( 
                                   uint8 **dest_ptr,       // Pointer of the response message.
                                   uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
                                   const uint8 *src_ptr,   // Pointer of the input message. 
                                   uint16 src_len          // Size of the source buffer in uint8.
                                  )
{
	uint16  len;                    
    uint16 *src_temp_ptr = PNULL;  
    uint8  *dest_temp_ptr = PNULL;
    uint16 *temp_ptr = PNULL;
    uint32  write_sum = 0;  
    SENSOR_ID_E sensor_id = SENSOR_MAIN;      
        
    src_temp_ptr = (uint16*)(src_ptr+sizeof(MSG_HEAD_T));
    
    sensor_id = (SENSOR_ID_E)src_temp_ptr[SENSOR_ID_OFFSET];
    write_sum  = src_temp_ptr[SENSOR_REG_SUM];

    len = sizeof(MSG_HEAD_T)+SENSOR_WRITE_OP_DATA_LEN;    
    dest_temp_ptr = (uint8*)SCI_ALLOCA(len);
    if(PNULL == dest_temp_ptr)
    {
        return TOOL_SENSOR_OP_HANDLED;
    }
    *dest_ptr = dest_temp_ptr;
    *dest_len_ptr = len;    
    
    SCI_MEMCPY(dest_temp_ptr,src_ptr,sizeof(MSG_HEAD_T));
    ((MSG_HEAD_T*)dest_temp_ptr)->len = len;
    
    dest_temp_ptr += sizeof(MSG_HEAD_T);
    temp_ptr = (uint16*)dest_temp_ptr;
    *temp_ptr++ = sensor_id;

    if((0 == write_sum) || (1 != _CheckSensor(sensor_id)))
    {
        if(0 == write_sum)
        {        
            *temp_ptr++ = TOOL_SENSOR_OP_FAIL_WRITE_SUM;//fail
        }
        else
        {
            *temp_ptr++ = TOOL_SENSOR_OP_FAIL_SENSOR_ID;//fail
        }
        return TOOL_SENSOR_OP_HANDLED;
    }    
    *temp_ptr++ = TOOL_SENSOR_OP_SUCCESS;
    
    Sensor_WriteReg(src_temp_ptr[SENSOR_WRITE_REG_ADDRESS_OFFSET],src_temp_ptr[SENSOR_WRITE_REG_VALUE_OFFSET]);
    if(write_sum > 1)
    {
        uint32 i = 0;
        uint32 j = SENSOR_WRITE_REG_VALUE_OFFSET+1;
        uint16 addr = 0;
        uint16 data = 0;
        write_sum -= 1;
        
        for( i = 0 ; i<write_sum ; i++)
        {
            addr = src_temp_ptr[j++];
            data = src_temp_ptr[j++];
            Sensor_WriteReg(addr,data);
        }
    }   

    return TOOL_SENSOR_OP_HANDLED;
}
/*****************************************************************************/
//Description:  get sum of resolution configuration table 
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL uint32 _GetSensorResolutionTabSum( 
                                                  uint8 **dest_ptr,       // Pointer of the response message.
                                                  uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
                                                  const uint8 *src_ptr,   // Pointer of the input message. 
                                                  uint16 src_len          // Size of the source buffer in uint8.
                                                 )
{
	uint16  len;                    
    uint16 *src_temp_ptr = PNULL;  
    uint8  *dest_temp_ptr = PNULL;
    uint16 *temp_ptr = PNULL;       
    uint32  table_sum = 0;
    SENSOR_ID_E sensor_id = SENSOR_MAIN;      
        
    src_temp_ptr = (uint16*)(src_ptr+sizeof(MSG_HEAD_T));
    
    sensor_id = (SENSOR_ID_E)src_temp_ptr[SENSOR_ID_OFFSET];

    len = sizeof(MSG_HEAD_T)+MSG_UNIT_SIZE+MSG_UNIT_SIZE+MSG_UNIT_SIZE; 

    dest_temp_ptr = (uint8*)SCI_ALLOCA(len);
    if(PNULL == dest_temp_ptr)
    {
        return TOOL_SENSOR_OP_HANDLED;
    }
    *dest_ptr = dest_temp_ptr;
    *dest_len_ptr = len;    

    SCI_MEMCPY(dest_temp_ptr,src_ptr,sizeof(MSG_HEAD_T));
    ((MSG_HEAD_T*)dest_temp_ptr)->len = len;
    
    dest_temp_ptr += sizeof(MSG_HEAD_T);
    temp_ptr = (uint16*)dest_temp_ptr;
    *temp_ptr++ = sensor_id;

    if(1 != _CheckSensor(sensor_id))
    {        
        *temp_ptr++ = TOOL_SENSOR_OP_FAIL_SENSOR_ID;
        return TOOL_SENSOR_OP_HANDLED;
    }  
    *temp_ptr++ = TOOL_SENSOR_OP_SUCCESS;

    {
       
        SENSOR_REG_TAB_INFO_T* resolution_info_ptr = NULL;
        uint32 i = 0;
        
        for(i=SENSOR_MODE_COMMON_INIT; i<SENSOR_MODE_MAX; i++)
        {
            resolution_info_ptr = &(s_sensor_table_ptr->resolution_tab_info_ptr[i]);
            if((PNULL!= resolution_info_ptr->sensor_reg_tab_ptr)||((0x00!=resolution_info_ptr->width)&&(0x00!=resolution_info_ptr->width)))
            {                                    
                table_sum++;
            }           
        }
    }

    *temp_ptr++ = table_sum;
    
    return TOOL_SENSOR_OP_HANDLED;
    
}
/*****************************************************************************/
//Description:  get a resolution configuration table of sensor 
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL uint32 _ReadSensorResolutionTab( 
                                              uint8 **dest_ptr,       // Pointer of the response message.
                                              uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
                                              const uint8 *src_ptr,   // Pointer of the input message. 
                                              uint16 src_len          // Size of the source buffer in uint8.
                                             )
{    
	uint16  len;                    
    uint16 *src_temp_ptr = PNULL;  
    uint8  *dest_temp_ptr = PNULL;
    uint16 *temp_ptr = PNULL;
    uint32  table_num = 0;
    SENSOR_ID_E sensor_id = SENSOR_MAIN;  
    SENSOR_REG_TAB_INFO_T_PTR resolution_ptr = PNULL;
        
    src_temp_ptr = (uint16*)(src_ptr+sizeof(MSG_HEAD_T));
    
    sensor_id = (SENSOR_ID_E)src_temp_ptr[SENSOR_ID_OFFSET];
    table_num = src_temp_ptr[SENSOR_ID_OFFSET+1];

    if((SENSOR_MODE_MAX <= table_num) || (1 != _CheckSensor(sensor_id)))
    {   
        len = sizeof(MSG_HEAD_T)+MSG_UNIT_SIZE+MSG_UNIT_SIZE; 
        dest_temp_ptr = (uint8*)SCI_ALLOCA(len);
        if(PNULL != dest_temp_ptr)
        {
            *dest_ptr = dest_temp_ptr;
            *dest_len_ptr = len;    

            SCI_MEMCPY(dest_temp_ptr,src_ptr,sizeof(MSG_HEAD_T));
            ((MSG_HEAD_T*)dest_temp_ptr)->len = len;
            dest_temp_ptr += sizeof(MSG_HEAD_T);
            temp_ptr = (uint16*)dest_temp_ptr;
            *temp_ptr++ = sensor_id;
            if(SENSOR_MODE_MAX <= table_num)
            {
                *temp_ptr = TOOL_SENSOR_OP_FAIL_TABLE_NUM;//fail
            }
            else
            {
                *temp_ptr = TOOL_SENSOR_OP_FAIL_SENSOR_ID;//fail
            }
        }
        return TOOL_SENSOR_OP_HANDLED;
    }    
  
    resolution_ptr = &s_sensor_table_ptr->resolution_tab_info_ptr[table_num];
       
    if(PNULL != resolution_ptr->sensor_reg_tab_ptr)
    {
        len = sizeof(MSG_HEAD_T)+2*MSG_UNIT_SIZE+4*resolution_ptr->reg_count+4*MSG_UNIT_SIZE+MSG_UNIT_SIZE; 
    }
    else
    {
        len = sizeof(MSG_HEAD_T)+MSG_UNIT_SIZE+MSG_UNIT_SIZE+MSG_UNIT_SIZE+4*MSG_UNIT_SIZE;  
    }

    dest_temp_ptr = (uint8*)SCI_ALLOCA(len);
    if(PNULL == dest_temp_ptr)
    {
        return TOOL_SENSOR_OP_HANDLED;
    }
    *dest_ptr = dest_temp_ptr;
    *dest_len_ptr = len;    

    SCI_MEMCPY(dest_temp_ptr,src_ptr,sizeof(MSG_HEAD_T));
    ((MSG_HEAD_T*)dest_temp_ptr)->len = len;
    
    dest_temp_ptr += sizeof(MSG_HEAD_T);
    temp_ptr = (uint16*)dest_temp_ptr;

    *temp_ptr++ = sensor_id;
    *temp_ptr++ = TOOL_SENSOR_OP_SUCCESS;
    if(PNULL != resolution_ptr->sensor_reg_tab_ptr)
    {
        uint32 i = 0;
        *temp_ptr++ = resolution_ptr->reg_count;
        for( i=0 ; i<resolution_ptr->reg_count ; i++)
        {
            *temp_ptr++ = resolution_ptr->sensor_reg_tab_ptr[i].reg_addr;
            *temp_ptr++ = resolution_ptr->sensor_reg_tab_ptr[i].reg_value;
        }        
    }
    else
    {
        *temp_ptr++ = 0;
    }
    *temp_ptr++ = resolution_ptr->width;
    *temp_ptr++ = resolution_ptr->height;
    *temp_ptr++ = resolution_ptr->xclk_to_sensor;
    *temp_ptr   = (uint16)resolution_ptr->image_format;

    return TOOL_SENSOR_OP_HANDLED;     

}
/*****************************************************************************/
//Description:  modify a resolution configuration table of sensor 
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL uint32 _WriteSensorResolutionTab( 
                                              uint8 **dest_ptr,       // Pointer of the response message.
                                              uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
                                              const uint8 *src_ptr,   // Pointer of the input message. 
                                              uint16 src_len          // Size of the source buffer in uint8.
                                             )
{
	uint16  len;                    
    uint16 *src_temp_ptr = PNULL;  
    uint8  *dest_temp_ptr = PNULL;
    uint16 *temp_ptr = PNULL;
    uint32  table_num = 0;
    uint16  table_len = 0;
    SENSOR_ID_E sensor_id = SENSOR_MAIN;  
    SENSOR_REG_TAB_INFO_T_PTR resolution_ptr = PNULL;
        
    src_temp_ptr = (uint16*)(src_ptr+sizeof(MSG_HEAD_T));
    
    sensor_id = (SENSOR_ID_E)src_temp_ptr[SENSOR_ID_OFFSET];
    table_num = src_temp_ptr[SENSOR_ID_OFFSET+1];
    table_len = src_temp_ptr[SENSOR_ID_OFFSET+2];

    src_temp_ptr += 3;

    len = sizeof(MSG_HEAD_T)+SENSOR_WRITE_OP_DATA_LEN; 
    dest_temp_ptr = (uint8*)SCI_ALLOCA(len);
    if(!dest_temp_ptr)
    {
        return TOOL_SENSOR_OP_HANDLED;
    }
    *dest_ptr = dest_temp_ptr;
    *dest_len_ptr = len;    

    SCI_MEMCPY(dest_temp_ptr,src_ptr,sizeof(MSG_HEAD_T));
    ((MSG_HEAD_T*)dest_temp_ptr)->len = len;
    dest_temp_ptr += sizeof(MSG_HEAD_T);
    temp_ptr = (uint16*)dest_temp_ptr;
    *temp_ptr++ = sensor_id;
    
    if((SENSOR_MODE_MAX <= table_num) || (1 != _CheckSensor(sensor_id)))
    {          
        if(SENSOR_MODE_MAX <= table_num)
        {
            *temp_ptr = TOOL_SENSOR_OP_FAIL_TABLE_NUM;//fail        
        }
        else
        {
            *temp_ptr = TOOL_SENSOR_OP_FAIL_SENSOR_ID;//fail   
        }
        return TOOL_SENSOR_OP_HANDLED;
    }         
    resolution_ptr = &s_sensor_table_ptr->resolution_tab_info_ptr[table_num];

    if(table_len <= resolution_ptr->reg_count)
    {
        uint32 i = 0;
        
        for( i=0 ; i<table_len ; i++)
        {
            resolution_ptr->sensor_reg_tab_ptr[i].reg_addr  = *src_temp_ptr++;
            resolution_ptr->sensor_reg_tab_ptr[i].reg_value = *src_temp_ptr++;
        }

        resolution_ptr->width = *src_temp_ptr++;
        resolution_ptr->height = *src_temp_ptr++;
        resolution_ptr->xclk_to_sensor = *src_temp_ptr++;
        resolution_ptr->image_format = (SENSOR_IMAGE_FORMAT)*src_temp_ptr;
        *temp_ptr = TOOL_SENSOR_OP_SUCCESS;
    }
    else
    {
        *temp_ptr = TOOL_SENSOR_OP_FAIL_TABLE_LEN;//fail    
    }
    
    return TOOL_SENSOR_OP_HANDLED;
}
/*****************************************************************************/
//Description:  read the polarity of vsync 
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL uint32 _ReadVysnc( 
                              uint8 **dest_ptr,       // Pointer of the response message.
                              uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
                              const uint8 *src_ptr,   // Pointer of the input message. 
                              uint16 src_len          // Size of the source buffer in uint8.
                           )
{  
	uint16  len;                    
    uint16 *src_temp_ptr = PNULL;  
    uint8  *dest_temp_ptr = PNULL;
    uint16 *temp_ptr = PNULL;
    SENSOR_ID_E sensor_id = SENSOR_MAIN;  
          
    src_temp_ptr = (uint16*)(src_ptr+sizeof(MSG_HEAD_T));
    
    sensor_id = (SENSOR_ID_E)src_temp_ptr[SENSOR_ID_OFFSET];

    len = sizeof(MSG_HEAD_T)+MSG_UNIT_SIZE+2*MSG_UNIT_SIZE; 
    dest_temp_ptr = (uint8*)SCI_ALLOCA(len);
    if(!dest_temp_ptr)
    {
        return TOOL_SENSOR_OP_HANDLED;
    }
    *dest_ptr = dest_temp_ptr;
    *dest_len_ptr = len;    

    SCI_MEMCPY(dest_temp_ptr,src_ptr,sizeof(MSG_HEAD_T));
    ((MSG_HEAD_T*)dest_temp_ptr)->len = len;
    dest_temp_ptr += sizeof(MSG_HEAD_T);
    temp_ptr = (uint16*)dest_temp_ptr;
    *temp_ptr++ = sensor_id;

    if(1 != _CheckSensor(sensor_id))
    {          
        *temp_ptr = TOOL_SENSOR_OP_FAIL_SENSOR_ID;//fail        
        return TOOL_SENSOR_OP_HANDLED;
    } 

    *temp_ptr++ = TOOL_SENSOR_OP_SUCCESS;

    *temp_ptr = s_sensor_table_ptr->hw_signal_polarity&0x0004;//bit2->vsync

    return TOOL_SENSOR_OP_HANDLED;

}
/*****************************************************************************/
//Description:  modify the polarity of vsync 
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL uint32 _WriteVysnc( 
                                  uint8 **dest_ptr,       // Pointer of the response message.
                                  uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
                                  const uint8 *src_ptr,   // Pointer of the input message. 
                                  uint16 src_len          // Size of the source buffer in uint8.
                                  )
{
	uint16  len;                    
    uint16 *src_temp_ptr = PNULL;  
    uint8  *dest_temp_ptr = PNULL;
    uint16 *temp_ptr = PNULL;     
    uint32  temp = 0;
    SENSOR_ID_E sensor_id = SENSOR_MAIN;  
          
    src_temp_ptr = (uint16*)(src_ptr+sizeof(MSG_HEAD_T));
    
    sensor_id = (SENSOR_ID_E)src_temp_ptr[SENSOR_ID_OFFSET];

    len = sizeof(MSG_HEAD_T)+SENSOR_WRITE_OP_DATA_LEN; 
    dest_temp_ptr = (uint8*)SCI_ALLOCA(len);
    if(!dest_temp_ptr)
    {
        return TOOL_SENSOR_OP_HANDLED;
    }
    *dest_ptr = dest_temp_ptr;
    *dest_len_ptr = len;    

    SCI_MEMCPY(dest_temp_ptr,src_ptr,sizeof(MSG_HEAD_T));
    ((MSG_HEAD_T*)dest_temp_ptr)->len = len;
    dest_temp_ptr += sizeof(MSG_HEAD_T);
    temp_ptr = (uint16*)dest_temp_ptr;
    *temp_ptr++ = sensor_id;

    if(1 != _CheckSensor(sensor_id))
    {          
        *temp_ptr = TOOL_SENSOR_OP_FAIL_SENSOR_ID;//fail        
        return TOOL_SENSOR_OP_HANDLED;
    } 

    *temp_ptr = TOOL_SENSOR_OP_SUCCESS;

    temp = s_sensor_table_ptr->hw_signal_polarity;
    temp &= 0xFFFB;//clear vysnc
    temp |= src_temp_ptr[SENSOR_VSYNC_OFFSET]&0x0004;//modify vsyn
    
    s_sensor_table_ptr->hw_signal_polarity = temp;

    return TOOL_SENSOR_OP_HANDLED;
}
/*****************************************************************************/
//Description:  read the polarity of hsync 
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL uint32 _ReadHysnc( 
                              uint8 **dest_ptr,       // Pointer of the response message.
                              uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
                              const uint8 *src_ptr,   // Pointer of the input message. 
                              uint16 src_len          // Size of the source buffer in uint8.
                            )
{
	uint16  len;                    
    uint16 *src_temp_ptr = PNULL;  
    uint8  *dest_temp_ptr = PNULL;
    uint16 *temp_ptr = PNULL;   
    SENSOR_ID_E sensor_id = SENSOR_MAIN;  
          
    src_temp_ptr = (uint16*)(src_ptr+sizeof(MSG_HEAD_T));
    
    sensor_id = (SENSOR_ID_E)src_temp_ptr[SENSOR_ID_OFFSET];

    len = sizeof(MSG_HEAD_T)+MSG_UNIT_SIZE+2*MSG_UNIT_SIZE; 
    dest_temp_ptr = (uint8*)SCI_ALLOCA(len);
    if(!dest_temp_ptr)
    {
        return TOOL_SENSOR_OP_HANDLED;
    }
    *dest_ptr = dest_temp_ptr;
    *dest_len_ptr = len;    

    SCI_MEMCPY(dest_temp_ptr,src_ptr,sizeof(MSG_HEAD_T));
    ((MSG_HEAD_T*)dest_temp_ptr)->len = len;
    dest_temp_ptr += sizeof(MSG_HEAD_T);
    temp_ptr = (uint16*)dest_temp_ptr;
    *temp_ptr++ = sensor_id;

    if(1 != _CheckSensor(sensor_id))
    {          
        *temp_ptr = TOOL_SENSOR_OP_FAIL_SENSOR_ID;//fail        
        return TOOL_SENSOR_OP_HANDLED;
    } 

    *temp_ptr++ = TOOL_SENSOR_OP_SUCCESS;

    *temp_ptr = s_sensor_table_ptr->hw_signal_polarity&0x0010;//bit4->hsync

    return TOOL_SENSOR_OP_HANDLED;

}
/*****************************************************************************/
//Description:  write the polarity of hsync 
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL uint32 _WriteHysnc( 
                              uint8 **dest_ptr,       // Pointer of the response message.
                              uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
                              const uint8 *src_ptr,   // Pointer of the input message. 
                              uint16 src_len          // Size of the source buffer in uint8.
                            )
{
	uint16  len;                    
    uint16 *src_temp_ptr = PNULL;  
    uint8  *dest_temp_ptr = PNULL;
    uint16 *temp_ptr = PNULL;     
    uint32  temp = 0;
    SENSOR_ID_E sensor_id = SENSOR_MAIN;  
          
    src_temp_ptr = (uint16*)(src_ptr+sizeof(MSG_HEAD_T));
    
    sensor_id = (SENSOR_ID_E)src_temp_ptr[SENSOR_ID_OFFSET];

    len = sizeof(MSG_HEAD_T)+SENSOR_WRITE_OP_DATA_LEN; 
    dest_temp_ptr = (uint8*)SCI_ALLOCA(len);
    if(!dest_temp_ptr)
    {
        return TOOL_SENSOR_OP_HANDLED;
    }
    *dest_ptr = dest_temp_ptr;
    *dest_len_ptr = len;    

    SCI_MEMCPY(dest_temp_ptr,src_ptr,sizeof(MSG_HEAD_T));
    ((MSG_HEAD_T*)dest_temp_ptr)->len = len;
    dest_temp_ptr += sizeof(MSG_HEAD_T);
    temp_ptr = (uint16*)dest_temp_ptr;
    *temp_ptr++ = sensor_id;

    if(1 != _CheckSensor(sensor_id))
    {          
        *temp_ptr = TOOL_SENSOR_OP_FAIL_SENSOR_ID;//fail        
        return TOOL_SENSOR_OP_HANDLED;
    } 

    *temp_ptr = TOOL_SENSOR_OP_SUCCESS;

    temp = s_sensor_table_ptr->hw_signal_polarity;
    temp &= 0xFFEF;//clear hysnc
    temp |= src_temp_ptr[SENSOR_HSYNC_OFFSET]&0x0010;//modify hsyn
    
    s_sensor_table_ptr->hw_signal_polarity = temp;

    return TOOL_SENSOR_OP_HANDLED;
}
/*****************************************************************************/
//Description:  get sum of sensor 
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL uint32 _ReadSensorSum( 
                                   uint8 **dest_ptr,       // Pointer of the response message.
                                   uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
                                   const uint8 *src_ptr,   // Pointer of the input message. 
                                   uint16 src_len          // Size of the source buffer in uint8.
                                  )
{
	uint16  len;                    
    uint16 *src_temp_ptr = PNULL;  
    uint8  *dest_temp_ptr = PNULL;
    uint16 *temp_ptr = PNULL;   
    uint16 sensor_sum = 0;
    SENSOR_INFO_T** sensor_info_tab_ptr = PNULL;

    sensor_info_tab_ptr=(SENSOR_INFO_T**)Sensor_GetInforTab(SENSOR_MAIN);
    if(sensor_info_tab_ptr)
    {
        sensor_sum++;
    }

    sensor_info_tab_ptr=(SENSOR_INFO_T**)Sensor_GetInforTab(SENSOR_SUB);
    if(sensor_info_tab_ptr)
    {
        sensor_sum++;
    }
          
    src_temp_ptr = (uint16*)(src_ptr+sizeof(MSG_HEAD_T));    

    len = sizeof(MSG_HEAD_T)+2*MSG_UNIT_SIZE; 
    dest_temp_ptr = (uint8*)SCI_ALLOCA(len);
    if(!dest_temp_ptr)
    {
        return TOOL_SENSOR_OP_HANDLED;
    }
    *dest_ptr = dest_temp_ptr;
    *dest_len_ptr = len;    

    SCI_MEMCPY(dest_temp_ptr,src_ptr,sizeof(MSG_HEAD_T));
    ((MSG_HEAD_T*)dest_temp_ptr)->len = len;
    dest_temp_ptr += sizeof(MSG_HEAD_T);
    temp_ptr = (uint16*)dest_temp_ptr;
    *temp_ptr++ = TOOL_SENSOR_OP_SUCCESS;
    *temp_ptr = sensor_sum;

    return TOOL_SENSOR_OP_HANDLED;    
}

LOCAL uint32 SensorTool_Init(void)
{
    if(SCI_TRUE != DCAMERA_IsOpen())
    {
        //SCI_TRACE_LOW:"[sensor_prod.c:SensorTool_Init ERROR!]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_PROD_743_112_2_18_0_26_21_27,(uint8*)"");
        return 0;
    }  

    DISPLAY_IsPermit(MAIN_LCD_ID,SCI_TRUE);

    return 1;
    
}
LOCAL uint32 _SensorTool_CmdRoutine( 
                                     uint8 **dest_ptr,       // Pointer of the response message.
                                     uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
                                     const uint8 *src_ptr,   // Pointer of the input message. 
                                     uint16 src_len          // Size of the source buffer in uint8.
                                    )
{
    uint32 ret = TOOL_SENSOR_OP_HANDLED;
    MSG_HEAD_T  *msg_head;                                   
	uint8   type;         
	TOOL_SENSOR_MSG_ID_E   subtype = TOOL_SENSOR_MSG_ID_MAX; 

    msg_head = (MSG_HEAD_T*)src_ptr;
    type = msg_head->type;

    //SCI_TRACE_LOW:"sensor_prod.c:_SensorTool_CmdRoutine:"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_PROD_767_112_2_18_0_26_21_28,(uint8*)"");

    if(DIAG_SENSOR_REG_F != type)
    {
        //SCI_TRACE_LOW:"[sensor_prod.c:_SensorTool_CmdRoutine:type error!,type = %d]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_PROD_771_112_2_18_0_26_21_29,(uint8*)"d",type);
        return ret;
    }
   
    if(!SensorTool_Init())
    {
        //SCI_TRACE_LOW:"[sensor_prod.c:_SensorTool_CmdRoutine:Init faile!]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_PROD_777_112_2_18_0_26_21_30,(uint8*)"");
        return ret;
    }
    
    subtype = (TOOL_SENSOR_MSG_ID_E)msg_head->subtype;

    switch(subtype)
    {
        case TOOL_SENSOR_READ_REG:
            ret = _ReadSensorReg(dest_ptr,dest_len_ptr,src_ptr,src_len);
            break;
        case TOOL_SENSOR_WRITE_REG:
            ret = _WriteSensorReg(dest_ptr,dest_len_ptr,src_ptr,src_len);
            break;
        case TOOL_SENSOR_READ_RESOLUTION_TABLE_SUM:
            ret = _GetSensorResolutionTabSum(dest_ptr,dest_len_ptr,src_ptr,src_len);
            break;
        case TOOL_SENSOR_READ_RESOLUTION_TABLE:
            ret = _ReadSensorResolutionTab(dest_ptr,dest_len_ptr,src_ptr,src_len);
            break;
        case TOOL_SENSOR_WRITE_RESOLUTION_TABLE:
            ret = _WriteSensorResolutionTab(dest_ptr,dest_len_ptr,src_ptr,src_len);
            break;
        case TOOL_SENSOR_READ_VSYNC:
            ret = _ReadVysnc(dest_ptr,dest_len_ptr,src_ptr,src_len);
            break;
        case TOOL_SENSOR_WRITE_VSYNC:
            ret = _WriteVysnc(dest_ptr,dest_len_ptr,src_ptr,src_len);
            break;
        case TOOL_SENSOR_READ_HSYNC:
            ret = _ReadHysnc(dest_ptr,dest_len_ptr,src_ptr,src_len);
            break;
        case TOOL_SENSOR_WRITE_HSYNC:
            ret = _WriteHysnc(dest_ptr,dest_len_ptr,src_ptr,src_len);
            break;    
        case TOOL_SENSOR_GET_SENSOR_SUM:
            //SCI_TRACE_LOW:"[sensor_prod.c:_SensorTool_CmdRoutine:TOOL_SENSOR_GET_SENSOR_SUM]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_PROD_813_112_2_18_0_26_21_31,(uint8*)"");
            ret = _ReadSensorSum(dest_ptr,dest_len_ptr,src_ptr,src_len);
            break;
        default:
            //SCI_TRACE_LOW:"[sensor_prod.c:_SensorTool_CmdRoutine subtype error,subtype=%d]"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_PROD_817_112_2_18_0_26_21_32,(uint8*)"d",subtype);
            break;
    }
    return ret;
}

/**--------------------------------------------------------------------------*
 **                         PUBLIC INTERFACE                                  *
 **--------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description:    This function is register diag command routine
//  Author:         Jianping.wang
//  Note:           
/*****************************************************************************/
PUBLIC void Sensor_RegisterCmdRoutine (void)
{    	
    DIAG_RegisterCmdRoutine(DIAG_SENSOR_REG_F, _SensorTool_CmdRoutine);
}

#if 0 //for test
LOCAL SENSOR_ID_E s_test_sensor_id = SENSOR_MAIN;
LOCAL uint16 s_reg_value = 0;
LOCAL uint16 s_addr = 0x300e;
PUBLIC void SENSORPROD_test(uint32 test_id)
{
    TOOL_SENSOR_MSG_ID_E subtype_id = TOOL_SENSOR_READ_REG;   
    uint8 *output_msg_ptr = PNULL;
    uint16 output_len = 0;
    MSG_HEAD_T *msg_head       = SCI_NULL;
    uint16	size = 0;
    uint8 *msg_src = PNULL;
    subtype_id = (TOOL_SENSOR_MSG_ID_E)test_id;
    switch(subtype_id)
    {
        case TOOL_SENSOR_READ_REG :
            //_SensorTool_CmdRoutine(&output_msg_ptr,&output_len,(const uint8 *)msg_head,size);
            s_reg_value = Sensor_ReadReg(s_addr);
            Sensor_WriteReg(s_addr, s_reg_value+1);
            s_reg_value = Sensor_ReadReg(s_addr);
            break;
        case TOOL_SENSOR_WRITE_REG:

            break;
        case TOOL_SENSOR_READ_RESOLUTION_TABLE_SUM:
            {
                uint16 *temp_ptr = PNULL;
                
                msg_src = SCI_ALLOCA(10);
                if(!msg_src)
                {
                    return;
                }                
                msg_head = (MSG_HEAD_T*)msg_src;
                msg_head->type = DIAG_SENSOR_REG_F;
                msg_head->subtype = TOOL_SENSOR_READ_RESOLUTION_TABLE_SUM;
                msg_head->len = 10;
                temp_ptr = (uint16*)(msg_src+sizeof(MSG_HEAD_T));
                *temp_ptr = SENSOR_MAIN;
                _SensorTool_CmdRoutine(&output_msg_ptr,&output_len,(const uint8 *)msg_head,size);

                if(output_msg_ptr)
                {
                    SCI_FREE(output_msg_ptr);
                    output_msg_ptr = PNULL;
                }
            }
            break;
        case TOOL_SENSOR_READ_RESOLUTION_TABLE:
            {
                uint16 *temp_ptr = PNULL;
                
                msg_src = SCI_ALLOCA(12);
                if(!msg_src)
                {
                    return;
                }                
                msg_head = (MSG_HEAD_T*)msg_src;
                msg_head->type = DIAG_SENSOR_REG_F;
                msg_head->subtype = TOOL_SENSOR_READ_RESOLUTION_TABLE;
                msg_head->len = 12;
                temp_ptr = (uint16*)(msg_src+sizeof(MSG_HEAD_T));
                *temp_ptr++ = (uint16)s_test_sensor_id;
                *temp_ptr = 1;
                _SensorTool_CmdRoutine(&output_msg_ptr,&output_len,(const uint8 *)msg_head,size);

                if(output_msg_ptr)
                {
                    SCI_FREE(output_msg_ptr);
                    output_msg_ptr = PNULL;
                }
                SCI_FREE(msg_src);
            }
            break;
        case TOOL_SENSOR_WRITE_RESOLUTION_TABLE:
            {
                uint16 *temp_ptr = PNULL;
                uint32 i = 0;
                uint32 j = 0;
                
                msg_src = SCI_ALLOCA(42);
                if(!msg_src)
                {
                    return;
                }                
                msg_head = (MSG_HEAD_T*)msg_src;
                msg_head->type = DIAG_SENSOR_REG_F;
                msg_head->subtype = TOOL_SENSOR_WRITE_RESOLUTION_TABLE;
                msg_head->len = 42;
                temp_ptr = (uint16*)(msg_src+sizeof(MSG_HEAD_T));
                *temp_ptr++ = (uint16)s_test_sensor_id;
                *temp_ptr++ = 1;
                *temp_ptr++ = 5;
                for( i=0 ; i<5 ; i++)
                {
                    *temp_ptr++ = j+0x0001;
                    *temp_ptr++ = j+0x0002;
                    j += 2;
                }
                *temp_ptr++ = 240;
                *temp_ptr++ = 320;
                *temp_ptr++ = 36;
                *temp_ptr++ = 7;
                _SensorTool_CmdRoutine(&output_msg_ptr,&output_len,(const uint8 *)msg_head,size);

                if(output_msg_ptr)
                {
                    SCI_FREE(output_msg_ptr);
                    output_msg_ptr = PNULL;
                }
                SCI_FREE(msg_src);
            }
            break;
        case TOOL_SENSOR_READ_VSYNC:

            break;
        case TOOL_SENSOR_WRITE_VSYNC:

            break;
        case TOOL_SENSOR_READ_HSYNC:

            break;
        case TOOL_SENSOR_WRITE_HSYNC:

            break;
        default:
            break;
        
    }
}
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

/* end of sensor_prod.c*/
 
