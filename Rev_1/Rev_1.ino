
///// CAN SENSE FLAMES NOW  /////

#include <SoftwareSerial.h>

SoftwareSerial BTserial(11, 12); // RX | TX

int Pol_1 = 2;
int Pol_2 = 3;
int Pol_3 = 4;
int Pol_4 = 5;

int flameSenDP = 8;
int flameLED = 7;
int digitalVal;

int PWM_1 = 9;
int PWM_2 = 10;

int SPEED_1 = 200;
int SPEED_2 = 100;

void setup() {

  Serial.begin(9600);
  BTserial.begin(9600);

  
  pinMode(Pol_1, OUTPUT);
  pinMode(Pol_2, OUTPUT);
  pinMode(Pol_3, OUTPUT);
  pinMode(Pol_4, OUTPUT);

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
                   digitalWrite(Pol_1, HIGH); digitalWrite(Pol_2, LOW); digitalWrite(Pol_3, LOW); digitalWrite(Pol_4, HIGH);    // FORWARD
                                                       
                   analogWrite(PWM_1, SPEED_1);    // SPEED CONTROL [FAST]
                   analogWrite(PWM_2, SPEED_1); 

                   break; 
                  }
        case 50:
                  {
                   digitalWrite(Pol_1, LOW);  digitalWrite(Pol_2, HIGH); digitalWrite(Pol_3, HIGH); digitalWrite(Pol_4, LOW);     // BACKWARD
                   
                   analogWrite(PWM_1, SPEED_1);    // SPEED CONTROL [FAST]
                   analogWrite(PWM_2, SPEED_1); 
                   
                   break;
                  }
        case 51:
                  {
                    digitalWrite(Pol_1, HIGH);  digitalWrite(Pol_2, LOW); digitalWrite(Pol_3, HIGH); digitalWrite(Pol_4, LOW);    // RIGHT

                    analogWrite(PWM_1, SPEED_2);    // SPEED CONTROL [SLOW]
                    analogWrite(PWM_2, SPEED_2);

                    break;
                  }
        case 52:
                  {
                    digitalWrite(Pol_1, LOW);  digitalWrite(Pol_2, HIGH); digitalWrite(Pol_3, LOW); digitalWrite(Pol_4, HIGH); // LEFT

                    analogWrite(PWM_1, SPEED_2);    // SPEED CONTROL [SLOW]
                    analogWrite(PWM_2, SPEED_2);
                    
                    break;
                  } 

        case 88:
                  {
                    digitalWrite(Pol_1, LOW); digitalWrite(Pol_2, LOW); digitalWrite(Pol_3, LOW); digitalWrite(Pol_4, LOW); // STOP
                    
                    analogWrite(PWM_1, 0);    
                    analogWrite(PWM_2, 0);

                    break;
                  }
                  
      }
        Serial.println(data);
   }
   delay(50);
}
