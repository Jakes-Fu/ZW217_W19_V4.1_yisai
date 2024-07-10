#ifndef _CARDREADER_H
#define _CARDREADER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MyInclude.h"

void Nfc_Card_Init(void);
void Nfc_SimCard_Init(void);
void Nfc_Card_Discover(void);
t_DoStatus Nfc_Card_Scanning(SL_NCI_PACK *nci_recv);

t_RetStatus Snci_Send_Data(uint8_t mode,SL_NCI_PACK nci_send, uint32_t timeout, SL_NCI_PACK *nci_recv);
t_RetStatus Snci_Send_String(uint8_t mode,uint8_t* pbuf, uint16_t plen, uint32_t timeout, SL_NCI_PACK *nci_recv) ;
void Nfc_Card_Reader(void);
void Nfc_Card_Simulation(void);

#ifdef __cplusplus
}
#endif
#endif
