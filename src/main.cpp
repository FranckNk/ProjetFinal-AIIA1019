#include <Arduino.h>
#include <Adafruit_AHTX0.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ledRGB.hpp"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Adafruit_AHTX0 aht;
LedRGB led(33, 25, 26);
int pinAmbiant = 32;
int pos = 0;

void setup() {
  Serial.begin(9600);
  //while (!Serial);
  pinMode(pinAmbiant, INPUT);
  Serial.println("128x64 OLED FeatherWing test");
  display.begin(0x3C, true); // Address 0x3C default

  Serial.println("OLED begun");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Hello, world!");
  display.display(); 

  if (aht.begin()) {
    Serial.println("Found AHT20");
  } else {
    Serial.println("Didn't find AHT20");
  }  
  delay(1000);
}

void loop() {
  display.clearDisplay();
  sensors_event_t humidity, temp;
  int val = analogRead(pinAmbiant);
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data

    //Formule trouvée par modélisation
  double ProbablityRain=0.03743225*humidity.relative_humidity - 2.21389335*temp.temperature - 0.09773871*val +198.85713381876218;

  display.setTextSize(1.5);
  display.setCursor(0,01);
  display.print("TEMP(C)");
  display.setCursor(50,01);
  display.print("HUM(%)");
  display.setCursor(100,01);
  display.print("LUM");
  display.setTextSize(1);
  display.setCursor(0,16);
  display.print(temp.temperature);;
  display.setCursor(50,16);
  display.print(humidity.relative_humidity);
  display.setCursor(100,16);
  display.println(val);
  display.setTextSize(2);
  display.setCursor(0,34);
  display.print("PROB RAIN:");
  display.setTextSize(1);
  display.setCursor(pos,56);
  display.print(ProbablityRain);
  if(ProbablityRain < 0)
    led.setGreen();
  else if(ProbablityRain < 100)
    led.setPurple();
  else
    led.setRed();

  /*
  display.print(humidity.relative_humidity); display.println(" %");
  Serial.print("Temperature: ");Serial.print(temp.temperature);Serial.println(" degrees C");
  Serial.print("Pressure: ");Serial.println(" RH %");
  */
  display.display();
  delay(50);
  pos = (pos > 100) ? 0 : pos + 1;

}