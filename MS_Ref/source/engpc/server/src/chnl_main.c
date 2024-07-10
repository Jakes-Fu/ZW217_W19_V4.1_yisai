#include "sci_types.h"
#include "os_api.h"

#include "sprd_fts_type.h"
#include "sprd_fts_log.h"

#include "chnl.h"
#include "modulemgr.h"
#include "chnl_cp.h"
#include "chnl_wcn.h"

/**---------------------------------------------------------------------------*
 **						Compiler Flag									  *
 **---------------------------------------------------------------------------*/
#ifdef	 __cplusplus
	extern	 "C"
	{
#endif

const CHNL_TYPE_NAME s_chnl_type_name[] = {
    {CHNL_DIAG_PC_CP, CP_DIAG, "[DIAG:PC->CP]"},
    {CHNL_DIAG_CP_PC, CP_DIAG, "[DIAG:CP->PC]"},
    {CHNL_DIAG_PC_WCN, WCN_DATA, "[DIAG:PC->WCN]"},
    {CHNL_DIAG_WCN_PC, WCN_DATA, "[DIAG:WCN->PC]"},
    {CHNL_AT_PC_CP, CP_AT, "[AT:PC->CP]"},
    {CHNL_AT_CP_PC, CP_AT, "[AT:CP->PC]"},
    {CHNL_AT_PC_WCN, WCN_AT, "[AT:PC->WCN"},
    {CHNL_AT_WCN_PC, WCN_AT, "[AT:WCN->PC]"}
};

PUBLIC char* findNameByChnlType(int chnl_type)
{
	int i = 0 ;
    for(i = 0; i < sizeof(s_chnl_type_name)/sizeof(CHNL_TYPE_NAME); i++){
        if (s_chnl_type_name[i].chnl_type == chnl_type){
            return s_chnl_type_name[i].name;
        }
    }

    return "[unknown]";
}

PUBLIC CHNL_DATA_TYPE findChnlDataTypeByChnlType(int chnl_type){
	int i = 0 ;

	for(i = 0; i < sizeof(s_chnl_type_name)/sizeof(CHNL_TYPE_NAME); i++){
	    if (s_chnl_type_name[i].chnl_type == chnl_type){
	        return s_chnl_type_name[i].data_type;
	    }
	}

    return DATA_UNKNOWN;
}

LOCAL BOOLEAN isDiagAT(uint8* src, int len)
{
    MSG_HEAD_T *msg_head_ptr = (MSG_HEAD_T *)(src+1);
    if(len < sizeof(MSG_HEAD_T)+1){
        return FALSE;
    }

    return (msg_head_ptr->type==0x68)?TRUE:FALSE;
}

PUBLIC int chnl_diag_trans(uint8 **dest_ptr, uint16 *dest_len_ptr, const uint8 *src_ptr, uint16 src_len)
{
    int cp_process = 0;
    int ret = 0;
    uint8 *local_src_ptr = NULL;
    uint8 *local_dest_ptr = NULL;
    local_dest_ptr = (uint8 *)SCI_ALLOC_APP(DIAG_BUFF_SIZE); 
    if(local_dest_ptr == NULL){
        ENG_LOG("%s: chnl_diag_trans malloc %d bytes fail...", findNameByChnlType(CHNL_DIAG_PC_CP), DIAG_BUFF_SIZE);
        return 0;
    }
    SCI_MEMSET(local_dest_ptr, 0, DIAG_BUFF_SIZE);

    DATA_TO_DIAG_NEW(local_src_ptr, src_ptr, src_len)

    ENG_LOG("%s: chnl_diag_trans", findNameByChnlType(CHNL_DIAG_PC_CP));
    printData(findNameByChnlType(CHNL_DIAG_PC_CP), CP_DIAG, local_src_ptr, src_len+2, PRINT_NUM_MAX, PRINT_BRK_FLAG);
    ret = processDiag(local_src_ptr, src_len+2, local_dest_ptr, DIAG_BUFF_SIZE, &cp_process);
    ENG_LOG("%s: chnl_diag_trans return %d, cp_process = %d", findNameByChnlType(CHNL_DIAG_PC_CP), ret, cp_process);
    printData(findNameByChnlType(CHNL_DIAG_PC_CP), CP_DIAG, local_dest_ptr, ret, PRINT_NUM_MAX, PRINT_BRK_FLAG);

    if (ret > 0){
        if (cp_process == 1){
            ret = ENG_DIAG_RET_TO_CP;
            goto freebuf;
        }else{
            int offset = 0;
            if(isDiagAT(local_src_ptr, src_len)){
                MSG_HEAD_T cmd_head = {0};
                int size = ret;
                AT_TO_DATA_MOVE(local_dest_ptr, size);
                cmd_head.seq_num = 0;
                cmd_head.len = (ret+sizeof(MSG_HEAD_T));
                cmd_head.type = 0x9C;
                cmd_head.subtype = 0x00;
                SCI_MEMCPY(local_dest_ptr, &cmd_head, sizeof(MSG_HEAD_T));
                ret += sizeof(MSG_HEAD_T);
            }else{
                DIAG_TO_DATA_MOVE(local_dest_ptr, offset, ret)
            }

            goto succ;
        }
    }else if (ret == ENG_DIAG_NO_RESPONSE){
        goto freebuf;
    }else{
        ret = 0;
        goto freebuf;
    }

    
freebuf:
    SCI_FREE(local_dest_ptr);
    local_dest_ptr = NULL;

succ:
    SCI_FREE(local_src_ptr);
    local_src_ptr = NULL;

    *dest_len_ptr = ret;
    *dest_ptr = local_dest_ptr;

    return ret;
}

PUBLIC int chnl_at_trans(uint8 **dest_ptr, uint16 *dest_len_ptr, const uint8 *src_ptr, uint16 src_len)
{
    int cp_process = 0;
    int ret = 0;
    uint8 *local_dest_ptr = NULL;
    local_dest_ptr = (uint8 *)SCI_ALLOC_APP(AT_BUFF_SIZE); 
    if(local_dest_ptr == NULL){
        ENG_LOG("%s: chnl_diag_trans malloc %d bytes fail...", findNameByChnlType(CHNL_AT_PC_CP), AT_BUFF_SIZE);
        return 0;
    }
    SCI_MEMSET(local_dest_ptr, 0, AT_BUFF_SIZE);

    ENG_LOG("%s: chnl_at_trans", findNameByChnlType(CHNL_AT_PC_CP));
    ret = processAT(src_ptr, src_len, local_dest_ptr, AT_BUFF_SIZE, &cp_process);
    ENG_LOG("%s: chnl_at_trans return %d", findNameByChnlType(CHNL_AT_PC_CP), ret);
    printData(findNameByChnlType(CHNL_AT_PC_CP), CP_AT, local_dest_ptr, ret, PRINT_NUM_MAX, PRINT_BRK_FLAG);

    if (ret > 0){
        if (cp_process ==1){
            ret = ENG_DIAG_RET_TO_CP;
            goto freebuf;
        }else{
            goto succ;
        }
    }else if (ret == ENG_DIAG_NO_RESPONSE){
        goto freebuf;
    }else{
        goto freebuf;
    }

freebuf:
    SCI_FREE(local_dest_ptr);
    local_dest_ptr = NULL;

succ:
    *dest_len_ptr = ret;
    *dest_ptr = local_dest_ptr;

    return ret;
}
void printData(char* name, CHNL_DATA_TYPE dataType, char* buff, int len, int col, int brk){
    if (dataType == CP_AT || dataType == WCN_AT){
        ENG_LOG("%s: data= %s\n", name, buff);
    }else{
        int i = 0;
        int j = 0;
        char tmpbuf[128] = {0};

        len = len>(sizeof(tmpbuf)-1)?(sizeof(tmpbuf)-1):len;

        for(i = 0; i < len; i++) {
            sprintf(tmpbuf + j, "%02X ", *(buff + i));
            j = strlen(tmpbuf);
            if(i % col == col -1 || i == len -1) {
                ENG_LOG("%s: data= %s\n", name, tmpbuf);
                if (brk) {
                    break;
                } else {
                    j = 0;
                    continue;
                }
            }
        }
    }
}

#ifndef ENGPC_W217
PUBLIC void chnl_init()
{
    ENG_LOG("\rchnl_init.....begin\r");

    ENG_LOG("chnl sipc init");
    if (0 == chnl_sipc_init()){
        ENG_LOG("chnl cp init");
        chnl_cp_init();
        //ENG_LOG("chnl wcn init");
        //chnl_wcn_init();
    }
    ENG_LOG("chnl pc init");
    chnl_pc_init();
    
    ENG_LOG("\rchnl_init.....end\r");
}
#endif
int eng_diag_ap_assert_func(char *buf, int len, char *rsp, int rsplen)
{
	SCI_ASSERT(0);
}


/**---------------------------------------------------------------------------*
 **						Compiler Flag									  *
 **---------------------------------------------------------------------------*/
#ifdef	 __cplusplus
	}
#endif
