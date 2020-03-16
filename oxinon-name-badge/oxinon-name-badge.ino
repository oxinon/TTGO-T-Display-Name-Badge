#include <Wire.h>
#include <WiFi.h>
#include <Button2.h>


#include "twitter.h"
#define twitter twitter

#include "oxinon.h"
#define oxinon oxinon

#include "oxinondev.h"
#define oxinondev oxinondev

#include "github.h"
#define github github


#include <TFT_eSPI.h>
#include <SPI.h>

#ifndef TFT_DISPOFF
#define TFT_DISPOFF 0x28
#endif

#ifndef TFT_SLPIN
#define TFT_SLPIN   0x10
#endif

#define TFT_MOSI        19
#define TFT_SCLK        18
#define TFT_CS          5
#define TFT_DC          16
#define TFT_RST         23

#define TFT_BL          4  // Display backlight control pin
#define ADC_EN          14
#define ADC_PIN         34

#define BUTTON_A_PIN  35
#define BUTTON_B_PIN  0

Button2 buttonA = Button2(BUTTON_A_PIN);
Button2 buttonB = Button2(BUTTON_B_PIN);

TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library


int btnCick = false;

// supportet colors
#define ST7735_BLACK   0x0000
#define ST7735_GRAY    0x8410
#define ST7735_WHITE   0xFFFF
#define ST7735_RED     0xF800
#define ST7735_ORANGE  0xEC80
#define ST7735_YELLOW  0xFFE0  
#define ST7735_LIME    0x07FF
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_AQUA    0x04FF
#define ST7735_BLUE    0x001F
#define ST7735_MAGENTA 0xF81F
#define ST7735_PINK    0xF8FF
#define ST7735_CHYN    0x2D05

#include "esp_adc_cal.h"

int vref = 1100;


void setup(){

  Serial.begin(115200);
  delay(50);
  Serial.println("\n\nName Batch");

  
  uint16_t time = millis();
  tft.setRotation(3);  //rotate 90 degree
  tft.fillScreen(ST7735_BLACK);
  time = millis() - time;
  
        if (TFT_BL > 0) {
        pinMode(TFT_BL, OUTPUT);
        digitalWrite(TFT_BL, HIGH);
        tft.init();  
        tft.fillScreen(ST7735_BLACK);
        tft.setSwapBytes(true);
        //tft.pushImage(0, 0, iotaiotWidth, iotaiotHeight, iotaiot);
        tft.pushImage(0, 0, oxinondevWidth, oxinondevHeight, oxinondev);
        tft.setTextColor(ST7735_BLACK);
        tft.drawString("oxinon", 135, 47, 4);
        tft.drawString("development", 138, 75, 2);

         WiFi.mode(WIFI_OFF);
         btStop();

         delay(3000);


        tft.fillScreen(ST7735_WHITE);
        tft.pushImage(0, 0, oxinonWidth, oxinonHeight, oxinon);
        tft.setTextColor(ST7735_BLACK);

        tft.drawString("Hans Tischer", 47, 20, 4);
        tft.drawLine(11, 50, 229, 50, ST7735_GRAY);
        tft.drawString("oxinon", 135, 80, 4);
       
          buttonA.setLongClickHandler(longpress);
          buttonB.setLongClickHandler(longpress);
    }
   }

float voltage = 0.0;
   
void loop() {
  buttonA.loop();

  buttonB.loop();

}


void longpress(Button2& btn) {
  
    if (btn == buttonA) {
        unsigned int time = btn.wasPressedFor();
      Serial.print("You clicked A ");
      if (time > 700) {
        Serial.println("a really long time."); 
        tft.pushImage(0, 0, githubWidth, githubHeight, github);   
    } 

      else {
        Serial.println("long.");  
        tft.setSwapBytes(true);
        tft.pushImage(0, 0, twitterWidth, twitterHeight, twitter);      
    }
  }
    
    
    if (btn == buttonB) {
        unsigned int time = btn.wasPressedFor();
      Serial.print("You clicked B ");
      if (time > 700) {
        Serial.println("a really long time.");  
        tft.setSwapBytes(true);
        tft.pushImage(0, 0, oxinonWidth, oxinonHeight, oxinon);
        tft.setTextColor(ST7735_BLACK);

        tft.drawString("Hans Tischer", 47, 20, 4);
        tft.drawLine(11, 50, 229, 50, ST7735_GRAY);
        tft.drawString("oxinon", 135, 80, 4);

// reding battery voltage
    uint16_t v = analogRead(ADC_PIN);
    float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
    String voltage = "Voltage " + String(battery_voltage) + "V";
    Serial.println(voltage);
    Serial.println("");

// battery symbol
    tft.fillRect(4, 4, 28, 2, ST7735_GRAY);
    tft.fillRect(4, 16, 28, 2, ST7735_GRAY);
    tft.fillRect(4, 4, 2, 12, ST7735_GRAY);
    tft.fillRect(32, 4, 2, 14, ST7735_GRAY);
    tft.fillRect(34, 8, 3, 6, ST7735_GRAY);

// battery level symbol
   if(battery_voltage <= 2.99){
      tft.fillRect(4, 4, 28, 2, ST7735_RED);
      tft.fillRect(4, 16, 28, 2, ST7735_RED);
      tft.fillRect(4, 4, 2, 14, ST7735_RED);
      tft.fillRect(32, 4, 2, 14, ST7735_RED);
      tft.fillRect(34, 8, 3, 6, ST7735_RED);
      tft.drawString(voltage, 45, 8, 1);
    }
       
   if(battery_voltage >= 3.0){
      tft.fillRect(6, 6, 4, 10, ST7735_RED);
      tft.drawString(voltage, 45, 8, 1);
    }
   if(battery_voltage >= 3.2){
      tft.fillRect(6, 6, 6, 10, ST7735_RED);
      tft.drawString(voltage, 45, 8, 1);
    }
   if(battery_voltage >= 3.4){
      tft.fillRect(6, 6, 11, 10, ST7735_YELLOW);
      tft.drawString(voltage, 45, 8, 1);
    }
   if(battery_voltage >= 3.6){
      tft.fillRect(6, 6, 16, 10, ST7735_YELLOW);
      tft.drawString(voltage, 45, 8, 1);
    }
   if(battery_voltage >= 3.8){
      tft.fillRect(6, 6, 21, 10, ST7735_GREEN);
      tft.drawString(voltage, 45, 8, 1);
    }
   if(battery_voltage >= 4.0){
      tft.fillRect(6, 6, 26, 10, ST7735_GREEN);
      tft.drawString(voltage, 45, 8, 1);
    }
   if(battery_voltage >= 4.60){
      tft.fillRect(6, 6, 26, 10, ST7735_GREEN);
      tft.setTextColor(ST7735_BLACK);
      tft.drawString("CHG", 10, 7, 1);
    }
   if(battery_voltage >= 4.85){
      tft.fillRect(6, 6, 26, 10, ST7735_AQUA);
      tft.setTextColor(ST7735_BLACK);
      tft.drawString("USB", 10, 7, 1);
    }


             
    } 
      else {
        Serial.println("long."); 
        tft.setSwapBytes(true);
        tft.pushImage(0, 0, oxinonWidth, oxinonHeight, oxinon);
        tft.setTextColor(ST7735_BLACK);

        tft.drawString("Hans Tischer", 47, 20, 4);
        tft.drawLine(11, 50, 229, 50, ST7735_GRAY);
        tft.drawString("oxinon", 135, 80, 4);
      
    }
  }
}
