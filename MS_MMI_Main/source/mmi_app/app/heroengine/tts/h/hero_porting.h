#ifndef __HERO_PORTING_H_
#define __HERO_PORTING_H_
#ifdef HERO_ENGINE_TTS_SUPPORT
#if !defined(WIN32)
#include "mmisrv.h"

#ifndef PUBLIC
#define PUBLIC
#endif /* no PUBLIC */

// [inkleak.zhao] log ¿ª¹Ø
#ifndef HERO_TTS_APP_LOGOPEN
//#define HERO_TTS_APP_LOGOPEN
#endif

//[inkleak.zhao] log
#ifdef HERO_TTS_APP_LOGOPEN
	extern void hero_tts_printf(const char *format,...);
	#define MDW_LOG_LOW 	hero_tts_printf
	#define MDW_LOG_MED 	hero_tts_printf
	#define MDW_LOG_HIGH 	hero_tts_printf
	#define MDW_LOG_ERR  	hero_tts_printf
#else
	#define MDW_LOG_LOW(...)	
	#define MDW_LOG_MED(...)	
	#define MDW_LOG_HIGH(...) 	
	#define MDW_LOG_ERR(...)  		
#endif

#endif
#endif
#endif
