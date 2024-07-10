#ifndef _SETH_H
#define _SETH_H

#include <os_api.h>
#include <sci_types.h>

#define SETH_NOTIFY_GET          0x0001
#define SETH_NOTIFY_RECV        0x0002
#define SETH_NOTIFY_STATUS    0x0003

typedef void (*rx_handle)(int32 evt, void *index);

typedef struct Seth_RX_Param
{
    uint32      channel;
    uint32      netid;
    rx_handle 	handle;
    void        *data;
} SETH_RX_PARAM_T;

/*
 * returns boolean SCI_ERROR or SCI_SUCCESS
 */
PUBLIC int32 seth_init (void);

/*
 * returns boolean SCI_ERROR or SCI_SUCCESS
 */
PUBLIC int32 seth_open ( uint32 index, rx_handle handle );

/*
 * returns boolean SCI_ERROR or SCI_SUCCESS
 */
PUBLIC int32 seth_close ( uint32 index );

/*
 * returns  SCI_ERROR or read data len
 */
PUBLIC int32 seth_read ( uint32 index, char **pbuf );

/*
 * returns  SCI_ERROR or write data len
 */
 PUBLIC int32 seth_wirte ( uint32 index, char* pheader, char *pbuf, int32 len);

/*
 * returns boolean SCI_ERROR or SCI_SUCCESS
 */
PUBLIC int32 seth_free( uint32 index, void *pbuf );

/*
 * returns boolean SCI_ERROR or SCI_SUCCESS
 */
PUBLIC int32 seth_destroy (uint32 index);

/*
 * returns boolean SCI_ERROR or SCI_SUCCESS
 */
PUBLIC int32 seth_early_init (void);


#endif /* _SETH_H */
