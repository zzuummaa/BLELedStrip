//
// Created by Stepan on 25.01.2020.
//

#ifndef BLELEDSTRIP_LED_STRIP_H
#define BLELEDSTRIP_LED_STRIP_H

#include "stdbool.h"
#include "stdint.h"

#define FLICKER_FREQ 60
#define RED_LED_GPIO 33
#define GREEN_LED_GPIO 25
#define BLUE_LED_GPIO 32

bool LEDStrip_init();
void LEDStrip_update(uint8_t isOn_, uint8_t* color_);

#endif //BLELEDSTRIP_LED_STRIP_H
