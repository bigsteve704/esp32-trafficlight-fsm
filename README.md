# esp32-trafficlight-fsm
This is an ESP32-based traffic light state system that makes use of two 74HC595 shift registers and a seven segment display, along with LEDs, to mimic a traffic light system with a pedestrian countdown and button.

<img width="768" height="1024" alt="FB9F5871-F0C2-4E36-8C58-8229CCC93903_1_105_c" src="https://github.com/user-attachments/assets/52bf5e1e-92a1-47b4-b407-cd21f82900cd" />


***HARDWARE***

1x ESP32 
2x 74HC595 Shift Registers
5x LEDs
1x Push Button 
1x Seven-Segment Display
Wires


***CODE***

The .ino file contains the code for the state system. The code uses a State enum data type to cycle through different states such as RED, GREEN, YELLOW, and REDGO which insinuates that a pedestrian button was pressed before the light turned red. The program cycles through all these states, displays them over the shift registers to the LEDs, and displays a countdown for the pedestrians if the button was pressed and the REDGO state is shifted out. 
