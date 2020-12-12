#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

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


// Update these with values suitable for your network.

const char* ssid = "KeyCard";
const char* password = "123456778";
const char* mqtt_server = "192.168.254.100";
//const char* mqtt_server = "192.168.254.101";
WiFiClient espClient;
PubSubClient client(espClient);
#define buttons  D1

int m = 0;
char incom;
char Mang[500];
String buf_s = "";
String teca ="";
int buttonState = 0;
char buf []= " ";

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

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

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
 
  if(teca == "GV"){
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Giang Vien.");
    opendoor();
  }else if(teca == "SV"){
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Sinh vien.");
    opendoor();
  }
  else if(teca == "NV"){
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Nhan vien.");
    opendoor();
  }
  else if(teca == "UK"){
    digitalWrite(D6,1); //LED DO
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("The khong");
    lcd.setCursor(3,1);
    lcd.print("ton tai!");
  }
  else if(teca == "clear"){
     digitalWrite(D5,0); //KHOA CUA
     digitalWrite(D6,0); //LED DO
     digitalWrite(D7,0); //LED XANH
     lcd.clear();
     lcd.setCursor(3,0);
     lcd.print("Chao ban!");
  }
  if(teca != "UK" && teca != "clear"){
    lcd.setCursor(0,1);
    lcd.print("Hi " + teca);
  }
}
void opendoor(){
    digitalWrite(D5, HIGH); //KHOA CUA
    digitalWrite(D7, HIGH); //LED XANH
    digitalWrite(D6, LOW);  //LED DO
}
void turnofflight(){
  if(digitalRead(D0) == 1)
  { 
    digitalWrite(D8,HIGH); // RELAY 1 (sensor)
  }else {
    digitalWrite(D8,LOW);
  }
  
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client-1")) {
      Serial.println("connected");
      client.subscribe("inTopic");
      client.subscribe("nameTopic");
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
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setServer(mqtt_server, 10823);
  client.setCallback(callback);
  pinMode(D8,OUTPUT);
  pinMode(D7,OUTPUT);
  pinMode(D6,OUTPUT);
  pinMode(D5,OUTPUT);
  
  pinMode(buttons,INPUT);//D1
  pinMode(D0,INPUT);
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
   //digitalWrite(D6, HIGH);
   buttonState = digitalRead(buttons);
   if( buttonState == HIGH){
    opendoor();
    client.publish("outTopic", "clear");
   }
   turnofflight();
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
    
    if (client.publish("outTopic", buf) == true) {
      Serial.println("Success sending message");
    } else {
      Serial.println("Error sending message");
    }
   
    client.loop();
    Serial.println("-------------");
    }
}

