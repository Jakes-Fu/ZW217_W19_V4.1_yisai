//Copyright (c) 2003, DPeshev
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without modification, 
//are permitted provided that the following conditions are met:
//
//- Redistributions of source code must retain the above copyright notice, 
//  this list of conditions and the following disclaimer. 
//
//- Redistributions in binary form must reproduce the above copyright notice, 
//  this list of conditions and the following disclaimer in the documentation 
//  and/or other materials provided with the distribution. 
//
//- Neither the name of the <ORGANIZATION> nor the names of its contributors may 
//  be used to endorse or promote products derived from this software without 
//  specific prior written permission. 
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
//ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
//LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
//SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
//STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
//IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
//POSSIBILITY OF SUCH DAMAGE.

#include <assert.h>

class CAudioPluginMgr : public IAudioPluginMgr, public IProfile
{
public:
	CAudioPluginMgr() : m_lRef(0) {};
	virtual ~CAudioPluginMgr() {}
public:
	// IUnknown
	STDMETHODIMP QueryInterface(REFIID riid, void __RPC_FAR* __RPC_FAR* ppObj);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	// IAudioPluginMgr
	virtual bool SetPluginDirectory(const char *szPluginDir);
	virtual bool LoadPlugins();
	virtual bool CanUnloadPlugins();
	virtual bool UnloadPlugins();
	virtual bool SetConvertor(IConvertorFactory *pCF, IStatus **ppStatus = NULL);
	virtual bool GetConvertor(IConvertorFactory **ppConvFactory);
	virtual bool GetUnsupportedComponents(IComponentEnum **ppEnum);
	virtual bool SendEvent(EEvent event, IUnknown *pSource);
	virtual bool SubscribeToEvents(IEventReceiver *pReceiver);
	virtual bool UnsubscribeToEvents(IEventReceiver *pReceiver);
	virtual bool OpenURLAudioFile(	const char			*szURL,
						const SWavFormat	*pFormat,
						IUnknown			**ppSrc,
						int					iTries,
						EAuxFlags			flagsInclude,
						EAuxFlags			flagsExclude,
						IStatus				**ppStatus);
	virtual bool CreateURLAudioTarget(	IURLAudioTargetFactory	*pFactory,
							IUnknown				**pTgt,
							const SWavFormat		&formatSrc,
							IStatus					**ppStatus	= NULL);
	virtual bool AttachAuxObject(REFCLSID clsID, IUnknown *pUnknown);
	virtual bool DetachAuxObject(REFCLSID clsID);
	virtual bool GetAuxObject(REFCLSID clsID, IUnknown **ppUnknown);
	virtual bool TranslateError(IStatus *pInStatus, IStatus **ppOutStatus);
	// IProfile
	virtual bool SetVendor(const char *szVendor, IStatus **ppStatus = NULL);
	virtual const char *GetVendor();
	virtual bool SetProduct(const char *szProduct, IStatus **ppStatus = NULL);
	virtual const char * GetProduct();
	// Data
protected:
	long    m_lRef;
};
//IAudioPluginMgr
bool CAudioPluginMgr::SetPluginDirectory(const char *szPluginDir) {return true;};
bool CAudioPluginMgr::LoadPlugins() {return true;};
bool CAudioPluginMgr::CanUnloadPlugins() {return true;};
bool CAudioPluginMgr::UnloadPlugins() {return true;};
bool CAudioPluginMgr::SetConvertor(IConvertorFactory *pCF, IStatus **ppStatus) {return true;};
bool CAudioPluginMgr::GetConvertor(IConvertorFactory **ppConvFactory) {return true;};
bool CAudioPluginMgr::GetUnsupportedComponents(IComponentEnum **ppEnum) {return true;};
bool CAudioPluginMgr::SendEvent(EEvent event, IUnknown *pSource) {return true;};
bool CAudioPluginMgr::SubscribeToEvents(IEventReceiver *pReceiver) {return true;};
bool CAudioPluginMgr::UnsubscribeToEvents(IEventReceiver *pReceiver) {return true;};
bool CAudioPluginMgr::OpenURLAudioFile(	const char			*szURL,
						const SWavFormat	*pFormat,
						IUnknown			**ppSrc,
						int					iTries,
						EAuxFlags			flagsInclude,
						EAuxFlags			flagsExclude,
						IStatus				**ppStatus) {return true;};
bool CAudioPluginMgr::CreateURLAudioTarget(	IURLAudioTargetFactory	*pFactory,
							IUnknown				**pTgt,
							const SWavFormat		&formatSrc,
							IStatus					**ppStatus) {return true;};
bool CAudioPluginMgr::AttachAuxObject(REFCLSID clsID, IUnknown *pUnknown) {return true;};
bool CAudioPluginMgr::DetachAuxObject(REFCLSID clsID) {return true;};
bool CAudioPluginMgr::GetAuxObject(REFCLSID clsID, IUnknown **ppUnknown) {return true;};
bool CAudioPluginMgr::TranslateError(IStatus *pInStatus, IStatus **ppOutStatus) {return true;};
// IProfile
bool CAudioPluginMgr::SetVendor(const char *szVendor, IStatus **ppStatus) {return true;};
const char *CAudioPluginMgr::GetVendor() { return "ahead";};
bool CAudioPluginMgr::SetProduct(const char *szProduct, IStatus **ppStatus) {return true;};
const char * CAudioPluginMgr::GetProduct() { return "Nero - Burning Rom"; };
// QueryInterface returns either IUnknown, IAudioPluginMgr or IProfile
STDMETHODIMP CAudioPluginMgr::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppObj) {
  // ppObj must not be NULL
  if(NULL == ppObj)
	{
    assert(FALSE);
    return E_INVALIDARG;
  }
  *ppObj = NULL;
  if(IsEqualIID(riid, IID_IUnknown)) *ppObj = static_cast<IUnknown*>(static_cast<IAudioPluginMgr*>(this));
  else if(IsEqualIID(riid, IID_IAudioPluginMgr)) *ppObj = static_cast<IAudioPluginMgr*>(this);
  else if(IsEqualIID(riid, IID_IProfile)) *ppObj = static_cast<IProfile*>(this);
  if(*ppObj) AddRef();
  if (NULL != *ppObj) return S_OK;
  else return E_NOINTERFACE;
}

// Increase the reference counter and return the number of references
STDMETHODIMP_(ULONG) CAudioPluginMgr::AddRef() {
  InterlockedIncrement(&m_lRef);
  return m_lRef;
}

// Decrease the reference counter and return the number of references
STDMETHODIMP_(ULONG) CAudioPluginMgr::Release() {
  InterlockedDecrement(&m_lRef);
  return m_lRef;
}

