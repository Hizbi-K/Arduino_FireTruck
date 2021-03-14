

///// TRUCK WILL NOW TURN IN TWO MODES: ROTATIONARY & REVOLUTIONARY  /////
///// SERVO SWEEP MOVED INTO A FUNCTION, TO BE CALLED IN THE MAIN "loop()"  /////
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
int servoAngle = 15;


int servoSweep_Start_Stop = 0;


int servoPin = 10;      //servo digital PWM pin


int In_1 = 7;
int In_2 = 8;
int In_3 = 9;
int In_4 = 13;


int flameSenDP = 3;
int flameLED = 4;
int flameVal;


//int stopSweepLoop = 0;


int PWM_1 = 5;
int PWM_2 = 6;


int SPEED_1 = 200;
int SPEED_2 = 150;



void setup() {

  Serial.begin(9600);
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

void loop() {
  // put your main code here, to run repeatedly:

    currentMillis = millis();
    
    flameVal = digitalRead(flameSenDP);   // FIRE SAFETY DISTANCE CHECK
    
    if(flameVal == HIGH) // if flame is detected
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
                   if (flameVal != HIGH)
                   {
                       digitalWrite(In_1, HIGH); digitalWrite(In_2, LOW); digitalWrite(In_3, LOW); digitalWrite(In_4, HIGH);    // FORWARD
                                                           
                       analogWrite(PWM_1, SPEED_1);    // SPEED CONTROL [FAST]
                       analogWrite(PWM_2, SPEED_1); 
                   }
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
                    digitalWrite(In_1, HIGH);  digitalWrite(In_2, LOW); /*digitalWrite(In_3, HIGH); digitalWrite(In_4, LOW);*/    // RIGHT

                    analogWrite(PWM_1, SPEED_2);    // SPEED CONTROL [SLOW]
                    analogWrite(PWM_2, SPEED_2);

                    break;
                  }
        case 52:
                  {
                    /*digitalWrite(In_1, LOW);  digitalWrite(In_2, HIGH);*/ digitalWrite(In_3, LOW); digitalWrite(In_4, HIGH); // LEFT

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
                    for (int angle=77; angle>=15; angle--)
                        {
                          demoServo.write(angle);
                          delay(10);
                        }
                    
                    servoSweep_Start_Stop = 1;
                    
                    break;
                  }
                  
                  
      }

       
      
        Serial.println(data);
   }

   if (servoSweep_Start_Stop == 1)
   {
     if (BTserial.read() == 79)
     {
      servoSweep_Start_Stop = -1;
     }
     servoSweep_Start_Stop = servoSweep();
     
   }
   
}



// EXPERIMENTAL !!! //

int servoSweep() 
{

 int r = 1;
 
 if (BTserial.read() == 79)
    {
      demoServo.write(servoAngleDefault);
      r = -1;   // STOPPING THE SWEEP ON DEMAMD
      return (r);
    }
   
 if (currentMillis - previousMillis >= intervalSweep)
 {
         
  if (BTserial.read() == 79)
   {
     demoServo.write(servoAngleDefault);
     r = -1;   // STOPPING THE SWEEP ON DEMAMD
     return (r);
   }
    
   previousMillis = previousMillis + intervalSweep;
   
   servoAngle = servoAngle + servoAngle_INCRI_DECRI; 
   

   if ((servoAngle >= 130) || (servoAngle <= 15))
   {         
     servoAngle_INCRI_DECRI = - servoAngle_INCRI_DECRI; // reverse direction
              
     servoAngle = servoAngle + servoAngle_INCRI_DECRI;
   }

       
   demoServo.write(servoAngle);   // ACTUALLY SWEEPING THE SERVO HERE   
   
 }

return (r);
}



// CONTROLLING THE OVERALL SERVO SWEEPING
/*void servoSweep ()
{
  int stopSweepLoop = 0;
  int switchCase = 1;
  
  while (stopSweepLoop != 1)   // SWEEPING STARTS HERE
  {                     
     switch(switchCase)
     {

      case 1:
              {                                  
                if (BTserial.read() == 79)    // NGL BUT I FORGOT WHY I PUT THIS HERE. IF I REMOVE THIS, THE BT INTERRUPT WON'T WORK
                {
                  stopSweepLoop = 1;
                  break;
                }

                stopSweepLoop = sweep_R2L ();   // SWEEPING FROM RIGHT TO LEFT
                  
                switchCase = 2;
                
                if (BTserial.read() == 79)
                {
                  switchCase = 0;
                  stopSweepLoop = 1;
                  break;
                }
                
                break;
              }

       case 2:
              {                                  
                if (BTserial.read() == 79)    // NGL BUT I FORGOT WHY I PUT THIS HERE. IF I REMOVE THIS, THE BT INTERRUPT WON'T WORK
                {
                  stopSweepLoop = 1;
                  break;
                }

                stopSweepLoop = sweep_L2R ();   // SWEEPING FROM LEFT TO RIGHT

                switchCase = 1;
                
                if (BTserial.read() == 79)
                {
                  switchCase = 0;
                  stopSweepLoop = 1;
                  break;
                }
                                                  
                break;
              }
     }                                                           
  }
return;
}*/



// FOR SWEEPING RIGHT TO LEFT
/*int sweep_R2L ()
{
  int r = 0;
  
  for (int angle=15; angle<=130; angle++)
  {                         
    if (BTserial.read() == 79)
    {
      r = 1;
      break;
    }
    
    /*if ((currentMillis - previousMillis_1) >= intervalSweep)
    {
      previousMillis_1 = currentMillis;
      demoServo.write(angle);
    }*/
    
    //demoServo.write(angle);
    //delay(10);
                                
  //}

  //return (r);
//}*/



// FOR SWEEPING LEFT TO RIGHT
/*int sweep_L2R ()
{
  int r = 0;
  
  for (int angle=130; angle>=15; angle--)
  {                         
    if (BTserial.read() == 79)
    {
      r = 1;
      break;
    }
    
    /*if ((currentMillis - previousMillis_2) >= intervalSweep)
    {
      previousMillis_2 = currentMillis;
      demoServo.write(angle);
    }*/
    
    //demoServo.write(angle);
    //delay(10);                            
//  }

  //return (r);
//}*/
