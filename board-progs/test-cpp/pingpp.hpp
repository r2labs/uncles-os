/* -*- mode: c++; c-basic-offset: 4; */
/* Created by Eric Crosson on 2015-04-26 */
#ifndef __pingpp__
#define __pingpp__

#include "circularbuffer.hpp"
#include "interruptable.hpp"
#include "semaphorepp.hpp"
#include "criticalpp.hpp"

#include <stdbool.h>
#include <stdint.h>

#define PING_BUFFER_LENGTH 32
#define PING_BUFFER_TYPE   int16_t

typedef uint32_t memory_address_t;

class ping : public critical, public interruptable {
private:
    semaphore *sem;
public:
    memory_address_t base;
    memory_address_t pin;

    ping();
    ping(memory_address_t port_base, memory_address_t port_pin, semaphore* sem);
    void sample(void);

    virtual void start(void);
    virtual void stop(void);
    virtual uint32_t ack(void);

    circularbuffer<PING_BUFFER_TYPE, PING_BUFFER_LENGTH> buf;
};

#endif  /* __PING__ */

/* Local Variables: */
/* firestarter: (compile "make -k -j32 -C ~/workspace/ee445m-labs/build/") */
/* End: */
