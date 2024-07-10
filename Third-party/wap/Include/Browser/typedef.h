
/* typedef.h
 *
 * This file contains type definations.
 *
 */

#ifndef TYPEDEF_INCLUDE
#define TYPEDEF_INCLUDE

#ifdef __cplusplus
extern "C" {
#endif

/*==============================
 * DATA TYPE DEFINITIONS
 *==============================*/
typedef unsigned char  unsigned8;      /* unsigned 8-bit  integer */
typedef unsigned short unsigned16;     /* unsigned 16-bit integer */
typedef unsigned long  unsigned32;     /* unsigned 32-bit integer */
typedef signed char      signed8;      /* 8-bit  signed integer */
typedef signed short     signed16;     /* 16-bit signed integer */
typedef signed long      signed32;     /* 32-bit signed integer */

/*==============================
 * TOD set or not
 *==============================*/
#define TOD_set 1
#define TOD_no_set 0

/*==============================
 * STACK ALIGNMENT
 *==============================*/
#define STACK_ALIGNMENT 4

/*==============================
 * SEMAPHORE OR MAILBOX WAIT TYPE
 *==============================*/
#define PRIORITY         0
#define FIFO             1

/*==============================
 * SEMAPHORE USE INHERIT OR NOT
 *==============================*/
#define PRIO_INHERIT    30
#define PRIO_NO_INHERIT 31

/*==============================
 * Delta-Core configuration table
 *==============================*/
typedef struct{
  signed16 maxtask;                           /* maximum tasks */
  signed16 maxmail;                           /* maximum mailboxes */
  signed16 maxqueue;                          /* maximum queues */
  signed16 maxmsgsize;                        /* maximum message size */
  signed16 maxsema;                           /* maximum semaphores */
  signed16 maxasyn;                           /* maximum asynchronous */
  signed16 maxalarm;                          /* maximum alarms */
  signed16 maxpart;                           /* maximum partitions */
  unsigned32 slice;                           /* slices per task in round robin */
  unsigned32 micro_per_tick;                  /* microseconds per tick */
  signed16 fence;                             /* realtime fence */
  signed8 (*taskcptr)(signed16, signed16);    /* extension func pointer for task creation */
  signed8 (*taskdptr)(signed16, signed16);    /* extension func pointer for task deletion */
  signed8 (*tasksptr)(signed16, signed16);    /* extension func pointer for task switch */
  signed8 (*taskpptr)(signed16, signed16);    /* extension func pointer for task change pri */
  unsigned32 heapsize;                        /* heapsize */
  signed8 bkcount;                            /* number of heap control block */
  signed8 *heapaddress;                       /* heap address */
  unsigned8  debug_mode;                      /* debug mode. 0 - burn  0xmm - debug vector */
                                              /* mm - Enable Mask Int vector */
}configuration_table;

/*==============================
 * TASK WAIT FOR CHAR OPTION
 *==============================*/
typedef enum {
  WAITCHAR,
  NOWAITCHAR
}tgetc_option;

/*==============================
 * ALARM OPTION
 *==============================*/
typedef enum {
  SINGLE,
  REPETIVE
}alarm_option;

/*==============================
 * TIMESLICE OPTION
 *==============================*/
/* task timeslice options */
typedef enum {
  ENABLE_TIMESLICE,   /*enable the certain task's timeslice*/
  DISABLE_TIMESLICE   /*disable the certain task's timeslice*/
} task_timeslice_option;

/*==============================
 * TASK DELETE OPTION
 *==============================*/
typedef enum {
  NOT_FORCE_DELETE, /* do not free resource used by task when delete */
  FORCE_DELETE      /* free resource used by task when delete */
} task_delete_option;

/* clock get options */
typedef enum {
  CLOCK_GET_TOD,
  CLOCK_GET_SECONDS_SINCE_EPOCH,
  CLOCK_GET_TICKS_SINCE_BOOT,
  CLOCK_GET_TICKS_PER_SECOND,
  CLOCK_GET_TIME_VALUE
} clock_get_options;

typedef enum {
  TASKC,        /* extension func pointer for task creation */
  TASKD,        /* extension func pointer for task deletion */
  TASKS,        /* extension func pointer for task switch */
  TASKP,        /* extension func pointer for task change pri */
  ERR           /* extension func pointer for error processing */
} task_extension;

/* The following record defines the time of control block.  This
 * control block is used to maintain the current time of day. */
typedef struct {
  unsigned32 year;                 /* year, A.D. */
  unsigned32 month;                /* month, 1 -> 12 */
  unsigned32 day;                  /* day, 1 -> 31 */
  unsigned32 hour;                 /* hour, 0 -> 23 */
  unsigned32 minute;               /* minute, 0 -> 59 */
  unsigned32 second;               /* second, 0 -> 59 */
  unsigned32 ticks;                /* elapsed ticks between secs */
}   TOD_Control;

/* Standard flavor style to return TOD in for a clock_get option. */
typedef struct {
  unsigned32 seconds;
  unsigned32 microseconds;
} clock_time_value;

typedef struct {
  unsigned32 low_offsets_bits : 16;
  unsigned32 segment_selector : 16;
  unsigned32 fixed_value_bits : 8;
  unsigned32 gate_type	: 5;
  unsigned32 privilege	: 2;
  unsigned32 present		: 1;
  unsigned32 high_offsets_bits: 16;
}interrupt_gate_descriptor;

typedef void (*Dcore_irq_hdl)(void);


#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
