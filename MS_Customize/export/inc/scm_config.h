
#ifndef _SCM_CONFIG_H_
#define _SCM_CONFIG_H_

#include "sci_types.h"
#include "scm_api.h"

extern PUBLIC uint32 SCM_TASK_STACK_SIZE ;
extern PUBLIC uint32 SCM_TASK_TASK_PRIO ;
extern PUBLIC uint32 g_slotInformationNum ;
extern SCM_SLOT_DESCRIPTION_T g_slotInformation[] ;


/*****************************************/
PUBLIC BOOLEAN SCM_BSD_REG(SCM_SLOT_NAME_E slotNO);
PUBLIC BOOLEAN SCM_BSD_UNREG(SCM_SLOT_NAME_E slotNO);

#endif // _SCM_CONFIG_H_