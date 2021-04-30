#include <MQ2.h>                               //Header file for read the data from MQ2
#include <ESP8266WiFi.h>                      // Header file for ESP8266 Nodumcq

int pinAout = A0;                            //Input pin for collecting data from MQ2
int lpg_gas, co_gas, smoke_gas;             // var for lpg , co and smoke

MQ2 mq2(pinAout);                          
String apiKey = "C1OC7MRVWJ7GOYKQ";       //  Write API key from ThingSpeak
const char* ssid =  "espx";              //   Enter nama wifi 
const char* pass =  "123456789";        //    Enter Password wifi 
const char* server = "api.thingspeak.com";
WiFiClient client;


void setup()
{
  Serial.begin(115200);
  mq2.begin();
  delay(10);


  Serial.println("Connecting to ");
  Serial.println(ssid);


  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print("*");
  }

  Serial.println("");
  Serial.println("***WiFi Connected***");

}

void loop() 
{

  if (client.connect(server, 80))                //   "184.106.153.149" or api.thingspeak.com
  {
    String sendData = apiKey + "&field1=" + String(lpg_gas) + "&field2=" + String(co_gas) + "&field3=" + String(smoke_gas) + "\r\n\r\n";

    //Serial.println(sendData);

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(sendData.length());
    client.print("\n\n");
    client.print(sendData);

    float* values = mq2.read(false); 
    lpg_gas = mq2.readLPG();
    co_gas = mq2.readCO();
    smoke_gas = mq2.readSmoke();

    Serial.print("LPG:");
    Serial.print(lpg_gas);
    Serial.print("\n");
    Serial.print(" CO:");
    Serial.print(co_gas);
    Serial.print("\n");
    Serial.print("SMOKE:");
    Serial.println(smoke_gas);
    Serial.print("\n");

    Serial.println("%. Data send to Thingspeak.");


  }

  client.stop();
  Serial.println("send data....");

  delay(2000); 
}
