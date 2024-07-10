#ifdef __cplusplus
extern "C" {
#endif
int RetrieveFileSize(TCHAR* filename);
int FileExist(TCHAR* filename);
int LoadTextFile(TCHAR* filename, TCHAR** buffer, int bufsize);
int FillWaveFormatExStructure(WAVEFORMATEX *pWaveFormatEx, int nSampleRate, int nBitsPerSample, int nChannels);
TCHAR* strredup(TCHAR* buf, TCHAR* str, int len);
int TimeStr2Int(TCHAR* timestr);
int BrowseForPath(TCHAR *buffer);
int SetHandleInheritance(LPHANDLE ph, BOOL fInherit);
#ifdef __cplusplus
}
#endif
