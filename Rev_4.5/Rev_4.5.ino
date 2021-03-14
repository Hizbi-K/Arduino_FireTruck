

///// TRUCK WILL NOW TURN IN TWO MODES, ROTATIONARY & REVOLUTIONARY  /////
///// SERVO SWEEP MOVED INTO A FUNCTION, TO BE CALLED IN THE MAIN "loop()"  /////
///// EVERYTHING HAS BEEN MOVED INTO FUCNTIONS THUS SIMPLIFYING THE PROGRAM /////
///// REPLACING "delay()" with "millis()" FOR MULTITASKING  /////
///// BT AND SERVO LIBRARIES REPLACED BY 3RD PARTY ONES WITH MUCH BETTER SUPPORT FOR WORKING TOGETHER  /////



#include <NeoSWSerial.h>
#include <Servo2.h>

NeoSWSerial BTserial(11, 12); // RX | TX
Servo demoServo;        //create a servo object


unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
int intervalSweep = 10;       // CLASSICAL DELAY BETWEEN EACH ANGLE MOVEMENT


int servoAngle_INCRI_DECRI = 1;   // ANGLE INCREMENT OR DECREMENT FOR SERVO SWEEPING
int servoAngleDefault = 77;
int servoAngle = servoAngleDefault;


int servoSweep_Start_Stop = 0;


int servoPin = 13;      //servo digital PWM pin


int In_1 = 7;
int In_2 = 8;
int In_3 = 9;
int In_4 = 10;


int flameSenDP = 3;
int flameLED = 4;
int flameVal;


//int stopSweepLoop = 0;


int PWM_1 = 5;
int PWM_2 = 6;


int SPEED_1 = 200;
int SPEED_2 = 150;
int SPEED_3 = 100;



void setup() {

  //Serial.begin(9600);
  BTserial.begin(9600);

  demoServo.attach(servoPin);  //attach the pin to the object so that we can send the signal to it.
  //I've defined the range between 1000-2000us or 1-2ms as the default range is non-standard.  

  pinMode(servoPin, OUTPUT);
  demoServo.write(servoAngleDefault);
  
  pinMode(In_1, OUTPUT);
  pinMode(In_2, OUTPUT);
  pinMode(In_3, OUTPUT);
  pinMode(In_4, OUTPUT);

  pinMode(flameSenDP, INPUT);
  pinMode(flameLED, OUTPUT);
  
  pinMode(PWM_1, OUTPUT);
  pinMode(PWM_2, OUTPUT);
          
}

void loop() 
{
  // put your main code here, to run repeatedly:

    currentMillis = millis();
    
    flameVal = digitalRead(flameSenDP);     //////////////////// FIRE SAFETY DISTANCE CHECK
    
    if(flameVal == HIGH) // if flame is detected
    {
      digitalWrite(flameLED, HIGH); // TURN ON LED
    }
    
    else
    {
      digitalWrite(flameLED, LOW); // TURN OFF LED
    }

    
    int dataBT = BTserial.read();          //////////////////// TRUCK MOVEMENT
    
    switch (dataBT)
    {
      case 49:                             // FORWARD 
      {
        forward();
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
        rightRotate();
        break;
      }

      case 53:                             // LEFT
      {
        left();
        break;
      }

      case 54:                             // LEFT ROTATION
      {
        leftRotate();
        break;
      }

      case 70:                             // START SERVO SWEEPING
      {
        servoSweep_Start_Stop = 1;
        break;
      }

      case 79:                             // STOP SERVO SWEEPING
      {
        servoSweep_Start_Stop = -1;
        break;
      }

      case 88:                             // STOP TRUCK MOVEMENT
      {
        stopMovement();
        break;
      } 
    
    }


   if (servoSweep_Start_Stop == 1)         //////////////////// SERVO SWEEPING
   {
      servoSweep();  
   }

   else if (servoSweep_Start_Stop == -1)
   {
      servoAngle = servoAngleDefault;     /// RESETTING THE SERVO PARAMETERS
      servoAngle_INCRI_DECRI = 1; 
      servoStop();
   }
   
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// STOP MOVEMENT IN ANY DIRECTION //

void stopMovement ()
{
  digitalWrite(In_1, LOW); digitalWrite(In_2, LOW); digitalWrite(In_3, LOW); digitalWrite(In_4, LOW);    
                                          
  analogWrite(PWM_1, 0);    // SPEED CONTROL [ZERO]
  analogWrite(PWM_2, 0); 
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// FORWARD MOVEMENT FUNCTION 49//

void forward ()
{
  if ((flameVal != HIGH) && (BTserial.read() != 88))
  {
      digitalWrite(In_1, HIGH); digitalWrite(In_2, LOW); digitalWrite(In_3, LOW); digitalWrite(In_4, HIGH);    
                                          
      analogWrite(PWM_1, SPEED_1);    // SPEED CONTROL [FAST]
      analogWrite(PWM_2, SPEED_1); 
  }

return;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// BACKWARD MOVEMENT FUNCTION 50//

void backward ()
{
  if (BTserial.read() != 88)
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
  if (BTserial.read() != 88)
  {
    digitalWrite(In_1, HIGH);  digitalWrite(In_2, LOW);    

    analogWrite(PWM_1, SPEED_2);    // SPEED CONTROL [MEDIUM]
    analogWrite(PWM_2, SPEED_2);
  }

return;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// RIGHT ROTATION FUNCTION 52//

void rightRotate ()
{
  if (BTserial.read() != 88)
  {
    digitalWrite(In_1, HIGH);  digitalWrite(In_2, LOW); digitalWrite(In_3, HIGH); digitalWrite(In_4, LOW);    

    analogWrite(PWM_1, SPEED_3);    // SPEED CONTROL [SLOW]
    analogWrite(PWM_2, SPEED_3);
  }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// LEFT MOVEMENT FUNCTION 53//

void left ()
{
  if (BTserial.read() != 88)
  {
    digitalWrite(In_3, LOW); digitalWrite(In_4, HIGH);    

    analogWrite(PWM_1, SPEED_2);    // SPEED CONTROL [MEDIUM]
    analogWrite(PWM_2, SPEED_2);
  }

return;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// LEFT ROTATION FUNCTION 54//

void leftRotate ()
{
  if (BTserial.read() != 88)
  {
    digitalWrite(In_1, LOW);  digitalWrite(In_2, HIGH);  digitalWrite(In_3, LOW);  digitalWrite(In_4, HIGH);    

    analogWrite(PWM_1, SPEED_3);    // SPEED CONTROL [SLOW]
    analogWrite(PWM_2, SPEED_3);
  }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////



// STOPPING THE SERVO// 

void servoStop ()
{
  demoServo.write(servoAngleDefault);
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
    

    if ((servoAngle >= 130) || (servoAngle <= 15))
    {         
      servoAngle_INCRI_DECRI = - servoAngle_INCRI_DECRI; // reverse direction
                
      //servoAngle = servoAngle + servoAngle_INCRI_DECRI;
    }

    
    demoServo.write(servoAngle);   // ACTUALLY SWEEPING THE SERVO HERE   
  
  }
               
return;
}


