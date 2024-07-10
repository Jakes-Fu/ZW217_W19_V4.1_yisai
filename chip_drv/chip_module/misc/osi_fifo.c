// copy from bbapp/osi_fifo.c

#include "osi_fifo.h"
#include "os_api.h"
#include <string.h>
#include <stdlib.h>

BOOLEAN osiFifoInit(osiFifo_t *fifo, void *data, uint32 size)
{
    if (fifo == NULL || size == 0)
        return FALSE;

    if (data == NULL)
    {
        data = malloc(size);
        if (data == NULL)
            return FALSE;
    }

    fifo->data = data;
    fifo->size = size;
    fifo->get = 0;
    fifo->put = 0;
    return TRUE;
}

int osiFifoPut(osiFifo_t *fifo, const void *data, uint32 size)
{
    uint32 len, offset, tail;
    if (fifo == NULL || data == NULL || size == 0)
        return 0;

    SCI_DisableIRQ();
    len = osiFifoSpace(fifo);
    if (len > size)
        len = size;

    offset = fifo->put % fifo->size;
    tail = fifo->size - offset;
    if (tail >= len)
    {
        memcpy((char *)fifo->data + offset, data, len);
    }
    else
    {
        memcpy((char *)fifo->data + offset, data, tail);
        memcpy(fifo->data, (char *)data + tail, len - tail);
    }

    fifo->put += len;
    SCI_RestoreIRQ();
    return len;
}

static int _peekInternal(osiFifo_t *fifo, void *data, uint32 size)
{
    uint32 offset, tail;
    uint32 len = osiFifoBytes(fifo);
    if (len > size)
        len = size;

    offset = fifo->get % fifo->size;
    tail = fifo->size - offset;
    if (tail >= len)
    {
        memcpy(data, (char *)fifo->data + offset, len);
    }
    else
    {
        memcpy(data, (char *)fifo->data + offset, tail);
        memcpy((char *)data + tail, fifo->data, len - tail);
    }
    return len;
}

void osiFifoSkipBytes(osiFifo_t *fifo, uint32 size)
{
    uint32 len;
    if (fifo == NULL || size == 0)
        return;

    SCI_DisableIRQ();
    len = osiFifoBytes(fifo);
    if (len > size)
        len = size;
    fifo->get += len;
    SCI_RestoreIRQ();
}

int osiFifoGet(osiFifo_t *fifo, void *data, uint32 size)
{
    int len;
    if (fifo == NULL || data == NULL || size == 0)
        return 0;

    SCI_DisableIRQ();
    len = _peekInternal(fifo, data, size);
    fifo->get += len;
    SCI_RestoreIRQ();
    return len;
}

int osiFifoPeek(osiFifo_t *fifo, void *data, uint32 size)
{
    int len;
    if (fifo == NULL || data == NULL || size == 0)
        return 0;

    SCI_DisableIRQ();
    len = _peekInternal(fifo, data, size);
    SCI_RestoreIRQ();
    return len;
}

void osiFifoReset(osiFifo_t *fifo)
{
    if (fifo != NULL)
    {
        SCI_DisableIRQ();
        fifo->put = fifo->get = 0;
        SCI_RestoreIRQ();
    }
}
