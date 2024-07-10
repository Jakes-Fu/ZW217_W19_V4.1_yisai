#ifndef CONFIG_H
#define CONFIG_H

#define ENABLE_XML 1

#ifndef CONFIG_WIN9X
#define _WIN32_WINNT 0x0500
#endif

#ifdef _USRDLL
#define EXPORT extern "C" __declspec(dllexport)
#else
#ifdef __cplusplus
#define EXPORT extern "C"
#else
#define EXPORT
#endif
#endif

#endif
