

         ///////////////////////////////////////////////
        //             The Welcome Gadget            //
       //      (An alarm mechanism for your room )  //
      //                by Kutluhan Aktar          // 
     ///////////////////////////////////////////////                                          




   // With a 4x20 LCD and a 2-way Joystick, make an interactive alarm system for your room and give scary to the unknown persons who enter your room. :)
   //
   // When the Alarm System is activated by the optimization of joysticks, it scans the whole room for an unexpected enter.
   //
   // And it notifies with the buzzer and a led until the correct password is written.
   //
   // The servo motors are adjustable to capture a new pattern of an angle by joysticks.
   //
   //
   // CONNECTİONS:
   //
   // Arduino Mega                    
   //                                            4x20 LCD Screen
   // Pin 45   ---------------------------------- rs
   // Pin 44   ---------------------------------- en
   // Pin 43   ---------------------------------- d4
   // Pin 42   ---------------------------------- d5
   // Pin 41   ---------------------------------- d6
   // Pin 40   ---------------------------------- d7
   //
   //                                            4x4 Matrix Keypad
   // Pin 50   ---------------------------------- ROW
   // Pin 51   ---------------------------------- ROW
   // Pin 52   ---------------------------------- ROW
   // Pin 53   ---------------------------------- ROW
   // Pin 46   ---------------------------------- COLS
   // Pin 47   ---------------------------------- COLS
   // Pin 48   ---------------------------------- COLS
   // Pin 49   ---------------------------------- COLS
   //
   //                                            Buzzer
   // Pin 3   ----------------------------------- 
   //
   //                                            LED
   // Pin 4   ----------------------------------- 
   // Pin 5   ----------------------------------- 
   // Pin 34  ----------------------------------- 
   // Pin 35  ----------------------------------- 
   //                                            BUTTONS
   // Pin 36  ----------------------------------- 
   // Pin 37  ----------------------------------- 
   //                                            Joystick (1)
   // Pin A8  ----------------------------------- X                     
   // Pin A9  ----------------------------------- Y            
   // Pin 38  ----------------------------------- KEY
   //                                            Joystick (2)
   // Pin A10 ----------------------------------- X                     
   // Pin A11 ----------------------------------- Y            
   // Pin 39  ----------------------------------- KEY
   //                                            HC-SR04 (1)
   // Pin 32  ----------------------------------- Trig
   // Pin 33  ----------------------------------- Echo
   //                                            HC-SR04 (2)
   // Pin 30  ----------------------------------- Trig
   // Pin 31  ----------------------------------- Echo
   //                                            Servo Motor (1)
   // Pin 9   ----------------------------------- 
   //                                            Servo Motor (2)
   // Pin 10   ----------------------------------- 



//Add the libraries.
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

Servo HC_SR04_1_SERVO; // Decsribe the Servo Motor as a object.
Servo HC_SR04_2_SERVO;

const int rs = 45, en = 44, d4 = 43, d5 = 42, d6 = 41, d7 = 40;// Initialize the library by associating any needed LCD interface pin
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                    //   with the arduino pin number it is connected to.

byte initial[8] = { // Make special characters for LCD Screen.
  0b11111,
  0b10001,
  0b10001,
  0b11111,
  0b10101,
  0b11110,
  0b11110,
  0b10001,
};
byte smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};
byte alert[8] = {
  0b10001,
  0b01010,
  0b00100,
  0b00100,
  0b00100,
  0b10101,
  0b01010,
  0b10001,
};


const byte ROWS = 4; //Four rows
const byte COLS = 4; //Four columns
//Define the cymbols on the buttons of the keypads
char controlKeys[ROWS][COLS] = {
  {'E','0','+','B'},
  {'7','8','9','X'},
  {'4','5','6','F'},
  {'1','2','3','A'}
};
byte rowPins[ROWS] = {50,51,52,53}; //Connect to the row pinouts of the keypad.
byte colPins[COLS] = {46,47,48,49}; //Connect to the column pinouts of the keypad.

//Initialize an instance of class NewKeypad
Keypad controlKeypad = Keypad( makeKeymap(controlKeys), rowPins, colPins, ROWS, COLS); 

int buzzerPin=3; //Define Buzzer pin.

int controlLed=4; //Define LED pins.

int Alert_Led=5;

int C = 262; // Define the frequancies of notes for Buzzer.
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_ = 523;
int z  =600;
int Z = 700;
int notes[] = {C, D, E, F, G, A, B, C_, z, Z}; // Create a group for notes.


int Joystick1_X = A8; // Describe Joysticks'pins.
int Joystick1_Y = A9;
int Joystick1_Key = 38;
int Joystick2_X = A10;
int Joystick2_Y = A11;
int Joystick2_Key = 39;

int Button1 = 36; // Define Control_Command and Scan_Command menu's buttons.
int Button2 = 37;
int led1 = 34;
int led2 = 35;

int TrigPin1=32; // Define HC-SR04 pins.
int EchoPin1=33;
int TrigPin2=30;
int EchoPin2=31;

char controlKey; // For saving the data came from Keypad.
String readString;

long duration1; 
long distance1;
long duration2;
long distance2;

// Volatile booleans allow Arduino to save the data while the requirement is true.
volatile boolean Control_command = false;
volatile boolean Scan_command = false;
volatile boolean OK = false;
volatile boolean Clear = false;

volatile boolean Joystick1_XR = false;
volatile boolean Joystick1_XL = false;
volatile boolean Joystick1_YU = false;
volatile boolean Joystick1_YD = false;
volatile boolean Joystick1_KeyB = false;

volatile boolean Joystick2_XR = false;
volatile boolean Joystick2_XL = false;
volatile boolean Joystick2_YU = false;
volatile boolean Joystick2_YD = false;
volatile boolean Joystick2_KeyB = false;

volatile boolean Alert_System_1 = false;
volatile boolean Alert_System_2 = false;
volatile boolean Alert_System_Activate = false;
volatile boolean Alert_System_Alarm = false;

void setup(){
  Serial.begin(9600);

  HC_SR04_1_SERVO.attach(9); // Define a PWM pin for Servo Motor.
  HC_SR04_2_SERVO.attach(10);
  
  pinMode(Joystick1_Key,INPUT);
  digitalWrite(Joystick1_Key,HIGH);
  pinMode(Joystick2_Key,INPUT);
  digitalWrite(Joystick2_Key,HIGH);

  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(Button1,INPUT);
  pinMode(Button2,INPUT);
  pinMode(buzzerPin,OUTPUT);
  pinMode(controlLed,OUTPUT);
  pinMode(Alert_Led,OUTPUT);

  pinMode(TrigPin1,OUTPUT);
  pinMode(EchoPin1,INPUT);
  pinMode(TrigPin2,OUTPUT);
  pinMode(EchoPin2,INPUT);
  
  lcd.begin(20,4);
  lcd.createChar(1,initial);
  lcd.createChar(2,smiley);
  lcd.createChar(3,alert);
  for(int i=0;i<500;i++){
    lcd.setCursor(0,0);
    lcd.print("Initializing...");
    lcd.setCursor(10,1);
    lcd.print(i);
    lcd.setCursor(i/50,2);
    lcd.write(1);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("The Welcome Gadget");
  lcd.setCursor(0,1);
  lcd.print("is ready to run!");
  lcd.setCursor(0,2);
  lcd.print("Please press OK ");
  lcd.setCursor(0,3);
  lcd.print("to continue!");
  

}
  
void loop(){
  
   HC_SR04(1);

  // Through the volatile booleans, Arduino Mega do the determined commands while the requirements are true.

  //Scan_command
  if(digitalRead(Button1)==1){
    Scan_command=true;
    Control_command=false;

     Clear=false;
      lcd.clear();
      digitalWrite(led1,HIGH);
      digitalWrite(led2,LOW);
      lcd.setCursor(0,1);
      lcd.print("Scan_command is");
      lcd.setCursor(0,2);
      lcd.print("ACTIVATED!");
      lcd.setCursor(11,2);
      lcd.write(2);
  }
  if(Scan_command==true){

    if(digitalRead(Joystick2_Key)==0){
    Clear=true;
    OK=false;
    Alert_System_1=false;
    Alert_System_2=false;
    Alert_System_Activate=false;
    Alert_Led_Circuit(1);
    
    }
    if(digitalRead(Joystick1_Key)==0){
    Clear=false;
    OK=true;
    Alert_System_1=false;
    Alert_System_2=false;
    Alert_System_Activate=false;
    Alert_Led_Circuit(1);
    
    }
    if(analogRead(Joystick1_Y)>1000){
      Clear=false;
      OK=false;
      Alert_System_1=true;
      Alert_System_2=false;

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Please turn ");
      lcd.setCursor(0,1);
      lcd.print("Joystick2 into up");
      lcd.setCursor(0,2);
      lcd.print("position to activate");
      lcd.setCursor(0,3);
      lcd.print("Alert_System!");
    }
    if(analogRead(Joystick2_Y)>1000){
      Clear=false;
      OK=false;
      Alert_System_1=false;
      Alert_System_2=true;
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Please turn ");
      lcd.setCursor(0,1);
      lcd.print("Joystick1 into up");
      lcd.setCursor(0,2);
      lcd.print("position to activate");
      lcd.setCursor(0,3);
      lcd.print("Alert_System!");
    }
    if(analogRead(Joystick1_Y)>600&&analogRead(Joystick2_Y)>600){
      Clear=false;
      OK=false;
      Alert_System_1=false;
      Alert_System_2=false;
      Alert_System_Activate=true;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alert_System ");
      lcd.setCursor(0,1);
      lcd.print("is Activated!");
      lcd.setCursor(0,2);
      lcd.print("Sensor's values");
      lcd.setCursor(0,3);
      lcd.print("are optimum.");
    }
  
    // MANEGAMENTS
    if(Clear==true){
    lcd.clear();
    readString="";
    }
    
    if(OK==true){
    KeypadVALUES(0);
       if(readString.indexOf("your birthday")>0){ // It is a little easter egg for the user.If someone write your birthday with Keypad correctly, "Happy Birthday!" blinks on the screen until "XXXX" command is run.
       lcd.setCursor(0,0);
       lcd.print("Happy Birthday!");
       lcd.setCursor(18,0);
       lcd.blink();
       }
        if(readString.indexOf("XXXX")>0){
        lcd.noBlink();
        }
    } 

    if(Alert_System_1==true){ // Warnings for the optimization of Alert function.
      for(int i=0;i<2;i++){
      Alert_Led_Circuit(i);
      delay(100);
      }
    }
    if(Alert_System_2==true){
      for(int i=0;i<2;i++){
      Alert_Led_Circuit(i);
      delay(100);
      }
    }
    if(Alert_System_Activate==true){
      digitalWrite(Alert_Led,HIGH);
      Alert(); // It locks all system until Arduino gets the correct PASSWORD through the Keypad.
    }
    
  }
  
  

  //Control_command
  if(digitalRead(Button2)==1){ // Adjust Servo Motors and observe the value of HC-SR04 Sensors.
  Scan_command=false;
  Control_command=true;
    lcd.clear();
    digitalWrite(led1,HIGH);
    digitalWrite(led2,LOW);
    lcd.setCursor(0,1);
    lcd.print("Control_command is");
    lcd.setCursor(0,2);
    lcd.print("ACTIVATED!");
    lcd.setCursor(11,2);
    lcd.write(2);
  }
  if(Control_command==true){
    digitalWrite(led2,HIGH);
    digitalWrite(led1,LOW);
   if(analogRead(Joystick1_X)<50){
 Joystick1_XR=true;
 Joystick1_XL=false;
 Joystick1_YU=false;
 Joystick1_YD=false;
 Joystick1_KeyB=false;

 Joystick2_XR=false;
 Joystick2_XL=false;
 Joystick2_YU=false;
 Joystick2_YD=false;
 Joystick2_KeyB=false;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Joystick1_X is Right");
    lcd.setCursor(0,2);
    lcd.print("HC_SR04_1_SERVO=90");
    for(int i=0;i<20;i++){
    lcd.setCursor(i,3);
    lcd.write(3);
    }
    }
    if(analogRead(Joystick1_X)>800){
 Joystick1_XR=false;
 Joystick1_XL=true;
 Joystick1_YU=false;
 Joystick1_YD=false;
 Joystick1_KeyB=false;

 Joystick2_XR=false;
 Joystick2_XL=false;
 Joystick2_YU=false;
 Joystick2_YD=false;
 Joystick2_KeyB=false;     
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Joystick1_X is Left");
    lcd.setCursor(0,2);
    lcd.print("HC_SR04_1_SERVO=0");
    for(int i=0;i<20;i++){
    lcd.setCursor(i,3);
    lcd.write(3);
    }
    }
    if(analogRead(Joystick1_Y)>1000){
 Joystick1_XR=false;
 Joystick1_XL=false;
 Joystick1_YU=true;
 Joystick1_YD=false;
 Joystick1_KeyB=false;

 Joystick2_XR=false;
 Joystick2_XL=false;
 Joystick2_YU=false;
 Joystick2_YD=false;
 Joystick2_KeyB=false;     
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Joystick1_Y is Up");
    lcd.setCursor(0,2);
    lcd.print("HC-SR04_1 = ");
    for(int i=0;i<20;i++){
    lcd.setCursor(i,3);
    lcd.write(3);
    }
    }
    if(analogRead(Joystick1_Y)<150){
 Joystick1_XR=false;
 Joystick1_XL=false;
 Joystick1_YU=false;
 Joystick1_YD=true;
 Joystick1_KeyB=false;

 Joystick2_XR=false;
 Joystick2_XL=false;
 Joystick2_YU=false;
 Joystick2_YD=false;
 Joystick2_KeyB=false;      
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Joystick1_Y is Down");
    lcd.setCursor(0,2);
    lcd.print("HC_SR04_1_SERVO=180");
    for(int i=0;i<20;i++){
    lcd.setCursor(i,3);
    lcd.write(3);
    }
    }
    if(digitalRead(Joystick1_Key)==0){ // Test the Buzzer works correctly.
 Joystick1_XR=false;
 Joystick1_XL=false;
 Joystick1_YU=false;
 Joystick1_YD=false;
 Joystick1_KeyB=true;

 Joystick2_XR=false;
 Joystick2_XL=false;
 Joystick2_YU=false;
 Joystick2_YD=false;
 Joystick2_KeyB=false;
      
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Joystick1_X is Clicked ");
    lcd.setCursor(0,2);
    lcd.print("BUZZER");
    for(int i=0;i<20;i++){
    lcd.setCursor(i,3);
    lcd.write(3);
    }
    }
    if(analogRead(Joystick2_X)<50){
 Joystick1_XR=false;
 Joystick1_XL=false;
 Joystick1_YU=false;
 Joystick1_YD=false;
 Joystick1_KeyB=false;

 Joystick2_XR=true;
 Joystick2_XL=false;
 Joystick2_YU=false;
 Joystick2_YD=false;
 Joystick2_KeyB=false;      
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Joystick2_X is Right");
    lcd.setCursor(0,2);
    lcd.print("HC_SR04_2_SERVO=90");
    for(int i=0;i<20;i++){
    lcd.setCursor(i,3);
    lcd.write(3);
    }
    }
    if(analogRead(Joystick2_X)>800){
 Joystick1_XR=false;
 Joystick1_XL=false;
 Joystick1_YU=false;
 Joystick1_YD=false;
 Joystick1_KeyB=false;

 Joystick2_XR=false;
 Joystick2_XL=true;
 Joystick2_YU=false;
 Joystick2_YD=false;
 Joystick2_KeyB=false;      
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Joystick2_X is Left");
    lcd.setCursor(0,2);
    lcd.print("HC_SR04_2_SERVO=0");
    for(int i=0;i<20;i++){
    lcd.setCursor(i,3);
    lcd.write(3);
    }
    }
    if(analogRead(Joystick2_Y)>1000){
 Joystick1_XR=false;
 Joystick1_XL=false;
 Joystick1_YU=false;
 Joystick1_YD=false;
 Joystick1_KeyB=false;

 Joystick2_XR=false;
 Joystick2_XL=false;
 Joystick2_YU=true;
 Joystick2_YD=false;
 Joystick2_KeyB=false;     
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Joystick2_Y is Up");
    lcd.setCursor(0,2);
    lcd.print("HC-SR04_2 = ");
    for(int i=0;i<20;i++){
    lcd.setCursor(i,3);
    lcd.write(3);
    }
    }
    if(analogRead(Joystick2_Y)<150){
 Joystick1_XR=false;
 Joystick1_XL=false;
 Joystick1_YU=false;
 Joystick1_YD=false;
 Joystick1_KeyB=false;

 Joystick2_XR=false;
 Joystick2_XL=false;
 Joystick2_YU=false;
 Joystick2_YD=true;
 Joystick2_KeyB=false;      
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Joystick2_Y is Down");
    lcd.setCursor(0,2);
    lcd.print("HC_SR04_2_SERVO=180");
    for(int i=0;i<20;i++){
    lcd.setCursor(i,3);
    lcd.write(3);
    }
    }
    if(digitalRead(Joystick2_Key)==0){ // Test the controlLed works correctly.
 Joystick1_XR=false;
 Joystick1_XL=false;
 Joystick1_YU=false;
 Joystick1_YD=false;
 Joystick1_KeyB=false;

 Joystick2_XR=false;
 Joystick2_XL=false;
 Joystick2_YU=false;
 Joystick2_YD=false;
 Joystick2_KeyB=true;
    ;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Joystick2_X is Clicked");
    lcd.setCursor(0,2);
    lcd.print("LED");
    for(int i=0;i<20;i++){
    lcd.setCursor(i,3);
    lcd.write(3);
    }
    }

    //MANAGEMENTS
    if(Joystick1_KeyB==true){
    tone(buzzerPin,300);
    }
    if(Joystick1_KeyB==false){
    noTone(buzzerPin);
    }
    if(Joystick2_KeyB==true){
    digitalWrite(controlLed,HIGH);
    }
    if(Joystick2_KeyB==false){
    digitalWrite(controlLed,LOW);
    }
    if(Joystick1_YU==true){
    HC_SR04(2);
    }
    if(Joystick2_YU==true){
    HC_SR04(3);
    }
    if(Joystick1_XR==true){
      HC_SR04_1_SERVO.write(90);
    }
    if(Joystick1_XL==true){
      HC_SR04_1_SERVO.write(0);
    }
    if(Joystick1_YD==true){
      HC_SR04_1_SERVO.write(180);
    }
    if(Joystick2_XR==true){
      HC_SR04_2_SERVO.write(90);
    }
    if(Joystick2_XL==true){
      HC_SR04_2_SERVO.write(0);
    }
    if(Joystick2_YD==true){
      HC_SR04_2_SERVO.write(180);
    }
  }
}

void KeypadVoices(char i){ // Define what voices sounds when the buttons of Keypad are pushed.
  switch(i){
    case '1':
    tone(buzzerPin,notes[1]);
    break;
    case '2':
    tone(buzzerPin,notes[2]);
    break;
    case '3':
    tone(buzzerPin,notes[3]);
    break;
    case '4':
    tone(buzzerPin,notes[4]);
    break;
    case '5':
    tone(buzzerPin,notes[5]);
    break;
    case '6':
    tone(buzzerPin,notes[6]);
    break;
    case '7':
    tone(buzzerPin,notes[7]);
    break;
    case '8':
    tone(buzzerPin,notes[8]);
    break;
    case '9':
    tone(buzzerPin,notes[9]);
    break;
    case '0':
    tone(buzzerPin,notes[10]);
    break;
    default:
    noTone(buzzerPin);
    break;
  }
}
void HC_SR04(int i){ // Get the information of distance from HC-SR04 Sensors.
  switch(i){
    case 1:
  digitalWrite(TrigPin1,LOW);
  delay(2);
  digitalWrite(TrigPin1,HIGH);
  delay(10);
  digitalWrite(TrigPin1,LOW);
  duration1=pulseIn(EchoPin1,HIGH);
  distance1=duration1/58.2;

  digitalWrite(TrigPin2,LOW);
  delay(2);
  digitalWrite(TrigPin2,HIGH);
  delay(10);
  digitalWrite(TrigPin2,LOW);
  duration2=pulseIn(EchoPin2,HIGH);
  distance2=duration2/58.2;
     break;
     case 2:
  lcd.setCursor(15,2);
  lcd.print(distance1);
     break;
     case 3:
  lcd.setCursor(15,2);
  lcd.print(distance2);
     break;
     default:
     break;
  }
  
}
void KeypadVALUES(int x){ // Write the Keypad values to 4x20 LCD.
    controlKey= controlKeypad.getKey();
    lcd.setCursor(0,x);
    lcd.print("Keypad Values :");
    if(controlKey){
    readString+=controlKey;
    KeypadVoices(controlKey);
    lcd.setCursor(0,x+1);
    lcd.print(readString);
    }
    
}
void Alert_Led_Circuit(int i){ // Alert led.
   switch(i){
      case 0:
    digitalWrite(Alert_Led,HIGH);
      break;
      case 1:
    digitalWrite(Alert_Led,LOW);
      break;
      default:
      break;
    }
}
void Alert(){ // If someone enters your room, Arduino Mega notifies you with the Buzzer and the  controlLed until who write the correct PASSWORD with Keypad.
              // And it is getting weirder for somebody who enters your room without your permission even though they know about the Welcome Gadget. :)
  if(distance1<50 || distance2<50){
    Alert_System_Alarm=true;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Please Write");
    lcd.setCursor(0,1);
    lcd.print("PASSWORD :");
  }
    if(Alert_System_Alarm==true){
    while(Alert_System_Alarm==true){
    Clear=false;
    OK=false;
    Alert_System_1=false;
    Alert_System_2=false;
    digitalWrite(controlLed,HIGH);
    tone(buzzerPin,100);
    KeypadVALUES(2);
      if(readString.indexOf("1234")>0 && controlKey=='E'){
        lcd.clear();
        lcd.setCursor(5,1);
        lcd.print("CORRECT");
        digitalWrite(controlLed,LOW);
        noTone(buzzerPin);
        Alert_System_Alarm=false;
      }
    }
  }
}



