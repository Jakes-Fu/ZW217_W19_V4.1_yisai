
/****************************************************************************************/
/*                                                                                      */
/*   demo:                                                                              */
/*        get nand vendor's ftl software function pointer.  SCI_FTL_LAYER will use these*/
/*        ftl pointer to operate nand flash                                             */
/*                                                                                      */
/*                                                                                      */
/*        if nand vendor do not provide ftl software, a default nand ftl software       */
/*        provided by spreadtrum will applied                                           */
/*                                                                                      */
/****************************************************************************************/
#include "sci_ftl_api.h"
#include "sprd_ftl_probe.h"
#include "xsr_probe.h"

typedef enum
{
	MCP_SW_VENDOR_L_SAMSUNG = 0xEC,
	MCP_SW_VENDOR_L_MICRON  = 0x2C,
	MCP_SW_VENDOR_L_TOSHIBA	= 0x98,
	MCP_SW_VENDOR_L_HYNIX   = 0xAD,
	MCP_SW_VENDOR_L_Numonyx	= 0x20
} MCP_SW_VENDOR_E;

#define _GETMID(ID) ((uint8)(ID>>8))

SCI_FTL_PROBE* SCI_GetVendorSwProbe(unsigned __int64 ID)
{
	uint8	MID;

	MID	= _GETMID(ID);
#if 0
	switch(ID)
	{
		case MCP_SW_VENDOR_L_SAMSUNG:
				return ???;
				break;
		case MCP_SW_VENDOR_L_HYNIX:
				return ???;
				break;
		case MCP_SW_VENDOR_L_Numonyx:
				return ???;
				break;
		case MCP_SW_VENDOR_L_MICRON:
				return ???;
				break;
		case MCP_SW_VENDOR_L_TOSHIBA:
				return ???;
				break;
		default:
			return &g_sprd_ftl_probe;
			break;
	}
#else
//#ifdef NFTL_OPT_SPRD_NFTL
//	return &g_sprd_ftl_probe;
//#elif defined NFTL_OPT_XSR_LAYER
    return &g_xsr_probe;
//#else
//	return &g_sprd_ftl_probe;
//#endif

#endif

}


