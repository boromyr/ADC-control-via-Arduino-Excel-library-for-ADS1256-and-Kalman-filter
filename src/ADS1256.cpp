/*
  Created:	26/03/2020
	Author:     Davide Romeo davideromeo93k@gmail.com
*/

#include "ADS1256.h"

/*Definisci PIN, voltaggio di riferimento e velocità di clock (PIN 18 e 19 ADS1256)*/
/* Define PIN, reference voltage and clock speed (PIN 18 and 19 ADS1256) */
ADS1256::ADS1256(int CS, int DRDY, int RESET_, float ref, float Mhz)
{
  PIN_CS = CS;
  PIN_DRDY = DRDY;
  PIN_RESET = RESET_;
  V_REF = ref;
  SPI_SPEED = Mhz * 1000000 / 4;
}

/*Inizializza ADS1256, default setting: PGA = 1, SPS = 15. Autocalibrazione*/
/*Initialize ADS1256, default setting: PGA = 1, SPS = 15. Self-calibration*/
void ADS1256::begin()
{
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_DRDY, INPUT);
  pinMode(PIN_RESET, OUTPUT);
  digitalWrite(PIN_CS, LOW);
  digitalWrite(PIN_RESET, LOW);
  digitalWrite(PIN_RESET, HIGH);
  SPI.begin();
  waitDRDY();
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
  DELAY;
  sendCMD(RESET);
  sendCMD(SYNC);
  SetPGA(PGA_1);
  SetSPS(SPS_15);
  sendCMD(SELFCAL);
  waitDRDY();
  sendCMD(SYSGCAL);
  waitDRDY();
}

/*Inizializza ADS1256, scegli manualmente le impostazioni GAIN e SPS. Autocalibrazione*/
/*Initialize ADS1256, manually choose GAIN and SPS settings. Auto calibration*/
void ADS1256::begin(unsigned char SPS, unsigned char GAIN)
{
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_DRDY, INPUT);
  pinMode(PIN_RESET, OUTPUT);
  digitalWrite(PIN_CS, LOW);
  digitalWrite(PIN_RESET, LOW);
  digitalWrite(PIN_RESET, HIGH);
  SPI.begin();
  waitDRDY();
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
  DELAY;
  sendCMD(RESET);
  sendCMD(SYNC);
  SetPGA(GAIN);
  SetSPS(SPS);
  sendCMD(SELFCAL);
  waitDRDY();
  sendCMD(SYSGCAL);
  waitDRDY();
}

/*Scrivi nel registro dell'ADS1256 all'indirizzo e valore scelti. Test di successo della funzione via monitor seriale*/
/*Write in the ADS1256 register at the chosen address and value. Success test of the function via serial monitor*/
void ADS1256::writeRegister(unsigned char Adress, unsigned char DATA)
{
  waitDRDY();
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
  digitalWrite(PIN_CS, LOW);
  DELAY;
  SPI.transfer(WREG | Adress);
  SPI.transfer(WAKEUP);
  SPI.transfer(DATA);
  DELAY;
  if (DATA != ADS1256::readRegister(Adress))
  {
    Serial.print("Write to Register 0x");
    Serial.print(Adress, HEX);
    Serial.println(" failed!");
  }
  //else{Serial.println("Write to Register succesfull");}
  SPI.endTransaction();
}

/*Leggi valore attuale nel registro all'indirizzo specificato*/
/*Read current value in the register at the specified address*/
unsigned char ADS1256::readRegister(unsigned char regAdress)
{
  waitDRDY();
  unsigned char buffer;
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
  digitalWrite(PIN_CS, LOW);
  DELAY;
  SPI.transfer(RREG | regAdress);
  SPI.transfer(WAKEUP);
  DELAY;
  buffer = SPI.transfer(NOP);
  DELAY;
  SPI.endTransaction();
  return buffer;
}

/*Manda comando all'ADS1256*/
/*Send command to ADS1256*/
void ADS1256::sendCMD(unsigned char cmd)
{
  waitDRDY();
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
  digitalWrite(PIN_CS, LOW);
  DELAY;
  SPI.transfer(cmd);
  DELAY;
  SPI.endTransaction();
}

/*Setta velocità di lettura. Verifica di scrittura nel registro e autocalibrazione*/
/*Set read speeds. Verification of writing in the register and self-calibration*/
void ADS1256::SetSPS(unsigned char SPS)
{
  waitDRDY();
  writeRegister(DRATE, SPS);
  if (SPS == ADS1256::readRegister(DRATE))
  {
    Serial.print("SPS set to: ");
    Serial.println(getSPS());
  }
  DELAY;
  sendCMD(SELFCAL);
  waitDRDY();
  sendCMD(SYSGCAL);
  waitDRDY();
}

/*Setta gain. Verifica di scrittura nel registro e autocalibrazione*/
/*Set gain. Verification of writing in the register and self-calibration*/
void ADS1256::SetPGA(unsigned char GAIN)
{
  waitDRDY();
  writeRegister(ADCON, GAIN);
  if (GAIN == ADS1256::readRegister(ADCON))
  {
    Serial.print("PGA set to: ");
    Serial.println(getPGA());
  }
  DELAY;
  sendCMD(SELFCAL);
  waitDRDY();
  sendCMD(SYSGCAL);
  waitDRDY();
  _pga = 1 << GAIN;
}

/*Restituisce il valore attuale di velocità di lettura*/
/*Returns the current reading speed value*/
int ADS1256::getSPS()
{
  int sps;
  int buffer = readRegister(DRATE);
  switch (buffer)
  {
  case 240: sps=30000; break;
  case 224: sps=15000; break; 
  case 208: sps=7500;  break;
  case 192: sps=3750;  break;
  case 176: sps=2000;  break;
  case 161: sps=1000;  break;
  case 146: sps=500;   break;
  case 130: sps=100;   break;
  case 114: sps=60;    break;
  case 99:  sps=50;    break;
  case 83:  sps=30;    break;
  case 67:  sps=25;    break;
  case 51:  sps=15;    break;
  case 35:  sps=10;    break;
  case 19:  sps=5;     break;
  case  3:  sps=2.5;   break;
  default:             break;
  }
  return sps;
}

/*Restituisce il valore attuale del gain*/
/*Returns the current gain value*/
unsigned char ADS1256::getPGA()
{
  unsigned char pga;
  unsigned char buffer = readRegister(ADCON);
  switch (buffer)
  {
  case 0: pga=1;   break;
  case 1: pga=2;   break;
  case 2: pga=4;   break;
  case 3: pga=8;   break;
  case 4: pga=16;  break;
  case 5: pga=32;  break;
  case 6: pga=64;  break;
  default:         break;
  }
  _pga = 1 << buffer;
  return pga;
}

/*Scegli canale di lettura differenziale, specificando CH+ e CH-*/
/*Choose differential reading channel, specifying CH + and CH-*/
void ADS1256::setChannel(unsigned char AIN_P, unsigned char AIN_N)
{
  waitDRDY();
  unsigned char AIN = AIN_P | AIN_N;
  writeRegister(MUX, AIN);
  sendCMD(SYNC);
  sendCMD(WAKEUP);
  DELAY;
}

/*Scegli canale di lettura assoluta CH+, CH- settato automaticamente al PIN N_AINCOM(PIN 4 ADS1256)*/ 
/*Choose absolute reading channel CH +, CH- automatically set to PIN N_AINCOM (PIN 4 ADS1256)*/
void ADS1256::setChannel(unsigned char channel)
{
  setChannel(channel, N_AINCOM);
}

/*Leggi milliolt al canale 0*/
double ADS1256::read_CH0()
{
  long value;
  unsigned char _highByte, _midByte, _lowByte;
  unsigned char channel = P_AIN0 | N_AINCOM;
  writeRegister(MUX, channel);
  return read_value();
}

/*Leggi millivolt specificando CH+. CH- settato automaticamente al PIN N_AINCOM(PIN 4 ADS1256)*/
/*Read millivolts specifying CH +. CH- automatically set to PIN N_AINCOM (PIN 4 ADS1256)*/
double ADS1256::read_mV(unsigned char P_AIN)
{
  long value;
  unsigned char _highByte, _midByte, _lowByte;
  unsigned char channel = P_AIN | N_AINCOM;
  writeRegister(MUX, channel);
  return read_value();
}

/*Leggi millivolt differenziali ai CH+  CH- scelti*/
/*Read differential millivolts at the selected CH + CH-*/
double ADS1256::read_mV(unsigned char P_AIN, unsigned char N_AIN)
{
  long value;
  unsigned char _highByte, _midByte, _lowByte;
  unsigned char channel = P_AIN | N_AIN;
  writeRegister(MUX, channel);
  return read_value();
}

double ADS1256::read_value()
{
  long value;
  unsigned char _highByte, _midByte, _lowByte;
  SPI.transfer(SYNC);
  DELAY;
  SPI.transfer(RDATA);
  DELAY;
  _highByte = SPI.transfer(WAKEUP);
  _midByte = SPI.transfer(WAKEUP);
  _lowByte = SPI.transfer(WAKEUP);

  value = ((long)_highByte << 16) + ((long)_midByte << 8) + ((long)_lowByte);
  /*if (value > 0x7fffff)
  {
    value = (16777215ul - value) + 1;
  }*/
  return value * (2000 * V_REF / 0x7FFFFF) / _pga;
}

void ADS1256::waitDRDY()
{
  while (digitalRead(PIN_DRDY) == HIGH){}
}