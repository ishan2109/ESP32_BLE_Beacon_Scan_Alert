# ESP32_BLE_Beacon_Scan_Alert
A program inspired by BLE_Scan example code of ESP32 BLE library by Neil Kolban to alert the user as soon as the beacon gets out of range.


This is just a snippet from a very big project that I am doing.In this code,you just need to pass the address of the beacon/(s) you want to track and as soon as the beacon gets out of range a function can be executed which in this case is a beep alert by the buzzer.

In this code whenever a device is not found in the scan,it performs max. of 3 scans before alerting the user. This can be easily changed by changing the value of int max_scan

In my particular project I have used shift register + ULN to save pins of the controller as well as write pwm signal to multiple devices to save power and control intensity of the output.
