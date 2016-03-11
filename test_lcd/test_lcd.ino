#include <U8glib.h>

// set pin numbers:
const int lcd_sck = 8;
const int lcd_mosi = 9;
const int lcd_cs = 13;
const int lcd_reset = 11;

U8GLIB_ST7920_128X64_1X u8g(lcd_cs );

void setup() {  
  u8g.setFont(u8g_font_unifont);
  u8g.setColorIndex(1); // Instructs the display to draw with a pixel on. 
  
  Serial.begin(9600);
}

void loop() {  
  u8g.firstPage();
  do {  
    draw();
  } while( u8g.nextPage() );
  
  Serial.print("HelloWorld");
  
  delay(1000);   
}
  
void draw(){
  u8g.drawStr( 0, 20, "Hello World");
    
}


