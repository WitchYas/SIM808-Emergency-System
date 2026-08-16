#ifndef GPIO_H
#define GPIO_H

#include "stm32f407xx.h"

/**
 * @brief Initialize GPIO pins for the project
 * Currently configures PD12 as output for LED control
 */
void GPIO_init(void);

/**
 * @brief Turn LED on (PD12)
 */
void LED_On(void);

/**
 * @brief Turn LED off (PD12)
 */
void LED_Off(void);

/**
 * @brief Toggle LED state (PD12)
 */
void LED_Toggle(void);

#endif // GPIO_H
