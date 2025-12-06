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

#include <Clock.hpp>
#include <debug.hpp>

#include <Arduino_FreeRTOS.h>
#include <timers.h>
#include <semphr.h>

#define TO_SECONDS(ucHours, ucMinutes, ucSeconds) \
    ((uint32_t)ucHours * 3600 + (uint32_t)ucMinutes * 60 + (uint32_t)ucSeconds)

/* ----------------------------- Clock handler ----------------------------- */

static void prvClockHandler(TimerHandle_t xTimer) {
    void *pvClock = pvTimerGetTimerID(xTimer);
    Clock *pxClock = (Clock *)(pvClock);

    pxClock->vUpdate();
}

/* ------------------------ Clock public interface ------------------------- */

void Clock::vTimeToString(uint32_t ulTime, char *buffer) {
    if(buffer == NULL) {
        LOG("SERIAL ERROR: NULL pointer passed to Clock::vTimeToString()");
        return;
    }

    if(ulTime >= ulSecondsPerDay) {
        LOG("SERIAL ERROR: invalid time passed to Clock::vTimeToString()");
        return;
    }

    uint32_t ulHours = (ulTime / 3600);
    uint32_t ulMinutes = (ulTime % 3600) / 60;
    uint32_t ulSeconds = ulTime % 60;

    vTimeToString(ulHours, ulMinutes, ulSeconds, buffer);
}

void Clock::vTimeToString(
    uint8_t ucHours,
    uint8_t ucMinutes,
    uint8_t ucSeconds,
    char *buffer
) {
    if(buffer == NULL) {
        LOG("SERIAL ERROR: NULL pointer passed to Clock::vTimeToString()");
        return;
    }

    buffer[0] = '0' + (ucHours / 10);
    buffer[1] = '0' + (ucHours % 10);
    buffer[2] = ':';
    buffer[3] = '0' + (ucMinutes / 10);
    buffer[4] = '0' + (ucMinutes % 10);
    buffer[5] = ':';
    buffer[6] = '0' + (ucSeconds / 10);
    buffer[7] = '0' + (ucSeconds % 10);
    buffer[8] = '\0';
}

Clock::Clock() {
    ulTimeSeconds = 0;
    ulAlarmSeconds = ulInvalidTime;

    xOperationMutex = NULL;
    xAlarmSignal = NULL;
    xTimer = NULL;
}

void Clock::vBegin() {
    xOperationMutex = xSemaphoreCreateMutex();

    if(xOperationMutex == NULL) {
        LOG("SERIAL ERROR: unable to create operation mutex on Clock::vBegin() function");
    }
    
    xAlarmSignal = xSemaphoreCreateMutex();
    
    if(xAlarmSignal == NULL) {
        LOG("SERIAL ERROR: unable to create alarm signal mutex on Clock::vBegin() function");
    } else {
        /* Guarantees that signal mutex is taken */
        xSemaphoreTake(xAlarmSignal, 0);
    }
    
    xTimer = xTimerCreate(
        "ClockTimer",
        pdMS_TO_TICKS(1000),
        pdTRUE,
        (void*)this,
        prvClockHandler
    );

    if(xTimer == NULL) {
        LOG("SERIAL ERROR: unable to create Timer on Clock::vBegin() function");
    } else {
        xTimerStart(xTimer, 0);
    }

    vSetTime(0, 0, 0);
}

void Clock::vSetTime(uint8_t ucHours, uint8_t ucMinutes, uint8_t ucSeconds) {
    xSemaphoreTake(xOperationMutex, portMAX_DELAY);

    uint32_t ulTimeSeconds = TO_SECONDS(ucHours, ucMinutes, ucSeconds);

    if(ulTimeSeconds >= ulSecondsPerDay) {
        LOG("SERIAL ERROR: invalid time passed to Clock::vSetTime()");
    } else {
        taskENTER_CRITICAL();
        this->ulTimeSeconds = ulTimeSeconds;
        taskEXIT_CRITICAL();
    }

    xSemaphoreGive(xOperationMutex);
}

void Clock::vSetAlarm(uint8_t ucHours, uint8_t ucMinutes, uint8_t ucSeconds) {
    xSemaphoreTake(xOperationMutex, portMAX_DELAY);
    
    uint32_t ulAlarmSeconds = TO_SECONDS(ucHours, ucMinutes, ucSeconds);

    if(ulAlarmSeconds >= ulSecondsPerDay) {
        LOG("SERIAL ERROR: invalid time passed to Clock::vSetAlarm()");
    } else {
        this->ulAlarmSeconds = ulAlarmSeconds;

        /* Guarantees that signal mutex is taken */
        xSemaphoreTake(xAlarmSignal, 0);
    }
    
    xSemaphoreGive(xOperationMutex);
}

uint32_t Clock::ulGetCurrentTime() {
    uint32_t ulBuffer;

    taskENTER_CRITICAL();
    ulBuffer = ulTimeSeconds;
    taskEXIT_CRITICAL();

    return ulBuffer;
}

boolean Clock::xIsAlarmTriggered() {
    xSemaphoreTake(xOperationMutex, portMAX_DELAY);
    
    boolean xIsTriggered = xSemaphoreTake(xAlarmSignal, 0) == pdTRUE;

    xSemaphoreGive(xOperationMutex);

    return xIsTriggered;
}

boolean Clock::xIsAlarmEnabled() {
    boolean xIsEnabled;

    taskENTER_CRITICAL();
    xIsEnabled = ulAlarmSeconds != ulInvalidTime;
    taskEXIT_CRITICAL();

    return xIsEnabled;
}

void Clock::vUpdate() {
    taskENTER_CRITICAL();
    ulTimeSeconds++;

    if(ulTimeSeconds >= ulSecondsPerDay) {
        ulTimeSeconds = 0;
    }
    taskEXIT_CRITICAL();

    if(!xSemaphoreTake(xOperationMutex, 0)) {
        return;
    }

    if(ulAlarmSeconds != ulInvalidTime && ulAlarmSeconds == ulTimeSeconds) {
        ulAlarmSeconds = ulInvalidTime;
        xSemaphoreGive(xAlarmSignal);
    }

    xSemaphoreGive(xOperationMutex);
}