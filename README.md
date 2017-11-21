# MSP430 Seven-Segment Display Driver
Interface the 4-Digit 7-Segment Display and HT16K33 for the MSP430F5529

# Instructions
To run this code, the user must have MSP430Ware installed on their Code Composer Studio.
The project should use the MSP430 DriverLib project template. It is crucial because the project
uses driverlib.h. The code uses a data buffer to provide the right information for the 7-segment 
display. The array indices for the buffer that tell the display which numbers to present are 0, 1,
3, and 4. Index 2 is used to tell the display whether or not to show the comma in between. 


