/* -*- mode: c++; c-basic-offset: 4; */
/* Created by Hershal Bhave and Eric Crosson on <2015-03-15 Sun> */
/* Revision History: Look in Git FGT */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"

#include "unclelib/ctlsysctl.hpp"
#include "unclelib/adcpp.hpp"
#include "unclelib/blinker.hpp"
#include "unclelib/uartpp.hpp"
#include "unclelib/shellpp.hpp"
#include "unclelib/semaphorepp.hpp"
#include "unclelib/motorpp.hpp"
#include "unclelib/drivepp.hpp"
#include "unclelib/canpp.hpp"
#include "unclelib/switchpp.hpp"

#include "uncleos/os.h"
#include "uncleos/schedule.h"

#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"

timer countdown_timer;
timer blink_timer;
blinker blink;
uart uart0;
uint32_t timer_data;

#define UART0_RX_BUFFER_SIZE 32
static semaphore UART0_RX_SEM;
static buffer<char, UART0_RX_BUFFER_SIZE> UART0_RX_BUFFER;

void timer_updater() {
    uint32_t clockspeed = SysCtlClockGet();

    while(1) {
        /* todo: populate these uint32_t's with sensor data */
        timer_data = TimerValueGet(TIMER2_BASE, TIMER_A)/clockspeed;
        os_surrender_context();
    }
}

void timer_outputter() {

    uint32_t timer_old_data;

    while(1) {
        if (timer_old_data != timer_data) {
            timer_old_data = timer_data;
            uart0.printf("timer: %u\n", timer_data);
        }
        os_surrender_context();
    }
}

int main(void) {

    ctlsys::set_clock();
    IntMasterDisable();

    blink = blinker(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    UART0_RX_BUFFER = buffer<char, UART0_RX_BUFFER_SIZE>(&UART0_RX_SEM);
    uart0 = uart(UART0_BASE, INT_UART0, &UART0_RX_BUFFER);

    countdown_timer = timer(2, TIMER_BOTH, TIMER_CFG_ONE_SHOT,
                            SysCtlClockGet()*180, TIMER_TIMA_TIMEOUT, true);

    blink_timer = timer(1, TIMER_A, TIMER_CFG_PERIODIC,
                        SysCtlClockGet()/2, TIMER_TIMA_TIMEOUT, true);

    os_threading_init();
    schedule(timer_updater, 200);
    schedule(timer_outputter, 200);
    os_launch();
}

extern "C" void Timer2A_Handler() {
    countdown_timer.ack();
    blink.toggle(PIN_RED);
}

extern "C" void Timer1A_Handler() {
    blink_timer.ack();
    blink.toggle(PIN_GREEN);
}

extern "C" void __cxa_pure_virtual() { while (1) {} }
