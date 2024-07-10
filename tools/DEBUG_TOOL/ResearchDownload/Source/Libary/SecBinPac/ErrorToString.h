/////////////////////////////////////////////////////////////////////////////
// ErrorToString.h : Error code definition file for Error to string translation
//

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

CString ErrorToString(int status);
typedef enum
{
    E_SUCCEED   = 0,
    // BMOpr error code [1+~4999)

    // Common error code [5000~9999)
    ERR_INSUFFICIENT_MEMORY     = 5000,             /* Insufficient memory */
	ERR_INVALIDE_PARAMETERS     = 5001,             /* Invalid parameters */
	ERR_LOAD_FILE				= 5002,             /* Failure to load file */
	ERR_OPEN_FILE				= 5003,             /* Failure to open file */
	ERR_READ_FILE				= 5004,             /* Failure to read file */
	ERR_GET_TEMP_PATH			= 5005,             /* Failure to get temp path */
	ERR_CREATE_DIRECTORY		= 5006,             /* Failure to create directory */
	ERR_CREAT_FILE				= 5007,             /* Failure to create file */
	ERR_WRITE_FILE				= 5008,             /* Failure to write file */
	ERR_SEEK_FILE				= 5009,             /* Failure to seek file */
	ERR_OPEN_ORG_PAC			= 5010,             /* Failure to open original pac */

    // Pac opr error code [10000+ ~10999) 
    ERR_LOAD_PACKET_FAIL        = 10000,            /* Failure to LoadPacket  */
	ERR_CHECK_PAC_SIZE_FAIL		= 10001,            /* Bin packet's size is too small,maybe it has been destructed!  */
	ERR_PAC_CRC_FAIL			= 10002,            /* CRC Error! PAC file may be damaged!  */
	ERR_UNSUPPORT_PAC_VERSION	= 10003,            /* Bin packet version is not support!  */
	ERR_INVALID_PAC_SIZE		= 10004,            /* Bin packet's size is not correct,maybe it has been destructed! */
	ERR_NOT_FIND_FILES_IN_PAC	= 10005,            /* There is no files in packet! */
	ERR_READ_PAC_FAIL			= 10006,            /* Read pakcet failed,maybe it has been destructed! */
	ERR_PARSE_PAC_INFO_FAIL		= 10007,            /* Failure to parse pac file info.*/


	
	

	// Encrypt/Decrypt error code [12000+ ~12999) 
    ERR_SAFE_ENCRYPT_FAIL        = 12000,            /* Failure to encrypt  */
    ERR_SAFE_DECRYPT_FAIL		 = 12001,            /* Failure to decrypt  */
    ERR_GET_ENCRYPTED_SIZE_FAIL  = 12002,            /* Failure to get encrypted size  */
    ERR_INVALID_DECRYPTED_DATA	 = 12003,            /* Invalid decrypted data  */

    E_UNDEFINE_ERR
};
