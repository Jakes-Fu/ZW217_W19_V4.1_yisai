#ifndef _AMUSE_VIDEO_DEVICE_H_
#define _AMUSE_VIDEO_DEVICE_H_

#ifdef __cplusplus
extern "C"{
#endif

int urlStreamReadForShadow(void* p, int* pos, char *dst, int size);
int urlStreamGetTotalSize(void* p);

#if 0
void* tp_video_init();
void tp_video_player_init(void* video_buffer, int type, int show);
int tp_video_player_has_inited(void* video_buffer);
int tp_video_fill(void *video_buffer, void* src);
void tp_video_fill_end(void* video_buffer);
void tp_video_setPos(void *video_buffer, int x, int y, int width, int height);
void tp_video_play(void* p);
void tp_video_pause(void* p);
void tp_video_close(void* p);
int tp_video_get_pos(void* p);
#else
#define tp_video_init() 0
#define tp_video_player_init 
#define tp_video_player_has_inited(video_buffer) 0
#define tp_video_fill(video_buffer, src) 0
#define tp_video_fill_end 
#define tp_video_setPos 
#define tp_video_play 
#define tp_video_pause 
#define tp_video_close 
#define tp_video_get_pos(p) 0
#endif
#ifdef __cplusplus
}
#endif
#endif

