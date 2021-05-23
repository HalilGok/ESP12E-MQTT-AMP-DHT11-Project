#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <dht11.h>

WiFiClient espClient;
PubSubClient client(espClient);
dht11 DHT11;

const char* WIFI_NAME = "***";
const char* WIFI_PASSWORD = "***";
const int port = 1883;
const char* IP = "***";
const char* userName = "***";
const char* password = "***";
const int DHT11Pin = 12;
const int LedPin = 2;
char msgMqtt[50];

void MQTT_Server_Connect();
void call_Back(char* topic, byte* payload, unsigned int length);
float dht11_read_Temp(int pin);
float dht11_read_Hum(int pin);

void setup() {
  pinMode(LedPin,OUTPUT);
  digitalWrite(LedPin,HIGH);
  Serial.begin(9600);
  WiFi.disconnect();
  delay(2000);
  Serial.println("Starting");
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (!(WiFi.status() == WL_CONNECTED))
  {
    delay(300);
    Serial.print("...");
  }
  Serial.println("intertnet connected..");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP().toString()));
  client.setServer(IP, port);
  client.setCallback(call_Back);

}
void loop() {

  if(!client.connected())
  {
    MQTT_Server_Connect();
 
  }
  client.loop();

}

void MQTT_Server_Connect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    String clientID = "ESP8266Client-";
    clientID += String(random(0xffff), HEX);
    if (client.connect(clientID.c_str(),userName,password))
    {
      Serial.println("Broker is connected");
      client.subscribe("/gokhalil723@gmail.com/LED");
      client.subscribe("/gokhalil723@gmail.com/TEMPERATURE");
      client.subscribe("/gokhalil723@gmail.com/HUMIDITY");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(2000);
    }
  }
}
void call_Back(char* topic, byte* payload, unsigned int length) 
{ 
  String MQTT_DATA ="";
  for(int i=0;i<length;i++)
  {
    MQTT_DATA +=(char)payload[i];  
  }
  
  if ((MQTT_DATA == "TEMPERATURE") && ((String)topic == "/gokhalil723@gmail.com/TEMPERATURE") )
  {
    snprintf(msgMqtt ,50,"%f",dht11_read_Temp(DHT11Pin));
    client.publish("/gokhalil723@gmail.com/TEMPERATURE",msgMqtt);
    MQTT_DATA ="";
    delay(100);
     (String)topic = "";  
  }
    if ((MQTT_DATA == "HUMIDITY") && ((String)topic == "/gokhalil723@gmail.com/HUMIDITY")  )
  {
    snprintf(msgMqtt ,50,"%f",dht11_read_Hum(DHT11Pin));
    client.publish("/gokhalil723@gmail.com/HUMIDITY",msgMqtt);
    MQTT_DATA ="";
    delay(100);
  (String)topic = "";  
  }
   if ((MQTT_DATA == "LED ON") && ((String)topic == "/gokhalil723@gmail.com/LED")) 
   {
    digitalWrite(LedPin,LOW); //  because the led is connected in reverse
    delay(50);
   (String)topic = ""; 
   MQTT_DATA="";
  }
  if ((MQTT_DATA =="LED OFF")  && ((String)topic == "/gokhalil723@gmail.com/LED") ) {
    digitalWrite(LedPin,HIGH);
    delay(50);
    
   (String)topic = "";  
    MQTT_DATA="";
  }
 
   (String)topic = ""; 
    MQTT_DATA="";
}

float dht11_read_Temp(int pin)
{
   int chk = DHT11.read(DHT11Pin);
   return ((float)DHT11.temperature);
}
float dht11_read_Hum(int pin)
{
   int chk = DHT11.read(DHT11Pin);
   return ((float)DHT11.humidity);
}
