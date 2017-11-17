#include <msp430.h> 
#include <driverlib.h>

#define SSEG_SLAVE_ADDRESS 0x70 // Default address for HT16K33

uint8_t brightnessArray[8] = {0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7};
uint16_t displaybuffer[8];

int main(void)
{
    WDT_A_hold(WDT_A_BASE); // Disable watchdog timer

    // Assign I2C pins to USCI_B1
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P4, GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2);

    // Init. master device for HT16K33
    USCI_B_I2C_initMasterParam parameter = { 0 };
    parameter.dataRate |= USCI_B_I2C_SET_DATA_RATE_400KBPS;
    parameter.i2cClk |= UCS_getSMCLK();
    parameter.selectClockSource |= USCI_B_I2C_CLOCKSOURCE_SMCLK;
    USCI_B_I2C_initMaster(USCI_B1_BASE, &parameter);

    // Specify slave address
    USCI_B_I2C_setSlaveAddress(USCI_B1_BASE, SSEG_SLAVE_ADDRESS);

    // Set in transmit mode
    USCI_B_I2C_setMode(USCI_B1_BASE, USCI_B_I2C_TRANSMIT_MODE);
    USCI_B_I2C_enable(USCI_B1_BASE);

    // Enable the internal oscillator for HT16K33
    USCI_B_I2C_masterSendMultiByteStart(USCI_B1_BASE, 0x21);
    //USCI_B_I2C_masterSendMultiByteStop(USCI_B1_BASE);

    // Enable the display and disable blinking for HT16K33
    USCI_B_I2C_masterSendMultiByteStart(USCI_B1_BASE,0x81);
    USCI_B_I2C_masterSendMultiByteStop(USCI_B1_BASE);

    // Set the dimming for the HT16K33 LED Controller
    USCI_B_I2C_masterSendMultiByteStart(USCI_B1_BASE, brightnessArray[5]);
    USCI_B_I2C_masterSendMultiByteStop(USCI_B1_BASE);

    USCI_B_I2C_masterSendMultiByteStart(USCI_B1_BASE, 0x71);
    USCI_B_I2C_masterSendMultiByteNext(USCI_B1_BASE, 0x71);
    USCI_B_I2C_masterSendMultiByteNext(USCI_B1_BASE, 0x71);
    USCI_B_I2C_masterSendMultiByteNext(USCI_B1_BASE, 0x71);
    USCI_B_I2C_masterSendMultiByteStop(USCI_B1_BASE);


    for(;;);
}
