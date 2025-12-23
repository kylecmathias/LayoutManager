#include "RowLayout.h"

RowLayout::RowLayout(Adafruit_GFX& tft, int16_t x1, int16_t x2, int16_t y1, int16_t y2, uint8_t cols, TS_Config config) : GridLayout(tft, x1, x2, y1, y2, 1, cols, config) {}

Code RowLayout::addButton(Adafruit_GFX_Button* button, uint8_t col, uint16_t tol, const char* label, uint16_t outline, uint16_t fill, uint16_t text, uint8_t textSize) {
    return GridLayout::addButton(button, 0, col, tol, label, outline, fill, text, textSize);
}

Code RowLayout::createButton(uint8_t col, uint16_t tol, const char* label, uint16_t outline, uint16_t fill, uint16_t text, uint8_t textSize) {
    return GridLayout::createButton(0, col, tol, label, outline, fill, text, textSize);
}