
#ifndef PACKET_H
#define PACKET_H

#include "bdl_cmd_def.h"

/* C header file */

#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

typedef enum
{
    PKT_NONE = 0,
    PKT_HEAD,
    PKT_GATHER,
    PKT_RECV,
    PKT_ERROR
} pkt_flag_s;

struct PACKET_BODY_tag
{
    unsigned short  type;
    unsigned short  size;
    unsigned char   content[ MAX_PKT_SIZE ];
};

typedef struct PACKET_tag
{
    struct PACKET_tag      *next;

    int     pkt_state;  // used by recv handle, see pkt_flag_s
    int     data_size;
    int     ack_flag;
	
    struct PACKET_BODY_tag packet_body;
} PACKET_T;

typedef enum 
{
    SYS_STAGE_NONE,
    SYS_STAGE_CONNECTED,
    SYS_STAGE_START,    
    SYS_STAGE_GATHER,
    SYS_STAGE_END,
    SYS_STAGE_ERROR
} down_flag_s;

typedef struct data_download_tag
{       
    down_flag_s      stage;
    unsigned long    file_size;
    unsigned long    recv_size;
    unsigned long    start_addr;
    unsigned long    write_addr;
}data_download, *data_download_ptr;

typedef struct sys_stage_tag
{   
    //BYTE             
    ret_status       data_verify;
    data_download    data;
}sys_stage_s, *sys_stage_ptr;

#define PACKET_HEADER_SIZE   4   // (type + size)


void packet_init( void );

PACKET_T * malloc_packet( void );

void free_packet( PACKET_T * ptr );

// Receive a packet, after handle it, should call free_packet to release it.
DLSTATUS   receive_packet(PACKET_T**  packet_ptr);

//
// This function be called when system is in idle to process the input char.
//
DLSTATUS packet_do_idle( void );

// send the packet.
void send_packet( PACKET_T * packet_ptr );

void send_ack_packet( cmd_pkt_type pkt_type );

void Send_Respond(cmd_pkt_type cmd, DLSTATUS status, unsigned long param);

void FDL_SendErrLog(const char *x_format, ...);

void FDL_SendTrace(const char *x_format, ...);

extern struct FDL_ChannelHandler *gFdlUsedChannel;

extern unsigned long g_fdl_pkt_crc_type;

#define FDL_

#ifdef __cplusplus  /* Insert end of extern C construct */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */


#endif  // BDL_PACKET_H

