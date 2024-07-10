#include "os_api.h"
#include "sci_types.h"

#include "sprd_fts_type.h"
#include "sprd_fts_diag.h"
#include "sprd_fts_log.h"

#include "ftmode.h"
#include "translate_packet.h"
#include "caliparam.h"
#include "phasecheck.h"

#include "chnl.h"
#include "chnl_pc.h"
#include "sio.h"
PUBLIC int SIO_SendCommData(void *src, int size);
int countFlag(char* buff, int size, char flag){
    int count = 0;
    while(size--){
        if(*buff++ == flag){
            count++;
        }
    }

    return count;
}
int eng_diag_product_ctrl(char *buf, int len, char *rsp, int rsplen) {
  unsigned short offset = 0;
  unsigned short data_len = 0;
  int head_len = 0;
  int rsp_len = 0;
  int count = 0;
  int size = 0;
  unsigned char *nvdata = NULL;
  MSG_HEAD_T msg_head = {0};
  BOOLEAN ret = FALSE;
  char* tmpRsp = NULL;

  memcpy(&msg_head, buf+1, sizeof(MSG_HEAD_T));

  ENG_LOG("%s: enter...", __FUNCTION__);
  head_len = sizeof(MSG_HEAD_T) + 2 * sizeof(unsigned short);
  memcpy(&offset, buf+1+sizeof(MSG_HEAD_T), sizeof(unsigned short));
  memcpy(&data_len, buf+1+sizeof(MSG_HEAD_T) + sizeof(unsigned short), sizeof(unsigned short));

  ENG_LOG("%s: offset: %d, data_len: %d\n", __FUNCTION__, offset, data_len);
  ENG_LOG("%s: need buf: %d\n", __FUNCTION__, head_len + data_len);

  if (!CaliParam_Open()){
      ENG_LOG("%s: open caliparam fail..", __FUNCTION__);
      return 0;
  }

  // 2: NVITEM_PRODUCT_CTRL_READ
  // 3: NVITEM_PRODUCT_CTRL_WRITE
  ENG_LOG("%s: msg_head.subtype: %d\n", __FUNCTION__, msg_head.subtype);
  switch (msg_head.subtype) {
    case 2: {
      MSG_HEAD_T head = {0};
      memcpy(&head, &msg_head, sizeof(MSG_HEAD_T));
      nvdata = (unsigned char *)SCI_ALLOC(data_len + head_len);
      memset(nvdata, 0, data_len + head_len);
      memcpy(nvdata, buf+1, head_len);

     ret = CaliParam_ReadOffset(PARAM_WHOLEPART, offset, data_len, nvdata + head_len);
      if (!ret) {
              int data = SP09_SPPH_MAGIC_NUMBER;
        ENG_LOG("%s: Read ERROR: %d\n", __FUNCTION__, ret);
        //data_len = 0;
              memcpy(nvdata+head_len, &data, sizeof(data));
              ret = TRUE;
      }

      head.subtype = (ret==TRUE)?0:1;
      head.len = head_len+data_len;
      memcpy(nvdata, &head, sizeof(MSG_HEAD_T));

      count = countFlag(nvdata, head_len+data_len, 0x7E);
      ENG_LOG("7E count %d", count);
      size = head_len+data_len+count;
      tmpRsp = (uint8 *)SCI_ALLOC_APP(size+2);
      if(NULL == tmpRsp){
        ENG_LOG("malloc %d fail, return", size+2);
        free(nvdata);
        return 0;
      }
      SCI_MEMSET(tmpRsp, 0, size+2);

      rsp_len = translate_packet_encode(tmpRsp, nvdata, head_len + data_len);

      free(nvdata);
    } break;
    case 3: {
      nvdata = (unsigned char *)SCI_ALLOC(data_len);
      memset(nvdata, 0, data_len);

      ret = CaliParam_ReadOffset(PARAM_WHOLEPART, 0, data_len, nvdata);
      if (!ret) {
        ENG_LOG("%s: Read before writing ERROR: %d\n", __FUNCTION__, ret);
      } else {
        memcpy(nvdata, buf+1+head_len, data_len);
              ret = CaliParam_WriteOffset(PARAM_WHOLEPART, 0, data_len, nvdata);
        if (!ret) {
          ENG_LOG("%s:Write ERROR: %d\n", __FUNCTION__, ret);
        }
      }

      free(nvdata);

      msg_head.subtype = (ret==TRUE)?0:1;
      msg_head.len = sizeof(MSG_HEAD_T);

      count = countFlag((unsigned char *)(&msg_head), sizeof(MSG_HEAD_T), 0x7E);
      ENG_LOG("7E count %d", count);
      size = sizeof(MSG_HEAD_T)+count;
      tmpRsp = (uint8 *)SCI_ALLOC_APP(size+2);
      if(NULL == tmpRsp){
        ENG_LOG("malloc %d fail, return", size+2);
        return 0;
      }
      SCI_MEMSET(tmpRsp, 0, size+2);

      rsp_len = translate_packet_encode(tmpRsp, (unsigned char *)(&msg_head), sizeof(MSG_HEAD_T));
    } break;
    default:
      CaliParam_Close();
      ENG_LOG("%s: ERROR Oper: %d !\n", __FUNCTION__, msg_head.subtype);
      return 0;
  }

  CaliParam_Close();
  ENG_LOG("%s: rsp_len : %d\n", __FUNCTION__, rsp_len);

  //SIO_SendCommData(tmpRsp, rsp_len);
  if(tmpRsp){
    pc_write_data(CP_DIAG, tmpRsp, rsp_len);
    SCI_Free(tmpRsp);
    tmpRsp = NULL;
  }

  return ENG_DIAG_NO_RESPONSE;
}

int eng_diag_product_ctrl_ex(char *buf, int len, char *rsp, int rsplen) {
  int offset = 0;
  int data_len = 0;
  int head_len = 0;
  int rsp_len = 0;
  int count = 0;
  int size = 0;
  unsigned char *nvdata = NULL;
  MSG_HEAD_T msg_head = {0};
  BOOLEAN ret = FALSE;
  char* tmpRsp = NULL;

  memcpy(&msg_head, buf+1, sizeof(MSG_HEAD_T));

  head_len = sizeof(MSG_HEAD_T) + 2 * sizeof(unsigned int);
  memcpy(&offset, buf+1+sizeof(MSG_HEAD_T), sizeof(int));
  memcpy(&data_len, buf+1+sizeof(MSG_HEAD_T) + sizeof(int), sizeof(int));


  ENG_LOG("%s: offset: %d, data_len: %d\n", __FUNCTION__, offset, data_len);
  ENG_LOG("%s: need buf: %d\n", __FUNCTION__, head_len + data_len);

  if (!CaliParam_Open()){
      ENG_LOG("%s: open caliparam fail..", __FUNCTION__);
      return 0;
  }

  // 12: NVITEM_PRODUCT_CTRL_READ_EX
  // 13: NVITEM_PRODUCT_CTRL_WRITE_EX
  ENG_LOG("%s: msg_head.subtype: %d\n", __FUNCTION__, msg_head.subtype);
  switch (msg_head.subtype) {
    case 0x12: {
      MSG_HEAD_T head = {0};
      memcpy(&head, &msg_head, sizeof(MSG_HEAD_T));

      nvdata = (unsigned char *)SCI_ALLOC(data_len + head_len);
      memset(nvdata, 0, data_len + head_len);
      memcpy(nvdata, buf+1, head_len);

      ret = CaliParam_ReadOffset(PARAM_WHOLEPART, offset, data_len, nvdata + head_len);
      if (!ret) {
        int data = SP09_SPPH_MAGIC_NUMBER;
        ENG_LOG("%s: Read ERROR: %d\n", __FUNCTION__, ret);
        //data_len = 0;
        memcpy(nvdata+head_len, &data, sizeof(data));
        ret = TRUE;
      }

      head.subtype = (ret==TRUE)?0:1;
      head.len = head_len + data_len;
      memcpy(nvdata, &head, sizeof(MSG_HEAD_T));

      count = countFlag(nvdata, head_len+data_len, 0x7E);
      ENG_LOG("7E count %d", count);
      size = head_len+data_len+count;
      tmpRsp = (uint8 *)SCI_ALLOC_APP(size+2);
      if(NULL == tmpRsp){
        ENG_LOG("malloc %d fail, return", size+2);
        free(nvdata);
        return 0;
      }
      SCI_MEMSET(tmpRsp, 0, size+2);

      rsp_len = translate_packet_encode(tmpRsp, nvdata, head_len + data_len);

      free(nvdata);
    } break;
    case 0x13: {
      nvdata = (unsigned char *)SCI_ALLOC(data_len);
      memset(nvdata, 0, data_len);

      ret = CaliParam_ReadOffset(PARAM_WHOLEPART, offset, data_len, nvdata);
      if (!ret) {
        ENG_LOG("%s: Read before writing ERROR: %d\n", __FUNCTION__, ret);
      } else {
        memcpy(nvdata, buf + 1 + head_len, data_len);
              ret = CaliParam_WriteOffset(PARAM_WHOLEPART, offset, data_len, nvdata);
        if (!ret) {
          ENG_LOG("%s:Write ERROR: %d\n", __FUNCTION__, ret);
       }
      }

      free(nvdata);

      msg_head.subtype = (ret==TRUE)?0:1;
      msg_head.len = sizeof(MSG_HEAD_T);

      count = countFlag((unsigned char *)(&msg_head), sizeof(MSG_HEAD_T), 0x7E);
      ENG_LOG("7E count %d", count);
      size = sizeof(MSG_HEAD_T)+count;
      tmpRsp = (uint8 *)SCI_ALLOC_APP(size+2);
      if(NULL == tmpRsp){
        ENG_LOG("malloc %d fail, return", size+2);
        return 0;
      }
      SCI_MEMSET(tmpRsp, 0, size+2);

      rsp_len = translate_packet_encode(tmpRsp, (unsigned char *)(&msg_head), sizeof(MSG_HEAD_T));
    } break;
    default:
      CaliParam_Close();
      ENG_LOG("%s: ERROR Oper: %d !\n", __FUNCTION__, msg_head.subtype);
      return 0;
  }

  CaliParam_Close();
  ENG_LOG("%s: rsp_len : %d\n", __FUNCTION__, rsp_len);

  //SIO_SendCommData(tmpRsp, rsp_len);
  if(tmpRsp){
    pc_write_data(CP_DIAG, tmpRsp, rsp_len);
    SCI_Free(tmpRsp);
    tmpRsp = NULL;
  }

  return ENG_DIAG_NO_RESPONSE;
}
