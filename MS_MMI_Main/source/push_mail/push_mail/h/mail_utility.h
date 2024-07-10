/*
* hisense pushmail
*/

#ifndef  _MAIL_UTILITY_H_    
#define  _MAIL_UTILITY_H_  

#include "mail_message.h"


#define MAX_MAIL_LENGTH (1800 + 10) * 1024

/*typedef struct FILE_INFORMATION
{
    uint32  size;				// filesize
    uint8   content[MAX_MAIL_LENGTH];		// content
	wchar   name[255];			// filename
}FILE_INFORMATION_T;*/

/*typedef struct AAA
{
    uint16  size;			
    uint8 * content;		
}AAA_T;
*/
ErrorType Read_File(wchar * fullname, MAIL_MESSAGE_T * message);

comm_uint8 * Trimmed(comm_uint8 * src, MAIL_MESSAGE_T * mailmessage);
void DecodeByCharset(comm_uint8 * in, Charset charset, MAIL_MESSAGE_T * mailmessage);
Charset GetCharsetByString(comm_uint8 * charset);
TransferEncoding GetTEncByString(comm_uint8 * tenc);
char * strdup_1 (const char *s);
char * strdup_1_l (const char *s, int len);
char * strdup_2( const char *s1, const char *s2 );
char * strdup_2_l( const char *s1, const char *s2, int subs2_l);
char * strcat_l (const char *s1,const char *s2,int len2);
char * Unfold(const char * in, char * header);
wchar * DecodeString(comm_uint8 * in);
int strcmp_nocase (const char *s1, const char *s2);
int strcmp_nocase_l(const char *s1, const char *s2, int len);
char * strchr_l (const char *s, char c, int len);
char * strstr_l (const char *s1, const char *s2, int s1_len);
comm_boolean Base64_decode(const unsigned char *in,  int inlen, 
                        unsigned char *out, int *outlen);
comm_boolean Base64_encode(const unsigned char *in,	int inlen, 
						unsigned char *out, int *outlen);
int Base64_decode_len (const char *str, int len);
unsigned long Base64_encode_len (unsigned long len);

comm_boolean HeaderEncode(char ** out, wchar * in, int * encodedSize, Charset charset);
int HeaderEncodeBase64(char * out, int out_l, int * encodedSize, const char * src);
int Q_len(const char *s );
comm_boolean Q_char( unsigned char c );
char * Mime_puthex( unsigned char c, char *trg );

int Getheaderencodedlength(int l);

/*==========================================================
 * Function 	: mail_generateMsgId
 * Author		: sunguochen      
 * Description :  Éú³Émsgid
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  7/15/2010 
 ==========================================================*/
void mail_generateMsgId(char *msg_id);

#endif

