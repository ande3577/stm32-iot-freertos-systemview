/**
  @page A_GetPos application for B-L475E-IOT01A1 board and X-NUCLEO-GNSS1A1 expansion
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    readme.txt  
  * @author  CL/AST
  * @version V1.0.0
  * @date    Sep-2018
  * @brief   This application shows how to enable Assisted-GNSS feature
  *          (both Predictive and Real-Time type) supported by the Teseo-LIV3F device.
  *          To this aim, the application needs to access the Internet and is customized
  *          for B-L475E-IOT01A1 providing the WiFi connectivity. For more details about Assisted GNSS,
  *          please refer to AN5160: RxNetworks Assisted GNSS Server Interface Specification
  *          and UM2399: ST Teseo III binary image - User manual available at st.com
  *          In addition, this application shows how real time GNSS data received by the GNSS
  *          Teseo-LIV3F device can be displayed through a serial connection and 
  *          a serial terminal on a PC. In addition, this application shows how real time GNSS data
  *          received by the GNSS. Teseo-LIV3F device can be displayed through a serial connection and 
  *          a serial terminal on a PC.
  ******************************************************************************
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

  
@par Example Description 

Application showing how to enable the Assisted-GNSS feature supported by the Teseo-LIV3F device.

Data received by the Teseo-LIV3F device can be displayed, through a serial connection and a serial terminal, on a PC.
The cJSON middleware support is included to parse the messages coming from the Assisted-GNSS network provider.
The A_GetPos application is built on top of the FreeRTOS support introducing
a task (consumer) to parse the messages (enqueued in a shared queue)
coming from the Teseo-LIV3F device; and a task (listener) to parse commands coming from the serial terminal.
Furthermore this sample application shows three advanced features supported by the Teseo-LIV3F device:
 - Geofencing
 - Odometer
 - Data Logging
The development of the application includes contribution by APG Division.
The Teseo-LIV3F device sends via a UART (or I2C) interface the received GNSS data to the STM32 
microcontroller, hosted on the B-L475E-IOT01A1 board, according to the NMEA 0183 Version 4.0 protocol.
The user can decide at compile time the interface to receive data (setting the macro USE_I2C in file
gnss1a1_conf.h).
The user can enable a specific feature at compile time (setting the corresponding macro in file gnss_app_cfg.h).

This A_GetPos sample application is able to:
    o	establish a serial connection between the B-L475E-IOT01A1 and X-NUCLEO-GNSS1 boards and 
        the PC
    o	enable the Assisted-GNSS feature (both Predictive and Real-Time).
    o	allow the user to select among different options for getting in a human readable format 
        information related to the acquired GNSS position, the satellites in view, the active 
        satellites, and more.		
	
This A_GetPos sample application allows also the user to run commands enabling three advanced features:
    o	Geofencing - allows the Teseo-LIV3F receiver to raise a NMEA message when the resolved GNSS position is close to or entering or exiting from a specific circle
    o	Odometer - provides information on the traveled distance using only the resolved GNSS position
    o	Data Logging - allows the Teseo-LIV3F receiver to save locally on the flash the resolved GNSS position to be retrieved on demand from the Host

After connecting the B-L475E-IOT01A1 board and the X-NUCLEO-GNSS1A1 expansion board and the multi antenna to the connector on the X-NUCLEO-GNSS1A1 expansion board,
connect the B-L475E-IOT01A1 board to your PC.
Drag and drop A_GetPos*.bin (in Binary folder) on B-L475E-IOT01A1 drive.

Run a Serial Terminal (e.g. TeraTerm) on your PC and open a serial connection using the 
following parameters:
    o	baud rate: 115200
    o  	data: 8 bit
    o	parity: none
    o	stop: 1bit
    o	flow control: none
    o	transmit delay: 5msec/char, 5msec/line

Reset the B-L475E-IOT01A1 board and select an option from the main menu appearing on Serial Terminal.

Preprocessor settings:
  Add in Compiler settings "USE_HAL_I2C_REGISTER_CALLBACKS=1" for I2C Bus ("USE_HAL_UART_REGISTER_CALLBACKS=1" for UART)
  
@par Keywords

GNSS, Geofencing, Odometer, Data Logging, WIFI, RTOS

@par Hardware and Software environment

  - This example runs on B-L475E-IOT01A1 board with GNSS STM32 expansion board
    (X-NUCLEO-GNSS1A1)
  - To use UART channel, modify the Jumper configuration on X-NUCLEO-GNSS1A1 as follows:
    o	J2 closed
    o	J3 open
    o	J4 open
    o	J5 closed

@par How to use it? 

In order to make the program work, you must do the following:
 - WARNING: before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.
 - Open STM32CubeIDE (this firmware has been successfully tested with Version 1.5.1).
   Alternatively you can use the Keil uVision toolchain (this firmware
   has been successfully tested with V5.31.0) or the IAR toolchain (this firmware has 
   been successfully tested with Embedded Workbench V8.50.5).
 - The Preprocessor symbol ASSISTED_GNSS should be included in project option.
 - Rebuild all files and load your image into target memory.
 - Run the example.
 - Alternatively, you can download the pre-built binaries in "Binary" 
   folder included in the distributed package.


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
