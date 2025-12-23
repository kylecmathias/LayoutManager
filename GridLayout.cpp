#include "GridLayout.h"

GridLayout::GridLayout(Adafruit_GFX& tft, int16_t x1, int16_t x2, int16_t y1, int16_t y2, uint8_t rows, uint8_t cols, TS_Config config) : Layout(tft, x1, x2, y1, y2, config), cols(min(cols, MAX_COLS)), rows(min(rows, MAX_ROWS)) {
    cellW = layoutW / this->cols;
    cellH = layoutH / this->rows;
    memset(owned, 0, sizeof(owned));
    memset(callbacks, 0, sizeof(callbacks)); 
}

GridLayout::~GridLayout() {
    deleteAll();
}

void GridLayout::setBg(uint16_t color) {
    tft.fillRect(startX, startY, layoutW, layoutH, color); 
}

void GridLayout::drawAll() {
    for (uint8_t r = 0; r < rows; r++) {
        for (uint8_t c = 0; c < cols; c++) {
            if (grid[r][c]) {
                grid[r][c]->drawButton(false);
            }
        }
    }
}

Code GridLayout::addButton(Adafruit_GFX_Button* button, uint8_t row, uint8_t col, uint16_t tol, const char* label = "", uint16_t outline = WHITE, uint16_t fill = BLUE, uint16_t text = BLACK, uint8_t textSize = 2) {
    if (row >= rows || col >= cols) return FAIL;
    if (grid[row][col] != nullptr) return FAIL;

    tol = static_cast<uint16_t>(min(tol, min(cellW, cellH) / 2));

    int16_t x = startX + (col * cellW) + tol;
    int16_t y = startY + (row * cellH) + tol;

    if (x >= tft.width() || y >= tft.height()) return FAIL;

    int16_t b_w = cellW - 2 * tol;
    int16_t b_h = cellH - 2 * tol;

    if (b_w <= 0 || b_h <= 0) {
        return FAIL;
    }

    grid[row][col] = button;
    owned[row][col] = false;

    button->initButtonUL(&tft, x, y, b_w, b_h, outline, fill, text, (char*)label, textSize);
    grid[row][col]->drawButton(false);

    return SUCCESS;
}

Code GridLayout::createButton(uint8_t row, uint8_t col, uint16_t tol, const char* label = "", uint16_t outline = WHITE, uint16_t fill = BLUE, uint16_t text = BLACK, uint8_t textSize = 2) {
    if (row >= rows || col >= cols) return FAIL;
    if (grid[row][col] != nullptr) return FAIL;

    tol = static_cast<uint16_t>(min(tol, min(cellW, cellH) / 2));

    int16_t x = startX + (col * cellW) + tol;
    int16_t y = startY + (row * cellH) + tol;

    int16_t b_w = cellW - 2 * tol;
    int16_t b_h = cellH - 2 * tol;

    if (b_w <= 0 || b_h <= 0) return FAIL;

    grid[row][col] = new Adafruit_GFX_Button();
    if (!grid[row][col]) return FAIL;
    owned[row][col] = true;

    grid[row][col]->initButtonUL(&tft, x, y, b_w, b_h, outline, fill, text, (char*)label, textSize);
    grid[row][col]->drawButton(false);

    return SUCCESS;
}

void GridLayout::setCallback(uint8_t row, uint8_t col, ButtonCallback cb) {
    if (row >= rows || col >= cols) return;
    callbacks[row][col] = cb; 
}

void GridLayout::clearCallback(uint8_t row, uint8_t col) {
    if (row >= rows || col >= cols) return;
    callbacks[row][col] = nullptr; 
}

Dimensions GridLayout::getCellDimensions(uint8_t row, uint8_t col, uint16_t tol) {
    Dimensions dims = {0, 0, 0, 0};
    
    if (row >= rows || col >= cols) return dims;

    int16_t xmin = startX + (col * cellW);
    int16_t ymin = startY + (row * cellH);

    dims.x1 = xmin + tol;
    dims.y1 = ymin + tol;
    dims.x2 = xmin + cellW - tol;
    dims.y2 = ymin + cellH - tol;

    return dims;
}

Adafruit_GFX_Button* GridLayout::getButton(uint8_t row, uint8_t col) {
    if (row >= rows || col >= cols) return nullptr;
    return grid[row][col];
}

Adafruit_GFX_Button* GridLayout::getPressed(TSPoint p, int8_t* outRow, int8_t* outCol) {
    if (p.z < config.MINPRESSURE || p.z > config.MAXPRESSURE) return nullptr;

    int16_t x = map(p.x, config.TS_LEFT, config.TS_RT, 0, tft.width());
    int16_t y = map(p.y, config.TS_TOP, config.TS_BOT, 0, tft.height());

    if (x < startX || x > startX + layoutW || 
        y < startY || y > startY + layoutH) {
        return nullptr; 
    }

    Adafruit_GFX_Button* pressedBtn = nullptr;

    for (uint8_t r = 0; r < rows; r++) {
        for (uint8_t c = 0; c < cols; c++) {
            Adafruit_GFX_Button* btn = grid[r][c];
            if (!btn) continue;

            btn->press(btn->contains(x, y));

            if (btn->justPressed()) {
                btn->drawButton(true);
                pressedBtn = btn;
                if (outRow) *outRow = r;
                if (outCol) *outCol = c;
            }
            if (btn->justReleased()) {
                btn->drawButton(false);
            }
        }
    }

    return pressedBtn;
}

Adafruit_GFX_Button* GridLayout::getPressed(TSPoint p) {
    if (p.z < config.MINPRESSURE || p.z > config.MAXPRESSURE) return nullptr;

    int16_t x = map(p.x, config.TS_LEFT, config.TS_RT, 0, tft.width());
    int16_t y = map(p.y, config.TS_TOP, config.TS_BOT, 0, tft.height());

    Adafruit_GFX_Button* pressedBtn = nullptr;

    for (uint8_t r = 0; r < rows; r++) {
        for (uint8_t c = 0; c < cols; c++) {
            Adafruit_GFX_Button* btn = grid[r][c];
            if (!btn) continue;

            btn->press(btn->contains(x, y));

            if (btn->justPressed()) {
                btn->drawButton(true);
                pressedBtn = btn;
                if (callbacks[r][c]) callbacks[r][c](r, c);
            }
            if (btn->justReleased()) {
                btn->drawButton(false);
            }
        }
    }

    return pressedBtn;
}

Code GridLayout::deleteButton(uint8_t row, uint8_t col) {
    if (row >= rows || col >= cols) return FAIL;
    if (grid[row][col] == nullptr) return FAIL;

    if (owned[row][col]) {
        delete grid[row][col];
    }

    int16_t x = startX + (col * cellW);
    int16_t y = startY + (row * cellH);

    tft.fillRect(x, y, cellW, cellH, bg);
    
    callbacks[row][col] = nullptr;
    grid[row][col] = nullptr;
    owned[row][col] = false;
    return SUCCESS;
}

Code GridLayout::deleteAll() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            deleteButton(r, c);
        }
    }
    return SUCCESS;
}
