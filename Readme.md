#PWM to RGB converter

Firmware for AVR microcontroller to work as a converter from PWM signal (PC fan control) to signals for RGB LED strip (not ARGB!). 
The rarget device is ATTiny85, though may be ported to any device having at least 3 PWM channels (in ATTiny 85 there are 
2 8-bit timers with 2 PWM outputs on each), 2 ADC inputs (maybe even 1, if the level of switching from rainbow to 
'load color' is hardcoded), and at least 2Kb ROM. EEPROM is not used. Fosc = 1 MHz. PWM is working with Fosc/64. So ATTiny25/45 should work
without any changes in the code.

The device provides two functions - show a color matching PWM duty cycle (0% - green, 100% - red, and yellow - orange flavors in between) 
and rainbow pattern. Due to lack of pins there're no controls for speed, modes and whatever else. The only control - a variable resistor 
to set the level of switching from 'rainbow' to 'PWM load'.

If PWM duty cycle is less than set by the resistor, the working mode is 'rainbow'. If cycle becomes higher, the mode is switched to show
the color of corresponding duty cycle value. So, by turning variable resistor to GND point, it is possible to turn 'rainbow' mode off and 
show only 'PWM load', and turning to VCC point - to turn off 'PWM' and enabling only 'rainbow'.

- inverting DC
- stab 5v

###Current status
works in simulator

###TODOs
- check PWM signal params (when oscilloscope comes)
- implement RC-circuit
- test on real device
- put ascii schematic here in readme
- put photo/videos?
- check readme for grammar
- go public on github



