#include "sci_types.h"
#include "rsplatform.h"
#include "rs_ua_fota_porting.h"

extern rs_bool rs_ua_setRecoveryPhase();


void rs_ua_execute(uint32 romAddr, uint32 deltaAddr)
{
	e_updateState update_state = UPSTATE_IDLE;
	rs_u32 ret = 0;

	rs_ua_setStartAddr(romAddr);
	rs_ua_setDeltaPartitionAddr(deltaAddr);
	
	rs_ua_flash_init();
	update_state = rs_ua_getUpdateState();
	if (update_state == UPSTATE_UPDATE)
	{
		rs_ua_setRecoveryPhase();
	}
	
	switch(update_state)
	{
		case UPSTATE_IDLE:
			break;
			
		case UPSTATE_CHECK:
			ret = rs_ua_validation();
			if(ret == 0)
			{
				ret = rs_ua_upgrade();
			}

			break;
			
		case UPSTATE_UPDATE:
			ret = rs_ua_upgrade();
			
			break;
			
		case UPSTATE_UPDATEED:
			break;
			
		default:
			break;
	}
}

