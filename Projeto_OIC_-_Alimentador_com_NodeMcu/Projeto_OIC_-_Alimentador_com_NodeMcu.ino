#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//Equipamentos utilizados
//NodeMcu V3
//Servo Motor SG90
//Sensor infravermelho reflexivo de obstáculo HW-201
//Led de alto brilho


// Configuracao do led
#define pinoLed D1
// Configuracao do Sensor
#define pinoSensor D4

// Configuracao do servo
#include <Servo.h> 
Servo s; 
int pos; 
#define pinoServo D3

// Setup WIFI e MQTT
const char* ssid = "RedeTeste"; //  nome da rede wifi
const char* password = "password"; // senha da rede wifi
const char* mqtt_server = "driver.cloudmqtt.com";
const int mqtt_port = 18819 ; // di
const char* mqtt_user = "apheflog";
const char* mqtt_password = "zSaFjeN3KUG6";
const char* mqtt_topic = "alimentador/status";

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  pinMode(pinoLed, OUTPUT);
  pinMode(pinoSensor,INPUT);
  s.attach(pinoServo);
  s.write(0);


  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  delay(10000);
  girar_servos();
  verifica_sensor();
  delay(60000);
  }


//FUNCOES AUXILIARES

void verifica_sensor(){
  if (digitalRead(pinoSensor) == 1){
    client.publish(mqtt_topic, "1",true);
    digitalWrite(pinoLed, HIGH);
  }
  else {
    client.publish(mqtt_topic, "0",true);
    digitalWrite(pinoLed, LOW);
  }
}

void girar_servos(){ // Gira o servo 180 graus aguarda 1 segundo e retorna a posição inicial
  for (pos = 0; pos < 180; pos++) { 
    s.write(pos); 
    delay(15); 
  }
  delay(1000); 
    for (pos = 180; pos >= 0; pos--) {
      s.write(pos); 
      delay(15);
   }
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(2000);
    }
  }
}
