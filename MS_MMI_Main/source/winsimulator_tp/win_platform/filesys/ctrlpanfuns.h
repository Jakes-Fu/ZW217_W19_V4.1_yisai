#ifndef _CTRLPANFUNS_H
#define _CTRLPANFUNS_H

#include "sci_api.h"
#include "sci_types.h"

void InnerThreadContextSave();

void InnerThreadContextRestore();

void InnerSCICreateSignal(xSignalHeader * ppSig,
						  uint16 nSigCode,
						  uint16 nSigSize,
						  BLOCK_ID nSender
						  );

void InnerSCISendSignal(xSignalHeader pSig,					
					    BLOCK_ID nReceiver
						);

void InnerSCIFreeSignal(xSignalHeader pSig);

int SendMsgToCtrlPanTask(xSignalHeader psig);

#endif //_CTRLPANFUNS_H