
#ifndef _BABYUI_H_
#define _BABYUI_H_

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <MUIU8g2.h>
#include <OneButton.h>
#include "ESPDateTime.h"
#include <ezBuzzer.h>


#define NEXT_PIN 25
#define PREV_PIN 27
#define SELECT_PIN 26

#define BUZZER_PIN 19

#define BAT_GLYPH_OFFSET 0xE236
#define WIFI_GLYPH_OFFSET 0xE217
#define SYMBOL_FONT u8g2_font_waffle_t_all

#define U8G2_FONT u8g2_font_helvR08_te
#define DURATION 100
#define FLUID_UNIT_OFFSET 2

ezBuzzer buzzer(BUZZER_PIN);

extern uint16_t totalChildren;

extern float measurement;

extern time_t elapsedTime;

extern float temperature;

extern uint16_t currentChild;

extern uint8_t batStatus;
extern uint8_t sigalStrength;

extern void addChild(char * firstName, char * lastName, char *  dateOfBirth);

extern void startActivity(uint8_t activityId);

extern void stopActivity(uint8_t activityId);

void activitySaveFailed(uint8_t activityId);

extern void calibrateScale(uint8_t requested);

extern void setActiveChild();

const extern char * childName(uint16_t index);

extern void startConfigPortal();

const extern char * currentfeedingMethodText(uint16_t index); 

extern uint8_t feedingMethodsCount;

uint16_t * currentFeedingMethod;

const extern char * currentfeedingTypeText(uint16_t index);

extern uint8_t feedingTypesCount;

uint16_t * currentFeedingType;

const char * unitsText[4] = {"℃", "℉", "mL","fl oz"};

const char *dateFormats[3] = { 
    "DD/MM/YYYY",
    "MM/DD/YYYY",
    "YYYY/MM/DD" 
};

const char *dateFormatStrings[3] = { 
    "d/m/Y",
    "m/d/Y",
    "Y/m/d" 
};

uint8_t units ;

bool redraw_screen = true;

uint16_t dateFormat;

uint16_t exit_code;

uint16_t yearPart;
uint16_t monthPart;
uint16_t dayPart;

char firstName[42];
char lastName[42];

char dateOfBirth[12];

void setDateOfBirth();

void configPortalEnded();

void initialiseUI();

void updateUI();

void prevButtonClicked();

void nextButtonClicked();

void selectButtonClicked();

void selectButtonHolding();

void beep();

void childAddOutcome(bool success);

bool isLeapYear(uint16_t year);

uint16_t getYear(uint16_t index);

MUIU8G2 muiConfig;

MUIU8G2 muiMain;

MUIU8G2 muiCalibrate;

 mui_t * activeUI;
 mui_t * returnUI;

OneButton selectButton;
OneButton nextButton;
OneButton prevButton;

bool heldBeeped;

void initialiseChildUI(uint16_t index);

bool isChildUIActive();

extern mui_t * getCurrentChildMUI(uint16_t index = 255);


#include "BabyUI.cpp"

#endif