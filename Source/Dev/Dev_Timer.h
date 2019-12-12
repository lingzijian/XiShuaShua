#ifndef DEV_TIMER_H
#define DEV_TIMER_H
#include "stm32f10x.h"

typedef struct Timer{
		u32 count;
		u8	status;
		struct Timer *Prev;
		struct Timer *next;
	}stTimer;

typedef enum{
	None 	= 0,
	Timing = 1,
	TimeOut = 2
	}TimerStatus;


//void TIM1_PWM_Init(u16 psc, u16 arr);
void TIM8_PWM_Init(u16 psc, u16 arr);
void SysTimerInit(void);
stTimer* CreatTimer(void);
void SetTimer(stTimer* timer, u32 s);
TimerStatus GetTimerStatus(stTimer* timer);
void DeleteTimer(stTimer* timer);
void PauseTimer(stTimer* timer);
void ResumeTimer(stTimer* timer);
void SetTimer_HMS(stTimer* timer, u32 hms);

#endif

