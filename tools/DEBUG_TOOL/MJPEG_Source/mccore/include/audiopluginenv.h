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
|* FILE: audiopluginenv.h
|*
|* PURPOSE: Definition of the audio plugin manager interfaces
******************************************************************************/

#ifndef _AUDIO_PLUGIN_ENV_
#	define _AUDIO_PLUGIN_ENV_

#include "AuxDefs.h"
#include "WavFormat.h"
#include "AudioTypes.h"
#include "AudioPluginEnvIID.h"
#include "AudioErrors.h"

///////////////////////////// GENERIC INTERFACES ///////////////////////////////

interface IIdentifiable : public IUnknown
{
	virtual void
	GetID(GUID *pGUID)													PURE;
};

//////////////////////////////////////////////////////////////// IExAudioInfo //

interface IExAudioInfo : public IUnknown
{
	virtual int
	GetBitrate()														PURE;
};

//////////////////////////////////////////////////////////////////// IControl //

interface IControl : public IUnknown
{
	virtual const char *
	GetClassName()														PURE;

	virtual const char *
	GetTitle()															PURE;

	virtual int
	GetID()																PURE;
};

///////////////////////////////////////////////////////////////// IURLSupport //

interface IURLSupport : public IUnknown
{
	virtual EURLType
	GetSupportedURLTypes()												PURE;
};

////////////////////////////////////////////////////////////////// IAbortable //

interface IAbortable : public IUnknown
{
	virtual bool
	Abort()																PURE;
};

/////////////////////////////////////////////////////////////////// ISeekable //

// Seeks in audio stream.

interface ISeekable : public IUnknown
{
// Moves the current position in the stream. UiNewPos contains the new
// position in blocks.
	virtual bool
	Seek(ULONGLONG uiNewPos, IStatus **ppStatus = NULL)					PURE;
};

/////////////////////////////////////////////////////////////// IConfigurable //

// Any object can expose this interface and become configurable

interface IConfigurable : public IUnknown
{
	virtual bool
	Configure()															PURE;
};

/////////////////////////////////////////////////////////////// IAggregatable //

// Allows the object to be aggregated by outer objects in order to override
// some functionality. For example outer object will aggregate IAudioRawReader
// interfaces in order to perform conversion from plugin format to the format
// requested by the application.

interface IAggregatable : public IUnknown
{
	virtual bool
	SetAggregator(IUnknown *pAggregator)								PURE;

	virtual bool
	GetAggregator(IUnknown **ppAggregator)								PURE;

	virtual HRESULT STDMETHODCALLTYPE
	InnerQueryInterface( REFIID riid, void __RPC_FAR *__RPC_FAR *ppObj)	PURE;

	virtual ULONG
	STDMETHODCALLTYPE InnerAddRef()										PURE;
            
	virtual ULONG
	STDMETHODCALLTYPE InnerRelease()									PURE;
};

///////////////////////////////////////////////////////////////////// IStatus //

// Pointers of this type are passed to some functions in audio items, factories
// and others. If error occures those functions create objects exposing this
// interface and the application can obtain the detailed information about the
// error occured.
interface IStatus : public IUnknown
{
	virtual const char *
	GetDescription()													PURE;

	virtual EAudioError
	GetAudioError()														PURE;

	virtual DWORD
	GetSysError()														PURE;
};

///////////////////////////////////////////////////////////// IStatusCategory //

interface IStatusCategory : public IUnknown
{
	virtual EStatusCategory
	GetCategory()														PURE;
};

///////////////////////////////////////////////////////////////////// Process //

// Exposes functionality of some object that can be started and ended.
// Currently is used as a part of source and target items.

interface IProcess : public IUnknown
{
	virtual bool
	Start(IStatus **ppStatus = NULL)									PURE;

	virtual bool
	End(IStatus **ppStatus = NULL)										PURE;

	virtual bool
	IsInProcess()														PURE;
};

////////////////////////////////////////////////////////////////// IURLHolder //

// Allows the owner object to store, change, retrieve and determine the type
// of URL. Currently used as a part of IURLAudioSource and IURLAudioTarget.

interface IURLHolder : public IUnknown
{
	virtual bool
	SetURL(const char *szURL, IStatus **ppStatus = NULL)				PURE;

	virtual const char *
	GetURL()															PURE;

	virtual EURLType
	GetType()															PURE;
};

/////////////////////////////////////////////////////////////////// ILanguage //

// Changes the current language for all the windows shown by plugins.
// Plugin manager stores the current language in it and since all the
// components can reach the plugin manager, they can obtain and use it
// whenever is necessary.

interface ILanguage : public IUnknown
{
	virtual bool
	SetLanguage(LANGID id, IStatus **ppStatus = NULL)					PURE;

	virtual LANGID
	GetLanguage()														PURE;
};

//////////////////////////////////////////////////////////////////// IProfile //

// This information is very useful for plugins if they want to store some
// settings in registry. For different products using this library all those
// settings thus will be stored in different places. Of course the application
// must not forget to specify these parameters to the plugin manager.
interface IProfile : public IUnknown
{
	virtual bool
	SetVendor(const char *szVendor, IStatus **ppStatus = NULL)			PURE;

	virtual const char *
	GetVendor()															PURE;

	virtual bool
	SetProduct(const char *szProduct, IStatus **ppStatus = NULL)		PURE;

	virtual const char *
	GetProduct()														PURE;
};

//////////////////////////////////////////////////////////////////// IExtEnum //

// Enumerates the extensions supported by certain source/target audio factories.

interface IExtEnum : public IUnknown
{
	virtual int
	GetCount()															PURE;

	// The returned value can't be stored for later use.
	// The application must copy it.
	virtual const char *
	GetExt(int iNum)													PURE;
};

///////////////////////////////////////////////////////// ISourceStreamLength //

// This might be useful in some cases when the efficiency of encoding depends
// on the prior knowledge about the source.
interface ISourceStreamLength : public IUnknown
{
	// dwLength is the source stream length in blocks
	virtual void
	SetLength(DWORD dwLength)											PURE;
};

/////////////////////////// INTERRELATED INTERFACES ////////////////////////////

///////////////////////////////////////////////////////////// IAudioComponent //

interface IAudioPluginMgr;

// Exposed by all the audio plugins, i.e. source and target audio factories,
// audio RAW convertor factories, etc.

interface IAudioComponent : public IUnknown
{
	virtual const char *
	GetName()															PURE;

	// Application should first call this method to determine the type
	// of the component and after that query the appropriate interface.
	virtual EAudioComponentType
	GetType()															PURE;

	// This method is called after all the components are enumerated.
	// It should perform the actual initialization and store pMgr for
	// later use to access functionality provided by other plugins.
	// If the component initialization fails, it should return false
	// and the plugin manager will remove it from components list so
	// it will not be available.
	virtual bool
	Init(IAudioPluginMgr *pMgr, IStatus **ppStatus = NULL)				PURE;

	virtual bool
	Done()																PURE;
};

///////////////////////////////////////////////////////////////// IVendorInfo //

interface IVendorInfo : public IUnknown
{
	virtual const char *
	GetVendorName()														PURE;

	virtual bool
	CanDisplayAboutBox()												PURE;

	virtual void
	DisplayAboutBox()													PURE;
};

////////////////////////////////////////////////////////////////// IAudioItem //

// Exported by audio items: sources, targets and others. Supplies the
// general information about them.

interface IAudioItem : public IUnknown
{
	virtual bool
	GetCreator(IAudioComponent **pCreator)								PURE;

	// Bitmask of auxiliary flags
	virtual EAuxFlags
	GetAuxFlags()														PURE;

	virtual void
	SetAuxFlags(EAuxFlags flags)										PURE;
};

///////////////////////////////////////////////////////////// IAudioRawReader //

// Audio items expose this interface when they can extract audio RAW data.
// This interface corresponds to the EAF_Readable auxiliary audio item flag.

interface IAudioRawReader : public IUnknown
{
	virtual SWavFormat
	GetRawFormat()														PURE;

	// iBufSize must contain the size in bytes of the buffer pointed by pBuf.
	virtual bool
	RawRead(BYTE *pBuf, int iBufSize, int *piRead, EAudioRawState &state,
			IStatus **ppStatus = NULL)									PURE;

	// Returns RAW data length in bytes.
	virtual ULONGLONG
	GetRawLen()															PURE;
};

//////////////////////////////////////////////////////////////// IAudioSource //

interface IAudioSource : public IUnknown
{
	// Returns free-form text string about the item.
	virtual const char *
	GetInfo()															PURE;

	// Returns the file duration in milliseconds.
	virtual ULONGLONG
	GetDuration()														PURE;
};

///////////////////////////////////////////////////////// IAudioSourceFactory //

interface IAudioSourceFactory
{
	virtual EAuxFlags
	GetAuxFlags()														PURE;
};

////////////////////////////////////////////////////// IURLAudioSourceFactory //

// Opens source audio items by URLs.

interface IURLAudioSourceFactory : public IAudioSourceFactory, IURLSupport
{
	// "format" specifies the raw data format which the application
	// wants to get.

	virtual bool
	Open(	const		char *szURL,
			IUnknown	**ppSrc,
			EAuxFlags	flagsInclude	= EAF_None,
			EAuxFlags	flagsExclude	= EAF_None,
			IStatus		**ppStatus		= NULL)							PURE;
};

////////////////////////////////////////////////////////////////// IConvertor //

interface IConvertor : public IUnknown
{
	virtual bool
	Convert(BYTE *pData, int iNumberOfBytes, IStatus **ppStatus)		PURE;

	virtual int
	GetOutputBufSize()													PURE;

	virtual BYTE *
	GetOutputBuf()														PURE;
};

////////////////////////////////////////////////////////// IConvertorFactory2 //

interface IConvertorFactory2 : public IUnknown
{
	virtual void
	SetMode(EConvFactoryMode mode)										PURE;
};

/////////////////////////////////////////////////////////// IConvertorFactory //

// Produces instances of certain type raw convertor objects.

interface IConvertorFactory : public IUnknown
{
	virtual EMediaType
	GetSupportedMediaTypes()											PURE;

	virtual bool
	CreateConvertor(const SWavFormat	&src,
					const SWavFormat	&tgt,
					IConvertor			**ppConv,
					IStatus				**ppStatus = NULL)				PURE;
};

////////////////////////////////////////////////////////////// IEventReceiver //

interface IEventReceiver : public IUnknown
{
	virtual void 
	ReceiveEvent(EEvent event, IUnknown *pSource)						PURE;
};

////////////////////////////////////////////////////////////// IComponentEnum //

// Enumerates audio plugin components. Exposed by the central plugin manager
// and by all the audio plugin DLLs that need to incapsulate more than one
// component.

interface IComponentEnum : public IUnknown
{
	virtual int
	GetCount()															PURE;

	virtual bool
	GetComponent(int iNum, IAudioComponent **ppComp)					PURE;
};

////////////////////////////////////////////////////////// IAudioRawBlockInfo //

// Provides information about an audio source in blocks (current position,
// size of block, size of the source in blocks).

interface IAudioRawBlockInfo : public IUnknown
{
	virtual ULONGLONG
	GetPos()															PURE;

	virtual ULONGLONG
	GetBlockSize()														PURE;

	// If the audio source is infinite like in internet streams, return value
	// will be NO_LENGTH. Otherwise this method returns the length of RAW data
	// in blocks.
	virtual ULONGLONG
	GetDataLength()														PURE;
};

///////////////////////////////////////////////////////////// IAudioRawWriter //

// Allows to write audio RAW data into an object.

interface IAudioRawWriter : public IUnknown
{
	virtual bool
	RawWrite(BYTE *pData, int iNumberOfBytesToWrite, EAudioRawState &state,
				IStatus **ppStatus = NULL)								PURE;

	// Since the object is created, this function can return different values
	// depending on output target settings. This only cannot be changed during
	// the writing process so the application/aggregator call this function
	// immediately before the process starting.
	virtual SWavFormat
	GetRawFormat()														PURE;
};

///////////////////////////////////////////////////////// IAudioTargetFactory //

interface IAudioTargetFactory
{
	// Returns true if settings are changed, otherwise returns false.
	virtual bool
	EditSettings(IUnknown **ppTgt, int iCount)							PURE;

	virtual bool
	CanEditSettings()													PURE;
};

////////////////////////////////////////////////////// IURLAudioTargetFactory //

// Creates target audio items at URLs.

interface IURLAudioTargetFactory : public IAudioTargetFactory, IURLSupport
{
	// Caller must fill "formatSrc" accordingly to the format of source data
	// that is to be written in this target.
	virtual bool
	CreateURLAudioTarget(	IUnknown			**ppTgt,
							const SWavFormat	&formatSrc,
							IStatus				**ppStatus = NULL)		PURE;
};

///////////////////////////////////////////////////////////////// IInfoReader //

// Allows to obtain the additional information from a source item.

interface IInfoReader : public IUnknown
{
	// All these functions can return NULL for no data indication.

	virtual const char *
	GetTitle()															PURE;

	virtual const char *
	GetArtist()															PURE;

	virtual const char *
	GetAlbum()															PURE;

	virtual const char *
	GetYear()															PURE;

	virtual const char *
	GetGenre()															PURE;
};

///////////////////////////////////////////////////////////////// IInfoWriter //

interface IInfoWriter : public IUnknown
{
	virtual void
	SetTitle(const char *szTitle)										PURE;

	virtual void
	SetArtist(const char *szArtist)										PURE;

	virtual void
	SetAlbum(const char *szAlbum)										PURE;

	virtual void
	SetYear(const char *szYear)											PURE;

	virtual void
	SetGenre(const char *szGenre)										PURE;
};

//////////////////////////////////////////////////////////////// ITrackNumber //

interface ITrackNumber : public IUnknown
{
	// 0-based index or -1 if no track number is available
	virtual int
	Get()																PURE;

	virtual bool
	Set(int iNum)														PURE;
};

//////////////////////////////////////////////////////////// ISrcInfoCallback //

interface ISrcInfoCallback : public IUnknown
{
	virtual int
	GetCustomControlCount()												PURE;

	virtual bool
	GetControl(int iNum, IControl **ppControl)							PURE;

	virtual void
	OnModified(bool b)													PURE;

	virtual void
	OnCustomButton(int iID)												PURE;

	virtual bool
	OnSave()															PURE;

	virtual bool
	OnClose()															PURE;
};

//////////////////////////////////////////////////////// ISrcInfoViewerEditor //

// Brings a dialog with additional information about an audio source and
// sometimes allows to edit it and store back in the source (for example 
// in the mp3 URL audio source item).

interface ISrcInfoViewerEditor : public IUnknown
{
	virtual bool
	GetCallback(ISrcInfoCallback **ppNewCallback)						PURE;

	virtual void
	SetCallback(ISrcInfoCallback *pNewCallback)							PURE;

	virtual bool
	DoModal(IStatus **ppStatus = NULL)									PURE;

	virtual bool
	CloseModal(IStatus **ppStatus = NULL)								PURE;

	virtual bool
	SaveDialogToObject(IStatus **ppStatus = NULL)						PURE;

	virtual bool
	SaveObjectToFile(IStatus **ppStatus = NULL)							PURE;

	virtual bool
	CanSaveObjectToFile()												PURE;
};

////////////////////////////////////////////////////////////////////// ILimit //

interface ILimit : public IUnknown
{
	virtual bool
	IsUnlimited()														PURE;

	virtual const char *
	GetDescription()													PURE;
};

///////////////////////////////////////////////////////////////// IIndexArray //

interface IIndexArray : public IUnknown
{
	virtual int
	GetIndexNumber()													PURE;

	virtual int
	GetIndex(int iNum)													PURE;
};

///////////////////////////////////////////////////////////// IAudioPluginMgr //
// The general interface of the central plugin manager. All the components
// in the system receive it's pointer during the initialization and can refer
// to it to obtain, for example the profile information, language settings and
// so on. Also applications work with this interface to open and create 
// source/target audio items.
 
interface IAudioPluginMgr : public IUnknown
{
	virtual bool
	SetPluginDirectory(const char *szPluginDir)							PURE;

	virtual bool
	LoadPlugins()														PURE;

	virtual bool
	CanUnloadPlugins()													PURE;

	virtual bool
	UnloadPlugins()														PURE;

	virtual bool
	SetConvertor(IConvertorFactory *pCF, IStatus **ppStatus = NULL)		PURE;

	virtual bool
	GetConvertor(IConvertorFactory **ppConvFactory)						PURE;

	virtual bool
	GetUnsupportedComponents(IComponentEnum **ppEnum)					PURE;

															// Event functions

	// Sends an event to all the subscribed event receivers, they can be either
	// application-side objects, plugins, plugin manager itself or other
	// objects.
	virtual bool
	SendEvent(EEvent event, IUnknown *pSource)							PURE;

	virtual bool
	SubscribeToEvents(IEventReceiver *pReceiver)						PURE;

	virtual bool
	UnsubscribeToEvents(IEventReceiver *pReceiver)						PURE;

																// Other

	// Opens an audio file from URL and if the source quality is not the same
	// as "pFormat", it creates a wrapper object which aggregates the source.
	// pFormat can be NULL, in this case the original format will be received.
	virtual bool
	OpenURLAudioFile(	const char			*szURL,
						const SWavFormat	*pFormat,
						IUnknown			**ppSrc,
						int					iTries,
						EAuxFlags			flagsInclude,
						EAuxFlags			flagsExclude,
						IStatus				**ppStatus)					PURE;

	// Application should never create targets directly by target factories
	// because, it will be necessary to perform attitional format conversion
	// which plugin manager does automatically.
	virtual bool
	CreateURLAudioTarget(	IURLAudioTargetFactory	*pFactory,
							IUnknown				**pTgt,
							const SWavFormat		&formatSrc,
							IStatus					**ppStatus	= NULL)	PURE;


	// Some nonstandard objects can be attached to the plugin manager by
	// plugins and requested and handled by others. Only a single object of
	// a CLSID can be attached to the plugin manager at a time.
	
	// This method attaches an auxiliary object to the plugin manager.
	// Returns false if such object is already attached.
	virtual bool
		AttachAuxObject(REFCLSID clsID, IUnknown *pUnknown)					{return false;};

	virtual bool
	DetachAuxObject(REFCLSID clsID)										PURE;

	// Reguests an auxiliary object from the plugin manager. Returns false if
	// no such object is attached. Second parameter can be NULL, this is useful
	// for determining if such object is attached to the manager.
	virtual bool
	GetAuxObject(REFCLSID clsID, IUnknown **ppUnknown)					PURE;

	virtual bool
	TranslateError(IStatus *pInStatus, IStatus **ppOutStatus)			PURE;
};

#endif // _AUDIO_PLUGIN_ENV_
