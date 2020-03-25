#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


const char* ssid = "esp32";
const char* password =  "esp32_01";
String payload_in="";
void setup() {
 
Serial.begin(115200);

if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);      
  display.setCursor(0, 10);
  display.println("Connecting to WiFi");
  display.display(); 
  
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
    Serial.println("Connected to the WiFi network");
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("Connected to WiFi");
    display.display(); 
 
  }
void disptitle(){
    display.clearDisplay();
    display.setCursor(15, 0);
    display.println("COVID-19 - INDIA");
    display.display();
}
void getIndia(){
    if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;
 
    http.begin("https://corona.lmao.ninja/countries/india"); //Specify the URL
    int httpCode = http.GET();                                        //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        payload_in = http.getString();
        Serial.println(httpCode);
        Serial.println(payload_in);
        parseJson(payload_in.c_str());
        }
 
    else {
      Serial.println("Error on HTTP request");
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println("Error on HTTP request");
      display.display();
    }
 
    http.end(); //Free the resources
  }
}





void loop() {
  getIndia();
  delay(10000);
 
}




//to parse json data recieved from OWM
void parseJson(const char * jsonString) {
  const size_t capacity = JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(11) + 270;
  DynamicJsonDocument doc(capacity);
  DeserializationError err = deserializeJson(doc, jsonString);
  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.c_str());
  }
  Serial.println("parsed");
  const char* country = doc["country"]; // "India"
  Serial.println(country);
  disptitle();
  int cases = doc["cases"]; // 562
  Serial.println(cases);
  display.setCursor(0, 11);
  display.println("CASES : ");
  display.setCursor(50, 11);
  display.println(cases);
  display.display();
  int todayCases = doc["todayCases"]; // 26
  Serial.println(todayCases);
  display.setCursor(0, 22);
  display.println("TODAY : ");
  display.setCursor(50, 22);
  display.println(todayCases);
  display.display();
  int deaths = doc["deaths"]; // 11
  Serial.println(deaths);
  delay(10000);
  disptitle();
  display.setCursor(0, 11);
  display.println("DEATHS : ");
  display.setCursor(50, 11);
  display.println(deaths);
  display.display();
  int todayDeaths = doc["todayDeaths"]; // 1
  Serial.println(todayDeaths);
  display.setCursor(0, 22);
  display.println("TODAY : ");
  display.setCursor(50, 22);
  display.println(todayDeaths);
  display.display();
  int recovered = doc["recovered"]; // 40
  Serial.println(recovered);
  delay(10000);
  disptitle();
  display.setCursor(0, 11);
  display.println("RECOVER: ");
  display.setCursor(50, 11);
  display.println(recovered);
  display.display();
  int active = doc["active"]; // 511
  Serial.println(active);
  display.setCursor(0, 22);
  display.println("ACTIVE : ");
  display.setCursor(50, 22);
  display.println(active);
  display.display();
  int critical = doc["critical"]; // 0
  Serial.println(critical);
  int casesPerOneMillion = doc["casesPerOneMillion"]; // 0
  Serial.println(casesPerOneMillion);
  int deathsPerOneMillion = doc["deathsPerOneMillion"]; // 0
  Serial.println(deathsPerOneMillion);
}
