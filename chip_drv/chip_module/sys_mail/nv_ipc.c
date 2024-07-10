

#include "os_api.h"
#include "nv_item_id.h"
#include "nvitem.h"
#include "ipc_drvapi.h"

#define  NV_ITEM_WRITE_SYNC          0          // write to running NV with sync
#define  NV_ITEM_WRITE_ASYNC         1          // write to running NV with async
#define  NV_ITEM_WRITE_UPDATECALI    2          // write to running NV and fixed NV

typedef struct NV_IPC_INFO_Tag
{
   uint16 nvItemId;         // nv item id of EFS
   uint8   nvIpcId;           // nv ipc id
   uint8   basync;            // basync, if nv write req is async
}NV_IPC_INFO;

NV_IPC_INFO   a_NV_IPCInfo[NV_ID_MAX];

static struct smd_ch *p_smdA2CChannel;

static void NV_IPC_callback(void *param, uint32 flag);


void NV_IPC_setNvItemInfo(uint8 nvIpcId, uint16 nvItemId, uint8 basync)
{
    if(nvIpcId >= NV_ID_MAX)
    {
      	 return;
    }

    a_NV_IPCInfo[nvIpcId].nvIpcId = nvIpcId;
    a_NV_IPCInfo[nvIpcId].nvItemId = nvItemId;
    a_NV_IPCInfo[nvIpcId].basync = basync;
}

void NV_IPC_init(void)
{
    int res;

    p_smdA2CChannel = NULL;
    res = ipc_ch_open(SMD_CH_A2C_CTRL, &p_smdA2CChannel, NULL, NV_IPC_callback);
    SCI_ASSERT(res >= 0);

    memset(a_NV_IPCInfo, 0, sizeof(a_NV_IPCInfo));

    // init nv info of ipc
	NV_IPC_setNvItemInfo(NV_ID_CFW_NV, NV_MODEM_CFW_NV, NV_ITEM_WRITE_ASYNC);  
	NV_IPC_setNvItemInfo(NV_ID_STATIC_NV, NV_MODEM_STATIC_NV, NV_ITEM_WRITE_SYNC); 
	NV_IPC_setNvItemInfo(NV_ID_DYNAMIC_NV, NV_MODEM_DYNAMIC_NV, NV_ITEM_WRITE_ASYNC); 
	NV_IPC_setNvItemInfo(NV_ID_PHY_NV, NV_MODEM_PHY_NV, NV_ITEM_WRITE_SYNC);    
	#if  !defined(PLATFORM_UWS6121E)
	NV_IPC_setNvItemInfo(NV_ID_STATIC_NV_2, NV_MODEM_STATIC_CARD2_NV, NV_ITEM_WRITE_SYNC); 
	NV_IPC_setNvItemInfo(NV_ID_DYNAMIC_NV_2, NV_MODEM_DYNAMIC_CARD2_NV, NV_ITEM_WRITE_ASYNC); 
	NV_IPC_setNvItemInfo(NV_ID_PHY_NV_2, NV_MODEM_PHY_CARD2_NV, NV_ITEM_WRITE_SYNC);    
	NV_IPC_setNvItemInfo(NV_ID_GSM_CALIB, NV_MODEM_RF_CALIB_GSM, NV_ITEM_WRITE_UPDATECALI);
	#endif
	NV_IPC_setNvItemInfo(NV_ID_LTE_CALIB, NV_MODEM_RF_CALIB_LTE, NV_ITEM_WRITE_UPDATECALI);
	NV_IPC_setNvItemInfo(NV_ID_GOLDEN_BOARD, NV_MODEM_RF_GOLDEN_BOARD, NV_ITEM_WRITE_UPDATECALI);
	//NV_IPC_setNvItemInfo(NV_ID_AUD_CALIB, NV_MODEM_AUD_CALIB, NV_ITEM_WRITE_SYNC);
	NV_IPC_setNvItemInfo(NV_ID_RF_NV, NV_MODEM_RF_NV, NV_ITEM_WRITE_SYNC);

}

static void NV_IPC_callback(void *param, uint32 flag)
{
    struct ipc_cmd     ipcCmd[IPC_CMD_QUEUE_LEN] = {0};
    struct ipc_cmd    *p_ipcCmd = PNULL;
    struct ipc_cmd     ipcCmdRes = {0};
    uint8    i = 0;
    uint16  nvItemId = 0;
    int32    len = 0;
	  NVITEM_ERROR_E	result = NVERR_NONE;

    // get avail number of ipccmd
    len = ipc_ch_read_avail(p_smdA2CChannel);

    if(len)
    {
      ipc_ch_read(p_smdA2CChannel, (void *)ipcCmd, len);
  
      for(i = 0; i < len; i++)
      {
          p_ipcCmd = &ipcCmd[i];

		  SCI_TraceLow("nv ipc: id: %d, oper: %d, addr: 0x%x, len: 0x%x", p_ipcCmd->id, p_ipcCmd->para0, p_ipcCmd->para1, p_ipcCmd->para2);

          if(NV_CMD_CALIB == p_ipcCmd->para0)
          {
              DIAG_CreateCaliCmdCnfToDiag(p_ipcCmd->id, p_ipcCmd->para2, (uint8 *)p_ipcCmd->para1);
		  }
		  else if(NV_CMD_IWRITE == p_ipcCmd->para0)
		  {
              EFS_NvitemWrite(p_ipcCmd->id, p_ipcCmd->para2, (p_ipcCmd->para1 + (uint32)rda_sm_base), NV_ITEM_WRITE_ASYNC);
		  }
		  else
		  {
              nvItemId = a_NV_IPCInfo[p_ipcCmd->id].nvItemId;
        
              if((p_ipcCmd->id != 0) && (nvItemId != 0) && p_ipcCmd->id < NV_ID_MAX)
              {
                  if(NV_CMD_WRITE == p_ipcCmd->para0)
                  {
                   if(NV_ITEM_WRITE_UPDATECALI == a_NV_IPCInfo[p_ipcCmd->id].basync)
                   {
                      result = NVITEM_UpdateCali(nvItemId, p_ipcCmd->para2, p_ipcCmd->para1);
				   }
				   else
				   {
                      result = EFS_NvitemWrite(nvItemId, p_ipcCmd->para2, p_ipcCmd->para1, a_NV_IPCInfo[p_ipcCmd->id].basync);
				   }
      
                   // fill response to CP
                   ipcCmdRes.id = p_ipcCmd->id;
                      ipcCmdRes.para0 = p_ipcCmd->para0;
                      
                  }
                  else
                  {
                      result = NVERR_NOT_EXIST;
                  }
                  
                  if(NVERR_NONE == result)
                  {
                      // success
                      ipcCmdRes.para1 = NV_CMD_SUCCESS;
                      ipcCmdRes.para2 = p_ipcCmd->para2;
                  }
                  else
                  {
                      // fail,use error code to rsp to cp
                      ipcCmdRes.para1 = result;    // NV_CMD_FAILURE;
                      ipcCmdRes.para2 = 0;
                  }
              }
              else
              {
                  // fill response to CP
                  ipcCmdRes.id = p_ipcCmd->id;
                  ipcCmdRes.para0 = p_ipcCmd->para0;
                  ipcCmdRes.para1 = NV_CMD_FAILURE;
                  ipcCmdRes.para2 = 0;
              }
    
              // send ipc cmd result to CP
              ipc_ch_write(p_smdA2CChannel, &ipcCmdRes, 1);
		  }
  
      }
    }  
}


void NV_IPC_sendCmdToCp(uint32 id, uint32 param0, uint32 param1, uint32 param2)
{
    struct ipc_cmd    ipcCmd;
	
    ipcCmd.id = id;
    ipcCmd.para0 = param0;
    ipcCmd.para1 = param1;
    ipcCmd.para2 = param2;
    
    // send ipc cmd result to CP
    ipc_ch_write(p_smdA2CChannel, &ipcCmd, 1);
}

