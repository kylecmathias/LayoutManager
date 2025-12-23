#include "TabLayout.h"

TabLayout::TabLayout(Adafruit_GFX& tft, int16_t x1, int16_t x2, int16_t y1, int16_t y2, TS_Config config) : Layout(tft, x1, x2, y1, y2, config) {
    tabW = layoutW;
    tabH = layoutH * 0.1;

    memset(tabs, 0, sizeof(tabs));
    memset(buttons, 0, sizeof(buttons));
    memset(labels, 0, sizeof(labels));
    memset(owned, 0, sizeof(owned));
    
    tabBarHeight = layoutH / 10;
    contentY1 = startY + tabBarHeight;
}

TabLayout::~TabLayout() {
    deleteAllTabs();
}

Code TabLayout::addTab(Layout* tab, Adafruit_GFX_Button* button, const char* label, uint16_t outline, uint16_t fill, uint16_t text, uint8_t textSize) {
    if (tabCount >= MAX_TABS || !tab || !button) return FAIL;

    tabs[tabCount] = tab;
    buttons[tabCount] = button;
    labels[tabCount] = (char*)label; 
    owned[tabCount] = false;
    tabCount++; 

    int16_t btnW = tabW / tabCount; 

    for (uint8_t i = 0; i < tabCount; i++) {
        int16_t btnX = startX + (i * btnW);
        buttons[i]->initButtonUL(&tft, btnX, startY, btnW, tabBarHeight, outline, fill, text, (char*)labels[i], textSize);
        buttons[i]->drawButton(i == currentTab);
    }
    return SUCCESS;
}

Code TabLayout::createTab(TabContentLayout layout, const char* label, uint16_t outline, uint16_t fill, uint16_t text, uint8_t textSize) {
    if (tabCount >= MAX_TABS) return FAIL;

    switch (layout) {
        case TabContentLayout::GRID:
            tabs[tabCount] = new GridLayout(tft, startX, startX + layoutW, contentY1, startY + layoutH, 2, 2, config);
            break;
        case TabContentLayout::ROW:
            tabs[tabCount] = new RowLayout(tft, startX, startX + layoutW, contentY1, startY + layoutH, 2, config);
            break;
        case TabContentLayout::COLUMN:
            tabs[tabCount] = new ColumnLayout(tft, startX, startX + layoutW, contentY1, startY + layoutH, 2, config);
            break;
        default:
            return FAIL;
    }

    Adafruit_GFX_Button* button = new Adafruit_GFX_Button();
    if (!button) {
        delete tabs[tabCount];
        tabs[tabCount] = nullptr;
        return FAIL;
    }

    buttons[tabCount] = button;
    labels[tabCount] = (char*)label; 
    owned[tabCount] = true;
    tabCount++;

    int16_t btnW = tabW / tabCount; 
    for (uint8_t i = 0; i < tabCount; i++) {
        int16_t btnX = startX + (i * btnW);
        buttons[i]->initButtonUL(&tft, btnX, startY, btnW, tabBarHeight, outline, fill, text, (char*)labels[i], textSize);
        buttons[i]->drawButton(i == currentTab);
    }

    return SUCCESS;
}

void TabLayout::drawAll() {
    for (uint8_t i = 0; i < tabCount; i++) {
        if (buttons[i]) {
            buttons[i]->drawButton(i == currentTab); 
        }
    }

    if (tabs[currentTab]) {
        tabs[currentTab]->drawAll();
    }
}

Dimensions TabLayout::getTabDimensions(uint16_t tol) {
    Dimensions dims;
    dims.x1 = startX + tol;
    dims.y1 = contentY1 + tol; 
    dims.x2 = startX + layoutW - tol;
    dims.y2 = startY + layoutH - tol;
    return dims;
}

Adafruit_GFX_Button* TabLayout::getPressed(TSPoint p) {
    if (p.z < config.MINPRESSURE || p.z > config.MAXPRESSURE) return nullptr;

    int16_t x = map(p.x, config.TS_LEFT, config.TS_RT, 0, tft.width());
    int16_t y = map(p.y, config.TS_TOP, config.TS_BOT, 0, tft.height());

    if (y >= startY && y < contentY1) {
        for (uint8_t i = 0; i < tabCount; i++) {
            if (buttons[i]->contains(x, y)) {
                if (i != currentTab) {
                    currentTab = i;
                    setBg(bg); 
                    drawAll();  
                }
                return buttons[i];
            }
        }
    }

    if (tabs[currentTab]) {
        return tabs[currentTab]->getPressed(p);
    }

    return nullptr;
}

Code TabLayout::deleteTab(uint8_t index) {
    if (index >= tabCount) return FAIL;

    if (owned[index] && tabs[index]) {
        delete tabs[index];
        tabs[index] = nullptr;
    }
    if (buttons[index]) {
        delete buttons[index];
        buttons[index] = nullptr;
    }
    labels[index] = nullptr;
    owned[index] = false;

    for (uint8_t i = index; i < tabCount - 1; i++) {
        tabs[i] = tabs[i + 1];
        buttons[i] = buttons[i + 1];
        labels[i] = labels[i + 1];
        owned[i] = owned[i + 1];
    }
    tabs[tabCount - 1] = nullptr;
    buttons[tabCount - 1] = nullptr;
    labels[tabCount - 1] = nullptr;
    owned[tabCount - 1] = false;
    tabCount--;

    if (currentTab >= tabCount && tabCount > 0) {
        currentTab = tabCount - 1;
    }

    int16_t btnW = tabW / (tabCount > 0 ? tabCount : 1); 
    for (uint8_t i = 0; i < tabCount; i++) {
        int16_t btnX = startX + (i * btnW);
        buttons[i]->initButtonUL(&tft, btnX, startY, btnW, tabBarHeight, WHITE, BLACK, WHITE, (char*)labels[i], 2);
        buttons[i]->drawButton(i == currentTab);
    }

    setBg(bg); 
    drawAll();  

    return SUCCESS;
}

Code TabLayout::deleteAllTabs() {
    for (uint8_t i = 0; i < tabCount; i++) {
        if (owned[i] && tabs[i]) {
            delete tabs[i];
            tabs[i] = nullptr;
        }
        if (buttons[i]) {
            delete buttons[i];
            buttons[i] = nullptr;
        }
        labels[i] = nullptr;
        owned[i] = false;
    }
    tabCount = 0;
    currentTab = 0;
    return SUCCESS;
}