#include <SoftwareSerial.h>
#define DEBUG true

SoftwareSerial ser(10, 11); // RX, TX

void setup() {
  // put your setup code here, to run once:
  delay(5000);               
  Serial.begin(115200);
  ser.begin(115200);
  connectWifi();
  Serial.print("TCP/UDP Connection...\n");
  sendCommand("AT+CIPMUX=0\r\n",2000,DEBUG); // reset module
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  sendDataID("90");
  delay(10000);
}

void sendDataID(String id) {
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  
  cmd += "192.168.0.100";
  cmd += "\",80\r\n";
  sendCommand(cmd,1000,DEBUG);
  delay(5000);
  
  String cmd2 = "GET /IMKA/receiver.php?apples="; // Link ke skrip PHP                    
  cmd2 += id;  
  cmd2 += " HTTP/1.1\r\n";
  cmd2 += "Host: 192.168.0.100\r\n\r\n\r\n"; // Host
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
