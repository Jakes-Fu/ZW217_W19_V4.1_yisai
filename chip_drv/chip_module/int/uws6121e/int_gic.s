;=====================================================================
; File Name : armcore.s
; Function  : c6320  A7  armcore code(Assembly)
; Date      : 2014-07-03
; version	: V0.0.1
; author    : xiongguangyin
;=====================================================================

GIC_BASE_ADDR    EQU   0x00800000

        PRESERVE8

        AREA  ARMCORE, CODE, READONLY
        CODE32
;==================================================================
; GIC. Generic Interrupt Controller Architecture Specification
;==================================================================

        ; CPU Interface offset from base of private peripheral space --> 0x0100
        ; Interrupt Distributor offset from base of private peripheral space --> 0x1000

        ; Typical calls to enable interrupt ID X:
        ; enableIntID(X)                    <-- Enable that ID
        ; setIntPriority(X, 0)              <-- Set the priority of X to 0 (the max priority)
        ; setPriorityMask(0x1F)             <-- Set CPU's priority mask to 0x1F (the lowest priority)
        ; enableGIC()                       <-- Enable the GIC (global)
        ; enableGICProcessorInterface()     <-- Enable the CPU interface (local to the CPU)

;==============================================================
; getGICStatus
;==============================================================
        EXPORT  getGICStatus
        ; u16 getGICStatus(void)
        ; get status of Global enable of the Interrupt Distributor
getGICStatus PROC
        ; Get base address of private peripheral space
        MRC     p15, 4, r1, c15, c0, 0     ; Read periph base address
        ADD     r1, r1, #0x1000            ; Add the GIC offset

        LDR     r0, [r1]                   ; Read the GIC's Enable Register  (ICDDCR)
        AND     r0, r0, #0x03              ; Set bit 0, the enable bit,support group1 and group0

        BX      lr
        ENDP


;==============================================================
; enableGIC
;==============================================================
        EXPORT  enableGIC
        ; void enableGIC(void)
        ; Global enable of the Interrupt Distributor
enableGIC PROC
        ; Get base address of private peripheral space
        MOV     r0, #GIC_BASE_ADDR         ; Read periph base address
        ADD     r0, r0, #0x1000            ; Add the GIC offset

        LDR     r1, [r0]                   ; Read the GIC's Enable Register  (ICDDCR)
        ORR     r1, r1, #0x01              ; Set bit 0, the enable bit,support group1 and group0
        STR     r1, [r0]                   ; Write the GIC's Enable Register  (ICDDCR)

        BX      lr
        ENDP

;==============================================================
; disableGIC
;==============================================================

        EXPORT disableGIC
        ; void disableGIC(void)
        ; Global disable of the Interrupt Distributor
disableGIC PROC
        ; Get base address of private peripheral space
        MOV     r0, #GIC_BASE_ADDR         ; Read periph base address
        ADD     r0, r0, #0x1000            ; Add the GIC offset

        LDR     r1, [r0]                   ; Read the GIC's Enable Register  (ICDDCR)
        BIC     r1, r1, #0x01              ; Clear bit 0, the enable bit, support group1 and group0
        STR     r1, [r0]                   ; Write the GIC's Enable Register  (ICDDCR)

        BX      lr
        ENDP

;==============================================================
; enableIntID
;==============================================================

        EXPORT  enableIntID
        ; void enableIntID(unsigned int ID)
        ; Enables the interrupt source number ID
enableIntID PROC
        ; Get base address of private peripheral space
        MOV     r1, r0                     ; Back up passed in ID value
        MOV     r0, #GIC_BASE_ADDR         ; Read periph base address

        ; Each interrupt source has an enable bit in the GIC.  These
        ; are grouped into registers, with 32 sources per register
        ; First, we need to identify which 32 bit block the interrupt lives in
        ADD     r1, r1, #32                 ; first reg is to sgi/ppi
        MOV     r2, r1                     ; Make working copy of ID in r2
        LSR     r2, r2, #5             ; LSR by 5 places, affective divide by 32
                                           ; r2 now contains the 32 bit block this ID lives in
        LSL     r2, r2, #2             ; Now multiply by 4, to covert offset into an address offset (four bytes per reg)

        ; Now work out which bit within the 32 bit block the ID is
        AND     r1, r1, #0x1F              ; Mask off to give offset within 32bit block
        MOV     r3, #1                     ; Move enable value into r3
        LSL     r3, r3, r1             ; Shift it left to position of ID

        ADD     r2, r2, #0x1100            ; Add the base offset of the Enable Set registers to the offset for the ID
        STR     r3, [r0, r2]               ; Store out  (ICDISER)

        BX      lr
        ENDP

;==============================================================
; disableIntID
;==============================================================

        EXPORT  disableIntID
        ; void disableIntID(unsigned int ID)
        ; Disables the interrupt source number ID
disableIntID PROC
        ; Get base address of private peripheral space
        MOV     r1, r0                     ; Back up passed in ID value
        MOV     r0, #GIC_BASE_ADDR         ; Read periph base address

        ; First, we need to identify which 32 bit block the interrupt lives in
        ADD     r1, r1, #32                 ; first reg is to sgi/ppi
        MOV     r2, r1                     ; Make working copy of ID in r2
        LSR     r2, r2, #5             ; LSR by 5 places, affective divide by 32
                                           ; r2 now contains the 32 bit block this ID lives in
        LSL     r2, r2, #2             ; Now multiply by 4, to covert offset into an address offset (four bytes per reg)

        ; Now work out which bit within the 32 bit block the ID is
        AND     r1, r1, #0x1F              ; Mask off to give offset within 32bit block
        MOV     r3, #1                     ; Move enable value into r3
        LSL     r3, r3, r1             ; Shift it left to position of ID in 32 bit block

        ADD     r2, r2, #0x1180            ; Add the base offset of the Enable Clear registers to the offset for the ID
        STR     r3, [r0, r2]               ; Store out (ICDICER)

        BX      lr
        ENDP

;==============================================================
; readIntIDStatus
;==============================================================

        EXPORT readIntIDStatus
        ; void readIntIDStatus(unsigned int ID)
        ; read the int of the specifed ID
        ; r0 = ID
readIntIDStatus PROC
        ; Get base address of private peripheral space
        MOV     r1, r0                     ; Back up passed in ID value
        MOV     r0, #GIC_BASE_ADDR         ; Read periph base address

        ; r0 = base addr
        ; r1 = ID

        ; Find which priority register this ID lives in
        ADD     r1, r1, #32                 ; first 32 to sgi/ppi

        MOV     r2, #0x1100
        ADD     r2, r2, r0
        
        LSR     r3, r1, #5
        LSL     r3, r3, #2
        ADD     r2, r2, r3
        
        AND     r1, r1, #0x1F              ; Mask off to give offset within 32bit block
        MOV     r3, #1                     ; Move enable value into r3
        LSL     r3, r3, r1             ; Shift it left to position of ID in 32 bit block
        
        LDR     r0, [r2]                   ; Read the GICD_ISPENDRn Register
        
        AND     r0, r0, r3
        
        LSR     r0, r0, r1

        BX      lr
        ENDP
        
;==============================================================
; setIntPriority
;==============================================================

        EXPORT setIntPriority
        ; void setIntPriority(unsigned int ID, unsigned int priority)
        ; Sets the priority of the specifed ID
        ; r0 = ID
        ; r1 = priority
setIntPriority PROC
        ; Get base address of private peripheral space
        ADD     r0, r0, #32                 ; first reg is to sgi/ppi
        MOV     r2, r0                     ; Back up passed in ID value
        MOV     r0, #GIC_BASE_ADDR         ; Read periph base address

        ; r0 = base addr
        ; r1 = priority
        ; r2 = ID

        ; Make sure that priority value is only 5 bits, and convert to expected format
        AND     r1, r1, #0x1F
        LSL     r1, r1, #3

        ; Find which priority register this ID lives in
        BIC     r3, r2, #0x03              ; Make a copy of the ID, clearing off the bottom two bits
                                           ; There are four IDs per reg, by clearing the bottom two bits we get an address offset
        ADD     r3, r3, #0x1400            ; Now add the offset of the Priority Level registers from the base of the private peripheral space
        ADD     r0, r0, r3                 ; Now add in the base address of the private peripheral space, giving us the absolute address

        ; Now work out which ID in the register it is
        AND     r2, r2, #0x03              ; Clear all but the bottom two bits, leaves which ID in the reg it is (which byte)
        LSL     r2, r2, #3             ; Multiply by 8, this gives a bit offset

        ; Read -> Modify -> Write
        MOV     r12, #0xFF                 ; Mask (8 bits)
        LSL     r12, r12, r2           ; Move mask into correct bit position
        LSL     r1, r1, r2             ; Also, move passed in priority value into correct bit position

        LDR     r3, [r0]                   ; Read current value of the Priority Level register (ICDIPR)
        BIC     r3, r3, r12                ; Clear appropriate field
        ORR     r3, r3, r1                 ; Now OR in the priority value
        STR     r3, [r0]                   ; And store it back again  (ICDIPR)

        BX      lr
        ENDP

;==============================================================
; readIntPriority
;==============================================================

        EXPORT readIntPriority
        ; void readIntPriority(unsigned int ID)
        ; read the priority of the specifed ID
        ; r0 = ID
readIntPriority PROC
        ; Get base address of private peripheral space
        MOV     r2, r0                     ; Back up passed in ID value
        MOV     r0, #GIC_BASE_ADDR         ; Read periph base address

        ; r0 = base addr
        ; r2 = ID

        ; Find which priority register this ID lives in
        ADD     r2, r2, #32                 ; first 32 to sgi/ppi
        BIC     r3, r2, #0x03              ; Make a copy of the ID, clearing off the bottom two bits
                                           ; There are four IDs per reg, by clearing the bottom two bits we get an address offset
        ADD     r3, r3, #0x1400            ; Now add the offset of the Priority Level registers from the base of the private peripheral space
        ADD     r0, r0, r3                 ; Now add in the base address of the private peripheral space, giving us the absolute address

        ; Now work out which ID in the register it is
        AND     r2, r2, #0x03              ; Clear all but the bottom two bits, leaves which ID in the reg it is (which byte)
        LSL     r2, r2, #3             ; Multiply by 8, this gives a bit offset

        ; Read -> Modify -> Write
        MOV     r12, #0xFF                 ; Mask (8 bits)
        LSL     r12, r12, r2           ; Move mask into correct bit position

        LDR     r3, [r0]                   ; Read current value of the Priority Level register (ICDIPR)
		AND     r3, r3, r12
        LSR     r3, r3, r2           ; Move mask into correct bit position

        ;MOV     r12, #0xFF                 ; Mask (8 bits)
        ;MOV     r1, r1, LSL r2             ; Also, move passed in priority value into correct bit position

        ;BIC     r3, r3, r12                ; Clear appropriate field
		MOV     r0, r3

        BX      lr
        ENDP


;==============================================================
; enableGICProcessorInterface
;==============================================================

        EXPORT enableGICProcessorInterface
        ; void enableGICProcessorInterface(void)
        ; Enables the processor interface
        ; Must be done on each core separately
enableGICProcessorInterface PROC
        MOV     r0, #GIC_BASE_ADDR         ; Read periph base address

		MOV     r2, #0x2000
		ADD     r2, r2, r0;
        LDR     r1, [r2]           ;Read the Processor Interface Control register   (ICCICR/ICPICR)
        ORR     r1, r1, #0x01              ;Bit 0: group0 interrupts, Bit 1: group1 interrupts
        STR     r1, [r2]           ; Write the Processor Interface Control register  (ICCICR/ICPICR)

        BX      lr
        ENDP


;==============================================================
; disableGICProcessorInterface
;==============================================================

        EXPORT disableGICProcessorInterface
        ; void disableGICProcessorInterface(void)
        ; Disables the processor interface
        ; Must be done on each core separately
disableGICProcessorInterface PROC

        MOV     r0, #GIC_BASE_ADDR         ; Read periph base address

		MOV     r2, #0x2000
		ADD     r2, r2, r0;
        LDR     r1, [r2]           ; Read the Processor Interface Control register   (ICCICR/ICPICR)
        BIC     r1, r1, #0x01               ; Bit 0: group0 interrupts, Bit 1: group1 interrupts
        STR     r1, [r2]           ; Write the Processor Interface Control register  (ICCICR/ICPICR)

        BX      lr
        ENDP


;==============================================================
; setPriorityMask
;==============================================================

        EXPORT setPriorityMask
        ; void setPriorityMask(unsigned int priority)
        ; Sets the Priority mask register for the CPU run on
        ; The reset value masks ALL interrupts!
setPriorityMask PROC

        ; Get base address of private peripheral space
        MOV     r1, #GIC_BASE_ADDR         ; Read periph base address

		MOV     r2, #0x2004;
		ADD     r2, r2, r1
        STR     r0, [r2]          ; Write the Priority Mask register (ICCPMR/ICCIPMR)

        BX      lr
        ENDP
 
;==============================================================
; setBinaryPoint
;==============================================================
       EXPORT  setBinaryPoint
       ; void setBinaryPoint(unsigned int priority)
       ; Sets the Binary Point Register for the CPU run on
setBinaryPoint PROC

       ; Get base address of private peripheral space
       MRC     p15, 4, r1, c15, c0, 0  ; Read periph base address

	   MOV     r2, #0x2008;
	   ADD     r2, r2, r0;
       STR     r0, [r2]       ; Write the Binary register   (ICCBPR/ICCBPR)

       BX      lr
       ENDP

;==============================================================
; readIntAck
;==============================================================

       EXPORT  readIntAck
       ; unsigned int readIntAck(void)
       ; Returns the value of the Interrupt Acknowledge Register
readIntAck PROC
       MRC     p15, 4, r0, c15, c0, 0      ; Read periph base address

       MOV     r2, #0x200c
       ADD     r2, r2, r0
       LDR     r0, [r2]               ; Read the Interrupt Acknowledge Register  (ICCIAR)
       BX      lr
       ENDP

;==============================================================
; readIntPendStatus
;==============================================================

       EXPORT  readIntPendStatus
       ; unsigned int readIntPendStatus(int id)
       ; Returns the value of the Interrupt Pend Status for SPI
readIntPendStatus PROC
	   ; r0 = id
	   MOV     r1, r0                      ; backup r0 register
       ADD     r1, r1, #32                 ; first 32 to sgi/ppi
       ;MRC     p15, 4, r0, c15, c0, 0      ; Read periph base address
       MOV     r0, #GIC_BASE_ADDR         ; Read periph base address
       
       ; r0 = base addr
       ; r1 = ID
       MOV     r2, #0x1200
       ADD     r2, r2, r0

       LSR     r3, r1, #5
       LSL     r3, r3, #2
       ADD     r2, r2, r3 

	   AND     r1, r1, #0x1F              ; Mask off to give offset within 32bit block
       MOV     r3, #1                     ; Move enable value into r3
       LSL     r3, r3, r1             ; Shift it left to position of ID in 32 bit block

	   LDR     r0, [r2]           		  ; Read the GICD_ISPENDRn Register

	   AND     r0, r0, r3

	   LSR     r0, r0, r1

       BX      lr
       ENDP

;==============================================================
; setIntPend
;==============================================================

       EXPORT  setIntPend
       ; void setIntPend(int id)
       ; set interrupt pend status
setIntPend PROC
	   ; r0 = id
	   MOV     r1, r0                      ; backup r0 register
       ADD     r1, r1, #32                 ; first 32 to sgi/ppi
       MOV     r0, #GIC_BASE_ADDR         ; Read periph base address

       MOV     r2, #0x1200
       ADD     r2, r2, r0

       LSR     r3, r1, #5
       LSL     r3, r3, #2
       ADD     r2, r2, r3

	   AND     r1, r1, #0x1F              ; Mask off to give offset within 32bit block
       MOV     r3, #1                     ; Move enable value into r3
       LSL     r3, r3, r1             ; Shift it left to position of ID in 32 bit block

;	   LDR     r0, [r2]           			; Read the GICD_ISPENDRn Register
;	   ORR     r0, r0, r3
;	   STR     r0, [r2]
	   STR	   r3, [r2]

       BX      lr
       ENDP

;==============================================================
; clearIntPend
;==============================================================

       EXPORT  clearIntPend
       ; void clearIntPend(int id)
       ; clear interrupt pend status
clearIntPend PROC
	   ; r0 = id
	   MOV     r1, r0                      ; backup r0 register
       ADD     r1, r1, #32                 ; first 32 to sgi/ppi
       MOV     r0, #GIC_BASE_ADDR         ; Read periph base address

       MOV     r2, #0x1280
       ADD     r2, r2, r0

       LSR     r3, r1, #5
       LSL     r3, r3, #2
       ADD     r2, r2, r3

	   AND     r1, r1, #0x1F              ; Mask off to give offset within 32bit block
       MOV     r3, #1                     ; Move enable value into r3
       LSL     r3, r3, r1             ; Shift it left to position of ID in 32 bit block

;	   LDR     r0, [r2]
;	   ORR     r0, r0, r3                 ; Read the GICD_ISPENDRn Register
;	   STR     r0, [r2]         		  ; Write the GICD_ISPENDRn Register
	   STR     r3,[r2]
	   
       BX      lr
       

       ENDP


;==============================================================
; setIntGroup
;==============================================================
       EXPORT  setIntGroup
       ; void setIntGroup(int id, int group)
       ; set interrupt group
setIntGroup PROC
	   ; r0 = id
	   ; r1 = group, 0:group 0, 1: group 1
	   MOV     r2, r0                      ; backup r0 register
       ADD     r2, r2, #32                 ; first 32 to sgi/ppi
       MOV     r0, #GIC_BASE_ADDR         ; Read periph base address

       MOV     r3, #0x1080
       ADD     r3, r3, r0

       LSR     r4, r2, #5
       LSL     r4, r4, #2
       ADD     r3, r3, r4 

	   AND     r5, r2, #0x1F              ; Mask off to give offset within 32bit block
       MOV     r4, #1                     ; Move enable value into r4
       LSL     r4, r4, r5             ; Shift it left to position of ID in 32 bit block

	   LDR     r0, [r3]                   ; Read the GICD_IGROUPRn Register
	   BIC     r0, r0, r4                 
	   ;ORR     r0, r1, LSL r5         ;暂时屏蔽，编译不过
	   STR     r0, [r3]         		  ; Write the GICD_IGROUPRn Register

       BX      lr
       ENDP


;==============================================================
; select cpu target to deal corresponding interrupt
;==============================================================
       EXPORT  selectIntCpu
       ; void selectIntCpu(int id, int cpu)
       ; select cpu target to deal corresponding interrupt.
selectIntCpu PROC
	   ; r0 = id
	   ; r1 = cpu 
	   MOV     r2, r0                      ; backup r0 register
       ADD     r2, r2, #32                 ; first 32 to sgi/ppi
       MOV     r0, #GIC_BASE_ADDR         ; Read periph base address

       MOV     r3, #0x1800
       ADD     r3, r3, r0

       LSR     r4, r2, #2
       LSL     r4, r4, #2
       ADD     r3, r3, r4

	   AND     r5, r2, #0x3               ; Mask off to give offset within 32bit block
	   LSL     r5, r5, #3

       MOV     r4, #0xFF                  
       LSL     r4, r4, r5             ; Shift it left to position of ID in 32 bit block

	   LDR     r0, [r3]                   ; Read the GICD_ITARGETSRn Register
	   BIC     r0, r0, r4                 
	   ;ORR     r0, r1, LSL r5        ;暂时屏蔽，编译不过
	   STR     r0, [r3]         		  ; Write the GICD_ITARGETSRn Register

       BX      lr
       ENDP


;==============================================================
; setIntActive
;==============================================================

       EXPORT  setIntActive
       ; void setIntActive(int id)
       ; set interrupt active status
setIntActive PROC
	   ; r0 = id
	   MOV     r1, r0                      ; backup r0 register
       ADD     r1, r1, #32                 ; first 32 to sgi/ppi
       MOV     r0, #GIC_BASE_ADDR         ; Read periph base address

       MOV     r2, #0x1300
       ADD     r2, r2, r0

       LSR     r3, r1, #5
       LSL     r3, r3, #2
       ADD     r2, r2, r3

	   AND     r1, r1, #0x1F              ; Mask off to give offset within 32bit block
       MOV     r3, #1                     ; Move enable value into r3
       LSL     r3, r3, r1             ; Shift it left to position of ID in 32 bit block

	   LDR     r0, [r2]           		  ; Read the GICD_ISACTIVERn Register

	   ORR     r0, r0, r3

	   STR     r0, [r2]

       BX      lr
       ENDP

;==============================================================
; clearIntActive
;==============================================================

       EXPORT  clearIntActive
       ; void clearIntActive(int id)
       ; clear interrupt active status
clearIntActive PROC
	   ; r0 = id
	   MOV     r1, r0                      ; backup r0 register
       ADD     r1, r1, #32                 ; first 32 to sgi/ppi
       MOV     r0, #GIC_BASE_ADDR         ; Read periph base address

       MOV     r2, #0x1380
       ADD     r2, r2, r0

       LSR     r3, r1, #5
       LSL     r3, r3, #2
       ADD     r2, r2, r3

	   AND     r1, r1, #0x1F              ; Mask off to give offset within 32bit block
       MOV     r3, #1                     ; Move enable value into r3
       LSL     r3, r3, r1             ; Shift it left to position of ID in 32 bit block

	   LDR     r0, [r2]
	   ORR     r0, r0, r3                 ; Read the GICD_ISACTIVERn Register
	   STR     r0, [r2]         		  ; Write the GICD_ISACTIVERn Register

       BX      lr

       ENDP



;==============================================================
; set interrupt type(level ,edge, 1-N or N-N model)
;==============================================================
       EXPORT  setIntType
       ; void setIntType(int id, int trigger_type, int mode)
       ; set interrupt type.
setIntType PROC
	   ; r0 = id
	   ; r1 = trigger type, 0:level, 1: edge;
	   ; r2 = gic mode, 0: N-N model,1: 1-N model
	   MOV     r3, r0                      ; backup r0 register
       ADD     r3, r3, #32                 ; first 32 to sgi/ppi
       MOV     r0, #GIC_BASE_ADDR         ; Read periph base address

       MOV     r4, #0x1c00
       ADD     r4, r4, r0

       LSR     r5, r3, #4
       LSL     r5, r5, #2
       ADD     r4, r4, r5

	   AND     r5, r3, #0x0F               ; Mask off to give offset within 32bit block

	   LSL     r5, r5, #1

       MOV     r6, #0x3
       LSL     r6, r6, r5             ; Shift it left to position of ID in 32 bit block

	   LDR     r0, [r4]                   ; Read the GICD_ICFGRn Register
	   BIC     r0, r0, r6

	   LSL     r1, r1, #1
	   ORR     r1, r1, r2
	   ;ORR     r0, r0, r1, LSL r5        ;暂时屏蔽，编译不过
	   STR     r0, [r4]         		  ; Write the GICD_ICFGRn Register

       BX      lr
       ENDP



;==============================================================
; writeEOI
;==============================================================


       EXPORT  writeEOI
       ; void writeEOI(unsigned int ID)
       ; Writes ID to the End Of Interrupt register
writeEOI PROC

       ; Get base address of private peripheral space
       ADD     r0, r0, #32                 ; first 32 to sgi/ppi
       MOV     r1, #GIC_BASE_ADDR         ; Read periph base address

       MOV     r2, #0x2010
       ADD     r2, r2, r1
       STR     r0, [r2]           ; Write ID to the End of Interrupt register (ICCEOIR)

       BX      lr
       ENDP

;==================================================================
; SGI
;==================================================================

       EXPORT sendSGI
       ; void sendSGI(unsigned int ID, unsigned int target_list, unsigned int filter_list);
       ; Send a software generate interrupt
sendSGI PROC
       AND     r3, r0, #0x0F               ; Mask off unused bits of ID, and move to r3
       AND     r1, r1, #0x0F               ; Mask off unused bits of target_filter
       AND     r2, r2, #0x0F               ; Mask off unused bits of filter_list

       LSL     r1, r1, #16
       ORR     r3, r3, r1                  ; Combine ID and target_filter
       LSL     r2, r2, #24
       ORR     r3, r3, r2                  ; and now the filter list

       ; Get the address of the GIC
       MOV     r0, #GIC_BASE_ADDR         ; Read periph base address
       ADD     r0, r0, #0x1F00             ; Add offset of the sgi_trigger reg

       STR     r3, [r0]                    ; Write to the Software Generated Interrupt Register  (ICDSGIR)

       BX      lr
       ENDP

;==================================================================
; SGI
;==================================================================

       EXPORT getSGIINTID
       ; void getSGIINTID();
       ; get a software generate interrupt
getSGIINTID PROC
       ; Get the address of the GIC
       MOV     r0, #GIC_BASE_ADDR         ; Read periph base address
	   MOV     r1, #0x200C
	   ADD     r1, r1, r0;
       LDR     r3, [r1]                    ; Write to the Software Generated Interrupt Register  (ICDSGIR)

       MOV     r2, #0x0
       AND     r3, r3, #0xF00               ; Mask off unused bits of target_filter
       LSR     r3, r3, #8
       ORR     r2, r2, r3                   ; Combine ID and target_filter

       BX      lr
       ENDP



;==========System Control========================================================


;==============================================================
; set vector
;==============================================================
	EXPORT  set_vector
	; void set_vector(int vector)
	; set application vector
set_vector PROC
		; r0= vector
        MCR   p15, 0, r0, c12, c0, 0
		BX    lr
		ENDP


;==================================================================
; enable branch prediction
;==================================================================
	EXPORT  enable_branch_prediction
	; void enable_branch_prediction
	; enable branch_prediction
enable_branch_prediction PROC
	  MRC    p15, 0, r0, c1, c0, 0       ;Read System Control Register
	  ORR    r0, r0, #(0x1 << 11)         ;Set bit11,Enable branch prediction
	  MCR    p15, 0, r0, c1, c0, 0       ;Write System Control Register

	  BX     lr
	  ENDP


;==============================================================
; disable branch prediction
;==============================================================
	EXPORT  disable_branch_prediction
	; void disable_branch_prediction
	; disable branch_prediction
disable_branch_prediction PROC
	  MRC    p15, 0, r0, c1, c0, 0       ;Read System Control Register
	  BIC    r0, r0, #(0x1 << 11)         ;clear bit11,disable branch prediction
	  MCR    p15, 0, r0, c1, c0, 0       ;Write System Control Register

	  BX     lr
	  ENDP

;==================================================================
; access flag enable control(AP[0] control)
;==================================================================
	EXPORT  enable_access_flag
	; void enable_access_flag(void)
	; enable access flag
enable_access_flag PROC
	  MRC    p15, 0, r0, c1, c0, 0       ;Read System Control Register
	  ORR    r0, r0, #(0x1 << 29)        ;Set bit29,Enable access flag
	  MCR    p15, 0, r0, c1, c0, 0       ;Write System Control Register

	  BX     lr
	  ENDP


	EXPORT  disable_access_flag
	; void disable_access_flag(void)
	; disable access flag
disable_access_flag PROC
	  MRC    p15, 0, r0, c1, c0, 0       ;Read System Control Register
	  BIC    r0, r0, #(0x1 << 29)         ;clear bit29,disable access flag
	  MCR    p15, 0, r0, c1, c0, 0       ;Write System Control Register

	  BX     lr
	  ENDP

;==================================================================
; TEX remap enable control
;==================================================================
	EXPORT  enable_tex_remap
	; void enable_tex_remap
	; enable remapping of the TEX[2:1]bits for use as two translation table bits.
enable_tex_remap PROC
	  MRC    p15, 0, r0, c1, c0, 0       ;Read System Control Register
	  ORR    r0, r0, #(0x1 << 28)        ;Set bit28,Enable tex
	  MCR    p15, 0, r0, c1, c0, 0       ;Write System Control Register

	  BX     lr
	  ENDP


	EXPORT  disable_tex_remap
	; void disable_tex_remap
	; disable remaping of the TEX[2:1]bits
disable_tex_remap PROC
	  MRC    p15, 0, r0, c1, c0, 0       ;Read System Control Register
	  BIC    r0, r0, #(0x1 << 28)         ;clear bit28,disable tex
	  MCR    p15, 0, r0, c1, c0, 0       ;Write System Control Register

	  BX     lr
	  ENDP

;==================================================================
; align fault control
;==================================================================
   EXPORT enable_align_fault
   ; void enable_align_fault(void)
   ; enable align fault
enable_align_fault  PROC
   	MRC 	p15,  0, r0, c1, c0, 0
   	ORR 	r0,	 r0, #(0x1 << 1)         ; set bit1, enable align fault
   	MCR 	p15,  0, r0, c1, c0, 0
    BX      lr
	ENDP


   EXPORT disable_align_fault
disable_align_fault  PROC
   	MRC 	p15,  0, r0, c1, c0, 0
   	BIC 	r0,	 r0, #(0x1 << 1)         ; clear bit1, disable align fault
   	MCR 	p15,  0, r0, c1, c0, 0
    BX      lr
	ENDP




;======== MMU. Memory Management System Control=======================

;==============================================================
; enable mmu
;==============================================================
	EXPORT  enable_mmu
	; void enable_mmu(void)
	; enable mmu control
enable_mmu PROC
	  MRC    p15, 0, r0, c1, c0, 0       ;Read System Control Register
	  ORR    r0, r0, #0x1                    ;Set bit0,Enable MMU
	  MCR    p15, 0, r0, c1, c0, 0       ;Write System Control Register

	  BX     lr
	  ENDP


;==============================================================
; disable mmu
;==============================================================
	EXPORT disable_mmu
	; void disable_mmu(void)
	; disable mmu control
disable_mmu PROC
	  MRC    p15, 0, r0, c1, c0, 0       ;Read System Control Register
	  BIC    r0, r0, #0x1                ;clear bit0, disable MMU
	  MCR    p15, 0, r0, c1, c0, 0       ;Write System Control Register

	  BX     lr

	  ENDP



;==============================================================
; set mmu ttb
;==============================================================
   EXPORT set_mmu_ttbase
   	; void set_mmu_ttbase(int address)
	; set mmu ttb base address, Cortex-A7 supports two translation tables
    ; Configure translation table base (TTB) control register cp15,c2
    ; to a value of all zeros, indicates we are using TTB register 0.
set_mmu_ttbase PROC    			;  /* 设置TTB地址  */
		;r0 = address
		LDR     r1,=0xFFFFC000
		AND     r0, r0, r1
        MOV     r1,#0x0
        MCR     p15, 0, r1, c2, c0, 2

        ; write the address of our page table base to TTB register 0
        MOV     r1, #0x08                  ; RGN=b01  (outer cacheable write-back cached, write allocate)
                                           ; S=0      (translation table walk to non-shared memory)
        ORR     r1,r1,#0x40                ; IRGN=b01 (inner cacheability for the translation table walk is Write-back Write-allocate)

        ORR     r0,r0,r1
        MCR     p15, 0, r0, c2, c0, 0

   	    BX    lr
   	    ENDP



;==============================================================
; set mmu domain
;==============================================================
   EXPORT set_mmu_domain
   ; void set_mmu_domain(int domain)
   ; set mmu domain
set_mmu_domain  PROC   				; /* 设置MMU domain */
   	  MCR  p15, 0, r0, c3, c0, 0
      BX   lr
      ENDP



;==============================================================
; invalidate unified tlb
;==============================================================
 EXPORT invalidate_unified_tlb           ; /* 无效统一的TLB */
    ; void invalidate_unified_tlb(void)
    ; invalidate unified TLB
invalidate_unified_tlb  PROC
   	 MCR  p15, 0, r0, c8, c7, 0
     BX   lr
     ENDP



;==============================================================
; invalidate instruction tlb
;==============================================================
    EXPORT invalidate_itlb                 ; /* 无效指令TLB */
    ; void invalidate_itlb(void)
    ; invalidate instruction TLB
invalidate_itlb   PROC
   	 MCR  p15, 0, r0, c8, c5, 0
     BX   lr
     ENDP



;==============================================================
; invalidate instruction tlb by mva
;==============================================================
    EXPORT invalidate_itlb_mva              ; /* 按照MVA地址方式无效指令TLB */
    ; void invalidate_itlb_mva(int mva)
    ; invalidate data TLB entry by MVA
invalidate_itlb_mva   PROC
   	  ;ro=mva
   	  MCR  p15, 0, r0, c8, c5, 1
      BX   lr
      ENDP


;==============================================================
; invalidate data tlb
;==============================================================
	EXPORT  invalidate_dtlb                ; /* 无效数据TLB */
	; void invalidate_dtlb(void)
	; invalidate data TLB
invalidate_dtlb PROC
	  MCR   p15, 0, r0, c8, c6, 0
	  BX    lr
	  ENDP


;==============================================================
; invalidate data tlb by mva
;==============================================================
	EXPORT invalidate_dtlb_mva            ; /* 按照MVA地址方式无效数据TLB */
	; void invalidate_dtlb_mva(int mva)
invalidate_dtlb_mva PROC
	  ;r0=mva
	  MCR   p15, 0, r0, c8, c6, 1
	  BX    lr
	  ENDP


;================== CACHE  Control==================================

;==============================================================
; get size of instruction cache
;==============================================================
	EXPORT  get_cache_size
	; void get_cache_size(unsigned int cache_type)
	; get cache size,includ icache dcache L2_cache
get_cache_size PROC
      MCR    p15, 2, r0, c0, c0, 0       ;write CSSELR reg ,select cache_type
	  MRC    p15, 1, r0, c0, c0, 0       ;Read System Control Register

	  BX     lr
	  ENDP

;==============================================================
; enable instruction cache
;==============================================================
	EXPORT  enable_icache
	; void enable_icache(void)
	; enable icache
enable_icache PROC
	  MRC    p15, 0, r0, c1, c0, 0       ;Read System Control Register
	  ORR    r0, r0, #(0x1 << 12)        ;Set bit12,Enable Icache
	  MCR    p15, 0, r0, c1, c0, 0       ;Write System Control Register

	  BX     lr
	  ENDP


;==============================================================
; disable instruction cache
;==============================================================
	EXPORT  disable_icache
	; void disable_icache(void)
	; disable icache
disable_icache PROC
	  MRC    p15, 0, r0, c1, c0, 0       ;Read System Control Register
	  BIC    r0, r0, #(0x1 << 12)        ;Clear bit12,Disable Icache
	  MCR    p15, 0, r0, c1, c0, 0       ;Write System Control Register

	  BX     lr
	  ENDP


;==============================================================
; enable data cache
;==============================================================
	EXPORT  enable_dcache
	; void enable_dcache(void)
	; enable dcache
enable_dcache PROC
	  MRC    p15, 0, r0, c1, c0, 0       ;Read System Control Register
	  ORR    r0, r0, #(0x1 << 2)          ;Set bit2,Enable Icache
	  MCR    p15, 0, r0, c1, c0, 0       ;Write System Control Register

	  BX     lr
	  ENDP


;==============================================================
; disable data cache
;==============================================================
	EXPORT  disable_dcache
	; void disable_dcache(void)
	; disable dcache
disable_dcache PROC
	  MRC    p15, 0, r0, c1, c0, 0       ;Read System Control Register
	  BIC    r0, r0, #(0x1 << 2)          ;Clear bit2,Disable Icache
	  MCR    p15, 0, r0, c1, c0, 0       ;Write System Control Register

	  BX     lr
	  ENDP

;==============================================================
; enable ACTLR SMP
;==============================================================
	EXPORT  enable_ACTLR_SMP
	; void enable_ACTLR_SMP(void)
	; enable_ACTLR_SMP
enable_ACTLR_SMP PROC
	  MRC    p15, 0, r0, c1, c0, 1       ;Read Auxiliary Control Register
	  ORR    r0, r0, #0x40               ;set bit6(SMP),enable CPU to receive cache and TLB from other CPUs
	  MCR    p15, 0, r0, c1, c0, 1       ;Write Auxiliary Control Register

	  BX     lr
	  ENDP

;==============================================================
; disable ACTLR SMP
;==============================================================
	EXPORT  disable_ACTLR_SMP
	; void disable_ACTLR_SMP(void)
	; disable_ACTLR_SMP
disable_ACTLR_SMP PROC
	  MRC    p15, 0, r0, c1, c0, 1       ;Read Auxiliary Control Register
	  BIC    r0, r0, #0x40               ;set bit6(SMP),enable CPU to receive cache and TLB from other CPUs
	  MCR    p15, 0, r0, c1, c0, 1       ;Write Auxiliary Control Register

	  BX     lr
	  ENDP

;==============================================================
; invalidate instruction cache
;==============================================================
   EXPORT invalidate_icache
   ; void invalidate_icache(void)
   ; invalidate all instruction cache
invalidate_icache PROC					; /* 无效ICACHE */
   	  MCR  p15, 0, r0, c7, c5, 0
      BX   lr
   	  ENDP



;==============================================================
; invalidate instruction cache by mva
;==============================================================
   EXPORT invalidate_icache_mva
   ; void invalidate_icache_mva(int address)
   ; instruction cache invalidate by MVA to PoU
invalidate_icache_mva 	PROC			;	/* 无效ICACHE line,使用MVA  */
   	  ;r0=mva
   	  MCR  p15, 0, r0, c7, c5, 1
	  BX   lr
	  ENDP



;==============================================================
; invalidate data cache by mva
;==============================================================
   EXPORT invalidate_dcache_mva
   ; void invalidate_dcache_mva(int address)
   ; data cache invalidate by MVA to PoC
invalidate_dcache_mva PROC				;  /* 按照虚拟地址方式,无效DCACHE*/
   	  ;r0=mva
   	  MCR  p15, 0, r0, c7, c6, 1
      BX   lr
	  ENDP



;==============================================================
; invalidate data cache by set/way
;==============================================================
	EXPORT invalidate_dcache_index
	; void invalidate_dcache_index(int index)
	; data cache invalidate by MVA to Poc;
invalidate_dcache_index PROC             ;  /* 按照set/way方式,无效DCACHE*/
	 ; ro= index
	  MCR  p15, 0, r0, c7, c6, 2
      BX   lr
	  ENDP



;==============================================================
;  clean data cache by mva
;==============================================================
    EXPORT clean_dcache_mva
    ; void clean_dcache_mva(int address)
    ; data cache clean line by MVA to Poc
clean_dcache_mva  PROC   			; /* 按照虚拟地址方式清空DCACHE中某块, */
   	 ;r0=mva
   	  MCR  p15, 0, r0, c7, c11, 1
  	  BX   lr
  	  ENDP



;==============================================================
;  clean and invalidate data cache by mva
;==============================================================
   EXPORT clean_invalidate_dcache_mva
   ; void clean_invalidate_dcache_mva(int mva)
   ; data cache clean and invalidate line by MVA to Poc
clean_invalidate_dcache_mva  PROC     ; /*  按照虚拟地址方式清空并无效DCACHE中某line */
   	 ;r0=mva
   	  MCR   p15, 0, r0, c7, c14, 1
   	  BX    lr
   	  ENDP



;==============================================================
;  clean and invalidate data cache by set/way
;==============================================================
    EXPORT clean_invalidate_dcache_index
    ; void clean_invalidate_dcache_index(int index)
    ; data cache clean and invalidate line by set/way
clean_invalidate_dcache_index PROC   ; /*按照组号/组内序号方式清空并无效DCACHE中某line,*/
    ;r0=index
     MCR   p15, 0, r0, c7, c14, 2
     BX    lr
     ENDP


;==============================================================
;  clean data cache by set/way
;==============================================================
     EXPORT clean_dcache_index 				; /* 按照组号/组内序号 方式清空DCACHE中某line, */
     ; void clean_dcache_index(int index)
     ; data cache clean line by set/way
clean_dcache_index  PROC
   	 ;r0=index
   	 MCR  p15, 0, r0, c7, c10, 2
   	 BX   lr
   	 ENDP


;=========CPSR control====================================================================

;==============================================================
;  enable arm core irq interrupt
;==============================================================
     EXPORT enable_core_irq 				; /* enable CPSR irq */
     ; void enable_core_irq(void)
     ; clear CPSR register irq interrupt bit, enable arm core irq interrupt.
enable_core_irq  PROC
   	 MRS  r0, cpsr
   	 BIC  r0, r0, #0x80
   	 MSR  cpsr_c, r0
   	 BX   lr
   	 ENDP



;==============================================================
;   disable arm core irq interrupt
;==============================================================
     EXPORT disable_core_irq 				; /* disable CPSR irq */
     ; void disable_core_irq(void)
     ; set CPSR register irq interrupt bit, disable arm core irq interrupt.
disable_core_irq  PROC
   	 MRS  r0, cpsr
   	 ORR  r0, r0, #0x80
   	 MSR  cpsr_c, r0
   	 BX   lr
   	 ENDP   	



;==============================================================
;    enable arm core fiq interrupt
;==============================================================
     EXPORT enable_core_fiq 				; /* enable CPSR fiq */
     ; void enable_core_fiq(void)
     ; clear CPSR register fiq interrupt bit, enable arm core fiq interrupt.
enable_core_fiq  PROC
   	 MRS  r0, cpsr
   	 ORR  r0, r0, #0x40
   	 MSR  cpsr_c, r0
   	 BX   lr
   	 ENDP 


;==============================================================
;    disable arm core fiq interrupt
;==============================================================
     EXPORT disable_core_fiq 				; /* disable CPSR fiq */
     ; void disable_core_fiq(void)
     ; set CPSR register fiq interrupt bit, disable arm core fiq interrupt.
disable_core_fiq  PROC
   	 MRS  r0, cpsr
   	 BIC  r0, r0, #0x40
   	 MSR  cpsr_c, r0
   	 BX   lr
   	 ENDP 
   	 

        EXPORT getCPUID
        ; unsigned int getCPUID(void)
        ; Returns the CPU ID (0 to 3) of the CPU executed on
getCPUID PROC
        MRC     p15, 0, r0, c0, c0, 5      ; Read CPU ID register
        AND     r0, r0, #0x03              ; Mask off, leaving the CPU ID field
        BX      lr
        ENDP

;==================================================================
; End of code
;==================================================================

       NOP     ;;clean warning
       END


