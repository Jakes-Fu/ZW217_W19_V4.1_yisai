#ifndef _BMAFEXPORT_H__
#define _BMAFEXPORT_H__

#ifdef BMAF_EXPORT_CLASS
#undef BMAF_EXPORT_CLASS
#endif //BMAF_EXPORT

#ifdef BMAF_EXPORT_API
#undef BMAF_EXPORT_API
#endif //BMAF_EXPORT

#ifdef BMAF_DLL
#define BMAF_EXPORT_CLASS __declspec(dllexport)
#define BMAF_EXPORT_API extern "C" __declspec(dllexport)
#else
#define BMAF_EXPORT_CLASS __declspec(dllimport)
#define BMAF_EXPORT_API extern "C" __declspec(dllimport)
#endif //BMAF_DLL

#endif //_BMAFEXPORT_H__
