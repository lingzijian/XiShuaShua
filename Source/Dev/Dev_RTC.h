#ifndef DEV_RTC_H
#define DEV_RTC_H	    
#include "stm32f10x.h"

//ʱ��ṹ��
typedef struct 
{
	//����������
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
	
extern u8 const mon_table[12];	//�·��������ݱ�
void Disp_Time(u8 x,u8 y,u8 size);//���ƶ�λ�ÿ�ʼ��ʾʱ��
void Disp_Week(u8 x,u8 y,u8 size,u8 lang);//��ָ��λ����ʾ����
u8 RTC_Init(void);        //��ʼ��RTC,����0,ʧ��;1,�ɹ�;
u8 Is_Leap_Year(u16 year);//ƽ��,�����ж�
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u8 RTC_Get(stTime* calendar);         //����ʱ��   
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//����ʱ��	
void TimeUpdate(const stTime * BaseTime, stTime * Time,  TimeType UpdateType, u32 num);


#endif

