/****************************************************************************************
** File Name:		L1Macros.h															*	
** Author:			Harry QIN															*
** DATE:			11/20/2001															*
** CopyRight		2001 SpreadTrum Communications CORP. All Rights Reserved.			*
** Description:		This head file contains general types and macros that are			*
					of use to all modules. The values or definitions are dependent		*
					on the specified target, otherwise the default is for ARM targets.	*
*****************************************************************************************


*****************************************************************************************
**					Edit History														*	
** -------------------------------------------------------------------------------------*
**																						*
** DATE:			NAME				DESCIPTION										*
** 11/20/2001		Harry QIN			Create.											*
*****************************************************************************************

 **-------------------------------------------------------------------------------------*
 **                         Dependencies												*
 **-------------------------------------------------------------------------------------*/
 
#ifndef _L1Macros_H
#define _L1Macros_H

//add macro to compile SDL file
#ifdef _RTOS
	#include "sci_types.h"
#else
    #include  "..\..\..\..\RTOS\export\inc\sci_types.h"
#endif

//#include "sharedMem.h"
/**-------------------------------------------------------------------------------------*
 **                         const definition											*
 **-------------------------------------------------------------------------------------*/
#define SCELL_BCCH_TC_INIT_MAP    0x00ff // Periodically Need to Rx normal block on all TC, TC0: Bit0, ... TC7: Bit 7.

#define BCCH_TC_MAX  8
#define SAVED_TC_MAX 4						
// FN related constant
#define SUPER_FN_SIZE			            ( 26*51*2048)	// Super Frame Number 2715648
#define MAX_Sleep_Delay			            (1024-1)		// Maximum Sleep Delay
#define MAX_TICKS                           0xffffffff
#define FRAMES_PER_BLOCK                    4               //A block contains 4 frames
#define MAX_FRAME_SLEEP_NUM          (0X1FF)
#define MIN_FRAME_IDLE_NUM                  0x1

#ifdef GSM_CUSTOMER_AFP_SUPPORT
#define MAX_SLEEP_PERIOD_NUM_NULL           (0x4FF)
#endif

#define	SEARCH_NUMBER_BEFORECAMPED_2SIM			26



#define FRAMES_PER_TCHHR_BLOCK              2               //A Half Rate TCH Block contains 2 frames

#define	SEARCH_NUMBER_BEFORECAMPED			26   
#define	SEARCH_NUMBER_CAMPED				13   
#define SEARCH_MARGIN                       3               //2 frames between bsic search should be set. 
#define SLEEP_THRESHOLD			            3

#define TCH_PERIOD_104                      104
#define SDCCH_PERIOD_102                    102
#define ONE_OF_EIGHTH_TCH_PERIOD_13			13

#define FRAMEINT_ADVANCE_NUM                3     

#define FRAMEINT_ADVANCE_0                  0
#define FRAMEINT_ADVANCE_1                  1  
#define FRAMEINT_ADVANCE_2                  2 
#define FRAMEINT_ADVANCE_NUM_SDCCH          FRAMEINT_ADVANCE_2
#define FRAMEINT_ADVANCE_NUM_TCH            4
#define FRAMEINT_ADVANCE_NUM_RACHAGCH       3

#define MAX_SLEEPING_COUNT                  1000

#define INITIAL_FN		                	0               //the initial value for current_fn

#define CELL_SIZE				            32
#define CELL_ID_MASK_RESPONSE 0x1f00
#define INVALID_CELL				        0xFF

#define TCH_BSIC_SEARCH_TIMES		        13
#define INVALID_LAST_SENT_FN		        SUPER_FN_SIZE	

#ifndef WIN32
	#define GSM_MEAS_PERIOD_IDLE_MODE		        5000
	//modified by @ for cr64578,2006.12.29
	//report_period = ppch_period*n,n is the min integer satisfy report_period>=800ms
	//for example,if ppch_period=700ms,report_period=1400ms;if ppch_period=900ms,report_period=900ms
	#define GPRS_MEAS_PERIOD_IDLE_MODE		    	1000
	#define MIN_GPRS_NC_MEAS_REPORT_PERIOD   		480
#else
	//4.615ms equals to 100 tick,so 5000ms should corresponding with 5000*100/4.615,but we decrease for simulation.
	#define GSM_MEAS_PERIOD_IDLE_MODE		        100000
	#define GPRS_MEAS_PERIOD_IDLE_MODE		    	20000
	#define MIN_GPRS_NC_MEAS_REPORT_PERIOD      	9600
#endif	




//for cr98948
//changed from 5 to 1 for cr113488 by tom.fu
#define MAX_MEAS_COUNT_IN_JUST_CAMPED_STATE		    1

#define RESYN_COUNT_BY_GSM_MEAS				6
#define RESYN_COUNT_BY_GPRS_MEAS			30
#define SCELL_BCCH_DEC_PERIOD		        30000			//30 seconds
#define NCELL_BSIC_DEC_PERIOD		        30000			//30 SECONDS
//#define STOP_RPT_CELL_VALID_PERIOD          10000           //10 seconds

//#define NO_BCCH_DEC					        0xff
#define NO_RACH_SEND				        0x0

#define L1_FINISHED_MAP                     0xffff  //this pattern means all tc's normal and ext bcch 
											        //have been received.
#define L1_CONFIRM_BCCH_MAP                 0xff00  //0x7600 //this means l1 need to confirm some tc's bcch.
										            //all normal blocks and ext blocks when tc=0,3,7

									
#define MAX_DISTANCE			            0x7FFF   //this is the max value of uint16, just used 
#define INVALID_POLLING                     0xFFFF                                                     //for comparing the minimum distance for bcch delay		

#define MAX_BLOCKS_51MFRMS                  10
#define GSM_51MFRMS                         51
#define GPRS_MAX_TA_INDEX                   16

#define SNGL_BLCK_ALLO		 	            2
#define SNGL_BLCK_WTHOUT_TBF 	            3

#define	INVALID_ARFCN_INDEX		            0xffff

#define CS1_BLOCK_SIZE			            23
#define CS2_BLOCK_SIZE			            34
#define CS3_BLOCK_SIZE			            40
#define CS4_BLOCK_SIZE			            54

#define WAKEUPFRAMES            			3
#define PAGEMODEWAKEUPFRAMES    			2

#define MAX_DELAY_OF_STARTIMG_TIME          42432
#define MIN_ELAPSED_DELAY                   32024

#define INNVALID_PARAMETER                  0xff

#define INVALID_CBCH_DELAY                  0x7fff

//modified by @ for cr99471,
//change the default reporting period(not in NC1 or NC2) in gprs traffic from 3.84sec to 0.96sec
#define NC_REPORTING_PERIOD_T_DEFAULT        1

#define INTERFERENCE_MEASURE_LOCATION_1      25
#define INTERFERENCE_MEASURE_LOCATION_2      51

#define MAX_I_LEVEL                          15

//test point for loading 
#define LOADING_GEN_BCCHCAMPED    0xB001

#define BLOCKS_NUM_PER_SMSCB				 8  //added by penyuli 05/31/2007

/**-------------------------------------------------------------------------------------*
 **                        type  definition		    									*
 **-------------------------------------------------------------------------------------*/

typedef uint32					Fn_T;			
typedef uint32					SYSClock_T;		
typedef uint8					HSN_T;
typedef uint8					NCC_T;
typedef uint8					Ta_T;	
typedef uint8					CellID_T;
typedef	int16					OTD_T;
typedef int8                    DSC_T;

/**-------------------------------------------------------------------------------------*
 **                         Macros definition											*
 **-------------------------------------------------------------------------------------*/

#define	BSIC_ON_THIS_FRAME(FnMod51)	FnMod51 == 1 ||	FnMod51 == 11 ||  \
								    FnMod51 ==21 || FnMod51 == 31 ||  \
									FnMod51 ==41


#define Fn_Mod(Fn,Mod)			(Fn_T)((Fn) % (Mod))

#define Add_Fn_Mod(Fn,n,Mod)	\
		(Fn) =(Fn)+ (n);  \
		if ((Fn) >= (Mod)) (Fn) = (Fn)-(Mod)
//-@CY
#define Sub_Fn_Mod(Fn,n,Mod)  \
        ((Fn) =  ((Fn)>=(n))?((Fn)-(n)):( ((Fn)-(n)) + (Mod)) )   

#define Add_Short_Fn_Mod(Fn,n,Mod)  \
        ((Fn)= ( ( (Fn)+(n) )>=(Mod) )?((Fn)+(n)-(Mod)):((Fn)+(n)) )

#define Sub_Short_Fn_Mod(Fn,n,Mod)    \
		((Fn) =  ((Fn)>(n))?((Fn)-(n)):( ((Fn)-(n)) + (Mod)) )

#define	Fn_inc(fn)		(fn)++; \
		if(fn >= SUPER_FN_SIZE) fn = 0

#define	FrameIndex_inc(index)		(index)++; \
		if(index >= FRAME_INSTR_SIZE) index = 0
//Find the diffrence of two time
#define Diff_Time(Time1,Time2) \
        ((Time1)>=(Time2))?((Time1)-(Time2)):( (Time1)-(Time2) + (MAX_TICKS) )   					

//Find the diffrence of two fn, with Mod
#define Diff_Fn_Mod(Fn1,Fn2,Mod)  ( (Fn1)>=(Fn2))?((Fn1)-(Fn2)):( (Fn1)-(Fn2) + (Mod) )   		

#define Find_Delay0(current, Mod )	\
		((Mod)-((current)%(Mod)))

#define Find_Delay_Next( current, next, Mod )	\
		(((next)<=((current)%(Mod))) ? ((Mod)-((current)%(Mod))+next) : ((next)-((current%(Mod))))

#define Find_Diff_Mod( current, next, Mod )		\
		( ( ((next)<=(current)?(Mod):0) + (next) ) - (current) )

#define Find_Delay_value( d )	\
		(( d > 0 ) ? (uint8)( (((d) / MAX_Sleep_Delay) + 1) ) : 0)

#define Mod(x,y)   ((x)%(y))

//just get the low 2 bits
#define MOD4(_x)   ((_x)&0x03)    
        
#ifndef WIN32
	#ifndef MIN
	#define MIN(x,y)	( (x) < (y) ? (x) : (y) )
	#endif 

	#ifndef MAX
	#define MAX(x,y)	( (x) > (y) ? (x) : (y) )
	#endif 

#endif //WIN32

#define    LAYER1_LOG_STATE(L1_SUBSTATE_VALUE,L1_STATENAME_STRING)  \
                SCI_LOG_STATE(P_LAYER1, L1_SUBSTATE_VALUE, L1_STATENAME_STRING);

#define    LAYER1_NEXTSTATE(L1_STATE,L1_STATE_VALUE,L1_STATENAME_STRING,L1_STATE_ID)	\
			SCI_NEXTSTATE(L1_STATE, L1_STATE_VALUE, L1_STATENAME_STRING, L1_STATE_ID);\
			SCI_ASSERT(L1_STATE < L1_STATE_NUM);\
	        g_layer1_state = L1_STATE;		
			
#ifdef	_LOADING_TEST	
		#define	   LAYER1_LOADING_TEST(_TEST_PT,_TEST_VALUE);	\
		   		_TEST_PT		= _TEST_VALUE;
		   		
#else
		#define	   LAYER1_LOADING_TEST(_TEST_PT,_TEST_VALUE);		//nothing to do
			   		
#endif		

/*hyman  layer1 is top priority,so to void call null function.
 *       this only generate the compiler' warning
 */
#ifdef _DEBUG
	#define PHY_TRACE					SCI_TRACE_SLIGHT	
	#define LAYER1_TRACE_INF			SCI_TRACE_LOW
	#define LAYER1_TRACE_FATAL_ERROR	SCI_TRACE_ERROR
	#define LAYER1_TRACE_ERROR			SCI_TRACE_HIGH
    #define PHY_TRACE_CBCH              //SCI_TRACE_LOW
	#define LAYER1_MP3SBC_TRACE         SCI_TRACE_LOW	
	#define PHY_TRACE_DUALSIM           //SCI_TRACE_LOW
	#define LAYER1_EDGE_TRACE           SCI_TRACE_LOW

#else
	#define PHY_TRACE					SCI_TRACE_SLIGHT
	#define LAYER1_TRACE_INF			//SCI_TRACE_LOW
	#define LAYER1_TRACE_FATAL_ERROR	//SCI_TRACE_LOW
	#define LAYER1_TRACE_ERROR			//SCI_TRACE_LOW
	#define PHY_TRACE_CBCH              //SCI_TRACE_LOW
	#define PHY_TRACE_DUALSIM           //SCI_TRACE_LOW    
	#define LAYER1_MP3SBC_TRACE         SCI_TRACE_NULL
	#define LAYER1_EDGE_TRACE           SCI_TRACE_NULL
#endif

//@Lucian For PBCCH mode 
#define IS_BCCH_READ_MODE( _read_mode ) \
	(_read_mode == CON_BCCH || _read_mode == CON_BCCH_A_EXT \
       ||_read_mode == SINGLE_BCCH || _read_mode == SINGLE_EXT_BCCH ) 

#define IS_PBCCH_READ_MODE( _read_mode ) \
 	( _read_mode == PACKET_SYSINFO_1 || _read_mode == CON_PBCCH \
 	|| _read_mode == CON_PBCCH_WITH_LOW_REP_RATE ||_read_mode == CON_PBCCH_WITH_HIGH_REP_RATE )

//added by pengyuli 
#define LAYER1_DISABLE_IRQ \
			SCI_DisableIRQ();
#define LAYER1_ENABLE_IRQ \
			SCI_RestoreIRQ();
			
#define LAYER1_DISABLE_IRQFIQ \
						SCI_DisableIRQ();\
						SCI_DisableFIQ();
			
#define LAYER1_ENABLE_IRQFIQ \
						SCI_RestoreFIQ();\
						SCI_RestoreIRQ();
	
#endif   //end of file

