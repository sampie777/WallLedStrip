//
// Created by samuel on 27-10-21.
//

#ifndef WALLLEDSTRIP_CONFIG_H
#define WALLLEDSTRIP_CONFIG_H

/** CONFIG **/

/* PINS */
#define PIN_WHITE 6
#define PIN_RED 9
#define PIN_GREEN 10
#define PIN_BLUE 11

#define PIN_BUTTON 2

/* BUTTON */
#define BUTTON_MIN_PRESS_TIME 100   // Minimum time the button must be pressed for it to register a valid press (ms)
#define BUTTON_DEBOUNCE_COOLDOWN_PERIOD 200 // Don't check the button after is has been pressed for this amount of time (ms)

/* SCENES */
#define MAX_OUTPUT_VALUE 255

// Traffic light
#define TRAFFICLIGHT_CYCLE_PERIOD 2000  // Each period, change a color

// Rainbow
#define RAINBOW_UPDATES_PER_SECOND 25
#define RAINBOW_COLOR_CYCLE_DURATION 16000
#define RAINBOW_WHITE_CYCLE_DURATION (RAINBOW_COLOR_CYCLE_DURATION * 4.7)

/* DEBUG */
#define ENABLE_SERIAL false

/** END CONFIG **/

#endif //WALLLEDSTRIP_CONFIG_H
