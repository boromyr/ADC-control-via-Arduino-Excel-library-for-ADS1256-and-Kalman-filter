#include "ADS1256.h"
#include "rExcel.h"
#include "Kalman.h"
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
//macro
#define CS                  10 
#define DRDY                 9 
#define RESET_               8 
#define V_REF             2.43
#define Mhz               8.00
//Class
ADS1256                   ADS(CS, DRDY, RESET_, V_REF, Mhz);
KalmanFilter              Kalman;
rExcel                    myExcel;
//rExcel
long                      idx = 0;
char                      worksheet[16];
char                      range[16];
unsigned int              row;
unsigned int              column;
char                      value[16];
//ADC
float                     mV, m;
double                    K;
int                       SPS, SPS_, gain, gain_, R, Q;
int                       outputTiming = 1000;
int                       N = 10;
//
void setup()
{
	Serial.begin(115200);
	//ADS
	ADS.begin(SPS_15, PGA_1);delay(1);
	//rExcel
	myExcel.clearInput();
	myExcel.clear("rExcel", "A2:E10000");
	myExcel.write("rExcel", "X5", "1");       //Stampa di default appena connesso
	delay(1);                                 //Default print just connected
}

void loop()
{
	myExcel.get("rExcel", "X5", value);       //Ciclo "if" per poter mettere in pausa
	int _intput = atoi(value);                // "if" cycle to be able to pause
	if (_intput == 1) {
		intput();
	}
	myExcel.get("rExcel", "X19", value);      //Ciclo "if" per inviare le nuove impostazioni ad Arduino.
	int _output = atoi(value);                //ATTENZIONE: se disabilitato l'ADS1256 riceverà continuamente nuove impostazioni per SPS e PGA,
	if (_output == 1) {                       // ricalibrando ad ogni lettura, causando ritardi ed errori.
		output();                             //"if" cycle to send the new settings to Arduino.
	}                                         //WARNING: if disabled the ADS1256 will continuously receive new settings for SPS and PGA,
	m = millis();                             //recalibrating at each reading, causing delays and errors.
}

void intput()
{
	static unsigned long  loopTime = 0;
	static unsigned long  time1 = 0;
	loopTime = millis();
	if ((loopTime - time1) >= outputTiming) {
		time1 = loopTime;
		/*int count = 0;                    //Ciclo "for" per la media dei conteggi avvenuti in un ciclo outputTiming. 
		double sum = 0;                     //Il numero dei conteggi N è calcolato automaticamente da Excel in modo da rientrare sempre in un ciclo outputTiming.
		for (int i = 0; i < N; i++)         //Valori di SPS troppo bassi possono far rallentare il ciclo. Utile per SPS elevati, anche se con Arduino sono arrivato a massimo 3750SPS.
		{                                   //"for" cycle for the average of the counts occurred in an outputTiming cycle.
			sum += ADS.read_mV(P_AIN0);     //The number of N counts is automatically calculated by Excel so that it always falls within an outputTiming cycle.
 			count++;                        //Too low SPS values ​​can slow down the cycle. Useful for high SPS, although with Arduino I reached a maximum of 3750SPS.
		}
		mV = sum / count;*/

		mV = ADS.read_mV(P_AIN0);
		K = Kalman.filter(R, Q, mV);
		myExcel.write("rExcel", "L2", mV, 4);
		myExcel.write("rExcel", "L4", idx);
		myExcel.writeIndexed("rExcel", idx + 2, 1, idx);
		myExcel.writeIndexed("rExcel", idx + 2, 2, mV, 4);
		myExcel.writeIndexed("rExcel", idx + 2, 3, K, 4);
		myExcel.writeIndexed("rExcel", idx + 2, 4, m / 1000, 2);
		//myExcel.writeIndexed("rExcel", idx + 2, 5, /*print what you want*/);
		idx++;
	}
}

void output()
{
	myExcel.get("rExcel", "K2", value);
	outputTiming = atoi(value);
	//Kalman                                //Parametri R e Q del filtro di Kalman, regolano il filtraggio dei dati in modo simile ad un filtro passa basso.
	myExcel.get("rExcel", "L2", value);     //Per un uso pratico ho costatato che il rapporto R/Q regola la velocità e la qualità di filtraggio, 
	R = atof(value);                        //tanto più aumenta, quanto più aumenterà la velocità di filtraggio a scapito della velocità.
	myExcel.get("rExcel", "L3", value);     //Kalman filter R and Q parameters regulate data filtering in a similar way to a low pass filter.
	Q = atof(value);                        //For practical use I have found that the R / Q ratio regulates the speed and quality of filtering,
	                                        //the more it increases, the more the filtering speed will increase at the expense of the speed. https://en.wikipedia.org/wiki/Kalman_filter

	//ADS                                   
	/*myExcel.get("rExcel", "X30", value);  //Solo per il ciclo 
	N = atoi(value);*/                      //Only for the cycle "for"
	//SPS
	myExcel.get("rExcel", "AE3", value);
	SPS_ = atoi(value);
	switch (SPS_)
	{
	case 2:    SPS = SPS_2_5;    break;
	case 5:    SPS = SPS_5;      break;
	case 10:   SPS = SPS_10;     break;
	case 15:   SPS = SPS_15;     break;
	case 25:   SPS = SPS_25;     break;
	case 30:   SPS = SPS_30;     break;
	case 50:   SPS = SPS_50;     break;
	case 60:   SPS = SPS_60;     break;
	case 100:  SPS = SPS_100;    break;
	default:break;
	}
	ADS.SetSPS(SPS);
	myExcel.write("rExcel", "L10", ADS.getSPS());
	//PGA
	myExcel.get("rExcel", "AA3", value);
	gain_ = atoi(value);
	switch (gain_)
	{
	case 1:   gain = PGA_1;    break;
	case 2:   gain = PGA_2;    break;
	case 4:   gain = PGA_4;    break;
	case 8:   gain = PGA_8;    break;
	case 16:  gain = PGA_16;   break;
	case 32:  gain = PGA_32;   break;
	case 64:  gain = PGA_64;   break;
	default:break;
	}
	ADS.SetPGA(gain);
	myExcel.write("rExcel", "M10", ADS.getPGA());
	//Clear
	myExcel.clear("rExcel", "X19");
}


