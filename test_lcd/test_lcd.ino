#include <U8glib.h>

// set pin numbers:
const int lcd_sck = 8;
const int lcd_mosi = 9;
const int lcd_cs = 13;
const int lcd_reset = 11;

//RS — (CS) Начало/окончание передачи данных (1 — начало, 0 — окончание)
//RW — (SID) Шина данных
//E — (SCLK) Строб

U8GLIB_ST7920_128X64_1X u8g(lcd_cs );

void setup() {  
  u8g.setFont(u8g_font_unifont);
  u8g.setColorIndex(1); // Instructs the display to draw with a pixel on. 
}

void loop() {  
  u8g.firstPage();
  do {  
    draw();
  } while( u8g.nextPage() );
  delay(1000);   
}
  
void draw(){
  u8g.drawStr( 0, 20, "Hello World");
    
}

/*
void setup() {
  // put your setup code here, to run once:
  u8g.setFont(u8g_font_unifont);
  u8g.setColorIndex(1); // Instructs the display to draw with a pixel on. 
    Serial.begin(9600);

  //  u8g.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  //draw();

  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.print("Hello");
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  Serial.print("World");
  delay(1000);              // wait for a second
}

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  //u8g.setFont(u8g_font_unifont);
  //u8g.drawStr(1, 1, "Hello World!");

  

  
}
*/
