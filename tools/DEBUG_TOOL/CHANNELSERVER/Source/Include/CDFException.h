

#ifndef CDFEXCEPTION_H
#define CDFEXCEPTION_H

// ============================================================================
// File Name:      CDFException.h
// Author:         caesar.shen
// DATE:           2001-10-09
// Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         
// Description:    Head file of CDF message naming space.
//
//	
// ============================================================================
#include "MsgObjects.h"
#pragma warning( push,2 )
#include <string>
#pragma warning( pop )
using namespace std;
// ============================================================================
//                        Edit History                                       
// ----------------------------------------------------------------------------
// DATE           NAME             DESCRIPTION                               
// 2001-10-09     caesar		   Create.                                   
// ============================================================================

//! Base exception class used in CDF message naming space
/*!
    CDF message naming space uses "c++ structured exception handling" technique to handle
    the abnormal situation. Some classes, such as CCDField, will throw an exception when
    any error occurs. 
*/
class CCDFException
{
private:
    int m_iErrorCode;
    string m_szErrorMessage;
public:
    //! Create a CCDFException.
    CCDFException(int iErrorCode,const char* szFormat, const char* szParam);
    virtual ~CCDFException(){}
    //! Delete this exception instance.
    /*!
        After you have handled the exception, it is your responsibility to delete it.
        There are two ways to accomplish this goal: 
        
        <ol>
          <li> Using delete operator: delete exception; 
          <li> Using the Delete method of the exception: exception.Delete();
        </ol>
        Where is no difference between these two ways, but the second way is more like a MFC
        exception handling style.
    */
    void Delete(){delete this;}
    //!Get the error message belonging to this exception.
    /*!
        You can use this function to get the error message of the exception.
    */
    const char* GetErrorMessage();
    //! Get the error code belonging to this exception.
    /*!
        You can use this function to get the error code of the exception
    */
    int GetErrorCode(){return m_iErrorCode;}
};
/*!
    \file CDFException.h
*/
/*!
    \enum CDF_EXEP_CODE 
    The possible error code of CCDFieldException.
*/
const int CDF_EXEP_COUNT =  23; 

#ifdef CDFEXCEPTION_IMPLEMENTATION

const char * CDF_EXEP_STRINGA[CDF_EXEP_COUNT+1]={
            "SUCCEEDED",
            "TE_E_DUPLICATE_NAME", 
            "TE_E_FIELD_NOT_FOUND",
            "TE_E_NOT_SIMPLE",
            "TE_E_NOT_ARRAY",
            "TE_E_NOT_STRUCTURE",
            "TE_E_ACCESS_COUNT_ON_SIMPLE",
            "TE_E_ARRAY_NOT_EMPTY",
            "TE_E_ACCESS_CHILD_ON_SIMPLE",
            "TE_E_NAME_TOO_LONG",
            "TE_E_OUT_OF_BOUND",
            "TE_E_BAD_ELEMENT_TYPE",
            "TE_E_ACCESS_NAME_ON_ELEMENT",
            "TE_E_CAN_NOT_ASSIGN",
			"TE_E_TOO_LESS_CHILDREN",
			"TE_E_TOO_MANY_CHILDREN",
			"TE_E_BAD_FIELD_TYPE",
			"TE_E_BAD_CODE",
			"TE_E_BIT_OUT_OF_BOUND",
			"TE_E_BYTE_OUT_OF_BOUND",
			"TE_E_TOO_MANY_EMBED_LEVEL",
			"TE_E_ALLOCATE_ERROR",
			"TE_E_DRIVER_NOT_INSTALLED",          
            "UNKNOWN ERROR"
};
const char** CDF_EXEP_STRING = CDF_EXEP_STRINGA;
#else
    extern const char ** CDF_EXEP_STRING;
#endif

enum CDF_EXEP_CODE{
    TE_E_SUCCESS = 0, /**<It is not used in this version.*/
    TE_E_DUPLICATE_NAME =1, /**< Occurs when you try to add a field with duplicated name
                                into a structure. In CDF naming space, field name should be unique in
                                one structure.*/
    TE_E_FIELD_NOT_FOUND,/**< Occurs when you try to get a field which does not exsist.*/
    TE_E_NOT_SIMPLE,/**< Occurs when you try to set/get the value of array field or structure field.
                        In CDF naming space, you can only set/get the value of simple field.
                      */
    TE_E_NOT_ARRAY,/**<  Occurs when you try to get/set the default element type of the field on a none-array
                field. In CDF naming space, Only array fields have default element type.*/
    TE_E_NOT_STRUCTURE,/**< Occurs when you try to get a child field by its name from none-structure fields.
                        In CDF naming space, you can only access the children by name on structure fields.*/
    TE_E_ACCESS_COUNT_ON_SIMPLE,/**< Occurs when you try to get the count of children on simple fields. In
                            CDF naming space, simple fields have no children belonging to it.*/
    TE_E_ARRAY_NOT_EMPTY,/**< Occurs when you try to set the default element type on none empty array fields.
                            In CDF naming space, each array element should have same type. So default element type can
                            not be set on none empty array.*/
    TE_E_ACCESS_CHILD_ON_SIMPLE,/**<Occurs when you try to get/set the children on simple fields.
                               In CDF naming space, simple fields have no children belonging to it. So you can not
                               access them either.*/
    TE_E_NAME_TOO_LONG,/**< Occurs when you try to field names, if the name contains more than 63 character.
                    In CDF naming space, the max length of field name is 63 character. */
    TE_E_OUT_OF_BOUND,/**< Occurs when you try to get/set/access a child field by its index, if the index is
                    less than zero, more than the children count minus 1 (when delete,get) or more than the children count minus
                    (when insert).*/
    TE_E_BAD_ELEMENT_TYPE,/**<Occurs when you try to add element with different type into an array. In CDF naming
                        space, each element in an array should has same type.*/
    TE_E_ACCESS_NAME_ON_ELEMENT,/**<Occurs when you try to get the names of elements in an array. In CDF
                    naming space, array fields,not like structure fields, its elements can not be accessed by their names.*/
    TE_E_CAN_NOT_ASSIGN,/**<Occurs when you try to assign a field to another. The assign operator between the field
    is forbidden in order to prevent memory leaks.*/
	TE_E_TOO_LESS_CHILDREN,/**<Occurs when encoding the message. It means that: some children fields are omited*/
	TE_E_TOO_MANY_CHILDREN,/**<Occurs when encoding the message. It means that: Too many children fields are defined.*/
	TE_E_BAD_FIELD_TYPE,/**<Occurs when encoding the message. It means that: field type of the field is incorrect!*/
	
	TE_E_BAD_CODE,/**<Occurs when you passed a wrong message code to the encode or decode function*/
	TE_E_BIT_OUT_OF_BOUND,/**<Occurs when the index is exceed the size of the bitbuffer. It is usually caused by the
						incurrect input buffer.*/
	TE_E_BYTE_OUT_OF_BOUND,/**<Occurs when the index is exceed the size of the byte buffer. It is usually caused bye the
						incurrect input buffer.*/
	TE_E_TOO_MANY_EMBED_LEVEL,/**<Occures when the embed level of the message is more than 32.*/
	TE_E_ALLOCATE_ERROR,/**<Occurs when failed to allocate the memory*/
	TE_E_DRIVER_NOT_INSTALLED,/**< Occurs when a selected output dirver is not installed*/

    //Extesion Error code for struct parser be Leo.Feng at 2002-05-29
    TE_E_EXT_PARAMETER_ERROR,            /**The Parameter is invalid */
    TE_E_EXT_OPEN_FILE_ERROR,            /**Can't Open the file */
    TE_E_EXT_FILE_FORMAT_ERROR,          /**the file Format is error*/
    TE_E_EXT_NOT_FILE_AVAILABLE,         /**The Message Description File Not Opened */
    TE_E_EXT_NOT_FIND_MSG,               /**Can't Find the message */
    TE_E_EXT_DATA_SIZE_ERROR,            /**Data Size is Error */
    TE_E_EXT_DATA_HEADER_ERROR,
    TE_E_EXT_DATA_CONTENT_ERROR,
    TE_E_EXT_UNKNOW_DATA_TYPE,
    TE_E_EXT_CONVERTING_ERROR,    
    TE_E_EXT_OUTOF_MEMORY,     
    //End of Extesion Error Code for struct parser

    //Extesion Error code for script parser be Leo.Feng at 2002-05-29
    TE_E_EXT_INIT_FAILED,                /**The Initialize Operation is Not Finished */
    /**For Scrpit Parser */
    TE_E_EXT_INVALID_SYMBOL,             /**The Symbol is invalid */
    TE_E_EXT_ANGLE_BRACKET_NOT_MATCH,    /**The Angle bracket is not matched */
    TE_E_EXT_SQUARE_BRACKET_NOT_MATCH,   /**The Square bracket is not matched */
    TE_E_EXT_INVALID_VAR_STARTING,       /**The Starting of variable is invalid */
    TE_E_EXT_INVALID_SEPERATOR,          /**The Seperator is Invalid */
    TE_E_EXT_INVALID_SYMBOL_HEX_CAHRSET, /**The Symbol is an invalid Hexadecima Charset */
    TE_E_EXT_INVALID_SYMBOL_HEX_ID,      /**The Symbol is an invalid Hexadecima Identify */
    TE_E_EXT_AMBIGUOUS_VAR_TYPE,         /**The Variable Type is Ambiguous */
    TE_E_EXT_INVALID_OPERATOR,           /**The operator is invalid */
    TE_E_EXT_INVALID_ARRAY_INDEX,        /**The Array index is an invalid vaule*/
    TE_E_EXT_LIST_EMPTY,                 /**The List has not Item */
    TE_E_EXT_MEMORY_OVERFLOW,            /**The Memory Access overflow */
    TE_E_EXT_SCRIPT_ZERO,                //add 
    TE_E_EXT_TOO_NEST_LEVEL,             //
    /**For Variable Table */
    TE_E_EXT_NULL_PARAMETER,             /**The Variable Name is NULL as Function Parameter */
    TE_E_EXT_TOO_INITIALIZED,            /**The Object has Been Intialized */
    TE_E_EXT_NOT_INITIALIZED,            /**The Object has Not Been Intialized */
    TE_E_EXT_NAME_TOO_LONG,              /**The Variable Name is Too Long */
    TE_E_EXT_DUPLICATE_NAME,             /**The Variable Name is Duplicate */
    TE_E_EXT_VAR_NOT_FOUND,              /**The Variable has not existed */
    TE_E_EXT_VAR_TYPE_NOT_MATCHED,       /**The Variable Type is not Matched */
    TE_E_EXT_UNKNOWN_ERROR               /**The Error is not Unknow */             
    //End of Extesion Error Code for script parser


};
//! Exception class derived from CCDFException.
/*!
    This class is derived from CCDFException, compare with its ancestor, it has some extra
    features:
    <ul>
    <li>You can use GetSource() function to get the field in which the exception occurs.
    <li>you can use GetSourceFullName() function to get the the full name of the field.
    The full name of a field includes all the names of its direct parents in hierarchy,
    each name are seperated use a dot.
    </ul>
    All the possible error codes are defined in: #CDF_EXEP_CODE 
     
*/
class CCDFieldException :public CCDFException{
private:
    CCDField* m_pSource;
    string m_szSourceFullName;
public:
    //! The Constructor.
    CCDFieldException(int iErrorCode,const char* szFormat, const char* szParam,CCDField* pSource);
    //! Get the field in which this exception occurs.
    const CCDField* GetSource();
    //! Get the full name of the field in which this exception occurs. 
    const char* GetSourceFullName();
    
};
//! Exception class derived from the CCDFException.
/*!
    This exception is used when Transfer the message from / to the bit buffer.
	No function is added, compared with its ancestor.
*/
class CCDFTransferException:public CCDFException
{
public:
	CCDFTransferException(int iErrorCode,const char* szFormat, const char* szParam)
	:CCDFException(iErrorCode,szFormat,szParam){}
};
#endif 
