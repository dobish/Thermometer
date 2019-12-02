#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
 
const char* ssid = "SSID";
const char* password = "PASS";
#define ONE_WIRE_BUS D1

//const int oneWireBus = 5;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
 
void setup () {
 
Serial.begin(115200);
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) {
 
delay(1000);
Serial.print("Connecting..");
 
}
   //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  sensors.begin();
}
 
void loop() {
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    WiFi.mode(WIFI_STA);
    HTTPClient http;  //Declare an object of class HTTPClient

//Temperature
sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  //float temperatureF = sensors.getTempFByIndex(0);
  
    

    // allocate the memory for the document
    // compute the required size
const size_t CAPACITY = JSON_ARRAY_SIZE(3);

    DynamicJsonDocument doc(1024);
    // create an object
    JsonObject root = doc.to<JsonObject>();
    root["Name"] = "Name";
    root["question"] = temperatureC;



JsonArray answers = root.createNestedArray("answers");


char reqSend[300];
serializeJson(root, reqSend);
    



    
    //postData = here goes the JSON;
    
    http.begin("http://copyoverflow.herokuapp.com/api/kittens");  //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
    int httpCode = http.POST(reqSend);        //Send the request
    
  if (httpCode > 0) { //Check the returning code
 
    String payload = http.getString();   //Get the request response payload
    Serial.println(payload);                     //Print the response payload
    Serial.println(httpCode);
    //Serial.println(object);
    Serial.println(reqSend);
     Serial.print(sensors.getTempCByIndex(0));

    // serialize the object and send the result to Serial
      serializeJson(doc, Serial);
} 
 
http.end();   //Close connection
 
}
 
delay(60000);    //Send a request every 30 seconds
 
}
