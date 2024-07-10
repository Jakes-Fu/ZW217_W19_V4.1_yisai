/*************************************************************************/
/*
 * Copyright (c) 2006 The libXnes Project.  All rights reserved.
 *
 **************************************************************************
 *
 * FILE NAME    : /include/vc_disable_warning.h
 *
 * CREATED      : 2011-07-29  22:20:28
 *
 * AUTHOR       : LL&CFA
 *
 **************************************************************************/
#ifndef __XINCL_VC_DISABLE_WARNING_H_INCLUDE__
#define __XINCL_VC_DISABLE_WARNING_H_INCLUDE__

#ifdef _MSC_VER 
	#pragma warning( disable : 4786 )
	#pragma warning( disable : 4711 )
	#pragma warning( disable : 4244 )
	#pragma warning( disable : 4127 )
	#pragma warning( disable : 4115 )
	#pragma warning( disable : 4206 )
	#pragma warning( disable : 4131 )
	#pragma warning( disable : 4505 )
	#pragma warning( disable : 4702 )
	#pragma warning( disable : 4054 )
	#pragma warning( disable : 4152 )
	#pragma warning( disable : 4005 )
	#pragma warning( disable : 4100 )
	#pragma warning( disable : 4514 )
	#pragma warning( disable : 4201 )

	#if _MSC_VER >= 1400 
		#pragma warning( disable : 4996 )
		#define _CRT_SECURE_NO_DEPRECATE
	#endif /* _MSC_VER >= 1400 */
#endif /* _MSC_VER */


#endif /* __XINCL_VC_DISABLE_WARNING_H_INCLUDE__ */

