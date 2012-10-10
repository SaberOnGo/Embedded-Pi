

echo off
set v_repo_root_self=..\..\..\..\..
set v_cox_peripheral_manual=%v_repo_root_self%\CoX\CoX_Peripheral\CoX_Peripheral_Manual
set v_doxygen_cfg=.\cox_en.cfg

echo --------------------------------------------
echo generate doxygen(en/html,en/rtf) 

doxygen %v_doxygen_cfg% >>nul
echo 0.
copy %v_repo_root_self%\CoX\Shield\Adafruit_Industries\LCD_Shield\doc\images\* ..\en\html >>nul
echo 1.
copy %v_repo_root_self%\CoX\Shield\Adafruit_Industries\Motor_Shield\doc\images\* ..\en\html >>nul
echo 2.
copy %v_repo_root_self%\CoX\Shield\Adafruit_Industries\Sensor_Shield\doc\images\* ..\en\html >>nul

echo 3.
copy %v_cox_peripheral_manual%\doc\images\* ..\en\html >>nul
copy %v_repo_root_self%\resource\doxygen\css\* ..\en\html >>nul
echo completed.
echo --------------------------------------------

if not defined v_repo_root pause





