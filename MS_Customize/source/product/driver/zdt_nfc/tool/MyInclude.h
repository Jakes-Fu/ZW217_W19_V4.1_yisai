#ifndef __MYINCLUDE_H__
#define __MYINCLUDE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "zdt_nfc.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//commmon
#include "nfc_list.h"
#include "nfc_des.h"
#include "sm4.h"
#include "buffer.h"
#include "MyTool.h"
//iso
#include "tag.h"
#include "nfcc.h"
#include "apdu.h"
#include "mifare.h"
#include "snci_lib.h"
//app
#include "CardReader.h"
#include "CardSimulation.h"
#include "FirmwareBank1.h"
#include "FirmwareBank2.h"
#include "FirmwareUpdate.h"
#include "SerialCommand.h"

#include "CardReader.h"



#ifdef __cplusplus
}
#endif
#endif
