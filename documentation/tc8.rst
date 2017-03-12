======================
TC8 driver
======================
A 8-bit Timer/Counter is a general purpose Timer/Counter module, with two independent Output
Compare Units, and PWM support. It allows accurate program execution timing (event management) and
wave generation.

Features
--------
* Initialization

Applications
------------
* PWM/Frequency generation
* Event counting/Input capture
* Application timing, and scheduling

Dependencies
------------
* clk\ :sub:`io` for clock
* Module is enabled by setting PRR.PRTIMn to 0

Knows issues and workarounds
----------------------------
N/A
