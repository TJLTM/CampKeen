const float ConversionFactor = 5.0 / 1023;
#define LPGSensor A1
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
float Vout = ConversionFactor * analogRead(LPGSensor);

Serial.print("Resistence:");
Serial.println(LPGResistence);
if (LPGResistence > 124){
  Serial.println("Check LPG Sensor connections");
}
else{
  Serial.println(map(LPGResistence,0,90,0,100));
}

delay(1000);
}
