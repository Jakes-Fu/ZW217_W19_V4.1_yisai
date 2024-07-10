/************************************************************************
* 版权所有 (C)2010,扬讯科技。
*
* 文件名称： // mex_app_id.h
* 文件标识：
* 内容摘要： // 定义应用程序ID
*
************************************************************************/

#ifndef __MEX_APP_ID_H__
#define __MEX_APP_ID_H__

//平台特殊版本ID，也就是应用程序ID
typedef enum
{
    E_MSVT_Normal       = 0,                //正常版本
    E_MSVT_QQ           = 11111,            //qq版本
    E_MSVT_MSN          = 11110,            //msn版本
    E_MSVT_TB           = 10804,            //tb版本
    E_MSVT_SMS          = E_MSVT_Normal,    //短信版本 (暂时未使用)
    E_MSVT_FETION       = 11109,            //飞信版本
    E_MSVT_WOCHAT       = 11113,            //沃米
    E_MSVT_LWSG         = 11072,            //乱舞三国
    E_MSVT_ZT           = 11114,            //征途
    E_MSVT_FARM         = 11107,            //农场
    E_MSVT_ZWDZJS       = 11070,            //植物大战僵尸
    E_MSVT_DDZ          = 11008,            //斗地主
    E_MSVT_FKCS         = 11064,            //CS
    E_MSVT_JSCB         = 11104,            //金山词霸
    E_MSVT_CSJ          = 11108,            //彩世界
    E_MSVT_ZFT          = 11117,            //找房
    E_MSVT_YMDS         = 11074,            //幽默大师
    E_MSVT_PLAYER       = 11115,            //播放器
    E_MSVT_HDHL         = 11116,            //红豆婚恋
    E_MSVT_HXZ          = 11082,            //说给大家听
    E_MSVT_DMC          = 11162,            //电子保卡
    E_MSVT_FNDXN        = 11178,            //愤怒的小鸟
    E_MSVT_WEATHER      = 11197,            //天气预报
    E_MSVT_WEIBO        = 11189,            //新浪微博
    E_MSVT_WOMIVIDEO    = 11113,            //沃米视频
    E_MSVT_TOMCAT       = 11156,            //Tom 猫
    E_MSVT_FAUI			= 11204,            //FAUI
    E_MSVT_FRUITSLICE   = 11157,            //切水果
    E_MSVT_AQ           = 11181,            //号码卫士
    E_MSVT_CQZ          = 11155,            //吹裙子
	E_MSVT_BULLETSFLY   = 11235,            //叫子弹飞
	E_MSVT_MLSZ         = 11284,            //魔力骰子
	E_MSVT_MVBOL        = 11282,            //动感平衡球
	E_MSVT_HDCQZ        = 11261,            //裙角飞扬
	E_MSVT_HDTQYB       = 11299,            //HD天气
	E_MSVT_HTC_TQ       = 11323,            //HTC天气
	E_MSVT_I_TQ         = 11324,            //i天气
	E_MSVT_LIGHTER      = 11240,            //魔幻打火机
	E_MSVT_LIGHTERHD    = 11317,            //HD魔幻打火机
    E_MSVT_DZDP         = 11273, 	        //大众点评
    E_MSVT_FNDXN_RIO    = 11321, 	        //愤怒的小鸟:里约大冒险
    E_MSVT_SETTIME      = 11175, 	        //自动对时
    E_MSVT_JJFB         = 11005, 	        //机甲风暴
    E_MSVT_JPFCKB       = 11289, 	        //极品飞车
    E_MSVT_GANJILIFE    = 11283, 	        //赶集生活
    E_MSVT_MEXSQAPP     = 11321, 	        //通用社区
    E_MSVT_SHSL         = 11234,            //深海狩猎
    E_MSVT_CSJJS         = 11267,            //cs狙击手
    E_MSVT_BLFC         = 11275,            //暴力飞车
    E_MSVT_MXSQY        = 11017,            //明星三缺一
    E_MSVT_HDCAT        = 11331,            //HD猫
    E_MSVT_DZT        = 11266,            //丢纸团 
	
    E_MSVT_Max
} E_Mex_Special_Version_Type;



#endif // __MEX_APP_ID_H__

