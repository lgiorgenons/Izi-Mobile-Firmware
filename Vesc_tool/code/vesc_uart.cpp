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

NOTE: I made a few changes to this file marked by "-3- RHC".  Rick Cavallaro
*/

#include "vesc_uart.h"
#include "buffer.h"
#include "crc.h"
#include "datatypes.h"
#include "config.h"

int process_received_msg(uint8_t *payloadReceived) {

    //Messages <= 255 start with 2. 2nd byte is length
    //Messages >255 start with 3. 2nd and 3rd byte is length combined with 1st >>8 and then &0xFF

    int counter = 0;
    int endMessage = 256;
    bool messageRead = false;
    uint8_t messageReceived[256];
    int lenPayload = 0;

    while (SERIALIO.available()) {

        messageReceived[counter++] = SERIALIO.read();

        if (counter == 2) {//case if state of 'counter' with last read 1

            switch (messageReceived[0])
            {
                case 2:
                    endMessage = messageReceived[1] + 5; //Payload size + 2 for sice + 3 for SRC and End.
                    lenPayload = messageReceived[1];
                    break;
                case 3:
                    //ToDo: Add Message Handling > 255 (starting with 3)
                    break;
                default:
                    break;
            }

        }
        if (counter >= sizeof(messageReceived))
        {
            break;
        }

        if (counter == endMessage && messageReceived[endMessage - 1] == 3) {//+1: Because of counter++ state of 'counter' with last read = "endMessage"
            messageReceived[endMessage] = 0;

            messageRead = true;
            break; //Exit if end of message is reached, even if there is still more data in buffer.
        }
    }
    bool unpacked = false;
    if (messageRead) {
        unpacked = unpack_payload(messageReceived, endMessage, payloadReceived, messageReceived[1]);
    }
    if (unpacked)
    {
        return lenPayload; //Message was read

    }
    else {
        return 0; //No Message Read
    }
}

bool unpack_payload(uint8_t *message, int lenMes, uint8_t *payload, int lenPay) {
    uint16_t crcMessage = 0;
    uint16_t crcPayload = 0;
    //Rebuild src:
    crcMessage = message[lenMes - 3] << 8;
    crcMessage &= 0xFF00;
    crcMessage += message[lenMes - 2];

    //Extract payload:
    memcpy(payload, &message[2], message[1]);

    crcPayload = crc16(payload, message[1]);

    if (crcPayload == crcMessage)
    {

        return true;
    }
    else
    {
        return false;
    }
}

int send_payload(uint8_t* payload, int lenPay) {
    uint16_t crcPayload = crc16(payload, lenPay);
    int count = 0;
    uint8_t messageSend[256];

    if (lenPay <= 256)
    {
        messageSend[count++] = 2;
        messageSend[count++] = lenPay;
    }
    else
    {
        messageSend[count++] = 3;
        messageSend[count++] = (uint8_t)(lenPay >> 8);
        messageSend[count++] = (uint8_t)(lenPay & 0xFF);
    }
    memcpy(&messageSend[count], payload, lenPay);

    count += lenPay;
    messageSend[count++] = (uint8_t)(crcPayload >> 8);
    messageSend[count++] = (uint8_t)(crcPayload & 0xFF);
    messageSend[count++] = 3;
    messageSend[count] = NULL;
    //Sending package
    SERIALIO.write(messageSend, count);

    //Returns number of send bytes
    return count;
}


bool process_read_package(uint8_t* message, mc_values& values, int len) {
    COMM_PACKET_ID packetId;
    int32_t ind = 0;

    packetId = (COMM_PACKET_ID)message[0];
    message++;//Eliminates the message id
    len--;

    switch (packetId)
    {
        case COMM_GET_VALUES:
            ind = 0;
            values.temp_mos1 = buffer_get_float16(message, 10.0, &ind);
            values.temp_mos2 = buffer_get_float16(message, 10.0, &ind);
            values.temp_mos3 = buffer_get_float16(message, 10.0, &ind);
            values.temp_mos4 = buffer_get_float16(message, 10.0, &ind);
            values.temp_mos5 = buffer_get_float16(message, 10.0, &ind);
            values.temp_mos6 = buffer_get_float16(message, 10.0, &ind);
            values.temp_pcb = buffer_get_float16(message, 10.0, &ind);

            values.current_motor = buffer_get_float32(message, 100.0, &ind);
            values.current_in = buffer_get_float32(message, 100.0, &ind);
            values.duty_now = buffer_get_float16(message, 1000.0, &ind);
            values.rpm = buffer_get_int32(message, &ind);
            values.v_in = buffer_get_float16(message, 10.0, &ind);
            values.amp_hours = buffer_get_float32(message, 10000.0, &ind);
            values.amp_hours_charged = buffer_get_float32(message, 10000.0, &ind);
            ind += 8; //Skip 9 bit
            values.tachometer = buffer_get_int32(message, &ind);
            values.tachometer_abs = buffer_get_int32(message, &ind);
            values.fault_code = (mc_fault_code)message[ind++];

            return true;
            break;

        default:
            return false;
            break;
    }
}



/*@@========================================================================================================================
  FW 2.18 version
  
  Message returned from the VESC has this format:
  - One Start byte (value 2 for short packets and 3 for long packets)
  - One or two bytes specifying the packet length (one byte for short packets, 2 bytes for long packets)
  - The payload of the packet (I believe the first byte of the payload will be the message_id or packet_id like COMM_GET_VALUES)
  - Two bytes with a CRC checksum on the payload
  - One stop byte (value 3)

NOTE -3- RHC: I added this header comment, changed the name of this function so I could call different versions of the
              function for different firmware releases, and made one or two minor changes.
============================================================================================================================*/
int get_vesc_values_218(mc_values& values)
    {
    uint8_t command[1] = { COMM_GET_VALUES };
    uint8_t payload[256];
    send_payload(command, 1);
    //delay(15); //needed, otherwise data is not read
    int lenPayload = process_received_msg(payload);
    if (lenPayload > 0)
        {
        int read;
        read = process_read_package(payload, values, lenPayload); //returns true if sucessfull
        return read;
        }
    else
        {
        return false;
        }
    }






/*@@========================================================================================================================
  FW 3.40 version

  Message returned from the VESC has this format:
  - One Start byte (value 2 for short packets and 3 for long packets)
  - One or two bytes specifying the packet length (one byte for short packets, 2 bytes for long packets)
  - The payload of the packet (I believe the first byte of the payload will be the message_id or packet_id like COMM_GET_VALUES)
  - Two bytes with a CRC checksum on the payload
  - One stop byte (value 3)

NOTE -3- RHC: I added this function to support FW 3.40
============================================================================================================================*/
int get_vesc_values_340(struct bldcMeasure_340 *values)
    {
    uint8_t command[1] = { COMM_GET_VALUES };
    uint8_t messageReceived[256];
    int counter = 0;
    int32_t ind;
    int crc_status = 0;
    long start;
    int timeout;
         
    send_payload(command, 1);  // Send request for values to VESC

    start = millis();
    timeout = 200;
    while (millis() - start < timeout) 
        {
        if (SerialBT.available()) messageReceived[counter] = SerialBT.read();
        else continue;
        counter++;
        if (counter == 1)
           {
           if (messageReceived[0] != 2) counter = 0; 
           }
        else if (counter == 2)
           {
           if (messageReceived[1] != 59) counter = 0; // start over if this isn't the beginning of a valid record.  FW 3.40 uses a payload length of 59.
           }
        if (counter >= 256) counter = 0;  // start over if I've read in more bytes than this record can hold.  It's definitely a bad packet in this case.
        }

    // Check CRC:
    if (counter == 64 && messageReceived[63] == 3) crc_status = check_crc(messageReceived, counter);
    else return(0);
            
    if (crc_status)
        {
        ind = 3; // skip the first three bytes of the message ('2', payload_size, message_id)
        values->tempFetFiltered    = buffer_get_float16(messageReceived, 1e1, &ind);
        values->tempMotorFiltered  = buffer_get_float16(messageReceived, 1e1, &ind);
        values->avgMotorCurrent    = buffer_get_float32(messageReceived, 100.0, &ind);
        values->avgInputCurrent    = buffer_get_float32(messageReceived, 100.0, &ind);
        values->avgId              = buffer_get_float32(messageReceived, 1e2, &ind);
        values->avgIq              = buffer_get_float32(messageReceived, 1e2, &ind);
        values->dutyNow            = buffer_get_float16(messageReceived, 1000.0, &ind);
        values->rpm                = buffer_get_float32(messageReceived, 1.0, &ind);
        values->inpVoltage         = buffer_get_float16(messageReceived, 10.0, &ind);
        values->ampHours           = buffer_get_float32(messageReceived, 10000.0, &ind);
        values->ampHoursCharged    = buffer_get_float32(messageReceived, 10000.0, &ind);
        values->wattHours          = buffer_get_float32(messageReceived, 1e4, &ind);
        values->watthoursCharged   = buffer_get_float32(messageReceived, 1e4, &ind);
        values->tachometer         = buffer_get_int32(messageReceived, &ind);
        values->tachometerAbs      = buffer_get_int32(messageReceived, &ind);
        values->faultCode          = messageReceived[ind]; 
        return(1);
        }
    else
        {
        return(0);
        }
    }





/*@@========================================================================================================================
  FW 5.1 version

  Message returned from the VESC has this format:
  - One Start byte (value 2 for short packets and 3 for long packets)
  - One or two bytes specifying the packet length (one byte for short packets, 2 bytes for long packets)
  - The payload of the packet (the first byte of the payload will be the message_id or packet_id like COMM_GET_VALUES)
  - Two bytes with a CRC checksum on the payload
  - One stop byte (value 3)

Here's where I found the description of the contents of the record returned from a "COMM_GET_VALUES" command:
  C:\Files\Gadgets\Electric mountain board\VESC DIY ESC\Cheap-FOCer-2 (2020-05-23)\Cheap-FOCer-2-master...
        \firmware\Source Code\bldc-master 5.01\bldc-master\commands.c

NOTE -3- RHC: I added this function to support FW 5.10 for Shaman's CFOC2 based on the VESC6
============================================================================================================================*/
int get_vesc_values_51(struct bldcMeasure_51 *values)
    {
    uint8_t command[1] = { COMM_GET_VALUES };
    uint8_t messageReceived[256];
    int counter = 0;
    int32_t ind;
    int packet_id;
    int crc_status = 0;
    long start;
    int timeout;
         
    send_payload(command, 1);  // Send request for values to VESC

    start = millis();
    timeout = 200;
    while (millis() - start < timeout) 
        {
        if (SerialBT.available()) messageReceived[counter] = SerialBT.read();
        else continue;
        counter++;
        if (counter == 1)
           {
           if (messageReceived[0] != 2) counter = 0; 
           }
        else if (counter == 2)
           {             
           if (messageReceived[1] != 73) counter = 0; // start over if this isn't the beginning of a valid record.  FW 5.1 uses a payload length of 73.
           }
        if (counter >= 256) counter = 0;  // start over if I've read in more bytes than this record can hold.  It's definitely a bad packet in this case.
        }

    // Check CRC:
    if (counter == 78 && messageReceived[77] == 3) crc_status = check_crc(messageReceived, counter);
    else return(0);

    if (crc_status)
        {
        ind = 2; // skip the first two bytes of the message ('2' and payload_size)
        packet_id                  = (int)(messageReceived[ind++]);
        values->temp_mos           = buffer_get_float16(messageReceived, 1e1, &ind);
        values->temp_motor         = buffer_get_float16(messageReceived, 1e1, &ind);
        values->current_motor      = buffer_get_float32(messageReceived, 1e2, &ind);
        values->current_in         = buffer_get_float32(messageReceived, 1e2, &ind);
        values->id                 = buffer_get_float32(messageReceived, 1e2, &ind);
        values->iq                 = buffer_get_float32(messageReceived, 1e2, &ind);
        values->duty_now           = buffer_get_float16(messageReceived, 1e3, &ind);
        values->rpm                = buffer_get_float32(messageReceived, 1e0, &ind);
        values->v_in               = buffer_get_float16(messageReceived, 1e1, &ind);
        values->amp_hours          = buffer_get_float32(messageReceived, 1e4, &ind);
        values->amp_hours_charged  = buffer_get_float32(messageReceived, 1e4, &ind);
        values->watt_hours         = buffer_get_float32(messageReceived, 1e4, &ind);
        values->watt_hours_charged = buffer_get_float32(messageReceived, 1e4, &ind);
        values->tachometer         = buffer_get_int32(messageReceived, &ind);
        values->tachometer_abs     = buffer_get_int32(messageReceived, &ind);
        values->fault_code         = (mc_fault_code)(messageReceived[ind++]);
        values->position           = buffer_get_float32(messageReceived, 1e6, &ind);
        values->vesc_id            = messageReceived[ind++];
        values->temp_mos_1         = buffer_get_float16(messageReceived, 1e1, &ind);
        values->temp_mos_2         = buffer_get_float16(messageReceived, 1e1, &ind);
        values->temp_mos_3         = buffer_get_float16(messageReceived, 1e1, &ind);
        values->vd                 = buffer_get_float32(messageReceived, 1e3, &ind);
        values->vq                 = buffer_get_float32(messageReceived, 1e3, &ind);     
        return(1);
        }
    else
        {
        return(0);
        }
    }





/*@@========================================================================================================================
 * Supports FW 3.40 and FW 5.1 versions

NOTE -3- RHC: I added this function to support Vedder FW 3.40 and Shaman's FW 5.10 for his CFOC2 based on the VESC6
============================================================================================================================*/
int check_crc(uint8_t *messageReceived, int message_len)
    {
    int i;
    uint8_t payload[256];
    uint16_t crcMessage = 0;
    uint16_t crcPayload = 0;
    int payload_len;
    
    crcMessage = 0;
    crcPayload = 0;

    payload_len = messageReceived[1];
  
    crcMessage = messageReceived[message_len - 3] << 8;
    crcMessage &= 0xFF00;
    crcMessage += messageReceived[message_len - 2];      
    
    for(i=0; i<payload_len; i++) payload[i] = messageReceived[i+2];
    crcPayload = crc16(payload, payload_len);

    if (crcMessage == crcPayload) return(1);
    else                          return(0);
    }


