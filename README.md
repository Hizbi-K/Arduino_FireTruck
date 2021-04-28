

# Arduino_FireTruck

<p align="center">
  <img src="FireTruck_Demonstration_Gifs\Front_Page.jpg" />
</p>


# Introduction

This project is aimed at beginners who want to get more involved into Arduino programming. The project is based around a FireTruck which is controlled through an Andriod phone. The MicroController used here is an Arduino Nano. The FireTruck can be controlled to drive around and get to the fire to extinguish it manually. 

Another feature of this FireTruck is the automatic mode, where just pressing a button on the Android will make the FireTruck locate a fire in its surroundings of atmost 2.5 feet. It will go near the fire and stop at a safe distance. The water pump and servo is then engaged to sweep the water nozzel across the fire and spray to extinghish it. This is all done automatically. The successs rate for automatic mode is 75%.

And also just for fun, the Water Pump and Servo sweeping can triggered manually without any fire detection or nearby fires to spray around some water!


# Fire Detection Algorithm

The fire detection algorithm used here is not an iterative process. The FireTruck tries to lock onto the fire once and then moves towards it, without checking again to corrects its path. This sometime causes miss alignsment problems with the fire, and making the FireTruck go across the fire without stopping at a safe distance to extinguish it. Best option will be to use an algorithm that continously checks for the fire on its way and adjusts it path accordingly. Better IR Flame Sensors can also be used to help in accurate fire detection. 


# Important Safety Feature

The FireTruck can detect when it is too close to the fire. At the distance of approximately 2" from the fire, the FireTruck will not move forwards into the fire and the white LED wil be switched on. It can only turn and move backwards to get back to a safe distance. This safety feature is always enabled ON, for both Manual and Automatic modes.


# Components

This list contains all the components required to recreate the FireTruck:

1. Arduino Nano
2. Four Wheels + 5 V Motors
3. Three IR Flame Sensors
4. HC-06 Bluetooth Module (Voltage divider required.) 
5. Three 18650 Lithium-Ions Cells (Use 2000 mAh if possible, otherwise 1200 mAh should work too.)
6. 5V Water Pump
7. Three Cells 18650 Battery Holder
8. DC - DC Step Down Voltage Regulator
9. Base Plate for Mounting all the Componenets
10. One LED for Fire Safety Detection (I used white LED.)
11. One Relay Module with 2 Relays (Only one will be used.)
12. One L298N Motor Driver Module
13. Self Locking Switch (For disconnecting BT module when uploading code to the Arduino.)
14. Resistor (Metal/Carbon Film): 220 ohms, 2000 ohms, 1000 ohms
15. Electrolytic Capacitors For Power Supply Decoupling: 470 uf at 25 V (Not mandatory)
16. Lots of Jumper Wires/Cables (M-M, M-F, F-F)
19. Small Pipe for Pumping the Water


# Images

<p align="center">
  <img src="FireTruck_Demonstration_Gifs\Top_and_Front.jpg" />
</p>

<p align="center">
  Top and Front View, showing All the Components
</p>


<p align="center">
  <img src="FireTruck_Demonstration_Gifs\Side_View.jpg" />
</p>

<p align="center">
  Side View, showing the Relay Module fixed under the Platform
</p>


# FireTruck Ability Demonstration

<p align="center">
  <img src="FireTruck_Demonstration_Gifs\Basic_Movements.gif" />
</p>

<p align="center">
  Basic Movements
</p>


<p align="center">
  <img src="FireTruck_Demonstration_Gifs\Basic_Water_Servo_Movements.gif" />
</p>

<p align="center">
  Basic Water Servo Movements
</p>


<p align="center">
  <img src="FireTruck_Demonstration_Gifs\Manual_Extinguishing_With_Water.gif" />
</p>

<p align="center">
  Manual Extinguishing with Water
</p>


<p align="center">
  <img src="FireTruck_Demonstration_Gifs\Automatic_Detection_Without_Water.gif" />
</p>

<p align="center">
  Automatic Detection Only (Without using the Water Pump to Extinguish the Fire)
</p>


<p align="center">
  <img src="FireTruck_Demonstration_Gifs\Automatic_Extinguishing_With_Water.gif" />
</p>

<p align="center">
  Automatic Fire Detection and Extinguishing with Water
</p>


# Revision History

### Rev_1:

* Can sense flames now

### Rev_2:

* Can rotate Servo now in beautiful sweeping motion 
* Fixed some variables names
* Fixed DC Motors not working if Servo is enabled (Don't use D9 and D10 for PWM when using "Servo.h")

### Rev_3:

* Now can't move closer to fire than 6" 
* Truck now turns like a normal car 
* Servo can now sweep infinitely and stop on demand (not using external interrupts) 

### Rev_4.0:

* Truck will now turn in two modes: Rotationary & Revolutionary 
* Servo sweep moved into a function, to be called in the main "loop()"  
* Replacing "delay()" with "millis()" for multitasking  
* BlueTooth and Servo libraries replaced by 3rd party ones with much better support for working together 

### Rev_4.5:

* Everything has been moved into functions thus simplifying the program

### Rev_5_Final

* Flame sensors can now output analog values (1023 -> 0) 
* Truck will now rotate till the fire is straight ahead 
* Truck will move towards the fire and extinguish it 
* Now using hardware serial for BlueTooth 
* Everything shifted to a longer breadboard, caps added and power supply simplified 
* Servo's sweeping range has been reduced


# Recreating This Project:

When recreating this project on your own, you might face some issues either coding or hardware components related. 

Make sure to use the Rev_5_Final revision for the Arduino code since it contains all the features without any bugs or issues. The other revisions are just for the sake of record.

The code itself is commented to make it easier for beginners to follow and understand.

Arduino Uno board can also be used instead of Nano, the only disadvantage being that Uno occupies more space and isn't breadboardable.

If there are any problems, I will be available for your queries at hizbullahkhan192@gmail.com.

<p align="center">
  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
</p>