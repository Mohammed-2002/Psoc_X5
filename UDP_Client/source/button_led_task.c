// button_led_task.c
#include "cyhal_gpio.h"
#include "cybsp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "button_led_task.h"

extern QueueHandle_t led_state_queue;  // Defined in main.c

void init_button() {
    cyhal_gpio_init(CYBSP_USER_BTN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, CYBSP_BTN_OFF);
}

void init_led() {
    cyhal_gpio_init(CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
}

bool is_button_pressed() {
    return (cyhal_gpio_read(CYBSP_USER_BTN) == 0);
}

void button_led_task(void *arg) {
    uint32_t led_state = LED_OFF_CMD;  // Initial LED state
    TickType_t last_send_time = xTaskGetTickCount();

    init_button();
    init_led();

    while (true) {
        if (is_button_pressed()) {
            led_state = (led_state == LED_OFF_CMD) ? LED_ON_CMD : LED_OFF_CMD;
            cyhal_gpio_write(CYBSP_USER_LED, !led_state);
            printf("Button pressed. LED state toggled to: %d\n", led_state);
            /*
            if (xQueueSend(led_state_queue, &led_state, portMAX_DELAY) != pdPASS) {
            	printf("Failed to send LED state to queue\n");
			}
			*/

            vTaskDelay(pdMS_TO_TICKS(100));  // Debounce delay
        }

        if (xTaskGetTickCount() - last_send_time >= pdMS_TO_TICKS(200)) {
        	if (xQueueSend(led_state_queue, &led_state, portMAX_DELAY) != pdPASS) {
            	printf("Failed to send LED state to queue\n");
			}

			last_send_time = xTaskGetTickCount();
        }

        vTaskDelay(pdMS_TO_TICKS(50));  // Polling delay
    }
}
