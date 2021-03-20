

///// CLEANED THE CODE AND REMOVED UNECCESSARY COMMENTS /////
///// TWEAKED THE ROTATION SPEED IN AUTOMATIC EXTINGUISHING MODE /////
///// ALSO TWEAKED THE FORWARD SPEED IN AUTOMATIC EXTINGUISHING MODE /////
///// CHANGED THE SERVO OBJECT NAME "demoServo" to "waterServo" ///// 


///// GitHub: @Hizbi-K /////


#include <Arduino.h>      /// MUST INCLUDE IN ALL PROJECTS IF NOT USING NeoSoftSerial as that library already includes this header file.
#include <Servo2.h>


Servo waterServo;        //Creating a servo object


void stopMovement();      /// DECLARING FUNCTIONS IN GLOBAL SCOPE

void forward(int speed);
void autoForward (int speed);

void backward();

void right();
void rightRotate(int speed);
void autoRightRotate (int speed);

void left();
void leftRotate(int speed);
void autoLeftRotate (int speed);

void servoStop();
void servoSweep();

void autoExt();
void flameDistanceCheck();


unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
unsigned int intervalSweep = 120;       // CLASSICAL DELAY BETWEEN EACH ANGLE MOVEMENT


int servoAngle_INCRI_DECRI = 15;   // ANGLE INCREMENT OR DECREMENT FOR SERVO SWEEPING
int servoAngleDefault = 77;
int angleMax = 120;
int angleMin = 44;
int servoAngle = servoAngleDefault;


bool servoSweep_Start_Stop = false;
bool servoActivated = false;


int servoPin = 12;      //servo digital PWM pin


int In_1 = 7;
int In_2 = 8;
int In_3 = 9;
int In_4 = 10;


int flameSenDP_CR = 3;     // FLAME SENSOR DIGITAL OUTPUT 1
bool flameVal_DR = LOW;

int flameLED = 4;

int flameSenDP_CL = 2;     // FLAME SENSOR DIGITAL OUTPUT 2
bool flameVal_DL = LOW;


int flameSenAP_1 = A0;     // FLAME SENSOR ANALOG OUTPUT 1
float flameVal_Center_R;


int flameSenAP_2 = A1;     // FLAME SENSOR ANALOG OUTPUT 2
float flameVal_Center_L;


int flameSenAP_3 = A2;     // FLAME SENSOR ANALOG OUTPUT 3
float flameVal_Left;

bool flameCheck = false;   // For checking Safe Distance


int pumpPin = 11;          // USING ANALOG PIN AS DIGITAL PIN DUE TO PIN CONSTRAINTS. NOTE THAT RELAYS ARE ACTIVELY LOW, THEY WORK OPPOSITELY
bool waterPump = false;    
bool fireInFront = false;

bool autoExtinguish = false;
bool fireFound = false;
bool spinRight = false;
bool spinLeft = false;
float divisor_1024 = 1024;


int PWM_1 = 5;
int PWM_2 = 6;


int SPEED_1 = 120;        // If too fast, the water in the tank will spill out! (Atleast in my unbalanced tank)
int SPEED_2 = 100;
int SPEED_3 = 100;



void setup() {

  Serial.begin(9600);         // NOTE: Higher the buad rate, lower will be the BT range, 
                              // but the transmission speeds will be faster. This might not be entirely true for SoftwareSerial.
                              
   
  waterServo.attach(servoPin);           // Attaching the servo and then detaching it in hope that it would remove the jitter WHENEVER the servo 
  pinMode(servoPin, OUTPUT);             // is commanded to move/sweep. So far so good, although I have also greatly increased the time interval between
  waterServo.write(servoAngleDefault);   // movement. Now it 0.2 seconds. Remember that 'servo.write' needs some time to work, so we need some delay between
                                         // the 'attach' and 'detach' commands. I would place the 'detach' at the end of the 'setup'.
                                         // NO NEED TO ATTACH & DETACH AGAIN, JITTERING WAS CAUSED BY A TIMING PROBLEM.
  
  pinMode(In_1, OUTPUT);
  pinMode(In_2, OUTPUT);
  pinMode(In_3, OUTPUT);
  pinMode(In_4, OUTPUT);

  pinMode(flameSenDP_CR, INPUT);
  pinMode(flameSenDP_CL, INPUT);
  pinMode(flameLED, OUTPUT);

  pinMode(flameSenAP_1, INPUT);
  pinMode(flameSenAP_2, INPUT);
  pinMode(flameSenAP_3, INPUT); 

  digitalWrite(pumpPin, HIGH);    // Turning the Relay OFF
  pinMode(pumpPin, OUTPUT); 
  
  pinMode(PWM_1, OUTPUT);
  pinMode(PWM_2, OUTPUT);
          

}

void loop() 
{
  // put your main code here, to run repeatedly:

   
    currentMillis = millis();
      
    flameDistanceCheck();

    
    
    int dataBT = Serial.read();              ///// TRUCK MOVEMENT AND OTHER FEATURES USING BLUETOOTH


    switch (dataBT)
    {
      case 49:                             // FORWARD 
      {
        forward(SPEED_1);
        break;
      }

      case 50:                             // BACKWARD
      {
        backward();
        break;
      }

      case 51:                             // RIGHT
      {
        right();
        break;
      }

      case 52:                             // RIGTH ROTATION
      {
        rightRotate(SPEED_3);
        break;
      }

      case 53:                             // LEFT
      {
        left();
        break;
      }

      case 54:                             // LEFT ROTATION
      {
        leftRotate(SPEED_3);
        break;
      }

      case 77:                             // START SERVO SWEEPING & PUMPING WATER
      {
        servoSweep_Start_Stop = true;
        previousMillis = currentMillis - intervalSweep;     // SYNCING this perimeter with the 'currentMillis'. This will remove ANY
        waterPump = true;                                   // kind of servo twitching once and for all. UPDATE: Twitching gone :)
        break;
      }

      case 109:                             // STOP SERVO SWEEPING & PUMPING WATER
      {
        servoSweep_Start_Stop = false;
        waterPump = false;
        digitalWrite(pumpPin, HIGH);
        break;
      }

      case 65:                             // AUTOMATIC FIRE EXTINGUISHING
      {
        autoExtinguish = true;         
        break;
      }

      case 97:                             // MANUAL FIRE EXTINGUISHING
      {
        autoExtinguish = false;
        fireFound = false;
        spinRight = false;
        spinLeft = false;
        waterPump = false;
        fireInFront = false;
        servoSweep_Start_Stop = false;
        digitalWrite(pumpPin, HIGH);
        stopMovement();
        break;
      }

      case 88:                             // STOP TRUCK MOVEMENT
      {
        stopMovement();
        break;
      }
    
    }



   if (autoExtinguish == true)           // AUTOMATIC FIRE EXTINGUISHING LOGIC
   {
      autoExt();
   }


   if (fireFound == true)
   {
     autoForward(75);
   }


   if (waterPump == true)
   {
     digitalWrite(pumpPin, LOW);
   }


   if (fireInFront == true)
   {
     flameVal_DR = digitalRead(flameSenDP_CR);
     flameVal_DL = digitalRead(flameSenDP_CL);   

     if ((flameVal_DR && flameVal_DL) == LOW)
     {
       waterPump = false;
       digitalWrite(pumpPin, HIGH);
       servoSweep_Start_Stop = false;

       fireInFront = false;    // Just getting out of the loop that stops the servo sweeping during 'auto ext'.
     }
   }
   

   if (servoSweep_Start_Stop == true)         //////////////////// SERVO SWEEPING
   {
      servoSweep();
      servoActivated = true;  
   }

   
   if ((servoSweep_Start_Stop == false) && (servoActivated == true))
   {
      servoStop();      /// RESETTING THE SERVO PARAMETERS
      servoActivated = false;
   }


}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// STOP MOVEMENT IN ANY DIRECTION //

void stopMovement ()
{
  digitalWrite(In_1, LOW); digitalWrite(In_2, LOW); digitalWrite(In_3, LOW); digitalWrite(In_4, LOW);    
                                          
  analogWrite(PWM_1, 0);    // SPEED CONTROL [ZERO]
  analogWrite(PWM_2, 0);

  return; 
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// FIRE DISTANCE CHECK //

void flameDistanceCheck ()
{

  flameVal_Center_R = (analogRead(flameSenAP_1) * 5) / divisor_1024;     //////////////////// FIRE SAFETY DISTANCE CHECK
  flameVal_Center_L = (analogRead(flameSenAP_2) * 5) / divisor_1024;

  
  if ((flameVal_Center_R <= 0.057) || (flameVal_Center_L <= 0.057)) // if flame is detected, the value compared to was calculated by averaging five values
  {
    digitalWrite(flameLED, HIGH); // TURN ON LED
    flameCheck = true;
  }
      
  else
  {
    digitalWrite(flameLED, LOW); // TURN OFF LED
    flameCheck = false;
  }


  return;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// AUTO FIRE EXTINGUISH //

void autoExt ()
{
    
    flameVal_Left = (analogRead(flameSenAP_3) * 5) / divisor_1024;

    flameVal_DR   = digitalRead(flameSenDP_CR);
    flameVal_DL   = digitalRead(flameSenDP_CL);

    
    if ((flameVal_Left > 4.800) && ((flameVal_DR && flameVal_DL) == HIGH))
    {
      stopMovement();
      fireFound = true;
      autoExtinguish = false;
    }
    
    else if ((flameVal_Left <= 4.800) && ((flameVal_DR && flameVal_DL) != HIGH) && (spinRight != true))
    {
      spinLeft = true;
      spinRight = false;
      autoLeftRotate(80);

    } 
    
    else if ((flameVal_Left > 4.800) && ((flameVal_DR && flameVal_DL) != HIGH) && (spinLeft != true))
    {
      spinRight = true;
      spinLeft = false;
      autoRightRotate(80);
    }


  return;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// FORWARD MOVEMENT FUNCTION 49//

void forward (int speed)
{
  if ((flameCheck == false) && (Serial.read() != 88))
  {
      digitalWrite(In_1, HIGH); digitalWrite(In_2, LOW); digitalWrite(In_3, LOW); digitalWrite(In_4, HIGH);    
                                          
      analogWrite(PWM_1, speed);    // SPEED CONTROL [FAST]
      analogWrite(PWM_2, speed); 
  }

return;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// AUTO FORWARD MOVEMENT FUNCTION //

void autoForward (int speed)
{
  
  flameVal_DR = digitalRead(flameSenDP_CR);
  flameVal_DL = digitalRead(flameSenDP_CL);

  
  if ((Serial.read() != (97 || 109 || 88)) && (flameCheck != true) && ((flameVal_DR || flameVal_DL) == HIGH))
  {
    digitalWrite(In_1, HIGH); digitalWrite(In_2, LOW); digitalWrite(In_3, LOW); digitalWrite(In_4, HIGH);    
                                        
    analogWrite(PWM_1, speed);    // SPEED CONTROL [SLOW]
    analogWrite(PWM_2, speed); 
  }
  else
  {
    fireFound = false;
    waterPump = true;
    servoSweep_Start_Stop = true;
    previousMillis = currentMillis - intervalSweep;     // SYNCING this perimeter with the 'currentMillis'. This will remove ANY
    fireInFront = true;                                 // kind of servo twitching once and for all. UPDATE: Twitching gone :)
    stopMovement();
  }
  
  
return;
}




////////////////////////////////////////////////////////////////////////////////////////////////////////



// BACKWARD MOVEMENT FUNCTION 50//

void backward ()
{
  if (Serial.read() != 88)
  {
    digitalWrite(In_1, LOW);  digitalWrite(In_2, HIGH); digitalWrite(In_3, HIGH); digitalWrite(In_4, LOW);     
                   
    analogWrite(PWM_1, SPEED_1);    // SPEED CONTROL [FAST]
    analogWrite(PWM_2, SPEED_1); 
  }

return;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// RIGHT MOVEMENT FUNCTION 51// 

void right ()
{
  if (Serial.read() != 88)
  {
    digitalWrite(In_1, HIGH);  digitalWrite(In_2, LOW);    

    analogWrite(PWM_1, SPEED_2);    // SPEED CONTROL [MEDIUM]
    analogWrite(PWM_2, SPEED_2);
  }

  return;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// AUTO RIGHT ROTATION FUNCTION //

void autoRightRotate (int speed)
{
  if (Serial.read() != (97 || 109 || 88))
  {
    digitalWrite(In_1, HIGH);  digitalWrite(In_2, LOW); digitalWrite(In_3, HIGH); digitalWrite(In_4, LOW);    

    analogWrite(PWM_1, speed);    // SPEED CONTROL [SLOW]
    analogWrite(PWM_2, speed);
  }
  return;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// RIGHT ROTATION FUNCTION 52//

void rightRotate (int speed)
{
  if (Serial.read() != 88)
  {
    digitalWrite(In_1, HIGH);  digitalWrite(In_2, LOW); digitalWrite(In_3, HIGH); digitalWrite(In_4, LOW);    

    analogWrite(PWM_1, speed);    // SPEED CONTROL [SLOW]
    analogWrite(PWM_2, speed);
  }
  return;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// LEFT MOVEMENT FUNCTION 53//

void left ()
{
  if (Serial.read() != 88)
  {
    digitalWrite(In_3, LOW); digitalWrite(In_4, HIGH);    

    analogWrite(PWM_1, SPEED_2);    // SPEED CONTROL [MEDIUM]
    analogWrite(PWM_2, SPEED_2);
  }

return;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// AUTO LEFT ROTATION FUNCTION //

void autoLeftRotate (int speed)
{
  if (Serial.read() != (97 || 109 || 88))
  {
    digitalWrite(In_1, LOW);  digitalWrite(In_2, HIGH);  digitalWrite(In_3, LOW);  digitalWrite(In_4, HIGH);    

    analogWrite(PWM_1, speed);    // SPEED CONTROL [SLOW]
    analogWrite(PWM_2, speed);
  }
  return;
}





////////////////////////////////////////////////////////////////////////////////////////////////////////



// LEFT ROTATION FUNCTION 54//

void leftRotate (int speed)
{
  if (Serial.read() != 88)
  {
    digitalWrite(In_1, LOW);  digitalWrite(In_2, HIGH);  digitalWrite(In_3, LOW);  digitalWrite(In_4, HIGH);    

    analogWrite(PWM_1, speed);    // SPEED CONTROL [SLOW]
    analogWrite(PWM_2, speed);
  }
  return;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// STOPPING THE SERVO// 

void servoStop ()
{
  waterServo.write(servoAngleDefault);
  servoAngle = servoAngleDefault;
  servoAngle_INCRI_DECRI = 15;

  return;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// SERVO SWEEPING//

void servoSweep() 
{

  if (currentMillis - previousMillis >= intervalSweep)
  {
          
    previousMillis = previousMillis + intervalSweep;
    
    servoAngle = servoAngle + servoAngle_INCRI_DECRI; 
    

    if ((servoAngle >= angleMax) || (servoAngle <= angleMin))
    {         
      servoAngle_INCRI_DECRI = - servoAngle_INCRI_DECRI; // reverse direction
                
      servoAngle = servoAngle + servoAngle_INCRI_DECRI;
    }

    
    waterServo.write(servoAngle);   // ACTUALLY SWEEPING THE SERVO HERE   
  
  }
               
return;
}
