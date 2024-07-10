/******************************************************************************
 ** File Name:      lzma_test.c                                             *
 ** Author:         changde                                                   *
 ** DATE:           09/09/2011                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    define trace interface just for testing usage             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/09/2010     changde          Create                                    *
 ******************************************************************************/
#include "chip_drv_trc.h"
#include "arm_reg.h"
#include "os_api.h"
#include "uart_drv.h"
#include "tb_comm.h"
#include "sc6530_reg_base.h"
#include "sc6530_reg_global.h"
#include "sc6530_reg_lzma.h"

#include "lzma_drv.h"

/**---------------------------------------------------------------------------*
 **                         Protocol Define                                   *
 **---------------------------------------------------------------------------*/
typedef struct lzma_cfg_t
{
    uint32 src_addr;
	uint32 src_len;
	uint32 dst_addr;
	uint32 dst_len;

	uint32 buf_addr;
	uint32 buf_len;
	uint32 buf_start_len;
} LZMA_CFG_T, *LZMA_CFG_PTR;
	
/**---------------------------------------------------------------------------*
 **                         Macro Define                                      *
 **---------------------------------------------------------------------------*/
#define TEST_ADDR_START(x)  (0x34000000+x)

#define TEST_SRC_ADDR    TEST_ADDR_START(0x100000)  
#define TEST_SRC_LEN     0x100000   
#define TEST_DST_ADDR    TEST_ADDR_START(0x200000)
#define TEST_DST_LEN     0x100000
#define TEST_BUF_ADDR    TEST_ADDR_START(0x300000)
#define TEST_DST_LEN     0x100000

#define TEST_CHECK_ADDR    TEST_ADDR_START(0x400000)
#define TEST_CHECK_LEN     0x100000

extern uint32 g_int_cnt[8];
/**---------------------------------------------------------------------------*
 **                         Local Define                                      *
 **---------------------------------------------------------------------------*/
// lzma test config pattern
LOCAL LZMA_CFG_T _lzma_cfg_table[] =
{
    /*
    {TEST_SRC_ADDR, 1024, TEST_DST_ADDR, 1024, TEST_BUF_ADDR, 1024, 290},
    {TEST_SRC_ADDR, 1024, TEST_DST_ADDR, 1024, TEST_BUF_ADDR, 1024, 291},
    {TEST_SRC_ADDR, 1024, TEST_DST_ADDR, 1024, TEST_BUF_ADDR, 1024, 292},
    {TEST_SRC_ADDR, 1024, TEST_DST_ADDR, 1024, TEST_BUF_ADDR, 1024, 293},
 

    {TEST_SRC_ADDR, (4096+1), TEST_DST_ADDR, (4096+1), TEST_BUF_ADDR, (4096+1), 0},
    {TEST_SRC_ADDR, (4096+1), TEST_DST_ADDR, (4096+1), TEST_BUF_ADDR, (4096+1), 2900},
    {TEST_SRC_ADDR, (4096+1), TEST_DST_ADDR, (4096+1), TEST_BUF_ADDR, (4096+1), 2901},
    {TEST_SRC_ADDR, (4096+1), TEST_DST_ADDR, (4096+1), TEST_BUF_ADDR, (4096+1), 2902},
    {TEST_SRC_ADDR, (4096+1), TEST_DST_ADDR, (4096+1), TEST_BUF_ADDR, (4096+1), 2903},

    {TEST_SRC_ADDR, (4096+2), TEST_DST_ADDR, (4096+2), TEST_BUF_ADDR, (4096+2), 0},    
    {TEST_SRC_ADDR, (4096+2), TEST_DST_ADDR, (4096+2), TEST_BUF_ADDR, (4096+2), 2900},
    {TEST_SRC_ADDR, (4096+2), TEST_DST_ADDR, (4096+2), TEST_BUF_ADDR, (4096+2), 2901},
    {TEST_SRC_ADDR, (4096+2), TEST_DST_ADDR, (4096+2), TEST_BUF_ADDR, (4096+2), 2902},
    {TEST_SRC_ADDR, (4096+2), TEST_DST_ADDR, (4096+2), TEST_BUF_ADDR, (4096+2), 2903},
  
    {TEST_SRC_ADDR, (4096+3), TEST_DST_ADDR, (4096+3), TEST_BUF_ADDR, (4096+3), 0},    
    {TEST_SRC_ADDR, (4096+3), TEST_DST_ADDR, (4096+3), TEST_BUF_ADDR, (4096+3), 2900},
    {TEST_SRC_ADDR, (4096+3), TEST_DST_ADDR, (4096+3), TEST_BUF_ADDR, (4096+3), 2901},
    {TEST_SRC_ADDR, (4096+3), TEST_DST_ADDR, (4096+3), TEST_BUF_ADDR, (4096+3), 2902},
    {TEST_SRC_ADDR, (4096+3), TEST_DST_ADDR, (4096+3), TEST_BUF_ADDR, (4096+3), 2903},
*/
    {TEST_SRC_ADDR, (4096+4), TEST_DST_ADDR, (4096+4), TEST_BUF_ADDR, (4096+4), 0},    
    {TEST_SRC_ADDR, (4096+4), TEST_DST_ADDR, (4096+4), TEST_BUF_ADDR, (4096+4), 2900},
    {TEST_SRC_ADDR, (4096+4), TEST_DST_ADDR, (4096+4), TEST_BUF_ADDR, (4096+4), 2901},
    {TEST_SRC_ADDR, (4096+4), TEST_DST_ADDR, (4096+4), TEST_BUF_ADDR, (4096+4), 2902},
    {TEST_SRC_ADDR, (4096+4), TEST_DST_ADDR, (4096+4), TEST_BUF_ADDR, (4096+4), 2903},    
 /*
    {TEST_SRC_ADDR, (1088), TEST_DST_ADDR, (1088), TEST_BUF_ADDR, (1088), 0},    
    {TEST_SRC_ADDR, (1088), TEST_DST_ADDR, (1088), TEST_BUF_ADDR, (1088), 990},
    {TEST_SRC_ADDR, (1088), TEST_DST_ADDR, (1088), TEST_BUF_ADDR, (1088), 991},
    {TEST_SRC_ADDR, (1088), TEST_DST_ADDR, (1088), TEST_BUF_ADDR, (1088), 992},
    {TEST_SRC_ADDR, (1088), TEST_DST_ADDR, (1088), TEST_BUF_ADDR, (1088), 993},
*/
};
 
/**---------------------------------------------------------------------------*
 **                         ADI test case  depend on adi_drv.c                *
 **---------------------------------------------------------------------------*/
void _lzma_int_finish_callback( void )
{
    //SCI_TraceLow:"_lzma_int_finish_callback is called."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_TEST_101_112_2_17_23_5_17_1060,(uint8*)"");
}


/***************************************************************************** 
 *  Description:  check mem byte by byte
 *  Author:                                                                  * 
 *  Note  :     the unit of len is byte                                      * 
******************************************************************************/
PUBLIC uint32 LZMA_CheckMem(uint32 src_addr, uint32 dst_addr, uint32 len)
{
    uint32 i=0;
    uint8 *src_pchar= (uint8*)(src_addr);
    uint8 *dst_pchar= (uint8*)(dst_addr);
    
    for(i=0; i<len; i++)
    {
        if(*src_pchar != *dst_pchar)
        {
	        //SCI_TraceLow:"LZMA_CheckMem:-->src_addr[0x%x],dst_addr[0x%x]"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_TEST_119_112_2_17_23_5_17_1061,(uint8*)"dd", src_pchar,dst_pchar);
            while(1);
            //return SCI_FALSE;
        }
        src_pchar++;
        dst_pchar++;
    }

    return SCI_TRUE;
}

void LZMA_IntstsTest()
{
    uint32 table_size = sizeof(_lzma_cfg_table)/sizeof(_lzma_cfg_table[0]);
    LZMA_CFG_PTR lzma_cfg_ptr = _lzma_cfg_table;
	LZMA_Init();
    LZMA_RegCallback(LZMA_INT_FINISH, _lzma_int_finish_callback);
    
    //enable the interrupt
    CHIP_REG_OR(LZMA_INT, (LZMA_INTEN_BUF_LEN_ERR_BIT| 
                           LZMA_INTEN_DEC_ERR_BIT    |
                           LZMA_INTEN_DST_LEN_ERR_BIT|
                           LZMA_INTEN_FINISH_BIT     |
                           LZMA_INTEN_MST_ERR_BIT   ));
    
	ISR_RegHandler(TB_MIDI_FM_ZIP_INT, LZMA_IsrHandler);
	TB_EnableINT(TB_MIDI_FM_ZIP_INT);

	//SCI_TraceLow:"LZMA_IntstsTest:--------------->start"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_TEST_147_112_2_17_23_5_17_1062,(uint8*)"");
    /*
    ------------case0: BUF_LEN_ERR error test ---------------------------------
    */
    SCI_MEMSET(lzma_cfg_ptr->dst_addr, 0, lzma_cfg_ptr->dst_len);
    LZMA_SetSrcCfg(lzma_cfg_ptr->src_addr, lzma_cfg_ptr->src_len);
    LZMA_SetDstCfg(lzma_cfg_ptr->dst_addr, lzma_cfg_ptr->dst_len); 
    LZMA_SetBufCfg(lzma_cfg_ptr->buf_addr, 100);   //BUF_LEN_ERR error
    LZMA_SetBufStart(0);
    
    _LZMA_BufferEn(SCI_TRUE);
    _LZMA_BufferFull(SCI_TRUE);

    LZMA_StartRun();
    
    //check if the int has been occured
    while( !(g_int_cnt[LZMA_INT_FINISH]) )
    {}
    if(g_int_cnt[LZMA_INT_FINISH] == 1)
        g_int_cnt[LZMA_INT_FINISH]=0;
    else
        //SCI_TraceLow:"LZMA_IntstsTest:-->ERROR,LZMA_INT_FINISH"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_TEST_168_112_2_17_23_5_17_1063,(uint8*)"");
    
    if(!g_int_cnt[LZMA_INT_BUF_LEN_ERR])
        //SCI_TraceLow:"LZMA_IntstsTest:-->ERROR,LZMA_INT_BUF_LEN_ERR"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_TEST_171_112_2_17_23_5_17_1064,(uint8*)"");
    else
        g_int_cnt[LZMA_INT_BUF_LEN_ERR]=0;

    /*
    ------------case1: DST_LEN_ERR error test ---------------------------------
    */
    SCI_MEMSET(lzma_cfg_ptr->dst_addr, 0, lzma_cfg_ptr->dst_len);
    LZMA_SetSrcCfg(lzma_cfg_ptr->src_addr, lzma_cfg_ptr->src_len);
    LZMA_SetDstCfg(lzma_cfg_ptr->dst_addr, 100);    //DST_LEN_ERR error
    LZMA_SetBufCfg(lzma_cfg_ptr->buf_addr, lzma_cfg_ptr->buf_len);   
    LZMA_SetBufStart(0);

    _LZMA_BufferEn(SCI_FALSE);
    _LZMA_BufferFull(SCI_FALSE);
    
    LZMA_StartRun();

    //check if the int has been occured
    while( !(g_int_cnt[LZMA_INT_FINISH]) )
    {}
    if(g_int_cnt[LZMA_INT_FINISH] == 1)
        g_int_cnt[LZMA_INT_FINISH]=0;
    else
        //SCI_TraceLow:"LZMA_IntstsTest:-->ERROR,LZMA_INT_FINISH"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_TEST_195_112_2_17_23_5_18_1065,(uint8*)"");
    
    if(!g_int_cnt[LZMA_INT_DST_LEN_ERR])
        //SCI_TraceLow:"LZMA_IntstsTest:-->ERROR,LZMA_INT_DST_LEN_ERR"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_TEST_198_112_2_17_23_5_18_1066,(uint8*)"");
    else
        g_int_cnt[LZMA_INT_DST_LEN_ERR]=0; 

    /*
    ------------case2: LZMA_INT_DEC_ERR error test ----------------------------
    */
    SCI_MEMSET(lzma_cfg_ptr->dst_addr, 0, lzma_cfg_ptr->dst_len);
    LZMA_SetSrcCfg(lzma_cfg_ptr->src_addr, lzma_cfg_ptr->src_len);
    LZMA_SetDstCfg(lzma_cfg_ptr->dst_addr, lzma_cfg_ptr->dst_len); 
    LZMA_SetBufCfg(lzma_cfg_ptr->buf_addr, lzma_cfg_ptr->buf_len);   
    LZMA_SetBufStart(0);

    LZMA_StartRun();
    
    //check if the int has been occured
    while( !(g_int_cnt[LZMA_INT_FINISH]) )
    {}
    if(g_int_cnt[LZMA_INT_FINISH] == 1)
        g_int_cnt[LZMA_INT_FINISH]=0;
    else
        //SCI_TraceLow:"LZMA_IntstsTest:-->ERROR,LZMA_INT_FINISH"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_TEST_219_112_2_17_23_5_18_1067,(uint8*)"");
    
    if(!g_int_cnt[LZMA_INT_DEC_ERR])
        //SCI_TraceLow:"LZMA_IntstsTest:-->ERROR,LZMA_INT_DEC_ERR"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_TEST_222_112_2_17_23_5_18_1068,(uint8*)"");
    else
        g_int_cnt[LZMA_INT_DEC_ERR]=0;  
    
	//SCI_TraceLow:"LZMA_IntstsTest:--------------->end"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_TEST_226_112_2_17_23_5_18_1069,(uint8*)"");
}

/*
CASE:   1--enable 
        0--disable 
        -1--defaultvalue
*/
PUBLIC void LZMA_PollingTest( int32 buf_en, int32 buf_full_en )
{
    uint32 i=0;
    uint32 table_size = sizeof(_lzma_cfg_table)/sizeof(_lzma_cfg_table[0]);
    LZMA_CFG_PTR lzma_cfg_ptr = _lzma_cfg_table;
    
	//SCI_TraceLow:"LZMA_PollingTest:-->buf_en=%d, buf_full_en=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_TEST_239_112_2_17_23_5_18_1070,(uint8*)"dd", buf_en,buf_full_en);

    for(i=0; i<table_size; i++)
    {
    	LZMA_Init();
        SCI_MEMSET(lzma_cfg_ptr->dst_addr, 0, lzma_cfg_ptr->dst_len);

        LZMA_SetSrcCfg(lzma_cfg_ptr->src_addr, lzma_cfg_ptr->src_len);
        LZMA_SetDstCfg(lzma_cfg_ptr->dst_addr, lzma_cfg_ptr->dst_len); 
        LZMA_SetBufCfg(lzma_cfg_ptr->buf_addr, lzma_cfg_ptr->buf_len);
        LZMA_SetBufStart(lzma_cfg_ptr->buf_start_len);

        if(buf_en >= 0)
        {
        	if(buf_en)
        		_LZMA_BufferEn(SCI_TRUE);
			else
        		_LZMA_BufferEn(SCI_FALSE);
        }
        if(buf_full_en >= 0)
        {
        	if(buf_full_en)
        		_LZMA_BufferFull(SCI_TRUE);
			else
        		_LZMA_BufferFull(SCI_FALSE);
        }
		
        LZMA_StartRun();

        LZMA_WaitingFinish( -1 ); //wait forever
        
        if(!buf_en)
            LZMA_CheckMem( lzma_cfg_ptr->dst_addr, TEST_CHECK_ADDR, lzma_cfg_ptr->dst_len);
        else
            LZMA_CheckMem( lzma_cfg_ptr->dst_addr, TEST_CHECK_ADDR+lzma_cfg_ptr->buf_start_len, lzma_cfg_ptr->dst_len-lzma_cfg_ptr->buf_start_len);            
        
        //SCI_TraceLow:"--------->Case[%d] succuss."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_TEST_275_112_2_17_23_5_18_1071,(uint8*)"d", i);
        lzma_cfg_ptr++;
    }
}

/*
CASE:   1--enable 
        0--disable 
        -1--defaultvalue
*/
PUBLIC void LZMA_IntTest( int32 buf_en, int32 buf_full_en )
{
    uint32 i=0;
    uint32 table_size = sizeof(_lzma_cfg_table)/sizeof(_lzma_cfg_table[0]);
    LZMA_CFG_PTR lzma_cfg_ptr = _lzma_cfg_table;
    
	//SCI_TraceLow:"LZMA_IntTest:-->buf_en=%d, buf_full_en=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_TEST_290_112_2_17_23_5_18_1072,(uint8*)"dd", buf_en,buf_full_en);

    for(i=0; i<table_size; i++)
    {
    	LZMA_Init();
        LZMA_RegCallback(LZMA_INT_FINISH, _lzma_int_finish_callback);
        SCI_MEMSET(lzma_cfg_ptr->dst_addr, 0, lzma_cfg_ptr->dst_len);
        
        //enable the interrupt
        CHIP_REG_OR(LZMA_INT, (LZMA_INTEN_BUF_LEN_ERR_BIT| 
                               LZMA_INTEN_DEC_ERR_BIT    |
                               LZMA_INTEN_DST_LEN_ERR_BIT|
                               LZMA_INTEN_FINISH_BIT     |
                               LZMA_INTEN_MST_ERR_BIT   ));
        
		ISR_RegHandler(TB_MIDI_FM_ZIP_INT, LZMA_IsrHandler);
		TB_EnableINT(TB_MIDI_FM_ZIP_INT);

        LZMA_SetSrcCfg(lzma_cfg_ptr->src_addr, lzma_cfg_ptr->src_len);
        LZMA_SetDstCfg(lzma_cfg_ptr->dst_addr, lzma_cfg_ptr->dst_len); 
        LZMA_SetBufCfg(lzma_cfg_ptr->buf_addr, lzma_cfg_ptr->buf_len);
        LZMA_SetBufStart(lzma_cfg_ptr->buf_start_len);

        if(buf_en >= 0)
        {
        	if(buf_en)
        		_LZMA_BufferEn(SCI_TRUE);
			else
        		_LZMA_BufferEn(SCI_FALSE);
        }
        if(buf_full_en >= 0)
        {
        	if(buf_full_en)
        		_LZMA_BufferFull(SCI_TRUE);
			else
        		_LZMA_BufferFull(SCI_FALSE);
        }
		
        LZMA_StartRun();

        //check if the int has been occured
        while( !(g_int_cnt[LZMA_INT_FINISH]) )
        {}
        if(g_int_cnt[LZMA_INT_FINISH] == 1)
            g_int_cnt[LZMA_INT_FINISH]=0;
        else
            //SCI_TraceLow:"LZMA_Case0Test:-->pattern[%d] is error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_TEST_336_112_2_17_23_5_18_1073,(uint8*)"d", i);

        if(!buf_en)
            LZMA_CheckMem( lzma_cfg_ptr->dst_addr, TEST_CHECK_ADDR, lzma_cfg_ptr->dst_len);
        else
            LZMA_CheckMem( lzma_cfg_ptr->dst_addr, TEST_CHECK_ADDR+lzma_cfg_ptr->buf_start_len, lzma_cfg_ptr->dst_len-lzma_cfg_ptr->buf_start_len);            
        
        //SCI_TraceLow:"--------->Case[%d] succuss."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_TEST_343_112_2_17_23_5_18_1074,(uint8*)"d", i);
        lzma_cfg_ptr++;        
    }
}

/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/

PUBLIC uint32 test_entry(void)
{ 
	uint32 i = 0;

    LZMA_IntstsTest  ();
    
	{
		//CASE0:BUFFER_EN set to 0 :
    	LZMA_PollingTest(0, -1);		
		//CASE1:BUFFER_EN set to 1, BUFFER_FULL_DEC set to 0
    	LZMA_PollingTest(1, 0);			
		//CASE2:BUFFER_EN set to 1, BUFFER_FULL_DEC set to 1 	
    	LZMA_PollingTest(1, 1);	

		//CASE0:BUFFER_EN set to 0 :
    	LZMA_IntTest(0, -1);		
		//CASE1:BUFFER_EN set to 1, BUFFER_FULL_DEC set to 0
    	LZMA_IntTest(1, 0);			
		//CASE2:BUFFER_EN set to 1, BUFFER_FULL_DEC set to 1 	
    	LZMA_IntTest(1, 1);	        
    	
		//SCI_TraceLow:"test entry"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,LZMA_TEST_372_112_2_17_23_5_18_1075,(uint8*)"");
		for(i = 0; i  < 10000; i++);
	}
	
    return TRUE;
}

