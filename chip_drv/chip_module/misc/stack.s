
		PRESERVE8
        AREA    |C$$code|, CODE, READONLY

	EXPORT __rt_stackheap_init
	EXPORT __rt_heap_extend
	EXPORT __rt_stack_postlongjmp
	EXPORT __rt_stack_overflow
	IMPORT StackOverflow

__rt_stackheap_init
	BX r14

__rt_heap_extend
	BX r14

__rt_stack_postlongjmp
	MOV     r10,r0
	MOV     r13,r1
	BX      r14
	
__rt_stack_overflow
	MOV r0, #0
	MOV r10, r0
	MOV r0, r14
	BL StackOverflow			
        END

