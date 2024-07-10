/*============================================================================*
 *
 *                          Copyright (c) 2002 Spreadtrum
 *                              All Rights Reserved
 *
 *-----------------------------------------------------------------------------
 *
 * File name:
 *      Dsp_logel_def.h
 *
 * Description:
 *      This header file defines the key information structs in DSP
 *
 * Note:
 *      DspKernel.dll and DspLogger.exe both use this file.
 *
 *============================================================================*/


/*============================================================================*
 *                          Edit History
 *-----------------------------------------------------------------------------
 * 2002-11-19   Steven Yao     Created
 *-----------------------------------------------------------------------------
   2002-11-20   Kai Liu     modify foreground and 
                            background task definitions
 *============================================================================*/


//foreground task definition:
/*******************************************************
//original definition
static const void (*fg_fun_task[15])() __attribute__((section(".DSECT const_data")))=
{

	(const void *) fg_dummy_task,            
	(const void *) fg_dummy_task,            
	(const void *) fg_dummy_task,            
	(const void *) run_tx_encoder_task,      
	(const void *) run_uart_fg_task,         
	(const void *) run_frame_eng_task,       
	(const void *) run_tx_interleaver_task,  
	(const void *) run_rx_task,              
	(const void *) run_rx_deinterleaver_task,
	(const void *) run_rx_dispatch_task,     
	(const void *) run_tx_multislot_task,    
	(const void *) run_mcu_to_dsp_task,      
	(const void *) run_timer_task,           
	(const void *) fg_dummy_task,            
	(const void *) fg_dummy_task             
};
********************************************************/
//new definition
struct fg_fun_task
{
    BYTE fg_dummy_task              : 2;
    BYTE run_timer_task             : 1;
    BYTE run_mcu_to_dsp_task        : 1;
    BYTE run_tx_multislot_task      : 1;
    BYTE run_rx_dispatch_task       : 1;
    BYTE run_rx_deinterleaver_task  : 1;
    BYTE run_rx_task                : 1;
    BYTE run_tx_interleaver_task    : 1;
    BYTE run_frame_eng_task         : 1;
    BYTE run_uart_fg_task           : 1;
    BYTE run_tx_encoder_task        : 1;
    BYTE fg_dummy_task1             : 4;
};

//background task definition:

/*******************************************************
//original definition
static const void (*bg_fun_task[15])() __attribute__((section(".DSECT const_data")))=
{
	(const void *) bg_dummy_task,			//  14
	(const void *) bg_dummy_task,			//  13
	(const void *) bg_dummy_task,			//  12
	(const void *) bg_dummy_task,			//  11
	(const void *) bg_dummy_task,			//  10
	(const void *) run_vocoder_dec_task,	//  9
	(const void *) run_vocoder_enc_task,	//  8
	(const void *) run_tone_bg_task,		//  7
	(const void *) bg_dummy_task,			//  6
	(const void *) bg_dummy_task,			//  5
	(const void *) bg_dummy_task,			//  4
	(const void *) bg_dummy_task,			//  3
	(const void *) run_uart_bg_task,		//  2
	(const void *) bg_dummy_task,			//  1
	(const void *) bg_dummy_task			//  0
};				   
********************************************************/
//new definition
struct bg_fun_task
{
    BYTE bg_dummy_task          : 2;
    BYTE run_uart_bg_task       : 1;
    BYTE bg_dummy_task1         : 4;
    BYTE run_tone_bg_task       : 1;
    BYTE run_vocoder_enc_task   : 1;
    BYTE run_vocoder_dec_task   : 1;
    BYTE bg_dummy_task2         : 6;
};

typedef unsigned _int16 uint16;
//frame instruction definition:
struct FrameInst_T
{
	uint16 RFOperation	: 5;  
	uint16 CellIndex	: 4;
	uint16 FrameType	: 7;
	
			// Offset 1		
	
	uint16 RxBurstNumber	: 2;	
	uint16 TxBurstNumber	: 2;
	uint16 RxChannelTag		: 4; //5
	uint16 fn1				: 8; //7
			
			// Offset 2	
	uint16 fn2;

			// Offset 3		
	union {
		struct { 
			uint16 Searching_count	  :12;
			uint16 TxSendBufferIndex  : 4;
		}Offset3_T1;
		
		struct {
			uint16   TxSlotMask		: 8;
			uint16	 FirstRxSlot	: 3;
			uint16	 Empty1			: 5;	//Reserved for the future use.
		} Offset3_T2;
	} Offset3_u;
	
			// Offset 4		
	uint16	 RxSlotMask		: 8;
	uint16	 TSC			: 4;			// Training Sequence Code
	uint16	 USFMode		: 1;		
	uint16	 TAF			: 1;
	uint16	 DTX			: 1;
	uint16	 EndMR			: 1;

				// Offset 5								
	uint16	 Empty2					: 6;	//Reserved for the future use.
	uint16	 TAUpdatedFlag			: 1;		
	uint16	 TAUpdatedIndex			: 2;
	uint16	 PowerUpdateFlag		: 1;
	uint16	 PowerUpdateIndex		: 2;
	uint16	 USFGranuarity			: 1;	//????
	uint16	 ExtDyn					: 1;	//????
	uint16	 ChModeUpdate			: 1;	//changed from 2 bits
	uint16	 USFValueUpdate			: 1;	//Changed from 4 bits
			
			// Offset 6
    uint16	 ARFCN0		: 10;
	uint16	 GainIndex0	: 6;
 


			// Offset 7
  	uint16	 ARFCN1		: 10 ;
	uint16	 GainIndex1	: 6;		
 
			// Offset 8
	uint16	 ARFCN2		: 10;
	uint16	 GainIndex2	: 6;

};

//MCU COMMAND definition:
struct CmdQ_T
{
	uint16 Command_Queue_ID;
	uint16 parameter0;
	uint16 parameter1;
	uint16 parameter2;
	uint16 parameter3;
} ;
