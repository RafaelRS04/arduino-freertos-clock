/***********************************************************************************

    MIT License

    Copyright (c) 2025 Rafael Rodrigues Sanches

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, ucModify, merge, publish, distribute, sublicense, and/or sell
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

#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include <semphr.h>
#include <task.h>

#include <LiquidCrystal.h>
#include <Ringtones.hpp>
#include <Joystick.hpp>
#include <Buzzer.hpp>
#include <Clock.hpp>
#include <debug.hpp>
#include <FSM.hpp>

/* --------------------------- Pins Declarations --------------------------- */

#define VRY_PIN A0
#define VRX_PIN A1
#define BUTTON_PIN 2
#define BUZZER_PIN 3
#define D4_PIN 4
#define D5_PIN 5
#define D6_PIN 6
#define D7_PIN 7
#define RS_PIN 8
#define EN_PIN 9

/* ------------------------ Macros and Declarations ------------------------ */

#define LCD_COLS 16
#define LCD_ROWS 2

#define DISPLAY_BUFFER_SIZE (LCD_COLS * LCD_ROWS)
#define TIMER_BUFFER_SIZE 9

#define NOT_USED(x) ((void)(x))

/* ------------------------------ Components ------------------------------- */

LiquidCrystal xLCD(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);
Joystick xJoystick(VRY_PIN, VRX_PIN, BUTTON_PIN);
Clock xClock;

/* ----------------------------- Global Context ---------------------------- */

/* Not Null-terminated buffers */
char pcDisplayContent[DISPLAY_BUFFER_SIZE];
char pcDisplayBuffer[DISPLAY_BUFFER_SIZE];

/* Null-terminated buffer */
char pcTimeBuffer[TIMER_BUFFER_SIZE];

/* Clock time variables */
uint8_t ucTimeSeconds = 0;
uint8_t ucTimeMinutes = 0;
uint8_t ucTimeHours = 0;

/* Alarm variables */
uint8_t ucAlarmSeconds = 0;
uint8_t ucAlarmMinutes = 0;
uint8_t ucAlarmHours = 0;
uint8_t ucRingtoneIndex = 0;

/* Display Mutex */
SemaphoreHandle_t xDisplayMutex = NULL;

/* -------------------------------- Helpers -------------------------------- */

void vWriteToDisplay(const char *pcBuffer, char *pcContent, size_t uxSize) {
    if(memcmp(pcContent, pcBuffer, DISPLAY_BUFFER_SIZE) == 0) {
        return;
    }

    memcpy(pcContent, pcBuffer, DISPLAY_BUFFER_SIZE);

    for(size_t uxIndex = 0; uxIndex < uxSize; uxIndex++) {
        if(uxIndex % LCD_COLS == 0) {
            xLCD.setCursor(0, uxIndex / LCD_COLS % LCD_ROWS);
        }
        
        xLCD.print(pcContent[uxIndex]);
    }
}

void vJoystickIncrement(uint8_t& pucCounter, uint8_t ucMod, uint8_t ucIncrement) {
    if(ucMod == 0 || ucIncrement % ucMod == 0) {
        LOG("SERIAL ERROR: Invalid arguments passed to vJoystickIncrement() function");
        return;
    }

    if(xJoystick.xMovedFromTo(Joystick::CENTER, Joystick::UP)) {
        pucCounter++;

        if(pucCounter == ucMod) {
            pucCounter = 0;
        }
    } else if(xJoystick.xMovedFromTo(Joystick::CENTER, Joystick::DOWN)) {
        if(pucCounter > 0) {
            pucCounter--;
        } else {
            pucCounter = ucMod - 1;
        }
    } else if(xJoystick.xMovedFromTo(Joystick::CENTER, Joystick::RIGHT)) {
        pucCounter = ((uint16_t)pucCounter + ucIncrement) % ucMod;
    } else if(xJoystick.xMovedFromTo(Joystick::CENTER, Joystick::LEFT)) {
        uint8_t safeDec = ucIncrement % ucMod;
        pucCounter = ((uint16_t)pucCounter + ucMod - safeDec) % ucMod;
    }
}

void vBufferWriteCenter(
    char *pcBuffer,
    size_t uxBufferSize,
    const char *pcText,
    size_t uxRow
) {
    size_t uxOffset = uxRow * LCD_COLS;
    
    if(uxOffset >= uxBufferSize) {
        LOG("SERIAL ERROR: Offset exceeds the maximum buffer size");
        return;
    }
    
    size_t uxSliceSize = uxBufferSize - uxOffset;
    
    if(uxSliceSize < LCD_COLS) {
        LOG("SERIAL ERROR: Buffer slice width is insufficient for LCD row");
        return;
    }
    
    size_t uxTextLength = strlen(pcText);

    if(uxTextLength > LCD_COLS) {
        LOG("SERIAL ERROR: Text exceeds the maximum row size for centering");
        return;
    }

    size_t uxLeftPadding = (LCD_COLS - uxTextLength) / 2;

    memset(&pcBuffer[uxOffset], ' ', LCD_COLS);
    memcpy(&pcBuffer[uxOffset + uxLeftPadding], pcText, uxTextLength);
}

/* ------------------------------ FSM Actions ------------------------------ */

void vClock() {
    uint32_t ulTimeSeconds = xClock.ulGetCurrentTime();

    xClock.vTimeToString(
        ulTimeSeconds,
        pcTimeBuffer
    );

    if(xSemaphoreTake(xDisplayMutex, 0) == pdTRUE) {
        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, "TIME", 0);
        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, pcTimeBuffer, 1);
        xSemaphoreGive(xDisplayMutex);
    }
}

void vAdjustClockSeconds() {
    vJoystickIncrement(ucTimeSeconds, 60, 10);

    xClock.vTimeToString(
        ucTimeHours,
        ucTimeMinutes,
        ucTimeSeconds,
        pcTimeBuffer
    );

    if(xSemaphoreTake(xDisplayMutex, 0) == pdTRUE) {
        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, "TIME ADJUST", 0);
        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, pcTimeBuffer, 1);
        xSemaphoreGive(xDisplayMutex);
    }
}

void vAdjustClockMinutes() {
    vJoystickIncrement(ucTimeMinutes, 60, 10);

    xClock.vTimeToString(
        ucTimeHours,
        ucTimeMinutes,
        ucTimeSeconds,
        pcTimeBuffer
    );

    if(xSemaphoreTake(xDisplayMutex, 0) == pdTRUE) {
        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, "TIME ADJUST", 0);
        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, pcTimeBuffer, 1);
        xSemaphoreGive(xDisplayMutex);
    }
}

void vAdjustClockHours() {
    vJoystickIncrement(ucTimeHours, 24, 10);

    xClock.vTimeToString(
        ucTimeHours,
        ucTimeMinutes,
        ucTimeSeconds,
        pcTimeBuffer
    );

    if(xSemaphoreTake(xDisplayMutex, 0) == pdTRUE) {
        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, "TIME ADJUST", 0);
        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, pcTimeBuffer, 1);
        xSemaphoreGive(xDisplayMutex);
    }
}

void vAlarm() {
    if(xSemaphoreTake(xDisplayMutex, 0) == pdTRUE) {

        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, "ALARM", 0);

        if(xClock.xIsAlarmEnabled()) {
            xClock.vTimeToString(
                ucAlarmHours,
                ucAlarmMinutes,
                ucAlarmSeconds,
                pcTimeBuffer
            );

            vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, pcTimeBuffer, 1);
        } else {
            vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, "DISABLED", 1);
        }

        xSemaphoreGive(xDisplayMutex);
    }
}

void vAdjustAlarmSeconds() {
    vJoystickIncrement(ucAlarmSeconds, 60, 10);

    xClock.vTimeToString(
        ucAlarmHours,
        ucAlarmMinutes,
        ucAlarmSeconds,
        pcTimeBuffer
    );

    if(xSemaphoreTake(xDisplayMutex, 0) == pdTRUE) {
        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, "ALARM ADJUST", 0);
        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, pcTimeBuffer, 1);
        xSemaphoreGive(xDisplayMutex);
    }
}

void vAdjustAlarmMinutes() {
    vJoystickIncrement(ucAlarmMinutes, 60, 10);

    xClock.vTimeToString(
        ucAlarmHours,
        ucAlarmMinutes,
        ucAlarmSeconds,
        pcTimeBuffer
    );

    if(xSemaphoreTake(xDisplayMutex, 0) == pdTRUE) {
        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, "ALARM ADJUST", 0);
        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, pcTimeBuffer, 1);
        xSemaphoreGive(xDisplayMutex);
    }
}

void vAdjustAlarmHours() {
    vJoystickIncrement(ucAlarmHours, 24, 10);

    xClock.vTimeToString(
        ucAlarmHours,
        ucAlarmMinutes,
        ucAlarmSeconds,
        pcTimeBuffer
    );

    if(xSemaphoreTake(xDisplayMutex, 0) == pdTRUE) {
        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, "ALARM ADJUST", 0);
        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, pcTimeBuffer, 1);
        xSemaphoreGive(xDisplayMutex);
    }
}

void vAlarmTriggered() {
    if(xSemaphoreTake(xDisplayMutex, 0) == pdTRUE) {
        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, "ALARM RINGING", 0);

        xClock.vTimeToString(
            ucAlarmHours,
            ucAlarmMinutes,
            ucAlarmSeconds,
            pcTimeBuffer
        );

        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, pcTimeBuffer, 1);

        xSemaphoreGive(xDisplayMutex);
    }
}

void vSelectRingtone() {
    vJoystickIncrement(ucRingtoneIndex, 2, 1);

    if(xSemaphoreTake(xDisplayMutex, 0) == pdTRUE) {
        vBufferWriteCenter(pcDisplayBuffer, DISPLAY_BUFFER_SIZE, "RINGTONE", 0);

        vBufferWriteCenter(
            pcDisplayBuffer,
            DISPLAY_BUFFER_SIZE,
            pxRingtones[ucRingtoneIndex].pcName,
            1
        );
        
        xSemaphoreGive(xDisplayMutex);
    }
}

/* --------------------------------- Tasks --------------------------------- */

void vFSM(void *pvParameters) {
    NOT_USED(pvParameters);

    static state_t eLastState = CLOCK;
    static state_t eCurrentState = CLOCK;
    static fsm_action pxCurrentAction = NULL;

    while(true) {
        xJoystick.vUpdate();
    
        switch(eCurrentState) {
        case CLOCK:
            pxCurrentAction = vClock;

            if(xJoystick.xIsRisingEdge()) {
                eLastState = eCurrentState;
                eCurrentState = ADJUST_CLOCK_SECONDS;
                ucTimeSeconds = 0;
                ucTimeMinutes = 0;
                ucTimeHours = 0;
            } else if(xJoystick.xMovedFromTo(Joystick::CENTER, Joystick::UP)) {
                eLastState = eCurrentState;
                eCurrentState = ALARM;
            } else if(xClock.xIsAlarmTriggered()) {
                eLastState = eCurrentState;
                eCurrentState = ALARM_TRIGGERED;

                Buzzer::vPlay(
                    pxRingtones[ucRingtoneIndex].eMelody,
                    pxRingtones[ucRingtoneIndex].usSize,
                    pxRingtones[ucRingtoneIndex].usBpm
                );
            }

            break;
        case ADJUST_CLOCK_SECONDS:
            pxCurrentAction = vAdjustClockSeconds;

            if(xJoystick.xIsRisingEdge()) {
                eLastState = eCurrentState;
                eCurrentState = ADJUST_CLOCK_MINUTES;
            }

            break;
        case ADJUST_CLOCK_MINUTES:
            pxCurrentAction = vAdjustClockMinutes;

            if(xJoystick.xIsRisingEdge()) {
                eLastState = eCurrentState;
                eCurrentState = ADJUST_CLOCK_HOURS;
            }

            break;
        case ADJUST_CLOCK_HOURS:
            pxCurrentAction = vAdjustClockHours;

            if(xJoystick.xIsRisingEdge()) {
                eLastState = eCurrentState;
                eCurrentState = CLOCK;

                xClock.vSetTime(ucTimeHours, ucTimeMinutes, ucTimeSeconds);
            }

            break;
        case ALARM:
            pxCurrentAction = vAlarm;

            if(xJoystick.xIsRisingEdge()) {
                eLastState = eCurrentState;
                eCurrentState = ADJUST_ALARM_SECONDS;
                ucAlarmSeconds = 0;
                ucAlarmMinutes = 0;
                ucAlarmHours = 0;
            }  else if(xJoystick.xMovedFromTo(Joystick::CENTER, Joystick::DOWN)) {
                eLastState = eCurrentState;
                eCurrentState = CLOCK;
            }  else if(xClock.xIsAlarmTriggered()) {
                eLastState = eCurrentState;
                eCurrentState = ALARM_TRIGGERED;

                Buzzer::vPlay(
                    pxRingtones[ucRingtoneIndex].eMelody,
                    pxRingtones[ucRingtoneIndex].usSize,
                    pxRingtones[ucRingtoneIndex].usBpm
                );
            }

            break;
        case ADJUST_ALARM_SECONDS:
            pxCurrentAction = vAdjustAlarmSeconds;

            if(xJoystick.xIsRisingEdge()) {
                eLastState = eCurrentState;
                eCurrentState = ADJUST_ALARM_MINUTES;
            }

            break;
        case ADJUST_ALARM_MINUTES:
            pxCurrentAction = vAdjustAlarmMinutes;

            if(xJoystick.xIsRisingEdge()) {
                eLastState = eCurrentState;
                eCurrentState = ADJUST_ALARM_HOURS;
            }

            break;
        case ADJUST_ALARM_HOURS:
            pxCurrentAction = vAdjustAlarmHours;

            if(xJoystick.xIsRisingEdge()) {
                eLastState = eCurrentState;
                eCurrentState = SELECT_RINGTONE;

                xClock.vSetAlarm(ucAlarmHours, ucAlarmMinutes, ucAlarmSeconds);
            }

            break;
        case SELECT_RINGTONE:
            pxCurrentAction = vSelectRingtone;

            if(xJoystick.xIsRisingEdge()) {
                eLastState = eCurrentState;
                eCurrentState = ALARM;
            }
            break;
        case ALARM_TRIGGERED:
            pxCurrentAction = vAlarmTriggered;

            if(xJoystick.xIsRisingEdge()) {
                eCurrentState = eLastState;
                eLastState = ALARM_TRIGGERED;
                
                Buzzer::vStop();
            }

            break;
        }

        pxCurrentAction();
    }
}

void vDisplayRefresh(void *pvParameters) {
    NOT_USED(pvParameters);

    while(true) {
        if(xSemaphoreTake(xDisplayMutex, portMAX_DELAY)) {
            vWriteToDisplay(
                pcDisplayBuffer,
                pcDisplayContent,
                DISPLAY_BUFFER_SIZE
            );

            xSemaphoreGive(xDisplayMutex);
        }
    }
}

/* --------------------------------- Setup --------------------------------- */

void setup() {
    Serial.begin(9600);

    xLCD.begin(LCD_COLS, LCD_ROWS);
    xLCD.setCursor(0, 0);
    xLCD.noCursor();

    Buzzer::vBegin(BUZZER_PIN);
    xClock.vBegin();

    xDisplayMutex = xSemaphoreCreateMutex();
    
    if(xDisplayMutex == NULL) {
        LOG("SERIAL ERROR: unable to create joystick mutex on setup() function");
        while(true);       
    }

    xTaskCreate(
        vFSM,
        "FSM",
        256,
        NULL,
        2,
        NULL
    );

    xTaskCreate(
        vDisplayRefresh,
        "DisplayRefresh",
        128,
        NULL,
        2,
        NULL
    );
}

void loop() {

}
