#include "task_receive_motor_duty_cycle.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include <string.h>
#include <stdlib.h>

/* Queue handle for motor duty cycle values */
QueueHandle_t motor_duty_cycle_queue;

/* PWM object */
static cyhal_pwm_t motor_pwm;

void task_receive_motor_duty_cycle(void *arg)
{
	uint8_t received;  // Buffer for receiving strings

    /* Initialize the PWM output pin */
    cy_rslt_t result = cyhal_pwm_init(&motor_pwm, P10_2, NULL);  // PWM on GPIO P0_5
    if (result != CY_RSLT_SUCCESS)
    {
        printf("PWM initialization failed!\n");
        CY_ASSERT(0);
    }

    /* Start the PWM output with an initial duty cycle of 75% */
    cyhal_pwm_set_duty_cycle(&motor_pwm, 100, 1000);  // 1000 Hz frequency
    cyhal_pwm_start(&motor_pwm);

    for (;;)
    {
        /* Receive a string (TRUE or FALSE) from the queue */
        if (xQueueReceive(motor_duty_cycle_queue, &received, portMAX_DELAY) == pdTRUE)
        {
        	printf("receive output: %d \n", received);
            /* Compare the received string and set the duty cycle */
            //if (received == 100)
            //{
            	printf("pwm output: %d \n", received);
            	cyhal_pwm_set_duty_cycle(&motor_pwm, received, 1000);  // Set duty cycle to 100%

            /*
            else if (received == 0)
            {
            	printf("pwm output: %d \n", received);
                cyhal_pwm_set_duty_cycle(&motor_pwm, received, 1000);  // Set duty cycle to 75%

            }

            else
            {
                printf("Invalid input received: %s\n", received);
            }
            */
        }

        /* Optional delay to reduce CPU load */
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
