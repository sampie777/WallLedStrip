# Wall LED strip

_Embedded code for Arduino/ESP32 like controllers for a RGBW LED strip._

## Setup

### Hardware

Connect your microcontroller to the LED strip using some kind of transistor setup. Define the connected pins in `config.h`. Each pin should drive a transistor which drives one color of the LED strip. Make sure you use PWM pins, otherwise fading won't work.

Also, connect a push button between `GND` and `PIN_BUTTON`. This button will advance the scenes. 

There are four output pins and these are predefined as:

| Name (`config.h`) | Purpose | Default pin |
| --- | --- | --- |
| `PIN_BUTTON` | Input, when button goes to low, the script will advance to the next scene | 2 |
| `PIN_WHITE` | Output, drives the transistor for the white color | 6 (PWM) |
| `PIN_RED` | Output, drives the transistor for the red color | 9 (PWM) |
| `PIN_GREEN` | Output, drives the transistor for the green color | 10 (PWM) |
| `PIN_BLUE` | Output, drives the transistor for the blue color | 11 (PWM) |

### Software

As previous mentioned, make sure to specify the correct pins in `config.h`.

You can also edit some parameters in `config.h`, like the cycle period for some of the scenes and the max PWM output value. Note that this output value doesn't define the output volts, but the value for the internal PWM driver. 

The button has a virtual debounce, which can be finetuned in `config.h`.

> Note that all time values in `config.h` are in ms.

