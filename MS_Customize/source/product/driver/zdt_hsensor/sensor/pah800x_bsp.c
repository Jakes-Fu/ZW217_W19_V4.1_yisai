// pah
#include "pah800x_bsp.h"


//============================================================================
// Definition
//============================================================================
#define PAH800xx_I2C_SLAVE_ID 0x40
#define I2C_PPG_PORT_ID 1

#if 0
#define PAH_SYSTEM_START_YEAR    (1970)//TENCENTϵͳ����ʼ���

typedef struct 
{
    unsigned short 	wYear;
    unsigned short	nMonth;
    unsigned short	nDay;
    unsigned short	nHour;
    unsigned short	nMin;
    unsigned short	nSec;
}PAHTime;
#endif

//#define USE_FAKE_SYS_TICK		// ONLY enable this when your system tick implementation is not ready


//============================================================================
// Public function
//============================================================================
void pah_delay_ms(uint32_t ms)
{
    /*
		customer need to fill in here with correct delay implementation of platform
		�ͻ���Ҫ������ȷ��ƽ̨��ʱʵ��
	*/	
	SCI_Sleep(ms);
}

#if 0
static uint32 PAHTimeAllDay(PAHTime a)   
{     
    uint32   notrun[12] = {31,29,31,30,31,30,31,31,30,31,30,31};   
    uint32   x = 0, i = 0;   

    x = (a.wYear)*365 + a.wYear/4 + a.wYear/400 - a.wYear/100;   
    
    if ((((a.wYear % 4) == 0) && ((a.wYear % 100) != 0)) ||((a.wYear % 400) == 0))   
    {   
        for (i = 0 ;i < a.nMonth; i++)
        {   
            if (i > 0)  
            {   
                x += notrun[i-1];   
            }   
        }   
    } 
    else 
    {   
        notrun[1] = 28;
        for (i = 0; i < a.nMonth; i++)   
        {   
            if (i > 0)
            {   
                x += notrun[i-1];   
            }   
        }   
    }   
    
    x += a.nDay;
    
    return  x;   
}

static uint32 PAHTimeInterval(PAHTime a, PAHTime b)   
{   
    uint32   x_days = 0, y_days = 0;   
    uint32   sum = 0;   

    //SCI_TRACE_LOW("[QQ]PAHTimeInterval");

    x_days = PAHTimeAllDay(a);   
    y_days = PAHTimeAllDay(b);   
    
    sum = (x_days-y_days)*24 + (a.nHour -b.nHour) - 8;//hours ��   
    return  (sum*3600 + (a.nMin-b.nMin)*60 + (a.nSec - b.nSec));   
}

static void PAHGetDateTime(PAHTime* time_ptr)
{
    SCI_DATE_T date = {0};
    SCI_TIME_T time = {0};
    
    if (PNULL == time_ptr)
    {
        return;
    }

    TM_GetSysDate(&date);
    TM_GetSysTime(&time);

    time_ptr->wYear = date.year;
    time_ptr->nMonth = date.mon;
    time_ptr->nDay = date.mday;
    time_ptr->nHour = time.hour;
    time_ptr->nMin = time.min; 
    time_ptr->nSec = time.sec;
}

extern uint32 PAHGetSystemTime(void)
{
    PAHTime time = {0};
    PAHTime startTime = {0};
    PAHTime time1 = {0};

    PAHGetDateTime(&time1);

    //SCI_TRACE_LOW("[QQ]QGetSystemTime");

    startTime.wYear = PAH_SYSTEM_START_YEAR;
    startTime.nMonth = 1;
    startTime.nDay = 1;
    startTime.nHour = 0;
    startTime.nMin = 0;
    startTime.nSec = 0;

    time.wYear =  time1.wYear;
    time.nMonth = time1.nMonth;
    time.nDay = time1.nDay;
    time.nHour = time1.nHour;
    time.nMin = time1.nMin;
    time.nSec = time1.nSec;

    return PAHTimeInterval(time, startTime);
}

extern uint64_t PAHGetSystemTimeMS(void)
{
    static uint64_t s_last_utc_ms = 0;
    static uint32 s_last_tick_count = 0;
    uint64_t ret = 0;
    uint64_t temp = 0;
    uint32 cur_tick;
    uint32 cur_sec;

    cur_tick = SCI_GetTickCount();
        
    if(s_last_utc_ms > 0 && s_last_tick_count > 0 && cur_tick > s_last_tick_count)
    {
        cur_tick = cur_tick - s_last_tick_count;
        temp = cur_tick;
        ret = s_last_utc_ms + temp;
    }
    else
    {
        s_last_tick_count = cur_tick;
        cur_tick = cur_tick%1000;
        cur_sec = PAHGetSystemTime();
        temp = cur_sec;
        s_last_utc_ms = temp*1000;
        temp = cur_tick;
        s_last_utc_ms += temp;
        ret = s_last_utc_ms;
    }
    return ret;
}
#endif

uint64_t pah_get_tick_count(void)
{
#ifndef USE_FAKE_SYS_TICK
	uint64_t real_tick_count = SCI_GetTickCount(); 
	
	/* 
		custoemr need to use a real system tick(in ms unit) implementation here to return tick count value in milliseconds unit
		ex: if you use STM32, you can call HAL_GetTick() to have tick count in milliseconds

		�ͻ���Ҫ�����ȡ��ʵϵͳ�δ�(���뵥λ)��ʵ�ַ�ʽ���Է���һ���Ժ���Ϊ��λ�ĵδ�ֵ
		ʾ���������ʹ�õ���STM32ƽ̨�����Ե���HAL_GetTick()��ȡ���뵥λ�ĵδ�
	*/
	//DEBUG_PRINT("real_tick_count = %llu \n", real_tick_count);

	return real_tick_count;
#else
	/* this is a simple method to get a pseudo "tick count", it's NOT ACCURATE, just for test, try it with precaution 
	   ����һ���򵥵ķ�ʽ��ȡα�δ�ֵ��׼ȷ�Ƚϲֻ�ǲ���Ŀ�ģ������ѡ��
	*/
	
	static uint64_t fake_tick_count = 0;

	/* everytime call this interface, the fake_tick_count will increase exactly by 40, to emulate "40ms" 
	   polling duration, if you change this value to 30, it will emulate "30ms" polling duration

	   ÿ�ε��ô�pah_get_tick_count()�ӿڣ�fake_tick_count��̶��ۼ�40����ģ��40ms��ѯ���ڣ������дֵΪ30��
	   ����ģ��30ms��ѯ����
	*/
	fake_tick_count += 40;
	
	//DEBUG_PRINT("FAKE_TICK_COUNT = %llu \n", fake_tick_count);
	
	return fake_tick_count;
#endif
}

pah_ret pah_comm_i2c_write(uint8_t addr, const uint8_t *data, size_t write_size)
{
    /*
		this is the interface of PAH8009 I2C communication, please fill in with I2C implementation 
		of your platform, be noted with the data type of two parameters in this interface
		
		1st parameter, "addr" : this is the address of target register
		2nd parameter, "data" : this is the pointer which point to the data you want to write
		3rd parameter, "write_size" : this is the number of data's bytes you want to write(no need to define, since only one-byte write operation needed)


		�˺���ΪPAH8009 I2CͨѶ�Ľӿڣ�����ʵ��ʹ�õ�ƽ̨��I2Cʵ������˽ӿڣ�ע��ӿڵĺ�������������
		��һ��������"addr"����Ϊ��Ҫд�����ݵ�Ŀ��Ĵ����ĵ�ַ
		�ڶ���������"data"����Ϊָ��������Ǵ���ִ��д���������ݱ����ĵ�ַ
		������������"write_size"����Ϊ��Ҫд��������ֽڸ������������ã���Ϊ������ֻ���õ����ֽ�д��
	*/

    //DEBUG_PRINT("[WRITE] A:0x%02x,D:0x%02x\n",addr,*data);
    Pah8009_write_reg(addr,data,write_size);
    return PAH_RET_SUCCESS;
}

pah_ret pah_comm_i2c_read(uint8_t addr, uint8_t *data, size_t read_size)
{
	/*
		this is the interface of PAH8009 I2C communication, please fill in with I2C implementation 
		of your platform, be noted with the data type of three parameters in this interface
        
		1st parameter, "addr" : this is the address of source register
		2nd parameter, "data" : this is the pointer which point to the place you want to store the readed data
		3rd parameter, "read_size" : this is the number of data's bytes you want to read

		�˺���ΪPAH8009 I2CͨѶ�Ľӿڣ�����ʵ��ʹ�õ�ƽ̨��I2Cʵ������˽ӿڣ�ע��ӿڵĺ�������������
		��һ��������"addr"����Ϊ��Ҫ��ȡ���ݵ�Դ�Ĵ����ĵ�ַ
		�ڶ���������"data"����Ϊָ�������ָ��洢�������ݵĵ�ַ
		������������"read_size"����Ϊ��Ҫ��ȡ�������ֽڸ���
	*/
	//DEBUG_PRINT("[READ] A:0x%02x,D:0x%02x\n",addr,*data);
    Pah8009_read_reg(addr,data,read_size);
    return PAH_RET_SUCCESS;
}

/*
void pah_print(
    int32_t                         fileline,
    const char                      *funcname,
    const char                      *filename,
    const char                      *format, ...)
{
    UNUSED_VAR(fileline);
    UNUSED_VAR(funcname);
    UNUSED_VAR(filename);

    char dest[256];
    va_list argptr;
    int char_num = 0;

    if (!format)
        return;

    va_start(argptr, format);
    char_num = vsprintf(dest, format, argptr);
    va_end(argptr);

    //uart_printf("%s", dest);

    //dest[char_num] = '\n';
    pxi_nrf_uart_transmit((uint8_t*)dest, (uint16_t)(char_num + 1));
}
*/

void ReadGSensor(float *x, float *y, float *z)
{
    
}
