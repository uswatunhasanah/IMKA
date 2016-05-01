// Keypad
#include <Keypad.h>
#include <Password.h>

//ESP8266
#include <SoftwareSerial.h>
#define DEBUG true

SoftwareSerial esp8266(10, 11); // RX, TX

// Motion Detector
int pd=12;                      //Photodiode to digital pin 12
int senRead=0;                 //Readings from sensor to analog pin 0  
int limit=850;                 //Threshold range of an obstacle  

// Password Keypad
String newPasswordString; //hold the new password
char newPassword[4]; //charater string of newPasswordString
byte maxPasswordLength = 4; 
byte currentPasswordLength = 0;
const byte ROWS = 4; // four rows
const byte COLS = 2; // four columns
//initialize password to 1234
//you can use password.set(newPassword) to overwrite it
Password password = Password( "1234" );
//define the keymap
char keys [ROWS] [COLS] = {
  {'1', '2'},
  {'3', '4'},
  {'5', '6'},
  {'R', 'C'}
};
byte rowPins[ROWS] = {
  9, 8, 7 ,6}; //connect keypad ROW1, ROW2, ROW3, ROW4 to these arduino pins
byte colPins[COLS] = {
  5, 4}; //connect keypad COL1, COL2, COL3, COL4 to these arduino pins
//create the keypad
Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
char key;

// MOTOR
int motorPin1 = 2;
int motorPin2 = 3;
bool doorLock = true;
  
// Variable pendukung
int val = 0; // variable to store values from the photodiode
bool keyPress;
bool passwordTrue = false;
bool alertOn = false;
int passkeyInt = 1234;

void setup() {
  // put your setup code here, to run once:
  // WIFI Configuration
  delay(5000);               
  Serial.begin(115200);
  esp8266.begin(115200);
  connectWifi(); // Connect to wifi

  // MOTOR Configuration
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  
  // Motion Detector
  pinMode(pd,OUTPUT);  
  pinMode(13,OUTPUT);  
  digitalWrite(pd,HIGH);     //supply 5 volts to photodiode  
  digitalWrite(13,LOW);      //set the LED in off mode (initial condition)  
  delay(5000);

  digitalWrite(13,HIGH);
  sendDataID("0",String(passkeyInt)); // Declare safe
  digitalWrite(13,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  key = myKeypad.getKey();

  if (key != NO_KEY){
    // KEYPAD Detection
    keyPress = true;
    while (keyPress) {
      if (key != NO_KEY){
        delay(60); 
        switch (key){
          case 'R': {
                 changePassword(); 
                 if (alertOn == true) {
                    digitalWrite(13,HIGH);     // LED On, indicate sending data
                    sendDataID("1",String(passkeyInt));
                    digitalWrite(13,LOW);     // LED Off, indicate sending data has done
                 } else if (alertOn == false) {
                    digitalWrite(13,HIGH);     // LED On, indicate sending data
                    sendDataID("0",String(passkeyInt));
                    digitalWrite(13,LOW);     // LED Off, indicate sending data has done
                 }
                 break; 
          }
          case 'C': {
                 checkPassword(); 
                 break;
          }
          default: processNumberKey(key);
        }
      }
      key = myKeypad.getKey();
    }
    if ((alertOn == true) && (passwordTrue == true)) {
      // Send data "safe"
      Serial.println("send data aman");
      digitalWrite(13,HIGH);     // LED On, indicate sending data
      sendDataID("0",String(passkeyInt));
      digitalWrite(13,LOW);     // LED Off, indicate sending data has done
            
      alertOn = false;
      passwordTrue = false;
    } else {
               
    }
  } else { 
    // Motion Detection
    val = analogRead(senRead);
    if(val <= limit)              //If obstacle is nearer than the Threshold range  
    {  
	    // Tidak terdeteksi gerakan
      delay(20);  
    } else if(val > limit)          //If obstacle is not in Threshold range  
    {  
	    // Terdeteksi gerakan
      if (alertOn==false) {
        Serial.println("send data bahaya");
        alertOn = true;
        changePassword();
        digitalWrite(13,HIGH);     // LED On, indicate sending data
        sendDataID("1",String(passkeyInt));
        digitalWrite(13,LOW);     // LED Off, indicate sending data has done
      } else {

      }
	    delay(20);  
    }
  }
}

void processNumberKey(char key) {
   Serial.print(key);
   currentPasswordLength++;
   password.append(key);
   if (currentPasswordLength == maxPasswordLength) {
      checkPassword();
   } 
}

void checkPassword() {
   if (password.evaluate()){
	   // Password true
     Serial.println(" OK.");
	   // Door unlocked
     Serial.println("Door OPEN");
     digitalWrite(motorPin1, LOW);
     digitalWrite(motorPin2, HIGH);
     delay(300);
     digitalWrite(motorPin1, LOW);
     digitalWrite(motorPin2, LOW);
	   delay(5000);
	   // Door locked
     Serial.println("Door CLOSED");
     digitalWrite(motorPin1, HIGH);
     digitalWrite(motorPin2, LOW);
     delay(300);
     digitalWrite(motorPin1, LOW);
     digitalWrite(motorPin2, LOW); 
     passwordTrue = true;
   } else {
     Serial.println(" Wrong passwowrd!");
     passwordTrue = false;
   } 
   resetPassword();
}

void resetPassword() {
   password.reset(); 
   currentPasswordLength = 0; 
   keyPress = false;
}

void changePassword() { 

   int digitNumber = 0;
   int satuan = random(1, 6);
   int puluhan = random(1, 6) * 10;
   int ratusan = random(1, 6) * 100;
   int ribuan = random(1, 6) * 1000;
   digitNumber = satuan + puluhan + ratusan + ribuan;
   passkeyInt = digitNumber;
   newPasswordString = String(digitNumber);
  
   newPasswordString.toCharArray(newPassword, newPasswordString.length()+1); //convert string to char array
   password.set(newPassword);
   resetPassword();
   Serial.print("Password changed to ");
   Serial.println(passkeyInt);
}

void sendDataID(String id, String pass) {
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "192.168.0.103";
  cmd += "\",80\r\n";
  sendCommand(cmd,1000,DEBUG);
  delay(5000);
  
  String cmd2 = "GET /IMKA/receiver.php?info="; // Link ke skrip PHP                    
  cmd2 += id;  
  cmd2 += "&pass=";
  cmd2 += pass;
  cmd2 += " HTTP/1.1\r\n";
  cmd2 += "Host: 192.168.0.103\r\n\r\n\r\n"; // Host
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
      
  esp8266.print(command); // send the read character to the esp8266
      
  long int time = millis();
      
  while( (time+timeout) > millis()) {
    while(esp8266.available()) {
      // The esp has data so display its output to the serial window 
      char c = esp8266.read(); // read the next character.
      response+=c;
    }  
  }
      
/*  if(debug) {
    Serial.print(response);
  } */
      
  return response;
}

void connectWifi() {
  //Set-wifi
  Serial.println("Restart Module...");
  sendCommand("AT+RST\r\n",2000,DEBUG); // reset module
  delay(5000);
  Serial.println("Set wifi mode : STA...");
  sendCommand("AT+CWMODE=1\r\n",1000,DEBUG); // configure as access point
  delay(5000);
  Serial.println("Connect to access point...");
  sendCommand("AT+CWJAP=\"TP-LINK_D7425A\",\"syahrini\"\r\n",3000,DEBUG);
  delay(5000);
  Serial.println("Check IP Address...");
  sendCommand("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  delay(5000);
  Serial.println("TCP/UDP Connection...");
  sendCommand("AT+CIPMUX=0\r\n",2000,DEBUG); // reset module
  delay(5000);
}
