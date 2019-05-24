/*
 * scheduler.h
 *
 *  Created on: 12.12.2017
 *      Author: Sester
 */

#ifndef SRC_SCHEDULER_SCHEDULER_H_
#define SRC_SCHEDULER_SCHEDULER_H_

#include <stdbool.h>
#include <stdint.h>

void schedulerCheck(void);
uint32_t schedulerRegisterInterval(void * callback, uint32_t interval);
uint32_t schedulerRegisterTimeout(void * callback, uint32_t timeout);
_Bool schedulerUnregister(uint32_t uid);
void schedulerUnregisterAll(void);

#endif /* SRC_SCHEDULER_SCHEDULER_H_ */
