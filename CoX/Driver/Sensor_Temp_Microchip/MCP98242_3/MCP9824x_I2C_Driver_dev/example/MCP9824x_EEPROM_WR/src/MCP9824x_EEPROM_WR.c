#include "xhw_types.h"
#include "xhw_memmap.h"
#include "xcore.h"
#include "xi2c.h"
#include "xsysctl.h"
#include "xhw_sysctl.h"
#include "xgpio.h"
#include "MCP98242_3.h"

//
// Enable I2C, GPIO clock
// Set pins to I2C funciton
//
void pinSet()
{
    //
    // Enable GPIO Clock
    //
    xSysCtlPeripheralEnable(xGPIOSPinToPeripheralId(PA8));
    xSysCtlPeripheralEnable(xGPIOSPinToPeripheralId(PA9));

    xSysCtlPeripheralEnable(xGPIOSPinToPeripheralId(PB13));

    xSPinTypeI2C(I2C0SDA, PA8);
    xSPinTypeI2C(I2C0SCK, PA9);
}

//
// Set MCP98242Dev struct:
// 1. I2C 
// 2. EEPROM Slave Address
// 3. Function to configure the GPIO Pins. 
//
MCP98242Dev dev = {I2C0_BASE, 0, 0x54, 0, 0, 0, 0, pinSet};

void eepromWR()
{
    unsigned char multWrite[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    unsigned char page_addr_data[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                                        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    unsigned char addr, *value;

    int i;

    //
    // Initialize the EEPROM.
    //
    MCP98242EEPROMInit(&dev, 20000);

    
    addr = 0xA0;
    value = multWrite;
    //
    // Write EEPROM with the content of multWrite from address 0xA0.
    //
    for(i=0; i<8; i++, addr++)
    {
        MCP98242ByteWrite(&dev, addr, multWrite[i]);
        xSysCtlDelay(10000);
    }

    //
    // Change part of multWrite.
    //
    value[0]=value[1]=value[2]=0x55;
    SysCtlDelay(10000);

    //
    // Read from address 0xA0 of EEPROM.
    //
    MCP98242MemRead(&dev, addr, value, sizeof(multWrite));

    if(page_addr_data[2] == 0x02)
    {
        //MCP98242ByteWrite and Read succeed!
    }

    addr = 0xB0;
    value = page_addr_data;

    //
    // Write a Page from address 0xB0 with content of page_addr_data.
    //
    MCP98242PageWrite(&dev, addr, value);

    //
    // Change part of page_addr_data.
    //
    value[0]=value[1]=value[2]=value[15]=value[14]=value[13]=0x55;
    SysCtlDelay(10000);

    //
    // Read from address 0xB0 of EEPROM.
    //
    MCP98242MemRead(&dev, addr, value, sizeof(page_addr_data));

    if(page_addr_data[13] == 0x0D)
    {
        //PageWrite and Read succeed!
    }

    //
    // Change part of page_addr_data.
    //
    value[0]=value[1]=value[2]=value[13]=value[14]=0x55;
    
    //
    // Sequential Read, read 15 byte sequentially from address 0xB0 of EEPROM.
    //
    MCP98242MemRead(&dev, addr, value, 15);

    if(page_addr_data[13] == 0x0D)
    {
        //Sequential Read succeed!
    }

    //
    // Current Address Read.We will read from address 0xBF.
    // Value in address 0xBF equels page_addr_data[15].
    //
    MCP98242MemRead(&dev, 0, value, 0);

    if(*value == 0x0F)
    {
        //Current Address Read succeed!
    }
   
}
