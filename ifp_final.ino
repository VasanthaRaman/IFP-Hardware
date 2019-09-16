#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include "DHT.h"
DHT dht;
//----------------  Fill in your credentails   ---------------------
char ssid[] = "OnePlus 3T";             // your network SSID (name) 
char pass[] = "sai12345";         // your network password
unsigned long myChannelNumber =  817389;  // Replace the 0 with your channel number
const char * myWriteAPIKey = "5EDCKABTH1VIZ0U3";    // Paste your ThingSpeak Write API Key between the quotes 
//------------------------------------------------------------------

WiFiClient  client;
//int number = 1;
 int sensorvalue,percentval;
void setup() {
  //Initialize serial and wait for port to open:
  dht.setup(D3);
  Serial.begin(9600);
  while (!Serial) {
   Serial.print(",") ; // wait for serial port to connect. Needed for native USB port only
  }

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client); 
  
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println("!");
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  sensorvalue = analogRead(A0);
  percentval = map(sensorvalue,1023,200,0,100);
  float humid = dht.getHumidity();/* Get humidity value */
  float temp = dht.getTemperature();
  //percentval=25;
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  Serial.print("percent_val");
  Serial.print(percentval);
  Serial.print("TEmp_val:");
  Serial.print(temp);
   Serial.print("humid_val:");
  Serial.print(humid);
  ThingSpeak.setField(1, (float)percentval);
  ThingSpeak.setField(2, (float)temp);
ThingSpeak.setField(3, (float)humid);
int x=ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
 /* int x = ThingSpeak.writeField(myChannelNumber, 1, percentval, myWriteAPIKey);
  int y= ThingSpeak.writeField(myChannelNumber, 2, temp, myWriteAPIKey);
   int z= ThingSpeak.writeField(myChannelNumber, 3, humid, myWriteAPIKey); */
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(20000); // Wait 20 seconds before sending a new value
}
