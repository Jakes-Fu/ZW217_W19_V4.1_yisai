#ifndef _CONPANFUNC_H
#define _CONPANFUNC_H

#include "sci_api.h"
#include "sci_types.h"


void InnerThreadContextSave();

void InnerThreadContextRestore();

void InnerSCICreateSignal(xSignalHeader * ppSig,
						uint16 nSigCode,
						uint16 nSigSize,
						BLOCK_ID nSender);

void InnerSCISendSignal(xSignalHeader pSig,					
					    BLOCK_ID nReceiver);

void InnerSCIFreeSignal(xSignalHeader pSig);

void InnerPSRegisterMsgHandler();

int SendCPMsgToPS(xSignalHeader psig);

int SendCPMsgToPSDummy(xSignalHeader psig);

// used by ATEST - added by Mingxuan.Zhu
int SendCPMsgToMMI(xSignalHeader psig);

int SendCPMsgToConPan(xSignalHeader psig);


void InnerInsertSIM(int card_num);
void InnerRejectSIM(int card_num);
void InnerShowPadSIM(int card_num);
BOOL InnerQuerySIM(int card_num);


#endif