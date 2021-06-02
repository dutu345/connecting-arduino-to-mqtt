#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
 
/*Then, we declare the constant variables which we will need later, setting them with the proper values . */
 
const char* ssid = "UPC8391611"; /*Network name*/
const char* pass = "Cjhf8wsfunur"; /* router pasword */
const char* brokerUser = "interviu.practica";
const char* brokerPass = "beiapractica";
const char* broker = "mqtt.beia-telemetrie.ro";  /*mqtt server */
const char* outTopic = "/training/arduino/Dutu_Daniel_Marian/out";
const char* inTopic = "/training/arduino/Dutu_Daniel_Marian/in";
boolean boot = true;
 
WiFiClient espClient;
PubSubClient client(espClient);
long currentTime, lastTime;
int count = 0;  /*counter*/
char messages[50];
 
const int ledPin = 2;
 
boolean setupWiFi()
{
  int retries = 0; /*initializing number of retries with 0 */
  delay(100);  /*delay in ms */
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, pass);
 
  while(WiFi.status() != WL_CONNECTED && retries <10) /*while is not connected to wifi*/
  {
	delay(100);
	Serial.print("-");
	retries++;  /*the number of retries is increased */
	
  }
  Serial.print("\nConnected to"); /*otherwise the connected message is displayed*/
  Serial.println(ssid);   /*display the Router Name*/
  Serial.println(WiFi.localIP()); /*display router IP */
  return true;
 
}
 
 
void reconnect()  /*void function that reconnects to the broker server*/
{
  while(!client.connected())
  {
	Serial.print("\nConnecting to"); /*the displayed message while attempting to connect*/
	Serial.println(broker); /*display broker “mqtt.beia-telemetrie.ro” */
	if(client.connect(brokerUser, brokerPass)) /* if login datas are correct */
	{
  	Serial.print("\nConnected to"); /*is displayed the broker and topic to which the device is connected */
  	Serial.println(broker);
  	client.subscribe(inTopic);
	}
	else
	{
  	Serial.println("\nTrying to connect again"); /*otherwise this message is displayed*/
  	delay(5000);
	}
	
	
  }
}
 
 
void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Received messages; ");
  Serial.println(topic);
  for(int i=0; i<length; i++)
  {
	Serial.print((char) payload[i]);  // Extracts the contents of the payload as a string and prints it
  }
  Serial.println();
}
 
void setup()
{
  // here can be put the setup code for running once:
Serial.begin(115200);
 
 
pinMode(ledPin, OUTPUT);
 
}
 
void loop() {
  // here comes the main code, to run repeatedly:
if (boot == true)
{
  if (setupWifi())   /* the setupWifi function is called, which returns a Boolean value */
  {
 	client.setServer(broker, 1883);
 	client.setCallback(callback);
  }
 
  boot = false;
}
if (!client.connected())  /*if the client is not connected*/
{
  reconnect(); /* the reconnect function is called */
}
client.loop();
digitalWrite(ledPin, HIGH); // turn the LED on (HIGH is the voltage level)
delay(1000);            	// 1 second delay
digitalWrite(ledPin, LOW);
delay(1000);
 
currentTime = millis();
if(currentTime - lastTime > 2000)
{
  count++;
  snprintf(messages, 75, "%ld", count);
  Serial.print("Sending messages: ");
  Serial.println(messages);
  client.publish(outTopic, messages);
  lastTime = millis();
}
 
 
}

