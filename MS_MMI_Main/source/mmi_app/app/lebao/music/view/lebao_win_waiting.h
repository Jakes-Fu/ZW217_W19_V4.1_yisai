#ifndef __LEBAO_WIN_WAITING_H
#define __LEBAO_WIN_WAITING_H

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
typedef void(*lebao_timeout_proc_t)(void* data);

typedef struct
{
	int fromPageId;
	int id;
	int txtId;
	char* title;
	int second;
	int checkPeriod;
	int elapsed;
	lebao_timeout_proc_t callback;
	lebao_timeout_proc_t check;
} lebao_waiting_data_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void lebao_waiting_open(void* data);
void lebao_waiting_open2(const int fromPageId, const int id, const int txtId, const char* title, const int second, lebao_timeout_proc_t callback, const int checkPeriod, lebao_timeout_proc_t check);
void lebao_waiting_close(void* data);
int  lebao_waiting_is_opened(void);
void lebao_waiting_update_title(const int txtId, const char* title);
void lebao_waiting_update_tips(const int txtId, const char* tips);
void lebao_waiting_change_timer(const int second, lebao_timeout_proc_t callback, const int checkPeriod, lebao_timeout_proc_t check);
void lebao_waiting_allow_return(const int canReturn);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__LEBAO_WIN_WAITING_H*/
