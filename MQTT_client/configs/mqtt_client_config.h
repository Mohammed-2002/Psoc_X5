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


//#define MQTT_PORT                         1833

/*
// Naar de public hiveMQ broker werkt.
#define MQTT_BROKER_ADDRESS               "broker.hivemq.com"
#define MQTT_PORT                         1883
#define MQTT_SECURE_CONNECTION            ( 0 )
#define MQTT_USERNAME                     ""
#define MQTT_PASSWORD                     ""
*/

#define MQTT_BROKER_ADDRESS					"192.168.132.204"
#define MQTT_PORT                         1883
#define MQTT_SECURE_CONNECTION            ( 0 )
#define MQTT_USERNAME                     ""
#define MQTT_PASSWORD                     ""

/*
// Naar de public hiveMQ broker werkt.
#define MQTT_BROKER_ADDRESS               "broker.hivemq.com"
#define MQTT_PORT                         8883
#define MQTT_SECURE_CONNECTION            ( 1 )
#define MQTT_USERNAME                     ""
#define MQTT_PASSWORD                     ""

/*
#define MQTT_BROKER_ADDRESS               "94e72a782d7b4b9c8ddc2224804a18ce.s1.eu.hivemq.cloud"
#define MQTT_PORT                         8883
#define MQTT_SECURE_CONNECTION            ( 1 )
#define MQTT_USERNAME                     "IoT_Johannes"
#define MQTT_PASSWORD                     "johanneschopov"
*/
/********************* MQTT MESSAGE CONFIGURATION MACROS **********************/
/* The MQTT topics to be used by the publisher and subscriber. */
#define MQTT_PUB_TOPIC_LEDSTATUS_LEDSTATUS                  "ledstate"
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
#define MQTT_DEVICE_ON_MESSAGE            "1"
#define MQTT_DEVICE_OFF_MESSAGE           "0"

#define MQTT_MOTOR_ON_MESSAGE			  "TURN ON"
#define MQTT_MOTOR_OFF_MESSAGE            "TURN OFF"

#define MQTT_MOTOR_FORWARD_MESSAGE			"GO FORWARD"
#define MQTT_MOTOR_BACKWARD_MESSAGE			"GO BACKWARD"
#define MQTT_MOTOR_LEFT_MESSAGE				"GO LEFT"
#define MQTT_MOTOR_RIGHT_MESSAGE			"GO RIGHT"

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
#define MQTT_SNI_HOSTNAME                 (MQTT_BROKER_ADDRESS)

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

#define CLIENT_CERTIFICATE      \
"-----BEGIN CERTIFICATE-----\n"\
"MIIDtTCCAp2gAwIBAgIUIgkDfh7PVhJauPwPbSRa3XNXgzIwDQYJKoZIhvcNAQEL\n"\
"BQAwgYIxCzAJBgNVBAYTAkJFMRAwDgYDVQQIDAdMaW1idXJnMRAwDgYDVQQHDAdI\n"\
"YXNzZWx0MREwDwYDVQQKDAhVSGFzc2VsdDEVMBMGA1UEAwwMSW9UX0pvaGFubmVz\n"\
"MSUwIwYJKoZIhvcNAQkBFhZqY2hvcG92MTAyNDBAZ21haWwuY29tMB4XDTI0MTAy\n"\
"MzE0MTc1NFoXDTI1MTAyMzE0MTc1NFowgYIxCzAJBgNVBAYTAkJFMRAwDgYDVQQI\n"\
"DAdMaW1idXJnMRAwDgYDVQQHDAdIYXNzZWx0MREwDwYDVQQKDAhVSGFzc2VsdDEV\n"\
"MBMGA1UEAwwMSW9UX0pvaGFubmVzMSUwIwYJKoZIhvcNAQkBFhZqY2hvcG92MTAy\n"\
"NDBAZ21haWwuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA2c7M\n"\
"r5DBHOSVhY1xFJDDPETM7tEBRR+VwDOC38oTRhf27oVdkajCJtnrREevrzfErmIT\n"\
"9dpeCT4AzmNrzg66o/3TPe2MXp09Gu97RL58wRtcgQHB8zdgPSBTt+OOpgDoJ9JS\n"\
"ijRJIV/jNcaLirJ2xO6qW3UfGIrF8sM0TDmBNJCtKuPM5Hq3thXR05qPuCwq9DIt\n"\
"jHLUxUZ1qjHTR3FjSEEokGc3o11h+h8FUma//cGOjA+CHo9ex4a/HjHcRCKge5jI\n"\
"JjIGUt7A4PQ7dzxnMY0pcC11fdIwZ3+M/mkBgHxx1h5TZJ98w1nJAHub0sTYUdhj\n"\
"r6UIj4QWm3AgckSJZQIDAQABoyEwHzAdBgNVHQ4EFgQU1S7BwTkJhNsR/sr/6IYs\n"\
"EPrVTgcwDQYJKoZIhvcNAQELBQADggEBAL0qJZV53sGJ8DBEIEYKQAVUy7THxFSe\n"\
"xcYMJja2vui9yVBdtaOwr8OiwUD1on2FpqwAJJEPtD/xL0Yh/iYO9ziMKE1Xds+8\n"\
"IbdeWnE3wP4nJupIg7TObaZWrpWSKQPt9h3jnIWtH7qhn8mGTYo6+qlaiACQdcKn\n"\
"+yo1GytjLNbHL0Wh/5BxUVSafDOeU8OXud8jvOBdLmXI/jFzz92Cpx/oAxIZlFjx\n"\
"z2jhP0K8oOAZJ5wl7vsCjKu86+mk91eC82mZzW3QVrQuHm1IgRyM3bTo2mZZb0Q2\n"\
"4wv9OGPVXvMYdJC/gS9675t68rnjxKjYmF9x6R0hrJYLIwFLPPSiVFs=\n"\
"-----END CERTIFICATE-----"


/* PEM-encoded client private key */

#define CLIENT_PRIVATE_KEY          \
"-----BEGIN RSA PRIVATE KEY-----\n"\
"MIIFNTBfBgkqhkiG9w0BBQ0wUjAxBgkqhkiG9w0BBQwwJAQQhFJjN6aCjP/54kjy\n"\
"vPoVsgICCAAwDAYIKoZIhvcNAgkFADAdBglghkgBZQMEASoEEA8i72d2UgNTTFo5\n"\
"uKChBQAEggTQgKrpB4qC6DsmoolVTf5ux9OG/DAtWcMhtFM/Y7a19YhYCys9Lx4/\n"\
"qo/mU006K24LokD8fEVpfobIc1Lc6uGoHm/wrcGrlMsRIF9HYug1UH4Di1UMaDN/\n"\
"rL4OmM3VyDLuW99tFpVMTcYek9j26VfpZl64beLPJbl1I3IEjOgT7ZDwU35+68es\n"\
"owvUZ04vpjYGvNkwO0f8amvKBG8LOH/+TgfvoJIPO1klcakR7/ElsdLhs97h0Idb\n"\
"/HrUSeWC4imIuR1eZZnaf13NxvGzF2zCQUZDnX29b9i72kQLE9RxprvqYCZ6Z9/z\n"\
"/d7lsMIV6a4pq4Go7Dh3Z5LeSuR8CaBF8K6vg6opFnZn4jafVV23ypVoV+PeLksE\n"\
"jll++tzHhP7+OgFoWdB8iCEDjmZ55inaG+KMH0zDIlUtYi46noViolBtt7jHy3Kx\n"\
"rQNYPpndjvKQLbdzLpZDydv0Y2CmDf8nmlIjQBiQm7+QZaPkrc6no0+c7fM6Ve1d\n"\
"7LRv9PHffIM0qhvt3+XZIgG/xBWEb5KxpTBGOeLtCHCH/SHWqWuRF2HLtTavUHii\n"\
"pJ+JZkHVtlJ6+zLNF5D8FbNb4M/zELvFeLiNdsyYyYIWAXxuc/NQL1wfd0d7wGEE\n"\
"St6WcJ/JifJPT715JXbYbocN7YtWcuHhDiVuUBKagMqC8X/oTvjvbCjLl6GhGO0x\n"\
"QCV0h8SAlP3TgB+yqmTxGsTDM8vP4xkxrU/X6ITPcIvdTrx8DNB3jCR8Zd7tLMMv\n"\
"RZjNT1cO5IieEUPrEYuUStTa77Ucag/04Kf+KQ0k4BABCfzjlmsQz73UadDqX7Vf\n"\
"Trab3dcE0lf2g/vqURwTyua0nA7MgGtnhs+/oh6CJzIgK3CP9iG4Ed4VLobkXcik\n"\
"858DIQTpe/ku6tXLkxZR5Tm4mvU2GrXwdKkgPjIg3fEa38jyttW7ynzuTASmwOYe\n"\
"e1Wd1txGiMZLlMZ4y22eznPqB8K2/eIB+3vb0qhZEtz3KRfuGrtSbLz6RciUGaEU\n"\
"tTX755bY4ohBGPzEymWLpMB+MND4A1xcvodU6O8gpDBhiYGbp2oPYp9SIOhHcp0Q\n"\
"qRzzHA9S10MpiZIMWBynYhca533wg+y7cX/isvweWVu30aPrP8vL8x0DILgbqYOf\n"\
"j0tTGMwRhYLOtDYmg1MZ+oSYRyToLhz3oEM2GwzGCswBJuAEZ0G3+mC9J0DkVY0L\n"\
"ei4W4tP1x0gG11mOA1tcn/D7DB8r6Logd4tLBl+E/tTPhPCFb0y4akexvUL6CfvX\n"\
"8MJ4fvbuUspubFFD1KI/EV0G/kZeT19cPgXzf4dULh/SX8/VTE6UtJ58wN55s4/h\n"\
"dhbU2tZjdulDvm2wT8LSXMor1++WIehQczcZn6gFDyDBin6YkQaMHvy1BUGVcura\n"\
"91IhBa76nbkRS08U36PNlKtw2QCEWxkm7vIi4lFcEcgwQ7OfEg9j0uZSGj1iIdhk\n"\
"WTa6x4xkIAVKvYhXQVRT+eua+PYw02rsOMYgZdsiCamUvYZ3v/h3eZslwZ02D0WZ\n"\
"ZMxzv8Ktnb1xR1Kg7lz4r2KIhgigvuphqhDPAIRgFchHE0G7GQLrwmlc1wIM9x/r\n"\
"x8umQfTn9ALettNdxuGUbwNoXuDIV5GMDozeGlRqQ0hg5TZhr6J9cBY=\n"\
"-----END RSA PRIVATE KEY-----"

/* PEM-encoded Root CA certificate */

#define ROOT_CA_CERTIFICATE     \
"-----BEGIN CERTIFICATE-----\n"\
"MIIFCzCCA/OgAwIBAgISBBxaSddAkSw9yGAzHMsl2PzKMA0GCSqGSIb3DQEBCwUA\n"\
"MDMxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQwwCgYDVQQD\n"\
"EwNSMTAwHhcNMjQwODI2MDAzODU3WhcNMjQxMTI0MDAzODU2WjAfMR0wGwYDVQQD\n"\
"DBQqLnMxLmV1LmhpdmVtcS5jbG91ZDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCC\n"\
"AQoCggEBAKVuz2sMPmxx2w/f81/YAEKTbNZMJPk2+ooLFg5hxXvReF+AwIT4XvZ+\n"\
"MLhSKvFxmghJF+BB9WyhqrcJLGDCP4s6SOLWTYixEoTcaLUviqqn+06kYqDJ6E83\n"\
"NGsc7T42DlPnzqcZZjPRed9rt4CP3RgeZlWyYZgiD8FoJG9gie8ytihF/FkGZT8T\n"\
"N4Vkl2vQa3mfBWeeKrcuhcLPxqIWDz/30iYfLtEe5JYYScoCKTXcP9SUStjpR8pD\n"\
"vfOWdvasOAuBy7yBbx01/4lcQt50hfbhTR/K14/D4rNkuuvU7ktSQnoxVXC8YDwG\n"\
"zkny10DFt65mVYLNZcBQtOLHHOZGV30CAwEAAaOCAiswggInMA4GA1UdDwEB/wQE\n"\
"AwIFoDAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwDAYDVR0TAQH/BAIw\n"\
"ADAdBgNVHQ4EFgQUgsEjDU35+EWJKBsFxJ0lM0PXMi4wHwYDVR0jBBgwFoAUu7zD\n"\
"R6XkvKnGw6RyDBCNojXhyOgwVwYIKwYBBQUHAQEESzBJMCIGCCsGAQUFBzABhhZo\n"\
"dHRwOi8vcjEwLm8ubGVuY3Iub3JnMCMGCCsGAQUFBzAChhdodHRwOi8vcjEwLmku\n"\
"bGVuY3Iub3JnLzAzBgNVHREELDAqghQqLnMxLmV1LmhpdmVtcS5jbG91ZIISczEu\n"\
"ZXUuaGl2ZW1xLmNsb3VkMBMGA1UdIAQMMAowCAYGZ4EMAQIBMIIBAwYKKwYBBAHW\n"\
"eQIEAgSB9ASB8QDvAHUAPxdLT9ciR1iUHWUchL4NEu2QN38fhWrrwb8ohez4ZG4A\n"\
"AAGRjFS2yQAABAMARjBEAiByWCVhihpQOmK2CSOfGblDapOqFeIyf8qox65JENXv\n"\
"xAIgKUa31O+21m2dG2fNFVTMAVMxmYnA/O3hpZRNByTdBBUAdgAZmBBxCfDWUi4w\n"\
"gNKeP2S7g24ozPkPUo7u385KPxa0ygAAAZGMVLbIAAAEAwBHMEUCIQCIfg3H4lSI\n"\
"8LzvQnjbZHR2CPOKl1j/Uy5dZ5Jbm/hRRAIgI27iaYce0r52RxJAaOEVWBNC6jPG\n"\
"qHfuz0MOlrPPNP4wDQYJKoZIhvcNAQELBQADggEBAM2U4dQTtrckAtm53SsLhIcF\n"\
"75a+J9kOwY1HHjq9h0tc8/HlFlWrZW4RyMdFOtI7dl0207cVJNqqF7/HHEIcy/Zk\n"\
"k/xbW7aqAjjhWD+C2PI42/7i0vU5gQi/khK2Xy2gdDhIaQWukmm4Vm1SD23QLs6l\n"\
"wNGZ5wD+jLgdWPARhPUVgcyauqF4Ebpynuk+RV42GVtdVIjCtaji1XwDeK1uprvh\n"\
"hQhnG3xvgX+gzTt7XfS7lm1Od2rv6hRDyQvTMNcr69mgbNp7pBQtMRpsjdefZH1B\n"\
"yzeTv+ECge9Twcz7vNvOJE5psDD5rFnF+2UvALh2Prr+HC5d/jRsYhBNRYRrbDI=\n"\
"-----END CERTIFICATE-----"


/******************************************************************************
* Global Variables
*******************************************************************************/
extern cy_mqtt_broker_info_t broker_info;
extern cy_awsport_ssl_credentials_t  *security_info;
extern cy_mqtt_connect_info_t connection_info;


#endif /* MQTT_CLIENT_CONFIG_H_ */
