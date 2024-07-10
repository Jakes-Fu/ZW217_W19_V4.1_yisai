/////////////////////////////////////////////////////////////////////////////
// ErrorToString.cpp : implementation file
//

#include "stdafx.h"
#include "ErrorToString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString ErrorToString(int status)
{
	CString s;
	switch (status)
	{
    case E_SUCCEED:
        s = _T("SUCCEED");
        break;
    // BMOpr error code (1+~4999)

    // Common error code (5000~9999)
    case ERR_INSUFFICIENT_MEMORY://     = 5000,              /* Insufficient memory */
        s = _T("Insufficient memory. ");
        break;
	case ERR_INVALIDE_PARAMETERS://     = 5001,              /* Invalid parameters */
        s = _T("Invalid parameters. ");
        break;
	case ERR_LOAD_FILE:			//     = 5002,              /* Failure to load file */
        s = _T("Failure to load file. ");
        break;
	case ERR_OPEN_FILE:			//     = 5003,              /* Failure to open file */
        s = _T("Failure to open file. ");
        break;
	case ERR_READ_FILE:			//     = 5004,              /* Failure to read file */
        s = _T("Failure to read file. ");
        break;
	case ERR_GET_TEMP_PATH:		//     = 5005,              /* Failure to get temp path */
        s = _T("Failure to get temp path. ");
        break;
	case ERR_CREATE_DIRECTORY:		//     = 5006,          /* Failure to create directory */
        s = _T("Failure to create directory. ");
        break;
	case ERR_CREAT_FILE:			//     = 5007,          /* Failure to create file */
        s = _T("Failure to create file. ");
        break;
	case ERR_WRITE_FILE:			//     = 5008,          /* Failure to write file */
        s = _T("Failure to write file. ");
        break;
	case ERR_SEEK_FILE:				//	   = 5009,             /* Failure to seek file */
		s = _T("Failure to seek file. ");
		break;
	case ERR_OPEN_ORG_PAC:		//			= 5010,             /* Failure to open original pac */
		s = _T("Failure to open original pac. ");
		break;
		
        // Pac opr error code (10000+ ~10999) 
    case ERR_LOAD_PACKET_FAIL: //        = 10000,            /* Failure to LoadPacket  */
        s = _T("Failure to LoadPacket. ");
        break;
	case ERR_CHECK_PAC_SIZE_FAIL: //       = 10001,          /* Bin packet's size is too small,maybe it has been destructed!  */
        s = _T("Bin packet's size is too small,maybe it has been destructed! ");
        break;

	case ERR_PAC_CRC_FAIL:			//       = 10002,        /* CRC Error! PAC file may be damaged!  */
        s = _T("CRC Error! PAC file may be damaged! ");
        break;	

	case ERR_UNSUPPORT_PAC_VERSION:	//       = 10003,        /* Bin packet version is not support!  */
        s = _T("Bin packet version is not support! ");
        break;	

	case ERR_INVALID_PAC_SIZE:	//       = 10004,			/* Bin packet's size is not correct,maybe it has been destructed!  */
        s = _T("Bin packet's size is not correct,maybe it has been destructed! ");
        break;

	case ERR_NOT_FIND_FILES_IN_PAC:	//      = 10005,		/* There is no files in packet!  */
        s = _T("There is no files in packet! ");
        break;
	case ERR_READ_PAC_FAIL:			//      = 10006,		/* Read pakcet failed,maybe it has been destructed!  */
        s = _T("Read pakcet failed,maybe it has been destructed! ");
        break;
	case ERR_PARSE_PAC_INFO_FAIL:	//      = 10007,		/* Failure to parse pac file info.  */
        s = _T("Failure to parse pac file info. ");
        break;


		// Encrypt/Decrypt error code [12000+ ~12999) 
	case ERR_SAFE_ENCRYPT_FAIL: //        = 12000,            /* Failure to encrypt  */
        s = _T("Failure to encrypt. ");
        break;
    case ERR_SAFE_DECRYPT_FAIL: //       = 12001,             /* Failure to decrypt  */
        s = _T("Failure to decrypt. ");
        break;
    case ERR_GET_ENCRYPTED_SIZE_FAIL:   //=12002,            /* Failure to get encrypted size  */
        s = _T("Failure to get encrypted size. ");
        break;
    case ERR_INVALID_DECRYPTED_DATA:    //= 12003,            /* Invalid decrypted data  */
        s = _T("Invalid decrypted data.maybe is password error.");
        break;

	default:
        s = _T("SecPac undefined error. ");
		break;
	}
	CString t;
	t.Format(_T(" (Error code = %d)"), status);
	s = s + t;
	return s;
}
