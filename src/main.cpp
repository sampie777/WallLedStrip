#include <Arduino.h>
#include "config.h"

// Interval (ms) between each update
unsigned long updateInterval = 100;

uint8_t nextScene = 0;

void setColor(int white, int red, int green, int blue);

bool timeHasPassed(unsigned long *lastUpdatedTime, unsigned long interval, bool updateTime);


/** SCENES **/

void trafficLight() {
    // Config
    static uint8_t nextStep = 0;
    updateInterval = TRAFFICLIGHT_CYCLE_PERIOD;

    // Execution
    setColor(nextStep == 0 ? MAX_OUTPUT_VALUE : 0,
             nextStep == 1 ? MAX_OUTPUT_VALUE : 0,
             nextStep == 2 ? MAX_OUTPUT_VALUE : 0,
             nextStep == 3 ? MAX_OUTPUT_VALUE : 0);

    nextStep++;
    nextStep = nextStep % 4;
}

void rainbow() {
    //Config
    static uint16_t nextColorStep = 0;
    static uint16_t nextWhiteStep = 0;
    updateInterval = 1000 / RAINBOW_UPDATES_PER_SECOND;
    uint16_t colorPeriodInSteps = RAINBOW_COLOR_CYCLE_DURATION / updateInterval;
    uint16_t whitePeriodInSteps = RAINBOW_WHITE_CYCLE_DURATION / (double) updateInterval;

    // Execution
    setColor(
            0.1 * 0.5 * MAX_OUTPUT_VALUE * (1 + sin(2 * 3.1415 * ((double) nextWhiteStep / whitePeriodInSteps + 0))),
            0.5 * MAX_OUTPUT_VALUE * (1 + sin(2 * 3.1415 * ((double) nextColorStep / colorPeriodInSteps + 0))),
            0.5 * MAX_OUTPUT_VALUE * (1 + sin(2 * 3.1415 * ((double) nextColorStep / colorPeriodInSteps + 1.0 / 3))),
            0.5 * MAX_OUTPUT_VALUE * (1 + sin(2 * 3.1415 * ((double) nextColorStep / colorPeriodInSteps + 2.0 / 3)))
    );

    nextColorStep++;
    nextWhiteStep++;
    nextColorStep = nextColorStep % colorPeriodInSteps;
    nextWhiteStep = nextWhiteStep % whitePeriodInSteps;
}

void colorHop() {
    //Config
    static unsigned long lastColorHopTime = 0;
    static uint16_t nextStep = 0;
    static uint16_t targetStep = UINT16_MAX;
    updateInterval = 1000 / COLOR_HOP_UPDATES_PER_SECOND;
    uint16_t periodInSteps = COLOR_HOP_MAX_TRANSITION_TIME / updateInterval;

    if (targetStep == UINT16_MAX || timeHasPassed(&lastColorHopTime, COLOR_HOP_PERIOD + COLOR_HOP_MAX_TRANSITION_TIME, true)) {
        targetStep = random(0, periodInSteps);
    }

    if (nextStep != targetStep) {
        nextStep++;
        nextStep = nextStep % periodInSteps;
    }

    // Execution
    setColor(
            0,
            0.5 * MAX_OUTPUT_VALUE * (1 + sin(2 * 3.1415 * ((double) nextStep / periodInSteps + 0))),
            0.5 * MAX_OUTPUT_VALUE * (1 + sin(2 * 3.1415 * ((double) nextStep / periodInSteps + 1.0 / 3))),
            0.5 * MAX_OUTPUT_VALUE * (1 + sin(2 * 3.1415 * ((double) nextStep / periodInSteps + 2.0 / 3)))
    );
}

void fullOnWhite() {
    setColor(MAX_OUTPUT_VALUE, 0, 0, 0);
}

void fullOnRed() {
    setColor(0, MAX_OUTPUT_VALUE, 0, 0);
}

void fullOnPurple() {
    setColor(0, MAX_OUTPUT_VALUE, 0, MAX_OUTPUT_VALUE);
}

void allFullOn() {
    setColor(MAX_OUTPUT_VALUE, MAX_OUTPUT_VALUE, MAX_OUTPUT_VALUE, MAX_OUTPUT_VALUE);
}

/** END SCENES **/


/** LOGIC **/

void checkInput() {
    static unsigned long lastActionTime = 0;

    // Debounce button using cooldown period
    if (!timeHasPassed(&lastActionTime, BUTTON_DEBOUNCE_COOLDOWN_PERIOD, false))
        return;

    // Debounce button using minimum pressed time
    unsigned long pressStartTime = millis();
    while (!digitalRead(PIN_BUTTON)) {
        delay(10);
    }

    if (millis() < pressStartTime + BUTTON_MIN_PRESS_TIME)
        return;

    // Advance to next scene if button is pressed
    nextScene++;
    updateInterval = 100;
    lastActionTime = millis();
}

void queue() {
    switch (nextScene) {
        case 0:
            colorHop();
            break;
        case 2:
            rainbow();
            break;
        case 3:
            fullOnRed();
            break;
        case 4:
            fullOnPurple();
            break;
        case 5:
            fullOnWhite();
            break;
        case 6:
            allFullOn();
            break;
        case 7:
            trafficLight();
            break;
        default:
            nextScene = 0;
    }
}

void setColor(int white, int red, int green, int blue) {
    analogWrite(PIN_WHITE, white);
    analogWrite(PIN_RED, red);
    analogWrite(PIN_GREEN, green);
    analogWrite(PIN_BLUE, blue);
}

bool needsUpdate() {
    static unsigned long lastUpdatedTime = 0;
    return timeHasPassed(&lastUpdatedTime, updateInterval, true);
}

bool timeHasPassed(unsigned long *lastUpdatedTime, unsigned long interval, bool updateTime) {
    if (millis() < *lastUpdatedTime + interval) {
        return false;
    }
    if (updateTime) {
        *lastUpdatedTime = millis();
    }
    return true;
}

void setup() {
    pinMode(PIN_BUTTON, INPUT_PULLUP);

    pinMode(PIN_WHITE, OUTPUT);
    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);

    setColor(0, 0, 0, 0);

    randomSeed(analogRead(0));

#if ENABLE_SERIAL
    Serial.begin(115200);
    delay(100);
    Serial.println("Ready");
#endif
}

void loop() {
    checkInput();

    if (!needsUpdate()) return;

    queue();
}
