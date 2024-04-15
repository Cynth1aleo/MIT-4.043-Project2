// ----------------------------- RGB setup -------------------------------
#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
int buttonState = 0;
const int button = 8;

// ----------------------------- NFC setup -------------------------------
#include <SoftwareSerial.h>
#include <PN532_SWHSU.h>
#include <PN532.h>

SoftwareSerial SWSerial( 3, 2 ); // RX, TX
PN532_SWHSU pn532swhsu( SWSerial );
PN532 nfc( pn532swhsu );
String tagId = "None", dispTag = "None";
byte nuidPICC[4];


void setup(void)
{
  Serial.begin(115200);
  Serial.println("Welcome to Magic Color Picker!");
  pinMode(button, INPUT);

  //--------------degub rgb----------------
  if (!tcs.begin()) {
        Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

  nfc.begin();
  //-----------debug NFC-----------------
  if (!nfc.getFirmwareVersion())
  {
    Serial.print("Didn't Find PN53x Module");
    while (1); // Halt
  }
   nfc.SAMConfig();

}
 

void loop()
{
  readNFC();
   float red, green, blue;
  buttonState = digitalRead(8);

 //if the button is pushed, start to pick the color
  if (buttonState == 1) {
  tcs.setInterrupt(false);  // turn on LED
  // delay(60);  // takes 50ms to read
  tcs.getRGB(&red, &green, &blue);
  tcs.setInterrupt(true);  // turn off LED

//print the color RGB
  Serial.print(int(red));
  Serial.print(",");
  Serial.print(int(green));
  Serial.print(",");
  Serial.println(int(blue));
  // Serial.print("\tButton:\t"); Serial.print(int(buttonState));
  }
}
 

void readNFC()
{
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                       // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  if (success)
  {
    // Serial.print("UID Length: ");
    // Serial.print(uidLength, DEC);
    // Serial.println(" bytes");
    // Serial.print("UID Value: ");
    for (uint8_t i = 0; i < uidLength; i++)
    {
      nuidPICC[i] = uid[i];
      // Serial.print(" "); Serial.print(uid[i], DEC);
    }
    Serial.println();
    tagId = tagToString(nuidPICC);
    dispTag = tagId;
    // Serial.print(F("tagId : "));
    Serial.println(tagId);
    
    // Serial.print(F("Theme : "));
    if(tagId == "4.249.147.133"){
      Serial.println("Travel Destination");
    } else if (tagId == "4.82.229.133"){
      Serial.println("Flower Design");
    } else if ((tagId == "4.85.105.133")){
      Serial.println("Home Decoration");
    }

    Serial.println("");
    delay(2000);  // 1 second halt
  }


}
String tagToString(byte id[4])
{
  String tagId = "";
  for (byte i = 0; i < 4; i++)
  {
    if (i < 3) tagId += String(id[i]) + ".";
    else tagId += String(id[i]);
  }
  return tagId;
}