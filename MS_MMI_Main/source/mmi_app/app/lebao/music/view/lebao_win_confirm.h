#ifndef __LEBAO_WIN_CONFIRM_H
#define __LEBAO_WIN_CONFIRM_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
*      INCLUDES
*********************/

/*********************
*      DEFINES
*********************/
typedef int(*lebao_confirm_proc_t)(void* data);

/**********************
*      TYPEDEFS
**********************/
typedef struct
{
	int fromPageId;
	int id;
	lebao_confirm_proc_t leftFunc;
	lebao_confirm_proc_t rightFunc;

	void* userData;
} lebao_confirm_data_t;

/**********************
* GLOBAL PROTOTYPES
**********************/
void lebao_confirm_open(void* data);
void lebao_confirm_close(void* data);
int  lebao_confirm_is_opened(void);

void lebao_login_open(void* data);
void lebao_login_close(void* data);
int  lebao_login_is_opened(void);
void lebao_login_set_captcha(const char* captcha);

/**********************
*      MACROS
**********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__LEBAO_WIN_CONFIRM_H*/