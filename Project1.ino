/*  * SevenSegmentMpx sketch 
    * Shows numbers ranging from 0 through 99 on a four-digit display 
    * This example displays the value of a sensor connected to an analog input */

// bits representing segments A through G (and decimal point) for numerals 0-9 

const int trigPin = 12;
const int echoPin = 13;
const int numeral[10] = {  
  //ABCDEFG /dp  
  B11111100,  // 0  
  B01100000,  // 1 
  B11011010,  // 2 
  B11110010,  // 3 
  B01100110,  // 4  
  B10110110,  // 5  
  B00111110,  // 6  
  B11100000,  // 7  
  B11111110,  // 8 
  B11100110,  // 9 
};

// pins for decimal point and each segment                         
// dp,G,F,E,D,C,B,A

const int segmentPins[] = { 4,7,8,6,5,3,2,9};
const int nbrDigits= 2;  // the number of digits in the LED display
                         //dig  1  2  
const int digitPins[nbrDigits] = {11,10};
void setup() {  
  Serial.begin(9600);
  for(int i=0; i < 8; i++)    
  pinMode(segmentPins[i], OUTPUT); 

// set segment and DP pins to output
  for(int i=0; i < nbrDigits; i++)    
  pinMode(digitPins[i], OUTPUT); 
}

void loop() {  
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

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
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  cm = cm + 1; 
    

  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  showNumber(cm); 
}

void showNumber( int number) {  
  if(number == 0)    
    showDigit( 0, nbrDigits-1) ; // display 0 in the rightmost digit 
  else  {    // display the value corresponding to each digit    // leftmost digit is 0, rightmost is one less than the number of places    
  for( int digit = nbrDigits-1; digit >= 0; digit--)    {      
    if(number > 0)      {         
      showDigit( number % 10,digit)  ;         
      number = number / 10; 
    } else if (number == 0) {
      showDigit( number % 10,digit) ; 
    }
    }
  } 
}

// Displays given number on a 7-segment display at the given digit position 

void showDigit( int number, int digit) {  
  digitalWrite( digitPins[digit], HIGH );  
  for(int segment = 1; segment < 8; segment++)  {    
    boolean isBitSet = bitRead(numeral[number], segment);    // isBitSet will be true if given bit is 1    
    //isBitSet = ! isBitSet; // remove this line if common cathode display  
    digitalWrite( segmentPins[segment], isBitSet);  
  } 
  delay(10);
  digitalWrite( digitPins[digit], LOW ); 
}


long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
