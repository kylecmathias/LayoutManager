# LayoutManager
A modular, object-oriented UI framework for Arduino and MCUFRIEND_kbv compatible TFT screens. This library simplifies touch interface design by providing automated grid coordinate mapping, touch event routing, and memory management.

## Features
- Hierarchical Layouts: Nest Grids inside Tabs, or Rows inside Grids.

- Automated Mapping: No more manual map() calls for every button.

- Memory Management: Smart tracking of "owned" objects to prevent memory leaks on microcontrollers.

- Touch Resilience: Built-in pin restoration for MCUFRIEND-style shared touch/data pins.

## Configuration
Before initializing your layout, you must define your hardware specifics using the TS_Config struct. This ensures the library is compatible with any screen size or calibration.
```
TS_Config myConfig = {
    8, A2, A3, 9,          // XP, XM, YP, YM (Pins)
    924, 128, 80, 901,     // TS_LEFT, TS_RT, TS_TOP, TS_BOT (Calibration)
    200, 1000              // MINPRESSURE, MAXPRESSURE
};
```
## Supported Layouts
### GridLayout
Divides the defined screen area into a matrix of equal-sized cells.

Usage: Keypads, dashboards, or uniform menus.

Example: ```GridLayout keypad(tft, x1, x2, y1, y2, 4, 3, myConfig);```

### RowLayout / ColumnLayout
Specialized 1D versions of the grid.

RowLayout: Forces 1 row; user only specifies columns. Perfect for headers/footers.

ColumnLayout: Forces 1 column; user only specifies rows. Ideal for sidebars or settings lists.

### TabLayout
A container that manages multiple "pages" of layouts.

Automatic Tab Bar: Reserves the top 10% of space for navigation buttons.

Dynamic Resizing: Tab buttons automatically resize their width as you add more tabs.