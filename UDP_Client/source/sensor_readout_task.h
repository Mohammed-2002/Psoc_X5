// sensor_readout.h
#ifndef SENSOR_READOUT_H
#define SENSOR_READOUT_H

#include "cyhal.h"
#include "FreeRTOS.h"
#include "queue.h"

// Struct to hold sensor data
/*
typedef struct {
    int sensor_id;
    float distance;
} SensorData_t;
*/

typedef struct {
    float distance0;
    float distance1;
    float distance2;
    float distance3;
} SensorData_t;

// Function to initialize distance measurement resources
void init_distance_timer(void);

// Task to periodically measure distance from sensors
void sensor_readout_task(void *params);

// Queue handle to access sensor data
extern QueueHandle_t distance_queue;

#endif // SENSOR_READOUT_H
