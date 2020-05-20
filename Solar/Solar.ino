#include <DallasTemperature.h>               //dodaj biblitekę LiquidCrystal.h

#define HISTEREZA 5
#define TEMP_MAX 70
#define ONE_WIRE_BUS D13 
#define SOLAR_PUMP_PIN D15 


int one_wire = ONE_WIRE_BUS;                           //Transmisja 1-Wire na pinie 10
bool SolarPumpOn = false;
OneWire oneWire(one_wire);                   //wywołujemy transmisję 1-Wire na pinie 10
DallasTemperature sensors(&oneWire);         //informujemy Arduino, ze przy pomocy 1-Wire
                                             //chcemy komunikowac sie z czujnikiem
                                             
 

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();                           //rozpocznij odczyt z czujnika
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(SOLAR_PUMP_PIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

 
void loop(void)
{ 
  float TempKolektora = sensors.getTempCByIndex(0);
  float TempZbiornika = sensors.getTempCByIndex(1);
  float TempNaDachu = sensors.getTempCByIndex(2);
  
  sensors.requestTemperatures();            //zazadaj odczyt temperatury z czujnika
  Serial.print("TempKolektora=");
  Serial.println(TempKolektora);
  Serial.print("TempZbiornika=");
  Serial.println(TempZbiornika);
  Serial.print("TempNaDachu=");
  Serial.println(TempNaDachu);
  
  if ( TempKolektora > (TempZbiornika + HISTEREZA))
  {
    //wlacz pompke
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    SolarPumpOn = true;
  }

  if ( TempKolektora > TEMP_MAX )
  {
    //wlacz pompe jesli temp max
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    SolarPumpOn = true;
  }
  else if ( TempKolektora < TempZbiornika)
  {
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
    SolarPumpOn = false;
  }

  if(SolarPumpOn==true)
  {
    digitalWrite(SOLAR_PUMP_PIN, HIGH);
    Serial.println("PUMP ON");
  }
  else
  {
    digitalWrite(SOLAR_PUMP_PIN, LOW);
    Serial.println("PUMP OFF");
  }
  
  delay(1000);                                //odczekaj 500ms
 
}
