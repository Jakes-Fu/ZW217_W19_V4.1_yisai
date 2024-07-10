#ifdef WIN32

#ifndef _MNSS_DUMMY_API_H
#define _MNSS_DUMMY_API_H

#include "mnss_dummy.h"



void MNSSDUMMY_StartSsCase(SS_TEST_CASE_INDEX_E case_index, MN_DUAL_SYS_E dual_sys);
void MNSSDUMMY_MmiDisplayRsltForSsCase( BOOLEAN is_ok, MN_DUAL_SYS_E dual_sys);

#endif
#endif
