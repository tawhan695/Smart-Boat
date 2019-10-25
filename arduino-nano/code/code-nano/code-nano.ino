/*############################################################################
 
                                     DO SENSOR
                                     
############################################################################*/
#include <avr/pgmspace.h>
#include <EEPROM.h>
String DDD="";
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

int Cardi=A7;
int VolueCAR;


/*############################################################################
 
                                    config sensor
                                     
############################################################################*/
int DO =A0;
int PH =A1;
int EC =A3;


/*############################################################################
 
                                   temp SENSOR
                                     
############################################################################*/
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4
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




  pinMode(DoSensorPin,INPUT);
   readDoCharacteristicValues();
   pinMode(A4, INPUT_PULLUP);
   pinMode(8, INPUT_PULLUP);
    pinMode(Cardi, INPUT_PULLUP);

}

void loop() { // run over and over

     Serial.print(DDD);
     Serial.println(("mg/L"));
 DOSENSOR();
 digitalWrite(7,1);

if((analogRead(Cardi)>100)&&(VolueCAR==0)){
       
        if((y==1)&&(VolueCAR==0)){
          VolueCAR=2;
          y+=1;
        }
        else if(y==0){
          VolueCAR=1;
            y+=1;
        }
         Serial.println(VolueCAR);
         Serial.print("y " );
         Serial.println(y);
     }    
     else{
       VolueCAR=0;
        Serial.print("         y :" );
        Serial.println(y);
        
     }
     if(VolueCAR==2){
      doCalibration(2);
      delay(10000);
       doCalibration(3);
     }
     if(VolueCAR==1){
      doCalibration(1);
     }

 
                     //****************
                    //int IDo=500;
                    //analogRead(A0);
                    //int Do1=0;
                    String Do=String(DDD);
                    //****************
                    int Iph=analogRead(A1);
                    int pH=map(Iph,0,1023,1,14);
                     String Ph=String(pH);
                     //****************
                    String Temp=String(temp());
                     //****************
                    int ec=analogRead(A3);
                    int Ec=map(ec,0,1023,0,20);//Measuring range: 1ms/cm--20ms/cm
                    String eC=String(Ec);
                     //****************
                
                    String orp=String(ORP_1());
                     //****************

                     
    String data;
    data +=Do;data +=",";
    data +=Ph;data +=",";
    data +=eC;data +=",";
    data +=orp;data +=",";
    data +=Temp;data +=",";
    data +=GPSS();data +=",";
    
sensorVal = digitalRead(A4);
    enstop = digitalRead(8);;
   // int Fenstop = digitalRead(8);
   //  Serial.println(Fenstop);
   //delay(1000);
   //Serial.println(data);
   currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
   // Serial.println(data);
    sensor.print(data); //if so, change the state of the LED.  Uses a neat trick to change the state
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }
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

     DDD=doValue;


   if(serialDataAvailable() > 0)
   {
      byte modeIndex = uartParse();  //parse the uart command received
      doCalibration(modeIndex);    // If the correct calibration command is received, the calibration function should be called.
   }

}

boolean serialDataAvailable(void)
{
  char receivedChar;
  static unsigned long receivedTimeOut = millis();
  while ( sensor.available() > 0 )
  {
    if (millis() - receivedTimeOut > 500U)
    {
      receivedBufferIndex = 0;
      memset(receivedBuffer,0,(ReceivedBufferLength+1));
    }
    receivedTimeOut = millis();
    receivedChar = sensor.read();
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
