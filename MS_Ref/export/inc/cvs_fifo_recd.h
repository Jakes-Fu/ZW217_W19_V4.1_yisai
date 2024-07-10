#include "sci_api.h"
#include "sci_types.h"

//#define FIFO_DEBUG(A, B...) SCI_TRACE_LOW("[FIFO][%s] "A, __FUNCTION__, ##B)
#ifndef WIN32
#define FIFO_DEBUG(A, B...)	(A,##B)
#endif

typedef struct block_t block_t;

typedef struct
{
	uint32 depth;
	uint32 depth_limit;			// 0: no limit
	block_t *first_block_p;
	block_t *end_block_p;
}fifo_t;

struct block_t
{
	block_t *last_block_p;
	uint8 *buf;
	uint32 buf_bytes;
	block_t *next_block_p;
};

PUBLIC uint32 Block_Alloc(block_t **block_pp, uint32 buf_bytes);
PUBLIC uint32 Block_Free(block_t *block_p);
PUBLIC uint32 Fifo_Init(fifo_t **fifo_pp, uint32 depth_limit);
PUBLIC uint32 Fifo_Add(fifo_t *fifo_p, block_t *block_p);
PUBLIC uint32 Fifo_Remove(fifo_t *fifo_p, block_t **block_pp);
PUBLIC uint32 Fifo_Deinit(fifo_t *fifo_p);

