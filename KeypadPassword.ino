/*

 Layout of 4x4 Dot Matrix Keyboard:
 [1] [2] [3] [o]
 [4] [5] [6] [o]
 [7] [8] [9] [o]
 [R] [0] [E] [o]
 
*/

#include <Keypad.h> //import keypad library
#include <Password.h>

String newPasswordString; //hold the new password
char newPassword[4]; //charater string of newPasswordString
 
//initialize password to 1234
//you can use password.set(newPassword) to overwrite it
Password password = Password( "1234" );
 
byte maxPasswordLength = 4; 
byte currentPasswordLength = 0;

const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns

//define the keymap
char keys [ROWS] [COLS] = {
  {'1', '2', '3', 'o'},
  {'4', '5', '6', 'o'},
  {'7', '8', '9', 'o'},
  {'R', '0', 'E', 'o'}
};
byte rowPins[ROWS] = {
  10 ,9 ,8 ,7}; //connect keypad ROW1, ROW2, ROW3, ROW4 to these arduino pins
byte colPins[COLS] = {
  6, 5, 4, 13}; //connect keypad COL1, COL2, COL3, COL4 to these arduino pins

//create the keypad
Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
  delay(2500);
}

void loop(){
  char key = myKeypad.getKey();

  if (key != NO_KEY){
      delay(60); 
      switch (key){
      case 'R': changePassword(); break; 
      case 'E': checkPassword(); break;
      case 'o': resetPassword(); break;
      default: processNumberKey(key);
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
      Serial.println(" OK.");
   } else {
      Serial.println(" Wrong passwowrd!");
   } 
   resetPassword();
}

void resetPassword() {
   password.reset(); 
   currentPasswordLength = 0; 
}

void changePassword() { 
   int randNumber = random(0, 9999);
   if (randNumber >=0 && randNumber < 10 ) {
      newPasswordString = "000" + String(randNumber);
   } else if (randNumber >=10 && randNumber <100) {
      newPasswordString = "00" + String(randNumber);
   } else if (randNumber >=100 && randNumber <1000) {
      newPasswordString = "0" + String(randNumber);
   } else {
      newPasswordString = String(randNumber);
   }
   newPasswordString.toCharArray(newPassword, newPasswordString.length()+1); //convert string to char array
   password.set(newPassword);
   resetPassword();
   Serial.print("Password changed to ");
   Serial.println(newPasswordString);
}
