#include <Arduino.h>
#include <Display.h>         // Display functions
#include <U8g2lib.h>         // OLED library

// OLED Display: 128x64 SSD1306 I2C
Display::Display()
    : u8g2(U8G2_R0, U8X8_PIN_NONE) { // I2C ohne Reset
}

// ---------------------------------------------------------------------------
// -- Draw custom minimalist icons --
void Display::drawTempIcon(int x, int y) 
{
  u8g2.drawCircle(x, y, 3);         // bulb
  u8g2.drawVLine(x, y - 8, 5);      // stem
}

void Display::drawHumidityIcon(int x, int y) 
{
  u8g2.drawTriangle(x, y - 3, x - 4, y + 5, x + 4, y + 5); // water droplet shape
}

void Display::drawPressureIcon(int x, int y) 
{
  u8g2.drawFrame(x - 3, y - 3, 6, 6); // box
  u8g2.drawHLine(x - 2, y, 4);        // horizontal bar
}

// ---------------------------------------------------------------------------
void Display::Init() 
{
    u8g2.begin();
    u8g2.enableUTF8Print();
}


// ---------------------------------------------------------------------------
// Dashboard display
void Display::Display_Werte(float temperature, float humidity, float pressure) 
{
  u8g2.clearBuffer();

  // Header
  u8g2.setFont(u8g2_font_7x14B_tf);  // Bold clean title font
  u8g2.setCursor(25, 13);
  u8g2.print("Dashboard");

  u8g2.drawHLine(0, 15, 128);  // Divider line

  // Set readable larger font for values
  u8g2.setFont(u8g2_font_5x8_tf);

  int lineY = 30;
  int lineSpacing = 15;

  // Temperature
  drawTempIcon(5, lineY - 4);
  u8g2.setCursor(20, lineY);
  u8g2.printf("Temp:     %.1f C", temperature);
  lineY += lineSpacing;

  // Humidity
  drawHumidityIcon(6, lineY - 5);
  u8g2.setCursor(20, lineY);
  u8g2.printf("Humidity: %.1f %%", humidity);
  lineY += lineSpacing;

  // Pressure
  drawPressureIcon(6, lineY - 5);
  u8g2.setCursor(20, lineY);
  u8g2.printf("Pressure: %.1f hPa", pressure);

  u8g2.sendBuffer();
}
void Display::TemperatureBoard(float temperature) 
{
  u8g2.clearBuffer();

  // Titelbereich (Box mit invertierter Schrift)
  u8g2.drawBox(0, 0, 128, 16);
  u8g2.setDrawColor(0);  // Invertiert schreiben
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.setCursor(4, 12);
  u8g2.print("TEMPERATUR");
  u8g2.setDrawColor(1);  // Normal weiterschreiben

  // Horizontaler Trennstrich
  u8g2.drawHLine(0, 18, 128);

  // Großer Wert mittig
  char tempStr[16];
  sprintf(tempStr, "%.1f C", temperature);
  u8g2.setFont(u8g2_font_logisoso34_tf);
  int textWidth = u8g2.getStrWidth(tempStr);
  u8g2.setCursor((128 - textWidth) / 2, 58);
  u8g2.print(tempStr);

  // Kleine Deko-Ecken
  u8g2.drawTriangle(0, 64, 0, 60, 4, 64);
  u8g2.drawTriangle(127, 64, 127, 60, 123, 64);

  u8g2.sendBuffer();
}

void Display::HumidityBoard(float humidity) 
{
  u8g2.clearBuffer();

  u8g2.drawBox(0, 0, 128, 16);
  u8g2.setDrawColor(0);
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.setCursor(4, 12);
  u8g2.print("LUFTFEUCHTIGKEIT");
  u8g2.setDrawColor(1);

  u8g2.drawHLine(0, 18, 128);

  char humStr[16];
  sprintf(humStr, "%.1f %%", humidity);
  u8g2.setFont(u8g2_font_logisoso32_tf);
  int textWidth = u8g2.getStrWidth(humStr);
  u8g2.setCursor((128 - textWidth) / 2, 58);
  u8g2.print(humStr);

  u8g2.drawDisc(4, 64, 2);
  u8g2.drawDisc(124, 64, 2);

  u8g2.sendBuffer();
}

void Display::PressureBoard(float pressure) 
{
  u8g2.clearBuffer();

  // Titel oben
  u8g2.drawBox(0, 0, 128, 16);
  u8g2.setDrawColor(0);
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.setCursor(4, 12);
  u8g2.print("LUFTDRUCK");
  u8g2.setDrawColor(1);

  u8g2.drawHLine(0, 18, 128);

  // Druckwert in kleiner, aber klarer Schrift
  char pressStr[16];
  sprintf(pressStr, "%.1f hPa", pressure);
  u8g2.setFont(u8g2_font_9x15_tf);
  int textWidth = u8g2.getStrWidth(pressStr);
  u8g2.setCursor((128 - textWidth) / 2, 50);
  u8g2.print(pressStr);

  // Dezente Linie unten
  u8g2.drawLine(0, 63, 128, 63);

  u8g2.sendBuffer();
}

void Display::ClockBoard(const char* timeStr) 
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawStr(10, 30, timeStr);
  u8g2.sendBuffer();
}



