/*
** Copyright (C) ARM Limited, 2000. All rights reserved.
*/


/*
** This implements a 'retarget' layer for low-level IO.  Typically, this
** would contain your own target-dependent implementations of fputc(),
** ferror(), etc.
**
** This example provides implementations of fputc(), ferror(),
** _sys_exit(), _ttywrch() and __user_initial_stackheap().
**
** Here, semihosting SWIs are used to display text onto the console
** of the host debugger.  This mechanism is portable across ARMulator,
** Angel, Multi-ICE and EmbeddedICE.
**
** Alternatively, to output characters from the serial port of an
** ARM Integrator Board (see serial.c), use:
**
**     #define USE_SERIAL_PORT
**
** or compile with
**
**     -DUSE_SERIAL_PORT
*/
#include "chip_drv_trc.h"
#include <stdio.h>
#include <rt_misc.h>
#include <rt_sys.h>
#include "sci_api.h"
//#include <assert.h>
// @Xueliang.Wang add temporarily(2003-03-18)
#include "threadx_os.h"

#ifdef __thumb
/* Thumb Semihosting SWI */
#define SemiSWI 0xAB
#else
/* ARM Semihosting SWI */
#define SemiSWI 0x123456
#endif

// @Xueliang.Wang added.(2002/05/22)
//extern unsigned int TXILLDB_SVC_STACK;
//extern unsigned int TXILLDB_SYS_STACK_SIZE;
// @Xueliang.Wang added end.

/* Write a character */
//__swi (SemiSWI) void _WriteC (unsigned op, char *c);
#define WriteC(c) _WriteC (0x3,c)

/* Exit */
#ifdef PLATFORM_SC6800H
//__swi (SemiSWI) void _Exit (unsigned op, unsigned except);
#endif
#define Exit() _Exit (0x18,0x20026)


struct __FILE /*lint !e753*/
{
    int handle;   /* Add whatever you need here */
};



/*
    write char through Tube
*/
void sendchar (char *ch_ptr)
{

}


int fputc (int ch, FILE *f)/*lint !e49 !e601*/
{/*lint !e49 */
    /* Place your implementation of fputc here     */
    /* e.g. write a character to a UART, or to the */
    /* debugger console with SWI WriteC            */
    return ch;
}


int ferror (FILE *f)/*lint !e832 -e832*/
{//lint !e101
    /* Your implementation of ferror */
    return EOF;
}


void _sys_exit (int return_code)
{
    SCI_PASSERT (0, ("System exit ! code is %d", return_code));/*assert verified*/
}


void _ttywrch (int ch)
{

}

__value_in_regs struct __initial_stackheap __user_initial_stackheap ( /*lint !e129 */
    unsigned R0, unsigned SP, unsigned R2, unsigned SL)
{
    struct __initial_stackheap config;//lint !e86 !e115

    config.stack_base = SP;//lint !e115
    return config;
}

/*
To place heap_base directly above the ZI area, use e.g:
    extern unsigned int Image$$ZI$$Limit;
    config.heap_base = (unsigned int)&Image$$ZI$$Limit;
(or &Image$$region_name$$ZI$$Limit for scatterloaded images)

To specify the limits for the heap & stack, use e.g:
    config.heap_limit = SL;
    config.stack_limit = SL;
*/

#if __ARMCC_VERSION > 220000 /*lint !e553*/
void $Sub$$__aeabi_idiv0(void)
#else
void __rt_div0 (void)
#endif
{
    int ret_addr = 0;

#if __ARMCC_VERSION > 220000 /*lint !e553 */

#pragma arm
    __asm
    {
        MOV R1, __return_address()
        MOV ret_addr, R1
    }
#if (1) //defined(RVCT_VERSION_V504)
#pragma arm
#else
#pragma thumb
#endif
#else
    __asm
    {
        MOV R1, R14
        MOV ret_addr, R1
    }

#endif

    //SCI_TRACE_LOW:"DIV 0 error, Return address 0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,RETARGET_141_112_2_17_23_5_20_1076,(uint8*)"d", ret_addr);
    SCI_PASSERT (0, ("DIV 0 error, Return address 0x%X", ret_addr));/*assert verified*/
}

void StackOverflow (int ret_addr)
{
    uint32      thread_ID;
    TX_THREAD   *thread_ptr = tx_thread_identify();

    thread_ID = SCI_IdentifyThread();

    //SCI_TRACE_LOW:"Stack Over flow, return address 0x%X, Thread ID:%d, Thread Name:%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,RETARGET_153_112_2_17_23_5_20_1077,(uint8*)"dds",ret_addr, thread_ID, thread_ptr->tx_thread_name);

    SCI_PASSERT (0, ("Stack Over flow, return address 0x%X, Thread ID:%d, Thread Name:%s", /*assert verified*/
                     ret_addr, thread_ID, thread_ptr->tx_thread_name));
}

void __assert (const char *p, const char *c, int i)
{
    SCI_Assert ( (void *) p, (void *) c, (int) i);/*assert verified*/
}

void * malloc(size_t  size)
{
    return SCI_ALLOC_APP(size);
}


void free(void *ptr)
{
    SCI_Free(ptr);
}

void* realloc(void *ptr, size_t size)
{
    return SCI_REALLOC(ptr, size);
    SCI_PASSERT(0, ("realloc should not be used on MOCOR")); /*assert to do*/
    //return 0;//lint !e527 for Coverity & Pclint tools
}
/*
 * Close a file. Should return 0 on success or a negative value on error.
 */
int $Sub$$_sys_close (FILEHANDLE handle/*fh*/) /*lint !e27*/
{
    return 0; /*lint !e26*/
}

/*
 * Write to a file. Returns 0 on success, negative on error, and
 * the number of characters _not_ written on partial success.
 * `mode' is either 0 or OPEN_B, consistent with the mode in which
 * the file was opened.
 */
int $Sub$$_sys_write (FILEHANDLE handle/*fh*/, const unsigned char *buf_ptr/*buf*/, /*lint !e27*/
                      unsigned len /*len*/, int mode /*mode*/)
{
    return 0;
}

/*
 * Read from a file. Can return:
 *  - zero if the read was completely successful
 *  - the number of bytes _not_ read, if the read was partially successful
 *  - the number of bytes not read, plus the top bit set (0x80000000), if
 *    the read was partially successful due to end of file
 *  - -1 if some error other than EOF occurred
 *
 * It is also legal to signal EOF by returning no data but
 * signalling no error (i.e. the top-bit-set mechanism need never
 * be used).
 *
 * So if (for example) the user is trying to read 8 bytes at a time
 * from a file in which only 5 remain, this routine can do three
 * equally valid things:
 *
 *  - it can return 0x80000003 (3 bytes not read due to EOF)
 *  - OR it can return 3 (3 bytes not read), and then return
 *    0x80000008 (8 bytes not read due to EOF) on the next attempt
 *  - OR it can return 3 (3 bytes not read), and then return
 *    8 (8 bytes not read, meaning 0 read, meaning EOF) on the next
 *    attempt
 *
 * `mode' is either 0 or OPEN_B, consistent with the mode in which
 * the file was opened.
 */
int $Sub$$_sys_read (FILEHANDLE handle /*fh*/, unsigned char *buf_ptr/*buf*/, /*lint !e27 !e129 !e832*/
                     unsigned len /*len*/, int mode /*mode*/)
{   /*lint !e745 !e101*/
    return 0;
}


/*
 * Move the file position to a given offset from the file start.
 * Returns >=0 on success, <0 on failure.
 */
int $Sub$$_sys_seek (FILEHANDLE handle /*fh*/, long pos /*pos*/) /*lint !e27 !e31*/
{
    return 0; /*lint !e26*/
}

/*
 * Flush any OS buffers associated with fh, ensuring that the file
 * is up to date on disk. Result is >=0 if OK, negative for an
 * error.
 */
int $Sub$$_sys_ensure (FILEHANDLE hanlde/*fh*/) /*lint !e27*/
{
    return 0;
}

/*
 * Return the current length of a file, or <0 if an error occurred.
 * _sys_flen is allowed to reposition the file pointer (so Unix can
 * implement it with a single lseek, for example), since it is only
 * called when processing SEEK_END relative fseeks, and therefore a
 * call to _sys_flen is always followed by a call to _sys_seek.
 */
long $Sub$$_sys_flen (FILEHANDLE handle/*fh*/) /*lint !e27 !e129 !e832*/
{   //lint !e18 !e745 !e101 confirmed by xuepeng
    return 0;
}

/*
 * Return the name for temporary file number sig in the buffer
 * name. Returns 0 on failure. maxlen is the maximum name length
 * allowed.
 */
int $Sub$$_sys_tmpnam (char *name /*name*/, int sig /*sig*/, unsigned max_len/*maxlen*/) /*lint !e27 !e31*/
{
    return 0; /*lint !e26*/
}


int $Sub$$__cpp_initialise (void) /*lint !e27*/
{
    return 0;
}

#if __ARMCC_VERSION > 220000  /*lint !e553*/
int $Sub$$__cpp_initialize__aeabi_ (void)
{
    return 0;
}
#endif

int $sub$$_sys_istty (void) /*lint !e27 !e129*/
{   //lint !e18 !e745 !e515confirmed by xuepeng
    return 0;
}


int $Sub$$_sys_open (void) /*lint !e27 !e31*/
{
    return 0; /*lint !e26*/
}
/*
new(unsigned int a)
{
    return 0;
}

delete(void* a)
{
    return 0;
}
*/
typedef void (*VECMAP1C_FUN_PTR) (void *);  /*lint !e19*/
int __vecmap1c (void *a , void *b, int c, VECMAP1C_FUN_PTR *d)  /*lint !e49 !e601*/
{   /*lint !e49 */
    return 0;
}
/* Retarget _fp_init*/
void _fp_init(void)
{
}
//End
