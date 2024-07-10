#pragma once
#ifndef __RTMP_PLAYER_H__
#define __RTMP_PLAYER_H__

typedef enum
{
	RTMP_STATUS_STOP = 0,
	RTMP_STATUS_CONNECTING,
	RTMP_STATUS_CACHED,
	RTMP_STATUS_PLAYING,
	RTMP_STATUS_ERROR,
} rtmp_player_status_t;
	
typedef struct mrtmp_packet
{
    void * data;
    struct mrtmp_packet * next;
    unsigned int pts;
    int size, type;
} mrtmp_packet_t;

typedef void (*MRTMP_PACKET_CALLBACK_T)(void * user, mrtmp_packet_t * pkt);
typedef void (*MRTMP_EVENT_CALLBACK_T)(void * user, int event, int code);

#ifdef __cplusplus
extern "C" {
#endif

int  rtmp_player_play(const char *url);
int  rtmp_player_stop();
void rtmp_player_set_cache_packet_count(const int count);
int  rtmp_player_get_status();

#ifdef __cplusplus
}
#endif

#endif