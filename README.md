

# Introduction

This project is aimed at beginners who want to get more involved into Arduino programming. The projectid based around a FireTruck which is controlled through a phone using an Andriod phone. The FireTruck is powered by an Arduino Nano and can be controlled to drive around and get to the fire to extinguish it manually. 

Another feature of this FireTruck is the automatic mode, through which with just a press of a button it will locate a fire in its surroundings of atmost 3 feet, then go near the fire and stop. The water pump and servo is then engaged to sweep the water nozzel across the fire and spray to extinghish it. This is all done automatically. The successs rate for automatic mode is 90%.


# Components and Materials

This list contains all the components required to recreate the FireTruck:

1. Arduino Nano

2. 4 Wheels + 5 V Motors
3. 3 IR Flame Sensors
4. HC-06 Bluetooth Module (Voltage divider required.) 
5. 3 18650 Lithium-Ions Cells (Use 2000 mAh if possible, otherwise 1200 mAh should work too.)
6. 5 V Water Pump
7. 3 Cells 18650 Battery Holder
8. DC - DC Step Down Voltage Regulator
9. Base Plate for Mounting all the Componenets
10. One LED (I used white LED.)
11. One Relay Module with 2 Relays (Only one will be used.)
12. One L298N Motor Driver Module
13. Self Locking Switch (For disconnecting BT module when uploading code to the Arduino.)
14. Resistor (Metal/Carbon Film): 220 ohms, 2000 ohms, 1000 ohms
15. Electrolytic Capacitors For Power Supply Decoupling: 470 uf at 25 V (Not mandatory.)
16. Lots Male-Male Jumper Wires
17. Lots Male-Female Jumper Wires
18. Lots FemaleE-Female Jumper Wires
19. Pipe for Pumping Water


# Revision History

### Rev_1:

* CAN SENSE FLAMES NOW

### Rev_2:

* CAN ROTATE SERVO NOW IN BEAUTIFUL SWEEPING MOTION
* FIXED VARIABLE NAMES FOR "Pol_1" TO "In_1" 
* FIXED DC MOTORS NOT WORKING IF SERVO IS ENABLED (DON'T USE D9 & D10 FOR PWM WHEN USING "Servo.h") 

### Rev_3:

* NOW CAN'T MOVE CLOSER TO FIRE THAN 6" 
* TRUCK NOW TURNS LIKE A NORMAL CAR 
* SERVO CAN NOW SWEEP INFINITELY AND STOP ON DEMAND (NOT USING EXTERNAL INTERRUPTS) 

### Rev_4.0:

* TRUCK WILL NOW TURN IN TWO MODES: ROTATIONARY & REVOLUTIONARY 
* SERVO SWEEP MOVED INTO A FUNCTION, TO BE CALLED IN THE MAIN "loop()"  
* REPLACING "delay()" with "millis()" FOR MULTITASKING  
* BT AND SERVO LIBRARIES REPLACED BY 3RD PARTY ONES WITH MUCH BETTER SUPPORT FOR WORKING TOGETHER 

### Rev_4.5:

* EVERYTHING HAS BEEN MOVED INTO FUCNTIONS THUS SIMPLIFYING THE PROGRAM 

### Rev_5_Final

* FLAME SENSORS CAN NOW OUTPUT ANALOG VALUES (1023 -> 0) 
* TRUCK WILL NOW ROTATE TILL THE FIRE IS STRAIGHT AHEAD 
* TRUCK WILL MOVE TOWARDS THE FIRE AND EXTINGUISH IT 
* NOW USING HARDWARE SERIAL FOR BLUETOOTH 
* EVERYTHING SHIFTED TO A LONGER BREADBOARD, CAPS ADDED AND POWER SUPPLY SIMPLIFIED 
* SERVO'S SWEEPING RANGE HAS BEEN REDUCED


# Recreating This Project:

When recreating this project on your own, you might face some issues either coding or hardware components related. 

Make sure to use the Rev_5_Final revision for the Arduino code since it contains all the features without any bugs or issues. The other revisions are just for the sake of record.

The code itself is commented to make it easier for beginners to follow and understand.

Arduino Uno board can also be used instead of Nano, the only disadvantage being that Uno occupies more space and isn't breadboardable.

If there are any problems, I will be available for your queries at hizbullahkhan192@gmail.com.

<p align="center">
  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
</p>