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

#ifndef INC_JOYSTICK_HPP
#define INC_JOYSTICK_HPP

#include <Arduino.h>

class Joystick {
public:
    static const uint32_t ulDefaultDebounceDelay = 50;
    static constexpr float fDefaultActivationPercent = 0.95f;
    
    static const int16_t sDefaultAnalogXCorrection = 512;
    static const int16_t sDefaultAnalogYCorrection = 512;
    
    static const int16_t sJoystickAnalogReadMin = 0;
    static const int16_t sJoystickAnalogReadMax = 1023;

    typedef enum {
        UP,
        DOWN,
        CENTER,
        LEFT,
        RIGHT
    } Direction_t;

    struct Vector {
        int16_t sX, sY;

        Vector operator+(const Vector& pxOther) {
            Vector xSum {
                .sX = sX + pxOther.sX,
                .sY = sY + pxOther.sY
            };

            return xSum;
        }

        Vector operator-(const Vector& pxOther) {
            Vector xDiff {
                .sX = sX - pxOther.sX,
                .sY = sY - pxOther.sY
            };

            return xDiff;
        }
    };

    Joystick(uint8_t ucAnalogXPin, uint8_t ucAnalogYPin, uint8_t ucButtonPin);
    
    void vUpdate();

    Vector xGetAnalogRead();
    Direction_t eGetAnalogDirection();
    boolean xMovedFromTo(Direction_t eFrom, Direction_t eTo);

    boolean xIsPressed();
    boolean xIsRisingEdge();
    boolean xIsFallingEdge();

    Vector xAnalogCorrection;
    float fActivationPercent;
    uint32_t ulDebounceDelay;

private:
    uint8_t ucAnalogXPin;
    uint8_t ucAnalogYPin;
    uint8_t ucButtonPin;

    Direction_t eLastAnalogState;
    Direction_t eAnalogState;
    Direction_t eAnalogPoll;

    uint8_t ucLastButtonState;
    uint8_t ucButtonState;
    uint8_t ucButtonPoll;

    uint32_t ulAnalogDebounceTime;
    uint32_t ulButtonDebounceTime;
};

#endif /* Joystick.hpp */