/*
	Copyright 2012-2016 Benjamin Vedder	benjamin@vedder.se

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    NOTE: I made changes to this file marked by "-3- RHC"  Rick Cavallaro
    */

#ifndef DATATYPES_H_
#define DATATYPES_H_

#include <stdint.h>
#include <stdbool.h>
//#include "ch.h"

/*
typedef enum {
    PWM_MODE_NONSYNCHRONOUS_HISW = 0, // This mode is not recommended
    PWM_MODE_SYNCHRONOUS, // The recommended and most tested mode
    PWM_MODE_BIPOLAR // Some glitches occasionally, can kill MOSFETs
} mc_pwm_mode;

typedef enum {
    COMM_MODE_INTEGRATE = 0,
    COMM_MODE_DELAY
} mc_comm_mode;

typedef enum {
    SENSOR_MODE_SENSORLESS = 0,
    SENSOR_MODE_SENSORED,
    SENSOR_MODE_HYBRID
} mc_sensor_mode;

typedef enum {
    FOC_SENSOR_MODE_SENSORLESS = 0,
    FOC_SENSOR_MODE_ENCODER,
    FOC_SENSOR_MODE_HALL
} mc_foc_sensor_mode;

typedef enum {
    MOTOR_TYPE_BLDC = 0,
    MOTOR_TYPE_DC,
    MOTOR_TYPE_FOC
} mc_motor_type;
*/



// -3- RHC:  Modified this to support FW version 5.1 for Shaman's CFOC2 based on VESC6
typedef enum {
  FAULT_CODE_NONE = 0,
  FAULT_CODE_OVER_VOLTAGE,
  FAULT_CODE_UNDER_VOLTAGE,
  FAULT_CODE_DRV,
  FAULT_CODE_ABS_OVER_CURRENT,
  FAULT_CODE_OVER_TEMP_FET,
  FAULT_CODE_OVER_TEMP_MOTOR,
  FAULT_CODE_GATE_DRIVER_OVER_VOLTAGE,
  FAULT_CODE_GATE_DRIVER_UNDER_VOLTAGE,
  FAULT_CODE_MCU_UNDER_VOLTAGE,
  FAULT_CODE_BOOTING_FROM_WATCHDOG_RESET,
  FAULT_CODE_ENCODER_SPI,
  FAULT_CODE_ENCODER_SINCOS_BELOW_MIN_AMPLITUDE,
  FAULT_CODE_ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE,
  FAULT_CODE_FLASH_CORRUPTION,
  FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_1,
  FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_2,
  FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_3,
  FAULT_CODE_UNBALANCED_CURRENTS,
  FAULT_CODE_BRK,
  FAULT_CODE_RESOLVER_LOT,
  FAULT_CODE_RESOLVER_DOS,
  FAULT_CODE_RESOLVER_LOS
} mc_fault_code;


// VESC Types
struct mc_values {
    float v_in;
    float temp_mos1;
    float temp_mos2;
    float temp_mos3;
    float temp_mos4;
    float temp_mos5;
    float temp_mos6;
    float temp_pcb;
    float current_motor;
    float current_in;
    float rpm;
    float duty_now;
    float amp_hours;
    float amp_hours_charged;
    float watt_hours;
    float watt_hours_charged;
    int32_t tachometer;
    int tachometer_abs;
    mc_fault_code fault_code;
};


// -3- RHC:  I added this to support FW 3.40
struct bldcMeasure_340
  {
  float tempFetFiltered;
  float tempMotorFiltered;
  float avgMotorCurrent;
  float avgInputCurrent;
  float avgId;
  float avgIq;
  float dutyNow;
  long rpm;
  float inpVoltage;
  float ampHours;
  float ampHoursCharged;
  float wattHours;
  float watthoursCharged;
  long tachometer;
  long tachometerAbs;
  int faultCode;
  };


// -3- RHC:  I added this to support FW version 5.1 for Shaman's CFOC2 based on VESC6
struct bldcMeasure_51
  {
  float v_in;
  float temp_mos;
  float temp_mos_1;
  float temp_mos_2;
  float temp_mos_3;
  float temp_motor;
  float current_motor;
  float current_in;
  float id;
  float iq;
  float rpm;
  float duty_now;
  float amp_hours;
  float amp_hours_charged;
  float watt_hours;
  float watt_hours_charged;
  int tachometer;
  int tachometer_abs;
  float position;
  mc_fault_code fault_code;
  int vesc_id;
  float vd;
  float vq;
  };


  
// Communication commands
typedef enum {
    COMM_FW_VERSION = 0,
    COMM_JUMP_TO_BOOTLOADER,
    COMM_ERASE_NEW_APP,
    COMM_WRITE_NEW_APP_DATA,
    COMM_GET_VALUES,
    COMM_SET_DUTY,
    COMM_SET_CURRENT,
    COMM_SET_CURRENT_BRAKE,
    COMM_SET_RPM,
    COMM_SET_POS,
    COMM_SET_DETECT,
    COMM_SET_SERVO_POS,
    COMM_SET_MCCONF,
    COMM_GET_MCCONF,
    COMM_GET_MCCONF_DEFAULT,
    COMM_SET_APPCONF,
    COMM_GET_APPCONF,
    COMM_GET_APPCONF_DEFAULT,
    COMM_SAMPLE_PRINT,
    COMM_TERMINAL_CMD,
    COMM_PRINT,
    COMM_ROTOR_POSITION,
    COMM_EXPERIMENT_SAMPLE,
    COMM_DETECT_MOTOR_PARAM,
    COMM_DETECT_MOTOR_R_L,
    COMM_DETECT_MOTOR_FLUX_LINKAGE,
    COMM_DETECT_ENCODER,
    COMM_DETECT_HALL_FOC,
    COMM_REBOOT,
    COMM_ALIVE,
    COMM_GET_DECODED_PPM,
    COMM_GET_DECODED_ADC,
    COMM_GET_DECODED_CHUK,
    COMM_FORWARD_CAN,
    COMM_SET_CHUCK_DATA,
    COMM_CUSTOM_APP_DATA
} COMM_PACKET_ID;

#endif /* DATATYPES_H_ */
