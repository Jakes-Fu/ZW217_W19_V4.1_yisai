#ifndef _ABUP_STDLIB_H_
#define _ABUP_STDLIB_H_


abup_extern abup_int32 abup_sprintf(abup_char * buffer, const abup_char *format, ...);
abup_extern abup_int32 abup_snprintf(abup_char * buffer, abup_int32 size,const abup_char *format, ...);

abup_extern void * abup_memcpy(void * dest, const void * src, abup_uint32 count);
abup_extern void * abup_memset(void * dest, char c, abup_uint32 count );
abup_extern abup_int32 abup_memcmp(void * buf1, void * buf2, abup_uint32 count);
abup_extern abup_char * abup_strcat(abup_char * strDestination, const abup_char * strSource );
abup_extern abup_char * abup_strncat(abup_char * strDestination, const abup_char * strSource,abup_uint32 len);

abup_extern abup_uint32 abup_strlen(const abup_char * str);
abup_extern abup_int32 abup_strncmp(const abup_char * string1, abup_char * string2, abup_uint32 count);
abup_extern abup_int32 abup_strcmp(const abup_char * string1, const abup_char * string2 );
abup_extern abup_int32 abup_wstrcmp(const abup_wchar * string1, const abup_wchar * string2);
abup_extern abup_char * abup_strcpy(abup_char * strDestination, const abup_char * strSource);
abup_extern abup_char * abup_strncpy(abup_char * strDest, const abup_char * strSource, abup_uint32 count);
abup_extern abup_char * abup_strstr(const abup_char * str, const abup_char * strSearch);
abup_extern abup_char * abup_strchr(const abup_char * str, abup_int32 ch);

abup_extern void abup_itoa(abup_uint32 i, abup_char *buf, abup_uint8 base);
abup_extern abup_uint32 abup_atoi(abup_char * str);

abup_extern abup_uint32 abup_dt_curtime2utcsec(abup_time_struct *currTime);

#endif
