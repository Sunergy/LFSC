/*
 * Potenciometro.c
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
#include "Potenciometro.h"


void PotenciometroInit(){
	//PortFunctionInit();

	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH13);// PD2
	ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH15);// PD0
	ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH14);// PD1
	ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH12);// PD3
	ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH5 | ADC_CTL_IE | ADC_CTL_END);// PD6
	ADCHardwareOversampleConfigure(ADC0_BASE, 64);
	ADCSequenceEnable(ADC0_BASE, 0);
	ADCIntClear(ADC0_BASE, 0);
}

uint32_t sample(int pot){

	uint32_t pui32ADC0Value[5];

	ADCIntClear(ADC0_BASE, 0);
	ADCProcessorTrigger(ADC0_BASE, 0);

	while(!ADCIntStatus(ADC0_BASE, 0, false))
	{
	}

	ADCSequenceDataGet(ADC0_BASE, 0, pui32ADC0Value);
	return pui32ADC0Value[pot];

}


