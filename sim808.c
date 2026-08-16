#include "sim808.h"
#include "uart.h"
#include "delay.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Configure SIM808 module for emergency functions
 * Sets up calling notifications, SMS mode, and saves settings
 * Each command has a 500ms delay to allow SIM808 to process
 */
void configSIM808(void) {
    // Enable calling line identification
    USART2_SendString("AT+CLIP=1\r\n");
    delay(500);

    // Set SMS notification mode
    // Format: +CNMI=<mode>,<mt>,<bm>,<ds>,<bfr>
    // 2,1,0,0,0: Send unsolicited result codes, route SMS to TE, etc.
    USART2_SendString("AT+CNMI=2,1,0,0,0\r\n");
    delay(500);

    // Set SMS text mode (as opposed to PDU mode)
    USART2_SendString("AT+CMGF=1\r\n");
    delay(500);

    // Save settings to SIM808 non-volatile memory
    USART2_SendString("AT&W\r\n");
    delay(500);
}

/**
 * @brief Make a call to the specified phone number
 * Sends ATD command and waits for 3 seconds
 * 
 * @param phoneNumber Null-terminated string containing the phone number
 */
void GSM_MakeCall(const char* phoneNumber) {
    char cmd[40];
    sprintf(cmd, "ATD%s;\r\n", phoneNumber);
    USART2_SendString(cmd);
    delay(3000);  // Let it ring for 3 seconds
}

/**
 * @brief Send an SMS message to the specified phone number
 * Uses text mode SMS transmission with Ctrl+Z terminator
 * 
 * @param phoneNumber Null-terminated string containing the recipient's phone number
 * @param message Null-terminated string containing the SMS message
 */
void GSM_SendSMS(const char* phoneNumber, const char* message) {
    char cmd[40];

    // Set SMS text mode
    USART2_SendString("AT+CMGF=1\r");
    delay(500);

    // Set recipient phone number
    sprintf(cmd, "AT+CMGS=\"%s\"\r", phoneNumber);
    USART2_SendString(cmd);
    delay(500);

    // Wait for '>' prompt indicating SIM808 is ready for message
    for (int i = 0; i < 2000000; i++) {
        if (uart2_available()) {
            char r = UART2_GetChar();
            if (r == '>') {
                break;
            }
        }
    }

    // Send message content
    USART2_SendString(message);

    // Send Ctrl+Z (0x1A) to finalize and transmit SMS
    UART2_SendChar(0x1A);
    delay(2000);  // Wait for transmission to complete
}

/**
 * @brief Get GPS coordinates from SIM808
 * Powers on GPS, requests current location, and parses the response
 * Extracts latitude and longitude from the CGNSINF response
 * 
 * @param lat Pointer to buffer where latitude string will be stored
 * @param lon Pointer to buffer where longitude string will be stored
 * 
 * Response format: +CGNSINF: <stat>,<lat>,<lon>,<altitude>,<speed>,<course>,<fix_time>,...
 * Field indices (1-based): stat(1), lat(4), lon(5)
 */
void GSM_GetGPS(char* lat, char* lon) {
    // Power on GPS module
    USART2_SendString("AT+CGNSPWR=1\r");
    delay(3000);  // Give GPS time to initialize

    // Request GPS information
    USART2_SendString("AT+CGNSINF=0\r");
    delay(1000);

    // Read response into buffer
    char buffer[256];
    int idx = 0;

    while (uart2_available()) {
        buffer[idx++] = UART2_GetChar();
        if (idx >= sizeof(buffer) - 1) {
            break;
        }
    }
    buffer[idx] = '\0';

    // Parse response using strtok
    // Expected format: +CGNSINF: <stat>,<lat>,<lon>,...
    char *token = strtok(buffer, ",");
    int field = 0;

    while (token != NULL) {
        field++;
        // Latitude is typically in field 4
        if (field == 4) {
            strcpy(lat, token);
        }
        // Longitude is typically in field 5
        if (field == 5) {
            strcpy(lon, token);
        }
        token = strtok(NULL, ",");
    }
}
