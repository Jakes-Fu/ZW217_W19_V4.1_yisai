#ifndef CDFMSG_H
#define CDFMSG_H
/*============================================================================
// File Name:      cdfmsg.h
// Author:         caesar.shen
// DATE:           2001-11-6
// Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         
// Description:    Head file of message struct for c. 
//
//	
// ============================================================================
*/
#include "Datatype_Layer_Abstract.h"
/* ============================================================================
//                        Edit History                                       
// ----------------------------------------------------------------------------
// DATE           NAME             DESCRIPTION                               
// 2001-11-6     caesar		   Create. 
// ============================================================================
*/
/*!
	\file cdfmsg.h
	\brief Functions to access the bulk output of the transfer engine.

  The transfer engine is designed to support muti output formats. Each output format
  corresponds to a output dirver which can be linked to the engine on demand.
  In the current implementation, we have defined two very different output drivers:
  Dynamic output dirver and Bulk output dirver, the first one output a tree and the
  other output a structure.
  The dynamic output driver is written in c++, so it can be used only in c++ program;
  the bulk output driver is written in c, so it can be used in both c and c++ programs.
  
  If you want to known more about dynamic output mode, please see the document of CCDFMessage and
  CCDField.

  When working in bulk mode, the transfer engine will working on a structure buffer, all the
  decoded data is stored in this structure buffer. In order to simplified the proces of create
  and manipulate the structure buffer, we provides a set of functions. Using them, you can:
  <ul>
  <li> Create the structure buffer.
  <li> Create the dynamic allocated buffer. 	
  <li> Easily destory the structure buffer, all the dynamic allocate memory are freed when the main
structure buffer is freed.
  </ul>

  The cdfcompiler and the bulk output dirver use a special technique to reduce the memory consumption of
  the structure buffer. The memory is allocated dynamically for the dynamic and open array field. Only the
  pointer are stored in the structure, the body of the array are stored in a seperate memory block which is
  allocated on demand. For a message which has a dynamic array field, it memory will like this:
\code
    main structure buffer  
    +-------------------+
    |     FIELD_A       |
    +-------------------+
    |     FIELD_B       |
    +-------------------+
    |ARRAY_A(32bit pt)  |------------------------------------> ARRAY_A buffer
    +-------------------+                                     +----------------+
    |ARRAY_B(32bit pt)  |-----------> ARRAY_B buffer          |                | 
    +-------------------+            +--------------+         +----------------+
                                     |              |
                                     +--------------+ 
\endcode
  Please see the description of cdf_alloc_buffer() and cdf_alloc_array() to known more about how it works.
\par Sample:


Samples on decode:
\code
	H_CDF_MSG msg;
	MSG_type* buffer;// Assume that MSG_type is defined in caimessage.h,
                     // which is created automatically by the script.
	int A,B;
	msg = cdf_create_message(0x010002);// channel =1,type =2. cdf compiler use
	                                    //this message define to create the 
                                        //MSG_type structure in caimessage.h

	Decode_Message(msg,bitbuffer,bitlength);// this function is defined in
	                                        //Transfer_engine_API_c.h 

	buffer =(MSG_type*)cdf_get_data(msg);
    // Now you can get the value through buffer.
    A=buffer->FIELD_A;
	B=buffer->FIELD_B;
    ...
	...
	cdf_free_message(msg); // free the structure buffer and 
                           // all dynamic allocated buffer associated
                           // to it.
\endcode


Samples on encode:
\code
	H_CDF_MSG msg;
	MSG_type* buffer;// Assume that MSG_type is defined in caimessage.h,
                     // which is created automatically by the script.

	msg=cdf_create_message(0x010002);// channel =1,type =2
	cdf_create_buffer(msg,sizeof(MSG_type));
	buffer = (MSG_type*)cdf_get_data(msg);

    // Fill the fields with values
	buffer->FIELD_A=1;
	buffer->FIELD_B=2;
	Encode_Message(msg,&bitbuffer,&bitlength);// this function is defined in
	                                        //Transfer_engine_API_c.h 
	cdf_free_message(msg);
\endcode
  \sa CCDFMessage
*/
#ifdef __cplusplus
extern "C" {
#endif

typedef DWORD H_CDF_MSG; 
/*!
	\fn H_CDF_MSG cdf_create_message(DWORD code)
	\brief Create a new structure buffer.
	\param code The code of the message.
	\return H_CDF_MSG The handle of the new created structure buffer.

	This function creates a new structure buffer and return its handle. If it is failed,
	NULL will be returned.
	The code parameter indicates the type of the message. The code should correspond to 
	the message channel and type in CDF script. In CDF script, channel and type properties 
	are used to identify the message, they are 16bit integer. The code is 
	a 32bit integer, so we can use this formula to transform between them:
	
	code = (int)channel << 16 + type. 
*/
H_CDF_MSG cdf_create_message(DWORD code);
/*!
	\fn DWORD cdf_get_code(H_CDF_MSG msg);
	\brief Get the code associated to the structure buffer.
	\param  msg A valide handle of a structure buffer.
	\return the Code of the structure buffer.

  \note

  The msg parameter should be a valid handle, or ASSERT() will fail.
*/
DWORD cdf_get_code(H_CDF_MSG msg);
/*!
	\fn BOOL cdf_alloc_buffer(H_CDF_MSG msg,DWORD size)
	\brief allocate the main structure buffer
	\param msg A valid handle of a structure buffer
	\param size The size of the main structure buffer in byte
	\return TRUE when success or FALSE when failure

	There is a headache problem when useing c structure to represent the cdf message:
	Dynamic array and open array are field types in cdf script whose element types are
	dynamic. But in c structure, all the structures should have a fixed length.
	In order to resolve the problem, two method can be used: Use Max size and Dynamic
	allocate. The first method has been proved to be infeasible because the size of the 
	sturcture is too large(>200K in some message). But the second methos need to allocate
	the memory dynamically which will cause memory leaks if someone forget to free it 
	later. But this will never happen, if you use cdf_alloc_buffer() and cdf_alloc_array()
	function to allocate memory. All the memory blocks allocated by these two functions are
	linked in a chain, when cdf_free_message() is called , all the memory blocks in the chain
	are freed.

	\note

	The msg parameter must be a valid handle, or ASSERT() will fail.
	If you use this function on the same handle twice, ASSERT() will fail.

  \sa  cdf_alloc_array() cdf_free_message()

	
*/
BOOL cdf_alloc_buffer(H_CDF_MSG msg,DWORD size);
/*! \fn BOOL cdf_alloc_array(H_CDF_MSG msg,BYTE** address, WORD element_size, WORD element_count)
	\brief Allocate a memory block to store a dyamic or open array field.
	\param msg A valid handle of a structure buffer
	\param address The address of the array point in the main structure buffer
	\param element_size Element size of the array in byte
	\param element_count Element count of the array
	\return TRUE in success or FALSE in failure

  In order to save the memory consumption, dynamic and open array fields are stored in seperated
  memory blocks. Only a 32 bit pointer is stored in the main structure buffer which point to the
  allocated memory block. You should use this function to allocate the memory block, because all
  the memory blocks allocated by this function are linked in a chain. They will be freed when 
  cdf_free_message() is called. Furthermore, this function also write the information of the
  buffer to the chain node, these information is used by cdf_element_size() and cdf_element_count();
  
  All the dynamic and open array field should use cdf_alloc_array() to allocate their memory
  blocks, or the transfer engine (formerly called decode and encode engine)can not work properly.
 
  \sampe

  This sample will demostrate how to allocate an array buffer.

\code
	// assume the structure is like this:
	// struct SAMPLE_MESSAGE_type{
	//				BYTE FIELD_A;
	//				BYTE FIELD_B;
	//				BYTE * ARRAY_A;
	//         }

	SAMPLE_MESSAGE_type * buffer;
	H_CDF_MSG msg;
	msg=cdf_create_message(0x10002);
	cdf_create_buffer(msg,sizeof(SAMPLE_MESSAGE_type);
	buffer = (SAMPLE_MESSAGE_type*)cdf_get_data(msg);

	// fill the simple field with value
	buffer->FIELD_A =1;
	buffer->FIELD_B=2;
	cdf_alloc_array(msg,&buffer->ARRAY_A,sizeof(BYTE),100); // this will allocate 
                                                 //BYTE array with 100 element
	// fill the array field with value
	cnt = cdf_element_count(msg,buffer->ARRAY_A); // Now, cnt = 100
	for(i=0;i<cnt;i++)
	{
		buffer->ARRAY_A[i]=i; // fill the each element of ARRAY_A;
	}
    ...
	...
	cdf_free_message(msg); // all the memory blocks are freed! 
\endcode

\note

	The msg parameter should be a valid handle of structure buffer, or ASSERT() will fail.
	The element_size and element_count parameter should not be 0, or ASSERT() will fail. 
    If this function is called twice on the same array field, ASSERT() will fail.

	


\sa cdf_free_message() cdf_element_size(0 cdf_element_count()


*/
BOOL cdf_alloc_array(H_CDF_MSG msg,BYTE** address, WORD element_size, WORD element_count);
/*!
	\fn BYTE* cdf_get_data(H_CDF_MSG msg)
	\brief Return the address of the main buffer.
	\param msg A valid handle of structure buffer.
	\return The address of the main buffer.

  This function returns the address of the main buffer. If no main buffer is allocated, this function
  will return NULL.

  The fix fields and point of the dynamic arrayfields of the message are stored in 
  the main buffer, you can use a explicit type cast to  convert this buffer into a 
  structure which is defined in caimessage.h. You can use cdf_create_buffer() to 
  allocate the main buffer.

 \note

  The msg parameter should be a valid handle, or ASSERT() will fail.
  \sa cdf_get_size() cdf_create_buffer()
*/
BYTE* cdf_get_data(H_CDF_MSG msg);
/*!
	\fn DWORD cdf_get_size(H_CDF_MSG msg)
    \brief Return the size of the main buffer in byte
	\param msg A valid handle of structure buffer
	\return The size of the main buffer
	
	This function returns the size of the main buffer. If no main buffer is allocated, this
	function will return 0.

	The main buffer is allocated by the function: cdf_create_buffer()
	\note

  The msg parameter should be a valid handle, or ASSERT() will fail.
  \sa cdf_create_buffer() cdf_get_size()

*/
DWORD cdf_get_size(H_CDF_MSG msg);
/*!
	\fn WORD cdf_element_count(H_CDF_MSG msg, BYTE* address)
	\brief Return the element count of the dynamic or open array.
	\param msg A valid handle of structure buffer
	\param address Address of the memory block belong to the array
	\return The element count of the array
	
	This function return the element count of the dynamic or open array. You should
	send the address of the array as the second parameter and the array must be allocated
	by the cdf_alloc_array() function. If the address you passed in is not allocated by
	cdf_alloc_array(), 0 will be returned.

	All the dynamic and open array fields should use cdf_alloc_array() to allocate their memory
	blocks. 
	
	\note

  The msg parmeter should be a valid handle, or ASSERT() will fail.  You should
	send the address of the array as the second parameter and the array must be allocated
	by the cdf_alloc_array() function. If the address you passed in is not allocated by
	cdf_alloc_array(), 0 will be returned.

	All the dynamic and open array fields should use cdf_alloc_array() to allocate their memory
	blocks. 

  \sa cdf_alloc_array() cdf_element_size()
	
*/
WORD cdf_element_count(H_CDF_MSG msg, BYTE* address);
/*! 
	\fn WORD cdf_element_size(H_CDF_MSG msg, BYTE* address)
	\brief Return the element size of the dynamic or open array.
	\param msg A valid handle of structure buffer
	\param address Address of the memory block belong to the array
	\return The element size of the array in byte

  This function return the element size of the dynamic or open array in byte.
*/
WORD cdf_element_size(H_CDF_MSG msg, BYTE* address);
/*!
	\fn void cdf_free_message(H_CDF_MSG msg)
	\brief Destruct the structure buffer, both main structure buffer and linked array buffers.
	\param msg A valid handle of structure buffer
	\return void

  This function will free the structure buffer indicated by its handle, all the memory blocks 
  associated to the handle are freed. In order to eliminate the memory leaks, cdf_create_message()
  and cdf_free_message() should appear in pairs.
  
  If the msg parameter is a NULL handle, nothing will happen.
*/
void cdf_free_message(H_CDF_MSG msg);

#ifdef __cplusplus
}
#endif

#endif /*CDFMSG_H*/