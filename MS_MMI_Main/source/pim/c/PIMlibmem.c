
/******************************************************************************
文件名称:PIMlibmem.c
文件描述:libmem功能接口执行文件
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

  /*  包含头文件*/
#include "mmi_pim_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#define _PIMLIBMEM_C_

#include "stdlib.h"
#include "PIMlibmem.h"
#include "mmipim.h"

#ifdef HS_PIM_DISPALY_MALLOC_NUM
uint32 sml_malloc_num = 0; // chenxiang_memory 20080215
#endif
  /*  数据结构声明*/
#ifdef PIM_MEMORY_LEAK_DEBUG // chenxiang_memory 20080314

typedef struct LeakLink_s
{
    uint32      ptr_addr;
    uint32      line;
    char        filename[70];
    struct LeakLink_s *  next;
} LeakLink_t, *LeakLinkPtr_t;
  
  /*  全局变量声明*/
LeakLinkPtr_t leak_link_head_ptr = NULL;
LeakLinkPtr_t leak_link_tail_ptr = NULL;

  /*  功能函数*/

/* chenxiang_memory 20080312 */
void AddToLeakLink(void *ptr, const char * file,  int line)
{
    if (NULL == leak_link_head_ptr)
    {
        leak_link_head_ptr = (LeakLinkPtr_t)SCI_ALLOC(sizeof(LeakLink_t));
        leak_link_tail_ptr = leak_link_head_ptr;
    }
    else
    {
        leak_link_tail_ptr->next = (LeakLinkPtr_t)SCI_ALLOC(sizeof(LeakLink_t));
        leak_link_tail_ptr = leak_link_tail_ptr->next;
    }
    leak_link_tail_ptr->ptr_addr = (uint32)ptr;
    SCI_MEMCPY(leak_link_tail_ptr->filename, file, strlen(file));
    leak_link_tail_ptr->filename[strlen(file)] = 0;
    leak_link_tail_ptr->line = (uint32)line;
    leak_link_tail_ptr->next = NULL;
}

/* chenxiang_memory 20080312 */
void DeleteFromLeakLink(void *ptr)
{
    LeakLinkPtr_t temp_ptr = leak_link_head_ptr;
    LeakLinkPtr_t temp_ptr_2 = leak_link_head_ptr;
    while ((NULL != temp_ptr) && (temp_ptr->ptr_addr != (uint32)ptr) )
    {
        temp_ptr_2= temp_ptr;
        temp_ptr = temp_ptr->next;
    }
    if (NULL != temp_ptr)
    {
        temp_ptr_2->next = temp_ptr->next;
        if (leak_link_tail_ptr == temp_ptr)
        {
            leak_link_tail_ptr = temp_ptr_2;
        }
        if (leak_link_head_ptr == temp_ptr)
        {
            leak_link_head_ptr = temp_ptr->next;
        }
        if ((leak_link_head_ptr == temp_ptr) && (leak_link_tail_ptr == temp_ptr))
        {
            leak_link_head_ptr = NULL;
            leak_link_tail_ptr = NULL;
        }
        SCI_FREE(temp_ptr);
    }
    else
    {
        //SCI_TRACE_MID:"MMIPIM chenxiang ptr is not in the link ! pObject = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMLIBMEM_95_112_2_18_3_25_50_225,(uint8*)"d", (uint32)ptr);
    }
}

/* chenxiang_memory 20080312 */
void DestroyLeakLink()
{
    LeakLinkPtr_t temp_ptr = leak_link_head_ptr;
    LeakLinkPtr_t temp_ptr_2 = leak_link_head_ptr;
    while (NULL != temp_ptr)
    {
        temp_ptr_2 = temp_ptr;
        temp_ptr = temp_ptr->next;
        SCI_FREE(temp_ptr_2);
    }
    leak_link_head_ptr = NULL;
    leak_link_tail_ptr = NULL;
    sml_malloc_num = 0;
}

/* chenxiang_memory 20080312 */
void PrintLeakLink()
{
    LeakLinkPtr_t temp_ptr = leak_link_head_ptr;
    uint32 i = 0;
    while (NULL != temp_ptr)
    {
        i++;
        //SCI_TRACE_MID:"MMIPIM PrintLeakLink %d  ptr: %d  file : %s  line %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMLIBMEM_121_112_2_18_3_25_50_226,(uint8*)"ddsd", i, temp_ptr->ptr_addr, temp_ptr->filename, temp_ptr->line);
        temp_ptr = temp_ptr->next;
    }
    //SCI_TRACE_MID:"MMIPIM PrintLeakLink total: %d  sml_malloc_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,PIMLIBMEM_124_112_2_18_3_25_50_227,(uint8*)"dd", i, sml_malloc_num);
    DestroyLeakLink();
}
#endif // PIM_MEMORY_LEAK_DEBUG
/**
 * FUNCTION: hs_smlLibFree
 *
 * Deallocates the memory of object "pObject", which has been allocated
 * previously.
 * If "pObject" is a NULL pointer nothing happens.
 * If "pObject" is a pointer to memory which has not been allocated 
 * previouly, the behaviour is undefined.
 * The contents of the deallocated memory object is destroyed.
 */
void hs_smlLibFree(void *pObject) // chenxiang_memory 20080221
{
    if (! pObject) return;
#ifdef HS_PIM_DISPALY_MALLOC_NUM
    sml_malloc_num--; // chenxiang_20080215
#endif
#ifdef PIM_MEMORY_LEAK_DEBUG // chenxiang_memory 20080314
    DeleteFromLeakLink(pObject); // chenxiang_memory 20080312
#endif
  //SCI_TRACE_MID_PIM("MMIPIM chenxiang sml_malloc_num = %d pObject = %d", sml_malloc_num, pObject); // chenxiang_20080215
    SCI_FREE(pObject);
//    pObject = NULL; // chenxiang_memory 20080219
}


/**
 * FUNCTION: hs_smlLibRealloc
 *
 * Changes size of preallocated space for memory object "pObject"
 * to the new size specified by "constSize".
 *
 * If the new size is larger than the old size, the old contents 
 * is not changed. Additionally space is added at the the end of 
 * "pObject". The new allocated space is not initialized 
 * to any special value.
 * If the new size is smaller than the old size, the unused space
 * is discarded.
 *
 * If "pObject" is a NULL pointer, this function behaves just like 
 * hs_smlLibMalloc().
 * If "pObject" does not point to a previously allocated memory area, 
 * the behavior is undefined.
 * If "constSize" is 0, a NULL pointer is returned and the space 
 * which "pObject" points to is freed up.
 *
 * Returns a pointer to the first byte of the resized object.
 * If no new memory could be allocated, a NULL Pointer is returned 
 * without changing the memory object "pObject" (Nothing happens to the content).
 *
 * IN/OUT           void *pObject,      memory object, which size should be changed
 * IN:              MemSize_t constSize new size the memory object shall use
 * RETURN:          void*               Pointer to memory object, which size has been
 *                                      be changed
 *                                      NULL, if not successfull or
 *                                            if constSize==0
 */
void *hs_smlLibRealloc(void *pObject, long constSize)
{
	return realloc(pObject, constSize);
}

  
  void *hs_smlLibMemset(void *pObject, int value, long count){
	return memset(pObject, value, count);
}
 void *hs_smlLibMemcpy(void *pTarget, const void *pSource, long count){
	return memcpy(pTarget, pSource, count);
}
 void *hs_smlLibMemmove(void *pTarget, const void *pSource, long count){
	return memmove(pTarget, pSource, count);
}
 int hs_smlLibMemcmp(const void *pTarget, const void *pSource, long count){
	return memcmp(pTarget, pSource, count);
}

 void *hs_smlLibMalloc(const char * file,  int line, long size) 
{
    void * ptr;
    ptr = SCI_ALLOC_APP(size);
#ifdef HS_PIM_DISPALY_MALLOC_NUM
    sml_malloc_num++; // chenxiang_20080215
#endif
#ifdef PIM_MEMORY_LEAK_DEBUG // chenxiang_memory 20080314
    AddToLeakLink(ptr, file, line); // chenxiang_memory 20080312
#endif
 //SCI_TRACE_MID("MMIPIM chenxiang file: %s , line: %d , sml_malloc_num = %d , size = %d ,  ptr = 0x%x", file, line, sml_malloc_num, size,  ptr); // chenxiang_20080215
        return (void *)ptr;
}

int hs_smlLibStrlen(const char *pString){
	return strlen((char *)pString);
}

char* hs_smlLibStrcpy(const char *pTarget, const char *pSource) {
	return strcpy((char *)pTarget, (char *)pSource);
}
 /**
 * FUNCTION: hs_smlLibStrdup
 *
 * Duplicates the String "constStringP".
 * Returns a pointer to the new copy of "constStringP". 
 *
 * IN:              String_t   constStringP     string, which is duplicated
 * RETURN:          String_t   pointer to the new copy,
 *                             null, if no copy could be allocated
 */
char* hs_smlLibStrdup (const char *constStringP)
{
	char* _new_str=NULL;

	// allocate memory for new copy
	_new_str = (char*)hs_smlLibMalloc(__FILE__, __LINE__, hs_smlLibStrlen(constStringP) + 1);
	
	// Copy the string into the new memory
	if (_new_str != NULL)
		hs_smlLibStrcpy(_new_str, constStringP);
	   
	return _new_str;
}
 //区分大小写
  int hs_smlLibStrcmp(const char *pTarget, const char *pSource){
	return strcmp((char *)pTarget, (char *)pSource);
}
   //不区分大小写
  int _smlLibStrcmp(const char *pTarget, const char *pSource){
      int targetlen;
      int sourcelen;
      int lesslen;
      int i;
      targetlen=strlen((char *)pTarget);
      sourcelen=strlen((char *)pSource);
      if(targetlen>sourcelen)
      {
         lesslen=sourcelen;
      }else{
         lesslen=targetlen;
      }
      for(i=0;i<lesslen;i++)
      {
         //it is english letter
         if(*(pSource+i)==*(pTarget+i))
         {
            continue;
         }else{
               if((0x41<=*(pSource+i)&&0x5A>=*(pSource+i))||(0x61<=*(pSource+i)&&0x7A>=*(pSource+i)))
               {
                  if(*(pSource+i)<*(pTarget+i))   
                  {
                     if((*(pSource+i)+0x20)==*(pTarget+i))
                     {
                           continue;
                     }else{
                           return 1;
                     }
                  }else{
                     if((*(pSource+i)-0x20)==*(pTarget+i))
                     {
                           continue;
                     }else{
                           return -1;
                     }
                  }
               }else{
                  if(*(pSource+i)<*(pTarget+i))   
                  {
                     return 1;
                  }else{
                     return -1;
                  }
               }
         }
      }

      return 0;
}
  
  char* hs_smlLibStrcat(const char *pTarget, const char *pSource){
	return strcat((char *)pTarget, (char *)pSource);
}

  char* hs_smlLibStrncpy(const char *pTarget, const char *pSource, int count){
	return strncpy((char *)pTarget, (char *)pSource, count);
}
 //区分大小写
int hs_smlLibStrncmp(const char *pTarget, const char *pSource, int count)
{
   if (PNULL !=pSource)
   {
       return strncmp((char *)pTarget, (char *)pSource, count);
   }
   else
   {
       return -1;
   }
}
 //不区分大小写
  int _smlLibStrncmp(const char *pTarget, const char *pSource, int count){
      int targetlen;
      int sourcelen;
      int lesslen;
      int i;
      targetlen=strlen((char *)pTarget);
      sourcelen=strlen((char *)pSource);
      if(targetlen<count)
      {
         return -1;
      }
      if(sourcelen<count)
      {
         return 1;
      }
      lesslen=count;
      for(i=0;i<lesslen;i++)
      {
         //it is english letter
         if(*(pSource+i)==*(pTarget+i))
         {
            continue;
         }else{
               if((0x41<=*(pSource+i)&&0x5A>=*(pSource+i))||(0x61<=*(pSource+i)&&0x7A>=*(pSource+i)))
               {
                  if(*(pSource+i)<*(pTarget+i))   
                  {
                     if((*(pSource+i)+0x20)==*(pTarget+i))
                     {
                           continue;
                     }else{
                           return 1;
                     }
                  }else{
                     if((*(pSource+i)-0x20)==*(pTarget+i))
                     {
                           continue;
                     }else{
                           return -1;
                     }
                  }
               }else{
                  if(*(pSource+i)<*(pTarget+i))   
                  {
                     return 1;
                  }else{
                     return -1;
                  }
               }
         }
      }

      return 0;
}

  char* hs_smlLibStrchr(const char *pString, char character){
	return strchr((char *)pString, character);
}
  
void hs_smlLibSleep(unsigned long ticks)
{
     SCI_Sleep( ticks );
}
