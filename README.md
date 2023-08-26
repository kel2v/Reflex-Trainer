# Reflex-Trainer
Simple implementation of a device that's used to improve our Reflexes (response time), using Raspberry Pi's Pico Mircrocontroller


## Inspiration
I saw a device in a youtube video where Formula 1 drivers used it as a part of game in which the participant's reflex is compared against time. Here's the video: https://youtu.be/BhLr43elNCE , Timestamp: 0:40.  This project aims to implement such a device.

## Materials Required
  Here's the list of products required for the project. The link following each product is to familarize you with the type of component I have used. These are not sponsored products and I'm in no way rewarded by any party. You can use alternate products of same kind (Except Pico microcontroller) as long as the working principle and terminals are same with the one I have shown in the link.


1. Raspberry Pi Pico H: 1 unit  

2. Generic Infrared Proximity Sensor: 5 units (https://robocraze.com/products/ir-proximity-sensor-1)  
When there's no detection of object, the **OUT** terminal transmits **Logical HIGH** signal (3.3v), and when the object is detected, it sends **Logical LOW** signal (0v or GND) and turns on the detection light on the sensor.  
4. Generic 16x2 LCD Display: 1 unit (https://robocraze.com/products/16x2-lcd-blue)  
Please make sure that your display supports data transmitted at 3.3v as Pico's Digital GPIOs only produce 3.3v at Logical HIGH  
5. LCD-I2C Adapter: 1 unit (https://robocraze.com/products/16x2-lcd-i2c-interface-adapter)
6. momentary push button switch: 1 unit (https://robocraze.com/products/2-pin-button-switch-pack-of-10)
7. 3.3v Passive/Active Buzzer speaker: 1 unit   
    Passive: https://robocraze.com/products/10mm-continuous-buzzer-pcb-mount  
    Active: https://robocraze.com/products/active-buzzer-module
8. Micro-USB cable: 1 unit (https://www.pishop.us/product/50cm-usb-micro-usb-cable/)
9. Jumper wires: M-M (40 units), M-F (40 units), F-F (40 units) (https://robocraze.com/products/jumper-wire-set-m2m-m2f-f2f-40-pcs-each)  
Please note that the project actually requires less number of cables than its stated here; 40 units is given as each type of cables is commonly available in set of 40 cables.
10. 840 Points Breadboard : 1 unit (https://robocraze.com/products/breadboard)
11. 3mm Green LED: 5 units (https://robocraze.com/products/3mm-green-led-pack-of-10)
12. Ribbon cable 10-wire: atleast 2 meters (https://robocraze.com/products/ribbon-cable-10-wire-1-meter)


## Block Diagram  
![image](https://github.com/kel2v/Reflex-Trainer/blob/3c8ba6ac32a195e84022a73b3ff432b0cfb31963/images/blockDiagram1.jpg)


**IRPS = Infrared Proximity Sensor  
LED = Light Emitting Diode (3mm Green)**  

Here, each IRPS is numbered from 0 to 4 and every IRPS is coupled with a 3mm Green LED, and they too are numbered from 0 to 4.

  
## Working
#### When the Pico is powered on:
1. turns on all 5 LEDs  

2. Displays “Press Button to start”
3. waits for button input to start the test

#### when the test is started:
1. turns off all the LED lights  

2. Displays “Ready” and gives a short beep
3. Displays “Set” and gives another short beep
4. Displays “Go” and gives a long beep.
5. Then, one of the five LEDs turn on randomly and the Pico waits for the corresponding IR sensor to report detection of the user palm.
6. After the detection of user’s palm, the corresponding IR sensor signals the Pico about the detection and immediately the Pico turns off the corresponding LED. The time elapsed between the turning on and off of this LED is recorded and saved by the Pico.
7. Then again, one of the five sensors’ LED is randomly lit on and again the Pico waits for that corresponding sensor to report the detection of the user’s palm. This cycle goes on for 10 times.
8. After the 10th round, no more LEDs are turned on, and the Pico starts to calculate the total sum of all the response times, fastest response time and the slowest response time. Finally it displays all these results. The sum value is displayed in the top line of the 16x2 display and, the fastest and slowest response times are displayed on the left and right of the second line of the display, respectively.
9. The result stays on screen for 10 secs and then the Pico resets the system to its waiting mode by turning on all the LEDs and again starts waiting for the button input to start the test. This cycle continues indefinitely till the power supply is cut-off.


#### Termination:  
The button serves 2 functions to our device:
1. To start the test
2. To stop the test in the middle of the test.

when the test is running, pressing the button immediately stops the test and returns to the waiting mode, except when any sensor is already waiting for user’s palm detection, then pressing the button doen’t terminate the test until the user clears that detection.  

#### Note:
1. After pressing the button, any successive button presses within the next 500 ms are discarded.
2. If button is pressed while the result is displayed, after 10 sec display time, the device immediately starts the next test.


## Connection

#### CAUTION: It is assumed that you would power the Pico with a USB cable which mostly supplies 5v power. Therefore, the Vcc supply rail is connected to Vbus terminal of Pico as it provides the same 5v thats supplied by USB. If you want to supply power with other sources like battery, connect +ve and -ve terminals of the power source to the supply rail's Vcc and GND rails respectively, and to Vsys and GND (any) of Pico respectively. Please refer the offical datasheet of Pico (https://datasheets.raspberrypi.com/pico/pico-datasheet.pdf) for more details on powering Pico with external source

Before connections, make sure you couple each LED to a IRPS. The idea is to consider the IRPS-LED pair as a single component with 4 terminals (3 terminals of IRPS: **Vcc**, **GND** and **OUT**, and 1 terminal of LED: **+ve terminal**). One idea is to solder the **-ve terminal** (shorter terminal of LED) of the LED with the **GND** terminal of the IRPS, thus end up having 4 terminals.  

 ![image](https://github.com/kel2v/Reflex-Trainer/blob/3c8ba6ac32a195e84022a73b3ff432b0cfb31963/images/connections4.jpg)


1. Connect IRPS0’s **LED** terminal to GP0,  **Data** terminal to GP1, **Vcc** terminal to 5v supply and **GND** terminal to ground  

2. Connect IRPS1’s **LED** terminal to GP4 and **Data** terminal to GP5, **Vcc** terminal to 5v supply and **GND** terminal to ground
3. Connect IRPS2’s **LED** terminal to GP8 and **Data** terminal to GP9, **Vcc** terminal to 5v supply and **GND** terminal to ground
4. Connect IRPS3’s **LED** terminal to GP12 and **Data** terminal to GP13, **Vcc** terminal to 5v supply and **GND** terminal to ground
5. Connect IRPS4’s **LED** terminal to GP14 and **Data** terminal to GP15, **Vcc** terminal to 5v supply and **GND** terminal to ground
6. Connect all 16 pins of 16x2 LCD to corresponding pins in I2C-LCD Adapter module
7. Connect **SDA** terminal of I2C-LCD adapter to GP20, **SCL** terminal of I2C-LCD adapter to GP21, **Vcc** terminal to 5v supply and **GND** terminal to ground
8. Connect one terminal of the Buzzer to GP17 and the other to ground
9. Connect one terminal of the Button to GP22 and the other to 3v3 pin
10. Power the Micro-controller using a Micro-USB cable


## Building the UF2 binary file

I have provided the UF2 generated for the given source in the respository. If you have modified the given source and want to build the UF2 file, follow these steps:

#### Note for contributors: I need someone's help to write build instructions for Windows and MacOS systems

#### On UBUNTU systems:  


#### Note for contributors: I need someone's help to write the CMakeLists.txt file such that the user can store the source file and UF2 file that's built using the source file in any directory they wish. Right now, my suggestion for everyone is to replace the `lcd_1602_i2c.c` file and `CMakeLists.txt` file present in the `~/pico/pico-examples/i2c/lcd_1602_i2c/` directory with the files that I have provided in this repository


Open Terminal and enter following commands:

1. Install `pico-SDK` and `pico-examples` (skip if its done):  

```
cd ~/
mkdir pico
cd ~/pico
git clone https://github.com/raspberrypi/pico-sdk.git --branch master
cd ~/pico/pico-sdk
git submodule update --init
cd ~/pico
git clone https://github.com/raspberrypi/pico-examples.git --branch master
``` 

**Note:** Assuming that you have installed the `pico` directory from the official Pico repo in the `~/` (home directory), to avoid confusion, all the paths given here and from here onwards are absolute paths.

2. Install all required Toolchains (skip if its done):
```
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential gcc g++ libstdc++-arm-none-eabi-newlib
```

3. Updating the SDK (skip if its done):
```
cd ~/pico/pico-sdk
git pull
git submodule update
```

4. Prepare your cmake build directory (skip if its done):
```
cd ~/pico/pico-examples
mkdir build
cd ~/pico/pico-examples/build
export PICO_SDK_PATH=../../pico-sdk
cmake ..
```

5. Replace the `lcd_1602_i2c.c` file and `CMakeLists.txt` present in the `~/pico/pico-examples/i2c/lcd_1602_i2c/` directory with the corresponding files that I have provided in this repository. Make your necessary changes in both of these files.
6. Build the UF2 file
```
cd ~/pico/pico-examples/build/i2c/lcd_1602_i2c/
make
```

Now you have your UF2 binary named as `lcd_1602_i2c.uf2` in `~/pico/pico-examples/build/i2c/lcd_1602_i2c/` directory
