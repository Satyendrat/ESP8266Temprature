#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>


const char *ssid =  "YOURSSID";   // Your AP Name cannot be longer than 32 characters!
const char *pass =  "Password";    //Your AP Password
const String url  =  "http://192.168.1.116:1500/temp.php?temp="; //Your Server URL. To make things simple I am using a GET method

//This is specific to WeMos D1 R2. I have connected the singal to D4 pin next to Ground and VCC 5
OneWire ds(D4);
float temp; 

//Pass OneWire ref to DallasTemprature
DallasTemperature sensors(&ds);

WiFiClient wclient;
HTTPClient http;

void setup() {
  
  Serial.begin(115200);
  delay(10);
  Serial.println("");
  Serial.println("");
}

void loop() {
  if(WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to AP => "); 
    Serial.println(ssid);
    WiFi.begin(ssid,pass);

    while(WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(500);
      
    }

      Serial.print("Connected to AP => ");
      Serial.println(ssid);
  }
  else
  {

      Serial.println("Requesting temperatures...");
      sensors.requestTemperatures(); // Send the command to get temperatures
      delay(1000);

      Serial.println("Temperature for Device 1 is =>");
      temp = sensors.getTempCByIndex(0);
      Serial.println(temp);

      //Send the temprature to the HTTP Server 
      String sendURL = url + temp;
      Serial.println(sendURL);
      
      http.begin(sendURL);
      
      int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        } 
        else 
        {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

    delay(1000);
    
  }

}
