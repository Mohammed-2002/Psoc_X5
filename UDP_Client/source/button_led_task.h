// button_led_task.h
#ifndef BUTTON_LED_TASK_H
#define BUTTON_LED_TASK_H

#include <stdint.h>
#include <stdbool.h>

#define LED_ON_CMD  1
#define LED_OFF_CMD 0

void init_button();
void init_led();
bool is_button_pressed();
void button_led_task(void *arg);

#endif  // BUTTON_LED_TASK_H
