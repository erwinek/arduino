#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
<<<<<<< HEAD
#include <ThingSpeak.h>
=======
#include "ThingSpeak.h"
>>>>>>> 2ca3cf8cc3605a0f3c9259e27b2eebc0451b76ad

#define HISTEREZA 6
#define TEMP_MAX 45
#define ONE_WIRE_BUS D13 
#define SOLAR_PUMP_PIN D15 
#define PUMP_ON_TIME 50
#define TEMP_MIN_PUMP 42

#ifndef STASSID
#define STASSID "ASUS"
#define STAPSK  "BezpieczneHasloWifi123"
#endif

const char * ssid = STASSID; // your network SSID (name)
const char * pass = STAPSK;  // your network password

unsigned long myChannelNumber = 61972;
const char * myWriteAPIKey = "7MK6L089WHKG1FB1";
<<<<<<< HEAD
WiFiClient  client;

=======

int timer_thingspeak = 0;
int pump_timer = 30;
>>>>>>> 2ca3cf8cc3605a0f3c9259e27b2eebc0451b76ad
int sensor_timer = 0;
int one_wire = ONE_WIRE_BUS;                           //Transmisja 1-Wire na pinie 10
bool SolarPumpOn = false;
OneWire oneWire(one_wire);                   //wywołujemy transmisję 1-Wire na pinie 10
DallasTemperature sensors(&oneWire);         //informujemy Arduino, ze przy pomocy 1-Wire
ESP8266WebServer server(80);
WiFiClient  WifiClient;

float TempKolektora;
float TempZbiornika;
float TempKolektora2;
  
float temp1 = 0;
float temp2 = 0;
float temp3 = 0;
float temp4 = 0;
DeviceAddress tempDeviceAddress1;
DeviceAddress tempDeviceAddress2;
DeviceAddress tempDeviceAddress3;
DeviceAddress tempDeviceAddress4;
                                             
void setup(void)
{
  Serial.begin(9600);

  sensors.begin();                           //rozpocznij odczyt z czujnika
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(SOLAR_PUMP_PIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output



  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  findDevices(ONE_WIRE_BUS);

  ThingSpeak.begin(WifiClient);
  
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

  //----------------------------------- WEB
    server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/gif", []() {
    static const uint8_t gif[] PROGMEM = {
      0x47, 0x49, 0x46, 0x38, 0x37, 0x61, 0x10, 0x00, 0x10, 0x00, 0x80, 0x01,
      0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x10, 0x00, 0x00, 0x02, 0x19, 0x8c, 0x8f, 0xa9, 0xcb, 0x9d,
      0x00, 0x5f, 0x74, 0xb4, 0x56, 0xb0, 0xb0, 0xd2, 0xf2, 0x35, 0x1e, 0x4c,
      0x0c, 0x24, 0x5a, 0xe6, 0x89, 0xa6, 0x4d, 0x01, 0x00, 0x3b
    };
    char gif_colored[sizeof(gif)];
    memcpy_P(gif_colored, gif, sizeof(gif));
    // Set the background to a random set of colors
    gif_colored[16] = millis() % 256;
    gif_colored[17] = millis() % 256;
    gif_colored[18] = millis() % 256;
    server.send(200, "image/gif", gif_colored, sizeof(gif_colored));
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  ThingSpeak.begin(client);  // Initialize ThingSpeak
}


void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}
 int cnt=0;
void loop(void)
{ 
  sensor_timer++;

  if(sensor_timer>100) {
    sensor_timer=0;

<<<<<<< HEAD
    uint8_t address0[8] = { 0x28, 0x37, 0xF2, 0x0F, 0x00, 0x00, 0x00, 0xC9 };
=======
    if(pump_timer) pump_timer--;

    uint8_t address0[8] = { 0x28, 0xB5, 0x83, 0x77, 0x91, 0x10, 0x02, 0x8A };
>>>>>>> 2ca3cf8cc3605a0f3c9259e27b2eebc0451b76ad
    uint8_t address1[8] = { 0x28, 0x83, 0x80, 0x77, 0x91, 0x19, 0x02, 0x29 };
    uint8_t address2[8] = { 0x28, 0xFF, 0x90, 0x70, 0x86, 0x16, 0x05, 0x9D };

    float temp = sensors.getTempC(address0);
    if((temp > -100) &&(temp!=85)) TempKolektora = temp; 

    temp = sensors.getTempC(address1);
    if((temp > -100) &&(temp!=85)) TempZbiornika = temp; 

    temp = sensors.getTempC(address2);
    if((temp > -100) &&(temp!=85)) TempKolektora2 = temp; 
    
    sensors.requestTemperatures();            //zazadaj odczyt temperatury z czujnika

    Serial.print(" TempKolektora=");
    Serial.println(TempKolektora);
    //ThingSpeak.setField (6, TempKolektora);

    Serial.print("TempZbiornika=");
    Serial.println(TempZbiornika);
    ThingSpeak.setField (7, TempZbiornika);

    Serial.print("TempKolektora2=");
    Serial.println(TempKolektora2);
<<<<<<< HEAD
    ThingSpeak.setField (8, TempKolektora2);


    
    temp1 = sensors.getTempCByIndex(0);
    temp2 = sensors.getTempCByIndex(1);
    temp3 = sensors.getTempCByIndex(2);
    temp4 = sensors.getTempCByIndex(3);

    sensors.getAddress(tempDeviceAddress1, 0);
    sensors.getAddress(tempDeviceAddress2, 1);
    sensors.getAddress(tempDeviceAddress3, 2);
    sensors.getAddress(tempDeviceAddress4, 3);

    Serial.print("id1=");
    printAddress(tempDeviceAddress1);
    Serial.print("id2=");
    printAddress(tempDeviceAddress2);
    Serial.print("id3=");
    printAddress(tempDeviceAddress3);
    Serial.print("id4=");
    printAddress(tempDeviceAddress4);
 
=======


  /*
>>>>>>> 2ca3cf8cc3605a0f3c9259e27b2eebc0451b76ad
  if ( TempKolektora > (TempZbiornika + HISTEREZA))
  {
    //wlacz pompke
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    SolarPumpOn = true;
  }
  if ( TempKolektora2 > (TempZbiornika + HISTEREZA))
  {
    //wlacz pompke
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    SolarPumpOn = true;
  }
  */
  
  if (( TempKolektora > TEMP_MAX ) || (TempKolektora2 > TEMP_MAX))
  {
    //wlacz pompe jesli temp max
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    SolarPumpOn = true;
  }
  else if ( TempKolektora < TempZbiornika + (HISTEREZA/2))
  {
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
    SolarPumpOn = false;
  }

  if(SolarPumpOn==true)
  {
    if(!pump_timer) {
      if((TempKolektora > TEMP_MIN_PUMP) | (TempKolektora2 > TEMP_MIN_PUMP) ) {
        pump_timer = PUMP_ON_TIME;
        digitalWrite(SOLAR_PUMP_PIN, HIGH);
        Serial.println("PUMP ON");
      }
    }    
    else Serial.printf("\n pumpt_timer=%d", pump_timer);
  }
  else
  {
    if(!pump_timer) {
      digitalWrite(SOLAR_PUMP_PIN, LOW);
      Serial.println("PUMP OFF");
    }
  }
  }

  timer_thingspeak++;
  if(timer_thingspeak>10000)
  {
    timer_thingspeak = 0;
/*
    // Write value to Field 1 of a ThingSpeak Channel
    int httpCode = ThingSpeak.writeField(myChannelNumber, 7, TempZbiornika, myWriteAPIKey);

    if (httpCode == 200) Serial.println("Channel write successful.");
    else  Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
    
    httpCode = ThingSpeak.writeField(myChannelNumber, 6, TempKolektora2, myWriteAPIKey);
    if (httpCode == 200) Serial.println("Channel write successful.");
    else  Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
    
    httpCode = ThingSpeak.writeField(myChannelNumber, 6, TempKolektora, myWriteAPIKey);
    if (httpCode == 200) Serial.println("Channel write successful.");
    else  Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
  */
  ThingSpeak.setField(7, TempZbiornika);
  ThingSpeak.setField(8, TempKolektora2);
  ThingSpeak.setField(6, TempKolektora);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  
  }
  if(cnt<1000) cnt++;
  else {
    cnt = 0;
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
  }

  delay(1);                                //odczekaj 500ms
  ArduinoOTA.handle();
  server.handleClient();
 
}
//--------End Loop----------------------------------------------------------------------
void handleRoot() {
  String out = "\n temp1=" + String(temp1) + "\n";
  out += "\n temp2=" + String(temp2);
  out += "\n temp3=" + String(temp3);
  out += "\n temp4=" + String(temp4);
  server.send(200, "text/plain", "System Solarny" + out);
  
}

void handleNotFound() {
  
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  
}

uint8_t findDevices(int pin)
{
  OneWire ow(pin);

  uint8_t address[8];
  uint8_t count = 0;

  if (ow.search(address))
  {
    Serial.print("\nuint8_t pin");
    Serial.print(pin, DEC);
    Serial.println("[][8] = {");
    do {
      count++;
      Serial.println("  {");
      for (uint8_t i = 0; i < 8; i++)
      {
        Serial.print("0x");
        if (address[i] < 0x10) Serial.print("0");
        Serial.print(address[i], HEX);
        if (i < 7) Serial.print(", ");
      }
      Serial.println("  },");
    } while (ow.search(address));

    Serial.println("};");
    Serial.print("// nr devices found: ");
    Serial.println(count);
  }
  return count;
}
