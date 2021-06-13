/*********************************************************************************************************************************************
 * ThatPersonOverThere, APR 15-29 2021
 * Project: E-Paper-Google-Calendar
 * 
 * This is the includes file which houses all global variables and includes we can get in here!!! 
 *********************************************************************************************************************************************/

//For ConnectGet
  #include <WiFi.h> //allows connection to internet
  #include <HTTPClient.h> // used to get the calendar events
  #include "time.h"// used for retrieving time from an NTP server
  //Declare the max time the ESP32 will try and get HTTP data 
  const unsigned long MAX_WAIT_MILLIS{60000}; //in milliseconds thus * 1000 to get 60s or 1min
  
  //Define the string that recieves the text ouput of Google Scripts
  String GS_DATA;

  //Set the number of array spaces to 8, thus only 7 events will be shown at a time 
  const int arrlimit = 8; //Set array limit to 8 to avoid memory problem on ESP32
                           //Memory problems involve either boot loops OR parsing Garbage!
                           //Because of this each array will have an extra buffer space
          
  //DATA ARRAYs: Declare START, month, date, time, year arrays
  String arrSTmonth[arrlimit] = { "" };
  String arrSTdate[arrlimit] = { "" };
  String arrSTtime[arrlimit] = { "" };
  String arrSTyear[arrlimit] = { "" };
      
  //DATA ARRAYs: Declare END, month, date, time, year arrays
  String arrEDmonth[arrlimit] = { "" };
  String arrEDdate[arrlimit] = { "" };
  String arrEDtime[arrlimit] = { "" };
  String arrEDyear[arrlimit] = { "" };
      
  //DATA ARRAYs:Declare event name array
  String arrEventName[arrlimit] = { "" };


//For ParseData
  //Hold characters in this stirng before we place them into a specific array
  //MUST BE GLOBAL FOR SWITCH CASE STATEMENT
  String placeHolderString = ""; 
      
  //Array that counts for each of the 9 DATA ARRAYS below
  // It is used for avoiding int count1, int Count2, etc... 
  //MUST BE GLOBAL FOR IF STATMENT  
  int arr_count[] = { 0,/*arrSTmonth*/
                      0,/*arrSTdate*/
                      0,/*arrSTtime*/
                      0,/*arrSTyear*/
                      0,/*arrEDmont*/
                      0,/*arrEDdate*/
                      0,/*arrEDtime*/
                      0,/*arrEDyear*/
                      0/*arrEventName*/};


//For DisplayData
  // base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
  // enable or disable GxEPD2_GFX base class
  #define ENABLE_GxEPD2_GFX 0
  
  //To change SPI pins for the ESP32 Used i.e. trigBoardV8
  #include <SPI.h>
  
  //Specifically for 3 color displays 
  #include <GxEPD2_3C.h>
  #define GxEPD2_DISPLAY_CLASS GxEPD2_3C
  #define GxEPD2_DRIVER_CLASS GxEPD2_750c_Z90 // GDEH075Z90  880x528
  #define GxEPD2_3C_IS_GxEPD2_3C true
  #define IS_GxEPD(c, x) (c##x)
  #define IS_GxEPD2_3C(x) IS_GxEPD(GxEPD2_3C_IS_, x)
  
  //For the ESP32 to work with the display
  #define MAX_DISPLAY_BUFFER_SIZE 65536ul // e.g.
  #define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
  
  //Adapt the constructor parameters to your wiring
  GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> display(GxEPD2_DRIVER_CLASS( SS, DC, RST, BUSY));
  
  //Include fonts from Adafruit_GFX
  #include <Fonts/FreeSerif12pt7b.h>
  #include <Fonts/FreeSerif18pt7b.h>
  #include <Fonts/FreeSerif24pt7b.h>

  //Images we want to display
  #include "ThatPersonOverThere.h"
  #include "kirby.h"
  #include "kirbyMad.h"


//For GetBatteryVoltage
  //Where we read our battery voltage from on trigBoardV8
  const int BatteryPin = 36;//analog Input


//For TimerMode
  //Defines & variables for esp_sleep.h
  #define convert_to_microseconds 1000000 // microsecond = 10^-6 thus we go 1*10^6 to get 1 second
  #define rtc_short_timer_in_seconds 300  //For 5 minutes in seconds

  //So apparently the ESP32 module I have can only go to 35 minutes reliably on timer.
  //  The trick is to use a boot count to have the esp32 boot up and go back should 
  //  the count not be high enough
  #define rtc_long_timer_in_seconds  1800 //30 minutes to make our boot count math easier..
  
  
  //Our boot count so we can see how many boots the ESP32 has gone through
  RTC_DATA_ATTR int bootCount = 0;
  RTC_DATA_ATTR int firstBootFlag = 0; 
  

//For Display Data
  //Declare this as a flag for the amount to events we want to display in for loop for 
  // our bullet points, start and end text, points for display data!
  int numOfEventsParsed;

  //For out Time server data, We need to load it into variables b/c of the way we call
  // the NTP server data in ConnectGet... This way we can display the variables correctly
  // on the display!
  char theWeekday[10];        //10 because Wednesday is the longest return so that 9 characters + a null terminator
  char theMonth[10];          //10 because September ^
  char theDay[3];             //3 because date as 1 to 31 is 2 characters + null terminator
  char theYear[5];            //5 because Year is 4 characters plus null terminator
  char lastUpdatedHour[3];    //3 because hour as 1 to 24 is 2 characters + null terminator
  char lastUpdatedMinute[3];  //3 because date as 1 to 59 is 2 characters + null terminator
  bool noTimeFromNTP_Flag = false; //set it false so it dosent trigger later on.... 


//Declare our functions spread across tabs (names self explintory)
void ConnectGet();
void ParseData(); 
void printOnScreen(); //Function contained in ParseData Tab for printing parsed arrays on serial monitor
float GetBatteryVoltage();
void DisplayData();
void TimerMode(int rtc_time);
