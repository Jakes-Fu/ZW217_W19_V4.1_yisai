#ifndef __RTMP_MP3_INFO_H
#define __RTMP_MP3_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	int frameBytes;
	int frameOffset;
	int channels;
	int hz;
	int layer;
	int bitrateKbps;
} rtmp_mp3_frame_info_t;


int rtmp_mp3_id3_length(const char *mp3, const int mp3_bytes);

int rtmp_mp3_decode_frame_info(const char *mp3, const int mp3_bytes, rtmp_mp3_frame_info_t *info);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__RTMP_MP3_INFO_H*/