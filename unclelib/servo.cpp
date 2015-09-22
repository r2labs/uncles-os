/* -*- mode: c++; c-basic-offset: 4; -*- */
#include <stdint.h>

#include "ctlsysctl.hpp"
#include "math.hpp"
#include "servo.hpp"

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

#include "inc/hw_memmap.h"

servo::servo() {}

servo::servo(memory_address_t pwm_base, memory_address_t pwm_gen,
             memory_address_t pwm_out) {
    this->pwm_base = pwm_base;
    this->pwm_gen = pwm_gen;
    this->pwm_out = pwm_out;

    ctlsys::enable_periph(pwm_base);

    pwm_init();
}

uint32_t servo::set(uint32_t pwm_clocks) {
    pwm_clocks = clamp(pwm_clocks, 0, pwm_max_period);
    PWMPulseWidthSet(pwm_base, pwm_out, pwm_clocks);
    return pwm_clocks;
}

void servo::start() {
    PWMGenEnable(pwm_base, pwm_gen);
}

void servo::stop() {
    PWMGenDisable(pwm_base, pwm_gen);
}

void servo::pwm_init() {
    uint32_t status = StartCritical();

    SysCtlPWMClockSet(SYSCTL_PWMDIV_8);
    PWMGenConfigure(pwm_base, pwm_gen, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(pwm_base, pwm_gen, pwm_max_period);
    PWMGenEnable(pwm_base, pwm_gen);

    uint32_t out_bit;
    uint32_t gpio_base;
    uint32_t gpio_pin;
    uint32_t gpio_configuration;

    switch(pwm_out) {
    case PWM_OUT_0:
        gpio_base = GPIO_PORTB_BASE;
        gpio_configuration = GPIO_PB6_M0PWM0;
        gpio_pin = GPIO_PIN_6;
        out_bit = PWM_OUT_0_BIT;
        break;
    case PWM_OUT_1:
        gpio_base = GPIO_PORTB_BASE;
        gpio_configuration = GPIO_PB7_M0PWM1;
        gpio_pin = GPIO_PIN_7;
        out_bit = PWM_OUT_1_BIT;
        break;
    case PWM_OUT_2:
        gpio_base = GPIO_PORTB_BASE;
        gpio_configuration = GPIO_PB4_M0PWM2;
        gpio_pin = GPIO_PIN_4;
        out_bit = PWM_OUT_2_BIT;
        break;
    case PWM_OUT_3:
        gpio_base = GPIO_PORTB_BASE;
        gpio_configuration = GPIO_PB5_M0PWM3;
        gpio_pin = GPIO_PIN_5;
        out_bit = PWM_OUT_3_BIT;
        break;
    case PWM_OUT_4:
        gpio_base = GPIO_PORTE_BASE;
        gpio_configuration = GPIO_PE4_M0PWM4;
        gpio_pin = GPIO_PIN_4;
        out_bit = PWM_OUT_4_BIT;
        break;
    default:
        while(1) {}
    }

    /* Enable the outputs. */
    ctlsys::enable_periph(gpio_base);
    GPIOPinConfigure(gpio_configuration);
    GPIOPinTypePWM(gpio_base, gpio_pin);
    PWMOutputState(pwm_base, out_bit, true);

    EndCritical(status);
}

uint32_t servo::get() {
    return duty_period;
}