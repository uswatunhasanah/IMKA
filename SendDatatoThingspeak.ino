#include <SoftwareSerial.h>
#include <stdlib.h>
#define DEBUG true

int ledPin = 13;
int lm35Pin = 0;
String apiKey = "UIZIQ0Z8Z947DRC8"; // Ubah dengan API Key ente bro!
SoftwareSerial ser(10, 11); // RX, TX

void setup() {
  delay(5000);               
  pinMode(ledPin, OUTPUT);   
  Serial.begin(115200);
  ser.begin(115200);
  connectWifi();
  Serial.print("TCP/UDP Connection...\n");
  sendCommand("AT+CIPMUX=0\r\n",2000,DEBUG); // reset module
  delay(5000);
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
 
  sendDataID(strTemp);
  delay(5000);
}

void sendDataID(String id) {
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  
  cmd += "184.106.153.149";
  cmd += "\",80\r\n";
  sendCommand(cmd,1000,DEBUG);
  delay(5000);
  
  String cmd2 = "GET /update?api_key="; // Link ke skrip thingspeak
  cmd2 += apiKey;
  cmd2 += "&field1=";
  cmd2 += id;  
  cmd2 += " HTTP/1.1\r\n";
  cmd2 += "Host: 184.106.153.149\r\n\r\n\r\n"; // Host
  String pjg="AT+CIPSEND=";
  pjg += cmd2.length();
  pjg += "\r\n";
    
  sendCommand(pjg,1000,DEBUG);
  delay(500);
  sendCommand(cmd2,1000,DEBUG);
  delay(5000);
}

String sendCommand(String command, const int timeout, boolean debug) {
  String response = "";
      
  ser.print(command); // send the read character to the esp8266
      
  long int time = millis();
      
  while( (time+timeout) > millis()) {
    while(ser.available()) {
      // The esp has data so display its output to the serial window 
      char c = ser.read(); // read the next character.
      response+=c;
    }  
  }
      
  if(debug) {
    Serial.print(response);
  }
      
  return response;
}


void connectWifi() {
  //Set-wifi
  Serial.print("Restart Module...\n");
  sendCommand("AT+RST\r\n",2000,DEBUG); // reset module
  delay(5000);
  Serial.print("Set wifi mode : STA...\n");
  sendCommand("AT+CWMODE=1\r\n",1000,DEBUG); // configure as access point
  delay(5000);
  Serial.print("Connect to access point...\n");
  sendCommand("AT+CWJAP=\"TP-LINK_D7425A\",\"syahrini\"\r\n",3000,DEBUG);
  delay(5000);
  Serial.print("Check IP Address...\n");
  sendCommand("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  delay(5000);
}
