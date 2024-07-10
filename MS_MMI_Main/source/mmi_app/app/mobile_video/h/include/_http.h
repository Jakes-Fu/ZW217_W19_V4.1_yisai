//
//	_Http.h
//
#ifndef	__HTTP_API_H
#define	__HTTP_API_H

#ifdef	__cplusplus
extern "C"
{
#endif

typedef enum
{
	EHttpStatus_None,
	EHttpStatus_Process,
	EHttpStatus_Done,
	EHttpStatus_Error
} THttpStatusEnum;

typedef enum
{
	EHttpMethod_Get,
	EHttpMethod_Post,
} THttpMethodEnum;

extern _HANDLE Http_Create(_VOID);
extern _VOID Http_Release(_HANDLE hHttp);
extern _INT Http_OpenRequest(_HANDLE hHttp, THttpMethodEnum nMethod, _CONST _CHAR* url, _CONST _CHAR* proxy);
extern _VOID Http_Reset(_HANDLE hHttp);

extern _INT Http_SetHeader(_HANDLE hHttp, _CONST _CHAR* strName, _CONST _CHAR* strValue);
extern _INT Http_ClearHeader(_HANDLE hHttp, _CONST _CHAR* strName);
extern _INT Http_SetFullHeader(_HANDLE hHttp, _CONST _CHAR* strHeaderString);
extern _INT Http_SetProxy(_HANDLE hHttp, _CONST _CHAR* proxy);

extern _INT Http_IssualGet(_HANDLE hHttp);
extern _INT Http_IssualPost(_HANDLE hHttp, _CONST _CHAR* pData, _INT nDataSize);

extern _CONST _CHAR* Http_GetHeaderValue(_HANDLE hHttp, _CONST _CHAR* header);
extern _CHAR* Http_GetContext(_HANDLE hHttp, _INT* pSize);

extern _INT Http_GetContextSize(_HANDLE hHttp);
extern THttpStatusEnum Http_GetStatus(_HANDLE hHttp);
extern _BOOLEAN Http_IsReady(_HANDLE hHttp);
extern _BOOLEAN Http_HeadParseFinished(_HANDLE hHttp);

extern _INT Http_ResponseCode(_HANDLE hHttp);
//===>> Modify by czh 2010-03-29
extern _VOID Http_Disconnect(_HANDLE hHttp);
extern _VOID  Http_Process(_HANDLE hHttp);
extern _VOID Http_SetStatus(_HANDLE hHttp,THttpStatusEnum status);
//<<=== Modify by czh 2010-03-29

#ifdef	__cplusplus
}
#endif

#endif	//__HTTP_API_H
