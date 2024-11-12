#include "sensor_readout.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Distance Measurement Definitions
#define SOUND_SPEED_CM_PER_US 0.0343
#define TIMEOUT_THRESHOLD 50000

#define SENSOR1_TRIGGER_PIN P11_7
#define SENSOR1_ECHO_PIN    P11_2
#define SENSOR2_TRIGGER_PIN P11_3
#define SENSOR2_ECHO_PIN    P11_4
#define SENSOR3_TRIGGER_PIN P12_3
#define SENSOR3_ECHO_PIN    P0_5
#define SENSOR4_TRIGGER_PIN P11_6
#define SENSOR4_ECHO_PIN    P11_5

// RPM Measurement Definitions
/*
#define SENSOR_PIN_1 P10_4
#define SENSOR_PIN_2 P10_0
#define SENSOR_PIN_3 P10_3
#define SENSOR_PIN_4 P10_2
*/
cyhal_timer_t timer_distance, timer_rpm; // Separate timers
QueueHandle_t distance_queue = NULL;

// Function Prototypes
//void init_distance_timer(void);
void init_rpm_timer(void);
void send_trigger_pulse(cyhal_gpio_t trigger_pin);
float measure_distance(cyhal_gpio_t trigger_pin, cyhal_gpio_t echo_pin);
void init_sensor_pin(cyhal_gpio_t pin);
//void update_pulse_count(int *pulse_count, int *prev_sensor_state, cyhal_gpio_t pin);
//void calculate_rpm_speed(int pulse_count, int *rpm, float *speed);

void init_distance_timer(void) {
    cyhal_timer_cfg_t timer_cfg = {
        .compare_value = 0,
        .period = 0xFFFFFFFF,
        .direction = CYHAL_TIMER_DIR_UP,
        .is_compare = false,
        .is_continuous = true,
        .value = 0
    };
    cyhal_timer_init(&timer_distance, NC, NULL);
    cyhal_timer_configure(&timer_distance, &timer_cfg);
    cyhal_timer_start(&timer_distance);
}

void init_rpm_timer(void) {
    cyhal_timer_cfg_t timer_cfg = {
        .compare_value = 0,
        .period = 0xFFFFFFFF,
        .direction = CYHAL_TIMER_DIR_UP,
        .is_compare = false,
        .is_continuous = true,
        .value = 0
    };
    cyhal_timer_init(&timer_rpm, NC, NULL);
    cyhal_timer_configure(&timer_rpm, &timer_cfg);
    cyhal_timer_start(&timer_rpm);
}

void send_trigger_pulse(cyhal_gpio_t trigger_pin) {
    cyhal_gpio_write(trigger_pin, 1);
    cyhal_system_delay_us(10);
    cyhal_gpio_write(trigger_pin, 0);
}

float measure_distance(cyhal_gpio_t trigger_pin, cyhal_gpio_t echo_pin) {
    cyhal_timer_reset(&timer_distance);
    while (cyhal_gpio_read(echo_pin) == 1);
    send_trigger_pulse(trigger_pin);
    while (cyhal_gpio_read(echo_pin) == 0) {
        if (cyhal_timer_read(&timer_distance) > TIMEOUT_THRESHOLD) return -1.0;
    }
    uint32_t start_time = cyhal_timer_read(&timer_distance);
    while (cyhal_gpio_read(echo_pin) == 1) {
        if (cyhal_timer_read(&timer_distance) - start_time > TIMEOUT_THRESHOLD) return -1.0;
    }
    uint32_t pulse_duration = cyhal_timer_read(&timer_distance) - start_time;
    return (pulse_duration / 2.0) * SOUND_SPEED_CM_PER_US;
}

void init_sensor_pin(cyhal_gpio_t pin) {
    cyhal_gpio_init(pin, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);
}
/*
void update_pulse_count(int *pulse_count, int *prev_sensor_state, cyhal_gpio_t pin) {
    int sensor_state = cyhal_gpio_read(pin);
    if (sensor_state == 1 && *prev_sensor_state == 0) {
        (*pulse_count)++;
    }
    *prev_sensor_state = sensor_state;
}

void calculate_rpm_speed(int pulse_count, int *rpm, float *speed) {
    *rpm = pulse_count * 0.05 * (60000 / 500);
    *speed = (0.03 * 2 * 3.14 * (*rpm)) / 60;
}
*/
void sensor_readout_task(void *params) {
	/*
    cybsp_init();
    __enable_irq();
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, 115200);
	*/
    init_distance_timer();
    //init_rpm_timer();

    cyhal_gpio_t trigger_pins[4] = { SENSOR1_TRIGGER_PIN, SENSOR2_TRIGGER_PIN, SENSOR3_TRIGGER_PIN, SENSOR4_TRIGGER_PIN };
    cyhal_gpio_t echo_pins[4] = { SENSOR1_ECHO_PIN, SENSOR2_ECHO_PIN, SENSOR3_ECHO_PIN, SENSOR4_ECHO_PIN };
    //cyhal_gpio_t rpm_pins[4] = { SENSOR_PIN_1, SENSOR_PIN_2, SENSOR_PIN_3, SENSOR_PIN_4 };

    int pulse_counts[4] = {0, 0, 0, 0};
    int prev_states[4] = {0, 0, 0, 0};

    // Initialize GPIOs for distance sensors and RPM sensors
    for (int i = 0; i < 3; i++) {
        cyhal_gpio_init(trigger_pins[i], CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
        cyhal_gpio_init(echo_pins[i], CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);
        //init_sensor_pin(rpm_pins[i]);
    }

    if (distance_queue == NULL) {
		distance_queue = xQueueCreate(10, sizeof(SensorData_t));
	}

    while (true) {
        // Measure and print distances for each sensor
        for (int i = 0; i < 3; i++) {
            float distance = measure_distance(trigger_pins[i], echo_pins[i]);
            //printf("Sensor %d Distance: %f cm\r\n", i + 1, distance);

            if (distance >= 0) {
				SensorData_t data = { .sensor_id = i + 1, .distance = distance };
				if (xQueueSend(distance_queue, &data, pdMS_TO_TICKS(100)) != pdPASS) {
					printf("Failed to send data to queue\n");
				}
				printf("Sensor %d Distance: %.2f cm\n", data.sensor_id, data.distance);
			}
        }
        vTaskDelay(pdMS_TO_TICKS(50));
        /*
        // Update pulse counts for each RPM sensor
        for (int i = 0; i < 4; i++) {
            //update_pulse_count(&pulse_counts[i], &prev_states[i], rpm_pins[i]);
        }

        // Check if 5 seconds have elapsed for RPM calculation

        if (cyhal_timer_read(&timer_rpm) / 1000 >= 5000) {
            for (int i = 0; i < 4; i++) {
                int rpm;
                float speed;
                calculate_rpm_speed(pulse_counts[i], &rpm, &speed);
                printf("Sensor %d RPM: %d, Speed: %f m/s\r\n", i + 1, rpm, speed);
                pulse_counts[i] = 0;
            }
            cyhal_timer_reset(&timer_rpm); // Reset only the RPM timer
        }
        */
    }
}
