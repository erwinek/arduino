#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 0 
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "led.h"

#define LED1_PIN     5    // kwadrat front + paski front dol
#define LED2_PIN     4    // paski gora bok
#define LED3_PIN     2    // daszek bok

#define NUM_LEDS1    33 + 41 + 33 + 41 + 27 + 27
#define NUM_LEDS2    36*4
#define NUM_LEDS3    43 + 43

#define CHIPSET     WS2812
#define COLOR_ORDER GRB

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];

#define BRIGHTNESS  100

#define TEMPERATURE_1 Tungsten100W
#define TEMPERATURE_2 OvercastSky

// How many seconds to show each temperature before switching
#define DISPLAYTIME 10
// How many seconds to show black between switches
#define BLACKTIME   3

#define RX_BUF_SIZE 50
char ReceivedCommand[RX_BUF_SIZE];
int ReceivedCnt = 0;
int ReceivedStart = 0;
uint8_t stateCnt = 0;          
uint8_t state = CMD_CHOINKA;
uint8_t value = 0;

void loop()
{
  switch(state) {
    case(CMD_CLEAR):
      fill_solid(leds1, NUM_LEDS1, CRGB::Black);
      fill_solid(leds2, NUM_LEDS2, CRGB::Black);
      fill_solid(leds3, NUM_LEDS3, CRGB::Black);
    break;

    case(CMD_RAINBOW):
      static uint8_t starthue = 0;
      fill_rainbow( leds1, NUM_LEDS1, --starthue, 20);    
      fill_rainbow( leds2, NUM_LEDS2, starthue + 255/3, 20);    
      fill_rainbow( leds3, NUM_LEDS3, starthue - 255/3, 20);    
    break;
    
    case(CMD_THREE_COLOURS):
      
    break;
    
    case(CMD_RUN_RGB):

    break;
    
    case(CMD_SWAP_RED):
       for( int i = 0; i < NUM_LEDS1; i++) {
          if(i%2) leds1[i] = CRGB::Red;
          else leds1[i] = CRGB::Black;
       }
       for( int i = 0; i < NUM_LEDS1; i++) {
          if(i%2) leds1[i] = CRGB::Red;
          else leds1[i] = CRGB::Black;
       }
       FastLED.show();
       FastLED.delay(300);
       
       for( int i = 0; i < NUM_LEDS1; i++) {
          if(i%2) leds1[i] = CRGB::Black;
          else leds1[i] = CRGB::Red;
       }
       FastLED.delay(300);

    break;

    case(CMD_CHOINKA):
      fill_rainbow( leds1, NUM_LEDS1, --starthue, 20);    
      fill_rainbow( leds2, NUM_LEDS2, starthue + 255/3, 20);    
      fill_rainbow( leds3, NUM_LEDS3, starthue - 255/3, 20);    
    break;
    
    case(CMD_SWAP_GREEN):
      for( int i = 0; i < NUM_LEDS1; i++) {
          if(i%2) leds1[i] = CRGB::Green;
          else leds1[i] = CRGB::Black;
       }
       for( int i = 0; i < NUM_LEDS1; i++) {
          if(i%2) leds1[i] = CRGB::Green;
          else leds1[i] = CRGB::Black;
       }
       FastLED.show();
       FastLED.delay(300);
       
       for( int i = 0; i < NUM_LEDS1; i++) {
          if(i%2) leds1[i] = CRGB::Black;
          else leds1[i] = CRGB::Green;
       }
       FastLED.delay(300);
    break;
    
    case(CMD_SWAP_BLUE):

    break;
    
    case(CMD_PERCENT):

    break;
  }
  

 while (Serial.available() > 0) {
    byte incomingByte;
    incomingByte = Serial.read();
    
    if((incomingByte==';') && (ReceivedStart==1)) {
      ReceivedCnt = 0;
      ReceivedStart = 0;

      uint8_t val = 0 ; 
      uint8_t cmd = 0;

      char hexDigit = toupper(ReceivedCommand[0]);
      int digit = (hexDigit >= 'A') ? hexDigit - 'A' + 10 : hexDigit - '0';
      cmd = digit << 4;

      hexDigit = toupper(ReceivedCommand[1]);
      digit = (hexDigit >= 'A') ? hexDigit - 'A' + 10 : hexDigit - '0';
      cmd = cmd + digit;

      hexDigit = toupper(ReceivedCommand[2]);
      digit = (hexDigit >= 'A') ? hexDigit - 'A' + 10 : hexDigit - '0';
      val = digit << 4;

      hexDigit = toupper(ReceivedCommand[3]);
      digit = (hexDigit >= 'A') ? hexDigit - 'A' + 10 : hexDigit - '0';
      val = val + digit;
      
      if((cmd > 0) && (cmd < CMD_LAST))  {
          state = cmd;
          Serial.write(cmd + 0x30);
          if(cmd == CMD_PERCENT)  {
            value = val;
          }
          Serial.print("OK");
      }
    }

    if(ReceivedStart) {
      ReceivedCommand[ReceivedCnt] = incomingByte;
      if(ReceivedCnt<RX_BUF_SIZE) ReceivedCnt++;
    }
    
    if(incomingByte==':') {
      ReceivedCnt = 0;
      ReceivedStart = 1;
    }
  }

  
  FastLED.show();
  
}

void setup() {
  delay( 3000 ); // power-up safety delay
  // It's important to set the color correction for your LED strip here,
  // so that colors can be more accurately rendered through the 'temperature' profiles
  FastLED.addLeds<CHIPSET, LED1_PIN, COLOR_ORDER>(leds1, NUM_LEDS1).setCorrection( TypicalSMD5050 );
  FastLED.addLeds<CHIPSET, LED2_PIN, COLOR_ORDER>(leds2, NUM_LEDS2).setCorrection( TypicalSMD5050 );
  FastLED.addLeds<CHIPSET, LED3_PIN, COLOR_ORDER>(leds3, NUM_LEDS3).setCorrection( TypicalSMD5050 );

  FastLED.setBrightness( BRIGHTNESS );

  //pinMode(13, INPUT);
  Serial.begin(57600);
  //Serial.swap();
  Serial.print("LED_v1.0");
  
  FastLED.setTemperature( OvercastSky ); // first temperature
}

