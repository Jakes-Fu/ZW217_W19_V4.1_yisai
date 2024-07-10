#ifndef _SCIDLLEXPORT_H__
#define _SCIDLLEXPORT_H__

#ifdef SCIDLL_EXPORT_CLASS
#undef SCIDLL_EXPORT_CLASS
#endif //SCIDLL_EXPORT

#ifdef SCIDLL_EXPORT_API
#undef SCIDLL_EXPORT_API
#endif //SCIDLL_EXPORT

#ifdef SCIRES_DLL
#define SCIDLL_EXPORT_CLASS __declspec(dllexport)
#define SCIDLL_EXPORT_API extern "C" __declspec(dllexport)
#else
#define SCIDLL_EXPORT_CLASS __declspec(dllimport)
#define SCIDLL_EXPORT_API extern "C" __declspec(dllimport)
#endif //SCIDLL_DLL

#endif //_SCIDLLEXPORT_H__