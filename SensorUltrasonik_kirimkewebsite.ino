//ESP8266
#include <SoftwareSerial.h\>
#define DEBUG true
SoftwareSerial esp8266(10, 11); // RX, TX// Sensor Ultrasonic
const int trigPin = 12;
const int echoPin = 13;// Variable pendukung
long sensor;
long sensorRef = 0;
void setup() {
// put your setup code here, to run once:
delay(5000);
Serial.begin(115200);
esp8266.begin(115200);
connectWifi(); // Connect to wifi
delay(5000);
}

void loop() {
// put your main code here, to run repeatedly:

// Deteksi ultrasonic
sensor = ultrasonicSensor();
if (sensor == sensorRef) {
// Tidak terdeteksi apa-apa
Serial.println(“status :: AMAN”);
sendDataID(“0″,”1234”);
} else {
// Terdeteksi sesuatu
Serial.print(“status :: BAHAYA TERDETEKSI”);
sendDataID(“1″,”0000”);
}
sensorRef = sensor;
}

long ultrasonicSensor() {
// establish variables for duration of the ping,
// and the distance result in inches and centimeters:
long duration, cm;

// The sensor is triggered by a HIGH pulse of 10 or more microseconds.
// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
pinMode(trigPin, OUTPUT);
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Read the signal from the sensor: a HIGH pulse whose
// duration is the time (in microseconds) from the sending
// of the ping to the reception of its echo off of an object.
pinMode(echoPin, INPUT);
duration = pulseIn(echoPin, HIGH);

// convert the time into a distance
cm = microsecondsToCentimeters(duration);
cm = cm + 1;

Serial.print(cm);
Serial.print(“cm”);
Serial.println();

return (cm);
}

long microsecondsToCentimeters(long microseconds)
{
// The speed of sound is 340 m/s or 29 microseconds per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
return microseconds / 29 / 2;
}

void sendDataID(String id, String pass) {
String cmd = “AT+CIPSTART=\”TCP\”,\””;

cmd += “192.168.0.101”;
cmd += “\”,80\r\n”;
sendCommand(cmd,1000,DEBUG);
delay(5000);

String cmd2 = “GET /IMKA/receiver.php?info=”; // Link ke skrip PHP
cmd2 += id;
cmd2 += “&pass=”;
cmd2 += pass;
cmd2 += ” HTTP/1.1\r\n”;
cmd2 += “Host: 192.168.0.101\r\n\r\n\r\n”; // Host
String pjg=”AT+CIPSEND=”;
pjg += cmd2.length();
pjg += “\r\n”;

sendCommand(pjg,1000,DEBUG);
delay(500);
sendCommand(cmd2,1000,DEBUG);
delay(5000);
}

String sendCommand(String command, const int timeout, boolean debug) {
String response = “”;

esp8266.print(command); // send the read character to the esp8266

long int time = millis();

while( (time+timeout) > millis()) {
while(esp8266.available()) {
// The esp has data so display its output to the serial window
char c = esp8266.read(); // read the next character.
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
Serial.print(“Restart Module…\n”);
sendCommand(“AT+RST\r\n”,2000,DEBUG); // reset module
delay(5000);
Serial.print(“Set wifi mode : STA…\n”);
sendCommand(“AT+CWMODE=1\r\n”,1000,DEBUG); // configure as access point
delay(5000);
Serial.print(“Connect to access point…\n”);
sendCommand(“AT+CWJAP=\”TP-LINK_D7425A\”,\”syahrini\”\r\n”,3000,DEBUG);
delay(5000);
Serial.print(“Check IP Address…\n”);
sendCommand(“AT+CIFSR\r\n”,1000,DEBUG); // get ip address
delay(5000);
Serial.print(“TCP/UDP Connection…\n”);
sendCommand(“AT+CIPMUX=0\r\n”,2000,DEBUG); // reset module
delay(5000);
}
