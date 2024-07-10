/* fdl_conf.h
 * Any platform relevent parameters should go here.
 */
#ifndef FDL_CONF_H
#define FDL_CONF_H

#define STACK_SIZE      0x800

/* Settings of packet */
#define MAX_PKT_SIZE    0x1000 /* Just data field of a packet excluding header and 
* checksum
*/
#define PACKET_MAX_NUM    2

#define MEMORY_START        0x30000000
#define MEMORY_SIZE         0x01000000 /* 16M */

#endif /* FDL_CONF_H */