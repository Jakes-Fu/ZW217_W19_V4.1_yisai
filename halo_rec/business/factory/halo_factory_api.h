#ifndef HALO_FACTORY_API_H
#define HALO_FACTORY_API_H
#include "system_hal.h"

#if(PLAY_ROLE == PLAY_AP)
#include "nvitem.h"
#include "nv_item_id.h"
#include "cmddef.h"
typedef NVITEM_ERROR_E HALO_NVITEM_E;
#define HALO_NVERR_NONE  0
#define HALO_NVITEM_Write(I,L,S) EFS_NvitemWrite(I, L,S,0)
#define HALO_NVITEM_Read(I,L,S)  EFS_NvitemRead(I, L,S)
#define _HALO_ReadUid(uid_ptr) EFUSE_HAL_ReadUid(uid_ptr)
#define HALO_Verify_Signature() Verify_Signature()
#define HALO_get_featurelist(list) Get_featurelist(list)
#define HALO_set_featurelist(list) Set_featurelist(list)
#define LICENSE_INDEX  NV_REF_FOR_LICENSE

#elif(PLAY_ROLE == PLAY_CP)
	


typedef uint8 HALO_NVITEM_E;
#define HALO_NVERR_NONE  0

#define HALO_NVITEM_Write(I,L,S) 0
#define HALO_NVITEM_Read(I,L,S) 0
#define _HALO_ReadUid(uid_ptr)  
#define HALO_Verify_Signature() 0
#define HALO_get_featurelist(list)  
#define HALO_set_featurelist(list)  
#define LICENSE_INDEX  0xdeadbeaf 


#endif


#define UIDLEN   8
#define SIGNLEN  128
#define F_LIST_LEN  84 //for 1024bit/2048bit license  64bytes feature 20 bytes hash
#define SIGNMAXLEN 256


typedef struct diag_Rlicense
{
uint8 errorcode;
uint8 signature_len;
uint8 padding0;
uint8 padding1;
#if 0
uint8 system_info[0];
#endif
}DIAG_READ_LICENSE_T;

typedef struct diag_uid
{
uint8 errorcode;
uint8 uid_info[UIDLEN];
uint8 padding0;
uint8 padding1;
uint8 padding2;
}DIAG_UID_T;

typedef struct diag_WLicense
{
uint8 len;
uint8 padding0;
uint8 padding1;
uint8 padding2;
uint8 sign_info[SIGNLEN];
}DIAG_WRITE_LICENSE_T;

typedef struct diag_WLICRET
{
uint8 errorcode;
uint8 padding0;
uint8 padding1;
uint8 padding2;
}DIAG_WRITE_LICRET_T;


typedef struct NV_SIGN_ITEM_T
{
    uint8 data[SIGNMAXLEN];
} NV_SIGN_ITEM_T;

typedef struct NV_EFUSE_SIGN_DATA_T
{
     uint8 length;
     NV_SIGN_ITEM_T signature;
	 
} NV_EFUSE_SIGN_DATA_T;

 uint8  Verify_Signature();
 

#endif

