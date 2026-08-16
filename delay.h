#ifndef DELAY_H
#define DELAY_H

/**
 * @brief Blocking delay function
 * Creates a delay by executing a busy-wait loop
 * Timing is approximate and depends on CPU frequency (assumes 16 MHz)
 * 
 * @param d Number of milliseconds to delay (approximately)
 * 
 * Note: This is a simple blocking delay. For production code, consider using
 * SysTick timer or other hardware timers for more accurate timing.
 */
void delay(int d);

#endif // DELAY_H
