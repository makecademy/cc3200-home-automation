// Libraries
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <dht.h>

// WiFi Client
WiFiClient client;

// Your network name also called SSID
char ssid[] = "yourNetworkName";
// your network password
char password[] = "password";
// your network key Index number (needed only for WEP)
int keyIndex = 0;

// Dweet parameters
char * server_name = "www.dweet.io";
#define thing_name  "yourThingName"

void setup() {

  Serial.begin(115200);      // Initialize serial communication

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }
  
  // Pin for the PIR sensor
  pinMode(8,INPUT);
  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

}


void loop() {
    
  // Measure motion
  int motion = digitalRead(8);
  Serial.println(motion);
  
  // Measure temperature & humidity  
  float temperature, humidity;
  if (dht::readFloatData(10, &temperature, &humidity, false) == 0)
  {
    Serial.print("T: ");
    Serial.print(temperature);
    Serial.print(" H: ");
    Serial.println(humidity);    
  }
  
  // Send data to server
  if (client.connect(server_name, 80)) {
    Serial.println("Connected");
    
    Serial.print(F("Sending request... "));
    
    client.print(F("GET /dweet/for/"));
    client.print(thing_name);
    client.print(F("?temperature="));
    client.print(temperature);
    client.print(F("&humidity="));
    client.print(humidity);
    client.print(F("&motion="));
    client.print(motion);
    client.println(F(" HTTP/1.1"));
    
    client.println(F("Host: dweet.io"));
    client.println(F("Connection: close"));
    client.println(F(""));
    
    Serial.println(F("done."));
  }
  
  // Read answer
  Serial.println(F("Reading answer..."));
  while (client.connected()) {
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  }
  Serial.println(F(""));
  
  // Close connection
  client.stop();
  Serial.println(F("Closing connection"));
  Serial.println(F(""));
  
 
}
