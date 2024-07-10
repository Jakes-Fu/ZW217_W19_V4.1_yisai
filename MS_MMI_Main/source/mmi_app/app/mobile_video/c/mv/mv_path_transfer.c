#define _MV_PATH_TRANSFER_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_mobile_video_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "sfs.h"
#include "mv_debug.h"
#include "sci_api.h"
#include "mv.h"

/*****************************************************************************/
//  Description : MV_path_transfer
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MV_path_transfer(uint16* orignal_path, uint16* new_path)
{
    uint16* p = PNULL;
    uint16 index = 0;
    uint16 k = 0;
#ifndef WIN32
    __align(4) char outputstr[256]={0};
#endif
    p=orignal_path;
    
    while((*p)!='\0')
	{
		if('/'==*p)
		{
			if(index==0)
			{
				p++;
				index++;
				continue;
			}
			else
			{
				new_path[k++] = '\\';
			}
        }
        else
        {
	  		new_path[k++] = *p;
			if(index==1)
            {
				new_path[k++] = ':';
			}
		}
		p++;
		index++;
	}	
    
	p=new_path;
    
	if('\\'==p[index-1])
		p[index-1]='\0';
    
#ifndef WIN32
	SCI_MEMSET(outputstr,0,256);
	CM_WideCharToMultiBytes(1,new_path,_wcslen(new_path)*2,outputstr,256);
#ifdef DEBUG_FS    
	//MV_TRACE_LOW:"MV_path_transfer	new_path[%s]"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_PATH_TRANSFER_63_112_2_18_2_39_17_138,(uint8*)"s",outputstr);
#endif
#endif
}
