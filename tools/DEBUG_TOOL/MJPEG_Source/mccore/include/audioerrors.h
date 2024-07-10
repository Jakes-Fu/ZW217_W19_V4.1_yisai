/******************************************************************************
|* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
|* ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
|* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
|* PARTICULAR PURPOSE.
|* 
|* Copyright 1995-2005 Nero AG. All Rights Reserved.
|*-----------------------------------------------------------------------------
|* NeroSDK / AudioPluginManager
|*
|* FILE: audioerrors.h
|*
|* PURPOSE: Definition of the audio plugin manager error codes
******************************************************************************/

#ifndef _AUDIO_ERRORS
#	define _AUDIO_ERRORS

enum EAudioError
{
	AE_Undefined				= 0x00000000,
	AE_Failure					= 0x80000000,
	AE_InvalidParameter			= 0x80000001,
	AE_InsufficientBuffer		= 0x80000002,
	AE_BadPathName				= 0x80000003,
	AE_FileInvalid				= 0x80000004,
	AE_CallNotImplemented		= 0x80000005,
	AE_EOF						= 0x80000006,
	AE_OpenFailed				= 0x80000007,
	AE_SeekFailed				= 0x80000008,
	AE_BadFormat				= 0x80000009,
	AE_ServiceNotActive			= 0x80000010,
	AE_ReadFault				= 0x80000011,
	AE_WriteFault				= 0x80000012,
	AE_WrongCall				= 0x80000013,
	AE_FileNotFound				= 0x80000014,
	AE_Aborted					= 0x80000015,
	AE_Skip						= 0x80000016,



	AE_STUB
};

#define RETURN_ERROR(x)					\
{										\
	ASSERT(FALSE);						\
										\
	if(ppStatus)						\
		*ppStatus = new CStatus(x);			\
										\
	return false;						\
}

#define RETURN_ERROR_NOASSERT(x)		\
{										\
	if(ppStatus)						\
		*ppStatus = new CStatus(x);		\
										\
	return false;						\
}


#endif // _AUDIO_ERRORS
