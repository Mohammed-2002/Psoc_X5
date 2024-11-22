#ifndef TEMPERATURE_TASK_H_
#define TEMPERATURE_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Function prototype for the temperature task */
void TemperatureTask(void *pvParameters);

#endif /* TEMPERATURE_TASK_H_ */

