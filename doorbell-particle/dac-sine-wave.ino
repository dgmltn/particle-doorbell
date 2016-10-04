// This #include statement was automatically added by the Particle IDE.

///Sine out with stored array
//by Amanda Ghassaei
//http://www.instructables.com/id/Arduino-Audio-Output/
//Sept 2012

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
*/

#include "SparkIntervalTimer/SparkIntervalTimer.h"

int SINE_LENGTH = 100;
byte SINE[] = {127, 134, 142, 150, 158, 166, 173, 181, 188, 195, 201, 207, 213, 219, 224, 229, 234, 238, 241, 245, 247, 250, 251, 252, 253, 254, 253, 252, 251, 250, 247, 245, 241, 238, 234, 229, 224, 219, 213, 207, 201, 195, 188, 181, 173, 166, 158, 150, 142, 134, 127, 119, 111, 103, 95, 87, 80, 72, 65, 58, 52, 46, 40, 34, 29, 24, 19, 15, 12, 8, 6, 3, 2, 1, 0, 0, 0, 1, 2, 3, 6, 8, 12, 15, 19, 24, 29, 34, 40, 46, 52, 58, 65, 72, 80, 87, 95, 103, 111, 119,};

IntervalTimer audio_clock;

// Bit shifting volume: 1-4
const int volume = 3;

volatile float t = 0;
volatile float step;

void setup() {
    pinMode(DAC, OUTPUT);
}

void loop() {
    for(int i = 200; i <= 4000; i += 40) {
        tone(i);
        delay(50);
    }
    for(int i = 4000; i >= 200; i -= 40) {
        tone(i);
        delay(50);
    }
    tone(0);
    delay(1000);
    // tone(500);
    // delay(5000);
}

void noTone(int pin /* ignore this, just make it compatible with builtin tone function */) {
    audio_clock.end();
    t = 0;
}

void tone(int pin /* ignore this, just make it compatible with builtin tone function */, int freq) {
    if (freq == 0) {
        noTone(pin);
    }
    else if (freq <= 400) {
        step = 1;
        audio_clock.begin(playback_handler, 20 * 400.0 / freq, uSec);
    }
    else {
        step = freq / 400.0;
        audio_clock.begin(playback_handler, 20, uSec);
    }
}

void playback_handler(void) {
    analogWrite(DAC, SINE[(int)t] << volume);
    t += step;
    while (t > SINE_LENGTH) t -= SINE_LENGTH;
}

// Unused
// void simple_sin_loop() {
//     //send sine wave to DAC, centered around (127/255)*5 = 2.5V
//     analogWrite(DAC, sine[t] * volume);
//     delayMicroseconds(50);//wait 50us
//     t = (t + 1) % 100;
// }
