#ifndef ERRORSTRING_H
#define ERRORSTRING_H
// ============================================================================
// File Name:      ErrorString.h
// Author:         caesar.shen
// DATE:           2001-10-09
// Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         
// Description:    Store the error message of CCDFException.
//
//	
// ============================================================================

// ============================================================================
//                        Edit History                                       
// ----------------------------------------------------------------------------
// DATE           NAME             DESCRIPTION                               
// 2001-10-09     caesar		   Create.                                   
// ============================================================================
#ifdef CDFEXCEPTION_IMPLEMENTATION

const char* CDF_EXEP_MSGSA[] ={
    "OK",
    "Field name: '%' is duplicated!",
    "Field name: '%' can not be founded!",
    "This operation can only be used on a simple field!",
    "This operation can only be used on a array field!",
    "This operation can only be used on a structure field!",
    "Can not access the children count on a simple field!",
    "Can not access the element type on a none-empty array!",
    "Can not access the child on a simple field!",
    "Field name can not exceed 63 characters!",
    "The index is out of the bound!",
    "Can not add elements with different type to an array!",
    "Can not access the name of an array element with simple type!",
    "Can not assign field from another!",
	"Encode error: The children of the field are less than needed!",
	"Encode error: The children of the field are more than needed!",
	"Encode error: The type of the field is incorrect!",
	"Engine error: Invalid message code!",
	"Engine error: The index is out of the boundary of the bit buffer!",
	"Engine error: The index is out of the boundary of the byte buffer!",
	"Engine error: Too many embed levels in the message!",
	"Engine error: Failed to allocate memory!",
	"Engine error: The selected output driver is not installed!",

    //Extesion Error string for struct parser be Leo.Feng at 2002-05-29
    "'%' The Parameter is invalid!",
    "Can't Open the file '%'!",
    "The File '%' Fromat is Error!",
    "'%' The File Not Opened !",    
    "Can't Find the message '%'!",
    "'%' Data Size is Error!",
    
    //For CEndianConvert
    "'%' Data Header is Error!",
    "'%' Data Content is Error!",
    "'%' Data Type is Error!",
    "'%' Data Convert is Failed!",
    
    //For System Memory
    "'%' The System has not enough memory to perform the operation! ",
    //End of Extesion Error string
   
    "% Initialize Failed!",
    "The Symbol ' % ' is not Valid!",
    "% The Symbol '{' and '}' is not matched !",                
    "% The Symbol '[' and ']' is not matched !",
    "The Symbol ' % ' is not as the strarting of a variable!",
    "The Symbol ' % ' is not as the Seperator between left variable and equal mark!",
    "The Symbol ' % ' is not valid symbol in a hexadecima variable!",
    "The Symbol ' % ' is not as the identify of a hexadecima variable!",
    "% The Variable Type is Ambiguous!",
    "The ' % ' is not an valid Operator!",
    "% The Array index is an invalid vaule!",
    "The List  % has not Item!",
    "% The Memory Access overflow!",
    "% The Script's Length is 0!",
    "% The structure nesting is overhead '255'!",
    
    //For Variable Table
    "% The Parameter Can't be NULL as % Parameter!",
    "% has been Initialized!",
    "% has not been Initialized!",
    "The Variable Name % Length is more than 64!",
    "% The Variable Name has already existed!",
    "% The Variable Name has not existed!",
    "% The Variable Type is not matched!",
    "UNKNOWN ERROR"

};
  const char ** CDF_EXEP_MSGS = CDF_EXEP_MSGSA;
#else
   extern const char** CDF_EXEP_MSGS;
#endif

#define THROW_DUPLICATE_NAME(s) \
    throw new CCDFieldException(TE_E_DUPLICATE_NAME,\
                                CDF_EXEP_MSGS[TE_E_DUPLICATE_NAME],\
                                s,\
                                this\
                                )

#define THROW_FIELD_NOT_FOUND(s) \
     throw new CCDFieldException(TE_E_FIELD_NOT_FOUND,\
                                CDF_EXEP_MSGS[TE_E_FIELD_NOT_FOUND],\
                                s,\
                                this\
                                )
#define THROW_NOT_SIMPLE \
    throw new CCDFieldException(TE_E_NOT_SIMPLE,\
                                CDF_EXEP_MSGS[TE_E_NOT_SIMPLE],\
                                NULL,\
                                this\
                                )
#define THROW_NOT_ARRAY \
    throw new CCDFieldException(TE_E_NOT_ARRAY,\
                                CDF_EXEP_MSGS[TE_E_NOT_ARRAY],\
                                NULL,\
                                this\
                                )
#define THROW_NOT_STRUCTURE \
    throw new CCDFieldException(TE_E_NOT_STRUCTURE,\
                                CDF_EXEP_MSGS[TE_E_NOT_STRUCTURE],\
                                NULL,\
                                this\
                                )

#define THROW_ACCESS_COUNT_ON_SIMPLE \
     throw new CCDFieldException(TE_E_ACCESS_COUNT_ON_SIMPLE,\
                                CDF_EXEP_MSGS[TE_E_ACCESS_COUNT_ON_SIMPLE],\
                                NULL,\
                                this\
                                )

#define THROW_ARRAY_NOT_EMPTY\
     throw new CCDFieldException(TE_E_ARRAY_NOT_EMPTY,\
                                CDF_EXEP_MSGS[TE_E_ARRAY_NOT_EMPTY],\
                                NULL,\
                                this\
                                )

#define THROW_ACCESS_CHILD_ON_SIMPLE \
     throw new CCDFieldException(TE_E_ACCESS_CHILD_ON_SIMPLE,\
                                CDF_EXEP_MSGS[TE_E_ACCESS_CHILD_ON_SIMPLE],\
                                NULL,\
                                this\
                                )

#define THROW_NAME_TOO_LONG \
     throw new CCDFieldException(TE_E_NAME_TOO_LONG,\
                                CDF_EXEP_MSGS[TE_E_NAME_TOO_LONG],\
                                NULL,\
                                this\
                                )

#define THROW_OUT_OF_BOUND \
     throw new CCDFieldException(TE_E_OUT_OF_BOUND,\
                                CDF_EXEP_MSGS[TE_E_OUT_OF_BOUND],\
                                NULL,\
                                this\
                                )

#define THROW_BAD_ELEMENT_TYPE \
     throw new CCDFieldException(TE_E_BAD_ELEMENT_TYPE,\
                                CDF_EXEP_MSGS[TE_E_BAD_ELEMENT_TYPE],\
                                NULL,\
                                this\
                                )

#define THROW_ACCESS_NAME_ON_ELEMENT \
     throw new CCDFieldException(TE_E_ACCESS_NAME_ON_ELEMENT,\
                                CDF_EXEP_MSGS[TE_E_ACCESS_NAME_ON_ELEMENT],\
                                NULL,\
                                this\
                                )

#define THROW_CAN_NOT_ASSIGN\
     throw new CCDFieldException(TE_E_ACCESS_NAME_ON_ELEMENT,\
                                CDF_EXEP_MSGS[TE_E_ACCESS_NAME_ON_ELEMENT],\
                                NULL,\
                                this\
                                )

#define THROW_TOO_LESS_CHILDREN(a)\
     throw new CCDFieldException(TE_E_TOO_LESS_CHILDREN,\
                                CDF_EXEP_MSGS[TE_E_TOO_LESS_CHILDREN],\
                                NULL,\
                                a\
                                )

#define THROW_TOO_MANY_CHILDREN(a)\
     throw new CCDFieldException(TE_E_TOO_MANY_CHILDREN,\
                                CDF_EXEP_MSGS[TE_E_TOO_MANY_CHILDREN],\
                                NULL,\
                                a\
                                )
#define THROW_BAD_FIELD_TYPE(a)\
     throw new CCDFieldException(TE_E_BAD_FIELD_TYPE,\
                                CDF_EXEP_MSGS[TE_E_BAD_FIELD_TYPE],\
                                NULL,\
                                a\
								)

#define THROW_BAD_CODE()\
	throw new CCDFTransferException(TE_E_BAD_CODE,\
									CDF_EXEP_MSGS[TE_E_BAD_CODE],\
									NULL)
#define THROW_BIT_OUT_OF_BOUND()\
	throw new CCDFTransferException(TE_E_BIT_OUT_OF_BOUND,\
									CDF_EXEP_MSGS[TE_E_BIT_OUT_OF_BOUND],\
									NULL)
#define THROW_TOO_MANY_EMBED_LEVEL()\
	throw new CCDFTransferException(TE_E_TOO_MANY_EMBED_LEVEL,\
									CDF_EXEP_MSGS[TE_E_TOO_MANY_EMBED_LEVEL],\
									NULL)											
#define THROW_ALLOCATE_ERROR()\
	throw new CCDFTransferException(TE_E_ALLOCATE_ERROR,\
						 CDF_EXEP_MSGS[TE_E_ALLOCATE_ERROR],\
						 NULL)

#define THROW_DRIVER_NOT_INSTALLED()\
	throw new CCDFTransferException(TE_E_DRIVER_NOT_INSTALLED,\
						 CDF_EXEP_MSGS[TE_E_DRIVER_NOT_INSTALLED],\
						 NULL)

//Extesion Error code for struct parser be Leo.Feng at 2002-05-29
#define THROW_EXCEPTION_EXT(nErrCode, szReplace) \
    throw new CCDFException(nErrCode,\
                                CDF_EXEP_MSGS[nErrCode],\
                                szReplace\
                                )
#endif