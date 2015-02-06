/* -*- mode: c; c-basic-offset: 4; -*- */
/*!
 * \brief Schedule a task (function) to be invoked periodically.
 * \param task A pointer to the function to execute every \period milliseconds
 * \param period Number of milliseconds to wait before invoking \task repeatedly
 * \param priority The value to be specified in the NVIC for this thread (task)
 * \param timer_peripheral The timer device (peripheral) to use for scheduling
 * \returns TODO
 * \note Note that this function name was chosen by the class
 * administrators. This name has been the source of some confusion;
 * namely the fact that the function name refers to a thread and the
 * function argument refers to a task. This function actually takes a
 * task (a function pointer, not a fully-fledged thread) and invokes
 * said function on the designated schedule. No true threads exist in
 * this context.
 */
int timer_add_periodic_thread(void(*task)(void),
			      unsigned long period,
			      unsigned long priority,
			      unsigned long timer_peripheral);