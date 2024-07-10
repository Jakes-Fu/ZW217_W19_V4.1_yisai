//author: Justin, for watch version

#ifndef __HTTP_CLIENT_H
#define __HTTP_CLIENT_H
#pragma  once

//#define _USER_CURL_WATCH_VERSION

#ifdef __cplusplus
extern "C" {
#endif

#include "sds.h"

typedef enum http_post_type_t {
	HTTP_POST_BUFFER = 0,
	HTTP_POST_RAW_FILE,
	HTTP_POST_FORM_DATA,
	HTTP_POST_X_WWW_FORM,

	HTTP_POST_FORM_DATA_STREAM,
	HTTP_POST_RAW_FILE_STREAM,
} http_post_type_t;

typedef int(*http_client_progress_t) (void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
typedef int(*http_client_body_chunk_t) (void *clientp, const char* data, const int length, const int position, const int contentLength, const int totalLength);

typedef struct http_t {
	// Request
	sds reqContentType;
	sds	requestHeader;
	sds	requestBody;	// HTTP_POST_BUFFER valid
	size_t exceedSize;	// the request would be discard, if the content size is exceed this value
	int rangeStart;
	int rangeEnd;

	http_post_type_t postType;		// 0: requestBody, 1: raw, 2: multipart/form-data, 3: application/x-www-form-urlencoded, 4: 2-stream, 5: 1-stream
	sds postFilename;				// full path name, when postType = 1
	int postFilesize;
	void* postStream;				// stream, when postType = 5

	// Response
	int statusCode;
	sds reasonPhrase;
	sds contentType;
	size_t contentLength;
	sds	responseHeader;
	sds	responseData;

	int ranged;			// 0: not supported 1: supported
	int totalLength;	// the current response size, not the content-length, when set the Range bytes
	int keepAlive;		// server support, connection, 0: closed 1: keep-alive
	int recvTotal;		// all bytes from network, include header, tags, body...
	int bodySize;		// body size

	int maxFileSize;
	int maxBufferSize;
	int chunkSize;
} http_t;

http_t* http_client_get(const char* uri);
http_t* http_client_post(const char* uri);
http_t* http_client_delete(const char* uri);

int http_client_process(http_t* http, http_client_progress_t callback, void* caller);
int http_client_download(http_t* http, const char* filename, http_client_progress_t callback, void* caller);
int http_client_stream(http_t* http, http_client_body_chunk_t chunk, http_client_progress_t callback, void* caller);

void http_client_release(http_t* http);

// multipart/form-data, name-value or upload file
// 1. name-value: 
//		http_client_add_form_value(http, "username", "star");
// 2. upload file: 
//		http_client_add_form_file(http, "voice", "record.wav", "audio/pcm", "D:\\record.wav", 100000);
//		or
//		http_client_add_form_stream(http, "voice", "record.wav", "audio/pcm", data_linklist, 100000);
void http_client_add_form_value(http_t* http, const char* name, const char* value);

void http_client_add_form_file(http_t* http, const char* name, const char* value,
	const char* contentType, const char* filepath, const int filesize);

void http_client_add_form_stream(http_t* http, const char* name, const char* value,
	const char* contentType, void* stream, const int streamSize);

// exclude: Host, Accept-Encoding, Content-Type, Content-Length, Range
void http_client_add_header_field(http_t* http, const char* name, const char* value);

// application/x-www-form-urlencoded, name-value
void http_client_add_x_form_field(http_t* http, const char* name, const char* value);

void http_client_set_https_mode(const int mode);

void http_client_dns_cache_init(void);
void http_client_dns_cache_free(void);
int  http_get_dns_cache(const char* host, unsigned int* ip);
void http_add_dns_cache(const char* host, const unsigned int ip);

#ifdef __cplusplus
}
#endif
#endif
