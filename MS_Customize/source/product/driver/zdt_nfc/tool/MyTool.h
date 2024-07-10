#ifndef __MYTOOL_H__
#define __MYTOOL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "MyInclude.h"

#pragma pack(push,1) //保存对齐状态  1字节 __packed
#define MYDEBUG  1

#if 1
#define MYLOG(level, fmt, ...)                                                                     \
    do {                                                                                           \
            ZDT_NFCTrace("[ " #level " ][ %05d %s ][ "fmt" ]\r\n", __LINE__,__FUNCTION__,##__VA_ARGS__); \
    } while(0)

#define MYHEX(level, buf, len)                                                                     \
    do {                                                                                           \
            ZDT_NFCTrace("[ " #level " ][ %05d %s ]",__LINE__, __FUNCTION__);                            \
            ZDT_NFCTrace("[ %02d ][ ",len);                                                              \
            ZDT_NFCTraceU8(buf,len);                                                            \
            ZDT_NFCTrace(" ]\r\n");                                                                      \
    } while(0)

#define MYHEXS(level, buf, len,fmt, ...)                                                          \
    do {                                                                                           \
            ZDT_NFCTrace("[ " #level " ][ %05d %s ]",__LINE__, __FUNCTION__);                            \
            ZDT_NFCTrace("[ %02d "fmt" ",len,##__VA_ARGS__);                                             \
            ZDT_NFCTraceU8(buf,len);                                                      \
            ZDT_NFCTrace(" ]\r\n");                                                                      \
    } while(0)

#define MYENTER(level)                                                                             \
    do {                                                                                           \
            ZDT_NFCTrace("%s",__FUNCTION__);                                                                        \
    } while(0)

#else
#define MYLOG(level, fmt, ...)
#define MYLOGHEX(level, buf, len)
//#define MYLOG(format,...) printf("File: "__FILE__", Line: %05d: "format"\r\n", __LINE__, ##__VA_ARGS__)
//unsigned char code DataStr[]=__DATE__;          //??
//unsigned char code TimeStr[]=__TIME__;          //??
//printf("The file is %s.\n", __FILE__);          //????
//printf("The date is %s.\n", __DATE__);          //????
//printf("The time is %s.\n", __TIME__);          //????
//printf("This is line %d.\n", __LINE__);         //??
//printf("This function is %s.\n", __FUNCTION__); //????
//printf("["%s %05d"]",__FUNCTION__,__LINE__);
#endif

#define ValBit(val,offset)    (VAR &   offset)
#define SetBit(val,offset)    (VAR |=  offset)
#define ClrBit(val,offset)    (VAR &=(~offset))


//typedef struct {
//    uint8_t  head;//包头0xaa
//    uint8_t  dev; //上肢0x0a 下肢0x0b
//    uint8_t  len; //长度
//    uint8_t  cmd; //命令
//    union {
//        uint16_t       Data[13];
//        JY_901_Upload  JY_901;  //加速度数据
//        HKJ_15C_Upload HKJ_15C; //肌肉数据
//    } Data;
//    uint8_t  sum;
//    uint8_t  tail;//尾部0x55
//} Upload_DATA;    //9轴 最大32字节

//typedef struct {
//    uint8_t *cmd;       //命令
//    uint8_t rdwr;       //读写标志 0可读 1可写不保存 2可写并保存
//    void *parameter;    //参数
//    uint8_t len;        //长度
//} AT_DEBUG_STR;         //字符串处理

enum loglevels {
    LOG_OFF = 0,    //OFF
    LOG_ERROR,      //ERROR
    LOG_WARNING,    //WARNING
    LOG_INFO,       //INFO
    LOG_DEBUG       //DEBUG
};

typedef struct {
    uint8_t  Debug;                       //RW 等级
    uint8_t  Debug2;                       //RW 等级
} GlobalFlag;

typedef struct {
    uint32_t start;     //start time
    uint32_t interval;  //timeout gap
} TIMER_PIB;


extern GlobalFlag globalflag;


#define tickGetDiff(a, b) ((a < b) ? (0xffffffff - (b - a)) : (a - b))
void TimerTickIncrease(void);
uint32_t TimerTickGet(void);
void TimerSet(TIMER_PIB *t, uint32_t interval);
void Timerstop(TIMER_PIB *t);
uint8_t TimerExpired(TIMER_PIB *t);
void delay_ms(uint32_t nms) ;



#pragma pack(pop) //恢复对齐状态
#ifdef __cplusplus
}
#endif
#endif
