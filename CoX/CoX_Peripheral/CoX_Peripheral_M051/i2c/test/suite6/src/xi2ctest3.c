//*****************************************************************************
//
//! @page xi2c_testcase xi2c register test
//!
//! File: @ref xi2ctest.c
//!
//! <h2>Description</h2>
//! This module implements the test sequence for the xi2c sub component.<br><br>
//! - \p Board: NUC140 <br><br>
//! - \p Last-Time(about): 0.5s <br><br>
//! - \p Phenomenon: Success or failure information will be printed on the UART. <br><br>
//! .
//!
//! <h2>Preconditions</h2>
//! The module requires the following options:<br><br>
//! - \p Option-define:
//! <br>(1)None.<br><br>
//! - \p Option-hardware:
//! <br>(1)Connect an USB cable to the development board.<br><br>
//! - \p Option-OtherModule:
//! <br>Connect an COM cable to the development board.<br>
//! .
//! In case some of the required options are not enabled then some or all tests
//! may be skipped or result FAILED.<br>
//!
//! <h2>Test Cases</h2>
//! The module contain those sub tests:<br><br>
//! - \subpage test_xi2c_register
//! .
//! \file xi2ctest.c
//! \brief xi2c test source file
//! \brief xi2c test header file <br>
//
//*****************************************************************************

#include "test.h"
#include "xhw_memmap.h"
#include "xhw_i2c.h"
#include "xi2c.h"

//*****************************************************************************
//
//!\page test_xi2c_register test_xi2c_register
//!
//!<h2>Description</h2>
//!Test xi2c register. <br>
//!
//
//*****************************************************************************

unsigned long ulMaster = I2C0_BASE;
unsigned char ucTempData[5] = "rong";
unsigned char ucTemp[5];
unsigned char ucTempCount = 0;
unsigned char ucTempCount1 = 0;

static tI2CMasterTransferCfg I2CMasterTrCfg;

static unsigned char SendBuf[] = "helloi2c";
static unsigned long SendLength =  9;
static unsigned char SlaveAddress = 0x12;

static unsigned char ReceiveBuf[20] = "\r\n";
static unsigned char ReceiveLength = 9;

//*****************************************************************************
//
//! \breif i2c slave interrupt install function
//! 
//! \return none
//
//*****************************************************************************
unsigned long I2CSlaveCallback(void *pvCBData, 
                               unsigned long ulEvent,
                               unsigned long ulMsgParam,
                               void *pvMsgData)
{
    if(ulMsgParam == xI2C_SLAVE_EVENT_RREQ)
    {

    }
    else if (ulMsgParam == xI2C_SLAVE_EVENT_TREQ)
    {

    }
    return 0;
}

//*****************************************************************************
//
//! \brief Get the Test description of xi2c001 register test.
//!
//! \return the desccription of the xi2c001 test.
//
//*****************************************************************************
static char* xI2C001GetTest(void)
{
    return "xI2C, 001, I2C Peripheral Clock Source Set test";
}

//*****************************************************************************
//
//! \brief something should do before the test execute of xi2c001 test.
//!
//! \return None.
//
//*****************************************************************************
static void xI2C001Setup(void)
{      
    //
    // Enable the i2c peripheral
    //
    SysCtlKeyAddrUnlock();  
    xSysCtlClockSet(12000000, xSYSCTL_XTAL_12MHZ | xSYSCTL_OSC_MAIN);
    
    //
    // Congigure the i2c pin
    //
    xSPinTypeI2C(I2C0SCK, PD5);
    xSPinTypeI2C(I2C0SDA, PD4);
    
    //
    // Enable the i2c peripheral
    //
    xSysCtlPeripheralEnable(xSYSCTL_PERIPH_I2C0);
    
    //
    // I2C master transfer config
    //
    I2CMasterTrCfg.ulSlave = SlaveAddress;
    I2CMasterTrCfg.pvWBuf = SendBuf;
    I2CMasterTrCfg.ulWLen = SendLength;
    I2CMasterTrCfg.ulWCount = 0;
    I2CMasterTrCfg.pvRBuf = ReceiveBuf;
    I2CMasterTrCfg.ulRLen = ReceiveLength;
    I2CMasterTrCfg.ulRCount = 0;   
    
}


//*****************************************************************************
//
//! \brief something should do after the test execute of xi2c001 test.
//!
//! \return None.
//
//*****************************************************************************
static void xI2C001TearDown(void)
{   int i;
    for (i=0;i<4;i++)
    {
        ucTemp[i] = 0;
    }
    xSysCtlPeripheralReset2(ulMaster);
}



//*****************************************************************************
//
//! \brief xi2c 001 test execute main body.
//!
//! \return None.
//
//*****************************************************************************
static void xI2C001Execute(void)
{
    
    I2CMasterInit(ulMaster, 400000);
    I2CEnable(ulMaster);
    
    xI2CMasterWriteS1(ulMaster, 0x12, 'a', xfalse);
    xI2CMasterWriteS2(ulMaster, 'b', xfalse);
    xI2CMasterWriteS2(ulMaster, 'c', xfalse);
    xI2CMasterWriteS2(ulMaster, 'd', xfalse);
    
    xI2CMasterReadBufS1(ulMaster, 0x12, ucTemp, 4, xtrue);
    TestAssert(ucTemp[0] == 'a',
                   "xi2c, \"I2C send or receive\" error!");
    TestAssert(ucTemp[1] == 'b',
                   "xi2c, \"I2C send or receive\" error!");
    TestAssert(ucTemp[2] == 'c',
                   "xi2c, \"I2C send or receive\" error!");
    TestAssert(ucTemp[3] == 'd',
                   "xi2c, \"I2C send or receive\" error!");
}


//*****************************************************************************
//
//! \brief Get the Test description of xi2c002 register test.
//!
//! \return the desccription of the xi2c002 test.
//
//*****************************************************************************
static char* xI2C002GetTest(void)
{
    return "xI2C, 002, I2C Peripheral Clock Source Set test";
}

void 
PrintTest(char *pcMsg)
{
    while (*pcMsg != '\0')
    {
        TestIOPut(*pcMsg++);
    }
}
//*****************************************************************************
//
//! \brief xi2c 002 test execute main body.
//!
//! \return None.
//
//*****************************************************************************
static void xI2C002Execute(void)
{
    xtBoolean bFlag;
    I2CMasterInit(ulMaster, 400000);
    I2CEnable(ulMaster);
    
    xI2CMasterWriteBufS1(ulMaster, 0x12, ucTempData, 4, xfalse);
    
    xI2CMasterReadS1(ulMaster, 0x12, ucTemp, xfalse);    
    xI2CMasterReadS2(ulMaster, &ucTemp[1], xfalse);
    xI2CMasterReadS2(ulMaster, &ucTemp[2], xfalse);
    xI2CMasterReadS2(ulMaster, &ucTemp[3], xtrue);
    
    xI2CMasterReadBufS1(ulMaster, 0x12, ucTemp, 4, xtrue);
    
    xI2CMasterWriteBufS1(ulMaster, 0x12, ucTempData, 4, xfalse);
    xI2CMasterReadBufS1(ulMaster, 0x12, ucTemp, 4, xtrue);
    
    xI2CMasterReadS1(ulMaster, 0x12, ucTemp, xfalse);
    xI2CMasterReadS2(ulMaster, &ucTemp[1], xfalse);
    xI2CMasterReadS2(ulMaster, &ucTemp[2], xfalse);
    xI2CMasterReadS2(ulMaster, &ucTemp[3], xtrue);
    
    TestAssert(ucTemp[0] == 'r',
                   "xi2c, \"I2C send or receive\" error!");
    TestAssert(ucTemp[1] == 's',
                   "xi2c, \"I2C send or receive\" error!");
    TestAssert(ucTemp[2] == 't',
                   "xi2c, \"I2C send or receive\" error!");
    TestAssert(ucTemp[3] == 'u',
                   "xi2c, \"I2C send or receive\" error!");
    
    bFlag = I2CMasterTransfer(I2C0_BASE, &I2CMasterTrCfg, I2C_TRANSFER_POLLING);
}

//
// xi2c register test case struct.
//
const tTestCase sTestXi2c001Register = {
		xI2C001GetTest,
		xI2C001Setup,
		xI2C001TearDown,
		xI2C001Execute
};

//
// xi2c register test case struct.
//
const tTestCase sTestXi2c003Register = {
		xI2C002GetTest,
		xI2C001Setup,
		xI2C001TearDown,
		xI2C002Execute
};


//
// xi2c test suits.
//
const tTestCase * const psPatternXi2c003[] =
{
    &sTestXi2c001Register,
    &sTestXi2c003Register,
    0
};