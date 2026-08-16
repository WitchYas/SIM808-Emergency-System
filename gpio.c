#include "gpio.h"

/**
 * @brief Initialize GPIO pins for the project
 * Configures PD12 as output for LED control
 * - MODER: Push-pull output
 * - OSPEEDR: Low speed
 * - PUPDR: Pull-up
 * - OTYPER: Push-pull (default)
 */
void GPIO_init(void) {
    // Enable GPIOD clock
    RCC->AHB1ENR |= (1u << 3);

    // PD12 as output
    GPIOD->MODER &= ~(1u << 25);
    GPIOD->MODER |= (1u << 24);

    // Low speed
    GPIOD->OSPEEDR &= ~(1u << 25);
    GPIOD->OSPEEDR &= ~(1u << 24);

    // Pull-up
    GPIOD->PUPDR &= ~(1u << 25);
    GPIOD->PUPDR |= (1u << 24);

    // Push-pull (default)
    GPIOD->OTYPER &= ~(1u << 12);

    // LED OFF initially
    GPIOD->ODR &= ~(1u << 12);
}

/**
 * @brief Turn LED on (PD12)
 */
void LED_On(void) {
    GPIOD->ODR |= (1u << 12);
}

/**
 * @brief Turn LED off (PD12)
 */
void LED_Off(void) {
    GPIOD->ODR &= ~(1u << 12);
}

/**
 * @brief Toggle LED state (PD12)
 */
void LED_Toggle(void) {
    GPIOD->ODR ^= (1u << 12);
}
