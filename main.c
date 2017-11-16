#include <msp430.h> 
#include <driverlib.h>

#define LED_MATRIX_SLAVE_ADDRESS 0x70 // Default for Holtek HT16K33 LED Controller

// Note: From the Holtek HT16K33 LED Controller datasheet, this brightness array has B3 position=0.
// It appears that if B3 position == 1, the display will be brighter.  Try this setting later.
uint8_t brightnessArray[8] = {0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7};

//Space Invader
//uint8_t greenArray[8] = {0x98, 0x5C, 0xB6, 0x5F, 0x5F, 0xB6, 0x5C, 0x98};
//uint8_t redArray[8] = {0x00, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x00};

// Test Pattern 1
//uint8_t greenArray[8] = {0xCC, 0xCC, 0x33, 0x33, 0xCC, 0xCC, 0x33, 0x33};
//uint8_t redArray[8] = {0x3C, 0x3C, 0x0F, 0x0F, 0xC3, 0xC3, 0xF0, 0xF0};

// Test Pattern 2
uint8_t greenArray[8] = {0xFF, 0x81, 0x81, 0x89, 0x91, 0x81, 0x81, 0xFF};
uint8_t redArray[8] = {0x00, 0x7E, 0x42, 0x4A, 0x52, 0x42, 0x7E, 0x00};

uint8_t counter;
uint8_t column;
uint8_t address;

int main(void)
{
    WDT_A_hold(WDT_A_BASE);                 // Disable the watchdog timer

    //****************************************************************************************
    // Set the I2C operation appropriately
    //****************************************************************************************

    // Assign I2C pins to USCI_B1
    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P4,
            GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2
            );

    // Init. master device
    // Note: The HT16K33 I2C LED Backpack datasheet states that the fSCL Clock Frequency should be 400kHz
    //       and that the MPU host is the master (and not slave).


    /*
    void USCI_B_I2C_initMaster(uint16_t baseAddress, USCI_B_I2C_initMasterParam *param)

    USCI_B_I2C_masterInit(USCI_B1_BASE,
                          USCI_B_I2C_CLOCKSOURCE_SMCLK,
                          UCS_getSMCLK(),
                          USCI_B_I2C_SET_DATA_RATE_400KBPS
                          );
    */
    
    USCI_B_I2C_initMasterParam parameter = { 0 };
    parameter.dataRate |= USCI_B_I2C_SET_DATA_RATE_400KBPS;
    parameter.i2cClk |= UCS_getSMCLK();
    parameter.selectClockSource |= USCI_B_I2C_CLOCKSOURCE_SMCLK;
    USCI_B_I2C_initMaster(USCI_B1_BASE, &parameter);

    // Specify slave address
    USCI_B_I2C_setSlaveAddress(USCI_B1_BASE,
                               LED_MATRIX_SLAVE_ADDRESS
                               );

    // Set in transmit mode
    USCI_B_I2C_setMode(USCI_B1_BASE,
                       USCI_B_I2C_TRANSMIT_MODE
                       );

    USCI_B_I2C_enable(USCI_B1_BASE);

    // Enable the internal oscillator for the Holtek HT16K33 LED Controller
    USCI_B_I2C_masterMultiByteSendStart(USCI_B1_BASE, 0x21);
    USCI_B_I2C_masterMultiByteSendStop(USCI_B1_BASE);

    // Enable the display and disable blinking for the Holtek HT16K33 LED Controller
    USCI_B_I2C_masterMultiByteSendStart(USCI_B1_BASE,0x81);  // D={1}, B1B0={0} Continuous
//    USCI_B_I2C_masterMultiByteSendStart(USCI_B1_BASE,0x83);  // D={1}, B1B0={0} 2Hz
//    USCI_B_I2C_masterMultiByteSendStart(USCI_B1_BASE,0x85);  // D={1}, B1B0={0} 1Hz
//    USCI_B_I2C_masterMultiByteSendStart(USCI_B1_BASE,0x87);  // D={1}, B1B0={0} 0.5Hz
    USCI_B_I2C_masterMultiByteSendStop(USCI_B1_BASE);

    // Set the dimming for the Holtek HT16K33 LED Controller
    USCI_B_I2C_masterMultiByteSendStart(USCI_B1_BASE,brightnessArray[7]);
    USCI_B_I2C_masterMultiByteSendStop(USCI_B1_BASE);

    // Send the LED Matrix data in the greenArray and redArray
    for (address=0, column=0; column < 8; address=address+2, column++)
    {
        USCI_B_I2C_masterMultiByteSendStart(USCI_B1_BASE, address);
        USCI_B_I2C_masterMultiByteSendNext(USCI_B1_BASE, greenArray[column]);
        USCI_B_I2C_masterMultiByteSendNext(USCI_B1_BASE, redArray[column]);
        USCI_B_I2C_masterMultiByteSendStop(USCI_B1_BASE);
    }

    while (1);

    return (0);
}

