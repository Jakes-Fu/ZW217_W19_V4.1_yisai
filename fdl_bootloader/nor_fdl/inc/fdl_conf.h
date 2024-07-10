#ifndef FDL_CONF_H
#define FDL_CONF_H
/* fdl_conf.h
 * Any platform relevent parameters should go here.
 */
/* transmit proctol */
/* 标志  类型  长度   信      息    CRC   标志  */
/*  1     2     2      0---1024      2      1   */
/* 0x7e  type  len      content     crc    ox7e */
#define PKT_FLAG_SIZE              1
#define PKT_CRC_SIZE               2
#define PKT_TYPE_SIZE              1
#define PKT_LEN_SIZE               2

#define DELAY_TIME                 0x100000
#define MAX_UNIT_NUM    3


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
#define MAX_PKT_SIZE            (0x4000)//(0x3400)          //Maximum packet size: 13KB
#ifdef SECURE_BOOT_SUPPORT
#define STACK_LIMIT             (0x0002400)
#else
#define STACK_LIMIT             (0x0000400)
#endif

#define PACKET_MAX_NUM   4

#define MIN_START_ADDESSS       (0x00010000)


#endif /* FDL_CONF_H */
