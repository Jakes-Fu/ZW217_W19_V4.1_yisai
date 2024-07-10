/**************************************************************************
 iekieImSocket.h
 
 Description: iekie 输入法 网络连接

 **************************************************************************/
#ifndef __IMSOCKET_H__
#define __IMSOCKET_H__

#include "IIMEk2api.h"


/*example*/
#if defined(SPREADTRUM_PRODUCT)
IIME_BOOL mmi_im_check_net_stat(void)
{
   IIME_BOOL net_stat = 0;
   net_stat = 1;
   return 1;
}
#endif


//请根据平台进行配置
#if defined(SPREADTRUM_PRODUCT)
/*填充检查网络连接状态函数*/
#define	IEKIE_CHECK_NET_STAT	mmi_im_check_net_stat
#else

#endif




#endif /* __IMSOCKET_H__ */
