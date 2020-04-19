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


const char* ssid = "";
const char* password =  "";
String payload_in="", payload_int="";
long disp_cases, disp_deaths, disp_cases_today,disp_deaths_today;
long totcases, totdeaths;
void setup() {
 
Serial.begin(115200);
pinMode(2, OUTPUT);
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
void disptitleINT(){
    display.clearDisplay();
    display.setCursor(30, 0);
    display.println("COVID-19");
    display.display();
}
void getdata(){
    if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http, httpINT;
 
    http.begin("https://corona.lmao.ninja/v2/countries/india"); //Specify the URL
    int httpCode = http.GET();                                        //Make the request
    httpINT.begin("https://corona.lmao.ninja/v2/all"); //Specify the URL

    int httpCodeINT = httpINT.GET();                                        //Make the request
 
    if (httpCode > 0 && httpCodeINT>0) { //Check for the returning code
 
        payload_in = http.getString();
        payload_int = httpINT.getString();
        parseJson(payload_in.c_str());
        parseJsonINT(payload_int.c_str());
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
  digitalWrite(2, HIGH);
  getdata();
  delay(10);
for(int i=0; i<10;i++){
  digitalWrite(2, LOW);
  disptitleINT();
  display.setCursor(1, 11);
  display.println("CASES : ");
  display.setCursor(51, 11);
  display.println(totcases);
  display.setCursor(1, 22);
  display.println("DEATHS : ");
  display.setCursor(51, 22);
  display.println(totdeaths);
  for(int i=0; i<121;i++){
      display.setCursor(i, 26);
      display.println(".");
      display.display();
      delay(60);
  }
  disptitle();
  display.setCursor(1, 11);
  display.println("CASES : ");
  display.setCursor(51, 11);
  display.println(disp_cases);
    
  display.setCursor(1, 22);
  display.println("TODAY : ");
  display.setCursor(51, 22);
  display.println(disp_cases_today);
  for(int i=0; i<121;i++){
      display.setCursor(i, 26);
      display.println(".");
      display.display();
      delay(60);
  }
  disptitle();
  display.setCursor(0, 11);
  display.println("DEATHS : ");
  display.setCursor(50, 11);
  display.println(disp_deaths);
  display.display();

  display.setCursor(0, 22);
  display.println("TODAY : ");
  display.setCursor(50, 22);
  display.println(disp_deaths_today);
  display.display();
  for(int i=0; i<121;i++){
      display.setCursor(i, 26);
      display.println(".");
      display.display();
      delay(60);
  }
}
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
  const char* country = doc["country"]; 
  disp_cases = doc["cases"]; 
  disp_cases_today = doc["todayCases"];
  disp_deaths = doc["deaths"];
  disp_deaths_today = doc["todayDeaths"];
  
}
void parseJsonINT(const char * jsonString) {
  const size_t capacity = JSON_OBJECT_SIZE(4) + 40;
  DynamicJsonDocument doc(capacity);
  DeserializationError err = deserializeJson(doc, jsonString);
  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.c_str());
  }
  totcases = doc["cases"]; 
  totdeaths = doc["deaths"]; 

}
