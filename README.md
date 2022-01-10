# Temperature Controlled 4-Pin PWM-Fan with Arduino Nano

## Intro
This code was written to control the temperature inside a server cabinet by measuring the temperature
with a DS18b20 Temperature probe and outputting a PWM signal with the Arduino Nano to control a 4-Pin fan.
Unlike the cheap control boards from Amazon or Ebay this code switches the fan off when the temperature
is low enough (0% PWM and optional High-Side-Switch).

**Choice of fan:** Please note that the shut-off feature only works if the fan supports it (tuns off when PWM = 0%). Not all fans do this.
According to the 4-Pin Fan specification by Intel 0%-Shutoff is supported on "Type B" and "Type C" 4-Pin Fans, not on "Type A" fans.

I think most CPU-Fans do offer 0%-Shutoff while most case-fans have a minimum RPM even at 0% PWM. For fans without this feature the circuit has to be modified if you want to shut if off at low temperatures. A simple high side switch will work (1 PMOS + 1 NMOS or 1 PNP+NPN Transistor).

You can test if your fan will shut off at 0% PWM by connecting the the fan to 12V and GND and connecting the PWM pin of the fan to GND. If it stops spinning everything is fine and the code arduino will work as intended (fan is Type B or Type C). But when it spins you need the mentioned high side switch for turning it off completely at low temperature (Type A fan).

If you don't mind the fan always slowly spinning at low temperatures every fan will work.

## Overview:
![Wireing Diagram](https://github.com/mariuste/Fan_Temp_Control/blob/main/image/WiringDiagram_bb.png)

## Features
Defining features of the code:
- simple code which can be easily altered or extended
- serial output for easy debugging
- adjustable parameters
- temperature regulation between minimum an maximum temperature (to minimize noise)
- shuts off fan at low temperatures (motivation for this project because the server network cabinat is located in a bedroom); only works if the fan supports 0% PWM mode

Application in network cabinet:
![Application in network cabinet](https://github.com/mariuste/Fan_Temp_Control/blob/main/image/Application_Image.jpeg)


## Acknowledgment
These two projects were used for inspiration and code snippets, especially the pwm timer part:
- for basic structure: "pwm_fan" by KaptenJansson https://create.arduino.cc/projecthub/KaptenJansson/pwn-fan-controller-with-temp-sensing-and-button-override-4f2e8d/embed
- for PWM timers: "How To Properly Control PWM Fans With Arduino". by Federico Dossena https://fdossena.com/?p=ArduinoFanControl/i.md
