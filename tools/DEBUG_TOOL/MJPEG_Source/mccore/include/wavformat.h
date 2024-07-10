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
|* FILE: wavformat.h
|*
|* PURPOSE: Definition of a structure holding the raw audio data formats
******************************************************************************/

#ifndef _WAV_FORMAT_
#	define _WAV_FORMAT_

#include "AudioDefs.h"

struct SWavFormat
{
// Construction/Destruction
	SWavFormat() { m_iSamplesPerSecond = m_iBitsPerSample = m_iChannels = -1; };
	SWavFormat(int iSamplesPerSecond, int iBitsPerSample, int iChannels)
	:	m_iSamplesPerSecond	(iSamplesPerSecond),
		m_iBitsPerSample	(iBitsPerSample),
		m_iChannels			(iChannels)
	{};

// Operations
	int GetFrameSize() const
	{
		return (m_iBitsPerSample / BITS_PER_BYTE * m_iChannels);
	};

	// Returns the amount of RAW data corresponding to this format
	int DataInOneSec() const
	{
		return (m_iBitsPerSample * m_iSamplesPerSecond * m_iChannels /
									BITS_PER_BYTE);
	}

	bool IsValid() const 
	{
		return (m_iBitsPerSample	> 0 &&
				m_iChannels			> 0 &&
				m_iSamplesPerSecond > 0);
	};

	void operator=(const SWavFormat &rSQ)
	{
		m_iBitsPerSample	= rSQ.m_iBitsPerSample;
		m_iChannels			= rSQ.m_iChannels;
		m_iSamplesPerSecond	= rSQ.m_iSamplesPerSecond;
	};

	bool operator==(const SWavFormat &rSQ) const
	{
		return (m_iBitsPerSample	== rSQ.m_iBitsPerSample		&&
				m_iChannels			== rSQ.m_iChannels			&&
				m_iSamplesPerSecond	== rSQ.m_iSamplesPerSecond);
	};

	bool operator!=(const SWavFormat &rSQ) const
	{
		return !operator==(rSQ);
	};

	float operator/(const SWavFormat &rSQ) const
	{
		return	(float)(m_iSamplesPerSecond * GetFrameSize()) /
				(float)(rSQ.m_iSamplesPerSecond * rSQ.GetFrameSize());
	};

// Data
	int	m_iSamplesPerSecond,	// ... 11025 ... 22050 ... 44100 ...

		m_iBitsPerSample,		// 8, 16, etc.

		m_iChannels;			// Number of channels in RAW
								// (1 - mono, 2 - stereo, etc.)
};

#define CD_QUALITY	SWavFormat(44100, 16, 2)

#endif // _WAV_FORMAT_
