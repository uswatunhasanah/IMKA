int motorPin1 = 5;
int motorPin2 = 6;void setup() {
// put your setup code here, to run once:
Serial.begin(9600);
pinMode(motorPin1, OUTPUT);
pinMode(motorPin2, OUTPUT);
digitalWrite(motorPin1, LOW);
digitalWrite(motorPin2, LOW);
}void loop() {
// put your main code here, to run repeatedly:
Serial.write(“motor 1 ON”);
digitalWrite(motorPin1, HIGH);
digitalWrite(motorPin2, LOW);
delay(30);
digitalWrite(motorPin1, LOW);
digitalWrite(motorPin2, LOW);
delay(3000);
Serial.write(“motor 2 ON”);
digitalWrite(motorPin1, LOW);
digitalWrite(motorPin2, HIGH);
delay(30);
digitalWrite(motorPin1, LOW);
digitalWrite(motorPin2, LOW);
delay(3000);
}
