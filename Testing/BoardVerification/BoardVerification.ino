#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_MAX31865.h>
#include <SPI.h>
#include <RTClib.h>
#include <EEPROM.h>
#include <ATM90E32.h>


//-----------------------------------------------------------
// Serial Communication, UI 
//-----------------------------------------------------------
#define USBSerial Serial
#define RS232 Serial2
//char* AcceptedCommands[] = {"UNITS?", "DEVICE?", "WATERSOURCE?", "WATERLEVEL?", "LPG?", "SEWAGE?", "GREY?",
//                            "ENERGY?", "BATTERY?", "RTCBATTERY?", "GENERATOR?", "TEMPS?", "UNITTEMP?", "WATERPUMPSENSE?", "WARNING?",
//                            "WATER?", "STREAMING?", "ACENMON?", "ALLDATA?", "UPDATEALL", "RESETWARNINGS", "RESETALLALARMS",
//                            "TIME?", "ACVOLTAGEGAIN?", "ACFREQ?", "ACPGAGAIN?", "ACLEGS?", "ACCT1GAIN?", "ACCT2GAIN?", "REBOOT",
//                            "RESET", "WATERDURATION?", "STREAMINGONBOOT?", "ACENMONONBOOT?", "WATERPUMPSENSEONBOOT?"
//                           };
//char* ParameterCommands[] = {"SETUNITS", "SETWATERPUMPSENSE", "WATER", "SETSTREAMINGDATA", "SETOUTPUT",
//                             "READINPUT", "SETTIME", "GETOUTPUT", "SETACENMON", "SETACFREQ", "SETACPGAGAIN",
//                             "SETACVOLTAGEGAIN", "SETACLEGS", "SETACCT1GAIN", "SETACCT2GAIN",
//                             "SETWATERDURATION", "READANALOG", "SETSTREAMINGONBOOT", "SETACENMONONBOOT",
//                             "SETWATERPUMPSENSEONBOOT"
//                            };
String inputString, inputStringRS232 = "";         // a String to hold incoming data from ports
bool stringComplete, stringCompleteRS232 = false;     // whether the string is complete for each respective port
LiquidCrystal_I2C lcd(0x27, 20, 4);
int DisplayCounter = 0;
#define LCDEnable 41
#define LCDPowerOut 23
//-----------------------------------------------------------
// ATM90E32 energy monitor
ATM90E32 eic{}; // Energy Monitor Object
unsigned short LineFreq, PGAGain, VoltageGain, CurrentGainCT1, CurrentGainCT2;
int NumberOfACLegs;
//-----------------------------------------------------------
//-----------------------------------------------------------
// System Level
RTC_DS3231 rtc;
const float ConversionFactor = 5.0 / 1023;
//-----------------------------------------------------------
//-----------------------------------------------------------
//Generator, Energy Monitoring, and Voltage
#define GenFuelPressure A12
#define RTDGenHeadRCS 45
#define RTDGenHeadLCS 47
#define RTDGenEnclosure 49
#define RREF      430.0
#define RNOMINAL  100.0
Adafruit_MAX31865 GenHeadR = Adafruit_MAX31865(RTDGenHeadRCS);
Adafruit_MAX31865 GenHeadL = Adafruit_MAX31865(RTDGenHeadLCS);
Adafruit_MAX31865 GenEnclosure = Adafruit_MAX31865(RTDGenEnclosure);
#define Camper12VoltSensor A0
#define RTCBattery A14
#define EnergyMonitorCS 24
//-----------------------------------------------------------
//-----------------------------------------------------------
//NTC Temperature Sensors
#define FrontACTemp A3
#define BackACTemp A4
#define HallwayTemp A5
#define BathroomTemp A6
#define Freezer A7
#define Refridgerator A8
#define Outside A9
#define BackCabin A10
#define FrontCabin A11
//-----------------------------------------------------------
//-----------------------------------------------------------
//Alarm
#define LEDBacklightOut 8
#define AlarmOut 4
#define WarningLED 5
#define AlarmReset 3
//-----------------------------------------------------------
//-----------------------------------------------------------
//Water Control / Tanks and LPG Tank
#define LPGSensor A1
#define WaterTankSensor A2
#define TankPowerRelay 22
#define WaterSourceSelectionInput 37
#define WaterPumpSense 43
#define KitchWaterButton 18
#define BathroomWaterButton 19
#define CityWaterValve 28
#define KitchenWaterButtonLED 6
#define BathroomWaterButtonLED 7
#define WaterPumpOut 26
//-----------------------------------------------------------
//-----------------------------------------------------------
//Spare IO and Analog
/*
   Spare Inputs are indexed by their number +1 because I don't
   want to start at zero.
*/
const int SpareInputs[] = {35, 33, 31, 29, 27, 25};
const int SpareInputSize = sizeof(SpareInputs) / sizeof(int);
int LastInputState[] = {};
const int SpareOutputs[] = {12, 11, 10, 9, 14, 15};
const int SpareOutputSize = sizeof(SpareOutputs) / sizeof(int);
const int SpareAnalog[] = {13,15};
const int SpareAnalogSize = sizeof(SpareAnalog) / sizeof(int);
//-----------------------------------------------------------
//-----------------------------------------------------------
// Holding Tank
bool ShittersGettinFull, GreyGettinFull, HoldingTankAlarm = false;
//SewageTankLevelPins
#define S14 48
#define S12 46
#define S34 44
#define S44 42
#define SewagePower 30
//GreyTankLevelPins
#define G14 40
#define G12 38
#define G34 36
#define G44 34
#define GreyWaterPower 32
//-----------------------------------------------------------
void setup() {
  rtc.begin();
  RS232.begin(115200);
  USBSerial.begin(115200);
  inputString.reserve(200);
  inputStringRS232.reserve(200);

  pinMode(LCDEnable, INPUT);
  pinMode(LCDPowerOut, OUTPUT);

  for (int i = 0; i <= SpareInputSize; i++) {
    pinMode(SpareInputs[i], INPUT);
    LastInputState[i] = digitalRead(SpareInputs[i]);
  }
  for (int i = 0; i <= SpareOutputSize; i++) {
    pinMode(SpareOutputs[i], OUTPUT);
  }

  pinMode(AlarmReset, INPUT);
  pinMode(WaterSourceSelectionInput, INPUT);
  pinMode(WaterPumpSense, INPUT);
  pinMode(KitchWaterButton, INPUT);
  pinMode(BathroomWaterButton, INPUT);

  pinMode(LEDBacklightOut, OUTPUT);
  pinMode(TankPowerRelay, OUTPUT);
  pinMode(WaterPumpOut, OUTPUT);
  pinMode(AlarmOut, OUTPUT);
  pinMode(WarningLED, OUTPUT);
  pinMode(CityWaterValve, OUTPUT);
  pinMode(KitchenWaterButtonLED, OUTPUT);
  pinMode(BathroomWaterButtonLED, OUTPUT);

  //Sewage Tank
  pinMode(SewagePower, OUTPUT);
  pinMode(S14, INPUT);
  pinMode(S12, INPUT);
  pinMode(S34, INPUT);
  pinMode(S44, INPUT);

  //Grey Water
  pinMode(GreyWaterPower, OUTPUT);
  pinMode(G14, INPUT);
  pinMode(G12, INPUT);
  pinMode(G34, INPUT);
  pinMode(G44, INPUT);

  //Generator
  pinMode(RTDGenHeadRCS, OUTPUT);
  pinMode(RTDGenHeadLCS, OUTPUT);
  pinMode(RTDGenEnclosure, OUTPUT);
  /*
      Please Reference Adafruit_MAX31865
      docs for setup of these boards if 
      you are going to use 4 or 2 wire
  */
  GenHeadR.begin(MAX31865_3WIRE);
  GenHeadL.begin(MAX31865_3WIRE);
  GenEnclosure.begin(MAX31865_3WIRE);

  //Energy
  pinMode(EnergyMonitorCS, OUTPUT);
}

void loop() {
  //SewAgeTank();
  //GreyTank();
  //GeneralInputs();
  
  }

void GeneralOutputs(){
  pinMode(LEDBacklightOut, OUTPUT);
  pinMode(TankPowerRelay, OUTPUT);
  pinMode(WaterPumpOut, OUTPUT);
  pinMode(AlarmOut, OUTPUT);
  pinMode(WarningLED, OUTPUT);
  pinMode(CityWaterValve, OUTPUT);
  pinMode(KitchenWaterButtonLED, OUTPUT);
  pinMode(BathroomWaterButtonLED, OUTPUT);
}

void GeneralInputs(){
  USBSerial.print("AlarmReset:");
  USBSerial.println(digitalRead(AlarmReset));
  USBSerial.print("WaterSourceSelectionInput:");
  USBSerial.println(digitalRead(WaterSourceSelectionInput));
  USBSerial.print("WaterPumpSense:");
  USBSerial.println(digitalRead(WaterPumpSense));
  USBSerial.print("KitchWaterButton:");
  USBSerial.println(digitalRead(KitchWaterButton));
  USBSerial.print("BathroomWaterButton:");
  USBSerial.println(digitalRead(BathroomWaterButton));
  delay(5000);
}

void SewAgeTank(){
  USBSerial.print("S14:");
  USBSerial.println(digitalRead(S14));
  USBSerial.print("S12:");
  USBSerial.println(digitalRead(S12));
  USBSerial.print("S34:");
  USBSerial.println(digitalRead(S34));
  USBSerial.print("S44:");
  USBSerial.println(digitalRead(S44));
  digitalWrite(SewagePower, HIGH);
  delay(5000);
}

void GreyTank(){
  USBSerial.print("G14:");
  USBSerial.println(digitalRead(G14));
  USBSerial.print("G12:");
  USBSerial.println(digitalRead(G12));
  USBSerial.print("G34:");
  USBSerial.println(digitalRead(G34));
  USBSerial.print("G44:");
  USBSerial.println(digitalRead(G44));
  digitalWrite(GreyWaterPower, HIGH);
  delay(5000);

}


void ReadAllInputs() {
  for (int i = 1; i <= SpareInputSize; i++) {
    int CurrentInputRead = ReadInput(SpareInputs[i]);
    USBSerial.print("Input " + String(i) + ":");
    USBSerial.println(digitalRead(CurrentInputRead));
    delay(1000);
  }
}

int ReadInput(int Number) {
  int Value = digitalRead(SpareInputs[Number]);
  return Value;
}
