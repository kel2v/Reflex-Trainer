# Reflex-Trainer
Simple and low budget implementation of a device that's used to improve our Reflexes (response time), using Raspberry Pi's Pico Mircrocontroller


# Materials Required
  Here's the list of products required for the project. The link following each product is to familarize you with the type of component I have used. These are not sponsored products and I'm in no way rewarded by any party. You can use alternate products of same kind (Except Pico microcontroller) as long as the working principle and terminals are same with the one I have shown in the link.


1. Raspberry Pi Pico H: 1 unit
2. Generic Infrared Proximity Sensor: 5 units. https://robocraze.com/products/ir-proximity-sensor-1
3. Generic 16x2 LCD Display: 1 unit. https://robocraze.com/products/16x2-lcd-blue
4. LCD-I2C Adapter: 1 unit. https://robocraze.com/products/16x2-lcd-i2c-interface-adapter
5. momentary push button switch: 1 unit. https://robocraze.com/products/2-pin-button-switch-pack-of-10
6. 3.3v Passive/Active Buzzer speaker: 1 unit.   
    Passive: https://robocraze.com/products/10mm-continuous-buzzer-pcb-mount  
    Active: https://robocraze.com/products/active-buzzer-module
7. Micro-USB cable: 1 unit. https://www.pishop.us/product/50cm-usb-micro-usb-cable/
8. Jumper wires: M-M (40 units), M-F (40 units), F-F (40 units). Please note that the project actually requires less number of cables than its stated here; 40 units is given as each type of cables is commonly available in set of 40 cables. https://robocraze.com/products/jumper-wire-set-m2m-m2f-f2f-40-pcs-each
9. 840 Points Breadboard : 1 unit. https://robocraze.com/products/breadboard
10. 3mm Green LED: 5 units. https://robocraze.com/products/3mm-green-led-pack-of-10
11. Ribbon cable 10-wire: atleast 2 meters. https://robocraze.com/products/ribbon-cable-10-wire-1-meter


# About
I saw a device in a Youtube video where Formula 1 drivers used it as a part of game in which the participant's reflex is compared against time. Here's the video: https://youtu.be/BhLr43elNCE  

This project aims to implement such a device at lower budget.


# Working
When the Pico is powered on:
1. turns on all 5 LEDs
2. Displays “Press Button to start”
3. waits for button input to start the test

when the test is started, the following cycle starts executing:
1. turns off all the LED lights
2. Displays “Ready” and gives a short beep
3. Displays “Set” and gives another short beep
4. Displays “Go” and gives a long beep.
5. Then, one of the five LEDs turn on at random and the Pico waits for the corresponding IR sensor to report detection of the user palm
6. After the detection of user’s palm, the corresponding IR sensor signals the Pico about the detection and immediately the Pico turns off the corresponding LED. The time elapsed between the turning on and off of this LED is recorded and saved by the Pico.
7. Then again, one of the five sensors’ LED is randomly lit on and again the Pico waits for that corresponding sensor to report the detection of the user’s palm. This cycle goes on for 10 times.
8. After the 10th round, no more LEDs are turned on, and the Pico starts to calculate the total sum of all the response times, fastest response time and the slowest response time. Finally it displays all these results. The sum value is displayed in the top line of the 16x2 display and, the fastest and slowest response times are displayed on the left and right of the second line of the display, respectively.
9. The result stays on screen for 10 secs and then the Pico resets the system to its waiting mode by turning on all the LEDs and again starts waiting for the button input to start the test. This cycle continues indefinitely till the power supply is cut-off.


Termination:
The Button serves 2 functions to our device:
1. To start the test
2. To stop the test in the middle of the test.

when the test is running, pressing the button immediately stops the test and returns to the waiting mode, except when any sensor is already waiting for user’s palm detection, then pressing the button doen’t terminate the test until the user clears that detection.
