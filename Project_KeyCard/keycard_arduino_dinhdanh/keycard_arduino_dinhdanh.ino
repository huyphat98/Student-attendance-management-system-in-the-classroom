//*************************** Create By Huy Phat *******************************

/* Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

#include <SPI.h>
#include <MFRC522.h>
#include <string.h>

#define RST_PIN   9     // Configurable, see typical pin layout above
#define SS_PIN    10    // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;

 byte dataBlock[5] ;
 byte sector         = 1;
 byte blockAddr      = 4;
 byte trailerBlock   = 7;
 MFRC522::StatusCode status;
 byte buffer[18];
 byte size = sizeof(buffer);
 String receiveData;
 char ReceiveComplete=0;
 boolean RxWriteData= false;
#include <SoftwareSerial.h>
String str1 = "";
String str2 = "";
 
void setup()
{
    pinMode(12, OUTPUT);
    Serial.begin(115200);
    while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();        // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522 card
    
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
}
 
void loop()
{
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;     
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;
    //Serial.print("@ID");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        str1 += String(mfrc522.uid.uidByte[i], HEX);
                 
    }
    Serial.print(str1);
    //Serial.print("#");
    str1 = "";       
    delay(400);
}
