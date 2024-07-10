
#ifndef __XNES_INIT__
#define __XNES_INIT__

#include "InfoNES_System.h"
#define DEFINE_NES_INTERFACE(func) func##_fun func


#if 0
xnes_peer_file* xnes_peer_fopen_4_read ( const char* path );

 xnes_peer_file* 
xnes_peer_fopen_4_write( const char* path );

 void 
xnes_peer_file_close( xnes_peer_file* );

 int  
xnes_peer_file_read ( void* , int , xnes_peer_file* );

	int  
xnes_peer_file_write( const void* , int , xnes_peer_file* );
XNESPEER_EXPORT void XNESCALL
xnes_peer_read_joypad( unsigned long *pad1 , unsigned long *pad2 , unsigned long *sys );
XNESPEER_EXPORT int  XNESCALL
xnes_peer_video_initialize      ( void );

XNESPEER_EXPORT void XNESCALL
xnes_peer_video_finalize		( void );
XNESPEER_EXPORT void XNESCALL
xnes_peer_video_update_screen	( void* vram );
XNESPEER_EXPORT int  XNESCALL
xnes_peer_sound_initialize		( void );

XNESPEER_EXPORT	int	 XNESCALL	/* MONO AFMT_U8 */
xnes_peer_sound_set_param		( int sample_rate );

XNESPEER_EXPORT void XNESCALL
xnes_peer_sound_finalize		( void );

XNESPEER_EXPORT void XNESCALL
xnes_peer_set_sound				( int on );

XNESPEER_EXPORT int  XNESCALL
xnes_peer_sound_write			( int samples , unsigned char* wave );

#endif

typedef xnes_peer_file *(*xnes_peer_fopen_4_read_fun)( const char* );
typedef xnes_peer_file *(*xnes_peer_fopen_4_write_fun)( const char* );
typedef void (*xnes_peer_file_close_fun)( xnes_peer_file* );
typedef int (*xnes_peer_file_read_fun)( void* , int , xnes_peer_file* );
typedef int (*xnes_peer_file_write_fun)( const void* , int , xnes_peer_file* );
typedef void (*xnes_peer_read_joypad_fun)( unsigned long *, unsigned long *, unsigned long *);

typedef int (*xnes_peer_video_initialize_fun)(void);
typedef void (*xnes_peer_video_finalize_fun)( void );
typedef void (*xnes_peer_video_update_screen_fun)( void*);
typedef int  (*xnes_peer_sound_initialize_fun)( void );
typedef int (*xnes_peer_sound_set_param_fun)( int );
typedef void (*xnes_peer_sound_finalize_fun)( void );
typedef void (*xnes_peer_set_sound_fun)( int );
typedef int (*xnes_peer_sound_write_fun)( int , unsigned char*  );
typedef void * (*NesMalloc_fun)( int );
typedef void (*NesFree_fun)( void * );

typedef struct __G_NES_INTERFACE_CTX
{
	DEFINE_NES_INTERFACE(xnes_peer_fopen_4_read);
	DEFINE_NES_INTERFACE(xnes_peer_fopen_4_write);
	DEFINE_NES_INTERFACE(xnes_peer_file_close);
	DEFINE_NES_INTERFACE(xnes_peer_file_read);
	DEFINE_NES_INTERFACE(xnes_peer_file_write);
	DEFINE_NES_INTERFACE(xnes_peer_read_joypad);

	DEFINE_NES_INTERFACE(xnes_peer_video_initialize);
	DEFINE_NES_INTERFACE(xnes_peer_video_finalize);
	DEFINE_NES_INTERFACE(xnes_peer_video_update_screen);
	
	DEFINE_NES_INTERFACE(xnes_peer_sound_initialize);
	DEFINE_NES_INTERFACE(xnes_peer_sound_set_param);
	DEFINE_NES_INTERFACE(xnes_peer_sound_finalize);
	DEFINE_NES_INTERFACE(xnes_peer_set_sound);
	DEFINE_NES_INTERFACE(xnes_peer_sound_write);

	DEFINE_NES_INTERFACE(NesMalloc);
	DEFINE_NES_INTERFACE(NesFree);

}G_NES_INTERFACE_CTX;

extern G_NES_INTERFACE_CTX g_nes_ctx;

extern int xnes_get_frame_flush_cnt( void );
extern int xnes_next_frame( void );
extern int  xnes_initialize( const char* rom );

extern void xnes_set_frame_skip( int skip );
extern void xnes_finalize  ( void );
extern int Xnes_All_Interface_init(G_NES_INTERFACE_CTX *g_nes, int size);


#endif /* __XINCL_XNES_API_H_INCLUDE__ */


