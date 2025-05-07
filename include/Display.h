#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <U8g2lib.h> 

class Display
{
    public:
        Display();
        void Init();
        void Display_Werte(float temperature, float humidity, float pressure);
        void setDimmed(bool dimm);
        void TemperatureBoard(float temperature);
        void HumidityBoard(float humidity);
        void PressureBoard(float pressure);
        void ClockBoard(const char* timeStr);
    
    private:
        U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

        void drawTempIcon(int x, int y);
        void drawHumidityIcon(int x, int y);
        void drawPressureIcon(int x, int y);
};

#endif