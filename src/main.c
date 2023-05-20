#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#define LED_STATUS_PIN PICO_DEFAULT_LED_PIN
#define INTERVAL_IM_ALIVE_MS (1000)

void gpio_initialization() {
    // Init the Status LED GPIO pin. (Also sets the pin to low.)
    gpio_init(LED_STATUS_PIN);

    // Set the Status LED GPIO pin as output.
    gpio_set_dir(LED_STATUS_PIN, GPIO_OUT);
}

void flash_led_once(void) {
    // Led ON
    gpio_put(LED_STATUS_PIN, 1);
    sleep_ms(200);
    // Led OFF
    gpio_put(LED_STATUS_PIN, 0);
}

bool repeating_timer_callback(struct repeating_timer *t) {
    // Print the I'm alive sign every 1 sec.
    printf("...\n");
    return true;
}

int main() {
    // Initialize the GPIOs. 
    gpio_initialization();

    stdio_init_all();
    // Wait for a while. Otherwise, USB CDC doesn't print all printfs.
    sleep_ms(500);
    printf("STARTED\n");

    // Print the  system frequency.
    printf("System clock is %u kHz\n", frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS));

    // Set the repeating timer for I'm alive sign.
#if INTERVAL_IM_ALIVE_MS > 0
    struct repeating_timer timer;
    add_repeating_timer_ms(INTERVAL_IM_ALIVE_MS, repeating_timer_callback, NULL, &timer);
#endif

    // Let the user know initialization was successful and the program is going to start.
    flash_led_once();
    printf("Starting to sample the POST codes.\n");

    while(1);
}