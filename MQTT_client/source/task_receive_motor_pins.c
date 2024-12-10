/* Header file includes. */
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include <inttypes.h>

/* FreeRTOS header file. */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

/* Declare the queue handle */
extern QueueHandle_t motor_pins_queue;

void task_receive_motor_pins(void *arg)
{
    uint8_t receivedValue;  // Variabele om de ontvangen waarde op te slaan

    /* Initialize the GPIO pins as outputs */
    cyhal_gpio_init(P0_5, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
    cyhal_gpio_init(P12_3, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
    cyhal_gpio_init(P11_4, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
    cyhal_gpio_init(P11_3, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
    cyhal_gpio_init(P11_2, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
    cyhal_gpio_init(P11_7, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
    cyhal_gpio_init(P11_5, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
    cyhal_gpio_init(P11_6, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);

    for (;;)
    {
        /* Ontvang de waarde van de queue */
        //if (xQueueReceive(motor_pins_queue, &receivedValue, portMAX_DELAY) == pdTRUE)
		if (xQueueReceive(motor_pins_queue, &receivedValue, pdMS_TO_TICKS(100)) == pdTRUE)
        {
            /* Zet de ontvangen 8 bits op de respectievelijke GPIO-pinnen */
            cyhal_gpio_write(P0_5,   (receivedValue >> 0) & 0x01);  // Bit 0
            cyhal_gpio_write(P12_3,  (receivedValue >> 1) & 0x01);  // Bit 1
            cyhal_gpio_write(P11_4,  (receivedValue >> 2) & 0x01);  // Bit 2
            cyhal_gpio_write(P11_3,  (receivedValue >> 3) & 0x01);  // Bit 3
            cyhal_gpio_write(P11_2,  (receivedValue >> 4) & 0x01);  // Bit 4
            cyhal_gpio_write(P11_7,  (receivedValue >> 5) & 0x01);  // Bit 5
            cyhal_gpio_write(P11_5,  (receivedValue >> 6) & 0x01);  // Bit 6
            cyhal_gpio_write(P11_6,  (receivedValue >> 7) & 0x01);  // Bit 7
        }

        /* Voeg eventueel een delay toe om CPU-belasting te verminderen */
        vTaskDelay(pdMS_TO_TICKS(10));  // Kleine delay
    }
}
