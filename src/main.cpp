#include <Arduino.h>

/** CONFIG **/

#define PIN_WHITE 6
#define PIN_RED 9
#define PIN_GREEN 10
#define PIN_BLUE 11

#define PIN_BUTTON 2

#define MAX_OUTPUT_VALUE 255
#define ENABLE_SERIAL false

// Interval (ms) between each update
unsigned long updateInterval = 100;


/** END CONFIG **/


static uint8_t nextScene = 0;

void setColor(int white, int red, int green, int blue);

bool needsUpdate();


/** SCENES **/

void trafficLight() {
    // Config
    static uint8_t nextStep = 0;
    updateInterval = 3000;

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
    updateInterval = 1000 / 40;
    uint16_t colorPeriod = 16000;
    uint16_t whitePeriod = colorPeriod * 4.7;
    uint16_t colorPeriodInSteps = colorPeriod / updateInterval;
    uint16_t whitePeriodInSteps = whitePeriod / updateInterval;

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

void fullOnWhite() {
    setColor(MAX_OUTPUT_VALUE, 0, 0, 0);
}

void fullOnRed() {
    setColor(0, MAX_OUTPUT_VALUE, 0, 0);
}

void fullOnPurple() {
    setColor(0, MAX_OUTPUT_VALUE, 0, MAX_OUTPUT_VALUE);
}

/** END SCENES **/


/** LOGIC **/

void checkInput() {
    if (digitalRead(PIN_BUTTON)) return;

    // Advance to next scene if button is pressed
    nextScene++;
    delay(500);
    updateInterval = 100;
}

void queue() {
    switch (nextScene) {
        case 0:
            rainbow();
            break;
        case 1:
            fullOnRed();
            break;
        case 2:
            fullOnPurple();
            break;
        case 3:
            fullOnWhite();
            break;
        case 4:
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
    static unsigned long last_updated_time = 0;
    if (last_updated_time + updateInterval > millis()) {
        return false;
    }
    last_updated_time = millis();
    return true;
}

void setup() {
    pinMode(PIN_BUTTON, INPUT_PULLUP);

    pinMode(PIN_WHITE, OUTPUT);
    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);

    setColor(0, 0, 0, 0);

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
