## ---- MICROPROCESSORS II: LAB 1 ---- ##
## ----------------------------------- ##
## ---------- Alan Bulavsky ---------- ##
## ----------- Ben Kelley ------------ ##
## ----------------------------------- ##
## ------------ Files: 01 ------------ ##

 FILE DESCRIPTION
-----------------------------------------
1. | trafficLightSM.iso
   | -- Arduino IDE code for a traffic light controller
   | -- begins with flashing red light, sequence begins after button press
   | -- tracks time using counter (counts in 200ms steps)
   | -- toggles between 3 main light states, as well as substates:
   |  RED, RED+BUZZ, GREEN, GREEN+BUZZ, YELLOW, YELLOW FLASH+BUZZ
   |  based on state of counter
   | -- counter resets for each light state change (R->G, G->Y, Y->R)
   |

   
 STEPS TO EXECUTE
-----------------------------------------
1. | Ensure associated schematic is constructed and connected to
   | Arduino device
   |
2. | Upload code to microprocessor from Arduino IDE 
   | 
3. | Verify red LED is flashing
   | -- if it is flashing, the code is on the device
   |
4. | Press button and observe traffic light pattern 
   |  