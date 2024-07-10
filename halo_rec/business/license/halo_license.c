
#include "system_hal.h"

#include "rsa_sprd.h"
#include "halo_factory_api.h"
//#include "license_key.h"

extern uint8 is_apcpverify_finished(void);
LOCAL uint8 HALO_verify_License();
LOCAL uint8 HALO_Get_featureList(uint8* F_list);
extern void halo_set_license_module(HALO_LICENSE_MODULE_T* mod);



LOCAL const HALO_LICENSE_MODULE_T Halo_Lic = {
	.Module_id = MODULE_ID,
	.name = "halo_license",
	.verify_License = HALO_verify_License,
	.Get_featureList = HALO_Get_featureList,
};

LOCAL struct LIC_CTRL
{
	HALO_FACTORY_MODULE_T* fac_ptr;
	HALO_APCP_MODULE_T * apap_ptr;
}Lic_Ctrl = {NULL}; 

PUBLIC uint8 license_init(void)
{
	uint8 ret = 0;
	halo_set_license_module(&Halo_Lic);
	halo_get_apap_module(&Lic_Ctrl.apap_ptr, Halo_Lic.Module_id);
	halo_get_factory_module(&Lic_Ctrl.fac_ptr, Halo_Lic.Module_id);
	return ret;
}

LOCAL uint8 HALO_verify_License()
{
	uint8 ret = 0; 
	ret = Lic_Ctrl.apap_ptr->is_apcpverify_finished();
	if(ret == TRUE)
	{
		//ret = HALO_Verify_Signature();
		ret = Lic_Ctrl.fac_ptr->verify_License();
	}
	else 
	{
		ret = 0xef; // apcp verify not finished yet
	}
		
	return ret;
}

LOCAL uint8 HALO_Get_featureList(uint8* F_list)
{
	uint16 i = 0;
	uint8 ret = 0;
	
	ret = Lic_Ctrl.apap_ptr->is_apcpverify_finished();
	if(ret == TRUE)
	{
		HALO_ASSERT(F_list != NULL);
		//ret = HALO_get_featurelist(F_list);
		ret = Lic_Ctrl.fac_ptr->Get_featureList(F_list);
		if (ret == TRUE) //get feature list success
		{
			PRTF("Get_featurelist success!\n"); 
			
		}
		else if (ret == 0xe1 || ret == 0xe2) //verify feature list failed
		{
			PRTF("Get_featurelist internal error!\n"); 		
		}	
	}
	else 
	{
		ret = 0xef; // apcp verify not finished yet
	}
	return ret;
}