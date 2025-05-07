#include <Arduino.h>
#include <Wire.h>
#include <BME280_t.h>          // Lightweight BME280 library
#include <U8g2lib.h>
#include <time.h>          
#include "Display.h"          // Display functions
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h> 
#include "HTML.h"


const char* ssid = "Netzwerk1";
const char* password = "derSchiechtl098";

/*
const char* ssid = "ESP32-AP";
const char* password = "12345678";  
*/
#define Pin_Button1 GPIO_NUM_5
#define Pin_Button2 GPIO_NUM_3
#define Pin_Akku GPIO_NUM_2

#define MYALTITUDE 700

int BoardNum{};

Display myDisplay;        // Display instance             // WiFi instance
BME280<> BMESensor;       // BME280 instance

float temperature, humidity, pressure, relativePressure, altitude{};

AsyncWebServer server(80);

void IRAM_ATTR handleInterrupt() 
{
  BoardNum++;
  if (BoardNum > 4) 
  {
    BoardNum = 0; 
  }
}
void IRAM_ATTR handleInterrupt2() 
{
  BoardNum--;
  if (BoardNum < 0) 
  {
    BoardNum = 3; 
  }
}

void Sensor_Auslesen() 
{
  BMESensor.refresh();

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

}

// ---------------------------------------------------------------------------

void setup() 
{
  pinMode(Pin_Button1, INPUT_PULLUP); // Button pin
  attachInterrupt(digitalPinToInterrupt(Pin_Button1), handleInterrupt, FALLING); // Interrupt on button press

  pinMode(Pin_Button2, INPUT_PULLUP); // Button pin
  attachInterrupt(digitalPinToInterrupt(Pin_Button2), handleInterrupt2, FALLING); // Interrupt on button press

  Serial.begin(115200);
  delay(1000);

  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print the ESP32's IP address
  Serial.print("ESP32 Web Server's IP address: ");
  Serial.println(WiFi.localIP());
  /*  
  // Starte den Access Point
  WiFi.softAP(ssid, password);

  // IP-Adresse des ESP im AP-Modus anzeigen
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(IP);
  */

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request){
    request->send(200, "text/html", index_html);
  });

  server.on("/sensor-data", HTTP_GET, [](AsyncWebServerRequest* request){

    String jsonResponse = "{\"temperature\": " + String(temperature, 1) + 
                          ", \"humidity\": " + String(humidity, 1) + 
                          ", \"pressure\": " + String(pressure, 1) + "}";
    request->send(200, "application/json", jsonResponse);
  });

  server.begin();

  Wire.begin();

  if (!BMESensor.begin()) 
  {
    Serial.println("BME280 failed!");
    while (true);
  }

  myDisplay.Init();  // Initialize display
  Serial.println("BME280 + OLED initialized");
}

void loop() 
{

  switch (BoardNum)
  {
  case 0:
    myDisplay.Display_Werte(temperature, humidity, pressure);
    /* code */
    break;
  case 1:
    myDisplay.TemperatureBoard(temperature);
    break;
  case 2:
    myDisplay.HumidityBoard(humidity);
    break;
  case 3:
    myDisplay.PressureBoard(pressure);
    break;
  default:
    break;
  }
  Sensor_Auslesen();
  delay(100);
}
