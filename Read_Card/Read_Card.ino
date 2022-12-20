#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <HTTPSRedirect.h>

#include<Wire.h>

#define BUZZER   D2

//---------------------------------------------------------------------------------------------------------
// Enter Google Script Deployment ID:
const char *GScriptId = "AKfycbwLWxWH-FinoH1dxAcLl8SagYewkxTvFByrhvcy9TBw5kPx_xpDEA_AxtEncwGdHqMVMA";
//---------------------------------------------------------------------------------------------------------
// Enter network credentials:
const char* ssid     = "Huflit-CB-NV";
const char* password = "huflitcbnv@123";
//---------------------------------------------------------------------------------------------------------
// Enter command (insert_row or append_row) and your Google Sheets sheet name (default is Sheet1):
String payload_base =  "{\"command\": \"insert_row\", \"sheet_name\": \"Sheet1\", \"values\": ";
String payload = "";
//---------------------------------------------------------------------------------------------------------
// Google Sheets setup (do not edit)
const char* host        = "script.google.com";
const int   httpsPort   = 443;
const char* fingerprint = "";
String url = String("/macros/s/") + GScriptId + "/exec";
HTTPSRedirect* client = nullptr;
//------------------------------------------------------------
// Declare variables that will be published to Google Sheets
String student_id;
//------------------------------------------------------------
int blocks[] = {4,5,6,8,9};
#define total_blocks  (sizeof(blocks) / sizeof(blocks[0]))
//------------------------------------------------------------
#define RST_PIN  0  //D3
#define SS_PIN   2  //D4
#define BUZZER   4  //D2
#define ledPin 5 //D1
//------------------------------------------------------------
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;  
MFRC522::StatusCode status;
//------------------------------------------------------------
/* Be aware of Sector Trailer Blocks */
int blockNum = 2;  

byte bufferLen = 18;
byte readBlockData[18];

byte nuidPICC[4];

//------------------------------------------------------------

/****************************************************************************************************
 * setup Function
****************************************************************************************************/
void setup() {
  //----------------------------------------------------------
  Serial.begin(9600);        
  delay(10);
  Serial.println('\n');
  //----------------------------------------------------------
  SPI.begin();
  //----------------------------------------------------------
  //initialize lcd screen
  
  //----------------------------------------------------------
  // Connect to WiFi
  WiFi.begin(ssid, password);             
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  //----------------------------------------------------------
  // Use HTTPSRedirect class to create a new TLS connection
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");
  //----------------------------------------------------------
  
  delay(5000);
  //----------------------------------------------------------
  Serial.print("Connecting to ");
  Serial.println(host);
  //----------------------------------------------------------
  // Try to connect for a maximum of 5 times
  bool flag = false;
  for(int i=0; i<5; i++){ 
    int retval = client->connect(host, httpsPort);
    //*************************************************
    if (retval == 1){
      flag = true;
      String msg = "Connected. OK";
      Serial.println(msg);
      delay(2000);
      break;
    }
    //*************************************************
    else
      Serial.println("Connection failed. Retrying...");
    //*************************************************
  }
  //----------------------------------------------------------
  if (!flag){
    //____________________________________________
    
    //____________________________________________
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    delay(5000);
    return;
    //____________________________________________
  }
  //----------------------------------------------------------
  delete client;    // delete HTTPSRedirect object
  client = nullptr; // delete HTTPSRedirect object
  //----------------------------------------------------------
  pinMode(BUZZER, OUTPUT);
  pinMode(ledPin, OUTPUT);
}



void loop() {
  //----------------------------------------------------------------
  static bool flag = false;
  if (!flag){
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    flag = true;
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");
  }
  if (client != nullptr){
    if (!client->connected())
      {client->connect(host, httpsPort);}
  }
  else{Serial.println("Error creating client object!");}
  //----------------------------------------------------------------
  
  /* Initialize MFRC522 Module */
  mfrc522.PCD_Init();
  /* Look for new cards */
  /* Reset the loop if no new card is present on RC522 Reader */
  if ( ! mfrc522.PICC_IsNewCardPresent()) {return;}
  /* Select one of the cards */
  if ( ! mfrc522.PICC_ReadCardSerial()) {return;}
  /* Read data from the same block */
  Serial.println();
  Serial.println(F("Reading last data from RFID..."));  
  //----------------------------------------------------------------
  String values = "", data;
  
  //----------------------------------------------------------------*/
  digitalWrite(BUZZER, HIGH);
  delay(200);
  digitalWrite(BUZZER, LOW);
  delay(200);
  digitalWrite(BUZZER, HIGH);
  delay(200);
  digitalWrite(BUZZER, LOW);
  //creating payload - method 2 - More efficient
  for (byte i = 0; i < total_blocks; i++) {
    ReadDataFromBlock(blocks[i], readBlockData);
    //*************************************************
    if(i == 0){
      data = String((char*)readBlockData);
      data.trim();
      student_id = data;
      values = "\"" + data + ",";
    }
    //*************************************************
    else if(i == total_blocks-1){
      data = String((char*)readBlockData);
      data.trim();
      values += data + "\"}";
    }
    //*************************************************
    else{
      data = String((char*)readBlockData);
      data.trim();
      values += data + ",";
    }
  }
 
  payload = payload_base + values;
  if (mfrc522.uid.uidByte[0] != nuidPICC[0] ||
          mfrc522.uid.uidByte[1] != nuidPICC[1] ||
          mfrc522.uid.uidByte[2] != nuidPICC[2] ||
          mfrc522.uid.uidByte[3] != nuidPICC[3] ) {
      Serial.println(F("A new card has been detected."));
      digitalWrite(ledPin,HIGH);
      
      // Store NUID into nuidPICC array
      for (byte i = 0; i < 4; i++) {
          nuidPICC[i] = mfrc522.uid.uidByte[i];
      }
      Serial.println(F("The NUID tag is:"));
      Serial.print(F("In hex: "));
      printHex(mfrc522.uid.uidByte, mfrc522.uid.size);
      Serial.println();
      Serial.print(F("In dec: "));
      printDec(mfrc522.uid.uidByte, mfrc522.uid.size);
      Serial.println();
          }
      
  
  else {Serial.println(F("Card read previously."));
  digitalWrite(ledPin,LOW);}
  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
  //----------------------------------------------------------------
  
  //----------------------------------------------------------------
  // Publish data to Google Sheets
  Serial.println("Publishing data...");
  Serial.println(payload);
  if(client->POST(url, host, payload)){ 
    // do stuff here if publish was successful
   
  }
  //----------------------------------------------------------------
  else{
    // do stuff here if publish was not successful
    Serial.println("Error while connecting");
    
  }
  //----------------------------------------------------------------
  // a delay of several seconds is required before publishing again    
  delay(5000);
}


void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{ 
  //----------------------------------------------------------------------------
  
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  //----------------------------------------------------------------------------
  /* Authenticating the desired data block for Read access using Key A */
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  //----------------------------------------------------------------------------s
  if (status != MFRC522::STATUS_OK){
     Serial.print("Authentication failed for Read: ");
     Serial.println(mfrc522.GetStatusCodeName(status));
     return;
  }
  //----------------------------------------------------------------------------
  else {
    Serial.println("Authentication success");
  }
  //----------------------------------------------------------------------------
  /* Reading data from the Block */
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  //----------------------------------------------------------------------------
  else {
    readBlockData[16] = ' ';
    readBlockData[17] = ' ';
    Serial.println("Block was read successfully");  
  }
  //----------------------------------------------------------------------------
}
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
      Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      Serial.print(buffer[i], HEX);
  }
}
/**
    Helper routine to dump a byte array as dec values to Serial.
*/
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
      Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      Serial.print(buffer[i], DEC);
  }
}
