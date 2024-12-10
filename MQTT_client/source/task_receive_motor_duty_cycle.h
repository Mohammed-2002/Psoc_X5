#ifndef TASK_RECEIVE_MOTOR_DUTY_CYCLE_H
#define TASK_RECEIVE_MOTOR_DUTY_CYCLE_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "cyhal.h"

/* Declare the queue handle for motor duty cycle values */
extern QueueHandle_t motor_duty_cycle_queue;

#define PWM_LOW            	(90)
#define PWM_HIGH            (100)

/* Function prototype for the task */
void task_receive_motor_duty_cycle(void *arg);

#endif // TASK_RECEIVE_MOTOR_DUTY_CYCLE_H
