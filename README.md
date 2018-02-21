# avr-serial
My training project for ATmega88P.

This project demonstrates:
 - using timer interrupt to execute action in regular intervals
 - using PWM to dim LED (timer mentioned above changes brightness)
 - using input interrupt to handle button press
  * how to prevent button instability by ignoring button state changes for short time after a change
 - using USART interrupt to read and send data
  * integrate with libc `stdio.h`
 - using software events to minimize ISR code
  * interrupt sets flag
  * main loop checks it, clears and performs action

Connections for Nokia display:

```
1-VCC -- 3.3V
2-GND -- 0
3-SCE ------ Vol -- 0V
4-RST ------ ta  -- PB2
5-D/C ------ ge  -- PB1
6-DN<MOSI> - Ada -- PB3
7-SCLK ----- pter - PB5
8-LED -- 680 Ohm -- 3.3V
```

Other connections:
```
PC5 -- button -- 0
PC4 -- analog input
PD2 -- radio receiver
PD3 -- ?
```
