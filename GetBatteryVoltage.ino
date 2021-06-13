/*********************************************************************************************************************************************
 * ThatPersonOverThere, APR 15-29 2021
 * Project: E-Paper-Google-Calendar
 * 
 * getBattery Function 
 *  This function is responsible for getting the battery voltage. It simply measures the battery voltage on an analog input 10 times, averages
 *  the summed value then multiplys by the mean caluclation offset and battery offset. The resulting value is then send back to be displayed 
 *  elsewhere.
 *  
 *  This is directly from Kevin Darrah's base firmware designed for the trigBoard:
 *  https://github.com/krdarrah/trigBoardV8_BaseFirmware/blob/master/battery.ino
 **********************************************************************************************************************************************/
float GetBatteryVoltage() {
  //Declare a local variable for counting the analog val of battery
  int batteryRawCount = 0;
  
  //For as long as count is less than 10, add the total analog val of the previos reads to the current read
  for (int i = 0; i < 10; i++) {
    batteryRawCount = batteryRawCount + analogRead(BatteryPin);
  }

  //Get the mean or average of the raw count, we divide by 10 for the 10 reads above
  batteryRawCount = batteryRawCount / 10;

  //Declare variables for calculation error
  float m = 0.002424242; //Mean
  float b = 0.562424242; //Battery offset (calibrated to his origional battery+ extra unessary code for this project)

  //Add the battery voltage with the offset 
  float batteryVoltage = batteryRawCount * m + b;

  //Return the battery voltage to be displayed!
  return batteryVoltage;
}//end of GetBatteryVoltage
