;;; Copyright ARM Ltd 2000. All rights reserved.
;
; This module performs ROM/RAM remapping (if required), initializes stack pointers and
; interrupts for each mode, and finally branches to __main in the C library (which
; eventually calls main()).
;
; On reset, the ARM core starts up in Supervisor (SVC) mode, in ARM state, with IRQ and FIQ disabled.

    PRESERVE8
    AREA    Init, CODE, READONLY

; --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs



    ENTRY

	IMPORT  Reset_Handler
	CODE32
    EXPORT  __vectors
__vectors
    LDR     pc,=Reset_Handler            ; Reset goes to startup function
    LDR     pc,=__undefined                 ; Undefined handler
    LDR     pc,=__swi_interrupt             ; Software interrupt handler
    LDR     pc,=__prefetch_handler       ; Prefetch exeception handler
    LDR     pc,=__abort_handler           ; Abort exception handler
    LDR     pc,=__reserved_handler       ; Reserved exception handler
    LDR     pc,=__irq_handler               ; IRQ interrupt handler
    LDR     pc,=__fiq_handler               ; FIQ interrupt handler



    EXPORT  __undefined
__undefined
    b __undefined

    EXPORT  __swi_interrupt
__swi_interrupt
    b __swi_interrupt

    EXPORT  __prefetch_handler
__prefetch_handler
    b __prefetch_handler

    EXPORT  __abort_handler
__abort_handler
    b __abort_handler

    EXPORT  __reserved_handler
__reserved_handler
    b __reserved_handler

    EXPORT  __irq_handler
__irq_handler
    b __irq_handler

    EXPORT  __fiq_handler
__fiq_handler
    b __fiq_handler


    END

