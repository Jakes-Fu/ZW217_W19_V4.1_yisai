/******************************************************************************
 ** File Name:      sio_fifo.h                                                *
 ** Author:         Leo.Feng                                                  *
 ** DATE:           30/11/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the interfaces of  the FIFO for serial  *
 **                 I/O operation. The FIFO is a clic queue.Writing operation *
 *                  can overlap the data has not be reading.                  * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 30/11/2001     Leo.Feng         Create.                                   *
 ** 11/12/2003     Zhemin.Lin       Enhanced according to cr6198              *
 ******************************************************************************/

#ifndef _CMD_DEFINE__H_
#define _CMD_DEFINE__H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

typedef enum CMD_PKT_TYPE
{
    BSL_PKT_TYPE_MIN,                   /* the bottom of the DL packet type range */
   
    /* Start of the Command can be received by phone*/
    /* System */
    //BSL_CMD_RESET = BSL_PKT_TYPE_MIN,   /* reset system */
    //BSL_CMD_CFG,                        /* configure communicating protocol */
    //BSL_CMD_STOP,                       /* Terminate download */
    //BSL_CMD_VER_REQ,                    /* request version infomation */
    
    /* Link Control */    
    BSL_CMD_CONNECT = BSL_PKT_TYPE_MIN,    

    /* Data Download */
    BSL_CMD_START_DATA,                  /* the start flag of the data downloading  */
    BSL_CMD_MIDST_DATA,                  /* the midst flag of the data downloading  */
    BSL_CMD_END_DATA,                    /* the end flag of the data downloading */
    //BSL_CMD_CHK_DATA,                    /* request verify the downloading data */
    BSL_CMD_EXEC_DATA,                   /* Execute from a certain address */
    /* End of Data Download command*/
    /* End of the Command can be received by phone*/
    
    BSL_CMD_NORMAL_RESET,
    
    BSL_CMD_READ_FLASH,
    
    BSL_CMD_READ_CHIP_TYPE,
    
    //@lin.liu. CR13668.
    // Add read nvitem interface
    BSL_CMD_LOOKUP_NVITEM,
    BSL_SET_BAUDRATE,
    BSL_ERASE_FLASH,
    
    /* Start of the Command can be transmited by phone*/
    BSL_REP_TYPE_MIN = 0x80,

    BSL_REP_ACK = BSL_REP_TYPE_MIN,      /* The operation acknowledge */     
    BSL_REP_VER,

    /* the operation not acknowledge */
    /* system  */    
    BSL_REP_INVALID_CMD,
    BSL_REP_UNKNOW_CMD,
    BSL_REP_OPERATION_FAILED,

    /* Link Control*/    
    BSL_REP_NOT_SUPPORT_BAUDRATE,

    /* Data Download */ 
    BSL_REP_DOWN_NOT_START ,
    BSL_REP_DOWN_MULTI_START,
    BSL_REP_DOWN_EARLY_END,
    BSL_REP_DOWN_DEST_ERROR,
    BSL_REP_DOWN_SIZE_ERROR,    
    BSL_REP_VERIFY_ERROR,
	BSL_REP_NOT_VERIFY,

    /* Phone Internal Error */
    BSL_PHONE_NOT_ENOUGH_MEMORY,    
    BSL_PHONE_WAIT_INPUT_TIMEOUT,

    /* Phone Internal return value */
    BSL_PHONE_SUCCEED,
    BSL_PHONE_VALID_BAUDRATE,    
    BSL_PHONE_REPEAT_CONTINUE,
    BSL_PHONE_REPEAT_BREAK,
    /* End of the Command can be transmited by phone*/
    
    BSL_REP_READ_FLASH,
    
    BSL_REP_READ_CHIP_TYPE,
    
    //@lin.liu. CR13668.
    // Add read nvitem interface
    BSL_REP_LOOKUP_NVITEM,

    BSL_UNKNOWN_DEVICE = 0x97,	

    BSL_EEROR_CHECKSUM = 0xA0,
    BSL_CHECKSUM_DIFF,
    BSL_WRITE_ERROR,
    BSL_PKT_TYPE_MAX
}cmd_pkt_type;

typedef cmd_pkt_type ret_status;

typedef struct pkt_header_tag
{
    unsigned short pkt_type;
    unsigned short pkt_size;
}pkt_header, *pkt_header_ptr;

/* transmit proctol */
/* 标志  类型  长度	  信      息	CRC	  标志  */
/*  1	  2	    2      0---1024      2      1   */
/* 0x7e	 type  len      content     crc    ox7e */
#define PKT_FLAG_SIZE              1
#define PKT_CRC_SIZE               2
#define PKT_TYPE_SIZE              1
#define PKT_LEN_SIZE               2

#define PKT_HEADER_SIZE            sizeof(pkt_header)
#define DATA_ADDR                  PKT_HEADER_SIZE
#define DELAY_TIME                 0x100000   

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/ 

/*The Follows defines the BootBlock Memory Map.                               *
 *                          SDRAM                                             *  
 *   0x02000000       ---------------------   RAM_BEGIN                       *
 *                    |     BootBlock      |                                  *
 *                    |     Run Space      |                                  *
 *                    |       128KB        |                                  *
 *   0x02020000       |--------------------|  FDL_BEGIN                       *
 *                    |   FDL Code Space   |                                  *
 *                    |       64KB         |                                  *
 *   0x02030000       |--------------------|  FDL_END(RECV_PKT_BEGIN)         *
 *                    | Proc Packet 16KB   |                                  *
 *   0x02034000       |--------------------|  RECV_PKT_END(SIO_RECV_BEGIN)    *
 *                    | SIO RecvBuf 16KB   |                                  *
 *   0x02038000       |--------------------|  SIO_RECV_END                    *
 *                    |                    |                                  *
 *                    |                    |                                  *
 *   0x02080000       |--------------------|  RAM_END                         *
 *                                                                            *  
 *                                                                            *  
 *End of the BootBlock Memory Map.                                            */

#define SIO_RECV_BEGIN          (0x02034000)      //SIO Receive Start Address
#define SIO_RECV_END            (0x02038000)      //SIO Receive End Address

#define REP_PKT_SIZE            (0x04)            //Response packet size: 8 Bytes


//for on-chip sram is only 64K, adjust PKT size to 13K
//total 39K
#if defined NOR_FDL_SC6600I
#define MAX_PKT_SIZE            (0x3400)          //Maximum packet size: 13KB
#define STACK_LIMIT             (0x00003400)
#elif defined NOR_FDL_SC6600L
#define MAX_PKT_SIZE            (0x40000)//(0x3400)          //Maximum packet size: 13KB
#define STACK_LIMIT             (0x0000200)
#else
#define MAX_PKT_SIZE            (0x2000)          //Maximum packet size: 13KB
#define STACK_LIMIT             (0x00002000)
#endif

#define MIN_START_ADDESSS       (0x00010000)

#define SEND_ERROR_RSP(x)         \
          {                       \
              send_ack(x);        \
              while(1);           \
          }  
 /**---------------------------------------------------------------------------*
 **                         External Variables                                *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif //_SIO_FIFO__H_
