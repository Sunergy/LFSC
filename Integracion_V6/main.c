

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/hibernate.h"
#include "inc/tm4c1294ncpdt.h"
#include "Interface_Motores.h"
#include "RTC.h"



//extern uint32_t g_ui32SysClock;

void configureInterupts(){

}
int main(void) {
	uint32_t g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
				SYSCTL_OSC_MAIN |
				SYSCTL_USE_PLL |
				SYSCTL_CFG_VCO_480), 120000000);

	RTC_init(g_ui32SysClock);
	ConfigureMotors();

	ConfigureHibInt();

	while(1){

	}

	return 0;
}
