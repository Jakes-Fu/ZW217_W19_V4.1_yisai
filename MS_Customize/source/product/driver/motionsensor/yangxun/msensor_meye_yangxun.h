
#ifndef _MSENSOR_MEYE_YANGXUN_
#define _MSENSOR_MEYE_YANGXUN_

#include "sci_types.h"


typedef uint32		BOOL;

//定义默认的灵敏度
#define MET_SENSITIVE_VALUE 1

//定义摄像头取景窗口的宽度
//定义摄像头取景窗口的高度
#define MET_IMAGE_WIDTH 	128
#define MET_IMAGE_HEIGHT 	128




//定义魔眼开关打开的标志
//定义魔眼开关关闭的标志
#define MET_CAMERA_CLOSE 0
#define MET_CAMERA_OPEN 1


// add by zhanglei 更换背景时的默认声音
#define MET_CAMERA_CHBG_DEFAULT_SOUND 0 



//定义无参的函数指针类型
typedef void (*MET_CAM_FUNC_PARAM0)(void);

//定义应用类型的枚举
typedef enum _MET_CAMERA_TYPE_APP
{
	MET_CAMERA_TYPE_APP_SHAKE_MUSIC,						//摇晃切歌
	MET_CAMERA_TYPE_APP_SHAKE_BACKGROUD,					//摇晃切背景
    MET_CAMERA_TYPE_APP_SHAKE_ALARM,                        //摇晃闹铃静音   
    MET_CAMERA_TYPE_APP_SHAKE_INCOMINGCALL,                 //摇晃来电静音
	MET_CAMERA_TYPE_APP_MAX
}MET_CAMERA_TYPE_APP;


//定义应用的结构
typedef struct _MET_CAMERA_APP
{
	int32					selected;							//可选择的应用
	int32 					available;							//选择了哪个应用
	MET_CAM_FUNC_PARAM0 	init;								//初始化的函数指针                        
}MET_CAMERA_APP;

//定义摄像头参数设置环境上下文
typedef struct _MET_CAMERA_CONTEXT
{
	BOOL					is_osd_horizontal;							//摄像头是否横放
	uint16   				preview_width;								//取摄像头数据的宽度(从数据流中截取的宽度)
	uint16   				preview_height;								//取摄像头数据的高度(从数据流中截取的高度)
	uint16					target_width;								//摄像头获取数据的目标宽度
	uint16 					target_height;								//摄像头获取数据的目标高度
	uint  					addr_intmem;								//摄像头流程内部使用的缓冲区首地址(定义在此处表示允许外部指定内存位置)
	uint8 *  				ptr_buffer;									//摄像头流程外部使用的缓冲区首地址
	int32  					delay_time;									//记录摄像头打开到准备之间的延迟信息
	BOOL  					cmotion_is_run;								//魔眼是否正在运行
	int32					cmotion_flag;								//魔眼当前运行的标志位，见宏定义
	MET_CAMERA_APP			apps[MET_CAMERA_TYPE_APP_MAX];				//魔眼应用的结构体数组
	int32 last_capp;
	
}MET_CAMERA_CONTEXT;

//定义摄像头系统设置结构体
typedef struct _MET_CAMERA_SYSTEM
{
	int32 	eviron;					//环境设置变量(默认为2 ，表示普通)
	int32 	open;					//开关设置变量(默认为1，表示开启)	?
	int32  	sound_type;            //更换背景时播放的音乐(默认值为0)   	
	int32  	selected_eviron;		//可选择的环境设置变量
	int32  	selected_open;			//可选择的开关设置变量
	int32  	selected_sound_type;    //可选择的提示音设置变量
	BOOL 	sleep_now;				//是否为屏幕休眠
	BOOL 	initialized;			//是否初始化过
	BOOL 	cut_initialized;		//切片模块是否初始化过
	 
}MET_CAMERA_SYSTEM;

//定义外部连接的函数及变量
extern MET_CAMERA_CONTEXT met_camera_context;
extern MET_CAMERA_SYSTEM  met_camera_system;

extern void * met_cam_alloc_ext_mem(uint32 size);
extern void met_cam_free_ext_mem(void ** pointer);


#endif

//#endif


