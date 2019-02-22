
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 3; //In seconds
BLEScan* pBLEScan;

int max_scan = 3;
int latchPin = 04;
int clockPin = 18;
int dataPin = 23;
byte r_byte = 0;
unsigned long timer_pwm,timer_beep;

int nf_one_count =0;
int nf_two_count =0;
int nf_three_count =0;

bool alert_flag=false;
bool beacon_one_flag = false;

std::string beacon_one_address = "ac:23:3f:29:12:78"; //address of the device you want to track

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        std::string address = advertisedDevice.getAddress().toString().c_str();
          if(address == beacon_one_address)
            beacon_one_flag=true;
          else
            beacon_one_flag=false;
    }
};

void setup() {
  Serial.begin(115200);
  BLEDevice::init("Beacon Scanner");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  if(!alert_flag){
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  pBLEScan->clearResults();
  }
  
  if(beacon_one_flag) nf_one_count =0;
  else if(!beacon_one_flag){
    nf_one_count++;
    if(nf_one_count>max_scan){
      alert_flag=true;
    }
  }
  if(alert_flag){
    beep_pattern(200,100,100,0B00000001);   /// Do something when the device is lost.In this case buzzer beeps. 
  }
  
}

void write_pwm(int high_time,int low_time,byte shift_byte){
    if(micros() - timer_pwm <= high_time){
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, r_byte|=shift_byte);
      digitalWrite(latchPin, HIGH);  
    }
    
   else if ((micros() - timer_pwm) > high_time && (micros() - timer_pwm) <(high_time + low_time) ){
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST,r_byte&=!(shift_byte));
      digitalWrite(latchPin, HIGH); 
   }
   
   else{
    timer_pwm =micros();
   }
}

void beep_pattern(int beep_interval,int high_time,int low_time,byte shift_byte){
  if(millis() - timer_beep <= beep_interval){
    write_pwm(high_time,low_time,shift_byte);
  }
  else if(millis() - timer_beep > beep_interval && millis() - timer_beep <= (2*beep_interval) ){
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST,r_byte&=!(shift_byte));
    digitalWrite(latchPin, HIGH);
}
else
timer_beep = millis();
}
