#ifndef TAB_LAYOUT_H
#define TAB_LAYOUT_H

#include "touchscreen_config.h"
#include "GridLayout.h"
#include "RowLayout.h"
#include "ColumnLayout.h"

enum class TabContentLayout {
    GRID = 0,
    ROW = 1,
    COLUMN = 2
};

class TabLayout : public Layout {
private:
    Layout* tabs[MAX_TABS] = {}; 
    Adafruit_GFX_Button* buttons[MAX_TABS] = {}; 
    char* labels[MAX_TABS] = {};
    bool owned[MAX_TABS] = {};
    uint8_t tabCount = 0;
    uint8_t currentTab = 0;
    int16_t tabW, tabH;

    int16_t tabBarHeight;
    int16_t contentY1;
public:
    TabLayout(Adafruit_GFX& tft, int16_t x1, int16_t x2, int16_t y1, int16_t y2, TS_Config config);
    ~TabLayout();
    Code addTab(Layout* tab, Adafruit_GFX_Button* button, const char* label, uint16_t outline = WHITE, uint16_t fill = BLACK, uint16_t text = WHITE, uint8_t textSize = 2);
    Code createTab(TabContentLayout layout, const char* label, uint16_t outline = WHITE, uint16_t fill = BLACK, uint16_t text = WHITE, uint8_t textSize = 2);
    void drawAll() override;

    Dimensions getTabDimensions(uint16_t tol);

    Adafruit_GFX_Button* getPressed(TSPoint p) override;

    Code deleteTab(uint8_t index);
    Code deleteAllTabs();
};

#endif