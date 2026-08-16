#include "uart.h"

/**
 * @brief Initialize USART2 for communication with SIM808
 * Configures:
 * - PA2 as TX (USART2_TX)
 * - PA3 as RX (USART2_RX)
 * - Baud rate: 9600 (for 16 MHz clock: BRR = 0x0681)
 * - Mode: Asynchronous, 8 data bits, 1 stop bit
 */
void USART2_Init(void) {
    // Enable GPIO and USART2 clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // PA2 (TX) and PA3 (RX) as alternate function
    GPIOA->MODER |= (1u << 5);   // PA2 alternate function
    GPIOA->MODER &= ~(1u << 4);
    GPIOA->MODER |= (1u << 7);   // PA3 alternate function
    GPIOA->MODER &= ~(1u << 6);

    // Set alternate function to AF7 (USART2)
    GPIOA->AFR[0] |= (7 << 8) | (7 << 12);

    // Set baud rate to 9600 (assuming 16 MHz clock)
    USART2->BRR = 0x0681;

    // Enable USART2, TX, and RX
    USART2->CR1 |= 0x200C;  // UE=1, TE=1, RE=1
}

/**
 * @brief Send a single character via USART2
 * Waits until the transmit register is ready
 * @param ch Character to send
 */
void UART2_SendChar(char ch) {
    USART2->DR = ch;
    // Wait for transmission complete (TC flag)
    while (!(USART2->SR & (1u << 6)));
}

/**
 * @brief Send a null-terminated string via USART2
 * @param str Pointer to the string to send
 */
void USART2_SendString(const char *str) {
    while (*str) {
        UART2_SendChar(*str++);
    }
}

/**
 * @brief Check if data is available to read from USART2
 * Checks the RXNE (Read Data Register Not Empty) flag
 * @return Non-zero if data is available, 0 otherwise
 */
int uart2_available(void) {
    return USART2->SR & USART_SR_RXNE;
}

/**
 * @brief Receive a single character from USART2 (blocking)
 * This function blocks until a character is received
 * @return The received character
 */
unsigned char UART2_GetChar(void) {
    while (!uart2_available());
    return (unsigned char)USART2->DR;
}
