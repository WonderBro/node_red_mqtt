
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

// Update these with values suitable for your network.
const char* ssid = "AP01";//put your wifi ssid here.
const char* password = "JollyUnicorn310";//put your wifi password here.
const char* mqtt_server = "10.0.0.124";

// Defining Constant MQTT Topics 
const char* topic_pan = "servo/pan";          // Topic MQTT pour servo Pan - Topic MQTT for Pan servor
const char* topic_tilt = "servo/tilt";

// 
Servo pan;
Servo tilt;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length) {
  String string;

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    // Serial.print ((char)payload[i]);
      string+=((char)payload[i]);
  }

      Serial.print(string);

if ( strcmp(topic, topic_pan) == 0 ) {
    Serial.print(" ");
  int result = string.toInt();
  int pos = map(result, 1, 100, 0, 180);  
  Serial.println(pos);
    pan.write(pos);
    delay(15);
} if ( strcmp(topic, topic_tilt) == 0 ) { 
    Serial.print(" ");
  int result = string.toInt();
  int pos = map(result, 1, 100, 0, 180);
  Serial.println(pos);
    tilt.write(pos);
    delay(15);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to    if (client.connect(clientId,userName,passWord))
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe(topic_pan);
      client.subscribe(topic_tilt);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} //end reconnect()

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pan.attach(D2);
  tilt.attach(D3);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}