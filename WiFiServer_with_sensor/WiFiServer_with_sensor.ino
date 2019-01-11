#include <ESP8266WiFi.h>
#include "DHT.h"         
#include "time.h"
#define DHTTYPE DHT11

const int DHTPin = 5; // D1
const int ledPin = 13; // D7
const char* ssid = "Dom";
const char* password = "Monikasu123456";
float t [1] = {};
float h [1] ={};
DHT dht(DHTPin, DHTTYPE);
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(10);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  float te = dht.readTemperature();
  float hu = dht.readHumidity();
  
  if (isnan(te) || isnan(hu)) {
    Serial.println(" No Data comin :( ");
    delay(50);
  } else {
    t[0] = te;
    h[0] = hu;
    Serial.println(" Receiving Data!");
  }
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
  if (request.indexOf("/REFRESH") != -1) {
  }
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  
  client.print("Led pin is now: ");
  
  if(value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  };
  
  client.println("<br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />"); 
  
  client.println("<br>Temperature: ");
  client.print(t[0]);
  client.print("*C</br>");
  client.println("<br>Humidity: ");
  client.print(h[0]);
  client.print(" %   </br");
  client.println("<br><a href=\"/REFRESH\"\"><button>Refresh</button></a><br />");
  client.println("</html>");
  
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
