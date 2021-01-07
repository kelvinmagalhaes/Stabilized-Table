//code original de Transductor, modifi� por El_Xed
#include <Wire.h>
#include <Servo.h>

Servo servo1;
Servo servo2;

// Adresse I2C du MPU 6050:
#define MPU 0x68

// Constante:
#define A_R 16384.0
#define G_R 131.0

//Conversion de radianes a grados 180/PI
#define RAD_A_DEG = 57.295779

//MPU-6050 valeurs de 16 bits
// Acceleration + gyro
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
int val1;
int val2;

float Acc[2];
float Gy[2];
float Angle[2];


void setup()
{
	Wire.begin();
	Wire.beginTransmission(MPU);
	Wire.write(0x6B);
	Wire.write(0);
	Wire.endTransmission(true);
	servo1.attach(5);
	servo2.attach(6);
	Serial.begin(38400);

}

void loop()
{
	//valeurs de l'accelerom�tre:
	Wire.beginTransmission(MPU);
	Wire.write(0x3B);               // Registre 0x3B - accelerometre
	Wire.endTransmission(false);
	Wire.requestFrom(MPU,6,true);   //6 lectures
	AcX=Wire.read()<<8|Wire.read(); 
	AcY=Wire.read()<<8|Wire.read();
	AcZ=Wire.read()<<8|Wire.read();

	//angles Y, X 
	Acc[1] = atan((AcX/A_R)/sqrt(pow((AcY/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
	Acc[0] = atan((AcY/A_R)/sqrt(pow((AcX/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
	// proximamente mas proyectos en  http://elxedelectronics.blogspot.mx

	// Valeurs du gyroscope:
	Wire.beginTransmission(MPU);
	Wire.write(0x43);
	Wire.endTransmission(false);
	Wire.requestFrom(MPU,4,true); 
	GyX=Wire.read()<<8|Wire.read();
	GyY=Wire.read()<<8|Wire.read();

	//Calcul angles gyroscope:
	Gy[0] = GyX/G_R;
	Gy[1] = GyY/G_R;

	//Filtre compl�mentaire:
	Angle[0] = 0.98 *(Angle[0]+Gy[0]*0.010) + 0.015*Acc[0];
	Angle[1] = 0.98 *(Angle[1]+Gy[1]*0.010) + 0.028*Acc[1];
	
	int Uangle0 = Angle[0] * 100; // Angle X
	int Uangle1 = Angle[1] * 100; // Angle Y

	Serial.print("Angle X: "); Serial.println(Uangle0);
	Serial.print("Angle Y: "); Serial.println(Uangle1); Serial.println("------------");
	
	float val1 = Uangle0;
	val1 = map(val1, -1900, 1, 31, 135); // -3500, 4500, 48, 132
	Serial.println(val1);
	servo1.write(val1);

	float val2 = Uangle1;
	val2= map(val2, -4000, 1, 31, 135); //-25, 32, 12, 220
	Serial.println(val2);
	servo2.write(val2);
	
	delay(5);
	
}


