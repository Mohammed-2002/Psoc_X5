#ifndef TASK_RECEIVE_MOTOR_PINS_H
#define TASK_RECEIVE_MOTOR_PINS_H

#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "cyhal.h"

/* Externe declaratie van de queue (wordt elders geïnitialiseerd) */
extern QueueHandle_t motor_pins_queue;

/* Functie prototype voor de receive task */
void task_receive_motor_pins(void *arg);

#endif /* TASK_RECEIVE_MOTOR_PINS_H */
