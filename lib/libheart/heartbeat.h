/* -*- mode: c; c-basic-offset: 4; -*- */
#ifndef __HEARTBEAT__
#define __HEARTBEAT__

/*! A pointer to a memory location on the ARM Cortex M4. */
typedef int32_t memory_address_t;

/*! Contains information to identify a GPIO pin. This includes the
 *  port base and the specific pin. */
typedef struct muscle_t {
    memory_address_t base;
    memory_address_t pin;
} muscle_t;

/*! The on-board LED colloquially referred to as the 'heart.' Does
 *  every computer have one? */
#define HEART_MUSCLE GPIO_PIN_2

/*! The GPIO base where the \HEART_MUSCLE may be found. Otherwise
 *  knows as a subdivision of the ventral body cavity which houses the
 *  heart. */
#define THORACIC_CAVITY GPIO_PORTF_BASE

/****************************************************************
 * Begin non-modal API -- implicit subject is \HEART_MUSCLE.
 ****************************************************************/

/*!
 *  \brief Initialize \HEART_MUSCLE for visible transformation.
 *  \returns void
 *  \ingroup Heart
 */
inline
void heart_init() {

    /* Enable the GPIO port that is used for the on-board LED. */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    /* Enable the GPIO pins for the LED (PF2). */
    GPIOPinTypeGPIOOutput(THORACIC_CAVITY, HEART_MUSCLE);
}

/*!
 * \brief Return the status of \HEART_MUSCLE.
 * \returns The current status of \HEART_MUSCLE.
 * \ingroup Heart
 */
inline
int32_t heart_status() {GPIOPinRead(THORACIC_CAVITY, HEART_MUSCLE);}

/*!
 *  \brief Turn \HEART_MUSCLE off.
 *  \returns void
 *  \ingroup Heart
 */
inline
void heart_off() {GPIOPinWrite(THORACIC_CAVITY, HEART_MUSCLE, 0);}

/*!
 *  \brief Turn \HEART_MUSCLE on.
 *  \returns void
 *  \ingroup Heart
 */
inline
void heart_on() {GPIOPinWrite(THORACIC_CAVITY, HEART_MUSCLE, 1);}

/*!
 *  \brief Toggle \HEART_MUSCLE once.
 *  \details end_state = ~starting_state
 *  \returns void
 *  \ingroup Heart
 */
inline
void heart_toggle() {

    GPIOPinWrite(THORACIC_CAVITY, HEART_MUSCLE, heart_status() ^ HEART_MUSCLE);
}

/*!
 *  \brief Toggle \HEART_MUSCLE twice.
 *  \details end_state = starting_state
 *  \returns void
 *  \ingroup Heart
 */
inline
void heart_beat() {

    GPIOPinWrite(THORACIC_CAVITY, HEART_MUSCLE, heart_status() ^ HEART_MUSCLE);
    GPIOPinWrite(THORACIC_CAVITY, HEART_MUSCLE, heart_status() ^ HEART_MUSCLE);
}

/****************************************************************
 * Begin GPIO API -- concentrate on a muscle and pump it baby
 *   One day, this section desires to be fully modal
 ****************************************************************/

/*! \brief An alias for \heart_init_. I like the imagery. */
#define heart_pump_(a, b) heart_init_(a, b)

/*!
 *  \brief Initialize \HEART_ANCILLARY_MUSCLE for visible
 *  transformation and set the modal state. That is to say, all modal
 *  function calls will act on \gpio_base and \gpio_pin until another
 *  mode is intialized.
 *  \param The muscle to use as an ancillary heart.
 *  \returns void
 *  \ingroup Heart
 *  \bug Ensure your SYSCTL_PERIPH_GPIOx is enabled, this fn isn't
 *  smart enough to do it yet.
 */
inline
void heart_init_(muscle_t* ancillary_muscle) {

    /* Enable the GPIO port that is used for \HEART_ANCILLARY_MUSCLE. */
    /* TODO: ensure your GPIO peripheral is enabled */
    /* SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); */

    /* Enable the GPIO pins for \HEART_ANCILLARY_MUSCLE. */
    GPIOPinTypeGPIOOutput(ancillary_muscle->base, ancillary_muscle->pin);
}

/*!
 * \brief Return the status of \HEART_ANCILLARY_MUSCLE.
 *  \param The ancillary muscle to act upon.
 * \returns The current status of \HEART_ANCILLARY_MUSCLE.
 * \ingroup Heart
 */
inline
int32_t heart_status_(muscle_t* ancillary_muscle) {

    GPIOPinRead(ancillary_muscle->base, ancillary_muscle->pin);
}

/*!
 *  \brief Turn \ancillary_muscle off.
 *  \param The ancillary muscle to act upon.
 *  \returns void
 *  \ingroup Heart
 */
inline
void heart_off_(muscle_t* ancillary_muscle) {

    GPIOPinWrite(ancillary_muscle->base, ancillary_muscle->pin, 0);
}

/*!
 *  \brief Turn \ancillary_muscle on.
 *  \param The ancillary muscle to act upon.
 *  \returns void
 *  \ingroup Heart
 */
inline
void heart_on_(muscle_t* ancillary_muscle) {

    GPIOPinWrite(ancillary_muscle->base, ancillary_muscle->pin, 1);
}

/*!
 *  \brief Toggle \ancillary_muscle once.
 *  \details end_state = ~starting_state
 *  \param The ancillary muscle to act upon.
 *  \returns void
 *  \ingroup Heart
 */
inline
void heart_toggle_(muscle_t* ancillary_muscle) {

    GPIOPinWrite(ancillary_muscle->base, ancillary_muscle->pin,
		 heart_status_(ancillary_muscle) ^ ancillary_muscle->pin);
}

/*!
 *  \brief Toggle \ancillary_muscle twice.
 *  \details end_state = starting_state
 *  \param The ancillary muscle to act upon.
 *  \returns void
 *  \ingroup Heart
 */
inline
void heart_beat_(muscle_t* ancillary_muscle) {

    GPIOPinWrite(ancillary_muscle->base, ancillary_muscle->pin,
		 heart_status_(ancillary_muscle) ^ ancillary_muscle->pin);
    GPIOPinWrite(ancillary_muscle->base, ancillary_muscle->pin,
		 heart_status_(ancillary_muscle) ^ ancillary_muscle->pin);
}

/****************************************************************
 * End GPIO API
 ****************************************************************/

/*!
 * \brief Allocate and initialize a muscle_t for control via libheart.
 * \details This is a convenience macro for developers. This wraps
 * \hew_heart_muscle and converts what was a three-line process to
 * allocate and initialize a muscle_t into a single call.
 * \param muscle_t to allocate and initialize
 * \param GPIO port base of the pin to target
 * \param GPIO pin on \base to control
 * \returns Initialized muscle_t from \heart_hew_muscle_
 * \note This macro creates a new muscle_t on the current function's
 * stack. Thus, according to C standards of yore this line belongs at
 * the top of your function with the rest of the memory-allocation
 * calls, before the execution code of the function.
 */
#define heart_hew_muscle(a, b, c) \
    muscle_t a;                   \
    heart_hew_muscle_(&a, b, c)

/*!
 * \brief Initialize \muscle according to \base and \pin, and
 * initialize the relevant GPIO peripherals on the Cortex M4.
 * \param Name of the muscle_t structure to allocate and initialize
 * \param GPIO port base of the pin to target
 * \param GPIO pin on \base to control
 * \returns Initialized muscle_t
 */
inline
muscle_t* heart_hew_muscle_(muscle_t*        muscle,
			    memory_address_t base,
			    memory_address_t pin) {

    /* Initialize the muscle_t data structure */
    muscle->base = base;
    muscle->pin  = pin;
    /* Ensure this peripheral is initialized properly */
    heart_init_(muscle);
    /* A convenience for the client developer */
    return muscle;
}

/* FEATURE TODO: use bind (or equivalent function) to wrap a fn pointer with
 * beat(), ptr(), toggle() and return prt()'s ret value */

#endif
