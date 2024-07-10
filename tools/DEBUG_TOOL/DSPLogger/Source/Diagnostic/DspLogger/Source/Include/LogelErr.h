//Message
//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: LOGEL_S_OK
//
// MessageText:
//
//  No error.
//
#define LOGEL_S_OK                         ((DWORD)0x00000000L)

//
// MessageId: LOGEL_S_FALSE
//
// MessageText:
//
//  S_FALSE.
//
#define LOGEL_S_FALSE                      ((DWORD)0x00000001L)

//
// MessageId: LOGELMO_S_RESET_TO_DEFAULT
//
// MessageText:
//
//  Reset to the default vaule.
//
#define LOGELMO_S_RESET_TO_DEFAULT         ((DWORD)0x00000002L)

//
// MessageId: LOGEL_S_NEED_SHOW_HIDE_CONSOLE_WINDOW
//
// MessageText:
//
//  show/hide console window//
#define LOGEL_S_NEED_SHOW_HIDE_CONSOLE_WINDOW   ((DWORD)0x00000003L)

//General LOGEL Error Message
//
// MessageId: LOGEL_E_FAILED
//
// MessageText:
//
//  Generic failure.
//
#define LOGEL_E_FAILED                     ((DWORD)0x80046001L)

//
// MessageId: LOGEL_E_UNEXPECTED
//
// MessageText:
//
//  Unexpected.
//
#define LOGEL_E_UNEXPECTED                 ((DWORD)0x80046002L)

//
// MessageId: LOGEL_E_OUTOFMEMORY
//
// MessageText:
//
//  Out of memory.
//
#define LOGEL_E_OUTOFMEMORY                ((DWORD)0x80046003L)

//
// MessageId: LOGEL_E_NOINTERFACE
//
// MessageText:
//
//  No such interface.
//
#define LOGEL_E_NOINTERFACE                ((DWORD)0x80046004L)

//
// MessageId: LOGEL_E_INVALIDARG
//
// MessageText:
//
//  The passed in arguments are invalid.
//
#define LOGEL_E_INVALIDARG                 ((DWORD)0x80046005L)

//
// MessageId: LOGEL_E_NOTIMPL
//
// MessageText:
//
//  The function is not implemented currently.
//
#define LOGEL_E_NOTIMPL                    ((DWORD)0x80046006L)

//
// MessageId: LOGEL_E_OUTOFRANGE
//
// MessageText:
//
//  The specified index is out of range.
//
#define LOGEL_E_OUTOFRANGE                 ((DWORD)0x80046007L)

//
// MessageId: LOGEL_E_INVALID_OPERATION
//
// MessageText:
//
//  Invalid operation.
//
#define LOGEL_E_INVALID_OPERATION          ((DWORD)0x80046008L)

//
// MessageId: LOGEL_E_TOOLBAR_NOT_FOUND
//
// MessageText:
//
//  The specified toolbar is not found.
//
#define LOGEL_E_TOOLBAR_NOT_FOUND          ((DWORD)0x80046009L)

//
// MessageId: LOGEL_E_TOO_MANY_TOOLBARS
//
// MessageText:
//
//  Too many toolbars created.
//
#define LOGEL_E_TOO_MANY_TOOLBARS          ((DWORD)0x8004600AL)

//
// MessageId: LOGEL_E_TOO_MANY_COMMANDS
//
// MessageText:
//
//  Too many commands added.
//
#define LOGEL_E_TOO_MANY_COMMANDS          ((DWORD)0x8004600BL)

//
// MessageId: LOGEL_E_MENU_NOT_FOUND
//
// MessageText:
//
//  The specified menu is not found.
//
#define LOGEL_E_MENU_NOT_FOUND             ((DWORD)0x8004600CL)

//
// MessageId: LOGEL_E_COMMAND_NOT_FOUND
//
// MessageText:
//
//  The specified command is not found.
//
#define LOGEL_E_COMMAND_NOT_FOUND          ((DWORD)0x8004600DL)

//
// MessageId: LOGEL_E_COMMAND_ALREADY_EXISTS
//
// MessageText:
//
//  The specified command already exists.
//
#define LOGEL_E_COMMAND_ALREADY_EXISTS     ((DWORD)0x8004600EL)

//
// MessageId: LOGEL_E_DEFAULT_MATCH
//
// MessageText:
//
//  Use the default search method.
//
#define LOGEL_E_DEFAULT_MATCH              ((DWORD)0x8004600FL)

//
// MessageId: LOGEL_E_VIEWENTRY_NOT_FOUND
//
// MessageText:
//
//  The specified management view entry is not found.
//
#define LOGEL_E_VIEWENTRY_NOT_FOUND        ((DWORD)0x80046010L)

//
// MessageId: LOGEL_E_VIEWENTRY_ALREADY_EXISTS
//
// MessageText:
//
//  The specified management view entry already exists.
//
#define LOGEL_E_VIEWENTRY_ALREADY_EXISTS   ((DWORD)0x80046011L)

//
// MessageId: LOGEL_E_REFUSE_CLOSE
//
// MessageText:
//
//  Refusing to be closed
//
#define LOGEL_E_REFUSE_CLOSE               ((DWORD)0x80046012L)

//
// MessageId: LOGEL_E_OUTOFLICENCE
//
// MessageText:
//
//  Out of licence.
//
#define LOGEL_E_OUTOFLICENCE               ((DWORD)0x80046013L)

//Managed Object Services Error
//
// MessageId: LOGELMO_E_OBJECT_NOT_FOUND
//
// MessageText:
//
//  The specified managed object can't be found.
//
#define LOGELMO_E_OBJECT_NOT_FOUND         ((DWORD)0x80047010L)

//
// MessageId: LOGELMO_E_OBJECT_ALREADY_EXISTS
//
// MessageText:
//
//  The specified managed object already exists.
//
#define LOGELMO_E_OBJECT_ALREADY_EXISTS    ((DWORD)0x80047011L)

//
// MessageId: LOGELMO_E_CLSDECL_NOT_FOUND
//
// MessageText:
//
//  The specified class declaration can't be found.
//
#define LOGELMO_E_CLSDECL_NOT_FOUND        ((DWORD)0x80047012L)

//
// MessageId: LOGELMO_E_CLSDECL_ALREADY_EXISTS
//
// MessageText:
//
//  The specified class declaration already exists.
//
#define LOGELMO_E_CLSDECL_ALREADY_EXISTS   ((DWORD)0x80047013L)

//
// MessageId: LOGELMO_E_PROPDECL_NOT_FOUND
//
// MessageText:
//
//  The specified property declaration can't be found.
//
#define LOGELMO_E_PROPDECL_NOT_FOUND       ((DWORD)0x80047014L)

//
// MessageId: LOGELMO_E_PROPDECL_ALREADY_EXISTS
//
// MessageText:
//
//  The specified property declaration already exists.
//
#define LOGELMO_E_PROPDECL_ALREADY_EXISTS  ((DWORD)0x80047015L)

//
// MessageId: LOGELMO_E_QUALIFIER_NOT_FOUND
//
// MessageText:
//
//  The specified qualifier can't be found.
//
#define LOGELMO_E_QUALIFIER_NOT_FOUND      ((DWORD)0x80047016L)

//
// MessageId: LOGELMO_E_PROPERTY_NOT_FOUND
//
// MessageText:
//
//  The specified property can't be found.
//
#define LOGELMO_E_PROPERTY_NOT_FOUND       ((DWORD)0x80047017L)

//
// MessageId: LOGELMO_E_INVALID_PROPERTY_NAME
//
// MessageText:
//
//  Invalid property name.
//
#define LOGELMO_E_INVALID_PROPERTY_NAME    ((DWORD)0x80047018L)

//
// MessageId: LOGELMO_E_INVALID_DATA_TYPE
//
// MessageText:
//
//  Invalid data type.
//
#define LOGELMO_E_INVALID_DATA_TYPE        ((DWORD)0x80047019L)

//
// MessageId: LOGELMO_E_TYPE_MISMATCH
//
// MessageText:
//
//  Data type mismatch occurs.
//
#define LOGELMO_E_TYPE_MISMATCH            ((DWORD)0x8004701AL)

//
// MessageId: LOGELMO_E_PROPERTY_READONLY
//
// MessageText:
//
//  Property readonly.
//
#define LOGELMO_E_PROPERTY_READONLY        ((DWORD)0x8004701BL)

//
// MessageId: LOGELMO_E_QUALIFIER_READONLY
//
// MessageText:
//
//  Property readonly.
//
#define LOGELMO_E_QUALIFIER_READONLY       ((DWORD)0x8004701CL)

//
// MessageId: LOGELMO_E_CLSDECL_HAS_OBJECTS
//
// MessageText:
//
//  There are objects spawned from the class declaration.
//
#define LOGELMO_E_CLSDECL_HAS_OBJECTS      ((DWORD)0x8004701DL)

//
// MessageId: LOGELMO_E_CLSDECL_HAS_CHILDREN
//
// MessageText:
//
//  The class declaration has sub class declarations.
//
#define LOGELMO_E_CLSDECL_HAS_CHILDREN     ((DWORD)0x8004701EL)

//
// MessageId: LOGELMO_E_PROPDECL_HAS_INSTANCE
//
// MessageText:
//
//  The property declaration has been used by objects.
//
#define LOGELMO_E_PROPDECL_HAS_INSTANCE    ((DWORD)0x8004701FL)

//
// MessageId: LOGELMO_E_NOT_DERIVED_CLSDECL
//
// MessageText:
//
//  It is not a derived class declaration.
//
#define LOGELMO_E_NOT_DERIVED_CLSDECL      ((DWORD)0x80047020L)

//
// MessageId: LOGELMO_E_INCOMPLETE_CLSDECL
//
// MessageText:
//
//  Incomplete class declaration.
//
#define LOGELMO_E_INCOMPLETE_CLSDECL       ((DWORD)0x80047021L)

//
// MessageId: LOGELMO_E_INCOMPLETE_PROPDECL
//
// MessageText:
//
//  Incomplete property declaration.
//
#define LOGELMO_E_INCOMPLETE_PROPDECL      ((DWORD)0x80047022L)

//
// MessageId: LOGELMO_E_INVALID_PROPERTY
//
// MessageText:
//
//  Invalid property.
//
#define LOGELMO_E_INVALID_PROPERTY         ((DWORD)0x80047023L)

//
// MessageId: LOGELMO_E_INVALID_CLSDECL
//
// MessageText:
//
//  Invalid class declaration.
//
#define LOGELMO_E_INVALID_CLSDECL          ((DWORD)0x80047024L)

//
// MessageId: LOGELMO_E_INHERITED_QUALIFIER
//
// MessageText:
//
//  Inherited qualifier.
//
#define LOGELMO_E_INHERITED_QUALIFIER      ((DWORD)0x80047025L)

//
// MessageId: LOGELMO_E_OPERATION_CANCELLED
//
// MessageText:
//
//  The operation is cancelled.
//
#define LOGELMO_E_OPERATION_CANCELLED      ((DWORD)0x80047026L)

//
// MessageId: LOGELMO_E_PROPERTY_REFRESHER_NOT_FOUND
//
// MessageText:
//
//  The property refresher is not found or not loaded successfully.
//
#define LOGELMO_E_PROPERTY_REFRESHER_NOT_FOUND ((DWORD)0x80047027L)

//
// MessageId: LOGELMO_E_INVALID_QUERY
//
// MessageText:
//
//  The query was not syntactically valid.
//
#define LOGELMO_E_INVALID_QUERY            ((DWORD)0x80047028L)

//
// MessageId: LOGELMO_E_SEMANTICAL_INVALID_QUERY
//
// MessageText:
//
//  The query was not semantically valid.
//
#define LOGELMO_E_SEMANTICAL_INVALID_QUERY ((DWORD)0x80047029L)

