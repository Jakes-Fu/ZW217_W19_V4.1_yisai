#include "string.h"
#include "sci_types.h"
#include "bdl_customif.h"
#include "bdl_packet.h"
#include "bdl_crc.h"
#include "bdl_cmd_def.h"
#include "sio_drv.h"
#include "fdl_sys.h"
#include "fdl_stdio.h"
#include "fdl_main.h"

extern void send_ack( cmd_pkt_type pkt_type );

#define PACKET_MAX_NUM   4

#pragma arm section zidata = "ZIDATA_AREA_1"
static PACKET_T  packet[ PACKET_MAX_NUM ];
#pragma arm section zidata

static PACKET_T *packet_free_list;
static PACKET_T *packet_completed_list;
static PACKET_T *packet_receiving;

unsigned long sio_get_char_en( unsigned char *pCh)
{
#define MAX_TIMEOUT_TIME  0xFFFFFFFF

    uint32 status = 0;
    uint32 cur_time = fdl_GetTickCount();

    *pCh = 0;
    
    while( !SIO_RX_READY(SIO_GET_RX_STATUS(FDL_GetUARTBaseAddr())) )
    {
       if( (cur_time + MAX_TIMEOUT_TIME) > fdl_GetTickCount() )
       {
           status = 1 ;
           break;
       }
    }
    
    if(0 == status)
    {
      *pCh  = SIO_GET_CHAR(FDL_GetUARTBaseAddr());
    }
    
    return (status);
}

void sio_put_char( char ch )
{
    /* Check if tx port is ready.*/
    while (!SIO_TX_READY(SIO_GET_TX_STATUS(FDL_GetUARTBaseAddr())))
    { ; }
    
	SIO_PUT_CHAR(FDL_GetUARTBaseAddr(), ch);

    /* Ensure the last byte is written successfully */
	while (!SIO_TX_READY(SIO_GET_TX_STATUS(FDL_GetUARTBaseAddr()))) {
		/* Do nothing */
	}	
}

void packet_init( void )
{ 
	uint32 i = 0;

	packet_free_list = &packet[0];
	for(i = 0; i < PACKET_MAX_NUM; i++)
	{
		FDL_memset( &packet[i], 0, sizeof(PACKET_T) );
		packet[i].next   = &packet[i+1];
	}

	packet[PACKET_MAX_NUM-1].next = PNULL;

    packet_completed_list = NULL;
    packet_receiving      = NULL;
}


PACKET_T * malloc_packet( void )
{
    PACKET_T  * tmp_ptr = NULL;
    
    if( NULL != packet_free_list )
    {
        tmp_ptr = packet_free_list;
        packet_free_list = tmp_ptr->next;
        
        // only clear the packet header
        FDL_memset( tmp_ptr, 0, 32 );
                
        tmp_ptr->next    	= NULL;            
        tmp_ptr->pkt_state 	= PKT_NONE;
        tmp_ptr->ack_flag   = 0;
        tmp_ptr->data_size  = 0;
    }
    return tmp_ptr;
}

void free_packet( PACKET_T * ptr )
{       
    ptr->next        = packet_free_list;
    packet_free_list = ptr;
}

DLSTATUS   receive_packet(PACKET_T**  packet_ptr)
{

     DLSTATUS  status = DL_SUCCESS;
        
	// waiting for a packet
    while (NULL == packet_completed_list)
    {
        packet_do_idle();
    }
     
     if(DL_SUCCESS == status)
     {
         PACKET_T * ptr;
           // remove from completed list
        ptr                     = packet_completed_list;        
        packet_completed_list   = ptr->next;
        ptr->next               = NULL; 
        
        *packet_ptr = ptr;
     }

    return status;

}

DLSTATUS packet_do_idle( void )
{
 	//FDL_BOOT_MODE_E          boot_mode;
    unsigned char * pdata      = NULL;
	PACKET_T *      packet_ptr = NULL;
	PACKET_T *      tmp_ptr    = NULL;
	//uint32          crc;
    unsigned char   ch;
    uint32 status = 0;
	//int ch1;

     // try get a packet to handle the receive char
    packet_ptr = packet_receiving;

    if ( NULL == packet_ptr )
    {
        packet_ptr = malloc_packet();
        if( NULL != packet_ptr )
        {
            packet_receiving    = packet_ptr;
            packet_ptr->next    = NULL;
        }
        else
        {
           return DL_ERROR;
        }
    }
    
    pdata  = (unsigned char *)&(packet_ptr->packet_body);

    //-------------uart boot mode ------------------------//
    while (1)
    {
        /*
        * here the reason that we don't call sio_get_char is 
        * that sio_get_char() isnot exited until got data from
        * outside,packet_do_idle() is called also in norflash write and erase
        * so we have to exited when no char recieived here,
        * usb virtual com is the same handle
        */

         status  = sio_get_char_en(&ch);
         if(status)
         {
            return DL_TIMEOUT;
         }
     
    	if( packet_ptr->data_size > MAX_PKT_SIZE )
    	{
        	packet_receiving = NULL;
        	free_packet( packet_ptr );
        	SIO_TRACE("data_size error" );
        	return  DL_ERROR;
    	}    
	    
    	// try handle this input data
    	switch (packet_ptr->pkt_state )
    	{
    	    case PKT_NONE:
    	    {
	        	if(HDLC_FLAG == ch)
	        	{
	            	packet_ptr->pkt_state = PKT_HEAD;
	            	packet_ptr->data_size = 0;
   	        	}
   	            break;
   	        }
	    	case PKT_HEAD:
        	{
        	    uint32 t= 0;
            	do
            	{
	            	*(pdata + packet_ptr->data_size)         = ch;
              	    packet_ptr->data_size ++;
	            	t++;
	            	if(t >= PACKET_HEADER_SIZE)
	            	{
	            	    break;
	            	}
                    status  = sio_get_char_en(&ch);
                    if(status)
                    {
                        SIO_TRACE("PKT_HEAD:Timeout %d\r\n", t);
                        return DL_TIMEOUT;
                    }

            	}while(1);
            	
            	//packet_ptr->packet_body.size = EndianConv_16 (packet_ptr->packet_body.size);
                //packet_ptr->packet_body.type = EndianConv_16 (packet_ptr->packet_body.type);
                
            	if(packet_ptr->packet_body.size)
            	{
            	    packet_ptr->pkt_state = PKT_GATHER;
            	}
            	else
            	{
            	    packet_ptr->pkt_state = PKT_RECV;
            	}

                break;
        	}	            
	    	case PKT_GATHER:
        	{
        	    uint32 t = 0;
        	    do 
        	    {
                  	*(pdata + packet_ptr->data_size)  = ch;
	            	packet_ptr->data_size ++;
                	t++;
                	if(t >= packet_ptr->packet_body.size)
                	{
                	    break;
                	}
                    status  = sio_get_char_en(&ch);
                    if(status)
                    {
                        SIO_TRACE("PKT_GATHER Timeout:%d\r\n", t);
                        return DL_TIMEOUT;
                    }
        	    } while(1);
        	    
        	    packet_ptr->pkt_state = PKT_RECV;
	        	break;
        	}
            case PKT_RECV:
            {
                uint32 t = 0;
                uint16 rev_crc = 0;
                uint16 calc_crc = 0;
                uint8* crc_ptr = (uint8*)&rev_crc;
        	    do
        	    {
                  	*crc_ptr++  = ch;
             	    t++;
             	    if(t >= sizeof(rev_crc))
             	    {
             	        break;
             	    }
                    status  = sio_get_char_en(&ch);
                    if(status)
                    {
                        SIO_TRACE("PKT_RECV Timeout:%d\r\n", t);
                        return DL_TIMEOUT;
                    }

        	    }while(1);
        	    
        	    packet_ptr->pkt_state = PKT_NONE;
        	    
        	    // check the packet. CRC should be 0
            	calc_crc = crc_16_l_calc((char *)&packet_ptr->packet_body, packet_ptr->data_size); 
            	if( rev_crc != calc_crc )
            	{  
                	// Verify error, reject this packet.
                	free_packet( packet_ptr );
                    packet_receiving = NULL;
                	SIO_TRACE( "CRC ERROR:calc:0x%X, recv:0x%X\r\n", calc_crc, rev_crc);
                	return DL_CRC_ERROR;
            	}
            	else
            	{               
                	// Check there are free packet?
                	if(    (NULL != packet_free_list)
                    	&& (BSL_CMD_MIDST_DATA == packet_ptr->packet_body.type) )
                	{    
                    	// send a ACK
                    	packet_ptr->ack_flag = 1;
                	}
                                
                	// It is a complete packet, move to completed list.                
                	packet_ptr->next = NULL;
                	if( NULL == packet_completed_list)
                	{
                    	packet_completed_list       = packet_ptr;
                	}
                	else
                	{
                    	// added to the tail
                    	tmp_ptr = packet_completed_list;
                    	while( NULL != tmp_ptr->next )
                    	{
                        	tmp_ptr = tmp_ptr->next;
                    	}
                    	tmp_ptr->next = packet_ptr;
                	}
                	
                	//set to null for next transfer
                	packet_receiving = NULL;
                
                	return DL_SUCCESS;
                }	
                break;
            }	        	
	    	default:
        	break;
    	}
	}

}

/******************************************************************************
 * write_packet
 ******************************************************************************/
void write_packet(const void * buf, int len)
{
	const unsigned char * pstart = (const unsigned char*)buf;
	const unsigned char * pend = pstart + len;
	
	while (pstart<pend) 
	{
        sio_put_char(*pstart);
		++pstart;
	}

}

uint32  data_process( PACKET_T * packet_ptr_src, PACKET_T * packet_ptr_dest)
{
    unsigned short  crc, size;
    int32           write_len;  /*orginal length*/
    int32           send_len;   /*length after encode*/
    //int32           i;
    //uint8           curval;

    uint8 *         des_ptr = NULL;
    uint8 *         src_ptr = NULL;

    size = packet_ptr_src->packet_body.size;
    write_len = size + sizeof(unsigned short) + PACKET_HEADER_SIZE;
    src_ptr = (uint8*)&packet_ptr_src->packet_body;

    //packet_ptr_src->packet_body.size = EndianConv_16 (packet_ptr_src->packet_body.size);
    //packet_ptr_src->packet_body.type = EndianConv_16 (packet_ptr_src->packet_body.type);

    /*src CRC calculation*/
    crc = crc_16_l_calc((char*)(&(packet_ptr_src->packet_body)), size + PACKET_HEADER_SIZE);
    packet_ptr_src->packet_body.content[ size ] = (crc)    & 0xFF;
    packet_ptr_src->packet_body.content[ size+1 ] = (crc >> 8) & 0xFF;

    /*******************************************
    *    des data preparation
    ********************************************/

    des_ptr = (uint8*)&packet_ptr_dest->packet_body;
    /*head flag*/
    *(des_ptr++) = HDLC_FLAG; 
    send_len = 1;

    FDL_memcpy(des_ptr, src_ptr, write_len);

    send_len += write_len;
     
    return send_len;
}
/******************************************************************************
 * send_packet
 ******************************************************************************/
void send_packet( PACKET_T * packet_ptr )
{
    int32           send_len;   /*length after encode*/
    PACKET_T *      tmp_packet_ptr = NULL;

    // send a ACK packet to notify PC that we are ready.
    tmp_packet_ptr = malloc_packet();
    if(NULL == tmp_packet_ptr)
    {
        free_packet(packet_receiving);
        tmp_packet_ptr = malloc_packet();
    }

    send_len = data_process(packet_ptr, tmp_packet_ptr);

    write_packet((char *)(&(tmp_packet_ptr->packet_body)), send_len); 

    free_packet( tmp_packet_ptr );

}

/******************************************************************************
 * send_ack_packet
 ******************************************************************************/
void send_ack_packet( cmd_pkt_type  pkt_type )
{    
     
    unsigned long ack_packet_src[8];
    unsigned long ack_packet_dst[8];
    PACKET_T * packet_ptr = (PACKET_T *)ack_packet_src;

    int32           send_len;   /*length after encode*/
    PACKET_T *      tmp_packet_ptr = NULL;

    packet_ptr->packet_body.type = pkt_type;
    packet_ptr->packet_body.size = 0;

    tmp_packet_ptr = (PACKET_T *)ack_packet_dst;

    send_len = data_process(packet_ptr, tmp_packet_ptr);
    write_packet((char *)(&(tmp_packet_ptr->packet_body)), send_len); 

}

PACKET_T   g_respond_packet;

void Send_Respond(cmd_pkt_type cmd, DLSTATUS status, uint32 param)
{
   uint8* rsp_buf = g_respond_packet.packet_body.content;
   uint16 len = 0 ;
   uint16 crc = 0;
   uint8* send_ptr = NULL;
   uint32 t = 0;
   
   switch(cmd)
   {
   case CMD_DL_BEGIN_RSP:
   {
      CMD_DL_BEGIN_RSP_T* cmd_rsp = (CMD_DL_BEGIN_RSP_T*)rsp_buf;
      cmd_rsp->status =  status;
      cmd_rsp->max_unit_size = param; 
      len = sizeof(CMD_DL_BEGIN_RSP_T);
      break;
   }
   case CMD_DL_DATA_RSP:
   {
      CMD_DL_DATA_RSP_T* cmd_rsp = (CMD_DL_DATA_RSP_T*)rsp_buf;
      cmd_rsp->status =  status;
      cmd_rsp->next_packet_id = param;
      len = sizeof(CMD_DL_DATA_RSP_T);
      break;
   }
   case CMD_DL_END_RSP:
   {
      CMD_DL_END_RSP_T* cmd_rsp = (CMD_DL_END_RSP_T*)rsp_buf;
      cmd_rsp->status =  status;
      len = sizeof(CMD_DL_END_RSP_T);      
      break;
   }
   case CMD_RUN_GSMSW_RSP:
   {
      CMD_RUN_GSMSW_RSP_T* cmd_rsp = (CMD_RUN_GSMSW_RSP_T*)rsp_buf;
      cmd_rsp->status =  status;   
      len = sizeof(CMD_RUN_GSMSW_RSP_T);       
      break;
   }   
   default:
   {
      return;
      break;
   }
   }
   
   g_respond_packet.packet_body.type = cmd;
   g_respond_packet.packet_body.size = len;
   
   //Send 0xAA
   sio_put_char(0xAA);
   
    //Send Body
    send_ptr = (uint8*)&g_respond_packet.packet_body;
    t = 0;
    while(t++ < (g_respond_packet.packet_body.size + PACKET_HEADER_SIZE))
    {
      sio_put_char(*send_ptr++);  
    }
    //Send CRC

    crc = crc_16_l_calc((char*)(&(g_respond_packet.packet_body)), g_respond_packet.packet_body.size + PACKET_HEADER_SIZE);
    send_ptr = (uint8*)&crc;
    t = 0;
    while(t++ < sizeof(crc))
    {
       sio_put_char(*send_ptr++);  
    }
}

void FDL_PacketDoIdle(void)
{
    packet_do_idle();
}

void FDL_FreePacket(PACKET_T * ptr)
{
    free_packet(ptr);
}

PACKET_T * FDL_GetPacket(void)
{
    return malloc_packet();
}

void FDL_SendErrLog(const char *x_format, ...)
{
#ifdef 0

    PACKET_T *packet_ptr = PNULL;

    int32      send_len = 0;
    char       format_str[256] = {0}; 
    va_list    args;

    va_start(args, x_format);
    send_len = vsprintf(format_str, x_format, args);
    if(send_len%2)
    {
        format_str[send_len++] = 0x20;
    }
    send_len += vsprintf(format_str+send_len, "\r\n", args);
    va_end(args);

    FDL_SendTrace(format_str);
        
    packet_ptr = FDL_MallocPacket();

    packet_ptr->packet_body.type = BSL_REP_LOG;
    packet_ptr->packet_body.size = send_len;

    FDL_memcpy (packet_ptr->packet_body.content, format_str, send_len);

    FDL_SendPacket (packet_ptr);

    FDL_FreePacket (packet_ptr);

    {
        int i;
        for(i=0; i<2000; i++);
    }
    
#endif    
    return;
}

/**********************************************************************************/
//  Description :
//      
//    Global resource dependence :
//      sector_size
//  Author:
//      henry.he 2012.4.8
//    Note:
//      
/**********************************************************************************/
void FDL_SendTrace(const char *x_format, ...)
{
#if 0
    int     ret = 0;
    int32   send_len = 0;
    char    format_str[256] = {0}; 
    va_list args;
        
    va_start(args, x_format);
    send_len = vsprintf(format_str, x_format, args);
    send_len += vsprintf(format_str+send_len, "\r\n", args);
    va_end(args);

    if(&gUart1Channel != FDL_ChannelGet())
    {
        ret = gUart1Channel.Write (&gUart1Channel, format_str, send_len);
    }

    if(&gUart0Channel != FDL_ChannelGet())
    {
        ret = gUart0Channel.Write (&gUart0Channel, format_str, send_len);
    }

    //FDL_SendTrace("%s %d", __func__, __LINE__);
#endif
    return;
}