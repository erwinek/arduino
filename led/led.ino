#include <Adafruit_NeoPixel.h>


#define PIN 6
#define LICZBADIOD 100
 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LICZBADIOD, PIN, NEO_GRB + NEO_KHZ800);
byte LedStrip[LICZBADIOD][3];

void setup() {
  Serial.begin(9600);
  pixels.begin(); // Inicjalizacja biblioteki
}

void SendLed(void)
{
  for(int i=0; i<LICZBADIOD; i++)
  {
    pixels.setPixelColor(i, LedStrip[i][0], LedStrip[i][1], LedStrip[i][2]);
  }
  pixels.show();  
  delay(10);
}

void loop() {
  Serial.print("sensor = " );

  for(int red=0;red<255;red++) {
    for(int i=0;i<100;i++)
    {
      LedStrip[i][0]=red;
      LedStrip[i][1]=0;
      LedStrip[i][2]=0;
    }
    SendLed();
  }
  for(int red=255;red>0;red--) {
    for(int i=0;i<100;i++)
    {
      LedStrip[i][0]=red;
      LedStrip[i][1]=0;
      LedStrip[i][2]=0;
    }
    SendLed();
  }
  
  for(int red=0;red<255;red++) {
    for(int i=0;i<100;i++)
    {
      LedStrip[i][0]=0;
      LedStrip[i][1]=red;
      LedStrip[i][2]=0;
    }
    SendLed();
  }
  for(int red=255;red>0;red--) {
    for(int i=0;i<100;i++)
    {
      LedStrip[i][0]=0;
      LedStrip[i][1]=red;
      LedStrip[i][2]=0;
    }
    SendLed();
  }

  for(int red=0;red<255;red++) {
    for(int i=0;i<100;i++)
    {
      LedStrip[i][0]=0;
      LedStrip[i][1]=0;
      LedStrip[i][2]=red;
    }
    SendLed();
  }
  for(int red=255;red>0;red--) {
    for(int i=0;i<100;i++)
    {
      LedStrip[i][0]=0;
      LedStrip[i][1]=0;
      LedStrip[i][2]=red;
    }
    SendLed();
  }

    
  
}
