#ifndef __LEBAO_CONFIG_H
#define __LEBAO_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sds.h"

typedef struct lebao_config {
	sds channel;
	sds key;
	sds accessToken;

	sds lebaoId;
	sds imei;
	sds msisdn;			// local sim
	sds imsi;			// local sim
	sds iccid;
	sds pimsi;			// userInfo.pimsi
	sds pmsisdn;		// userInfo.pmsisdn
	sds virtualMsisdn;	// userInfo.msisdn
	sds lastImsi;		// userInfo.imsi
	sds payType;		// userInfo.payment
	sds addTime;		// userInfo.addTime
	sds validTime;		// userInfo.validTime
	sds openId;			// userInfo.openId

	sds baiduToken;

	sds rootPath;
	sds cachePath;
	sds qrFilename;
	sds playlistFilename;
	sds savePath;
	sds recordFile;

	int maxCycle;
	int showImage;
	int maxFiles;
	int orderType;

	int maxCount;
	int maxDayCount;
	int maxSearch;
	int maxDaySearch;

	sds timestamp;
	sds nowDay;
	int myCount;
	int myDayCount;
	int mySearch;
	int myDaySearch;

	int spaceMinSize;
	int cacheImageFile;
	int imageMaxSize;

	int mp3MaxBufferSize;
	int mp3MaxFileSize;
	int mp3MinFileSize;
	int mp3MaxCacheSize;
	int mp3ChunkSize;
	int mp3MaxSaveCount;
	int mp3MaxFavorites;
	int listMaxCount;

	sds supportOrderType;

	sds smsCalled;
	sds smsOrder;
	sds smsCode;
	sds miguSmsCalled;
	sds smspre;
	sds captcha;

	sds ivrCalled;
	sds ivrValidTime;

	int agreement;
	int imsiChanged;
	int price;

    int volume;
} lebao_config_t;

void lebao_set_key_config(const char * id, const char * key);

void lebao_set_account_type_config(int type);
char* lebao_get_account(void); // const

lebao_config_t* lebao_get_config(void);
int 	lebao_load_config(void);
int 	lebao_save_config(const char* filename);
int 	lebao_free_config(void);
void	lebao_delete_config_files(void);

sds lebao_get_ring_mp3_path(const char * id);
sds lebao_get_img_cache_path(const char * filename);

#ifdef __cplusplus
}
#endif

#endif // __LEBAO_CONFIG_H
