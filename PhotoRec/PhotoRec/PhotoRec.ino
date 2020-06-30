zan#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"
#include "WiFi.h"

#include "OLEDDisplayUi.h"
#include "images.h"

SSD1306  display(0x3c, 4, 15);

//#define DEMO_DURATION 3000
//typedef void (*Demo)(void);

OLEDDisplayUi ui     ( &display );

void msOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(128, 0, String(millis()));
}

void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->drawXbm(x, y, BT_width, BT_height, BT_bits);
  display->drawXbm(x + 12 + 1, y, WIFI_width, WIFI_height, WIFI_bits);
  display->drawXbm(x + 108, y, BAT_width, BAT_height, BAT_bits);
  display->drawXbm(x + 34, y + 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

void drawFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->drawXbm(x, y, BT_width, BT_height, BT_bits);
  display->drawXbm(x + 12 + 1, y, WIFI_width, WIFI_height, WIFI_bits);
  display->drawXbm(x + 108, y, BAT_width, BAT_height, BAT_bits);
  display->drawXbm(x + 34, y + 12, LoRa_Logo_width, LoRa_Logo_height, LoRa_Logo_bits);
}



void drawFrame3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(x, y, "LoRa recevier...");
  display->setFont(ArialMT_Plain_10);
  display->drawString(x, y + 20, "Received one Packet");
  display->drawString(x, y + 30, "hello 47");
  display->drawString(x, y + 40, "with RSSI -37");
}

FrameCallback frames[] = { drawFrame1, drawFrame2, drawFrame3, };

int frameCount = 3;

//OverlayCallback overlays[] = { msOverlay };
//int overlaysCount = 1;

void WIFIScan(unsigned int value)
{
  unsigned int i;
  for(i=0;i<value;i++){
    display.clear();
    
    display.drawString(0, 0, "Scan start...");
    display.display();
    delay(500);
    display.clear();
    
    int n = WiFi.scanNetworks();
    display.drawString(0, 0, "Scan done");
    display.display();
    delay(500);
    display.clear();
    
    if (n == 0) {
          display.drawString(0, 0, "no network found");
          display.display();
          delay(500);
      } else {
  //        display.print(n);
  //        display.println(" networks found");
          display.drawString(0, 0, (String)n);
          display.drawString(12, 0, "networks found:");
          display.display();
          delay(500);
          
          for (int i = 0; i < n; ++i) {
              // Print SSID and RSSI for each network found
              display.drawString(0, (i+2)*9,(String)(i + 1));
              display.drawString(6, (i+2)*9, ":");
              display.drawString(12,(i+2)*9, (String)(WiFi.SSID(i)));
              display.drawString(90,(i+2)*9, " (");
              display.drawString(98,(i+2)*9, (String)(WiFi.RSSI(i)));
              display.drawString(114,(i+2)*9, ")");
  //            display.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
              delay(10);
          }
      }
  //    display.println("");
  
    display.display();
    delay(5000);
    display.clear();
    }
}

void setup() {
  Serial.begin(115200);
  
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(16, HIGH);    // turn the LED off by making the voltage LOW
  
  ui.setTargetFPS(30);

  // Customize the active and inactive symbol
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);

  // You can change this to
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(BOTTOM);

  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  // Add frames
  ui.setFrames(frames, frameCount);

  // Add overlays
  //ui.setOverlays(overlays, overlaysCount);

  // Initialising the UI will init the display too.
  ui.init();

  display.flipScreenVertically();

  //WIFI初始化 + 扫描演示
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  display.drawString(0, 0, "WIFI Setup done");
  display.display();
  delay(1000);

  WIFIScan(4);
}

void loop() {
  int remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0) {
    // You can do some work here
    // Don't do stuff if you are below your
    // time budget.
    delay(remainingTimeBudget);
  }
 }
