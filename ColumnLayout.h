#ifndef COLUMN_LAYOUT_H
#define COLUMN_LAYOUT_H

#include "touchscreen_config.h"
#include "GridLayout.h"

class ColumnLayout : public GridLayout {
public:
    ColumnLayout(Adafruit_GFX& tft, int16_t x1, int16_t x2, int16_t y1, int16_t y2, uint8_t rows, TS_Config config);
    Code addButton(Adafruit_GFX_Button* button, uint8_t row, uint16_t tol, const char* label = "", uint16_t outline = WHITE, uint16_t fill = BLUE, uint16_t text = BLACK, uint8_t textSize = 2);
    Code createButton(uint8_t row, uint16_t tol, const char* label = "", uint16_t outline = WHITE, uint16_t fill = BLUE, uint16_t text = BLACK, uint8_t textSize = 2);
};

#endif