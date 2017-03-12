======================
EXINT init driver
======================

The External Interrupts are triggered by the INT0 pin or any of the PCINT pins. Observe that, if enabled,
the interrupts will trigger even if the INT0 or PCINT[27:0] pins are configured as outputs. This feature
provides a way of generating a software interrupt.

Features
--------
* Initialization

Applications
------------
* Interrupts triggered by external stimuli
* Software generated interrupts

Dependencies
------------
N/A

Knows issues and workarounds
----------------------------
N/A
