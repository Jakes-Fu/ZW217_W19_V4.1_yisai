
        AREA    |C$$code|, CODE, READONLY

;/*********************************************************/
;//Description
;//r0 holds the address of the semaphore
;//r1 holds the ID of the process requesting the lock 
;/*********************************************************/
; uint32 _semaphore_exchange(uint32 *target, uint32 value)	

	EXPORT _semaphore_exchange
	CODE32
	
_semaphore_exchange

	SWP r2, r1, [r0]
	;load r2 from address r0 and store r1 to address r1
	
	MOV r0, r2
	BX	lr

	
	END
