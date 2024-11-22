#ifndef TASK_RECEIVE_MOTOR_PINS_H
#define TASK_RECEIVE_MOTOR_PINS_H

#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "cyhal.h"

#define MOTOR_FORWARD					   (0b10101010u)//(0xFFu)
#define MOTOR_BACKWARD					   (0b01010101u)
#define MOTOR_LEFT						   (0b01101001u)
#define MOTOR_RIGHT						   (0b10010110u)

#define MOTOR_CLOCKWISE						(0b10011001u)
#define MOTOR_COUNTERCLOCKWISE				(0b01100110u)

#define MOTOR_OFF_STATE					   (0b00000000u)//(0x00u)

/* Externe declaratie van de queue (wordt elders geïnitialiseerd) */
extern QueueHandle_t motor_pins_queue;

/* Functie prototype voor de receive task */
void task_receive_motor_pins(void *arg);

#endif /* TASK_RECEIVE_MOTOR_PINS_H */
