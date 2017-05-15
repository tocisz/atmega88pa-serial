======================
SPI init driver
======================
The Serial Peripheral Interface (SPI) allows high-speed synchronous data transfer between the device
and peripheral units, or between several AVR devices.

Features
--------
* Initialization

Applications
------------
* External memory (EEPROM, FLASH)
* External peripheral
* MCU to MCU communication

Dependencies
------------
* SPI peripheral
* clk\ :sub:`io` for clock, enabled by setting PRR.PRSPIn to 0
* IO lines for signals

Concurrency
-----------
N/A

Limitations
-----------
N/A

Knows issues and workarounds
----------------------------
N/A
