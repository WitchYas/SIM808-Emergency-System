#ifndef UART_H
#define UART_H

#include "stm32f407xx.h"

/**
 * @brief Initialize USART2 for communication with SIM808
 * Configures PA2 (TX), PA3 (RX) at 9600 baud
 * Assumes 16 MHz system clock
 */
void USART2_Init(void);

/**
 * @brief Send a single character via USART2
 * @param ch Character to send
 */
void UART2_SendChar(char ch);

/**
 * @brief Send a null-terminated string via USART2
 * @param str Pointer to the string to send
 */
void USART2_SendString(const char *str);

/**
 * @brief Check if data is available to read from USART2
 * @return Non-zero if data is available, 0 otherwise
 */
int uart2_available(void);

/**
 * @brief Receive a single character from USART2 (blocking)
 * This function blocks until a character is received
 * @return The received character
 */
unsigned char UART2_GetChar(void);

#endif // UART_H
