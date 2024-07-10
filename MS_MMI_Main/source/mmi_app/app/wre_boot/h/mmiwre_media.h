#ifndef _MMIWRE_MEDIA_H_
#define _MMIWRE_MEDIA_H_

//2.3.3	通知码
#define MEDIA_NOTIFY_FINISHED				0x1001
#define MEDIA_NOTIFY_REQUESTDATA			0x1002
#define MEDIA_NOTIFY_REPONSEDATA			0x1003
/////////////////////////////////////////////////////////////////////
//2.3.1	错误码（通知附代码）
#define MEDIA_ERROR_NOERROR			0x00000000	//没有错误
#define MEDIA_ERROR_UNKNOWNERROR	0x800A0001	//未知错误
#define MEDIA_ERROR_BADFORMAT		0x800A0002	//媒体格式不支持
#define MEDIA_ERROR_NOTENABLED		0x800A0003	//设备打开失败
#define MEDIA_ERROR_INVALHANDLE		0x800A0004	//设备句柄无效
#define MEDIA_ERROR_NOMEM			0x800A0005	//内存分配失败
#define MEDIA_ERROR_NOTSUPPORTED	0x800A0006	//功能不支持
#define MEDIA_ERROR_INVALID			0x800A0007	//无效的ID
#define MEDIA_ERROR_INVALFLAG		0x800A0008	//无效的标志
#define MEDIA_ERROR_INVALPARAM		0x800A0009	//无效的参数
#define MEDIA_ERROR_FILEOPENERR		0x800A000A	//文件打开出错
#define MEDIA_ERROR_OUTOFRANGE		0x800A000B	//数据溺出
#define MEDIA_ERROR_GETSTREAM_WAIT  0x800A000C	//流数据获取等待
#define MEDIA_ERROR_GETSTREAM_ASY   0x800A000D	//流数据异步处理中
#define MEDIA_ERROR_CONFLICT		0x800A000E	//设备被抢占
/////////////////////////////////////////////////////////////////////
//2.3.2	媒体格式支持
#define MEDIA_FORMAT_MIDI			L"audio/mid"
#define MEDIA_FORMAT_WAV			L"audio/wav"
#define MEDIA_FORMAT_MP3			L"audio/mp3"
#define MEDIA_FORMAT_AAC			L"audio/aac"
#define MEDIA_FORMAT_M4A			L"audio/m4a"
#define MEDIA_FORMAT_AMR			L"audio/amr"
#define MEDIA_FORMAT_TONE			L"audio/xts"
#define MEDIA_FORMAT_3GP			L"video/3gp"
#define MEDIA_FORMAT_MP4			L"video/mp4"
/////////////////////////////////////////////////////////////////////
typedef unsigned char BOOLEAN;
typedef unsigned long  HMEDIAHANDLE;  //设备句柄
typedef long HRESULT;
/////////////////////////////////////////////////////////////////////
//媒体控制类型
typedef enum  
{ 
	MEDIA_TYPE_UNKNOWN=-1, //未知
	MEDIA_TYPE_FILE,//文件的方式播放/录制
	MEDIA_TYPE_BUFFER,//buffer的方式播放/录制
	MEDIA_TYPE_STREAM,//流的方式进行播放/录制  
	MEDIA_TYPE_TONE  //Tone音播放
		
}eMediaType;

//指媒体seek的类型
typedef enum 
{ 
	MEDIA_SEEK_TYPE_POS, //以字节的方式进行    ===暂不支持
	MEDIA_SEEK_TYPE_TIME //以时间的方式进行
		
}eMMSeekType;

//媒体当前状态
typedef enum 
{ 
	MEDIA_STATE_UNKNOWN=-1, //未知状态
	MEDIA_STATE_PLAYING,    //正在进行播放
	MEDIA_STATE_PAUSE,      //暂停状态
	MEDIA_STATE_STOP,       //停止状态
	MEDIA_STATE_RECORDING   //正在进行录制
		
}eMMState;


//媒体初始化控制结构
typedef struct  tagMEDIADATA 
{
    unsigned char           isPlay;     //设备使用类型，TRUE -表示用来播放，FALSE-表示用来录制
    eMediaType              eType;      //媒体控制类型
    unsigned short*         lpData;     //媒体数据，辨别根据控制类型,对于流媒体，指向媒体头数据缓冲区
    unsigned long           dwDataLen;  //媒体数据长度，如果控制类型是文件，那么该值将不做参考，否则将会作为数据长度
    const unsigned short*   lpFormat; //媒体处理的格式
    void                    (*hOwner)(unsigned int wParam,unsigned int lParam,unsigned int unReserved); //媒体播放需要进行消息交互的callback，
																								//如果为空，将不进行交互
    unsigned long           dwReserved; //保留单元
    //流相关定义
    unsigned long           uiSampleRate;//采样级别  如:8000/16000/32000
    unsigned long           uiBitPerSample;//采样大小 如:8/16/32
    unsigned long           uiChannelNum;//通道号:0-stereo/1-joint stereo/2-dual channel/3-single channel
    int			            (*GetStreamData)(unsigned int unHWho,unsigned char** ppucDataAddr,unsigned int* puiDataLength); //流媒体播放需要进行数据获取

}MEDIAData,*PMEDIAData;


/*********************************************************************
 *  mmOpen
 *
 * 构造并初始化一个媒体设备
 *
 * 参数
 * hParent
 *   [in] 指向一个设备句柄。用于声音叠加,可以为NULL。
 * lpmmd
 *   [in] 指向一个MMData 结构。在做为参数传递之前，必须按照合适的类型属性来填充这个结构。
 * 返回值
 *  如果成功，返回值是一个32位的HMMDev。
 *  如果失败，返回值是零。

 * 说明
 *   对于tone音,目前只有媒体类型有用。
 *   目前声音叠加父必须是MP3,子必须是WAV,而且只支持两级;
*/
HMEDIAHANDLE  OpenMedia(MEDIAData *lpmmd);
HMEDIAHANDLE  _OpenMediaEx(HMEDIAHANDLE hParent,MEDIAData *lpmmd);
/*********************************************************************
 *  mmClose
 *
 * 关闭并析构一个媒体设备
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
*/
HRESULT  CloseMedia(HMEDIAHANDLE  hwho);

/*********************************************************************
 *  mmPlay
 *
 * 启动媒体播放
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。

 * 说明
 *   对于tone音,返回失败。
*/
HRESULT  PlayMedia(HMEDIAHANDLE  hwho);

/*********************************************************************
 *  mmPause
 *
 * 暂停媒体播放/录制
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。

 * 说明
 *   对于tone音,返回失败。
*/
HRESULT  PauseMedia(HMEDIAHANDLE  hwho);

/*********************************************************************
 *  mmResume
 *
 * 继续播放/录制暂停的媒体
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。

 *说明
 *   对于tone音,返回失败。
*/
HRESULT  ResumeMedia(HMEDIAHANDLE  hwho);

/*********************************************************************
 *  mmStop
 *
 * 停止播放/录制的媒体
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。

 *说明
 *   对于tone音,返回失败。
*/
HRESULT  StopMedia(HMEDIAHANDLE  hwho);


/*********************************************************************
 *  mmSeek
 *
 * 媒体播放定位
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * esType
 *   [in] 指定位搜查类型，详看2.1.2
 * lValue
 *   [in] 指定位的值，按字节还是毫秒，根据类型确定
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
 * 说明
 *   对于tone音,返回失败。
*/
HRESULT  SeekMedia(HMEDIAHANDLE  hwho,     // 设备句柄
				eMMSeekType  esType,
				long     lValue);

/*********************************************************************
 *  mmSetVolume
 *
 * 设置设备音量
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * dwValue
 *   [in] 指音量值
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
*/
HRESULT  SetMediaVolume(
				HMEDIAHANDLE  hwho,     // 设备句柄
				unsigned long dwValue);

/*********************************************************************
 *  mmGetVolume
 *
 * 获取设备音量值
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * pdwValue
 *    [out] 指音量值
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
*/
HRESULT  GetMediaVolume(
				HMEDIAHANDLE  hwho,    // 设备句柄
				unsigned long *pdwValue);

/*********************************************************************
 *  mmGetState
 *
 * 获取当前媒体控制状态
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * pdwState
 *   [out] 指状态值，对于播放来说，是指：播放、暂停、停止、未知；
 *                    对于录制来说：录制、暂停、停止、未知。 
 *
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
*/
HRESULT  GetMediaState(
				HMEDIAHANDLE  hwho,    // 设备句柄
				unsigned long *pdwState);

/*********************************************************************
 *  mmGetDuration
 *
 * 获取媒体播放总时长
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * pdwDuration
 *   [out] 指媒体播放总时长(毫秒)
 * 
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
*/
HRESULT  GetMediaDuration(
				HMEDIAHANDLE  hwho,     // 设备句柄
				unsigned long *pdwDuration);

/*********************************************************************
 *  mmGetTime
 *
 * 获取媒体播放当前时间
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * pdwTime
 *   [out] 指媒体播放当前时间(毫秒)
 * 
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
*/
HRESULT  GetMediaTime(
				HMEDIAHANDLE  hwho,     // 设备句柄
				unsigned long *pdwTime);

/*********************************************************************
 *  mmRecord
 *
 * 媒体录制
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * 
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
 * 说明
 * 对于tone音,返回失败。
 *
*/
HRESULT  RecordMedia(HMEDIAHANDLE  hwho);

/*********************************************************************
 *  mmSetMute
 *
 * 静音设置
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * isMute
 *   [in] 静音开关，TRUE 静音，FALSE 关闭静音
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
 *
*/
HRESULT  SetMediaMute(
				HMEDIAHANDLE  hwho,     // 设备句柄
				BOOLEAN    isMute);

/*********************************************************************
 *  mmPlayTone
 *
 * 播放tone音
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * dwDuration
 *  [in] 播放时长
 * dwFreq
 *  [in] 播放频率
 *
 *
 *返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
 *
*/
HRESULT  PlayTone(
				HMEDIAHANDLE  hwho,     // 设备句柄
				unsigned long      dwDuration,
				unsigned long   dwFreg);


BOOLEAN Enable_Tp_Key_Tone(int tone_type,BOOLEAN opt);
/*********************************************************************
 *  GetMediaMicDb
 *
 * 获取当前设备Mic的灵敏度
 *
 * 参数
 * hwho
 *   [in] 指向一个设备句柄。
 * pdwMicDb
 *    [out] 指Mic灵敏度
 *
 * 返回值
 *  如果成功，返回值是零。
 *  如果失败，返回值是 一个 HRESULT类型的值。
 *
 *
*/
HRESULT  _GetMediaMicDb(
				HMEDIAHANDLE  hwho,     // 设备句柄
				unsigned long    *pdwMicDb);



/*********************************************************************
 *  MMIWRE_AdjustVolumeBySideKey_UP
 *
 * the function is called when the side up_key is pressed in wre application 
 *
 * param:
 *
 * return value:
 * If the function successes ,the return value is true,
 * If the function fails ,the return value is false
 *
 *author :chi.llei 20120417
**/
PUBLIC BOOLEAN MMIWRE_AdjustVolumeBySideKey_UP(void);

/*********************************************************************
 *  MMIWRE_AdjustVolumeBySideKey_DOWN
 *
 * the function is called when the side down_key is pressed in wre application 
 * param:
 *
 * return value:
 * If the function successes ,the return value is true,
 * If the function fails ,the return value is false
 *
 *author :chi.llei 20120417
**/
PUBLIC BOOLEAN MMIWRE_AdjustVolumeBySideKey_DOWN(void);

PUBLIC void MMIWRE_InitSideKeyVolume(void);

PUBLIC int MMIWRE_RecordBufferStop(void);
PUBLIC BOOLEAN MMIWRE_StartRecordBuffer(const uint8 * buffer, uint32 buf_len);

#endif //_ADAPTOR_MEDIA_
