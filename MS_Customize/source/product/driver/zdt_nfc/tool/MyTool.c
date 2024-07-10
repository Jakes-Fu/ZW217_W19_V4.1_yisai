#include "MyTool.h"

GlobalFlag globalflag = {
    LOG_DEBUG
};

void TimerTickIncrease(void) {
}

uint32_t TimerTickGet(void) {
    return SCI_GetTickCount();
}

void TimerSet(TIMER_PIB *t, uint32_t interval) {
    t->interval = interval;
    t->start = TimerTickGet();
}

void Timerstop(TIMER_PIB *t) {
    t->start = 0;
    t->interval = 0;
}

uint8_t TimerExpired(TIMER_PIB *t) {
    uint32_t curTick = 0;
    curTick = TimerTickGet();
    if(curTick < t->start) {
        t->start = TimerTickGet();                            //防止halTimerOflow溢出,以小减大,重新清计时初始时刻
        return 0;
    }
    if(tickGetDiff(curTick, t->start) >= t->interval) { //定时时间到
        return 1;
    } else { //定时未到
        return 0;
    }
}

void delay_ms(uint32_t nms) {
    TIMER_PIB  delay;
    TimerSet(&delay, nms);//设置延时时间
    while(1) {
        if(TimerExpired(&delay) == 1) {
            return;
        }
    }
}

