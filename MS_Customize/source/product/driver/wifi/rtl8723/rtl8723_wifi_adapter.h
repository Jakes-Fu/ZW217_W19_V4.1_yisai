/*
 * wpa_supplicant/hostapd / OS specific functions
 * Copyright (c) 2005-2006, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */
#if 1
#ifndef RTL_WIFI_ADAPER_H
#define RTL_WIFI_ADAPER_H
#include <stddef.h>

//#include "os_api.h"
//#include "tcpip_types.h"
#include <stdarg.h>
#include "sci_types.h"
#include "in_message.h"

//#define OS_NO_C_LIB_DEFINES //limx debug


typedef long os_time_t;

/**
 * os_sleep - Sleep (sec, usec)
 * @sec: Number of seconds to sleep
 * @usec: Number of microseconds to sleep
 */
//void os_sleep(os_time_t sec, os_time_t usec);
void rtlwifi_sleep_time(os_time_t sec, os_time_t usec);
void rtl_delay_ms(unsigned int time);
void rtl_delay_us(unsigned int us);

void rtl_sleep_ms(unsigned int msecs);
struct rtl_os_time {
	os_time_t sec;
	os_time_t usec;
};

/**
 * os_get_time - Get current time (sec, usec)
 * @t: Pointer to buffer for the time
 * Returns: 0 on success, -1 on failure
 */
//int os_get_time(struct os_time *t);
int rtl_get_time(struct rtl_os_time *t);


/* Helper macros for handling struct os_time */
/*
#define os_time_before(a, b) \
	((a)->sec < (b)->sec || \
	 ((a)->sec == (b)->sec && (a)->usec < (b)->usec))

#define os_time_sub(a, b, res) do { \
	(res)->sec = (a)->sec - (b)->sec; \
	(res)->usec = (a)->usec - (b)->usec; \
	if ((res)->usec < 0) { \
		(res)->sec--; \
		(res)->usec += 1000000; \
	} \
} while (0)
*/

/**
 * os_mktime - Convert broken-down time into seconds since 1970-01-01
 * @year: Four digit year
 * @month: Month (1 .. 12)
 * @day: Day of month (1 .. 31)
 * @hour: Hour (0 .. 23)
 * @min: Minute (0 .. 59)
 * @sec: Second (0 .. 60)
 * @t: Buffer for returning calendar time representation (seconds since
 * 1970-01-01 00:00:00)
 * Returns: 0 on success, -1 on failure
 *
 * Note: The result is in seconds from Epoch, i.e., in UTC, not in local time
 * which is used by POSIX mktime().
 */
/*int os_mktime(int year, int month, int day, int hour, int min, int sec,
	      os_time_t *t);
*/

/**
 * rtl_os_daemonize - Run in the background (detach from the controlling terminal)
 * @pid_file: File name to write the process ID to or %NULL to skip this
 * Returns: 0 on success, -1 on failure
 */
//int rtl_os_daemonize(const char *pid_file);

/**
 * rtl_os_daemonize_terminate - Stop running in the background (remove pid file)
 * @pid_file: File name to write the process ID to or %NULL to skip this
 */
//void rtl_os_daemonize_terminate(const char *pid_file);

/**
 * rtl_os_get_random - Get cryptographically strong pseudo random data
 * @buf: Buffer for pseudo random data
 * @len: Length of the buffer
 * Returns: 0 on success, -1 on failure
 */
//int rtl_os_get_random(unsigned char *buf, size_t len);

/**
 * rtl_os_random - Get pseudo random value (not necessarily very strong)
 * Returns: Pseudo random value
 */
//unsigned long rtl_os_random(void);

/**
 * rtl_os_rel2abs_path - Get an absolute path for a file
 * @rel_path: Relative path to a file
 * Returns: Absolute path for the file or %NULL on failure
 *
 * This function tries to convert a relative path of a file to an absolute path
 * in order for the file to be found even if current working directory has
 * changed. The returned value is allocated and caller is responsible for
 * freeing it. It is acceptable to just return the same path in an allocated
 * buffer, e.g., return strdup(rel_path). This function is only used to find
 * configuration files when rtl_os_daemonize() may have changed the current working
 * directory and relative path would be pointing to a different location.
 */
//char * rtl_os_rel2abs_path(const char *rel_path);

/**
 * rtl_os_program_init - Program initialization (called at start)
 * Returns: 0 on success, -1 on failure
 *
 * This function is called when a programs starts. If there are any OS specific
 * processing that is needed, it can be placed here. It is also acceptable to
 * just return 0 if not special processing is needed.
 */
//int rtl_os_program_init(void);

/**
 * rtl_os_program_deinit - Program deinitialization (called just before exit)
 *
 * This function is called just before a program exists. If there are any OS
 * specific processing, e.g., freeing resourced allocated in rtl_os_program_init(),
 * it should be done here. It is also acceptable for this function to do
 * nothing.
 */
//void rtl_os_program_deinit(void);

/**
 * rtl_os_setenv - Set environment variable
 * @name: Name of the variable
 * @value: Value to set to the variable
 * @overwrite: Whether existing variable should be overwritten
 * Returns: 0 on success, -1 on error
 *
 * This function is only used for wpa_cli action scripts. OS wrapper does not
 * need to implement this if such functionality is not needed.
 */
//int rtl_os_setenv(const char *name, const char *value, int overwrite);

/**
 * rtl_os_unsetenv - Delete environent variable
 * @name: Name of the variable
 * Returns: 0 on success, -1 on error
 *
 * This function is only used for wpa_cli action scripts. OS wrapper does not
 * need to implement this if such functionality is not needed.
 */
//int rtl_os_unsetenv(const char *name);

/**
 * rtl_os_readfile - Read a file to an allocated memory buffer
 * @name: Name of the file to read
 * @len: For returning the length of the allocated buffer
 * Returns: Pointer to the allocated buffer or %NULL on failure
 *
 * This function allocates memory and reads the given file to this buffer. Both
 * binary and text files can be read with this function. The caller is
 * responsible for freeing the returned buffer with os_free().
 */
//char * rtl_os_readfile(const char *name, size_t *len);

/**
 * os_zalloc - Allocate and zero memory
 * @size: Number of bytes to allocate
 * Returns: Pointer to allocated and zeroed memory or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer with os_free().
 */
//void * os_zalloc(size_t size);
void * rtl_zalloc(unsigned int size);


/*
 * The following functions are wrapper for standard ANSI C or POSIX functions.
 * By default, they are just defined to use the standard function name and no
 * os_*.c implementation is needed for them. This avoids extra function calls
 * by allowing the C pre-processor take care of the function name mapping.
 *
 * If the target system uses a C library that does not provide these functions,
 * build_config.h can be used to define the wrappers to use a different
 * function name. This can be done on function-by-function basis since the
 * defines here are only used if build_config.h does not define the os_* name.
 * If needed, os_*.c file can be used to implement the functions that are not
 * included in the C library on the target system. Alternatively,
 * OS_NO_C_LIB_DEFINES can be defined to skip all defines here in which case
 * these functions need to be implemented in os_*.c file for the target system.
 */

//#ifdef OS_NO_C_LIB_DEFINES



/**
 * os_malloc - Allocate dynamic memory
 * @size: Size of the buffer to allocate
 * Returns: Allocated buffer or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer with os_free().
 */
//void * os_malloc(size_t size);
void *rtl_malloc(unsigned int size);

/**
 * os_realloc - Re-allocate dynamic memory
 * @ptr: Old buffer from os_malloc() or os_realloc()
 * @size: Size of the new buffer
 * Returns: Allocated buffer or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer with os_free().
 * If re-allocation fails, %NULL is returned and the original buffer (ptr) is
 * not freed and caller is still responsible for freeing it.
 */
//void * os_realloc(void *ptr, size_t size);
void * rtl_realloc(void *ptr, unsigned int size);

/**
 * os_free - Free dynamic memory
 * @ptr: Old buffer from os_malloc() or os_realloc(); can be %NULL
 */
//void os_free(void *ptr);
void rtl_free(void *ptr);

/**
 * os_memcpy - Copy memory area
 * @dest: Destination
 * @src: Source
 * @n: Number of bytes to copy
 * Returns: dest
 *
 * The memory areas src and dst must not overlap. rtl_os_memmove() can be used with
 * overlapping memory.
 */
//void * os_memcpy(void *dest, const void *src, size_t n);
void * rtl_memcpy(void *dest, const void *src, size_t n);

int rtl_memset(void *dest, int value, unsigned int len);

int rtl_memcpy_to_user(void *dest, void *src, unsigned int count);

void *rtl_malloc_debug(unsigned int size,const char * file,unsigned int line);

void rtl_init_listhead(struct list_head *list);

uint32 rtl_is_list_empty(struct list_head *phead);

void rtl_list_insert_tail(struct list_head *plist, struct list_head *phead);

void rtl_list_delete(struct list_head *plist);



/**
 * rtl_os_memmove - Copy memory area
 * @dest: Destination
 * @src: Source
 * @n: Number of bytes to copy
 * Returns: dest
 *
 * The memory areas src and dst may overlap.
 */
//void * rtl_os_memmove(void *dest, const void *src, size_t n);

/**
 * rtl_os_memset - Fill memory with a constant byte
 * @s: Memory area to be filled
 * @c: Constant byte
 * @n: Number of bytes started from s to fill with c
 * Returns: s
 */
//void * rtl_os_memset(void *s, int c, size_t n);

/**
 * rtl_os_memcmp - Compare memory areas
 * @s1: First buffer
 * @s2: Second buffer
 * @n: Maximum numbers of octets to compare
 * Returns: An integer less than, equal to, or greater than zero if s1 is
 * found to be less than, to match, or be greater than s2. Only first n
 * characters will be compared.
 */
//int rtl_os_memcmp(const void *s1, const void *s2, size_t n);

/**
 * rtl_os_strdup - Duplicate a string
 * @s: Source string
 * Returns: Allocated buffer with the string copied into it or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer with os_free().
 */
//char * rtl_os_strdup(const char *s);

/**
 * rtl_os_strlen - Calculate the length of a string
 * @s: '\0' terminated string
 * Returns: Number of characters in s (not counting the '\0' terminator)
 */
//size_t rtl_os_strlen(const char *s);

/**
 * rtl_os_strcasecmp - Compare two strings ignoring case
 * @s1: First string
 * @s2: Second string
 * Returns: An integer less than, equal to, or greater than zero if s1 is
 * found to be less than, to match, or be greatred than s2
 */
//int rtl_os_strcasecmp(const char *s1, const char *s2);

/**
 * rtl_os_strncasecmp - Compare two strings ignoring case
 * @s1: First string
 * @s2: Second string
 * @n: Maximum numbers of characters to compare
 * Returns: An integer less than, equal to, or greater than zero if s1 is
 * found to be less than, to match, or be greater than s2. Only first n
 * characters will be compared.
 */
//int rtl_os_strncasecmp(const char *s1, const char *s2, size_t n);

/**
 * rtl_os_strchr - Locate the first occurrence of a character in string
 * @s: String
 * @c: Character to search for
 * Returns: Pointer to the matched character or %NULL if not found
 */
//char * rtl_os_strchr(const char *s, int c);

/**
 * rtl_os_strrchr - Locate the last occurrence of a character in string
 * @s: String
 * @c: Character to search for
 * Returns: Pointer to the matched character or %NULL if not found
 */
//char * rtl_os_strrchr(const char *s, int c);

/**
 * rtl_os_strcmp - Compare two strings
 * @s1: First string
 * @s2: Second string
 * Returns: An integer less than, equal to, or greater than zero if s1 is
 * found to be less than, to match, or be greatred than s2
 */
//int rtl_os_strcmp(const char *s1, const char *s2);

/**
 * rtl_os_strncmp - Compare two strings
 * @s1: First string
 * @s2: Second string
 * @n: Maximum numbers of characters to compare
 * Returns: An integer less than, equal to, or greater than zero if s1 is
 * found to be less than, to match, or be greater than s2. Only first n
 * characters will be compared.
 */
//int rtl_os_strncmp(const char *s1, const char *s2, size_t n);

/**
 * rtl_os_strncpy - Copy a string
 * @dest: Destination
 * @src: Source
 * @n: Maximum number of characters to copy
 * Returns: dest
 */
//char * rtl_os_strncpy(char *dest, const char *src, size_t n);

/**
 * rtl_os_strstr - Locate a substring
 * @haystack: String (haystack) to search from
 * @needle: Needle to search from haystack
 * Returns: Pointer to the beginning of the substring or %NULL if not found
 */
//char * rtl_os_strstr(const char *haystack, const char *needle);

/**
 * rtl_os_snprintf - Print to a memory buffer
 * @str: Memory buffer to print into
 * @size: Maximum length of the str buffer
 * @format: printf format
 * Returns: Number of characters printed (not including trailing '\0').
 *
 * If the output buffer is truncated, number of characters which would have
 * been written is returned. Since some C libraries return -1 in such a case,
 * the caller must be prepared on that value, too, to indicate truncation.
 *
 * Note: Some C library implementations of snprintf() may not guarantee null
 * termination in case the output is truncated. The OS wrapper function of
 * rtl_os_snprintf() should provide this guarantee, i.e., to null terminate the
 * output buffer if a C library version of the function is used and if that
 * function does not guarantee null termination.
 *
 * If the target system does not include snprintf(), see, e.g.,
 * http://www.ijs.si/software/snprintf/ for an example of a portable
 * implementation of snprintf.
 */
//int rtl_os_snprintf(char *str, size_t size, const char *format, ...);

//#else /* OS_NO_C_LIB_DEFINES */


//#endif /* OS_NO_C_LIB_DEFINES */

/*
#ifdef OS_REJECT_C_LIB_FUNCTIONS
#define malloc OS_DO_NOT_USE_malloc
#define realloc OS_DO_NOT_USE_realloc
#define free OS_DO_NOT_USE_free
#define memcpy OS_DO_NOT_USE_memcpy
#define memmove OS_DO_NOT_USE_memmove
#define memset OS_DO_NOT_USE_memset
#define memcmp OS_DO_NOT_USE_memcmp
#undef strdup
#define strdup OS_DO_NOT_USE_strdup
#define strlen OS_DO_NOT_USE_strlen
#define strcasecmp OS_DO_NOT_USE_strcasecmp
#define strncasecmp OS_DO_NOT_USE_strncasecmp
#undef strchr
#define strchr OS_DO_NOT_USE_strchr
#undef strcmp
#define strcmp OS_DO_NOT_USE_strcmp
#undef strncmp
#define strncmp OS_DO_NOT_USE_strncmp
#undef strncpy
#define strncpy OS_DO_NOT_USE_strncpy
#define strrchr OS_DO_NOT_USE_strrchr
#define strstr OS_DO_NOT_USE_strstr
#undef snprintf
#define snprintf OS_DO_NOT_USE_snprintf

#define strcpy OS_DO_NOT_USE_strcpy
*/
//#endif /* OS_REJECT_C_LIB_FUNCTIONS */

//#endif /* OS_H */

//#ifndef _OS_FUNCTION_IF_H_
//#define _OS_FUNCTION_IF_H_
//#include "os_api.h"
//#include "tcpip_types.h"
//#include <stdarg.h>

/*******************************************************************************/
#define GPIO_WIFI_IRQ_ID 0

#define RTL_TASK_PRIORITY 27
// Auto start option on thread
#define RTL_SCI_DONT_START              0
#define RTL_SCI_AUTO_START              1

// Indicates if the thread is preemptable.
#define RTL_SCI_NO_PREEMPT              0
#define RTL_SCI_PREEMPT                 1

// Auto start option on timer.
#define RTL_SCI_NO_ACTIVATE             0
#define RTL_SCI_AUTO_ACTIVATE           1

#define RTL_SCI_NO_INHERIT 0
#define RTL_SCI_INHERIT 1

// Option used to set/get event.
#define RTL_SCI_OR                      0
#define RTL_SCI_OR_CLEAR                1
#define RTL_SCI_AND                     2
#define RTL_SCI_AND_CLEAR               3

// Wait option.
#define RTL_SCI_NO_WAIT                 0x0
#define RTL_SCI_WAIT_FOREVER            0xFFFFFFFF
// unsigned char value
#define RTL_SCI_TRUE                    1
#define RTL_SCI_FALSE                   0   

#define RTL_SCI_SUCCESS		0x00

/*******************************************************************************/

typedef  unsigned long     RTL_TCPIP_IP_ADDR_T; 
typedef  unsigned long     RTL_TCPIP_NET_ID_T; 
typedef unsigned char			RTL_BOOLEAN_T;
typedef  unsigned long		RTL_BLOCK_ID;
typedef void    *RTL_EVENT_GROUP_PTR;
typedef unsigned long          RTL_BLOCK_ID;
typedef void    *RTL_SCI_MUTEX_PTR;
typedef void    *RTL_SCI_SEMAPHORE_PTR;

typedef void    (*RTL_TIMER_FUN)(unsigned long);
typedef void    *RTL_TIMER_PTR;


#define  	SFS_MAX_PATH 255
//#define 	SPI_CARD_DEFAULT_CLOCK		24000000

typedef enum
{
	RTL_MESSAGE_START = WIFISUPP_SIG_CUSTOM_START,
	RTL_MESSAGE_LINK_RETRY,
	RTL_MESSAGE_DM_CALLBACK,
	RTL_MESSAGE_SURVEY_CALLBACK,
	RTL_MESSAGE_JOIN_CALLBACK,
	RTL_MESSAGE_REORDER_CALLBACK,
	RTL_MESSAGE_EAPOL_RETRY_CALLBACK,
	RTL_MESSAGE_INACTIVEPS_CALLBACK,
	RTL_MESSAGE_BLINK_CALLBACK,
	RTL_MESSAGE_MAX = WIFISUPP_SIG_CUSTOM_END
}RTL_MESSAGE_E;


typedef enum
{
    RTL_ADAP_TASK_PRIO_1 = 1,
    RTL_ADAP_TASK_PRIO_2,
    RTL_ADAP_TASK_PRIO_3,
    RTL_ADAP_TASK_PRIO_4,
    RTL_ADAP_TASK_PRIO_MAX	//invalid
}RTL_ADAP_TASK_PRIO_E;	// 1~4从高到低分别对应TCPIP(18), SUPPLICANT(26), APP(28) 这三个task划分开的4个区间
								// 1 -> 17
								// 2-> 25
								// 3->27
								// 4->29


// EAPOL data receive upcall function
typedef void (*RTL_TCPIP_EAPOL_RX_CALLBACK_FPTR)( 
    const unsigned char*    data_ptr,       /* EAPOL data pointer */
    unsigned int          data_len,       /* EAPOL data length */
    const unsigned char*    src_mac_ptr,    /* source mac address pointer */
    unsigned int          src_mac_len,    /* source mac length, unit: byte 
                                     * for ethernet, it should be 6 */
    RTL_TCPIP_NET_ID_T   net_id );       /* net interface ID */


// TCPIP interface error enum
typedef enum
{
    RTL_TCPIP_ERROR_OK = 0,     /* no error */
    RTL_TCPIP_ERROR_INVALRTLM, /* invalid parameter */
    RTL_TCPIP_ERROR_INVALNETID, /* invalid net id */
    RTL_TCPIP_ERROR_MEMALLOC,   /* memory alloc fail */
    RTL_TCPIP_ERROR_LOGICAL,    /* calling or running logical error */
    RTL_TCPIP_ERROR_TIMEOUT,    /* time out */
    RTL_TCPIP_ERROR_MAX
} RTL_TCPIP_ERROR_E;


typedef enum
{
    RTL_TCPIP_PKTTYPE_NULL = 0,
    RTL_TCPIP_PKTTYPE_IP,       /* packet is encapsulated as IP */
    RTL_TCPIP_PKTTYPE_ETHER,    /* packet is encapsulated as Ethernet */
    RTL_TCPIP_PKTTYPE_MAX
} RTL_TCPIP_PACKET_TYPE_E;

// TCPIP net interface IP addresses
// NOTES : IP addresses should be in Big-Ending
typedef struct _netif_ipaddr_tag
{
    RTL_TCPIP_IP_ADDR_T  ipaddr;     /* host IP */
    RTL_TCPIP_IP_ADDR_T  snmask;     /* subnet mask */
    RTL_TCPIP_IP_ADDR_T  gateway;    /* gateway */
    RTL_TCPIP_IP_ADDR_T  dns1;       /* primary DNS */
    RTL_TCPIP_IP_ADDR_T  dns2;       /* secondary DNS */
} RTL_TCPIP_NETIF_IPADDR_T;

// TCPIP net interface hardware address
typedef struct _netif_haddr_tag
{
    unsigned char*  addr_ptr;           /* hard address pointer */
    unsigned int  addr_len;           /* hard address length, unit: byte */
} RTL_TCPIP_NETIF_HADDR_T;


// TCPIP packet info struct
typedef struct _packet_info_tag
{
    unsigned char*          data_ptr;   /* data pointer */
    unsigned int          data_len;   /* data length - full packet encapsulation length */
    RTL_TCPIP_NET_ID_T   net_id;     /* net interface ID */
} RTL_TCPIP_PACKET_INFO_T;


typedef unsigned int (*RTL_TCPIP_TX_REQ_FPTR)( 
    const RTL_TCPIP_PACKET_INFO_T*  pkt_info_ptr );

// TCPIP receive flow control setting function 
typedef void (*RTL_TCPIP_RX_FLCTRL_FPTR)(
    RTL_BOOLEAN_T         is_set,     /* flow control flag: TRUE - set; FALSE - not set */
    RTL_TCPIP_NET_ID_T   net_id );   /* net interface ID */



// TCPIP net interface configuration struct
typedef struct _netif_cfg_tag
{
    RTL_TCPIP_PACKET_TYPE_E     pkt_type;       /* packet type between tcpip and low layer */
    RTL_TCPIP_TX_REQ_FPTR       tx_req_fptr;    /* tx function pointer for data sending from tcpip to low layer */
    RTL_BOOLEAN_T                 is_async_tx;    /* tx mode: TRUE - async send; FALSE - sync send */
    RTL_TCPIP_NETIF_IPADDR_T    saddr;          /* soft (IP) address */
    RTL_TCPIP_NETIF_HADDR_T     haddr;          /* hard (MAC) address */
    unsigned int                  mtu;            /* MTU of the low layer */
    unsigned int                  tx_rate;        /* max tx send rate - it's useful in tx flow control */
    RTL_TCPIP_RX_FLCTRL_FPTR    rx_flctrl_fptr; /* rx flow control function pointer, 
                                             * this is optional, can be NULL */
    char*                   netif_name_ptr; /* net interface name, given by low layer,
                                             * it can be NULL, but we recommend caller set one for better debug */
} RTL_TCPIP_NETIF_CFG_T;

/*******************************************************************************/
// Signal head structure.
// Signal vars used when send signals from one task to anther.
// The pointer is a structure whose head is SIGNAL_VARS.
#define _RTL_SIGNAL_VARS \
    unsigned short SignalCode; \
    unsigned short SignalSize; \
    rtl_xSignalHeader  Pre; \
    rtl_xSignalHeader  Suc; \
    RTL_BLOCK_ID       Sender;

//#ifndef _BSSIM
    #define RTL_SIGNAL_VARS	_RTL_SIGNAL_VARS
//#else
//   #define RTL_RTL_SIGNAL_VARS _RTL_RTL_SIGNAL_VARS
//   void  *SigP;
//#endif // End of _BSSIM

// Signal head structure.
typedef struct rtl_xSignalHeaderStruct *rtl_xSignalHeader;
typedef struct rtl_xSignalHeaderStruct {
  RTL_SIGNAL_VARS
} rtl_xSignalHeaderRec;

/*******************************************************************************/
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#if 0
#define RTL_WIFISUPP_BSSID_LEN              6               //the length of BSSID in bytes
#define RTL_WIFISUPP_SSID_MAX_LEN           32              //the MAX length of SSID in bytes
#define RTL_WIFISUPP_WEP_IV_LEN             3               //the length of IV in WEP
#define RTL_WIFISUPP_WEP_64BITS_KEY_LEN     8               //the length of 64bits WEP key
#define RTL_WIFISUPP_WEP_128BITS_KEY_LEN    16              //the length of 128bits WEP key
#define RTL_WIFISUPP_WPA_PSK_LEN            63              //the length of WPA PSK
#define RTL_WIFISUPP_WEP_KEY_ID_MAX         4               //the max WEP key ID

#define RTL_WIFISUPP_MAX_USERNAME_LEN       32
#define RTL_WIFISUPP_MAX_PSW_LEN            32


typedef enum
{
    RTL_WIFISUPP_NETWORK_MODE_INFRASTRUCTURE,   //infrastructure
    RTL_WIFISUPP_NETWORK_MODE_ADHOC,            //ad-hoc
    RTL_WIFISUPP_NETWORK_MODE_MAX
} RTL_WIFISUPP_NETWORK_MODE_E;


typedef enum
{
    RTL_WIFISUPP_RESULT_SUCC,           //success
    RTL_WIFISUPP_RESULT_FAIL,           //fail
    RTL_WIFISUPP_RESULT_NOT_FOUND,      //the AP is not found
    RTL_WIFISUPP_RESULT_TIMEOUT,        //operation time out
    RTL_WIFISUPP_RESULT_RESTRICTED,     //connection is restricted
    RTL_WIFISUPP_RESULT_MAX
} RTL_WIFISUPP_RESULT_E;

typedef enum
{
    RTL_WIFISUPP_ENCRYP_PROTOCOL_OPENSYS,   //open system
    RTL_WIFISUPP_ENCRYP_PROTOCOL_WEP,       //WEP
    RTL_WIFISUPP_ENCRYP_PROTOCOL_WPA,       //WPA
    RTL_WIFISUPP_ENCRYP_PROTOCOL_WPA2,      //WPA2
    RTL_WIFISUPP_ENCRYP_PROTOCOL_WAPI,      //WAPI: Not support in this version
    RTL_WIFISUPP_ENCRYP_PROTOCOL_MAX
}RTL_WIFISUPP_ENCRYP_PROTOCOL_E;

typedef enum
{
    RTL_WIFISUPP_WEP_KEY_TYPE_64BITS,       //64bits_type WEP Key
    RTL_WIFISUPP_WEP_KEY_TYPE_128BITS,      //128bits_type WEP Key
    RTL_WIFISUPP_WEP_KEY_TYPE_MAX
}RTL_WIFISUPP_WEP_KEY_TYPE_E;

typedef enum
{
    RTL_WIFISUPP_WPA_CREDENTIAL_TYPE_PSK,   //WPA using PSK
    RTL_WIFISUPP_WPA_CREDENTIAL_TYPE_EAP,   //WPA using EAP
    RTL_WIFISUPP_WPA_CREDENTIAL_TYPE_MAX
}RTL_WIFISUPP_WPA_CREDENTIAL_TYPE_E;

typedef enum
{
    RTL_WIFISUPP_WPA_EAP_TYPE_TLS,          //EAP using TLS
    RTL_WIFISUPP_WPA_EAP_TYPE_TTLS,         //EAP using TTLS
    RTL_WIFISUPP_WPA_EAP_TYPE_PEAP,         //EAP using PEAP
    RTL_WIFISUPP_WPA_EAP_TYPE_LEAP,         //EAP using LEAP
    RTL_WIFISUPP_WPA_EAP_TYPE_SIM,          //EAP using SIM
    RTL_WIFISUPP_WPA_EAP_TYPE_AKA,          //EAP using AKA
    RTL_WIFISUPP_WPA_EAP_TYPE_FAST,         //EAP using FAST
    RTL_WIFISUPP_WPA_EAP_TYPE_MAX
} RTL_WIFISUPP_WPA_EAP_TYPE_E;

typedef enum
{
    RTL_WIFISUPP_INNER_CREDENTIAL_TYPE_CHAP,        //CHAP credential
    RTL_WIFISUPP_INNER_CREDENTIAL_TYPE_PAP,         //PAP credential
    RTL_WIFISUPP_INNER_CREDENTIAL_TYPE_MSCHAP,      //MSCHAP credential
    RTL_WIFISUPP_INNER_CREDENTIAL_TYPE_MSCHAP_V2,   //MACHAP_V2 credential
    RTL_WIFISUPP_INNER_CREDENTIAL_TYPE_GTC,         //GTC credential
    RTL_WIFISUPP_INNER_CREDENTIAL_TYPE_MAX
} RTL_WIFISUPP_INNER_CREDENTIAL_TYPE_E;
#endif

typedef enum _RTL_SUPP_MESSAGE{
    RTL_WIFISUPP_SIG_START = 1,
	RTL_WIFISUPP_SIG_ON_REQ,
	RTL_WIFISUPP_SIG_ON_CNF,
	RTL_WIFISUPP_SIG_OFF_REQ,
	RTL_WIFISUPP_SIG_OFF_CNF,
	RTL_WIFISUPP_SIG_SCAN_REQ,
	RTL_WIFISUPP_SIG_SCAN_IND,
	RTL_WIFISUPP_SIG_SCAN_CNF,
	RTL_WIFISUPP_SIG_CONNECT_REQ,
	RTL_WIFISUPP_SIG_CONNECT_CNF,
	RTL_WIFISUPP_SIG_DISCONNECT_REQ,
	RTL_WIFISUPP_SIG_DISCONNECT_CNF,
	RTL_WIFISUPP_SIG_DISCONNECT_IND,
	RTL_WIFISUPP_SIG_BSSID_CHANGED_IND,
	RTL_WIFISUPP_SIG_RSSI_CHANGED_IND,
	RTL_WIFISUPP_SIG_CUSTOM_START,
	RTL_WIFISUPP_SIG_CUSTOM_END = RTL_WIFISUPP_SIG_CUSTOM_START + 50,
	RTL_WIFISUPP_SIG_END
}RTL_SUPP_SIG_E;

#if 0
typedef struct
{
    RTL_WIFISUPP_RESULT_E   wifi_on_result;
    unsigned int              net_id;             //the net id got from TCP/IP
    RTL_unsigned char_T             is_support_roam;    //wether support roam
    RTL_unsigned char_T             is_support_protocol[RTL_WIFISUPP_ENCRYP_PROTOCOL_MAX];  //the encrypt protocol supported
    RTL_unsigned char_T             is_support_eap[RTL_WIFISUPP_WPA_EAP_TYPE_MAX];          //the EAP methods supported
    RTL_unsigned char_T             is_support_inner_eap[RTL_WIFISUPP_INNER_CREDENTIAL_TYPE_MAX];   //the inner eap methos supported
} RTL_WIFISUPP_ON_RESULT_T; // wifi on result

/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
typedef struct
{
    unsigned char ssid[RTL_WIFISUPP_SSID_MAX_LEN];
    unsigned char ssid_len;
} RTL_WIFISUPP_SSID_T;  //SSID

typedef struct
{
    unsigned char bssid[RTL_WIFISUPP_BSSID_LEN];
} RTL_WIFISUPP_BSSID_T; //BSSID




typedef struct
{
    RTL_WIFISUPP_RESULT_E   result;
    RTL_WIFISUPP_SSID_T     ssid;
    RTL_WIFISUPP_BSSID_T    bssid_info;
} RTL_WIFISUPP_CONNECT_RESULT_T;    //connect result

typedef struct 
{
    RTL_WIFISUPP_RESULT_E   wifi_off_result;
} RTL_WIFISUPP_OFF_RESULT_T;    //wifi off result



typedef struct
{
    RTL_WIFISUPP_SSID_T                 ssid;
    RTL_WIFISUPP_BSSID_T                bssid_info;
    unsigned int                          signal_qua;         //signal quality: RSSI
    unsigned int                          noise;              //SNR: signal noise ratio
    unsigned int                          beacon_interval;    //the BEACON interval
    unsigned int                          channel;            //the channel using
    RTL_WIFISUPP_ENCRYP_PROTOCOL_E      encryp_protocol;    //the protocol used by encryption    
    unsigned int                          rate;               //the rate
    RTL_WIFISUPP_NETWORK_MODE_E         network_mode;       //network mode
} RTL_WIFISUPP_SCAN_AP_INFO_T;  //the AP info scanned


typedef struct
{
    _RTL_RTL_SIGNAL_VARS
    RTL_WIFISUPP_RESULT_E           scan_result;
} RTL_WIFISUPP_SCAN_CNF_T;  //a scan is finished in a period


typedef struct 
{
    RTL_WIFISUPP_RESULT_E   result;
    RTL_WIFISUPP_SSID_T     ssid;
} RTL_WIFISUPP_DISCONNECT_RESULT_T; //disconnect result

typedef struct
{
    RTL_WIFISUPP_SSID_T     ssid;
} RTL_WIFISUPP_DISCONNECT_INFO_T;   //disconnect information
#endif

/*******************************************************************************/
typedef enum {
	RTL_LOGGING_LEVEL_NONE 	 = 0x0001,
	RTL_LOGGING_LEVEL_NORMAL	 = 0x0002,
	RTL_LOGGING_LEVEL_VERBOSE = 0x0004,
	RTL_LOGGING_LEVEL_MAX		 = 0x0008
}RTL_LOGGING_LEVEL_T;

/*******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define RTL_WIFISUPP_BSSID_LEN              6               //the length of BSSID in bytes
#define RTL_WIFISUPP_SSID_MAX_LEN           32              //the MAX length of SSID in bytes
#define RTL_WIFISUPP_WEP_IV_LEN             3               //the length of IV in WEP
#define RTL_WIFISUPP_WEP_64BITS_KEY_LEN     8               //the length of 64bits WEP key
#define RTL_WIFISUPP_WEP_128BITS_KEY_LEN    16              //the length of 128bits WEP key
#define RTL_WIFISUPP_WPA_PSK_LEN            63              //the length of WPA PSK
#define RTL_WIFISUPP_MAX_FILE_PATH_LEN      255    //the MAX file path supported
#define RTL_WIFISUPP_WEP_KEY_ID_MAX         4               //the max WEP key ID

#define RTL_WIFISUPP_MAX_USERNAME_LEN       32
#define RTL_WIFISUPP_MAX_PSW_LEN            32
/**--------------------------------------------------------------------------*
 **                         typedef                                          *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          Enum                                             */
/*---------------------------------------------------------------------------*/
typedef enum
{
    RTL_WIFISUPP_NETWORK_MODE_INFRASTRUCTURE,   //infrastructure
    RTL_WIFISUPP_NETWORK_MODE_ADHOC,            //ad-hoc
    RTL_WIFISUPP_NETWORK_MODE_MAX
} RTL_WIFISUPP_NETWORK_MODE_E;

typedef enum
{
    RTL_WIFISUPP_RESULT_SUCC,           //success
    RTL_WIFISUPP_RESULT_FAIL,           //fail
    RTL_WIFISUPP_RESULT_NOT_FOUND,      //the AP is not found
    RTL_WIFISUPP_RESULT_TIMEOUT,        //operation time out
    RTL_WIFISUPP_RESULT_RESTRICTED,     //connection is restricted
    RTL_WIFISUPP_RESULT_MAX
} RTL_WIFISUPP_RESULT_E;

typedef enum
{
    RTL_WIFISUPP_ENCRYP_PROTOCOL_OPENSYS,   //open system
    RTL_WIFISUPP_ENCRYP_PROTOCOL_WEP,       //WEP
    RTL_WIFISUPP_ENCRYP_PROTOCOL_WPA,       //WPA
    RTL_WIFISUPP_ENCRYP_PROTOCOL_WPA2,      //WPA2
    RTL_WIFISUPP_ENCRYP_PROTOCOL_WAPI,      //WAPI: Not support in this version
#ifdef WLAN_EAP_SUPPORT
    RTL_WIFISUPP_ENCRYP_PROTOCOL_EAP,      //WAPI
#endif /* WLAN_EAP_SUPPORT */
    RTL_WIFISUPP_ENCRYP_PROTOCOL_MAX
}RTL_WIFISUPP_ENCRYP_PROTOCOL_E;

typedef enum
{
    RTL_WIFISUPP_WEP_KEY_TYPE_64BITS,       //64bits_type WEP Key
    RTL_WIFISUPP_WEP_KEY_TYPE_128BITS,      //128bits_type WEP Key
    RTL_WIFISUPP_WEP_KEY_TYPE_MAX
}RTL_WIFISUPP_WEP_KEY_TYPE_E;

typedef enum
{
    RTL_WIFISUPP_WPA_CREDENTIAL_TYPE_PSK,   //WPA using PSK
    RTL_WIFISUPP_WPA_CREDENTIAL_TYPE_EAP,   //WPA using EAP
    RTL_WIFISUPP_WPA_CREDENTIAL_TYPE_MAX
}RTL_WIFISUPP_WPA_CREDENTIAL_TYPE_E;

typedef enum
{
    RTL_WIFISUPP_WPA_EAP_TYPE_TLS,          //EAP using TLS
    RTL_WIFISUPP_WPA_EAP_TYPE_TTLS,         //EAP using TTLS
    RTL_WIFISUPP_WPA_EAP_TYPE_PEAP,         //EAP using PEAP
    RTL_WIFISUPP_WPA_EAP_TYPE_LEAP,         //EAP using LEAP
    RTL_WIFISUPP_WPA_EAP_TYPE_SIM,          //EAP using SIM
    RTL_WIFISUPP_WPA_EAP_TYPE_AKA,          //EAP using AKA
    RTL_WIFISUPP_WPA_EAP_TYPE_FAST,         //EAP using FAST
    RTL_WIFISUPP_WPA_EAP_TYPE_MAX
} RTL_WIFISUPP_WPA_EAP_TYPE_E;

typedef enum
{
    RTL_WIFISUPP_INNER_CREDENTIAL_TYPE_CHAP,        //CHAP credential
    RTL_WIFISUPP_INNER_CREDENTIAL_TYPE_PAP,         //PAP credential
    RTL_WIFISUPP_INNER_CREDENTIAL_TYPE_MSCHAP,      //MSCHAP credential
    RTL_WIFISUPP_INNER_CREDENTIAL_TYPE_MSCHAP_V2,   //MACHAP_V2 credential
    RTL_WIFISUPP_INNER_CREDENTIAL_TYPE_GTC,         //GTC credential
    RTL_WIFISUPP_INNER_CREDENTIAL_TYPE_MAX
} RTL_WIFISUPP_INNER_CREDENTIAL_TYPE_E;

//bruce add for coexistence: DHCP notice
typedef enum
{
    RTL_WIFISUPP_DHCP_NOTICE_START,		//APP begin to do dhcp
    RTL_WIFISUPP_DHCP_NOTICE_END,			//APP end dhcp
    RTL_WIFISUPP_DHCP_NOTICE_MAX
}RTL_WIFISUPP_DHCP_NOTICE_E;

/*BEGIN: added by George.Liu 01526, 2010/8/23  MS:00196819*/
typedef enum
{
    RTL_WIFISUPP_CIPHER_TKIP,       //TKIP
    RTL_WIFISUPP_CIPHER_CCMP,       //CCMP
    RTL_WIFISUPP_CIPHER_WEP,
    RTL_WIFISUPP_CIPHER_SMS4,       //WAPI SMS-4加密
    RTL_WIFISUPP_CIPHER_MAX
} RTL_WIFISUPP_CIPHER_E;

/*END:   added by George.Liu 01526, 2010/8/23  MS:00196819*/

/*---------------------------------------------------------------------------*/
/*                          Struct                                           */
/*---------------------------------------------------------------------------*/
typedef struct
{
    unsigned char ssid[RTL_WIFISUPP_SSID_MAX_LEN];
    unsigned char ssid_len;
} RTL_WIFISUPP_SSID_T;  //SSID

typedef struct
{
    unsigned char bssid[RTL_WIFISUPP_BSSID_LEN];
} RTL_WIFISUPP_BSSID_T; //BSSID
#define RTL_WIFISUPP_WAPI_PSK_LEN           64              //the length of WAPI PSK,最后1位永远为0,作为终止判断标志
typedef enum
{
    RTL_WIFISUPP_WAPI_CREDENTIAL_TYPE_PSK,      //预共享密钥
    RTL_WIFISUPP_WAPI_CREDENTIAL_TYPE_CER,      //WAPI证书模式
//  RTL_WIFISUPP_WAPI_CREDENTIAL_TYPE_RADIUS,   //使用Radius服务器进行认证(EAP认证)  
    RTL_WIFISUPP_WAPI_CREDENTIAL_TYPE_MAX
} RTL_WIFISUPP_WAPI_CREDENTIAL_TYPE_E;

typedef struct RTL_WIFISUPP_WAPI_PSK_tag
{
    uint8   psk[RTL_WIFISUPP_WAPI_PSK_LEN];
} RTL_WIFISUPP_WAPI_PSK_T;   //WAPI Pre_shared Key

typedef struct
{
    uint8    *as_certificate_buf_ptr;    //证书颁发者的证书，里面包括证书颁发者的public key等信息
    uint32    as_certificate_buf_len; 
    uint8    *asue_certificate_buf_ptr;  //用户证书
    uint32    asue_certificate_buf_len; 
} RTL_WIFISUPP_WAPI_CER_T;

typedef struct RTL_WIFISUPP_WAPI_CREDENTIAL_tag
{
    RTL_WIFISUPP_WAPI_CREDENTIAL_TYPE_E  credential_type;
    union
    {
        RTL_WIFISUPP_WAPI_PSK_T  wapi_psk_info;    
        RTL_WIFISUPP_WAPI_CER_T  wapi_cer_info;
    } credential_info;
} RTL_WIFISUPP_WAPI_CREDENTIAL_T;

typedef enum
{
    RTL_WIFISUPP_CREDENTIAL_TYPE_PSK,      //预共享密钥
    RTL_WIFISUPP_CREDENTIAL_TYPE_RADIUS,   //使用Radius服务器进行认证(EAP认证)  
    RTL_WIFISUPP_CREDENTIAL_TYPE_CER,      //WAPI证书模式
    RTL_WIFISUPP_CREDENTIAL_TYPE_MAX
} RTL_WIFISUPP_CREDENTIAL_TYPE_E;

typedef struct
{
    RTL_WIFISUPP_SSID_T                 ssid;
    RTL_WIFISUPP_BSSID_T                bssid_info;
    unsigned long                          signal_qua;         //signal quality: RSSI
    unsigned long                          noise;              //SNR: signal noise ratio
    unsigned long                          beacon_interval;    //the BEACON interval
    unsigned long                          channel;            //the channel using
    RTL_WIFISUPP_ENCRYP_PROTOCOL_E      encryp_protocol;    //the protocol used by encryption    
    unsigned long                          rate;               //the rate
    RTL_WIFISUPP_NETWORK_MODE_E         network_mode;       //network mode
    
    RTL_WIFISUPP_CIPHER_E               pairwise_cipher;
    RTL_WIFISUPP_CIPHER_E               group_cipher;
    RTL_WIFISUPP_CREDENTIAL_TYPE_E      credential_type;    //AKM类型
} RTL_WIFISUPP_SCAN_AP_INFO_T;  //the AP info scanned

typedef struct
{
    _RTL_SIGNAL_VARS
    RTL_WIFISUPP_SCAN_AP_INFO_T     ap_info;
} RTL_WIFISUPP_SCAN_IND_T;  //scan indication when new ap is scanned

typedef struct
{
    _RTL_SIGNAL_VARS
    RTL_WIFISUPP_RESULT_E           scan_result;
} RTL_WIFISUPP_SCAN_CNF_T;  //a scan is finished in a period

typedef struct
{
    unsigned char   psk[RTL_WIFISUPP_WPA_PSK_LEN];
}RTL_WIFISUPP_WPA_PSK_T;    //WPA Pre_shared Key

typedef struct
{
    unsigned char     is_use_file;
    unsigned short    certificate_file_path[RTL_WIFISUPP_MAX_FILE_PATH_LEN];  //the tls certificate file path, this MAY NOT available when using buffer
    unsigned char       *certificate_buf_ptr;   //the tls certificate buffer, this MAY NOT available when using file
    unsigned long      certificate_buf_len;    //the tls certificate buffer length, this MAY NOT available when using file
} RTL_WIFISUPP_WPA_EAP_TLS_T;   //EAP_TLS information

typedef struct
{
    RTL_WIFISUPP_INNER_CREDENTIAL_TYPE_E    inner_credential_type;
    char                                username_arr[RTL_WIFISUPP_MAX_USERNAME_LEN+1];
    char                                psw_arr[RTL_WIFISUPP_MAX_PSW_LEN+1];
} RTL_WIFISUPP_WPA_EAP_TTLS_PEAP_T; //EAP_TTLS/EAP_PEAP information

typedef struct
{
    char    username_arr[RTL_WIFISUPP_MAX_USERNAME_LEN+1];
    char    psw_arr[RTL_WIFISUPP_MAX_PSW_LEN+1];
}RTL_WIFISUPP_WPA_EAP_LEAP_T;   //EAP_LEAP information

typedef struct
{
    unsigned char     is_use_file;
    unsigned short    pac_file_path[RTL_WIFISUPP_MAX_FILE_PATH_LEN];//the FAST PAC file path, this MAY NOT available when using buffer
    unsigned char       *pac_buf_ptr;   //the FAST PAC buffer, this MAY NOT available when using file
    unsigned long      pac_buf_len;    //the FAST PAC buffer length, this MAY NOT available when using file
    char        username_arr[RTL_WIFISUPP_MAX_USERNAME_LEN+1];
    char        psw_arr[RTL_WIFISUPP_MAX_PSW_LEN+1];
}RTL_WIFISUPP_WPA_EAP_FAST_T;   //EAP_FAST information

typedef struct 
{ 
//    MN_DUAL_SYS_E simcard_num; //0表示卡1，1表示卡2，以此类推 
	int simcard_num; //0表示卡1，1表示卡2，以此类推

}RTL_WIFISUPP_WPA_EAP_SIM_T; 

typedef struct
{
    RTL_WIFISUPP_SSID_T     ssid;
}RTL_WIFISUPP_DISCONNECT_REQ_INFO_T;    //information in the request for disconnecting a connection

typedef struct
{
    RTL_WIFISUPP_WPA_EAP_TYPE_E eap_type;
    union
    {
        RTL_WIFISUPP_WPA_EAP_TLS_T          tls_info;
        RTL_WIFISUPP_WPA_EAP_TTLS_PEAP_T    ttls_info;
        RTL_WIFISUPP_WPA_EAP_TTLS_PEAP_T    peap_info;
        RTL_WIFISUPP_WPA_EAP_LEAP_T         leap_info;
        RTL_WIFISUPP_WPA_EAP_FAST_T         fast_info;
		RTL_WIFISUPP_WPA_EAP_SIM_T			eapsim_info;//  gaole.zhang add 
    }eap_info;
} RTL_WIFISUPP_WPA_EAP_T; //WPA EAP information

typedef struct
{
    RTL_WIFISUPP_WPA_CREDENTIAL_TYPE_E  credential_type;
    union
    {
        RTL_WIFISUPP_WPA_PSK_T  wpa_psk_info;
        RTL_WIFISUPP_WPA_EAP_T  wpa_eap_info;
    }credential_info;
} RTL_WIFISUPP_WPA_CREDENTIAL_T;    //WPA credential information

typedef struct
{
    RTL_WIFISUPP_WEP_KEY_TYPE_E     key_type;
    unsigned char					 is_open_mode;
    unsigned long						key_in_use;
    union
    {   
        unsigned char   key_64bits_arr[RTL_WIFISUPP_WEP_KEY_ID_MAX][RTL_WIFISUPP_WEP_64BITS_KEY_LEN - RTL_WIFISUPP_WEP_IV_LEN];
        unsigned char   key_128bits_arr[RTL_WIFISUPP_WEP_KEY_ID_MAX][RTL_WIFISUPP_WEP_128BITS_KEY_LEN - RTL_WIFISUPP_WEP_IV_LEN];
    }key;
} RTL_WIFISUPP_WEP_CREDENTIAL_T;    //WEP credential information


typedef struct
{
    RTL_WIFISUPP_SSID_T             ssid;
    RTL_WIFISUPP_BSSID_T            bssid_info;
    RTL_WIFISUPP_NETWORK_MODE_E     network_mode;
    RTL_WIFISUPP_ENCRYP_PROTOCOL_E  encryp_protocol;
    unsigned long                      channel;
    union
    {
        RTL_WIFISUPP_WEP_CREDENTIAL_T   wep_credential;
        RTL_WIFISUPP_WPA_CREDENTIAL_T   wpa_credential;
        RTL_WIFISUPP_WAPI_CREDENTIAL_T  wapi_credential;
    }credential;

    RTL_WIFISUPP_CIPHER_E           pairwise_cipher;
    RTL_WIFISUPP_CIPHER_E           group_cipher;
} RTL_WIFISUPP_SSID_CONFIG_T;   //the config of a SSID

typedef struct
{
    RTL_WIFISUPP_RESULT_E   result;
    RTL_WIFISUPP_SSID_T     ssid;
    RTL_WIFISUPP_BSSID_T    bssid_info;
    uint32             channel;
    int32               rssi;
} RTL_WIFISUPP_CONNECT_RESULT_T;    //connect result

typedef struct
{
    _RTL_SIGNAL_VARS
    RTL_WIFISUPP_CONNECT_RESULT_T   connect_result;
} RTL_WIFISUPP_CONNECT_CNF_T;   //connect confirmation

typedef struct 
{
    RTL_WIFISUPP_RESULT_E   result;
    RTL_WIFISUPP_SSID_T     ssid;
} RTL_WIFISUPP_DISCONNECT_RESULT_T; //disconnect result

typedef struct
{
    _RTL_SIGNAL_VARS
    RTL_WIFISUPP_DISCONNECT_RESULT_T    disconnect_result;
} RTL_WIFISUPP_DISCONNECT_CNF_T;    //disconnect confirmation

typedef struct
{
    RTL_WIFISUPP_SSID_T     ssid;
} RTL_WIFISUPP_DISCONNECT_INFO_T;   //disconnect information

typedef struct
{
    _RTL_SIGNAL_VARS
    RTL_WIFISUPP_DISCONNECT_INFO_T      disc_info;
} RTL_WIFISUPP_DISCONNECT_IND_T;    //disconnect indication

typedef struct
{
    RTL_WIFISUPP_SSID_T     ssid;
    RTL_WIFISUPP_BSSID_T    new_bssid_info;
} RTL_WIFISUPP_BSSID_CHANGED_INFO_T;    //new bssid information

typedef struct
{
    _RTL_SIGNAL_VARS
    RTL_WIFISUPP_BSSID_CHANGED_INFO_T   bssid_changed_info;
} RTL_WIFISUPP_BSSID_CHANGED_IND_T; //BSSID changed indication

typedef struct
{
    RTL_WIFISUPP_SSID_T     ssid;
    RTL_WIFISUPP_BSSID_T    bssid_info;
    unsigned long              new_signal_qua;
} RTL_WIFISUPP_RSSI_CHANGED_INFO_T; //RSSI information

typedef struct
{
    _RTL_SIGNAL_VARS
    RTL_WIFISUPP_RSSI_CHANGED_INFO_T    rssi_changed_info;
} RTL_WIFISUPP_RSSI_CHANGED_IND_T;  //RSSI changed indication

typedef struct
{
    RTL_WIFISUPP_RESULT_E   wifi_on_result;
    unsigned long              net_id;             //the net id got from TCP/IP
    unsigned char             is_support_roam;    //wether support roam
    unsigned char             is_support_protocol[RTL_WIFISUPP_ENCRYP_PROTOCOL_MAX];  //the encrypt protocol supported
    unsigned char             is_support_eap[RTL_WIFISUPP_WPA_EAP_TYPE_MAX];          //the EAP methods supported
    unsigned char             is_support_inner_eap[RTL_WIFISUPP_INNER_CREDENTIAL_TYPE_MAX];   //the inner eap methos supported
} RTL_WIFISUPP_ON_RESULT_T; // wifi on result

typedef struct
{
    _RTL_SIGNAL_VARS
    RTL_WIFISUPP_ON_RESULT_T    on_result;
} RTL_WIFISUPP_ON_CNF_T;    //wifi on confirmation

typedef struct 
{
    RTL_WIFISUPP_RESULT_E   wifi_off_result;
} RTL_WIFISUPP_OFF_RESULT_T;    //wifi off result

typedef struct
{
    _RTL_SIGNAL_VARS
    RTL_WIFISUPP_OFF_RESULT_T   off_result;
} RTL_WIFISUPP_OFF_CNF_T;   //wifi off confirmation


//bruce add for coexistence: DHCP notice
typedef struct
{
    _RTL_SIGNAL_VARS
    RTL_WIFISUPP_DHCP_NOTICE_E   dhcp_notice;
} RTL_WIFISUPP_DHCP_NOTICE_IND_T;   //wifi off confirmation


typedef struct
{
    RTL_WIFISUPP_SSID_T     ssid;
} RTL_WIFISUPP_SCAN_REQ_INFO_T;

enum {
	RTL_MP_OPEN = 0,
	RTL_MP_CLOSE,
	RTL_MP_SET_MACADDR,
	RTL_MP_SET_CHANNEL,
	RTL_MP_SET_RATE,
	RTL_MP_TX_POWER,
	RTL_MP_TX_PKT,
	RTL_MP_SET_RX,
	RTL_MP_QUERY_TRX,
	RTL_MP_CUSTOM,
	RTL_MP_MAX
};
typedef int (*wifi_priv_handler)(char *extra);
typedef struct
{
    int (*wifi_hw_open_func)(RTL_TCPIP_NET_ID_T *netid);			//wifi on func
    int (*wifi_hw_close_func)(void);			//wifi close func
    int (*wifi_interrupt_func)(void);			//spi interrupt func
    int (*wifi_scan_proc_func)(RTL_WIFISUPP_SCAN_REQ_INFO_T *scanReqInfo);
    int (*wifi_connect_req_func)(RTL_WIFISUPP_SSID_CONFIG_T* connectInfo);
    int (*wifi_disconnect_req_func)(RTL_WIFISUPP_DISCONNECT_REQ_INFO_T* disconnectInfo);
    wifi_priv_handler *wifi_mp_handle;
} RTL_PRIV_OPS_T;

/*---------------------------------------------------------------------------*/
unsigned int rtl8723_getThread_Identify(void);

unsigned char rtl8723_eut_set_TX_param(unsigned char on_off, int tx_packet_mode);

unsigned char rtl8723_eut_get_RX_packets(unsigned long *tatal_packets_ptr, unsigned long *err_packets_ptr);

int rtl8723_eut_custom_data(const uint* pBuf);

unsigned char rtl8723_set_eut_mode(unsigned char is_enter);

int rtl8723_set_eut_rxtx_channel(int channel);

int rtl8723_set_eut_tx_rate(long ratio);

int rtl8723_set_eut_tx_pwr(long pwr);

unsigned char rtl8723_eut_set_RX_param(int rx_command);

RTL_SCI_SEMAPHORE_PTR rtl_createSemaphore(char* pName, unsigned int count);

unsigned int rtl_getSemaphore(RTL_SCI_SEMAPHORE_PTR pSem,unsigned int option);

unsigned int rtl_putSemaphore(RTL_SCI_SEMAPHORE_PTR pSem);

void rtl_freeSemaphore(RTL_SCI_SEMAPHORE_PTR pSem);

RTL_BLOCK_ID rtl_createThread(
    const char  *thread_name,           
    const char  *queue_name,            
    void    (*entry)(uint32, void*),    
    uint32  argc,                       
    void    *argv,                      
    uint32  stack_size,                 
    uint32  queue_num,   
    RTL_ADAP_TASK_PRIO_E  para_priority,    
    uint32  preempt,     
    uint32  auto_start   
    );

void rtl_deleteThread(BLOCK_ID thread_id);

void rtl_exitThread(void);

unsigned int rtl_suspendThread(RTL_BLOCK_ID    threadID);

unsigned int rtl_resumeThread(RTL_BLOCK_ID    threadID);

rtl_xSignalHeader rtl_getSignal(RTL_BLOCK_ID threadId);

unsigned int rtl_sendSignal(rtl_xSignalHeader   pSignal, RTL_BLOCK_ID threadId);

void *rtl_createTimer(
    const char  *timer_name,    // Name string of the timer
    RTL_TIMER_FUN   timer_fun,      // Timer callback function
    uint32      input,          // Input value for timer callback function 
    uint32      timer_expire,   // Specifies the timer expire value in     
                                // milliseconds.                           
    uint32      auto_activate   // Option to check if auto active the timer
);

uint32 rtl_changeTimer(RTL_TIMER_PTR timer_ptr,RTL_TIMER_FUN timer_fun,uint32 timer_expire) ;

uint32 rtl_activeTimer(RTL_TIMER_PTR timer_ptr);

unsigned int rtl_deactiveTimer(RTL_TIMER_PTR timer_ptr);

uint32 rtl_deleteTimer(RTL_TIMER_PTR timer_ptr);

RTL_SCI_MUTEX_PTR rtl_createMutex(const char* pName,unsigned int priority);

unsigned int rtl_getMutex(RTL_SCI_MUTEX_PTR pMutex,unsigned int option );

unsigned int rtl_putMutex(RTL_SCI_MUTEX_PTR pMutex);

unsigned int rtl_deleteMutex(RTL_SCI_MUTEX_PTR pMutex);

RTL_EVENT_GROUP_PTR prtl_createEvent(char *pName);

RTL_TCPIP_NET_ID_T rtl_tcpipRegNetInterface(RTL_TCPIP_NETIF_CFG_T* pConfig);

void rtl_TCPIP_DeregNetInterface(RTL_TCPIP_NET_ID_T net_id);

RTL_TCPIP_ERROR_E rtl_tcpipRX(RTL_TCPIP_PACKET_INFO_T* pPacket);

void rtl8723_supp_notifyConnectCnf(const RTL_WIFISUPP_CONNECT_RESULT_T *pResult );

void rtl8723_supp_notifyScanInd(const RTL_WIFISUPP_SCAN_AP_INFO_T*pScanInfo );

void rtl8723_supp_notifyScanCnf(const RTL_WIFISUPP_RESULT_E *pResult);

void rtl8723_supp_notifyDisconnectCnf(const RTL_WIFISUPP_DISCONNECT_RESULT_T *pDiscCnf);

void rtl8723_supp_notifyDisconnectInd(const RTL_WIFISUPP_DISCONNECT_INFO_T *pDiscInd);

void rtl8723_set_BT_status(int status);// 1: start 0: stop

void rtl8723_wifi_supplicant_handle_custom_msg(uint32 msg_id, void *msg_body_ptr);

unsigned char rtl8723_wifi_supplicant_get_connect_info(RTL_WIFISUPP_SCAN_AP_INFO_T* connectionInfo);

void tcmd_main(uint32, char*);

void rtl8723_eut_rxReport(unsigned int*);

void rtl8723_gpioEnableIntCtl(void);

void rtl8723_gpioDisableIntCtl(void);

unsigned int rtl_getSystemTickCount(void);
BOOLEAN rtl8723_GetMAC(unsigned char hwmac[6]);
#endif
#endif

