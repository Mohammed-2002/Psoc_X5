/*
 * accelerometer.h
 *
 *  Created on: 15 nov. 2024
 *      Author: Johannes
 */

#ifndef SOURCE_ACCELEROMETER_TASK_H_
#define SOURCE_ACCELEROMETER_TASK_H_

#include "cyhal.h"
#include "FreeRTOS.h"
#include "queue.h"

typedef struct {
	float accel_x_g;
	float accel_y_g;
	float accel_z_g;
} AcceleroData_t;

void accelerometer_task(void *params);

extern QueueHandle_t accelero_queue;

#endif /* SOURCE_ACCELEROMETER_TASK_H_ */
