#ifndef TOUCHSCREEN_CONFIG_H
#define TOUCHSCREEN_CONFIG_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

#define WHITE 0xFFFF
#define RED   0xF800
#define BLUE  0x001F
#define GREEN 0x07E0
#define BLACK 0x0000

#define MAX_ROWS 8
#define MAX_COLS 8
#define MAX_TABS 8

enum class Code {
    S = 0,
    F = 1
};

#define SUCCESS Code::S
#define FAIL Code::F

typedef struct {
    uint16_t x1, x2, y1, y2;
} Dimensions; 

typedef struct {
    int XP, XM, YP, YM;
    int TS_LEFT, TS_RT, TS_TOP, TS_BOT;
    int MINPRESSURE, MAXPRESSURE;
} TS_Config;

#endif
