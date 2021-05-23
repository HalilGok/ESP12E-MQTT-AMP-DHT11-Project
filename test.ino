        /* İSTANBUL'A */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
WiFiClient espClient;
PubSubClient client(espClient);

#define LED_SW D0

const char* WIFI_NAME = "**********";
const char* WIFI_PASSWORD = "**********";
const int port = 1883;
const char* IP = "**********";
const char* userName = "**********";
const char* password = "**********";
char msgMqtt[50];

void ShowLCD();
void MQTT_Server_Connect();
void call_Back(char* topic, byte* payload, unsigned int length);

String temperature ="";
String humidity="";

void setup(){
  pinMode(LED_SW,OUTPUT);
  digitalWrite(LED_SW,LOW);
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
  lcd.begin();
}

void loop(){
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
  
  if (((String)topic == "/gokhalil723@gmail.com/TEMPERATURE"))
  {
    temperature=MQTT_DATA;
    ShowLCD();
    MQTT_DATA ="";
    delay(100);
     (String)topic = "";  
  }  
    if (((String)topic == "/gokhalil723@gmail.com/HUMIDITY"))
  {
    humidity=MQTT_DATA;
    ShowLCD();
    MQTT_DATA ="";
    delay(100);
  (String)topic = "";  
  } 
   if ((MQTT_DATA == "LED ON") && ((String)topic == "/gokhalil723@gmail.com/LED")) 
   {
    digitalWrite(LED_SW,HIGH); //  because the led is connected in reverse
    delay(50);
   (String)topic = ""; 
   MQTT_DATA="";
  }
  if ((MQTT_DATA =="LED OFF")  && ((String)topic == "/gokhalil723@gmail.com/LED")){
    digitalWrite(LED_SW,LOW);
    delay(50);
    
   (String)topic = "";  
    MQTT_DATA="";
  }
 
   (String)topic = ""; 
    MQTT_DATA="";
}

void ShowLCD(){ 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TEMPERATURE: ");
  lcd.setCursor(13,0);
  lcd.print(temperature);
 
  lcd.setCursor(0,1);
  lcd.print("HUMIDITY : ");
  lcd.setCursor(11,1);
  lcd.print("%");
  lcd.setCursor(12,1);
  lcd.print(humidity);
  delay(300);
}
