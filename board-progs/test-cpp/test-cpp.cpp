/* -*- mode: c++; c-basic-offset: 4; */
/* Created by Hershal Bhave on <2015-03-15 Sun> */
/* Revision History: Look in Git FGT */

#include "blinker.hpp"
#include "timerpp.hpp"

#include "libos/os.h"
#include "libschedule/schedule.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "inc/hw_memmap.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

blinker blink;
timer timer0a;

void thread_0() {

    while (1) {
        blink.toggle(PIN_BLUE);
        os_surrender_context();
    }
}

void thread_1() {

    while (1) {
        blink.toggle(PIN_GREEN);
        os_surrender_context();
    }
}


int main(void) {

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    blink = blinker(GPIO_PORTF_BASE);
    timer0a = timer(0, TIMER_A, TIMER_CFG_PERIODIC, SysCtlClockGet() / 2, TIMER_TIMA_TIMEOUT);
    timer0a.start();

    /* begin os init */
    os_threading_init();
    schedule(thread_1, 200, DL_SOFT);
    schedule(thread_0, 50, DL_SOFT);
    os_launch();
    /* end os init */

    /* main never terminates */
    while (1);
}

extern "C" void Timer0A_Handler() {
    timer0a.ack();
    blink.toggle(PIN_RED);
}

extern "C" void __cxa_pure_virtual() { while (1); }
