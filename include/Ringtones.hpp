/***********************************************************************************

    MIT License

    Copyright (c) 2025 Rafael Rodrigues Sanches

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

***********************************************************************************/

#ifndef INC_RINGTONES_HPP
#define INC_RINGTONES_HPP

#include <Arduino.h>
#include <Buzzer.hpp>

#define ringtoneGET_MELODY_SIZE(array) (sizeof (array) / sizeof(Buzzer::Note_t))

const Buzzer::Note_t eBeeps[] = {
    Buzzer::NOTE_A5, Buzzer::NO_TONE,
    Buzzer::NOTE_A5, Buzzer::NO_TONE,
    Buzzer::NOTE_A5, Buzzer::NO_TONE,
    Buzzer::NO_TONE, Buzzer::NO_TONE
};

/******************************************************************************

- Based On: https://www.songsterr.com/a/wsa/foo-fighters-everlong-tab-s86505t4
- BPM: 632

******************************************************************************/
const Buzzer::Note_t eEverlong[] = {
    Buzzer::NOTE_D3, Buzzer::NO_TONE,
    Buzzer::NOTE_D3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_D3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_D3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_D3, Buzzer::NO_TONE,
    Buzzer::NOTE_D3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_D3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_D3, Buzzer::NO_TONE,

    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_D3, Buzzer::NOTE_A3,

    Buzzer::NOTE_G3, Buzzer::NO_TONE,
    Buzzer::NOTE_G3, Buzzer::NO_TONE,
    Buzzer::NOTE_D3, Buzzer::NOTE_A4,
    Buzzer::NOTE_G3, Buzzer::NO_TONE,
    Buzzer::NOTE_G3, Buzzer::NO_TONE,
    Buzzer::NOTE_G3, Buzzer::NO_TONE,
    Buzzer::NOTE_D3, Buzzer::NOTE_A4,
    Buzzer::NOTE_G3, Buzzer::NO_TONE,

    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,

    Buzzer::NOTE_D3, Buzzer::NO_TONE,
    Buzzer::NOTE_D3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_D3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_D3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_D3, Buzzer::NO_TONE,
    Buzzer::NOTE_D3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_D3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_D3, Buzzer::NO_TONE,

    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_D3, Buzzer::NOTE_A3,

    Buzzer::NOTE_G3, Buzzer::NO_TONE,
    Buzzer::NOTE_G3, Buzzer::NO_TONE,
    Buzzer::NOTE_D3, Buzzer::NOTE_A4,
    Buzzer::NOTE_G3, Buzzer::NO_TONE,
    Buzzer::NOTE_G3, Buzzer::NO_TONE,
    Buzzer::NOTE_G3, Buzzer::NO_TONE,
    Buzzer::NOTE_D3, Buzzer::NOTE_A4,
    Buzzer::NOTE_G3, Buzzer::NO_TONE,

    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_B3, Buzzer::NO_TONE,
    Buzzer::NOTE_FS3, Buzzer::NOTE_CS4
};

struct Ringtone {
    const Buzzer::Note_t *eMelody;
    const char *pcName;
    uint16_t usSize;
    uint16_t usBpm;
};

Ringtone pxRingtones[] = {
    [0] = Ringtone{
        eBeeps,
        "Beeps",
        ringtoneGET_MELODY_SIZE(eBeeps),
        240
    },

    [1] = Ringtone{
        eEverlong,
        "Everlong",
        ringtoneGET_MELODY_SIZE(eEverlong),
        632
    }
};

#endif /* Ringtones.hpp */