/*
  MQUnifiedsensor Library - calibrating an MQ2 to MQ9

  Demonstrates the use a MQ2 sensor.
  Library originally added 01 may 2019
  by Miguel A Califa, Yersson Carrillo, Ghiordy Contreras, Mario Rodriguez
 
  Added example
  modified 27 May 2019
  by Miguel Califa 

 This example code is in the public domain.

*/

//Include the library
#include <MQUnifiedsensor.h>

//Definitions
#define placa "Arduino Mega 2560"
#define Voltage_Resolution 5
#define type "MQ-Board"
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define pin2 A2 //Analog input 2 of your arduino
#define pin3 A3 //Analog input 3 of your arduino
#define pin4 A4 //Analog input 4 of your arduino
#define pin5 A5 //Analog input 5 of your arduino
#define pin6 A6 //Analog input 6 of your arduino
#define pin7 A7 //Analog input 7 of your arduino
#define pin8 A8 //Analog input 8 of your arduino
#define pin9 A9 //Analog input 9 of your arduino

#define RatioMQ2CleanAir 9.83 //RS / R0 = 9.83 ppm 
#define RatioMQ3CleanAir 60   //RS / R0 = 60 ppm 
#define RatioMQ4CleanAir 4.4  //RS / R0 = 4.4 ppm 
#define RatioMQ5CleanAir 6.5  //RS / R0 = 6.5 ppm 
#define RatioMQ6CleanAir 10   //RS / R0 = 10 ppm 
#define RatioMQ7CleanAir 27.5 //RS / R0 = 27.5 ppm  
#define RatioMQ8CleanAir 70   //RS / R0 = 70 ppm   
#define RatioMQ9CleanAir 9.6  //RS / R0 = 9.6 ppm  

//Declare Sensor
MQUnifiedsensor MQ2(placa, Voltage_Resolution, ADC_Bit_Resolution, pin2, type);
MQUnifiedsensor MQ3(placa, Voltage_Resolution, ADC_Bit_Resolution, pin3, type);
MQUnifiedsensor MQ4(placa, Voltage_Resolution, ADC_Bit_Resolution, pin4, type);
MQUnifiedsensor MQ5(placa, Voltage_Resolution, ADC_Bit_Resolution, pin5, type);
MQUnifiedsensor MQ6(placa, Voltage_Resolution, ADC_Bit_Resolution, pin6, type);
MQUnifiedsensor MQ7(placa, Voltage_Resolution, ADC_Bit_Resolution, pin7, type);
MQUnifiedsensor MQ8(placa, Voltage_Resolution, ADC_Bit_Resolution, pin8, type);
MQUnifiedsensor MQ9(placa, Voltage_Resolution, ADC_Bit_Resolution, pin9, type);

//Variables
float  LPG, Alcohol, CH4, H2, CH42, CO, H22, LPG2;

void setup() {
  //Init serial port
  Serial.begin(9600);
  //init the sensor
  MQ2.init();
  MQ2.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ2.setA(574.25); MQ2.setB(-2.222); // Configurate the ecuation values to get LPG concentration
  MQ2.setR0(9.659574468);

  MQ3.init(); 
  MQ3.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ3.setA(4.8387); MQ3.setB(-2.68); // Configurate the ecuation values to get Benzene concentration
  MQ3.setR0(3.86018237);

  MQ4.init(); 
  MQ4.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ4.setA(1012.7); MQ4.setB(-2.786); // Configurate the ecuation values to get CH4 concentration
  MQ4.setR0(3.86018237);

  MQ5.init(); 
  MQ5.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ5.setA(1163.8); MQ5.setB(-3.874); // Configurate the ecuation values to get H2 concentration
  MQ5.setR0(71.100304);
  
  MQ6.init(); 
  MQ6.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ6.setA(2127.2); MQ6.setB(-2.526); // Configurate the ecuation values to get CH4 concentration
  MQ6.setR0(13.4285714);
  
  MQ7.init(); 
  MQ7.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ7.setA(99.042); MQ7.setB(-1.518); // Configurate the ecuation values to get CO concentration
  MQ7.setR0(4);
  
  MQ8.init(); 
  MQ8.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ8.setA(976.97); MQ8.setB(-0.688); // Configurate the ecuation values to get H2 concentration
  MQ8.setR0(1);
  
  MQ9.init(); 
  MQ9.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ9.setA(1000.5); MQ9.setB(-2.186); // Configurate the ecuation values to get LPG concentration
  MQ9.setR0(9.42857143);

   /*****************************  MQ CAlibration ********************************************/ 
  // Explanation: 
  // In this routine the sensor will measure the resistance of the sensor supposing before was pre-heated
  // and now is on clean air (Calibration conditions), and it will setup R0 value.
  // We recomend execute this routine only on setup or on the laboratory and save on the eeprom of your arduino
  // This routine not need to execute to every restart, you can load your R0 if you know the value
  // Acknowledgements: https://jayconsystems.com/blog/understanding-a-gas-sensor
  Serial.print("Calibrating please wait.");
  float MQ2calcR0 = 0, 
  MQ3calcR0 = 0,
  MQ4calcR0 = 0,
  MQ5calcR0 = 0,
  MQ6calcR0 = 0,
  MQ7calcR0 = 0,
  MQ8calcR0 = 0,
  MQ9calcR0 = 0;
  for(int i = 0; i<=20; i ++)
  {
    //Update the voltage lectures
    MQ2.update();
    MQ3.update();
    MQ4.update();
    MQ5.update();
    MQ6.update();
    MQ7.update();
    MQ8.update();
    MQ9.update();

    MQ2calcR0 += MQ2.calibrate(RatioMQ2CleanAir);
    MQ3calcR0 += MQ2.calibrate(RatioMQ3CleanAir);
    MQ4calcR0 += MQ2.calibrate(RatioMQ4CleanAir);
    MQ5calcR0 += MQ2.calibrate(RatioMQ5CleanAir);
    MQ6calcR0 += MQ2.calibrate(RatioMQ6CleanAir);
    MQ7calcR0 += MQ2.calibrate(RatioMQ7CleanAir);
    MQ8calcR0 += MQ2.calibrate(RatioMQ8CleanAir);
    MQ9calcR0 += MQ2.calibrate(RatioMQ9CleanAir);

    Serial.print(".");
  }
  MQ2.setR0(MQ2calcR0/20);
  MQ3.setR0(MQ3calcR0/20);
  MQ4.setR0(MQ4calcR0/20);
  MQ5.setR0(MQ5calcR0/20);
  MQ6.setR0(MQ6calcR0/20);
  MQ7.setR0(MQ7calcR0/20);
  MQ8.setR0(MQ8calcR0/20);
  MQ9.setR0(MQ9calcR0/20);
  Serial.println("  done!.");
  
  Serial.print("Valores de R0 para cada sensor (MQ2 - MQ9):");
  Serial.print(MQ2calcR0/10); Serial.print(" | ");
  Serial.print(MQ3calcR0/10); Serial.print(" | ");
  Serial.print(MQ4calcR0/10); Serial.print(" | ");
  Serial.print(MQ5calcR0/10); Serial.print(" | ");
  Serial.print(MQ6calcR0/10); Serial.print(" | ");
  Serial.print(MQ7calcR0/10); Serial.print(" | ");
  Serial.print(MQ8calcR0/10); Serial.print(" | ");
  Serial.print(MQ9calcR0/10); Serial.println(" |");

  if(isinf(MQ2calcR0) || isinf(MQ3calcR0) || isinf(MQ4calcR0) || isinf(MQ5calcR0)  || isinf(MQ6calcR0) || isinf(MQ7calcR0) || isinf(MQ8calcR0) || isinf(MQ9calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(MQ2calcR0 == 0 || MQ3calcR0 == 0 || MQ4calcR0 == 0 || MQ5calcR0 == 0 || MQ6calcR0 == 0 || MQ7calcR0 == 0 || MQ8calcR0 == 0 || MQ9calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
  /*****************************  MQ CAlibration ********************************************/ 
 
  //Print in serial monitor
  Serial.println("MQ2 to MQ9 - lecture");
  Serial.println("*************************** Lectures from MQ-board ***************************");
  Serial.println("|    LPG   |  Benzene |   CH4    |    H2    |   CH4    |    CO    |    H2    |    LPG   |");  
  Serial.println("|    MQ-2  |   MQ-3   |   MQ-4   |   MQ-5   |   MQ-6   |   MQ-7   |   MQ-8   |   MQ-9   |");  
  //pinMode(calibration_button, INPUT);
}

void loop() {
  //Update the voltage lectures
  MQ2.update();
  MQ3.update();
  MQ4.update();
  MQ5.update();
  MQ6.update();
  MQ7.update();
  MQ8.update();
  MQ9.update();
  /*
  //Rutina de calibracion - Uncomment if you need (setup too and header)
  if(calibration_button)
  {
    float R0 = MQ2.calibrate();
    MQ2.setR0(R0):
    R0 = MQ3.calibrate();
    MQ3.setR0(R0):
    R0 = MQ4.calibrate();
    MQ4.setR0(R0):
    R0 = MQ5.calibrate();
    MQ5.setR0(R0):
    R0 = MQ6.calibrate();
    MQ6.setR0(R0):
    R0 = MQ7.calibrate();
    MQ7.setR0(R0):
    R0 = MQ8.calibrate();
    MQ8.setR0(R0):
    R0 = MQ9.calibrate();
    MQ9.setR0(R0):
  }
  */
  //Read the sensor and print in serial port
  float MQ2Lecture =  MQ2.readSensor();
  float MQ3Lecture =  MQ3.readSensor();
  float MQ4Lecture =  MQ4.readSensor();
  float MQ5Lecture =  MQ5.readSensor();
  float MQ6Lecture =  MQ6.readSensor();
  float MQ7Lecture =  MQ7.readSensor();
  float MQ8Lecture =  MQ8.readSensor();
  float MQ9Lecture =  MQ9.readSensor();

  Serial.print("| "); Serial.print(MQ2Lecture);
  Serial.print("   |   "); Serial.print(MQ3Lecture);
  Serial.print("   |   "); Serial.print(MQ4Lecture);
  Serial.print("   |   "); Serial.print(MQ5Lecture);
  Serial.print("   |   "); Serial.print(MQ6Lecture);
  Serial.print("   |   "); Serial.print(MQ7Lecture);
  Serial.print("   |   "); Serial.print(MQ8Lecture);
  Serial.print("   |   "); Serial.print(MQ9Lecture);
  Serial.println("|");
}
