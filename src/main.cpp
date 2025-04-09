#include <Wire.h>
#include <BME280_t.h>         // Lightweight BME280 library
#include <U8g2lib.h>          // OLED library

#define I2C_SDA 4             // ESP32-C3: SDA pin
#define I2C_SCL 5             // ESP32-C3: SCL pin
#define MYALTITUDE 700        // Altitude in meters for relative pressure

// Sensor instance
BME280<> BMESensor;

// OLED Display: 128x64 SSD1306 I2C
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2
(
  U8G2_R0, /* reset=*/ U8X8_PIN_NONE
);

float temperature, humidity, pressure, relativePressure, altitude;

// -- Draw custom minimalist icons --
void drawTempIcon(int x, int y) 
{
  u8g2.drawCircle(x, y, 3);         // bulb
  u8g2.drawVLine(x, y - 8, 5);      // stem
}

void drawHumidityIcon(int x, int y) 
{
  u8g2.drawTriangle(x, y - 3, x - 4, y + 5, x + 4, y + 5); // water droplet shape
}

void drawPressureIcon(int x, int y) 
{
  u8g2.drawFrame(x - 3, y - 3, 6, 6); // box
  u8g2.drawHLine(x - 2, y, 4);        // horizontal bar
}



void Display_Werte() 
{
  u8g2.clearBuffer();

  // Header
  u8g2.setFont(u8g2_font_7x14B_tf);  // Bold clean title font
  u8g2.setCursor(24, 13);
  u8g2.print("BME280 Dashboard");

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


void Sensor_Auslesen() 
{
  temperature = BMESensor.temperature;
  humidity = BMESensor.humidity;
  pressure = BMESensor.pressure / 100.0F; // Pa → hPa
  relativePressure = BMESensor.seaLevelForAltitude(MYALTITUDE);
  altitude = BMESensor.pressureToAltitude(relativePressure);

  // Serial debug output
  Serial.printf("Temperature: %.1f °C\n", temperature);
  Serial.printf("Humidity:    %.1f %%\n", humidity);
  Serial.printf("Pressure:    %.1f hPa\n", pressure);
  Serial.printf("RelPress:    %.1f hPa\n", relativePressure / 100.0F);
  Serial.printf("Altitude:    %.1f m\n", altitude);
  Serial.println("----------------------");

  Display_Werte();  // Show updated info on display
}

void setup() 
{
  Serial.begin(115200);
  delay(1000);

  Wire.begin();

  if (!BMESensor.begin()) {
    Serial.println("BME280 failed!");
    while (true);
  }

  u8g2.begin();
  u8g2.enableUTF8Print();

  Serial.println("BME280 + OLED initialized");
}

void loop() {
  BMESensor.refresh();     // Update sensor data
  Sensor_Auslesen();       // Print and display
  delay(2000);             // Delay between reads
}
