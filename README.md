# saab-cdc-player

## Project structure

include - Put project header files here
lib - Put project specific (private) libraries here
src - Put project source files here
platformio.ini - Project Configuration File

## Goals

- emulate saab cdc
- read steering wheel buttons to control the player (be it spotify, tidal, etc.)
- play music through balanced audio input of the cdc connector

## CDC connector

- pin 7 R+
- pin 2 R-
- pin 8 L+
- pin 3 L-
- pin 5 CAN-H
- pin 11 CAN-L
- pin 6 +
- pin 12 -

## Building blocks

- Saab 9-5 Aero MY06
- CPT DC-DC step down converted 12V -> 5V
- MCP2515 CAN Interface
- Raspberry Pi Zero (W) => Raspberry Pi 4
- HiFiBerry DAC+ Pro XLR
- Arduino Nano (if I cannot pair raspi with can shield)

## MCP2515 => Arduino Nano connections

- 1 INT => GPIO 12
- 2 SCK => GPIO 11 SPI0.SCLK
- 3 SI => GPIO 10 SPIO.MOSI
- 4 SO => GPIO 9 SPIO.MISO
- 5 CS => GPIO 8 SPI0.CEO
- 6 GND => GND
- 7 VCC => GPIO 1 5V or 3.3V??

## Arduino Nano => Raspi connections

## HifiBerry => CDC connector connections

## Operating System

- Thoughts on the operating system selection
- Requirements:
  - spotify connect (spotifyd)
  - C++/Python/Rust or even just shell program to forward instructions from arduino via serial to the player
  - Home Assistant entity possibility (long run)
  - Possibility to have 4G networking

### HiFiBerryOS

- lots of players ready
- Based on buildroot, which means that integrating additional software needs to be done via buildroot build system
- MPRIS needed to control the player
- updating arduino software from here might be a no go

## RaspberryPi OS

- manual installation spotifyd
- need to control somehow, dbus or playerctl etc
- running own software way easier

## OTA

- https://github.com/rauc/rauc

## References

- https://bluesaab.blogspot.com/
- https://github.com/kveilands/BlueSaab
- https://github.com/rekomerio/trionic7-can-mcp2515
- https://circuitdigest.com/microcontroller-projects/arduino-can-tutorial-interfacing-mcp2515-can-bus-module-with-arduino
- https://pikkupossu.1g.fi/tomi/projects/i-bus/i-bus.html
- http://secuduino.blogspot.com/2012/10/emulate-cd-changer-for-old-generation.html
