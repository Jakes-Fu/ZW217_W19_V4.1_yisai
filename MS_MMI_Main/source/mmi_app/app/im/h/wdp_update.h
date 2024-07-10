#ifndef _WDP_UPDATE_H_
#define _WDP_UPDATE_H_ 
#include "mmk_type.h"
#include "app_tcp_if.h"
#include "mmk_timer.h"
#include "mn_api.h"

#define SG_SOCKET_WAP_GATEWAY  0x0A0000AC //"10.0.0.172" 
#define SG_SKINS_ADDR          0x3D87B506 //"61.135.181.6"//"221.179.173.171"
#define SG_SKIN_SERVER_URL     "fp.shouji.sogou.com"
#define SG_MINIMIZE_FREESPACE           (1000 * 1024)
#ifdef MAINLCD_SIZE_240X320
#define SG_DOWNLOAD_SKIN_MINIMIZE_FREESPACE (11 * 1024)
#elif defined MAINLCD_SIZE_128X160
#define SG_DOWNLOAD_SKIN_MINIMIZE_FREESPACE (8 * 1024)
#elif defined MAINLCD_SIZE_176X220
#define SG_DOWNLOAD_SKIN_MINIMIZE_FREESPACE (9 * 1024)
#elif defined MAINLCD_SIZE_240X400
#define SG_DOWNLOAD_SKIN_MINIMIZE_FREESPACE (11 * 1024)
#elif defined MAINLCD_SIZE_320X480
#define SG_DOWNLOAD_SKIN_MINIMIZE_FREESPACE (14 * 1024)
#else
#define SG_DOWNLOAD_SKIN_MINIMIZE_FREESPACE (14 * 1024)
#endif

typedef enum
{
	SG_IM_UPD_S_START,
	SG_IM_UPD_S_SETTING_NETWORK,
	SG_IM_UPD_S_CREATING_SOCKET,
	SG_IM_UPD_S_UNPACKING,
	SG_IM_UPD_S_END,
       SG_IM_UPD_S_PDPACTIVE_TIMEOUT,
	SG_IM_UPD_S_NONE
} SG_IM_UPD_S_E;

typedef struct {
	uint16   startpos;
	uint16   index;
	uint16   pic_name[40];
	uint8    picID[50];
	uint8    dirLevel[5];
	uint8    dirLevelIndex[5];
	uint8    level;
}SkinInf;

/*****************************************************************************/
//  Description : ���»ص�����
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGUpdateCallBackFunc(void);

/*****************************************************************************/
//  Description : �����������״̬
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGSetUpdateStatus(SG_IM_UPD_S_E status);

/*****************************************************************************/
//  Description : �ͷŸ���
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGUpdateRelease(void);

/*****************************************************************************/
//  Description : ���³�ʱ
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGUpDateTimeout(void);

/*****************************************************************************/
//  Description : ��ȡƤ����������ַ
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGGetSkinServerAdd(void);

/*****************************************************************************/
//  Description : �����������״̬
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGSetUpdateStatus(SG_IM_UPD_S_E status);

/*****************************************************************************/
//  Description : ��ȡTIMER id
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIIM_SGGetLibTimerId(void);

/*****************************************************************************/
//  Description : ��ȡPDP����TIMER id
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIIM_SGPdpTimerId(void);

/*****************************************************************************/
//  Description : ���õ�ǰѡ���sim id
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIIM_SGSetSelectedSIM(MN_DUAL_SYS_E sim_id);

/*****************************************************************************/
//  Description : ��ȡ��������ַ
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIIM_SGGetServerAddress(void);

/*****************************************************************************/
//  Description : ��ȡ��������״̬
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_SGGetIsSetNetwork(void);

/*****************************************************************************/
//  Description : ��ȡ��������ʱ��ʱ��ID
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIIM_GetServerDelayTimerId(void);

/*****************************************************************************/
//  Description : ��ȡʹ�õ�PDP id
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIIM_SGGetNetId(void);
#endif
