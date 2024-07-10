#ifdef WRE_SUPPORT

//#include "sci_types.h"
#include "mmk_app.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmipdp_export.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "watchdog.h"
#include "mmiwre_charset.h"
#include "../../../../../../wre/wrekernel//include/sdk/connect_type.h"
#include "../../../../../../wre/wrekernel/include/sdk/videoplayer.h"
#include "mmiwre_dial.h"
#include "mmiwre_font.h"
#include "mmiwre_gdi.h"
#include "mmiwre_image.h"
#include "mmiwre_ime.h"
//#include "adaptor_mdu.h"
#include "mmiwre_media.h"
#include "mmiwre_other.h"
#include "mmiwre_thread.h"
#include "mmiwre_file.h"
#include "mmiwre_sema.h"
#include "mmiwre_socket.h"
#include "mmiwre_timer.h"
#include "mmiwre_tapi.h"
#include "mmiwre_layer.h"
#include "mmiwre_adaptor.h"
#include "mmiwre_pb.h"
#include "ctype.h"
#include "stdio.h"

//extern void * SetSubScreenBuffer();
//extern BOOLEAN MMK_IsMocorAppTask(void);
#ifdef WIN32
extern void find_first_file(void* lpFileName,void *lpFindData);
extern BOOL find_next_file(void* hFindFile,void *lpFindFileData);
extern BOOL find_close(void* hFindFile);
#endif
extern  int adapt_sock_asyncrecvfrom(int so, char *buf, int len, int flag, void *from, int *left_len_ptr);
uint32 Adapt_ActiveTimer(void * timer_ptr);
#ifdef WIN32
extern void* Get_Module_File_Name(void *hModule,void* *lpFilename,DWORD nSize);
extern void* Get_Module_File_NameEx(void* hProcess,void *hModule,void* *lpFilename,DWORD nSize);
extern  void* Get_Current_Process(void*);
extern void* Get_Module_Handle(void* *lpModuleName);
#endif
extern BOOL IsNetworkOk(void);
extern int strnicmp(const char *s1, const char *s2, size_t n);
extern void wre_kernel_pad1(void);
extern void wre_kernel_pad2(void);
extern void wre_kernel_pad3(void);
extern void wre_kernel_pad4(void);
extern void wre_kernel_pad5(void);
extern void wre_kernel_pad6(void);
extern void wre_kernel_pad7(void);
extern void wre_kernel_pad8(void);
extern void wre_kernel_pad9(void);
extern void wre_kernel_pad10(void);
extern void wre_kernel_pad11(void);
extern void wre_kernel_pad12(void);
extern void wre_kernel_pad13(void);
extern void wre_kernel_pad14(void);
extern void wre_kernel_pad15(void);
extern void wre_kernel_pad16(void);
extern void *wre_kernel_hook_proc(int index);
extern void boot_reboot(int  code );
extern int WRE_Is_Module_OK(int m);
extern void WreInitForAssert(void);
extern void WreAddDll(char* ap_name_ptr,uint32 uID,uint32 ro_start,uint32 ro_end,uint32 rw_start,uint32 rw_end);
//extern void WreAddDll(char* ap_name_ptr,uint32 ro_start,uint32 ro_end,uint32 rw_start,uint32 rw_end);
extern void WreDelDll(char* ap_name_ptr,uint32 ro_start,uint32 ro_end,uint32 rw_start,uint32 rw_end);
void Get_Last_Error(void){}
extern char* GetBOOTBuild(void);
extern void WRE_GetPLVersion(VERSION_T * version);
extern BOOLEAN _SetRingInfo(void *param);
extern BOOLEAN _SetPictureToWallpaper(void *param);
extern BOOL  WRE_RotateLCD(int angle,int  * out );
extern void boot_Invliad_arm_cache();
extern void MMIWRE_Shell_NotifyIcon(DWORD dwMessage, void* lpData);
extern void BOOT_MessageBox(int ,void*);
extern void *Wre_CreateDevice(uint16 *deviceName_ptr);
extern void Wre_DestroyDevice(void *device_ptr, BOOLEAN CFlag);
extern BOOLEAN Wre_ReadDevice(void *device_ptr, void *buffer, unsigned int count, unsigned int *poutcnt);
extern BOOLEAN Wre_WriteDevice(void *device_ptr, void *buffer, unsigned int count, unsigned int *poutcnt);
extern BOOLEAN Wre_DeviceIoControl(
    void   *device_ptr,
    uint32 IoControlCode,
    void   *InBuffer_ptr,
    uint32 nInBufferSize,
    void   *OutBuffer_ptr,
    uint32 nOutBufferSize,
    uint32 *nBytesReturned,
    uint32 *BytesReturned_ptr);

int wboot_wre_thumb_abs( int n )
{
	return abs(n);
}

unsigned long int wboot_wre_thumb_strtoul( const char *nptr, char **endptr, int base )
{
	return strtoul(nptr, endptr, base);
}

void wboot_wre_thumb_qsort( void *base, size_t num, size_t width, int (*compare)(const void *elem1, const void *elem2 ) )
{
	qsort(base, num, width, compare);
}

long wboot_wre_thumb_strtol( const char *nptr, char **endptr, int base )
{
    return strtol( nptr, endptr, base );
}
int wboot_wre_thumb_atoi(const char *nptr)
{
    return atoi(nptr);
}
int wboot_wre_thumb_atol(const char *nptr )
{
    return atol(nptr);
}
size_t wboot_wre_thumb_strxfrm ( char *strDest, const char *strSource, size_t count )
{
    return strxfrm ( strDest, strSource, count );
}

int wboot_wre_thumb_strcoll( const char *string1, const char *string2 )
{
    return strcoll( string1, string2 );  
}
void wboot_wre_thumb_srand( unsigned int seed )
{
    srand( seed );
}
int wboot_wre_thumb_rand( void )
{
    return rand();
}
void *wboot_wre_thumb_memchr( const void *buf, int c, size_t count )
{
    return memchr( buf, c, count );  
}
void *wboot_wre_thumb_memset(void *dest, int c, size_t count )
{
    return memset(dest, c, count );   
}

void *wboot_wre_thumb_memmove(void *dest, const void *src, size_t count)
{
    return memmove(dest, src, count);
}
int wboot_wre_thumb_strnicmp( const char *string1, const char *string2, size_t count )
{
    return strnicmp( string1, string2, count );
}
char *wboot_wre_thumb_strncpy( char *strDest, const char *strSource, size_t count )
{
    return strncpy( strDest, strSource, count );   
}
char *wboot_wre_thumb_strerror( int errnum )
{
    return strerror( errnum );
}
int wboot_wre_thumb_isalpha( int c )
{
    return isalpha( c );    /*lint !e718 !e48*/
}

int wboot_wre_thumb_isalnum( int c )
{
    return isalnum( c );    /*lint !e48*/
}

int wboot_wre_thumb_toupper( int c )
{
    return toupper( c );
}
int wboot_wre_thumb_tolower( int c )
{
    return tolower( c );
}

size_t wboot_wre_thumb_strlen( const char *string )
{
    return strlen( string );
}

char *wboot_wre_thumb_strcpy( char *strDestination, const char *strSource )
{
    return strcpy( strDestination, strSource );
}

int wboot_wre_thumb_isxdigit( int c )
{
    return isxdigit( c );   /*lint !e48*/
}

int wboot_wre_thumb_isupper( int c )
{
    return isupper( c );   /*lint !e48*/
}
int wboot_wre_thumb_isspace( int c )
{
    return isspace( c );   /*lint !e48*/
}
int wboot_wre_thumb_ispunct( int c )
{
    return ispunct( c );   /*lint !e48*/
}
int wboot_wre_thumb_isprint( int c )
{
    return isprint( c );   /*lint !e48*/
}
int wboot_wre_thumb_islower( int c )
{
    return islower( c );   /*lint !e48*/
}

int wboot_wre_thumb_isgraph( int c )
{
    return isgraph( c );   /*lint !e48*/
}

int wboot_wre_thumb_isdigit( int c )
{
    return isdigit( c );   /*lint !e48*/
}

int wboot_wre_thumb_iscntrl( int c )
{
    return iscntrl( c );   /*lint !e48*/
}

void *wboot_wre_thumb_memcpy(void *dest, const void *src, size_t count)
{
    return memcpy(dest, src, count);
}
int wboot_wre_thumb_sscanf(const char *a, const char *b, ...)
{
    return 0;
}
int wboot_wre_thumb_vsprintf(char *buf, const char *fmt, va_list va)
{
    return 0;
}
int wboot_wre_thumb_vsnprintf(char *buf, size_t size, const char *fmt, va_list va)
{
    return 0;
}
int wboot_wre_thumb_memcmp( const void *buf1, const void *buf2, size_t count )
{
    return memcmp( buf1, buf2, count );
}
int wboot_wre_thumb_sprintf(char *a, const char *fmt, ...)
{
    return 0;
}
int wboot_wre_thumb_strcmp(const char *str1, const char *str2)
{
    return strcmp(str1, str2);
}
void wboot_wre_thumb_SCI_Assert(
						  void    *exp,     // Expression (including pointers) that evaluates                       // to nonzero or 0
						  void    *file,    // File name where assert
						  uint32  line      // Line number where assert
						  )
{
	SCI_Assert(exp,    file,   line      );/*assert verified*/
}

char *wboot_wre_thumb_strtok(char * s1, const char * s2){
    return strtok(s1,s2);
}
char *wboot_wre_thumb_strstr(const char * s1, const char * s2){
    return strstr(s1,s2);
}
int wboot_wre_thumb_strspn(const char * s1, const char * s2){
    return strspn(s1,s2);
}
char *wboot_wre_thumb_strrchr(const char * s, int c){
    return strrchr(s,c);
}
char *wboot_wre_thumb_strpbrk(const char * s1, const char * s2){
    return strpbrk(s1,s2);
}

int wboot_wre_thumb_strcspn(const char * s1, const char * s2){
    return strcspn(s1,s2);
}
char *wboot_wre_thumb_strchr(const char * s, int c){
    return strchr(s,c);
}
int wboot_wre_thumb_strncmp(const char * s1, const char * s2, int n){
    return strncmp(s1,s2,n);
}
char *wboot_wre_thumb_strncat(char * s1, const char * s2, int n){
    return strncat(s1,s2,n);
}
char *wboot_wre_thumb_strcat(char * s1, const char * s2){
    return strcat(s1,s2);
}


uint32 wboot_wre_thumb_SCI_GetTickCount(void)
{
    return SCI_GetTickCount();
}



void wboot_wre_thumb_SCI_RestoreIRQ(void)
{
    SCI_RestoreIRQ();
}


void wboot_wre_thumb_SCI_DisableIRQ(void)
{
    SCI_DisableIRQ();
}

size_t wboot_wre_thumb_MMIAPICOM_Wstrlen( const wchar* str )
{
	return  MMIAPICOM_Wstrlen(str);
	
}

wchar* wboot_wre_thumb_MMIAPICOM_StrToWstr(
									 const uint8* src,
									 wchar* dst
									 )
{
	return MMIAPICOM_StrToWstr( src, dst );
}


void *wboot_wre_thumb_SCI_MallocApp(
							  uint32         mem_len,
							  const char     *file_ptr, 
							  uint32         line
							  )
{
	return SCI_MallocApp(mem_len,file_ptr,line);
}
void wboot_wre_thumb_SCI_Free(void *mem_ptr)
{
  	SCI_Free(mem_ptr);
}

wchar* wboot_wre_thumb_MMIAPICOM_Wstrncat( 
									wchar* dst,
									const wchar* src,
									size_t count
									)
{
	return  MMIAPICOM_Wstrncat( 
		dst,
		src,
		count
		);
}

wchar* wboot_wre_thumb_MMIAPICOM_Wstrcat( 
								   wchar* dst,
								   const wchar* src
								   )
{
	return MMIAPICOM_Wstrcat(  dst, src );
}

wchar* wboot_wre_thumb_MMIAPICOM_Wstrncpy( 
									wchar* dst,
									const wchar* src,
									size_t count
									) 
{
	return MMIAPICOM_Wstrncpy(  dst, src, count) ;
}

wchar* wboot_wre_thumb_MMIAPICOM_Wstrcpy( 
								   wchar* dst,
								   const wchar* src
								   )
{
	return  MMIAPICOM_Wstrcpy( dst, src );
}


#include "boot_stub.def"

#endif
