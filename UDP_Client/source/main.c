/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for UDP Client Example in ModusToolbox.
*              The example establishes a connection with a remote UDP server
*              and based on the command received from the UDP server, turns
*              the user LED ON or OFF.
*
* Related Document: See README.md
*
*******************************************************************************
* Copyright 2020-2024, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

/* Header file includes. */
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

/* FreeRTOS header file. */
#include <FreeRTOS.h>
#include <task.h>
#include "queue.h"

/* UDP client task header file. */
#include "udp_client.h"
#include "button_led_task.h"
#include "sensor_readout.h"
/* Include serial flash library and QSPI memory configurations only for the
 * kits that require the Wi-Fi firmware to be loaded in external QSPI NOR flash.
 */
#if defined(CY_DEVICE_PSOC6A512K)
#include "cy_serial_flash_qspi.h"
#include "cycfg_qspi_memslot.h"
#endif

/*******************************************************************************
* Macros
********************************************************************************/
/* RTOS related macros. */
#define UDP_CLIENT_TASK_STACK_SIZE        (5 * 1024)
#define UDP_CLIENT_TASK_PRIORITY          (1)

/*******************************************************************************
* Global Variables
********************************************************************************/

/* UDP server task handle. */
TaskHandle_t client_task_handle;

QueueHandle_t led_state_queue;

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  System entrance point. This function sets up user tasks and then starts
*  the RTOS scheduler.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* Initialize retarget-io to use the debug UART port */
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);

    /* Initialize the User LED */
    //cyhal_gpio_init(CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);

    led_state_queue = xQueueCreate(10, sizeof(uint32_t));
        if (led_state_queue == NULL) {
            printf("Queue creation failed\n");
            CY_ASSERT(0);
        }
    xTaskCreate(button_led_task, "Button_LED_Task", 1024, NULL, 1, NULL);

    xTaskCreate(sensor_readout_task, "Sensor task", 1024, NULL, 1, NULL);

    /* Init QSPI and enable XIP to get the Wi-Fi firmware from the QSPI NOR flash */
    /*
    #if defined(CY_DEVICE_PSOC6A512K)
    const uint32_t bus_frequency = 50000000lu;
    cy_serial_flash_qspi_init(smifMemConfigs[0], CYBSP_QSPI_D0, CYBSP_QSPI_D1,
                                  CYBSP_QSPI_D2, CYBSP_QSPI_D3, NC, NC, NC, NC,
                                  CYBSP_QSPI_SCK, CYBSP_QSPI_SS, bus_frequency);

    cy_serial_flash_qspi_enable_xip(true);
    #endif
	*/
    /* \x1b[2J\x1b[;H - ANSI ESC sequence to clear screen. */
    printf("\x1b[2J\x1b[;H");
    printf("============================================================\n");
    printf("CE230437 - UDP Client\n");
    printf("============================================================\n\n");

    /* Create the tasks. */
    xTaskCreate(udp_client_task, "Network task", UDP_CLIENT_TASK_STACK_SIZE, NULL,
                UDP_CLIENT_TASK_PRIORITY, &client_task_handle);
    /* Start the FreeRTOS scheduler. */
    vTaskStartScheduler();

    /* Should never get here. */
    CY_ASSERT(0);
}

/* [] END OF FILE */
