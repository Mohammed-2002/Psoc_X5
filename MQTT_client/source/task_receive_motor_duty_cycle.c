#include "task_receive_motor_duty_cycle.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

/* Queue handle for motor duty cycle values */
QueueHandle_t motor_duty_cycle_queue;

/* PWM object */
static cyhal_pwm_t motor_pwm;

void task_receive_motor_duty_cycle(void *arg)
{
    uint8_t receivedDutyCycle;

    /* Initialize the PWM output pin */
    cy_rslt_t result = cyhal_pwm_init(&motor_pwm, P0_5, NULL);  // PWM on GPIO P0_5
    if (result != CY_RSLT_SUCCESS)
    {
        printf("PWM initialization failed!\n");
        CY_ASSERT(0);
    }

    /* Set PWM frequency */
    //cyhal_pwm_set_frequency(&motor_pwm, 1000);  // Set frequency to 1 kHz

    /* Start the PWM output */
    cyhal_pwm_set_duty_cycle(&motor_pwm, 0, 1000);  // 1000 Hz frequency

    cyhal_pwm_start(&motor_pwm);

    for (;;)
    {
        /* Receive the duty cycle value from the queue (0-100) */
        if (xQueueReceive(motor_duty_cycle_queue, &receivedDutyCycle, portMAX_DELAY) == pdTRUE)
        {
            /* Set the duty cycle (0-100%) */
            cyhal_pwm_set_duty_cycle(&motor_pwm, receivedDutyCycle, 1000);  // 1000 Hz frequency
        }

        /* Optional delay to reduce CPU load */
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}





