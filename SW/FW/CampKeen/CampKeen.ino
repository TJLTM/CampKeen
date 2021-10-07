#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_MAX31865.h>
#include <SPI.h>
#include <RTClib.h>
#include <EEPROM.h>

#define ControlComPort Serial1
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 20, 4);
int DisplayCounter = 0;
const int DisplayInvterval = 3000;

const float ConversionFactor = 5.0 / 1023;
long WaterTimer, ShitterTankTimer, GreyTankTimer, WATERLPGtimer, FiveMinTimer, LastMillis1, NTCTimer;
bool ShittersGettinFull = false;
bool GreyGettinFull = false;
bool HoldingTankAlarm = false;
bool BathroomLEDState = false, KitchenLEDState = false, WaterSourseSelection = false, WaterOn = false;
//WaterSourceSelection false = pump true = City Water
bool StreamingData = true;

//LastValues
String LastWaterLevel = "Empty";
String LastTimeWaterLevel = "";
String LastSewageLevel = "Empty";
String LastTimeSewageLevel = "";
String LastGreyWater = "Empty";
String LastTimeGreyWater = "";
float LastLPGLevel = 0.0;
String LastTimeLPGLevel = "";
float LastDCVoltage = 0.0;
String LastTimeDCVoltage = "";
float LastACVoltage = 0.0;
String LastTimeACVoltage = "";
float LastACCurrent = 0.0;
String LastTimeACCurrent = "";
float LastFrontACTemp = 0.0;
String LastTimeFrontACTemp = "";
float LastBackACTemp = 0.0;
String LastTimeBackACTemp = "";
float LastOutsideTemp = 0.0;
String LastTimeOutsideTemp = "";
float LastBackCabinTemp = 0.0;
String LastTimeBackCabinTemp = "";
float LastHallwayTemp = 0.0;
String LastTimeHallwayTemp = "";
float LastFreezerTemp = 0.0;
String LastTimeFreezerTemp = "";
float LastFridgeTemp = 0.0;
String LastTimeFridgeTemp = "";
float LastBathroomTemp = 0.0;
String LastTimeBathroomTemp = "";
float LastGenFuel = 0.0;
String LastTimeGenFuel = "";
float LastGenEnclosureTemp = 0.0;
String LastTimeGenEnclosureTemp = "";
float LastGenHeadRightTemp = 0.0;
String LastTimeGenHeadRightTemp = "";
float LastGenHeadLeftTemp = 0.0;
String LastTimeGenHeadLeftTemp = "";

//Generator
#define RTDGenHeadRCS 45
#define RTDGenHeadLCS 47
#define RTDGenEnclosure 49
#define RREF      430.0
#define RNOMINAL  100.0
Adafruit_MAX31865 GenHeadR = Adafruit_MAX31865(RTDGenHeadRCS);
Adafruit_MAX31865 GenHeadL = Adafruit_MAX31865(RTDGenHeadLCS);
Adafruit_MAX31865 GenEnclosure = Adafruit_MAX31865(RTDGenEnclosure);

//ADC Pins
#define Camper12VoltSensor A0
#define LPGSensor A1
#define WaterTankSensor A2
#define FrontACTemp A3
#define BackACTemp A4
#define HallwayTemp A5
#define BathroomTemp A6
#define Freezer A7
#define Refridgerator A8
#define Outside A9
#define BackCabin A10
#define ACVoltage A11
#define ACAMPS A12
#define GenFuelPressure A13

//OutputPins
#define LEDBacklightOut 4
#define TankPowerRelay 43
#define GreyWaterPowerRelay 32
#define SewagePowerRelay 30
#define WaterPumpOut 26
#define AlarmOut 35
#define AlarmLED 33
#define CityWaterValve 37
#define KitchenWaterButtonLED 31
#define BathroomWaterButtonLED 29

//InputPins
#define AlarmReset 22
#define WaterSourceSelectionInput 28
#define WaterPumpSense 23
#define KitchWaterButton 25
#define BathroomWaterButton 27

//SparePins and ports that are wired on board
#define OtherComPort Serial2
#define SPADC1 14
#define SPADC2 15
#define SPIn1 10
#define SPIn2 9
#define SPIn3 8
#define SPIn4 7
#define SPIn5 6
#define SPIn6 5
#define SPO1 41
#define SPO2 13
#define SPO3 12
#define SPO4 11
#define SPO5 24

//SewageTankLevelPins
#define S14 48
#define S12 46
#define S34 44
#define S44 42

//GreyTankLevelPins
#define G14 40
#define G12 38
#define G34 36
#define G44 34

void setup() {
  ControlComPort.begin(115200);
  lcd.init();
  lcd.backlight();
  rtc.begin();

  pinMode(RTDGenHeadRCS, OUTPUT);
  pinMode(RTDGenHeadLCS, OUTPUT);
  pinMode(RTDGenEnclosure, OUTPUT);

  pinMode(AlarmReset, INPUT);
  pinMode(WaterSourceSelectionInput, INPUT);
  pinMode(WaterPumpSense, INPUT);
  pinMode(KitchWaterButton, INPUT);
  pinMode(BathroomWaterButton, INPUT);

  pinMode(S14, INPUT);
  pinMode(S12, INPUT);
  pinMode(S34, INPUT);
  pinMode(S44, INPUT);

  pinMode(G14, INPUT);
  pinMode(G12, INPUT);
  pinMode(G34, INPUT);
  pinMode(G44, INPUT);

  GenHeadR.begin(MAX31865_3WIRE); // set to 2WIRE or 4WIRE as necessary
  GenHeadL.begin(MAX31865_3WIRE); // set to 2WIRE or 4WIRE as necessary
  GenEnclosure.begin(MAX31865_3WIRE); // set to 2WIRE or 4WIRE as necessary

  ControlComPort.println("Starting system up");
  lcd.setCursor(0, 0);
  lcd.print("Starting system up");
  ReadGreyTank();
  ReadSewageTank();
  ReadOtherTempSensors();
  ReadCamperBatteryVoltage();
  ReadWaterAndLPG();
  GeneratorSensors();
  ControlComPort.println("System Initialized and values populated");
  lcd.setCursor(0, 0);
  //lcd.print(DeviceName);
  lcd.setCursor(0, 1);
  //lcd.print(Version);
  lcd.setCursor(0, 2);
  //lcd.print("Units: "+Units);
  lcd.setCursor(0, 3);
  lcd.print("Ready");
}

void loop() {
  //Read Current Water Source Selection
  WaterSourseSelection = digitalRead(WaterSourceSelectionInput);
  // read the source selection input
  if (digitalRead(WaterSourceSelectionInput) == HIGH) {
    WaterSourseSelection = true;//city water
  }
  else {
    WaterSourseSelection = false; //Tank
  }

  //if the water is on and the timer says it's been on for more than 5 mins turn off.
  if (WaterOn == true && (millis() - WaterTimer > 300000)) {
    TurnOffWater;
  }

  //Check to see if any of the buttons are pressed
  int KicthenButtonState = digitalRead(KitchWaterButton);
  int BathroomButtonState = digitalRead(BathroomWaterButton);
  if (KicthenButtonState == HIGH || BathroomButtonState == HIGH) {
    TurnOnWater();
  }

  //Check the States of pump and or logical state and set the LEDs accordingly
  WaterLEDState();

  //Read Grey and Sewage Tanks at 15 min intervals unless it is at 3/4 full then switch to Constant
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

if (digitalRead(AlarmReset) == HIGH) {
  ResetAllAlarms();
}

if (abs(millis() - NTCTimer) > 3000) {
  //Read NTC temp Sensors
  ReadOtherTempSensors();
  //Read AC Voltage and Current
  ACReadings();
  //Read Generator Sensors
  GeneratorSensors();
}

if (abs(millis() - WATERLPGtimer) > 3.6e+6) {
  //ReadLPG and Water Tank at 1 hour intervals
  ReadWaterAndLPG();
  WATERLPGtimer = millis();
}

if (abs(millis() - FiveMinTimer) > 300000) {
  //Read DC voltage at 5 min intervals
  ReadCamperBatteryVoltage();
  FiveMinTimer = millis();
}

//After all the sensors have been updated stream out the data if the flag is set to do so
if (StreamingData == true) {
  OutputAllData();
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


void OutputAllData() {
  String OutputSentence = LastTimeWaterLevel + "Water," + LastWaterLevel + '\r'
                          + LastTimeSewageLevel + "Sewage," + LastSewageLevel + '\r'
                          + LastTimeGreyWater + "Grey," +  LastGreyWater + '\r'
                          + LastTimeLPGLevel + "LPG," + LastLPGLevel + '\r'
                          + LastTimeDCVoltage + "Camper DC," + LastDCVoltage + '\r'
                          + LastTimeACVoltage + "ACV," + LastACVoltage + '\r'
                          + LastTimeACCurrent + "AC Amps," + LastACCurrent + '\r'
                          + LastTimeFrontACTemp + "Front AC Temp," + LastFrontACTemp + '\r'
                          + LastTimeBackACTemp + "Back AC Temp," + LastBackACTemp + '\r'
                          + LastTimeOutsideTemp + "Under Awning Temp," + LastOutsideTemp + '\r'
                          + LastTimeBackCabinTemp + "Back Cabin Temp," + LastBackCabinTemp + '\r'
                          + LastTimeHallwayTemp + "Hallway Temp," + LastHallwayTemp + '\r'
                          + LastTimeFreezerTemp + "Freezer," + LastFreezerTemp + '\r'
                          + LastTimeFridgeTemp + "Fridge," + LastFridgeTemp + '\r'
                          + LastTimeBathroomTemp + "Bathroom Temp," + LastBathroomTemp + '\r'
                          + LastTimeGenFuel + "Generator Fuel Pressure," + LastGenFuel + '\r'
                          + LastTimeGenEnclosureTemp + "Generator Enclosure Temp," + LastGenEnclosureTemp + '\r'
                          + LastTimeGenHeadRightTemp + "Generator Right Head Temp," + LastGenHeadRightTemp + '\r'
                          + LastTimeGenHeadLeftTemp + "Generator Left Head Temp," + LastGenHeadLeftTemp + '\r' ;

  ControlComPort.println(OutputSentence);
}


void ResetAllAlarms() {
  HoldingTankAlarm = false;
  digitalWrite(AlarmOut, LOW);
  digitalWrite(AlarmLED, LOW);
}

void LCDDisplay() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

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

void TurnOnWater() {
  if (WaterOn == false && HoldingTankAlarm == false) {
    WaterOn = true;
    WaterTimer = millis();
    digitalWrite(KitchenWaterButtonLED, HIGH);
    digitalWrite(BathroomWaterButtonLED, HIGH);
    if (WaterSourseSelection == true) {
      digitalWrite(WaterPumpOut, HIGH);
    }
    else {
      digitalWrite(GreyWaterPowerRelay, HIGH);
    }
  }
}

void TurnOffWater() {
  WaterOn = false;
  digitalWrite(GreyWaterPowerRelay, LOW);
  digitalWrite(WaterPumpOut, LOW);
  digitalWrite(KitchenWaterButtonLED, LOW);
  digitalWrite(BathroomWaterButtonLED, LOW);
  delay(250);
}

void WaterLEDState() {
  if (digitalRead(WaterPumpSense) == LOW && WaterSourseSelection == false) {
    digitalWrite(KitchenWaterButtonLED, LOW);
    digitalWrite(BathroomWaterButtonLED, LOW);
  }

  if (WaterSourseSelection == true && digitalRead(CityWaterValve) == LOW) {
    digitalWrite(KitchenWaterButtonLED, LOW);
    digitalWrite(BathroomWaterButtonLED, LOW);
  }

  if (digitalRead(WaterSourseSelection) == true && digitalRead(CityWaterValve) == HIGH) {
    digitalWrite(KitchenWaterButtonLED, HIGH);
    digitalWrite(BathroomWaterButtonLED, HIGH);
  }

  if (digitalRead(WaterPumpSense) == HIGH) {
    digitalWrite(KitchenWaterButtonLED, HIGH);
    digitalWrite(BathroomWaterButtonLED, HIGH);
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
    LastGenFuel = (7.5 * ConversionFactor * (FuelPressureSum / Samples)) - 3.75;
  }
  else {
    LastGenFuel = 0.0;
  }
  LastTimeGenFuel = GetCurrentTime();


  uint16_t rtd0 = GenHeadR.readRTD();
  float ratio0 = rtd0;
  ratio0 /= 32768;
  LastGenHeadRightTemp = GenHeadR.temperature(RNOMINAL, RREF);
  LastTimeGenHeadRightTemp = GetCurrentTime();

  uint16_t rtd1 = GenHeadL.readRTD();
  float ratio1 = rtd1;
  ratio1 /= 32768;
  LastGenHeadLeftTemp = GenHeadL.temperature(RNOMINAL, RREF);
  LastTimeGenHeadLeftTemp = GetCurrentTime();

  uint16_t rtd2 = GenEnclosure.readRTD();
  float ratio2 = rtd2;
  ratio2 /= 32768;
  LastGenEnclosureTemp = GenEnclosure.temperature(RNOMINAL, RREF);
  LastTimeGenEnclosureTemp = GetCurrentTime();
}

void ACReadings() {
  int Samples = 50;
  long ACCurrentSum = 0;
  long ACVoltageSum = 0;

  float nVPP = getVPP();
  LastACCurrent = (nVPP / 18.0) * 100.0 * 0.707106781186548;
  LastTimeACVoltage = GetCurrentTime();

  for (int x = 0; x < Samples; x++) {
    ACVoltageSum = ACVoltageSum + analogRead(ACVoltage);
  }
  LastACVoltage = 0.159814451300763 * (ACVoltageSum / Samples) - 3.65917457720521;
  LastTimeACCurrent = GetCurrentTime();
}

float getVPP() {
  float result;
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  uint32_t start_time = millis();
  while ((millis() - start_time) < 1000) //sample for 1 Sec
  {
    readValue = analogRead(ACAMPS);
    // see if you have a new maxValue
    if (readValue > maxValue)
    {
      /*record the maximum sensor value*/
      maxValue = readValue;
    }
  }
  result = (maxValue - 510)*ConversionFactor;
  return result;
}

void ReadSewageTank() {
  // Turn On votlage to tank
  digitalWrite(SewagePowerRelay, HIGH);
  int Quarter = digitalRead(S14);
  int Half = digitalRead(S12);
  int ThreeQuarters = digitalRead(S34);
  int Full = digitalRead(S44);

  if (Quarter == HIGH || Half == HIGH || ThreeQuarters == HIGH || Full == HIGH) {
    LastSewageLevel = "ERROR Check Tank";
  }

  if (Quarter == LOW && Half == LOW && ThreeQuarters == LOW && Full == LOW) {
    LastSewageLevel = "Empty";
    ShittersGettinFull = false;
  }

  if (Quarter == HIGH && Half == LOW && ThreeQuarters == LOW && Full == LOW) {
    LastSewageLevel = "1/4";
    ShittersGettinFull = false;
  }

  if (Quarter == HIGH && Half == HIGH && ThreeQuarters == LOW && Full == LOW) {
    LastSewageLevel = "1/2";
    ShittersGettinFull = false;
  }

  if (Quarter == HIGH && Half == HIGH && ThreeQuarters == HIGH && Full == LOW) {
    LastSewageLevel = "3/4";
    ShittersGettinFull = true;
  }

  if (Quarter == HIGH && Half == HIGH && ThreeQuarters == HIGH && Full == HIGH) {
    LastSewageLevel = "Full";
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
  int Quarter = digitalRead(G14);
  int Half = digitalRead(G12);
  int ThreeQuarters = digitalRead(G34);
  int Full = digitalRead(G44);

  if (Quarter == HIGH || Half == HIGH || ThreeQuarters == HIGH || Full == HIGH) {
    LastGreyWater = "ERROR Check Tank";
  }

  if (Quarter == LOW && Half == LOW && ThreeQuarters == LOW && Full == LOW) {
    LastGreyWater = "Empty";
    GreyGettinFull = false;
  }

  if (Quarter == HIGH && Half == LOW && ThreeQuarters == LOW && Full == LOW) {
    LastGreyWater = "1/4";
    GreyGettinFull = false;
  }

  if (Quarter == HIGH && Half == HIGH && ThreeQuarters == LOW && Full == LOW) {
    LastGreyWater = "1/2";
    GreyGettinFull = false;
  }

  if (Quarter == HIGH && Half == HIGH && ThreeQuarters == HIGH && Full == LOW) {
    LastGreyWater = "3/4";
    GreyGettinFull = true;
  }

  if (Quarter == HIGH && Half == HIGH && ThreeQuarters == HIGH && Full == HIGH) {
    LastGreyWater = "Full";
  }

  LastTimeGreyWater = GetCurrentTime();


  if (GreyGettinFull == false && WaterOn == false) {
    // Turn Off Voltage to tank
    digitalWrite(GreyWaterPowerRelay, LOW);
  }

}

void ReadWaterAndLPG() {
  // Turn On votlage to tanks
  digitalWrite(TankPowerRelay, HIGH);
  delay(150);
  int R2 = 47;
  float Vout = ConversionFactor * analogRead(LPGSensor);
  String LastTimeLPGLevel = GetCurrentTime();
  float R1LPG = R2 * ((5.0 / Vout) - 1);
  LastLPGLevel = map(R1LPG, 0, 122, 0, 100);

  LastWaterLevel = "EXTRA FULL";
  LastTimeWaterLevel = GetCurrentTime();
  Vout = ConversionFactor * analogRead(WaterTankSensor);
  float R1 = R2 * ((5.0 / Vout) - 1);
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

void ReadCamperBatteryVoltage() {
  int Samples = 50;
  int DCVoltageSum = 0;

  for (int x = 0; x < Samples; x++) {
    DCVoltageSum = DCVoltageSum + analogRead(Camper12VoltSensor);
  }
  LastDCVoltage = 0.0; // Do some math here
  LastTimeDCVoltage = GetCurrentTime();
}

void ReadOtherTempSensors() {
  int R2 = 10000;
  float c1 = 1.009249522e-03;
  float c2 = 2.378405444e-04;
  float c3 = 2.019202697e-07;

  float VoutACF = ConversionFactor * analogRead(FrontACTemp);
  float R1ACF = log(R2 * ((5.0 / VoutACF) - 1));
  LastFrontACTemp = ConvertCtoF((((1.0 / (c1 + c2 * R1ACF + c3 * R1ACF * R1ACF * R1ACF))) - 273.15));

  float VoutACB = ConversionFactor * analogRead(BackACTemp);
  float R1ACB = log(R2 * ((5.0 / VoutACB) - 1));
  LastBackACTemp = ConvertCtoF((((1.0 / (c1 + c2 * R1ACB + c3 * R1ACB * R1ACB * R1ACB))) - 273.15));

  float VoutHallway = ConversionFactor * analogRead(HallwayTemp);
  float R1Hallway = log(R2 * ((5.0 / VoutHallway) - 1));
  LastHallwayTemp = ConvertCtoF((((1.0 / (c1 + c2 * R1Hallway + c3 * R1Hallway * R1Hallway * R1Hallway))) - 273.15));

  float VoutBathroom = ConversionFactor * analogRead(BathroomTemp);
  float R1Bathroom = log(R2 * ((5.0 / VoutBathroom) - 1));
  LastBathroomTemp = ConvertCtoF((((1.0 / (c1 + c2 * R1Bathroom + c3 * R1Bathroom * R1Bathroom * R1Bathroom))) - 273.15));

  float VoutFreezer = ConversionFactor * analogRead(Freezer);
  float R1Freezer = log(R2 * ((5.0 / VoutFreezer) - 1));
  LastFreezerTemp = ConvertCtoF((((1.0 / (c1 + c2 * R1Freezer + c3 * R1Freezer * R1Freezer * R1Freezer))) - 273.15));

  float VoutFridge = ConversionFactor * analogRead(Refridgerator);
  float R1Fridge = log(R2 * ((5.0 / VoutFridge) - 1));
  LastFridgeTemp = ConvertCtoF((((1.0 / (c1 + c2 * R1Fridge + c3 * R1Fridge * R1Fridge * R1Fridge))) - 273.15));

  float VoutOutside = ConversionFactor * analogRead(Outside);
  float R1Outside = log(R2 * ((5.0 / VoutOutside) - 1));
  LastOutsideTemp = ConvertCtoF((((1.0 / (c1 + c2 * R1Outside + c3 * R1Outside * R1Outside * R1Outside))) - 273.15));

  float VoutBackCabin = ConversionFactor * analogRead(BackCabin);
  float R1BackCabin = log(R2 * ((5.0 / VoutBackCabin) - 1));
  LastBackCabinTemp = ConvertCtoF((((1.0 / (c1 + c2 * R1BackCabin + c3 * R1BackCabin * R1BackCabin * R1BackCabin))) - 273.15));

}

float ConvertCtoF(float C) {
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
