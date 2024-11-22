#include <stdio.h>
#include <string.h>
#include "temperature_task.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "cy8ckit_028_epd.h"
#include "mtb_thermistor_ntc_gpio.h"
#include "publisher_task.h"
/* Configuration file for MQTT client */
#include "mqtt_client_config.h"


cyhal_adc_t adc;
mtb_thermistor_ntc_gpio_t thermistor;

void TemperatureTask(void *pvParameters)
{
    (void)pvParameters;
    cy_rslt_t result;

    mtb_thermistor_ntc_gpio_cfg_t thermistor_cfg = {
        .r_ref = CY8CKIT_028_EPD_THERM_R_REF,
        .b_const = CY8CKIT_028_EPD_THERM_B_CONST,
        .r_infinity = CY8CKIT_028_EPD_THERM_R_INFINITY,
    };

    /* Initialize the ADC */
    result = cyhal_adc_init(&adc, CY8CKIT_028_EPD_PIN_THERM_OUT1, NULL);
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    /* Initialize the thermistor */
    result = mtb_thermistor_ntc_gpio_init(&thermistor, &adc,
        CY8CKIT_028_EPD_PIN_THERM_GND, CY8CKIT_028_EPD_PIN_THERM_VDD, CY8CKIT_028_EPD_PIN_THERM_OUT1,
        &thermistor_cfg, MTB_THERMISTOR_NTC_WIRING_VIN_NTC_R_GND);
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    for (;;)
    {
        /* Measure the temperature */
        float temperature = mtb_thermistor_ntc_gpio_get_temp(&thermistor);

        publisher_data_t publisher_q_data;

		/* Assign the publish command to be sent to the publisher task. */
		publisher_q_data.cmd = PUBLISH_MQTT_MSG;

		/* Assign the publish message payload so that the device state toggles. */

		char temperature_str[20]; // Genoeg ruimte voor de temperatuur en een terminator

		/* Float omzetten naar string */
		snprintf(temperature_str, sizeof(temperature_str), "%.2f", temperature); // Precisie van 2 decimalen

		/* Toewijzen van de geformatteerde string aan de data */
		publisher_q_data.data = temperature_str;

		publisher_q_data.topic = (char *) MQTT_PUB_TOPIC_THERMISTOR;

		/* Send the command and data to publisher task over the queue */
		xQueueSend(publisher_task_q, &publisher_q_data, pdMS_TO_TICKS(100));


        /* Delay for 1 second */
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}




