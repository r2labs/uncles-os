/* -*- mode: c++; c-basic-offset: 4; -*- */
/* Created by Hershal Bhave and Eric Crosson 2015-04-19 */
/* Revision history: Look in Git FGT */
#ifndef __timerpp__
#define __timerpp__

#include <stdint.h>
#include <stdbool.h>

#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"

/*! \addtogroup
   * @{
    */

typedef uint8_t timer_t;
typedef uint32_t subtimer_t;
typedef uint32_t reload_t;

const uint32_t TIMER_DEFAULT_PRIORITY = 0;

class timer {
private:

    /*! Defined between 0 to 4, where 0 indicates TIMER0 */
    timer_t id;

    /*! TIMER_A, TIMER_B, or TIMER_BOTH */
    subtimer_t subtimer;

    /*! The reload value for the timer */
    reload_t reload_value;

    /*! The base address of the specified timer */
    uint32_t base;

    /*! The configuration bitset for the timer */
    /*! \note Reference timer.h for possible options (e.g. TIMER_CFG_PERIODIC) */
    uint32_t configuration;

    /*! The timer interrupt to be acknowdeged, enabled, disabled */
    /*! \note Reference timer.h for possible options (e.g. TIMER_TIMA_TIMEOUT) */
    uint32_t interrupt;

public:
    timer();
    timer(timer_t timer_id, subtimer_t timer_subtimer,
          uint32_t timer_configuration, reload_t timer_load_val,
          uint32_t timer_interrupt);

    /*! Start a timer. */
    void start();

    /*! Stop a timer. */
    void stop();

    /*! Reload the timer with the initial reload value */
    void reload();

    /*! Acknowledge an interrupt. Clears the interrupt bits for this timer. */
    void ack();
};

#endif

/* End Doxygen group
    * @}
     */
