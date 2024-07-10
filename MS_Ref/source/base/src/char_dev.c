/******************************************************************************
 ** File Name:      char_dev.c                                                *
 ** Author:                                                                   *
 ** DATE:                                                                     *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file provide func operations for sio/mux             *
*******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE                NAME                     DESCRIPTION                  *
 ******************************************************************************/

#include "sci_types.h"
#include "char_dev.h"


#define	MAX_CHAR_DEV_NUM		32

extern const  sio_operation_st *uart_get_operation(void);
extern const sio_operation_st *ucom_get_operation(void);
#ifdef SPRD_RAW_CARD_LOG
extern const sio_operation_st *CARDLOG_get_operation(void);
#endif
CHAR_OPER_T	* char_dev_operation[MAX_CHAR_DEV_NUM]={NULL};

PUBLIC void		CharDev_Register_Operation(uint8 dev_id,const CHAR_OPER_T* char_dev_op_ptr)
{
	char_dev_operation[dev_id] = (CHAR_OPER_T	*)char_dev_op_ptr;
}
PUBLIC void		CharDev_Unregister_Operation(uint32 dev_id)
{
	if(dev_id < MAX_CHAR_DEV_NUM )
	{
		char_dev_operation[dev_id] = NULL;
	}
    return; 
}
LOCAL CHAR_OPER_T *CharDev_Get_Operation(uint32 dev_id)
{
	if(dev_id < MAX_CHAR_DEV_NUM )
	{
		return char_dev_operation[dev_id];
	}
		
    return NULL; 
}


PUBLIC CHAR_OPER_T *CharDev_Get_Operation_By_PhyPort(uint32        phy_port)
 {
    CHAR_OPER_T *operation;
	
    switch (phy_port)
    {
        case COM0:
        case COM1:
        case COM2:
        case COM3:
            operation = CharDev_Get_Operation(CHAR_DEV_UART);
            break;
        case VIR_COM0:
        case VIR_COM1:
	 case VIR_COM3:
        case VIR_COM4:		
            operation = CharDev_Get_Operation(CHAR_DEV_UCOM);
            break;
        case DSK_COM0:
        case DSK_COM1:
            operation = CharDev_Get_Operation(CHAR_DEV_CARD);
            break;
        case VIR_COM2:
            operation = CharDev_Get_Operation(CHAR_DEV_BT);
            break;          
        default:
            operation = NULL;
            break;
    }
    return operation;
}

PUBLIC void	CharDev_Initialize(void)
{
	Debug_Uart4_puts("Enter CharDev_Initialize_1\n");
	CharDev_Register_Operation(CHAR_DEV_UART,uart_get_operation());
	Debug_Uart4_puts("Enter CharDev_Initialize_2\n");
	CharDev_Register_Operation(CHAR_DEV_UCOM,ucom_get_operation());
	Debug_Uart4_puts("Enter CharDev_Initialize_3\n");
#ifdef SPRD_RAW_CARD_LOG	
    CharDev_Register_Operation(CHAR_DEV_CARD,CARDLOG_get_operation());
#endif

}
PUBLIC void	CharDev_Uninitialize(void)
{
	
}
PUBLIC BOOLEAN CharDev_Install_Operation(CHAR_OPER_T *dest_op,uint32 phy_port)
{
	CHAR_OPER_T *dev_op;

	dev_op =(CHAR_OPER_T *)CharDev_Get_Operation_By_PhyPort(phy_port);
	if( dev_op && dest_op )
    {
    	 *dest_op = *dev_op;
/*    	 
       dest_op->init      = dev_op->init ;        
       dest_op->close     = dev_op->close;
       dest_op->get_txfifocnt = dev_op->get_txfifocnt;
       dest_op->read      = dev_op->read;
       dest_op->write     = dev_op->write;
       dest_op->get_char  = dev_op->get_char;
       dest_op->put_char  = dev_op->put_char;
       dest_op->tx_enable = dev_op->tx_enable;
       dest_op->rx_enable = dev_op->rx_enable;
	   dest_op->flush     = dev_op->flush;   
	   dest_op->put_chars = dev_op->put_chars;   
	   dest_op->get_rxfifocnt = dev_op->get_rxfifocnt ;*/
       return TRUE;
	}
    return FALSE;
}


