

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/27/2016                      Create.                                   *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

 #include "spzip_api.h"
 #include "efs_comm.h" 
 #include "nv_type.h"
 #include "load_modem.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

extern void ipc_register_exec_handler(void (*handler) (void));
extern uint8 *ipc_get_ims_nv_buf_addr(void);
extern uint8 *ipc_get_ims_nv_buf_info_addr(void);
extern void ipc_set_delta_nv_bin_addr(uint32 addr, uint32 len);

#define MEM_INDEX_LIST_BASE 0x80000000  
#define MEM_ID_MAX          32   // 1024 / 32
typedef struct T_MEM_INDEX_Tag
{
	char name[MEM_NAME_MAX_SIZE];
	uint32  addr;
	uint32  len;	
	uint32  cpneed;
}T_MEM_INDEX;

typedef struct T_MEM_INDEX_LIST_Tag
{
 	T_MEM_INDEX index_unit[MEM_ID_MAX];
}T_MEM_INDEX_LIST;




uint32 v_cp_jump_addr;
uint32 v_zsp_jump_addr;

static int v_load_modem_is_finish = 0;
static ims_nv_info_t *ims_nv_buf_addr_info;
static const uint32 s_dp_ap_ro[] = {0x3, 0x0, 0x2, 0x0};

/*****************************************************************************/
 //  Description:    Get the dump ip register save address
//	Param: 
//  Return:
//  return dump save address
//	Note:
/*****************************************************************************/
T_MEM_RELAY_ADDRESS_AREA* GET_IPRegisterDumpAreaSaveAddr(void)
{
    int i = 0;
   T_MEM_INDEX_LIST *pt_mem_list = (T_MEM_INDEX_LIST *)MEM_INDEX_LIST_BASE;
  T_MEM_RELAY_ADDRESS_AREA* pt_relay_address_area;
   
     for(i = 0; i < MEM_ID_MAX; i++)
    {
            if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_RELAY_ADDR_AREA_NAME) == 0)
             {
		  return (T_MEM_RELAY_ADDRESS_AREA*)pt_mem_list->index_unit[i].addr;		 
             }
    }  

   return NULL;
}


/*****************************************************************************/
 //  Description:    Get the memory address and length 
 //              from memory index list 
//	Param: 
//  @name: memory name string,defined the Load_modem.h
//  @len: if @len isn,t equal NULL,memory len from memory index list
//  Return:
//  return memory start address
//  @len: if @len isn,t equal NULL,return memory len from memory index list
//	Note:
/*****************************************************************************/
void* GET_MemoryStartAddressAndLen(char *name, uint32 *len)
{
    int i = 0;
   T_MEM_INDEX_LIST *pt_mem_list = (T_MEM_INDEX_LIST *)MEM_INDEX_LIST_BASE;
   
   if(NULL == name)			    
     return NULL;
	
     for(i = 0; i < MEM_ID_MAX; i++)
    {
            if(strcmp((char *)pt_mem_list->index_unit[i].name, name) == 0)
             {
                 if(NULL != len)			    
               	*len  = pt_mem_list->index_unit[i].len;
				 
 	         return (void*)pt_mem_list->index_unit[i].addr;
             }
    }  

   return NULL;
}

static int MEM_bzpLoad(LZMA_BZP_IDX_T idx, uint8*load_addr,uint32 load_len)
{
    uint32 unzip_data_len = 0;
    uint32 i=0;
    
    LZMA_INPUT_T  in  = {0};
    LZMA_OUTPUT_T out = {0};

    in.zip_buf_addr   = LZMA_GetBzpBinPtr(idx);
    in.zip_buf_len    = LZMA_GetBzpBinSize(idx);
    unzip_data_len = LZMA_GetBzpDecSize(idx);
    //mon_Event(0x10adf000);
    //mon_Event(idx&0xff);
    //mon_Event(in.zip_buf_addr);
    //mon_Event( in.zip_buf_len);
   // mon_Event(unzip_data_len);
    //mon_Event(load_len);
    //mon_Event(load_addr);
    SCI_TraceLow("Load %d,bzp_addr:0x%x bzp_len:0x%x unzip_len:0x%x load_addr:0x%x load_len:0x%x\n",idx,in.zip_buf_addr,in.zip_buf_len,unzip_data_len,(uint32)load_addr,load_len);

    if((uint32)load_addr%8!=0)
    {
      SCI_PASSERT(0, ("load addr is must align 8 bytes.!\n"));
      return -1;
    }
    if(unzip_data_len > load_len)
    {
      SCI_PASSERT(0, ("load len is less than the actual len!\n"));
      return -1;
    }

#if 0//def BZP_SUPPORT

#else

    if(load_use_lzma() ==1)
        {
            if(idx==LZMA_BZP_EX01 || idx==LZMA_BZP_EX02 )
            {
                mon_Event(0x1fad7001);
                lzma_uzip(idx,load_addr,&in);
            }
            else
            {

                memcpy(load_addr,in.zip_buf_addr,in.zip_buf_len);  
            }
       }
       else
       {
             memcpy(load_addr,in.zip_buf_addr,in.zip_buf_len);  
       }
#endif

     return 0;

}

void ipc_set_ims_nv_addr(uint16 nv_id, uint32 addr, uint32 len)
{
    switch (nv_id)
    {
        case 0x800:
            ims_nv_buf_addr_info->ims_csm_addr = addr;
            ims_nv_buf_addr_info->ims_csm_len = len;
            break;
         case 0x801:
            ims_nv_buf_addr_info->ims_sapp_addr = addr;
            ims_nv_buf_addr_info->ims_sapp_len = len;
            break;
         case 0x813:
            ims_nv_buf_addr_info->ims_isim_addr = addr;
            ims_nv_buf_addr_info->ims_isim_len = len;
            break;
         case 0x870:
            ims_nv_buf_addr_info->ims_sim1_delta_addr = addr;
            ims_nv_buf_addr_info->ims_sim1_delta_len = len;
            break;
         case 0x871:
            ims_nv_buf_addr_info->ims_sim2_delta_addr = addr;
            ims_nv_buf_addr_info->ims_sim2_delta_len = len;
            break;
         case 0x872:
            ims_nv_buf_addr_info->ims_plmn1_delta_addr = addr;
            ims_nv_buf_addr_info->ims_plmn1_delta_len = len;
            break;
         case 0x873:
            ims_nv_buf_addr_info->ims_plmn2_delta_addr = addr;
            ims_nv_buf_addr_info->ims_plmn2_delta_len = len;
            break;
         default:
            SCI_TraceLow("%s: nv_id ERROR 0x%x\n", __FUNCTION__, nv_id);
            break;
    }
}

void load_delta_nv_data(void)
{
    uint8 *ims_nv_buf;
    uint16 len = 0, ret = 0;
	uint32 denv_part_addr = 0;
	uint32 denv_part_size = 0;

    /* Read Delta NV to PSRAM */
    ims_nv_buf = ipc_get_ims_nv_buf_addr();
    ims_nv_buf_addr_info = (ims_nv_info_t *)ipc_get_ims_nv_buf_info_addr();
    SCI_TraceLow("[Delta NV]: ims_nv_buf addr 0x%x, ims_nv_buf_addr_info addr 0x%x\n", 
            ims_nv_buf, ims_nv_buf_addr_info);

    len = NVITEM_GetLength(0x800);
    ret = EFS_NvitemRead(0x800, len, ims_nv_buf);
    if (ret == 0)
        ipc_set_ims_nv_addr(0x800, (uint32)ims_nv_buf, len);
    ims_nv_buf += len;
    SCI_TraceLow("[Delta NV]: NVITEM 0x800,  len %d, buf 0x%x, ret %d\n", len, ims_nv_buf, ret);

    len = NVITEM_GetLength(0x801);
    ret = EFS_NvitemRead(0x801, len, ims_nv_buf);
    if (ret == 0)
        ipc_set_ims_nv_addr(0x801, (uint32)ims_nv_buf, len);
    ims_nv_buf += len;
    SCI_TraceLow("[Delta NV]: NVITEM 0x801,  len %d, buf 0x%x, ret %d\n", len, ims_nv_buf, ret);

    len = NVITEM_GetLength(0x813);
    ret = EFS_NvitemRead(0x813, len, ims_nv_buf);
    if (ret == 0)
        ipc_set_ims_nv_addr(0x813, (uint32)ims_nv_buf, len);
    ims_nv_buf += len;
    SCI_TraceLow("[Delta NV]: NVITEM 0x813,  len %d, buf 0x%x, ret %d\n", len, ims_nv_buf, ret);

    len = NVITEM_GetLength(0x870);
    ret = EFS_NvitemRead(0x870, len, ims_nv_buf);
    if (ret == 0)
        ipc_set_ims_nv_addr(0x870, (uint32)ims_nv_buf, len);
    ims_nv_buf += len;
    SCI_TraceLow("[Delta NV]: NVITEM 0x870,  len %d, buf 0x%x, ret %d\n", len, ims_nv_buf, ret);

    len = NVITEM_GetLength(0x871);
    ret = EFS_NvitemRead(0x871, len, ims_nv_buf);
    if (ret == 0)
        ipc_set_ims_nv_addr(0x871, (uint32)ims_nv_buf, len);
    ims_nv_buf += len;
    SCI_TraceLow("[Delta NV]: NVITEM 0x871,  len %d, buf 0x%x, ret %d\n", len, ims_nv_buf, ret);

    len = NVITEM_GetLength(0x872);
    ret = EFS_NvitemRead(0x872, len, ims_nv_buf);
    if (ret == 0)
        ipc_set_ims_nv_addr(0x872, (uint32)ims_nv_buf, len);
    ims_nv_buf += len;
    SCI_TraceLow("[Delta NV]: NVITEM 0x872,  len %d, buf 0x%x, ret %d\n", len, ims_nv_buf, ret);

    len = NVITEM_GetLength(0x873);
    ret = EFS_NvitemRead(0x873, len, ims_nv_buf);
    if (ret == 0)
        ipc_set_ims_nv_addr(0x873, (uint32)ims_nv_buf, len);
    ims_nv_buf += len;
    SCI_TraceLow("[Delta NV]: NVITEM 0x873,  len %d, buf 0x%x, ret %d\n", len, ims_nv_buf, ret);

	denv_part_addr = SPIFLASH_GetOperatorDataAddress();
	denv_part_size = SPIFLASH_GetOperatorDataSize();
	SCI_TraceLow("[Delta NV]: denv_part 0x%x, %d\n", denv_part_addr, denv_part_size);
    memcpy(ims_nv_buf, denv_part_addr, denv_part_size);
    ipc_set_delta_nv_bin_addr((uint32)ims_nv_buf, denv_part_size);
    ims_nv_buf += denv_part_size;
    SCI_TraceLow("[Delta NV]: delta nv bin, len %d, buf 0x%x\n", denv_part_size, ims_nv_buf);
}

//LZMA_BZP_EX00:memory_index_list.bin
//LZMA_BZP_EX01:cp.bin
//LZMA_BZP_EX02:zsp.bin
//LZMA_BZP_EX03:bcpu_gsm.bin
//LZMA_BZP_EX04:riscV.bin
//uint16 v_test_1 = 1;

void LOAD_modem(void)
{
   int i = 0;
   uint16 len = 0;
  T_MEM_INDEX_LIST *pt_mem_list = (T_MEM_INDEX_LIST *)MEM_INDEX_LIST_BASE;

    //while(v_test_1);

     SCI_TraceLow("Load memory_list.bin...\n");
  if(!(MEM_bzpLoad(LZMA_BZP_EX00,MEM_INDEX_LIST_BASE,4096)))	
     SCI_TraceLow("Load memory_list.bin OK\n");
  else
     SCI_TraceLow("Load memory_list.bin failed\n");		

    load_delta_nv_data();

   for(i = 0; i < MEM_ID_MAX; i++)
   {
       if(pt_mem_list->index_unit[i].cpneed != 0)
      	{
            // if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_IMEI_NAME) == 0)
            {

            }

	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_DYNAMIC_NV_NAME) == 0)
            {
                 SCI_TraceLow("Load Modem Dynamic NV...\n");
                 len = EFS_ReadItem(NV_MODEM_DYNAMIC_NV, pt_mem_list->index_unit[i].addr, pt_mem_list->index_unit[i].len);
                 SCI_TraceLow("Has Loaded Modem Dynamic NV to 0x%x, len 0x%x,max len 0x%x\n", pt_mem_list->index_unit[i].addr, len, pt_mem_list->index_unit[i].len);
          }
		 
	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_DYNAMIC_NV_2_NAME) == 0)
            {
                 SCI_TraceLow("Load Modem Dynamic sim2 NV...\n");
                 len = EFS_ReadItem(NV_MODEM_DYNAMIC_CARD2_NV, pt_mem_list->index_unit[i].addr, pt_mem_list->index_unit[i].len);
                 SCI_TraceLow("Has Loaded Modem Dynamic card2 NV to 0x%x, len 0x%x,max len 0x%x\n", pt_mem_list->index_unit[i].addr, len, pt_mem_list->index_unit[i].len);
          }

	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_STATIC_NV_NAME) == 0)
            {
                 SCI_TraceLow("Load Modem Static NV...\n");
                 len = EFS_ReadItem(NV_MODEM_STATIC_NV, pt_mem_list->index_unit[i].addr,pt_mem_list->index_unit[i].len);
                 SCI_TraceLow("Has Loaded Modem Static NV to 0x%x, len 0x%x,max len 0x%x\n", pt_mem_list->index_unit[i].addr, len, pt_mem_list->index_unit[i].len);
            }
		 
	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_STATIC_NV_2_NAME) == 0)
            {
                 SCI_TraceLow("Load Modem Static sim2 NV...\n");
                 len = EFS_ReadItem(NV_MODEM_STATIC_CARD2_NV, pt_mem_list->index_unit[i].addr,pt_mem_list->index_unit[i].len);
                 SCI_TraceLow("Has Loaded Modem Static card2 NV to 0x%x, len 0x%x,max len 0x%x\n", pt_mem_list->index_unit[i].addr, len, pt_mem_list->index_unit[i].len);
          }

	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_PHY_NV_NAME) == 0)
            {
                 SCI_TraceLow("Load Modem Phy NV...\n");
                 len = EFS_ReadItem(NV_MODEM_PHY_NV, pt_mem_list->index_unit[i].addr,pt_mem_list->index_unit[i].len);
                 SCI_TraceLow("Has Loaded Modem Phy NV to 0x%x, len 0x%x,max len 0x%x\n", pt_mem_list->index_unit[i].addr, len, pt_mem_list->index_unit[i].len);
          }
		 
	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_PHY_NV_2_NAME) == 0)
            {
                 SCI_TraceLow("Load Modem Phy sim2 NV...\n");
                 len = EFS_ReadItem(NV_MODEM_PHY_CARD2_NV, pt_mem_list->index_unit[i].addr, pt_mem_list->index_unit[i].len);
                 SCI_TraceLow("Has Loaded Modem Phy card2 NV to 0x%x, len 0x%x,max len 0x%x\n", pt_mem_list->index_unit[i].addr, len, pt_mem_list->index_unit[i].len);
          }

	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_CFW_NV_NAME) == 0)
            {
                 SCI_TraceLow("Load CFW NV...\n");
                 len = EFS_ReadItem(NV_MODEM_CFW_NV, pt_mem_list->index_unit[i].addr,pt_mem_list->index_unit[i].len);
                 SCI_TraceLow("Has Loaded CFW NV to 0x%x, len 0x%x,max len 0x%x\n",pt_mem_list->index_unit[i].addr,len,pt_mem_list->index_unit[i].len);
            }

	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_RF_NV_NAME) == 0)
            {
                 SCI_TraceLow("Load RF NV...\n");
				 // EFS_NvitemRead(NV_MODEM_RF_NV,pt_mem_list->index_unit[i].len,pt_mem_list->index_unit[i].addr);
                 len = EFS_ReadItem(NV_MODEM_RF_NV, pt_mem_list->index_unit[i].addr,pt_mem_list->index_unit[i].len);
                 SCI_TraceLow("Has Loaded RF NV to 0x%x, len 0x%x,max len 0x%x\n",pt_mem_list->index_unit[i].addr,len,pt_mem_list->index_unit[i].len);
            }

	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_LTE_RF_CALIB_NAME) == 0)
            {
                 SCI_TraceLow("Load LTE RF Calib...\n");
                 //EFS_ReadItem(NV_MODEM_RF_CALIB_LTE, pt_mem_list->index_unit[i].addr,pt_mem_list->index_unit[i].len);
				 len = EFS_NvitemRead(NV_MODEM_RF_CALIB_LTE,pt_mem_list->index_unit[i].len,pt_mem_list->index_unit[i].addr);
				 SCI_TraceLow("Has Loaded  LTE RF Calib to 0x%x, status 0x%x,max len 0x%x\n",pt_mem_list->index_unit[i].addr,len,pt_mem_list->index_unit[i].len);
            }

	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_GSM_RF_CALIB_NAME) == 0)
            {
                 SCI_TraceLow("Load GSM RF Calib...\n");
                 //EFS_ReadItem(NV_MODEM_RF_CALIB_GSM, pt_mem_list->index_unit[i].addr,pt_mem_list->index_unit[i].len);
				 len = EFS_NvitemRead(NV_MODEM_RF_CALIB_GSM,pt_mem_list->index_unit[i].len,pt_mem_list->index_unit[i].addr);
				 SCI_TraceLow("Has Loaded  GSM RF Calib to 0x%x, status 0x%x,max len 0x%x\n",pt_mem_list->index_unit[i].addr,len,pt_mem_list->index_unit[i].len);
            }
	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_GOLDEN_BOARD_NAME) == 0)
            {
                 SCI_TraceLow("Load golden board...\n");
				 len = EFS_NvitemRead(NV_MODEM_RF_GOLDEN_BOARD,pt_mem_list->index_unit[i].len,pt_mem_list->index_unit[i].addr);
				 SCI_TraceLow("Has Loaded  golden board to 0x%x, status 0x%x,max len 0x%x\n",pt_mem_list->index_unit[i].addr,len,pt_mem_list->index_unit[i].len);
            }		 
	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_AUDIO_CALIB_NAME) == 0)
            {
                 SCI_TraceLow("Load AUD Calib...\n");
                 len = EFS_ReadItem(NV_MODEM_AUD_CALIB, pt_mem_list->index_unit[i].addr, pt_mem_list->index_unit[i].len);
                 SCI_TraceLow("Has Loaded  AUD Calib to 0x%x, len 0x%x,max len 0x%x\n",pt_mem_list->index_unit[i].addr,len,pt_mem_list->index_unit[i].len);
            }
		 
	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_CP_BIN_NAME) == 0)
            {
                   SCI_TraceLow("Load cp.bin...\n");
		     v_cp_jump_addr = pt_mem_list->index_unit[i].addr;
                   if(!(MEM_bzpLoad(LZMA_BZP_EX01,(uint8 *)pt_mem_list->index_unit[i].addr,(uint8 *)pt_mem_list->index_unit[i].len)))	
		        SCI_TraceLow("Load cp.bin OK\n");
	            else
			SCI_TraceLow("Load cp.bin failed\n");	
				
		     mmu_MapOnPage_Set (pt_mem_list->index_unit[i].addr, (uint32) pt_mem_list->index_unit[i].addr,MEM_TYPE_DATA_RO, pt_mem_list->index_unit[i].len);		   		
            }

	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_ZSP_BIN_NAME) == 0)
            {
                   SCI_TraceLow("Load zsp.bin...\n");
			v_zsp_jump_addr = pt_mem_list->index_unit[i].addr;
                   if(!(MEM_bzpLoad(LZMA_BZP_EX02,(uint8 *)pt_mem_list->index_unit[i].addr,(uint8 *)pt_mem_list->index_unit[i].len)))	
		        SCI_TraceLow("Load zsp.bin OK\n");
	            else
			SCI_TraceLow("Load zsp.bin failed\n");	
				
		      mmu_MapOnPage_Set (pt_mem_list->index_unit[i].addr, (uint32) pt_mem_list->index_unit[i].addr,MEM_TYPE_DATA_RO, pt_mem_list->index_unit[i].len); 			   
            }
		 
	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_BCPU_GSM_BIN_NAME) == 0)
            {
                   SCI_TraceLow("Load bcpu_gsm.bin...\n");
                   if(!(MEM_bzpLoad(LZMA_BZP_EX03,(uint8 *)pt_mem_list->index_unit[i].addr,(uint8 *)pt_mem_list->index_unit[i].len)))	
		        SCI_TraceLow("Load bcpu_gsm.bin OK\n");
	            else
			SCI_TraceLow("Load bcpu_gsm.bin failed\n");		
				
		    mmu_MapOnPage_Set (pt_mem_list->index_unit[i].addr, (uint32) pt_mem_list->index_unit[i].addr,MEM_TYPE_DATA_RO, pt_mem_list->index_unit[i].len); 			   
				
            }
	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_RISCV_BIN_NAME) == 0)
            {
            	     CHIP_REG_AND(((RF_ISRAM_BASE)+0x35008),(~0x10));  //reset RISCV. and wait CP release

                   SCI_TraceLow("Load riscV.bin...\n");
				   
                   if(!(MEM_bzpLoad(LZMA_BZP_EX04,(uint8 *)pt_mem_list->index_unit[i].addr,(uint8 *)pt_mem_list->index_unit[i].len)))	
		        SCI_TraceLow("Load riscV.bin OK\n");
	            else
			SCI_TraceLow("Load riscV.bin failed\n");
				
			//mmu_MapOnPage_Set (pt_mem_list->index_unit[i].addr, (uint32) pt_mem_list->index_unit[i].addr,MEM_TYPE_DATA_RO, pt_mem_list->index_unit[i].len); 			   
				
            }      		
       }
   }
   //bug1059181. set  cp sysram mmu to RO.
  mmu_MapOnPage_Set(INTER_RAM_BEGIN+0x4000, INTER_RAM_BEGIN+0x4000,MEM_TYPE_DATA_RO, (INTER_RAM_SIZE - 0x4000));
  mmu_MapOnPage_Set(INTER_RAM_BEGIN+0x3000, INTER_RAM_BEGIN+0x3000,MEM_TYPE_NOCACHED_DATA, 0x1000); 
  mmu_MapOnPage_Set(INTER_RAM_BEGIN+INTER_RAM_SIZE, INTER_RAM_BEGIN+INTER_RAM_SIZE,MEM_TYPE_DATA_RO, INTER_RAM_SIZE);   /*0x840000--0x880000 invalid addr to RO*/
   v_load_modem_is_finish = 1;
}

int LOAD_modem_isFinish(void)
{
    return v_load_modem_is_finish;
}


void *Get_AudCalibBuf(uint32 *len)
{
    return GET_MemoryStartAddressAndLen(MEM_AUDIO_CALIB_NAME, len);
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

