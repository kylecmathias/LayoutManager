#ifndef GRID_LAYOUT_H
#define GRID_LAYOUT_H

#include "touchscreen_config.h"
#include "Layout.h"

using ButtonCallback = void(*)(uint8_t row, uint8_t col);

class GridLayout : public Layout {
private:
    Adafruit_GFX_Button* grid[MAX_ROWS][MAX_COLS] = {}; 
    bool owned[MAX_ROWS][MAX_COLS] = {};
    ButtonCallback callbacks[MAX_ROWS][MAX_COLS] = {};
    uint8_t rows = 0;
    uint8_t cols = 0;
    int16_t cellW, cellH;

public:
    GridLayout(Adafruit_GFX& tft, int16_t x1, int16_t x2, int16_t y1, int16_t y2, uint8_t rows, uint8_t cols, TS_Config config);
    virtual ~GridLayout();

    //setup functions

    //sets default background color
    void setBg(uint16_t color) override;
    //sets buttons in all cells
    void drawAll() override;
    //adds existing button to layout (external ownership)
    Code addButton(Adafruit_GFX_Button* button, uint8_t row, uint8_t col, uint16_t tol, const char* label = "", uint16_t outline = WHITE, uint16_t fill = BLUE, uint16_t text = BLACK, uint8_t textSize = 2);
    //creates a button owned by the layout
    Code createButton(uint8_t row, uint8_t col, uint16_t tol, const char* label = "", uint16_t outline = WHITE, uint16_t fill = BLUE, uint16_t text = BLACK, uint8_t textSize = 2);
    //adds callback function for button press
    void setCallback(uint8_t row, uint8_t col, ButtonCallback cb);
    //clears callback
    void clearCallback(uint8_t row, uint8_t col);
    //gets cell dimensions as a struct
    Dimensions getCellDimensions(uint8_t row, uint8_t col, uint16_t tol);


    //usage functions

    //gets the reference to the button at row and column
    Adafruit_GFX_Button* getButton(uint8_t row, uint8_t col);
    Adafruit_GFX_Button* getPressed(TSPoint p, int8_t* outRow, int8_t* outCol);
    Adafruit_GFX_Button* getPressed(TSPoint p) override;


    //cleanup methods

    //delete button at row and column
    Code deleteButton(uint8_t row, uint8_t col);
    //delete all buttons
    Code deleteAll();
};

#endif