#include "RowLayout.h"

RowLayout::RowLayout(Adafruit_GFX& tft, int16_t x1, int16_t x2, int16_t y1, int16_t y2, uint8_t cols, TS_Config config) : GridLayout(tft, x1, x2, y1, y2, 1, cols, config) {}

Code RowLayout::addComponent(UIComponent* component, uint8_t col, uint16_t tol, const char* label, uint16_t outline, uint16_t fill, uint16_t text, uint8_t textSize) {
    return GridLayout::addComponent(component, 0, col, tol, label, outline, fill, text, textSize);
}

Code RowLayout::createComponent(Components component, uint8_t col, uint16_t tol, const char* label, uint16_t outline, uint16_t fill, uint16_t text, uint8_t textSize) {
    return GridLayout::createComponent(component, 0, col, tol, label, outline, fill, text, textSize);
}