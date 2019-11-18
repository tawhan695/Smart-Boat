/*
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
1.การต่อขา gps เข้ากับ arduino
          arduino || GPS 
              5V -> Vcc
              GND -> GND
              ขา15 -> Tx
              ขา14 -> Rx

2.serial การส่งข้อมูลของ arduino

          arduino Mega 2560   ||   wemos mini d1 เป็นตัวรับไวไฟ
                    ขา     16 ->  ขา D1
                    ขา     17 -> ขา  D2
          
          เป็นการส่อสารแบบอนุกรม

3.

4.

5.

6.

$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/

int pump_in  = 12;
int pump_out = 13;

int Status = 0;

const int in1Pin = 5;  // H-Bridge input pins

const int in2Pin = 4;

const int in3Pin = 3;  // H-Bridge pins for second motor

const int in4Pin = 2;
String DDD;

#include <avr/pgmspace.h>
#include <EEPROM.h>


#include <TinyGPS++.h>
TinyGPSPlus gps;

float longitude;
String incomingByte;
float latitude;


int setTime = 0;
unsigned long previousTime = 3000; 


unsigned long previousMillis = 0;        
int sec = 0, minn = 0;




/*############################################################################
 
                                    config sensor
                                     
############################################################################*/
int DO =A0;
int PH =A1;
int EC =A3;

int Delay =32000;


/*############################################################################
 
                                     DO SENSOR
                                     
############################################################################*/

#define DoSensorPin  A0    //dissolved oxygen sensor analog output pin to arduino mainboard
#define VREF 5000    //for arduino uno, the ADC reference is the AVCC, that is 5000mV(TYP)
float doValue;      //current dissolved oxygen value, unit; mg/L
float temperature = 25;    //default temperature is 25^C, you can use a temperature sensor to read it

#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

#define ReceivedBufferLength 20
char receivedBuffer[ReceivedBufferLength+1];    // store the serial command
byte receivedBufferIndex = 0;

#define SCOUNT  30           // sum of sample point
int analogBuffer[SCOUNT];    //store the analog value in the array, readed from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;

#define SaturationDoVoltageAddress 12          //the address of the Saturation Oxygen voltage stored in the EEPROM
#define SaturationDoTemperatureAddress 16      //the address of the Saturation Oxygen temperature stored in the EEPROM
float SaturationDoVoltage,SaturationDoTemperature;
float averageVoltage;

const float SaturationValueTab[41] PROGMEM = {      //saturation dissolved oxygen concentrations at various temperatures
14.46, 14.22, 13.82, 13.44, 13.09,
12.74, 12.42, 12.11, 11.81, 11.53,
11.26, 11.01, 10.77, 10.53, 10.30,
10.08, 9.86,  9.66,  9.46,  9.27,
9.08,  8.90,  8.73,  8.57,  8.41,
8.25,  8.11,  7.96,  7.82,  7.69,
7.56,  7.43,  7.30,  7.18,  7.07,
6.95,  6.84,  6.73,  6.63,  6.53,
6.41,
};


/*############################################################################
 
                                   temp SENSOR
                                     
############################################################################*/
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 10
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
/*############################################################################
 
                                     orp SENSOR
                                     
############################################################################*/
#define VOLTAGE 5.00 //system voltage
#define OFFSET 0 //zero drift voltage
//#define LED 13 //operating instructions
double orpValue;
#define ArrayLenth 40 //times of collection
#define orpPin A2 //orp meter output,connect to Arduino controller ADC pin
int orpArray[ArrayLenth];
int orpArrayIndex=0;

void setup() {
 

  // put your setup code here, to run once:
Serial2.begin(19200);
Serial3.begin(9600);
Serial.begin(115200);


pinMode(pump_in, OUTPUT);
pinMode(pump_out, OUTPUT);

 digitalWrite(pump_in,1);
  digitalWrite(pump_out,1);
   pinMode(DoSensorPin,INPUT);
   readDoCharacteristicValues(); 

  pinMode(in1Pin, OUTPUT);

  pinMode(in2Pin, OUTPUT);

  pinMode(in3Pin, OUTPUT);

  pinMode(in4Pin, OUTPUT);
}

void loop() {
DOSENSOR();

   //#################################################################################3
/*
                                  รับค่า จากบอร์ด wemos
*/
 //#################################################################################

if (Serial2.available() > 0) {
   incomingByte = Serial2.readString();
   //Serial.println(incomingByte);
   
   if(incomingByte.toInt()==10){
     Serial.println("Moter  up");

      digitalWrite(in1Pin,HIGH);

      digitalWrite(in2Pin,LOW);

      digitalWrite(in3Pin,0);

      digitalWrite(in4Pin,1);
     
   }else if(incomingByte.toInt()==11){
    
     Serial.println("Moter  down");

      digitalWrite(in1Pin,LOW);

      digitalWrite(in2Pin,HIGH);

      digitalWrite(in3Pin,1);

      digitalWrite(in4Pin,0);
   }else if (incomingByte.toInt()==100){
     Serial.println("Moter L OFF");
      digitalWrite(in1Pin,LOW);

      digitalWrite(in2Pin,LOW);

      digitalWrite(in3Pin,LOW);

      digitalWrite(in4Pin,LOW);
   }

   if(incomingByte.toInt()==20){
      Serial.println("Moter L");
      digitalWrite(in1Pin,0);

      digitalWrite(in2Pin,0);

      digitalWrite(in3Pin,0);

      digitalWrite(in4Pin,1);
    
   }else if(incomingByte.toInt()==21){
       Serial.println("Moter R ");
       digitalWrite(in1Pin,1);

      digitalWrite(in2Pin,0);

      digitalWrite(in3Pin,0);

      digitalWrite(in4Pin,LOW);
   }else if(incomingByte.toInt()==200){

        Serial.println("Moter R OFF");
      digitalWrite(in1Pin,LOW);

      digitalWrite(in2Pin,LOW);

      digitalWrite(in3Pin,LOW);

      digitalWrite(in4Pin,LOW);
   }

   if(incomingByte.toInt()==500){

    if(Status==0){
        digitalWrite(pump_in,0);
        delay(33000);
        digitalWrite(pump_in,1);
        Status=1;
    }

    
   }else if(incomingByte.toInt()==600){

      if(Status==1){
        digitalWrite(pump_out,0);
        delay(34900);
        digitalWrite(pump_out,1);
        Status=0;
    }
    
   }

   int VolueCAR=0;
  if(incomingByte.toInt()==999){
    VolueCAR=1;
  }else if(incomingByte.toInt()==888){
     VolueCAR=2;
  }
       if(VolueCAR==2){
      doCalibration(2);
      delay(10000);
       doCalibration(3);
     }
     if(VolueCAR==1){
      doCalibration(1);
     }

//   if(incomingByte.toInt()==40){
//    
//   }else if(incomingByte.toInt()==41){
//    
//   }
  }

 //#################################################################################3

 //                                     ส่งค่า GPS

 //#################################################################################
 if (Serial3.available() > 0){
    if (gps.encode(Serial3.read())){
 unsigned long currentMillis = millis();
 
    if(currentMillis - previousMillis >= 3000) {
      //Serial2.println(displayInfo());
         //Serial.println(displayInfo());
                     String Do=String(DDD);
                     Serial.println(Do);
//                    //****************
                    int Iph=analogRead(PH);
                    int pH=map(Iph,0,1023,1,14);
                     String Ph=String(pH);
                     Serial.println(Ph);
//                     //****************
                    String Temp=String(temp());
                     Serial.println(Temp);
//                     //****************
//                    int ec=analogRead(EC);
//                    int Ec=map(ec,0,1023,0,20);//Measuring range: 1ms/cm--20ms/cm
//                    String eC=String(Ec);
//                     Serial.println(eC);
//                     //****************
//                
                     String orp=String(ORP_1());
                     Serial.println(orp);
//                     //****************

                     
    String data;
    data +=Do;data +=",";
    data +=Ph;data +=",";
//    data +=eC;data +=",";
    data +=orp;data +=",";
    data +=Temp;data +=",";
    data +=displayInfo();data +=",";
    
    Serial2.println(data);
      previousMillis = currentMillis;
    }
 
    }
 }
  //###############################################################################

 //                                   อ่านค่าจาก seonor

 //################################################################################



}



  //###############################################################################

 //                                    รับค่าจาก GPS

 //################################################################################
String displayInfo()
{
  if (gps.location.isValid())
  {
latitude = gps.location.lat();
longitude = gps.location.lng();
  }
  String buf;
  buf += String(latitude, 7);
  buf += F(",");
  buf += String(longitude, 7);
  //Serial.println(buf);
  

return buf;
}

/*############################################################################
 
                                   temp SENSOR
                                     
############################################################################*/
 float temp()
{
  sensors.requestTemperatures(); 
 return sensors.getTempCByIndex(0);
}

/*############################################################################
 
                                     orp SENSOR
                                     
############################################################################*/
double avergearray(int* arr, int number){
int i;
int max,min;
double avg;
long amount=0;
if(number<=0){
printf("Error number for the array to avraging!/n");
return 0;
}
if(number<5){ //less than 5, calculated directly statistics
for(i=0;i<number;i++){
amount+=arr[i];
}
avg = amount/number;
return avg;
}else{
if(arr[0]<arr[1]){
min = arr[0];max=arr[1];
}
else{
min=arr[1];max=arr[0];
}
for(i=2;i<number;i++){
if(arr[i]<min){
amount+=min; //arr<min
min=arr[i];
}else {
if(arr[i]>max){
amount+=max; //arr>max
max=arr[i];
}else{
amount+=arr[i]; //min<=arr<=max
}
}//if
}//for
avg = (double)amount/(number-2);
}//if
return avg;
}

int ORP_1() {
static unsigned long orpTimer=millis(); //analog sampling interval
static unsigned long printTime=millis();
if(millis() >= orpTimer)
{
orpTimer=millis()+20;
orpArray[orpArrayIndex++]=analogRead(orpPin); //read an analog value every 20ms
if (orpArrayIndex==ArrayLenth) {
orpArrayIndex=0;
}
orpValue=((30*(double)VOLTAGE*1000)-(75*avergearray(orpArray, ArrayLenth)*VOLTAGE*1000/1024))/75-OFFSET;

//convert the analog value to orp according the circuit
}
if(millis() >= printTime) //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
{
printTime=millis()+800;
//Serial.println((int)orpValue);
return ((int)orpValue);
}
}


/*############################################################################
 
                                     do SENSOR
                                     
############################################################################*/

void DOSENSOR()
{
   static unsigned long analogSampleTimepoint = millis();
   if(millis()-analogSampleTimepoint > 30U)     //every 30 milliseconds,read the analog value from the ADC
   {
     analogSampleTimepoint = millis();
     analogBuffer[analogBufferIndex] = analogRead(DoSensorPin);    //read the analog value and store into the buffer
     analogBufferIndex++;
     if(analogBufferIndex == SCOUNT)
         analogBufferIndex = 0;
   }

   static unsigned long tempSampleTimepoint = millis();
   if(millis()-tempSampleTimepoint > 500U)  // every 500 milliseconds, read the temperature
   {
      tempSampleTimepoint = millis();
      //temperature = readTemperature();  // add your temperature codes here to read the temperature, unit:^C
   }

   static unsigned long printTimepoint = millis();
   if(millis()-printTimepoint > 1000U)
   {
      printTimepoint = millis();
      for(copyIndex=0;copyIndex<SCOUNT;copyIndex++)
      {
        analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
      }
      averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 1024.0; // read the value more stable by the median filtering algorithm

      doValue = pgm_read_float_near( &SaturationValueTab[0] + (int)(SaturationDoTemperature+0.5) ) * averageVoltage / SaturationDoVoltage;  //calculate the do value, doValue = Voltage / SaturationDoVoltage * SaturationDoValue(with temperature compensation)

     DDD=String(doValue);
//     Serial.print("Do");
//     Serial.println(doValue);


   if(serialDataAvailable() > 0)
   {
      byte modeIndex = uartParse();  //parse the uart command received
      doCalibration(modeIndex);    // If the correct calibration command is received, the calibration function should be called.
   }

}
}

boolean serialDataAvailable(void)
{
  char receivedChar;
  static unsigned long receivedTimeOut = millis();
  while ( Serial2.available() > 0 )
  {
    if (millis() - receivedTimeOut > 500U)
    {
      receivedBufferIndex = 0;
      memset(receivedBuffer,0,(ReceivedBufferLength+1));
    }
    receivedTimeOut = millis();
    receivedChar = Serial2.read();
    if (receivedChar == '\n' || receivedBufferIndex == ReceivedBufferLength)
    {
    receivedBufferIndex = 0;
    strupr(receivedBuffer);
    return true;
    }else{
        receivedBuffer[receivedBufferIndex] = receivedChar;
        receivedBufferIndex++;
    }
  }
  return false;
}

byte uartParse()
{
    byte modeIndex = 0;
    if(strstr(receivedBuffer, "CALIBRATION") != NULL)
        modeIndex = 1;
    else if(strstr(receivedBuffer, "EXIT") != NULL)
        modeIndex = 3;
    else if(strstr(receivedBuffer, "SATCAL") != NULL)
        modeIndex = 2;
    return modeIndex;
}

void doCalibration(byte mode)
{
    char *receivedBufferPtr;
    static boolean doCalibrationFinishFlag = 0,enterCalibrationFlag = 0;
    float voltageValueStore;
    switch(mode)
    {
      case 0:
      if(enterCalibrationFlag)
         Serial.println(F("Command Error"));
      break;

      case 1:
      enterCalibrationFlag = 1;
      doCalibrationFinishFlag = 0;
      Serial.println();
      Serial.println(F(">>>Enter Calibration Mode<<<"));
      Serial.println(F(">>>Please put the probe into the saturation oxygen water! <<<"));
      Serial.println();
    /////   lcd.clear();
    //   lcd.print(0,0,"PleasePutTheProbeInto");
     //  lcd.print(0,2,"the oxygen water");
  
      break;

     case 2:
      if(enterCalibrationFlag)
      {
         Serial.println();
         Serial.println(F(">>>Saturation Calibration Finish!<<<"));
         Serial.println();
       //     lcd.clear();
      // lcd.print(0,0,"Saturation");
     //  lcd.print(0,2," Finish!");
         EEPROM_write(SaturationDoVoltageAddress, averageVoltage);
         EEPROM_write(SaturationDoTemperatureAddress, temperature);
         SaturationDoVoltage = averageVoltage;
         SaturationDoTemperature = temperature;
         doCalibrationFinishFlag = 1;
      }
      break;

        case 3:
        if(enterCalibrationFlag)
        {
            Serial.println();
            if(doCalibrationFinishFlag)
               Serial.print(F(">>>Calibration Successful"));
                     //  lcd.clear();
     //  lcd.print(0,0,"Successful");
        }
            else{
              Serial.print(F(">>>Calibration Failed"));
            Serial.println(F(",Exit Calibration Mode<<<"));
            Serial.println();
         //           lcd.clear();
     //  lcd.print(0,0,"Calibration Failed");
       //lcd.print(0,2," Finish!");
            doCalibrationFinishFlag = 0;
            enterCalibrationFlag = 0;
        }
        break;
    }
}

int getMedianNum(int bArray[], int iFilterLen)
{
      int bTab[iFilterLen];
      for (byte i = 0; i<iFilterLen; i++)
      {
      bTab[i] = bArray[i];
      }
      int i, j, bTemp;
      for (j = 0; j < iFilterLen - 1; j++)
      {
      for (i = 0; i < iFilterLen - j - 1; i++)
          {
        if (bTab[i] > bTab[i + 1])
            {
        bTemp = bTab[i];
            bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
         }
      }
      }
      if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
      else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
      return bTemp;
}

void readDoCharacteristicValues(void)
{
    EEPROM_read(SaturationDoVoltageAddress, SaturationDoVoltage);
    EEPROM_read(SaturationDoTemperatureAddress, SaturationDoTemperature);
    if(EEPROM.read(SaturationDoVoltageAddress)==0xFF && EEPROM.read(SaturationDoVoltageAddress+1)==0xFF && EEPROM.read(SaturationDoVoltageAddress+2)==0xFF && EEPROM.read(SaturationDoVoltageAddress+3)==0xFF)
    {
      SaturationDoVoltage = 1127.6;   //default voltage:1127.6mv
      EEPROM_write(SaturationDoVoltageAddress, SaturationDoVoltage);
    }
    if(EEPROM.read(SaturationDoTemperatureAddress)==0xFF && EEPROM.read(SaturationDoTemperatureAddress+1)==0xFF && EEPROM.read(SaturationDoTemperatureAddress+2)==0xFF && EEPROM.read(SaturationDoTemperatureAddress+3)==0xFF)
    {
      SaturationDoTemperature = 25.0;   //default temperature is 25^C
      EEPROM_write(SaturationDoTemperatureAddress, SaturationDoTemperature);
    }
}
