
#ifdef WIN32 
#include <stdio.h>
#include "sci_types.h"

uint8 *rda_sm_base=NULL;
int32 ipc_ch_open(int32 ch_id, struct smd_ch **ch, void *priv,
		void (*notify) (void *, uint32))
{
	return 0;
}

void ipc_enable_ch_irq(struct smd_ch *ch)
{

}

int32 ipc_free_dl_ps_buf(struct smd_ch *ch, uint32 offset)
{
	return 0;	
}

int32 ipc_ch_read(struct smd_ch *ch, void *data, uint32 len)
{
	return 0;
}
int32 ipc_ch_read_avail(struct smd_ch *ch)
{
	return 0;
}

void ipc_disable_ch_irq(struct smd_ch *ch)
{
	
}

#endif

