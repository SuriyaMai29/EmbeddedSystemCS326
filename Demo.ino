#include <TimerOne.h>
#define LDRPhotoresistorSensor  A4
#define WaterTurbiditySensor A1
#define WaterLavelSensor1 A0
#define WaterLavelSensor12 A6
#define Pin0 14
#define Pin1 15  
#define Pin2 16
#define Pin3 17 
bool IsActive_state = true;
bool lightState;
bool waterTurbidityState;
bool waterLavelState;

int valueOfPhotoresistorSensor=0;
const int numReadingLDR = 100;
int readingsLDR [numReadingLDR];
int readIndexLDR =0;
int totalLDR =0;
int averageLDR =0;
void lighting(bool*state);

int ValueOfWaterTurbiditySensor=0;
const int numReadingWT = 100;
int readingsWT [numReadingWT];
int readIndexWT =0;
int totalWT =0;
int averageWT =0;
void waterTurbidity(bool *state);

int ValueOfWaterLavel=0;
const int numReadingLV = 10;
int readingsLV [numReadingLV];
int readIndexLV =0;
int totalLV =0;
int averageLV =0;
void intitLV();
void lavel(bool * state);
boolean isExposedToWater();
void setState();

int sensorMax = 0, sensorMin = 1023;


void setup() {
  Serial.begin(9500);
  intit();
  for(int i =0;i<numReadingLDR;i++)
  readingsLDR[i]=0;
  for(int i =0;i<numReadingWT;i++)
  readingsWT[i]=0;
  for(int i =0;i<numReadingLV;i++)
  readingsLV[i]=0;
 
  intitLDR();
  intitWT();
  intitLV();
  setState();
}

void loop() {
    waterTurbidity(&waterTurbidityState);
    while(!isExposedToWater()){
    Serial.println("tank over.....refilling");
    digitalWrite(Pin2,LOW);
    delay(10000);
  }
  digitalWrite(Pin2,HIGH);
  if(IsActive_state){
     lavel(&waterLavelState);
     waterTurbidity(&waterTurbidityState);
     lighting(&lightState);
    if(lightState){
      Serial.println("light mode");
      digitalWrite(Pin0,HIGH);
    }else{
      Serial.println("dark mode");
      digitalWrite(Pin0,LOW);
    }
    while(!waterLavelState){
      Serial.println("water out");
      digitalWrite(Pin1,LOW);
      delay(30000);
      lavel(&waterLavelState);
    }
     Serial.println("water in");
     digitalWrite(Pin1,HIGH);
     
    if(waterLavelState && waterTurbidityState){
      IsActive_state = false;
    }else IsActive_state = true;
  }else{
   Serial.println("water out pumping...");
    digitalWrite(Pin3,LOW);
    delay(60UL * 60UL * 1000UL); //1hr
    digitalWrite(Pin3,HIGH);
    Serial.println("InActive...");
    Timer1.initialize(5000000);  
    Timer1.attachInterrupt(refilling);
   do{
    digitalWrite(Pin1,LOW);
    delay(5000);
    lavel(&waterLavelState);
   }while(!waterLavelState);
   Timer1.stop();
   digitalWrite(Pin2,HIGH);
   digitalWrite(Pin1,HIGH);

    Serial.println("seting...");
    delay(30UL * 60UL * 1000UL);
    IsActive_state = true;
  }
delay(1000);
}
void intit(){
  IsActive_state = true;
  lightState = true;
  waterLavelState= true;
  waterTurbidityState=false;
  pinMode(LDRPhotoresistorSensor,INPUT);
  pinMode(WaterTurbiditySensor,INPUT);
  pinMode(WaterLavelSensor1,INPUT);
  pinMode(WaterLavelSensor12,INPUT);
  pinMode(Pin0,OUTPUT);
  pinMode(Pin1,OUTPUT);
  pinMode(Pin2,OUTPUT);
  pinMode(Pin3,OUTPUT);
  digitalWrite(Pin0,HIGH);
  digitalWrite(Pin1,HIGH);
  digitalWrite(Pin2,HIGH);
  digitalWrite(Pin3,HIGH);
}
void intitLDR(){
  for(int i =0 ;i< numReadingLDR;i++){
  valueOfPhotoresistorSensor = analogRead(LDRPhotoresistorSensor);
  delay(100);
  totalLDR = totalLDR - readingsLDR[readIndexLDR];
  readingsLDR[readIndexLDR] = valueOfPhotoresistorSensor;
  totalLDR =totalLDR+readingsLDR[readIndexLDR];
  readIndexLDR = readIndexLDR+1;

  if(readIndexLDR>= numReadingLDR)
  readIndexLDR =0;
  averageLDR = totalLDR/numReadingLDR;
  }
}
void intitWT(){
  for(int i =0 ;i< numReadingWT;i++){
  ValueOfWaterTurbiditySensor = analogRead(WaterTurbiditySensor);
  delay(100);
  totalWT = totalWT - readingsWT[readIndexWT];
  readingsWT[readIndexWT] = ValueOfWaterTurbiditySensor;
  totalWT =totalWT+readingsWT[readIndexWT];
  readIndexWT = readIndexWT+1;

  if(readIndexWT>= numReadingWT)
  readIndexWT =0;
  averageWT = totalWT/numReadingWT;
  }
}
void intitLV(){
  for(int i =0;i< numReadingLV;i++){
  ValueOfWaterLavel = analogRead(WaterLavelSensor1);
  delay(100);
  totalLV = totalLV - readingsLV[readIndexLV];
  readingsLV[readIndexLV] = ValueOfWaterLavel;
  totalLV =totalLV+readingsLV[readIndexLV];
  readIndexLV = readIndexLV+1;

  if(readIndexLV>= numReadingLV)
  readIndexLV =0;
  averageLV = totalLV/numReadingLV;
  }
}

void waterTurbidity(bool *state){
  ValueOfWaterTurbiditySensor = analogRead(WaterTurbiditySensor);
  totalWT = totalWT - readingsWT[readIndexWT];
  readingsWT[readIndexWT] = ValueOfWaterTurbiditySensor;
  totalWT =totalWT+readingsWT[readIndexWT];
  readIndexWT = readIndexWT+1;

  if(readIndexWT>= numReadingWT)
  readIndexWT =0;
  averageWT = totalWT/numReadingWT;

  Serial.println(ValueOfWaterTurbiditySensor);
  if(ValueOfWaterTurbiditySensor >= 550 )
  *state = false;
  else if(ValueOfWaterTurbiditySensor <= 540)
  *state = true;
  
   
}
void lighting(bool *state){
  valueOfPhotoresistorSensor = analogRead(LDRPhotoresistorSensor);
  delay(100);
  totalLDR = totalLDR - readingsLDR[readIndexLDR];
  readingsLDR[readIndexLDR] = valueOfPhotoresistorSensor;
  totalLDR =totalLDR+readingsLDR[readIndexLDR];
  readIndexLDR = readIndexLDR+1;

  if(readIndexLDR>= numReadingLDR)
  readIndexLDR =0;
  averageLDR = totalLDR/numReadingLDR;
  if(valueOfPhotoresistorSensor<=150&&valueOfPhotoresistorSensor>0)
  *state = true;
  else if(valueOfPhotoresistorSensor>=200 || valueOfPhotoresistorSensor <0)
  *state = false;
}
void lavel(bool * state){
  ValueOfWaterLavel = analogRead(WaterLavelSensor1);
  delay(100);
  totalLV = totalLV - readingsLV[readIndexLV];
  readingsLV[readIndexLV] = ValueOfWaterLavel;
  totalLV =totalLV+readingsLV[readIndexLV];
  readIndexLV = readIndexLV+1;

  if(readIndexLV>= numReadingLV)
  readIndexLV =0;
  averageLV = totalLV/numReadingLV;
 // Serial.println(averageLV);
  if(ValueOfWaterLavel>=320)
  *state = true;
  else *state=false;
}
boolean isExposedToWater(){
  return (analogRead(WaterLavelSensor12) >= 390)? true: false;
}
void setState(){
  Serial.println("setup");
  for(int i=0 ;i<300;i++){
  lavel(&waterLavelState);
  waterTurbidity(&waterTurbidityState);
  }
}
void refilling(){
  Serial.println("refilling...");
  if(!isExposedToWater()){
     digitalWrite(Pin2,LOW);
  }
  else{
    Serial.println("of");
    digitalWrite(Pin2,HIGH);
  }
}
