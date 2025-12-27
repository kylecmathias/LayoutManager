#include "ColumnLayout.h"

ColumnLayout::ColumnLayout(Adafruit_GFX& tft, int16_t x1, int16_t x2, int16_t y1, int16_t y2, uint8_t rows, TS_Config config) : GridLayout(tft, x1, x2, y1, y2, rows, 1, config) {}

Code ColumnLayout::addComponent(UIComponent* component, uint8_t row, uint16_t tol, const char* label, uint16_t outline, uint16_t fill, uint16_t text, uint8_t textSize) {
    return GridLayout::addComponent(component, row, 0, tol, label, outline, fill, text, textSize);
}

Code ColumnLayout::createComponent(Components component, uint8_t row, uint16_t tol, const char* label, uint16_t outline, uint16_t fill, uint16_t text, uint8_t textSize) {
    return GridLayout::createComponent(component, row, 0, tol, label, outline, fill, text, textSize);
}