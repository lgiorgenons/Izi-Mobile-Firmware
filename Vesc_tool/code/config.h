/*
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

NOTE: I made minor changes marked by "-3- RHC" Rick Cavallaro
*/

//The Config.h is a file, that I use in other programs usualy. So I define also the serial ports there. If you don't want to
//use it, just comment the include statement in the VescUart.h out.

#ifndef _CONFIG_h
#define _CONFIG_h


#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define SERIALIO SerialBT  // -3- RHC: I changed this from "Serial" to "SerialBT" to communicate over the ESP32 Serial-blue-tooth channel

#endif  // -3- RHC: what's this doing here!?

