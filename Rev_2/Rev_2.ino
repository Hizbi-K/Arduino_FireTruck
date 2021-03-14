
///// CAN ROTATE SERVO NOW IN BEAUTIFUL SWEEPING MOTION  /////
///// FIXED VARIABLE NAMES FOR "Pol_1" TO "In_1"  /////
///// FIXED DC MOTORS NOT WORKING IF SERVO IS ENABLED (DON'T USE D9 & D10 FOR PWM WHEN USING "Servo.h")  /////


#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial BTserial(11, 12); // RX | TX
Servo demoServo;        //create a servo object

int servoPin = 13;      //servo PWM pin

bool fireExtngsh;      //for triggering the fire extinguishing mechanism

int In_1 = 7;
int In_2 = 8;
int In_3 = 9;
int In_4 = 10;

int flameSenDP = 3;
int flameLED = 4;
int digitalVal;

int PWM_1 = 5;
int PWM_2 = 6;

int SPEED_1 = 200;
int SPEED_2 = 100;

void setup() {

  Serial.begin(9600);
  BTserial.begin(9600);

  demoServo.attach(servoPin);  //attach the pin to the object so that we can send the signal to it. 
                                //I've defined the range between 1000-2000us or 1-2ms as the default range is non-standard.  
  pinMode(servoPin, OUTPUT);
  demoServo.write(77);
  
  pinMode(In_1, OUTPUT);
  pinMode(In_2, OUTPUT);
  pinMode(In_3, OUTPUT);
  pinMode(In_4, OUTPUT);

  pinMode(flameSenDP, INPUT);
  pinMode(flameLED, OUTPUT);
  
  pinMode(PWM_1, OUTPUT);
  pinMode(PWM_2, OUTPUT);
          
}

void loop() {
  // put your main code here, to run repeatedly:


    digitalVal = digitalRead(flameSenDP);   // FIRE SAFETY DISTANCE CHECK
    
    if(digitalVal == HIGH) // if flame is detected
    {
      digitalWrite(flameLED, HIGH); // TURN ON LED
    }
    
    else
    {
      digitalWrite(flameLED, LOW); // TURN OFF LED
    }

    
if(BTserial.available()>0)
   {     
      int data = BTserial.read(); // reading the data received from the bluetooth module  
            
      switch(data) // BASIC TRUCK MOVEMENT
      {
        case 49:
                  {
                   digitalWrite(In_1, HIGH); digitalWrite(In_2, LOW); digitalWrite(In_3, LOW); digitalWrite(In_4, HIGH);    // FORWARD
                                                       
                   analogWrite(PWM_1, SPEED_1);    // SPEED CONTROL [FAST]
                   analogWrite(PWM_2, SPEED_1); 

                   break; 
                  }
        case 50:
                  {
                   digitalWrite(In_1, LOW);  digitalWrite(In_2, HIGH); digitalWrite(In_3, HIGH); digitalWrite(In_4, LOW);     // BACKWARD
                   
                   analogWrite(PWM_1, SPEED_1);    // SPEED CONTROL [FAST]
                   analogWrite(PWM_2, SPEED_1); 
                   
                   break;
                  }
        case 51:
                  {
                    digitalWrite(In_1, HIGH);  digitalWrite(In_2, LOW); digitalWrite(In_3, HIGH); digitalWrite(In_4, LOW);    // RIGHT

                    analogWrite(PWM_1, SPEED_2);    // SPEED CONTROL [SLOW]
                    analogWrite(PWM_2, SPEED_2);

                    break;
                  }
        case 52:
                  {
                    digitalWrite(In_1, LOW);  digitalWrite(In_2, HIGH); digitalWrite(In_3, LOW); digitalWrite(In_4, HIGH); // LEFT

                    analogWrite(PWM_1, SPEED_2);    // SPEED CONTROL [SLOW]
                    analogWrite(PWM_2, SPEED_2);
                    
                    break;
                  } 

        case 88:
                  {
                    digitalWrite(In_1, LOW); digitalWrite(In_2, LOW); digitalWrite(In_3, LOW); digitalWrite(In_4, LOW); // STOP
                    
                    analogWrite(PWM_1, 0);    
                    analogWrite(PWM_2, 0);

                    break;
                  }
       case 70:
                  {
                    fireExtngsh = true;

                    if (fireExtngsh == true)
                    {
                       for (int angle=77; angle>=15; angle--)
                        {
                          demoServo.write(angle);
                          delay(10);
                        } 
                       
                       for (int angle=15; angle<=130; angle++)
                        {
                          demoServo.write(angle);
                          delay(10);
                        }
                  
                      for (int angle=130; angle>=15; angle--)
                        {
                          demoServo.write(angle);
                          delay(10);
                        }
            
                      for (int angle=15; angle<=77; angle++)
                        {
                          demoServo.write(angle);
                          delay(10);
                        }
                      
                    }  
            
                    break;
                  }
       case 79:
                  {
                    fireExtngsh = false;
                    demoServo.write(77);
            
                    break;
                  }
                  
      }

       
      
        Serial.println(data);
   }
   delay(50);
}
