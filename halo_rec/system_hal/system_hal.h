#include "halo_role_config.h"
#if(PLAY_ROLE == PLAY_AP)
#include "system_mocor.h"

#elif(PLAY_ROLE == PLAY_CP)
#include "system_osi.h"

#endif