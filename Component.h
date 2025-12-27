#ifndef COMPONENT_H
#define COMPONENT_H

#include "touchscreen_config.h"

enum class Components {
    TOUCHBUTTON,
    LABEL
};

class UIComponent {
protected:
    int16_t x, y, w, h; 
    Adafruit_GFX* tft;
    Components type;
public:
    virtual ~UIComponent() {}
    
    virtual void setBounds(int16_t x, int16_t y, int16_t w, int16_t h) {
        this->x = x; this->y = y; this->w = w; this->h = h;
    }

    virtual void draw() = 0; 
    virtual bool handleTouch(int16_t tx, int16_t ty) = 0;

    Components getComponentType() {
        return type;
    }
};

class TouchButton : public Adafruit_GFX_Button, public UIComponent {
private:
    uint16_t outline, fill, textColor;
    uint8_t textsize;
    const char* text;
public:
    TouchButton(Adafruit_GFX* display, uint16_t outline = WHITE, uint16_t fill = BLUE, uint16_t textColor = WHITE, const char* label = "", uint8_t textsize = 2) {
        this->tft = display;
        this->outline = outline;
        this->fill = fill;
        this->textColor = textColor;
        this->textsize = textsize;
        this->text = label;
        this->type = Components::TOUCHBUTTON;
    }

    void setBounds(int16_t x, int16_t y, int16_t w, int16_t h) override {
        UIComponent::setBounds(x, y, w, h); 
        this->initButtonUL(tft, x, y, w, h, outline, fill, textColor, (char*)text, textsize);
    }

    void draw() override {
        this->drawButton(false); 
    }

    bool handleTouch(int16_t tx, int16_t ty) override {
        return this->contains(tx, ty);
    }
};

class Label : public UIComponent {
private:
    const char* text;
    uint16_t color;
    uint8_t textSize;
public:
    Label(Adafruit_GFX* tft, const char* text, uint16_t color, uint8_t textSize) {
        this->tft = tft; 
        this->text = text; 
        this->color = color;
        this->textSize = textSize;
        this->type = Components::LABEL;
    }

    void draw() override {
        tft->setTextSize(textSize);
        tft->setTextColor(color);
        tft->setCursor(x + (w/4), y + (h/2)); 
        tft->print(text);
    }

    bool handleTouch(int16_t tx, int16_t ty) override {
        return false; 
    }
};

#endif