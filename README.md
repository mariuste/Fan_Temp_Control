# Temperature Controlled 4-Pin Fan with Arduino Nano

## Intro
This code was written to control the temperature inside a server cabinet by measuring the temperature
with a DS18b20 Temperature probe and outputting a PWM signal with the Arduino Nano to control a 4-Pin fan.
Unlike the cheap control boards from Amazon or Ebay this code switches the fan off when the temperature
is low enough.

## Overview:
![Wireing Diagram](https://github.com/mariuste/Fan_Temp_Control/blob/main/image/WiringDiagram_bb.png)

## Features
Defining features of the code:
- simple code which can be easily altered or extended
- serial output for easy debugging
- adjustable parameters

## Acknowledgment
These two projects were used for inspiration and code snippets, especially the pwm timer part:
- for basic structure: "pwm_fan" by KaptenJansson https://create.arduino.cc/projecthub/KaptenJansson/pwn-fan-controller-with-temp-sensing-and-button-override-4f2e8d/embed
- for PWM timers: "How To Properly Control PWM Fans With Arduino". by Federico Dossena https://fdossena.com/?p=ArduinoFanControl/i.md
