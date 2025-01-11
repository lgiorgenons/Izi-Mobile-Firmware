//
// Created by Daniel Claes on 01/05/2016.
//

#ifndef VESC_UART_H
#define VESC_UART_H
#include "config.h"

//#ifndef _CONFIG_h

/*---------------------------------------------------------------------
Copyright 2015 - 2017 Andreas Chaitidis Andreas.Chaitidis@gmail.com

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.

NOTE: I made a few changes to this file marked by "-3- RHC".  Rick Cavallaro
----------------------------------------------------------------------*/

/* This library was created on an Adruinio 2560 with different serial ports to have a better possibility
to debug. The serial ports are define with #define:
#define SERIALIO Serial1  		for the UART port to VESC
#define DEBUGSERIAL Serial		for debuging over USB
So you need here to define the right serial port for your arduino.
If you want to use debug, uncomment DEBUGSERIAL and define a port.*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "datatypes.h"

//-3- RHC: I added these to support the ESP32 serial-bluetooth channel
#include "BluetoothSerial.h"
extern BluetoothSerial SerialBT;

bool unpack_payload(uint8_t *message, int lenMes, uint8_t *payload, int lenPa);
bool process_read_package(uint8_t *message, mc_values &values, int len);

///PackSendPayload Packs the payload and sends it over Serial.
///Define in a Config.h a SERIAL with the Serial in Arduino Style you want to you
///@param: payload as the payload [unit8_t Array] with length of int lenPayload
///@return the number of bytes send
int send_payload(uint8_t* payload, int lenPay);

///Sends a command to VESC and stores the returned data
///@param bldcMeasure struct with received data
//@return true if sucess
//-3- RHC: I broke "get_vesc_values" into three separate functions to support 3 different firmware versions.
int get_vesc_values_218(struct mc_values &values);        // FW 2.18 version
int get_vesc_values_340(struct bldcMeasure_340 *values);  // FW 3.40 version
int get_vesc_values_51(struct bldcMeasure_51 *values);    // FW 5.1 version

///ReceiveUartMessage receives the a message over Serial
///Define in a Config.h a SERIAL with the Serial in Arduino Style you want to you
///@parm the payload as the payload [unit8_t Array]
///@return the number of bytes receeived within the payload

int process_received_msg(uint8_t* payloadReceived);

//-3- RHC: I added this
int check_crc(uint8_t *messageReceived, int message_len);

#endif //VESC_UART_H
