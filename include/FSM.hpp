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

#ifndef INC_FSM_HPP
#define INC_FSM_HPP

#include <Arduino.h>

typedef enum {
    CLOCK,
    ADJUST_CLOCK_SECONDS,
    ADJUST_CLOCK_MINUTES,
    ADJUST_CLOCK_HOURS,
    ALARM,
    ADJUST_ALARM_SECONDS,
    ADJUST_ALARM_MINUTES,
    ADJUST_ALARM_HOURS,
    ALARM_TRIGGERED,
    SELECT_RINGTONE
} state_t;

typedef void (*fsm_action)();

#endif /* FSM.h */