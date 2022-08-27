
#include <PubSubClient.h>
#include <ESP8266WiFi.h>


//EDIT THESE LINES TO MATCH YOUR SETUP
#define MQTT_SERVER "" //Add IP adress of broker
const char* ssid = "ENTER-WIFI-NAME";
const char* password = "ENTER-WIFI-PASSWORD ";

const int switchPin = 14; //Connect switch to GPIO pin 14. You can use other pins but edit this value in code
char* lightTopic = "/home/room/strip"; //The topic that the publisher will send data to



WiFiClient wifiClient;
PubSubClient client(MQTT_SERVER, 1883, wifiClient);

void setup() {
  //initialize the light as an output and set to LOW (off)
  pinMode(switchPin, INPUT_PULLUP);

  //start the serial line for debugging
  Serial.begin(115200);
  delay(100);


  //start wifi subsystem
  WiFi.begin(ssid, password);
  //attempt to connect to the WIFI network and then connect to the MQTT server
  reconnect();

  //wait a bit before starting the main loop
      delay(2000);
}

int OldState = digitalRead(14);
void loop(){


  //reconnect if connection is lost
  if (!client.connected() && WiFi.status() == 3) {reconnect();}

  //maintain MQTT connection
  client.loop();
  
  int switchValue = digitalRead(14);



  if(switchValue == HIGH and OldState == 0){ //Send data when switch changes state
    client.publish("/home/room/strip", "1");
    Serial.println("sending 1");
    OldState = 1;
    delay(3000);
    
  }

  else if(switchValue == LOW and OldState == 1){
    client.publish("/home/room/strip", "0");
    Serial.println("sending 0");
    OldState = 0;
    delay(3000);
  }

  //MUST delay to allow ESP8266 WIFI functions to run
  delay(10); 
}




void reconnect() {

  //attempt to connect to the wifi if connection is lost
  if(WiFi.status() != WL_CONNECTED){
    //debug printing
    Serial.print("Connecting to ");
    Serial.println(ssid);

    //loop while we wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    //print out some more debug once connected
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  //make sure we are connected to WIFI before attemping to reconnect to MQTT
  if(WiFi.status() == WL_CONNECTED){
  // Loop until we're reconnected to the MQTT server
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");

      // Generate client name based on MAC address and last 8 bits of microsecond counter
      String clientName;
      clientName += "esp8266-";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);

      //if connected, subscribe to the topic(s) we want to be notified about
      if (client.connect((char*) clientName.c_str())) {
        Serial.print("\tMTQQ Connected");
      }
    }
  }
}

//generate unique name from MAC addr
String macToStr(const uint8_t* mac){

  String result;

  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);

    if (i < 5){
      result += ':';
    }
  }

  return result;
}
