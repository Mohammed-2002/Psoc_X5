/*******************************************************************************
* File Name: main.c
*
* Description: Initializing the MPU-6050 accelerometer using I2C
*
* Related Document: See README.md
*
*******************************************************************************/
#include "accelerometer_task.h"

#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Define I2C pins for the MPU-6050
#define I2C_SDA_PIN    P6_1  // SDA pin , gelieve de pin niet te veranderen , anders werkt het niet.
#define I2C_SCL_PIN    P6_0  // SCL pin-, gelieve de pin niet te veranderen , anders werkt het niet.
#define MPU6050_I2C_ADDRESS  0x69

// MPU-6050 register addresses
#define MPU6050_ACCEL_XOUT_H 0x3B // Accelerometer X-axis high byte

// Define frequency
#define I2C_MASTER_FREQUENCY 100000UL  // I2C frequency at 100 kHz

// Sensitivity for ±4g (8192 LSB/g)
#define SENSITIVITY 16384.0f

QueueHandle_t accelero_queue = NULL;

/*
void init_mpu6050(cyhal_i2c_t* i2c_master_obj) {
    uint8_t reset_value = 0x00;  // Waarde om de MPU-6050 uit slaapstand te halen
    uint8_t accel_config = 0x00;  // ±4g

    // Schrijf naar het Power Management-register (0x6B) om de MPU-6050 te activeren
    cy_rslt_t result = cyhal_i2c_master_mem_write(i2c_master_obj,
                                                  MPU6050_I2C_ADDRESS,  // I2C-adres van de MPU-6050 (7-bit)
                                                  0x6B,                 // Registeradres voor Power Management 1
                                                  1,                    // Grootte van het geheugenadres (1 byte, want het registeradres is 1 byte)
                                                  &reset_value,         // Gegevens om te schrijven (0x00 om de slaapmodus uit te schakelen)
                                                  1,                    // Grootte van de data (1 byte)
                                                  1000);                // Timeout van 1000 ms

    // Controleer of het schrijven succesvol was
    if (result == CY_RSLT_SUCCESS) {
        printf("MPU6050 is nu wakker\r\n");
    } else {
        printf("Er ging iets mis bij het wakker maken van de MPU6050, foutcode: %lu\r\n", (unsigned long)result);
    }

    result = cyhal_i2c_master_mem_write(i2c_master_obj,
										  MPU6050_I2C_ADDRESS,  // I2C adres
										  0x1C,                 // Register voor ACCEL_CONFIG
										  1,                    // Geheugenadresgrootte (1 byte)
										  &accel_config,        // Gegevens om te schrijven
										  1,                    // Grootte van de data (1 byte)
										  1000);                // Timeout van 1000 ms

	if (result == CY_RSLT_SUCCESS) {
		printf("Accelerometer range ingesteld op ±4g\r\n");
	} else {
		printf("Fout bij het instellen van de versnellingsmeter, foutcode: %lu\r\n", (unsigned long)result);
	}
}


*/
void init_mpu6050(cyhal_i2c_t* i2c_master_obj) {
    uint8_t reset_value = 0x00;  // Value to wake up MPU-6050
    uint8_t accel_config = 0x00; // ±4g configuration

    cy_rslt_t result;
    bool initialized = false;

    while (!initialized) {
        // Write to Power Management register to wake up MPU-6050
        result = cyhal_i2c_master_mem_write(i2c_master_obj,
                                            MPU6050_I2C_ADDRESS,  // I2C address of MPU-6050
                                            0x6B,                 // Register address for Power Management 1
                                            1,                    // Memory address size (1 byte)
                                            &reset_value,         // Data to write (0x00 to disable sleep mode)
                                            1,                    // Data size (1 byte)
                                            1000);                // Timeout in ms

        if (result == CY_RSLT_SUCCESS) {
            printf("MPU6050 is now awake\r\n");

            // Attempt to set accelerometer configuration
            result = cyhal_i2c_master_mem_write(i2c_master_obj,
                                                MPU6050_I2C_ADDRESS,  // I2C address
                                                0x1C,                 // Register for ACCEL_CONFIG
                                                1,                    // Memory address size (1 byte)
                                                &accel_config,        // Data to write
                                                1,                    // Data size (1 byte)
                                                1000);                // Timeout in ms

            if (result == CY_RSLT_SUCCESS) {
                printf("Accelerometer range set to ±4g\r\n");
                initialized = true; // Exit loop if both writes are successful
            } else {
                printf("Failed to configure accelerometer, error code: %lu\r\n", (unsigned long)result);
            }
        } else {
            printf("Failed to wake up MPU6050, error code: %lu\r\n", (unsigned long)result);
        }

        // Optional: Add a short delay between retries to prevent rapid looping
        if (!initialized) {
            cyhal_system_delay_ms(500); // 500 ms delay
        }
    }
}

void read_accelerometer_data(cyhal_i2c_t* i2c_master_obj, int16_t* accel_x, int16_t* accel_y, int16_t* accel_z) {
    uint8_t buffer[6];  // Buffer to hold the data read from the accelerometer

    // Read 6 bytes of accelerometer data (X, Y, Z)
    cy_rslt_t result = cyhal_i2c_master_mem_read(i2c_master_obj,
                                                 MPU6050_I2C_ADDRESS,  // I2C address of the MPU-6050 (7-bit)
                                                 0x3B,                 // Register address for ACCEL_XOUT[15:8]
                                                 1,                    // Mem address size (1 byte)
                                                 buffer,               // Buffer to store the received data
                                                 6,                    // Size of the data to read (6 bytes)
                                                 1000);                // Timeout of 1000 ms

    if (result == CY_RSLT_SUCCESS) {
        printf("read result success\r\n");
    } else {
        printf("read result failed, error code: %lu\r\n", (unsigned long)result);
    }

    // Convert the raw data into 16-bit values for X, Y, and Z axes
    *accel_x = (int16_t)(buffer[0] << 8 | buffer[1]);  // Combine the high and low byte for X-axis
    *accel_y = (int16_t)(buffer[2] << 8 | buffer[3]);  // Combine the high and low byte for Y-axis
    *accel_z = (int16_t)(buffer[4] << 8 | buffer[5]);  // Combine the high and low byte for Z-axis
}

float convert_to_g(int16_t raw_value) {
    // Bereken de versnelling in g door de ruwe waarde te delen door de sensitiviteit
    return (float)raw_value / SENSITIVITY;
}


void accelerometer_task(void *params) {

    cy_rslt_t result;
    cyhal_i2c_t i2c_master_obj;

    if (accelero_queue == NULL) {
    	accelero_queue = xQueueCreate(10, sizeof(AcceleroData_t));
	}

	// Instellen van I2C-masterconfiguratie
	cyhal_i2c_cfg_t i2c_master_config = {
	    .is_slave = false,                  // Master mode
	    .address = 0,                       // Niet nodig voor master mode
	    .frequencyhal_hz = I2C_MASTER_FREQUENCY
	};

	// Initialiseren van de I2C-master
	result = cyhal_i2c_init(&i2c_master_obj, I2C_SDA_PIN, I2C_SCL_PIN, NULL);
	if (result != CY_RSLT_SUCCESS) {

		printf("de initialisatie van I2C faalt\r\n");

	    CY_ASSERT(0);  // Stopt het programma als de initialisatie faalt
	}

	// Configureren van de I2C-master met de opgegeven instellingen
	result = cyhal_i2c_configure(&i2c_master_obj, &i2c_master_config);
	if (result != CY_RSLT_SUCCESS) {
		printf("de config van I2C faalt\r\n");
	    CY_ASSERT(0);  // Stopt het programma als de configuratie faalt
	}

	printf("de config van I2C succeeded\r\n");

    // Initialize MPU-6050
    init_mpu6050(&i2c_master_obj);

    // Enable global interrupts
    //__enable_irq();

    int16_t accel_x, accel_y, accel_z;

    for (;;) {
        // Read accelerometer data
        read_accelerometer_data(&i2c_master_obj, &accel_x, &accel_y, &accel_z);

        float accel_x_g = convert_to_g(accel_x);
		float accel_y_g = convert_to_g(accel_y);
		float accel_z_g = convert_to_g(accel_z);

		AcceleroData_t data =
			{ .accel_x_g = accel_x_g - 1.1,
			  .accel_y_g = accel_y_g,
			  .accel_z_g = accel_z_g - 0.1,
			};

		if (xQueueSend(accelero_queue, &data, pdMS_TO_TICKS(100)) != pdPASS) {
			printf("Failed to send data to queue\n");
		}
		//printf("Accel X: %.4f g, Accel Y: %.4f g, Accel Z: %.4f g\r\n", accel_x_g, accel_y_g, accel_z_g);

        // Add a delay between reads
		vTaskDelay(pdMS_TO_TICKS(2));
    }
}
