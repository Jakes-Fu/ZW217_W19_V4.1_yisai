#ifndef __HELPER_MEM_H
#define __HELPER_MEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "port_cfg.h"
//#include "lv_conf.h"

#ifdef USE_UI_MEM_ALLOC
#ifndef LV_MEM_USE_MALLOC_DEFINE
	extern void *	lv_mem_alloc(uint32_t);
	extern void		lv_mem_free(const void *);
#endif
	extern void *	lv_mem_realloc(void *, uint32_t);
	extern void *	os_calloc_impl(size_t, size_t);
#endif

#ifdef __cplusplus
}
#endif

#endif // __HELPER_MEM_H