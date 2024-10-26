/* Header file includes. */
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include <inttypes.h>

/* FreeRTOS header file. */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

extern QueueHandle_t motor_pins_queue;

void task_send_motor_pins(void *arg)
{
    uint8_t motorPinsValue = 0b01010101;  // Begin met 01010101

    for (;;)
    {
        /* Stuur de waarde naar de motor_pins_queue */
        xQueueSend(motor_pins_queue, &motorPinsValue, portMAX_DELAY);

        /* Wissel de waarde tussen 01010101 en 10101010 */
        if (motorPinsValue == 0b01010101)
        {
            motorPinsValue = 0b10101010;  // Wissel naar 10101010
        }
        else
        {
            motorPinsValue = 0b01010101;  // Wissel terug naar 01010101
        }

        /* Wacht 3 seconden voordat je de waarde opnieuw verstuurt */
        vTaskDelay(pdMS_TO_TICKS(3000));  // 3000 ms (3 seconden) delay
    }
}
