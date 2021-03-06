/* RFM69 library and code by Felix Rusu - felix@lowpowerlab.com
  // Get libraries at: https://github.com/LowPowerLab/
  // Make sure you adjust the settings in the configuration section below !!!
  // **********************************************************************************
  // Copyright Felix Rusu, LowPowerLab.com
  // Library and code by Felix Rusu - felix@lowpowerlab.com
  // **********************************************************************************
  // License
  // **********************************************************************************
  // This program is free software; you can redistribute it
  // and/or modify it under the terms of the GNU General
  // Public License as published by the Free Software
  // Foundation; either version 3 of the License, or
  // (at your option) any later version.
  //
  // This program is distributed in the hope that it will
  // be useful, but WITHOUT ANY WARRANTY; without even the
  // implied warranty of MERCHANTABILITY or FITNESS FOR A
  // PARTICULAR PURPOSE. See the GNU General Public
  // License for more details.
  //
  // You should have received a copy of the GNU General
  // Public License along with this program.
  // If not, see <http://www.gnu.org/licenses></http:>.
  //
  // Licence can be viewed at
  // http://www.gnu.org/licenses/gpl-3.0.txt
  //
  // Please maintain this license information along with authorship
  // and copyright notices in any redistribution of this code
  // **********************************************************************************/

#include <RFM69.h>    //get it here: https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>

//*********************************************************************************************
// *********** IMPORTANT SETTINGS - YOU MUST CHANGE/ONFIGURE TO FIT YOUR HARDWARE *************
//*********************************************************************************************
#define NETWORKID     100  //the same on all nodes that talk to each other
#define NODEID        1

//Match frequency to the hardware version of the radio on your Feather
//#define FREQUENCY     RF69_433MHZ
#define FREQUENCY     RF69_868MHZ
// #define FREQUENCY      RF69_915MHZ
#define ENCRYPT false
#define ENCRYPTKEY     "ABCDEFGHIJKLMNOP" //exactly the same 16 characters/bytes on all nodes!
#define IS_RFM69HCW    true // set to 'true' if you are using an RFM69HCW module

//*********************************************************************************************
#define SERIAL_BAUD   115200

/* for Arduino UNO */
#define RFM69_CS      10  // Digital PIN
#define RFM69_IRQ     2 // Digital PIN 2 - G0
#define RFM69_IRQN    0  // Pin 7 is IRQ 4!
#define RFM69_RST     9 // Digital PIN

#define RFM69_GND     "GND"
#define RFM69_VIN     "3.3V"
#define RFM69_G0      2
#define RFM69_SCK     13 // Digital
#define RFM69_MISO    12 // Digital
#define RFM69_MOSI    11 // Digital




#define LED           13  // onboard blinky
//#define LED           0 //use 0 on ESP8266

int16_t packetnum = 0;  // packet counter, we increment per xmission

RFM69 radio = RFM69(RFM69_CS, RFM69_IRQ, IS_RFM69HCW, RFM69_IRQN);

void setup() {
  // while (!Serial); // wait until serial console is open, remove if not tethered to computer. Delete this line on ESP8266
  Serial.begin(SERIAL_BAUD);

  Serial.println("Feather RFM69HCW Receiver");

  // Hard Reset the RFM module
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, HIGH);
  delay(100);
  digitalWrite(RFM69_RST, LOW);
  delay(100);

  // Initialize radio
  radio.initialize(FREQUENCY, NODEID, NETWORKID);
  if (IS_RFM69HCW) {
    radio.setHighPower();    // Only for RFM69HCW & HW!
  }
  radio.setPowerLevel(31); // power output ranges from 0 (5dBm) to 31 (20dBm)

  radio.encrypt(ENCRYPTKEY);
  radio.promiscuous(true);
  pinMode(LED, OUTPUT);

  Serial.print("\nListening at ");
  Serial.print(FREQUENCY == RF69_433MHZ ? 433 : FREQUENCY == RF69_868MHZ ? 868 : 915);
  Serial.println(" MHz");
}

void loop() {
  //check if something was received (could be an interrupt from the radio)
  if (radio.receiveDone())
  {
    //print message received to serial
    //Serial.print('[ SenderID: '); Serial.print(radio.SENDERID); Serial.println("] ");
    Serial.println((char*)radio.DATA);
    Serial.flush(); //make sure all serial data is clocked out before sleeping the MCU
    // Serial.print("\t [RX_RSSI:"); Serial.print(radio.RSSI); Serial.println("]");


    if (radio.ACKRequested())
    {
      radio.sendACK();
      // Serial.println(" - ACK sent");
    }
    Blink(LED, 50, 3); //blink LED 3 times, 40ms between blinks

  }

  radio.receiveDone(); //put radio in RX mode
}

void Blink(byte PIN, byte DELAY_MS, byte loops)
{
  for (byte i = 0; i < loops; i++)
  {
    digitalWrite(PIN, HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN, LOW);
    delay(DELAY_MS);
  }
}
