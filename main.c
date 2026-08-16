#include "stm32f407xx.h"
#include <string.h>
#include <stdio.h>

// Module headers
#include "gpio.h"
#include "uart.h"
#include "delay.h"
#include "sim808.h"

/*.......................... MAIN APPLICATION ..........................*/
/**
 * @brief Emergency system application
 * 
 * Sequence:
 * 1. Initialize hardware (GPIO, UART, SIM808)
 * 2. Make emergency call
 * 3. Retrieve GPS coordinates
 * 4. Send SMS with location
 * 5. Indicate completion
 */
int main(void) {
    // Configuration
    const char* emergency_number = "26367129";
    char lat[20] = {0};
    char lon[20] = {0};
    char msg[160];

    // Initialize hardware modules
    GPIO_init();
    USART2_Init();
    configSIM808();

    // System active indicator
    LED_On();

    // Make emergency call
    GSM_MakeCall(emergency_number);
    delay(3000);  // Wait during call

    // Get GPS location
    GSM_GetGPS(lat, lon);

    // Prepare and send SMS with location link
    sprintf(msg, "Help! My location is https://maps.google.com/?q=%s,%s", lat, lon);
    GSM_SendSMS(emergency_number, msg);

    // Indicate completion
    LED_Off();

    // Infinite loop (system halted)
    while (1);

    return 0;
}
