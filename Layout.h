#ifndef LAYOUT_H
#define LAYOUT_H

#include "touchscreen_config.h"

class Layout {
protected:
    Adafruit_GFX& tft;
    int16_t startX, startY, layoutW, layoutH;
    uint16_t bg = BLACK; 
    TS_Config config;
public:
    Layout(Adafruit_GFX& tft, int16_t x1, int16_t x2, int16_t y1, int16_t y2, TS_Config config) : tft(tft), startX(x1), startY(y1), layoutW(x2-x1), layoutH(y2-y1), config(config) {}

    virtual ~Layout() {}

    virtual void drawAll() = 0;
    virtual Adafruit_GFX_Button* getPressed(TSPoint p) = 0;

    // This can be shared by everyone
    virtual void setBg(uint16_t color) {
        tft.fillRect(startX, startY, layoutW, layoutH, color);
    }

    //function to restore display pins after touchscreen reading
    void restoreDisplayPins() {
        pinMode(config.XM, OUTPUT);
        pinMode(config.YP, OUTPUT);
        digitalWrite(config.XM, HIGH);
        digitalWrite(config.YP, HIGH);
    }

    //function to check where the screen has been touched
    void touchInput(TouchScreen& ts) {
        TSPoint point = ts.getPoint();
        restoreDisplayPins();
        this->getPressed(point);
    }
};

#endif