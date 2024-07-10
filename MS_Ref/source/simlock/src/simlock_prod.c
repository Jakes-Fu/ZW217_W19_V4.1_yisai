/******************************************************************************
 ** File Name:      simlock_prod.c                     *
 ** Author:                                            *
 ** Date:           01/26/2016                                                *
 ** Copyright:          All Rights Reserved.   *                                    *
 ** Description:    simlock prod info update to secureboot                                                       *
 ** 
 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date                  01.26.2016                   *
 ** version              v2  
 ** Description         update simlock IMEI, version to secureboot partition 
 **---------------------------------------------------------------------------*
 **
 ******************************************************************************/
#include "sci_types.h"
#include "os_api.h"
#include "simlock.h"
#include "simlock_prod.h"
#include "simlock_serv.h"
#include "efuse_drvapi.h"

  uint32 simlock_get_uid(uint8* uid)
 {
	char* cmdline_addr = NULL;
	char *tmp = NULL;
	char ori_uid[32] = {0};
	EFUSE_RETURN_E ret;


	ret = EFUSE_HAL_ReadUid(ori_uid);
	if(EFUSE_RESULT_SUCCESS != ret) {
		SCI_TRACE_LOW("SIMLOCK: get uid failed, ret=%d\n", ret);
		return 0;
	}
	//SCI_TRACE_LOW("SIMLOCK: uid: %s\n", ori_uid);

 	sha2(ori_uid, sizeof(ori_uid), uid,0);

 	return 1;
 }
 BOOLEAN Simlock_Prod_Update(uint8 *imei_arr)
{
	//RESERVED for other product parameter

	return TRUE;
}
