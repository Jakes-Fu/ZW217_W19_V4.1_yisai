
#include "boot1_copy_kernel_img.h"
#include "sci_types.h"
#include "arm_reg.h"
#include "sc_reg.h"
#include "nlc_drv.h"
#include "chng_freq.h"
#include "chip.h"
#include "sci_ftl_api.h"

#define BL_TRACE_ENABLED
#include "bl_trace.h"

extern void set_pc (int value);
static void call_kernel (void);

PUBLIC int main(void)
{
	uint32 nandId;
	BOOLEAN cpyRet;

//	MMU_Init();			// no choice , because of limitation of code size

    BL_TRACE_OPEN();
    BL_TRACE0 ("The second boot loader started ...\r\n");

	NANDCTL_Init (100000000);
	SCI_FTL_MODULE_Init();
	nandId = 0;
	do
	{
		if(SCI_FTL_ERROR_NONE != SCI_FTL_Load(nandId))
		{
			BL_TRACE ("SCI_FTL_Load failed.\r\n");
			break;
		}
		cpyRet = LoadImg();

		SCI_FTL_UnLoad(nandId);

		if (TRUE == cpyRet)
		{
			call_kernel();
		}
	}
	while (0);

	for(;;){}
	// should add power off here. Jason.Wu
	return 0;	/*lint !e527*/
}

/******************************************************************************
 * call_kernel
 ******************************************************************************/
PUBLIC void call_kernel (void)
{
    set_pc (SDRAM_BASE_ADDR);
}


