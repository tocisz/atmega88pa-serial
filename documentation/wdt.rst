======================
WDT driver
======================
The Watchdog Timer (WDT) is a system function for monitoring correct
program operation. It allows to recover from error situations such as
runaway or deadlocked code.  The WDT is a timer, configured to a
predefined timeout period, and is constantly running when enabled. If
the WDT is not Reset within the timeout period, it will issue a System
Reset. The WDT is reset by executing the WDR (Watchdog Timer Reset)
instruction from the application code.

For devices having the Enhanced Watchdog Timer, it can also be
configured to produce an interrupt instead of a System Reset, or a
combination of the two.

Features
--------
* Initialization

Applications
------------
* Recover MCU from runaway or deadlock condition
* In interrupt mode, wake up from sleep modes, or use as a general system timer (Enhanced WDT only)

Dependencies
------------
* SYSCTRL for system control

Security
--------
* WDT Change Enable bit (WDCE) in the control register (WDTCSR).

Concurrency
-----------
N/A

Limitations
-----------
N/A

Knows issues and workarounds
----------------------------
Some datasheets lack important information of how to change the
Watchdog Enable (WDE) and Watchdog Prescaler (WDP) bits in the
Watchdog Timer Control register (WDTCSR):

In order to disable the Watchdog Timer, or change the prescaler
settings, both WDCE *and WDE* must be written to logic one
first. The following procedure must be followed:

1. In the same operation, write a logic one to WDCE and WDE. A logic
one must be written to WDE even if it was set to one previously.
2. Within the next four clock cycles, write the desired value to the
WDE bit and the WDP bits in a single operation.

