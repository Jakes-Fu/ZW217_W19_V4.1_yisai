#ifndef _LIVE_UPDATES_API_H_
#define _LIVE_UPDATES_API_H_

/// µ¼³öºê
#ifdef LIVEUPDATES_DLL_EXPORTS
#define LIVEUPDATES_API extern "C" __declspec(dllexport)
#else
#define LIVEUPDATES_API extern "C" __declspec(dllimport)
#endif

/** Check for version updates and automatically upgrade 
*
* @param bAuto          : Whether to automatically upgrade
* @param pszModuleName  : Absolute path of the application
*						  such as "D:\ResearchDownload_R9.19.0402\Bin\ResearchDownload.exe"
* @param pszPackageName : the Folder name of the tool package
*						  Such as "/Download/RESEARCHDOWNLOAD_R22.19.1401.rar" --> PackageName is "Download"
* @param pExtend		: Reserved
*
* explanation :If using default parameters,the application file name must be same as the package name.such as "/NVTOOL/NVTOOL_R1.19.1201.rar"
* e.g. CheckToolVerUpdate(TRUE,"D:\ResearchDownload_R9.19.0402\Bin\ResearchDownload.exe", "Download"); ///Download/RESEARCHDOWNLOAD_R22.19.1401.rar
* e.g. CheckToolVerUpdate(FALSE,"D:\NVTOOL_R1.18.5101\Bin\NVTool.exe", "NVTool");//\NVTOOL\NVTOOL_R1.19.1201.rar
*/

LIVEUPDATES_API void CheckToolVerUpdate(BOOL bAuto = TRUE,
										const char* pszModuleName = NULL,
										const char* pszPackageName = NULL,
										void* pExtend = NULL);

#endif // _LIVE_UPDATES_API_H_
