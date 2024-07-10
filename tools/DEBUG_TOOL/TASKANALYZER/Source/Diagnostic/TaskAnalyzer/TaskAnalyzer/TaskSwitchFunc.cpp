#include "stdafx.h"
#include "TaskSwitchFunc.h"
#include "TaskAnalyzerDlg.h"

//apple 07-03-06
#pragma warning( disable : 4786 )
#include <map>
#include <float.h>

using namespace std;
//

#define DS_IDLE_REQ            0x10
#define DS_TASK_REQ            0x20
#define DS_TASK_SUSP_REQ	   0x21
#define DS_ENTER_INT_REQ       0x30
#define DS_LEAVE_INT_REQ       0x40
#define DS_ENTER_SLEEP_REQ     0x50
#define DS_LEAVE_SLEEP_REQ     0x60
#define DS_TEST_POINT          0x70
#define DS_ENTER_FIQ_REQ	   0x80
#define DS_LEAVE_FIQ_REQ	   0x90

#define TABLE_LEN 128
#define TABLE_NUM 128
#define LAST_ZERO_REGION 100
#define TASKMONITORBEGIN_FIR 0x4b534154
#define TASKMONITORBEGIN_SEC 0x494e4f4d
#define TASKMONITORBEGIN_THR 0x42524f54
#define TASKMONITORBEGIN_FOU 0x4e494745
#define MAX_TIME_SIZE 0xFFFF

#define MAX_ERROR_US 100000

typedef struct TASK_TEST_POINTtag
{
	unsigned short  id;
	unsigned short  accuracy_time;
	unsigned long   pointer;
	unsigned long   return_addr;
	unsigned long   time;
}TASK_TEST_POINT_T;

//apple 07-03-06
typedef struct TaskInfo_tag
{
	char szName[128];
	int npriority;
	int nStartTime;
	int nTotalTime;
}TASKINFO_T;

typedef map<int, TASKINFO_T>  TaskInfoMap;

typedef  struct TX_THREAD_STRUCT
{	
	UINT        reserved[10] ;                /**/		
	char*       tx_thread_name;         /* Pointer to thread's name */	
    UINT        tx_priority;            /* Priority of thread (0-31)*/	
} TX_THREAD;

#define MAX_SPACE_TIME          40000 //(us)

//


typedef enum
{
	F_3204,
	F_6408,
	F_6416,
	F_SDRAM_OR_12832,
	F_INVALID
} FLASH_TYPE_E;

static FLASH_TYPE_E flash_type;

unsigned long g_nMaxSwitchNum = 570;
unsigned int g_percentMode = 0; //0:for ms; 1: for us
extern unsigned long g_nTaskTableOffset;
extern unsigned long g_nTcbPtr[TABLE_NUM];

char g_szDebugString[MAX_PATH] = {0};

//unsigned long* buf[FILE_MAX_SIZE / 4];

#define CONVERT_UINT32(result, source) \
result = (source >> 24) & 0xFF; \
result += ((source >> 16) & 0xFF) << 8; \
result += ((source >> 8) & 0xFF) << 16; \
result += (source & 0xFF) << 24; 

#define CONVERT_UINT16(result, source) \
result = (source >> 8) & 0xFF; \
result += (source & 0xFF) << 8; 

static unsigned long file_size;

//teana hu 2010.04.08
extern CTaskAnalyzerDlg* g_pdlg;

//teana hu 2010.08.30
extern BOOL g_bBigEndian;

//unsigned long buf2[ 0x4E2000 / 4];

bool Analyze( const char* szProduct, const char * path_name, unsigned long* buf, 
			 UINT uiMaxFileSize , UINT uiOffsize , char* szTaskList, char* szIntList, 
			 const char* source_bin_file_name /* = NULL  */)
{
	FILE * fp_sleep = NULL;
	//apple 07-03-06
//	FILE* fp_sta = NULL;
	FILE* fp_source_bin = NULL;
	FILE* fp_mem_bin = NULL;

	//
	char int_table[64][16] = {0};
	char  string_table[128][16] = {0};
	UINT uiTaskNum = 0 ;
	int j = 0;
	char* token;

	token = strtok( szTaskList, "{}, " );
	while( token != NULL )
	{
		strcpy( string_table[j], token );
		token = strtok( NULL, "{}, " );
		++j;
	}
	uiTaskNum = j;
	j = 0;

	token = strtok( szIntList, "{}, " );
	while( token != NULL )
	{
		strcpy( int_table[j], token );
		token = strtok( NULL, "{}, " );
		++j;
	}
	
    unsigned long wakeup_time   = 0;
    unsigned long sleep_time    = 0;
    unsigned long sleep_status  = 0;
    
    unsigned long test_offset = 0;
    
	fp_mem_bin = fopen(path_name, "rb");


	if (NULL == fp_mem_bin )
	{
		return false;
	}

	bool  usefile = false;
	if( source_bin_file_name != NULL && ( fp_source_bin = fopen( source_bin_file_name, "rb")) != NULL  )
	{
		usefile = true;
	}

	if(fp_source_bin == NULL)
	{
		return false;
	}

	// First get the offset of the task switch file
	file_size = fread(buf, 1, uiMaxFileSize, fp_mem_bin );

	if (file_size < 1024 * 1024)
	{
		flash_type = F_3204;
	}
	else if (file_size < 2048 * 1024)
	{
		flash_type = F_6408;
	}
	else if (file_size < 4096 * 1024)
	{
		flash_type = F_6416;
	}
	else
	{
		if( stricmp( szProduct, "M_PLATFORM") == 0 )
			flash_type = F_SDRAM_OR_12832;
		else
		{
			if (file_size < 64 * 1024 * 1024) //ding
			{
				flash_type = F_SDRAM_OR_12832;
			}
			else
			{
				flash_type = F_INVALID;
				return false;
			}
		}
	}
	// Search the buf to find the begin of the data
	// TASKMONITORBEGIN
	for (unsigned long i = file_size / 4 - 101; i >= 0; i--)
	{
		if (buf[i - 3] == TASKMONITORBEGIN_FIR && buf[i - 2] == TASKMONITORBEGIN_SEC &&
			buf[i - 1] == TASKMONITORBEGIN_THR && buf[i] == TASKMONITORBEGIN_FOU)
		{
			break;
		}
	}

	i -= 3; //move i to the head of "TASKMONITORBEGIN"
	
//    for (unsigned long i = 0; i < file_size / 4 - 100; i++)
//	{
//		if (buf[i] == TASKMONITORBEGIN_FIR && buf[i + 1] == TASKMONITORBEGIN_SEC &&
//			buf[i + 2] == TASKMONITORBEGIN_THR && buf[i + 3] == TASKMONITORBEGIN_FOU)
//		{
//			break;
//		}
//	}
	
	if (i == file_size / 4 - 100)
		return false;

	TASK_TEST_POINT_T* test_point = NULL;

	if( stricmp( szProduct, "M_PLATFORM") == 0 )
	{
		test_point = new TASK_TEST_POINT_T[ g_nMaxSwitchNum + 2 ];   
	}
	else
	{
		test_point = new TASK_TEST_POINT_T[ g_nMaxSwitchNum ];    
	}

	memcpy(test_point, &buf[i + 4], g_nMaxSwitchNum * 16 );

	// Get the offset
	for (i = 0; i < g_nMaxSwitchNum; i++)
	{
		TASK_TEST_POINT_T temp;
		if(g_bBigEndian)
		{
			CONVERT_UINT16(temp.id, test_point[i].id)
			CONVERT_UINT16(temp.accuracy_time, test_point[i].accuracy_time)
			CONVERT_UINT32(temp.pointer, test_point[i].pointer)
			CONVERT_UINT32(temp.return_addr, test_point[i].return_addr)
			CONVERT_UINT32(temp.time, test_point[i].time)
		}
		else
		{
			temp.id = test_point[i].id;
			temp.accuracy_time = test_point[i].accuracy_time;
			temp.pointer = test_point[i].pointer;
			temp.return_addr = test_point[i].return_addr;
			temp.time = test_point[i].time;
		}
		
		memcpy(&test_point[i], &temp, sizeof(TASK_TEST_POINT_T));
	}

	// Get the offset
	for (i = 0; i < g_nMaxSwitchNum; i++)
	{
		if (test_point[i].pointer == 0xff && test_point[i].return_addr == 0xff)
		{
			test_offset = i + 1;
			break;
		}
	}

	// Printf the signals to a text file
	char output_name[256];
	strcpy(output_name, path_name);

	unsigned long count;
	count = strlen(output_name);
	strcpy(&output_name[count - 4], ".txt");

	FILE* fp = fopen(output_name, "wt");
	if(NULL == fp)
	{
		delete [] test_point;
		return false;
	}

	strcpy(&output_name[count - 4], ".slp");
	fp_sleep = fopen(output_name, "wt");
	if(NULL == fp_sleep)
	{
		delete [] test_point;
		return false;
	}

	//apple 07-03-06
	strcpy( &output_name[count - 4], ".csv" );
//	fp_sta = fopen( output_name, "wt" );
//	if( fp_sta == NULL )
//	{
//		delete [] test_point;
//		return false;
//	}
//	fprintf( fp_sta, "name,priority,time(ms),time(us),percent,\n" );
	int nStartTime = 0x7FFFFFFF;
	int nEndTime = 0 ;
	int nLastId = -1;
	int nLastIrq = -1;
	int nLastFiq = -1;
	TaskInfoMap   taskMap;
	TaskInfoMap   irqMap;
	TaskInfoMap   fiqMap;
	int nTime = 0;
	int nLastTime = 0;
	int nSleepStartTime = 0;
	int nSleepTotolTime = 0;
	int nIdleStartTime = 0;
	int nIdleTotolTime = 0 ;
	int nErrorTime = 0;
	//apple

	unsigned long temp_offset = test_offset;

	char test_txt[400] = {0};
	char sleep_txt[400] = {0};

	fputs("Action       Start time    Accuracy time\r\n", fp);

	for (i = 0; i < g_nMaxSwitchNum; i++)
	{
		unsigned long delta = 0;
		if( stricmp( szProduct, "TD") == 0)
		{
			if(test_point[temp_offset].accuracy_time > TIMER1_TICK_1MS)
			{
				delta = TIMER1_TICK_1MS - (0xffff -
					test_point[temp_offset].accuracy_time);
			}
			else
			{
				delta = TIMER1_TICK_1MS - test_point[temp_offset].accuracy_time;
			}
			delta = (unsigned long) (delta * TIMER1_US_1TICK);
		}
		else
		{
			if (temp_offset == 0)
			{
				delta = test_point[g_nMaxSwitchNum - 1].accuracy_time - test_point[0].accuracy_time;
			}
			else
			{
				delta = test_point[temp_offset - 1].accuracy_time - test_point[temp_offset].accuracy_time;
			}
			
			
			if( stricmp( szProduct, "GSM") == 0 )
			{
				delta = (unsigned long) (delta * 1.2);
			}
		}

		
		if (sleep_status == 1)
		{
			sleep_status = 0;
			
			if ((temp_offset + 1)>= g_nMaxSwitchNum)
			{
				sprintf(sleep_txt, "LINE: %d, SLEEP: %dms\r\n", i+2, test_point[0].time - sleep_time);
				wakeup_time = test_point[0].time;
			}
			else
			{
				sprintf(sleep_txt, "LINE: %d, SLEEP: %dms\r\n", i+2, test_point[temp_offset + 1].time - sleep_time);
				wakeup_time = test_point[temp_offset + 1].time;
			}

			fputs(sleep_txt, fp_sleep);
		}
		//apple
		nTime = test_point[temp_offset].time * 1000 + delta;
		if( 0 == nLastTime )
		{
			nLastTime = nTime;
		}
		
		while( nLastTime > nTime )
		{	
			nTime += 1000;
		}

		nEndTime  = max( nEndTime, nTime );
		nStartTime = min( nStartTime, nTime );				
		
		if( nTime > nLastTime +  MAX_SPACE_TIME )
		{
			nErrorTime += nTime - nLastTime;	
			nLastTime = nTime;
			i = i -1;
			continue;
		}
		nLastTime = nTime;

		//apple
		switch(test_point[temp_offset].id)
		{
		case DS_IDLE_REQ:
			{
			sprintf(test_txt, "IDLE       : %dms;    0x%4x(%6dus); Sleep mode 0x%x\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta, test_point[temp_offset].pointer);
			//apple
			TaskInfoMap::iterator jt = taskMap.find( nLastId );
			if( jt != taskMap.end() )
			{
				while( nTime < (*jt).second.nStartTime )
				{
					nTime += 1000;
				}
//				if( nLastId == -213128437 )
//				{
//					TRACE("%d,%d,%d\n",(*jt).second.nTotalTime, nTime, (*jt).second.nStartTime);
//				}
				(*jt).second.nTotalTime += nTime - (*jt).second.nStartTime;
			}
			nLastId = -1;

			nIdleStartTime = nTime;
			}
			//
			break;
		case DS_TASK_REQ:
			{		
				unsigned long task_id, offset, temp_id;
				unsigned long priority_id = 0;
				unsigned long priority = 0;
				char szname[16] = {0};				
				
				if( usefile )
				{
					offset = test_point[temp_offset].pointer;
					//CONVERT_UINT32(offset, temp_id);
					offset -=  0x00C00000;
					if ( fseek( fp_mem_bin, offset, SEEK_SET ) != 0 )
					{
						goto NoUseFile;
					}

					TX_THREAD tx;
					if( fread( &tx, sizeof( TX_THREAD ), 1, fp_mem_bin ) != 1 ) 
					{
						goto NoUseFile;
					}		
					priority_id = tx.tx_priority ;
					if(g_bBigEndian)
					{
						CONVERT_UINT32( priority, priority_id);

						CONVERT_UINT32( offset, (DWORD)(tx.tx_thread_name) );
					}
					else
					{
						priority = priority_id;

						offset = (DWORD)(tx.tx_thread_name);
					}

					if( fseek( fp_source_bin, offset, SEEK_SET ) != 0)
					{
						goto NoUseFile;
					}
					fread(  szname, 1, 16, fp_source_bin );	
					szname[15] = '\0';
				}
NoUseFile:				
				offset = (test_point[temp_offset].pointer - 4) & 0xffffffff - uiOffsize;
				if( offset >= file_size )
				{
//					sprintf(test_txt, "ERROR in TASK point pointer : %d;    ,pointer: %d;\r\n", test_point[temp_offset].id,  test_point[temp_offset].pointer );
					AfxMessageBox("error! offset excess file max size!");
					if(test_point != NULL)
					{
						delete [] test_point;
					}
					return false;
				}
				temp_id = *((unsigned long *) &buf[offset / 4]);
				
				if( priority_id == 0 && stricmp( szProduct, "TD") == 0 )	
				{
					if( offset /4 + 12 >= file_size /4 )
					{
						sprintf(test_txt, "ERROR in test point pointer : %d;    ,pointer: %d;\r\n", test_point[temp_offset].id,  test_point[temp_offset].pointer );
						break;
					}
					priority_id = *((unsigned long *)&buf[offset / 4 + 12]);
					if(g_bBigEndian)
					{
						CONVERT_UINT32(priority, priority_id);
					}
					else
					{
						priority = priority_id;
					}
				}

				if(g_bBigEndian)
				{
					CONVERT_UINT32(task_id, temp_id)
				}
				else
				{
					task_id = temp_id;
				}

				if (task_id < uiTaskNum)
				{
					if( szname[0] == '\0' )
					{
						strcpy(  szname, string_table[task_id ] );	
					}
					if( stricmp( szProduct, "TD") == 0 )	
					{
						sprintf(test_txt, "TASK       : %dms;    0x%4x(%6dus); Task       0x%x(0x%x, %s) priority=%d\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta, test_point[temp_offset].pointer, task_id, szname, priority);
					}
					else
					{
						sprintf(test_txt, "TASK       : %dms;    0x%4x(%6dus); Task       0x%x(0x%x, %s)\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta, test_point[temp_offset].pointer, task_id, szname );
					}
				}
				else
				{
					if( szname[0] == '\0' )
					{
						sprintf(  szname, "Unknown0x%X", task_id );	
					}
					if( stricmp( szProduct, "TD") == 0 )	
					{
						sprintf(test_txt, "TASK       : %dms;    0x%4x(%6dus); Task       0x%x(0x%x, %s) priority=%d\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta, test_point[temp_offset].pointer, task_id, "Unknown", priority);
					}
					else
					{
						sprintf(test_txt, "TASK       : %dms;    0x%4x(%6dus); Task       0x%x(0x%x, %s)\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta, test_point[temp_offset].pointer, task_id, "Unknown");
					}
				}
				//apple
				TaskInfoMap::iterator jt = taskMap.find( task_id );
				if( jt != taskMap.end() )
				{
					(*jt).second.nStartTime = nTime; 
				}
				else
				{
					TASKINFO_T taskinfo;
					strcpy( taskinfo.szName, szname);						
					taskinfo.nStartTime = nTime;
					taskinfo.npriority = priority;
					taskinfo.nTotalTime = 0 ;
					taskMap[ task_id ] = taskinfo;
				}
				nLastId = task_id;

				if( nIdleStartTime != 0 )
				{
					nIdleTotolTime  += nTime - nIdleStartTime;
					nIdleStartTime = 0;
				}
				//apple
				
			}
			break;

		case DS_TASK_SUSP_REQ:
			{
				if( stricmp( szProduct, "TD") == 0 )	
				{
					unsigned long task_id, offset, temp_id;
					
					//bo.chen
					unsigned long priority_id = 0;
					unsigned long priority = 0;
					
					offset = (test_point[temp_offset].pointer - 4) & 0xffffff - 0x00800000;
					if( offset /4 + 12 >= file_size /4 )
					{
						sprintf(test_txt, "ERROR in test point pointer : %d;    ,pointer: %d;\r\n", test_point[temp_offset].id,  test_point[temp_offset].pointer );
						break;
					}
					temp_id = *((unsigned long *) &buf[offset / 4]);
					
					
					//bo.chen
					priority_id = *((unsigned long *)&buf[offset / 4 + 12]);
					if(g_bBigEndian)
					{
						CONVERT_UINT32(priority, priority_id);
					
						CONVERT_UINT32(task_id, temp_id);
					}
					else
					{
						priority = priority_id;
					
						task_id = temp_id;
					}
					
					if (task_id < uiTaskNum )
					{
						sprintf(test_txt, "Suspend    : %dms;    0x%4x(%6dus); Task       0x%x(0x%x, %s) priority=%d\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta, test_point[temp_offset].pointer, task_id, string_table[task_id], priority);
					}
					else
					{
						sprintf(test_txt, "Suspend    : %dms;    0x%4x(%6dus); Task       0x%x(0x%x, %s) priority=%d\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta, test_point[temp_offset].pointer, task_id, "Unknown", priority);
					}
				}
				//apple
				TaskInfoMap::iterator jt = taskMap.find( nLastId );
				if( jt != taskMap.end() )
				{
					while( nTime < (*jt).second.nStartTime )
					{
						nTime += 1000;
					}
					(*jt).second.nTotalTime += nTime - (*jt).second.nStartTime;
				}
				nLastId = -1;

				if( nIdleStartTime != 0 )
				{
					nIdleTotolTime  += nTime - nIdleStartTime;
					nIdleStartTime = 0;
				}
				
			    //
			}
			break;
		case DS_ENTER_INT_REQ:
			{
				char IRQ_String[400];
				unsigned long irq_status = test_point[temp_offset].pointer;

				IRQ_String[0] = '\0';

				for (int m = 0; m < 32; m++)
				{
					if (irq_status & (1 << m))
					{
						strcat(IRQ_String, int_table[m]);
						strcat(IRQ_String, "+");
					}
				}
				int len = strlen(IRQ_String);
				IRQ_String[len - 1] = '\0';

				sprintf(test_txt, "ENTER IRQ  : %dms;    0x%4x(%6dus); Irq status 0x%x(%s); Return Addr 0x%x\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta, test_point[temp_offset].pointer, IRQ_String, test_point[temp_offset].return_addr);
				
				//apple
				TaskInfoMap::iterator l = taskMap.find( nLastId );
				if( l != taskMap.end() )
				{
					while( nTime < (*l).second.nStartTime )
					{
						nTime += 1000;
					}
					(*l).second.nTotalTime += nTime - (*l).second.nStartTime;
				}
				

				TaskInfoMap::iterator k = irqMap.find( irq_status );
				if( k != irqMap.end() )
				{
					(*k).second.nStartTime = nTime; 
				}
				else
				{
					TASKINFO_T taskinfo;
					if( IRQ_String[0] != '\0' )
					{
						strcpy( taskinfo.szName, IRQ_String );					
					}
					else
					{
						sprintf( taskinfo.szName, "UnknownIRQ%d", irq_status );
					}
					
					taskinfo.nStartTime = nTime;
					taskinfo.npriority = 0;
					taskinfo.nTotalTime = 0 ;
					irqMap[ irq_status ] = taskinfo;
				}
				nLastIrq = irq_status;

				if( nIdleStartTime != 0 )
				{
					nIdleTotolTime  += nTime - nIdleStartTime;
					nIdleStartTime = 0;
				}
				
				//apple
			}
			break;
		case DS_LEAVE_INT_REQ:
			{
				sprintf(test_txt, "LEAVE IRQ  : %dms;    0x%4x(%6dus);\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta);
				//apple				
				TaskInfoMap::iterator jt = irqMap.find( nLastIrq );
				if( jt != irqMap.end() )
				{
					if( (*jt).second.nStartTime != 0 )
					{						
						while( nTime < (*jt).second.nStartTime )
						{
							nTime += 1000;
						}
						
						(*jt).second.nTotalTime += nTime - (*jt).second.nStartTime ; 
						(*jt).second.nStartTime = 0;					
					}


				}

				jt = taskMap.find( nLastId );
				if( jt != taskMap.end() )
				{
					(*jt).second.nStartTime  = nTime;
				}				
				//apple
			}
			break;
		case DS_ENTER_SLEEP_REQ:
			{
			sprintf(test_txt, "ENTER SLEEP: %dms;    0x%4x(%6dus);\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta);
			
			sprintf(sleep_txt, "************************LINE: %d, WAKEUP: %dms\r\n", i+2, test_point[temp_offset].time - wakeup_time);
			sleep_status = 1;
			sleep_time = test_point[temp_offset].time;

			fputs(sleep_txt, fp_sleep);

			//apple
			TaskInfoMap::iterator jt = taskMap.find( nLastId );
			if( jt != taskMap.end() )
			{
				while( nTime < (*jt).second.nStartTime )
				{
					nTime += 1000;
				}
				(*jt).second.nTotalTime += nTime - (*jt).second.nStartTime;
			}
			nSleepStartTime = nTime;

			if( nIdleStartTime != 0 )
			{
				nIdleTotolTime  += nTime - nIdleStartTime;
				nIdleStartTime = 0;
			}
			
			//apple
			}
			break;
		case DS_LEAVE_SLEEP_REQ:
			{
			sprintf(test_txt, "LEAVE SLEEP: %dms;    0x%4x(%6dus);\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta);
			//apple
			TaskInfoMap::iterator jt = taskMap.find( nLastId );
			if( jt != taskMap.end() )
			{
				(*jt).second.nStartTime  = nTime;
			}
			nSleepTotolTime += nTime - nSleepStartTime;
			nSleepStartTime = 0;
			
			//
			}
			break;
		case DS_TEST_POINT:
			sprintf(test_txt, "TEST POINT : %dms;    0x%4x(%6dus),   0x%x,   %d\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta, test_point[temp_offset].pointer, test_point[temp_offset].return_addr);
			break;
		case DS_ENTER_FIQ_REQ:
			{
				char FIQ_String[400];
				unsigned long fiq_status = test_point[temp_offset].pointer;
				
				FIQ_String[0] = '\0';
				
				for (int m = 0; m < 24; m++)
				{
					if (fiq_status & (1 << m))
					{
						strcat(FIQ_String, int_table[m]);
						strcat(FIQ_String, "+");
					}
				}
				int len = strlen(FIQ_String);
				FIQ_String[len - 1] = '\0';
				
				sprintf(test_txt, "ENTER FIQ  : %dms;    0x%4x(%6dus); Irq status 0x%x(%s); Return Addr 0x%x\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta, test_point[temp_offset].pointer, FIQ_String, test_point[temp_offset].return_addr);
				
				//apple
				TaskInfoMap::iterator k = fiqMap.find( fiq_status );
				if( k != fiqMap.end() )
				{
					(*k).second.nStartTime = nTime; 
				}
				else
				{
					TASKINFO_T taskinfo;
					if( FIQ_String[0] != '\0' )
					{
						strcpy( taskinfo.szName, FIQ_String );					
					}
					else
					{
						sprintf( taskinfo.szName, "UnknowFIQ%d", fiq_status );
					}
					taskinfo.nStartTime = nTime;
					taskinfo.nTotalTime = 0 ;
					taskinfo.npriority = 0;
					fiqMap[ fiq_status ] = taskinfo;
				}
				nLastFiq = fiq_status;

				TaskInfoMap::iterator l = taskMap.find( nLastId );
				if( l != taskMap.end() )
				{
					while( nTime < (*l).second.nStartTime )
					{
						nTime += 1000;
					}
					(*l).second.nTotalTime += nTime - (*l).second.nStartTime;
				}

				if( nIdleStartTime != 0 )
				{
					nIdleTotolTime  += nTime - nIdleStartTime;
					nIdleStartTime = 0;
				}
				
				//apple
			}
			break;
		case DS_LEAVE_FIQ_REQ:
			{
				sprintf(test_txt, "LEAVE FIQ  : %dms;    0x%4x(%6dus);\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta);
				//apple
				
				TaskInfoMap::iterator jt = fiqMap.find( nLastFiq );
				if( jt != fiqMap.end() )
				{
					if( (*jt).second.nStartTime != 0 )
					{						
						while( nTime < (*jt).second.nStartTime )
						{
							nTime += 1000;
						}
						
						(*jt).second.nTotalTime += nTime - (*jt).second.nStartTime ; 
						(*jt).second.nStartTime = 0;	
					}
				}

				jt = taskMap.find( nLastId );
				if( jt != taskMap.end() )
				{
					(*jt).second.nStartTime  = nTime;
				}
				//apple
			}
			break;
		default:
			sprintf(test_txt, "TEST POINT : %d;    %dms;    0x%4x(%6dus);\r\n", test_point[temp_offset].id, test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta );
			break;
		}
		
		fputs(test_txt, fp);
		temp_offset++;

		if (temp_offset >= g_nMaxSwitchNum )
			temp_offset = 0;
	}

	//apple
	int nAllTotolTime = nEndTime - nStartTime;
	int nFixTime = nAllTotolTime - nErrorTime;
//	fprintf(fp_sta, "Totol time, ,%d, %d \n", nAllTotolTime / 1000, nAllTotolTime % 1000 );
//	fprintf(fp_sta, "Fix time,, %d, %d \n", nFixTime / 1000, nFixTime % 1000 );
//	fprintf( fp_sta, "Idle and Switch time,, %d, %d, %3f%%\n", nIdleTotolTime / 1000, nIdleTotolTime % 1000 , (double)((double)nIdleTotolTime / 10.0) / (double)((double)nFixTime / 1000.0) );
	
//	fprintf( fp_sta, "Sleep time, ,%d, %d,%3f%%\n", nSleepTotolTime / 1000, nSleepTotolTime % 1000 , (double)((double)nSleepTotolTime / 10.0) / (double)((double)nFixTime / 1000.0 ) );
//	TaskInfoMap::iterator it;
//	fprintf( fp_sta, "=========Task==========\n" );
//	for( it = taskMap.begin(); it != taskMap.end(); ++it )
//	{
//		fprintf( fp_sta, "%s,%d,%d,%d,%3f%%\n",(*it).second.szName, (*it).second.npriority, (*it).second.nTotalTime/1000, (*it).second.nTotalTime % 1000, (double)((double)(*it).second.nTotalTime / 10.0 )/ (double)((double)nFixTime / 1000.0));
//	}
//	fprintf( fp_sta, "============IRQ==========\n" );
//	for( it = irqMap.begin(); it != irqMap.end(); ++it )
//	{
//		fprintf( fp_sta, "%s,,%d,%d,%3f%%\n",(*it).second.szName, (*it).second.nTotalTime/1000, (*it).second.nTotalTime % 1000, (double)((double)(*it).second.nTotalTime / 10.0) / (double)((double)nFixTime / 1000.0));
//	}
//	fprintf( fp_sta, "============FIQ==========\n" );
//	for( it = fiqMap.begin(); it != fiqMap.end(); ++it )
//	{
//		fprintf( fp_sta, "%s,,%d,%d,%3f%%\n",(*it).second.szName, (*it).second.nTotalTime/1000, (*it).second.nTotalTime % 1000, (double)((double)(*it).second.nTotalTime / 10.0) / (double)((double)nFixTime / 1000.0));
//	}
//	fclose( fp_sta );


	fclose( fp_mem_bin );

	fclose( fp_source_bin );	
	//


	sprintf(test_txt, "**********************End of the task switch*********************\r\n");
	fputs(test_txt, fp);
	if(fp != NULL)//lint !e774
	{
		fclose(fp);
	}
	if(fp_sleep != NULL)//lint !e774
	{
		fclose(fp_sleep);
	}

	delete [] test_point;    
	
    return true;
}//lint !e429


void SplitFile(const char * path_name,unsigned long* buf,
			   UINT uiDataAreaSize, UINT uiShareMemorySize,
			   UINT uiIramSize , UINT uiRegSize)
{
	FILE * fp;

	char output_name[256];
	strcpy(output_name, path_name);
	unsigned long count;
	count = strlen(output_name);

	switch(flash_type)
	{
	case F_3204:
		// Get the sharmemory file
		strcpy(&output_name[count - 4], ".shr");

		fp = fopen(output_name, "wb");
		if(fp == NULL)
		{
			return;
		}

		fwrite(&buf[0x80000 / 4], 1, 4096, fp);
		fclose(fp);

		// Get internal ram file
		strcpy(&output_name[count - 4], ".irm");

		fp = fopen(output_name, "wb");
		if(fp == NULL)
		{
			return;
		}

		fwrite(&buf[0x81000 / 4], 1, 48 * 1024, fp);
		fclose(fp);

		// Get register file
		strcpy(&output_name[count - 4], ".arm");

		fp = fopen(output_name, "wb");
		if(fp == NULL)
		{
			return;
		}

		fwrite(&buf[0x8d000 / 4], 1, 13 * 1024, fp);
		fclose(fp);

		if (file_size > 0x90400)
		{
			unsigned long size = file_size - 0x90400;
			// Get dsp memory file
			strcpy(&output_name[count - 4], ".drm");

			fp = fopen(output_name, "wb");
			if(fp == NULL)
			{
				return;
			}
			
			fwrite(&buf[0x90400 / 4], 1, size, fp);
			fclose(fp);
		}
		break;
	case F_6408:
		// Get the sharmemory file
		strcpy(&output_name[count - 4], ".shr");

		fp = fopen(output_name, "wb");
		if(fp == NULL)
		{
			return;
		}

		fwrite(&buf[0x100000 / 4], 1, 4096, fp);
		fclose(fp);

		// Get internal ram file
		strcpy(&output_name[count - 4], ".irm");

		fp = fopen(output_name, "wb");
		if(fp == NULL)
		{
			return;
		}

		fwrite(&buf[0x101000 / 4], 1, 48 * 1024, fp);
		fclose(fp);

		// Get register file
		strcpy(&output_name[count - 4], ".arm");

		fp = fopen(output_name, "wb");
		if(fp == NULL)
		{
			return;
		}

		fwrite(&buf[0x10d000 / 4], 1, 13 * 1024, fp);
		fclose(fp);

		if (file_size > 0x110400)
		{
			unsigned long size = file_size - 0x110400;
			// Get dsp memory file
			strcpy(&output_name[count - 4], ".drm");

			fp = fopen(output_name, "wb");
			if(fp == NULL)
			{
				return;
			}

			fwrite(&buf[0x110400 / 4], 1, size, fp);
			fclose(fp);
		}

		break;
	case F_6416:
		// Get the sharmemory file
		strcpy(&output_name[count - 4], ".shr");

		fp = fopen(output_name, "wb");
		if(fp == NULL)
		{
			return;
		}

		fwrite(&buf[0x200000 / 4], 1, 4096, fp);
		fclose(fp);

		// Get internal ram file
		strcpy(&output_name[count - 4], ".irm");

		fp = fopen(output_name, "wb");
		if(fp == NULL)
		{
			return;
		}

		fwrite(&buf[0x201000 / 4], 1, 48 * 1024, fp);
		fclose(fp);

		// Get register file
		strcpy(&output_name[count - 4], ".arm");

		fp = fopen(output_name, "wb");
		if(fp == NULL)
		{
			return;
		}

		fwrite(&buf[0x20d000 / 4], 1, 13 * 1024, fp);
		fclose(fp);

		if (file_size > 0x210400)
		{
			unsigned long size = file_size - 0x210400;
			// Get dsp memory file
			strcpy(&output_name[count - 4], ".drm");

			fp = fopen(output_name, "wb");
			if(fp == NULL)
			{
				return;
			}

			fwrite(&buf[0x210400 / 4], 1, size, fp);
			fclose(fp);
		}

		break;
		
	case F_SDRAM_OR_12832:
		// Get the sharmemory file
		strcpy(&output_name[count - 4], ".shr");
		
		fp = fopen(output_name, "wb");
		if(fp == NULL)
		{
			return;
		}
		
		fwrite(&buf[uiDataAreaSize / 4], 1, uiShareMemorySize, fp);
		fclose(fp);
		
		// Get internal ram file
		strcpy(&output_name[count - 4], ".irm");
		
		fp = fopen(output_name, "wb");
		if(fp == NULL)
		{
			return;
		}
		
		fwrite(&buf[(uiDataAreaSize + uiShareMemorySize) / 4], 1, uiIramSize, fp);
		fclose(fp);
		
		// Get register file
		strcpy(&output_name[count - 4], ".arm");
		
		fp = fopen(output_name, "wb");
		if(fp == NULL)
		{
			return;
		}
		
		fwrite(&buf[(uiDataAreaSize + uiShareMemorySize + uiIramSize) / 4], 1, uiRegSize, fp);
		fclose(fp);
		
		if (file_size > (uiDataAreaSize + uiShareMemorySize + uiIramSize + uiRegSize))
		{
			unsigned long size = file_size - (uiDataAreaSize + uiShareMemorySize + uiIramSize + uiRegSize);
			// Get dsp memory file
			strcpy(&output_name[count - 4], ".drm");
			
			fp = fopen(output_name, "wb");
			if(fp == NULL)
			{
				return;
			}
			
			fwrite(&buf[(uiDataAreaSize + uiShareMemorySize + uiIramSize + uiRegSize) / 4], 1, size, fp);
			fclose(fp);
		}
		
		break;

	default:
		break;
	}

}


bool Analyze( const char * path_name, unsigned long* buf, UINT uiMaxFileSize, 
			 UINT uiOffsize, char* szIntList, char* szTaskList)
{
	FILE* fp_mem_bin = NULL;
	char int_table[TABLE_NUM][TABLE_LEN] = {0};
	char  string_table[TABLE_NUM][TABLE_LEN] = {0};
	int index = 0;
	UINT uiTaskNum = 0 ;
	char* token;
	unsigned long file_size_t;

	token = strtok( szTaskList, "," );
	while( token != NULL )
	{
		strcpy( string_table[index], token );
		token = strtok( NULL, "," );
		++index;
	}
	uiTaskNum = index;
	index = 0;

	token = strtok( szIntList, "," );
	while( token != NULL )
	{
		strcpy( int_table[index], token );
		token = strtok( NULL, "," );
		++index;
	}

    unsigned long wakeup_time   = 0;
    unsigned long sleep_time    = 0;
    unsigned long sleep_status  = 0;
    
    unsigned long test_offset = 0;
    
	if((fp_mem_bin = fopen(path_name, "rb")) == NULL)
	{
		return false;
	}

	// First get the offset of the task switch file
	file_size_t = fread(buf, 1, uiMaxFileSize, fp_mem_bin );

	// Search the buf to find the begin of the data
	// TASKMONITORBEGIN
	for (unsigned long i = file_size_t / 4 - 101; i >= 0; i--)
	{
		if (buf[i - 3] == TASKMONITORBEGIN_FIR && buf[i - 2] == TASKMONITORBEGIN_SEC &&
			buf[i - 1] == TASKMONITORBEGIN_THR && buf[i] == TASKMONITORBEGIN_FOU)
		{
			break;
		}
	}

	i -= 3; //move i to the head of "TASKMONITORBEGIN"
	

//    for (unsigned long i = 0x240000; i < file_size_t / 4 - LAST_ZERO_REGION; i++)
//	{
//		if (buf[i] == TASKMONITORBEGIN_FIR && buf[i + 1] == TASKMONITORBEGIN_SEC &&
//			buf[i + 2] == TASKMONITORBEGIN_THR && buf[i + 3] == TASKMONITORBEGIN_FOU)
//		{
//			//break;
//		}
//	}
	
	if (i == file_size_t / 4 - LAST_ZERO_REGION)
	{
		g_pdlg->m_strErr = "The offset of \"TASKMONITORBEGIN\" flag should be multiple of 4!";
		//AfxMessageBox("Error,Please check TASKMONITOR flag position.");
		return false;
	}

	TASK_TEST_POINT_T* test_point = NULL;

	test_point = new TASK_TEST_POINT_T[ g_nMaxSwitchNum ];


	if(i + g_nTaskTableOffset / 4 + g_nMaxSwitchNum * 16 / 4 > file_size_t/4)
	{
		g_pdlg->m_strErr = "The test point is out of file range!";
		//AfxMessageBox("Memory file size maybe error!\nPlease check it.");
		if(test_point != NULL)
		{
			delete [] test_point;
		}
		return false;
	}

	memcpy(test_point, &buf[i + g_nTaskTableOffset / 4], g_nMaxSwitchNum * 16 );

	// Get the offset
	for (i = 0; i < g_nMaxSwitchNum; i++)
	{
		TASK_TEST_POINT_T temp;
		if(g_bBigEndian)
		{
			CONVERT_UINT16(temp.id, test_point[i].id)
			CONVERT_UINT16(temp.accuracy_time, test_point[i].accuracy_time)
			CONVERT_UINT32(temp.pointer, test_point[i].pointer)
			CONVERT_UINT32(temp.return_addr, test_point[i].return_addr)
			CONVERT_UINT32(temp.time, test_point[i].time)
		}
		else
		{
			temp.id = test_point[i].id;
			temp.accuracy_time = test_point[i].accuracy_time;
			temp.pointer = test_point[i].pointer;
			temp.return_addr = test_point[i].return_addr;
			temp.time = test_point[i].time;
 		}
		
		memcpy(&test_point[i], &temp, sizeof(TASK_TEST_POINT_T));
	}

	// Get the offset
	for (i = 0; i < g_nMaxSwitchNum; i++)
	{
		if (test_point[i].pointer == 0xff && test_point[i].return_addr == 0xff)
		{
			test_offset = i + 1;
			break;
		}
	}

	// Printf the signals to a text file
	char output_name[256];
	strcpy(output_name, path_name);

	unsigned long count;
	count = strlen(output_name);
	strcpy(&output_name[count - 4], ".txt");

	FILE* fp = fopen(output_name, "wt");
	if( fp == NULL )
	{
		delete [] test_point;
		return false;
	}

	strcpy(&output_name[count - 4], ".slp");
	FILE* fp_sleep = fopen(output_name, "wt");
	if( fp_sleep == NULL )
	{
		delete [] test_point;
		return false;
	}

	strcpy( &output_name[count - 4], ".csv" );
//	FILE* fp_sta = fopen( output_name, "wt" );
//	if( fp_sta == NULL )
//	{
//		delete [] test_point;
//		return false;
//	}
//	fprintf( fp_sta, "name,priority,time(ms),time(us),percent,\n" );

	//test add for teana only under us mode
	FILE *fp_debug = NULL;
	if(g_percentMode == 1)
	{
		strcpy( &output_name[count - 4], "_debug.txt" );
		fp_debug = fopen(output_name, "wt");
	}
	//test
	
	int nStartTime = 0x7FFFFFFF;
	int nEndTime = 0 ;
	int nLastId = -1;
	int nLastIrq = -1;
	int nLastFiq = -1;
	TaskInfoMap   taskMap;
	TaskInfoMap   irqMap;
	TaskInfoMap   fiqMap;
	int nTime = 0;
	int nLastTime = 0;
	int nSleepStartTime = 0;
	int nSleepTotalTime = 0;
	int nIdleStartTime = 0;
	int nIdleTotalTime = 0 ;
	int nErrorTime = 0;

	//teana hu 
	unsigned long nTotalUsTime = 0;
	unsigned long nIdleUsTime = 0;
	bool bIdle = false;

	unsigned long temp_offset = test_offset;

	char test_txt[400] = {0};
	char sleep_txt[400] = {0};

	fputs("Action       Start time       Accuracy time\r\n", fp);

	for (i = 0; i < g_nMaxSwitchNum; i++)
	{
		unsigned long delta = 0;
		if (temp_offset == 0)
		{
			if(g_percentMode == 0)
			{
				delta = test_point[g_nMaxSwitchNum - 1].accuracy_time - test_point[0].accuracy_time;
				delta = (unsigned long) (delta * 1.2);
			}
			else if(g_percentMode == 1)
			{
				if(test_point[g_nMaxSwitchNum - 1].accuracy_time >= test_point[0].accuracy_time)
				{
					delta = test_point[g_nMaxSwitchNum - 1].accuracy_time - test_point[0].accuracy_time;					
				}
				else
				{
					delta = test_point[g_nMaxSwitchNum - 1].accuracy_time + MAX_TIME_SIZE - test_point[0].accuracy_time;
				}
				delta = (unsigned long)(delta * (1000 / 32.768));
			}
		}
		else
		{	
			if(g_percentMode == 0)
			{
				delta = test_point[temp_offset - 1].accuracy_time - test_point[temp_offset].accuracy_time;
				delta = (unsigned long) (delta * 1.2);
			}
			else if(g_percentMode == 1)
			{
				if(test_point[temp_offset - 1].accuracy_time >= test_point[temp_offset].accuracy_time)
				{
					delta = test_point[temp_offset - 1].accuracy_time - test_point[temp_offset].accuracy_time;
				}
				else
				{
					delta = test_point[temp_offset - 1].accuracy_time + MAX_TIME_SIZE - test_point[temp_offset].accuracy_time;
				}
				delta = (unsigned long)(delta * (1000 / 32.768));
			}
		}
		if(g_percentMode == 1)
		{
			if(delta > MAX_ERROR_US)
			{
				temp_offset++;
				continue;
			}
		}
		
		if (sleep_status == 1)
		{
			sleep_status = 0;
			
			if ((temp_offset + 1) >= g_nMaxSwitchNum)
			{
				sprintf(sleep_txt, "LINE: %d, SLEEP: %dms\r\n", i+2, test_point[0].time - sleep_time);
				wakeup_time = test_point[0].time;
			}
			else
			{
				sprintf(sleep_txt, "LINE: %d, SLEEP: %dms\r\n", i+2, test_point[temp_offset + 1].time - sleep_time);
				wakeup_time = test_point[temp_offset + 1].time;
			}

			fputs(sleep_txt, fp_sleep);
		}
		//apple
		
		//teana
		if(g_percentMode == 0)
		{
			nTime = test_point[temp_offset].time * 1000 + delta;
		}
		else if(g_percentMode == 1)
		{
			nTime = delta;
		}

		if( 0 == nLastTime )
		{
			nLastTime = nTime;
		}

		while( nLastTime > nTime )
		{	
			nTime += 1000;
		}		

		nEndTime  = max( nEndTime, nTime );
		nStartTime = min( nStartTime, nTime );				
		
		if( nTime > nLastTime +  MAX_SPACE_TIME )
		{
			nErrorTime += nTime - nLastTime;	
			nLastTime = nTime;
			i = i -1;
			continue;
		}
		nLastTime = nTime;

		//apple
		switch(test_point[temp_offset].id)
		{
		case DS_IDLE_REQ:
			{
			sprintf(test_txt, "IDLE       : %dms;    0x%4x(%6dus); Sleep mode 0x%x\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta, test_point[temp_offset].pointer);
			//apple
			TaskInfoMap::iterator j = taskMap.find( nLastId );
			if( j != taskMap.end() )
			{
				if(g_percentMode == 0)
				{
					while( nTime < (*j).second.nStartTime )
					{
						nTime += 1000;
					}
					(*j).second.nTotalTime += nTime - (*j).second.nStartTime;
				}
				else
				{
					(*j).second.nTotalTime += (int)delta;
				}
				if(strstr((*j).second.szName, g_szDebugString) != NULL && fp_debug != NULL)
				{
					char szTest[128] = {0};
					sprintf(szTest, "-----------%s: %d, total: %d\n", g_szDebugString, delta, (*j).second.nTotalTime);
					fwrite(szTest, 1, strlen(szTest), fp_debug);
				}
			}
			nLastId = -1;

			nIdleStartTime = nTime;
			bIdle = true;
			}
			//
			break;
		case DS_TASK_REQ:
			{		
				unsigned long task_id = 0;
				unsigned long offset = 0;
				unsigned long priority_id = 0;
				unsigned long priority = 0;
				char szname[TABLE_LEN] = {0};	
				
				offset = test_point[temp_offset].pointer;

				for(int k = 0; k < TABLE_NUM; k++)
				{
					if(g_nTcbPtr[k] == offset)
					{
						strcpy(szname, string_table[k]);
						task_id = k;
						break;
					}
				}
				offset -= uiOffsize;
				fseek( fp_mem_bin, offset, SEEK_SET );			
				TX_THREAD tx;
				fread( &tx, sizeof( TX_THREAD ), 1, fp_mem_bin );		
				priority_id = tx.tx_priority ;
				if(g_bBigEndian)
				{
					CONVERT_UINT32( priority, priority_id);
				}
				else
				{
					priority = priority_id;
				}
				
				
				if( szname[0] == '\0' )
				{
					task_id = test_point[temp_offset].pointer;
					sprintf(  szname, "Unknown 0x%x", task_id);
					sprintf(test_txt, "TASK       : %dms;    0x%4x(%6dus); Task       0x%x(%s)\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta, test_point[temp_offset].pointer, szname);
				}
				else
				{
					sprintf(test_txt, "TASK       : %dms;    0x%4x(%6dus); Task       0x%x(0x%x, %s)\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta, test_point[temp_offset].pointer, task_id, szname);
				}
				
				//apple
				TaskInfoMap::iterator j = taskMap.find( task_id );
				if( j != taskMap.end() )
				{
					(*j).second.nStartTime = nTime; 
				}
				else
				{
					TASKINFO_T taskinfo;
					strcpy( taskinfo.szName, szname);						
					taskinfo.nStartTime = nTime;
					taskinfo.npriority = priority;
					taskinfo.nTotalTime = 0 ;
					taskMap[ task_id ] = taskinfo;
				}
				nLastId = task_id;

				if( nIdleStartTime != 0 )
				{
					nIdleTotalTime  += nTime - nIdleStartTime;
					nIdleStartTime = 0;
				}
				//apple

				//teana hu
				if(bIdle)
				{
					nIdleUsTime += delta;
					bIdle = false;
					
					if(fp_debug != NULL && strstr(g_szDebugString, "Idle") != NULL)
					{
						char szTest[128] = {0};
						sprintf(szTest, "-----------Idle: %d, total: %d\n", delta, nIdleUsTime);
						fwrite(szTest, 1, strlen(szTest), fp_debug);
					}
				}
				
			}
			break;

		case DS_TASK_SUSP_REQ:
			{
				//apple
				TaskInfoMap::iterator j = taskMap.find( nLastId );
				if( j != taskMap.end() )
				{
					if(g_percentMode == 0)
					{
						while( nTime < (*j).second.nStartTime )
						{
							nTime += 1000;
						}
						(*j).second.nTotalTime += nTime - (*j).second.nStartTime;
					}
					else
					{
						 (*j).second.nTotalTime += (int)delta;
					}
					if(strstr((*j).second.szName, g_szDebugString) != NULL && fp_debug != NULL)
					{
						char szTest[128] = {0};
						sprintf(szTest, "-----------%s: %d, total: %d\n", g_szDebugString, delta, (*j).second.nTotalTime);
						fwrite(szTest, 1, strlen(szTest), fp_debug);
					}
				}
				nLastId = -1;

				if( nIdleStartTime != 0 )
				{
					nIdleTotalTime  += nTime - nIdleStartTime;
					nIdleStartTime = 0;
				}
				
			    //

				//teana hu
				if(bIdle)
				{
					nIdleUsTime += delta;
					bIdle = false;
					
					if(fp_debug != NULL && strstr(g_szDebugString, "Idle") != NULL)
					{
						char szTest[128] = {0};
						sprintf(szTest, "-----------Idle: %d, total: %d\n", delta, nIdleUsTime);
						fwrite(szTest, 1, strlen(szTest), fp_debug);
					}
				}
			}
			break;
		case DS_ENTER_INT_REQ:
			{
				char IRQ_String[400];
				unsigned long irq_status = test_point[temp_offset].pointer;

				IRQ_String[0] = '\0';

				for (int j = 0; j < 32; j++)
				{
					if (irq_status & (1 << j))
					{
						strcat(IRQ_String, int_table[j]);
						strcat(IRQ_String, "+");
					}
				}
				int len = strlen(IRQ_String);
				IRQ_String[len - 1] = '\0';

				sprintf(test_txt, "ENTER IRQ  : %dms;    0x%4x(%6dus); Irq status 0x%x(%s); Return Addr 0x%x\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta, test_point[temp_offset].pointer, IRQ_String, test_point[temp_offset].return_addr);
				
				//apple
				TaskInfoMap::iterator l = taskMap.find( nLastId );
				if( l != taskMap.end() )
				{
					if(g_percentMode == 0)
					{
						while( nTime < (*l).second.nStartTime )
						{
							nTime += 1000;
						}
						(*l).second.nTotalTime += nTime - (*l).second.nStartTime;
					}
					else
					{
						(*l).second.nTotalTime += (int)delta;
					}
					if(strstr((*l).second.szName, g_szDebugString) != NULL && fp_debug != NULL)
					{
						char szTest[128] = {0};
						sprintf(szTest, "-----------%s: %d, total: %d\n", g_szDebugString, delta, (*l).second.nTotalTime);
						fwrite(szTest, 1, strlen(szTest), fp_debug);
					}
				}
				

				TaskInfoMap::iterator k = irqMap.find( irq_status );
				if( k != irqMap.end() )
				{
					(*k).second.nStartTime = nTime; 
				}
				else
				{
					TASKINFO_T taskinfo;
					if( IRQ_String[0] != '\0' )
					{
						strcpy( taskinfo.szName, IRQ_String );					
					}
					else
					{
						sprintf( taskinfo.szName, "UnknownIRQ%d", irq_status );
					}
					
					taskinfo.nStartTime = nTime;
					taskinfo.npriority = 0;
					taskinfo.nTotalTime = 0 ;
					irqMap[ irq_status ] = taskinfo;
				}
				nLastIrq = irq_status;

				if( nIdleStartTime != 0 )
				{
					nIdleTotalTime  += nTime - nIdleStartTime;
					nIdleStartTime = 0;
				}
				
				//apple

				//teana hu
				if(bIdle)
				{
					nIdleUsTime += delta;
					bIdle = false;
					
					if(fp_debug != NULL && strstr(g_szDebugString, "Idle") != NULL)
					{
						char szTest[128] = {0};
						sprintf(szTest, "-----------Idle: %d, total: %d\n", delta, nIdleUsTime);
						fwrite(szTest, 1, strlen(szTest), fp_debug);
					}
				}
			}
			break;
		case DS_LEAVE_INT_REQ:
			{
				sprintf(test_txt, "LEAVE IRQ  : %dms;    0x%4x(%6dus);\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta);
				//apple				
				TaskInfoMap::iterator j = irqMap.find( nLastIrq );
				if( j != irqMap.end() )
				{
					if( (*j).second.nStartTime != 0 )
					{
						if(g_percentMode == 0)
						{
							while( nTime < (*j).second.nStartTime )
							{
								nTime += 1000;
							}
							(*j).second.nTotalTime += nTime - (*j).second.nStartTime;
						}
						else
						{
							(*j).second.nTotalTime += (int)delta;
						}
						
						(*j).second.nStartTime = 0;	
						
						if(strstr((*j).second.szName, g_szDebugString) != NULL && fp_debug != NULL)
						{
							char szTest[128] = {0};
							sprintf(szTest, "-----------%s: %d, total: %d\n", g_szDebugString, delta, (*j).second.nTotalTime);
							fwrite(szTest, 1, strlen(szTest), fp_debug);
						}
						
					}


				}

				j = taskMap.find( nLastId );
				if( j != taskMap.end() )
				{
					(*j).second.nStartTime  = nTime;
				}				
				//apple

				//teana hu
				if(bIdle)
				{
					nIdleUsTime += delta;
					bIdle = false;
					
					if(fp_debug != NULL && strstr(g_szDebugString, "Idle") != NULL)
					{
						char szTest[128] = {0};
						sprintf(szTest, "-----------Idle: %d, total: %d\n", delta, nIdleUsTime);
						fwrite(szTest, 1, strlen(szTest), fp_debug);
					}
				}
			}
			break;
		case DS_ENTER_SLEEP_REQ:
			{
			sprintf(test_txt, "ENTER SLEEP: %dms;    0x%4x(%6dus);\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta);
			
			sprintf(sleep_txt, "************************LINE: %d, WAKEUP: %dms\r\n", i+2, test_point[temp_offset].time - wakeup_time);
			sleep_status = 1;
			sleep_time = test_point[temp_offset].time;

			fputs(sleep_txt, fp_sleep);

			//apple
			TaskInfoMap::iterator j = taskMap.find( nLastId );
			if( j != taskMap.end() )
			{
				if(g_percentMode == 0)
				{
					while( nTime < (*j).second.nStartTime )
					{
						nTime += 1000;
					}
					(*j).second.nTotalTime += nTime - (*j).second.nStartTime;
				}
				else
				{
					(*j).second.nTotalTime += (int)delta;
				}
				if(strstr((*j).second.szName, g_szDebugString) != NULL && fp_debug != NULL)
				{
					char szTest[128] = {0};
					sprintf(szTest, "-----------%s: %d, total: %d\n", g_szDebugString, delta, (*j).second.nTotalTime);
					fwrite(szTest, 1, strlen(szTest), fp_debug);
				}
			}
			nSleepStartTime = nTime;

			if( nIdleStartTime != 0 )
			{
				nIdleTotalTime  += nTime - nIdleStartTime;
				nIdleStartTime = 0;
			}
			
			//apple

			//teana hu
				if(bIdle)
				{
					nIdleUsTime += delta;
					bIdle = false;
					
					if(fp_debug != NULL && strstr(g_szDebugString, "Idle") != NULL)
					{
						char szTest[128] = {0};
						sprintf(szTest, "-----------Idle: %d, total: %d\n", delta, nIdleUsTime);
						fwrite(szTest, 1, strlen(szTest), fp_debug);
					}
				}
			}
			break;
		case DS_LEAVE_SLEEP_REQ:
			{
			sprintf(test_txt, "LEAVE SLEEP: %dms;    0x%4x(%6dus);\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta);
			//apple
			TaskInfoMap::iterator j = taskMap.find( nLastId );
			if( j != taskMap.end() )
			{
				(*j).second.nStartTime  = nTime;
			}
			nSleepTotalTime += nTime - nSleepStartTime;
			nSleepStartTime = 0;
			
			//

			//teana hu
				if(bIdle)
				{
					nIdleUsTime += delta;
					bIdle = false;
					
					if(fp_debug != NULL && strstr(g_szDebugString, "Idle") != NULL)
					{
						char szTest[128] = {0};
						sprintf(szTest, "-----------Idle: %d, total: %d\n", delta, nIdleUsTime);
						fwrite(szTest, 1, strlen(szTest), fp_debug);
					}
				}
			}
			break;
		case DS_TEST_POINT:
			sprintf(test_txt, "TEST POINT : %dms;    0x%4x(%6dus),   0x%x,   %d\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta, test_point[temp_offset].pointer, test_point[temp_offset].return_addr);
			break;
		case DS_ENTER_FIQ_REQ:
			{
				char FIQ_String[400];
				unsigned long fiq_status = test_point[temp_offset].pointer;
				
				FIQ_String[0] = '\0';
				
				for (int j = 0; j < 24; j++)
				{
					if (fiq_status & (1 << j))
					{
						strcat(FIQ_String, int_table[j]);
						strcat(FIQ_String, "+");
					}
				}
				int len = strlen(FIQ_String);
				FIQ_String[len - 1] = '\0';
				
				sprintf(test_txt, "ENTER FIQ  : %dms;    0x%4x(%6dus); Irq status 0x%x(%s); Return Addr 0x%x\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta, test_point[temp_offset].pointer, FIQ_String, test_point[temp_offset].return_addr);
				
				//apple
				TaskInfoMap::iterator k = fiqMap.find( fiq_status );
				if( k != fiqMap.end() )
				{
					(*k).second.nStartTime = nTime; 
				}
				else
				{
					TASKINFO_T taskinfo;
					if( FIQ_String[0] != '\0' )
					{
						strcpy( taskinfo.szName, FIQ_String );					
					}
					else
					{
						sprintf( taskinfo.szName, "UnknowFIQ%d", fiq_status );
					}
					taskinfo.nStartTime = nTime;
					taskinfo.nTotalTime = 0 ;
					taskinfo.npriority = 0;
					fiqMap[ fiq_status ] = taskinfo;
				}
				nLastFiq = fiq_status;

				TaskInfoMap::iterator l = taskMap.find( nLastId );
				if( l != taskMap.end() )
				{
					if(g_percentMode == 0)
					{
						while( nTime < (*l).second.nStartTime )
						{
							nTime += 1000;
						}
						(*l).second.nTotalTime += nTime - (*l).second.nStartTime;
					}
					else
					{
						(*l).second.nTotalTime += (int)delta;
					}
					if(strstr((*l).second.szName, g_szDebugString) != NULL && fp_debug != NULL)
					{
						char szTest[128] = {0};
						sprintf(szTest, "-----------%s: %d, total: %d\n", g_szDebugString, delta, (*l).second.nTotalTime);
						fwrite(szTest, 1, strlen(szTest), fp_debug);
					}
				}

				if( nIdleStartTime != 0 )
				{
					nIdleTotalTime  += nTime - nIdleStartTime;
					nIdleStartTime = 0;
				}
				
				//apple

				//teana hu
				if(bIdle)
				{
					nIdleUsTime += delta;
					bIdle = false;
					
					if(fp_debug != NULL && strstr(g_szDebugString, "Idle") != NULL)
					{
						char szTest[128] = {0};
						sprintf(szTest, "-----------Idle: %d, total: %d\n", delta, nIdleUsTime);
						fwrite(szTest, 1, strlen(szTest), fp_debug);
					}
				}
			}
			break;
		case DS_LEAVE_FIQ_REQ:
			{
				sprintf(test_txt, "LEAVE FIQ  : %dms;    0x%4x(%6dus);\r\n", test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta);
				//apple
				
				TaskInfoMap::iterator j = fiqMap.find( nLastFiq );
				if( j != fiqMap.end() )
				{
					if( (*j).second.nStartTime != 0 )
					{
						if(g_percentMode == 0)
						{
							while( nTime < (*j).second.nStartTime )
							{
								nTime += 1000;
							}
							(*j).second.nTotalTime += nTime - (*j).second.nStartTime;
							(*j).second.nStartTime = 0;	
						}
						else
						{
							(*j).second.nTotalTime += (int)delta;
						}

						if(strstr((*j).second.szName, g_szDebugString) != NULL && fp_debug != NULL)
						{
							char szTest[128] = {0};
							sprintf(szTest, "-----------%s: %d, total: %d\n", g_szDebugString, delta, (*j).second.nTotalTime);
							fwrite(szTest, 1, strlen(szTest), fp_debug);
						}
					}
				}

				j = taskMap.find( nLastId );
				if( j != taskMap.end() )
				{
					(*j).second.nStartTime  = nTime;
				}
				//apple

				//teana hu
				if(bIdle)
				{
					nIdleUsTime += delta;
					bIdle = false;
					
					if(fp_debug != NULL && strstr(g_szDebugString, "Idle") != NULL)
					{
						char szTest[128] = {0};
						sprintf(szTest, "-----------Idle: %d, total: %d\n", delta, nIdleUsTime);
						fwrite(szTest, 1, strlen(szTest), fp_debug);
					}
				}
			}
			break;
		default:
			sprintf(test_txt, "TEST POINT : %d;    %dms;    0x%4x(%6dus);\r\n", test_point[temp_offset].id, test_point[temp_offset].time, test_point[temp_offset].accuracy_time, delta );
			break;
		}

		
		fputs(test_txt, fp);
		temp_offset++;

		if (temp_offset >= g_nMaxSwitchNum )
			temp_offset = 0;
	}

	//apple
	int nAllTotalTime = nEndTime - nStartTime;
	int nFixTime = nAllTotalTime - nErrorTime;
	if(g_percentMode == 1)
	{
		TaskInfoMap::iterator it;
		for( it = taskMap.begin(); it != taskMap.end(); ++it )
		{
			nTotalUsTime += (unsigned int)(*it).second.nTotalTime;
		}

		for( it = irqMap.begin(); it != irqMap.end(); ++it )
		{
			nTotalUsTime += (unsigned int)(*it).second.nTotalTime;
		}
		
		for( it = fiqMap.begin(); it != fiqMap.end(); ++it )
		{
			nTotalUsTime += (unsigned int)(*it).second.nTotalTime;
		}
		nIdleTotalTime = nIdleUsTime;

		nTotalUsTime += (unsigned int)nIdleTotalTime;

		nFixTime = nAllTotalTime = nTotalUsTime;
	}

//	fprintf(fp_sta, "Total time, ,%d, %d \n", nAllTotalTime / 1000, nAllTotalTime % 1000 );
//	fprintf(fp_sta, "Fix time,, %d, %d \n", nFixTime / 1000, nFixTime % 1000 );
//	double t = (double)nFixTime / 1000.0;
//	t = t != 0 ? t : LDBL_EPSILON;
//	fprintf( fp_sta, "Idle and Switch time,, %d, %d, %3f%%\n", nIdleTotalTime / 1000, nIdleTotalTime % 1000 , (double)((double)nIdleTotalTime / 10.0) / t );
	
//	fprintf( fp_sta, "Sleep time, ,%d, %d,%3f%%\n", nSleepTotalTime / 1000, nSleepTotalTime % 1000 , (double)((double)nSleepTotalTime / 10.0) / t );
//	TaskInfoMap::iterator it;
//	fprintf( fp_sta, "=========Task==========\n" );
//	for( it = taskMap.begin(); it != taskMap.end(); ++it )
//	{
//		fprintf( fp_sta, "%s,%d,%d,%d,%3f%%\n",(*it).second.szName, (*it).second.npriority, (*it).second.nTotalTime/1000, (*it).second.nTotalTime % 1000, (double)((double)(*it).second.nTotalTime / 10.0 )/ t);
//	}
//	fprintf( fp_sta, "============IRQ==========\n" );
//	for( it = irqMap.begin(); it != irqMap.end(); ++it )
//	{
//		fprintf( fp_sta, "%s,,%d,%d,%3f%%\n",(*it).second.szName, (*it).second.nTotalTime/1000, (*it).second.nTotalTime % 1000, (double)((double)(*it).second.nTotalTime / 10.0) / t);
//	}
//	fprintf( fp_sta, "============FIQ==========\n" );
//	for( it = fiqMap.begin(); it != fiqMap.end(); ++it )
//	{
//		fprintf( fp_sta, "%s,,%d,%d,%3f%%\n",(*it).second.szName, (*it).second.nTotalTime/1000, (*it).second.nTotalTime % 1000, (double)((double)(*it).second.nTotalTime / 10.0) / t);
//	}
//	fclose( fp_sta );

	if( fp_mem_bin != NULL )//lint !e774
	{
		fclose( fp_mem_bin );
	}
	
	if(fp_debug != NULL)
	{
		fclose(fp_debug);
		fp_debug = NULL;
	}
	//


        
	sprintf(test_txt, "**********************End of the task switch*********************\r\n");
	fputs(test_txt, fp);
	fclose(fp);
	fclose(fp_sleep);

	delete [] test_point;
	
    return true;
}//lint !e429

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool SplitFile(const char * path_name,unsigned long* buf,
			   UINT uiDspSize, UINT uiShareOffset, 
			   UINT uiIramOffset, UINT uiRegOffset, UINT uiDspOffset, UINT uiShareMemorySize,
			   UINT uiIramSize , UINT uiRegSize, UINT uiMaxFileSize)
{
	FILE * fp;

	char output_name[256];
	strcpy(output_name, path_name);
	unsigned long count;
	count = strlen(output_name);
	
	BOOL bSplit = TRUE;

	// Get the sharmemory file
	if(uiMaxFileSize < (uiShareOffset + uiShareMemorySize)/ 4)
	{
		g_pdlg->AddTextToList("Split share memory block failed! Please check share memory size!", FAIL_COLOR, FALSE);
		CString strContent;
		strContent.Format("    Offset: 0x%x, size: 0x%x", uiShareOffset, uiShareMemorySize);
		g_pdlg->AddTextToList(strContent, FAIL_COLOR, FALSE);
		bSplit = FALSE;
		//AfxMessageBox("Sharememory block size may be error!\nPlease check it!");
		//return false;
	}
	
	strcpy(&output_name[count - 4], ".shr");
	
	if((fp = fopen(output_name, "wb")) == NULL)
	{
		AfxMessageBox("Failed to open *.shr file.\n");
		//return false;
	}
	if(fp != NULL)
	{
		fwrite(&buf[uiShareOffset / 4], 1, uiShareMemorySize, fp);
		fclose(fp);
		if(bSplit)
		{
			g_pdlg->AddTextToList("Split share memory block successfully!", NORMAL_COLOR, FALSE);
		}
	}
	
	// Get internal ram file
	bSplit = TRUE;
	//if(uiMaxFileSize < (uiShareOffset + uiShareMemorySize + uiIramSize) / 4 )
	if(uiMaxFileSize < (uiIramOffset + uiIramSize) / 4 )
	{
		g_pdlg->AddTextToList("Split ram memory block failed! Please check ram memory size!", FAIL_COLOR, FALSE);
		CString strContent;
		strContent.Format("    Offset: 0x%x, size: 0x%x", uiIramOffset, uiIramSize);
		g_pdlg->AddTextToList(strContent, FAIL_COLOR, FALSE);
		bSplit = FALSE;
		//AfxMessageBox("Ram block size may be error!\nPlease check it!");
		//return false;
	}
	strcpy(&output_name[count - 4], ".irm");
	
	if((fp = fopen(output_name, "wb")) == NULL)
	{
		AfxMessageBox("Failed to open *.irm file.\n");
		//return false;
	}

	if(fp != NULL)
	{
		fwrite(&buf[uiIramOffset / 4], 1, uiIramSize, fp);
		fclose(fp);
		if(bSplit)
		{
			g_pdlg->AddTextToList("Split ram memory block successfully!", NORMAL_COLOR, FALSE);
		}
	}
	
	// Get register file
	bSplit = TRUE;
	//if(uiMaxFileSize < (uiShareOffset + uiShareMemorySize + uiIramSize + uiRegSize) / 4 )
	if(uiMaxFileSize < (uiRegOffset + uiRegSize) / 4 )
	{
		g_pdlg->AddTextToList("Split register memory block failed! Please check register memory size!", FAIL_COLOR, FALSE);
		CString strContent;
		strContent.Format("    Offset: 0x%x, size: 0x%x", uiRegOffset, uiRegSize);
		g_pdlg->AddTextToList(strContent, FAIL_COLOR, FALSE);
		bSplit = FALSE;
		//AfxMessageBox("Register block size may be error!\nPlease check it!");
		//return false;
	}
	strcpy(&output_name[count - 4], ".arm");
	
	if((fp = fopen(output_name, "wb")) == NULL)
	{
		AfxMessageBox("Failed to open *.arm file.\n");
		//return false;
	}
	
	if(fp != NULL)
	{
		fwrite(&buf[uiRegOffset / 4], 1, uiRegSize, fp);
		fclose(fp);
		if(bSplit)
		{
			g_pdlg->AddTextToList("Split register memory block successfully!", NORMAL_COLOR, FALSE);
		}
	}
	
	// Get dsp memory file
	//teana.hu 2010.03.22  L has dsp memory, but 8800s not
	if(uiDspSize == 0)
	{
		return true;
	}
	//
	bSplit = TRUE;
	//if(uiMaxFileSize < (uiShareOffset + uiShareMemorySize + uiIramSize + uiRegSize + uiDspSize) / 4)
	if(uiMaxFileSize < (uiDspOffset + uiDspSize) / 4)
	{
		g_pdlg->AddTextToList("Split dsp memory block failed! Please check dsp memory size!", FAIL_COLOR, FALSE);
		CString strContent;
		strContent.Format("    Offset: 0x%x, size: 0x%x", uiDspOffset, uiDspSize);
		g_pdlg->AddTextToList(strContent, FAIL_COLOR, FALSE);
		bSplit = FALSE;
		//AfxMessageBox("Dsp block size may be error!\nPlease check it!");
		//return false;
	}

	strcpy(&output_name[count - 4], ".drm");
	
	if((fp = fopen(output_name, "wb")) == NULL)
	{
		AfxMessageBox("Failed to open *.drm file.\n");
		//return false;
	}
	
	if(fp != NULL)
	{
		fwrite(&buf[uiDspOffset / 4], 1, uiDspSize, fp);
		fclose(fp);
		if(bSplit)
		{
			g_pdlg->AddTextToList("Split dsp memory block successfully!", NORMAL_COLOR, FALSE);
		}
	}

	return true;
}
/*
void SwapChar(char* pStr, int nLen)
{
	int i = 0;
	while(i < nLen)
	{
		if(*pStr == '/')
		{
			*pStr = ' ';
		}
		pStr ++;
		i ++;
	}
}*/

//modify by teana 2011.01.17


#define TM_IDLE_REQ            0x10
#define TM_TASK_REQ            0x20
#define TM_ENTER_INT_REQ       0x30
#define TM_LEAVE_INT_REQ       0x40
#define TM_ENTER_SLEEP_REQ     0x50
#define TM_LEAVE_SLEEP_REQ     0x60
#define TM_TEST_POINT_REQ      0x70
#define TM_ENTER_FIQ       	   0x80
#define TM_LEAVE_FIQ       	   0x90

typedef struct _TM_TIME_INFO
{
	uint32 acuracy_time;
	uint32 time;
}TM_TIME_INFO;

TM_TIME_INFO g_TMIdleInfo = {0};
TM_TIME_INFO g_TMTaskInfo = {0};
TM_TIME_INFO g_TMIrqInfo = {0};
TM_TIME_INFO g_TMFiqInfo = {0};
TM_TIME_INFO g_TMSleepInfo = {0};
TM_TIME_INFO g_TMTestInfo = {0};

unsigned char g_szIrqName[32][32] = 
{
"Special",
"Software-trigged channel",
"UART0 Interrupt",
"UART1 Interrupt",
"UART2 Interrupt",
"Generic Timer 0 Interrupt",
"Generic Timer 1 Interrupt",
"Generic Timer 2 Interrupt",
"GPIO Combined Interrupt",
"SPI Interrupt",
"Keypad Interrupt",
"I2C Interrupt",
"SIM Interrupt",
"PIU_SER_INT",
"PIU_CR_HINT",
"DSP IRQ0",
"DSP IRQ1",
"System Timer Interrupt",
"EPT Interrupt",
"IIS Interrupt",
"DSP INT",
"DMA Interrupt",
"VBC Interrupt",
"VSP Interrupt",
"Ana_ irq",
"ADI module Interrupt",
"USB Interrupt",
"DCAM Interrupt",
"NFC Interrupt",
"LCDC Interrupt",
"SDIO Interrpt",
"Bus_mon"
};

TM_TIME_INFO g_TMIrqInfoX[32] = {0};
TM_TIME_INFO g_TMTaskInfoX[1024] = {0};
unsigned int g_TestTime = 0;
unsigned int uiIrqBefore = 0;
unsigned int g_LastTaskPointer = 0;

TM_INFO g_TMInfo = {0};
TM_TASK_NAME g_TMTaskName[1024] = {0};
unsigned int g_TaskNameCount = 0;

unsigned int GetDeltaTime(unsigned int uiNow, unsigned int uiBefore)
{
	TM_INFO *p_TMInfo = &g_TMInfo;
	unsigned int uiNowTime = p_TMInfo->point_tab[uiNow].time;
	unsigned int uiBeforeTime = p_TMInfo->point_tab[uiBefore].time;
	unsigned int uiNowAcuracy = p_TMInfo->point_tab[uiNow].acuracy_time;
	unsigned int uiBeforeAcuracy = p_TMInfo->point_tab[uiBefore].acuracy_time;
	unsigned int uiRetAcuracy = 0;

	if (uiNowTime < uiBeforeTime)
	{
	//	AfxMessageBox("Time calculate Error[Time Unlegal]....\n");
		return 0;
	}

	if (uiNowAcuracy <= uiBeforeAcuracy)
	{
		uiRetAcuracy = uiBeforeAcuracy - uiNowAcuracy;
	}
	else
	{
		uiRetAcuracy = 0xFFFF - uiNowAcuracy + uiBeforeAcuracy;
	}

	return uiRetAcuracy;
}

unsigned int AssignTaskTime(unsigned int uiDeltaTime, unsigned int uiPointer)
{
	unsigned int i = 0;
	for (i = 0; i < g_TaskNameCount; i++)
	{
		if (g_TMTaskName[i].taskid == uiPointer)
		{
			g_TMTaskInfoX[i].time += uiDeltaTime;
			return 0;
		}
	}

	g_TMTaskName[g_TaskNameCount].taskid = uiPointer;
	sprintf((char *)g_TMTaskName[g_TaskNameCount].taskname, "UnKnown 0x%08x", uiPointer);
	g_TMTaskInfoX[g_TaskNameCount].time += uiDeltaTime;
	g_TaskNameCount++;
	return 0;
}

unsigned int AssignDeltaTime(unsigned int uiPrevId, unsigned int uiNowId, unsigned int uiDeltaTime, 
							 unsigned int uiPrevPos)
{
	unsigned int j = 0;

	switch (uiPrevId)
	{
	case TM_IDLE_REQ:
		g_TMIdleInfo.time += uiDeltaTime;
		uiIrqBefore = TM_IDLE_REQ;
		break;
	case TM_TASK_REQ:
		g_TMTaskInfo.time += uiDeltaTime;
		g_LastTaskPointer = g_TMInfo.point_tab[uiPrevPos].pointer;
		AssignTaskTime(uiDeltaTime, g_LastTaskPointer);
		uiIrqBefore = TM_TASK_REQ;
		break;
	case TM_ENTER_INT_REQ:
		if (uiNowId != TM_LEAVE_INT_REQ)
		{
			g_pdlg->m_strErr = "The IRQ Enter and Leave no match...";
			//AfxMessageBox("The IRQ Enter and Leave no match...\n");
			break;
		}
		else
		{
			g_TMIrqInfo.time += uiDeltaTime;
			for (j = 0; !((g_TMInfo.point_tab[uiPrevPos].pointer >> j) & 0x1) && j < 32; j++);
			g_TMIrqInfoX[j].time += uiDeltaTime;
		}
		break;
	case TM_LEAVE_INT_REQ:
		if (uiIrqBefore == TM_IDLE_REQ || uiIrqBefore == 0)
		{
			g_TMIdleInfo.time += uiDeltaTime;
		}
		else if (uiIrqBefore == TM_TASK_REQ)
		{
			g_TMTaskInfo.time += uiDeltaTime;
			AssignTaskTime(uiDeltaTime, g_LastTaskPointer);
		}
		else
		{
			g_pdlg->m_strErr = "The status before enter IRQ was error...";
			//AfxMessageBox("The status before enter IRQ was error...\n");
			break;
		}
		break;
	case TM_ENTER_SLEEP_REQ:
		if (uiNowId != TM_LEAVE_SLEEP_REQ)
		{
			g_pdlg->m_strErr = "The SLEEP Enter and Leave no match...";
			//AfxMessageBox("The SLEEP Enter and Leave no match...\n");
			break;
		}
		else
		{
			g_TMSleepInfo.time += uiDeltaTime;
		}
		break;
	case TM_LEAVE_SLEEP_REQ:
		g_TMIdleInfo.time += uiDeltaTime;
		break;
	case TM_ENTER_FIQ:
		if (uiNowId != TM_LEAVE_FIQ)
		{
			g_pdlg->m_strErr = "The FIQ Enter and Leave no match...";
			//AfxMessageBox("The FIQ Enter and Leave no match...\n");
			break;
		}
		else
		{
			g_TMFiqInfo.time += uiDeltaTime;
		}
		break;
	case TM_LEAVE_FIQ:
		if (uiIrqBefore == TM_IDLE_REQ || uiIrqBefore == 0)
		{
			g_TMIdleInfo.time += uiDeltaTime;
		}
		else if (uiIrqBefore == TM_TASK_REQ)
		{
			g_TMTaskInfo.time += uiDeltaTime;
			AssignTaskTime(uiDeltaTime, g_LastTaskPointer);
		}
		else
		{
			g_pdlg->m_strErr = "The status before enter FIQ was error...";
			//AfxMessageBox("The status before enter FIQ was error...\n");
			break;
		}
		break;
	case TM_TEST_POINT_REQ:
		break;
	default:
		g_pdlg->m_strErr = "The point id error... ...";
		//AfxMessageBox("The point id error... ...\n");
		break;
	}

	return 0;
}

bool AnalyzeCsvFile(TM_INFO* p_TMInfo, char* szTaskList, char* lpFileName)
{
	char szFileName[256] = {0};
	strcpy(szFileName, lpFileName);
	unsigned int i = 0, j= 0;
	TM_TASK_NAME *p_TMTaskName = NULL;
	unsigned int uiLastPoint = 0, uiMaxTime = 0, uiMinTime = 0;
	unsigned int uiStartPos = 0, uiEndPos = 0, uiDeltaTime = -1;
	unsigned int uiPrevId = 0, uiStartFlag = 0, uiPrevPos = 0;

	memcpy((unsigned char *)(&g_TMInfo), p_TMInfo, sizeof(g_TMInfo));

	char* token;
	int index = 0;

	token = strtok( szTaskList, "," );
	while( token != NULL )
	{
		memcpy( g_TMTaskName[index].taskname, token, strlen(token) );
		g_TMTaskName[index].taskid = index;
		token = strtok( NULL, "," );
		++index;
	}
	g_TaskNameCount = index;

	if(!g_bBigEndian)
	{
		for (i = 0; i < g_TaskNameCount; i++)
		{
			g_TMTaskName[i].taskid = SwapDword(g_TMTaskName[i].taskid);
		}
	}

	for (i = 0; i < 3000; i++)
	{
		if (p_TMInfo->point_tab[i].time >= uiMaxTime)
		{
			uiMaxTime = p_TMInfo->point_tab[i].time;
			uiLastPoint = i;
		}
	}

	if (uiLastPoint == 3000 - 1)
	{
		uiStartPos = 0;
	}
	else
	{
		uiStartPos = uiLastPoint + 1;
	}
	uiMinTime = p_TMInfo->point_tab[uiStartPos].time;
	uiEndPos = 3000;

	for (i = uiStartPos, j = 0; i < uiEndPos && j < 3000; i++, j++)
	{
		if (uiStartFlag != 0)
		{
			if (i == 0)
			{
				if (p_TMInfo->point_tab[i].id == TM_TEST_POINT_REQ)
				{
					g_TestTime += GetDeltaTime(i, 3000 - 1);
				}
				else
				{
					g_TestTime += GetDeltaTime(i, 3000 - 1);
					AssignDeltaTime(uiPrevId, p_TMInfo->point_tab[i].id, g_TestTime, uiPrevPos);
					g_TestTime = 0;
				}
			}
			else
			{
				if (p_TMInfo->point_tab[i].id == TM_TEST_POINT_REQ)
				{
					g_TestTime += GetDeltaTime(i, i - 1);
				}
				else
				{
					if (i == 0x40d)
					{
						g_TestTime += 0;
					}
					g_TestTime += GetDeltaTime(i, i - 1);
					AssignDeltaTime(uiPrevId, p_TMInfo->point_tab[i].id, g_TestTime, uiPrevPos);
					g_TestTime = 0;
				}
			}
		}


		if (p_TMInfo->point_tab[i].id != TM_TEST_POINT_REQ)
		{
			uiPrevId = p_TMInfo->point_tab[i].id;
			uiStartFlag = 1;
			uiPrevPos = i;
		}

		if (i == 3000 - 1)
		{
			uiEndPos = uiLastPoint;
			i = 0;
		}
	}

	memcpy(szFileName + strlen(szFileName) - 3, "csv", 3);
	FILE *fp_csv = fopen(szFileName, "wt");
	if (fp_csv == NULL )
	{
		g_pdlg->m_strErr = "Create the csv file failed... ...";
		//AfxMessageBox("Create the csv file failed... ...\n");
		return false;
	}
	
	fprintf(fp_csv, "Name, Priority, Time(ms), Time(us), Percent\n");
	unsigned int uiTotalTime;
	uiTotalTime = g_TMIdleInfo.time + g_TMSleepInfo.time + g_TMTaskInfo.time + g_TMIrqInfo.time;
//	fprintf(fp_csv, "All time, ,%d, %d\n", uiMaxTime - uiMinTime, 0);
	fprintf(fp_csv, "Total time, ,%d, %d\n", uiTotalTime * 1000 / 32768, ((uiTotalTime * 1000) % 32768) * 1000 / 32768);
	fprintf(fp_csv, "Idle time, ,%d, %d, %3f%%\n", g_TMIdleInfo.time * 1000/32768, 
		((g_TMIdleInfo.time * 1000) % 32768) * 1000 / 32768, (double)g_TMIdleInfo.time*100/uiTotalTime);
	fprintf(fp_csv, "Sleep time, ,%d, %d, %3f%%\n", g_TMSleepInfo.time*1000/32768,
		((g_TMSleepInfo.time * 1000) % 32768) * 1000 / 32768, (double)g_TMSleepInfo.time * 100/uiTotalTime);
	fprintf(fp_csv, "========================Task time======================\n");
	fprintf(fp_csv, "Task time, ,%d, %d\n", g_TMTaskInfo.time*1000/32768,
		((g_TMTaskInfo.time * 1000) % 32768) * 1000 / 32768);
	for (j = 0; j < 1024; j++)
	{
		if (g_TMTaskInfoX[j].time != 0)
		{
			fprintf(fp_csv, "%s, ,%d, %d, %3f%%\n", g_TMTaskName[j].taskname, g_TMTaskInfoX[j].time*1000/32768, 
				((g_TMTaskInfoX[j].time * 1000) % 32768) * 1000 / 32768, (double)g_TMTaskInfoX[j].time * 100/uiTotalTime);
		}
	}
	fprintf(fp_csv, "========================IRQ time======================\n");
	fprintf(fp_csv, "IRQ time, ,%d, %d\n", g_TMIrqInfo.time*1000/32768,
		((g_TMIrqInfo.time * 1000) % 32768) * 1000 / 32768);
	for (j = 0; j < 32; j++)
	{
		if (g_TMIrqInfoX[j].time != 0)
		{
			fprintf(fp_csv, "IRQ [%s], ,%d, %d, %3f%%\n", g_szIrqName[j], g_TMIrqInfoX[j].time*1000/32768, 
				((g_TMIrqInfoX[j].time * 1000) % 32768) * 1000 / 32768, (double)g_TMIrqInfoX[j].time * 100/uiTotalTime);
		}
	}
	fprintf(fp_csv, "========================FIQ time======================\n");
	fprintf(fp_csv, "FIQ time, ,%d, %d, %3f%%\n", g_TMFiqInfo.time*1000/32768, 
		((g_TMFiqInfo.time * 1000) % 32768) * 1000 / 32768, (double)g_TMFiqInfo.time * 100/uiTotalTime);

	fclose(fp_csv);

	return true;
}












