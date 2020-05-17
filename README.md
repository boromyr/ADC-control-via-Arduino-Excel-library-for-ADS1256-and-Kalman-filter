# ADC control via Arduino/Excel, library for ADS1256 and Kalman filter

![](https://i.ibb.co/SfZ5TZX/arduino-4-569256.png)![](https://i.ibb.co/g7kcpp4/texas-instruments-logo-vector-01-1.png)![](https://insmac.org/uploads/posts/2019-06/1561383549_xcel.png)
----

![](https://img.shields.io/github/v/release/boromyr/ADC-control-via-Arduino-Excel-library-for-ADS1256-and-Kalman-filter) ![](https://img.shields.io/badge/Created-12%2F04%2F2020-blue) ![](https://img.shields.io/github/downloads/boromyr/ADC-control-via-Arduino-Excel-library-for-ADS1256-and-Kalman-filter/total)

### Features

- Library for the use of the Texas Instruments ADS1256 via Arduino;

- Control of ADS1256 functions: SPS, PGA, write and read register, send command;

- SPS from 2.5 to 30000, Gain from1x to 64x, 8 channels, range +- 5V;

- Complete real-time control via Excel, data plot, dynamic graph, sampling time setting;

- Kalman filter for noise filtering, setting of parameters R and Q;

- Configurable for all ADC;

### **Indice**

* [Features](#Features)
* [About the library](#About-the-library)
  * [Instructions](#Instructions)
  * [Download](#Download)
  * [Functions](#Functions)
* [rExcel](#rExcel)
  * [Excel commands](#Excel-commands)
  * [Functionality](#Functionality)
* [Kalman filter](#Kalman-filter)
* [Links](#Links)
* [Feedback](#Feedback)
* [Note](#Note)
* [Contatti](#Contatti)

### About the library

This library was written for the use of the ADS1256 through Arduino and Excel, with the aim of maximizing user control in real time.
It was also based on the works of Flydroid, dariosalvi78, chepo92 and adienakhmad.

The communication library between Arduino and Excel was written by  [Roberto Valgolio](http://www.robertovalgolio.com/sistemi-programmi/arduino-excel), downloadable from his website, and is already present as an attachment without any modification. The Excel sheet has been modified by me with the addition of further functions for the control of the ADS1256. By modifying the Arduino code it is possible to control other digital modules, such as other ADCs (tested), digital potentiometers (tested), stepper motors (tested), DACs, optical or motion sensors.

### Download 

| Last                                                       | Releases                                                     |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [1.0](https://github.com/boromyr/ADC-control-via-Arduino-Excel-library-for-ADS1256-and-Kalman-filter/releases/download/1.0/ADS1256.rExcel.2.1.rar) | [view](https://github.com/boromyr/ADC-control-via-Arduino-Excel-library-for-ADS1256-and-Kalman-filter/releases) |


### Instructions

It's use is simple, and the functions are self-explanatory:
Include the library;

```cpp
 #include "ads1256.h"
```

Declare the class;

```cpp
 ADS1256   ADS;
```

Initialize the ADS1256 at 15SPS and gain 1;

```cpp
ADS.begin(SPS_15,PGA_1);
```

Read millivolts to the selected channel;

```cpp
ADS.read_mV(P_AIN0);
```

#### Functions

| Function                         | Description                                                  |
| -------------------------------- | ------------------------------------------------------------ |
| begin(SPS,PGA)                   | Initialize the ADS1256 by choosing sample rate and gain.     |
| writeRegister(address,value)     | Write the desired value in the register at the specified address. In case of error, a warning is shown in the serial monitor. |
| readRegister(address)            | Read the current register value at the desired address.      |
| sendCMD(command)                 | Send the designated command.                                 |
| setSPS(value)                    | set the new sample rate value, then recalibrate the ADS1256. WARNING: this function only needs to be called once, otherwise the system will continue to recalibrate with each new cycle, this can cause latency problems during code execution, especially at low SPS values. |
| setPGA(value)                    | Set the desired gain.                                        |
| setChannel(channel +)            | Set the positive reading channel, the negative channel is set as the default AINCOM. |
| setChannel(cahnnel +, channel -) | Sets the differential reading channel by specifying the positive channel and the negative channel. |
| read_mV()                        | Read millivolts for channel P_AIN0, AINCOM.                  |
| read_mV(channel +, AINCOM)       | Read millivolts for the desired channel P_AIN "X", AINCOM.   |
| read_mV(channel +,channel-)      | Read the differential millivolts between two chosen channels P_AINX, N_AINX. |
| getSPS()                         | Returns the current value of sample rate.                    |
| getPGA()                         | Returns the current value of gain.                           |


![](https://i.ibb.co/dQrQxm3/adc-24bit.png)

 > ADS1256 Circut

[![Serial output, measurement Arduino 3,3V output](https://i.ibb.co/xSy4Jz5/Screenshot-49.png "Serial output, measurement Arduino 3,3V output")](https://i.ibb.co/xSy4Jz5/Screenshot-49.png "Serial output, measurement Arduino 3,3V output")

>Serial output, measurement Arduino 3,3V power supply

### rExcel

rExcel is a communication protocol for Arduino that transforms a normal Excel worksheet into a serial monitor in turn divided by the number of cells in the sheet, which all operate independently of each other. Through it it is possible to collect data in real time from Arduino, or to send multiple commands, execute sections of code or pause Arduino void loop().
The executable file contains the drivers for the library, the code examples for Arduino and a basic rExcel sheet with integrated VBA functions, as well as all the instructions on its functions and its use.

#### Excel commands

- write(worksheet, range, value) : writes and updates value to the designated cell;
- writeIndexed(worksheet, row, column, value) : writes sequence of values in the designated column;
- get(worksheet, range, value) : follows the value in the designated cell.

Using the get (...) function together with an "if" loop, you can activate specific sections of the code or pause them by setting a value in the target cell.

Examples:

-  run only if there is a value of 1 in the target cell

```cpp
myExcel.get("worksheet", "range", value);
	int example = atoi(value);
	if (example == 1) {
		code1();
	}
```

- run only once when there is a value of 1 in the target cell

```cpp
myExcel.get("worksheet", "range", value);
	int example2 = atoi(value);
	if (example2 == 1) {
		code2();
		myExcel.clear("worksheet", "range");
	}
```

*in this case the arguments of get() and clear() must be the same.

#### Preview

![ADS1256_rExcel.xlms](https://i.ibb.co/6sGcG4p/Screenshot-11.png "ADS1256_rExcel.xlms")


#### Functionality

+ **Connect** - Open the connection for the COM port and default baund rate. They must be customized by opening Visual Basic from Excel (ALT + F11), VBAProject (ADS1256 rExcel.xlsm), Microsoft Excel objects, Sheet 1 (rExcel), Private Sub CommandButton10_Click().
+ **Restart** - Restart the connection. It should be customized as "Connect" in Private Sub CommandButton6_Click()
+ **Send** - Send the new settings to Arduino. It must be called at each change of SPS, PGA, Sample Time, R and Q.
+ **FormConnect** - Opens the COM port selection window, baund rate and connection start.
+ **Show/hide** - Show or hide the chart. I have integrated this function by default in the connection and restart button, as the real-time graph can enormously slow down the connection and cause delays. It can be disabled by going to Visual Basic from Excel (ALT + F11), VBAProject (ADS1256 rExcel.xlsm), Form, FormConnect, Public Sub ShowInfo().
+ **Close** - Save and close quickly without any confirmation notice.
+ **Clear** - Clear the range A2:E10000
+ **ΔY graph** - Select the increment to add to the Y axis of the graph. By default the graph chooses Y max. and Y min. from column B. For positive values there is a negative zoom effect, for negative values a positive zoom effect.
+ **X min.** - Select the minimum value for the X axis. If idx> 500, the graph will automatically set the value of X min. obtaining a pleasant sliding effect.


### Kalman filter

This library implements a simple Kalman filter. It can be called with the .filter function (R, Q, input), returning in filtered value.
The filter is regulated by the R and Q parameters. For a practical use the filtering quality is regulated by the R / Q ratio.
                    

### Links

[https://github.com/Flydroid/ADS12xx-Library](https://github.com/Flydroid/ADS12xx-Library)

[https://gist.github.com/dariosalvi78/f2e990b4317199d235bbf5963c3486ae](https://gist.github.com/dariosalvi78/f2e990b4317199d235bbf5963c3486ae)

[https://github.com/CieNTi/serial_port_plotter](https://github.com/CieNTi/serial_port_plotter) advanced plotter.

[http://www.robertovalgolio.com/sistemi-programmi/arduino-excel](http://www.robertovalgolio.com/sistemi-programmi/arduino-excel) original source rExcel.

[https://it.wikipedia.org/wiki/Filtro_di_Kalman](https://it.wikipedia.org/wiki/Filtro_di_Kalman) 

[https://github.com/chepo92/ADS1256](https://github.com/chepo92/ADS1256)  for a more professional version.

### Test

- Library tested on Arduino uno, ADS1256 on breadboard with 8.00 Mhz quartz oscillator and 2 18 pF capacitors;

- ADS1115, ADS1248;
- Excel 2019 x64 on Windows 10 x64.


It should also work in commercial versions already ready on board, send me feedback by email if it also works with other microcontrollers and boards, I will update this list.

### Feedback

If you liked my project, it was useful to you, or you encountered bugs, you can contact me by email. Tell me about your project or send me a photo if you like.

### Incoming

Real-time control of stepper motors (already in progress); additions of scientific tools for data analysis (currently being worked on in the field of spectroscopy); test on ESP32;

#### Soon:

- update with the new rExcel 3.0;

- version for ADS1115;

### Note

- I'm not a professional programmer, any corrections are welcome.
- The Kalman filter code was found at some forum on the web, the origin cannot be remembered. I adapted it into a library that can be used in Arduino.
- Bug: Kalman's plot of values may not work with connection via FormConnect

### Contacts

Project of: Davide Romeo


email: davideromeo93k@gmail.com

### Changelog
14/05/2020 - Add Arduino Excel 3.0  from http://www.robertovalgolio.com/sistemi-programmi/arduino-excel;

16/05/2020 - Files moved to Releases, added download link;



Translated with [Crow Translate.](https://github.com/crow-translate/crow-translate)

README.md written with [Typora](https://typora.io)



