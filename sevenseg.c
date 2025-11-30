#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"

#include <stdio.h>

// Raspberry Pi Pico Pins
#define CLOCK_PIN       2   // GP2, SPI0 SCK
#define DATA_PIN        3   // GP3, SPI0 TX
#define LATCH_PIN       6   // GP6

#define NUM_BCD_DIGITS  20

// Shift register (74HC595) pin 1-7 are used (Q_B - Q_H).
// Pin 15 (Q_A) is unused. Pin 9 (Q_H') is serial data output for next digit.
// Clock and latch signals are shared among all daisy chained shift registers.
//
// Shift register => 7 segment
// QB/1 => b/6
// QC/2 => a/7
// QD/3 => f/9
// QE/4 => c/4
// QF/5 => d/2
// QG/6 => e/1
// QH/7 => g/10
//
// 7 6 5 4 3 2 1 0
// g e d c f a b _
//
// Decimal digit => 7 segment pins (abcdefg) => 8-bit data (QH is MSB)
// 0 => abcdef_ => 0b01111110
// 1 => _bc____ => 0b00010010
// 2 => ab_de_g => 0b11100110
// 3 => abcd__g => 0b10110110
// 4 => _bc__fg => 0b10011010
// 5 => a_cd_fg => 0b10111100
// 6 => a_cdefg => 0b11111100
// 7 => abc__f_ => 0b00011110
// 8 => abcdefg => 0b11111110
// 9 => abcd_fg => 0b10111110

// Digit to 7-segment map (abcdefg + dp) for common cathode
const uint8_t digit_map[10] = {
    0b01111110, // 0
    0b00010010, // 1
    0b11100110, // 2
    0b10110110, // 3
    0b10011010, // 4
    0b10111100, // 5
    0b11111100, // 6
    0b00011110, // 7
    0b11111110, // 8
    0b10111110  // 9
};

uint8_t bcd_digits[NUM_BCD_DIGITS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void pulse_latch() {
    gpio_put(LATCH_PIN, 1);
    sleep_us(1);
    gpio_put(LATCH_PIN, 0);
}

// Push the whole 20-digit frame, then latch once.
void display_all_digits() {
    for(int i=NUM_BCD_DIGITS-1; i>=0; i--) {
        spi_write_blocking(spi0, &digit_map[bcd_digits[i]], 1);
    }
    pulse_latch();
}

// Increment the BCD array with carry (wraps from 10^20 - 1 back to 0)
void increment_bcd() {
    for (int i = 0; i < NUM_BCD_DIGITS; ++i) {
        if (++bcd_digits[i] <= 9) {
            break; // no carry needed
        }
        bcd_digits[i] = 0; // carry to next digit
        // if we carried past the last digit, we just wrapped to 0
    }
}

void blink_led() {
    if(cyw43_arch_init()) {
        return;
    }

    while(true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(500);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(500);
    }
}

int main() {
    stdio_init_all();
    multicore_launch_core1(blink_led);

    // 1 MHz baudrate
    spi_init(spi0, 1000000);

    // Clock polarity = 0 (clock idle low)
    // Clock phase = 0 (sample on rising edge)
    // LSB ot MSB, 8 bits
    spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_LSB_FIRST);

    gpio_set_function(CLOCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(DATA_PIN, GPIO_FUNC_SPI);

    gpio_init(LATCH_PIN);
    gpio_set_dir(LATCH_PIN, GPIO_OUT);
    gpio_put(LATCH_PIN, 0); // Initialize to LOW

    while(true) {
        display_all_digits();
        increment_bcd();
        sleep_ms(1);
    }
}