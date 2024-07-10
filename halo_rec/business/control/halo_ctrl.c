#include "system_hal.h"
#include <stdint.h>

typedef int32 (* apcpverify_init)(void);
typedef int32 (* apcpverify_deinit)(void);
typedef uint8 (* verify_License)(void);
typedef uint8 (* Get_featureList)(uint8* F_list);

typedef struct halo_ctrl
{
	uint32 Module_id;
    uint8 (*apcp_init)(void); 
	uint8 (*license_init)(void);
	uint8 (*factory_init)(void);
	HALO_LICENSE_MODULE_T* lic_ptr;
	HALO_FACTORY_MODULE_T* fac_ptr;
	HALO_APCP_MODULE_T * apap_ptr;
	

}HALO_CTRL_T;

typedef struct halo_conf
{
    uint8 halo_init_timeout;// seconds
	uint8 halo_apcpverify_timeout;// seconds
	uint16 halo_NV_channel;// channel ID
	uint16 halo_EFUSE_channel; // index ID
	uint8 debug_flag; // open/close flag
}HALO_CONF_T;


extern int32 init_apcpverify(void);
extern int32 deinit_apcpverify(void);
extern uint8 HALO_verify_License();
extern uint8 HALO_Get_featureList(uint8* F_list);

extern  uint8 apcp_init(void);
extern  uint8 license_init(void);
extern  uint8 factory_init(void);

#if(PLAY_ROLE == PLAY_AP)


LOCAL struct module_class 
{
	HALO_LICENSE_MODULE_T* lic_ptr;
	HALO_FACTORY_MODULE_T* fac_ptr;
	HALO_APCP_MODULE_T* apap_ptr;
	HALO_CLASS_DATA_T* share_data_ptr;
} module_set = {NULL};

PUBLIC void halo_set_license_module(HALO_LICENSE_MODULE_T* mod)
{
	HALO_ASSERT(mod != NULL);
	if(mod->Module_id != MODULE_ID)
	{
		return;
	}	
	module_set.lic_ptr = mod;
}

PUBLIC void halo_set_factory_module(HALO_FACTORY_MODULE_T* mod)
{
	HALO_ASSERT(mod != NULL);
	if(mod->Module_id != MODULE_ID)
	{
		return;
	}	
	module_set.fac_ptr = mod;
}

PUBLIC void halo_set_apap_module(HALO_APCP_MODULE_T* mod)
{
	HALO_ASSERT(mod != NULL);
	if(mod->Module_id != MODULE_ID)
	{
		return;
	}	
	module_set.apap_ptr = mod;
}

PUBLIC void halo_set_shared_data(HALO_CLASS_DATA_T* data)
{
	HALO_ASSERT(data != NULL);
	if(data->Module_id != MODULE_ID)
	{
		return;
	}	
	module_set.share_data_ptr = data;
}

PUBLIC uint8 halo_get_license_module(HALO_LICENSE_MODULE_T** mod, uint32 id)
{
	if(module_set.lic_ptr == NULL)
		return 0xe1;
	if(module_set.lic_ptr->Module_id == 0)
		return 0xe2;
	if(id == module_set.lic_ptr->Module_id)
	{
		*mod = module_set.lic_ptr;
		return 0x0;
	}
	else
	{
		return 0xe3;
	}	
}

PUBLIC uint8 halo_get_factory_module(HALO_FACTORY_MODULE_T** mod, uint32 id)
{
	if(module_set.fac_ptr == NULL)
		return 0xe1;
	if(module_set.fac_ptr->Module_id == 0)
		return 0xe2;
	if(id == module_set.fac_ptr->Module_id)
	{
		*mod = module_set.fac_ptr;
		return 0x0;
	}
	else
	{
		return 0xe3;
	}	
}

PUBLIC uint8 halo_get_apap_module(HALO_FACTORY_MODULE_T** mod, uint32 id)
{
	if(module_set.apap_ptr == NULL)
		return 0xe1;
	if(module_set.apap_ptr->Module_id == 0)
		return 0xe2;
	if(id == module_set.apap_ptr->Module_id)
	{
		*mod = module_set.apap_ptr;
		return 0x0;
	}
	else
	{
		return 0xe3;
	}	
}
PUBLIC uint8 halo_get_shared_data(HALO_CLASS_DATA_T** data, uint32 id)
{
	if(module_set.share_data_ptr == NULL)
		return 0xe1;
	if(module_set.share_data_ptr->Module_id == 0)
		return 0xe2;
	if(id == module_set.share_data_ptr->Module_id)
	{
		*data = module_set.share_data_ptr;
		return 0x0;
	}
	else
	{
		return 0xe3;
	}	
}


#if 1	
LOCAL  HALO_CTRL_T Halo_Ctrl = {
	.Module_id = MODULE_ID,
	.apcp_init = apcp_init,
	.license_init = license_init,
	.factory_init = factory_init,
};

LOCAL  HALO_CLASS_DATA_T share_data = {
	.Module_id = MODULE_ID,
};
#else
LOCAL  HALO_CTRL_T Halo_Ctrl={
	apcp_init,
	license_init,
	factory_init,
};
#endif

PUBLIC uint8 halo_ctrl_init(uint8* conf)
{
	HALO_CONF_T* conf_ptr = NULL;
	HALO_ASSERT(conf != NULL);
	
	if (*conf == 0x03)
	{
		share_data.calib_mode =0xca;
	}	
	else
	{
		share_data.calib_mode =0x0;
		//conf_ptr = (HALO_CONF_T*)conf;
	}
	share_data.padding = *conf;
	

	
	Halo_Ctrl.apcp_init();
	Halo_Ctrl.factory_init();
	Halo_Ctrl.license_init();
	halo_get_apap_module(&Halo_Ctrl.apap_ptr,Halo_Ctrl.Module_id);
	halo_get_factory_module(&Halo_Ctrl.fac_ptr,Halo_Ctrl.Module_id);
	halo_get_license_module(&Halo_Ctrl.lic_ptr,Halo_Ctrl.Module_id);
	halo_set_shared_data(&share_data);
	Halo_Ctrl.apap_ptr->apcpverify_init();
	
	return 0;
}

PUBLIC uint8 halo_ctrl_verify_license(void)
{
	uint8 ret =0;
	ret = Halo_Ctrl.lic_ptr->verify_License();
	return ret;
}

PUBLIC uint8 halo_ctrl_get_featureList(uint8* F_list)
{
	uint8 ret =0;
	ret = Halo_Ctrl.lic_ptr->Get_featureList(F_list);
	return ret;
}

PUBLIC uint8 halo_ctrl_exit(uint8* param)
{
	Halo_Ctrl.apap_ptr->apcpverify_deinit();
	return 0;
}

#endif