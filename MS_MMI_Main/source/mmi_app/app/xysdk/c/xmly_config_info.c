#include "xmly_config_info.h"

#if 1		//YS_公版key
#define XY_APP_KEY "af440de526064e8caa2eeb1b03438edb"
#define XY_APP_SECRET "C04F065038BE25103BEF9B7509A0F2F7"
#define XY_SN "11713_00_100713"

#define XY_HOMEPAGE_CH_ID  "04ce0e1271984007961d968f167a5750"
#else		// XMLY_努比亚key
#define XY_APP_KEY "e8106170770244978931eb3dd560ed11"
#define XY_APP_SECRET "7088A514935ABA87F0348B538020F389"
#define XY_SN "111060_00_1001060"

#define XY_HOMEPAGE_CH_ID  "bb81c10521b047f2bd231086210537d1"
#endif 

PUBLIC void mmixysdk_api_get_appkey(char *appkey)
{
	if(appkey)
	{
		strcpy(appkey,XY_APP_KEY);
	}
}


PUBLIC void mmixysdk_api_get_appsecret(char *appsecret)
{
	if(appsecret)
	{
		strcpy(appsecret,XY_APP_SECRET);
	}
}

PUBLIC void mmixysdk_api_get_sn(char *sn)
{
	if(sn)
	{
		strcpy(sn,XY_SN);
	}
}

PUBLIC void mmixysdk_api_get_ch_id(char *chid)
{
	if(chid)
	{
		strcpy(chid,XY_HOMEPAGE_CH_ID);
	}
}

/*watch端返回vol值给xmly端
 *
 *
 */
PUBLIC int  mmixysdk_api_get_watch_vol()
{
	return 0;
}

/*xmly端传vol值给watch端
 *
 *
 */
PUBLIC void  mmixysdk_api_set_watch_vol(int vol)
{
	
}

uint32 xmly_iot_socket_get_netid(){

	return MMIAPIPDP_GetPublicPdpLinkNetid();

}

PUBLIC BOOL mmixysdk_api_get_save_record_file_flags()
{
	return FALSE; /*TRUE 保存在D:\\xmly\\xmly_record.pcm，FALSE 不保存*/
}


