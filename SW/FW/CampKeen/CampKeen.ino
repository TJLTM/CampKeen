#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_MAX31865.h>
#include <SPI.h>
#include <RTClib.h>
#include <EEPROM.h>
#include <ATM90E32.h>

#define ControlComPort Serial
char* AcceptedCommands[] = {"UNITS?", "DEVICE?", "WATERSOURCE?", "WATERLEVEL?", "LPG?", "SEWAGE?", "GREY?",
                            "ENERGY?", "BATTERY?", "RTCBATTERY?", "GENERATOR?", "TEMPS?", "UNITTEMP?", "WATERPUMPSENSE?", "WARNING?",
                            "WATER?", "STREAMING?", "ACENMON?", "ALLDATA?", "UPDATEALL", "RESETWARNINGS", "RESETALLALARMS"
                           };
char* ParameterCommands[] = {"SETUNITS", "SETWATERPUMPSENSE", "WATER", "SETSTREAMINGDATA", "SETOUTPUT",
                             "READINPUT", "SETRTC", "GETOUTPUT", "SETACENMON"
                            };
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;     // whether the string is complete
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 20, 4);
int DisplayCounter = 0;
#define LCDEnable 10
#define LCDPowerOut 5
//-----------------------------------------------------------
// ATM90E32 energy monitor settings and calibrations
/*
   The Calibration values for the Voltage and Current Gain I had to
   Use some meters to test and calibrate these values and these are
   what I have put in.
*/
ATM90E32 eic{}; // Energy Monitor Object

/* 4485 for 60 Hz (North America)
  389 for 50 hz (rest of the world)
*/
unsigned short LineFreq = 4485;

/* 0 for 10A (1x)
   21 for 100A (2x)
   42 for between 100A - 200A (4x)
*/
unsigned short PGAGain = 21;

/*
   For meter <= v1.3:
      42080 - 9v AC Transformer - Jameco 112336
      32428 - 12v AC Transformer - Jameco 167151
   For meter > v1.4:
      37106 - 9v AC Transformer - Jameco 157041
      38302 - 9v AC Transformer - Jameco 112336
      29462 - 12v AC Transformer - Jameco 167151
   For Meters > v1.4 purchased after 11/1/2019 and rev.3
      7611 - 9v AC Transformer - Jameco 157041
*/
unsigned short VoltageGain = 1975;

/* 25498 - SCT-013-000 100A/50mA
   39473 - SCT-016 120A/40mA
   46539 - Magnalab 100A
*/
unsigned short CurrentGainCT1 = 34500;
unsigned short CurrentGainCT2 = 34500;

//-----------------------------------------------------------
//-----------------------------------------------------------
// System Level
const String DeviceName = "CampKeen";
const String FWVersion = "0.7.3";
const int DisplayInvterval = 3000;
const float ConversionFactor = 5.0 / 1023;
bool WarningActive = false;
int ArrayOfWarnings[5] = {};
long WaterTimer, ShitterTankTimer, GreyTankTimer, WATERLPGtimer, FiveMinTimer, DisplayTimer, NTCTimer,
     EnergyTimer, OutputTimer, HoldingTankTimer, LastTimeWaterWasTurnedOn, WarningBlinkTimer;
String TempUnits = "F";
String PressureUnits = "PSI";
bool LCDSetup = false;
//WaterSourceSelection //false = pump //true = City Water
bool WaterSourseSelection = false, WaterOn = false, LastSourceForCheck = false;
bool EnableACEnergyMonitoring = false;
//Figure out if i want to store these in EEPROM
String Units = "I"; // address 0
bool UseWaterPumpSense = false; // address 1
bool StreamingData = false; // address 2
//-----------------------------------------------------------
/*
   All the Stored Values and Times to have states
   that can be recalled if streaming is turned off
*/
//Water and LPG
String LastTimeWaterSource = "";
String LastSource = "Tank";
String LastWaterLevel = "Empty";
String LastTimeWaterLevel = "";
int LastLPGLevel = 0;
//Holding tanks
String LastSewageLevel = "Empty";
String LastTimeSewageLevel = "";
String LastGreyWater = "Empty";
String LastTimeGreyWater = "";
//Battery Monitoring
float LastDCVoltage = 0.0;
String LastTimeDCVoltage = "";
float LastRTCVoltage = 0.0;
String LastTimeRTCVoltage = "";
//NTC Temps
String LastTimeNTCTemp = "";
float LastFrontACTemp = 0.0;
float LastBackACTemp = 0.0;
float LastOutsideTemp = 0.0;
float LastBackCabinTemp = 0.0;
float LastHallwayTemp = 0.0;
float LastFreezerTemp = 0.0;
float LastFridgeTemp = 0.0;
float LastBathroomTemp = 0.0;
//Generator
String LastTimeGenSensors = "";
float LastGenEnclosureTemp = 0.0;
float LastGenHeadRightTemp = 0.0;
float LastGenHeadLeftTemp = 0.0;
float LastGenFuel = 0.0;
//AC&Energy Monitoring
String LastTimeACVoltage = "";
float LastACVoltage = 0.0;
float LastACCurrent = 0.0;
float LastPowerFactor = 0.0;
float LastFreq = 0.0;
float LastACWatts = 0.0;
float LastACReactive = 0.0;
float LastACApparent = 0.0;
float LastACFundimental = 0.0;
float LastACHarmonic = 0.0;
float LastATM90E32Temp = 0.0;
float LastACRealPower = 0.0;
//RTC TEMP
float LastRTCTemp = 0.0;
String LastTimeRTCTemp = "";
//-----------------------------------------------------------
//-----------------------------------------------------------
//Generator, Energy Monitoring, and Voltage
#define GenFuelPressure A13
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
#define EnergyMonitorCS 41
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
//-----------------------------------------------------------
//-----------------------------------------------------------
//Alarm
#define LEDBacklightOut 4
#define AlarmOut 35
#define WarningLED 33
#define AlarmReset 25
//-----------------------------------------------------------
//-----------------------------------------------------------
//Water Control / Tanks and LPG Tank
#define LPGSensor A1
#define WaterTankSensor A2
#define TankPowerRelay 43
#define WaterSourceSelectionInput 28
#define WaterPumpSense 23
#define KitchWaterButton 2
#define BathroomWaterButton 3
#define CityWaterValve 37
#define KitchenWaterButtonLED 31
#define BathroomWaterButtonLED 29
#define WaterPumpOut 26
//-----------------------------------------------------------
//-----------------------------------------------------------
//Spare IO
/*
   Spare Inputs are indexed by their number +1 because I don't
   want to start at zero.
*/
int SpareInputs[] = {9, 8, 7, 6};
int SpareInputSize = sizeof(SpareInputs) / sizeof(int);
int LastInputState[] = {};
int SpareOutputs[] = {12, 11, 24};
int SpareOutputSize = sizeof(SpareOutputs) / sizeof(int);

/*
   Not sure how i'm going to deal with ADCs as spare analog inputs
*/
#define SPADC2 15
//-----------------------------------------------------------
//-----------------------------------------------------------
// Holding Tank
bool ShittersGettinFull = false;
bool GreyGettinFull = false;
bool HoldingTankAlarm = false;
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
  ControlComPort.begin(115200);
  inputString.reserve(200);

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
      docs for setup of these boards
  */
  GenHeadR.begin(MAX31865_3WIRE);
  GenHeadL.begin(MAX31865_3WIRE);
  GenEnclosure.begin(MAX31865_3WIRE);

  //Energy
  pinMode(EnergyMonitorCS, OUTPUT);
  SetupEnergyMonitor();

  //Set up displays and output on the Serial Port
  ControlComPort.println("Starting " + DeviceName);
  ControlComPort.println("FW: " + FWVersion);
  SetUnitsForOutput();
  digitalWrite(LCDPowerOut, HIGH);
  delay(250);
  SetupLCD();
  lcd.setCursor(0, 0);
  lcd.print("Starting " + DeviceName);
  //Run through the sensors and get values for everything
  ForceCompleteUpdateOfAllStates();
  ControlComPort.print("System Initialized and values populated:");
  ControlComPort.println(GetCurrentTime());
  lcd.setCursor(0, 1);
  lcd.print("System Initialized");
  lcd.setCursor(0, 2);
  delay(750);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(GetCurrentTime());
  lcd.setCursor(0, 1);
  lcd.print(FWVersion);
  lcd.setCursor(0, 2);
  lcd.print("Units: " + Units);
  delay(1000);
  digitalWrite(LCDPowerOut, digitalRead(LCDEnable));
}

void loop() {
  ReadAllInputs();

  if (digitalRead(AlarmReset) == HIGH) {
    ResetAllAlarmsAndWarnings();
  }
  else {
    Warning();
  }

  LCDControl();
  WaterControl();
  HoldingTankMonitoring();

  if (abs(millis() - NTCTimer) > 3000) {
    //Read NTC temp Sensors
    ReadOtherTempSensors();
    //Read Generator Sensors
    GeneratorSensors();
    NTCTimer = millis();
    if (StreamingData == true) {
      GetNTCTemps();
      GetGenStatus();
    }
  }

  if ((abs(millis() - EnergyTimer) > 3000) && EnableACEnergyMonitoring == true) {
    //Read Energy
    EnergyMetering();
    EnergyTimer = millis();
    if (StreamingData == true) {
      GetEnergyStatus();
    }
  }

  if (abs(millis() - WATERLPGtimer) > 1800000) {
    //ReadLPG and Water Tank at 1/2 hour intervals
    ReadWaterAndLPG();
    WATERLPGtimer = millis();
    if (StreamingData == true) {
      GetWaterLevel();
      GetLPGLevel();
    }
  }

  if (abs(millis() - FiveMinTimer) > 300000) {
    //Read DC voltage at 5 min intervals
    ReadBatteryVoltages();
    FiveMinTimer = millis();
    if (StreamingData == true) {
      GetDCBatteryVoltage();
      GetRTCBatteryVotlage();
    }
  }

  if (stringComplete) {
    inputString = PainlessInstructionSet(inputString);
    stringComplete = false;
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
    }
    LCDOutput();
    LCDDisplay();
  }
  else {
    LCDSetup = false;
    digitalWrite(LCDPowerOut, LOW);
  }
}

void SetupLCD() {
  lcd.init();
  lcd.backlight();
  rtc.begin();
  LCDSetup = true;
}

void LCDOutput() {
  if (abs(millis() - DisplayTimer) > DisplayInvterval)
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
      lcd.setCursor(15, 0);
      lcd.print(LastWaterLevel);

      lcd.setCursor(0, 1);
      lcd.print("LPG");
      lcd.setCursor(15, 1);
      lcd.print(LastLPGLevel);

      lcd.setCursor(0, 2);
      lcd.print("Grey Water");
      lcd.setCursor(15, 2);
      lcd.print(LastGreyWater);

      lcd.setCursor(0, 3);
      lcd.print("Sewage");
      lcd.setCursor(5, 15);
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
      lcd.print("Generator Fuel");
      lcd.setCursor(15, 0);
      lcd.print(LastGenFuel);

      lcd.setCursor(0, 1);
      lcd.print("Enclosure");
      lcd.setCursor(15, 1);
      lcd.print(LastGenEnclosureTemp, 1);

      lcd.setCursor(0, 2);
      lcd.print("Head Right");
      lcd.setCursor(15, 2);
      lcd.print(LastGenHeadRightTemp, 1);

      lcd.setCursor(0, 3);
      lcd.print("Head Left");
      lcd.setCursor(15, 3);
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
      lcd.setCursor(6, 1);
      lcd.print(LastFrontACTemp, 1);

      lcd.setCursor(0, 2);
      lcd.print("Fridge");
      lcd.setCursor(6, 2);
      lcd.print(LastFridgeTemp, 1);

      lcd.setCursor(0, 3);
      lcd.print("Freezer");
      lcd.setCursor(15, 3);
      lcd.print(LastFreezerTemp, 1);
      break;
    case 5:
      lcd.setCursor(0, 0);
      lcd.print("Watts");
      lcd.setCursor(15, 0);
      lcd.print(LastACWatts, 1);
      //Amps
      lcd.setCursor(0, 1);
      lcd.print("AC Amps");
      lcd.setCursor(15, 1);
      lcd.print(LastACCurrent);
      //Voltage
      lcd.setCursor(0, 2);
      lcd.print("AC Voltage");
      lcd.setCursor(15, 2);
      lcd.print(LastACVoltage);

      lcd.setCursor(0, 3);
      lcd.print("PF");
      lcd.setCursor(15, 3);
      lcd.print(LastPowerFactor, 0);
      break;
  }
}
//------------------------------------------------------------------
//Water Control
//------------------------------------------------------------------
void WaterControl() {
  //Read Current Water Source Selection
  LastSourceForCheck = WaterSourseSelection;
  if (digitalRead(WaterSourceSelectionInput) == HIGH) {
    WaterSourseSelection = true;//city water
    LastSource = "City";
  }
  else {
    WaterSourseSelection = false; //Tank
    LastSource = "Tank";
  }

  if (WaterOn == true && (LastSourceForCheck != WaterSourseSelection)) {
    TurnOffWater();
    delay(1000);
    TurnOnWater();
    if (StreamingData == true) {
      GetWaterSource();
    }
  }

  //if the water is on and the timer says it's been on for more than 2.5 mins turn off.
  if (WaterOn == true && (abs(millis() - WaterTimer) > 150000)) {
    TurnOffWater();
  }
  //Check to see if any of the buttons are pressed
  int KicthenButtonState = digitalRead(KitchWaterButton);
  int BathroomButtonState = digitalRead(BathroomWaterButton);
  if (KicthenButtonState == HIGH || BathroomButtonState == HIGH) {
    TurnOnWater();
    LastTimeWaterWasTurnedOn = millis();
  }

  //Check the States of pump and or logical state and set the LEDs accordingly
  WaterLEDState();
}

void TurnOnWater() {
  if (WaterOn == false && HoldingTankAlarm == false) {
    WaterOn = true;
    WaterTimer = millis();
    if (WaterSourseSelection == true) {
      digitalWrite(CityWaterValve, HIGH);
    }
    else {
      digitalWrite(WaterPumpOut, HIGH);
    }
  }
}

void TurnOffWater() {
  digitalWrite(CityWaterValve, LOW);
  digitalWrite(WaterPumpOut, LOW);
  digitalWrite(KitchenWaterButtonLED, LOW);
  digitalWrite(BathroomWaterButtonLED, LOW);
  WaterOn = false;
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
    ControlComPort.println(GetCurrentTime() + ",Error,Not receiving data from energy meter");
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

    /*
       If your Panel has L1 & L2 with a neutral. Then uncomment thes following two lines.
    */
    //LastACCurrent =  CT1 + CT2;
    //LastACWatts = (voltageA * CT1) + (voltageC * CT2);

    /*
       If your Panel has only L1 with a neutral. Then uncomment thes following three lines. '
       CT2 can be used to monitor a particular current source if you want to as it is not
       used in this set up.
    */
    LastACCurrent =  CT1;
    LastACWatts = (voltageA * CT1);
    LastPowerFactor = eic.GetTotalPowerFactor();


    // Standard Values to be read and streamed out.
    LastFreq = eic.GetFrequency();
    LastACReactive = eic.GetTotalReactivePower();
    LastACApparent = eic.GetTotalApparentPower();
    LastACFundimental = eic.GetTotalActiveFundPower();
    LastACHarmonic = eic.GetTotalActiveHarPower();
    LastACRealPower =  eic.GetTotalActivePower();

    if (Units == "I") {
      LastATM90E32Temp = ConvertCtoF(eic.GetTemperature());
    }
    else {
      LastATM90E32Temp = eic.GetTemperature();
    }
  }
}

void GeneratorSensors() {
  int Samples = 50;
  long FuelPressureSum = 0;

  for (int x = 0; x < Samples; x++) {
    FuelPressureSum = FuelPressureSum + analogRead(GenFuelPressure);
  }
  //Sensors i'm using drop out at .5 volts and anything below that will show neg pressure. DN 102 translates to 0.5V
  if ((FuelPressureSum / Samples) > 102) {
    if (Units == "I") {
      LastGenFuel = (7.5 * ConversionFactor * (FuelPressureSum / Samples)) - 3.75;
    }
    else {
      LastGenFuel = ConvertPSItoKPa((7.5 * ConversionFactor * (FuelPressureSum / Samples)) - 3.75);
    }
  }
  else {
    LastGenFuel = 0.0;
  }


  uint16_t rtd0 = GenHeadR.readRTD();
  float ratio0 = rtd0;
  ratio0 /= 32768;
  if (Units == "I") {
    LastGenHeadRightTemp = ConvertCtoF(GenHeadR.temperature(RNOMINAL, RREF));
  }
  else {
    LastGenHeadRightTemp = GenHeadR.temperature(RNOMINAL, RREF);
  }

  uint16_t rtd1 = GenHeadL.readRTD();
  float ratio1 = rtd1;
  ratio1 /= 32768;
  if (Units == "I") {
    LastGenHeadLeftTemp = ConvertCtoF(GenHeadL.temperature(RNOMINAL, RREF));
  }
  else {
    LastGenHeadLeftTemp = GenHeadL.temperature(RNOMINAL, RREF);
  }

  uint16_t rtd2 = GenEnclosure.readRTD();
  float ratio2 = rtd2;
  ratio2 /= 32768;
  if (Units == "I") {
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
    ShitterTankTimer = millis();
  }

  if ((millis() - GreyTankTimer) > 900000 || GreyGettinFull == true || WaterOn == true) {
    ReadGreyTank();
    GreyTankTimer = millis();
  }

  if (WaterOn == true) {
    /*Post every 10 Seconds if the water is on repurpose the Grey Timer but update
       both Sewage and Grey Timers so that when the water is back off they will
       update on their correct interval
    */
    if ((millis() - GreyTankTimer) > 10000) {
      GetGreyLevel();
      GetSewageLevel();
      GreyTankTimer = millis();
      ShitterTankTimer = millis();
    }
  }

  if (ShittersGettinFull == true || GreyGettinFull == true) {
    //Set Warning State
    AddWarningToList(2);
    // Also put in a check for FUll State on either and turn off pump or city water
    if (LastSewageLevel == "Full" || LastGreyWater == "Full") {
      HoldingTankAlarm = true;
      TurnOffWater();
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
  digitalWrite(SewagePower, HIGH);
  delay(2000); // Have to put this in here for the Relay to completely close
  byte TankStatus;
  bitWrite(TankStatus, 0, digitalRead(S14)); //quater
  bitWrite(TankStatus, 1, digitalRead(S12)); //Half
  bitWrite(TankStatus, 2, digitalRead(S34)); //Three Quater
  bitWrite(TankStatus, 3, digitalRead(S44)); //Full

  switch (TankStatus) {
    case 0:
      LastSewageLevel = "Empty";
      ShittersGettinFull = false;
      break;
    case 1:
      LastSewageLevel = "1/4";
      ShittersGettinFull = false;
      break;
    case 3:
      LastSewageLevel = "1/2";
      ShittersGettinFull = false;
      break;
    case 7:
      LastSewageLevel = "3/4";
      ShittersGettinFull = true;
      break;
    case 15:
      LastSewageLevel = "Full";
      ShittersGettinFull = true;
      break;
    default:
      LastGreyWater = "ERROR Check Tank:" + String(TankStatus, BIN);
      AddWarningToList(3);
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
  digitalWrite(GreyWaterPower, HIGH);
  delay(2000); // Have to put this in here for the Relay to completely close
  byte TankStatus;
  bitWrite(TankStatus, 0, digitalRead(G14)); //quater
  bitWrite(TankStatus, 1, digitalRead(G12)); //Half
  bitWrite(TankStatus, 2, digitalRead(G34)); //Three Quater
  bitWrite(TankStatus, 3, digitalRead(G44)); //Full

  switch (TankStatus) {
    case 0:
      LastGreyWater = "Empty";
      GreyGettinFull = false;
      break;
    case 1:
      LastGreyWater = "1/4";
      GreyGettinFull = false;
      break;
    case 3:
      LastGreyWater = "1/2";
      GreyGettinFull = false;
      break;
    case 7:
      LastGreyWater = "3/4";
      GreyGettinFull = true;
      break;
    case 15:
      LastGreyWater = "Full";
      GreyGettinFull = true;
      break;
    default:
      LastGreyWater = "ERROR Check Tank:" + String(TankStatus, BIN);
      AddWarningToList(4);
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
     that is mapped to 100%. confusing yes. but it's how it works. anythign over the 80% mark will read as
     a greater percentage than 100

     The Water tank level was determined from setting the tank sensor at 1/4 1/2 3/4 and full. Anything above
     the full mark shows up as "EXTRA FULL"
  */

  // Turn On votlage to tanks
  digitalWrite(TankPowerRelay, HIGH);
  delay(1000);
  int LPGResistence = 47 * (1 / ((5 / (ConversionFactor * analogRead(LPGSensor))) - 1));
  if (LPGResistence > 122) {
    LastLPGLevel = "ERROR Check Tank Sensor";
  }
  else {
    LastLPGLevel = map(LPGResistence, 0, 90, 0, 100);
  }

  LastWaterLevel = "EXTRA FULL";
  LastTimeWaterLevel = GetCurrentTime();
  float R1 = 47 * ((5.0 / (ConversionFactor * analogRead(WaterTankSensor))) - 1);
  if (R1 > 40)
  {
    LastWaterLevel = "Full";
    if (R1 > 60)
    {
      LastWaterLevel = "3/4";
      if (R1 > 80)
      {
        LastWaterLevel = "1/2";
        if (R1 > 100)
        {
          LastWaterLevel = "1/4";
          if (R1 > 150)
          {
            LastWaterLevel = "Empty";
          }
        }
      }
    }
  }
  // Turn Off Voltage to tanks
  digitalWrite(TankPowerRelay, LOW);
}
//------------------------------------------------------------------
//Other sensors
//------------------------------------------------------------------
void ReadBatteryVoltages() {
  int Samples = 50;
  float DCVoltageSum, RTCVoltageSum = 0;

  for (int x = 0; x < Samples; x++) {
    DCVoltageSum = DCVoltageSum + analogRead(Camper12VoltSensor);
    RTCVoltageSum = RTCVoltageSum + analogRead(RTCBattery);
  }
  LastDCVoltage = 3.8 * ConversionFactor * (DCVoltageSum / Samples) - 1.2;
  LastTimeDCVoltage = GetCurrentTime();

  LastRTCVoltage = ConversionFactor * (RTCVoltageSum / Samples);
  LastTimeRTCVoltage = LastTimeDCVoltage;

  if (LastDCVoltage < 10.5) {
    AddWarningToList(5);
  }

  if (LastRTCVoltage < 2.3) {
    AddWarningToList(1);
  }

}

float NTCReadInC(int R2, float ResistenceRead) {
  /*
     Using the Resistence that is calced from an ADC read, a known calibrated resistence
     value, and https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation to get the
     tempetature from these values.
  */
  float c1 = 1.009249522e-03;
  float c2 = 2.378405444e-04;
  float c3 = 2.019202697e-07;
  float C = (1.0 / (c1 + c2 * ResistenceRead + c3 * ResistenceRead * ResistenceRead * ResistenceRead)) - 273.15;
  return C;
}

void ReadOtherTempSensors() {
  int R2 = 10000;

  float VoutACF = ConversionFactor * analogRead(FrontACTemp);
  float R1ACF = log(R2 * ((5.0 / VoutACF) - 1));
  if (Units == "I") {
    LastFrontACTemp = ConvertCtoF(NTCReadInC(R2, R1ACF));
  }
  else {
    LastFrontACTemp = NTCReadInC(R2, R1ACF);
  }

  float VoutACB = ConversionFactor * analogRead(BackACTemp);
  float R1ACB = log(R2 * ((5.0 / VoutACB) - 1));
  if (Units == "I") {
    LastBackACTemp = ConvertCtoF(NTCReadInC(R2, R1ACB));
  }
  else {
    LastBackACTemp =  NTCReadInC(R2, R1ACB);
  }

  float VoutHallway = ConversionFactor * analogRead(HallwayTemp);
  float R1Hallway = log(R2 * ((5.0 / VoutHallway) - 1));
  if (Units == "I") {
    LastHallwayTemp = ConvertCtoF( NTCReadInC(R2, R1Hallway));
  }
  else {
    LastHallwayTemp =  NTCReadInC(R2, R1Hallway);
  }

  float VoutBathroom = ConversionFactor * analogRead(BathroomTemp);
  float R1Bathroom = log(R2 * ((5.0 / VoutBathroom) - 1));
  if (Units == "I") {
    LastBathroomTemp = ConvertCtoF(NTCReadInC(R2, R1Bathroom));
  }
  else {
    LastBathroomTemp =  NTCReadInC(R2, R1Bathroom);
  }

  float VoutFreezer = ConversionFactor * analogRead(Freezer);
  float R1Freezer = log(R2 * ((5.0 / VoutFreezer) - 1));
  if (Units == "I") {
    LastFreezerTemp = ConvertCtoF(NTCReadInC(R2, R1Freezer));
  }
  else {
    LastFreezerTemp =  NTCReadInC(R2, R1Freezer);
  }

  float VoutFridge = ConversionFactor * analogRead(Refridgerator);
  float R1Fridge = log(R2 * ((5.0 / VoutFridge) - 1));
  if (Units == "I") {
    LastFridgeTemp = ConvertCtoF(NTCReadInC(R2, R1Fridge));
  }
  else {
    LastFridgeTemp =  NTCReadInC(R2, R1Fridge);
  }

  float VoutOutside = ConversionFactor * analogRead(Outside);
  float R1Outside = log(R2 * ((5.0 / VoutOutside) - 1));
  if (Units == "I") {
    LastOutsideTemp = ConvertCtoF(NTCReadInC(R2, R1Outside));
  }
  else {
    LastOutsideTemp =  NTCReadInC(R2, R1Outside);
  }

  float VoutBackCabin = ConversionFactor * analogRead(BackCabin);
  float R1BackCabin = log(R2 * ((5.0 / VoutBackCabin) - 1));
  if (Units == "I") {
    LastBackCabinTemp = ConvertCtoF(NTCReadInC(R2, R1BackCabin));
  }
  else {
    LastBackCabinTemp =  NTCReadInC(R2, R1BackCabin);
  }

  LastTimeNTCTemp = GetCurrentTime();
}
//------------------------------------------------------------------
//Helper functions
//------------------------------------------------------------------
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
  String DateTimeString = String(now.year())
                          + "/" + String(now.month())
                          + "/" + String(now.day())
                          + "-" + String(now.hour())
                          + ":" + String(now.minute())
                          + ":" + String(now.second());

  LastTimeRTCTemp = DateTimeString;
  if (Units == "I") {
    LastRTCTemp = ConvertCtoF(rtc.getTemperature());
  }
  else {
    LastRTCTemp = rtc.getTemperature();
  }
  return DateTimeString;
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

void ReadAllInputs() {
  for (int i = 1; i <= SpareInputSize; i++) {
    int CurrentInputRead = ReadInput(SpareInputs[i]);
    if (CurrentInputRead != LastInputState[i]) {
      LastInputState[i] = CurrentInputRead;
      if (StreamingData == true) {
        PrintInputState(i, CurrentInputRead);
      }
    }
  }
}

int ReadInput(int Number) {
  int Value = digitalRead(SpareInputs[Number]);
  return Value;
}

int ReadOutput(int Number) {
  Serial.println(SpareOutputs[Number - 1]);
  int Value = digitalRead(SpareOutputs[Number]);
  return Value;
}

void ForceCompleteUpdateOfAllStates() {
  ReadGreyTank();
  ReadSewageTank();
  ReadOtherTempSensors();
  ReadBatteryVoltages();
  ReadWaterAndLPG();
  GeneratorSensors();
  ReadAllInputs();
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
  while (ControlComPort.available()) {
    // get the new byte:
    char inChar = (char)ControlComPort.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a carriage return, set a flag so the main loop can
    // do something about it:
    if (inChar == '\r') {
      stringComplete = true;
    }
  }
}

void OutputAllData() {
  GetDeviceInfo();
  GetStreamingState();
  GetWaterPumpSense();
  GetACEnmon();
  GetWaterState();
  GetUnits();
  GetWaterSource();
  GetWaterLevel();
  GetSewageLevel();
  GetGreyLevel();
  GetLPGLevel();
  GetDCBatteryVoltage();
  GetRTCBatteryVotlage();
  GetNTCTemps();
  GetHeadUnitTemp();
  GetGenStatus();
  GetEnergyStatus();
}

void GetWaterSource() {
  ControlComPort.println("%R," + GetCurrentTime() + ",Water Source," + LastSource);
}

void GetWaterLevel() {
  ControlComPort.println("%R," + LastTimeWaterLevel + ",Water Tank Level," + LastWaterLevel);
}

void GetSewageLevel() {
  ControlComPort.println("%R," + LastTimeSewageLevel + ",Sewage," + LastSewageLevel);
}

void GetGreyLevel() {
  ControlComPort.println("%R," + LastTimeGreyWater + ",Grey," + LastGreyWater);
}

void GetLPGLevel() {
  ControlComPort.println("%R," + LastTimeWaterLevel   + ",LPG," + LastLPGLevel);
}

void GetDCBatteryVoltage() {
  ControlComPort.println("%R," + LastTimeDCVoltage + ",Camper VDC," + LastDCVoltage);
}

void GetRTCBatteryVotlage() {
  ControlComPort.println("%R," + LastTimeRTCVoltage + ",RTCBattery," + LastRTCVoltage);
}

void GetNTCTemps() {
  ControlComPort.println("%R," + LastTimeNTCTemp + ",NTC Tempetatures,Units," + TempUnits + ",Front AC Temp,"
                         + LastFrontACTemp + ",Back AC Temp," + LastBackACTemp + ",Under Awning Temp," + LastOutsideTemp + ",Back Cabin Temp,"
                         + LastBackCabinTemp + ",Hallway Temp," + LastHallwayTemp + ",Freezer," + LastFreezerTemp + ",Fridge,"
                         + LastFridgeTemp + ",Bathroom Temp," + LastBathroomTemp);
}

void GetHeadUnitTemp() {
  ControlComPort.println("%R," + LastTimeRTCTemp + ",Head Unit Temp,Units," + TempUnits + "," + LastRTCTemp);
}

void GetGenStatus() {
  ControlComPort.println("%R," + LastTimeGenSensors + ",Generator Fuel Pressure,Units," + PressureUnits + ","
                         + LastGenFuel + ",Generator Temps, Enclosure,Units," + TempUnits + "," + LastGenEnclosureTemp +
                         ",Right Head Temp," + LastGenHeadRightTemp + ",Left Head Temp," + LastGenHeadLeftTemp);
}

void GetEnergyStatus() {
  ControlComPort.println("%R," + LastTimeACVoltage + ",Energy Monitor," + LastACVoltage + ",V," + LastACCurrent +
                         ",A,"  + LastPowerFactor + ",PF," + LastACRealPower + ",W{real)," + LastFreq + ",Hz," + LastACWatts + ",W(total),"
                         + LastACReactive + ",var(reactive),"  + LastACApparent + ",VA(apparent)," + LastACFundimental + ",W(fundimental),"
                         + LastACHarmonic + ",W(harmonic)");
}

void GetUnits() {
  ControlComPort.println("%R,Units," + Units);
}

void GetDeviceInfo() {
  ControlComPort.println("%R," + DeviceName + ",FW," + FWVersion);
}

void GetStreamingState() {
  String State = "Off";
  if (StreamingData == true) {
    State = "On";
  }
  ControlComPort.println("%R,StreamingData," + State);
}

void GetWaterPumpSense() {
  String State = "Off";
  if (UseWaterPumpSense == true) {
    State = "On";
  }
  ControlComPort.println("%R,WaterPumpSense," + State);
}

void GetWaterState() {
  String State = "Off";
  if (WaterOn == true) {
    State = "On";
  }
  ControlComPort.println("%R,Water," + State);
}

void GetACEnmon() {
  String State = "Off";
  if (EnableACEnergyMonitoring == true) {
    State = "On";
  }
  ControlComPort.println("%R,AC Energy Monitoring," + State);
}
//------------------------------------------------------------------
//Alarm and Warnings
//------------------------------------------------------------------
void Error(int Number) {
  const char* Errors[] = {"command not recognized", "command Parameter out of range", "command not supported on this platform", "command can not be processed", "Invalid Parameter", "Invalid Command Format"};
  ControlComPort.println("%R,Error," + String(Errors[Number]));
}

void Warning() {
  if (WarningActive == true) {
    if (abs(millis() - WarningBlinkTimer) >  333) {
      WarningBlinkTimer = millis();
      if (digitalRead(WarningLED) == LOW) {
        digitalWrite(WarningLED, HIGH);
      } else {
        digitalWrite(WarningLED, LOW);
      }
    }
  }
}

void AddWarningToList(int WarningID) {
  WarningActive = true;
  ArrayOfWarnings[WarningID] = WarningID;
  OutputWarningMessage(WarningID);
}

void OutputWarningMessage(int ID) {
  if (ArrayOfWarnings[ID] != 0) {
    String Message = "";
    switch (ID) {
      case 1:
        Message = "RTC Battery voltage is low ";
        break;
      case 2:
        Message = "One of the holding tanks is getting full";
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
    }
    ControlComPort.println("%R," + GetCurrentTime() + ",Warning," + Message);
  }
}

void AllWarningMessages() {
  for (int i = 1; i <= 5; i++) {
    OutputWarningMessage(i);
  }
}

void ALARM() {
  digitalWrite(AlarmOut, HIGH);
}

void ResetAlarm() {
  digitalWrite(AlarmOut, LOW);
}

void ResetWarnings() {
  ArrayOfWarnings[5] = {};
  WarningActive = false;
  digitalWrite(WarningLED, LOW);
}

void ResetAllAlarmsAndWarnings() {
  HoldingTankAlarm = false;
  ResetAlarm();
  ResetWarnings();
}
//------------------------------------------------------------------
//Commands
//------------------------------------------------------------------
void SetUnits(String Value) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End - 1);
  if (ThingToTest.length() == 1) {
    bool CorrectParam = false;
    if (ThingToTest == "I") {
      Units = "I";
      CorrectParam = true;
    }
    if (ThingToTest == "M") {
      Units = "M";
      CorrectParam = true;
    }
    if (CorrectParam == true) {
      GetUnits();
    }
    else {
      Error(4);
    }
  }
  else {
    Error(1);
  }
  SetUnitsForOutput();
}

void SetWaterPumpSenseOverRide(String Value) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End - 1);
  bool CorrectParam = false;
  if (ThingToTest == "OFF") {
    UseWaterPumpSense = false;
    CorrectParam = true;
  }
  if (ThingToTest == "ON") {
    UseWaterPumpSense = true;
    CorrectParam = true;
  }

  if (CorrectParam == true) {
    GetWaterPumpSense();
  }
  else {
    Error(4);
  }
}

void SetWater(String Value) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End - 1);
  bool CorrectParam = false;
  if (ThingToTest == "OFF") {
    TurnOffWater();
    CorrectParam = true;
  }
  if (ThingToTest == "ON") {
    TurnOnWater();
    CorrectParam = true;
  }

  if (CorrectParam == true) {
    GetWaterState();
  }
  else {
    Error(4);
  }
}

void SetStreamingData(String Value) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String ThingToTest = Value.substring(Index + 1, End - 1);
  bool CorrectParam = false;
  if (ThingToTest == "OFF") {
    StreamingData = false;
    CorrectParam = true;
  }
  if (ThingToTest == "ON") {
    StreamingData = true;
    CorrectParam = true;
  }

  if (CorrectParam == true) {
    GetStreamingState();
  }
  else {
    Error(4);
  }
}

void SetOutputState(String Value) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  String Start = Value.substring(Index + 1, End - 1);
  int SecondParam = Start.indexOf("*");
  int Number = Start.substring(0, SecondParam).toInt();
  String State = Start.substring(SecondParam + 1);
  if (Number > 0 && Number <= (SpareOutputSize)) {
    bool CorrectParam = false;
    if (State == "OFF") {
      digitalWrite(SpareOutputs[Number], LOW);
      CorrectParam = true;
    }
    if (State == "ON") {
      digitalWrite(SpareOutputs[Number], HIGH);
      CorrectParam = true;
    }
    if (CorrectParam == true) {
      PrinOutputState(Number);
    }
    else {
      Error(4);
    }
  }
  else {
    Error(4);
  }
}

void GetOutputState(String Value) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  int ThingToTest = (Value.substring(Index + 1, End - 1).toInt());
  if (ThingToTest > 0 && ThingToTest <= (SpareOutputSize)) {
    PrinOutputState(ThingToTest);
  }
  else {
    Error(4);
  }
}

void PrinOutputState(int Output) {
  int CurrentState = ReadOutput(Output);
  String State = "Off";
  if (CurrentState == 1) {
    State = "On";
  }
  ControlComPort.println("%R," + GetCurrentTime() + ",Output," + Output + "," + State);
}


void ReadInputState(String Value) {
  int Index = Value.indexOf("*");
  int End = Value.indexOf("\r");
  int ThingToTest = (Value.substring(Index + 1, End - 1).toInt());
  if (ThingToTest > 0 && ThingToTest <= (SpareInputSize)) {
    int CurrentInputRead = ReadInput(ThingToTest);
    PrintInputState(ThingToTest, CurrentInputRead);
  }
  else {
    Error(4);
  }
}

void PrintInputState(int Input, int CurrentInputRead) {
  String State = "Off";
  if (CurrentInputRead == 1) {
    State = "On";
  }
  ControlComPort.println("%R," + GetCurrentTime() + ",Input," + Input + "," + State);
}

void SetRTCDateTime(String Value) {

}

void SetACEnmon(String Value) {
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
    GetACEnmon();
  }
  else {
    Error(4);
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
*/

String PainlessInstructionSet(String & TestString) {
  int Search = 1;
  while (Search == 1) {
    int FindStart = TestString.indexOf('%');
    int Param = TestString.indexOf('*');
    int FindEnd = TestString.indexOf('\r');
    if (TestString != "") {
      if (FindStart != -1) { //case 1
        if (FindStart != 0) { //case 2
          //Serial.println("PIS Case 2");
          ControlComPort.println("%R," + GetCurrentTime() + ",Error,BAD Command Format No Start or Stop Delimiters");
          TestString.remove(0, FindStart);
        }
        else { //Case 3 & Case 5 & Case 4
          String Case5Test = TestString.substring(FindStart + 1);
          int FindStart1 = Case5Test.indexOf('%');
          int FindEnd1 = Case5Test.indexOf('\r');
          if ((FindEnd1 > FindStart1) && (FindStart1 != -1)) {
            ControlComPort.println("%R," + GetCurrentTime() + ",Error,BAD Command Format - No End Delimiter");
            //Serial.println("PIS Case 5");
            TestString.remove(0, FindStart1 + 1);
          }
          else {
            if (FindEnd != -1 || FindEnd1 != -1) {
              //Serial.println("PIS Case 3");
              String CommandCandidate = TestString.substring(FindStart + 1, FindEnd);
              CommandCandidate.toUpperCase();
              if ((Param < FindEnd) && Param != -1) {
                //Serial.println("getting here1");
                for (int i = 0; i < (sizeof(ParameterCommands) / sizeof(int)); i++)
                {
                  //Serial.println("PIS Case 3A");
                  String ParamHeader = CommandCandidate.substring(FindStart, Param - 1);
                  ParamHeader.toUpperCase();
                  if (ParamHeader == ParameterCommands[i]) {
                    ParamCommandToCall(i, CommandCandidate);
                  }
                }
              }
              else {
                for (int i = 0; i < (sizeof(AcceptedCommands) / sizeof(int)); i++)
                {
                  //Non Parameter Commands
                  if (CommandCandidate == AcceptedCommands[i]) {
                    //Serial.println("PIS Case 3B");
                    CommandToCall(i);
                  }
                }
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
      }
    }//if TestString is empty
    else { //Exit Search While if Buffer is empty
      Search = 0;
      //Serial.println("PIS Case 6");
    }
  }//End of Search While
  return TestString;
}//End of PIS Function


void ParamCommandToCall(int Index, String CommandRaw) {
  //Serial.print("Param Command to call:");
  //Serial.println(Index);
  switch (Index)
  {
    case 0:
      //Set Units
      SetUnits(CommandRaw);
      break;
    case 1:
      //Set Water Pump Sense
      SetWaterPumpSenseOverRide(CommandRaw);
      break;
    case 2:
      //Water
      SetWater(CommandRaw);
      break;
    case 3:
      //Set Streaming Data Output
      SetStreamingData(CommandRaw);
      break;
    case 4:
      //Set Output State
      SetOutputState(CommandRaw);
      break;
    case 5:
      //Read Input State
      ReadInputState(CommandRaw);
      break;
    case 6:
      //Set RTC Datetime
      SetRTCDateTime(CommandRaw);
      break;
    case 7:
      //Read OutputSate
      GetOutputState(CommandRaw);
      break;
    case 8:
      //Enable/disable AC energy Monitoring
      SetACEnmon(CommandRaw);
      break;
  }
}

void CommandToCall(int Index) {
  //Serial.print("Command to call:");
  //Serial.println(Index);
  switch (Index)
  {
    case 0:
      //UNITS
      GetUnits();
      break;
    case 1:
      //DEVICE INFORMATION
      GetDeviceInfo();
      break;
    case 2:
      //WATERSOURCE
      GetWaterSource();
      break;
    case 3:
      //WATERLEVEL
      GetWaterLevel();
      break;
    case 4:
      //LPG LEVEL
      GetLPGLevel();
      break;
    case 5:
      //SEWAGE LEVEL
      GetSewageLevel();
      break;
    case 6:
      //GREY LEVEL
      GetGreyLevel();
      break;
    case 7:
      //ENERGY MONITOR
      GetEnergyStatus();
      break;
    case 8:
      //CAMPER DV VOLTAGE
      GetDCBatteryVoltage();
      break;
    case 9:
      //RTC BATTERY VOLTAGE
      GetRTCBatteryVotlage();
      break;
    case 10:
      //GENERATOR STATUS
      GetGenStatus();
      break;
    case 11:
      //NTC TEMPS
      GetNTCTemps();
      break;
    case 12:
      //HEAD UNIT TEMP
      GetHeadUnitTemp();
      break;
    case 13:
      //WATERPUMPSENSE
      GetWaterPumpSense();
      break;
    case 14:
      //WARNING
      if (WarningActive == true) {
        AllWarningMessages();
      }
      else {
        ControlComPort.println("%R," + GetCurrentTime() + ",Warning,None");
      }
      break;
    case 15:
      //WATER
      GetWaterState();
      break;
    case 16:
      //STREAMING
      GetStreamingState();
      break;
    case 17:
      //AC energy monitoring
      GetACEnmon();
      break;
    case 18:
      //Get All Data at one go
      OutputAllData();
      break;
    case 19:
      //Force Update of all states
      ForceCompleteUpdateOfAllStates();
      ControlComPort.println("%R," + GetCurrentTime() + ",All States Updated");
      break;
    case 20:
      //Reset Warnings
      ResetWarnings();
      ControlComPort.println("%R," + GetCurrentTime() + ",Reset Warnings");
      break;
    case 21:
      //Reset All alarms and warnings
      ResetAllAlarmsAndWarnings();
      ControlComPort.println("%R," + GetCurrentTime() + ",Reset All Alarms and Warnings");
      break;
  }
}
