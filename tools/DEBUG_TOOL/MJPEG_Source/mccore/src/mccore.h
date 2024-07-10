/*************************************************************************
* MediaCoder - The universal audio/video transcoder
*
* Copyright (c) 2005-06 Stanley Huang <reachme@citiz.net>
* All rights reserved.
*
* The include file for MediaCoder Core DLL
*************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
* Riff header structure
*******************************************************************************/
typedef struct {
	unsigned long  chunkID;
	long	chunkSize;
	long	format;
} CHUNK_DESC;

typedef struct {
  unsigned long  chunkID;
  long           chunkSize;
  short          wAudioFormat;
  unsigned short wChannels;
  unsigned long  dwSamplesPerSec;
  unsigned long  dwAvgBytesPerSec;
  unsigned short wBlockAlign;
  unsigned short wBitsPerSample;
/* Note: there may be additional fields here, depending upon wAudioFormat. */
} FORMAT_CHUNK;

typedef struct {
	unsigned long chunkID;
	long chunkSize;
} DATA_CHUNK;

typedef struct {
	CHUNK_DESC		desc;
	FORMAT_CHUNK	fmt;
	DATA_CHUNK		data;
} RIFF_HEADER;

/*******************************************************************************
* Audio/Video/Container identities
*******************************************************************************/
typedef enum {
	//AC_FAAC = 0,
	//AC_AMR,
	//AC_COPY,
	//AC_NONE,
	//AC_COUNT,
	//AC_PCM,

	//@richardguan
	AC_AMR = 0,
	AC_NONE,
	AC_COPY,
	AC_FAAC,
	AC_COUNT,
	AC_PCM,
} AUDIO_CODEC;

typedef enum {
	VC_NONE=0,
	VC_XVID,
	VC_MJPEG,
	VC_COUNT,
	VC_COPY,
} VIDEO_CODEC;

typedef enum {
	VE_MENCODER = 0,
	VE_COUNT,
} VIDEO_ENCODER;

typedef enum {
	CF_DEFAULT = 0,
	CF_MP4,
    CF_SPMP4,
	CF_COUNT,
} CONTAINER_FORMAT;

typedef enum {
	MT_MEDIAINFO = 0,
	MT_SHIBATCH,
	MT_COUNT,
} MEDIA_TOOLS;

/*******************************************************************************
* Transcoding information structures
*******************************************************************************/
typedef enum {
	ST_IDLE=0,					// no work is in progress
	ST_EXTRACTING_PCM,			// raw PCM is being extracted
	ST_PROCESSING_AUDIO,		// audio is being transcoded
	ST_PROCESSING_VIDEO,		// video is being transcoded
	ST_MUXING,					// audio/video streams are being muxed
} STAGES;

typedef struct {
	TCHAR* artist;
	TCHAR* title;
	TCHAR* album;
	TCHAR* genre;
	TCHAR* comment;
	short year;
	short track;
	short trackcount;
	short valid;
} TAGS;

typedef enum {
	CT_UNKNOWN=0,
	CT_AUDIO,
	CT_VIDEO,
	CT_SUBTITLE,
	CT_CUE,
	CT_QUEUE,
	CT_SPECIAL,
} TRACK_TYPE;

typedef enum {
	FT_UNKNOWN = 0, FT_MPEG_PS, FT_MPEG_TS, FT_MP4, FT_MPA, FT_3GP, FT_MP3, FT_M4A, FT_OGG, FT_AAC, FT_RA, FT_RM, FT_AMR,
	FT_WAV, FT_MPC, FT_WMA, FT_APE, FT_FLAC, FT_WV, FT_CDA, FT_MKA, FT_MKV, FT_AVI, FT_MOV, FT_WMV, FT_263, FT_264,
	FT_CUE, FT_M3U, FT_IDX, FT_SUB,
} EXT_TYPE;

typedef struct {
	TCHAR* fileext;					// file extension name
	EXT_TYPE exttype;				// extension type
	TCHAR* desc;					// description text of file type
	TRACK_TYPE type;				// track type
} FILE_TYPE;

typedef struct {
	int bitrate;					// audio bit rate
	int bitratemode;				// bit rate mode (1 for CBR, 2 for VBR, 0 for unknown)
	int samplerate;					// sampling rate
	int channels;					// number of channels
	char* codec;					// audio codec name string
} ATTR_AUDIO;

typedef struct {
	int bitrate;					// video bit rate
	int width;						// video width in pixels
	int height;						// video height in pixels
	float fps;						// video frame rate
	char* codec;					// video codec name string
} ATTR_VIDEO;

typedef struct {
	FILE_TYPE* filetype;			// file type information
	int duration;					// duration in millisecond
	long long filesize;				// file size in bytes
	char* container;				// container name string
	ATTR_VIDEO video;				// video attributes
	ATTR_AUDIO audio;				// audio attributes
	TAGS tags;						// tags
	TCHAR* discroot;				// root path of the disc
	int track;						// track number of the disc
	int chapter;					// chapter number of the track
	int begin;						// begin position
	int end;						// end position
} MEDIA_INFO;

typedef struct {
	CONTAINER_FORMAT container;		// target container
	VIDEO_ENCODER videoenc;			// target video encoder
	AUDIO_CODEC audioformat;		// target audio format
	VIDEO_CODEC videoformat;		// target video format
	float fps;						// target video frame rate
} TARGET_SETTING;

#define IF_PRIVATE_FORMAT 0x1		// use private target format setting
#define IF_INFO_RETRIEVED 0x2		// information of the media file is retrieved
#define IF_DONE 0x4					// the item is done with processing

typedef struct _T_ITEM {
	struct _T_ITEM *next;			// point to the next T_ITEM entry
	TCHAR* filename;				// source file name
	TCHAR* destfile;				// destination file name
	int flags;						// flags of the item
	MEDIA_INFO info;				// information of the source file
	TARGET_SETTING target;			// target settings
} T_ITEM;

typedef enum {
	MC_MSG_INFO = 1,				// display information message
	MC_MSG_ALERT,					// display alert message
	MC_MSG_CMD,						// display command line
	MC_MSG_POPUP,					// pop up a message
	MC_MSG_YESNOCANCEL,				// display a YES/NO/CANCEL confirmation message
	MC_MSG_YESNO,					// display a YES/NO confirmation message
	MC_UPDATE_LOG,					// update log text display
	MC_UPDATE_PROGRESS,				// update progress display
	MC_UPDATE_STATS,				// update statistics display
	MC_UPDATE_TIME,					// update time display
	MC_UPDATE_TITLE,				// update window title
	MC_UPDATE_STATE,				// update state display
	MC_WRITE_CONSOLE,				// update console display
	MC_JOB_START,					// indicate a job is to be started
	MC_JOB_FINISH,					// indicate a job is to be finished
} MC_EVENT;

// event results
#define ER_OK 0
#define ER_BYPASSED -1

typedef enum {
	EA_HALT = 0,
	EA_CONTINUE,
} EVENT_ACTIONS;

typedef struct {
	unsigned int pos;
	unsigned int frame;
	float percent;
	float fps;
	unsigned int remaintime;
	unsigned int cursize;
	unsigned int estsize;
	unsigned int vbitrate;
	unsigned int abitrate;
	unsigned int skippedframes;
	unsigned int badframes;
	unsigned int duplicatedframes;
	unsigned int audiosamples;
	unsigned int videosamples;
	unsigned int length;
	int width;
	int height;
} MP_STATS;

typedef struct {
	float percent;
	int pos;
	int speed;
	int size;
	int length;
	long long bytes;
} AUDIO_STATS;

typedef struct {
	DWORD dwEncStartTime;
	DWORD dwTaskStartTime;
	DWORD dwClipStartTime;
	DWORD dwElapsed;
	unsigned int originalKB;
	unsigned int resultKB;
	int fileprocessed;
	int encodedFiles;
	int skippedFiles;
	int errorFiles;
	float percent;
} TASK_STATS;

typedef struct _MC_AUDIO_FILTER {
	int (*pfnAudioDsp)(short* pwSamples, int iNumOfSamples);
	int samplerate;
	int channels;
	int bps;
	struct _MC_AUDIO_FILTER* next;
} MC_AUDIO_FILTER;

typedef enum {
	STATE_IDLE=0,		// no work in progress
	STATE_ENCODING,		// task is in progress
	STATE_STOPPING,		// task is going to be stopped
	STATE_PLAYING,		// a file is playing (when not in transcoding progress)
	STATE_SKIPPING,		// a file is going to be skipped
	STATE_ERROR,		// an has been encountered
	STATE_SHUTDOWN,		// the program is going to shut down
} STATES;

/*******************************************************************************
* CPU information
*******************************************************************************/
typedef struct cpucaps_s {
	int cpuType;
	int cpuStepping;
	int hasMMX;
	int hasMMX2;
	int has3DNow;
	int has3DNowExt;
	int hasSSE;
	int hasSSE2;
	int isX86;
	unsigned cl_size; /* size of cache line */
	int hasAltiVec;
	int hasTSC;
	int freq;
	char vendor[20];
	char* cpuFullName;
} CpuCaps;

/*******************************************************************************
* transcoding state
*******************************************************************************/
typedef struct {
	STATES state;					// current state
	STAGES stage;					// current stage of the job
	int currentpass;				// current pass of the job
	int totalpass;					// number of passes of the job
	int totalfiles;					// number of files in the queue
	TASK_STATS stats;				// task statistics
	AUDIO_STATS astats;				// audio statistics
	MP_STATS* vstats;				// video statistics
	CpuCaps cpucaps;				// CPU capability information
} MC_STATE;

typedef int (*PFN_EVENT_HANDLER)(MC_EVENT event, void* arg);
typedef int (*PARAM_NOTIFY)(int, void*);

/*******************************************************************************
* API functions to start and control transcoding task
*******************************************************************************/
int mcStartTranscode();
int mcStopTranscode();
STATES mcGetState();
void mcSetState(STATES state);
void mcInvalidiateStatsData();

/*******************************************************************************
* API functions to perform initialization and clean-ups
*******************************************************************************/
MC_STATE* mcInit(
	TCHAR* pchLangFile,						// language file
	PFN_EVENT_HANDLER pfnEventCallback	// pointer to event handler function
);

int mcPostInit(HWND hwndMain);
void mcUninit();

/*******************************************************************************
* API functions and data types related with file type identification
*******************************************************************************/
typedef enum {
	DUMMY_FLAG = 0,
	FLAG_SBR_AAC,
} CF_FLAGS;

FILE_TYPE* mcIdentifyFileType(TCHAR *filename);
FILE_TYPE* mcGetFileTypeByIndex(int index);

/*******************************************************************************
* API functions and data types related with container
*******************************************************************************/
typedef struct {
	TCHAR* filename;
	TRACK_TYPE type;
	int flags;
	int delay;
	int trackid;
} CONTAINER_TRACKS;

int mcContainerAddFile(TRACK_TYPE type, TCHAR* filename, CF_FLAGS flags);
int mcContainerMuxFiles(TCHAR* destfile);
int mcContainerPurge(int fDelete);

/*******************************************************************************
* API functions to create/add/remove transcoding items
*******************************************************************************/
int mcAddItem(T_ITEM* itemtoadd);
void mcRemoveAllItems();
int mcRemoveItem(T_ITEM* item);
T_ITEM* mcMakeItem(TCHAR* filename);
void mcFreeItem(T_ITEM* item);
T_ITEM* mcGetItemByName(TCHAR* filename);
T_ITEM* mcGetItemByIndex(int index);
T_ITEM* mcGetNextItem(T_ITEM* prev);

/*******************************************************************************
* API functions to load/save/get/set preference values
*******************************************************************************/
int mcLoadParam(TCHAR* cfgfile, int fInit);
int mcSaveParam(TCHAR* cfgfile);
void mcApplyParam();
int mcGetPrefValueInt(int id);
TCHAR* mcGetPrefValueStr(int id);
TCHAR* mcGetPrefValueEnum(int id);
TCHAR* mcGetPrefValueEnumDefs(int id, int index);
float mcGetPrefValueFloat(int id);
void* mcSetPrefValue(int id, void* value);
void* mcGetPrefValue(int id);
int mcSetPrefValueInt(int id, int value);
float mcSetPrefValueFloat(int id, float value);
int mcGetPrefIdByKey(char* key);
char* mcGetPrefKeyById(int id);

typedef enum { 
	PP_KEY = 0,
	PP_TYPE,
	PP_FLAG,
	PP_VALMIN,
	PP_VALMAX,
	PP_CAPTION,
	PP_DESC,
} PARAM_PROPERTY;

int mcGetPrefProperty(int id, PARAM_PROPERTY property, void* pval);

#define SetPrefValue(key, val) mcSetPrefValue(mcGetPrefIdByKey(key), (void*)val)
#define GetPrefValue(key) mcGetPrefValue(mcGetPrefIdByKey(key))
#define GetPrefValueFloat(key) mcGetPrefValueFloat(mcGetPrefIdByKey(key));

/*******************************************************************************
* mcShowConfigDialog
* Show configuration window
*******************************************************************************/
typedef enum {
	CD_PREF = 0,
	CD_AUDIO_SRC,
	CD_DSP_PLUGIN,
	CD_AUDIO_ENC,
	CD_CONTAINER,
} CONFIG_DIALOG;

int mcShowConfigDialog(CONFIG_DIALOG type, PARAM_NOTIFY notify);

/*******************************************************************************
* mcGetToolName / mcGetToolVersion
* Get the name / version of a tool (including decoders, encoders, container wrappers
* and taggers
*******************************************************************************/
typedef enum {
	TL_AUDIO_ENCODER = 0,
	TL_AUDIO_DECODER,
	TL_VIDEO_CODEC,
	TL_VIDEO_ENCODER,
	TL_VIDEO_DECODER,
	TL_CONTAINER,
	TL_TAGGER,
	TL_TOOL,
} MC_TOOLS;

TCHAR* mcGetToolName(MC_TOOLS type, int nametype, int id);
TCHAR* mcGetToolVersion(MC_TOOLS type, int id);

/*******************************************************************************
* mcGetToolCommand
* Get the command line of a tool (including decoders, encoders, container
* wrappers and taggers
*******************************************************************************/
TCHAR* mcGetToolCommand(MC_TOOLS type, int pass);

/*******************************************************************************
* mcRunMediaTool
* Call and run media tools (tagger, file info reader etc.)
*******************************************************************************/
TCHAR* mcRunMediaTool(MEDIA_TOOLS type, int action, T_ITEM* item);

/*******************************************************************************
* mcPlayerStart
* Start the audio/video player
*******************************************************************************/
int mcPlayerStart(T_ITEM* item, HWND hWndParent, HWND* phVideoWnd, BOOL fNoFilter, char* extraopts);

/*******************************************************************************
* mcPlayerStart
* Stop the audio/video player if it is playing
*******************************************************************************/
int mcPlayerStop();

/*******************************************************************************
* mcPlayerStart
* Send command to audio/video player
*******************************************************************************/
int mcPlayerCommand(char* cmd);

/*******************************************************************************
* mcPreviewStart
* Start the audio/video player to preview encoded file
*******************************************************************************/
int mcPreviewStart(T_ITEM* item, HWND hWndParent, HWND* phVideoWnd, char* extraopts);

/*******************************************************************************
* mcGetMediaInfo
* Retrieve the information of a media file
*******************************************************************************/
int mcGetMediaInfo(T_ITEM* item);

/*******************************************************************************
* mcGetMediaInfo
* Retrieve the information of a media file
*******************************************************************************/
typedef enum {
	TP_AUDIO_BITRATE,
	TP_AUDIO_BITRATE_MODE,
	TP_VIDEO_BITRATE,
	TP_VIDEO_BITRATE_MODE,
} TARGET_PARAM;

int mcGetTargetParam(TARGET_PARAM type);
int mcSetTargetParam(TARGET_PARAM type, int val);

/*******************************************************************************
* CD/VCD/DVD/CueSheet handling API functions
*******************************************************************************/
TCHAR* mcGetDiscInfo(int disctype, int track, TCHAR* discroot);
int mcGetDriveTrackCount(int drvno);
int mcGetDriveCount();

/*******************************************************************************
* Misc. API functions
*******************************************************************************/
TCHAR* mcGetLogText(int maxsize);
void mcSetLogText(TCHAR* logtext);
void mcLogInfo(int id, TCHAR* arg);
int mcGetBuildInfo(TCHAR** ptrdate);

#ifdef __cplusplus
}
#endif
