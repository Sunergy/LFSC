/*
 * Motor.c
 *
 *  Created on: Nov 13, 2014
 *      Author: User
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/adc.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "inc/tm4c1294ncpdt.h"
#include "Interface_Motores.h"
#include "Motor.h"


//uint8_t HS[] = {~(0x05),~(0x01),~(0x09),~(0x08),~(0x0A),~(0x02),~(0x06),~(0x04)};
//uint8_t FS[] = {~(0x08),~(0x02),~(0x04),~(0x01)};
//uint8_t FSA[] = {~(0x0A),~(0x06),~(0x05),~(0x09)};
//uint8_t HS[] = {0x05,0x01,0x09,0x08,0x0A,0x02,0x06,0x04};
//uint8_t FS[] = {0x08,0x02,0x04,0x01};
//uint8_t FSA[] = {0x0A,0x06,0x05,0x09};
uint8_t HS[] = {0x55,0x11,0x99,0x88,0xAA,0x22,0x66,0x44};
uint8_t FS[] = {0x88,0x22,0x44,0x11};
uint8_t FSA[] = {0xAA,0x66,0x55,0x99};
unsigned int  index = 0;
#define DELAY .20//.09

void StepFWHS(motor* last){

	SysCtlDelay((SysCtlClockGet()/3)*DELAY);
	*last->Motor_Port = HS[last->index]&(last->mask);
	last->index = (last->index+1)%8;
	last->err += 0.02109375f;

}
void StepFWFS(motor* last){

	SysCtlDelay((SysCtlClockGet()/3)*DELAY);
	*last->Motor_Port = FS[last->index]&(last->mask);
	last->index = (last->index+1)%4;


}
void StepFWFSA(motor* last){

	SysCtlDelay((SysCtlClockGet()/3)*DELAY);
	*last->Motor_Port = FSA[last->index]&(last->mask);
	last->index = (last->index+1)%4;


}

void StepBWHS(motor* last){

	SysCtlDelay((SysCtlClockGet()/3)*DELAY);
	*last->Motor_Port = HS[last->index]&(last->mask);
	last->index = (last->index-1)%8;
	last->err -= 0.02109375f;
}

void StepBWFS(motor* last){

	SysCtlDelay((SysCtlClockGet()/3)*DELAY);
	*last->Motor_Port = FS[last->index]&(last->mask);
	last->index = (last->index-1)%4;


}
void StepBWFSA(motor* last){

	SysCtlDelay((SysCtlClockGet()/3)*DELAY);
	*last->Motor_Port = FSA[last->index]&(last->mask);
	last->index = (last->index-1)%4;


}

