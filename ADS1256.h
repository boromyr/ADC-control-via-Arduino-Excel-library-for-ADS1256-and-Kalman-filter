/*
  Created:	26/03/2020
	Author:     Davide Romeo davideromeo93k@gmail.com
	Based on the works of Flydroid, dariosalvi78, chepo92, adienakhmad.
*/

#ifndef ADS1256_h
#define ADS1256_h
#include "Arduino.h"
#include "SPI.h"

// ADS1256 Register
#define	STATUS                 0x00 //Status Control Register 0
#define	MUX 	                 0x01 //Multiplexer Control Register 0
#define	ADCON                  0x02 //PGA Control Register 0
#define	DRATE	                 0x03 //SPS Data Rate Control Register 0
#define	IO	                   0X04 //GPIO Control Register 0
#define	OFC0  	               0x05 //Offset Calibration Coefficient Register 1
#define	OFC1  	               0x06 //Offset Calibration Coefficient Register 2
#define	OFC2  	               0x07 //Offset Calibration Coefficient Register 2
#define	FSC0   	               0x08 //Full scale Callibration Coefficient Register 0
#define	FSC1	                 0x09 //Full scale Callibration Coefficient Register 1
#define	FSC2	                 0x0A //Full scale Callibration Coefficient REgister 2
// ADS1256 Command
#define	WAKEUP	               0x00 //Exit Sleep Mode
#define STANDBY                0xFD //Enter Sleep Mode
#define SYNC 	                 0xFC //Synchornize the A/D Conversion
#define RESET	                 0xFE //Reset To Power UP values
#define	NOP	                   0xFF //No operation da provare
//DATA READ
#define	RDATA	                 0x01 //Read data once
#define RDATAC		             0x03 //Read data continously
#define SDATAC 	               0x0F //Stop reading data continously
//REGISTER 
#define RREG	                 0x10 //Read From Register
#define WREG 	                 0x50 //Write To Register
//Calibration 
#define SYSOCAL	               0xF3 //System Offset Calibration
#define SYSGCAL                0xF2 //System Gain Calibration
#define	SELFCAL	               0xF0 //Self Offset Calibration
#define SELFOCAL               0xF1
#define SELFGCAL               0xF2
//
#define STATUS_RESET           0x01 // Reset STATUS Register
#define ORDER_MSB         B00000000 // Most significant Bit first (default)
#define ORDER_LSB         B00001000 //Least significant Bit first
//Auto Calibration
#define ACAL_OFF          B00000000 // Auto Calibration Disabled (default)
#define ACAL_ON           B00000100 // Auto Calibration Enabled
//Analog Input Buffer Enable
#define BUFEN_OFF         B00000000 //Buffer Disabled (default)
#define BUFEN_ON          B00000010 //BUffer Enabled
//Data Ready (Read Only) Duplicates the state of the DRDY pin
#define MUX_RESET              0x01 // Reset MUX0 Register
#define ADCON_RESET            0x20 // Reset ADCON Register
//D0/CLKOUT Clock Out Rate Setting
#define CLK_OFF           B00000000 //Clock Out off
#define CLK_1             B00100000 //Clock Out Frequency = fCLKIN (default)
#define CLK_2             B01000000 //Clock Out Frequency = fCLKIN/2
#define CLK_4             B01100000 //Clock Out Frequency = fCLKIN/4
//Sensor Detection Current Sources
#define SDCS_OFF          B00000000 //Sensor Detect Off (default)
#define SDCS_05           B00001000 //Sensor Detect Current 0.5?A
#define SDCS_2            B00010000 //Sensor Detect Current 2?A
#define SDCS_10           B00011000 //Sensor Detect Current 10?A
//P
#define P_AIN0                 0x00
#define P_AIN1                 0x10
#define P_AIN2                 0x20
#define P_AIN3                 0x30
#define P_AIN4                 0x40
#define P_AIN5                 0x50
#define P_AIN6                 0x60
#define P_AIN7                 0x70
#define P_AINCOM               0x80
//N
#define N_AIN0                 0x00
#define N_AIN1                 0x01
#define N_AIN2                 0x02
#define N_AIN3                 0x03
#define N_AIN4                 0x04
#define N_AIN5                 0x05
#define N_AIN6                 0x06
#define N_AIN7                 0x07
#define N_AINCOM               0x08
//PGA
#define PGA_1                  0x00
#define PGA_2                  0x01
#define PGA_4                  0x02
#define PGA_8                  0x03
#define PGA_16                 0x04
#define PGA_32                 0x05
#define PGA_64                 0x06
//SPS
#define SPS_30000              0xF0
#define SPS_15000              0xE0
#define SPS_7500               0xD0
#define SPS_3750               0xC0  
#define SPS_2000               0xB0
#define SPS_1000               0xA1
#define SPS_500                0x92
#define SPS_100                0x82
#define SPS_60                 0x72
#define SPS_50                 0x63
#define SPS_30                 0x53
#define SPS_25                 0x43
#define SPS_15                 0x33
#define SPS_10                 0x23
#define SPS_5                  0x13
#define SPS_2_5                0x03

#define DELAY  delayMicroseconds(10)

class ADS1256
{
public:
  ADS1256(int CS, int DRDY, int RESET_, float ref, float Mhz);
  void begin();
  void begin(unsigned char SPS, unsigned char GAIN);
  void writeRegister(unsigned char Adress, unsigned char DATA);
  void SetSPS(unsigned char SPS);
  void SetPGA(unsigned char GAIN);
  void sendCMD(unsigned char cmd);
  void setChannel(unsigned char AIN_P);
  void setChannel(unsigned char AIN_P, unsigned char AIN_N);
  double read_CH0();
  double read_mV(unsigned char P_AIN);
  double read_mV(unsigned char P_AIN, unsigned char N_AIN);
  unsigned char readRegister(unsigned char regAdress);
  unsigned char getPGA();
  int getSPS();

private:
  int PIN_CS;
  int PIN_DRDY;
  int PIN_RESET;
  float SPI_SPEED;
  float V_REF;
  float _pga;
  void waitDRDY();
  double read_value();
};
#endif
