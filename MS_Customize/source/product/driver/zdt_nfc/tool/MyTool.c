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
        t->start = TimerTickGet();                            //��ֹhalTimerOflow���,��С����,�������ʱ��ʼʱ��
        return 0;
    }
    if(tickGetDiff(curTick, t->start) >= t->interval) { //��ʱʱ�䵽
        return 1;
    } else { //��ʱδ��
        return 0;
    }
}

void delay_ms(uint32_t nms) {
    TIMER_PIB  delay;
    TimerSet(&delay, nms);//������ʱʱ��
    while(1) {
        if(TimerExpired(&delay) == 1) {
            return;
        }
    }
}

