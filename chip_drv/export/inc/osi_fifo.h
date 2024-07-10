// copy from bbapp/osi_fifo.h

#ifndef _CIRCLE_BUF_H_
#define _CIRCLE_BUF_H_

#include "sci_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    void *data;
    uint32 size;
    uint32 get;
    uint32 put;
} osiFifo_t;

BOOLEAN osiFifoInit(osiFifo_t *fifo, void *data, uint32 size);
int osiFifoPut(osiFifo_t *fifo, const void *data, uint32 size);
int osiFifoGet(osiFifo_t *fifo, void *data, uint32 size);
int osiFifoPeek(osiFifo_t *fifo, void *data, uint32 size);
void osiFifoReset(osiFifo_t *fifo);
void osiFifoSkipBytes(osiFifo_t *fifo, uint32 size);

static uint32 osiFifoBytes(osiFifo_t *fifo) { return fifo->put - fifo->get; }
static uint32 osiFifoSpace(osiFifo_t *fifo) { return fifo->size - osiFifoBytes(fifo); }
static BOOLEAN osiFifoIsFull(osiFifo_t *fifo) { return osiFifoSpace(fifo) == 0; }
static BOOLEAN osiFifoIsEmpty(osiFifo_t *fifo) { return osiFifoBytes(fifo) == 0; }

#ifdef __cplusplus
}
#endif

#endif
