# avr-serial
This project demonstrates:
 - using timer interrupt to execute action in regular intervals
 - using PWM to dim LED (timer mentioned above changes brightness)
 - using input interrupt to handle button press
  * how to prevent button instability by ignoring button state changes for short time after a change
 - using USART interrupt to read and send data
  * only echo back action on RX (+ switch LED state)
  * very simple `puts(char *)` implementation to send button press status
