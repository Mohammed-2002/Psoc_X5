#ifndef TASK_SEND_MOTOR_PINS_H
#define TASK_SEND_MOTOR_PINS_H

#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

/* Externe declaratie van de queue (wordt elders geïnitialiseerd) */
extern QueueHandle_t motor_pins_queue;

/* Functie prototype voor de send task */
void task_send_motor_pins(void *arg);

#endif /* TASK_SEND_MOTOR_PINS_H */
