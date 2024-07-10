
#ifndef __XINCL_XNES_API_H_INCLUDE__
#define __XINCL_XNES_API_H_INCLUDE__

//#include <xnes_peer.h>

#ifndef XNES_EXPORT
	#ifdef  X4C_EXPORT_infoNES
	#define XNES_EXPORT			//_XLIB_EXPORT_
	#else
	#define XNES_EXPORT			//_XLIB_IMPORT_
	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
#define XNESCALL

XNES_EXPORT	int  XNESCALL
xnes_initialize( const char* rom );

XNES_EXPORT void XNESCALL
xnes_finalize  ( void );

/*
 *	return -1 for show top menu.
 */
XNES_EXPORT int XNESCALL
xnes_next_frame( void );

XNES_EXPORT void XNESCALL
xnes_set_frame_skip( int skip );

XNES_EXPORT int XNESCALL
xnes_get_frame_flush_cnt(void);

XNES_EXPORT void XNESCALL
xnes_set_sound( int on );

void app_snd_pause(void);
void app_snd_resume(void);

XNES_EXPORT void XNESCALL
xnes_reset( void );


#ifdef __cplusplus
};
#endif

#endif /* __XINCL_XNES_API_H_INCLUDE__ */


