#ifndef TRANSFER_ENGINE_API_CPP_H
#define TRANSFER_ENGINE_API_CPP_H
/*============================================================================
// File Name:      Transfer_Engine_api_cpp.h
// Author:         caesar.shen
// DATE:           2001-11-9
// Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         
// Description:    Head file of c++ interface of the transfer_engine 
//
//	
// ============================================================================
*/
#ifndef CPP_LIBRARY
#define CPP_LIBRARY
#endif
#include "msgobjects.h"
#include "cdfmsg.h"
#include "Datatype_layer_Abstract.h"

/* ============================================================================
//                        Edit History                                       
// ----------------------------------------------------------------------------
// DATE           NAME             DESCRIPTION                               
// 2001-11-9     caesar		   Create. 
// ============================================================================
*/
//! high level c++ api for the transfer engine
/*!
	This class provides the c++ api for the transfer engine. In order to use the c++ api interface, 
	you have to include this file, and your program must link to TrnsEngineCPP.lib(or TRnsEngineCPPD.lib 
	if is Debug ). All the output drivers are supported by the C++ api. 
	
	All the output driver can be installed(linked to the library) on demand, you can modify the
	Output_driver_Configeration.h to adjust the default setting.

	All the functions in this class are declared as static, you can use them without create the instant
	of the class. 

	This class uses standard c++ exceptions handle to deal with the abnormal conditions. There are two
	kind of the exception can be threw by the class: CCDFieldException and CCDFTransferException. Please
	handle them carefully. You can use the CCDFException::GetErrorCode() to get the error code, the 
	possible error code is listed in CDF_EXEP_CODE.

	\sa CDF_EXEP_CODE CCDFieldException CCDFTransferException 

*/
class CTransferEngine{
private:
	static void Check_Result(int Error_No);
public:
	//Constructor and destructor
	CTransferEngine();
	virtual ~CTransferEngine();
	//! Decode a message using dynamic output driver.
	/*!
		\param msg A reference of a CCDFMessage instance.
		\param bits Address of the bit encoded message buffer.
		\param bit_len The length of the buffer in bit.
		\exception CCDFieldException
		\exception CCDFTransferException

	This function transfer a bit encoded message and fill the decoded message into msg. 
	The bit encoded message buffer's address is indicated by the bits parameter and the 
	length of the buffer is indicated by the bit_len parameter(the length is in bit).

    After this function is successfully returned, the decoded message is stored in the
	msg or an exception will be threw. 

  \sa CDF_EXEP_CODE CCDFieldException CCDFTransferException.
	*/
	static void Decode(CCDFMessage& msg, BYTE* bits, DWORD bit_len);
	//! Decode a message using bulk output driver.
	/*!
		\param msg A Valid Structure buffer handle.
		\param bits Address of the bit encoded message buffer.
		\param The length of the buffer in bit.
		\exception CCDFTransferException

	This function transfer a bit encoded message and fill the decoded message into
	the structure buffer. The bit encoded message buffer's address is indicated by
	the bits parameter and the length of the buffer is indicated by	the bit_len 
	parameter(the length is in bit).

	Its behavior is identical to the decode_message() function in c api except one
	aspect: The error condition is handled by the exception not by the return value.

   \sa CDF_EXEP_CODE CCDFTransferException

	*/
	static void Decode(H_CDF_MSG msg, BYTE* bits, DWORD bit_len);
	//! Encode a message using dynamic output driver.
	/*!
		\param msg A reference of CCDFMessage instance.
		\param bits Address of the bit buffer pointer.
		\param bit_len The length of the bit buffer in bit.
		\exception CCDFTransferException
		\exception CCDFieldException

	This function compose the data in msg into a bit encoded message. The buffer of the
  bit message is allocated by this function and should be freed by the caller. The length of
  the bit encoded message is stored in the bit_len parameter(the length is in bit).

  After this function is successfully returned, the * bits points to a buffer which contains
  the bit message and * bit_len conatins the buffer length in bit.

  If an error occurs, an CCDFTransferException will be threw. 

  \sa CDF_EXEP_CODE CCDFTranserException CCDFieldExcepton
	*/
	static void Encode(CCDFMessage& msg, BYTE** bits, DWORD& bit_len);
	//! Encode a message using bulk output driver.
	/*!
		\param msg A valid structure buffer handle.
		\param bits Address of the bit buffer pointer.
		\param The length of the bit buffer in bit.
		\exception CCDFTransferException

  This function compose the data in msg into a bit encoded message. 

  The behavior of the function is idendical to the Encode_Message() function in c api, 
  except one  aspect: The error condition is handled by the exception not by the 
  return value.

   \sa CDF_EXEP_CODE CCDFTransferException
	*/
	static void Encode(H_CDF_MSG msg, BYTE** bits, DWORD& bit_len);
	//! Trace a message using trace output driver.
	/*!
		\param code A message code.
		\exception CCDFTransferException

  This function is identical to the Trace_Message() function in c api, except one
  aspect: The error condition is handled by the exception not by the 
  return value.

   \sa CDF_EXEP_CODE CCDFTransferException
	*/
	static void Trace(DWORD code);
};

#endif /*TRANSFER_ENGINE_API_CPP_H*/