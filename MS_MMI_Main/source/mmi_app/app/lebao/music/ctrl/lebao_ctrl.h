#ifndef MMI_LEBAO_CTRL_H
#define MMI_LEBAO_CTRL_H

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
void	lebao_exit(void);
int		lebao_is_running(void);
int		lebao_is_playing(void);
void	lebao_stop_playing(void);
void	lebao_reset_data(void);
void    lebao_set_https_mode(const int mode);

void	lebao_set_app_key(const char * appId, const char * key);
void	lebao_set_account_type(const int type);

void 	lebao_ctrl_init(void);
void	lebao_ctrl_destroy(void);
int		lebao_ctrl_can_write_file(const int needSize);
void	lebao_ctrl_clear_mp3(void);

// playing control
void	lebao_ctrl_pause_music(const int eventId, const int pageId, void* data);
void	lebao_ctrl_resume_music(const int eventId, const int pageId, void* data);
void	lebao_ctrl_stop_music(const int eventId, const int pageId, void* data);

// file control
void	lebao_ctrl_mp3_deleted_callback(int(*callback)(const int cmd, const int status, void *data));

// 0 : file, 1 : buffer
void	lebao_ctrl_set_record_mode(const int mode);

void	lebao_ctrl_play_sound(const int index);
void	lebao_ctrl_set_can_play_tip_sound(const int canPlay);
void	lebao_ctrl_quit_streaming(const int tryWaiting);

void    lebao_ctrl_post_ui_event(void* data);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*MMI_LEBAO_CTRL_H*/
