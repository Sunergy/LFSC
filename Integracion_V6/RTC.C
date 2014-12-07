#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "driverlib/sysctl.h"
#include "driverlib/hibernate.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c1294ncpdt.h"
#include "Interface_Motores.h"
#include "spa.h"
#include "spa_calculation.h"


uint32_t g_ui32SysClock;
struct tm loal_time;
int delay = 30;//240;
float angle = 20.0f;
float motor_pos[] = {18.21f,-67.14f,11.57f};

void RTC_init(uint32_t SysClock){
	g_ui32SysClock =  SysClock;

	// Enable the hibernation peripheral before using it.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
	// Enable clocking to the Hibernation module.
	HibernateEnableExpClk(g_ui32SysClock);
	// Allow crystal to power up and stabilize.
	SysCtlDelay((SysCtlClockGet()/3));

	// Configure the clock source for Hibernation module and enable the RTC
	// feature.
	//
	HibernateClockConfig(HIBERNATE_OSC_LOWDRIVE);
	HibernateRTCEnable();

	// Set the RTC to 0 or an initial value.  The RTC can be set once when the
	// system is initialized after the cold startup and then left to run.  Or
	// it can be initialized before every hibernate.
	HibernateRTCSet(0);
	//
	// Enable the GPIO port that is used for the on-board LED.
	//
	SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R12;

	//
	// Enable the GPIO pin for the LED (PN0).  Set the direction as output, and
	// enable the GPIO pin for digital function.
	//
	GPIO_PORTN_DIR_R = 0x01;
	GPIO_PORTN_DEN_R = 0x01;

}


//void HibernateHandler(void){
//
//}
void HibernateHandler(void)
{
	uint32_t ui32Status;
	//
	// Get the interrupt status and clear any pending interrupts.
	//
	ui32Status = HibernateIntStatus(1);
	HibernateIntDisable(ui32Status);
	HibernateIntClear(ui32Status);
//	HibernateCalendarGet(&loal_time);
//	loal_time.tm_sec += delay;
//	time_t temp1 = mktime(&loal_time);
//	HibernateCalendarMatchSet(0,localtime(&temp1));
//	HibernateRTCMatchSet(0,HibernateRTCGet()+delay);
	//
	// Process the RTC match 0 interrupt.
	//
	if(ui32Status & HIBERNATE_INT_RTC_MATCH_0)
	{
		//
		// RTC match 0 interrupt actions go here.
		//
		spa_data spa_val;

		HibernateCalendarGet(&loal_time);
		loal_time.tm_min = loal_time.tm_min+4;
//		loal_time.tm_sec += delay;
		time_t temp2 = mktime(&loal_time);

		spa_calculate_M(localtime(&temp2),&spa_val,motor_pos);
		float motor_0 = calculate_motor_position(0,&spa_val);
		float motor_1 = calculate_motor_position(1,&spa_val);
		SetMotor(0,motor_0);
		SetMotor(1,motor_1);
//		angle = angle+0.4f;
		GPIO_PORTN_DATA_R ^= 0x01;
	}
	HibernateCalendarGet(&loal_time);
	loal_time.tm_sec += delay;
	time_t temp1 = mktime(&loal_time);
	HibernateCalendarMatchSet(0,localtime(&temp1));
	HibernateIntEnable(ui32Status);

}
//void MainHibernateHandler(void){
//	HibernateHandler(void);
//}
void ConfigureHibInt(void){
	//	uint32_t ui32Status;
	//	uint32_t pui32NVData[64];

	//
	// Set Match 0 for 30 seconds from now.
	//
	//	HibernateRTCMatchSet(0, HibernateRTCGet() + 30);
	//
	// Set up interrupts on the Hibernation module to enable the RTC match
	// 0 interrupt.  Clear all pending interrupts and register the
	// interrupt handler.
	//

//	HibernateRTCMatchSet(0, HibernateRTCGet() + 30);
	HibernateCounterMode(HIBERNATE_COUNTER_24HR);

	//	HibernateCalendarSet(&loal_time); //<-- the struct declared
	loal_time.tm_min = 50;
	loal_time.tm_sec = 0;
	loal_time.tm_mon = 10;
	loal_time.tm_mday = 26;
	loal_time.tm_year = 114;
	loal_time.tm_hour = 10;
	//
	HibernateCalendarSet(&loal_time); //<-- the struct declared
	loal_time.tm_sec += delay;
	time_t temp1 = mktime(&loal_time);
	HibernateCalendarMatchSet(0,localtime(&temp1));
//	HibernateRTCMatchSet(0,HibernateRTCGet()+delay);
	HibernateIntEnable(HIBERNATE_INT_RTC_MATCH_0);
	HibernateIntClear(HIBERNATE_INT_PIN_WAKE | HIBERNATE_INT_LOW_BAT |HIBERNATE_INT_RTC_MATCH_0);
	HibernateIntRegister(HibernateHandler);

}
void RTC_interupt_reset(int lapse){
	delay = lapse;
	HibernateCalendarGet(&loal_time);
	loal_time.tm_sec += delay;
	time_t temp1 = mktime(&loal_time);
	HibernateCalendarMatchSet(0,localtime(&temp1));
}
void RTC_interupt_trigger(){
//	delay = lapse;
	HibernateCalendarGet(&loal_time);
	loal_time.tm_sec += delay;
	time_t temp1 = mktime(&loal_time);
	HibernateCalendarMatchSet(0,localtime(&temp1));
}
int getDay(){
	HibernateCalendarGet(&loal_time);
	return loal_time.tm_mday;
}
int getMonth(){
	HibernateCalendarGet(&loal_time);
	return loal_time.tm_mon;
}
int getYear(){
	HibernateCalendarGet(&loal_time);
	return loal_time.tm_year+1900;
}
int getHour(){
	HibernateCalendarGet(&loal_time);
	return loal_time.tm_hour;
}
int getMinute(){
	HibernateCalendarGet(&loal_time);
	return loal_time.tm_min;
}
int getSec(){
	HibernateCalendarGet(&loal_time);
	return loal_time.tm_sec;
}

void setDay(int day){
	HibernateCalendarGet(&loal_time);
	loal_time.tm_mday = day;
	HibernateCalendarSet(&loal_time);
	RTC_interupt_reset(delay);
}
void setMonth(int month){
	HibernateCalendarGet(&loal_time);
	loal_time.tm_mon = month;
	HibernateCalendarSet(&loal_time);
	RTC_interupt_reset(delay);
}
void setYear(int year){
	HibernateCalendarGet(&loal_time);
	loal_time.tm_year = year - 1900;
	HibernateCalendarSet(&loal_time);
	RTC_interupt_reset(delay);
}
void setHour(int hour){
	HibernateCalendarGet(&loal_time);
	loal_time.tm_hour = hour;
	HibernateCalendarSet(&loal_time);
	RTC_interupt_reset(delay);
}
void setMinute(int minute){
	HibernateCalendarGet(&loal_time);
	loal_time.tm_min = minute;
	HibernateCalendarSet(&loal_time);
	RTC_interupt_reset(delay);
}
void setSec(int sec){
	HibernateCalendarGet(&loal_time);
	loal_time.tm_sec = sec;
	HibernateCalendarSet(&loal_time);
	RTC_interupt_reset(delay);
}

