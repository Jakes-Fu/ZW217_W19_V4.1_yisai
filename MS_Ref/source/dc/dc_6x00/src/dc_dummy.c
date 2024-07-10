#include "dc_app_6x00.h"
//#include "dal_osd.h"

/******************************************************************************
// Description:	set mpeg4 recoding param
// Author:		frank.yang
// Input:     		none
// Output:     		none
// Return:     		0           successful
//             		others      failed
// Note:       	
******************************************************************************/
PUBLIC int DC_SetMpeg4RecParam(const DC_MJPG_PARAM_T *pst_param)
{
	return SCI_SUCCESS;
}




/******************************************************************************
// Description:	start mpeg4 recording
// Author:		frank.yang
// Input:     		none
// Output:     		none
// Return:     		0           successful
//             		others      failed
// Note:       	
******************************************************************************/
PUBLIC int DC_StartMpeg4Rec(uint32 dw_framenum, DC_RECORD_CALLBACK_FUNC pf_callback)
{
	return SCI_SUCCESS;
}

/******************************************************************************
// Description:	stop mpeg4 recording
// Author:		frank.yang
// Input:     		none
// Output:     		none
// Return:     		0           successful
//             		others      failed
// Note:       	
******************************************************************************/
PUBLIC int DC_StopMpeg4Rec(void)
{
	return SCI_SUCCESS;
}

/*****************************************************************************
//  Description:	This function is used to control sensor's parameters
//  Author:         	Frank.Yang
//  Input:		mode_sequence-the index of mode arrays
//  Return:		the result of sensor operations
//  Note:           
*****************************************************************************/
#if 0
PUBLIC uint32 DSENSOR_IOCtrl(IO_CTRL_SRC src,
				uint32 param_cmd, uint32 arg)
{
	return 0;
}				


PUBLIC uint32 DSENSOR_GetInfo(IO_CTRL_SRC src,
						uint32  param_cmd,uint32*info_ptr)
{

	return 0;

}						

#endif

int DC_SetOsdEnableMode(int flag)
{
	return SCI_SUCCESS;
}

int DC_SetOsdImage(int i_zone, const uint8 *p_data)
{
	return SCI_SUCCESS;
}

/*
int DC_SetOsdZoneMode(int i_zone, const DC_OSD_MODE *pst_mode)
{
	return SCI_SUCCESS;
}
*/
int DC_SetOsdColorPalette(const uint32 *p_table)
{

	return SCI_SUCCESS;
}

int DC_ClearOSDMenu(uint16 chroma_color)
{
	return SCI_SUCCESS;
}

int DC_SetFrameImage(uint8* pFrame, uint32 nSize, uint16 chroma_color)
{
	return SCI_SUCCESS;
}






