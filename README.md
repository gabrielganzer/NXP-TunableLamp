## Operational Systems for Embedded Systems - Tunable Lamp ##

This project was implemented using a S32K144 evaluation board running the Micrium µC/OS3 operating system. Features covered by the implementation:
1. The S32K144 RGB led simulating the lamp;
2. The S32K144 potentiometer for varying the lamp intensity;
3. The S32K144 SW2 and SW3 as input buttons for changing the lamp color;
The lamp operates as follows:
1. At the start-up the RED color is selected, with an intensity proportional to the potentiometer position. When the potentiometer output is 0V, the RGB is activated by a 10 KHz PWM signal with 0% duty cycle (i.e., the led is always OFF); when the potentiometer output is 5V, the RGB is activated by a 10 KHz PWM signal with 100% duty cycle (i.e., the led is always ON). When the potentiometer output is between 0V and 5V, the RGB is activated by a 10 KHz PWM signal whose duty cycle is proportional to the potentiometer output;
2. Each time SW2 is pressed, the selected color is changed. The sequence in RED -> GREEN -> BLUE- > RED.
3. Each time SW3 is pressed, the selected color is changed. The sequence is RED -> BLUE -> GREEN -> RED.
4. If any of the button (SW2/SW3) remains pressed, the color shall not change.
