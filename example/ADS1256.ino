#include "ADS1256.h"
/*
    Name:       ADS1256.ino
	Created:	26/03/2020 
	Author:     Davide Romeo davideromeo93k@gmail.com 

    PIN ADS1256 |      |PIN ARDUINO 

	SCLK - 24   |----->|13 - MCLK    
	DOUT - 22   |----->|12 - MISO
	DIN  - 23   |----->|11 - MOSI
	CS   - 20   |----->|10 - CS
	DRDY - 21   |----->|9  - DRDY
	RESET- 15   |----->|8  - RESET
	AVDD----->+5V
	AGND----->GND
	DVDD----->+3.3V
	DGND----->GND
	VREFP---->V_REF
	VREFN---->GND
	AINCOM--->GND
*/
#define CS          10 
#define DRDY         9 
#define RESET_       8 
#define V_REF     2.43
#define Mhz       7.68 //ADS1256 clock
//class
ADS1256           ADS(CS, DRDY, RESET_, V_REF, Mhz);
float             d_t;
void setup()
{
	Serial.begin(115200);
	ADS.begin(SPS_25, PGA_1);
	Serial.println("ADS1256 ready...");
}

void loop()
{
	//cycle_for(100);                       //Scegli il tipo di ciclo che vuoi usare commentando gli altri 2.
	//ADS.cycle_while(1000);                // Choose the type of cycle you want to use by commenting on the other 2.
	read_simple(P_AIN0);
}

void read_simple(unsigned char P_AIN)       //Lettura normale, scegli il canale di lettura e stampa i risultati.
{                                           // Normal reading, choose the reading channel and print the results.
	float         t0 = micros();
	int        count = 0;
	double       sum = 0;
	Serial.print(ADS.read_mV(P_AIN), 3);
	Serial.print("mV");
	Serial.print(" , SPS = ");
	float t1 = micros();
	d_t = (t1 - t0) / 1000;
	Serial.print(1000 / d_t);
	Serial.print(" , \u0394t = ");
	Serial.print(d_t, 2);
	Serial.println(" ms");
}
void cycle_for(int n)                      //Ciclo "for". Calcola la media di un campione n di letture e stampa i risultati.
{                                          //"for" cycle. Calculate the average of a sample n of readings and print the results.
	float         t0 = micros();           
	int        count = 0;
	double       sum = 0;
	for (int i = 0; i < n; i++) {
		sum += ADS.read_mV(P_AIN0);
		count++;
	}
	//
	float a = (sum / count);
	Serial.print(a, 3);
	Serial.print("mV");
	Serial.print(" , SPS = ");
	float t1 = micros();
	d_t = (t1 - t0) / 1000;
	Serial.print(count * (1000 / d_t));
	Serial.print(" , \u0394t = ");
	Serial.print(d_t, 2);
	Serial.println(" ms");
}

void cycle_while(int sample_time)            //Ciclo "while". Calcola la media dei valori letti in un intervallo di tempo e stampa i risultati.
{                                            //Cycle "while". Calculate the average of the values ​​read over a period of time and print the results.
	float         t0 = micros();
	int        count = 0;
	double       sum = 0;
	long          tx = millis();
	while (millis() - tx < sample_time) {
		sum += ADS.read_mV(P_AIN0);
		count++;
	}
	//
	float a = (sum / count);
	Serial.print(a, 3);
	Serial.print("mV");
	Serial.print(" , SPS = ");
	float t1 = micros();
	d_t = (t1 - t0) / 1000;
	Serial.print(count * (1000 / d_t), 2);
	Serial.print(" , \u0394t = ");
	Serial.print(d_t, 2);
	Serial.println(" ms");
}




