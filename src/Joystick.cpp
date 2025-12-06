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

#include <Joystick.hpp>

typedef Joystick::Direction_t Direction_t;
typedef Joystick::Vector Vector;

/* ----------------------- Joystick public interface ----------------------- */

Joystick::Joystick(uint8_t ucAnalogXPin, uint8_t ucAnalogYPin, uint8_t ucButtonPin) {
    pinMode(ucButtonPin, INPUT_PULLUP);

    this->ucAnalogXPin = ucAnalogXPin;
    this->ucAnalogYPin = ucAnalogYPin;
    this->ucButtonPin = ucButtonPin;

    ulDebounceDelay = ulDefaultDebounceDelay;

    xAnalogCorrection = {
        .sX = sDefaultAnalogXCorrection,
        .sY = sDefaultAnalogYCorrection
    };

    fActivationPercent = fDefaultActivationPercent;

    eAnalogState = CENTER;
    eAnalogPoll = CENTER;

    ucButtonState = LOW;
    ucButtonPoll = LOW;

    ulAnalogDebounceTime = millis();
    ulButtonDebounceTime = millis();
}

void Joystick::vUpdate() {
    const int16_t sMaxTravel = (sJoystickAnalogReadMax - sJoystickAnalogReadMin) / 2;
    int16_t sActivationThreshold = fActivationPercent * sMaxTravel;

    Vector xAnalog = xGetAnalogRead();

    Direction_t eAnalogCurrentPoll = CENTER;
    
    if(xAnalog.sX >= sActivationThreshold) {
        eAnalogCurrentPoll = RIGHT;
    } else if(xAnalog.sX <= -sActivationThreshold) {
        eAnalogCurrentPoll = LEFT;
    }

    if(xAnalog.sY >= sActivationThreshold) {
        eAnalogCurrentPoll = UP;
    } else if(xAnalog.sY<= -sActivationThreshold) {
        eAnalogCurrentPoll = DOWN;
    }

    if(eAnalogCurrentPoll != eAnalogPoll) {
        ulAnalogDebounceTime = millis();
    }

    eAnalogPoll = eAnalogCurrentPoll;

    if(millis() - ulAnalogDebounceTime > ulDebounceDelay) {
        eLastAnalogState = eAnalogState;
        eAnalogState = eAnalogPoll;
    }

    uint8_t ucButtonCurrentPoll = !digitalRead(ucButtonPin);

    if(ucButtonCurrentPoll != ucButtonPoll) {
        ulButtonDebounceTime = millis();
    }

    ucButtonPoll = ucButtonCurrentPoll;

    if(millis() - ulButtonDebounceTime > ulDebounceDelay) {
        ucLastButtonState = ucButtonState;
        ucButtonState = ucButtonPoll;
    }
}

Vector Joystick::xGetAnalogRead() {
    Vector xCurrentRead {
        .sX = analogRead(ucAnalogXPin),
        .sY = analogRead(ucAnalogYPin)
    };

    return xCurrentRead - xAnalogCorrection;
}

Direction_t Joystick::eGetAnalogDirection() {
    return eAnalogState;
}

boolean Joystick::xMovedFromTo(Direction_t eFrom, Direction_t eTo) {
    return eFrom == eLastAnalogState && eTo == eAnalogState;
}

boolean Joystick::xIsPressed() {
    return ucButtonState;
}

boolean Joystick::xIsRisingEdge() {
    return ucLastButtonState == LOW && ucButtonState == HIGH;
}

boolean Joystick::xIsFallingEdge() {
    return ucLastButtonState == HIGH && ucButtonState == LOW;
}
