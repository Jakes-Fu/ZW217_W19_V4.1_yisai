MCFLAG_OPT 		= -D__APCS_SWST -D__APCS_INTERWORK -D__TARGET_FPU_NONE -D__BIG_ENDIAN

MASMFLAG_OPT	= -PD "TX_TIMER_INT_PER_TICK SETL {FALSE}"  -PD "TX_ASSERT_DEBUG SETL {TRUE}"  -PD "RTOS_ROM SETL {FALSE}"  -PD "TX_CHECK_STACK SETL {TRUE}" -PD "DEEP_SLEEP_NEED_EMPTY_FIFO SETL {FALSE}" 

MINCPATH		= RTOS/source/threadx/thumb  RTOS/export/inc \
			CMMB/source/os_al/include  CMMB/source/os_al/source/KD/threadx \
		CMMB/source/os_al/include/KD

MSRCPATH		= CMMB/source/os_al/source/KD/threadx

SOURCES     	= threads.c memory.c error.c events.c string.c assert_log.c start_exit.c time.c timer.c
