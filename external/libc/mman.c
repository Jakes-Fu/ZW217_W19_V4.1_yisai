#include "mman.h"

#define _PAGE_SIZE          (1u << 12)
#define IS_PAGE_ALIGNED(va) (0 == ((va) & (_PAGE_SIZE -1)))
#define UP_TO_PAGESIZE(va)  IS_PAGE_ALIGNED(va) ? va : ((((va) >> 12) + 1) << 12)

typedef struct mmapInfoTag
{
    void  *addr; //return addr
    size_t len;
    int    prot; //protection
    int    flags;
    int    fildes;
    int    off;
    struct mmapInfoTag* next;
} MmapInfo;

MmapInfo mmapInfoHeader = {0, 0, 0, 0,-1, 0, 0};

static int addToLink(void *addr, size_t len, int prot, int flags, int fildes, off_t off)
{
    //WaitResult wr;
    //g_link_mutex.Lock(&wr);

    MmapInfo* temp = (MmapInfo*)malloc( sizeof(MmapInfo) );
    temp->addr = addr;
    temp->len = len;
    temp->prot = prot;
    temp->flags = flags;
    temp->fildes = fildes;
    temp->off = off;
    temp->next = mmapInfoHeader.next;
    mmapInfoHeader.next = temp;
    return 0;
    
} //addToLink()


static MmapInfo* getMmapInfo(void *addr, size_t len)
{
    //WaitResult wr;
    //g_link_mutex.Lock(&wr);
    
    //ECode ec = NOERROR;
    size_t     it;

    //ec = _CVirtualMemory_Query((Address)addr, (Address *)&addr, &len);        
    //if( FAILED(ec) )
    //	return  NULL;
    
    MmapInfo* p = mmapInfoHeader.next;
    while (p) 
    {
        //it = UP_TO_PAGESIZE(p->len);
        if( (uint32_t)addr >= (uint32_t)p->addr && (uint32_t)addr <= (uint32_t)p->addr + p->len ) 
            return p;
            
        p = p->next;
    }
    //g_link_mutex.Unlock();
    return NULL;
}//getMmapInfo()


static int delFromLink(void *addr, size_t len)
{
    //WaitResult wr;
    //g_link_mutex.Lock(&wr);
    MmapInfo* p = mmapInfoHeader.next;
    MmapInfo* before = &mmapInfoHeader;
    while (p) 
    {
        if( p->addr == addr && len == p->len ) 
        {
            before->next = p->next;
            //if(p->fildes >= 0) _close(p->fildes);
            free((void*)p);
            return 0;
        }
        before = p;
        p = p->next;
    }
    //g_link_mutex.Unlock();
    return -1;
}

//Upon successful completion, the mmap() function returns the address at which the mapping was placed (pa);
//otherwise, it returns a value of MAP_FAILED and sets errno to indicate the error. The symbol MAP_FAILED is defined in the header < sys / mman.h >.
// No successful return from mmap() will return the value MAP_FAILED.
void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off)
{
    void* retBaseAddr = malloc( len );
    if( addToLink((void*)retBaseAddr, len, prot, flags, fildes, off) ) 
        return MAP_FAILED;
        
    if( PROT_NONE == prot ) 
    	return (void*)retBaseAddr;


    if( fildes == 0 ) 
    {
    	memset((void *)retBaseAddr, 0, len);
    } 
    else if( fildes > 0 ) 
    {
        int pos = lseek(fildes, off, SEEK_SET);
        if (pos == -1L)
            return MAP_FAILED;

        read(fildes, (void *)retBaseAddr, len);
    }
    return (void*)retBaseAddr;
}

int mprotect(void *addr, size_t len, int prot)
{
    MmapInfo* mmapInfo = NULL;

    mmapInfo = getMmapInfo(addr, len);
    if( !mmapInfo ) 
        return -1;
        
    mmapInfo->prot = prot;  
    return 0;
    
} //end of mprotect()


//we cann't control the mmu ,so ignore the parameter flags.
int msync(void *addr, size_t len, int flags)
{
    MmapInfo* mmapInfo;

    mmapInfo = getMmapInfo(addr, len);
    if(!mmapInfo ) 
    	return -1;
    	
    if( MS_SYNC == flags || MS_ASYNC == flags || (MAP_SHARED == mmapInfo->flags && ((mmapInfo->prot) & 0x2)) )
    {  //copy the memory to file
        if( mmapInfo->fildes > 0 ) 
        {
            lseek(mmapInfo->fildes, mmapInfo->off, SEEK_SET);
            write(mmapInfo->fildes, mmapInfo->addr, mmapInfo->len);
        }
    } 
    else 
    {
        return 1;
    }
    
    return 0;
    
} //end of msync()


int mlock(const void *addr, size_t len)
{
    return -1;
}


int munlock(const void *addr, size_t len)
{
    return -1;
}

int munmap(void* baseAddr, size_t len)
{
    if( msync(baseAddr, len, MS_SYNC) )
        return -1;

    MmapInfo* mmapInfo = getMmapInfo(baseAddr, len);
    if( !mmapInfo )
        return -1;
    
    baseAddr = mmapInfo->addr;
    len = mmapInfo->len;
    
    if( delFromLink(baseAddr, len) )
        return -1;
    
    free(baseAddr);
        
    return 0;
    
} //end of munmap()


int mincore(void*  start, size_t  length, unsigned char*  vec)
{
	return 0;
}