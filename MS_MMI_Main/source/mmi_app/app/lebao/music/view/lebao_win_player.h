#ifndef __LEBAO_WIN_PLAYER_H
#define __LEBAO_WIN_PLAYER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
*      INCLUDES
*********************/

/*********************
*      DEFINES
*********************/

/**********************
*      TYPEDEFS
**********************/

/**********************
* GLOBAL PROTOTYPES
**********************/
void lebao_player_open(void* data);
void lebao_player_close(void* data);
int	lebao_player_is_opened(void);
int	lebao_player_is_playing(void);
int	lebao_player_stop_playing(void);

// the number of songs played automatically
void lebao_player_set_next_max_count(const int count);

int lebao_player_play_uri_backend(const char* uri);

// turn ring tone settings on or off, 0 : off, 1 : on
void lebao_player_set_open_ringtone(const int opened);

void lebao_playe_init_circle_xy_cache(const int x0, const int y0, const int radius);

/**********************
*      MACROS
**********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__LEBAO_WIN_PLAYER_H*/