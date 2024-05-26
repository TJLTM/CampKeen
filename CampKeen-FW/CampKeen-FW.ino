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
char* AcceptedCommands[] = {"UNITS?", "DEVICE?", "WATERSOURCE?", "WATERLEVEL?", "LPG?", "SEWAGE?", "GREY?",
                            "ENERGY?", "BATTERY?", "RTCBATTERY?", "GENERATOR?", "TEMPS?", "UNITTEMP?", "WATERPUMPSENSE?", "WARNING?",
                            "WATER?", "STREAMING?", "ACENMON?", "ALLDATA?", "UPDATEALL", "RESETWARNINGS", "RESETALLALARMS",
                            "TIME?", "ACVOLTAGEGAIN?", "ACFREQ?", "ACPGAGAIN?", "ACLEGS?", "ACCT1GAIN?", "ACCT2GAIN?", "REBOOT",
                            "RESET", "BATHROOMWATERDURATION?", "STREAMINGONBOOT?", "ACENMONONBOOT?", "WATERPUMPSENSEONBOOT?", "STATUS?", "PORT?",
                            "ALARM?", "WATERSOURCEOVERRIDE?", "WATERSOURCEOVERRIDEONBOOT?", "TRAVEL?", "KITCHENWATERDURATION?", "TANKALARMOVERRIDE?",
                            "WARNINGINDICATOR?"
                           };
char* ParameterCommands[] = {"SETUNITS", "SETWATERPUMPSENSE", "WATER", "SETSTREAMINGDATA", "SETTIME", "SETACENMON", "SETACFREQ",
                             "SETACPGAGAIN", "SETACVOLTAGEGAIN", "SETACLEGS", "SETACCT1GAIN", "SETACCT2GAIN", "SETBATHROOMWATERDURATION",
                             "SETSTREAMINGONBOOT", "SETACENMONONBOOT", "SETWATERPUMPSENSEONBOOT", "SETWATERSOURCEOVERRIDE",
                             "SETWATERSOURCE", "SETWATERSOURCEOVERRIDEONBOOT", "SETTRAVEL", "SETKITCHENWATERDURATION", "SETTANKALARMOVERRIDE",
                             "SETWARNINGINDICATOR"
                            };
String inputString, inputStringRS232 = "";         // a String to hold incoming data from ports
bool stringComplete, stringCompleteRS232 = false;     // whether the string is complete for each respective port
LiquidCrystal_I2C lcd(0x27, 20, 4);
int DisplayCounter = 0;
//#define LCDEnable 41
#define LCDEnable 33
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
const String DeviceName = "CampKeen";
const String FWVersion = "1.5.4";
const float ConversionFactor = 5.0 / 1023;
bool WarningActive, TankAlarmOverRide, AlarmActive = false;
bool WarningIndicator = true;
int TotalWarnings = 8;
int ArrayOfWarnings[] = {};
int BathroomWaterDurationInSeconds, KitchenWaterDurationInSeconds, WhoTurnedOnTheWater;
long BathroomWaterTimer, ShitterTankTimer, GreyTankTimer, WATERLPGtimer, FiveMinTimer, DisplayTimer, NTCTimer,
     EnergyTimer, OutputTimer, HoldingTankTimer, WarningBlinkTimer, KitchenWaterTimer, HoldingSewageTankPost, HoldingGreyTankPost;
bool WaterSourseSelection, WaterOn, EnableACEnergyMonitoring,
     UseWaterPumpSense, StreamingDataUSB, StreamingDataRS232, LCDSetup, WaterSourceOverRide, Travel, LastSourceForCheck = false;
bool ButtonsReleased = true;
char Units;
String TempUnits, PressureUnits;
//-----------------------------------------------------------
/*
   All the Stored Values and Times to have states
   that can be recalled if streaming is turned off
*/
//Water and LPG
String LastTimeWaterSource, LastTimeWaterLevel = "";
String LastWaterLevel = "Empty";
int LastLPGLevel = 0;
//Holding tanks
String LastSewageLevel, LastGreyWater = "Empty";
String LastTimeSewageLevel, LastTimeGreyWater = "";
//Battery Monitoring
float LastDCVoltage, LastRTCVoltage = 0.0;
String LastTimeDCVoltage, LastTimeRTCVoltage = "";
//NTC Temps
String LastTimeNTCTemp = "";
float LastFrontACTemp, LastBackACTemp, LastOutsideTemp, LastBackCabinTemp, LastHallwayTemp, LastFreezerTemp, LastFridgeTemp,
      LastBathroomTemp, LastFrontCabinTemp = 0.0;
//Generator
String LastTimeGenSensors = "";
float LastGenEnclosureTemp, LastGenHeadRightTemp, LastGenHeadLeftTemp, LastGenFuel = 0.0;
//AC&Energy Monitoring
String LastTimeACVoltage = "";
float LastACVoltage, LastACCurrent, LastPowerFactor, LastFreq, LastACWatts, LastACReactive, LastACApparent, LastACFundimental,
      LastACHarmonic, LastATM90E32Temp, LastACRealPower = 0.0;
//RTC TEMP
float LastRTCTemp = 0.0;
String LastTimeRTCTemp = "";
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
#define EnergyMonTransformerEnable 15
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
// Spare Inputs Outputs and Analog
#define SpareInput1 35
//#define SpareInput2 33
#define SpareInput3 31
#define SpareInput4 29
#define SpareInput5 27
#define SpareInput6 25

#define SpareOutput1 12
#define SpareOutput2 11
#define SpareOutput3 10
#define SpareOutput4 9
#define SpareOutput5 14

#define AUX1 13
#define AUX2 15
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
  //Load from EEProm
  EnableACEnergyMonitoring = GetFromEEPROMACENMONOnBoot();
  LineFreq = GetFromEEPROMACFREQ();
  PGAGain = GetFromEEPROMACPGAGain();
  VoltageGain = GetFromEEPROMACVOLTAGEGAIN();
  CurrentGainCT1 = GetFromEEPROMACCurrentGainCT1();
  CurrentGainCT2 = GetFromEEPROMACCurrentGainCT2();
  NumberOfACLegs = GetFromEEPROMACLegs();

  //Load Streaming Setting from EEProm
  StreamingDataUSB = GetFromEEPROMStreamOnBootUSB();
  StreamingDataRS232 = GetFromEEPROMStreamOnBootRS232();

  //load system settings from EEProm
  BathroomWaterDurationInSeconds = GetFromEEPROMBathroomWaterDuration();
  KitchenWaterDurationInSeconds = GetFromEEPROMKitchenWaterDuration();
  UseWaterPumpSense = GetFromEEPROMWaterPumpSenseOnBoot();
  WaterSourceOverRide = GetFromEEPROMWaterSourceOverRideOnBoot();

  pinMode(EnergyMonTransformerEnable, OUTPUT);
  pinMode(EnergyMonitorCS, OUTPUT);
  EnablingPowerToTransformerForEnergyMonitoring();

  //Set Warning Array up with -1 for being cleared.
  ResetWarnings();

  //Set up displays and output on the Serial Port
  BroadCast("Starting " + DeviceName);
  BroadCast("FW: " + FWVersion);
  Units = GetFromEEPROMUnits();
  SetUnitsForOutput();
  BroadCast("Units: " + String(Units));
  digitalWrite(LCDPowerOut, HIGH);
  delay(250);
  SetupLCD();
  lcd.setCursor(0, 0);
  lcd.print("Starting " + DeviceName);
  //Run through the sensors and get values for everything
  ForceCompleteUpdateOfAllStates();
  BroadCast("System Initialized and values populated:" + GetCurrentTime());
  lcd.setCursor(0, 1);
  lcd.print("System Initialized");
  lcd.setCursor(0, 2);
  lcd.print(GetCurrentTime());
  lcd.setCursor(0, 3);
  lcd.print(FWVersion + " Units: " + Units);
  delay(2000);
  digitalWrite(LCDPowerOut, digitalRead(LCDEnable));
}

void loop() {
  MainApplication();
  /*
    Handle the Incoming commands from the Serial Ports
    after all the other operations have been done
  */
  if (stringComplete) {
    inputString = PainlessInstructionSet(inputString, 0);
    stringComplete = false;
  }

  serialRS232();
  if (stringCompleteRS232) {
    stringCompleteRS232 = PainlessInstructionSet(inputStringRS232, 1);
    stringCompleteRS232 = false;
  }
}


void MainApplication() {
  /*
     All the basic functions that need to be handled
     everytime the loop comes back around
  */
  //LCDControl();
  WaterControl();
  if (Travel == false) {
    HoldingTankMonitoring();
  }
  /*
     Handle reseting the warnings and alarms
     If reset High warnings/alarms are
     flushed and silenced until brought low
  */
  if (digitalRead(AlarmReset) == HIGH) {
    ResetAllAlarmsAndWarnings();
  }
  else {
    Warning();
  }

  /*
     Read Sensors at 3 Second intervals
     Sensors to be read:
     NTC Temp sensors
     Generator Fuel Pressure & Temp sensors
  */
  if (abs(millis() - NTCTimer) > 3000) {
    ReadOtherTempSensors();
    GeneratorSensors();
    NTCTimer = millis();
    if (StreamingDataUSB == true) {
      GetNTCTemps(0);
      GetGenStatus(0);
    }
    if (StreamingDataRS232 == true) {
      GetNTCTemps(1);
      GetGenStatus(1);
    }
  }

  /*
     Read Energy Montioring at 10 Second Intervals if
     enabled and mmodule is installed
  */
  if ((abs(millis() - EnergyTimer) > 10000) && EnableACEnergyMonitoring == true) {
    EnergyMetering();
    EnergyTimer = millis();
    if (StreamingDataUSB == true) {
      GetEnergyStatus(0);
    }
    if (StreamingDataRS232 == true) {
      GetEnergyStatus(1);
    }
  }

  /*
     Read Sensors at 30 Min Intervals
     Sensors to be read:
     Water Tank Level
     LPG Tank Level
  */
  if (Travel == false) {
    if (abs(millis() - WATERLPGtimer) > 1800000) {
      ReadWaterAndLPG();
      WATERLPGtimer = millis();
      if (StreamingDataUSB == true) {
        GetWaterLevel(0);
        GetLPGLevel(0);
      }
      if (StreamingDataRS232 == true) {
        GetWaterLevel(1);
        GetLPGLevel(1);
      }
    }
  }

  /*
      Read Sensors at 5 Min Intervals
      Sensors to be read:
      Camper Battery Voltage
      RTC Battery Voltage
  */
  if (abs(millis() - FiveMinTimer) > 300000) {
    ReadBatteryVoltages();
    FiveMinTimer = millis();
    if (StreamingDataUSB == true) {
      GetDCBatteryVoltage(0);
      GetRTCBatteryVotlage(0);
    }
    if (StreamingDataRS232 == true) {
      GetDCBatteryVoltage(1);
      GetRTCBatteryVotlage(1);
    }
  }
}

//------------------------------------------------------------------
//LCD
//------------------------------------------------------------------
void LCDControl() {
  if (digitalRead(LCDEnable) == HIGH) {
    digitalWrite(LCDPowerOut, HIGH);
    if (LCDSetup == false) {
      delay(250);
      SetupLCD();
      delay(250);
    }
    LCDOutput();
  }
  else {
    lcd.clear();
    LCDSetup = false;
    digitalWrite(LCDPowerOut, LOW);
  }
}

void SetupLCD() {
  lcd.init();
  lcd.backlight();
  LCDSetup = true;
  DisplayCounter = 0;
}

void LCDOutput() {
  if (abs(millis() - DisplayTimer) > 7500)
  {
    lcd.clear();
    LCDDisplay();
    DisplayTimer = millis();

    if (DisplayCounter < 5) {
      DisplayCounter += 1;
    }
    else {
      DisplayCounter = 0;
    }
  }
}

void LCDDisplay() {
  //setCursor(position,line)
  switch (DisplayCounter) {
    case 0:
      //Tank Levels
      lcd.setCursor(0, 0);
      lcd.print("Water");
      lcd.setCursor(14, 0);
      lcd.print(LastWaterLevel);

      lcd.setCursor(0, 1);
      lcd.print("LPG");
      lcd.setCursor(14, 1);
      lcd.print(LastLPGLevel);

      lcd.setCursor(0, 2);
      lcd.print("Grey Water");
      lcd.setCursor(14, 2);
      if (LastGreyWater == "Empty" || LastGreyWater == "1/4" || LastGreyWater == "1/2" || LastGreyWater == "3/4" || LastGreyWater == "Full") {
        lcd.setCursor(14, 3);
      }
      else {
        lcd.setCursor(10, 3);
      }
      lcd.print(LastGreyWater);
      lcd.setCursor(0, 3);
      lcd.print("Sewage");
      if (LastSewageLevel == "Empty" || LastSewageLevel == "1/4" || LastSewageLevel == "1/2" || LastSewageLevel == "3/4" || LastSewageLevel == "Full") {
        lcd.setCursor(14, 3);
      }
      else {
        lcd.setCursor(10, 3);
      }
      lcd.print(LastSewageLevel);
      break;
    case 1:
      //Electrical
      lcd.setCursor(0, 0);
      lcd.print("Camper Battery");
      lcd.setCursor(15, 0);
      lcd.print(LastDCVoltage);

      lcd.setCursor(0, 1);
      lcd.print("RTC Battery");
      lcd.setCursor(15, 1);
      lcd.print(LastRTCVoltage);
      break;
    case 2:
      //Generator
      lcd.setCursor(0, 0);
      lcd.print("Gen Fuel");
      lcd.setCursor(12, 0);
      lcd.print(LastGenFuel);

      lcd.setCursor(0, 1);
      lcd.print("Enclosure");
      lcd.setCursor(12, 1);
      lcd.print(LastGenEnclosureTemp, 1);

      lcd.setCursor(0, 2);
      lcd.print("Head Right");
      lcd.setCursor(12, 2);
      lcd.print(LastGenHeadRightTemp, 1);

      lcd.setCursor(0, 3);
      lcd.print("Head Left");
      lcd.setCursor(12, 3);
      lcd.print(LastGenHeadLeftTemp, 1);
      break;
    case 3:
      //Temps
      lcd.setCursor(0, 0);
      lcd.print("Hallway");
      lcd.setCursor(15, 0);
      lcd.print(LastHallwayTemp, 1);

      lcd.setCursor(0, 1);
      lcd.print("Bathroom");
      lcd.setCursor(15, 1);
      lcd.print(LastBathroomTemp, 1);

      lcd.setCursor(0, 2);
      lcd.print("Back Cabin");
      lcd.setCursor(15, 2);
      lcd.print(LastBackCabinTemp, 1);

      lcd.setCursor(0, 3);
      lcd.print("Outside");
      lcd.setCursor(15, 3);
      lcd.print(LastOutsideTemp, 1);
      break;
    case 4:
      //Temps
      lcd.setCursor(0, 0);
      lcd.print("Back AC");
      lcd.setCursor(15, 0);
      lcd.print(LastBackACTemp, 1);

      lcd.setCursor(0, 1);
      lcd.print("Front AC");
      lcd.setCursor(15, 1);
      lcd.print(LastFrontACTemp, 1);

      lcd.setCursor(0, 2);
      lcd.print("Fridge");
      lcd.setCursor(15, 2);
      lcd.print(LastFridgeTemp, 1);

      lcd.setCursor(0, 3);
      lcd.print("Freezer");
      lcd.setCursor(15, 3);
      lcd.print(LastFreezerTemp, 1);
      break;
    case 5:
      lcd.setCursor(0, 0);
      lcd.print("Watts");
      lcd.setCursor(12, 0);
      lcd.print(LastACWatts, 1);
      //Amps
      lcd.setCursor(0, 1);
      lcd.print("AC Amps");
      lcd.setCursor(12, 1);
      lcd.print(LastACCurrent);
      //Voltage
      lcd.setCursor(0, 2);
      lcd.print("AC Voltage");
      lcd.setCursor(12, 2);
      lcd.print(LastACVoltage);

      lcd.setCursor(0, 3);
      lcd.print("PF");
      lcd.setCursor(12, 3);
      lcd.print(LastPowerFactor, 0);
      break;
  }
}
//------------------------------------------------------------------
//Water Control
//------------------------------------------------------------------
void WaterControl() {
  if (WaterSourceOverRide == false) {
    //Read Current Water Source Selection
    LastSourceForCheck = WaterSourseSelection;
    if (digitalRead(WaterSourceSelectionInput) == HIGH) {
      WaterSourseSelection = true;//city water
    }
    else {
      WaterSourseSelection = false; //Tank
    }
  }

  if (WaterOn == true && (LastSourceForCheck != WaterSourseSelection)) {
    TurnOffWater();
    delay(1000);
    TurnOnWater(true);
    if (StreamingDataUSB == true) {
      GetWaterSource(0);
    }
    if (StreamingDataRS232 == true) {
      GetWaterSource(1);
    }
  }

  //if the water is on and the timer says it's more than the set WaterDuration then turn it off.
  bool SkipTurningOnIfIjustTurnedItOff = false;
  long Delta, WaterDurationInSeconds = 0;
  if (WhoTurnedOnTheWater == 1 || WhoTurnedOnTheWater == 0) {
    Delta = abs(millis() - BathroomWaterTimer) / 1000;
    WaterDurationInSeconds = BathroomWaterDurationInSeconds;
  }
  if (WhoTurnedOnTheWater == 2) {
    Delta = abs(millis() - KitchenWaterTimer) / 1000;
    WaterDurationInSeconds = KitchenWaterDurationInSeconds;
  }

  if (WaterOn == true && (Delta > WaterDurationInSeconds)) {
    TurnOffWater();
    SkipTurningOnIfIjustTurnedItOff = true;
  }

  if (SkipTurningOnIfIjustTurnedItOff == false) {
    //  //Check to see if any of the buttons are pressed
    if (digitalRead(KitchWaterButton) == HIGH || digitalRead(BathroomWaterButton) == HIGH) {
      if (ButtonsReleased == true) { //only do something if buttons have been released between reads
        if (digitalRead(KitchWaterButton) == HIGH && WaterOn == false) {
          WhoTurnedOnTheWater = 2;
        }
        if (digitalRead(BathroomWaterButton) == HIGH && WaterOn == false) {
          WhoTurnedOnTheWater = 1;
        }
        ButtonsReleased = false; //set flag to skip toggle
        if (WaterOn == true) { //toggle the water on and off.
          TurnOffWater();
        }
        else {
          TurnOnWater(false);
        }
      }
    }

    if (digitalRead(BathroomWaterButton) == LOW && digitalRead(KitchWaterButton) == LOW) {
      ButtonsReleased = true;
    }
  }
  //Check the States of pump and or logical state and set the LEDs accordingly
  WaterLEDState();
}

void TurnOnWater(bool SwitchingSources) {
  if (WaterOn == false && HoldingTankAlarm == false) {
    WaterOn = true;
    if (WhoTurnedOnTheWater == 1 && SwitchingSources == false) {
      BathroomWaterTimer = millis();
    }
    if (WhoTurnedOnTheWater == 2 && SwitchingSources == false) {
      KitchenWaterTimer = millis();
    }
    if (WaterSourseSelection == true) {
      digitalWrite(CityWaterValve, HIGH);
    }
    else {
      digitalWrite(WaterPumpOut, HIGH);
    }
    GetWaterState(0);
    GetWaterState(1);
  }
}

void TurnOffWater() {
  digitalWrite(CityWaterValve, LOW);
  digitalWrite(WaterPumpOut, LOW);
  digitalWrite(KitchenWaterButtonLED, LOW);
  digitalWrite(BathroomWaterButtonLED, LOW);
  WaterOn = false;
  GetWaterState(0);
  GetWaterState(1);
}

void WaterLEDState() {
  if (UseWaterPumpSense == true) {
    if ((digitalRead(WaterPumpSense) == LOW && WaterSourseSelection == false) || (WaterSourseSelection == true && digitalRead(CityWaterValve) == LOW)) {
      digitalWrite(KitchenWaterButtonLED, LOW);
      digitalWrite(BathroomWaterButtonLED, LOW);
      WaterOn = false;
    }

    if (digitalRead(CityWaterValve) == HIGH || digitalRead(WaterPumpSense) == HIGH) {
      digitalWrite(KitchenWaterButtonLED, HIGH);
      digitalWrite(BathroomWaterButtonLED, HIGH);
      WaterOn = true;
    }
  }
  else {
    if (WaterOn == true) {
      digitalWrite(KitchenWaterButtonLED, HIGH);
      digitalWrite(BathroomWaterButtonLED, HIGH);
    }
    else {
      digitalWrite(KitchenWaterButtonLED, LOW);
      digitalWrite(BathroomWaterButtonLED, LOW);
    }
  }
}
//------------------------------------------------------------------
//Generator and Energy
//------------------------------------------------------------------
void SetupEnergyMonitor() {
  eic.begin(EnergyMonitorCS, LineFreq, PGAGain, VoltageGain, CurrentGainCT1, 0, CurrentGainCT2);
}

void EnablingPowerToTransformerForEnergyMonitoring() {
  if (EnableACEnergyMonitoring == true) {
    digitalWrite(EnergyMonTransformerEnable, HIGH);
    delay(2500);
    SetupEnergyMonitor();
  }
  else {
    digitalWrite(EnergyMonTransformerEnable, LOW);
  }
}

void EnergyMetering() {
  float voltageA, voltageC, CT1, CT2;
  LastTimeACVoltage = GetCurrentTime();
  unsigned short sys0 = eic.GetSysStatus0(); //EMMState0
  unsigned short sys1 = eic.GetSysStatus1(); //EMMState1
  //unsigned short en0 = eic.GetMeterStatus0();//EMMIntState0
  //unsigned short en1 = eic.GetMeterStatus1();//EMMIntState1

  //if true the MCU is not getting data from the energy meter
  //set all AC Values to 0
  if (sys0 == 65535 || sys0 == 0) {
    BroadCast("R," + GetCurrentTime() + ",Error,Not receiving data from energy meter");
    LastACVoltage = 0;
    LastACCurrent = 0;
    LastPowerFactor = 0;
    LastFreq = 0;
    LastACWatts = 0;
    LastACReactive = 0;
    LastACApparent = 0;
    LastACFundimental = 0;
    LastACHarmonic = 0;
    LastATM90E32Temp = 0;
    LastACRealPower =  0;
    //Attempt to reconnect and setup Energy Monitor
    SetupEnergyMonitor();
  }
  else {
    //get voltage
    voltageA = eic.GetLineVoltageA();
    voltageC = eic.GetLineVoltageC();
    //get current
    CT1 = eic.GetLineCurrentA();
    CT2 = eic.GetLineCurrentC();

    if (LineFreq = 4485) {
      LastACVoltage = voltageA + voltageC;     //is split single phase, so only 120v per leg
    }
    else {
      LastACVoltage = voltageA;     //voltage should be 220-240 at the AC transformer
    }

    if (NumberOfACLegs == 2) {
      LastACCurrent =  CT1 + CT2;
      LastACWatts = (voltageA * CT1) + (voltageC * CT2);
    }
    else {
      LastACCurrent =  CT1;
      LastACWatts = (voltageA * CT1);
      LastPowerFactor = eic.GetTotalPowerFactor();
    }

    // Standard Values to be read and streamed out.
    LastFreq = eic.GetFrequency();
    LastACReactive = eic.GetTotalReactivePower();
    LastACApparent = eic.GetTotalApparentPower();
    LastACFundimental = eic.GetTotalActiveFundPower();
    LastACHarmonic = eic.GetTotalActiveHarPower();
    LastACRealPower =  eic.GetTotalActivePower();

    if (Units == 'I') {
      LastATM90E32Temp = ConvertCtoF(eic.GetTemperature());
    }
    else {
      LastATM90E32Temp = eic.GetTemperature();
    }
  }
}

void GeneratorSensors() {
  float FuelPressureDN = ReadAnalog(25, GenFuelPressure);

  //Sensors i'm using drop out at .5 volts and anything below that will show neg pressure. DN 102 translates to 0.5V
  if (FuelPressureDN > 102) {
    if (Units == 'I') {
      LastGenFuel = (7.5 * ConversionFactor * FuelPressureDN) - 3.75;
    }
    else {
      LastGenFuel = ConvertPSItoKPa((7.5 * ConversionFactor * FuelPressureDN) - 3.75);
    }
  }
  else {
    LastGenFuel = 0.0;
  }

  uint16_t rtd0 = GenHeadR.readRTD();
  //float ratio0 = rtd0;
  //ratio0 /= 32768;
  if (Units == 'I') {
    LastGenHeadRightTemp = ConvertCtoF(GenHeadR.temperature(RNOMINAL, RREF));
  }
  else {
    LastGenHeadRightTemp = GenHeadR.temperature(RNOMINAL, RREF);
  }

  uint16_t rtd1 = GenHeadL.readRTD();
  //float ratio1 = rtd1;
  //ratio1 /= 32768;
  if (Units == 'I') {
    LastGenHeadLeftTemp = ConvertCtoF(GenHeadL.temperature(RNOMINAL, RREF));
  }
  else {
    LastGenHeadLeftTemp = GenHeadL.temperature(RNOMINAL, RREF);
  }

  uint16_t rtd2 = GenEnclosure.readRTD();
  //float ratio2 = rtd2;
  //ratio2 /= 32768;
  if (Units == 'I') {
    LastGenEnclosureTemp = ConvertCtoF(GenEnclosure.temperature(RNOMINAL, RREF));
  }
  else {
    LastGenEnclosureTemp = GenEnclosure.temperature(RNOMINAL, RREF);
  }

  LastTimeGenSensors = GetCurrentTime();
}
//------------------------------------------------------------------
//Tanks
//------------------------------------------------------------------
void HoldingTankMonitoring() {
  //Read Grey and Sewage Tanks at 1hr intervals unless it is at 3/4 full then switch to Constant
  //Read Grey and Sewage Tanks Continuously when WaterOn == True
  if ((millis() - ShitterTankTimer) > 900000 || ShittersGettinFull == true || WaterOn == true) {
    ReadSewageTank();
    if (millis() - HoldingSewageTankPost > 30000) {
      GetSewageLevel(0);
      GetSewageLevel(1);
      HoldingSewageTankPost = millis();
    }
    ShitterTankTimer = millis();
  }

  if ((millis() - GreyTankTimer) > 900000 || GreyGettinFull == true || WaterOn == true) {
    ReadGreyTank();
    if (millis() - HoldingGreyTankPost > 30000) {
      GetGreyLevel(0);
      GetGreyLevel(1);
      HoldingGreyTankPost = millis();
    }
    GreyTankTimer = millis();
  }

  if (ShittersGettinFull == true || GreyGettinFull == true) {
    // Also put in a check for FULL State on either and turn off pump or city water
    if (LastSewageLevel == "Full" || LastGreyWater == "Full") {
      HoldingTankAlarm = true;
      if (TankAlarmOverRide == false) {
        if (WaterOn == true) {
          TurnOffWater();
        }
      }
      ALARM();
    }
    else {
      // if the tanks drop below full make sure the alarm is off
      HoldingTankAlarm = false;
      ResetAlarm();
    }
  }
}

void ReadSewageTank() {
  // Turn On votlage to tank
  if (digitalRead(SewagePower) == LOW) {
    digitalWrite(SewagePower, HIGH);
    delay(2000); // Have to put this in here for the Relay to completely close
  }
  byte TankStatus;
  bitWrite(TankStatus, 0, digitalRead(S14)); //quater
  bitWrite(TankStatus, 1, digitalRead(S12)); //Half
  bitWrite(TankStatus, 2, digitalRead(S34)); //Three Quater
  bitWrite(TankStatus, 3, digitalRead(S44)); //Full

  switch (TankStatus) {
    case 0:
      LastSewageLevel = "Empty";
      ShittersGettinFull = false;
      RemoveWarningToList(4);
      RemoveWarningToList(8);
      break;
    case 1:
      LastSewageLevel = "1/4";
      ShittersGettinFull = false;
      RemoveWarningToList(4);
      RemoveWarningToList(8);
      break;
    case 3:
      LastSewageLevel = "1/2";
      ShittersGettinFull = false;
      RemoveWarningToList(4);
      RemoveWarningToList(8);
      break;
    case 7:
      LastSewageLevel = "3/4";
      ShittersGettinFull = true;
      //Set Warning State
      AddWarningToList(8);
      break;
    case 15:
      LastSewageLevel = "Full";
      ShittersGettinFull = true;
      //Set Warning State
      AddWarningToList(8);
      break;
    default:
      LastSewageLevel = "ERROR:" + String(TankStatus, BIN);
      AddWarningToList(4);
      break;
  }

  LastTimeSewageLevel = GetCurrentTime();

  if (ShittersGettinFull == false && WaterOn == false) {
    // Turn Off Voltage to tank
    digitalWrite(SewagePower, LOW);
  }
}

void ReadGreyTank() {
  // Turn On votlage to tank
  if (digitalRead(GreyWaterPower) == LOW) {
    digitalWrite(GreyWaterPower, HIGH);
    delay(2000); // Have to put this in here for the Relay to completely close
  }
  byte TankStatus;
  bitWrite(TankStatus, 0, digitalRead(G14)); //quater
  bitWrite(TankStatus, 1, digitalRead(G12)); //Half
  bitWrite(TankStatus, 2, digitalRead(G34)); //Three Quater
  bitWrite(TankStatus, 3, digitalRead(G44)); //Full

  switch (TankStatus) {
    case 0:
      LastGreyWater = "Empty";
      GreyGettinFull = false;
      RemoveWarningToList(2);
      RemoveWarningToList(3);
      break;
    case 1:
      LastGreyWater = "1/4";
      GreyGettinFull = false;
      RemoveWarningToList(2);
      RemoveWarningToList(3);
      break;
    case 3:
      LastGreyWater = "1/2";
      GreyGettinFull = false;
      RemoveWarningToList(2);
      RemoveWarningToList(3);
      break;
    case 7:
      LastGreyWater = "3/4";
      GreyGettinFull = true;
      //Set Warning State
      AddWarningToList(2);
      break;
    case 15:
      LastGreyWater = "Full";
      GreyGettinFull = true;
      //Set Warning State
      AddWarningToList(2);
      break;
    default:
      LastGreyWater = "ERROR:" + String(TankStatus, BIN);
      AddWarningToList(3);
      break;
  }
  LastTimeGreyWater = GetCurrentTime();

  if (GreyGettinFull == false && WaterOn == false) {
    // Turn Off Voltage to tank
    digitalWrite(GreyWaterPower, LOW);
  }
}

void ReadWaterAndLPG() {
  /*
     Using a 47 1% for R2 in both circuits
     LPG tank is as far as i can tell is a 0-122 ohm resistence and knowing that the 80% full at ~90 ohms
     that is mapped to 100%. confusing yes. but it's how it works. anything over the 80% mark will read as
     a greater percentage than 100
     The Water tank level was determined from setting the tank sensor at 1/4 1/2 3/4 and full. Anything above
     the full mark shows up as "EXTRA FULL"
  */

  // Turn On votlage to tanks
  digitalWrite(TankPowerRelay, HIGH);
  delay(1000);
  int LPGResistence = 47 * (1 / ((5 / (ConversionFactor * ReadAnalog(50, LPGSensor))) - 1));
  if (LPGResistence >= 122) {
    LastLPGLevel = "ERROR Check Tank Sensor";
  }
  else {
    LastLPGLevel = map(LPGResistence, 0, 90, 0, 100);
  }

  LastWaterLevel = "EXTRA FULL";
  LastTimeWaterLevel = GetCurrentTime();
  float R1 = 47 * ((5.0 / (ConversionFactor * ReadAnalog(50, WaterTankSensor))) - 1);
  if (R1 > 40)
  {
    LastWaterLevel = "Full";
    RemoveWarningToList(6);
    if (R1 > 60)
    {
      LastWaterLevel = "3/4";
      RemoveWarningToList(6);
      if (R1 > 80)
      {
        LastWaterLevel = "1/2";
        RemoveWarningToList(6);
        if (R1 > 100)
        {
          LastWaterLevel = "1/4";
          AddWarningToList(6);
          if (R1 > 150)
          {
            AddWarningToList(6);
            LastWaterLevel = "Empty";
          }
        }
      }
    }
  }
  // Turn Off Voltage to tanks
  digitalWrite(TankPowerRelay, LOW);

  if (LastWaterLevel == "1/4") {
    OutputWarningMessage(6); //Water
  }

  if (LastLPGLevel <= 25 || LastLPGLevel == "ERROR") {
    OutputWarningMessage(7); //LPG
  }
  else {
    RemoveWarningToList(7);
  }
}
//------------------------------------------------------------------
//Other sensors
//------------------------------------------------------------------
void ReadBatteryVoltages() {
  float DCVoltageDN = ReadAnalog(50, Camper12VoltSensor);
  float RTCVoltageDN = ReadAnalog(50, RTCBattery);

  LastDCVoltage = 3.8 * ConversionFactor * DCVoltageDN - 1.2;
  LastTimeDCVoltage = GetCurrentTime();

  LastRTCVoltage = ConversionFactor * RTCVoltageDN;
  LastTimeRTCVoltage = LastTimeDCVoltage;

  if (LastDCVoltage < 10.5) {
    AddWarningToList(5);
  }
  else{
    RemoveWarningToList(5);
  }

  if (LastRTCVoltage < 2.3) {
    AddWarningToList(1);
  }
  else{
    RemoveWarningToList(1);
  }

}

float NTCReadInC(int R2, float ResistenceRead) {
  /*
     Using the Resistence that is calced from an ADC read, a known calibrated resistence
     value, and https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation to get the
     tempetature from these values.

     int R2 == Calibrated static resistor used
     float ResistenceRead == Log() of the resistence value read
  */
  float c1 = 1.009249522e-03;
  float c2 = 2.378405444e-04;
  float c3 = 2.019202697e-07;
  float C = (1.0 / (c1 + c2 * ResistenceRead + c3 * ResistenceRead * ResistenceRead * ResistenceRead)) - 273.15;
  return C;
}

void ReadOtherTempSensors() {
  int R2 = 10000;

  float VoutACF = ConversionFactor * ReadAnalog(10, FrontACTemp);
  float R1ACF = log(R2 * ((5.0 / VoutACF) - 1));
  if (Units == 'I') {
    LastFrontACTemp = ConvertCtoF(NTCReadInC(R2, R1ACF));
  }
  else {
    LastFrontACTemp = NTCReadInC(R2, R1ACF);
  }

  float VoutACB = ConversionFactor * ReadAnalog(10, BackACTemp);
  float R1ACB = log(R2 * ((5.0 / VoutACB) - 1));
  if (Units == 'I') {
    LastBackACTemp = ConvertCtoF(NTCReadInC(R2, R1ACB));
  }
  else {
    LastBackACTemp =  NTCReadInC(R2, R1ACB);
  }

  float VoutHallway = ConversionFactor * ReadAnalog(10, HallwayTemp);
  float R1Hallway = log(R2 * ((5.0 / VoutHallway) - 1));
  if (Units == 'I') {
    LastHallwayTemp = ConvertCtoF( NTCReadInC(R2, R1Hallway));
  }
  else {
    LastHallwayTemp =  NTCReadInC(R2, R1Hallway);
  }

  float VoutBathroom = ConversionFactor * ReadAnalog(10, BathroomTemp);
  float R1Bathroom = log(R2 * ((5.0 / VoutBathroom) - 1));
  if (Units == 'I') {
    LastBathroomTemp = ConvertCtoF(NTCReadInC(R2, R1Bathroom));
  }
  else {
    LastBathroomTemp =  NTCReadInC(R2, R1Bathroom);
  }

  float VoutFreezer = ConversionFactor * ReadAnalog(10, Freezer);
  float R1Freezer = log(R2 * ((5.0 / VoutFreezer) - 1));
  if (Units == 'I') {
    LastFreezerTemp = ConvertCtoF(NTCReadInC(R2, R1Freezer));
  }
  else {
    LastFreezerTemp =  NTCReadInC(R2, R1Freezer);
  }

  float VoutFridge = ConversionFactor * ReadAnalog(10, Refridgerator);
  float R1Fridge = log(R2 * ((5.0 / VoutFridge) - 1));
  if (Units == 'I') {
    LastFridgeTemp = ConvertCtoF(NTCReadInC(R2, R1Fridge));
  }
  else {
    LastFridgeTemp =  NTCReadInC(R2, R1Fridge);
  }

  float VoutOutside = ConversionFactor * ReadAnalog(10, Outside);
  float R1Outside = log(R2 * ((5.0 / VoutOutside) - 1));
  if (Units == 'I') {
    LastOutsideTemp = ConvertCtoF(NTCReadInC(R2, R1Outside));
  }
  else {
    LastOutsideTemp =  NTCReadInC(R2, R1Outside);
  }

  float VoutBackCabin = ConversionFactor * ReadAnalog(10, BackCabin);
  float R1BackCabin = log(R2 * ((5.0 / VoutBackCabin) - 1));
  if (Units == 'I') {
    LastBackCabinTemp = ConvertCtoF(NTCReadInC(R2, R1BackCabin));
  }
  else {
    LastBackCabinTemp =  NTCReadInC(R2, R1BackCabin);
  }

  float VoutFrontCabin = ConversionFactor * ReadAnalog(10, FrontCabin);
  float R1FrontCabin = log(R2 * ((5.0 / VoutFrontCabin) - 1));
  if (Units == 'I') {
    LastFrontCabinTemp = ConvertCtoF(NTCReadInC(R2, R1FrontCabin));
  }
  else {
    LastFrontCabinTemp =  NTCReadInC(R2, R1FrontCabin);
  }

  LastTimeNTCTemp = GetCurrentTime();
}
//------------------------------------------------------------------
//EEPROM functions
//------------------------------------------------------------------
bool GetFromEEPROMWaterSourceOverRideOnBoot() {
  int Value = EEPROM.read(17);
  if (0 > Value || Value >= 2) {
    Value = 0;
    EEPROM.update(17, Value);
  }
  return Value;
}

int GetFromEEPROMWaterPumpSenseOnBoot() {
  int Value = EEPROM.read(16);
  if (0 > Value || Value >= 2) {
    Value = 0;
    EEPROM.update(16, Value);
  }
  return Value;
}

int GetFromEEPROMACENMONOnBoot() {
  int Value = EEPROM.read(2);
  if (0 > Value || Value >= 2) {
    Value = 0;
    EEPROM.update(2, Value);
  }

  return Value;
}

int GetFromEEPROMStreamOnBootUSB() {
  int Value = EEPROM.read(4);
  if (0 > Value || Value >= 2) {
    Value = 0;
    EEPROM.update(4, Value);
  }
  return Value;
}

int GetFromEEPROMStreamOnBootRS232() {
  int Value = EEPROM.read(5);
  if (0 > Value || Value >= 2) {
    Value = 0;
    EEPROM.update(5, Value);
  }
  return Value;
}

int GetFromEEPROMBathroomWaterDuration() {
  int Value = EEPROM.read(20) << 8 | EEPROM.read(21);
  if (60 >= Value || Value >= 1800) {
    Value = 150;
    EEPROM.update(20, highByte(Value));
    EEPROM.update(21, lowByte(Value));
  }
  return Value;
}

int GetFromEEPROMKitchenWaterDuration() {
  int Value = EEPROM.read(18) << 8 | EEPROM.read(19);
  if (60 >= Value || Value >= 1800) {
    Value = 150;
    EEPROM.update(18, highByte(Value));
    EEPROM.update(19, lowByte(Value));
  }
  return Value;
}

char GetFromEEPROMUnits() {
  char Value = EEPROM.read(0);
  if (Value != 'I' && Value != 'M') {
    Value = 'I';
    EEPROM.update(0, Value);
  }
  return Value;
}

unsigned short GetFromEEPROMACVOLTAGEGAIN() {
  unsigned short Value = EEPROM.read(8) << 8 | EEPROM.read(9);
  if (Value == 65535 || Value == 0) {
    Value = 1975;
    EEPROM.update(8, highByte(Value));
    EEPROM.update(9, lowByte(Value));
  }
  return Value;
}

unsigned short GetFromEEPROMACCurrentGainCT1() {
  unsigned short Value = EEPROM.read(10) << 8 | EEPROM.read(11);
  if (Value == 65535 || Value == 0) {
    Value = 34500;
    EEPROM.update(10, highByte(Value));
    EEPROM.update(11, lowByte(Value));
  }
  return Value;
}

unsigned short GetFromEEPROMACCurrentGainCT2() {
  unsigned short Value = EEPROM.read(12) << 8 | EEPROM.read(13);
  if (Value == 65535 || Value == 0) {
    Value = 34500;
    EEPROM.update(12, highByte(Value));
    EEPROM.update(13, lowByte(Value));
  }
  return Value;
}

unsigned short GetFromEEPROMACPGAGain() {
  unsigned short Value = EEPROM.read(14) << 8 | EEPROM.read(15);
  return Value;
}

int GetFromEEPROMACLegs() {
  int Value = EEPROM.read(7);
  if (0 >= Value || Value >= 3) {
    Value = 1;
    EEPROM.update(7, Value);
  }
  return Value;
}

unsigned short GetFromEEPROMACFREQ() {
  unsigned short Value = 0;
  int ReadFromMem = EEPROM.read(6);
  if (ReadFromMem == 60) {
    Value = 4485; //for 60 Hz (North America)
  }
  if (ReadFromMem == 50) {
    Value = 389; //for 50 hz (rest of the world)
  }

  if (Value == 0) {
    /*
       If it isn't either one of those values
       set the default of 60 hz
    */
    EEPROM.update(6, 60);
    Value = 4485;
  }
  return Value;
}
//------------------------------------------------------------------
//Helper functions
//------------------------------------------------------------------
void(* resetFunc) (void) = 0;  // declare reset fuction at address 0

String StatesForOutput(int State) {
  String Value = "Off";
  if (State == 1) {
    Value = "On";
  }
  return Value;
}

void RebootDisBitch() {
  BroadCast("%R," + GetCurrentTime() + ",Rebooting");
  delay(2000);
  resetFunc();
}

void MIBFLASH() {
  BroadCast("%R," + GetCurrentTime() + ",Reseting wait for reboot Message");
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  RebootDisBitch();
}

float ConvertCtoF(float C) {
  float F = (1.8 * C) + 32;
  return F;
}

float ConvertPSItoKPa(float PSI) {
  float KPA = 6.8947572932 * PSI;
  return KPA;
}

String GetCurrentTime() {
  DateTime now = rtc.now();

  char buf1[20];
  sprintf(buf1, "%02d:%02d:%02d-%02d/%02d/%02d",  now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());

  LastTimeRTCTemp = buf1;
  if (Units == 'I') {
    LastRTCTemp = ConvertCtoF(rtc.getTemperature());
  }
  else {
    LastRTCTemp = rtc.getTemperature();
  }
  return buf1;
}

String GetCurrentDate() {
  DateTime now = rtc.now();
  String CurrentDate = String(now.year()) + String(now.month()) + String(now.day());
  return CurrentDate;
}

void SetUnitsForOutput() {
  if (Units == "M") {
    TempUnits = "C";
    PressureUnits = "KPa";
  }
  else {
    TempUnits = "F";
    PressureUnits = "PSI";
  }
}

float ReadAnalog(int Samples, int PinNumber) {
  long Sum = 0;
  float Value = 0;
  for (int x = 0; x < Samples; x++) {
    Sum = Sum + analogRead(PinNumber);
  }
  Value = (Sum / Samples);
  return Value;
}

void ForceCompleteUpdateOfAllStates() {
  ReadGreyTank();
  ReadSewageTank();
  ReadOtherTempSensors();
  ReadBatteryVoltages();
  ReadWaterAndLPG();
  GeneratorSensors();
  if (EnableACEnergyMonitoring == true) {
    EnergyMetering();
  }
}
//------------------------------------------------------------------
//Serial
//------------------------------------------------------------------
/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (USBSerial.available()) {
    // get the new byte:
    char inChar = (char)USBSerial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a carriage return, set a flag so the main loop can
    // do something about it:
    if (inChar == '\r') {
      stringComplete = true;
    }
  }
}

void serialRS232() {
  while (RS232.available()) {
    // get the new byte:
    char inChar = (char)RS232.read();
    // add it to the inputString:
    inputStringRS232 += inChar;
    // if the incoming character is a carriage return, set a flag so the main loop can
    // do something about it:
    if (inChar == '\r') {
      stringCompleteRS232 = true;
    }
  }
}

void BroadCast(String Message) {
  SendItOut(Message, 0);
  SendItOut(Message, 1);
}

void CurrentSerialPort(int WhichPort) {
  if (WhichPort == 0) {
    USBSerial.println("%R,Current Port,USB");
  }
  else {
    RS232.println("%R,Current Port,RS232");
  }
}

void SendItOut(String Message, int WhichPort) {
  if (WhichPort == 0) {
    USBSerial.println(Message);
  }
  else {
    RS232.println(Message);
  }
}

//------------------------------------------------------------------
//Serial Output
//------------------------------------------------------------------
void OutputAllData(int WhichPort) {
  GetDeviceInfo(WhichPort);
  GetSystemTime(WhichPort);
  AllWarningMessages(WhichPort);
  GetStreamingState(WhichPort);
  GetWaterPumpSense(WhichPort);
  GetACEnmon(WhichPort);
  GetWaterState(WhichPort);
  GetUnits(WhichPort);
  GetWaterSource(WhichPort);
  GetWaterLevel(WhichPort);
  GetKitchenWaterDuration(WhichPort);
  GetBathroomWaterDuration(WhichPort);
  GetSewageLevel(WhichPort);
  GetGreyLevel(WhichPort);
  GetLPGLevel(WhichPort);
  GetDCBatteryVoltage(WhichPort);
  GetRTCBatteryVotlage(WhichPort);
  GetNTCTemps(WhichPort);
  GetHeadUnitTemp(WhichPort);
  GetGenStatus(WhichPort);
  GetEnergyStatus(WhichPort);
  GetACVOLTAGEGAIN(WhichPort);
  GetACFREQ(WhichPort);
  GetACPGAGAIN(WhichPort);
  GetACLEGS(WhichPort);
  GetACCT1GAIN(WhichPort);
  GetACCT2GAIN(WhichPort);
  GetStreamingOnBoot(WhichPort);
  GETACENMONOnBoot(WhichPort);
  GETWaterpumpsenseBoot(WhichPort);
  GetWaterSourceOverRideOnBoot(WhichPort);
  GetWaterSourceOverRide(WhichPort);
}

void OutputLiveData(int WhichPort) {
  GetWaterPumpSense(WhichPort);
  GetWaterState(WhichPort);
  GetWaterSource(WhichPort);
  GetWaterLevel(WhichPort);
  GetSewageLevel(WhichPort);
  GetGreyLevel(WhichPort);
  GetLPGLevel(WhichPort);
  GetDCBatteryVoltage(WhichPort);
  GetRTCBatteryVotlage(WhichPort);
  GetNTCTemps(WhichPort);
  GetHeadUnitTemp(WhichPort);
  GetGenStatus(WhichPort);
  GetEnergyStatus(WhichPort);
  GetStreamingOnBoot(WhichPort);
  GetStreamingState(WhichPort);
  GETACENMONOnBoot(WhichPort);
  GETWaterpumpsenseBoot(WhichPort);
}

void GetWaterSource(int WhichPort) {
  if (WaterSourseSelection == true) {
    SendItOut("%R,Water Source,City", WhichPort);
  }
  else {
    SendItOut("%R,Water Source,Tank", WhichPort);
  }
}

void GetWaterLevel(int WhichPort) {
  SendItOut("%R," + LastTimeWaterLevel + ",Water Tank Level," + LastWaterLevel, WhichPort);
}

void GetSewageLevel(int WhichPort) {
  SendItOut("%R," + LastTimeSewageLevel + ",Sewage," + LastSewageLevel, WhichPort);
}

void GetGreyLevel(int WhichPort) {
  SendItOut("%R," + LastTimeGreyWater + ",Grey," + LastGreyWater, WhichPort);
}

void GetLPGLevel(int WhichPort) {
  SendItOut("%R," + LastTimeWaterLevel   + ",LPG," + LastLPGLevel, WhichPort);
}

void GetDCBatteryVoltage(int WhichPort) {
  SendItOut("%R," + LastTimeDCVoltage + ",Camper VDC," + LastDCVoltage, WhichPort);
}

void GetRTCBatteryVotlage(int WhichPort) {
  SendItOut("%R," + LastTimeRTCVoltage + ",RTCBattery," + LastRTCVoltage, WhichPort);
}

void GetNTCTemps(int WhichPort) {
  SendItOut("%R," + LastTimeNTCTemp + ",NTC Tempetatures,Units," + TempUnits + ",Front AC Temp,"
            + LastFrontACTemp + ",Back AC Temp," + LastBackACTemp + ",Under Awning Temp," + LastOutsideTemp + ",Back Cabin Temp,"
            + LastBackCabinTemp + ",Hallway Temp," + LastHallwayTemp + ",Freezer," + LastFreezerTemp + ",Fridge,"
            + LastFridgeTemp + ",Bathroom Temp," + LastBathroomTemp + ",Front Cabin Temp," + LastFrontCabinTemp, WhichPort);
}

void GetHeadUnitTemp(int WhichPort) {
  SendItOut("%R," + LastTimeRTCTemp + ",Head Unit Temp,Units," + TempUnits + "," + LastRTCTemp, WhichPort);
}

void GetGenStatus(int WhichPort) {
  SendItOut("%R," + LastTimeGenSensors + ",Generator Fuel Pressure,Units," + PressureUnits + ","
            + LastGenFuel + ",Generator Temps, Enclosure,Units," + TempUnits + "," + LastGenEnclosureTemp +
            ",Right Head Temp," + LastGenHeadRightTemp + ",Left Head Temp," + LastGenHeadLeftTemp, WhichPort);
}

void GetEnergyStatus(int WhichPort) {
  if (LastTimeACVoltage == "" || EnableACEnergyMonitoring == false) {
    LastTimeACVoltage = GetCurrentTime();
  }
  if (EnableACEnergyMonitoring == true) {
    SendItOut("%R," + LastTimeACVoltage + ",Energy Monitor," + LastACVoltage + ",V," + LastACCurrent +
              ",A,"  + LastPowerFactor + ",PF," + LastACRealPower + ",W{real)," + LastFreq + ",Hz," + LastACWatts + ",W(total),"
              + LastACReactive + ",var(reactive),"  + LastACApparent + ",VA(apparent)," + LastACFundimental + ",W(fundimental),"
              + LastACHarmonic + ",W(harmonic)", WhichPort);
  }
  else {
    SendItOut("%R," + LastTimeACVoltage + ",Energy Monitor,disabled" , WhichPort);
  }
}

void GetUnits(int WhichPort) {
  SendItOut("%R,Units," + String(Units), WhichPort);
}

void GetDeviceInfo(int WhichPort) {
  SendItOut("%R," + DeviceName + "," + FWVersion, WhichPort);
}

void GetStreamingState(int WhichPort) {
  SendItOut("%R,StreamingData,USB," + StatesForOutput(StreamingDataUSB) + ",RS232," +
            StatesForOutput(StreamingDataRS232) , WhichPort);
}

void GetWaterPumpSense(int WhichPort) {
  SendItOut("%R,Water Pump Sense," + StatesForOutput(UseWaterPumpSense), WhichPort);
}

void GetWaterState(int WhichPort) {
  SendItOut("%R,Water," + StatesForOutput(WaterOn), WhichPort);
}

void GetSystemTime(int WhichPort) {
  SendItOut("%R,System Time," + GetCurrentTime(), WhichPort);
}

void GetACEnmon(int WhichPort) {
  SendItOut("%R,AC Energy Monitoring," + StatesForOutput(EnableACEnergyMonitoring), WhichPort);
}

void GetACVOLTAGEGAIN(int WhichPort) {
  SendItOut("%R,ACVOLTAGEGAIN," + String(VoltageGain), WhichPort);
}

void GetACFREQ(int WhichPort) {
  if (LineFreq == 4485) {
    SendItOut("%R,ACFREQ,60", WhichPort);
  }
  if (LineFreq == 389) {
    SendItOut("%R,ACFREQ,50", WhichPort);
  }
}

void GetACPGAGAIN(int WhichPort) {
  SendItOut("%R,ACPGAGAIN," + String(PGAGain), WhichPort);
}

void GetACLEGS(int WhichPort) {
  SendItOut("%R,ACLEGS," + String(NumberOfACLegs), WhichPort);
}

void GetACCT1GAIN(int WhichPort) {
  SendItOut("%R,ACCT1GAIN," + String(CurrentGainCT1), WhichPort);
}

void GetACCT2GAIN(int WhichPort) {
  SendItOut("%R,ACCT2GAIN," + String(CurrentGainCT2), WhichPort);
}

void GetBathroomWaterDuration(int WhichPort) {
  SendItOut("%R,BATHROOMWATERDURATION," + String(GetFromEEPROMBathroomWaterDuration()), WhichPort);
}

void GetKitchenWaterDuration(int WhichPort) {
  SendItOut("%R,KITCHENWATERDURATION," + String(GetFromEEPROMKitchenWaterDuration()), WhichPort);
}

void GetStreamingOnBoot(int WhichPort) {
  SendItOut("%R,Streaming Data on boot,USB," + StatesForOutput(GetFromEEPROMStreamOnBootUSB()) + ",RS232," +
            StatesForOutput(GetFromEEPROMStreamOnBootRS232()) , WhichPort);
}

void GETACENMONOnBoot(int WhichPort) {
  SendItOut("%R,AC Energy Monitoring on boot," + StatesForOutput(GetFromEEPROMACENMONOnBoot()), WhichPort);
}

void GETWaterpumpsenseBoot(int WhichPort) {
  SendItOut("%R,Water Pump Sense on boot," + StatesForOutput(GetFromEEPROMWaterPumpSenseOnBoot()), WhichPort);
}

void GetAlarmStatus(int WhichPort) {
  String Message = "%R," + GetCurrentTime() + ",ALARM,Cleared";
  if (AlarmActive == true) {
    Message = "%R," + GetCurrentTime() + ",ALARM,Active";
  }
  SendItOut(Message, WhichPort);
}

void GetWaterSourceOverRide(int WhichPort) {
  String Message = "%R,Water Source Override," + StatesForOutput(WaterSourceOverRide);
  SendItOut(Message, WhichPort);
}

void GetWaterSourceOverRideOnBoot(int WhichPort) {
  String Message = "%R,Water Source Override On Boot," + StatesForOutput(GetFromEEPROMWaterSourceOverRideOnBoot());
  SendItOut(Message, WhichPort);
}

void GetTravel(int WhichPort) {
  String Message = "%R,Travel Mode," + StatesForOutput(Travel);
  SendItOut(Message, WhichPort);
}


void GetTankAlarmOverRide(int WhichPort) {
  String Message = "%R,Tank Alarm Override," + StatesForOutput(TankAlarmOverRide);
  SendItOut(Message, WhichPort);
  if (TankAlarmOverRide == 1) {
    BroadCast("%R,CAUTION CAUTION CAUTION Tank Alarm Override is enabled holding tanks can be overflowed");
  }
}

void GetWarningIndicator(int WhichPort) {
  String Message = "%R,Warning Indicator Override," + StatesForOutput(WarningIndicator);
  SendItOut(Message, WhichPort);
}

//------------------------------------------------------------------
//Alarm and Warnings
//------------------------------------------------------------------
void Error(int Number, int WhichPort) {
  const char* Errors[] = {"command not recognized", "command Parameter out of range", "command not supported on this platform",
                          "command can not be processed", "Invalid Parameter", "Invalid Command Format"
                         };
  SendItOut("%R,Error," + String(Errors[Number]), WhichPort);
}

void Warning() {
  if (WarningActive == true && WarningIndicator == true) {
    if (abs(millis() - WarningBlinkTimer) >  333) {
      WarningBlinkTimer = millis();
      if (digitalRead(WarningLED) == LOW) {
        digitalWrite(WarningLED, HIGH);
      } else {
        digitalWrite(WarningLED, LOW);
      }
    }
  }
  else {
    if (digitalRead(WarningLED) == HIGH) {
      digitalWrite(WarningLED, LOW);
    }
  }
}

void AddWarningToList(int WarningID) {
  WarningActive = true;
  if (ArrayOfWarnings[WarningID] != WarningID) {
    ArrayOfWarnings[WarningID] = WarningID;
    OutputWarningMessage(WarningID);
  }
}

void RemoveWarningToList(int WarningID) {
  if (ArrayOfWarnings[WarningID] != -1) {
    ArrayOfWarnings[WarningID] = -1;
  }
  bool ClearedWarnings = true;
  for (int i = 1; i <= TotalWarnings; i++) {
    if (ArrayOfWarnings[i] != -1) {
      ClearedWarnings = false;
    }
  }
  if (ClearedWarnings == true) {
    WarningActive = false;
  }
}

void OutputWarningMessage(int ID) {
  if (ArrayOfWarnings[ID] != 0) {
    String Message = "";
    switch (ID) {
      case 1:
        Message = "RTC Battery voltage is low ";
        break;
      case 2:
        Message = "Grey tank is getting full";
        break;
      case 3:
        Message = "Grey Water Tank ERROR";
        break;
      case 4:
        Message = "Sewage Tank ERROR";
        break;
      case 5:
        Message = "Camper Battery voltage is low";
        break;
      case 6:
        Message = "Water Tank Level is low";
        break;
      case 7:
        Message = "LPG Level is low";
        break;
      case 8:
        Message = "Sewage tank is getting full";
        break;
    }
    BroadCast("%R,Warning," + Message);
  }
}

void AllWarningMessages(int WhichPort) {
  if (WarningActive == true) {
    for (int i = 1; i <= TotalWarnings; i++) {
      if (ArrayOfWarnings[i] != -1) {
        OutputWarningMessage(i);
        WarningActive = true;
      }
    }
  }
  else {
    SendItOut("%R,Warning,Clear", WhichPort);
  }
}

void ALARM() {
  digitalWrite(AlarmOut, HIGH);
  if (AlarmActive == false) {
    AlarmActive = true;
    GetAlarmStatus(0);
    GetAlarmStatus(1);
  }
}

void ResetAlarm() {
  digitalWrite(AlarmOut, LOW);
  if (AlarmActive == true) {
    AlarmActive = false;
    GetAlarmStatus(0);
    GetAlarmStatus(1);
  }
}

void ResetWarnings() {
  for (int i = 1; i <= TotalWarnings; i++) {
    ArrayOfWarnings[i] = -1;
  }
  WarningActive = false;
  digitalWrite(WarningLED, LOW);
}

void ResetAllAlarmsAndWarnings() {
  HoldingTankAlarm = false;
  ShittersGettinFull = GreyGettinFull = false;
  ResetAlarm();
  ResetWarnings();
}
//------------------------------------------------------------------
//Commands
//------------------------------------------------------------------
void SetUnits(String Value, int WhichPort) {
  char CurrentUnits = Units;
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End - 1);
  if (ThingToTest.length() == 1) {
    bool CorrectParam = false;
    if (ThingToTest == "I") {
      Units = 'I';
      CorrectParam = true;
    }
    if (ThingToTest == "M") {
      Units = 'M';
      CorrectParam = true;
    }
    if (CorrectParam == true) {
      EEPROM.update(0, Units);
      GetUnits(WhichPort);
      if (CurrentUnits != Units) {
        ForceCompleteUpdateOfAllStates();
      }
    }
    else {
      Error(4, WhichPort);
    }
  }
  else {
    Error(1, WhichPort);
  }
  SetUnitsForOutput();
}

void SetWaterPumpSenseOverRide(String Value, int WhoToSet, int WhichPort) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End - 1);
  bool CorrectParam = false;
  if (ThingToTest == "OFF") {
    UseWaterPumpSense = false;
    if (WhoToSet == 1) {
      EEPROM.update(16, 0);
    }
    CorrectParam = true;
  }
  if (ThingToTest == "ON") {
    UseWaterPumpSense = true;
    if (WhoToSet == 1) {
      EEPROM.update(16, 1);
    }
    CorrectParam = true;
  }

  if (CorrectParam == true) {
    if (WhoToSet == 1) {
      GETWaterpumpsenseBoot(WhichPort);
    }
    GetWaterPumpSense(WhichPort);
  }
  else {
    Error(4, WhichPort);
  }
}

void SetWater(String Value, int WhichPort) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End - 1);
  bool CorrectParam = false;
  if (ThingToTest == "OFF") {
    TurnOffWater();
    CorrectParam = true;
  }
  if (ThingToTest == "ON") {
    TurnOnWater(false);
    WhoTurnedOnTheWater = 0;
    CorrectParam = true;
  }

  if (CorrectParam == false) {
    Error(4, WhichPort);
  }
}

void SetStreamingData(String Value, int WhoToSet, int WhichPort) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String Start = Value.substring(Index + 1, End - 1);
  int SecondParam = Start.indexOf("*");
  String SerialPortName  = Start.substring(0, SecondParam);
  String State = Start.substring(SecondParam + 1, End - 1);
  bool CorrectParam = false;
  if (SerialPortName == "USB") {
    if (State == "OFF") {
      if (WhoToSet == 1) {
        EEPROM.update(4, 0);
      }
      StreamingDataUSB = false;
      CorrectParam = true;
    }
    if (State == "ON") {
      if (WhoToSet == 1) {
        EEPROM.update(4, 1);
      }
      StreamingDataUSB = true;
      CorrectParam = true;
    }
  }

  if (SerialPortName == "RS232") {
    if (State == "OFF") {
      if (WhoToSet == 1) {
        EEPROM.update(5, 0);
      }
      StreamingDataRS232 = false;
      CorrectParam = true;
    }
    if (State == "ON") {
      if (WhoToSet == 1) {
        EEPROM.update(5, 1);
      }
      StreamingDataRS232 = true;
      CorrectParam = true;
    }
  }

  if (CorrectParam == true) {
    if (WhoToSet == 1) {
      GetStreamingOnBoot(WhichPort);
    }
    GetStreamingState(WhichPort);
  }
  else {
    Error(4, WhichPort);
  }
}

void SetRTCDateTime(String Value, int WhichPort) {
  // need to set Year Month Day Hour Min Second
  //rtc.adjust(DateTime(year, month, day, hour, minute, seconds));
  //Example 8-19-2020 5:25:45 PM settime*2020:8:19:17:25:45\r
  bool CorrectParam = true;
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End);
  int NextIndex = -1, PreviousIndex;
  int TimeArray[6] = { -1, -1, -1, -1, -1, -1};

  for (int i = 0; i < 6; i++) {
    NextIndex = ThingToTest.indexOf(":");
    if (NextIndex != PreviousIndex || NextIndex == 2) {
      TimeArray[i] = ThingToTest.substring(0, NextIndex).toInt();
      ThingToTest = ThingToTest.substring(NextIndex + 1);
      PreviousIndex = NextIndex;
    }
  }

  for (int i = 0; i < 6; i++) {
    //check that all the params are filled out.
    if (TimeArray[i] == -1) {
      CorrectParam = false;
    }
  }

  if (2021 > TimeArray[0]) {
    SendItOut("%R,Error,Can't set the year older than when i made this mess : " + String(TimeArray[0]), WhichPort);
    CorrectParam = false;
  }

  if (1 > TimeArray[1] ||  TimeArray[1] > 12) {
    SendItOut("%R,Error,Month 1-12 accepted : " + String(TimeArray[1]), WhichPort);
    CorrectParam = false;
  }

  if (1 > TimeArray[2] ||  TimeArray[2] > 31) {
    SendItOut("%R,Error,Date 1-31 accepted : " + String(TimeArray[2]), WhichPort);
    CorrectParam = false;
  }

  if (0 > TimeArray[3] ||  TimeArray[3] > 24) {
    SendItOut("%R,Error,Hour 0-23 accepted : " + String(TimeArray[3]), WhichPort);
    CorrectParam = false;
  }

  if (0 > TimeArray[4] || TimeArray[4] >= 60) {
    SendItOut("%R,Error,Min 0-59 accepted : " + String(TimeArray[4]), WhichPort);
    CorrectParam = false;
  }

  if (0 > TimeArray[5] ||  TimeArray[5] >= 60) {
    SendItOut("%R,Error,Sec 0-59 accepted : " + String(TimeArray[5]), WhichPort);
    CorrectParam = false;
  }

  if (CorrectParam == true) {
    rtc.adjust(DateTime(TimeArray[0], TimeArray[1], TimeArray[2], TimeArray[3], TimeArray[4], TimeArray[5]));
    delay(100);
    GetSystemTime(WhichPort);
  }
  else {
    Error(4, WhichPort);
  }
}

void SetACEnmonOnBooot(String Value, int WhichPort) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End - 1);
  bool CorrectParam = false;
  if (ThingToTest == "OFF") {
    EEPROM.update(2, 0);
    CorrectParam = true;
  }

  if (ThingToTest == "ON") {
    EEPROM.update(2, 1);
    CorrectParam = true;
  }

  if (CorrectParam == true) {
    GETACENMONOnBoot(WhichPort);
  }
  else {
    Error(4, WhichPort);
  }
}

void SetACEnmon(String Value, int WhichPort) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End - 1);
  bool CorrectParam = false;
  if (ThingToTest == "OFF") {
    EnableACEnergyMonitoring = false;
    CorrectParam = true;
  }

  if (ThingToTest == "ON") {
    EnableACEnergyMonitoring = true;
    CorrectParam = true;
  }

  if (CorrectParam == true) {
    GetACEnmon(WhichPort);
  }
  else {
    Error(4, WhichPort);
  }
  EnablingPowerToTransformerForEnergyMonitoring();
}

void SetACFREAK(String Value, int WhichPort) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End - 1);
  bool CorrectParam = false;
  if (ThingToTest == "50") {
    LineFreq = 389;
    EEPROM.update(6, 50);
    CorrectParam = true;
  }

  if (ThingToTest == "60") {
    EEPROM.update(6, 60);
    LineFreq = 4485;
    CorrectParam = true;
  }

  if (CorrectParam == true) {
    GetACFREQ(WhichPort);
  }
  else {
    Error(4, WhichPort);
  }
}

void SetACLEGS(String Value, int WhichPort) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End - 1);
  bool CorrectParam = false;
  if (ThingToTest == "1") {
    NumberOfACLegs = 1;
    CorrectParam = true;
  }

  if (ThingToTest == "2") {
    NumberOfACLegs = 2;
    CorrectParam = true;
  }

  if (CorrectParam == true) {
    EEPROM.update(7, NumberOfACLegs);
    GetACLEGS(WhichPort);
  }
  else {
    Error(4, WhichPort);
  }
}

void SetACVOLTAGEGAIN(String Value, int WhichPort) {
  unsigned short CurrentVOLTAGEGAIN = VoltageGain;
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  unsigned short ThingToTest = Value.substring(Index + 1, End - 1).toInt();
  VoltageGain = ThingToTest;
  EEPROM.update(8, highByte(ThingToTest));
  EEPROM.update(9, lowByte(ThingToTest));
  GetACVOLTAGEGAIN(WhichPort);
  if (CurrentVOLTAGEGAIN != VoltageGain) {
    SetupEnergyMonitor();
  }
}

void SetACCT1GAIN(String Value, int WhichPort) {
  unsigned short CurrentCT1GAIN = CurrentGainCT1;
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  unsigned short ThingToTest = Value.substring(Index + 1, End - 1).toInt();
  CurrentGainCT1 = ThingToTest;
  EEPROM.update(10, highByte(ThingToTest));
  EEPROM.update(11, lowByte(ThingToTest));
  GetACCT1GAIN(WhichPort);
  if (CurrentCT1GAIN != CurrentGainCT1) {
    SetupEnergyMonitor();
  }
}

void SetACCT2GAIN(String Value, int WhichPort) {
  unsigned short CurrentCT2GAIN = CurrentGainCT2;
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  unsigned short ThingToTest = Value.substring(Index + 1, End - 1).toInt();
  CurrentGainCT2 = ThingToTest;
  EEPROM.update(12, highByte(ThingToTest));
  EEPROM.update(13, lowByte(ThingToTest));
  GetACCT2GAIN(WhichPort);
  if (CurrentCT2GAIN != CurrentGainCT2) {
    SetupEnergyMonitor();
  }
}

void SetACPGAGAIN(String Value, int WhichPort) {
  unsigned short CurrentPGAGain = PGAGain;
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  unsigned short ThingToTest = short(Value.substring(Index + 1, End - 1).toInt());
  PGAGain = ThingToTest;
  EEPROM.update(14, highByte(ThingToTest));
  EEPROM.update(15, lowByte(ThingToTest));
  GetACPGAGAIN(WhichPort);
  if (CurrentPGAGain != PGAGain) {
    SetupEnergyMonitor();
  }
}

void SetWaterBathroomDurationInSeconds(String Value, int WhichPort) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  int ThingToTest = Value.substring(Index + 1, End - 1).toInt();
  if (60 <= ThingToTest && ThingToTest <= 1800) {
    BathroomWaterDurationInSeconds = ThingToTest;
    EEPROM.update(20, highByte(ThingToTest));
    EEPROM.update(21, lowByte(ThingToTest));
  }
  else {
    SendItOut("%R," + GetCurrentTime() + ",Error,BathroomWaterDuration must be between 150 and 1800 seconds", WhichPort);
    Error(1, WhichPort);
  }
  GetBathroomWaterDuration(WhichPort);
}

void SetWaterKitchenDurationInSeconds(String Value, int WhichPort) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  int ThingToTest = Value.substring(Index + 1, End - 1).toInt();
  if (60 <= ThingToTest && ThingToTest <= 1800) {
    KitchenWaterDurationInSeconds = ThingToTest;
    EEPROM.update(18, highByte(ThingToTest));
    EEPROM.update(19, lowByte(ThingToTest));
  }
  else {
    SendItOut("%R," + GetCurrentTime() + ",Error,KitchenWaterDuration must be between 150 and 1800 seconds", WhichPort);
    Error(1, WhichPort);
  }
  GetKitchenWaterDuration(WhichPort);
}

void SetWaterSourceOverRide(String Value, int WhichPort) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End - 1);
  bool CorrectParam = false;
  if (ThingToTest == "OFF") {
    WaterSourceOverRide = false;
    CorrectParam = true;
  }

  if (ThingToTest == "ON") {
    WaterSourceOverRide = true;
    CorrectParam = true;
  }

  if (CorrectParam == true) {
    GetWaterSourceOverRide(WhichPort);
  }
  else {
    Error(4, WhichPort);
  }
}

void SetWaterSource(String Value, int WhichPort) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End - 1);
  bool CorrectParam = false;
  LastSourceForCheck = WaterSourseSelection;
  if (ThingToTest == "CITY") {
    WaterSourseSelection = false;
    CorrectParam = true;
  }

  if (ThingToTest == "TANK") {
    WaterSourseSelection = true;
    CorrectParam = true;
  }

  if (CorrectParam == true) {
    GetWaterSource(WhichPort);
  }
  else {
    Error(4, WhichPort);
  }
}

void SetWaterSourceOverRideOnBoot(String Value, int WhichPort) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End - 1);
  bool CorrectParam = false;
  if (ThingToTest == "OFF") {
    EEPROM.update(17, 0);
    CorrectParam = true;
  }

  if (ThingToTest == "ON") {
    EEPROM.update(17, 1);
    CorrectParam = true;
  }

  if (CorrectParam == true) {
    GetWaterSourceOverRideOnBoot(WhichPort);
  }
  else {
    Error(4, WhichPort);
  }
}

void SetTravel(String Value, int WhichPort) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End - 1);
  bool CorrectParam = false;
  if (ThingToTest == "OFF") {
    Travel = false;
    CorrectParam = true;
  }

  if (ThingToTest == "ON") {
    Travel = true;
    CorrectParam = true;
  }

  if (CorrectParam == true) {
    GetTravel(WhichPort);
  }
  else {
    Error(4, WhichPort);
  }
}

void SetTankAlarmOverRide(String Value, int WhichPort) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End - 1);
  bool CorrectParam = false;
  if (ThingToTest == "OFF") {
    TankAlarmOverRide = false;
    CorrectParam = true;
  }

  if (ThingToTest == "ON") {
    TankAlarmOverRide = true;
    CorrectParam = true;
  }

  if (CorrectParam == true) {
    GetTankAlarmOverRide(WhichPort);
  }
  else {
    Error(4, WhichPort);
  }
}

void SetWarningIndicator(String Value, int WhichPort) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End - 1);
  bool CorrectParam = false;
  if (ThingToTest == "OFF") {
    WarningIndicator = false;
    CorrectParam = true;
  }

  if (ThingToTest == "ON") {
    WarningIndicator = true;
    CorrectParam = true;
    digitalWrite(WarningLED, LOW);
  }

  if (CorrectParam == true) {
    GetWarningIndicator(WhichPort);
  }
  else {
    Error(4, WhichPort);
  }
}

/*
  SCC = start command character
  case 1 - no SCC found and there is data in the buffer - dump the buffer
  case 2 - SCC is found and not at position 0 - trim the buffer up to the SCC and insert error
  case 3 - SCC is found and at position 0  - process command
  case 4 - SCC is found and no delimiter found and there is data in the buffer  - add back to the buffer
  case 5 - SCC is found no delimiter found and another scc is found trim up to the second
  case 6 - No SCC and No Delimiter and there is data in teh buffer - dump the buffer
  case 7 - Valid SSC and Delimiter is found but the command is not in the list of commands - tell the user
*/

String PainlessInstructionSet(String & TestString, int WhichPort) {
  int Search = 1;
  while (Search == 1) {
    bool ParamCommandCalled = false;
    bool CommandCalled = false;
    int FindStart = TestString.indexOf('%');
    int Param = TestString.indexOf('*');
    int FindEnd = TestString.indexOf('\r');
    if (TestString != "") {
      if (FindStart != -1) { //case 1
        if (FindStart != 0) { //case 2
          //Serial.println("PIS Case 2");
          SendItOut("%R,Error,BAD Command Format No Start or Stop Delimiters", WhichPort);
          TestString.remove(0, FindStart);
        }
        else { //Case 3 & Case 5 & Case 4
          String Case5Test = TestString.substring(FindStart + 1);
          int FindStart1 = Case5Test.indexOf('%');
          int FindEnd1 = Case5Test.indexOf('\r');
          if ((FindEnd1 > FindStart1) && (FindStart1 != -1)) {
            SendItOut("%R,Error,BAD Command Format - No End Delimiter", WhichPort);
            //Serial.println("PIS Case 5");
            TestString.remove(0, FindStart1 + 1);
          }
          else {
            if (FindEnd != -1 || FindEnd1 != -1) {
              //Serial.println("PIS Case 3");
              String CommandCandidate = TestString.substring(FindStart + 1, FindEnd);
              CommandCandidate.toUpperCase();
              if ((Param < FindEnd) && Param != -1) {
                for (int i = 0; i < (sizeof(ParameterCommands) / sizeof(int)); i++)
                {
                  //Serial.println("PIS Case 3A");
                  String ParamHeader = CommandCandidate.substring(FindStart, Param - 1);
                  ParamHeader.toUpperCase();
                  if (ParamHeader == ParameterCommands[i]) {
                    ParamCommandToCall(i, CommandCandidate, WhichPort);
                    ParamCommandCalled = true;
                  }
                }
              }
              else {
                for (int i = 0; i < (sizeof(AcceptedCommands) / sizeof(int)); i++)
                {
                  //Non Parameter Commands
                  if (CommandCandidate == AcceptedCommands[i]) {
                    //Serial.println("PIS Case 3B");
                    CommandToCall(i, WhichPort);
                    CommandCalled = true;
                  }
                }
              }
              if (CommandCalled == false && ParamCommandCalled == false) {
                //Serial.println("PIS Case 7");
                SendItOut("%R,Error,Command not recognized", WhichPort);
              }
              TestString.remove(0, FindEnd + 1);
            }
            else {
              //Serial.println("PIS Case 4");
              Search = 0;
            }
          }
        }
      }
      else { //Case 1 Dump the buffer if ther is no start character is found
        //Serial.println("PIS Case 1");
        Search = 0;
        TestString = "";
        SendItOut("%R," + GetCurrentTime() + ",Error,BAD Command Format - No Start Command Character", WhichPort);
      }
    }//if TestString is empty
    else { //Exit Search While if Buffer is empty
      Search = 0;
      //Serial.println("PIS Case 6");
    }
  }//End of Search While
  return TestString;
}//End of PIS Function

void ParamCommandToCall(int Index, String CommandRaw, int WhichPort) {
  switch (Index)
  {
    case 0:
      //Set Units
      SetUnits(CommandRaw, WhichPort);
      break;
    case 1:
      //Set Water Pump Sense
      SetWaterPumpSenseOverRide(CommandRaw, 0, WhichPort);
      break;
    case 2:
      //Water
      SetWater(CommandRaw, WhichPort);
      break;
    case 3:
      //Set Streaming Data Output
      SetStreamingData(CommandRaw, 0, WhichPort);
      break;
    case 4:
      //Set RTC Datetime
      SetRTCDateTime(CommandRaw, WhichPort);
      break;
    case 5:
      //Enable/disable AC energy Monitoring
      SetACEnmon(CommandRaw, WhichPort);
      break;
    case 6:
      //SET AC FREQUNECY 50/60 Hz
      SetACFREAK(CommandRaw, WhichPort);
      break;
    case 7:
      //SET AC PGAGAIN
      SetACPGAGAIN(CommandRaw, WhichPort);
      break;
    case 8:
      //SET AC VOLTAGEGAIN
      SetACVOLTAGEGAIN(CommandRaw, WhichPort);
      break;
    case 9:
      SetACLEGS(CommandRaw, WhichPort);
      break;
    case 10:
      //SET AC CT1 GAIN
      SetACCT1GAIN(CommandRaw, WhichPort);
      break;
    case 11:
      //SET AC CT2 GAIN
      SetACCT2GAIN(CommandRaw, WhichPort);
      break;
    case 12:
      //SET BATHROOM WATERDURATION
      SetWaterBathroomDurationInSeconds(CommandRaw, WhichPort);
      break;
    case 13:
      //SET STREAMING ON BOOT
      SetStreamingData(CommandRaw, 1, WhichPort);
      break;
    case 14:
      //SET ACEMON ON BOOT
      SetACEnmonOnBooot(CommandRaw, WhichPort);
      break;
    case 15:
      //SET Waterpumpsense ON BOOT
      SetWaterPumpSenseOverRide(CommandRaw, 1, WhichPort);
      break;
    case 16:
      //SETWATERSOURCEOVERRIDE
      SetWaterSourceOverRide(CommandRaw, WhichPort);
      break;
    case 17:
      //SETWATERSOURCE
      SetWaterSource(CommandRaw, WhichPort);
      break;
    case 18:
      //SETWATERSOURCEOVERRIDEONBOOT
      SetWaterSourceOverRideOnBoot(CommandRaw, WhichPort);
      break;
    case 19:
      //SETTRAVEL
      SetTravel(CommandRaw, WhichPort);
      break;
    case 20:
      // SetWaterKitchenDurationInSeconds
      SetWaterKitchenDurationInSeconds(CommandRaw, WhichPort);
      break;
    case 21:
      //SetTankAlarmOverRide
      SetTankAlarmOverRide(CommandRaw, WhichPort);
      break;
    case 22:
      //SetWarningIndicator
      SetWarningIndicator(CommandRaw, WhichPort);
      break;
  }
}

void CommandToCall(int Index, int WhichPort) {
  switch (Index)
  {
    case 0:
      //UNITS
      GetUnits(WhichPort);
      break;
    case 1:
      //DEVICE INFORMATION
      GetDeviceInfo(WhichPort);
      break;
    case 2:
      //WATERSOURCE
      GetWaterSource(WhichPort);
      break;
    case 3:
      //WATERLEVEL
      GetWaterLevel(WhichPort);
      break;
    case 4:
      //LPG LEVEL
      GetLPGLevel(WhichPort);
      break;
    case 5:
      //SEWAGE LEVEL
      GetSewageLevel(WhichPort);
      break;
    case 6:
      //GREY LEVEL
      GetGreyLevel(WhichPort);
      break;
    case 7:
      //ENERGY MONITOR
      GetEnergyStatus(WhichPort);
      break;
    case 8:
      //CAMPER DV VOLTAGE
      GetDCBatteryVoltage(WhichPort);
      break;
    case 9:
      //RTC BATTERY VOLTAGE
      GetRTCBatteryVotlage(WhichPort);
      break;
    case 10:
      //GENERATOR STATUS
      GetGenStatus(WhichPort);
      break;
    case 11:
      //NTC TEMPS
      GetNTCTemps(WhichPort);
      break;
    case 12:
      //HEAD UNIT TEMP
      GetHeadUnitTemp(WhichPort);
      break;
    case 13:
      //WATERPUMPSENSE
      GetWaterPumpSense(WhichPort);
      break;
    case 14:
      //WARNING
      AllWarningMessages(WhichPort);
      break;
    case 15:
      //WATER
      GetWaterState(WhichPort);
      break;
    case 16:
      //STREAMING
      GetStreamingState(WhichPort);
      break;
    case 17:
      //AC energy monitoring
      GetACEnmon(WhichPort);
      break;
    case 18:
      //Get All Data at one go
      OutputAllData(WhichPort);
      break;
    case 19:
      //Force Update of all states
      ForceCompleteUpdateOfAllStates();
      BroadCast("%R," + GetCurrentTime() + ",All States Updated");
      break;
    case 20:
      //Reset Warnings
      ResetWarnings();
      BroadCast("%R," + GetCurrentTime() + ",Reset Warnings");
      break;
    case 21:
      //Reset All alarms and warnings
      ResetAllAlarmsAndWarnings();
      BroadCast("%R," + GetCurrentTime() + ",Reset All Alarms and Warnings");
      break;
    case 22:
      //Show System Time
      GetSystemTime(WhichPort);
      break;
    case 23:
      //Show AC VOLTAGEGAIN
      GetACVOLTAGEGAIN(WhichPort);
      break;
    case 24:
      //Show AC FREQUENCY
      GetACFREQ(WhichPort);
      break;
    case 25:
      //Show AC PGAGAIN
      GetACPGAGAIN(WhichPort);
      break;
    case 26:
      //Show AC LEGS
      GetACLEGS(WhichPort);
      break;
    case 27:
      //Show AC CT1GAIN
      GetACCT1GAIN(WhichPort);
      break;
    case 28:
      //Show AC CT2GAIN
      GetACCT2GAIN(WhichPort);
      break;
    case 29:
      //Reboot the MCU
      RebootDisBitch();
      break;
    case 30:
      //reset to defaults
      MIBFLASH();
      break;
    case 31:
      //BathroomWaterDuration
      GetBathroomWaterDuration(WhichPort);
      break;
    case 32:
      //Streaming on boot
      GetStreamingOnBoot(WhichPort);
      break;
    case 33:
      //ACENMON ON BOOT
      GETACENMONOnBoot(WhichPort);
      break;
    case 34:
      //Waterpumpsense ON BOOT
      GETWaterpumpsenseBoot(WhichPort);
      break;
    case 35:
      //Status update for live system stuff
      OutputLiveData(WhichPort);
      break;
    case 36:
      //Which port you are communicating on
      CurrentSerialPort(WhichPort);
      break;
    case 37:
      //Alarm
      GetAlarmStatus(WhichPort);
      break;
    case 38:
      //WaterSourceOverRide
      GetWaterSourceOverRide(WhichPort);
      break;
    case 39:
      //WaterSourceOverRideOnBoot
      GetWaterSourceOverRideOnBoot(WhichPort);
      break;
    case 40:
      //Travel
      GetTravel(WhichPort);
      break;
    case 41:
      GetKitchenWaterDuration(WhichPort);
      break;
    case 42:
      //ALARMWATEROFFOVERRIDE?
      GetTankAlarmOverRide(WhichPort);
      break;
    case 43:
      //INDICATORS?
      GetWarningIndicator(WhichPort);
      break;
  }
}
