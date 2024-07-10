/******************************************************************************
                                   海信WCDMA  自主业务模块
        由于3G  网络的逐渐成熟，大量的业务需求成为第三代
        移动终端的亮点，逐渐成为不可或缺的一部分。本
        模块设计基于OMA  联盟提出的基于syncml 协议的数据和终
        端管理，实现在多平台、多操作系统中的PIM业务。目标
        是要达到任意平台的可移植性良好的功能稳定的模块。
        本模块的实现分为三个步骤:
        1.实现Data sync功能
        2.实现Device Management功能
        3.实现多平台的平滑移植
        模块开发参考OMA成员共同开发的SRT_c。
        目前模块计划开发支持OMA syncML 协议1.1


        版权声明:本软件版权属于海信通信，保留一切法律权利
        开发人员:刘刚
        开发日期:2005年4月11日。
******************************************************************************/


/******************************************************************************
文件名称:PIMlibmem.h
文件描述:libmem功能接口头文件
文件说明:完成不同平台不同操作系统下的不同内存分配操作
开发人员:刘刚
首次开发日期:2005-4-11
******************************************************************************/


/******************************************************************************
                                                            修改记录
  日期                   作者                修改原因
  -----------------------------------------------------------------------------
  2004-4-11              刘刚                第一次开发编码。
  -----------------------------------------------------------------------------
  *****************************************************************************/
/*加入此FEATURE防止头文件重复包含*/
  #ifndef   HEADER_FILE_LIBMEM
  #define  HEADER_FILE_LIBMEM
  
  /*  包含头文件*/
  #include "string.h"
  #include "stdlib.h"
  
  #include "os_api.h"
  /*  数据结构声明*/

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


  /*  全局变量声明*/

  /*  功能函数声明*/
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
   int hs_smlLibStrcmp(const char *pTarget, const char *pSource);    //区分大小写
   int _smlLibStrcmp(const char *pTarget, const char *pSource);  //不区分大小写
   char* hs_smlLibStrcat(const char *pTarget, const char *pSource);
   char* hs_smlLibStrncpy(const char *pTarget, const char *pSource, int count);
   int hs_smlLibStrncmp(const char *pTarget, const char *pSource, int count); //区分大小写
   int _smlLibStrncmp(const char *pTarget, const char *pSource, int count);  //不区分大小写
   char* hs_smlLibStrchr(const char *pString, char character);

   void hs_smlLibSleep(unsigned long ticks);
#endif
