#ifndef SIM808_H
#define SIM808_H

/**
 * @brief Configure SIM808 module for emergency functions
 * Sets up calling notifications, SMS mode, and saves settings
 * 
 * Commands sent:
 * - AT+CLIP=1: Enable calling line identification
 * - AT+CNMI=2,1,0,0,0: Set SMS notification mode
 * - AT+CMGF=1: Set text mode for SMS
 * - AT&W: Save settings to SIM808 memory
 */
void configSIM808(void);

/**
 * @brief Make a call to the specified phone number
 * @param phoneNumber Null-terminated string containing the phone number (e.g., "26367129")
 * 
 * Note: Call will ring for approximately 3 seconds by default
 */
void GSM_MakeCall(const char* phoneNumber);

/**
 * @brief Send an SMS message to the specified phone number
 * @param phoneNumber Null-terminated string containing the recipient's phone number
 * @param message Null-terminated string containing the SMS message (max 160 characters)
 * 
 * Process:
 * 1. Switch to SMS text mode
 * 2. Set recipient phone number
 * 3. Wait for '>' prompt
 * 4. Send message content
 * 5. Send Ctrl+Z to finalize and send
 */
void GSM_SendSMS(const char* phoneNumber, const char* message);

/**
 * @brief Get GPS coordinates from SIM808
 * @param lat Pointer to buffer where latitude string will be stored (min 20 chars)
 * @param lon Pointer to buffer where longitude string will be stored (min 20 chars)
 * 
 * Process:
 * 1. Power on GPS module
 * 2. Wait for GPS to acquire signal
 * 3. Request GPS information
 * 4. Parse response to extract latitude and longitude
 * 
 * Note: GPS acquisition can take 30+ seconds on first use (cold start)
 */
void GSM_GetGPS(char* lat, char* lon);

#endif // SIM808_H
