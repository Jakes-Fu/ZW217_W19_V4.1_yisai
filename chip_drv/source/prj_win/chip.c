/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

PUBLIC CHIP_TYPE_E CHIP_GetChipType (void)
{
#ifdef PLATFORM_SC6800H
	#ifdef CHIP_VER_6800H1
	return SC6800H;
	#endif
	#ifdef CHIP_VER_6800H3
	return SC6800H3;
	#endif
	return SC6800H;
#endif

#ifdef PLATFORM_SC8800G
	#ifdef CHIP_VER_8800G
	return SC8800G2;
	#endif
	#ifdef CHIP_VER_8801G
	return SC8801G2;
	#endif
	#ifdef CHIP_VER_8802G
	return SC8802G2;
	#endif
	return SC8800G2;
#endif

#ifdef PLATFORM_SC6530
	return SC6530;
#endif

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
	return SC6531EFM;
#endif

	return CHIP_TYPE_MAX;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

/* diag_routine_sp7100.c  */
