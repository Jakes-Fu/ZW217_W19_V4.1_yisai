
#ifndef CMD_DEFINE_H
#define CMD_DEFINE_H

/* C header file */

#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

typedef enum _CMD_TYPE
{
    BSL_PKT_TYPE_MIN = 0,                       /* the bottom of the DL packet type range */
    BSL_CMD_TYPE_MIN = BSL_PKT_TYPE_MIN,        /* 0x0 */
    CMD_DL_SYNC =  BSL_PKT_TYPE_MIN ,           /* 0x0 */
    CMD_DL_BEGIN ,                  			/* 0x1 */    	 
    CMD_DL_BEGIN_RSP,                  		    /* 0x2 */
    CMD_DL_DATA,                  		        /* 0x3 */
    CMD_DL_DATA_RSP,                    		/* 0x4 */
    CMD_DL_END,    						        /* 0x5 */               	
    CMD_DL_END_RSP,						    	/* 0x6 */
    CMD_RUN_GSMSW,							    /* 0x7 */
    CMD_RUN_GSMSW_RSP,						    /* 0x8 */

    /* Link Control */
    BSL_CMD_CONNECT = BSL_CMD_TYPE_MIN,         /* 0x0 */
    /* Data Download */
    /* the start flag of the data downloading */
    BSL_CMD_START_DATA,                         /* 0x1 */
    /* the midst flag of the data downloading */
    BSL_CMD_MIDST_DATA,                         /* 0x2 */
    /* the end flag of the data downloading */
    BSL_CMD_END_DATA,                           /* 0x3 */
    /* Execute from a certain address */
    BSL_CMD_EXEC_DATA,                          /* 0x4 */
    BSL_CMD_NORMAL_RESET,                       /* 0x5 */
    BSL_CMD_READ_FLASH,                         /* 0x6 */
    BSL_CMD_READ_CHIP_TYPE,                     /* 0x7 */
    BSL_CMD_LOOKUP_NVITEM,                      /* 0x8 */
    BSL_SET_BAUDRATE,                           /* 0x9 */
    BSL_ERASE_FLASH,                            /* 0xA */
    BSL_REPARTITION,                            /* 0xB */


    /* Start of the Command can be transmited by phone*/
    BSL_REP_TYPE_MIN = 0x80,

    /* The operation acknowledge */
    BSL_REP_ACK = BSL_REP_TYPE_MIN,         /* 0x80 */
    BSL_REP_VER,                            /* 0x81 */

    /* the operation not acknowledge */
    /* system  */
    BSL_REP_INVALID_CMD,                    /* 0x82 */
    BSL_REP_UNKNOW_CMD,                     /* 0x83 */
    BSL_REP_OPERATION_FAILED,               /* 0x84 */

    /* Link Control*/
    BSL_REP_NOT_SUPPORT_BAUDRATE,           /* 0x85 */

    /* Data Download */
    BSL_REP_DOWN_NOT_START,                 /* 0x86 */
    BSL_REP_DOWN_MULTI_START,               /* 0x87 */
    BSL_REP_DOWN_EARLY_END,                 /* 0x88 */
    BSL_REP_DOWN_DEST_ERROR,                /* 0x89 */
    BSL_REP_DOWN_SIZE_ERROR,                /* 0x8A */
    BSL_REP_VERIFY_ERROR,                   /* 0x8B */
    BSL_REP_NOT_VERIFY,                     /* 0x8C */

    /* Phone Internal Error */
    BSL_PHONE_NOT_ENOUGH_MEMORY,            /* 0x8D */
    BSL_PHONE_WAIT_INPUT_TIMEOUT,           /* 0x8E */

    /* Phone Internal return value */
    BSL_PHONE_SUCCEED,                      /* 0x8F */
    BSL_PHONE_VALID_BAUDRATE,               /* 0x90 */
    BSL_PHONE_REPEAT_CONTINUE,              /* 0x91 */
    BSL_PHONE_REPEAT_BREAK,                 /* 0x92 */

    BSL_REP_READ_FLASH,                     /* 0x93 */
    BSL_REP_READ_CHIP_TYPE,                 /* 0x94 */
    BSL_REP_LOOKUP_NVITEM,                  /* 0x95 */

    BSL_INCOMPATIBLE_PARTITION,             /* 0x96 */
    BSL_UNKNOWN_DEVICE,                     /* 0x97 */
    BSL_INVALID_DEVICE_SIZE,                /* 0x98 */

    BSL_ILLEGAL_SDRAM,                      /* 0x99 */
    BSL_WRONG_SDRAM_PARAMETER,              /* 0x9a */
    BSL_EEROR_CHECKSUM = 0xA0,
    BSL_CHECKSUM_DIFF,
    BSL_WRITE_ERROR,
    BSL_UDISK_IMAGE_SIZE_OVERFLOW,          /* 0xa3 */
    BSL_FLASH_CFG_ERROR= 0xa4,              /* 0xa4 */
    BSL_REP_DOWN_STL_SIZE_ERROR,
#ifdef SECURE_BOOT_SUPPORT
    BSL_REP_SECURITY_VERIFICATION_FAIL = 0xa6,
#endif
    BSL_REP_LOG = 0xFF,                             /* 0xff */
    BSL_PKT_TYPE_MAX,
    BSL_CMD_TYPE_MAX = BSL_PKT_TYPE_MAX
} CMD_TYPE;



typedef signed short cmd_pkt_type;
typedef cmd_pkt_type ret_status;

typedef enum _DLSTATUS
{
   DL_SUCCESS = 0,
   DL_CRC_ERROR,
   DL_FLASH_ERROR,
   DL_DOWNLOADING,
   DL_PACKET_ERROR,
   DL_RSP_ERROR,
   DL_ERROR ,
   DL_TIMEOUT
}DLSTATUS;

#ifdef SECURE_BOOT_SUPPORT
#ifdef FOTA_SUPPORT_NONE
#define SYS_MODEM_PROGRAM_START_ADDR  0x10000 + 1024  //added SECURE_CODE_OFF 1024
#else
#define SYS_MODEM_PROGRAM_START_ADDR  0x20000 + 1024  //added SECURE_CODE_OFF 1024
#endif
#else
#ifdef FOTA_SUPPORT_NONE
#define SYS_MODEM_PROGRAM_START_ADDR  0x10000
#else
#define SYS_MODEM_PROGRAM_START_ADDR  0x20000
#endif
#endif

#define MAX_VERSION_SIZE  4

typedef struct CMD_DL_BEGIN_Tag
{
   	unsigned char version[MAX_VERSION_SIZE];
   
}CMD_DL_BEGIN_T;

typedef struct CMD_DL_BEGIN_RSP_Tag
{
   	unsigned short status;
   	unsigned short max_unit_size;
}CMD_DL_BEGIN_RSP_T;


typedef struct CMD_DL_DATA_Tag
{
	unsigned long packet_id;
}CMD_DL_DATA_T;

typedef struct CMD_DL_DATA_RSP_Tag
{
   	unsigned short status;
   	unsigned long next_packet_id;
}CMD_DL_DATA_RSP_T;

typedef struct CMD_DL_END_RSP_Tag
{
	unsigned short status;
}CMD_DL_END_RSP_T;

typedef struct CMD_RUN_GSMSW_RSP_Tag
{
   	unsigned short status;
}CMD_RUN_GSMSW_RSP_T;

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

#define MAX_PKT_SIZE_B            (0x400)          //Maximum packet size:  1KB
#define REP_PKT_SIZE            (0x04)            //Response packet size: 8 Bytes


/**---------------------------------------------------------------------------*
 ** The Follow defines the packet processed result table                      *
 ** packet_protocol:                                                          *
 ** HDLC_FLAG   PKT_TYPE     DATALENGHT    [DATA]    CRC       HDLC_FLAG      *
 **   0x7E      MAX:255      MAX:65536      ...      ...          0x7E        *
 **    1           2             2           0        2             1         *
 ** response_packet:                                                          *
 **   0x7E      MAX:255          0           --      ...          0x7E        *
 **    1           2             2           --       2             1         *
 ** response packet length: 8 bytes                                           *
 **---------------------------------------------------------------------------*/

typedef struct _PKT_HEADER
{
    unsigned short type;
    unsigned short size;
} PKT_HEADER, *PPKT_HEADER;

typedef struct pkt_header_tag
{
    unsigned short pkt_type;
    unsigned short pkt_size;
} pkt_header, *pkt_header_ptr;

/* transmit proctol */
/* 标志  类型  长度	  信      息	CRC	  标志  */
/*  1	  2	    2      0---1024      2      1   */
/* 0x7e	 type  len      content     crc    ox7e */
#define PKT_FLAG_SIZE              1
#define PKT_CRC_SIZE               2
#define PKT_TYPE_SIZE              1
#define PKT_LEN_SIZE               2

#define PKT_HEADER_SIZE            sizeof(PKT_HEADER)
#define DATA_ADDR                  PKT_HEADER_SIZE
#define DELAY_TIME                 0x100000   

#define MAX_MTU_SIZE            (0x4000)   //16KB
#define MAX_PKT_SIZE            (MAX_MTU_SIZE + 0x100)//(0x3400)          //Maximum packet size: 13KB
#ifdef SECURE_BOOT_SUPPORT
#define STACK_LIMIT             (0x00002000)
#else
#define STACK_LIMIT             (0x00001000)
#endif
#define SIO_RECV_LIMIT          (0x00000400)      //SIO Receive Limit: 1KB
#define MIN_START_ADDESSS       (0x00010000)

#if 1
#define SEND_ERROR_RSP(x)
#else
#define SEND_ERROR_RSP(x)      {send_ack(x); while(1);}  
#endif          
     
#ifdef __cplusplus  /* Insert end of extern C construct */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */


#endif 
