
/*
*此文件为VM lib内、外部接口的函数原型及数据结构定义，请不要修改这个文件。
*/

#ifndef _HERO_ENGINE_API_H_
#define _HERO_ENGINE_API_H_

#include "mmk_type.h"

/*image process cmd*/
#define HR_IMG_GET_INFO 3001
#define HR_IMG_DEC_JPG 3002
#define HR_IMG_GET_STAT 3003
#define HR_IMG_DEC_GIF 3004
#define HR_IMG_FREE_GIF 3005
#define HR_IMG_DRAW_JPG 3010 //绘制JPG图片

/*mr AUDIO type*/
#define HR_ACI_AUDIO_MIDI 1
#define HR_ACI_AUDIO_WAV 2
#define HR_ACI_AUDIO_MP3 3
#define HR_ACI_AUDIO_AMR 4
#define HR_ACI_AUDIO_PCM 5
#define HR_ACI_AUDIO_AAC 6
#define HR_ACI_AUDIO_M4A 7

/*audio process cmd*/
#define HR_ACI_OPEN	         1       //打开设备
#define HR_ACI_LOAD_FILE	     2	     //加载一个文件
#define HR_ACI_LOAD_BUF       3	     //加载缓冲数据
#define HR_ACI_PLAY           4	     //从当前的位置开始播放
#define HR_ACI_PAUSE          5       //暂停播放
#define HR_ACI_RESUME         6       //继续播放
#define HR_ACI_STOP           7       //停止播放
#define HR_ACI_CLOSE          8       //关闭设备
#define HR_ACI_STATUS         9       //获取当前设备的状态
#define HR_ACI_POSITION_TIME  10      //设置播放位置，已经播放了多少时间
#define HR_ACI_POSITION_LEN   11      //设置播放位置，已经播放了相对文件或者缓冲的起始位置的偏移量
#define HR_ACI_TOTALTIME      12      //获取整首歌的播放时间
#define HR_ACI_PLAYTIME       13      //获取当前的播放进度时间
#define HR_ACI_PLAYLEN        14      //获取当前的播放进度，相对起始位置的偏移量
#define HR_ACI_PLAYTIME_MS    15      //获取当前的播放进度时间(ms)
#define HR_ACI_SETTIME_MS     31      //设置当前的播放进度时间(ms)

#define HR_ACI_MULTIPATH_OPEN     22  //多通道播放加载数据
#define HR_ACI_MULTIPATH_PLAY     23  //多通道播放已加载的数据
#define HR_ACI_MULTIPATH_STOP     24  //多通道播放停止
#define HR_ACI_MULTIPATH_CLOSE    25  //释放多通道播放加载的数据


/*PLAT CMD CODES DEFINE*/
#define HR_PLAT_ADAPT_CPUCLK 1	//适配CPU频率
#define HR_PLAT_GET_KEY_NUM 2		//获取key映射的数目
#define HR_PLAT_GET_KEY_STATE 3    //查询某键的状态
#define HR_PLAT_NES_ACTION_IND 4	//模拟器游戏启动或关闭通知
#define HR_PLAT_ENVINFO_WRITE 5	//MRP应用写记录
#define HR_PLAT_ENVINFO_READ 6	//MRP应用读记录
#define HR_PLAT_SET_KEYSOUND 7	//按键音开启关闭控制: 0 关闭， 1 开启
#define HR_PLAT_SET_NESWAPSAVEDEV 8	//设置wap下载的.nes保存所使用的存储器。
#define HR_PLAT_SET_LCD_REFRESH_MODE 9	//设置LCD刷屏方式:0 同步，1异步
#define HR_PLAT_SET_TP_SOUND         10//设置触屏提示音:0 关闭，1开启
#define HR_PLAT_SET_LCD_ROTATE       101//设置LCD旋转角度

#ifdef HERO_ENGINE_HMGS_SUPPORT
#define HR_GET_PHONE_NUMBER_LOCATION__x   902 // 查询手机号码归属地
#endif

// 20181107 start
#ifdef HERO_ENGINE_TTS_SUPPORT
#define HR_PLAY_TTS_REQ__x   910
#define HR_STOP_TTS_REQ__x   911
#define HR_GET_TTS_TYPE   912
#endif
// 20181107 end

#define HR_PLAT_CALL_INTERFACE_SUPPORT   920  //移植层hr_call能否正常支持

#if defined(HERO_ENGINE_TTS_SUPPORT)&&!defined(HERO_ENGINE_TTS_PLATUI)
#define HERO_GET_TTS_SWITCH__x    9800
#define HERO_SET_TTS_SWITCH__x    9801
#endif

#define HERO_HTTP_SUPPORT__x				9700
#define HERO_HTTP_GET_REQ__x				9701
#define HERO_HTTP_POST_REQ__x				9702
#define HERO_HTTP_STOP_REQ__x				9703
#define HERO_HTTP_DECODE_PKG__x				9704
#define HERO_HTTP_UPDATE_EXTDATA__x			9705

//add 20200507 begin
#define HERO_APP_GET_PRICE_REQ				9720
#define HERO_APP_REST_PRICE_REQ				9721 
#define HERO_APP_IS_SHOW_NETTIP_REQ			9722
#define HERO_APP_GET_TOOLBAR_TYPE_REQ		9723
#define HERO_APP_GET_MENU_SWTICE_REQ		9724
//add 20200507 end

#define HR_GET_DEVICE_SUPPORT  8001
#define HR_CHECK_GPRS_SUPPORT  8002

#define HR_PLAT_CONNECT_RESULT 1001	//NO-BLOCKING mode时查询当前连接状态
#define HR_PLAT_SET_CONNECT_TIMEOUT 1002		//设置连接超时时间
#define HR_PLAT_APP_ACTIVE2FRONT_REQ 1003		//通知移植层将后台运行的应用激活到前台来运行。
#define HR_PLAT_SET_ACTIVE_SIM 1004		//多sim卡待机时设置所用sim卡
#define HR_PLAT_APP_PICSTOP_IND      1006  //停止在idle上显示图标
#define HR_PLAT_SMS_RECIVE_ACTION    1011  //设置收到新短信时是否显示提示界面
#define HR_PLAT_GET_BACKLIGHT_STATE  1020  //获取当前背光状态
#define HR_PLAT_GET_SC_REQ           1106  //获取短信中心的号码的请求
#define HR_PLAT_VM_PAUSE_MODE 1202		//设置VM在pause时移植层是否还将事件传达至VM。
#define HR_PLAT_GET_TC_INFO 1205 	//	获取手机是否是触摸屏信息。
#define HR_PLAT_GET_LANG_INFO 1206 	//	获取手机语言设置信息。
#define HR_PLAT_GET_RAND_NUM 1211   //取随机数。
#define HR_PLAT_GET_CUR_SCENE        1213  //获取当前情景模式
#define HR_PLAT_FORBID_HUP2IDLE      1214  //屏蔽该界面下按挂机键回idle的功能
#define HR_PLAT_GET_CELLINFO_START   1215  //初始化cell id
#define HR_PLAT_GET_CELLINFO_END     1216  //结束获取cell id
#define HR_PLAT_WEATHER_UPDATED_IND 1217  //
#define HR_PLAT_TCARD_STATUS         1218  //获取存储卡状态
#define HR_PLAT_GET_FILE_POS         1231  //获得当前文件指针位置

#define HR_PLAT_GET_SOUND_VOLUME    1301	//获取声音大小的设置
#define HR_PLAT_SET_SOUND_VOLUME    1302	//设置声音的大小

#define HR_PLAT_CHECK_WIFI          1327   //检查WIFI是否开启
#define HR_PLAT_SET_WIFI_SWITCH     1328   //设置为WIFI模式
#define HR_PLAT_BACKSTAGE_SUPPORT   1391   //是否支持后台运行

//modify 20211211 begin
#define HR_PLAT_GET_DEVIDE_TYPE     1051  //获取设备类型
//modify 20211211 end

#define HR_PLAT_CAMERA_INIT         2500
#define HR_PLATEX_CAMERA_PREVIEW    2501
#define HR_PLAT_CAMERA_PAUSE        2502
#define HR_PLAT_CAMERA_RESUME       2503
#define HR_PLAT_CAMERA_CLOSE        2504
#define HR_PLATEX_CAMERA_CAPTURE    2505
#define HR_PLAT_CAMERA_ZOOM_MAX     2506
#define HR_PLAT_CAMERA_ZOOM_IN      2507
#define HR_PLAT_CAMERA_ZOOM_OUT     2508
#define HR_PLATEX_CAMERA_GETDATA    2509

/* motion sensor */
#define	HR_MOTION_STOP_LISTEN    4001
#define	HR_MOTION_PWRON           4002
#define	HR_MOTION_PWROFF          4003
#define	HR_MOTION_LISTEN_SHAKE   4004
#define	HR_MOTION_LISTEN_TILT    4005
#define	HR_MOTION_GET_MAX_ACC    4006

#define HR_PLATEX_CREATE_LIST 1
#define HR_PLATEX_SET_LISTITEM 2
#define HR_PLATEX_GET_MP3_DIR 11
#define HR_PLATEX_GET_PIC_DIR 12		
#define HR_PLATEX_GET_LCD_INFO 1001
#define HR_PLATEX_FREE_LCD_BUF 1002
#define HR_PLATEX_SOCKET_BIND 1003
#define HR_PLATEX_APP_BACKRUN_IND 1004//通知移植层本应用需要后台运行
#define HR_PLATEX_APP_PICIND_REQ 1005  //请求移植层显示一张图片（图片显示的位置由移植来确定，可能在idle，或者icon条，或者两个地方都显示等等情况）
#define HR_PLATEX_START_LOCALAPP 1006  //启动本地应用
#define HR_PLATEX_SET_WALLPAPER  1009  //设置墙纸或屏保
#define HR_PLATEX_ALLOC_MEM          1014  //分配内存扩展接口
#define HR_PLATEX_FREE_MEM           1015  //释放内存扩展接口
#define HR_PLATEX_GET_BUILD_DT       1116  //获取工程编译日期时间
#define HR_PLATEX_GET_HW_VER         1117  //获取硬件版本号
#define HR_PLATEX_SET_FILE_DEV       1204
#define HR_PLATEX_UCS_TO_GB          1207
#define HR_PLATEX_SET_RINGS          1208
#define HR_PLATEX_GET_NES_KEYSCONFIG 1210
#define HR_PLATEX_GET_NES_DIR_NAME 1220
#define HR_PLATEX_GET_NES_TITLE_NAME 1221
#define HR_PLATEX_LCD_SLEEP_OFF      1222  //设置lcd背光常开，lcd 不能进入sleep		
#define HR_PLATEX_LCD_SLEEP_ON       1223	//恢复lcd 背光设置，lcd 可定时进入sleep		
#define HR_PLATEX_GET_CELL_INFO      1224  //获取小区信息
#define HR_PLATEX_GET_NETWORK_ID     1226  //获取网络ID
#define HR_PLATEX_GET_FILE_CRTTIME   1232  //获取文件创建时间
#define HR_PLATEX_GET_DISK_SPACE 1305//获得指定盘符的剩余空加大小；
#define HR_PLATEX_GET_DISK_NAME 1306//获得指定盘符的显示名称
#define HR_PLATEX_GET_DUALSIM_INFO 1307//获得当前sim的状态-for dual sim sys

#define HR_PLATEX_SET_BLT_LAYER                  1308 //设置参与刷新的层
#define HR_PLATEX_CREATE_LAYER                   1309 //创建层
#define HR_PLAT_RELEASE_LAYER                    1310 //释放层
#define HR_PLATEX_GET_BASE_LAYER                 1311 //获得Base层句柄
#define HR_PLAT_SET_ACTIVE_LAYER                 1312 //设置激活层
#define HR_PLATEX_SET_LAYER_POS                  1313 //设置层位置
#define HR_PLATEX_SET_PNGBG_LAYER                1314 //设置PNG背景层
#define HR_PLATEX_GET_ACTICE_LAYER               1315 //获取激活层信息
#define HR_PLATEX_LAYER_APPEND                   1316 //层叠加
#define HR_PLAT_SET_LAYER_ALPHA                  1317 //设置层的透明值
#define HR_PLAT_SET_LAYER_NO_ALPHA              1318 //关闭层的透明属性
#define HR_PLAT_SET_LAYER_TRANSPARENT           1319 //设置层的透明色
#define HR_PLAT_SET_LAYER_NO_TRANSPARENT        1320 //关闭层的透明色处理
#define HR_PLATEX_LOCK_INVALIDATE                1321 //设置刷屏锁
#define HR_PLATEX_UNLOCK_INVALIDATE              1322 //取消刷屏锁

#define HR_PLATEX_FMGR_OPERATION_BASE 		1400
#define HR_PLATEX_BROWSE_HERO_FMGR_FILTER_INIT 	(HR_PLATEX_FMGR_OPERATION_BASE + 1)
#define HR_PLATEX_BROWSE_HERO_FMGR_FILTER_SET 	(HR_PLATEX_FMGR_OPERATION_BASE + 2)
#define HR_PLATEX_BROWSE_HERO_FMGR_FILTER_CLEAR 	(HR_PLATEX_FMGR_OPERATION_BASE + 3)
#define HR_PLATEX_BROWSE_FMGR 				(HR_PLATEX_FMGR_OPERATION_BASE + 4)
#define HR_PLATEX_BROWSE_FMGR_GET_PATH 		(HR_PLATEX_FMGR_OPERATION_BASE + 5)
#define HR_PLATEX_BROWSE_FMGR_EXIT 			(HR_PLATEX_FMGR_OPERATION_BASE + 6)
#define HR_PLATEX_DOVE_SET_SMS_STRATEGY    4101

#define HR_PLATEX_CREATE_PB_OBJ 4041
#define HR_PLATEX_GET_A_PB_INFO 4042
#define HR_PLATEX_CLOSE_PB_OBJ 4043
#define HR_PLATEX_SET_TIMES 5002
#ifdef __HR_CFG_MULTITP_SUPPORT__
#define HR_PLATEX_GET_TP_POINTS 4200
#endif
#define HR_PLATEX_LOCK_IRAM   0xA0003 //申请IRAM返回IRAM起始地址和长度
#define HR_PLATEX_UNLOCK_IRAM 0xA0004 //释放IRAM

#ifdef __HR_CFG_SHORTCUT_SUPPORT__
#define HR_PLATEX_SHORTCUT_SUPPORT   0xB0000 //是否支持ShortCut
#define HR_PLATEX_SHORTCUT_INSTALL   0xB0001 //安装app
#define HR_PLATEX_SHORTCUT_REMOVE    0xB0002 //卸载app
#define HR_PLATEX_SHORTCUT_PROGESS   0xB0003 //更新下载进度
#endif


#define HR_PLATEX_CHECK_WIFI_SUPPORT         0x90003   //检查是否支持WIFI
#define HR_PLATEX_GET_HEADSET_EX             9510//0x90004   //获取扩展机型 // 20190430

#define HR_PLATEX_GET_ABLUM_PATH             0x9000A   //获取系统相册默认路径
#define HR_PLATEX_IMAGE_ENCODE               0x9000B   //将RGB565的数据保存为指定格式的图片
#define HR_PLATEX_GET_TOUCHSCREEN_TYPE      0x9000C   //获取触屏类型
#define HR_PLATEX_GET_CAMERA_SUPPORT        0x9000D   //获取是否支持CAMERA接口

/* begin:added by Tommy.yan 20120120 */
#define HR_PLATEX_SEND_SMS_LOCALUI		1008	//调用本地UI发送短信
#define HR_PLATEX_SAVE_SMS				1010	//保存一条短信到收件箱中
#define HR_PLAT_GET_SMS_STATUS 		1100  	//获取短信模块状态
#define HR_PLAT_GET_SMS_NUM			1101 	//获取短信信箱中短信的条数
#define HR_PLATEX_GET_SMS_INFO			1102 	//读取指定一条短信的信息(号码、状态、时间戳)
#define HR_PLATEX_GET_SMS_CONTENT 		1103 	//读取一条短信(短信内容)
#define HR_PLATEX_DELETE_SMS			1104 	//删除一条短信
#define HR_PLAT_GET_SMS_CAPACITY		1105 	//获取手机和sim卡短信存储容量
#define HR_PLATEX_SET_SMS_STATE		0x9000F //修改收件箱/发件箱短信状态
#define HR_PLATEX_REPLY_SMS_REQ        1130  //调用本地UI回复短信
#define HR_PLATEX_SEND_MMS           	1203   	//发送彩信
/* end:added by Tommy.yan 20120120 */

//2010-02-03 add begin
#define HR_PLATEX_SKY_QQ 1332
//2010-02-03 add end

// 判断是否为自己的虚拟机
#define HR_PLATEX_GET_VM_TYPE   1610

//modify 20181117 begin
#define HR_START_DSM_REQ__x 				 900
#define HR_DSM_APP_VERSION__x				 901
// 950 - 970 已被占用
#define HR_START_HERO_APP_REQ__x			 950

#define HERO_APP_API_BASE 9000
#define HERO_APP_API_SEND_SMS 9001
#define HERO_APP_API_GET_AVAILABLE_SIM 9002
#define HERO_APP_API_ADD_KEY 9003
#define HERO_APP_API_GET_VERSION 9004
#define HERO_APP_API_SET_CYCLE 9005
#define HERO_APP_API_SET_START_PARAMETER 9006
//modify 20181117 end

#ifdef HERO_APP_SHORTCUT_SUPPORT
#define HERO_APP_API_GET_SHORTCUT_DATA 9300
#define HERO_APP_API_SET_SHORTCUT_DATA 9301
#endif

//2009-7-7 add begin
#define HR_GET_DATA_ACCOUNT_REQ          1107
#define HR_GET_DATA_ACCOUNT              1108
#define HR_SET_DATA_ACCOUNT              1109
#define HR_SET_DATA_ACCOUNT_USE_FLAG     1110
#define HR_SET_DATA_ACCOUNT_MTK_UI       1111
//2009-7-7 add end

#define HR_VOICE_RECORD_START             2700
#define HR_VOICE_RECORD_PAUSE             2701
#define HR_VOICE_RECORD_RESUME            2702
#define HR_VOICE_RECORD_STOP              2703
#define HR_VOICE_RECORD_STREAM            2704

#define HR_LCD_ROTATE_NORMAL			0
#define HR_LCD_ROTATE_90				1
#define HR_LCD_ROTATE_180				2
#define HR_LCD_ROTATE_270				3
#define HR_LCD_MIRROR					4
#define HR_LCD_MIRROR_ROTATE_90			5
#define HR_LCD_MIRROR_ROTATE_180		6
#define HR_LCD_MIRROR_ROTATE_270		7

/*如果手机平台没有定义下面的类型，将#if 语句内的定义生效*/
#if 0
typedef  unsigned short     uint16;      /* Unsigned 16 bit value */
typedef  unsigned long int  uint32;      /* Unsigned 32 bit value */
typedef  long int  int32;                /* signed 32 bit value */
typedef  unsigned char      uint8;       /* Signed 8  bit value */
typedef  signed char        int8;        /* Signed 8  bit value */
typedef  signed short       int16;       /* Signed 16 bit value */
#endif

//Type：
#define HR_CALL_RING 	0
#define HR_SMS_RING 	1
#define HR_ALARM_RING 	2


#define HR_FILE_RDONLY     1//以只读的方式打开文件。
#define HR_FILE_WRONLY     2//以只写的方式打开文件。
#define HR_FILE_RDWR       4//以读写的方式打开文件。
#define HR_FILE_CREATE     8//如果文件不存在，创建该文件。
#define HR_FILE_SHARE      16//以共享的方式打开文件。

#define HR_IS_FILE     	   1      //文件
#define HR_IS_DIR      	   2      //目录
#define HR_IS_INVALID  	   8  //无效(非文件、非目录)

/* file browser */
#define HR_FMGR_TYPE_ALL 			1
#define HR_FMGR_TYPE_FOLDER 		2
#define HR_FMGR_TYPE_FOLDER_DOT 	3
#define HR_FMGR_TYPE_UNKNOW 		4

/* image */
#define HR_FMGR_TYPE_BMP 		    5
#define HR_FMGR_TYPE_JPG 			6
#define HR_FMGR_TYPE_JPEG 			7	
#define HR_FMGR_TYPE_GIF 			8
#define HR_FMGR_TYPE_PNG 			9
#define HR_FMGR_TYPE_EMS 			10
#define HR_FMGR_TYPE_ANM 			11
#define HR_FMGR_TYPE_WBMP 			12
#define HR_FMGR_TYPE_WBM 			13

/* audio */
#define HR_FMGR_TYPE_IMY 			14
#define HR_FMGR_TYPE_MID 			15
#define HR_FMGR_TYPE_MIDI 			16
#define HR_FMGR_TYPE_WAV 			17
#define HR_FMGR_TYPE_AMR 			18
#define HR_FMGR_TYPE_AAC 			19
#define HR_FMGR_TYPE_DAF 			20
#define HR_FMGR_TYPE_VM 			21
#define HR_FMGR_TYPE_AWB 			22
#define HR_FMGR_TYPE_AIF 			23
#define HR_FMGR_TYPE_AIFF 			24
#define HR_FMGR_TYPE_AIFC 			25
#define HR_FMGR_TYPE_AU 			26
#define HR_FMGR_TYPE_SND 			27
#define HR_FMGR_TYPE_M4A 			28
#define HR_FMGR_TYPE_MMF  			29
#define HR_FMGR_TYPE_WMA 			30

/* video */
#define HR_FMGR_TYPE_3GP 			31
#define HR_FMGR_TYPE_MP4 			32
#define HR_FMGR_TYPE_AVI 			33	

/* others */
#define HR_FMGR_TYPE_JAD 			34
#define HR_FMGR_TYPE_JAR 			35
#define HR_FMGR_TYPE_VCF 			36
#define HR_FMGR_TYPE_VCS 			37
#define HR_FMGR_TYPE_THEME 			38
#define HR_FMGR_TYPE_TOO 			39
#define HR_FMGR_TYPE_NES 			40
#define HR_FMGR_TYPE_ZIP 			41
#define HR_FMGR_TYPE_ZPK          	42


#define HR_FILE_HANDLE int32
#define HR_SEARCH_HANDLE int32

#define HR_SUCCESS  0    //成功
#define HR_FAILED   -1    //失败
#define HR_IGNORE   1     //不关心
#define HR_WAITING   2     //异步(非阻塞)模式
//#define MR_NO_SUPPORT   -2     //不支持

#define HR_PLAT_VALUE_BASE 1000    //用于某些返回值的基础值
#define HERO_APP_AUDIO_VOL_MAX 5

//电话本操作返回值
#define HR_SMS_NOT_READY	1000
#define HR_SMS_READY		1001

#define HR_PHB_MAX_NAME 				(36*2) 
#define HR_PHB_MAX_NUMBER 			    (48*2) 
#define HR_SMS_MAX_CONTENT            (160)
#define HR_SMS_MAX_NUMBER			21
 


#define HR_PLATEX_PHB_OPERATION_BASE 		   4000
#define HR_PLATEX_PHB_SEARCH_ENTRY           (HR_PLATEX_PHB_OPERATION_BASE + 11) 
#define HR_PLATEX_PHB_SET_ENTRY              (HR_PLATEX_PHB_OPERATION_BASE + 12)
#define HR_PLATEX_PHB_GET_ENTRY                (HR_PLATEX_PHB_OPERATION_BASE + 16) 
#define HR_PLATEX_PHB_COPY_ENTRY               (HR_PLATEX_PHB_OPERATION_BASE + 17) 
#define HR_PLATEX_PHB_SET_OWNER_ENTRY        (HR_PLATEX_PHB_OPERATION_BASE + 21)  
#define HR_PLATEX_PHB_GET_OWNER_ENTRY         (HR_PLATEX_PHB_OPERATION_BASE + 22) 
#define HR_PLATEX_PHB_DELETE_ENTRY_ALL          (HR_PLATEX_PHB_OPERATION_BASE + 31) 
#define HR_PLATEX_PHB_GET_COUNT                (HR_PLATEX_PHB_OPERATION_BASE + 32) 
#define HR_PLATEX_PHB_GET_STATUS                (HR_PLATEX_PHB_OPERATION_BASE + 33)
#define HR_PLATEX_GET_PB_TOTAL_NUM             (HR_PLATEX_PHB_OPERATION_BASE + 44)

#ifdef HERO_CAMERA_SUPPORT
#define HR_CAMERA_SUPPORT	 		6510
#define HR_CAMERA_OPEN				6511
#define HR_CAMERA_CLOSE 			6512
#define HR_CAMERA_CAPTURE	 		6513
#define HR_CAMERA_CAPTURE_RELEASE   6514
#define HR_CAMERA_GET_ROTATION_MODE 6515
#endif

#ifdef HERO_QR_DECODE_SUPPORT
#define HR_QR_DECODE_SUPPORT		6610
#define HR_QR_DECODE_OPEN			6611
#define HR_QR_DECODE_CLOSE			6612
#define HR_QR_DECODE_PUSH_IMG		6613
#endif

//modify 20181114 begin
#define HERO_APP_API_GET_CELL_INFO  9007

#define HR_INIT_CALL_LOG_OPERATE		9100
#define HR_DESTORY_CALL_LOG_OPERATE		9102
#define HR_GET_CALL_LOG_COUNT			9103
#define HR_GET_CALL_LOG_BY_INDEX		9104
#define HR_DELETE_CALL_LOG_BY_INDEX		9105

#define HR_INIT_SMS_OPERATE		9120
#define HR_DESTORY_SMS_OPERATE		9121
#define HR_GET_SMS_COUNT		9122
#define HR_GET_SMS_BY_INDEX		9123
#define HR_DELETE_SMS_BY_INDEX		9124
#define HR_DELETE_SMS_IS_MEMORY_FULL		9125

#define HR_GET_AMAM_INFO				9200
#define HR_SET_AMAM_INFO				9201

#define HR_GET_EXT_PARAM				9140

#define HR_REQ_POWER_OFF				9205
#define HR_REQ_SET_VOLUME				9206
#define HR_REQ_SET_CUR_SENCE			9207

#define HR_READ_OLALA_CFG_DATA			9220
#define HR_WRITE_OLALA_CFG_DATA			9221

#define HR_SET_FORWARD_REQ				9230
#define HR_CANCEL_FORWARD_REQ			9231

#define HR_FAMILY_DURABLE_DATA_LIST_IS_FULL 9241
#define HR_FAMILY_DURABLE_DATA_LIST_SIZE 9242
#define HR_FAMILY_DURABLE_DATA_ADD 9243
#define HR_FAMILY_DURABLE_DATA_FIND_START 9244
#define HR_FAMILY_DURABLE_DATA_FIND_NEXT 9245
#define HR_FAMILY_DURABLE_DATA_REMOVE 9246
#define HR_FAMILY_CLEAR_ALL_DURABLE_DATA 9247
#define HR_FAMILY_GET_GLOBAL_BUF 9248

// smart
#define HR_START_UP_PLAT_APP  9520 
#define HR_GET_AUDIO_PLAY_LIST_DIR 9521 
#define HR_GET_SMART_AUTH_ID  9522 
#define HR_GET_CUSTOMER_KEY  9523 
#define HR_GET_CUSTOMER_HELP_STR  9504 
#define HR_GB_2_UNICODE  9505 
//modify 20181120 begin
#define HR_GET_REMINDER_NV_DATA  9506
#define HR_SET_REMINDER_NV_DATA  9507
//modify 20181120 end
//modify 20181114 end

#define HERO_FAMILY_TCP_SUPPORT 9750
#define HERO_FAMILY_TCP_INIT_CONNECTION  9751
#define HERO_FAMILY_TCP_START_CONNECTION 9752
#define HERO_FAMILY_TCP_STOP_CONNECTION 9753
#define HERO_FAMILY_TCP_IS_CONNECTED 9754
#define HERO_FAMILY_TCP_SEND_DATA 9755

//modify 20210203 begin
#define HERO_FM_AMR_STREAM_OPEN 9811
#define HERO_FM_AMR_STREAM_CLOSE 9812
#define HERO_FM_AMR_STREAM_PUSH 9813
#define HERO_FM_AMR_STREAM_RESET 9814
#define HERO_FM_AMR_STREAM_GET_CACHE_SIZE 9815
#define HERO_FM_AMR_STREAM_SET_VOLUME 9816
#define HERO_FM_AMR_STREAM_GET_MAX_CACHE_SIZE 9817
#define HERO_FM_AMR_STREAM_PAUSE 9818
#define HERO_FM_AMR_STREAM_RESUME 9819
#define HERO_FM_AMR_STREAM_GET_CURRENT_TIME 9820
#define HERO_FM_AMR_STREAM_GET_VOLUME 9821
//modify 20210203 end

#define HERO_EXTERN_CHECK_APP_SUPPORT 9860

// modify 20210301
#define HR_PHONE_FREE_SIZE_STATE 9901
// modify 20210301
typedef struct
{
	int32 index;
	int8 type;
	int8 state;
}T_HR_SET_SMS_STATE_REQ;

typedef struct
{
	int32 index;//显示时候索引
	int8 type; //HR_SMS_INBOX, HR_SMS_OUTBOX
}T_HR_GET_SMS_INFO_REQ;

typedef struct
{
	int8 t_stamp[6];  /*t_stamp[0]年t_stamp[1]月t_stamp[2]日
					   t_stamp[3]时t_stamp[4]分t_stamp[5]秒*/
	int8 num[HR_SMS_MAX_NUMBER];		/*发送方号码*/  
	int8 sms_status;	/*Read or Unread */
	int8 sms_storage;   /* HR_SMS_STORAGE_ME , HR_SMS_STORAGE_SIM*/
}T_HR_MSG_INFO;

typedef enum
{
	HR_SMS_NOBOX ,
	HR_SMS_UNREAD,
	HR_SMS_INBOX,
	HR_SMS_OUTBOX,
	HR_SMS_DRAFTS,
	HR_SMS_AWAITS,
	HR_SMS_DATA,
	HR_SMS_UNSENT,
	HR_SMS_READED,
	HR_SMS_SENT
}T_HR_SMS_BOX_E;

/**
 * 短信存储类型
 */
typedef enum
{
	/** 存储在SIM卡上 */
	HR_SMS_STORAGE_SIM = 0,
	/** 存储在手机上 */
	HR_SMS_STORAGE_ME	
} HR_SMS_STORAGE_E;

typedef struct
{
    int8  number[HR_PHB_MAX_NUMBER]; 
    int32 simid; 
}T_HR_SMS_REPLY_ENTRY_REQ;

typedef struct
{
    int8 number[HR_PHB_MAX_NUMBER]; 
    int8 content[HR_SMS_MAX_CONTENT]; 
	int32 status;
	int32 flag; 
}T_HR_SMS_SAVE_ENTRY_REQ;

/**
 * 发送短信请求
 */
typedef struct
{
	/** 目的号码(ASCII) */
	char *addr;
	/** UNICODE BE格式的内容 */
	char *content;
} HR_SMS_SEND_REQ_T;

/**
 * 保存短信的标记flag
 */
typedef enum
{
    HR_SMSSAVE_DISP_SAVINGSCREEN = 0x0010,
    HR_SMSSAVE_DISP_RESULTSCREEN = 0x0020,
    HR_SMSSAVE_DISP_SIM_OPT = 0x0200,
    HR_SMSSAVE_TO_SIM1 = 0x0400,
    HR_SMSSAVE_TO_SIM2 = 0x0800,
    HR_SMSSAVE_TO_SIM3 = 0x0800,
    HR_SMSSAVE_TO_SIM4 = 0x0800,
    HR_SMSSAVE_ENUM_END
}T_HR_SMS_SAVE_FLAG_E;

typedef enum
{
    SAFE_HISTORY_UNREAD,
    SAFE_HISTORY_READ
}SAFE_HISTORY_STATUS_E;


typedef enum
{
    SAFE_SIMCARD_1,
    SAFE_SIMCARD_2,
    SAFE_SIMCARD_3,
    SAFE_SIMCARD_4
}SAFE_SIMCARD_E;

/**
 * 保存短信请求参数
 */
typedef struct
{
	/** 短信的发送方号码 */
	int8 number[HR_PHB_MAX_NUMBER];
	/** UNICODE BE格式的内容 */
	int8 content[HR_SMS_MAX_CONTENT];
	/** 短信状态0: 表示未读 1: 表示已读 */
	int32 status;
	/** 参考 #HR_SMS_SAVE_FLAG_E, 目前 HR_SMSSAVE_TO_SIM1 or HR_SMSSAVE_TO_SIM2 */
	int32 flag;
} HR_SMS_SAVE_ENTRY_REQ_T;

enum
{
	HR_SMS_OP_GET_ME_SIZE,
	HR_SMS_OP_GET_SIM_SIZE,
	HR_SMS_OP_DELETE,
	HR_SMS_OP_GET_MSG_CONTENT,
	//wsl add for safe
	HR_SMS_OP_SAVE_SMS,
	HR_SMS_OP_SET_STATUS
};

enum {
   HR_KEY_0,               //按键 0
   HR_KEY_1,               //按键 1
   HR_KEY_2,               //按键 2
   HR_KEY_3,               //按键 3
   HR_KEY_4,               //按键 4
   HR_KEY_5,               //按键 5
   HR_KEY_6,               //按键 6
   HR_KEY_7,               //按键 7
   HR_KEY_8,               //按键 8
   HR_KEY_9,               //按键 9
   HR_KEY_STAR,            //按键 *
   HR_KEY_POUND,           //按键 #
   HR_KEY_UP,              //按键 上
   HR_KEY_DOWN,            //按键 下
   HR_KEY_LEFT,            //按键 左
   HR_KEY_RIGHT,           //按键 右
   HR_KEY_POWER,           //按键 挂机键
   HR_KEY_SOFTLEFT,        //按键 左软键
   HR_KEY_SOFTRIGHT,       //按键 右软键
   HR_KEY_SEND,            //按键 接听键
   HR_KEY_SELECT,          //按键 确认/选择（若方向键中间有确认键，建议设为该键）
   HR_KEY_VOLUME_UP,       //按键 侧键上
   HR_KEY_VOLUME_DOWN,     //按键 侧键下
   HR_KEY_CLEAR,
   HR_KEY_A,               //游戏模拟器A键
   HR_KEY_B,               //游戏模拟器B键
   HR_KEY_CAPTURE,         //拍照键

   HR_KEY_QWERTY_A,
   HR_KEY_QWERTY_B,
   HR_KEY_QWERTY_C,
   HR_KEY_QWERTY_D,
   HR_KEY_QWERTY_E,
   HR_KEY_QWERTY_F,
   HR_KEY_QWERTY_G,
   HR_KEY_QWERTY_H,
   HR_KEY_QWERTY_I,
   HR_KEY_QWERTY_J,
   HR_KEY_QWERTY_K,
   HR_KEY_QWERTY_L,
   HR_KEY_QWERTY_M,
   HR_KEY_QWERTY_N,
   HR_KEY_QWERTY_O,
   HR_KEY_QWERTY_P,
   HR_KEY_QWERTY_Q,
   HR_KEY_QWERTY_R,
   HR_KEY_QWERTY_S,
   HR_KEY_QWERTY_T,
   HR_KEY_QWERTY_U,
   HR_KEY_QWERTY_V,
   HR_KEY_QWERTY_W,
   HR_KEY_QWERTY_X,
   HR_KEY_QWERTY_Y,
   HR_KEY_QWERTY_Z,
   HR_KEY_QWERTY_DEL,
   HR_KEY_QWERTY_ENTER,
   HR_KEY_QWERTY_SPACE,
   HR_KEY_QWERTY_SYMBOL,
   /*************************************/
   HR_KEY_QWERTY_SYMBOL_1, /*;  ,*/
   HR_KEY_QWERTY_SYMBOL_2, /*:  .*/
   HR_KEY_QWERTY_SYMBOL_3, /*/ @*/
   HR_KEY_QWERTY_SYMBOL_4, /*& '*/
   HR_KEY_QWERTY_SYMBOL_5, /*! ?*/
   /*************************************/
   HR_KEY_QWERTY_SHIFT,
   HR_KEY_QWERTY_CTRL,
   HR_KEY_QWERTY_ALT,
   HR_KEY_QWERTY_FN,

   HR_KEY_NONE             //按键 保留
};

enum {
   HR_NET_ID_MOBILE,                  //移动
   HR_NET_ID_CN,          // 联通gsm
   HR_NET_ID_CDMA,       //联通CDMA
   HR_NET_ID_NONE,       //未插卡
/*2009-10-22 111,17787 add. for overseas app*/
   HR_NET_ID_OTHER = 4,     /*其他网络*/
/*2009-10-22 111,17787 add end*/
};

enum {
   HR_EDIT_ANY,              //任何字符
   HR_EDIT_NUMERIC,         // 数字
   HR_EDIT_PASSWORD,       //密码，用"*"显示
   HR_EDIT_ALPHA            //只允许输入字母和数字
};

enum {
   HR_SIM_NEW,     //新手机第一次插入SIM卡
   HR_SIM_CHANGE, //用户更换SIM卡
   HR_SIM_SAME    //未进行换卡操作
};

enum {
   HR_DIALOG_OK,                   //对话框有"确定"键。
   HR_DIALOG_OK_CANCEL,
   HR_DIALOG_CANCEL,
   HR_DIALOG_CANCEL_OK,
   HR_DIALOG_NOKEY = 100
};

enum {
   HR_DIALOG_KEY_OK,
   HR_DIALOG_KEY_CANCEL
};

enum {
   HR_LOCALUI_OK,
   HR_LOCALUI_CANCEL,
   HR_LOCALUI_APPRESUME = 2,
};

enum {
   HR_TCARD_PLUGIN = 0,//tcard插入
   HR_TCARD_PLUGOUT,//拔出
};

enum
{
	HR_MSDC_NOT_EXIST = HR_PLAT_VALUE_BASE,
	HR_MSDC_OK,
	HR_MSDC_NOT_USEFULL, /*可能在usb模式导致无法操作t卡*/
	HR_LARGE_PHONE_FREE_SPACE
};

enum {
   HR_KEY_PRESS,
   HR_KEY_RELEASE,
   HR_MOUSE_DOWN,
   HR_MOUSE_UP,
   HR_MENU_SELECT,
   HR_MENU_RETURN,
   HR_DIALOG_EVENT,
   HR_SMS_INDICATION,
   HR_EXIT_EVENT,
   HR_SMS_RESULT,  
   HR_LOCALUI_EVENT,
   HR_OSD_EVENT,	  /*11*/
   HR_MOUSE_MOVE,	  /*12*/
   HR_ERROR_EVENT,	  /*13执行异常通过这个事件来通知*/  
   HR_PHB_EVENT, 
   HR_SMS_OP_EVENT, //15
   HR_SMS_GET_SC,
   HR_DATA_ACCOUNT_EVENT,
   HR_MOTION_EVENT,
   HR_AUDIO_EVENT,
   HR_EVENT_IDLE_WIDGET, /*20*/
   HR_MCHAT_EVENT = 100,
   HR_DRM_EVENT   = 101,
   HR_TCARD_EVENT = 102,
   HR_SEND_SMS_EVENT = 900,
   HR_SEND_HEART_BEAT_EVENT = 901,
//modify 20181117 begin
   HERO_APP_EVENT_BASE = 9000,
   HERO_APP_EVENT_SMS_RET,
   HERO_APP_EVENT_NEW_MSG,
   HERO_APP_EVENT_HEART_BEAT,
//modify 20181117 end

   HR_OPREATE_SMS_EVENT = 9120,
#if (defined(HERO_ENGINE_FAMILY_SUPPORT) && defined(HERO_ENGINE_FAMILY_CONTROL_SUPPORT))
   HR_FAMILY_SMS_NOTIFY_START = 9125,
   HR_FAMILY_TCP_RECV_NOTIFY = 9126,
#endif

   HERO_APP_EVENT_HTTP_RSP = 9700,
};

typedef enum
{
    HR_DRM_EVENT_INSTALL_RIGHTS,
    HR_DRM_EVENT_MAX
}HR_DRM_EVENT_E;

//2009-7-7 add begin   
enum
{
    HR_DATA_ACCOUNT_OP_GET,
    HR_DATA_ACCOUNT_OP_SET,
};
//2009-7-7 add end   

typedef enum 
{
   HR_SOUND_MIDI,
   HR_SOUND_WAV,
   HR_SOUND_MP3,
   HR_SOUND_PCM, //8K
   HR_SOUND_M4A,
   HR_SOUND_AMR,
   HR_SOUND_AMR_WB
}HR_SOUND_TYPE;

typedef enum 
{
   HR_FONT_SMALL,
   HR_FONT_MEDIUM,
   HR_FONT_BIG
}HR_FONT_TYPE;

enum
{
   HR_SEEK_SET, 
   HR_SEEK_CUR, 
   HR_SEEK_END
};

enum
{
   MR_SOCK_STREAM,
   MR_SOCK_DGRAM
};

enum
{
   HR_IPPROTO_TCP,
   HR_IPPROTO_UDP
};

enum
{
   HR_ENCODE_ASCII,
   HR_ENCODE_UNICODE
};

#define   HR_SMS_ENCODE_FLAG   7
#define   HR_SMS_REPORT_FLAG   8
#define   HR_SMS_RESULT_FLAG   16

enum {
   HR_SOCKET_BLOCK,          //阻塞方式（同步方式）
   HR_SOCKET_NONBLOCK       //非阻塞方式（异步方式）
};

typedef enum
{
	HR_SCENE_NORMAL,
	HR_SCENE_MEETING,
	HR_SCENE_INDOOR,
	HR_SCENE_OUTDOOR,
	HR_SCENE_MUTE
}HR_SCENE_TYPE;

typedef enum
{
	HR_BACKLIGHT_OFF = HR_PLAT_VALUE_BASE,
	HR_BACKLIGHT_ON
}HR_BACKLIGHT_STATE;

typedef enum
{
    HR_SCREEN_TYPE_RESISTOR = HR_PLAT_VALUE_BASE,
    HR_SCREEN_TYPE_CAPACITOR
} HR_SCREEN_TYPE_ENUM;

typedef enum
{
	HR_DSPL_NONE,
	HR_DSPL_IMAGE,
	HR_DSPL_AUDIO,
	HR_DSPL_VIDEO,
	HR_DSPL_NES
}HR_E_FMGR_DFT_DSPL;

typedef enum
{
	HR_ERROR_NO_ERROR =0,
	HR_ERROR_UNKNOW,
	HR_ERROR_NO_SPACE,
	HR_ERROR_TERMINATED
}HR_RECORD_ERROR_E;

/**
 * 屏幕输入类型(触摸屏和键盘)
 */
typedef enum
{
	/** 只有按键, 没有触摸屏 */
	HR_SETTING_SCREEN_NORMAL = HR_PLAT_VALUE_BASE,
	/** 触摸屏和键盘都有 */
	HR_SETTING_SCREEN_TOUCH,
	/** 全触屏 */
	HR_SETTING_SCREEN_ONLY_TOUCH
} HR_SETTING_SCREEN_TYPE_E;

/*2009-10-22 111,17787 add. for overseas*/
typedef enum
{
	MR_CHINESE = HR_PLAT_VALUE_BASE,
	MR_ENGLISH,
	MR_TCHINESE,//繁体
	MR_SPANISH,//西班牙
	MR_DANISH,//丹麦语
	MR_POLISH,//波兰
	MR_FRENCH,//法国
	MR_GERMAN,//德国
	MR_ITALIAN,//意大利
	MR_THAI,//泰语
	MR_RUSSIAN,// 俄罗斯
	MR_BULGARIAN,//保加利亚
	MR_UKRAINIAN,//乌克兰
	MR_PORTUGUESE,//葡萄牙
	MR_TURKISH,//土耳其
	MR_VIETNAMESE,//越南
	MR_INDONESIAN,//印度尼西亚
	MR_CZECH,//捷克
	MR_MALAY,//马来西亚
	MR_FINNISH,//芬兰的
	MR_HUNGARIAN,//匈牙利
	MR_SLOVAK,//斯洛伐克
	MR_DUTCH,//荷兰
	MR_NORWEGIAN,//挪威
	MR_SWEDISH,//瑞典
	MR_CROATIAN,//克罗地亚
	MR_ROMANIAN,//罗马尼亚
	MR_SLOVENIAN,//斯洛文尼亚
	MR_GREEK,//希腊语
	MR_HEBREW,//希伯来
	MR_ARABIC,//阿拉伯
	MR_PERSIAN,//波斯
	MR_URDU,//乌尔都语
	MR_HINDI,//北印度
	MR_MARATHI,//马拉地语(属印欧语系印度语族) 
	MR_TAMIL,//泰米尔语
	MR_BENGALI,//孟加拉人(语
	MR_PUNJABI,//印度西北部的一地方
	MR_TELUGU//泰卢固语(印度东部德拉维拉语言)
}MR_LANGUAGE;
/*2009-10-22 111,17787 add end*/

typedef enum{
	HR_EXTERN_APP_AUDIO_STREAM = 0x01,
}HR_EXTERN_APP_SUPPORT;

typedef struct
{
   uint16 year;                 //年
   uint8  month;                //月
   uint8  day;                  //日
   uint8  hour;                 //时，24小时制
   uint8  minute;               //分
   uint8  second;               //秒
}hr_datetime;

typedef struct
{
   uint32 width;                  //屏幕宽
   uint32 height;                 //屏幕高
   uint32 bit;                    //屏幕象素深度，单位bit
}hr_screeninfo;

typedef struct
{
	uint8       IMEI[16];	      //IMEI len eq 15
	uint8       IMSI[16];	      //IMSI len not more then 15
	char        manufactory[8];  //厂商名，最大7个字符，空字节填\0
	char        type[8];         //mobile type，最大7个字符，空字节填\0
	uint32      ver;            //SW ver
	uint8       spare[12];       //备用
}hr_userinfo;

typedef struct
{
   int32 socket;
   int32 port;
}hr_bind_st;



#ifdef HR_FS_ASYN

typedef int32 (*HR_ASYN_FS_CB)(int32 result, uint32  cb_param);

typedef struct
{
   void* buf;                 //文件缓存地址
   uint32  buf_len;           //缓冲长度，即要读取/写入的长度
   uint32  offset;            //文件读取/写入偏移
   HR_ASYN_FS_CB  cb;         //回调函数
   uint32  cb_param;          //回调参数
}hr_asyn_fs_param;
#endif

typedef struct 
{	
	int32 dft_list;
	char  list_path[256];
}HR_T_FMGR_REQ;

typedef struct
{
	char *src;
	int32 src_len;
	int32 src_type;
	int32 format;
}HR_T_VOICE_REC_REQ;


typedef void (*HR_ACI_PLAY_CB)(int32 result);

typedef struct
{
    HR_ACI_PLAY_CB cb;  //回调函数
    int32 loop;
    int32 block;
}hr_audio_play_info_t;

#define HR_ACI_PLAY_COMPLETE   0  //播放结束
#define HR_ACI_PLAY_ERROR      1  //播放时遇到错误
/*------------extra structs define--------------*/

/*audio device state*/
typedef enum
{//顺序不能变	
	IDLE = 1,
	INITED,
	LOADED,
	PLAYING,
	PAUSE,
	SUSPENDED,
	SUSPENDING
}HR_AUDIO_STATUS_T;

typedef enum
{
   SRC_NAME,    //传到移植接口是文件名
   SRC_STREAM   //传到移植接口的是图片数据流
}HR_SRC_TYPE;

/*nes action event*/
typedef enum
{//DO NOT CHANGE THE VALUES
   NES_LOCK_IRAM = 0,    //模拟器游戏启动
   NES_UNLOCK_IRAM = 1,  //模拟器游戏退出
   NES_LOCK_OTHERS = 2,
   NES_UNLOCK_OTHERS = 3
}HR_NES_ACTION;

typedef enum
{//don't change the values
	HR_LIST_NO_ICON = 0,
	HR_LIST_NORMAL_ICON = 1,
	HR_LIST_SINGLE_SEL = 2,
	HR_LIST_MUTI_SEL = 3,

	HR_LIST_TYPE_MAX
}MMIHR_LISTTYPE_E;

typedef struct
{
	char *title;
	int16 itemNum;
	int16 listType;
}MMIHR_LISTBOXSELECT_T;

typedef struct
{
	int32 menu;
	char *text;
	int32 index;
	int32 selected;
}MMIHR_LISTBOXITEM_T;

typedef struct
{
   int32 socket;
   int32 port;
}HRAPP_BIND_T;

typedef struct
{
	int32 pos;
}HRAPP_AUDIO_OFFSET_T;

typedef struct
{
	char* buf;         //缓冲数据指针
	int32 buf_len;     //缓冲数据长度
}HRAPP_AUDIO_BUF_T;

#define HR_MMA_CHANNEL_MAX  (3)

typedef enum
{
	HR_MMA_INIT,
	HR_MMA_LOADED,
	HR_MMA_PLAY,
	HR_MMA_STOP,
	HR_MMA_CLOSE
}HR_MMA_STATE_E;

typedef struct
{
	int32 cur;
	int32 loop;
	int32 len;
	int32 handle;
	int32 state;
	int16 *dataBuf;

}hr_mma_info_t;

typedef struct
{
	uint32 total_channel;
	uint32 play_channel;
    uint32 hCodecHandle;
    uint32 hExpHandle;
	hr_mma_info_t mmaInfo[HR_MMA_CHANNEL_MAX];
}hr_mma_ctrl_t;

typedef struct
{
	uint8 *data;
	uint32 len;
	int32 loop;
}hr_bg_play_info_t;

/** 图片格式 */
typedef enum
{
	HR_IMAGE_IMG_TYPE_BMP,				//bitmap格式
	HR_IMAGE_IMG_TYPE_JPG,				//JPG或者JPEG格式
	HR_IMAGE_IMG_TYPE_PNG,				//PNG格式
	HR_IMAGE_IMG_TYPE_GIF,				//GIF格式
	HR_IMAGE_IMG_TYPE_SCREEN_BUFFER,	//原始的screen数据，一般为565格式
	HR_IMAGE_IMG_TYPE_NONE				//没有合法的图片格式或者不识别
} HR_IMAGE_IMG_TYPE_E;

typedef struct
{
	char *src;           //文件名，或是数据流的buf的地址，如果是文件名，是GB格式
	int32 len;           //src所指的buf的大小
	HR_SRC_TYPE src_type;    //指明src中存放的是文件名，还是数据流
}HRAPP_IMAGE_ORIGIN_T;

typedef struct
{
	int32 width;    //图片的宽度
	int32 height;   //图片的高
	int32 img_type; //图片的格式， 参考 #HR_IMAGE_IMG_TYPE_E
}HRAPP_IMAGE_INFO_T;

typedef struct
{
	char *src;           //文件名，或是数据流的buf的地址，如果是文件名，是GB格式
	int32 len;           //src所指的buf的大小
	int32 width;         //用户图片显示的区域的宽度
	int32 height;        //用于图片显示的区域的高度
	HR_SRC_TYPE src_type;   //指明src中存放的是文件名，还是数据流
	char *dest;          //解码后的图片数据存放的buf
	int32 type;
}HRAPP_IMAGE_DECODE_T;

typedef enum
{
   IMG_BMP,    //BMP 图片
   IMG_JPG,    //jpg 图片
   IMG_PNG,    //png 图片
   IMG_GIF    //gif 图片
}IMG_TYPE;

typedef struct {
	int16 w;	// 图像宽度
	int16 h;	// 图像高度
	void* buf;	// 图像数据地址
	int16* path;	// 保存的文件路径，UNICODE编码
	int32 type;	// 保存的文件类型，IMG_TYPE
} hr_image_encode_info;

typedef struct 
{
     char *src;          //当type为SRC_STREAM时，此参数为图片数据流buffer；当type为SRC_NAME时，此参数为图片相对路径
     int32 src_len;      //当type为SRC_STREAM时，此参数为图片数据流buffer大小；当type为SRC_NAME时，此参数为0
     int32 src_type;    //枚举HR_SRC_TYPE
     int32 ox;            //图片显示在x坐标
     int32 oy;            //图片显示在y坐标
     int32 w;             //图片宽度
     int32 h;              //图片高度
}T_HRAW_DIRECT_REQ;

typedef struct _HRAPP_GIF_FRAME_INFO
{
	int32 fwidth;                //本帧的宽度
	int32 fheight;               //本帧的高度
	int32 ox;                    //本帧左上角的x坐标(相对逻辑屏幕)
	int32 oy;                    //本帧左上角的y坐标(相对逻辑屏幕)
	int32 transparent_flag;      //是否需要透明显示
	int32 transparent_color;     //透明显示的颜色
	int32 delay_time;            //本帧显示的时间
	char *pdata;                 //解压好的图片数据
	struct _HRAPP_GIF_FRAME_INFO *next;      //指向下一帧的数据结构
}HRAPP_GIF_FRAME_INFO_T;

typedef struct
{
	int32 id;
	int32 width;              //gif的宽度
	int32 height;             //gif的高度
	int bg_color;             //gif的背景色
	int frame_count;          //gif总的帧数
	HRAPP_GIF_FRAME_INFO_T *first; //指向gif的第一帧的数据结构
}HRAPP_GIF_HEADER;

typedef struct
{
	int32 id;
}HRAPP_GIF_ID;

typedef struct//must align
{
	uint16 lac;	//LAC
	uint16 cell_id;	 //cell ID
    uint8 bsic; //6bit
    uint8 dummy[3]; //for align
}HRAPP_NCELL_INFO_T;

typedef struct 
{
	uint16 lac;	//LAC
	uint16 cell_id;	 //cell ID
	uint8 mnc[2];	//MNC
	uint8 mcc[3];	//MCC
	uint8 mnc3[4]; //mnc3[0]中表示的mnc的第三位
}HRAPP_CELL_INFO_T;

typedef struct 
{
	uint32 lac;	//LAC
	uint32 cell_id;	 //cell ID
	uint8 mnc[4];	//MNC
	uint8 mcc[4];	//MCC
	uint8 mnc3[4]; //mnc3[0]中表示的mnc的第三位
}HRAPP_CELL_INFO_T_V2;

typedef struct
{
	uint16	  nYear;
	uint8	  nMonth;
	uint8	  nDay;
	uint8	  nHour;
	uint8	  nMin;
	uint8	  nSec;
	uint8	  DayIndex; /* 0=Sunday */
}HRAPP_MYTIME;

/*模拟器游戏中的按键定义*/
typedef struct  _hr_nes_keys_st                       
{
   uint8 k_up;//方向向上
   uint8 k_down;//方向向下
   uint8 k_left;//方向向左
   uint8 k_right;//方向向右
   uint8 k_se;//选择键
   uint8 k_st;//开始键
   uint8 k_a;//'A'键
   uint8 k_b;//'B'键
   uint8 auto_a;//自动键'A'
   uint8 auto_b;//自动键'B'
}hr_nes_keys_st;

typedef struct
{
	uint32 total;    //总的大小
	uint32 tunit;    //总大小的单位
	uint32 account;  //剩余空间的大小
	uint32 unit;      //剩余大小的单位
}T_HR_FREE_SAPCE;
/*
Unit：
    1             ：单位是    byte
    1024          ：单位是    k byte
    1024x1024     ：单位是    m byte
    1024x1024x1024：单位是    g byte
*/

#ifdef __HR_CFG_MULTITP_SUPPORT__
#define MAX_TOUCH_POINTS 5

typedef struct
{
	/** x 坐标 */
	int16 x;
	/** y 坐标 */
	int16 y;
} HR_TOUCH_PANEL_COORD_T; 


typedef struct
{
	/** touch事件 #HR_EVENT_MOUSE_DOWN, #HR_EVENT_MOUSE_UP, #HR_EVENT_MOUSE_MOVE */
	int16 event;
	/** x 坐标 */
	int16 x;
	/** y 坐标 */
	int16 y;
	int16 z;
} HR_TOUCH_PANEL_COORD_EX_T; 


typedef struct
{   
	int16 touch_count;
	int16 padding;
	HR_TOUCH_PANEL_COORD_EX_T points[MAX_TOUCH_POINTS];
} HR_TOUCH_PANEL_COORD_LIST_T;
#endif

#ifdef __HR_CFG_SHORTCUT_SUPPORT__
/**
* \brief:标题的最大字串长度(bytes)
**/
#define HR_SCUT_TITLE_LENGHT_MAX	32  
/**
* \brief:路径参数的最大字串长度(bytes)
**/
#define HR_SCUT_PATH_LENGTH_MAX	128  
/**
* \brief:数据参数的最大字串长度(bytes)
**/
#define HR_SCUT_DATA_LENGTH_MAX	128  


/**
* \brief:下载下来的应用信息结构
**/
typedef struct 
{
	///母语的应用标题,比如:中文(UCS BE)
	char title_mt[HR_SCUT_TITLE_LENGHT_MAX]; 
	///英文的标题(asc)
	char title_en[HR_SCUT_TITLE_LENGHT_MAX]; 
	///ICON 的路径,上层应用传下来是相对虚拟机的路径.但是传给IDLE层会转换成绝对路径.
	char  image_path[HR_SCUT_PATH_LENGTH_MAX]; 
	///下载应用的厂商标识
	int32 provider; 
	///应用的唯一APPID
	int32 appid;
	///应用的总大小(bytes)
	uint32 total_size;
	///当前已经下载的大小
	uint32 cur_size; 
	///启动应用的参数.如"skyread,2914"
	char start_data[HR_SCUT_DATA_LENGTH_MAX]; 
	///应用的保存路径,应用传下来是相对路径,但是传给IDLE层会转换成绝对路径
	char filepath[HR_SCUT_PATH_LENGTH_MAX]; 
}HR_SHORTCUT_APPINFO_T;

/**
* \brief:单个应用的管理节点.
**/
typedef struct 
{
	///应用的信息.参考 #HR_SHORTCUT_APPINFO_T.
	HR_SHORTCUT_APPINFO_T info;
	///显示icon 的绝对路径(UCS LE BMP标准格式),有时IDLE端需要这个参数.
	uint16  image_path_ucs[HR_SCUT_PATH_LENGTH_MAX]; 
	///该节点是否仍然需要.
	int32  is_need;
}HR_SHORTCUT_APPINFO_ITEM_T;

/**
* \brief: 应用下载进度信息
**/
typedef struct 
{
	///下载应用的厂商标识
	int32 provider; 
	///应用的唯一APPID
	int32 appid;
	///应用总大小
	int32 total_size; 
	///应用当前已经下载的大小
	int32 progress;
}HR_SHORTCUT_PROGRESS_T;

/**
* \brief:删除应用时的参数结构
**/
typedef struct 
{
	///下载应用的厂商标识
	int32 provider; 
	///应用的唯一APPID
	int32 appid;
}HR_SHORTCUT_REMOVE_T;
#endif

#define HR_PHB_MAX_NAME 				(36*2) 
#define HR_PHB_MAX_NUMBER 			    (48*2) 
typedef struct 
{
	int8	name[HR_PHB_MAX_NAME];
	int8	number[HR_PHB_MAX_NUMBER];
	int8	homeNumber[HR_PHB_MAX_NUMBER];
	int8	officeNumber[HR_PHB_MAX_NUMBER];
	int8	faxNumber[HR_PHB_MAX_NUMBER];
	int8	emailAddress[HR_PHB_MAX_NUMBER];
}T_HR_PHB_ENTRY;

//设置手机铃声。
typedef struct
{
	int32 scene;
	int32 type;
	char *path;//gb编码 
}HR_RING_SET_T;

typedef struct
{
    int32 setType;         /* HR_IDLE_BG_PAPER 墙纸，HR_IDLE_SAVER_PAPER屏保*/
    uint8 *filename;       /*图片的相对路径*/
}HR_SET_IDLE_PAPER_REQ;

typedef enum
{
    HR_IDLE_BG_PAPER,     /*背景*/
    HR_IDLE_SAVER_PAPER   /*屏保*/
}HR_IDLE_PAPER_TYPE;

typedef struct
{
    char *addr;	// ASCII
    char *content;	// UNICODE
    uint16* subject;	// UNICODE
    uint16* file_path;	// UNICODE
}T_HERO_MMS;

typedef struct
{
	char* keystr[3];//软键字符串指针,依次：左中右,未定义的键值为0
}t_hr_keystrs;

//2007add for idle qq
typedef struct
{
	uint32 appid;   //app id
	uint8 describe[20];     //应用标志符
	uint8* param;     //留着扩展用
}hr_backstage_st;

typedef struct
{
   int32 appid;
   int32 time;
   int32 img_type;
   int32 img_size;
   int32 width;
   int32 height;
   uint16 color;
   uint8 *buff;
}hr_pic_req;

typedef struct
{
	uint8 totalNum;   //sim numbers, max to 6
	uint8 curActive;  //index of simId[] witch actived
	uint16 dummy;     //for align
	int32 simId[4];   //mapping to plat sim socket id
	char* simNamePtr[4]; //sim卡名字,unicode
	uint16 simNameLen[4]; //名字长度
}HR_DUALSIM_SET_T;

typedef struct{
	int32 x;
	int32 y;
	int32 w;
	int32 h;
	int32 size;
	char *buffer;
}hr_layer_create_info_t;

typedef struct{
	int32 x;
	int32 y;
}hr_layer_position_info_t;

typedef struct{
	int32 layer1;
	int32 layer2;
	int32 layer3;
	int32 layer4;
}hr_layer_blt_info_t;

typedef struct{
	int32 handle;
	int32 w;
	int32 h;
	char *buffer;
}hr_layer_info_t;

typedef struct{
	int32 block_id;
    GUI_RECT_T clip;
    GUI_RECT_T clips[16];
    uint8 clips_top;
}hr_layer_active_info_t;

typedef struct skyclip
{
	int32 x1;
	int32 y1;
	int32 x2;
	int32 y2;
}skyclip,*pskyclip;

typedef struct 
{
	int32 p1;
	int32 p2;
}hr_common_rsp_t;

typedef enum 
{
	VM_STOP,	/* 虚拟机空闲 */
	VM_RUN,	    /* 运行状态 */
	VM_PAUSE,	/* 暂停状态 */
	VM_BACK_RUN /* 后台运行 */
}HR_VM_STATE_E;


/*定时器到期时调用定时器事件，Mythroad平台将对之进行处理。
p是启动定时器时传入的Mythroad定时器数据*/
extern int32 hr_timer(void);
#if defined(WIN32)
int32 hr_event(int16 type, int32 param1, int32 param2);
int32 hr_stop(void);
#else
/*在Mythroad平台中对按键事件进行处理，press	= HR_KEY_PRESS按键按下，
= HR_KEY_RELEASE按键释放，key	对应的按键编码*/
extern int32 hr_event(int16 type, int32 param1, int32 param2);
/*退出Mythroad并释放相关资源*/
extern int32 hr_stop(void);
#endif


/****************外部接口定义*********************/

int32 hr_mainRom(char *arg);

int32 hr_mainTcard(char *filename);

int32 hr_mainInstallRomExtern(uint8 *p, int32 len);

int32 hr_mainInstallRom(uint8 *p, int32 len);

int32 hr_mainInstallRomFull(uint8 *p, int32 len, uint8 *appRom, int32 appRomLen);


/*暂停应用*/
extern int32 hr_pauseApp(void);

/*恢复应用*/
extern int32 hr_resumeApp(void);

/*当手机收到短消息时调用该函数*/
extern int32 hr_smsIndiaction(uint8 *pContent, int32 nLen, uint8 *pNum, int32 type);

/*对下载内容（保存在内存区中的一个下载的文件）进行判断，
若下载文件是DSM菜单，由DSM引擎对下载文件进行保存。使用
本函数时，下载文件应该已经下载完全，并且全部内容保存在
所给的内存中。*/
extern int32 hr_save_ho(void *p,uint32 l);

/*功能同hr_save_ho，但传入的是一个打开的文件句柄，文件由
调用者关闭。该函数目前尚未实现，若需要使用，请联系ouli*/
extern int32 hr_save_ho_with_handle(HR_FILE_HANDLE f);

/*用户SIM卡变更*/
extern int32 hr_newSIMInd(int16  type, uint8* old_IMSI);

/*函数hr_initNetwork使用的回调函数定义*/
typedef int32 (*HR_INIT_NETWORK_CB)(int32 result);

/*函数hr_initNetwork使用的回调函数定义*/
typedef int32 (*HR_CONNECT_CB)(int32 result);

/*函数hr_getHostByName使用的回调函数定义*/
typedef int32 (*HR_GET_HOST_CB)(int32 ip);


/****************外部接口定义*********************/







/*********************以下是抽象接口定义******************************/





/*调试打印*/
extern void hr_printf(const char *format,...);


/*内存申请*/
extern int32 hr_mem_get (char** mem_base, uint32* mem_len);
/*内存释放*/
extern int32 hr_mem_free (char* mem, uint32 mem_len);


/*******************************************************************/
/*当使用本地屏幕缓冲时使用的接口*/
/*在屏幕上绘BMP*/
extern void hr_drawBitmap(uint16* bmp, int16 x, int16 y, uint16 w, uint16 h);
/*取得获取unicode码ch指向的字体的点阵信息，并告之
该字体的宽和高，获取到的点阵信息每一个bit表示
字体的一个象素，字体每行的象素必须按字节对其，
也就是说如果一个字体宽为12，则需要用两个字节
来表示该信息，第二个字节的后四个bit为0，从第三
个字节开始才表示下一行的点阵数据*/
extern const char *hr_getCharBitmap(uint16 ch, uint16 fontSize, int *width, int *height);
/*******************************************************************/

/*启动定时器*/
extern int32 hr_timerStart(uint16 t);
/*停止定时器。*/
extern int32 hr_timerStop(void);
/*取得时间，单位ms*/
extern uint32 hr_getTime(void);
/*获取系统日期时间。*/
extern int32 hr_getDatetime(hr_datetime* datetime);
/*取得手机相关信息。*/
extern int32 hr_getUserInfo(hr_userinfo* info);
/*任务睡眠，单位ms*/
extern int32 hr_sleep(uint32 ms);
/*平台扩展接口*/
extern int32 hr_plat(int32 code, int32 param);
/*增强的平台扩展接口*/
typedef void (*HR_PLAT_EX_CB)(uint8* output, int32 output_len);
extern int32 hr_platEx(int32 code, uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb);


/*文件和目录操作*/
extern int32 hr_ferrno(void);
extern HR_FILE_HANDLE hr_open(const char* filename,  uint32 mode);
extern int32 hr_close(HR_FILE_HANDLE f);
extern int32 hr_info(const char* filename);
extern int32 hr_write(HR_FILE_HANDLE f,void *p,uint32 l);
extern int32 hr_read(HR_FILE_HANDLE f,void *p,uint32 l);
extern int32 hr_seek(HR_FILE_HANDLE f, int32 pos, int method);
extern int32 hr_getLen(const char* filename);
extern int32 hr_remove(const char* filename);
extern int32 hr_rename(const char* oldname, const char* newname);
extern int32 hr_mkDir(const char* name);
extern int32 hr_rmDir(const char* name);


/*异步文件读写接口*/
#ifdef HR_FS_ASYN
extern int32 hr_asyn_read(HR_FILE_HANDLE f, hr_asyn_fs_param *param);
extern int32 hr_asyn_write(HR_FILE_HANDLE f, hr_asyn_fs_param *param);
#endif

/*目录搜索开始*/
extern HR_SEARCH_HANDLE hr_findStart(const char* name, char* buffer, uint32 len);
/*取得一个目录搜索结果*/
extern int32 hr_findGetNext(HR_SEARCH_HANDLE search_handle, char* buffer, uint32 len);
/*目录搜索结束*/
extern int32 hr_findStop(HR_SEARCH_HANDLE search_handle);

/*退出平台*/
extern int32 hr_exit(void);

/*同步Cache*/
extern int32 hr_cacheSync(void* addr, int32 len);

/*开始手机震动*/
extern int32 hr_startShake(int32 ms);
/*结束手机震动*/
extern int32 hr_stopShake(void);

/*播放音频数据*/
extern int32 hr_playSound(int type, const void* data, uint32 dataLen, int32 loop);
/*停止播放音频*/
extern int32 hr_stopSound (int type);

/*发送一条短消息*/
extern int32 hr_sendSms(char* pNumber, char*pContent, int32 flags);
/*拨打电话*/
extern void hr_call(char *number);
/*取得网络ID，0 移动，1 联通*/
extern int32 hr_getNetworkID(void);
/*连接WAP*/
extern void hr_connectWAP(char* wap);


/*GUI 接口*/
extern int32 hr_menuCreate(const char* title, int16 num);
extern int32 hr_menuSetItem(int32 menu, const char *text, int32 index);
extern int32 hr_menuShow(int32 menu);
/*设置选中项目，保留*/
extern int32 hr_menuSetFocus(int32 menu, int32 index);
extern int32 hr_menuRelease(int32 menu);
extern int32 hr_menuRefresh(int32 menu);



extern int32 hr_dialogCreate(const char * title, const char * text, int32 type);
extern int32 hr_dialogRelease(int32 dialog);
extern int32 hr_dialogRefresh(int32 dialog, const char * title, const char * text, int32 type);

extern int32 hr_textCreate(const char * title, const char * text, int32 type);
extern int32 hr_textRelease(int32 text);
extern int32 hr_textRefresh(int32 handle, const char * title, const char * text);

extern int32 hr_editCreate(const char * title, const char * text, int32 type, int32 max_size);
extern int32 hr_editRelease(int32 edit);
extern const char* hr_editGetText(int32 edit);

extern int32 hr_winCreate(void);
extern int32 hr_winRelease(int32 win);

extern int32 hr_getScreenInfo(hr_screeninfo * screeninfo);


/*Socket接口*/
extern int32 hr_initNetwork(HR_INIT_NETWORK_CB cb, const char *mode);
extern int32 hr_closeNetwork(void);
extern int32 hr_getHostByName(const char *name, HR_GET_HOST_CB cb);
extern int32 hr_socket(int32 type, int32 protocol);
extern int32 hr_connect(int32 s, int32 ip, uint16 port, int32 type);
extern int32 hr_closeSocket(int32 s);
extern int32 hr_recv(int32 s, char *buf, int len);
extern int32 hr_recvfrom(int32 s, char *buf, int len, int32 *ip, uint16 *port);
extern int32 hr_send(int32 s, const char *buf, int len);
extern int32 hr_sendto(int32 s, const char *buf, int len, int32 ip, uint16 port);

/* begin:added by Tommy.yan for convert endian 20120117 */
extern int32 hr_str_wstrlen(const char *str);
extern char* hr_str_convert_endian(char *str);
extern char* hr_str_convert_endian_with_size(char *str, int32 size);
/* end:adde by Tommy.yan for convert endian 20120117 */
#define HR_GIF_DECODE_SUCCESS 1001

#endif
