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
|* FILE: audiotypes.h
|*
|* PURPOSE: Definitions of Audio types
******************************************************************************/

#ifndef _AUDIO_TYPES_
#	define _AUDIO_TYPES_

///////////////////////////////////////////////////////// EAudioComponentType //

enum EAudioComponentType
{
	ACT_None						= 0,
	ACT_PluginMgr					= 1,
	ACT_ComponentEnumerator			= 2,
	ACT_AudioURLSourceFactory		= 3,
	ACT_AudioURLTargetFactory		= 4,
	ACT_ConvertorFactory			= 5,
	ACT_Downloader					= 6
};

//////////////////////////////////////////////////////////////////// EURLType //

enum EURLType
{
	URL_None						= 0x00000000,
	URL_LocalFile					= 0x00000001,
	URL_Http						= 0x00000002,
	URL_Ftp							= 0x00000004
};

/////////////////////////////////////////////////////////////////// EAuxFlags //

enum EAuxFlags
{
	EAF_None						= 0x00000000,

																	// Can...
	// Can record on CD
	EAF_CanRecOnAudioCD				= 0x00000010,
	EAF_CanRecOnDataCD				= 0x00000020,

	// Copy protection
	EAF_CanCopy						= 0x00000040,

	// Can be downloaded to an mp3 player or a portable device
	EAF_CanDLToMp3PlayerOrPortabDvc	= 0x00000080,

	// Can be converted to other format
	EAF_CanConvert					= 0x00000100,
	
																// Source type
	// Indicates that the RAW data can be read from the item.
	EAF_Readable					= 0x00001000,

	// Indicates that the source can play itself (also pause, resume, etc.).
	EAF_Playable					= 0x00002000,

															// Additional flags


	// Indicates that the source can edit the additional information such as
	// the artist and title name and store it back in the source file.
	EAF_SrcCanSaveAddInfo			= 0x00010000,

	// Some of sources can read/write data at 1.0 x only (internet streams,
	// CD players and others). Must be specified with EAF_BadReadable or
	// EAF_Readable.
	EAF_RealTimeOnly				= 0x00020000,

	// This flag is declared by plugin and can be requested by applications
	// in order to open sources in light version i.e. with minimal information
	// and minimal resource waste (time, memory, connnections and others.).
	// This can be useful for example for internet radio, big files an so on.
	EAF_LightOpen					= 0x00040000
};

///////////////////////////////////////////////////////////// EMainScreenMode //

enum EMainScreenMode
{
	MSM_None						= 0,
	MSM_Normal						= 1,
	MSM_Shade						= 2,
	MSM_Minimized					= 3
};


/////////////////////////////////////////////////////////////// EAppearMethod //

enum EAppearMethod
{
	EAM_None						= 0,
	EAM_Photoplate					= 1,
	EAM_Stars						= 2,
	EAM_HorizontalJalousie			= 3,
	EAM_VerticalJalousie			= 4,
	EAM_Compass						= 5,
	EAM_ClockFace					= 6
};

////////////////////////////////////////////////////////////////////// EEvent //

#define EEvent DWORD

#define EE_None							0

#define EE_AudioSourceStart				100
	
// Shaded, normal, minimized, hidden, shown, restored
#define EE_MainWndModeChange			200
#define EE_MainWndMoving				201
#define EE_MainWndMoved					202
#define EE_MainWndChanged				203
	
#define EE_SkinChange					300
#define EE_AppearMethodChange			301
#define EE_SkinFolderChange				302

#define EE_LanguageChange				400

#define EE_ProfileChange				500

// This event is being sent periodically to announce all the architecture
// components that the main application is in the idle state.
#define EE_AppIdle						600

// 700 - 799 are reserved

// Sent when plugin want hide/show equalizer
#define EE_EqualizerHide				800
#define EE_EqualizerShow				801

// Send to plugins when application hide or show equalizer
#define EE_EqualizerIsHidden			850
#define EE_EqualizerIsShown				851

#define EE_DownloadStart				900
#define EE_DownloadFinish				901
#define EE_DownloadProgress				902

///////////////////////////////////////////////////////////////// EScreenMode //

enum EWndMode
{
	ESM_Undefined					= 0x00000000,
	ESM_Minimized					= 0x00000001,
	ESM_Maximized					= 0x00000002,
	ESM_Shaded						= 0x00000004,
	ESM_Visible						= 0x00000008
};

////////////////////////////////////////////////////////////// EAudioRawState //

enum EAudioRawState
{
	ERS_None						= 0x00000000,
	ERS_FirstAfterSeek				= 0x00000001,
	ERS_EndOfFile					= 0x00000002
};

////////////////////////////////////////////////////////////////// EMediaType //

enum EMediaType
{
	EMT_None						= 0x00000000,

	// 8 and 16 bit, whole frequency range, 1 or 2 channels.
	EMT_BasicAudio					= 0x00000001,

	// 8, 16, 24, 32 bit, whole frequency range, 1 or 2 channels.
	EMT_ExtAudio					= 0x00000002
};

////////////////////////////////////////////////////////////////// ELimitType //

enum ELimitType
{
	ERT_NoRestrictions				= 0,
	ERT_LimitedTimes				= 1,
	ERT_Expired						= 3,

	// One part has no restrictions, the other is limited.
	// For example, MP3 fully working but mp3PRO is limited.
	ERT_PartiallyLimited			= 4
};

//////////////////////////////////////////////////////////// EConvFactoryMode //

enum EConvFactoryMode
{
	ECFM_Playback,
	ECFM_Other
};

///////////////////////////////////////////////////////////// EStatusCategory //

enum EStatusCategory
{
	ESC_General,
	ESC_Exceptional
};

#endif // _AUDIO_TYPES_
