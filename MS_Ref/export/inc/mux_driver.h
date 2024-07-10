/******************************************************************************
 ** File Name:      mux_drive.h                                      *
 ** Author:         yayan.xu                                              *
 ** DATE:           07/10/2008                                              *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    LLI header info for MUX LLI module*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE                       NAME             DESCRIPTION                               *
 ** 07/10/2008     yayan.xu                 Create.                                   *
 ******************************************************************************/
#ifndef _MUX_DRIVE_H
#define _MUX_DRIVE_H
#include "sci_types.h"
#include "mux_api.h"
#include "mux_os.h"
#include "os_api.h"
#include "sio.h"

#ifdef   __cplusplus
extern   "C"
{
#endif

extern MUX_EVENT_GROUP_PTR   g_MuxRecv_Eptr ;

extern uint32     mux_recvtask_event;



#define MUX_GET_FREEBUF_SIZE(free_size, _buf)  \
do  \
{   \
    if (_buf->end_point >= _buf->start_point) \
    {\
        free_size = (int) ((int)_buf->size - _buf->end_point + _buf->start_point - 1);\
    }\
    else\
    {\
        free_size = (int) (_buf->start_point - _buf->end_point - 1);\
    }\
} while(0);



    /*------------------------------------------------------------------------------
    Defination :MUX contanst 
    ------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------
    struct :  the system config struct define
    ------------------------------------------------------------------------------*/
    typedef SIO_BUF_S                   MUX_LLI_BUF_T ;
    typedef  SIO_CONTROL_S              MUX_CONTROL_T;
    typedef  sio_operation_st    MUX_OPER_T;
    typedef void (*MUX_PORTHDL_FUNC)(uint32 event);


    typedef struct
    {
        uint32          open_flag;   
        uint32          phy_port;
        MUX_CONTROL_T   dcb;
        MUX_LLI_BUF_T   tx_buf;
        MUX_LLI_BUF_T   rx_buf; 
        volatile uint32 last_tx;    // Last time for tx
        MUX_OPER_T      mux_op;  /* operation for COM*/
        MUX_PORTHDL_FUNC  func;
    } MUX_LLI_INFO_T;

	
    /*Driver send to MUX RECV TASK Signal to notity data arrived*/
    typedef struct
    {
        SIGNAL_VARS      uint32 data_length;
        uint8                  *data_ptr;
    } MUX_RECV_DATA_DRIVER_SIG_T;



    /*------------------------------------------------------------------------------
    Defination :MUX var
    ------------------------------------------------------------------------------*/
    extern MUX_MUTEX_PTR    mux_driver_mutex[MAX_MUX_NUM];    // driver communication mutex
    extern MUX_LLI_INFO_T   mux_port[MAX_MUX_NUM];

    /*------------------------------------------------------------------------------
    Defination :MUX function
    ------------------------------------------------------------------------------*/
    PUBLIC HSIO MUX_Create(uint32 port, uint32  phy_port,  MUX_CONTROL_T *dcb);
    PUBLIC void MUX_Close(uint32 port);

    PUBLIC uint32 MUX_GetTxFifoCnt(uint8 mux_index);
    PUBLIC uint32 MUX_GetRxFifoCnt(uint8 mux_index);    
    PUBLIC void MUX_Tx_Int_Enable(BOOLEAN enable_flag, uint8 mux_index);
    PUBLIC void MUX_Rx_Int_Enable(BOOLEAN enable_flag, uint8 mux_index);
    PUBLIC uint32 MUX_RxForPort(uint8 *data_buf, uint32 rec_num, uint8 mux_index) ;
    PUBLIC uint32 MUX_TxForPort(uint8 *data_buf, uint32 fifo_size, uint8 mux_index);
    PUBLIC uint32 MUX_Data_Tx(uint8 *buf, uint32 len, uint8 mux_index);
    
    //PUBLIC MUX_RETURN_E     MUX_PutChar_Tx(uint8 ch);
    /*cr136950 for SME test. MUX_Create shall not called by SME test and SME PRI is lower than MUX recv task*/	
    PUBLIC void    MUX_Fill_Rxbuff(uint8 *data_buf, uint32 rec_num);
    PUBLIC uint32 MUX_Get_TxDataNeedSend(uint8 mux_index);
    PUBLIC BOOLEAN MUX_IsUsbDataEnable(void);
	PUBLIC void  MUX_Ring_Clear( SIO_BUF_S *pRing );
	PUBLIC void MUX_EnableFlowCtl(void);	
    PUBLIC BOOLEAN  MUX_IsNeedToActivePS(void);
#ifdef   __cplusplus
}
#endif
#endif/*_MUX_ALL_H*/



