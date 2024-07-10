
ENABLE_VFP           EQU    0x40000000
DISABLE_VFP          EQU    0x00000000

                              AREA VFP_PROCESS, CODE, READONLY
                              CODE32

;Enable then restore saved registers of VFP

                            EXPORT __vfp_enable
__vfp_enable                FUNCTION
                            STMDB R13!, {R0-R3}
                            ;1.Enable VFP
                            MOV R0, #ENABLE_VFP
                            FMXR FPEXC, R0
                            LDMIA R13!, {R0-R3}
                            BX LR
                            ;No space to save a new task's cxt,may be the MAX_VFP_GROUP_NUM is too small??
                            ENDFUNC

;disable vfp
                            EXPORT __vfp_disable
__vfp_disable               FUNCTION
                            STMDB R13!, {R0-R3} 
                            MOV R0, #DISABLE_VFP
                            FMXR FPEXC, R0
                            LDMIA R13!, {R0-R3}
                            BX LR
                            ENDFUNC 

                            END
