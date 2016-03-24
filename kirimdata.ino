#include <SoftwareSerial.h>
#include <stdlib.h>

int ledPin = 13;
int lm35Pin = 0;
String apiKey = "EUILCPS5SLZNV8XR"; // Ubah dengan API Key ente bro!
SoftwareSerial ser(10, 11); // RX, TX

void setup() {               
  pinMode(ledPin, OUTPUT);   
  Serial.begin(9600);
  ser.begin(115200);
  ser.println("AT+RST");
}

void loop() {
 
  digitalWrite(ledPin, HIGH);  
  delay(200);              
  digitalWrite(ledPin, LOW);

  int val = 0;
  for(int i = 0; i < 10; i++) {
      val += analogRead(lm35Pin);  
      delay(500);
  }
  float temp = val*50.0f/1023.0f;
  char buf[16];
  String strTemp = dtostrf(temp, 4, 1, buf);
  Serial.println(strTemp);

  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
  Serial.println(cmd);
  
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
 
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(strTemp);
  getStr += " HTTP/1.0\r\nHost: www.thingspeak.com\r\n\r\n";

  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);
  Serial.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
    Serial.println(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    Serial.println("AT+CIPCLOSE");
  }
    delay(6000); 
}
