#ifndef __LEBAO_PLAYLIST_H
#define __LEBAO_PLAYLIST_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lebao_api.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

link_list_t*	lebao_get_playlist(void);
void	lebao_free_playlist(void);
int		lebao_save_playlist(const int noHttp);
void	lebao_del_playlist(void);
void	lebao_check_playlist(void);
void 	lebao_limit_playlist(void);
int		lebao_reduce_playlist(void);
int		lebao_add_playlist_song(song_info_t* song);
int		lebao_del_playlist_song(const char* songId);
int		lebao_change_playlist_uri(const char* songId, const char* uri);
sds		lebao_check_playlist_ring_exist(const char* songId);
void	lebao_set_delete_callback_playlist(int(*callback)(const int cmd, const int status, void *data));

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__LEBAO_PLAYLIST_H*/
