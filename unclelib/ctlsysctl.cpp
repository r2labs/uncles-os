/* -*- mode: c++; c-basic-offset: 4; */
/* Created by Hershal Bhave and Eric Crosson on <2015-03-15 Sun> */
/* Revision History: Look in Git FGT */

/*! \addtogroup CppTest Test of cpp functionality
 * @{
 */

#include "ctlsysctl.hpp"

#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include "driverlib/timer.h"

/*! Enable peripherals based on a GPIO_PORTx_BASE. */
void ctlsys::enable_periph(uint32_t sys_periph) {
    uint32_t periph_base = 0xDEADBEEF;
    switch(sys_periph) {
    case GPIO_PORTA_BASE: periph_base = SYSCTL_PERIPH_GPIOA; break;
    case GPIO_PORTB_BASE: periph_base = SYSCTL_PERIPH_GPIOB; break;
    case GPIO_PORTC_BASE: periph_base = SYSCTL_PERIPH_GPIOC; break;
    case GPIO_PORTD_BASE: periph_base = SYSCTL_PERIPH_GPIOD; break;
    case GPIO_PORTE_BASE: periph_base = SYSCTL_PERIPH_GPIOE; break;
    case GPIO_PORTF_BASE: periph_base = SYSCTL_PERIPH_GPIOF; break;
    case GPIO_PORTG_BASE: periph_base = SYSCTL_PERIPH_GPIOG; break;
    case GPIO_PORTH_BASE: periph_base = SYSCTL_PERIPH_GPIOH; break;
    case GPIO_PORTJ_BASE: periph_base = SYSCTL_PERIPH_GPIOJ; break;
    case PWM0_BASE:       periph_base = SYSCTL_PERIPH_PWM0; break;
    case PWM1_BASE:       periph_base = SYSCTL_PERIPH_PWM1; break;
    case CAN0_BASE:       periph_base = SYSCTL_PERIPH_CAN0; break;
    case CAN1_BASE:       periph_base = SYSCTL_PERIPH_CAN1; break;
    default: while(1) {}
    }
    uint32_t ui32Status = StartCritical();
    SysCtlPeripheralEnable(periph_base);
    EndCritical(ui32Status);
}

uint32_t ctlsys::timer_timeout_from_subtimer(uint32_t subtimer) {

    uint32_t timeout =  0x0;
    switch(subtimer) {
    case TIMER_BOTH:
    case TIMER_A: timeout = TIMER_TIMA_TIMEOUT; break;
    case TIMER_B: timeout = TIMER_TIMB_TIMEOUT; break;
    default: while(1) {}
    }
    return timeout;
}

uint32_t ctlsys::gpio_pin_to_int(uint32_t pin) {
    switch(pin) {
    case GPIO_PIN_0: return GPIO_INT_PIN_0;
    case GPIO_PIN_1: return GPIO_INT_PIN_1;
    case GPIO_PIN_2: return GPIO_INT_PIN_2;
    case GPIO_PIN_3: return GPIO_INT_PIN_3;
    case GPIO_PIN_4: return GPIO_INT_PIN_4;
    case GPIO_PIN_5: return GPIO_INT_PIN_5;
    case GPIO_PIN_6: return GPIO_INT_PIN_6;
    case GPIO_PIN_7: return GPIO_INT_PIN_7;
    default: while(1) {}
    }
}

uint32_t ctlsys::periph_to_int(uint32_t periph) {
    switch(periph) {
    case GPIO_PORTA_BASE: return INT_GPIOA_TM4C123;
    case GPIO_PORTB_BASE: return INT_GPIOB_TM4C123;
    case GPIO_PORTC_BASE: return INT_GPIOC_TM4C123;
    case GPIO_PORTD_BASE: return INT_GPIOD_TM4C123;
    case GPIO_PORTE_BASE: return INT_GPIOE_TM4C123;
    case GPIO_PORTF_BASE: return INT_GPIOF_TM4C123;
    case GPIO_PORTG_BASE: return INT_GPIOG_TM4C123;
    case GPIO_PORTH_BASE: return INT_GPIOH_TM4C123;
    case CAN0_BASE:       return INT_CAN0_TM4C123;
    default: while(1) {}
    }
}

void ctlsys::gpio_int_clear(uint32_t base, uint32_t pin) {
    uint32_t interrupt = gpio_pin_to_int(pin);
    GPIOIntClear(base, interrupt);
}

void ctlsys::gpio_int_disable(uint32_t base, uint32_t pin) {

    uint32_t interrupt = gpio_pin_to_int(pin);
    GPIOIntDisable(base, interrupt);
}

void ctlsys::gpio_int_enable(uint32_t base, uint32_t pin, bool clear_int) {

    uint32_t interrupt = gpio_pin_to_int(pin);
    if (clear_int) {
        gpio_int_clear(base, interrupt);
    }
    GPIOIntEnable(base, interrupt);
}

/*! Set the clock as used in our labs. */
void ctlsys::set_clock(void) {
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
}

/* End Doxygen group
 * @}
 */
