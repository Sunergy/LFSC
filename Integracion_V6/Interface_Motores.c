//*****************************************************************************
// Copyright (c) 2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions
//   are met:
// 
//   Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the  
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// This file was automatically generated by the Tiva C Series PinMux Utility
// Version: 1.0.4
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/eeprom.h"
#include "Interface_Motores.h"
#include "inc/tm4c1294ncpdt.h"
#include "Potenciometro.h"
#include "Motor.h"

//*****************************************************************************
motor config[5];

void SetMotor(int motor,float angle){

	//	getDeltaStepsHS(motor,angle);
	int steps;
	float diff = getAngleDiff(motor,angle);
	//		while((steps < 0) | (steps > 0)){// iff negative
	while((diff< -0.2f)|(diff>0.2f)){
		steps = getDeltaStepsHS(motor,angle);
		SetMotorHS(motor,steps);
		//		steps = getDeltaStepsHS(motor,angle);
		steps = 0;
		if(config[motor].err > 0.9f){
			steps = config[motor].err/0.9f;
			config[motor].err = config[motor].err - (float)steps*0.9f;
		}
		if(config[motor].err < -0.9f){
			steps = config[motor].err/0.9f;
			config[motor].err = config[motor].err - (float)steps*0.9f;
		}
		SetMotorHS(motor,steps);
		*config[motor].Motor_Port = 0x00;
		diff = getAngleDiff(motor,angle);
	}



}
void SetMotorHS(int motor,int steps){

	int i;
	if(steps <= -1){// iff negative
		for(i=0;i>steps;i--){
			StepFWHS(&config[motor]);
		}
	}
	if(steps >= 1) {
		for(i=0;i<steps;i++){


			StepBWHS(&config[motor]);
		}
	}
}
void SetMotorFS(int motor,int steps){

	int i;
	if(steps <= -1){// iff negative
		for(i=0;i>steps;i--){
			StepFWFS(&config[motor]);
		}
	}
	if(steps >= 1) {
		for(i=0;i<steps;i++){


			StepBWFS(&config[motor]);
		}
	}
}
void SetMotorCenter(int motor){
	int i,samples = 0;

	for(i=0;i<5;i++){
		samples += sample(config[motor].Potenciometer);
	}
	config[motor].center =  samples/5;

	EEPROMProgram((uint32_t*)&config[motor].center, (motor)*sizeof(config[motor].center), sizeof(config[motor].center));
}
float getMotorAngle(int motor){

	//		return((float)((int)(getNormalizedQuantizer(motor)))*0.87890625f-(getNormalizedQuantizer(motor)*0.021095))/4.8f;
	return((float)((int)(getNormalizedQuantizer(motor)))*0.18310546875f);
}
int getNormalizedQuantizer(int motor){

	return getQuantizer(motor)-config[motor].center;

}
int getQuantizer(int motor){
	//	SysCtlDelay((SysCtlClockGet()/3)*15);
	return sample(config[motor].Potenciometer);
}
float getVoltage(int motor){
	return (getQuantizer(motor)*(3.3/4095.0));
}
float getAngleDiff(int motor,float angle){
	return (angle - getMotorAngle(motor));
}
int getDeltaStepsHS(int motor,float angle){
	float delta = getAngleDiff(motor,angle);
	float deltaS = (delta*5.4613f);//*5.46133f;// 5.45
	return (int)deltaS;
}
int getDeltaStepsFS(int motor,float angle){

	float deltaS = getAngleDiff(motor,angle)*2.667f;
	return (int)deltaS;
}
void ConfigureMotors(void)
{
	//
	// Enable Peripheral Clocks
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);

	//
	// Enable pin PD0 for ADC AIN15
	//
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0);

	//
	// Enable pin PD7 for ADC AIN4
	// First open the lock and select the bits we want to modify in the GPIO commit register.
	//
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0x80;

	//
	// Now modify the configuration of the pins that we unlocked.
	//
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_7);

	//
	// Enable pin PD1 for ADC AIN14
	//
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);

	//
	// Enable pin PD6 for ADC AIN5
	//
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_6);

	//
	// Enable pin PD3 for ADC AIN12
	//
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3);

	//
	// Enable pin PD2 for ADC AIN13
	//
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_2);

	//
	// Enable pin PK7 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_7);

	//
	// Enable pin PK1 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_1);

	//
	// Enable pin PK4 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_4);

	//
	// Enable pin PK3 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_3);

	//
	// Enable pin PK5 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_5);

	//
	// Enable pin PK2 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_2);

	//
	// Enable pin PK0 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_0);

	//
	// Enable pin PK6 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_6);

	//
	// Enable pin PL0 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_0);

	//
	// Enable pin PL1 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_1);

	//
	// Enable pin PL3 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_3);

	//
	// Enable pin PL2 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_2);

	//
	// Enable pin PM4 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_4);

	//
	// Enable pin PM2 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_2);

	//
	// Enable pin PM3 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_3);

	//
	// Enable pin PM1 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_1);

	//
	// Enable pin PM0 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_0);

	//
	// Enable pin PM7 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_7);

	//
	// Enable pin PM5 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_5);

	//
	// Enable pin PM6 for GPIOOutput
	//
	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_6);

	//
	// Enable pin PP2 for UART0 U0DTR
	//
	GPIOPinConfigure(GPIO_PP2_U0DTR);
	GPIOPinTypeUART(GPIO_PORTP_BASE, GPIO_PIN_2);

	//
	// Enable pin PA1 for UART0 U0TX
	//
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
	PotenciometroInit();
	//	EEPROMInit();
	//	EEPROMRead(pui32Read, 0x0, sizeof(config));
	//	EEPROMProgram(pui32Data, 0x0, sizeof(config));
	//	EEPROMRead(pui32Read, 0x0, sizeof(config));

	int i;

	for(i=0;i<5;i++){
		//		if(i<2){
		//			config[i].Motor_Port = GPIO_PORTL_DATA_R;
		//		}else if(i<4){
		//			config[i].Motor_Port = GPIO_PORTK_DATA_R;
		//		}else if(i==4){
		//			config[i].Motor_Port = GPIO_PORTM_DATA_R;
		//		}

		switch (i) {
		case 0:
			config[i].Motor_Port = &GPIO_PORTK_DATA_R;
			config[i].mask = 0x0F;
			//			config[i].Motor_Port = 0x00&(config[i].mask);
			break;
		case 1:
			config[i].Motor_Port = &GPIO_PORTM_DATA_R;
			config[i].mask = 0xF0;
			//			config[i].Motor_Port = 0x00&(config[i].mask);
			break;
		case 2:
			config[i].Motor_Port = &GPIO_PORTM_DATA_R;
			config[i].mask = 0x0F;
			//			config[i].Motor_Port = 0x00&(config[i].mask);
			break;
		case 3:
			config[i].Motor_Port = &GPIO_PORTL_DATA_R;
			config[i].mask = 0x0F;
			break;
		case 4:
			config[i].Motor_Port = &GPIO_PORTK_DATA_R;
			config[i].mask = 0xF0;
			//			config[i].Motor_Port = 0x00&(config[i].mask);
			break;
		}

		config[i].index = 0;
		config[i].Potenciometer = i;
		config[i].center = 2048;
		uint32_t temp;
		EEPROMRead(&temp, (i)*sizeof(config[i].center), sizeof(config[i].center));
		if(temp == 0xFFFFFFFF){
			//config[i].center = 2048;
		}else if(temp>400){
			config[i].center = temp;
		}
	}


}

//void ConfigureMotors(void)
//{
//	//
//	// Enable Peripheral Clocks
//	//
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
//
//	//
//	// Enable pin PD1 for ADC AIN14
//	//
//	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);
//
//	//
//	// Enable pin PD2 for ADC AIN13
//	//
//	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_2);
//
//	//
//	// Enable pin PD3 for ADC AIN12
//	//
//	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3);
//
//	//
//	// Enable pin PD6 for ADC AIN5
//	//
//	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_6);
//
//	//
//	// Enable pin PD0 for ADC AIN15
//	//
//	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0);
//
//	//
//	// Enable pin PK2 for GPIOOutput
//	//
//	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_2);
//
//	//
//	// Enable pin PK5 for GPIOOutput
//	//
//	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_5);
//
//	//
//	// Enable pin PK0 for GPIOOutput
//	//
//	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_0);
//
//	//
//	// Enable pin PK1 for GPIOOutput
//	//
//	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_1);
//
//	//
//	// Enable pin PK6 for GPIOOutput
//	//
//	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_6);
//
//	//
//	// Enable pin PK4 for GPIOOutput
//	//
//	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_4);
//
//	//
//	// Enable pin PK3 for GPIOOutput
//	//
//	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_3);
//
//	//
//	// Enable pin PK7 for GPIOOutput
//	//
//	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_7);
//
////	//
////	// Enable pin PL6 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_6);
//
//	//
//	// Enable pin PL2 for GPIOOutput
//	//
//	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_2);
////
////	//
////	// Enable pin PL7 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_7);
//
//	//
//	// Enable pin PL0 for GPIOOutput
//	//
//	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_0);
//
////	//
////	// Enable pin PL5 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_5);
//
////	//
////	// Enable pin PL4 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_4);
//
//	//
//	// Enable pin PL3 for GPIOOutput
//	//
//	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_3);
//
//	//
//	// Enable pin PL1 for GPIOOutput
//	//
//	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_1);
//
//	//
//	// Enable pin PM2 for GPIOOutput
//	//
//	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_2);
//
//	//
//	// Enable pin PM0 for GPIOOutput
//	//
//	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_0);
//
//	//
//	// Enable pin PM3 for GPIOOutput
//	//
//	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_3);
//
//	//
//	// Enable pin PM1 for GPIOOutput
//	//
//	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_1);
//
//    //
//    // Enable pin PM6 for GPIOOutput
//    //
//    GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_6);
//
//    //
//    // Enable pin PM5 for GPIOOutput
//    //
//    GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_5);
//
//    //
//    // Enable pin PM7 for GPIOOutput
//    //
//    GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_7);
//
//    //
//    // Enable pin PM4 for GPIOOutput
//    //
//    GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_4);
//	//
//	// Enable pin PB4 for UART0 U0CTS
//	//
//	GPIOPinConfigure(GPIO_PB4_U0CTS);
//	GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_4);
//
//	//
//	// Enable pin PP2 for UART0 U0DTR
//	//
//	GPIOPinConfigure(GPIO_PP2_U0DTR);
//	GPIOPinTypeUART(GPIO_PORTP_BASE, GPIO_PIN_2);
//
//	//
//	// Enable pin PB5 for UART0 U0RTS
//	//
//	GPIOPinConfigure(GPIO_PB5_U0RTS);
//	GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_5);
//
//	//
//	// Enable pin PH2 for UART0 U0DCD
//	//
//	GPIOPinConfigure(GPIO_PH2_U0DCD);
//	GPIOPinTypeUART(GPIO_PORTH_BASE, GPIO_PIN_2);
//
//	//
//	// Enable pin PA1 for UART0 U0TX
//	//
//	GPIOPinConfigure(GPIO_PA1_U0TX);
//	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_1);
//
//	//
//	// Enable pin PM7 for UART0 U0RI
//	//
//	GPIOPinConfigure(GPIO_PM7_U0RI);
//	GPIOPinTypeUART(GPIO_PORTM_BASE, GPIO_PIN_7);
//
//	//
//	// Enable pin PA0 for UART0 U0RX
//	//
//	GPIOPinConfigure(GPIO_PA0_U0RX);
//	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0);
//
//	//
//	// Enable pin PH3 for UART0 U0DSR
//	//
//	GPIOPinConfigure(GPIO_PH3_U0DSR);
//	GPIOPinTypeUART(GPIO_PORTH_BASE, GPIO_PIN_3);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
//	PotenciometroInit();
//	//	EEPROMInit();
//	//	EEPROMRead(pui32Read, 0x0, sizeof(config));
//	//	EEPROMProgram(pui32Data, 0x0, sizeof(config));
//	//	EEPROMRead(pui32Read, 0x0, sizeof(config));
//
//	int i;
//
//	for(i=0;i<5;i++){
////		if(i<2){
////			config[i].Motor_Port = GPIO_PORTL_DATA_R;
////		}else if(i<4){
////			config[i].Motor_Port = GPIO_PORTK_DATA_R;
////		}else if(i==4){
////			config[i].Motor_Port = GPIO_PORTM_DATA_R;
////		}
//
//		switch (i) {
//		case 0:
//			config[i].Motor_Port = &GPIO_PORTL_DATA_R;
//			config[i].mask = 0x0F;
//			break;
//		case 1:
//			config[i].Motor_Port = &GPIO_PORTM_DATA_R;
//			config[i].mask = 0xF0;
//			break;
//		case 2:
//			config[i].Motor_Port = &GPIO_PORTK_DATA_R;
//			config[i].mask = 0x0F;
//			break;
//		case 3:
//			config[i].Motor_Port = &GPIO_PORTK_DATA_R;
//			config[i].mask = 0xF0;
//			break;
//		case 4:
//			config[i].Motor_Port = &GPIO_PORTM_DATA_R;
//			config[i].mask = 0x0F;
//			break;
//		}
//
//		config[i].index = 0;
//		config[i].Potenciometer = i;
//		config[i].center = 2048;
//		EEPROMRead((uint32_t*)&config[i].center, (i)*sizeof(config[i].center), sizeof(config[i].center));
//		if(config[i].center == 0xFFFFFFFF){
//			config[i].center = 2048;
//		}
//	}
//
//
//}
//
////void
////ConfigureMotors(void)
////{
////	//
////	// Enable Peripheral Clocks
////	//
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_EPHY0);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_EMAC0);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
////	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
////
////	//
////	// Enable pin PD7 for ADC AIN4
////	// First open the lock and select the bits we want to modify in the GPIO commit register.
////	//
////	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
////	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0x80;
////
////	//
////	// Now modify the configuration of the pins that we unlocked.
////	//
////	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_7);
////
////	//
////	// Enable pin PD0 for ADC AIN15
////	//
////	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0);
////
////	//
////	// Enable pin PD6 for ADC AIN5
////	//
////	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_6);
////
////	//
////	// Enable pin PD2 for ADC AIN13
////	//
////	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_2);
////
////	//
////	// Enable pin PD1 for ADC AIN14
////	//
////	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);
////
////	//
////	// Enable pin PD3 for ADC AIN12
////	//
////	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3);
////
////	//
////	// Enable pin PG0 for EMAC0 EN0PPS
////	//
////	GPIOPinConfigure(GPIO_PG0_EN0PPS);
////	GPIOPinTypeEthernetMII(GPIO_PORTG_BASE, GPIO_PIN_0);
////
////	//
////	// Enable pin PF1 for EPHY0 EN0LED2
////	//
////	GPIOPinConfigure(GPIO_PF1_EN0LED2);
////	GPIOPinTypeEthernetLED(GPIO_PORTF_BASE, GPIO_PIN_1);
////
////	//
////	// Enable pin PF4 for EPHY0 EN0LED1
////	//
////	GPIOPinConfigure(GPIO_PF4_EN0LED1);
////	GPIOPinTypeEthernetLED(GPIO_PORTF_BASE, GPIO_PIN_4);
////
////	//
////	// Enable pin PF0 for EPHY0 EN0LED0
////	//
////	GPIOPinConfigure(GPIO_PF0_EN0LED0);
////	GPIOPinTypeEthernetLED(GPIO_PORTF_BASE, GPIO_PIN_0);
////
////	//
////	// Enable pin PB3 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3);
////
////	//
////	// Enable pin PB0 for GPIOInput
////	//
////	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0);
////
////	//
////	// Enable pin PB2 for GPIOInput
////	//
////	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_2);
////
////	//
////	// Enable pin PB1 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_1);
////
////	//
////	// Enable pin PE4 for GPIOInput
////	//
////	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4);
////
////	//
////	// Enable pin PE2 for GPIOInput
////	//
////	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2);
////
////	//
////	// Enable pin PE3 for GPIOInput
////	//
////	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_3);
////
////	//
////	// Enable pin PE1 for GPIOInput
////	//
////	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1);
////
////	//
////	// Enable pin PE0 for GPIOInput
////	//
////	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0);
////
////	//
////	// Enable pin PK1 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_1);
////
////	//
////	// Enable pin PK2 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_2);
////
////	//
////	// Enable pin PK5 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_5);
////
////	//
////	// Enable pin PK3 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_3);
////
////	//
////	// Enable pin PK4 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_4);
////
////	//
////	// Enable pin PK7 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_7);
////
////	//
////	// Enable pin PK0 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_0);
////
////	//
////	// Enable pin PK6 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_6);
////
////	//
////	// Enable pin PL6 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_6);
////
////	//
////	// Enable pin PL0 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_0);
////
////	//
////	// Enable pin PL5 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_5);
////
////	//
////	// Enable pin PL1 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_1);
////
////	//
////	// Enable pin PL7 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_7);
////
////	//
////	// Enable pin PL4 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_4);
////
////	//
////	// Enable pin PL3 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_3);
////
////	//
////	// Enable pin PL2 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_2);
////
////	//
////	// Enable pin PM0 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_0);
////
////	//
////	// Enable pin PM3 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_3);
////
////	//
////	// Enable pin PM2 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_2);
////
////	//
////	// Enable pin PM1 for GPIOOutput
////	//
////	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_1);
////
////	//
////	// Enable pin PP0 for SSI3 SSI3XDAT2
////	//
////	GPIOPinConfigure(GPIO_PP0_SSI3XDAT2);
////	GPIOPinTypeSSI(GPIO_PORTP_BASE, GPIO_PIN_0);
////
////	//
////	// Enable pin PQ0 for SSI3 SSI3CLK
////	//
////	GPIOPinConfigure(GPIO_PQ0_SSI3CLK);
////	GPIOPinTypeSSI(GPIO_PORTQ_BASE, GPIO_PIN_0);
////
////	//
////	// Enable pin PP1 for SSI3 SSI3XDAT3
////	//
////	GPIOPinConfigure(GPIO_PP1_SSI3XDAT3);
////	GPIOPinTypeSSI(GPIO_PORTP_BASE, GPIO_PIN_1);
////
////	//
////	// Enable pin PQ1 for SSI3 SSI3FSS
////	//
////	GPIOPinConfigure(GPIO_PQ1_SSI3FSS);
////	GPIOPinTypeSSI(GPIO_PORTQ_BASE, GPIO_PIN_1);
////
////	//
////	// Enable pin PQ3 for SSI3 SSI3XDAT1
////	//
////	GPIOPinConfigure(GPIO_PQ3_SSI3XDAT1);
////	GPIOPinTypeSSI(GPIO_PORTQ_BASE, GPIO_PIN_3);
////
////	//
////	// Enable pin PQ2 for SSI3 SSI3XDAT0
////	//
////	GPIOPinConfigure(GPIO_PQ2_SSI3XDAT0);
////	GPIOPinTypeSSI(GPIO_PORTQ_BASE, GPIO_PIN_2);
////
////	//
////	// Enable pin PP2 for UART0 U0DTR
////	//
////	GPIOPinConfigure(GPIO_PP2_U0DTR);
////	GPIOPinTypeUART(GPIO_PORTP_BASE, GPIO_PIN_2);
////
////	//
////	// Enable pin PA0 for UART0 U0RX
////	//
////	GPIOPinConfigure(GPIO_PA0_U0RX);
////	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0);
////
////	//
////	// Enable pin PB4 for UART0 U0CTS
////	//
////	GPIOPinConfigure(GPIO_PB4_U0CTS);
////	GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_4);
////
////	//
////	// Enable pin PH2 for UART0 U0DCD
////	//
////	GPIOPinConfigure(GPIO_PH2_U0DCD);
////	GPIOPinTypeUART(GPIO_PORTH_BASE, GPIO_PIN_2);
////
////	//
////	// Enable pin PA1 for UART0 U0TX
////	//
////	GPIOPinConfigure(GPIO_PA1_U0TX);
////	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_1);
////
////	//
////	// Enable pin PB5 for UART0 U0RTS
////	//
////	GPIOPinConfigure(GPIO_PB5_U0RTS);
////	GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_5);
////
////	//
////	// Enable pin PH3 for UART0 U0DSR
////	//
////	GPIOPinConfigure(GPIO_PH3_U0DSR);
////	GPIOPinTypeUART(GPIO_PORTH_BASE, GPIO_PIN_3);
////
////	//
////	// Enable pin PM7 for UART0 U0RI
////	//
////	GPIOPinConfigure(GPIO_PM7_U0RI);
////	GPIOPinTypeUART(GPIO_PORTM_BASE, GPIO_PIN_7);
////
////	//
////	// Enable pin PN3 for UART2 U2CTS
////	//
////	GPIOPinConfigure(GPIO_PN3_U2CTS);
////	GPIOPinTypeUART(GPIO_PORTN_BASE, GPIO_PIN_3);
////
////	//
////	// Enable pin PD4 for UART2 U2RX
////	//
////	GPIOPinConfigure(GPIO_PD4_U2RX);
////	GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4);
////
////	//
////	// Enable pin PD5 for UART2 U2TX
////	//
////	GPIOPinConfigure(GPIO_PD5_U2TX);
////	GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_5);
////
////	//
////	// Enable pin PN2 for UART2 U2RTS
////	//
////	GPIOPinConfigure(GPIO_PN2_U2RTS);
////	GPIOPinTypeUART(GPIO_PORTN_BASE, GPIO_PIN_2);
////
////	//
////	// Enable pin PC7 for UART5 U5TX
////	//
////	GPIOPinConfigure(GPIO_PC7_U5TX);
////	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_7);
////
////	//
////	// Enable pin PC6 for UART5 U5RX
////	//
////	GPIOPinConfigure(GPIO_PC6_U5RX);
////	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6);
////
////	//
////	// Enable pin PC5 for UART7 U7TX
////	//
////	GPIOPinConfigure(GPIO_PC5_U7TX);
////	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_5);
////
////	//
////	// Enable pin PC4 for UART7 U7RX
////	//
////	GPIOPinConfigure(GPIO_PC4_U7RX);
////	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4);
////}