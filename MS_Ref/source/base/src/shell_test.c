/* Test Code for Shell. */


#ifdef SHELL_DEBUG_SUPPORT
#include "bt_abs.h"
#include "diag.h"
#include "sci_types.h"
#include "os_api.h"
#include "os_apiext.h"
#include "threadx_os.h"
#include "version.h"
#include "os_import.h"
#include "threadx_assert.h"

#include "ms_ref_base_trc.h" 
#ifndef BLUETOOTH_SUPPORT_NONE
#define SHELL_CMD_BT_TEST
#endif
#ifdef WIFI_SUPPORT
#define SHELL_CMD_WLAN_TEST
#endif
LOCAL BT_ADDRESS addr;

#define SHELL_DEBUG 1
#if (SHELL_DEBUG == 1)
#define SHELL_TRACE(_x_) SCI_TraceLow _x_
#define SHELL_ASSERT(_x_) SCI_Assert(_x_)
#else
#define SHELL_TRACE(_x_)      ((void)0)
#endif

/* Funtions declarations. */
//extern SCI_THREAD_T	*s_static_thread_ptr_table[];
extern const char * SCI_GetRtosCompileDate(void);
extern const char * GetRefCompileDate(void);

LOCAL void SHELL_stackInfoShow(int argc, char *argv[]);
LOCAL void SHELL_taskInfoShow(int argc, char *argv[]);
LOCAL void SHELL_cpuStatsShow(int argc, char *argv[]);
LOCAL void SHELL_versionShow(int argc, char *argv[]);
LOCAL void SHELL_systemAssert(int argc, char *argv[]);
LOCAL void SHELL_systemReboot(int argc, char *argv[]);

LOCAL uint32 Shell_getNumFromStr(uint32 *value_ptr, char * src_ptr)
{
	char	base[3];
	char    *end_ptr;

    // Check input parameter.
    if (SCI_NULL == src_ptr)
    {
        return SCI_ERROR;
    }
    
    // Get base of the value.
	base[0] = *src_ptr;     
	base[1] = *(src_ptr + 1);
	base[2] = '\0';	

	// Check the base of value, then convert from string to uint32.
	if ((0 == strcmp((char *)base, "0B")) || (0 == strcmp((char *)base, "0b")))
	{
        // Value base is 2.
		*value_ptr = strtoul((char *)(src_ptr + 2), &end_ptr, 2);
	}
	else if ((0 == strcmp((char *)base, "0O")) || (0 == strcmp((char *)base, "0o")))
	{
        // Value base is 8.
		*value_ptr = strtoul((char *)(src_ptr + 2), &end_ptr, 8);
	}
	else if ((0 == strcmp((char *)base, "0X")) || (0 == strcmp((char *)base, "0x")))
	{
        // Value base is 16.
		*value_ptr = strtoul((char *)(src_ptr + 2), &end_ptr, 16);
	}
	else if ((0 == strcmp((char *)base, "0D")) || (0 == strcmp((char *)base, "0d")))
	{
        // Value base is 10.
		*value_ptr = strtoul((char *)(src_ptr + 2), &end_ptr, 10);
	}
	else if ((base[0] >= '0') && (base[0] <= '9'))
	{
        // Value base is 10.
		*value_ptr = strtoul((char *)src_ptr, &end_ptr, 10);
	}
	else
	{
        // Error src_ptr.
		end_ptr = src_ptr;
		*value_ptr = 0;
		
		return SCI_ERROR;
	}

    return SCI_SUCCESS;	
}


LOCAL void SHELL_memLeakCheck(int argc, char *argv[])
{
    uint8    *mem_info_ptr = NULL;
    uint32    mem_info_len = 0;
    uint32   total_list_len = 0;
    uint32   send_len = 0;
    uint32   mem_offset = 0;
    
    total_list_len =  SCI_GetMemInfoCount();
    //SHELL_TRACE:"SHELL_memLeakCheck!Total Memory Count=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_94_112_2_18_1_5_38_61,(uint8*)"d", total_list_len);

    if(total_list_len > 0)
    {
          mem_info_ptr    = (uint8*) SCI_ALLOC_APP(MAX_MEM_INFO_NUM* sizeof(MEM_PACK_T));
          if( mem_info_ptr == NULL )
          {
                //SCI_TRACE_LOW:"SHELL_memLeakCheck:Allocate mem failure!\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_101_112_2_18_1_5_38_62,(uint8*)"");
               return;
          }    

          DIAG_SendInfoHead(SHELL_DATA_MEM_INFO, total_list_len * sizeof(MEM_PACK_T));          
          mem_offset = 0; 
          while(mem_offset < total_list_len )
          {
                send_len = total_list_len - mem_offset;
                if( send_len >  MAX_MEM_INFO_NUM)
                {
                        send_len = MAX_MEM_INFO_NUM;
                }
                 
                /* Allocate the memory to load the memory info into it. */
                mem_info_len = send_len* sizeof(MEM_PACK_T);      
               if( SCI_MemInfoGet( mem_info_ptr, mem_info_len , mem_offset) == MEM_INFO_GETTING_COMPLETE )
                {
					//SCI_TRACE_LOW:"SHELL_memLeakCheck: Memory Offset=%d\r\n"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_119_112_2_18_1_5_38_63,(uint8*)"d", mem_offset);
					DIAG_SendInfoData(SHELL_DATA_MEM_INFO, mem_info_ptr, mem_info_len);
					mem_info_len = 0;
					mem_offset += send_len;
                }
			    else
			    {
			    	//SCI_TRACE_LOW:"SHELL_memLeakCheck: Memory Get Error\r\n"
			    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_126_112_2_18_1_5_38_64,(uint8*)"");
					break;
			    }

          }
          DIAG_SendInfoEnd(SHELL_DATA_MEM_INFO);
          SCI_FREE(mem_info_ptr);
    }

}

LOCAL void SHELL_memDump(int argc, char *argv[])
{
    int i;
    uint32 mem_addr, mem_len, * value_ptr, print_array_cnt;
    
    //SHELL_TRACE:"[SHELL] SHELL_memDump: argc=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_142_112_2_18_1_5_38_65,(uint8*)"d",argc );
    
    for(i=0; i<argc; i++)
    {
        //SHELL_TRACE:"[SHELL] SHELL_memDump: argv[%d]=%s\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_146_112_2_18_1_5_38_66,(uint8*)"ds",i,argv[i] );
    }

    if(argc < 3 )
    {
        DIAG_SendFormatInfo("SHELL_memDump: Input args are not enough");
        return;
    }
    
    /* Get the dump memory address. */
    if( SCI_SUCCESS != Shell_getNumFromStr(&mem_addr, argv[0]))
    {
        DIAG_SendFormatInfo("SHELL_memDump: Input parameters error!"); 
        return;
    }

    /* Get the dump memory len. */
    if( SCI_SUCCESS != Shell_getNumFromStr(&mem_len, argv[1]))
    {
         DIAG_SendFormatInfo("SHELL_memDump: Input parameters error!");       
         return;
    }

    DIAG_SendFormatInfo("   ADDR: 0x%08x,  LEN: 0x%08x \n", mem_addr, mem_len);
    
    /* Get the dump memory type. */
    if(0 == memcmp(argv[2], "b", 1))
    {
        DIAG_SendInfo(SHELL_DATA_BINARY, (uint8 *)mem_addr, mem_len);   
        DIAG_SendFormatInfo("Dump over!\n");
    }
    else if(0 == memcmp(argv[2], "t", 1))
    {
        //DIAG_SendInfo(SHELL_DATA_TEXT, (uint8 *)mem_addr, mem_len);
        value_ptr = (uint32 *)mem_addr;

        print_array_cnt = ((mem_len/16) == 0) ? 1 : (mem_len/16);
        
        for(i=0; i<(int)print_array_cnt; i++)
        {
            DIAG_SendFormatInfo("   %08x: %08x %08x %08x %08x    \t\r", &value_ptr[i*4], value_ptr[i*4], value_ptr[i*4+1], value_ptr[i*4+2], value_ptr[i*4+3]);
        }
        DIAG_SendFormatInfo("   Dump over!\n");
      
    }
    else
    {
        DIAG_SendFormatInfo("SHELL_memDump: Input parameters error!");                
    }

    return;
}

LOCAL void SHELL_MemStatus(int argc, char *argv[])
{
    uint32 flag = POOL_INFO_SEEK_FIRST;
	uint32 status = SCI_SUCCESS;
	SYS_POOL_INFO_T   sys_pool;
	
    DIAG_SendFormatInfo("\n\tPool_Name\tStart_Addr\tEnd_Addr\tTotal_Num\tMax_Used\tAvail_Num\n");
	do
	{
	     status = SCI_GetSysPoolInfo(&sys_pool, flag);
         if((SCI_SUCCESS == status) || (SCI_POOL_INFO_END == status))
         {
 		 	DIAG_SendFormatInfo("\t%-s\t0x%-08X\t0x%-08X\t%-8ld\t%-8ld\t%-8ld\n", sys_pool.name, sys_pool.mem_start_addr,  \
		  															   sys_pool.mem_end_addr, sys_pool.total_num, \
		  															   sys_pool.max_used_num,sys_pool.availble_num);			
			
			flag = POOL_INFO_SEEK_NEXT;		
			
			if(SCI_POOL_INFO_END == status)
			{
				DIAG_SendFormatInfo("-----------End-----------\n");  
				break;
			}
         }
		 else
		 {
		 	  DIAG_SendFormatInfo("Some Error Happend!\n");  
		 }
	}while(SCI_SUCCESS == status);

}

#ifdef SHELL_CMD_BT_TEST
LOCAL void cmdscript_bt_startparing(int argc, char *argv[])
{

    BT_STATUS status;
    int i;
    
    //SHELL_TRACE:"[SHELL] test_handler_play: argc=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_238_112_2_18_1_5_38_67,(uint8*)"d",argc );
    
    for(i=0; i<argc; i++)
    {
        //SHELL_TRACE:"[SHELL] test_handler_play: argv[%d]=%s\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_242_112_2_18_1_5_38_68,(uint8*)"ds",i,argv[i] );
        DIAG_SendFormatInfo("[SHELL] DIAG_SendFormatInfo: argv[%d]=%s",i,argv[i]);
    }

    SCI_MEMSET( &addr, 0, sizeof(BT_ADDRESS) );
    
    for(i=0; i<6; i++ )
    {
        char ch1 = argv[0][i*2];
        char ch2 = argv[0][i*2+1];
        
        if( ch1 >= '0' && ch1 <= '9' )
        {
            addr.addr[5-i] += (ch1 - '0') * 16;  
        }
        else if(ch1 >= 'A' && ch1 <= 'F' )
        {
            addr.addr[5-i] += ((ch1-'A') + 10) * 16;
        }
        else if(ch1 >= 'a' && ch1 <= 'f' )
        {
            addr.addr[5-i] += ((ch1-'a') + 10) * 16;
        }
        else
        {
             DIAG_SendInfo(SHELL_DATA_TEXT, (uint8*)"Input addr is error!", strlen("Input addr is error!"));
        }

        if( ch2 >= '0' && ch2 <= '9' )
        {
            addr.addr[5-i] += (ch2 - '0') ;  
        }
        else if(ch2 >= 'A' && ch2 <= 'F' )
        {
            addr.addr[5-i] += ((ch2-'A') + 10) ;
        }
        else if(ch2 >= 'a' && ch2 <= 'f' )
        {
            addr.addr[5-i] += ((ch2-'a') + 10) ;
        }
        else
        {
             DIAG_SendInfo(SHELL_DATA_TEXT, (uint8*)"Input addr is error!", strlen("Input addr is error!"));
        }
        DIAG_SendFormatInfo("[SHELL] DIAG_SendFormatInfo: addr[%d]=0x%02x",(5-i),addr.addr[5-i]);
    }

    status = BT_PairDevice(&addr);

    DIAG_SendScriptInfo(status);
    
}

LOCAL void cmdscript_bt_inputcode(int argc, char *argv[])
{
    BT_STATUS status;
    int i;
    
    //SHELL_TRACE:"[SHELL] test_handler_play: argc=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_300_112_2_18_1_5_38_69,(uint8*)"d",argc );
    
    for(i=0; i<argc; i++)
    {
        //SHELL_TRACE:"[SHELL] test_handler_play: argv[%d]=%s\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_304_112_2_18_1_5_38_70,(uint8*)"ds",i,argv[i] );
        DIAG_SendFormatInfo("[SHELL] DIAG_SendFormatInfo: argv[%d]=%s",i,argv[i]);
    }

    status = BT_InputPairPin(&addr,(uint8*)argv[0], strlen(argv[0]));   
    DIAG_SendScriptInfo(SHELL_PENDING);
}

LOCAL DIAG_PRIMARY_CMD_T cmdscript_pri_cmd = 
{
    "bt",
    "bt action:startparing <bd_addr>|inputcode <pin_code> "
};

LOCAL DIAG_SECONDLY_CMD_T cmdscript_sec_cmd_1 = 
{
    "startparing",
     1,
     cmdscript_bt_startparing,
};

LOCAL DIAG_SECONDLY_CMD_T cmdscript_sec_cmd_2 = 
{
    "inputcode",
     1,
     cmdscript_bt_inputcode,
};

LOCAL void test_handler_1(int argc, char *argv[])
{
    int i;
    
    //SHELL_TRACE:"[SHELL] test_handler_1: argc=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_336_112_2_18_1_5_38_71,(uint8*)"d",argc );
    
    for(i=0; i<argc; i++)
    {
        //SHELL_TRACE:"[SHELL] test_handler_1: argv[%d]=%s\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_340_112_2_18_1_5_38_72,(uint8*)"ds",i,argv[i] );
    }
}
LOCAL void test_handler_2(int argc, char *argv[])
{
    int i;
    
    //SHELL_TRACE:"[SHELL] test_handler_2: argc=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_347_112_2_18_1_5_38_73,(uint8*)"d",argc );
    
    for(i=0; i<argc; i++)
    {
        //SHELL_TRACE:"[SHELL] test_handler_2: argv[%d]=%s\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_351_112_2_18_1_5_38_74,(uint8*)"ds",i,argv[i] );
    }    
}

LOCAL void test_handler_play(int argc, char * argv[])
{
    int i;
    
    //SHELL_TRACE:"[SHELL] test_handler_play: argc=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_359_112_2_18_1_5_38_75,(uint8*)"d",argc );
    
    for(i=0; i<argc; i++)
    {
        //SHELL_TRACE:"[SHELL] test_handler_play: argv[%d]=%s\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_363_112_2_18_1_5_38_76,(uint8*)"ds",i,argv[i] );
        DIAG_SendFormatInfo("[SHELL] DIAG_SendFormatInfo: argv[%d]=%s",i,argv[i]);
    }
    
    DIAG_SendInfo(SHELL_DATA_TEXT, (uint8*)"I have received play com!", strlen("I have received play cmd!"));

}

LOCAL void test_handler_pause(int argc, char *argv[])
{
    int i;
    
    //SHELL_TRACE:"[SHELL] test_handler_pause: argc=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_375_112_2_18_1_5_38_77,(uint8*)"d",argc );
    
    for(i=0; i<argc; i++)
    {
        //SHELL_TRACE:"[SHELL] test_handler_pause: argv[%d]=%s\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_379_112_2_18_1_5_38_78,(uint8*)"ds",i,argv[i] );
    }    
    DIAG_SendInfo(SHELL_DATA_TEXT, (uint8*)"I have received pause com!", strlen("I have received pause cmd!"));
}

LOCAL void test_handler_resume(int argc, char *argv[])
{
    int i;
    
    //SHELL_TRACE:"[SHELL] test_handler_resume: argc=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_388_112_2_18_1_5_38_79,(uint8*)"d",argc );
    
    for(i=0; i<argc; i++)
    {
        //SHELL_TRACE:"[SHELL] test_handler_resume: argv[%d]=%s\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_392_112_2_18_1_5_38_80,(uint8*)"ds",i,argv[i] );
    }

    DIAG_SendInfo(SHELL_DATA_TEXT, (uint8*)"I have received rusume com!", strlen("I have received rusume cmd!"));
}
LOCAL void test_handler_stop(int argc, char *argv[])
{
    int i;
    
    //SHELL_TRACE:"[SHELL] test_handler_stop: argc=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_401_112_2_18_1_5_38_81,(uint8*)"d",argc );
    
    for(i=0; i<argc; i++)
    {
        //SHELL_TRACE:"[SHELL] test_handler_stop: argv[%d]=%s\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_405_112_2_18_1_5_38_82,(uint8*)"ds",i,argv[i] );
    }    
}


LOCAL DIAG_PRIMARY_CMD_T test_pri_cmd = 
{
    "log",
    "log action:on <module,sub_module>|off <module> "
};

LOCAL DIAG_SECONDLY_CMD_T test_sec_cmd_1 = 
{
    "on",
     1,
     test_handler_1,
};

LOCAL DIAG_SECONDLY_CMD_T test_sec_cmd_2 = 
{
    "off",
     1,
     test_handler_2,
};

LOCAL DIAG_PRIMARY_CMD_T mp3_pri_cmd = 
{
    "mp3",
    "mp3 action:play <filename> | pause <> | resume <> | stop <>"
};

LOCAL DIAG_SECONDLY_CMD_T test_sec_cmd_play = 
{
    "play",
     10,
     test_handler_play,
};

LOCAL DIAG_SECONDLY_CMD_T test_sec_cmd_pause = 
{
    "pause",
     1,
     test_handler_pause,
};

LOCAL DIAG_SECONDLY_CMD_T test_sec_cmd_resume = 
{
    "resume",
     1,
     test_handler_resume,
};
LOCAL DIAG_SECONDLY_CMD_T test_sec_cmd_stop = 
{
    "stop",
     1,
     test_handler_stop,
};
#endif



#ifdef SHELL_CMD_WLAN_TEST
LOCAL void SHELL_wlanCmd(int argc, char *argv[])
{
    extern void WIFI_CmdHandler(void (*oprintf)(const char *x_format, ...),
                    int argc, char *argv[]);
    WIFI_CmdHandler(DIAG_SendFormatInfo, argc, argv);
}


LOCAL DIAG_PRIMARY_CMD_T wlan_pri_cmd = 
{
    "wlan",
    "wlan action:cmd <>"
};


LOCAL DIAG_SECONDLY_CMD_T wlan_cmd_sec_cmd = 
{
    "cmd",
     10,
     SHELL_wlanCmd,
};
#endif


LOCAL DIAG_PRIMARY_CMD_T mem_check_pri_cmd = 
{
    "mem",
    "mem action:check <> | dump <addr{0xXXXXXXXX}, len{0xXXXXXXXX}, type{'t','b'}> | status <>"
};


LOCAL DIAG_SECONDLY_CMD_T mem_check_sec_cmd = 
{
    "check",
     5,
     SHELL_memLeakCheck,
};

LOCAL DIAG_SECONDLY_CMD_T mem_dump_sec_cmd = 
{
    "dump",
     5,
     SHELL_memDump,
};

LOCAL DIAG_SECONDLY_CMD_T mem_status_sec_cmd = 
{
    "status",
     6,
     SHELL_MemStatus,
};

/* Register some commmands that often are used.  */
/* Stack info commands. */
LOCAL DIAG_PRIMARY_CMD_T stack_info_cmd = 
{
    "stack",
    "stack action:show <>"
};

LOCAL DIAG_SECONDLY_CMD_T stack_info_sec_cmd = 
{
    "show",
     5,
     SHELL_stackInfoShow,
};

/* Task info commands. */
LOCAL DIAG_PRIMARY_CMD_T task_info_cmd = 
{
    "task",
    "task action:show <>"
};

LOCAL DIAG_SECONDLY_CMD_T task_info_sec_cmd = 
{
    "show",
     5,
     SHELL_taskInfoShow,
};


/* CPU statitics info commands. */
LOCAL DIAG_PRIMARY_CMD_T cpu_stats_cmd = 
{
    "cpustats",
    "cpustats action:show <>"
};

LOCAL DIAG_SECONDLY_CMD_T cpu_stats_sec_cmd = 
{
    "show",
     5,
     SHELL_cpuStatsShow,
};

/* Version command */
LOCAL DIAG_PRIMARY_CMD_T version_cmd = 
{
    "version",
    "version action:show <>"
};

LOCAL DIAG_SECONDLY_CMD_T version_show_sec_cmd = 
{
    "show",
     5,
     SHELL_versionShow,
};

/* System commands */
LOCAL DIAG_PRIMARY_CMD_T system_cmd = 
{
    "system",
    "system action:assert <> | reboot <>"
};

LOCAL DIAG_SECONDLY_CMD_T system_assert_sec_cmd = 
{
    "assert",
     5,
     SHELL_systemAssert,
};

LOCAL DIAG_SECONDLY_CMD_T system_reboot_sec_cmd = 
{
    "reboot",
     5,
     SHELL_systemReboot,
};

LOCAL void SHELL_stackInfoShow(int argc, char *argv[])
{
    //SHELL_TRACE:"[SHELL] SHELL_stackInfoShow: argc=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_595_112_2_18_1_5_39_83,(uint8*)"d",argc );
    SCI_PrintStackInfoToShell();    
}

LOCAL void SHELL_taskInfoShow(int argc, char *argv[])
{
    //SHELL_TRACE:"[SHELL] SHELL_taskInfoShow: argc=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_601_112_2_18_1_5_39_84,(uint8*)"d",argc );
    SCI_PrintThreadInfoToShell();
}

LOCAL void SHELL_cpuStatsShow(int argc, char *argv[])
{
    
}

LOCAL void SHELL_versionShow(int argc, char *argv[])
{
    const   char * info_ptr = NULL;
    uint32 t = 0;
    uint32 num = VERSION_GetInfoNum();

    DIAG_SendFormatInfo("\r\n============Version Information=============\r\n"); 
    for(t = 0; t < num; t++)
    {
        info_ptr = VERSION_GetInfo(t);
        if(info_ptr)
        {
           DIAG_SendFormatInfo(info_ptr); 
        }
    }

	// Show RTOS build time!
    info_ptr = SCI_GetRtosCompileDate();
    DIAG_SendFormatInfo("RTOS Compile Date : %s", info_ptr);

	// Show Ref build time!
    info_ptr = GetRefCompileDate();
    DIAG_SendFormatInfo("REF Compile Date : %s", info_ptr);
    
    DIAG_SendFormatInfo("\r\n=============================================\r\n"); 
    
}

LOCAL void SHELL_systemAssert(int argc, char *argv[])
{
    /* Assert MS */
    SCI_ASSERT(FALSE);
}

LOCAL void SHELL_systemReboot(int argc, char *argv[])
{
    /*  Reboot MS. */
    OS_Reset();
}

void SHELL_TestRegisterShellCmd(void)
{
#ifdef SHELL_CMD_WLAN_TEST
    HANDLE wlan_handle;
#endif
#ifdef SHELL_CMD_BT_TEST
    HANDLE   log_handle, mp3_handle, mem_check_handle, cmd_handle;

    //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd:\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_658_112_2_18_1_5_39_85,(uint8*)"" );

    /* Register the primary CMD.          */
    log_handle = DIAG_RegisterPriCmd(&test_pri_cmd);
    //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: log_handle=0x%08x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_662_112_2_18_1_5_39_86,(uint8*)"d", log_handle );
    if(!log_handle)
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_665_112_2_18_1_5_39_87,(uint8*)"s",test_pri_cmd.primary_cmd );
    }

    /* Register the secondly CMD. */
    //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: Register the secondly cmd1.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_669_112_2_18_1_5_39_88,(uint8*)"");
    if(!DIAG_RegisterSecCmd(log_handle, &test_sec_cmd_1))
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: Secondly cmd:%s has been registered.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_672_112_2_18_1_5_39_89,(uint8*)"s",test_sec_cmd_1.secondly_cmd );
    }

    //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: Register the secondly cmd2.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_675_112_2_18_1_5_39_90,(uint8*)"");
    if(!DIAG_RegisterSecCmd(log_handle, &test_sec_cmd_2))
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: Secondly cmd:%s has been registered.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_678_112_2_18_1_5_39_91,(uint8*)"s",test_sec_cmd_1.secondly_cmd);
    }

    /* mp3 cmd test */
    mp3_handle = DIAG_RegisterPriCmd(&mp3_pri_cmd);
    if(!mp3_handle)
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_685_112_2_18_1_5_39_92,(uint8*)"s",mp3_pri_cmd.primary_cmd );
    }    
    DIAG_RegisterSecCmd(mp3_handle, &test_sec_cmd_play);
    DIAG_RegisterSecCmd(mp3_handle, &test_sec_cmd_pause);
    DIAG_RegisterSecCmd(mp3_handle, &test_sec_cmd_resume);
    DIAG_RegisterSecCmd(mp3_handle, &test_sec_cmd_stop);
	
    /* mem check test */
    mem_check_handle = DIAG_RegisterPriCmd(&mem_check_pri_cmd);
    if(!mem_check_handle)
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_696_112_2_18_1_5_39_93,(uint8*)"s",mem_check_pri_cmd.primary_cmd );
    }    
    
    DIAG_RegisterSecCmd(mem_check_handle, &mem_check_sec_cmd);
    DIAG_RegisterSecCmd(mem_check_handle, &mem_dump_sec_cmd);
	DIAG_RegisterSecCmd(mem_check_handle, &mem_status_sec_cmd);

    /* Cmd shell script test. */
    cmd_handle = DIAG_RegisterPriCmd(&cmdscript_pri_cmd);
    if(!cmd_handle)
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_707_112_2_18_1_5_39_94,(uint8*)"s",mem_check_pri_cmd.primary_cmd );
    }    
    
    DIAG_RegisterSecCmd(cmd_handle, &cmdscript_sec_cmd_1);
    DIAG_RegisterSecCmd(cmd_handle, &cmdscript_sec_cmd_2);

    /* Register stack info command. */
    cmd_handle = DIAG_RegisterPriCmd(&stack_info_cmd);
    if(!cmd_handle)
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_717_112_2_18_1_5_39_95,(uint8*)"s",stack_info_cmd.primary_cmd );
    }    
    DIAG_RegisterSecCmd(cmd_handle, &stack_info_sec_cmd);

    /* Register task info command. */
    cmd_handle = DIAG_RegisterPriCmd(&task_info_cmd);
    if(!cmd_handle)
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_725_112_2_18_1_5_39_96,(uint8*)"s",task_info_cmd.primary_cmd );
    }    
    DIAG_RegisterSecCmd(cmd_handle, &task_info_sec_cmd);

    /* Register CPU stats command. */
    /*
    cmd_handle = DIAG_RegisterPriCmd(&cpu_stats_cmd);
    if(!cmd_handle)
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_734_112_2_18_1_5_39_97,(uint8*)"s",cpu_stats_cmd.primary_cmd );
    }    
    DIAG_RegisterSecCmd(cmd_handle, &cpu_stats_sec_cmd);
    */
    /* Register version info command. */
    cmd_handle = DIAG_RegisterPriCmd(&version_cmd);
    if(!cmd_handle)
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_742_112_2_18_1_5_39_98,(uint8*)"s",version_cmd.primary_cmd );
    }    
    DIAG_RegisterSecCmd(cmd_handle, &version_show_sec_cmd);
    
    /* Register version info command. */
    cmd_handle = DIAG_RegisterPriCmd(&system_cmd);
    if(!cmd_handle)
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_750_112_2_18_1_5_39_99,(uint8*)"s",system_cmd.primary_cmd );
    }    
    DIAG_RegisterSecCmd(cmd_handle, &system_assert_sec_cmd);
    DIAG_RegisterSecCmd(cmd_handle, &system_reboot_sec_cmd);
#endif
/* Avoid SHELL_CMD_BT_TEST covers SHELL_CMD_WLAN_TEST as this would disable WIFI shell commands
when BT is unsupported and WIFI is supported (SR25312) */
#ifdef SHELL_CMD_WLAN_TEST
    wlan_handle = DIAG_RegisterPriCmd(&wlan_pri_cmd);
    if(!wlan_handle)
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_760_112_2_18_1_5_39_100,(uint8*)"s",wlan_pri_cmd.primary_cmd );
    }
    DIAG_RegisterSecCmd(wlan_handle, &wlan_cmd_sec_cmd);
#endif
}
    
LOCAL void SHELL_TestDisposeShellCmd()
{
#ifdef SHELL_CMD_BT_TEST
    HANDLE   log_handle, mp3_handle;
    DIAG_PARSE_RESULT_T   parse_result;

    //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd:\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_772_112_2_18_1_5_39_101,(uint8*)"" );

    /* Register the primary CMD.          */
    log_handle = DIAG_RegisterPriCmd(&test_pri_cmd);
    //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: log_handle=0x%08x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_776_112_2_18_1_5_39_102,(uint8*)"d", log_handle );
    if(!log_handle)
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_779_112_2_18_1_5_39_103,(uint8*)"s",test_pri_cmd.primary_cmd );
    }

    /* Register the same primary CMD AGAIN!!!. */
    log_handle = DIAG_RegisterPriCmd(&test_pri_cmd);
    //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: log_handle=0x%08x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_784_112_2_18_1_5_39_104,(uint8*)"d", log_handle );
    if(!log_handle)
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_787_112_2_18_1_5_39_105,(uint8*)"s",test_pri_cmd.primary_cmd );
    }
   

    /* Register the secondly CMD. */
    //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: Register the secondly cmd1.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_792_112_2_18_1_5_39_106,(uint8*)"");
    if(!DIAG_RegisterSecCmd(log_handle, &test_sec_cmd_1))
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: Secondly cmd:%s has been registered.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_795_112_2_18_1_5_39_107,(uint8*)"s",test_sec_cmd_1.secondly_cmd );
    }

    //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: Register the secondly cmd2.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_798_112_2_18_1_5_39_108,(uint8*)"");
    if(!DIAG_RegisterSecCmd(log_handle, &test_sec_cmd_2))
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: Secondly cmd:%s has been registered.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_801_112_2_18_1_5_39_109,(uint8*)"s",test_sec_cmd_1.secondly_cmd);
    }

    /* Register the secondly cmd AGAIN!!!. *****          */
    //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: Register the secondly cmd2.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_805_112_2_18_1_5_39_110,(uint8*)"");
    if(!DIAG_RegisterSecCmd(log_handle, &test_sec_cmd_2))
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: Secondly cmd:%s has been registered.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_808_112_2_18_1_5_39_111,(uint8*)"s",test_sec_cmd_1.secondly_cmd );
    }
    
    /* mp3 cmd test */
    mp3_handle = DIAG_RegisterPriCmd(&mp3_pri_cmd);
    if(!mp3_handle)
    {
        //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: CMD:%s has been registered in list\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_815_112_2_18_1_5_39_112,(uint8*)"s",mp3_pri_cmd.primary_cmd );
    }    
    DIAG_RegisterSecCmd(mp3_handle, &test_sec_cmd_play);
    DIAG_RegisterSecCmd(mp3_handle, &test_sec_cmd_pause);
    DIAG_RegisterSecCmd(mp3_handle, &test_sec_cmd_resume);
    DIAG_RegisterSecCmd(mp3_handle, &test_sec_cmd_stop);

   
    /* Test the parse function. */
    parse_result = DIAG_ParseCmdline("log on mmi", 0);
    //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd Parse finished and result=%d.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_825_112_2_18_1_5_39_113,(uint8*)"d", parse_result);
    parse_result = DIAG_ParseCmdline("log off driver", 0);
    parse_result = DIAG_ParseCmdline("log off all", 0);

    //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: Parse finished and result=%d.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_829_112_2_18_1_5_39_114,(uint8*)"d", parse_result);
    
    //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: Unregister primary cmd.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_831_112_2_18_1_5_39_115,(uint8*)"");

    /* Test the parsing of the mp3 cmd */
    //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: Parsing mp3 cmd *****************.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_834_112_2_18_1_5_39_116,(uint8*)"");
    parse_result = DIAG_ParseCmdline("mp3 play dddddddddddd.mp3", 0);
    parse_result = DIAG_ParseCmdline("mp3 resume cccccccccccc.mp3", 0);
    parse_result = DIAG_ParseCmdline("mp3 stop aaaaaaaa.mp3", 0);
    parse_result = DIAG_ParseCmdline("mp3 pause bbbbbbbb.mp3", 0);

    /* Test the error cmd for mp3.     */
    //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: Parsing error mp3 cmd *****************.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_841_112_2_18_1_5_39_117,(uint8*)"");
    parse_result = DIAG_ParseCmdline("mp3 plaay eeeeeeeeeeeeeee.mp3", 0);
    //SHELL_TRACE:"[SHELL] DIAG_TestDisposeShellCmd: Parsing mp3 cmd end and parse_result=%d!! *****************.\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SHELL_TEST_843_112_2_18_1_5_39_118,(uint8*)"d", parse_result);

    /* Test the unregister funtion.     */
    DIAG_UnregisterPriCmd(log_handle);
    DIAG_UnregisterPriCmd(mp3_handle);
#endif    
}

#endif /* SHELL_DEBUG_SUPPORT*/

/* Test code end! */


