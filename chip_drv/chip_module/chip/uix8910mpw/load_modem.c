

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
	
    SCI_TraceLow("Load %d,bzp_addr:0x%x bzp_len:0x%x unzip_len:0x%x load_addr:0x%x load_len:0x%x\n",idx,in.zip_buf_addr,in.zip_buf_len,unzip_data_len,(uint32)load_addr,load_len);

    if(unzip_data_len > load_len)
    {
      SCI_PASSERT(0, ("load len is less than the actual len!\n"));
      return -1;
    }

#if 0//def BZP_SUPPORT

#else
      memcpy(load_addr,in.zip_buf_addr,in.zip_buf_len);     
#endif

     return 0;

}

//LZMA_BZP_EX00:memory_index_list.bin
//LZMA_BZP_EX01:cp.bin
//LZMA_BZP_EX02:zsp.bin
//LZMA_BZP_EX03:bcpu_gsm.bin
//LZMA_BZP_EX04:riscV.bin

void LOAD_modem(void)
{
   int i = 0;
   uint16 len = 0;
  T_MEM_INDEX_LIST *pt_mem_list = (T_MEM_INDEX_LIST *)MEM_INDEX_LIST_BASE;

  
     SCI_TraceLow("Load memory_list.bin...\n");
  if(!(MEM_bzpLoad(LZMA_BZP_EX00,MEM_INDEX_LIST_BASE,4096)))	
     SCI_TraceLow("Load memory_list.bin OK\n");
  else
     SCI_TraceLow("Load memory_list.bin failed\n");		


   for(i = 0; i < MEM_ID_MAX; i++)
   {
       if(pt_mem_list->index_unit[i].cpneed != 0)
      	{
            if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_IMEI_NAME) == 0)
            {

            }

	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_DYNAMIC_NV_NAME) == 0)
            {
                 SCI_TraceLow("Load Modem Dynamic NV...\n");
                 len = EFS_ReadItem(NV_MODEM_DYNAMIC_NV, pt_mem_list->index_unit[i].addr,pt_mem_list->index_unit[i].len);
                 SCI_TraceLow("Has Loaded Modem Dynamic NV to 0x%x, len 0x%x,max len 0x%x\n",pt_mem_list->index_unit[i].addr,len,pt_mem_list->index_unit[i].len);
          }

	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_STATIC_NV_NAME) == 0)
            {
                 SCI_TraceLow("Load Modem Static NV...\n");
                 EFS_ReadItem(NV_MODEM_STATIC_NV, pt_mem_list->index_unit[i].addr,pt_mem_list->index_unit[i].len);
                 SCI_TraceLow("Has Loaded Modem Static NV to 0x%x, len 0x%x,max len 0x%x\n",pt_mem_list->index_unit[i].addr,len,pt_mem_list->index_unit[i].len);
            }

	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_CFW_NV_NAME) == 0)
            {
                 SCI_TraceLow("Load CFW NV...\n");
                 EFS_ReadItem(NV_MODEM_CFW_NV, pt_mem_list->index_unit[i].addr,pt_mem_list->index_unit[i].len);
                 SCI_TraceLow("Has Loaded CFW NV to 0x%x, len 0x%x,max len 0x%x\n",pt_mem_list->index_unit[i].addr,len,pt_mem_list->index_unit[i].len);
            }

	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_RF_NV_NAME) == 0)
            {
                 SCI_TraceLow("Load RF NV...\n");
                 EFS_ReadItem(NV_MODEM_RF_NV, pt_mem_list->index_unit[i].addr,pt_mem_list->index_unit[i].len);
                 SCI_TraceLow("Has Loaded RF NV to 0x%x, len 0x%x,max len 0x%x\n",pt_mem_list->index_unit[i].addr,len,pt_mem_list->index_unit[i].len);
            }

	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_LTE_RF_CALIB_NAME) == 0)
            {
                 SCI_TraceLow("Load LTE RF Calib...\n");
                 EFS_ReadItem(NV_MODEM_RF_CALIB_LTE, pt_mem_list->index_unit[i].addr,pt_mem_list->index_unit[i].len);
                 SCI_TraceLow("Has Loaded  LTE RF Calib to 0x%x, len 0x%x,max len 0x%x\n",pt_mem_list->index_unit[i].addr,len,pt_mem_list->index_unit[i].len);
            }

	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_GSM_RF_CALIB_NAME) == 0)
            {
                 SCI_TraceLow("Load GSM RF Calib...\n");
                 EFS_ReadItem(NV_MODEM_RF_CALIB_GSM, pt_mem_list->index_unit[i].addr,pt_mem_list->index_unit[i].len);
                 SCI_TraceLow("Has Loaded  GSM RF Calib to 0x%x, len 0x%x,max len 0x%x\n",pt_mem_list->index_unit[i].addr,len,pt_mem_list->index_unit[i].len);
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
                 EFS_ReadItem(NV_MODEM_AUD_CALIB, pt_mem_list->index_unit[i].addr,pt_mem_list->index_unit[i].len);
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
            }

	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_ZSP_BIN_NAME) == 0)
            {
                   SCI_TraceLow("Load zsp.bin...\n");
			v_zsp_jump_addr = pt_mem_list->index_unit[i].addr;
                   if(!(MEM_bzpLoad(LZMA_BZP_EX02,(uint8 *)pt_mem_list->index_unit[i].addr,(uint8 *)pt_mem_list->index_unit[i].len)))	
		        SCI_TraceLow("Load zsp.bin OK\n");
	            else
			SCI_TraceLow("Load zsp.bin failed\n");		
            }
		 
	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_BCPU_GSM_BIN_NAME) == 0)
            {
                   SCI_TraceLow("Load bcpu_gsm.bin...\n");
                   if(!(MEM_bzpLoad(LZMA_BZP_EX03,(uint8 *)pt_mem_list->index_unit[i].addr,(uint8 *)pt_mem_list->index_unit[i].len)))	
		        SCI_TraceLow("Load bcpu_gsm.bin OK\n");
	            else
			SCI_TraceLow("Load bcpu_gsm.bin failed\n");		
            }
	     if(strcmp((char *)pt_mem_list->index_unit[i].name, MEM_RISCV_BIN_NAME) == 0)
            {
                   SCI_TraceLow("Load riscV.bin...\n");
                   if(!(MEM_bzpLoad(LZMA_BZP_EX04,(uint8 *)pt_mem_list->index_unit[i].addr,(uint8 *)pt_mem_list->index_unit[i].len)))	
		        SCI_TraceLow("Load riscV.bin OK\n");
	            else
			SCI_TraceLow("Load riscV.bin failed\n");		
            }      		
       }
   }

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

