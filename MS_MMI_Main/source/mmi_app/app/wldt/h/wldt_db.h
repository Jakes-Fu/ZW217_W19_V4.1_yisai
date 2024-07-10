
#ifndef _WLDT_APP_DB_H_
#define _WLDT_APP_DB_H_

#include "wldt_common.h"

#define WLDT_DB_DIR "WLDTDB"

BOOLEAN WLDT_DB_Init(void);
BOOLEAN WLDT_DB_DelAll(void);
BOOLEAN WLDT_DB_Free(void);


#endif /*_WLDT_APP_DB_H_*/
