#ifndef TASK_RECEIVE_MOTOR_PINS_H
#define TASK_RECEIVE_MOTOR_PINS_H

#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "cyhal.h"

											//LV RV LA RA

#define MOTOR_FORWARD					   (0b10101010u)//(0xFFu)
#define MOTOR_BACKWARD					   (0b01010101u)
#define MOTOR_LEFT						   (0b01101001u)
#define MOTOR_RIGHT						   (0b10010110u)

#define MOTOR_FORWARDRIGHT					(0b10000010u)
#define MOTOR_FORWARDLEFT					(0b00101000u)
#define MOTOR_BACKWARDRIGHT					(0b00010100u)
#define MOTOR_BACKWARDLEFT					(0b01000001u)

#define BACK_WHEELS_OFF_LEFT                (0b01100000u)
#define BACK_WHEELS_OFF_RIGHT               (0b10010000u)
#define FRONT_WHEELS_OFF_LEFT               (0b00001001u)
#define FRONT_WHEELS_OFF_RIGHT              (0b00000110u)
#define RIGHT_WHEELS_OFF_FORWARD            (0b10001000u)
#define RIGHT_WHEELS_OFF_BACKWARD           (0b01000100u)
#define LEFT_WHEELS_OFF_FORWARD             (0b00100010u)
#define LEFT_WHEELS_OFF_BACKWARD            (0b00010001u)

#define MOTOR_CLOCKWISE						(0b10011001u)
#define MOTOR_COUNTERCLOCKWISE				(0b01100110u)

#define MOTOR_OFF_STATE					   (0b00000000u)//(0x00u)

/* Externe declaratie van de queue (wordt elders geïnitialiseerd) */
extern QueueHandle_t motor_pins_queue;

/* Functie prototype voor de receive task */
void task_receive_motor_pins(void *arg);

#endif /* TASK_RECEIVE_MOTOR_PINS_H */
