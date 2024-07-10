#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "sfr_carrier_manager.h"

#ifdef SFR_SUPPORT_CTCC_SMS
extern SFR_CARRIER sfr_carrier_ctcc;
#endif
#ifdef SFR_SUPPORT_CMCC
extern SFR_CARRIER sfr_carrier_cmcc;
#endif
#ifdef SFR_SUPPORT_CUCC
extern SFR_CARRIER sfr_carrier_cucc;
#endif
static SFR_CARRIER *sfr_carrier_array[] =
{
#ifdef SFR_SUPPORT_CTCC_SMS
    //ctcc
    &sfr_carrier_ctcc,
#endif

#ifdef SFR_SUPPORT_CMCC
    // cmcc
    &sfr_carrier_cmcc,
#endif

#ifdef SFR_SUPPORT_CUCC
    // cucc
    &sfr_carrier_cucc,
#endif
   //TODO, add some other
    // ......
};

#define SFR_CARRIER_COUNT (sizeof(sfr_carrier_array) / sizeof(SFR_CARRIER *))

static int sfr_carrier_idx = 0;

SFR_CARRIER *SFRM_GetFirstRegCarrier(void)
{
    SFR_CARRIER *ret = NULL;

    sfr_carrier_idx = 0;

    while (sfr_carrier_idx < SFR_CARRIER_COUNT)
    {
        ret = sfr_carrier_array[sfr_carrier_idx];
	 if (ret != NULL)
	 {
	     ret->carr_context = ret->carrier_constructer();
	     if (ret->carr_context != NULL)
	     {
	         return ret;
	     }
	 }
	 sfr_carrier_idx++;
    }
    return NULL;
}

SFR_CARRIER *SFRM_GetNextRegCarrier(void)
{
    SFR_CARRIER *ret = NULL;

    sfr_carrier_idx++;

    if (sfr_carrier_idx >= SFR_CARRIER_COUNT)
        return NULL;

    while (sfr_carrier_idx < SFR_CARRIER_COUNT)
    {
        ret = sfr_carrier_array[sfr_carrier_idx];
	 if (ret != NULL)
	 {
	     ret->carr_context = ret->carrier_constructer();
	     if (ret->carr_context != NULL)
	     {
	         return ret;
	     }
	 }
	 sfr_carrier_idx++;
    }
    return NULL;
}
