======================
ADC driver
======================

An Analog-to-Digital Converter(ADC) converts analog signals on any of its single ended voltage inputs to digital values based on reference voltage. Some devices have ADC also supporting differential mode and gain settings.

Multiple reference voltages are supported by the ADC, both internal and external with difference voltage levels. The reference voltage have an impact on the accuracy, 
and should be selected to cover the full range of the analog input signal and never less than the expected maximum input voltage.

The ADC measurements can either be started by application software, or the ADC can be free-running, starting a new measurement immediately after the previous is finished, or it can be triggered by other sources (TC overflow/compare match, external interrupt, or Analog Comparator)

Features
--------
* Initialization

Applications
------------
* Measurement of internal sensor. E.g., MCU internal temperature sensor value.
* Measurement of external sensor. E.g., Temperature, humidity sensor value.
* Sampling and measurement of a signal. E.g., sinusoidal wave, square wave.

Dependencies
------------
* ADC peripheral
* SYSCTRL for clock
* EXINT for Interrupt
* PORT for I/O Lines and Connections


Concurrency
-----------
N/A

Limitations
-----------
N/A

Knows issues and workarounds
----------------------------
N/A
