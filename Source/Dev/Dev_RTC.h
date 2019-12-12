#ifndef DEV_RTC_H
#define DEV_RTC_H	    
#include "stm32f10x.h"

//时间结构体
typedef struct 
{
	//公历年月日
	vu16 year;
	vu8  month;
	vu8  date;
	
	vu8 hour;
	vu8 min;
	vu8 sec;			
	
	vu8  week;		 
}stTime;			

typedef enum{
	TimSec = 0,
	TimMin,
	TimHour,
	TimDay,
	TimMon,
	TimYear
	}TimeType;
	
extern u8 const mon_table[12];	//月份日期数据表
void Disp_Time(u8 x,u8 y,u8 size);//在制定位置开始显示时间
void Disp_Week(u8 x,u8 y,u8 size,u8 lang);//在指定位置显示星期
u8 RTC_Init(void);        //初始化RTC,返回0,失败;1,成功;
u8 Is_Leap_Year(u16 year);//平年,闰年判断
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u8 RTC_Get(stTime* calendar);         //更新时间   
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//设置时间	
void TimeUpdate(const stTime * BaseTime, stTime * Time,  TimeType UpdateType, u32 num);


#endif

