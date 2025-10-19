#include "pico/stdlib.h"
#include <stdio.h>

// Define GPIO pins for 74HC595
#define DATA_PIN    2   // SER
#define CLOCK_PIN   3   // SRCLK
#define LATCH_PIN   4   // RCLK

// Digit to 7-segment map (abcdefg + dp) for common cathode
const uint8_t digit_map[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9 
};

void pulse_clock() {
    gpio_put(CLOCK_PIN, 1);
    sleep_us(1);
    gpio_put(CLOCK_PIN, 0);
}

void pulse_latch() {
    gpio_put(LATCH_PIN, 1);
    sleep_us(1);
    gpio_put(LATCH_PIN, 0);
}

// Shift out one byte, MSB first
void shift_out(uint8_t data) {
    for(int i=7; i >= 0; i--) {
        gpio_put(DATA_PIN, (data >> i) & 1);
        pulse_clock();
    }
    pulse_latch();
}

int main() {
    stdio_init_all();

    gpio_init(DATA_PIN);
    gpio_init(CLOCK_PIN);
    gpio_init(LATCH_PIN);

    gpio_set_dir(DATA_PIN, GPIO_OUT);
    gpio_set_dir(CLOCK_PIN, GPIO_OUT);
    gpio_set_dir(LATCH_PIN, GPIO_OUT);

    while(true) {
        for(int i=0; i < 10; i++) {
            shift_out(digit_map[i]);
            sleep_ms(100);
        }
    }
}