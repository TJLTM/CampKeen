#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_MAX31865.h>
#include <SPI.h>
#include <RTClib.h>
#include <EEPROM.h>
#include <ATM90E32.h>

#define ControlComPort Serial
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
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
const String FWVersion = "0.5.4";
const String HWVersion = "0.5";
const int DisplayInvterval = 3000;
const float ConversionFactor = 5.0 / 1023;
int WarningState = 0; //
long WaterTimer, ShitterTankTimer, GreyTankTimer, WATERLPGtimer, FiveMinTimer, DisplayTimer, NTCTimer, EnergyTimer, OutputTimer, HoldingTankTimer, LastTimeWaterWasTurnedOn, WarningBlinkTimer;
String Units = "I"; //I = Imperial M = Metric
bool StreamingData = true;
bool LCDSetup = false;
//WaterSourceSelection //false = pump //true = City Water
bool WaterSourseSelection = false, WaterOn = false;
bool UseWaterPumpSense = false;
String LastSourceForCheck = "";
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
float LastHeadUnitTemp = 0.0;
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
//SparePins and ports that are wired on board
#define SPADC2 15
#define SPIn2 9
#define SPIn3 8
#define SPIn4 7
#define SPIn5 6
#define SPO3 12
#define SPO4 11
#define SPO5 24
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
#define SewagePowerRelay 30

//GreyTankLevelPins
#define G14 40
#define G12 38
#define G34 36
#define G44 34
#define GreyWaterPowerRelay 32
//-----------------------------------------------------------
void setup() {
  ControlComPort.begin(115200);
  inputString.reserve(200);

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
  pinMode(SewagePowerRelay, OUTPUT);
  pinMode(S14, INPUT);
  pinMode(S12, INPUT);
  pinMode(S34, INPUT);
  pinMode(S44, INPUT);

  //Grey Water
  pinMode(GreyWaterPowerRelay, OUTPUT);
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
  ControlComPort.println("HW: " + HWVersion);
  ControlComPort.println("FW: " + FWVersion);
  digitalWrite(LCDPowerOut, HIGH);
  delay(250);
  SetupLCD();
  lcd.setCursor(0, 0);
  lcd.print("Starting " + DeviceName);
  //Run through the sensors and get values for everything
  ReadGreyTank();
  ReadSewageTank();
  ReadOtherTempSensors();
  ReadBatteryVoltages();
  ReadWaterAndLPG();
  GeneratorSensors();
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
  lcd.setCursor(0, 3);
  lcd.print(HWVersion);
  delay(1000);
  digitalWrite(LCDPowerOut, digitalRead(LCDEnable));

}

void loop() {
  if (digitalRead(AlarmReset) == HIGH) {
    ResetAllAlarmsAndWarnings();
  }

  WaterControl();
  HoldingTankMonitoring();

  if (abs(millis() - NTCTimer) > 3000) {
    //Read NTC temp Sensors
    ReadOtherTempSensors();
    //Read Generator Sensors
    GeneratorSensors();
    NTCTimer = millis();
  }

  if (abs(millis() - EnergyTimer) > 3000) {
    //Read Energy
    EnergyMetering();
    EnergyTimer = millis();
  }

  if (abs(millis() - WATERLPGtimer) > 1800000) {
    //ReadLPG and Water Tank at 1/2 hour intervals
    ReadWaterAndLPG();
    WATERLPGtimer = millis();
  }

  if (abs(millis() - FiveMinTimer) > 300000) {
    //Read DC voltage at 5 min intervals
    ReadBatteryVoltages();
    FiveMinTimer = millis();
  }

  //After all the sensors have been updated stream out the data if the flag is set to do so
  if (StreamingData == true) {
    if (abs(millis() - OutputTimer) > 5000) {
      OutputAllData();
      OutputTimer = millis();
    }
  }

  //  if (digitalRead(LCDEnable) == HIGH) {
  //    digitalWrite(LCDPowerOut, HIGH);
  //    if (LCDSetup == false) {
  //      delay(250);
  //      SetupLCD();
  //    }
  //    LCDOutput();
  //    LCDDisplay();
  //  }
  //  else {
  //    LCDSetup = false;
  //    digitalWrite(LCDPowerOut, LOW);
  //  }

  Warning();
}

//------------------------------------------------------------------
//LCD
//------------------------------------------------------------------
void SetupLCD() {
  lcd.init();
  lcd.backlight();
  rtc.begin();
  LCDSetup = true;
}

void LCDOutput() {
  if (abs(millis() - DisplayTimer) > DisplayInvterval)
  {
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
  lcd.clear();
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
  if (digitalRead(WaterSourceSelectionInput) == HIGH) {
    WaterSourseSelection = true;//city water
    LastSource = "City";
  }
  else {
    WaterSourseSelection = false; //Tank
    LastSource = "Tank";

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
    LastHeadUnitTemp = 0;
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
      LastHeadUnitTemp = ConvertCtoF(eic.GetTemperature());
    }
    else {
      LastHeadUnitTemp = eic.GetTemperature();
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

  if (ShittersGettinFull == true || GreyGettinFull == true) {
    //Set Warning State
    WarningState = 2;
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
  else {
    digitalWrite(WarningLED, LOW);
  }
}

void ReadSewageTank() {
  // Turn On votlage to tank
  digitalWrite(SewagePowerRelay, HIGH);
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
      LastSewageLevel = "ERROR Check Tank";
      WarningState = 3;
      break;
  }

  LastTimeSewageLevel = GetCurrentTime();


  if (ShittersGettinFull == false && WaterOn == false) {
    // Turn Off Voltage to tank
    digitalWrite(SewagePowerRelay, LOW);
  }
}

void ReadGreyTank() {
  // Turn On votlage to tank
  digitalWrite(GreyWaterPowerRelay, HIGH);
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
      LastGreyWater = "ERROR Check Tank";
      WarningState = 3;
      break;
  }
  LastTimeGreyWater = GetCurrentTime();

  if (GreyGettinFull == false && WaterOn == false) {
    // Turn Off Voltage to tank
    digitalWrite(GreyWaterPowerRelay, LOW);
  }
}

void ReadWaterAndLPG() {
  /*
   * Using a 47 1% for R2 in both circuits
   * LPG tank is as far as i can tell is a 0-122 ohm resistence and knowing that the 80% full at ~90 ohms 
   * that is mapped to 100%. confusing yes. but it's how it works. anythign over the 80% mark will read as 
   * a greater percentage than 100
   * 
   * The Water tank level was determined from setting the tank sensor at 1/4 1/2 3/4 and full. Anything above 
   * the full mark shows up as "EXTRA FULL" 
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
}

float NTCReadInC(int R2, float ResistenceRead) {
  /*
   * Using the Resistence that is calced from an ADC read, a known calibrated resistence 
   * value, and https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation to get the 
   * tempetature from these values. 
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

//------------------------------------------------------------------
//System Control and Output
//------------------------------------------------------------------
void OutputAllData() {
  ControlComPort.println(LastTimeWaterLevel   + ",Water Tank Level,"                  + LastWaterLevel);
  ControlComPort.println(GetCurrentTime()     + ",Water Source,"                      + LastSource);
  ControlComPort.println(LastTimeSewageLevel  + ",Sewage,"                            + LastSewageLevel);
  ControlComPort.println(LastTimeGreyWater    + ",Grey,"                              + LastGreyWater);
  ControlComPort.println(LastTimeWaterLevel   + ",LPG,"                               + LastLPGLevel);
  ControlComPort.println(LastTimeDCVoltage    + ",Camper VDC,"                        + LastDCVoltage);
  ControlComPort.println(LastTimeNTCTemp      + ",NTC Tempetatures,Front AC Temp,"    + LastFrontACTemp + ",Back AC Temp," + LastBackACTemp + ",Under Awning Temp," + LastOutsideTemp + ",Back Cabin Temp," + LastBackCabinTemp + ",Hallway Temp," + LastHallwayTemp + ",Freezer," + LastFreezerTemp + ",Fridge," + LastFridgeTemp + ",Bathroom Temp," + LastBathroomTemp);
  ControlComPort.println(LastTimeGenSensors   + ",Generator Fuel Pressure,"           + LastGenFuel);
  ControlComPort.println(LastTimeGenSensors   + ",Generator Temps, Enclosure,"        + LastGenEnclosureTemp + ",Right Head Temp," + LastGenHeadRightTemp + ",Left Head Temp," + LastGenHeadLeftTemp);
  ControlComPort.println(LastTimeRTCVoltage   + ",RTCBattery,"                        + LastRTCVoltage);
  ControlComPort.println(LastTimeRTCTemp      + ",Head Unit Temp," +                  + LastRTCTemp);
  ControlComPort.println(LastTimeACVoltage    + ",Energy Monitor,"                    + LastACVoltage + ",V," + LastACCurrent + ",A,"  + LastPowerFactor + ",PF," + LastACRealPower + ",W{real)," + LastFreq + ",Hz," + LastACWatts + ",W(total)," + LastACReactive + ",var(reactive),"  + LastACApparent + ",VA(apparent)," + LastACFundimental + ",W(fundimental)," + LastACHarmonic + ",W(harmonic)");
}

void Warning() {
  if (WarningState != 0) {
    if (abs(millis() - WarningBlinkTimer) > (1000 * WarningState)) {
      WarningBlinkTimer = millis();
      if (digitalRead(WarningLED) == LOW) {
        digitalWrite(WarningLED, HIGH);
      } else {
        digitalWrite(WarningLED, LOW);
      }
    }
  }
}

void ALARM() {
  digitalWrite(AlarmOut, HIGH);
}

void ResetAlarm() {
  digitalWrite(AlarmOut, LOW);
}


void ResetAllAlarmsAndWarnings() {
  HoldingTankAlarm = false;
  digitalWrite(AlarmOut, LOW);
  WarningState = 0;
  digitalWrite(WarningLED, LOW);
}

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
