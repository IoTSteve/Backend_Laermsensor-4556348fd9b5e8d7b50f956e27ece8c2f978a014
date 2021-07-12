#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>




//Smoothing variables

float EMA_function(float alpha, int latest, int stored);

int sensor_pin = A0;

float ema_a = 0.06;
int ema_ema = 0;
int ema = 0;
int DEMA = 0;

const int numReadings = 5000;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int offsetval = 0;              // the average that has been offset by a given ammount to pull it to zero
int seconds = 30;



// Update these with values suitable for your network.


const char* ssid = "FRITZ!Box 6591 Cable TQ";
const char* password = "12006358710297476723";
const char* mqtt_server = "mqtt.hfg.design";
String clientId = "";
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    clientId = WiFi.macAddress();
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  /*for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
    }*/

}



void loop() {

  int sensor_value = analogRead(sensor_pin);

  ema = EMA_function(ema_a, sensor_value, ema);
  ema_ema = EMA_function(ema_a, ema, ema_ema);

  int DEMA = 2 * ema - ema_ema;
  int offsetDEMA = abs(DEMA - 400);

  delay(1);
  //Serial.println(DEMA);
  /*int currentReading = abs(DEMA);
    total = total + currentReading;
    readIndex = readIndex + 1;


    if (readIndex >= numReadings) {
    average = total / numReadings;
    //Serial.println(average - offsetval);

    total = 0;
    readIndex = 0;
    }*/
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > seconds * 1000) {
    lastMsg = now;
    //++value;

    snprintf (msg, MSG_BUFFER_SIZE, "%d", offsetDEMA);
    char output[48];
    StaticJsonDocument<48> doc;
  

    doc["dB"] = msg;
    doc["MAC"] = clientId.c_str();



    serializeJson(doc, output);




    Serial.print("Publish message: ");
    Serial.println(msg);
    //Serial.print("mac adresse: ");
    //Serial.println(WiFi.macAddress());

    client.publish("/sweavs/steve/soundlevel", output);
  }
  
}

float EMA_function(float alpha, int latest, int stored) {
  return round(alpha * latest) + round((1 - alpha) * stored);

}


String mac2String(byte ar[]) {
  String s;
  for (byte i = 0; i < 6; ++i)
  {
    char buf[3];
    sprintf(buf, "%02X", ar[i]); // J-M-L: slight modification, added the 0 in the format for padding 
    s += buf;
    if (i < 5) s += ':';
  }
  return s;
}
