#include <DallasTemperature.h>               //dodaj biblitekę LiquidCrystal.h
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <ThingSpeak.h>

#define HISTEREZA 20
#define HISTEREZA_OFF 15
#define TEMP_MAX 65
#define ONE_WIRE_BUS D12
#define SOLAR_PUMP_PIN D15 
#define SREDNIA_Z_POMIAROW 10

#ifndef STASSID
#define STASSID "ASUS"
#define STAPSK  "BezpieczneHasloWifi123"
#endif

const char * ssid = STASSID; // your network SSID (name)
const char * pass = STAPSK;  // your network password

unsigned long myChannelNumber = 61972;
const char * myWriteAPIKey = "7MK6L089WHKG1FB1";
WiFiClient  client;

int sensor_timer = 0;
int one_wire = ONE_WIRE_BUS;                           //Transmisja 1-Wire na pinie 10
bool SolarPumpOn = false;
OneWire oneWire(one_wire);                   //wywołujemy transmisję 1-Wire na pinie 10
DallasTemperature sensors(&oneWire);         //informujemy Arduino, ze przy pomocy 1-Wire
ESP8266WebServer server(80);

char LogArray[1000] = "";

float TempKolektora;
float TempZbiornika;
float TempKolektora2;
  
float temp1[SREDNIA_Z_POMIAROW];
float temp2[SREDNIA_Z_POMIAROW];
float temp3[SREDNIA_Z_POMIAROW];
float temp4[SREDNIA_Z_POMIAROW];

float AvgTemp1 = 0;
float AvgTemp2 = 0;
float AvgTemp3 = 0;
float AvgTemp4 = 0;

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
 int pSrednia = 0;
void loop(void)
{ 
  sensor_timer++;

  if(sensor_timer>300) {
    sensor_timer=0;

    pSrednia++;
    if(pSrednia >= 10) pSrednia = 0;

    uint8_t address0[8] = { 0x28, 0x37, 0xF2, 0x0F, 0x00, 0x00, 0x00, 0xC9 };
    uint8_t address1[8] = { 0x28, 0x83, 0x80, 0x77, 0x91, 0x19, 0x02, 0x29 };
    uint8_t address2[8] = { 0x28, 0xFF, 0x90, 0x70, 0x86, 0x16, 0x05, 0x9D };
    
    temp1[pSrednia] = sensors.getTempCByIndex(0);
    if(temp1[pSrednia]==85.00) temp1[pSrednia] = AvgTemp1;
    temp2[pSrednia] = sensors.getTempCByIndex(1);
    if(temp2[pSrednia]==85.00) temp2[pSrednia] = AvgTemp2;
    temp3[pSrednia] = sensors.getTempCByIndex(2);
    //delay(10);
    //temp4[pSrednia] = sensors.getTempCByIndex(3);

    Serial.println(temp1[pSrednia]);
    Serial.println(temp2[pSrednia]);
    Serial.println(temp3[pSrednia]);
    

    sensors.requestTemperatures();            //zazadaj odczyt temperatury z czujnika

    float tempAvg = 0;
    for(int i=0;i<10;i++) tempAvg += temp1[i];
    AvgTemp1 = tempAvg/10;

    tempAvg = 0;
    for(int i=0;i<10;i++) tempAvg += temp2[i];
    AvgTemp2 = tempAvg/10;

    tempAvg = 0;
    for(int i=0;i<10;i++) tempAvg += temp3[i];
    AvgTemp3 = tempAvg/10;

    tempAvg = 0;
    for(int i=0;i<10;i++) tempAvg += temp4[i];
    AvgTemp4 = tempAvg/10;

/*
    Serial.print("AvgTemp1=");
    Serial.println(AvgTemp1);
    Serial.print("AvgTemp2=");
    Serial.println(AvgTemp2);
    Serial.print("AvgTemp3=");
    Serial.println(AvgTemp3);
*/

    TempKolektora = AvgTemp3; 
    TempKolektora2 = AvgTemp2; 
    TempZbiornika = AvgTemp1; 

    Serial.print(" TempKolektora=");
    Serial.println(TempKolektora);
    ThingSpeak.setField (6, TempKolektora);

    Serial.print("TempZbiornika=");
    Serial.println(TempZbiornika);
    ThingSpeak.setField (7, TempZbiornika);

    Serial.print("TempKolektora2=");
    Serial.println(TempKolektora2);
    ThingSpeak.setField (8, TempKolektora2);

    

    /*
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
  */
  if (( TempKolektora > (TempZbiornika + HISTEREZA)) || ( TempKolektora2 > (TempZbiornika + HISTEREZA)))
  {
    //wlacz pompke
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    SolarPumpOn = true;
  }

  if (( TempKolektora > TEMP_MAX ) || (TempKolektora2 > TEMP_MAX))
  {
    //wlacz pompe jesli temp max
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    SolarPumpOn = true;
  }
  else if (( TempKolektora <= (TempZbiornika + HISTEREZA_OFF)) && ( TempKolektora2 <= (TempZbiornika + HISTEREZA_OFF) ))
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
  }
  if(cnt<2000) cnt++;
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

  delay(10);                                //odczekaj 500ms
  //ArduinoOTA.handle();
  server.handleClient();
 
}
//--------End Loop----------------------------------------------------------------------
void handleRoot() {
  String out = "\n temp1=" + String(AvgTemp1) + "\n";
  out += "\n temp2=" + String(AvgTemp2);
  out += "\n temp3=" + String(AvgTemp3);
  out += "\n temp4=" + String(AvgTemp4);
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
