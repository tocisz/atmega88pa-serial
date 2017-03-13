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
