/*
 * RTC.h
 *
 *  Created on: Nov 26, 2014
 *      Author: User
 */

#ifndef RTC_H_
#define RTC_H_

//extern void RTC_init(uint32_t SysClock);
//void SetHib(void);
//extern void HibernateHandler(void);
//void ConfigureHibInt(void);
//void MainHibernateHandler(void);

extern void RTC_init(uint32_t SysClock);
extern void HibernateHandler(void);
extern void ConfigureHibInt(void);
extern void RTC_interupt_reset(int lapse);
extern void RTC_interupt_trigger();
extern int getDay();
extern int getMonth();
extern int getYear();
extern int getHour();
extern int getMinute();
extern int getSec();
extern void setDay(int day);
extern void setMonth(int month);
extern void setYear(int year);
extern void setHour(int hour);
extern void setMinute(int minute);
extern void setSec(int sec);

#endif /* RTC_H_ */
