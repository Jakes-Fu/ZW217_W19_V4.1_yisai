/******************************************************************************
                                   ����WCDMA  ����ҵ��ģ��
        ����3G  ������𽥳��죬������ҵ�������Ϊ������
        �ƶ��ն˵����㣬�𽥳�Ϊ���ɻ�ȱ��һ���֡���
        ģ����ƻ���OMA  ��������Ļ���syncml Э������ݺ���
        �˹���ʵ���ڶ�ƽ̨�������ϵͳ�е�PIMҵ��Ŀ��
        ��Ҫ�ﵽ����ƽ̨�Ŀ���ֲ�����õĹ����ȶ���ģ�顣
        ��ģ���ʵ�ַ�Ϊ��������:
        1.ʵ��Data sync����
        2.ʵ��Device Management����
        3.ʵ�ֶ�ƽ̨��ƽ����ֲ
        ģ�鿪���ο�OMA��Ա��ͬ������SRT_c��
        Ŀǰģ��ƻ�����֧��OMA syncML Э��1.1


        ��Ȩ����:�������Ȩ���ں���ͨ�ţ�����һ�з���Ȩ��
        ������Ա:����
        ��������:2005��4��11�ա�
******************************************************************************/


/******************************************************************************
�ļ�����:PIMlibmem.h
�ļ�����:libmem���ܽӿ�ͷ�ļ�
�ļ�˵��:��ɲ�ͬƽ̨��ͬ����ϵͳ�µĲ�ͬ�ڴ�������
������Ա:����
�״ο�������:2005-4-11
******************************************************************************/


/******************************************************************************
                                                            �޸ļ�¼
  ����                   ����                �޸�ԭ��
  -----------------------------------------------------------------------------
  2004-4-11              ����                ��һ�ο������롣
  -----------------------------------------------------------------------------
  *****************************************************************************/
/*�����FEATURE��ֹͷ�ļ��ظ�����*/
  #ifndef   HEADER_FILE_LIBMEM
  #define  HEADER_FILE_LIBMEM
  
  /*  ����ͷ�ļ�*/
  #include "string.h"
  #include "stdlib.h"
  
  #include "os_api.h"
  /*  ���ݽṹ����*/

//#define PIM_MEMORY_LEAK_DEBUG // chenxiang_memory 20080314

 #define xppStrcpy(target, source)        strcpy(target, source)
 #define xppStrncpy(target, source, cnt)  strncpy(target, source, cnt)
 #define xppStrcat(target, source)        strcat(target, source)
 #define xppStrncat(target, source, cnt)  strncat(target, source, cnt)
 #define xppStrcmp(first, second)         strcmp(first, second)
 #define xppStrncmp(first, second, cnt)   strncmp(first, second, cnt)
 #define xppStrchr(string, c)             strchr(string, c)
 #define xppStrstr(string, substr)        strstr(string, substr)
 #define xppStrlen(string)                strlen(string)
 #define xppAtoi(string)                  atoi(string)

 #define xppMemset(s, c, n)               memset(s, c, n)
 #define xppMemcpy(target, source, cnt)   memcpy(target, source, cnt)
 #define xppMemmove(target, source, cnt)  memmove(target, source, cnt)
 #define xppMemcmp(target, source, cnt)   memcmp(target, source, cnt)
 #define xppMalloc(size)                  SCI_ALLOC(size)
 #define xppRealloc(ptr, size)            realloc(ptr, size)
 #define xppFree(ptr)                     SCI_FREE(ptr)

 /* These aren't ANSI C functions, but they're pretty common */

 #define xppStricmp(first, second)       strcmp(first, second)

  // MWERKS support
#define xppMemicmp(first, second, cnt)  memcmp(first, second, cnt)


  /*  ȫ�ֱ�������*/

  /*  ���ܺ�������*/
   void	*hs_smlLibRealloc(void *pObject, long size);
   void	hs_smlLibFree(void *pObject);
   void 	*hs_smlLibMemset(void *pObject, int value, long count);
   void 	*hs_smlLibMemcpy(void *pTarget, const void *pSource, long count);
   void 	*hs_smlLibMemmove(void *pTarget, const void *pSource, long count);
   int		hs_smlLibMemcmp(const void *pTarget, const void *pSource, long count);
   void	*hs_smlLibMalloc(const char * file,  int line, long size); // chenxiang_memory 20080311

   int hs_smlLibStrlen(const char *pString);
   char* hs_smlLibStrcpy(const char *pTarget, const char *pSource) ;
   char* hs_smlLibStrdup (const char *constStringP);
   int hs_smlLibStrcmp(const char *pTarget, const char *pSource);    //���ִ�Сд
   int _smlLibStrcmp(const char *pTarget, const char *pSource);  //�����ִ�Сд
   char* hs_smlLibStrcat(const char *pTarget, const char *pSource);
   char* hs_smlLibStrncpy(const char *pTarget, const char *pSource, int count);
   int hs_smlLibStrncmp(const char *pTarget, const char *pSource, int count); //���ִ�Сд
   int _smlLibStrncmp(const char *pTarget, const char *pSource, int count);  //�����ִ�Сд
   char* hs_smlLibStrchr(const char *pString, char character);

   void hs_smlLibSleep(unsigned long ticks);
#endif
