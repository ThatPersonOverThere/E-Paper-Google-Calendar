/***************************************************************************
 * ThatPersonOverThere, APR 15-29 2021
 * Project: E-Paper-Google-Calendar
 ***************************************************************************/

/*
HOW TO GET YOUR OWN ROOT CERTIFICATE
1. To get a root certificate on Google Chrome go to your
    calendar_url link. (USING WINDOWS 10)
2. Click on the safety lock in the left side of the search bar
3. Click "Certificate(Valid)"
4. Click "Certificate Path"
5. Double Click "GTS CA 101" or the 2nd to last certificate 
    in the pop up menu
6. Click "Details" in the new pop up window
7. In the lower right click "Copy to File..."
8. In the Certification Export Wizard click "Next"
9. Select "Base-64 encoded x.509 (.CER)" then click "Next"
10. Click "Browse" fill in the name of the file and save it
      in a easily accessable location 
11. Open the certificate in Notepad or better yet Notepad++
12. EDIT the certificate to have all nessary
     "~~~~~\n" \ etc. So it looks like the one used in the
     main code
NOTE: CERTIFICATES DO EXPIRE SO THEY WILL NEED TO BE REPLACED
*/

//Learn about HTTPClient for an ESP32 here https://github.com/espressif/arduino-esp32/tree/master/libraries/HTTPClient 
