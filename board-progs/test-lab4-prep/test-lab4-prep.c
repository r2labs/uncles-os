/* Created by Hershal Bhave and Eric Crosson on 2015-01-25 */
/* Revision History: Look in Git FGT */

/* Standard Libs */
#include <stdint.h>
#include <stdbool.h>

/* TI Includes */
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"

/* Driverlib Includes */
#include "driverlib/debug.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

#include "libadc/adc.h"
#include "libbutton/button.h"
#include "libtimer/timer.h"
#include "libstd/nexus.h"
#include "libdisplay/ST7735.h"
#include "libshell/shell.h"
#include "libuart/uart.h"

#define HEARTBEAT_MODAL

#include "libheart/heartbeat.h"

extern semaphore_t HW_ADC_SEQ2_SEM;
extern semaphore_t HW_BUTTON_RAW_SEM;
extern uint32_t ADC0_SEQ2_SAMPLES[4];
uint32_t red_work = 0;
uint32_t blue_work = 0;
uint32_t green_work = 0;

volatile uint32_t button_left_pressed;
volatile uint32_t button_right_pressed;

volatile uint32_t button_debounced_mailbox;

volatile semaphore_t button_debounced_new_data;

int8_t plot_en;

void led_blink_red() {
    while (1) {
        ++red_work;
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,
                     GPIO_PIN_1 ^ GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1));
        os_surrender_context();
    }
}

void led_blink_green() {
    while (1) {
        sem_guard(HW_ADC_SEQ2_SEM) {
            sem_take(HW_ADC_SEQ2_SEM);
            ++green_work;
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,
                         GPIO_PIN_3 ^ GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3));
            os_surrender_context();
        }
    }
}

void led_blink_blue() {
    while (1) {
        ++blue_work;
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,
                     GPIO_PIN_2 ^ GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2));
        os_surrender_context();
    }
}

/* This function assumes that there is enough space in the string
   buffer to store the digits of the stringified integer. */
char* fixed_4_digit_i2s(char* string_buf, int32_t data_12bit) {
    uint8_t tmp;
    uint16_t i, j, k, l;
    uint32_t num;

    num = data_12bit & 0x0FFF;
    for (j=0; j<4; ++j) {
        string_buf[3-j] = (num % 10) + 0x30;
        num /= 10;
    }

    string_buf[j] = 0;
    return string_buf;
}

void display_all_adc_data() {

    int8_t i;
    char string_buf[5];
    ST7735_PlotClear(0, 4095);
    plot_en = 1;

    while (1) {
        sem_guard(HW_ADC_SEQ2_SEM && plot_en) {
            sem_take(HW_ADC_SEQ2_SEM);

            fixed_4_digit_i2s(string_buf, ADC0_SEQ2_SAMPLES[0]);
            ST7735_DrawString(1, 1, string_buf, ST7735_YELLOW);

            ST7735_PlotLine(ADC0_SEQ2_SAMPLES[0]);
            if (ST7735_PlotNext()) {
                ST7735_PlotClear(0, 4095);
            }
        }
        os_surrender_context();
    }
}

void display_digital_adc_data() {

    int8_t i;
    uint8_t string_buf[5];

    while (1) {
        sem_guard(HW_ADC_SEQ2_SEM) {
            sem_take(HW_ADC_SEQ2_SEM);

            for (i=0; i<4; ++i) {
                fixed_4_digit_i2s(string_buf, ADC0_SEQ2_SAMPLES[i]);
                ST7735_DrawString(2, 2+i, string_buf, ST7735_YELLOW);
            }
        }
        os_surrender_context();
    }
}

void display_analog_adc_data() {

    int8_t i;
    char string_buf[5];
    ST7735_PlotClear(0, 4095);

    while (1) {
        sem_guard(HW_ADC_SEQ2_SEM) {
            sem_take(HW_ADC_SEQ2_SEM);

            fixed_4_digit_i2s(string_buf, ADC0_SEQ2_SAMPLES[0]);
            ST7735_DrawString(1, 1, string_buf, ST7735_YELLOW);

            ST7735_PlotLine(ADC0_SEQ2_SAMPLES[0]);
            if (ST7735_PlotNext()) {
                ST7735_PlotClear(0, 4095);
            }
        }
        os_surrender_context();
    }
}

/* void button_debounce_end(notification button_notification) { */

/*     button_debounced_mailbox = GPIOPinRead(GPIO_PORTF_BASE, BUTTONS_BOTH); */
/*     sem_post(button_debounced_new_data); */
/* } */

/* /\* what the btn handler calls *\/ */
/* void button_debounce_start(notification button_notification) { */

/*     /\* button_debounced_wtf = GPIOPinRead(GPIO_PORTF_BASE, BUTTONS_BOTH); *\/ */
/*     timer_metadata_init(TIMER0_BASE, 10 Hz, INT_TIMER0A, TIMER_CFG_ONE_SHOT); */
/*     hw_channel_init(HW_TIMER, timer_metadata); */
/*     hw_subscribe_single_shot(HW_TIMER, timer_metadata, */
/*                              button_debounce_end); */
/* } */

void button_debounce_daemon() {

    int32_t button_raw_data = 0xff;

    while (1) {
        sem_guard(HW_BUTTON_RAW_SEM) {
            sem_take(HW_BUTTON_RAW_SEM);
            button_raw_data = GPIOPinRead(GPIO_PORTF_BASE, BUTTONS_BOTH);

            if (~button_raw_data & BUTTON_LEFT) {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,
                             GPIO_PIN_2 ^ GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2));
                /* ++button_left_pressed; */
                ST7735_DrawString(1, 2, "1", ST7735_YELLOW);
            } else {
                ST7735_DrawString(1, 2, "0", ST7735_YELLOW);
            }
            if (~button_raw_data & BUTTON_RIGHT) {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,
                             GPIO_PIN_2 ^ GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2));
                /* ++button_right_pressed; */
                ST7735_DrawString(2, 2, "1", ST7735_YELLOW);
            } else {
                ST7735_DrawString(2, 2, "0", ST7735_YELLOW);
            }
        }
        os_surrender_context();
    }
}

int plot_on() {
    plot_en = 1;
}

int plot_off() {
    plot_en = 0;
}

int main(void) {

    hw_metadata metadata;

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);

    /* Enable processor interrupts */
    IntMasterDisable();

    /* Initialize the output screen */
    Output_On();

    /* initialize the led gpio pins */
    heart_init();
    heart_init_(GPIO_PORTF_BASE, GPIO_PIN_1);
    heart_init_(GPIO_PORTF_BASE, GPIO_PIN_2);
    heart_init_(GPIO_PORTF_BASE, GPIO_PIN_3);

    /* Activate the ADC on PE1, 2, and 3 (AIN0-2). */
    /* start adc init */
    metadata.adc.base = ADC0_BASE;
    metadata.adc.trigger_source = ADC_TRIGGER_TIMER;
    metadata.adc.sample_sequence = 2;
    metadata.adc.channel = 0;
    metadata.adc.channel_configuration =
        ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END;
    metadata.adc.trigger_metadata.timer.base = TIMER1_BASE;
    metadata.adc.trigger_metadata.timer.frequency = 20 Hz;
    metadata.adc.trigger_metadata.timer.interrupt = INT_TIMER1A;
    metadata.adc.trigger_metadata.timer.periodic = TIMER_CFG_PERIODIC;

    adc_init(metadata);
    adc_channel_init(metadata);
    adc_interrupt_init(metadata);
    /* end adc init */

    /* begin timer init for button debouncer */
    timer_metadata_init(TIMER0_BASE, 10 Hz, INT_TIMER0A, TIMER_CFG_ONE_SHOT);
    hw_driver_init(HW_TIMER, timer_metadata);
    /* end timer init for button debouncer */

    /* button init */
    button_left_pressed = 0;
    button_right_pressed = 0;
    button_debounced_mailbox = 0xff;
    sem_init(button_debounced_new_data);

    button_metadata_init(GPIO_PORTF_BASE, BUTTONS_BOTH, GPIO_BOTH_EDGES);

    hw_init(HW_BUTTON, button_metadata);
    /* hw_subscribe(HW_BUTTON, button_metadata, button_debounce_start); */
    /* end button init */

    os_threading_init();
    schedule(led_blink_red, 100 Hz, DL_SOFT);
    schedule(display_all_adc_data, 200 Hz, DL_SOFT);
    schedule(hw_daemon, 100 Hz, DL_SOFT);
    /* schedule(display_adc_graph, 100 Hz, DL_SOFT); */
    /* schedule(led_blink_blue, 100 Hz, DL_SOFT); */
    /* schedule(led_blink_green, 100 Hz, DL_SOFT); */
    schedule(button_debounce_daemon, 100 Hz, DL_SOFT);

    system_init();
    system_register_command((const char*) "plot_on", plot_on);
    system_register_command((const char*) "plot_off", plot_off);

    /* Initialize hardware devices */
    uart_metadata_init(UART_DEFAULT_BAUD_RATE, UART0_BASE, INT_UART0);
    hw_init(HW_UART, uart_metadata);

    /* Initialize the shell and the system it interacts with */
    shell_spawn();

    IntMasterEnable();
    os_launch();

    while (1) {

    }
}
