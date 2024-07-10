// ============================================================================
// File Name:      MsgObjects.h
// Author:         caesar.shen
// DATE:           2001-10-08
// Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         
// Description:    Head file of CDF message naming space.
//
//	
// ============================================================================
#ifndef MSGOBJECTS_H
#define MSGOBJECTS_H
#pragma warning( push,2 )
#include <vector>
#pragma warning( pop )
using namespace std;
// ============================================================================
//                        Edit History                                       
// ----------------------------------------------------------------------------
// DATE           NAME             DESCRIPTION                               
// 2001-10-08     caesar		   Create.  
// 2001-11-01	  caesar		  Add function : GetLevel(),
//												 SetUsrData(), 	
//												 GetUsrData(), to the CCDField Class. 
// 2001-11-02	  caesar		  
//	Improve the CCDField::operator[], now it can use such syntax to access the
// child of the array:
// array_field["#1"]
//					Get the second child of the array_field.
// root["array_field.#0"]
//					Get the first child of the array_field. 	                                    
// ============================================================================


/*!\file MsgObjects.h
   \brief Head file for CDF message naming space.

    This file defines essential types and classes for the CDF message naming space.
    Using CDF message naming space, you can access the contents of CDF messages dynamicly.
 
*/
/*!\enum CDF_Field_Type
   \brief Type of field. 

    CDF_Field_Type is used to determine the type of a field.  
*/
enum CDF_Field_Type{
 	CDV_SIMPLE,/**< This field is a simple field.*/
	CDV_ARRAY,/**< This field is an array field*/
	CDV_STRUCTURE/**< This field is a sub structure field*/
};


//! CCDField represents a field of message.
/*!
	In dynamic mode, the decode engine will use the decoded data to construct a tree, which is called 
    CDF message naming space. Each fields of the message are stored as tree nodes. There are two types of
    fields in CDF message naming space, they are: simple fields and compound fields. The simple fields, implied
    by its name, are fields which have no children. Oppositely, the compound fields can have children fields
    which can also be an simple field or a compound field. Using this architecture, a complicated message
    can have many hierarchies and each hierarch has a lot of fields. We use a technique,called "naming space",
    to access the content in such a complicated hierarchy. Naming space is something like a directory in file system,
    which has such characters:
    <ul>
    <li>File names in same directories are unique.
    <li>File names in different directories can be duplicated.
    <li>We can identify a file by its name and directory no matter how many files and sub directories are stored in 
        the file system.
    </ul>
    With the similar concept, we can define the CDF message naming space as :
    <ul>
    <li> Simple fields are stored in leaf nodes of the message tree, just like the file in file system.
    <li> Compound fields are stored in none-leaf nodes of the message tree, just like the directory in file system.
    <li> Field names within same parent field are unique.
    <li> Field names in different parent field(different structure scope) can be duplicated.
    <li> We can identify a field by its name and all its parent fields' name.
    </ul>
    In addition, CDF message naming space has some characters which file system does not have:
    <ul>
    <li>Field can identify by its index. And for array field, which is also a compound field, it is the only method to access
        its children fields (or more percisely speaking, the array elements).
    </ul>
   
    We use CCDField to represent the fields, simple fields or compound fields, in the message.
  \par
	CCDField provids many methods to access the message content. Using it you can do such things:
  <ul>
  <li>Get the type of fields.
  <li>Locate fields by full name or index.
  <li>Access the values of simple fields.
  <li>Get the children count of compound fields and enumerate each children of it recursively.
  <li>Manipulate children fields of compound fields.
  </ul>	 
	
  \par How to get the type of fields
  <br>

  We can use GetType() function to get the type. 

  Sample:

\code
    CDF_Field_Type type;
    type = node.GetType();
    switch(type)
    {
    case CDV_SIMPLE:{
        printf("The value of the simple field is %d",
                (int)node);
        break;
    }
    case CDV_ARRAY:{
        printf("The count of the array field is %d",
                node.GetCount());
        break;
    }
    case CDV_STRUCTURE:{
        printf("The name of the sub-structure is %s",
                node.Getname());
        break;
    }
    default:
		_ASSERT(0);
	}
\endcode
  
\par How to locate fields by name and index

  In CDF message object model, you can use two method to locate a field: By name or by index.
  
  Only structure fields support locate a child field by its name, and you must provide the full name of the child
  field to prevent ambiguous.
  
  Both structure and array support locate a child field by its index. And to array, this is the only way.
  
  Samples:
   
	The message is like this:<br>
	\code
root-+-CNT(type = simple, value = 100)
     |
     +-CNA(type = array, count = 4)
     |  |
     |  +-(value =1)
     |  |
     |  +-(value =2)
     |  |
     |  +-(value =3)
     |  |
     |  +-(value =4)
     |
     +-DDX(type = structure)
     |  |
     |  +-SI1(type = simple, value = 2)	
     |  |
     |  +-SI2(type = simple, value =3)	
     |
     +-SI1(type = simple, value =30)
     |
     +-SI2(type = simple, value = 39)
	\endcode
		
    Each lines below can get the SI1 node in root :
	\code
		CCDField& node = root["SI1"];
		node = root[3];
	\endcode
	Each lines below can get the SI1 node in DDX structure:
	\code
		node = root["DDX.SI"];
		node = root["DDX"]["SI"];
		node = root["DDX"][0];
		node = root[2][0]; // the index is zero based.
		node = root[2]["SI"];
	\endcode

  Each lines below can get the first element of CNA array:
  \code
		node = root["CNA"][0];
		node = root["CNA.#0"];If you want to use the index, please use # before the digit.
		node = root[1][0]; // the index is zero based.
		node = root["#0.#0"]; 
  \endcode

  These codes are illegal, they will cause a C++ exception in runtime:
  \code
	CCDField& = root["NAME_NOT_EXISTED"]; // this field name are not existed.
	node = root["CNA.1"]; // Array element can not be located by name.
	node = root["CNA"]["1"];// Array element can not be located by name.
	node = root["CNA"][4];// Array index out of bound.
  \endcode

  \par How to access the values of simple fields
<br>

    we use the operator override technical to simplified the process. You can use the "="operater to
	assign the value between integer and CCDField. Also, you can convert a CCDField to a integer use type casting
	operator (int).
  \note You can only get/set the value of a simple field, or it will cause a c++ exception in runtime.
  
  Sample:

  The message is like this:<br>
	\code
root-+-CNT(type = simple, value = 100)
     |
     +-CNA(type = array, count = 4)
     |  |
     |  +-(value =1)
     |  |
     |  +-(value =2)
     |
     +-DDX(type = structure)
        |
        +-SI1(type = simple, value = 2)	
        |
        +-SI2(type = simple, value =3)	

		 
	\endcode
	Each lines below can set/get the value of CNT field:
	\code
		root["CNT"]=20; 
		int cnt_value = root["CNT"]; 
	\endcode
	Each lines below can set/get the value of CNA's first element:
	\code
		root["CNA"][0] = 23; 
		int cnt_value = root["CNA"][0];
	\endcode
  Each lines below can cast a simple CCDField to a integer type:
  \code
		printf("The value of the field is %d",(int)root["CNA"]);
  \endcode
  Thes codes are illegal, they will cause C++ exception in runtime:
  \code
	root["CNA"] =20; // CNA is a Array, only simple field 
                     //can be access.
	root["DDX"] =1; // DDX is a structure, only simple field
                    //can be access.
	printf("%D",(int)rort["CNA"]); // CNA is a Array, only 
                                //simple field can be casted.
	printf("%D",(int)rort["DDX"]); // DDX is a Array, only 
                                //simple field can be casted.
	root["CNA"][4] =100; // Array index out of bound.
  \endcode
  \par How to get the children count of compound fields and enumerate each children of it recursively
<br>

  You can use GetCount()to get the count of direct children of compound fields and use
  a "for loop" to enumerate the children.

  \note Use GetCount() on a simple field will cause a C++ exception in runtime.

  sample:

  This function will enumerate the tree and print the name and value of all the fields:
  \code
void EnumNode(const CCDField& Parent)
{
  int cnt = Parent.GetCount();
  for(int i=0;i<cnt;i++)
  {
    CCDField& child = Parent[i];
    switch(child.GetType())
    {
    case CDV_SIMPLE:{
         printf("%s=%d",child.GetName(),(int)child);
         break;
      }
    case CDV_ARRAY:{
         printf("Array Name =%s,count = %d",
                 child.GetName(),
                 child.GetCount()
                 );
         EnumNode(child);
         break;
      }
    case CDV_STRUCTURE:{
         printf("Structure Name =%s",
                child.GetName()
                );
         EnumNode(child);
         break;
      }	
    }
  }
}
  \endcode
  \par How to manipulate children of compound fields
  <br>

  You can use Append() function to add a sub field to structure fields or add a 
  element to a array field. You can use Delete() function to delete a sub field from
  strucuture fields or delete an element from array fields.
  
  Sample:

  The original message is like this:<br>
	\code
root-+-CNT(type = simple, value = 100)
     |
     +-CNA(type = array, count = 4)
     |  |
     |  +-(value =1)
     |  |
     |  +-(value =2)
     |
     +-DDX(type = structure)
        |
        +-SI1(type = simple, value = 2)	
        |
        +-SI2(type = simple, value =3)	
		 
	\endcode
    These codes will append fields to DDX:
    \code
    CCDField node;
    CCDField& = root["DDX"].Append(CDV_SIMPLE,"SI3"); // Add a simple field named SI3
                                                      //to the DDX structure.
    node = 3; // Set the new added field's value to 3;
    node = root["DDX"].Append(CDV_ARRAY,"SI4");// Add a sub array named SI4 to the 
                                               //DDX structure.
    node.SetElementType(DCV_SIMPLE);  // Set the type of the array element. 
    node.Append(10);
    node.Append(20);

    node = root["DDX"].Append(CDV_STRUCTURE,"SI5");// add a sub structure named 
                                                   // SI5 to the DDX structure. 
    node.Append(CDV_SIMPLE,"SI51",0);  // This is a convenient method to add a 
                                        //simple field.
\endcode
    After executing, the DDX structure is like this:
\code
root-+- ...
     |
     |  ...
     |
     +-DDX(type = structure)
        |
        +-SI1(type = simple, value = 2)	
        |
        +-SI2(type = simple, value =3)
        |
        +-SI3(type = simple, value =3)
        |
        +-SI4(type = array, count =2)
        |   |
        |   +-(value = 10)
        |   |
        |   +-(value = 20)
        |
        +-SI5(type = structure)
           |
           +-SI51(type = simple, value =0)
    \endcode
    These codes will delete fields from DDX:
    \code
    Root["DDX"].Delete(0);// delete the first field,SI1;
    Root["DDX"].Delete("SI2"); // delete SI2;
    Root["DDX.SI4"].Delete(1); // delete the second element of DDX.SI4
\endcode
After executing, the DDX structure is like this:
\code
root-+- ...
     |
     |  ...
     |
     +-DDX(type = structure)
        |
        +-SI3(type = simple, value =3)
        |
        +-SI4(type = array, count =2)
        |   |
        |   +-(value = 10)
        |
        +-SI5(type = structure)
           |
           +-SI51(type = simple, value =0)
    
    \endcode

    These codes are illegal, they will cause c++ exception in runtime:
    \code
        This code will be added later.
    \endcode

*/ 
class CCDField{
protected:
	 //! This pervent the default assign operator
    CCDField& operator=(const CCDField& /*rhs*/){}
protected:
    string m_szName;
	CDF_Field_Type m_Type,m_ElementType;
    CCDField* m_Parent;
	union{
	        int m_Value;
		    vector<CCDField*> * m_pChildren;	
        };
	//! A UsrData, not used by this class, you can stored everything in it.
	unsigned int m_uiUsrData;
	//! The level of the node
	int m_iLevel;
    //! Protected constructor.
    CCDField(CCDField* Parent,CDF_Field_Type Type);
    
public:
    //! Destructor.
	virtual ~CCDField();
    /**@name Attrubute manipulative methods
    */
//@{
    //! Get the name of the field.
    const char* GetName();
    //! Get the value of the field.
    int GetValue();
	//! Get the Parent of the field.
	//! Set the value of the field.
	void SetValue(int iValue){operator=(iValue);}
    CCDField* GetParent();
    //! Set the name of the field.
    void SetName(const char* szName);
    //! Get the type of the field
    CDF_Field_Type GetType();
    //! Get the children count of the field.
    int GetCount();
    //! Set the element type of the array field.
    void SetElementType(CDF_Field_Type ElementType);
    //! Get the element type of the array field.
    CDF_Field_Type GetElementType();
    //! Get the child field's index.
    int IndexOf(CCDField & Child);

	/*==============================================================
	The functions below is added by caesar in 2001-11-1
	===============================================================*/

	//! Get the level of the currrent field. The outmost level is 0.
	int GetLevel(){return m_iLevel;}

	//! Set a unsigned integer value to the class. 
	/*!
		User data is not used by CCDField, it is provided for the convenience only.
		You can set whatever you want and get it using GetUsrData() later.
	*/
	void SetUsrData(unsigned int uiUsrData){m_uiUsrData = uiUsrData;}
	//! Get the value seted by SetUsrData()
	int GetUsrData(){return m_uiUsrData;}
     
//@}
/** @name Overrided operators
CCDField has overrided some operators to simplify the data manipulative process.  
 */
//@{
    //! Get a child field by its name.
    CCDField& operator[](const char* szName);
    //! Get a child field by its index
    CCDField& operator[](int Index);
    //! Convert a simple field to an integer value
    operator int() ;
    //! Assign an integer value to a simpel field.
    CCDField& operator=(int i);
    //! Determine whether two fields and their children are identical.
    bool operator ==(const CCDField& rhs);
	//! Determine whether two fields and their children are not equal.
	bool operator !=(const CCDField& rhs){return !(operator==(rhs));}

    //@}
/**@name Hierarchy manipultive methods
    
*/
    //@{
       
    //! Create a new field and insert it into the location indicated by index. 
    CCDField* Insert(int Index,CDF_Field_Type Type,const char* szName);
    //! Create a new Simple field and insert it into the location indicated by index. The value of the
    //! inserted field is specified by iValue.
    CCDField* Insert(int Index,const char* szName,int iValue);
    //! Create a new array element and insert it to the location indicated by index. This function
    //! can only works on array fields.
    CCDField* Insert(int Index);
    //! Create a new array element and insert it to the location indicated by index. The Value of
    //! the element is specified by iValue. This function can only add simple element to the array fields 
    CCDField* Insert(int Index,int iValue);
    
    //! Same as Insert(GetCount(),Type,szName);
    /*!
       Please see the document for this function:<br>
         Insert(int Index,CDF_Field_Type Type,const char* szName);
    */
    CCDField* Append(CDF_Field_Type Type,const char* szName)
    {
        return Insert(GetCount(),Type,szName);
    }
    //! Same as Insert(GetCount(),szName,iValue);
     /*!
       Please see the document for this function:<br>
         Insert(int Index,const char* szName,int iValue);
    */
    CCDField* Append(const char* szName,int iValue)
    {
        return Insert(GetCount(),szName,iValue);
    }
    //! Same as Insert(GetCount());
    /*!
       Please see the document for this function:<br>
         Insert(int Index);
    */
    CCDField* Append()
    {
        return Insert(GetCount());
    }
    //! Same as Insert(GetCount(),iValue);
    /*!
       Please see the document for this function:<br>
       Insert(int Index,int iValue);
    */
    CCDField* Append(int iValue)
    {
        return Insert(GetCount(),iValue);
    }
    //! Delete a child field by its index.
    void Delete(int Index);
    //! Delete a child field ty its name.
    void Delete(const char*szName);
    
    //! Delete all the children fields.
    void Clear();
    //@}
    /**@name Utilities methods
    */
    //@{
    //! Determine whether a direct child field with this name exists.
    bool FieldExists(const char * szName);
    //! Check whether the given name is unqiue in the current structure scope.
    void CheckUnique(const char * szName);
    //@}
    };
//!CCDFMessage represents a message.
/*!
    In dynamic mode, the output of the decode engine is a tree. We use CCDFMessage to represent the
    message(or the root field of the message). CCDFMessage is derived from CCDField, so it has all the attributes and 
    methods that CCDField has. CCDFMessage extends the CCDField in these aspects:
    <ul>
    <li> The constructor of CCDFMessge is public, you can create instance of it in you codes.
    <li> It has a m_icode attribute, you can use GetCode() method to get it.
    </ul>
    
    When you create a instance of CCDFMessage, you have to tell the constructor the type of the message, this information
    is passed by the iCode parameter of the constructor. In order to prevent the term 'type' from confusing with Field type
    in CCDField, we use 'code' insteads of 'type' to represent the type of message. The decode/encode engine depends 
    on the message's code to determine what type of the message it should decode(or encode). The code should correspond 
    to the message channel and type in CDF script. In CDF script, channel and type properties are  used to identify the 
    message, they are 16bit integer. The code of CCDFMessage is a 32bit integer, so we can use this formula to transform 
    between them:
    code = (int)channel << 16 + type.

    You can not change the code of a CCDFMessage after it is constructed.
    \par Samples
    1. Decode a specific message. (pseudo code)
    \code
        BitBuffer = GetMessageFromChannelServer(...);//read the bit message from the channel server.
        int MessageType = GetTypeFromBuffer(BitBuffer);//detemine the message type from the first some bytes.
        CCDFMessage Msg(MessageType);
        try
        {
            // Assume that DynamicMsgTransfer is a dynamic mode decode/encode engine
            DynamicMsgTransfer.Decode(BitBuffer,Msg);

            //Decode succeeded! Now you can access the content of the Msg.
        }
        catch()
        {
            //Decode failed!
            
        }
        

    \endcode
    2. Encode a specific message.(pseudo code)
    \code
        CCDFMessage Msg(MessageType);
        FillTheMessage(Msg); // Fill the content of the message.
        try
        {
            // Assume that DynamicMsgTransfer is a dynamic mode decode/encode engine
            DynamicMsgTransfer.Encode(Msg,BitBuffer);
            //Encode succeeded!
            SendMessageToChannelServer(BitBuffer); // Send the encoded bit buffer to channel server.
        }
        catch()
        {
            //Encode failed!
            
        }
        
    \endcode
*/
    class CCDFMessage:public CCDField
    {
    private:
        int m_iCode;
    public:
        //! Constructor 
        CCDFMessage(int iCode);
        //!Get the code of the message.
        /*!
            \return The code of the message.
            
            This function return the code of the message. To each message, their codes are unique. You should set code in
            the constructor of the message and it can not be modified then.

            The code of CCDFMessage corresponds to the channel and type property of the message in CDF script. You can use
            this formula to transform between them: code =(int)channel<<16 + type.
        */
        int GetCode(){return m_iCode;}
    };
    
// global routine and operators
#endif
