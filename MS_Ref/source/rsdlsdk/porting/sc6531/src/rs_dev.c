#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "stdlib.h"
#include "rs_datatype.h"
#include "rs_dev.h"
#include "rs_mem.h"
#include "rs_error.h"
#include "rs_flash_operate.h"

// 6531相关头文件
#include "os_api.h"
#include "nv_item_id.h"
#include "mn_type.h"
#include "nvitem.h"
#include "version.h"
#include "sci_service.h"
#include "mn_events.h"
#include "sig_code.h"
#include "sci_api.h"





rs_s32 rs_device_init()
{
	rs_s32 errCode = RS_ERR_OK;

	errCode = rs_flash_init();

	return errCode;
}




