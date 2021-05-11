#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
 
const char* ssid = "IndihomeMawar";
const char* password =  "rahasiadong123";
 
const String jogja = "http://api.openweathermap.org/data/2.5/weather?q=Yogyakarta&appid=";
const String palembang = "http://api.openweathermap.org/data/2.5/weather?q=Palembang&appid=";
const String nuremberg = "http://api.openweathermap.org/data/2.5/weather?q=Nuremberg&appid=";

const String key = "82ef5cde022973673bb2087e180acdf9";

DynamicJsonDocument doc(2048);

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeOutTime = 1*15*1000;

int merah = 21, hijau = 22, biru = 23;

float convertToCelsius(float kelvin){
  float cel = kelvin - 273.15;
  return cel;
}

void message(float temp, String city){
  /* dingin  => < 18
   * sejuk  => 18 - 30
   * panas   => > 30
   */

   if(temp < 18){
      Serial.print("Kota ");
      Serial.print(city);
      Serial.print(" pada saat ini memiliki suhu ");
      Serial.print(temp);
      Serial.print(" Derajat Celsius");
      Serial.println("Status AC LEVEL 1 - DINGIN!");
      digitalWrite(merah, HIGH);
      digitalWrite(hijau, LOW);
      digitalWrite(biru, LOW);
   }
   if(temp >= 18 && temp <= 30){
      Serial.print("Kota ");
      Serial.print(city);
      Serial.print(" pada saat ini memiliki suhu ");
      Serial.print(temp);
      Serial.print(" Derajat Celsius");
      Serial.println("Status AC LEVEL 2 - SEJUK!");
      digitalWrite(merah, LOW);
      digitalWrite(hijau, HIGH);
      digitalWrite(biru, LOW);
   }
   if(temp > 30){
      Serial.print("Kota ");
      Serial.print(city);
      Serial.print(" pada saat ini memiliki suhu ");
      Serial.print(temp);
      Serial.print(" Derajat Celsius");
      Serial.println("Status AC LEVEL 3 - PANAS!");
      digitalWrite(merah, LOW);
      digitalWrite(hijau, LOW);
      digitalWrite(biru, HIGH);
   }
}
 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("wifi connected with IP : ");
  Serial.println(WiFi.localIP());

  pinMode(merah, OUTPUT);
  pinMode(hijau, OUTPUT);
  pinMode(biru, OUTPUT);
}
 
void loop() {
  String endpoint = "";
  currentTime = millis();
  if ((WiFi.status() == WL_CONNECTED)) { 
    for(int i=0; i<3; i++){
    HTTPClient http;
      //if(currentTime - previousTime > timeOutTime){
        if(i == 0){
          endpoint = jogja;
        }else if(i == 1){
          endpoint = palembang;
        }else{
          endpoint = nuremberg;
        }
        http.begin(endpoint + key);
        int httpCode = http.GET(); 
     
        if (httpCode > 0) {
     
            String payload = http.getString();
            Serial.println(httpCode);
            //Serial.println(payload);
    
            DeserializationError error = deserializeJson(doc, payload);
    
            if (error) {
              Serial.print(F("deserializeJson() failed: "));
              Serial.println(error.f_str());
              return;
            }
            JsonObject main = doc["main"];
            float main_temp = main["temp"];
            float tempCelsius = convertToCelsius(main_temp);
            const char* cityName = doc["name"];
            
            //print temp
            message(tempCelsius, cityName);
        }
        else {
          Serial.print("Error on HTTP request : ");
          Serial.println(httpCode);
        }               
      //}
        http.end(); //Free the resources
        delay(15000);
    }
  }
}
