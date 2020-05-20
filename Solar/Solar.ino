#include <DallasTemperature.h>               //dodaj biblitekę LiquidCrystal.h
 
int one_wire = D13;                           //Transmisja 1-Wire na pinie 10
  
OneWire oneWire(one_wire);                   //wywołujemy transmisję 1-Wire na pinie 10
DallasTemperature sensors(&oneWire);         //informujemy Arduino, ze przy pomocy 1-Wire
                                             //chcemy komunikowac sie z czujnikiem
                                             
 

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();                           //rozpocznij odczyt z czujnika
  
}
 
void loop(void)
{ 
  
  sensors.requestTemperatures();            //zazadaj odczyt temperatury z czujnika
  Serial.print("T0=");
  Serial.println(sensors.getTempCByIndex(0));
  Serial.print("T1=");
  Serial.println(sensors.getTempCByIndex(1));
  delay(500);                                //odczekaj 50ms
 
 
}
