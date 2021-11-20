#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_MAX31865.h>
#include <SPI.h>
#include <RTClib.h>
#include <EEPROM.h>
#include <ATM90E32.h>

#define ControlComPort Serial
//#define USBComPort Serial
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 20, 4);
int DisplayCounter = 0;
#define LCDenablePin 10
//-----------------------------------------------------------
// ATM90E32 energy monitor settings and calibrations
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

      These are only starting points i had to play
      with this number to get it close enough
*/
unsigned short VoltageGain = 4005;
/* 25498 - SCT-013-000 100A/50mA
   39473 - SCT-016 120A/40mA
   46539 - Magnalab 100A
*/
unsigned short CurrentGainCT1 = 25498;
unsigned short CurrentGainCT2 = 25498;

ATM90E32 eic{};


//-----------------------------------------------------------
//-----------------------------------------------------------
// Timers and Intervals
const int DisplayInvterval = 3000;
const float ConversionFactor = 5.0 / 1023;
long WaterTimer, ShitterTankTimer, GreyTankTimer, WATERLPGtimer, FiveMinTimer, LastMillis1, NTCTimer, LCDDisplaySwitchTimer, DataOutTimer;
//-----------------------------------------------------------
//-----------------------------------------------------------
// System Level
String Units = "I"; //Default Units I = Imperial M = Metric
bool StreamingData = true;
bool LCDSetup = false;
//WaterSourceSelection //false = pump //true = City Water
bool BathroomLEDState = false, KitchenLEDState = false, WaterSourseSelection = false, WaterOn = false;
//-----------------------------------------------------------
/*
   All the Stored Values and Times to have states
   that can be recalled if streaming is turned off
*/
String LastWaterLevel = "Empty";
String LastTimeWaterLevel = "";
String LastSewageLevel = "Empty";
String LastTimeSewageLevel = "";
String LastGreyWater = "Empty";
String LastTimeGreyWater = "";
int LastLPGLevel = 0;
String LastTimeLPGLevel = "";
float LastDCVoltage = 0.0;
String LastTimeDCVoltage = "";
float LastRTCVoltage = 0.0;
String LastTimeRTCVoltage = "";

String LastTimeNTCTemp = "";
float LastFrontACTemp = 0.0;
float LastBackACTemp = 0.0;
float LastOutsideTemp = 0.0;
float LastBackCabinTemp = 0.0;
float LastHallwayTemp = 0.0;
float LastFreezerTemp = 0.0;
float LastFridgeTemp = 0.0;
float LastBathroomTemp = 0.0;

float LastGenFuel = 0.0;
String LastTimeGenFuel = "";

String LastTimeGenTemps = "";
float LastGenEnclosureTemp = 0.0;
float LastGenHeadRightTemp = 0.0;
float LastGenHeadLeftTemp = 0.0;

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
float LastACRealPower = 0.0;
float LastHeadUnitTemp = 0.0;

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
#define AlarmLED 33
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
#define SPIn6 5
#define SPO2 13
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

  pinMode(LCDenablePin, INPUT);
  pinMode(LEDBacklightOut, OUTPUT);
  
  pinMode(AlarmOut, OUTPUT);
  pinMode(AlarmLED, OUTPUT);
  pinMode(AlarmReset, INPUT);
  
  pinMode(WaterSourceSelectionInput, INPUT);
  pinMode(WaterPumpSense, INPUT);
  pinMode(KitchWaterButton, INPUT);
  pinMode(BathroomWaterButton, INPUT);
  pinMode(TankPowerRelay, OUTPUT);
  pinMode(WaterPumpOut, OUTPUT);
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
  GenHeadR.begin(MAX31865_3WIRE); // set to 2WIRE or 4WIRE as necessary
  GenHeadL.begin(MAX31865_3WIRE); // set to 2WIRE or 4WIRE as necessary
  GenEnclosure.begin(MAX31865_3WIRE); // set to 2WIRE or 4WIRE as necessary

  //Energy Monitor
  //pinMode(EnergyMonitorCS, OUTPUT);
  //SetupEnergyMonitor();

  //Set up displays and output on the Serial Port
  ControlComPort.println("Starting system up");
  //Run through the sensors and get values for everything
  ReadGreyTank();
  ReadSewageTank();
  ReadOtherTempSensors();
  ReadBatteryVoltages();
  ReadWaterAndLPG();
  GeneratorSensors();

  ControlComPort.print("System Initialized and values populated: ");
  ControlComPort.println(GetCurrentTime());
}

void loop() {
<<<<<<< release/0.5
Serial.println("getting here");
delay(1000);
=======
  //WaterControlTesting();
  WaterControl();
  //digitalWrite(KitchenWaterButtonLED, HIGH);
}


void WaterControlTesting(){
  int KicthenButtonState = digitalRead(KitchWaterButton);
  int BathroomButtonState = digitalRead(BathroomWaterButton);

  digitalWrite(KitchenWaterButtonLED, KicthenButtonState);
  digitalWrite(BathroomWaterButtonLED, BathroomButtonState);
  
>>>>>>> local
}

void Test(){
  if (digitalRead(AlarmReset) == HIGH) {
    ResetAllAlarms();
  }

  WaterControl();

  if (abs(millis() - WATERLPGtimer) > 3.6e+6 || WaterOn == true) {
    //ReadLPG and Water Tank at 1 hour intervals unless the pump or city water valve are on.
    ReadWaterAndLPG();
    WATERLPGtimer = millis();
  }

  if (abs(millis() - NTCTimer) > 3000) {
    //Read NTC temp Sensors
    ReadOtherTempSensors();
    //Read Generator Sensors
    GeneratorSensors();
    //Read DC Voltages
    ReadBatteryVoltages();
    //Save Current Time
    NTCTimer = millis();
  }

  if (abs(millis() - FiveMinTimer) > 300000) {
    FiveMinTimer = millis();
  }

  //After all the sensors have been updated stream out the data if the flag is set to do so
  if (StreamingData == true && abs(millis() - DataOutTimer) > 3000) {
    OutputAllData();
    DataOutTimer = millis();
  }

  if (digitalRead(LCDenablePin) == HIGH) {
    if (abs(millis() - LCDDisplaySwitchTimer) > DisplayInvterval) {
      SetupLCD();
      LCDOutput();
      LCDDisplaySwitchTimer = millis();
    }
  }
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
  if (digitalRead(LCDenablePin) == HIGH) {
    if (LCDSetup == false) {
      SetupLCD();
    }

    if (abs(millis() - LastMillis1) > DisplayInvterval)
    {
      LCDDisplay();
      LastMillis1 = millis();

      if (DisplayCounter < 5) {
        DisplayCounter += 1;
      }
      else {
        DisplayCounter = 0;
      }
    }
  }
  else {
    LCDSetup = false;
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

      //lcd.setCursor(0,3);
      //lcd.print("RPM");
      //lcd.setCursor(15,3);
      //lcd.print(RPMValue,0);
      break;
    case 2:
      //Generator
      lcd.setCursor(0, 0);
      lcd.print("Generator Fuel:");
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
  }
}

//------------------------------------------------------------------
//Water Control
//------------------------------------------------------------------
void WaterControl() {
  //Read Current Water Source Selection
  //add logic that if flipped while the pump is on turn it off and open the city water valve
  //ControlComPort.print("WaterSourseSelection:");
  if (digitalRead(WaterSourceSelectionInput) == HIGH) {
    WaterSourseSelection = true;//city water
    //ControlComPort.println("City Water");
  }
  else {
    WaterSourseSelection = false; //Tank
    //ControlComPort.println("Tank");
  }

<<<<<<< release/0.5

  //if the water is on and the timer says it's been on for more than 5 mins turn off.
  if (WaterOn == true && (millis() - WaterTimer > 300000)) {
    TurnOffWater;
  }
  //Check to see if any of the buttons are pressed
  int KicthenButtonState = digitalRead(KitchWaterButton);
  int BathroomButtonState = digitalRead(BathroomWaterButton);
  //Serial.print("Kitchen Button:");
  //Serial.println(KicthenButtonState);
  digitalWrite(KitchenWaterButtonLED, KicthenButtonState);
  //Serial.print("Bathroom Button:");
  //Serial.println(BathroomButtonState);
  digitalWrite(BathroomWaterButtonLED, BathroomButtonState);

=======
  
  //if the water is on and the timer says it's been on for more than 2.5 mins turn off.
  if (WaterOn == true && (abs(millis() - WaterTimer) > 150000)) {
    TurnOffWater();
  }
  //Check to see if any of the buttons are pressed
  int KicthenButtonState = digitalRead(KitchWaterButton);
  int BathroomButtonState = digitalRead(BathroomWaterButton);  
>>>>>>> local
  if (KicthenButtonState == HIGH || BathroomButtonState == HIGH) {
    TurnOnWater();
  }

  //Check the States of pump and or logical state and set the LEDs accordingly
  WaterLEDState();
}

void TurnOnWater() {
  if (WaterOn == false && HoldingTankAlarm == false) {
    WaterTimer = millis();
    if (WaterSourseSelection == true) {
      digitalWrite(CityWaterValve, HIGH);
      Serial.println("Turning on CityWater");
    }
    else {
      Serial.println("Turning on WaterPump");
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
  if ((digitalRead(WaterPumpSense) == LOW && WaterSourseSelection == false) || WaterSourseSelection == true && digitalRead(CityWaterValve) == LOW) {
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

//------------------------------------------------------------------
//Generator and Energy
//------------------------------------------------------------------
void SetupEnergyMonitor() {
  eic.begin(EnergyMonitorCS, LineFreq, PGAGain, VoltageGain, CurrentGainCT1, 0, CurrentGainCT2);
}

<<<<<<< release/0.5
void EnergyMetering() {
  float voltageA, voltageC, currentCT1, currentCT2;
  unsigned short sys0 = eic.GetSysStatus0(); //EMMState0
  unsigned short sys1 = eic.GetSysStatus1(); //EMMState1
  unsigned short en0 = eic.GetMeterStatus0();//EMMIntState0
  unsigned short en1 = eic.GetMeterStatus1();//EMMIntState1

  Serial.println("Sys Status: S0:0x" + String(sys0, HEX) + " S1:0x" + String(sys1, HEX));
  Serial.println("Meter Status: E0:0x" + String(en0, HEX) + " E1:0x" + String(en1, HEX));
  delay(10);
=======
void EnergyMetering(){  
    float voltageA, voltageC,currentCT1,currentCT2;
    LastTimeACVoltage = LastTimeACCurrent = LastTimePowerFactor = LastTimeFreq = LastTimeWatts = 
    LastTimeReactive = LastTimeACApparent = LastTimeACFundimental = LastTimeACHarmonic = LastTimeHeadUnitTemp = 
    LastTimeRealPower = GetCurrentTime();
    unsigned short sys0 = eic.GetSysStatus0(); //EMMState0
    unsigned short sys1 = eic.GetSysStatus1(); //EMMState1
    //unsigned short en0 = eic.GetMeterStatus0();//EMMIntState0
    //unsigned short en1 = eic.GetMeterStatus1();//EMMIntState1

    //Serial.println("Sys Status: S0:0x" + String(sys0, HEX) + " S1:0x" + String(sys1, HEX));
    //Serial.println("Meter Status: E0:0x" + String(en0, HEX) + " E1:0x" + String(en1, HEX));
    //delay(10);

    //if true the MCU is not getting data from the energy meter
    if (sys0 == 65535 || sys0 == 0) Serial.println("Error: Not receiving data from energy meter - check your connections");

    //get voltage
    voltageA = eic.GetLineVoltageA();
    voltageC = eic.GetLineVoltageC();

    if (LineFreq = 4485) {
      LastACVoltage = voltageA + voltageC;     //is split single phase, so only 120v per leg
    }
    else {
      LastACVoltage = voltageA;     //voltage should be 220-240 at the AC transformer
    }

    //get current
    currentCT1 = eic.GetLineCurrentA();
    currentCT2 = eic.GetLineCurrentC(); //this is disconnected
    //totalCurrent = currentCT1 + currentCT2;
    //totalWatts = (voltageA * currentCT1) + (voltageC * currentCT2);

    LastACCurrent = eic.GetLineCurrentA(); //Motorhome panel is only one leg 
    LastPowerFactor = eic.GetTotalPowerFactor();
    LastFreq = eic.GetFrequency();
    LastACWatts = (voltageA * currentCT1);
    LastACReactive = eic.GetTotalReactivePower();
    LastACApparent = eic.GetTotalApparentPower();
    LastACFundimental = eic.GetTotalActiveFundPower();
    LastACHarmonic = eic.GetTotalActiveHarPower();
    LastHeadUnitTemp = eic.GetTemperature();
    LastACRealPower =  eic.GetTotalActivePower();
  
>>>>>>> local

  //if true the MCU is not getting data from the energy meter
  if (sys0 == 65535 || sys0 == 0) Serial.println("Error: Not receiving data from energy meter - check your connections");

  //get voltage
  voltageA = eic.GetLineVoltageA();
  voltageC = eic.GetLineVoltageC();

  if (LineFreq = 4485) {
    LastACVoltage = voltageA + voltageC;     //is split single phase, so only 120v per leg
  }
  else {
    LastACVoltage = voltageA;     //voltage should be 220-240 at the AC transformer
  }

  //get current
  currentCT1 = eic.GetLineCurrentA();
  currentCT2 = eic.GetLineCurrentC(); //this is disconnected
  //totalCurrent = currentCT1 + currentCT2;
  //totalWatts = (voltageA * currentCT1) + (voltageC * currentCT2);

  LastACCurrent = eic.GetLineCurrentA(); //Motorhome panel is only one leg
  LastPowerFactor = eic.GetTotalPowerFactor();
  LastFreq = eic.GetFrequency();
  LastACWatts = (voltageA * currentCT1);
  LastACReactive = eic.GetTotalReactivePower();
  LastACApparent = eic.GetTotalApparentPower();
  LastACFundimental = eic.GetTotalActiveFundPower();
  LastACHarmonic = eic.GetTotalActiveHarPower();
  LastHeadUnitTemp = eic.GetTemperature();
  LastACRealPower =  eic.GetTotalActivePower();
  LastTimeACVoltage = GetCurrentTime();

  Serial.println("Voltage 1: " + String(voltageA) + "V");
  Serial.println("Voltage 2: " + String(voltageC) + "V");
  Serial.println("Current 1: " + String(currentCT1) + "A");
  Serial.println("Current 2: " + String(currentCT2) + "A");
  Serial.println("Active Power: " + String(LastACRealPower) + "W");
  Serial.println("Power Factor: " + String(LastPowerFactor));
  Serial.println("Fundimental Power: " + String(LastACFundimental) + "W");
  Serial.println("Harmonic Power: " + String(LastACHarmonic) + "W");
  Serial.println("Reactive Power: " + String(LastACReactive) + "var");
  Serial.println("Apparent Power: " + String(LastACApparent) + "VA");
  Serial.println("Chip Temp: " + String(LastHeadUnitTemp) + "C");
  Serial.println("Frequency: " + String(LastFreq) + "Hz");

  delay(1000);
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
    else {}
  }
  else {
    LastGenFuel = 0.0;
  }
  LastTimeGenFuel = GetCurrentTime();

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

  LastTimeGenTemps = GetCurrentTime();
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
    //Turn On warning LED
    digitalWrite(AlarmLED, HIGH);
    // Also put in a check for FUll State on either and turn off pump or city water
    if (LastSewageLevel == "Full" || LastGreyWater == "Full") {
      HoldingTankAlarm = true;
      TurnOffWater();
      digitalWrite(AlarmOut, HIGH);
    }
    else {
      // if the tanks drop below full make sure the alarm is off
      HoldingTankAlarm = false;
      digitalWrite(AlarmOut, LOW);
    }
  }
  else {
    digitalWrite(AlarmLED, LOW);
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
      break;
  }

  LastTimeSewageLevel = GetCurrentTime();


  if (ShittersGettinFull == false && WaterOn == false) {
    // Turn Off Voltage to tank
    digitalWrite(SewagePowerRelay, LOW);
  }
  //Serial.print("Shit:");
  //Serial.println(LastSewageLevel);
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
      break;
  }
  LastTimeGreyWater = GetCurrentTime();

  if (GreyGettinFull == false && WaterOn == false) {
    // Turn Off Voltage to tank
    digitalWrite(GreyWaterPowerRelay, LOW);
  }
  //Serial.print("Grey:");
  //Serial.println(LastGreyWater);
}

void ReadWaterAndLPG() {
  // Turn On votlage to tanks
  digitalWrite(TankPowerRelay, HIGH);
  delay(1000);
  String LastTimeLPGLevel = GetCurrentTime();
<<<<<<< release/0.5
  int LPGResistence = 47 * (1 / ((5 / (ConversionFactor * analogRead(LPGSensor))) - 1));
  if (LPGResistence > 124) {
=======
  int LPGResistence = 47*(1/((5/(ConversionFactor * analogRead(LPGSensor)))-1));
  if (LPGResistence > 122){
>>>>>>> local
    //LastLPGLevel = "ERROR Check Tank Sensor";
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
  if (WaterOn != true){
    digitalWrite(TankPowerRelay, LOW);
  }
  
  
    Serial.print("LastLPGLevel:");
    Serial.println(LastLPGLevel);
    Serial.print("LastWaterLevel:");
    Serial.println(LastWaterLevel);
    delay(5000);
  
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
  LastTimeRTCVoltage = GetCurrentTime();

  ///*Test Code
    ControlComPort.print("Camper Voltage: ");
    ControlComPort.print(LastTimeDCVoltage);
    ControlComPort.print(" : ");
    ControlComPort.println(LastDCVoltage);

    ControlComPort.print("RTC Battery Voltage: ");
    ControlComPort.print(LastTimeRTCVoltage);
    ControlComPort.print(" : ");
    ControlComPort.println(LastRTCVoltage);

    delay(1000);
  
}

void ReadOtherTempSensors() {
  int R2 = 10000;
  float c1 = 1.009249522e-03;
  float c2 = 2.378405444e-04;
  float c3 = 2.019202697e-07;

  float VoutACF = ConversionFactor * analogRead(FrontACTemp);
  float R1ACF = log(R2 * ((5.0 / VoutACF) - 1));
  if (Units == "I") {
    LastFrontACTemp = ConvertCtoF((((1.0 / (c1 + c2 * R1ACF + c3 * R1ACF * R1ACF * R1ACF))) - 273.15));
  }
  else {
    LastFrontACTemp = (((1.0 / (c1 + c2 * R1ACF + c3 * R1ACF * R1ACF * R1ACF))) - 273.15);
  }

  float VoutACB = ConversionFactor * analogRead(BackACTemp);
  float R1ACB = log(R2 * ((5.0 / VoutACB) - 1));
  if (Units == "I") {
    LastBackACTemp = ConvertCtoF((((1.0 / (c1 + c2 * R1ACB + c3 * R1ACB * R1ACB * R1ACB))) - 273.15));
  }
  else {
    LastBackACTemp = (((1.0 / (c1 + c2 * R1ACB + c3 * R1ACB * R1ACB * R1ACB))) - 273.15);
  }

  float VoutHallway = ConversionFactor * analogRead(HallwayTemp);
  float R1Hallway = log(R2 * ((5.0 / VoutHallway) - 1));
  if (Units == "I") {
    LastHallwayTemp = ConvertCtoF((((1.0 / (c1 + c2 * R1Hallway + c3 * R1Hallway * R1Hallway * R1Hallway))) - 273.15));
  }
  else {
    LastHallwayTemp = (((1.0 / (c1 + c2 * R1Hallway + c3 * R1Hallway * R1Hallway * R1Hallway))) - 273.15);
  }

  float VoutBathroom = ConversionFactor * analogRead(BathroomTemp);
  float R1Bathroom = log(R2 * ((5.0 / VoutBathroom) - 1));
  if (Units == "I") {
    LastBathroomTemp = ConvertCtoF((((1.0 / (c1 + c2 * R1Bathroom + c3 * R1Bathroom * R1Bathroom * R1Bathroom))) - 273.15));
  }
  else {
    LastBathroomTemp = (((1.0 / (c1 + c2 * R1Bathroom + c3 * R1Bathroom * R1Bathroom * R1Bathroom))) - 273.15);
  }

  float VoutFreezer = ConversionFactor * analogRead(Freezer);
  float R1Freezer = log(R2 * ((5.0 / VoutFreezer) - 1));
  if (Units == "I") {
    LastFreezerTemp = ConvertCtoF((((1.0 / (c1 + c2 * R1Freezer + c3 * R1Freezer * R1Freezer * R1Freezer))) - 273.15));
  }
  else {
    LastFreezerTemp = (((1.0 / (c1 + c2 * R1Freezer + c3 * R1Freezer * R1Freezer * R1Freezer))) - 273.15);
  }

  float VoutFridge = ConversionFactor * analogRead(Refridgerator);
  float R1Fridge = log(R2 * ((5.0 / VoutFridge) - 1));
  if (Units == "I") {
    LastFridgeTemp = ConvertCtoF((((1.0 / (c1 + c2 * R1Fridge + c3 * R1Fridge * R1Fridge * R1Fridge))) - 273.15));
  }
  else {
    LastFridgeTemp = (((1.0 / (c1 + c2 * R1Fridge + c3 * R1Fridge * R1Fridge * R1Fridge))) - 273.15);
  }

  float VoutOutside = ConversionFactor * analogRead(Outside);
  float R1Outside = log(R2 * ((5.0 / VoutOutside) - 1));
  if (Units == "I") {
    LastOutsideTemp = ConvertCtoF((((1.0 / (c1 + c2 * R1Outside + c3 * R1Outside * R1Outside * R1Outside))) - 273.15));
  }
  else {
    LastOutsideTemp = (((1.0 / (c1 + c2 * R1Outside + c3 * R1Outside * R1Outside * R1Outside))) - 273.15);
  }

  float VoutBackCabin = ConversionFactor * analogRead(BackCabin);
  float R1BackCabin = log(R2 * ((5.0 / VoutBackCabin) - 1));
  if (Units == "I") {
    LastBackCabinTemp = ConvertCtoF((((1.0 / (c1 + c2 * R1BackCabin + c3 * R1BackCabin * R1BackCabin * R1BackCabin))) - 273.15));
  }
  else {
    LastBackCabinTemp = (((1.0 / (c1 + c2 * R1BackCabin + c3 * R1BackCabin * R1BackCabin * R1BackCabin))) - 273.15);
  }

  LastTimeNTCTemp = GetCurrentTime();
}


//------------------------------------------------------------------
//Helper functions
//------------------------------------------------------------------
float ConvertCtoF(float C) {
  //Serial.print("C:");
  //Serial.println(C);
  float F = (1.8 * C) + 32;
  return F;
}

String GetCurrentTime() {
  DateTime now = rtc.now();
  String DateTimeString = String(now.year())
                          + "/" + String(now.month())
                          + "/" + String(now.day())
                          + "-" + String(now.hour())
                          + ":" + String(now.minute())
                          + ":" + String(now.second());
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
  ControlComPort.println(LastTimeWaterLevel + ",Water," + LastWaterLevel);
  ControlComPort.println(LastTimeSewageLevel + ",Sewage," + LastSewageLevel);
  ControlComPort.println(LastTimeGreyWater + ",Grey," +  LastGreyWater);
  ControlComPort.println(LastTimeLPGLevel + ",LPG," + LastLPGLevel);
  ControlComPort.println(LastTimeDCVoltage + ",Camper VDC," + LastDCVoltage);
  ControlComPort.println(LastTimeNTCTemp + ",NTC Tempetatures,Front AC Temp," + LastFrontACTemp + ",Back AC Temp," + LastBackACTemp + ",Under Awning Temp," + LastOutsideTemp + ",Back Cabin Temp," + LastBackCabinTemp + ",Hallway Temp," + LastHallwayTemp + ",Freezer," + LastFreezerTemp + ",Fridge," + LastFridgeTemp + ",Bathroom Temp," + LastBathroomTemp);  
  ControlComPort.println(LastTimeGenFuel + ",Generator Fuel Pressure," + LastGenFuel);
  ControlComPort.println(LastTimeGenTemps + "Generator Temps, Enclosure," + LastGenEnclosureTemp + ",Right Head Temp," + LastGenHeadRightTemp + ",Left Head Temp," + LastGenHeadLeftTemp);
  ControlComPort.println(LastTimeRTCVoltage + "RTCBattery," + LastRTCVoltage);

  ControlComPort.println(LastTimeACVoltage + "Head Unit Temp," + "" + LastHeadUnitTemp);
  String EnergyOutputSentence = LastTimeACVoltage + "," + LastACVoltage + ",V," + LastACCurrent + ",A,"  + LastPowerFactor + ",PF," + LastACRealPower + ",W{real)," + LastFreq + ",Hz," + LastACWatts + ",W(total)," + LastACReactive + ",var(reactive),"  + LastACApparent + ",VA(apparent)," + LastACFundimental + ",W(fundimental)," + LastACHarmonic + ",W(harmonic)\r";
  ControlComPort.println(EnergyOutputSentence);
}

void ResetAllAlarms() {
  HoldingTankAlarm = false;
  digitalWrite(AlarmOut, LOW);
  digitalWrite(AlarmLED, LOW);
}
