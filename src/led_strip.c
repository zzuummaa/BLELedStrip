//
// Created by Stepan on 25.01.2020.
//

#include "led_strip.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "driver/mcpwm.h"

inline float color2duty(uint8_t color) {
    return (float)color / 255.f * 100.f;
}

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
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, RED_LED_GPIO);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, GREEN_LED_GPIO);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, BLUE_LED_GPIO);

    mcpwm_config_t pwm_config;
    pwm_config.frequency = 60;
    pwm_config.cmpr_a = 50;    //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 50;    //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    ESP_ERROR_CHECK(mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config));

    pwm_config.cmpr_b = 0;
    ESP_ERROR_CHECK(mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config));

    return true;
}

void LEDStrip_update(uint8_t isOn_, uint8_t* color_) {
    if (isOn_ > 0) {
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, color2duty(color_[0]));
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, color2duty(color_[1]));
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, color2duty(color_[2]));
    } else {
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 0);
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, 0);
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, 0);
    }
}