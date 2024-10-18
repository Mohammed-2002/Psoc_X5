/* Header file includes */
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

/* Include de custom headers voor de taken */
#include "task_send_motor_pins.h"
#include "task_receive_motor_pins.h"

/* Queue handle declaratie */
QueueHandle_t motor_pins_queue;

int main(void)
{
    /* Hardware initialisatie */
    cy_rslt_t result;
    result = cybsp_init(); // Initialiseert het board support package

    /* Check of hardware initialisatie succesvol is */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0); // Assert als er iets misgaat bij de hardware init
    }

    /* Initialiseer retarget IO om printf te kunnen gebruiken via UART */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                                 CY_RETARGET_IO_BAUDRATE);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Print debug info */
    printf("PSoC 6 FreeRTOS Motor Pin Control Demo\n");

    /* Create motor_pins_queue voor 8-bit waarden */
    motor_pins_queue = xQueueCreate(10, sizeof(uint8_t));  // Queue voor 10 items van 8-bit data

    /* Check of de queue succesvol is aangemaakt */
    if (motor_pins_queue == NULL)
    {
        printf("Failed to create queue.\n");
        CY_ASSERT(0);
    }

    /* Maak de taak die motor pin waarden verstuurt */
    xTaskCreate(task_send_motor_pins, "SendMotorPinsTask", 1024, NULL, 1, NULL);

    /* Maak de taak die motor pin waarden ontvangt en naar GPIO schrijft */
    xTaskCreate(task_receive_motor_pins, "ReceiveMotorPinsTask", 1024, NULL, 1, NULL);

    /* Start FreeRTOS Scheduler */
    vTaskStartScheduler();

    /* Scheduler zou nooit moeten terugkeren naar dit punt, dit is een safeguard */
    for (;;)
    {
        // In principe zal de code hier nooit komen, tenzij de scheduler faalt
    }
}
