

echo off
set v_repo_root_self=..\..\..\..\..
set v_cox_peripheral_manual=%v_repo_root_self%\CoX\CoX_Peripheral\CoX_Peripheral_Manual
set v_doxygen_cfg=.\cox_en.cfg

echo --------------------------------------------
echo generate doxygen(en/html,en/rtf) 
doxygen %v_doxygen_cfg% >>nul
copy %v_repo_root_self%\CoX\Driver\LCD_Character\HD44780\Character_LCD_HD44780_Driver\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\LCD_Character\UC1601\uc1601_single\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\Memory_Flash_Atmel\AT25FS0x\AT25FS0x_Single\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\Memory_Flash_ST\M25Pxx\M25Pxx_single\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\Memory_Flash_Winbond\W25X\W25X_SPI_Driver\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\Memory_SDCard\SDCard\SDCard_SPI_Driver\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\Sensor_Temp_ADI\AD7415\AD7415_I2C_Driver\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\Sensor_Temp_ADI\AD7417\AD7417_I2C_Driver\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\Sensor_Temp_ADI\ADT75\Sensor_Temp_ADT75_Driver\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\Sensor_Temp_ADI\ADT7301\ADT7301_SPI_Driver\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\Sensor_Temp_Maxim\DS18B20\DS18B20_Driver\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\Sensor_Temp_Microchip\MCP9800_3\MCP9800_3_I2C_Driver_dev\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\Sensor_Temp_Microchip\MCP9804_5\MCP9804_I2C_Driver_dev\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\Sensor_Temp_Microchip\MCP98242_3\MCP9824x_I2C_Driver_dev\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\Sensor_Temp_Microchip\TC72\TC72_SPI_Driver\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\Sensor_Temp_Microchip\TC74\TC74_I2C_Driver\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\Sensor_Temp_Microchip\TC77\TC77_SPI_Driver\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\Sensor_Temp_Microchip\TCN75\TCN75_I2C_Driver\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\CoX\Driver\Sensor_Temp_ST\STLM75\STLM75_Driver\doc\images\* ..\en\html >>nul

copy %v_cox_peripheral_manual%\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\resource\doxygen\css\* ..\en\html >>nul
echo completed.
echo --------------------------------------------

if not defined v_repo_root pause





