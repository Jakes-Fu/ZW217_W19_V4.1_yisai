#ifndef _TASKSWITCHFUNC_H_
#define _TASKSWITCHFUNC_H_

#define TIMER1_TICK_1MS	812	
#define TIMER1_US_1TICK	1.23
#define SwapWord(x) (WORD)((x << 8) | (x >> 8))
#define SwapDword(x) (DWORD)((x << 24) | ((x & 0x0000FF00) << 8) | ((x & 0x00FF0000) >> 8) | (x >> 24))

bool Analyze( const char* szProduct, const char * path_name, 
			 unsigned long* buf, UINT uiMaxFileSize , UINT uiOffsize , 
			char* szTaskList, char* szIntList, 
			const char* source_bin_file_name = NULL );
bool Analyze( const char * path_name, unsigned long* buf, 
			 UINT uiMaxFileSize , UINT uiOffsize , char* szIntList, char* szTaskList);

void SplitFile(const char * path_name, unsigned long* buf,
			   UINT uiDataAreaSize, UINT uiShareMemorySize,
			   UINT uiIramSize , UINT uiRegSize);			   

bool SplitFile(const char * path_name, unsigned long* buf,
			   UINT uiDspSize, UINT uiShareOffset, 
			   UINT uiIramOffset, UINT uiRegOffset, UINT uiDspOffset, UINT uiShareMemorySize, 
			   UINT uiIramSize , UINT uiRegSize,  UINT uiMaxFileSize);

//add by teana 2011.01.17
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

typedef struct _TM_INFO
{
	struct
	{
		uint8  magic[16];
		uint16 nrof_irq;
		uint16 irq_tab_offset;
		uint16 nrof_mem_region;
		uint16 mem_tab_offset;
		uint16 nrof_point;
		uint16 point_tab_offset;
	}head;
	
	char irq_tab[32][16];

	struct
	{
		uint32 start_addr;
		uint32 size;
		uint32 offset;
	} mem_tab[6];
	
	struct
	{
		uint16  id;
		uint16  acuracy_time;
		uint32	pointer;
		uint32	return_addr;
		uint32  time;
	}point_tab[3000 + 2];
}TM_INFO;

typedef struct _POINT_TAB
{
	uint16  id;
	uint16  acuracy_time;
	uint32	pointer;
	uint32	return_addr;
	uint32  time;
}POINT_TAB;

typedef struct _TM_TASK_NAME
{
	uint32 taskid;
	unsigned char taskname[32];
} TM_TASK_NAME;

bool AnalyzeCsvFile(TM_INFO* p_TMInfo, char* szTaskList, char* lpFileName);


//void SwapChar(char* pStr, int nLen);			   

#endif // _TASKSWITCHFUNC__H_