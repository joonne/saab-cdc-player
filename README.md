# saab-cdc-player

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
- Raspberry Pi Zero (W)
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
