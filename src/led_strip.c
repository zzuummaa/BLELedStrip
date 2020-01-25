//
// Created by Stepan on 25.01.2020.
//

#include "led_strip.h"
#include "esp_timer.h"
#include "driver/gpio.h"

uint64_t TIMER_PERIOD = (1000UL * 1000UL) / (FLICKER_FREQ * 256UL);
esp_timer_handle_t strip_control_timer;

volatile uint8_t redBrightness = 255;
volatile uint8_t greenBrightness = 255;
volatile uint8_t blueBrightness = 255;
uint8_t counter = 0;

void strip_control_timer_callback(void* arg) {
    gpio_set_level(BLUE_LED_GPIO, counter < blueBrightness);
    counter++;
}

static const esp_timer_create_args_t strip_timer_args = {
        .callback = &strip_control_timer_callback,
        /* name is optional, but may help identify the timer when debugging */
        .name = "led_strip"
};

bool LEDStrip_init() {
    gpio_pad_select_gpio(BLUE_LED_GPIO);
    gpio_set_direction(BLUE_LED_GPIO, GPIO_MODE_OUTPUT);

    ESP_ERROR_CHECK(esp_timer_create(&strip_timer_args, &strip_control_timer));
    esp_timer_start_periodic(strip_control_timer, TIMER_PERIOD);

    return true;
}

void LEDStrip_update(uint8_t isOn_, uint8_t* color_) {
    ESP_ERROR_CHECK(esp_timer_stop(strip_control_timer));
    if (isOn_ > 0) {
        redBrightness = color_[0];
        greenBrightness = color_[1];
        blueBrightness = color_[2];
    } else {
        redBrightness = 0;
        greenBrightness = 0;
        blueBrightness = 0;
    }
    esp_timer_start_periodic(strip_control_timer, TIMER_PERIOD);
}