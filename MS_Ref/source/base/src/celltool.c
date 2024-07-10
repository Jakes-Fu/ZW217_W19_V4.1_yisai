
#include "comm_def.h"  
#include "ps_outport.h"

#include "os_api.h"
#include "version.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

extern int EFS_Add_Diagnostic_Info(const uint8 *ptr, int len);

int Add_Diagnostic_Info( const uint8 *ptr, int len)
{
    return EFS_Add_Diagnostic_Info( ptr, len );
}

static const char * get_file_name( const char *file )
{
    const char * ptr = file;
static    const char null_string[] = "\0";
    
    if( NULL == file )
    {
        return null_string;
    }
    
    // to tail.
    while( *ptr ){
        ptr++;
    }
    ptr--;  // maybe the ptr < file if (*file == '\0' )
    
    while( (ptr >= file) && ('\\' != *ptr) && ('/' != *ptr) )
    {
        ptr--;
    }
    return (ptr >= file) ? ptr + 1 : file;
}


// 
//  Called when ASSERT.
//  Get Diagnostic infomation, then record it to nvitem.
//  Author: Lin.liu
//
#define MAX_DIAGNOSTIC_LEN  138
#define MAX_DIAGNOSTIC_BUF  1024
extern const char * COMMON_GetProjectVersionInfo(void);     // return software version 

void Record_Diagnostic_Info( const char * file, int line, 
    const char * exp, const char * assert_info)
{
static    char   			diag[ MAX_DIAGNOSTIC_LEN + 2 ];      // store one diagnostic record.
static    char              buf [ MAX_DIAGNOSTIC_BUF + 2 ];      // the tmp buf.

    DIAG_NET_INFO_LOG_T    *net_info = (DIAG_NET_INFO_LOG_T *) (&diag[0]);
        
    char   				   *packet = (char *)(net_info + 1 );    // the packet address.
    char                   *packet_end = &diag[ MAX_DIAGNOSTIC_LEN - 2 ];
    
    char                   *buf_start  = &buf[0];
    char   				   *buf_end    = &buf[ MAX_DIAGNOSTIC_BUF - 4];
    
    char                   *tmp_ptr;
    const char             *sw_build;
    uint32                  seq_id, seq_num;
    int     				len;
    
    
    // get net information
    // When call this, maybe will cause abort!
    GetNetInfoFromPs( net_info );    
    
    
    sw_build = COMMON_GetProjectVersionInfo();
        
    if( NULL == exp )
    {
        exp = "NULL";
    }
    
    file = get_file_name( file );
        
    // Build the assert information.
    len = sprintf( buf_start, "%s\r\nAssert:%s(%d)(%s)",
            sw_build,             // software build information
            file, line,           // assert location.
            exp );                // the express.    
    tmp_ptr = buf_start + len;
        
    // copy assert info. 
    // for assert maybe too long, so need check overflow.
    if( NULL != assert_info )
    {
        *tmp_ptr++ = '\r';
        *tmp_ptr++ = '\n';
        while( (tmp_ptr < buf_end) && (*assert_info) )
        {
            *tmp_ptr++ = *assert_info++;
        }
    }
    *tmp_ptr = '\0';
    
    // maybe need div to serveral packet.
    seq_id  = SCI_GetTickCount();
    seq_num = 0;    
    while( '\0' != *buf_start )
    {
        len     = sprintf( packet, "Seq(0x%08X--%03d)\r\n", seq_id, seq_num++ );
        tmp_ptr = packet + len;
        
        while( (tmp_ptr < packet_end) && (*buf_start) )
        {
            *tmp_ptr++ = *buf_start++;            
        }
        *tmp_ptr++ = '\0';
        
        // Wrtite to Nvitem.        
        Add_Diagnostic_Info( (uint8 *)diag, tmp_ptr- diag );
    }
}

// Delete all diagnostic information that stored in nvitem.
extern void EFS_Delete_Diagnostic_Info(void);
void Delete_Diagnostic_Info(void)
{
    EFS_Delete_Diagnostic_Info();
}


//  Get First diagnostic information.
//  Input:
//      sms_info :  the buffer that receive diagnostic information.
//                  It should be large enough,(larger that 160)
//  Return:
//      return 0 means no diagnostic infomation.(data in sms_info is invalid)
//      else return current index;
//  Author: Lin.liu
//  Note: 
//     the length stored in first byte of sms_info.
extern int EFS_Get_First_Diagnostic_Info(uint8 *sms_info);
int Get_First_Diagnostic_Info(uint8 * sms_info)
{
    return EFS_Get_First_Diagnostic_Info(sms_info);
}

// Get Next Diagnostic information
// Input: 
//    index : the value that Get_First_Diagnostic_Info or Get_Next... returned.
//    sms_info : see above( Get_First_Diagnostic_Info )
// Return: 
//    return 0 means no more. and data in sms_info is invalid
// Author: Lin.liu
// Note: 
//     the length stored in first byte of sms_info.
extern int EFS_Get_Next_Diagnostic_Info(int index, uint8 *sms_info);
int Get_Next_Diagnostic_Info(int index, uint8 *sms_info)
{
    return EFS_Get_Next_Diagnostic_Info(index, sms_info);
}

// Send the Diagnostic information to Diag Tools via serial port(or other channel)
//  Input:
//      phone_num : the phone number(which send the diagnostic information)
//      info      : diagnostic information
//  Return: 
//      If success, return 0,
//      else return the error code.
//  Author: Lin.liu
int Send_Diagnostic_Info(const uint8 *phone_num, const uint8 *info)
{
    info = info;
    return 0;
}

// used by lin.liu to test diagnstic function
void test_diag_sms(void)
{
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif