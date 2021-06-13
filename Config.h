/*********************************************************************************************************************************************
 * ThatPersonOverThere, APR 15-29 2021
 * Project: E-Paper-Google-Calendar
 * 
 * This is the user configuration file. All that the user should configure is here. This is to just take the code and run it off your own WIFI
 * a your own web app url as well as own root certificate... FYI This code will only work with the trigBoard unless you customize 
 * the SPI pins. That would be in includes.h should you not want to buy a $30 trigBoard...  
 *********************************************************************************************************************************************/
#include "HowToGetRootCertificate.h"
#include "HowToReadAJavaScriptFile.h"

//CONFIGURE YOUR SPI PINS HERE
//You can find these in the DisplayData.ino
#define SCK   14
#define MISO  12
#define MOSI  13
#define SS    15

//You Can find these refrenced in the includes.ino
#define DC    33
#define RST   32
#define BUSY  39

//This is a variable used for the number of 30 minute sessions that the ESP32 wakes
// it is set to 17 because the number is 1 above the amount of 30 minute sessions
// nessary to properly opperate. This is because upon boot, the ESP32 increments 
// the boot count so, by the first boot it hasnt slept 30 minutes.
int numOfBootsToUpdate = 17;

//Define WiFi SSID and Password
const char* SSID{"YOUR_SSID"};
const char* PASSWORD{"YOUR_PASSWORD"};

//Declare NTP timezone, and dayling savings offset
//See: https://lastminuteengineers.com/esp32-ntp-server-date-time-tutorial/
const char* NTP_SERVER{"ch.pool.ntp.org"}; //To get the time
const long  UTC_EST_OFFSET_SECS{3600 * -5}; //3600 sec = 1 hour, times that by -4 means we are going back 4 hours from GMT, GMT would be {0} ( SET TO EST TIME ZONE)
const int   DAYLIGHT_OFFSET_SECS{3600}; //Set to 3600 for day light savings time, 0 for no savings! 

//Declare custom Web-App url 
const char* GoogleScript_URL{"https://script.google.com/macros/s/YOUR_GOOGLE_SCRIPT_KEY/exec"};

//Certificate Valid from 6/14/2017 to 12/14/21
const char* ROOT_CA{\
"-----BEGIN CERTIFICATE-----\n" \
"MIIESjCCAzKgAwIBAgINAeO0mqGNiqmBJWlQuDANBgkqhkiG9w0BAQsFADBMMSAw\n" \  
"HgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMjETMBEGA1UEChMKR2xvYmFs\n" \  
"U2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjAeFw0xNzA2MTUwMDAwNDJaFw0yMTEy\n" \  
"MTUwMDAwNDJaMEIxCzAJBgNVBAYTAlVTMR4wHAYDVQQKExVHb29nbGUgVHJ1c3Qg\n" \  
"U2VydmljZXMxEzARBgNVBAMTCkdUUyBDQSAxTzEwggEiMA0GCSqGSIb3DQEBAQUA\n" \  
"A4IBDwAwggEKAoIBAQDQGM9F1IvN05zkQO9+tN1pIRvJzzyOTHW5DzEZhD2ePCnv\n" \  
"UA0Qk28FgICfKq----------------------------------EPZRrAzDsiKUDzRr\n" \  
"mBBJ5wudgzndIM-----GET YOUR OWN CERTIFICATE-----11BmsK+eQmMF++Ac\n" \  
"xGNhr59qM/9il7----------------------------------JP0aM3T4I+DsaxmK\n" \  
"FsbjzaTNC9uzpFlgOIg7rR25xoynUxv8vNmkq7zdPGHXkxWY7oG9j+JkRyBABk7X\n" \  
"rJfoucBZEqFJJSPk7XA0LKW0Y3z5oz2D0c1tJKwHAgMBAAGjggEzMIIBLzAOBgNV\n" \  
"HQ8BAf8EBAMCAYYwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMBIGA1Ud\n" \  
"EwEB/wQIMAYBAf8CAQAwHQYDVR0OBBYEFJjR+G4Q68+b7GCfGJAboOt9Cf0rMB8G\n" \  
"A1UdIwQYMBaAFJviB1dnHB7AagbeWbSaLd/cGYYuMDUGCCsGAQUFBwEBBCkwJzAl\n" \  
"BggrBgEFBQcwAYYZaHR0cDovL29jc3AucGtpLmdvb2cvZ3NyMjAyBgNVHR8EKzAp\n" \   
"MCegJaAjhiFodHRwOi8vY3JsLnBraS5nb29nL2dzcjIvZ3NyMi5jcmwwPwYDVR0g\n" \  
"BDgwNjA0BgZngQwBAgIwKjAoBggrBgEFBQcCARYcaHR0cHM6Ly9wa2kuZ29vZy9y\n" \  
"ZXBvc2l0b3J5LzANBgkqhkiG9w0BAQsFAAOCAQEAGoA+Nnn78y6pRjd9XlQWNa7H\n" \
"TgiZ/r3RNGkmUmYHPQq6Scti9PEajvwRT2iWTHQr02fesqOqBY2ETUwgZQ+lltoN\n" \
"FvhsO9tvBCOIazpswWC9aJ9xju4tWDQH8NVU6YZZ/XteDSGU9YzJqPjY8q3MDxrz\n" \
"mqepBCf5o8mw/wJ4a2G6xzUr6Fb6T8McDO22PLRL6u3M4Tzs3A2M1j6bykJYi8wW\n" \
"IRdAvKLWZu/axBVbzYmqmwkm5zLSDW5nIAJbELCQCZwMH56t2Dvqofxs6BBcCFIZ\n" \
"USpxu6x6td0V7SvJCCosirSmIatj/9dSSVDQibet8q/7UK4v4ZUN80atnZz1yg==\n" \
"-----END CERTIFICATE-----\n"};
