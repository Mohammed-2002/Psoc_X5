/******************************************************************************
* File Name:   mqtt_client_config.h
*
* Description: This file contains all the configuration macros used by the
*              MQTT client in this example.
*
* Related Document: See README.md
*
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

#ifndef MQTT_CLIENT_CONFIG_H_
#define MQTT_CLIENT_CONFIG_H_

#include "cy_mqtt_api.h"

/*******************************************************************************
* Macros
********************************************************************************/

/***************** MQTT CLIENT CONNECTION CONFIGURATION MACROS *****************/
// VOOR EEN OF ANDERE REDEN WERKT HIVE NIET. ZELFS NIET DE PUBLIC BROKER.
/* MQTT Broker/Server address and port used for the MQTT connection. */

// MQTTS mosquitto
/*
#define MQTT_BROKER_ADDRESS               "test.mosquitto.org"
#define MQTT_PORT                         1883
#define MQTT_SECURE_CONNECTION            ( 0 )
#define MQTT_USERNAME                     "testtest"
#define MQTT_PASSWORD                     "testtest"
*/


#define MQTT_BROKER_ADDRESS					"192.168.132.204"
#define MQTT_PORT                         1883
#define MQTT_SECURE_CONNECTION            ( 0 )
#define MQTT_USERNAME                     ""
#define MQTT_PASSWORD                     ""


/*
// Naar de public hiveMQ broker werkt.
#define MQTT_BROKER_ADDRESS               "broker.hivemq.com"
#define MQTT_PORT                         1883
#define MQTT_SECURE_CONNECTION            ( 0 )
#define MQTT_USERNAME                     "IoT_Johannes"
#define MQTT_PASSWORD                     "johanneschopov"
*/

/*
#define MQTT_BROKER_ADDRESS               "94e72a782d7b4b9c8ddc2224804a18ce.s1.eu.hivemq.cloud"
#define MQTT_PORT                         8883
#define MQTT_SECURE_CONNECTION            ( 1 )
#define MQTT_USERNAME                     "IoT_Johannes"
#define MQTT_PASSWORD                     "johanneschopov"
*/
/********************* MQTT MESSAGE CONFIGURATION MACROS **********************/
/* The MQTT topics to be used by the publisher and subscriber. */
#define MQTT_PUB_TOPIC_LEDSTATUS        					"ledstate"
#define MQTT_PUB_TOPIC_THERMISTOR         					"Thermistor"
//#define MQTT_SUB_TOPIC                    "ledstate"

#define MQTT_LED_CONTROL_SUB_TOPIC 							"ledstate"
#define MQTT_MOTOR_CONTROL_SUB_TOPIC						"motorstate"

/* Set the QoS that is associated with the MQTT publish, and subscribe messages.
 * Valid choices are 0, 1, and 2. Other values should not be used in this macro.
 */
#define MQTT_MESSAGES_QOS                 ( 0 )

/* Configuration for the 'Last Will and Testament (LWT)'. It is an MQTT message
 * that will be published by the MQTT broker if the MQTT connection is
 * unexpectedly closed. This configuration is sent to the MQTT broker during
 * MQTT connect operation and the MQTT broker will publish the Will message on
 * the Will topic when it recognizes an unexpected disconnection from the client.
 *
 * If you want to use the last will message, set this macro to 1 and configure
 * the topic and will message, else 0.
 */
#define ENABLE_LWT_MESSAGE                ( 0 )
#if ENABLE_LWT_MESSAGE
    #define MQTT_WILL_TOPIC_NAME          MQTT_PUB_TOPIC "/will"
    #define MQTT_WILL_MESSAGE             ("MQTT client unexpectedly disconnected!")
#endif

/* MQTT messages which are published on the MQTT_PUB_TOPIC that controls the
 * device (user LED in this example) state in this code example.
 */
/*
#define MQTT_DEVICE_ON_MESSAGE            "1"
#define MQTT_DEVICE_OFF_MESSAGE           "0"
*/

#define MQTT_DEVICE_ON_MESSAGE            "TURN ON"
#define MQTT_DEVICE_OFF_MESSAGE           "TURN OFF"

#define MQTT_MOTOR_ON_MESSAGE			  "TURN ON"
#define MQTT_MOTOR_OFF_MESSAGE            "TURN OFF"

#define MQTT_MOTOR_FORWARD_MESSAGE			"GO FORWARD"
#define MQTT_MOTOR_BACKWARD_MESSAGE			"GO BACKWARD"
#define MQTT_MOTOR_LEFT_MESSAGE				"GO LEFT"
#define MQTT_MOTOR_RIGHT_MESSAGE			"GO RIGHT"

#define MQTT_MOTOR_FORWARDRIGHT_MESSAGE			"GO FORWARDRIGHT"
#define MQTT_MOTOR_FORWARDLEFT_MESSAGE			"GO FORWARDLEFT"
#define MQTT_MOTOR_BACKWARDRIGHT_MESSAGE		"GO BACKWARDRIGHT"
#define MQTT_MOTOR_BACKWARDLEFT_MESSAGE			"GO BACKWARDLEFT"

#define MQTT_MOTOR_BACK_WHEELS_OFF_RIGHT_MESSAGE            "BACK WHEELS OFF RIGHT"
#define MQTT_MOTOR_BACK_WHEELS_OFF_LEFT_MESSAGE                "BACK WHEELS OFF LEFT"
#define MQTT_MOTOR_FRONT_WHEELS_OFF_RIGHT_MESSAGE            "FRONT WHEELS OFF RIGHT"
#define MQTT_MOTOR_FRONT_WHEELS_OFF_LEFT_MESSAGE            "FRONT WHEELS OFF LEFT"
#define MQTT_MOTOR_RIGHT_WHEELS_OFF_FORWARD_MESSAGE            "RIGHT WHEELS OFF FORWARD"
#define MQTT_MOTOR_RIGHT_WHEELS_OFF_BACKWARD_MESSAGE        "RIGHT WHEELS OFF BACKWARD"
#define MQTT_MOTOR_LEFT_WHEELS_OFF_FORWARD_MESSAGE            "LEFT WHEELS OFF FORWARD"
#define MQTT_MOTOR_LEFT_WHEELS_OFF_BACKWARD_MESSAGE            "LEFT WHEELS OFF BACKWARD"

#define MQTT_MOTOR_CLOCKWISE_MESSAGE				"GO CLOCKWISE"
#define MQTT_MOTOR_COUNTERCLOCKWISE_MESSAGE			"GO COUNTERCLOCKWISE"

/******************* OTHER MQTT CLIENT CONFIGURATION MACROS *******************/
/* A unique client identifier to be used for every MQTT connection. */
#define MQTT_CLIENT_IDENTIFIER            "psoc6-mqtt-client"

/* The timeout in milliseconds for MQTT operations in this example. */
#define MQTT_TIMEOUT_MS                   ( 5000 )

/* The keep-alive interval in seconds used for MQTT ping request. */
#define MQTT_KEEP_ALIVE_SECONDS           ( 60 )

/* Every active MQTT connection must have a unique client identifier. If you
 * are using the above 'MQTT_CLIENT_IDENTIFIER' as client ID for multiple MQTT
 * connections simultaneously, set this macro to 1. The device will then
 * generate a unique client identifier by appending a timestamp to the
 * 'MQTT_CLIENT_IDENTIFIER' string. Example: 'psoc6-mqtt-client5927'
 */
#define GENERATE_UNIQUE_CLIENT_ID         ( 1 )

/* The longest client identifier that an MQTT server must accept (as defined
 * by the MQTT 3.1.1 spec) is 23 characters. However some MQTT brokers support
 * longer client IDs. Configure this macro as per the MQTT broker specification.
 */
#define MQTT_CLIENT_IDENTIFIER_MAX_LEN    ( 23 )

/* As per Internet Assigned Numbers Authority (IANA) the port numbers assigned
 * for MQTT protocol are 1883 for non-secure connections and 8883 for secure
 * connections. In some cases there is a need to use other ports for MQTT like
 * port 443 (which is reserved for HTTPS). Application Layer Protocol
 * Negotiation (ALPN) is an extension to TLS that allows many protocols to be
 * used over a secure connection. The ALPN ProtocolNameList specifies the
 * protocols that the client would like to use to communicate over TLS.
 *
 * This macro specifies the ALPN Protocol Name to be used that is supported
 * by the MQTT broker in use.
 * Note: For AWS IoT, currently "x-amzn-mqtt-ca" is the only supported ALPN
 *       ProtocolName and it is only supported on port 443.
 *
 * Uncomment the below line and specify the ALPN Protocol Name to use this
 * feature.
 */
// #define MQTT_ALPN_PROTOCOL_NAME           "x-amzn-mqtt-ca"

/* Server Name Indication (SNI) is extension to the Transport Layer Security
 * (TLS) protocol. As required by some MQTT Brokers, SNI typically includes the
 * hostname in the Client Hello message sent during TLS handshake.
 *
 * Specify the SNI Host Name to use this extension
 * as specified by the MQTT Broker.
 */
#define MQTT_SNI_HOSTNAME                 "test.mosquitto.org"

/* A Network buffer is allocated for sending and receiving MQTT packets over
 * the network. Specify the size of this buffer using this macro.
 *
 * Note: The minimum buffer size is defined by 'CY_MQTT_MIN_NETWORK_BUFFER_SIZE'
 * macro in the MQTT library. Please ensure this macro value is larger than
 * 'CY_MQTT_MIN_NETWORK_BUFFER_SIZE'.
 */
#define MQTT_NETWORK_BUFFER_SIZE          ( 2 * CY_MQTT_MIN_NETWORK_BUFFER_SIZE )

/* Maximum MQTT connection re-connection limit. */
#define MAX_MQTT_CONN_RETRIES            (150u)

/* MQTT re-connection time interval in milliseconds. */
#define MQTT_CONN_RETRY_INTERVAL_MS      (2000)


/**************** MQTT CLIENT CERTIFICATE CONFIGURATION MACROS ****************/

/* Configure the below credentials in case of a secure MQTT connection. */
/* PEM-encoded client certificate */
/*
#define CLIENT_CERTIFICATE      \
"-----BEGIN CERTIFICATE-----\n"\
"MIID2zCCAsOgAwIBAgIBADANBgkqhkiG9w0BAQsFADCBkDELMAkGA1UEBhMCR0Ix\n"\
"FzAVBgNVBAgMDlVuaXRlZCBLaW5nZG9tMQ4wDAYDVQQHDAVEZXJieTESMBAGA1UE\n"\
"CgwJTW9zcXVpdHRvMQswCQYDVQQLDAJDQTEWMBQGA1UEAwwNbW9zcXVpdHRvLm9y\n"\
"ZzEfMB0GCSqGSIb3DQEJARYQcm9nZXJAYXRjaG9vLm9yZzAeFw0yNDExMjkwOTQw\n"\
"MzhaFw0yNTAyMjcwOTQwMzhaMIG0MQswCQYDVQQGEwJiZTEQMA4GA1UECAwHTGlt\n"\
"YnVyZzEQMA4GA1UEBwwHSGFzc2VsdDERMA8GA1UECgwIVUhhc3NlbHQxIjAgBgNV\n"\
"BAsMGUlJWAgbW0MICAgbW0MbW0MbW0MICAhJSVcxETAPBgNVBAMMCE1vaGFtbWVk\n"\
"MTcwNQYJKoZIhvcNAQkBFihtb2hhbW1lZG1rLmFsYmF5eW91a0BzdHVkZW50LnVo\n"\
"YXNzZWx0LmJlMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA06yO2m9W\n"\
"OrUJY8sg46MzU+Q1NpSgcAlh9e81ETuQ8FXWd/Tm4X5uitdU7T+nKIDnPktFK+g/\n"\
"OGMqv3e4jImZO8kdWLH4+xuF9vmQG/NKtdwOaL27Z+790srDernPB1oXo7K3Rt6T\n"\
"jzdWIoTLCRrlgQWVTyZvJc3dboFvS3ft2XUac1vTF4141r+uP1DXZcHBntBRRb8R\n"\
"A2PWk9ID+B/QpMnq5pdzyj8MxrPspOdfGv3CTVHVgOd5UWFKmP/A2UZayv9+GmjG\n"\
"q6gNqPenmx/1WEQ/LFhpMVKZ3FRGlwKw4zcLm1C6wMkyMwdNAkQQgMxUHX0jPPeh\n"\
"XrfpswQLGr5iRQIDAQABoxowGDAJBgNVHRMEAjAAMAsGA1UdDwQEAwIF4DANBgkq\n"\
"hkiG9w0BAQsFAAOCAQEAOiLtw0tD63IkL7owKaMSyzu1Y4+R1IwbsB64IxhLntk6\n"\
"rHSWcmg8xNGI18etpb0WlQWV8NUHRLSwrXf7EqC308xUiQxmuJ7iZyNQXTvZOqfo\n"\
"kAFidXr6jwzPXF/YuTp4WNXdoRpy2Urgx8eqCQ/ZwdT2uINec5OE+dEHw1j0H4YK\n"\
"Vyw4iUTwyv8c7TcjRmxVl7RIodaqG1p82cv/AEVk4+vHQ+8sYik14hNLJQK6WA11\n"\
"n27wUgLAWOigAtRNc/aRBpTeVQjT6N3AU0WRJuHteY6DE3TcbFRi/zIWFcGwVo0g\n"\
"ij+n4trnRzBxTbDhHkYoAeCxz71vcmZdK7Mr0IKqaw==\n"\
"-----END CERTIFICATE-----\n"

/* PEM-encoded client private key */
/*
#define CLIENT_PRIVATE_KEY          \
"-----BEGIN PRIVATE KEY-----\n"\
"MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDTrI7ab1Y6tQlj\n"\
"yyDjozNT5DU2lKBwCWH17zURO5DwVdZ39Obhfm6K11TtP6cogOc+S0Ur6D84Yyq/\n"\
"d7iMiZk7yR1Ysfj7G4X2+ZAb80q13A5ovbtn7v3SysN6uc8HWhejsrdG3pOPN1Yi\n"\
"hMsJGuWBBZVPJm8lzd1ugW9Ld+3ZdRpzW9MXjXjWv64/UNdlwcGe0FFFvxEDY9aT\n"\
"0gP4H9Ckyerml3PKPwzGs+yk518a/cJNUdWA53lRYUqY/8DZRlrK/34aaMarqA2o\n"\
"96ebH/VYRD8sWGkxUpncVEaXArDjNwubULrAyTIzB00CRBCAzFQdfSM896Fet+mz\n"\
"BAsavmJFAgMBAAECggEAP4FPwn0Qs8BItuXkUidOaObYiC2DDNnjx/MiSq9CRria\n"\
"BZc2BYT3CTSdSCiGsAIsXmYatJeTEAnAc30poxBNJsCpy0hp2Hxjbrtk3t3ZFkKV\n"\
"GBuENCRhhnJKv3EriZsf6H7fuaVhQ+BoOSb5+xtUq6bPKtXvFrpyqoqup0et1kVd\n"\
"9Wrdbup7dUOXG7FWKfXF0FmEobfUmwkbrUI0Vpw/fRQWCQ0/MCANoEbTu5+KFUTj\n"\
"j7dgBaZjMc/iGkn/kpfJvB0tYMkGokKeMfHvHaj2fubBdvvXzSgV2sKJzvIvRx9r\n"\
"EDGtV2lWgwHjYPspNE2aXksxYq/kOfKXbItMXaQB6QKBgQD0voAxeDUOn5uAsE/7\n"\
"oWObA6CZ3TYo8gKOv4D7Vko2BAVSClHI52ryTiQcqUrJ7aisiTZsW37gMBMFjKiR\n"\
"Yt5Y78oNISDLaQx46LfTtOgLoiA8Q+pOZ4SbGjKtKDpurZvobJnnerLqoLuYact+\n"\
"3XxHoajGC/Vtzv99h6Q+TyPPEwKBgQDdaLS+D6Gb0zY+s8rNFvESZcLHLCr5yjdY\n"\
"xtROl1vQM4Jjg/rF7H/QnfMNESgBT51PM9vgWCEbbprNeE0pad0AROS/7YF8TE3s\n"\
"clXyrxcttDKJddzecL/DnDzhx17lxrE3jQWM7OQTR0uLT40+B70fNTbuDc/T+Gq6\n"\
"JQl9xwq8RwKBgQCoYOeRIUJfbS9uud5NIBuG6lfzg11fjiB0VQxH3nXv85Q1fSQH\n"\
"NC8/xMLTdSUfB4KkMUJgChKf6nMrvjm6zGpshYxcvA3z47HQ/Up7PFropgHHdHIe\n"\
"vtlcXAnBV4vb2VUJ8q1vs9qj0TE/CooWHgsCvRWAK3oOsfrUQh2dQJVCQQKBgB8G\n"\
"D+VdzVhHRR+OE8mfAFKCncNIxhDQK+ctnQwSG1RWE1wcXyQimlpB94+KKt/cJv+1\n"\
"FYce8iyk194YkD9QTR4/iQn3nqArFPQfpTlnHUlTKY2nTTaB9+zR1JoSVJK/v484\n"\
"HGH9w4uL/uLbV8AsV2yrCTJcDxX1hgblL6pHHJL7AoGAd4CRN4M3Z05I1UWI0OGV\n"\
"cvfcwlKa4ePRmKq80BM5/V+1ZZ24Mh0ulpmvE6I8d1IELsHdypemnuQrbRQd8EM4\n"\
"cnSZ58zKgsMVnKUcqnlKeliIFV0B2rN4OOJWGLMA+7tyRV9tS/kQpvFxrZTYuGsi\n"\
"63SlFFm4mKJnvCZBCnJReKM=\n"\
"-----END PRIVATE KEY-----\n"

/* PEM-encoded Root CA certificate */
/*//MOSQUITTO
#define ROOT_CA_CERTIFICATE     \
"-----BEGIN CERTIFICATE-----\n"\
"MIIEAzCCAuugAwIBAgIUBY1hlCGvdj4NhBXkZ/uLUZNILAwwDQYJKoZIhvcNAQEL\n"\
"BQAwgZAxCzAJBgNVBAYTAkdCMRcwFQYDVQQIDA5Vbml0ZWQgS2luZ2RvbTEOMAwG\n"\
"A1UEBwwFRGVyYnkxEjAQBgNVBAoMCU1vc3F1aXR0bzELMAkGA1UECwwCQ0ExFjAU\n"\
"BgNVBAMMDW1vc3F1aXR0by5vcmcxHzAdBgkqhkiG9w0BCQEWEHJvZ2VyQGF0Y2hv\n"\
"by5vcmcwHhcNMjAwNjA5MTEwNjM5WhcNMzAwNjA3MTEwNjM5WjCBkDELMAkGA1UE\n"\
"BhMCR0IxFzAVBgNVBAgMDlVuaXRlZCBLaW5nZG9tMQ4wDAYDVQQHDAVEZXJieTES\n"\
"MBAGA1UECgwJTW9zcXVpdHRvMQswCQYDVQQLDAJDQTEWMBQGA1UEAwwNbW9zcXVp\n"\
"dHRvLm9yZzEfMB0GCSqGSIb3DQEJARYQcm9nZXJAYXRjaG9vLm9yZzCCASIwDQYJ\n"\
"KoZIhvcNAQEBBQADggEPADCCAQoCggEBAME0HKmIzfTOwkKLT3THHe+ObdizamPg\n"\
"UZmD64Tf3zJdNeYGYn4CEXbyP6fy3tWc8S2boW6dzrH8SdFf9uo320GJA9B7U1FW\n"\
"Te3xda/Lm3JFfaHjkWw7jBwcauQZjpGINHapHRlpiCZsquAthOgxW9SgDgYlGzEA\n"\
"s06pkEFiMw+qDfLo/sxFKB6vQlFekMeCymjLCbNwPJyqyhFmPWwio/PDMruBTzPH\n"\
"3cioBnrJWKXc3OjXdLGFJOfj7pP0j/dr2LH72eSvv3PQQFl90CZPFhrCUcRHSSxo\n"\
"E6yjGOdnz7f6PveLIB574kQORwt8ePn0yidrTC1ictikED3nHYhMUOUCAwEAAaNT\n"\
"MFEwHQYDVR0OBBYEFPVV6xBUFPiGKDyo5V3+Hbh4N9YSMB8GA1UdIwQYMBaAFPVV\n"\
"6xBUFPiGKDyo5V3+Hbh4N9YSMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEL\n"\
"BQADggEBAGa9kS21N70ThM6/Hj9D7mbVxKLBjVWe2TPsGfbl3rEDfZ+OKRZ2j6AC\n"\
"6r7jb4TZO3dzF2p6dgbrlU71Y/4K0TdzIjRj3cQ3KSm41JvUQ0hZ/c04iGDg/xWf\n"\
"+pp58nfPAYwuerruPNWmlStWAXf0UTqRtg4hQDWBuUFDJTuWuuBvEXudz74eh/wK\n"\
"sMwfu1HFvjy5Z0iMDU8PUDepjVolOCue9ashlS4EB5IECdSR2TItnAIiIwimx839\n"\
"LdUdRudafMu5T5Xma182OC0/u/xRlEm+tvKGGmfFcN0piqVl8OrSPBgIlb+1IKJE\n"\
"m/XriWr/Cq4h/JfB7NTsezVslgkBaoU=\n"\
"-----END CERTIFICATE-----\n"
*/
/*
//HIVEMQ
#define ROOT_CA_CERTIFICATE     \
"-----BEGIN CERTIFICATE-----\n"\
"MIIFCzCCA/OgAwIBAgISBCtYGv7FR2wkunFzzlcW/S0UMA0GCSqGSIb3DQEBCwUA\n"\
"MDMxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQwwCgYDVQQD\n"\
"EwNSMTEwHhcNMjQxMDI0MjM0NDE1WhcNMjUwMTIyMjM0NDE0WjAfMR0wGwYDVQQD\n"\
"DBQqLnMxLmV1LmhpdmVtcS5jbG91ZDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCC\n"\
"AQoCggEBAKVuz2sMPmxx2w/f81/YAEKTbNZMJPk2+ooLFg5hxXvReF+AwIT4XvZ+\n"\
"MLhSKvFxmghJF+BB9WyhqrcJLGDCP4s6SOLWTYixEoTcaLUviqqn+06kYqDJ6E83\n"\
"NGsc7T42DlPnzqcZZjPRed9rt4CP3RgeZlWyYZgiD8FoJG9gie8ytihF/FkGZT8T\n"\
"N4Vkl2vQa3mfBWeeKrcuhcLPxqIWDz/30iYfLtEe5JYYScoCKTXcP9SUStjpR8pD\n"\
"vfOWdvasOAuBy7yBbx01/4lcQt50hfbhTR/K14/D4rNkuuvU7ktSQnoxVXC8YDwG\n"\
"zkny10DFt65mVYLNZcBQtOLHHOZGV30CAwEAAaOCAiswggInMA4GA1UdDwEB/wQE\n"\
"AwIFoDAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwDAYDVR0TAQH/BAIw\n"\
"ADAdBgNVHQ4EFgQUgsEjDU35+EWJKBsFxJ0lM0PXMi4wHwYDVR0jBBgwFoAUxc9G\n"\
"pOr0w8B6bJXELbBeki8m47kwVwYIKwYBBQUHAQEESzBJMCIGCCsGAQUFBzABhhZo\n"\
"dHRwOi8vcjExLm8ubGVuY3Iub3JnMCMGCCsGAQUFBzAChhdodHRwOi8vcjExLmku\n"\
"bGVuY3Iub3JnLzAzBgNVHREELDAqghQqLnMxLmV1LmhpdmVtcS5jbG91ZIISczEu\n"\
"ZXUuaGl2ZW1xLmNsb3VkMBMGA1UdIAQMMAowCAYGZ4EMAQIBMIIBAwYKKwYBBAHW\n"\
"eQIEAgSB9ASB8QDvAHYAzxFW7tUufK/zh1vZaS6b6RpxZ0qwF+ysAdJbd87MOwgA\n"\
"AAGSwSAw7AAABAMARzBFAiAwAiRSrhzlH221ZTJtp3j4/+gIDD94ERv5rj/1ibVF\n"\
"WQIhALrBnoA3Eph3agnYXQDVv6vSFfTzFLk9pNRGMAH5GzkfAHUA5tIxY0B3jMEQ\n"\
"QQbXcbnOwdJA9paEhvu6hzId/R43jlAAAAGSwSAwzwAABAMARjBEAiBYLoQtnHmj\n"\
"pXSeGlMficdDWtNEYBrn1FE7y6vlHZlD6QIgZNqsjSiq1jlNbpo7nRHvWdW/hNtr\n"\
"iUXMkxSOP9f7EMYwDQYJKoZIhvcNAQELBQADggEBADxje/Hu4QNC1MKeSLLDr/Bn\n"\
"6jrX8sJL0yCTAMBeY644P4e9oJC2aN46ngywLW/I+Kjvv0Tz3k/42ROBJIRNqmxK\n"\
"GBYoWraNCBksPQewbZLMi1mYhpraLxDzSSNewfxxBm8d6vzJMaGKQNktZNPgmQ+O\n"\
"vqerlcHEWUVG4uN+LPwd4/f43e2ahkM3FDXBsCVXGDw2kvBgBPjOL4i9l3dm/jNJ\n"\
"sVR15uXB7eu2NupOhw3WLBlkOeJvfAZtE+7KcBuGPsPJTC5R2CyYx9s+tQl96YDp\n"\
"wJwYKWUXSk+J/O5T+bvOXxhg1uyXfVB6wrEd+lm6ZAfB577s6CokXkDs0UKc1Wo=\n"\
"-----END CERTIFICATE-----"

/******************************************************************************
* Global Variables
*******************************************************************************/
extern cy_mqtt_broker_info_t broker_info;
extern cy_awsport_ssl_credentials_t  *security_info;
extern cy_mqtt_connect_info_t connection_info;


#endif /* MQTT_CLIENT_CONFIG_H_ */
