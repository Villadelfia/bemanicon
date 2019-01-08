# BemaniCon
A USB HID controller fully supporting BT 5.X with 11 digital inputs, 12 HID lights, 2 quadrature encoders, and a 4x4 matrix keypad.

The schematic is not finalized but has been tested and works. 

There are a few flaws in the current pcb: 

- None of the buttons need access to the +5v rail. In the next revision the pinout for those will be GND GND SENSE LED.
- It seems that not all pins have internal pullups, the next version will have external pullups. For now, either disable the B0 and D5 pins in the code or tie them to positive via the PWR header.
- Hardware debouncing should be added, though the software solution seems to work fine. Until hardware debouncing is added, it is advised to only use quadrature encoders utilizing hall effect sensors or those using optical interruptors.
- The decoupling caps should be lay-out in a more sensible manner, though it seems to work just fine.
- The option should be provided to power the shift registers from a separate USB supply with the option to tie them to the main supply.