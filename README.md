# Real-time Lab monitoring using TinyML
# by Rahul Khanna D

RP2040 temperature monitor using WIZnet Ethernet HAT and edgeimpulse. 
![WIZnet Ethernet HAT](./images/1.jpg)

## Why Did We Build This? & The Solution

Prevention is better than cure" is one of the effective measures to prevent the spreading of COVID-19 and to protect mankind. Many researchers and doctors are The maintenance of the ambiance in places such as laboratories, especially chemical labs is essential. The temperature and humidity have to be monitored continuously and the level of air quality and heat levels are used as an interrupt to prevent laboratory accidents. The data from the sensors are fed to an ML algorithm to identify an anomaly and inform the security team via email. 

## Requirements

### Hardware

* Raspberry Pi Pico 
* WIZnet Ethernet HAT
* DHT22 Temperature Sensor (optional)
* microUSB cable
* LAN cable
* 1S Li-ion Power bank

### Software

*   Arduino IDE 1.8.9
*   Edge Impulse Studio
*   Arduino-pico Library
*   WIZnet Arduino Ethernet Library


## Basic Hardware Components
  
### Raspberry Pi Pico

  Raspberry Pi Pico is a low-cost, high-performance microcontroller board with flexible digital interfaces. Key features include:

* RP2040 microcontroller chip designed by Raspberry Pi Foundation
* Dual-core Arm Cortex M0+ processor, flexible clock running up to 133 MHz
* 264KB of SRAM, and 2MB of on-board Flash memory
* USB 1.1 with device and host support
* Low-power sleep and dormant modes
* 26 × multi-function GPIO pins
* 2 × SPI, 2 × I2C, 2 × UART, 3 × 12-bit ADC, 16 × controllable PWM channels
* Accurate clock and timer on-chip
* Temperature sensor
* Accelerated floating-point libraries on-chip
* 8 × Programmable I/O (PIO) state machines for custom peripheral support

![Raspberry Pi Pico](./images/pico-pin.jpg)  
  
## WIZnet Ethernet HAT
  
  WIZnet Ethernet HAT (Hardware Attached on Top) is a Raspberry Pi Pico pin-compatible board that utilizes W5100S and supports both 3.3V & 5V.

  Please refer to this link to find more information about W5100S.

* Raspberry Pi Pico Pin-Compatible
* Ethernet (W5100S Hardwired TCP/IP CHIP)

![WIZnet Ethernet HAT pinout](./images/pinout_wiznet.png)  

Features

* Includes W5100S
* Supports Hardwired Internet Protocols: TCP, UDP, WOL over UDP, ICMP, IGMPv1/v2, IPv4, ARP, PPPoE
* Supports 4 Independent Hardware SOCKETs simultaneously
* Internal 16 Kbytes Memory for TX/ RX Buffers
* Operation Voltage 3.3V / 5V - Built-in LDO (LM8805SF5-33V)
* SPI Interface 5V I/O tolerance
* 10 / 100 Ethernet PHY embedded
* Supports Auto Negotiation - Full / Half Duplex, 10 / 100 Based
* Built-in RJ45 (RB1-125BAG1A)
  
## Connections
  
  Solder the female berg sticks to the Raspbeery Pi Pico as shown below. 

![img 2](./images/2.jpg)

  Attach the WIZnet Ethernet Hat to the Raspberry Pi Pico as shown. 
  
![img 3](./images/3.jpg)
  
Now the connection are done. We will be using the internal temperature sensor from the Raspberry Pi Pico for data collection. 

## IDE setup 

  The next step is to add the Raspberry Pi Pico to the Arduino IDE. This is done by the following steps. First, copy the following text
  " https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json " and paste it to the File->Preferences and click OK. 
  
![ ](./images/ss1.png)
  
  Once done, go to Tools -> Boards -> Boards Manager and type "pico" in the search box. Install the Raspberry Pi Pico / RP 2040 board to the IDE. 

![ ](./images/ss2.png)  

Now the board is installed to the Arduino IDE.

## An approach of this projects
The block diagram of the project is shown below. The Raspberry Pi pico is mounted to the WIZnet HAT. With the temperture data being serial pushed, an ML model is generated using Edge Impulse. With the inference output, the MQTT/IFTTT is triggered to send the alert to the user. 

![ ](./images/block.png)


## TinyML on Raspberry Pi Pico 
The main idea of this project is to use Raspberry Pi pico to detect some anomaly on the temperature data and the entire classification is done on the RP2040 MCU. 

Depending on the hardware capacity and memory size, different types of MCU/Application can be used in the TinyML, as shown in the bellow chart.

![ ](./images/comp.png)

To run a machine learning model on the Raspberry Pi pico, we need the ambient temperature data. Since the Raspberry Pi Pico has internal temperature sensor, we use them to collect the temperature data. The ARM Cortex M0+ is suitable for the Anomaly Detection and Sensor Classification which allows us to implement this project. 

**Data Collection from Sensor**
The First step is to collect the temperature data from the internal temperature sensor. Data is loaded using the built-in CLI which reads the sensor data from Pico's serail port. We can also import the data in the form of CSV or JSON file format. 

Arduino Code to read internal temperature and send data via serial port
```
void setup() {
  Serial.begin(115200);
  delay(5000);
}

void loop() {
  Serial.println(analogReadTemp());
  delay(1000);
}
```
![ ](./images/ss3.png)  


Output Printed on Serial Monitor
![ ](./images/ss4.png)  

At your terminal, run:

''' edge-impulse-data-forwarder '''

The data forwarder will ask you for the server you want to connect to, prompt you to log in, and then configure the device.
This is an example of the output of the forwarder:

'''
Edge Impulse data forwarder v1.5.0
? What is your user name or e-mail address (edgeimpulse.com)? jan@edgeimpulse.com
? What is your password? [hidden]
Endpoints:
    Websocket: wss://remote-mgmt.edgeimpulse.com
    API:       https://studio.edgeimpulse.com
    Ingestion: https://ingestion.edgeimpulse.com

[SER] Connecting to /dev/tty.usbmodem401203
[SER] Serial is connected
[WS ] Connecting to wss://remote-mgmt.edgeimpulse.com
[WS ] Connected to wss://remote-mgmt.edgeimpulse.com
? To which project do you want to add this device? accelerometer-demo-1
? 3 sensor axes detected. What do you want to call them? Separate the names with ',': accX, accY, accZ
? What name do you want to give this device? Jan's DISCO-L475VG
[WS ] Authenticated

'''
Now go to Data acquisition and you should see it:
![ ](./images/a1.png)  

Now start sampling data using sample length long enough. At the end, you have the raw data samples that we will use to detect anomaly using Raspberry Pi Pico and machine learning:

![ ](./images/a2.png)  

Next, let us create the machine learning model that will use on the Pico to detect anomalies:

![ ](./images/a3.png)  

Then, let us extract features from the raw data and under anomaly detection select the following axes:

![ ](./images/a4.png)  


## Setting up WIZnet Lib to Arduino IDE

First we download libraries from the link below. 
1. https://github.com/WIZnet-ArduinoEthernet/Ethernet/tree/8419aa392750642c58a5d18bb850624b67f32007
2. https://github.com/WIZnet-ArduinoEthernet/arduino-pico

Click on Sketch -> Include Library -> Add .ZIP Library. Now add the downloaded libraries to the Arduino IDE. 

Classifying data (Arduino)
![ ](./images/a5.png)  

Now it is time to detect anomaly with Rapberry Pi Pico. Try to run the code above and when the values read adhere to the normal pattern you will have:
![ ](./images/temp.jpg)  

Now try to blow on the sensor and you will notice that the machine learning model will detect the anomaly. Once the anomaly is detected, the data is sent to the server which in return sends an alert to the mail. 

![ ](./images/a6.png)  

![ ](./images/a7.png)  

*If you faced any issues in building this project, feel free to ask me. Please do suggest new projects that you want me to do next.*

*Share this video if you like.*

*Happy to have you subscribed: https://www.youtube.com/c/rahulkhanna24june?sub_confirmation=1*

**Thanks for reading!**
