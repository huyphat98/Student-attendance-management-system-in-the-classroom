#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
LiquidCrystal_I2C lcd(0x3F,16,2);

byte degree[8] =
{
  0B01110,
  0B01010,
  0B01110,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000
};

char* ssid_ap = "KeyCard";
char* pass_ap = "123456778";
IPAddress ip_ap(192,168,254,1);
IPAddress gateway_ap(192,168,254,1);
IPAddress subnet_ap(255,255,255,0);

// Update these with values suitable for your network.
const char* mqtt_server = "192.168.254.100";
const uint16_t mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
int m = 0;
char incom;
char Mang[500];
String buf_s = "";
String teca ="";
char buf []= " ";
#define ledxanh D7
#define leddo D8

void callback(char* topic, byte* payload, unsigned int length) {
  teca = "";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    teca += (char)payload[i];
  }
  Serial.println();
  Serial.println("TeCa");
  Serial.print(teca);
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
//    String clientId = "ESP8266Client-";
//    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "XXX");
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
  EEPROM.begin(512);       //Khởi tạo bộ nhớ EEPROM
  delay(10);
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip_ap, gateway_ap, subnet_ap);
  WiFi.softAP(ssid_ap,pass_ap,1,false);
  Serial.println("Soft Access Point mode!");
  Serial.print("Please connect to ");
  Serial.println(ssid_ap);
  Serial.print("Web Server IP Address: ");
  Serial.println(ip_ap);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
 
  pinMode(D8,OUTPUT);
  pinMode(D7,OUTPUT);
  lcd.begin();
  Wire.begin(2, 0);           //Bắt đầu 2 chân SDA và SCK của I2C
  lcd.clear();
  lcd.backlight();            // Bật đèn nền
  lcd.home();                 //Đưa con trỏ về vị trí 0,0
  lcd.setCursor(3,0);
  lcd.print("Chao ban!");
  //lcd.display();              // Hiển thị lên màn hình.
  //lcd.blink();                // Nhấp nháy con trỏ ở vị trí cuối cùng
  lcd.createChar(0, degree);
}

void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  if(Serial.available()){
    //-------------------------------------------------------------
    buf_s = Serial.readString();
    Serial.println(buf_s);
    for (byte len = 1;len<=buf_s.length()+1; len++){
      buf_s.toCharArray(buf,len);
    }
    //-------------------------------------------------------------
    
    Serial.println("Sending message to MQTT topic..");
    
    if (client.publish("UID", buf) == true) {
      Serial.println("Success sending message");
      digitalWrite(ledxanh,HIGH);
      digitalWrite(leddo,LOW);
    } else {
      Serial.println("Error sending message");
      digitalWrite(leddo,HIGH);
      digitalWrite(ledxanh,LOW);
    }
   
    client.loop();
    Serial.println("-------------");
    }
}

