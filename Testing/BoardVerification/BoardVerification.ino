#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_MAX31865.h>
#include <SPI.h>
#include <RTClib.h>
#include <EEPROM.h>
#include <ATM90E32.h>

/***** CALIBRATION SETTINGS *****/
/* 
 * 4485 for 60 Hz (North America)
 * 389 for 50 hz (rest of the world)
 */
unsigned short LineFreq = 4485;         

/* 
 * 0 for 10A (1x)
 * 21 for 100A (2x)
 * 42 for between 100A - 200A (4x)
 */
unsigned short PGAGain = 21;            

/* 
 * For meter <= v1.3:
 *    42080 - 9v AC Transformer - Jameco 112336
 *    32428 - 12v AC Transformer - Jameco 167151
 * For meter > v1.4:
 *    37106 - 9v AC Transformer - Jameco 157041
 *    38302 - 9v AC Transformer - Jameco 112336
 *    29462 - 12v AC Transformer - Jameco 167151
 * For Meters > v1.4 purchased after 11/1/2019 and rev.3
 *    7611 - 9v AC Transformer - Jameco 157041
 */
unsigned short VoltageGain = 4005;     
                                       
/*
 * 25498 - SCT-013-000 100A/50mA
 * 39473 - SCT-016 120A/40mA
 * 46539 - Magnalab 100A
 */                                  
unsigned short CurrentGainCT1 = 25498;  
unsigned short CurrentGainCT2 = 25498; 

//-----------------------------------------------------------
// Serial Communication, UI 
//-----------------------------------------------------------
#define USBSerial Serial
#define RS232 Serial2
LiquidCrystal_I2C lcd(0x27, 20, 4);
#define LCDEnable 41
#define LCDPowerOut 23
//-----------------------------------------------------------
// ATM90E32 energy monitor
ATM90E32 eic{}; // Energy Monitor Object
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

  GenHeadR.begin(MAX31865_3WIRE);
  GenHeadL.begin(MAX31865_3WIRE);
  GenEnclosure.begin(MAX31865_3WIRE);

  //Energy
  pinMode(EnergyMonitorCS, OUTPUT);
  eic.begin(EnergyMonitorCS, LineFreq, PGAGain, VoltageGain, CurrentGainCT1, 0, CurrentGainCT2);
}

void loop() {
  //SewAgeTank();
  //GreyTank();
  //GeneralInputs();
  //LCDTesting();
  //GeneralOutputs();
  //SpareOutputWalk();
  //ReadAllInputs();
  //SpareAnalogRead();
  //ReadADCsVoltages();
  //ReadOtherTempSensors();
  //ReadWaterAndLPG();
  //readRTD();
  //EnergyMonitor();
  }

void LCDTesting(){
  USBSerial.print("LCDEnable:");
  USBSerial.println(digitalRead(LCDEnable));
  delay(1000);
  USBSerial.print("LCDPowerOut:HIGH");
  digitalWrite(LCDPowerOut, HIGH);
  delay(5000);
  USBSerial.print("LCDPowerOut:LOW");
  digitalWrite(LCDPowerOut, LOW);
  delay(5000);
}

void GeneralOutputs(){
  USBSerial.print("LEDBacklightOut:HIGH");
  digitalWrite(LEDBacklightOut, HIGH);
  delay(5000);
  USBSerial.print("LEDBacklightOut:LOW");
  digitalWrite(LEDBacklightOut, LOW);
  delay(5000);
  
  USBSerial.print("TankPowerRelay:HIGH");
  digitalWrite(TankPowerRelay, HIGH);
  delay(5000);
  USBSerial.print("TankPowerRelay:LOW");
  digitalWrite(TankPowerRelay, LOW);
  delay(5000);
  
  USBSerial.print("WaterPumpOut:HIGH");
  digitalWrite(WaterPumpOut, HIGH);
  delay(5000);
  USBSerial.print("WaterPumpOut:LOW");
  digitalWrite(WaterPumpOut, LOW);
  delay(5000);
  
  USBSerial.print("AlarmOut:HIGH");
  digitalWrite(AlarmOut, HIGH);
  delay(5000);
  USBSerial.print("AlarmOut:LOW");
  digitalWrite(AlarmOut, LOW);
  delay(5000);
 
  USBSerial.print("WarningLED:HIGH");
  digitalWrite(WarningLED, HIGH);
  delay(5000);
  USBSerial.print("WarningLED:LOW");
  digitalWrite(WarningLED, LOW);
  delay(5000);

  USBSerial.print("CityWaterValve:");
  digitalWrite(CityWaterValve, HIGH);
  delay(5000);
  USBSerial.print("CityWaterValve:LOW");
  digitalWrite(CityWaterValve, LOW);
  delay(5000);

  USBSerial.print("KitchenWaterButtonLED:");
  digitalWrite(KitchenWaterButtonLED, HIGH);
  delay(5000);
  USBSerial.print("KitchenWaterButtonLED:LOW");
  digitalWrite(KitchenWaterButtonLED, LOW);
  delay(5000);

  USBSerial.print("BathroomWaterButtonLED:HIGH");
  digitalWrite(BathroomWaterButtonLED, HIGH);
  delay(5000);
  USBSerial.print("BathroomWaterButtonLED:LOW");
  digitalWrite(BathroomWaterButtonLED, LOW);
  delay(5000);
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

void SpareOutputWalk(){
    for (int i = 0; i <= SpareOutputSize; i++) {
    USBSerial.print("Output " + String(i) + ": High");
    digitalWrite(SpareOutputs[i],HIGH);
    delay(5000);
    USBSerial.print("Output " + String(i) + ": Low");
    digitalWrite(SpareOutputs[i],LOW);
    delay(5000);
  }
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

void ReadADCsVoltages() {
  USBSerial.print("DCVoltageDN:");
  USBSerial.println(ReadAnalog(50, Camper12VoltSensor));
  USBSerial.print("RTCVoltageDN:");
  USBSerial.println(ReadAnalog(50, RTCBattery));
  delay(5000);
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


void ReadOtherTempSensors() {
  int R2 = 10000;

  float VoutACF = ConversionFactor * ReadAnalog(10, FrontACTemp);
  float R1ACF = log(R2 * ((5.0 / VoutACF) - 1));
  USBSerial.print("R1ACF:");
  USBSerial.println(R1ACF);
  
  float VoutACB = ConversionFactor * ReadAnalog(10, BackACTemp);
  float R1ACB = log(R2 * ((5.0 / VoutACB) - 1));
  USBSerial.print("R1ACB:");
  USBSerial.println(R1ACB);

  float VoutHallway = ConversionFactor * ReadAnalog(10, HallwayTemp);
  float R1Hallway = log(R2 * ((5.0 / VoutHallway) - 1));
  USBSerial.print("R1Hallway:");
  USBSerial.println(R1Hallway);

  float VoutBathroom = ConversionFactor * ReadAnalog(10, BathroomTemp);
  float R1Bathroom = log(R2 * ((5.0 / VoutBathroom) - 1));
  USBSerial.print("R1Bathroom:");
  USBSerial.println(R1Bathroom);

  float VoutFreezer = ConversionFactor * ReadAnalog(10, Freezer);
  float R1Freezer = log(R2 * ((5.0 / VoutFreezer) - 1));
  USBSerial.print("R1Freezer:");
  USBSerial.println(R1Freezer);

  float VoutFridge = ConversionFactor * ReadAnalog(10, Refridgerator);
  float R1Fridge = log(R2 * ((5.0 / VoutFridge) - 1));
  USBSerial.print("R1Fridge:");
  USBSerial.println(R1Fridge);

  float VoutOutside = ConversionFactor * ReadAnalog(10, Outside);
  float R1Outside = log(R2 * ((5.0 / VoutOutside) - 1));
  USBSerial.print(":");
  USBSerial.println();

  float VoutBackCabin = ConversionFactor * ReadAnalog(10, BackCabin);
  float R1BackCabin = log(R2 * ((5.0 / VoutBackCabin) - 1));
  USBSerial.print("R1BackCabin:");
  USBSerial.println(R1BackCabin);
  delay(10000);
}


void ReadWaterAndLPG() {
  int LPGResistence = 47 * (1 / ((5 / (ConversionFactor * ReadAnalog(50, LPGSensor))) - 1));
  USBSerial.print("LPGResistence:");
  USBSerial.println(LPGResistence);

  float R1 = 47 * ((5.0 / (ConversionFactor * ReadAnalog(50, WaterTankSensor))) - 1);
  USBSerial.print("WaterResistance:");
  USBSerial.println(R1);
  delay(5000);
  
}


void EnergyMonitor(){
    float voltageA, voltageC, totalVoltage, currentCT1, currentCT2, totalCurrent, realPower, powerFactor, temp, freq, totalWatts;

    unsigned short sys0 = eic.GetSysStatus0(); //EMMState0
    unsigned short sys1 = eic.GetSysStatus1(); //EMMState1
    unsigned short en0 = eic.GetMeterStatus0();//EMMIntState0
    unsigned short en1 = eic.GetMeterStatus1();//EMMIntState1

    USBSerial.println("Sys Status: S0:0x" + String(sys0, HEX) + " S1:0x" + String(sys1, HEX));
    USBSerial.println("Meter Status: E0:0x" + String(en0, HEX) + " E1:0x" + String(en1, HEX));
    delay(10);

    //if true the MCU is not getting data from the energy meter
    if (sys0 == 65535 || sys0 == 0) Serial.println("Error: Not receiving data from energy meter - check your connections");

    //get voltage
    voltageA = eic.GetLineVoltageA();
    voltageC = eic.GetLineVoltageC();

    if (LineFreq = 4485) {
      totalVoltage = voltageA + voltageC;     //is split single phase, so only 120v per leg
    }
    else {
      totalVoltage = voltageA;     //voltage should be 220-240 at the AC transformer
    }

    //get current
    currentCT1 = eic.GetLineCurrentA();
    currentCT2 = eic.GetLineCurrentC();
    totalCurrent = currentCT1 + currentCT2;

    realPower = eic.GetTotalActivePower();
    powerFactor = eic.GetTotalPowerFactor();
    temp = eic.GetTemperature();
    freq = eic.GetFrequency();
    totalWatts = (voltageA * currentCT1) + (voltageC * currentCT2);

    USBSerial.println("Voltage 1: " + String(voltageA) + "V");
    USBSerial.println("Voltage 2: " + String(voltageC) + "V");
    USBSerial.println("Current 1: " + String(currentCT1) + "A");
    USBSerial.println("Current 2: " + String(currentCT2) + "A");
    USBSerial.println("Active Power: " + String(realPower) + "W");
    USBSerial.println("Power Factor: " + String(powerFactor));
    USBSerial.println("Fundimental Power: " + String(eic.GetTotalActiveFundPower()) + "W");
    USBSerial.println("Harmonic Power: " + String(eic.GetTotalActiveHarPower()) + "W");
    USBSerial.println("Reactive Power: " + String(eic.GetTotalReactivePower()) + "var");
    USBSerial.println("Apparent Power: " + String(eic.GetTotalApparentPower()) + "VA");
    USBSerial.println("Phase Angle A: " + String(eic.GetPhaseA()));
    USBSerial.println("Chip Temp: " + String(temp) + "C");
    USBSerial.println("Frequency: " + String(freq) + "Hz");
    
    delay(1000);
}


void readRTD(){
  uint16_t rtd0 = GenHeadR.readRTD();
  float ratio0 = rtd0;
  ratio0 /= 32768;
  USBSerial.print("GenHeadR:");
  USBSerial.println(GenHeadR.temperature(RNOMINAL, RREF));
  

  uint16_t rtd1 = GenHeadL.readRTD();
  float ratio1 = rtd1;
  ratio1 /= 32768;
  USBSerial.print("GenHeadL:");
  USBSerial.println(GenHeadL.temperature(RNOMINAL, RREF));
  

  uint16_t rtd2 = GenEnclosure.readRTD();
  float ratio2 = rtd2;
  ratio2 /= 32768;
  USBSerial.print("GenEnclosure:");
  USBSerial.println(GenEnclosure.temperature(RNOMINAL, RREF));
  
  


}
