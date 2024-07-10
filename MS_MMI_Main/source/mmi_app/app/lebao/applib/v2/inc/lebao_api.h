//author: Justin, for watch version
#ifndef __LEBAO_API_H
#define __LEBAO_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sds.h"
#include "link_list.h"
#include "http_client.h"

#ifndef REAL_WATCH_RTOS
#include <sys/types.h>
#endif

typedef struct chart_info {
	sds chartId;
	sds chartName;
    sds chartPicUrl;
	sds chartInfo;
	sds chartGrade;
	sds chartType;
	int needUpdate;
	sds cachePicFileName;
	sds data;
	void* userData;
	
	void* control;
} chart_info_t;

typedef struct song_info {
	sds chartId;
	sds contentId;
	sds songName;
	sds singerName;
	sds picUrl;
	sds ringListenUrl;
	sds ringDownloadUrl;
	sds lrcDir;
	sds amrUrl;
	sds isHeart;
	int needUpdate;
	sds cachePicFileName;	
	sds data;
	void* userData;

	void* control;
	int uriCacheTime;
} song_info_t;

typedef struct product_info {
	sds channelCode;
	sds channelKey;
	sds defSeq;
	sds excode;
	sds info;
	sds orderType;
	sds orderUrl;
	sds price;
	sds secretKey;
	sds serviceId;
	sds serviceName;
	sds status;
	sds type;
	sds auth;

	sds asrType;
	int mp3Type;
} product_info_t;

typedef struct user_agreement {
	int protocolVersion;
	sds privacyProtocolUrl;
	sds privacyProtocolContent;
	sds serviceProtocolUrl;
	sds serviceProtocolContent;
} user_agreement_t;

typedef struct tip_sound {
	int index;
	sds cId;
	sds code;
	sds msg;
	sds soundUrl;
	int count;
} tip_sound_t;

typedef enum
{
	API_LEBAO_ASR_IFLYTEK,
	API_LEBAO_ASR_BAIDU,
	API_LEBAO_ASR_MIGU,
	API_LEBAO_ASR_MIGUDCT,
} lebao_api_asr_type_t;

void clear_chart(chart_info_t* chart);
void clear_song(song_info_t* song);
void clear_product(product_info_t* info);

void free_charts(link_list_t* charts);
void free_song(song_info_t* song);
void free_songs(link_list_t* songs);
void free_user_agreement(user_agreement_t * data);
void free_tip_sound(tip_sound_t * data);

int lebao_login(http_client_progress_t callback, void* caller);
//sds lebao_get_splash_pic(http_client_progress_t callback, void* caller);
int lebao_is_vip(http_client_progress_t callback, void* caller, const int postImsi);
//sds lebao_add_user(http_client_progress_t callback, void* caller);
link_list_t* lebao_query_chart_list(const int grade, const int pageNo, const int pageSize, http_client_progress_t callback, void* caller);
link_list_t* lebao_query_chart_music(const int type, const char* chartId, http_client_progress_t callback, void* caller);
//song_info_t* lebao_query_music_info(const char* contentId, http_client_progress_t callback, void* caller);

int lebao_download_to_file(const char* uri, const char* filename, const size_t rangeStart, const size_t exceedSize, const size_t maxSize, http_client_progress_t callback, void* caller, int* dwSize);
int lebao_download_to_buffer(const char* uri, sds* buffer, const size_t rangeStart, const size_t exceedSize, const size_t maxSize, http_client_progress_t callback, void* caller);
int lebao_download_stream(const char* uri, const size_t rangeStart, const size_t rangeEnd, http_client_body_chunk_t dataCallback, http_client_progress_t callback, void* caller);

product_info_t* lebao_query_product(http_client_progress_t callback, void* caller);
//sds lebao_order_product(const char* type, const product_info_t* info);
int lebao_order_by_msisdn(sds* orderId, sds* type);
sds lebao_check_order_status(const char* orderId, const char* type);
int lebao_submit_order_captcha(const char* orderId, const char* type, const char* captcha);
int lebao_query_smsinfo(http_client_progress_t callback, void* caller);

sds lebao_get_asr_token(const lebao_api_asr_type_t type, http_client_progress_t callback, void* caller);
sds lebao_post_baidu_voice(const char* uri, const char* contentType, const char* filename, const int filesize, http_client_progress_t callback, void* caller);
sds lebao_post_baidu_voice_stream(const char* uri, const char* contentType, link_list_t* stream, const int streamsize, http_client_progress_t callback, void* caller);
sds lebao_post_migu_voice(const char* contentType, const char* filename, const int filesize, http_client_progress_t callback, void* caller);
sds lebao_post_migu_voice_stream(const char* contentType, link_list_t* stream, const int streamsize, http_client_progress_t callback, void* caller);
link_list_t* lebao_search_music_by_keyword(const int type, const char* key, const char* keyType, const int page, const int count, http_client_progress_t callback, void* caller);
song_info_t* lebao_query_ring_uri(const int type, const char* contentId, http_client_progress_t callback, void* caller);

user_agreement_t* lebao_get_user_agreement(http_client_progress_t callback, void* caller);
int	lebao_query_ivr_info(http_client_progress_t callback, void* caller);
link_list_t* lebao_query_tips_sound(http_client_progress_t callback, void* caller);

sds lebao_post_migu_asr2(const char* token, const char* contentType, const char* filename, const int filesize, http_client_progress_t callback, void* caller);
sds lebao_post_migu_asr2_stream(const char* token, const char* contentType, link_list_t* stream, const int streamsize, http_client_progress_t callback, void* caller);

#ifdef __cplusplus
}
#endif

#endif
