Ultimate_dashboard.ino is the module I wrote to drive the bluetooth connected LCD dashboard. 
It is intended to run on an ESP32 which I program and flash via the arduino IDE

Please note that I use this on two separate dashboards.  One is used on my 4-wheel mountain-board
and the other is used on my 2-wheel street-board.

The following variables near the top of the "setup()" function define what the current build does...

   vesc_fw_version = 51;      // Which version the VESC firmware is running.  2.18 => 218 and 3.40 => 340; 5.1 => 51
   num_cells = 6;             // Number of battery cells in series.  We don't care how many might be in parallel.
   battery_type = SPIM08HP;   // SPIM08HP (LiIon cells) or LIPO (standard Hobbyking style Lipo's)
   board_type = FOUR_WHEEL;   // TWO_WHEEL or FOUR_WHEEL

Notably, the FOUR_WHEEL dashboard reads only one controller on a board that uses two 
controllers (currently both are Shaman's CFOC2 v0.9 hardware).  And I double the current readings
I get with the assumption that both are pulling the same battery current.

Note also that this code supports a push-button that controls which battery type (LiPo or LiIon) is
used on the board.  They have different discharge curves.

For your version you may want to calibrate the A2D and your voltage divider resistors and store
the values in these lines:

#define TWO_WHEEL_DIVIDER  (0.6526)
#define FOUR_WHEEL_DIVIDER (0.655)

Finally, I have different functions to model the battery draw-down based on voltage (and in one 
case current draw as well).  You should choose the one appropriate to your packs.  The one I wrote for 
the SPIM08HP batteries accounts for voltage sag based on current draw.  This has the very nice feature that
the dashboard shows a nice linear draw-down.  It doesn't show my capacity remaining dropping near zero 
when I'm climbing a steep hill.

The following files come from "https://github.com/R0b0shack/VESC-UART-Arduino" but have a few changes that I added.

The changes are marked by "-3- RHC"

 buffer.cpp
 buffer.h
 config.h
 crc.dpp
 crc.h
 datatypes.h
 vesc_uart.cpp
 vesc_uart.h

