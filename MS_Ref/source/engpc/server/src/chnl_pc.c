#include "sci_types.h"
#include "os_api.h"

#include "sprd_fts_type.h"
#include "sprd_fts_log.h"

#include "chnl.h"
#include "sio.h"
#include "chnl_wifi.h"
#include "chnl_pc.h"
#include "chnl_sock.h"
void chnl_daps_start();
LOCAL PC_CONNECT_TYPE s_pc_connect_type = PC_CONNECT_COM;

LOCAL uint8 s_diag_emgpty[] = {0x7E, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0xD5, 0x00, 0x7E};
LOCAL uint8 s_at_ok[] = {0x7E, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x9C, 0x00, 0x0D, 0x0A, 0x4F, 0x4B, 0x0D, 0xA, 0x7E};
LOCAL char *s_err_invalidate_at = "+CME ERROR: Invalidate AT cmd\r";

PUBLIC void activeCurConnectType(PC_CONNECT_TYPE type){
    s_pc_connect_type = type;
}

PUBLIC PC_CONNECT_TYPE getCurConnectType(){
    return s_pc_connect_type;
}

PUBLIC void pc_write_empty_diag()
{
    if (s_pc_connect_type == PC_CONNECT_COM){
        SIO_SendCommData(s_diag_emgpty, sizeof(s_diag_emgpty));
#ifndef ENGPC_W217
    }else if(s_pc_connect_type == PC_CONNECT_WIFI){
        pc_sock_write_data(CP_DIAG, s_diag_emgpty, sizeof(s_diag_emgpty));
    }else{
#endif
    }
}

PUBLIC void pc_write_at_ok()
{
    if (s_pc_connect_type == PC_CONNECT_COM){
        SIO_SendCommData(s_at_ok, sizeof(s_at_ok));
#ifndef ENGPC_W217
    }else if(s_pc_connect_type == PC_CONNECT_WIFI){
        pc_sock_write_data(CP_DIAG, s_at_ok, sizeof(s_at_ok));
    }else{
#endif
    }
}


#ifdef ENGPC_W217


PUBLIC int pc_write_diag(char* buff, int len)
{
    int ret = 0;
    if (s_pc_connect_type == PC_CONNECT_COM){
        ret = SIO_SendDiag(buff, len);
        ENG_LOG("%s: pc write return %d", findNameByChnlType(CHNL_DIAG_CP_PC), ret);
    }else{
        ENG_LOG("%s: unkown type: s_pc_connect_type = %d", __FUNCTION__, s_pc_connect_type);
    }

    return ret;
}


#else
LOCAL int diag_encode(uint8* buff, int len){
    int retLen = len;
    uint8 tmp;
	int i = 0, j =0;
	
    for( i = 1; i< len-1; i++){
        if ((buff[i] == 0x7D) || (buff[i] == 0x7E)) {
            tmp = buff[i] ^ 0x20;
            ENG_LOG("%s, tmp=%x, buf[%d]=%x", __FUNCTION__, tmp, i, buff[i]);
            buff[i] = 0x7D;
            for(j = len; j > i+1; j--){
                buff[j] = buff[j-1];
            }
            buff[i+1] = tmp;
            len++;
            retLen++;
        }
    }

    ENG_LOG("%s: retlen = %d", __FUNCTION__, retLen);
    return retLen;
}

PUBLIC int pc_read_data(CHNL_DATA_TYPE type, char* buff, int len)
{
    int retRd = 0;
    int status = 0;
    int chnl_type = CHNL_DIAG_PC_CP;

    if(type == CP_DIAG){
        chnl_type = CHNL_DIAG_PC_CP;
        ENG_LOG("%s: pc read", findNameByChnlType(chnl_type));
        status  = SIO_ReadPPPFrame(COM_DEBUG, buff, len, &retRd);
    }else if(type == CP_AT){
        chnl_type = CHNL_AT_PC_CP;
        ENG_LOG("%s: pc read", findNameByChnlType(chnl_type));
        SIO_ATC_ReadCmdLine(buff, len, &retRd);
    }else{
        ENG_LOG("unknow type: %d", type);
    }

    ENG_LOG("%s: pc read return = %d", findNameByChnlType(chnl_type), retRd);
    printData(findNameByChnlType(chnl_type), type, buff, retRd, PRINT_NUM_MAX, PRINT_BRK_FLAG);
    return retRd;
}

PUBLIC int pc_write_data(CHNL_DATA_TYPE type, char* buff, int len)
{
    uint32 ret = 0;

    if(type == CP_DIAG){
        ENG_LOG("%s: pc write count = %d", findNameByChnlType(CHNL_DIAG_CP_PC), len);
        printData(findNameByChnlType(CHNL_DIAG_CP_PC), type, buff, len, PRINT_NUM_MAX, PRINT_BRK_FLAG);
        //ret = SIO_SendFrame(buff, len);
        if (s_pc_connect_type == PC_CONNECT_COM){
            SIO_SendCommData(buff, len);
            ENG_LOG("%s: pc write return %d", findNameByChnlType(CHNL_DIAG_CP_PC), ret);
        }else if(s_pc_connect_type == PC_CONNECT_WIFI){
            pc_sock_write_data(CP_DIAG, buff, len);
        }else{
            ENG_LOG("%s: unkown type: s_pc_connect_type = %d", __FUNCTION__, s_pc_connect_type);
        }
    }else if(type == CP_AT){
        ENG_LOG("%s: pc write count = %d", findNameByChnlType(CHNL_AT_CP_PC), len);
        printData(findNameByChnlType(CHNL_AT_CP_PC), type, buff, len, PRINT_NUM_MAX, PRINT_BRK_FLAG);
        SIO_ATC_WriteCmdRes(buff, len);
        ENG_LOG("%s: pc write return ", findNameByChnlType(CHNL_AT_CP_PC));
    }else{
        ENG_LOG("unknow type: %d", type);
    }

    return -1;
}

PUBLIC int pc_write_diag(char* buff, int len)
{
    int ret = 0;
    if (s_pc_connect_type == PC_CONNECT_COM){
        ret = SIO_SendDiag(buff, len);
        ENG_LOG("%s: pc write return %d", findNameByChnlType(CHNL_DIAG_CP_PC), ret);
    }else if(s_pc_connect_type == PC_CONNECT_WIFI){
        uint8* txbuff = (uint8 *)SCI_ALLOC_APP(len*2+2);
        if(txbuff != NULL){
            int txsize = 0;
            SCI_MEMSET(txbuff, 0, len*2+2);
            SCI_MEMCPY(txbuff+1, buff, len);
            txbuff[0] = DIAG_FLAG;
            txbuff[len+1] = DIAG_FLAG;
            txsize = diag_encode(txbuff, len+2);
            ret = pc_sock_write_data(CP_DIAG, txbuff, txsize);
            SCI_FREE(txbuff);
        }else{
            ENG_LOG("%s: fatal error---malloc %d fail!!", __FUNCTION__, len*2);
        }
    }else{
        ENG_LOG("%s: unkown type: s_pc_connect_type = %d", __FUNCTION__, s_pc_connect_type);
    }

    return ret;
}

PUBLIC int pc_write_package(char* buff, int len)
{
    int ret = 0;

    if (s_pc_connect_type == PC_CONNECT_COM){
        ret = SIO_SendPacket(buff, len);
        ENG_LOG("%s: pc write return %d", findNameByChnlType(CHNL_DIAG_CP_PC), ret);
    }else if(s_pc_connect_type == PC_CONNECT_WIFI){
        uint8* txbuff = (uint8 *)SCI_ALLOC_APP(len*2+2);
        if(txbuff != NULL){
            int txsize = 0;
            SCI_MEMSET(txbuff, 0, len*2+2);
            SCI_MEMCPY(txbuff+1, buff, len);
            txbuff[0] = DIAG_FLAG;
            txbuff[len+1] = DIAG_FLAG;
            txsize = diag_encode(txbuff, len+2);
            ret = pc_sock_write_data(CP_DIAG, txbuff, txsize);
            SCI_FREE(txbuff);
        }else{
            ENG_LOG("%s: fatal error---malloc %d fail!!", __FUNCTION__, len*2);
        }
    }else{
        ENG_LOG("%s: unkown type: s_pc_connect_type = %d", __FUNCTION__, s_pc_connect_type);
    }

    return ret;
}

LOCAL void pc_at_callback(uint32 len)
{
    int retRD = 0;
    int retWR = 0;
    char *buff = (uint8 *)SCI_ALLOC_APP(AT_BUFF_SIZE);
    SCI_MEMSET(buff, 0, AT_BUFF_SIZE);

    ENG_LOG("%s: pc_at_callback.....", findNameByChnlType(CHNL_AT_PC_CP));

    if((retRD = pc_read_data(CP_AT, buff, AT_BUFF_SIZE)) > 0)
    {
        uint8   *output_msg_ptr = SCI_NULL;
        uint16  output_len = 0;
        int ret = 0;
        //add engpc process
        ret = chnl_at_trans(&output_msg_ptr, &output_len, buff, retRD);
        if(ret == ENG_DIAG_NO_RESPONSE){
            return ;
        }
        if (output_len != 0)
        {
            pc_write_data(CP_AT, output_msg_ptr, output_len);
            SCI_FREE(output_msg_ptr);
            output_len = 0;
        }else{
            retWR = cp_write_data(CP_AT, buff, retRD);
        }
    }

    SCI_FREE(buff);
}

extern void chnl_daps_start();
PUBLIC void chnl_pc_init()
{
    SIO_ATC_SetCallback(pc_at_callback);
    chnl_daps_start();
    chnl_wifi_init(CHNL_TYPE_FT);
    chnl_sock_init(CHNL_TYPE_FT);
}
#endif

