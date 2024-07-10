//author: Justin, for watch version

#ifndef __HELPER_MP3_H
#define __HELPER_MP3_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sds.h"

typedef int(*MP3_STREAM_GET_DATA_CALLBACK_T)(unsigned int from_pos, unsigned char *data, unsigned int size);
typedef void(*MP3_STREAM_STATUS_CALLBACK_T)(const int status, void* mp3);

typedef enum
{
	// external
	STATUS_MP3_NONE = 0,
	STATUS_MP3_CONNECTING,
	STATUS_MP3_OPENED,
	STATUS_MP3_READY,
	STATUS_MP3_END,
	STATUS_MP3_FAILED,
	STATUS_MP3_STOP,
	STATUS_MP3_URI_UPDATE,
} http_mp3_status_t;

typedef struct
{
	int frameBytes;
	int frameOffset;
	int channels;
	int hz;
	int layer;
	int bitrateKbps;
	int samples;
} helper_mp3_frame_info_t;

typedef struct
{
	sds uri;				// mp3 uri
	sds id;					// mp3 id

	// not thread safe, not write, read-only
	http_mp3_status_t status;
	helper_mp3_frame_info_t info;

	int filesize;			// original mp3 file size
	int recvBytes;			// total received
	int playBytes;			// size of played or decoded frames
	int duration;			// total duration
	int id3Length;			// the length of ID3V2.3
	int hasID3Tag;			// 0 : no any tags, 1 : exists

	int maxCacheSize;
	int chunkSize;
} http_mp3_data_t;

int helper_mp3_id3_length(const char* data, const int mp3_bytes);
int helpe_mp3_decode_frame_info(const char* data, const int mp3_bytes, helper_mp3_frame_info_t* info);

http_mp3_data_t* http_mp3_open_stream_uri(const char* uri, MP3_STREAM_STATUS_CALLBACK_T cb);
http_mp3_data_t* http_mp3_open_stream_id(const char* id, sds(*get_uri_cb)(void* id), MP3_STREAM_STATUS_CALLBACK_T cb);
http_mp3_data_t* http_mp3_open_stream(const char* uri, const char* id, sds(*update_uri_cb)(void* id), MP3_STREAM_STATUS_CALLBACK_T cb);
int http_mp3_stream_get_data(http_mp3_data_t* mp3, unsigned char *data, unsigned int size);
int http_mp3_close_stream(http_mp3_data_t* mp3);
int http_mp3_stream_is_running(http_mp3_data_t* mp3);

int http_mp3_manager_init(OS_DATA_CALLBACK delayFreeCallback);
int http_mp3_manager_exit(void);
void http_mp3_manager_free(void* data);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__HELPER_MP3_H*/


