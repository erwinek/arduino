#include <DallasTemperature.h>               //dodaj biblitekę LiquidCrystal.h
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

#define HISTEREZA 5
#define TEMP_MAX 70
#define ONE_WIRE_BUS D13 
#define SOLAR_PUMP_PIN D15 

#ifndef STASSID
#define STASSID "ASUS"
#define STAPSK  "HasloWifi123"
#endif

const char * ssid = STASSID; // your network SSID (name)
const char * pass = STAPSK;  // your network password

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

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

    ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
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
  
  delay(1);                                //odczekaj 500ms
  ArduinoOTA.handle();
 
}
