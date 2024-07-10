/******************************************************************************
 ** File Name:      device_info.c                                         *
 ** Author:         gloria.he                                               *
 ** Date:           07/08/2015                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 07/08/2015     gloria      Create.                                   *
** 	18/08/2015     gloria.he    Modify.                                   *
 ******************************************************************************/
#include "device_info.h"
#include "nv_item_id.h"
#include "nvitem.h"
#include "production_test.h"
//#include "version_software_mocor.h"
#include "version.h"

const char* hw_ver[] =
{
    _HWID_V1_,
    _HWID_V2_,
    _HWID_
};
// the NDT structure defined in the nv
 typedef struct
{
	uint8 lp[4];
	uint8 pcode[12];	//product code
	uint8 ndt_version[12];
	uint8 res_version[12];
}DEVICE_NDT_VERSION;


PUBLIC uint8 * HWID_Read(void)
{
    uint8 result = 0;
#ifdef RF_HW_SELFADAPTIVE_ENABLE
    result = PH_GetAdaptiveGpioValue();
#else
    result = 0; //(sizeof(hw_ver)-1)<<1;
#endif
    return hw_ver[result>>1];
}



PUBLIC void HW_VER_Read(char* hw_ver)
{
    strcpy(hw_ver, VERSION_GetInfo(HW_VERSION));
}

PUBLIC ERR_DEVOPER_E Ndt_Version_Read(uint8 * data)
{
	DEVICE_NDT_VERSION *ndt_version;

	if(NULL==data)
		return ERR_INVALID_PARAM;

	ndt_version=SCI_ALLOCA(sizeof(DEVICE_NDT_VERSION));
	if(NULL==ndt_version)
		return ERR_NOT_ENOUGH_RESOURCE;
	memset(ndt_version,0,sizeof(DEVICE_NDT_VERSION));

	if(NVERR_NONE==EFS_NvitemRead(SETTINGS_NV_NDT_VERSION,sizeof(DEVICE_NDT_VERSION),
		(void*)ndt_version) )
	{
		memcpy(data,ndt_version->ndt_version,sizeof(ndt_version->ndt_version));
		SCI_FREE(ndt_version);
		return ERR_NONE;//success
	}
	else
	{
		SCI_FREE(ndt_version);
		return ERR_NV_ACCESS;//falied
	}

}

PUBLIC ERR_DEVOPER_E LP_CODE_Read(uint8 * data)
{
	DEVICE_NDT_VERSION *ndt_version;

	if(NULL==data)
		return ERR_INVALID_PARAM;

	ndt_version=SCI_ALLOCA(sizeof(DEVICE_NDT_VERSION));
	if(NULL==ndt_version)
		return ERR_NOT_ENOUGH_RESOURCE;
	memset(ndt_version,0,sizeof(DEVICE_NDT_VERSION));

	if(NVERR_NONE==EFS_NvitemRead(SETTINGS_NV_NDT_VERSION,sizeof(DEVICE_NDT_VERSION),
		(void*)ndt_version) )
	{
		memcpy(data,ndt_version->lp,sizeof(ndt_version->lp));
		SCI_FREE(ndt_version);
		return ERR_NONE;//success
	}
	else
	{
		SCI_FREE(ndt_version);
		return ERR_NV_ACCESS;//falied
	}

}

PUBLIC ERR_DEVOPER_E PRODUCT_CODE_Read(uint8 * data)
{
	DEVICE_NDT_VERSION *ndt_version;

	if(NULL==data)
		return ERR_INVALID_PARAM;

	ndt_version=SCI_ALLOCA(sizeof(DEVICE_NDT_VERSION));
	if(NULL==ndt_version)
		return ERR_NOT_ENOUGH_RESOURCE;
	memset(ndt_version,0,sizeof(DEVICE_NDT_VERSION));

	if(NVERR_NONE==EFS_NvitemRead(SETTINGS_NV_NDT_VERSION,sizeof(DEVICE_NDT_VERSION),
		(void*)ndt_version) )
	{
		memcpy(data,ndt_version->pcode,sizeof(ndt_version->pcode));
		SCI_FREE(ndt_version);
		return ERR_NONE;//success
	}
	else
	{
		SCI_FREE(ndt_version);
		return ERR_NV_ACCESS;//falied
	}
}



PUBLIC ERR_DEVOPER_E PRODUCT_CODE_Write(uint8 * data)
{
	DEVICE_NDT_VERSION *ndt_version;
	if(NULL==data)
		return ERR_INVALID_PARAM;

	ndt_version=SCI_ALLOCA(sizeof(DEVICE_NDT_VERSION));
	if(NULL==ndt_version)
		return ERR_NOT_ENOUGH_RESOURCE;
	memset(ndt_version,0,sizeof(DEVICE_NDT_VERSION));

	if(NVERR_NONE!=EFS_NvitemRead(SETTINGS_NV_NDT_VERSION,sizeof(DEVICE_NDT_VERSION),
		(void*)ndt_version))
	{
		SCI_FREE(ndt_version);
		return ERR_NV_ACCESS;
	}
	memcpy(ndt_version->pcode,data, sizeof(ndt_version->pcode));
	if(NVERR_NONE==EFS_NvitemWrite(SETTINGS_NV_NDT_VERSION,sizeof(DEVICE_NDT_VERSION), (void*)ndt_version,0))
	{
		SCI_FREE(ndt_version);
		return ERR_NONE;
	}

	else
	{
		SCI_FREE(ndt_version);
		return ERR_NV_ACCESS;
	}

}

PUBLIC  char* LpProductNdt_CODE_Read(void)
{
	DEVICE_NDT_VERSION *ndt_version;
	char str[256] = {0};
	static char straim[256] = {0};
	ndt_version=SCI_ALLOCA(sizeof(DEVICE_NDT_VERSION));
	if(NULL==ndt_version)
		return ERR_NOT_ENOUGH_RESOURCE;
	memset(ndt_version,0,sizeof(DEVICE_NDT_VERSION));

	if(NVERR_NONE==EFS_NvitemRead(SETTINGS_NV_NDT_VERSION,sizeof(DEVICE_NDT_VERSION),(void*)ndt_version))
	{
		sprintf(str, "lp : %s \nproduct code: %s \nndt_version: %s \n", \
				 ndt_version->lp, ndt_version->pcode, ndt_version->ndt_version);
		strcpy(straim,str);
		SCI_FREE(ndt_version);
        return straim;
	}
	else
	{
	      sprintf(str, "LpProductNdt_CODE_Read NV item read error \n" );
		  strcpy(straim,str);
		  SCI_FREE(ndt_version);
          return straim;
	}
}

PUBLIC uint8* PRODUCT_TYPE_Read(void)
{
    uint8 result = 0;

#ifdef RF_HW_SELFADAPTIVE_ENABLE
    result = PH_GetAdaptiveGpioValue();
#endif

    SCI_TRACE_LOW("PH_GetAdaptiveGpioValue result=%d",result);

    if (result & 0x01) {
        return TA_VER_SUK2;
    } else {
        return TA_VER_SUK1;
    }
}

PUBLIC uint8* PRODUCT_NAME_Read(void)
{
    return _PRODUCT_NAME_;
}

PUBLIC uint8* VERSION_Read(void)
{
    return VERSION_GetInfo(PROJECT_VERSION);
}

PUBLIC uint8* OS_NAME_Read(void)
{
    return _OS_NAME_;
}

PUBLIC uint8* PROCESSOR_MODEL_Read(void)
{
    return _PROCESSOR_MODEL_;
}




