#include "pingpp.hpp"

#include "delay.hpp"
#include "ctlsysctl.hpp"

#include "driverlib/interrupt.c"

ping::ping() {}

ping::ping(memory_address_t port_base, memory_address_t port_pin, semaphore* sem) {

    base = port_base;
    pin = port_pin;
    this->sem = sem;
    *(this->sem) = semaphore();

    ctlsys::enable_periph(base);
}

/*! \warning this device should not be sampled more than once every
 *  200 micro-seconds */
void ping::sample() {

    uint32_t status = StartCritical();
    Delay d;

    /* Disable interrupts in SIG */
    ctlsys::gpio_int_disable(base, pin);
    IntDisable(INT_GPIOB_TM4C123);
    IntDisable(INT_GPIOB);      /* todo: parametrize */

    /* Set Ping))) SIG to output */
    GPIOPinTypeGPIOOutput(base, pin);
    GPIOPinWrite(base, pin, 1);
    /* Set SIG high for 5usec */
    d = Delay(4);

    GPIOPinWrite(base, pin, 0);

    /* Set Ping))) SIG to input */
    GPIOPinTypeGPIOInput(base, pin);
    GPIOIntTypeSet(base, pin, GPIO_BOTH_EDGES);

    d = Delay(200);

    /* Enable interupts on SIG */
    ctlsys::gpio_int_enable(base, pin, true);
    IntEnable(INT_GPIOB_TM4C123);
    IntEnable(INT_GPIOB);

    EndCritical(status);
}
/* resume: finish hooking this above function up, the receiving ISR,
 * internal data structures, etc */

/* todo: first pass ping() constructor a timer so this can use a timer
 * for distance measurements without colliding with another in-use
 * timer */
/* resume:: implement these virtual functions */
void ping::start() {

    /* TODO: place here the if clause from the GPIOPortB_Handler from
     * test-can.c */
}


void ping::stop() {

    /* TODO: place here the if clause from the GPIOPortB_Handler from
     * test-can.c */
}

uint32_t ping::ack() {


    /* TODO: place here the if clause from the GPIOPortB_Handler from
     * test-can.c */
}

/* Local Variables: */
/* firestarter: (compile "make -k -j32 -C ~/workspace/ee445m-labs/build/") */
/* End: */