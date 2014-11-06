#include <PubSubClient.h>

/*
 Basic MQTT example with Authentication
 
  - connects to an MQTT server, providing username
    and password
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

int RightPower = 7;    // Arduino Pin to control relay 1
int RightColor = 6;    // Arduino Pin to control relay 2
int LeftPower = 5;    // Arduino Pin to control relay 3
int LeftColor = 4;    // Arduino Pin to control relay 4

char message_buff[100];
char topic_buff[100];

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0x00, 0x12 };
byte server[] = { 10, 10, 10, 223 };
byte ip[]     = { 10, 10, 10, 177 };

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  
  int i = 0;
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  String msgString = String(message_buff);
  String topicString = String(topic);


  
  if(topicString.substring(14) == "command"){
  
    if(msgString == "0"){ 
      lightsoff();

    }
  
    if(msgString == "1"){
      lightson();


   }
  }
  
  if(topicString.substring(14)== "color"){  //need to swap these in the future for consistency
    if(msgString == "0"){
      color1();
      lightson();


    }
    if(msgString == "1"){
      color2();
      lightson();


    }
  }
}



EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

void setup()
{
    pinMode(RightPower, OUTPUT);  //set relay shield pins
    pinMode(RightColor, OUTPUT);
    pinMode(LeftPower, OUTPUT);  //set relay shield pins
    pinMode(LeftColor, OUTPUT);
    digitalWrite(RightPower,LOW);  //always turn off the relays first
    digitalWrite(RightColor,LOW);
    digitalWrite(LeftPower,LOW);  //always turn off the relays first
    digitalWrite(LeftColor,LOW);
    

    
    Ethernet.begin(mac, ip);
    
    if (client.connect("arduinoClient", "USERNAME", "PASSWORD")) {
      client.publish("aquariumLight/report",(char*)"hello world " + '\0');
      client.subscribe("aquariumLight/#");
      
    }
    
  }

void loop()
{
  int counter = 0;
 
  if (!client.connected()){
    counter++;
    

    if (client.connect("arduinoClient", "USERNAME", "PASSWORD")) {
      client.publish("aquariumLight/report",(char*)"hello world " + counter + '\0');
      client.subscribe("aquariumLight/command");
      client.subscribe("aquariumLight/color");
    }
  }
  
  client.loop();
}

void lightson()
{
  digitalWrite(RightPower,HIGH);
  digitalWrite(LeftPower,HIGH);
}

void lightsoff()
{
  digitalWrite(RightPower,LOW);
  digitalWrite(LeftPower,LOW);
}

void color1()
{
  digitalWrite(RightColor,LOW);
  digitalWrite(LeftColor,LOW);
}

void color2()
{
  digitalWrite(RightColor,HIGH);
  digitalWrite(LeftColor,HIGH);
}
