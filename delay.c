#include "delay.h"

/**
 * @brief Blocking delay function
 * Creates a delay by executing a busy-wait loop
 * Calibrated for approximately 1ms per iteration at 16 MHz
 * 
 * @param d Number of milliseconds to delay (approximately)
 * 
 * Calculation: Each inner loop takes approximately 3000 cycles at 16 MHz
 * So 3000 cycles = 1ms, making the loop count of 3000 appropriate for 1ms delay
 */
void delay(int d) {
    for (; d > 0; d--) {
        // Volatile to prevent compiler optimization
        for (volatile int i = 0; i < 3000; i++);
    }
}
