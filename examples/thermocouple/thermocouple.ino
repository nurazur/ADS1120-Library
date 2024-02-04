#include <ADS1120.h>
#include <SPI.h>
#include <avr/sleep.h>
/**
* Internal Temp Example for using the Arduino Library for the Texas Instruments ADS11120
*/

// **********************************************************************************
// Copyright nurazur@gmail.com
// **********************************************************************************
// License
// **********************************************************************************
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

// Licence can be viewed at
// http://www.fsf.org/licenses/gpl.txt

// Please maintain this license information along with authorship
// and copyright notices in any redistribution of this code
// *********************************************************************************


// Definition of the pins to be used
#define CS      10
#define DRDY    9

#define SLEEP 1
#define POLL  0


// Creating an ADS1120 object
ADS1120 ads1120;


void configure_basic()
{
    ads1120.setDataRate(0x00);        // 20 SPS
    ads1120.setConversionMode(0);     // Single shot (default)
    ads1120.setVoltageRef(0);         // Internal 2.048 V (default)
    ads1120.setOpMode(0x00);          // Normal mode (default)
    ads1120.setMultiplexer(0);        // AIN0 vs AIN1
    ads1120.setGain(32); 
    ads1120.setFIR(2);                // 50 Hz Notch Filter
}

void configure_temperature()
{
    ads1120.setTemperatureMode(1);    // Enables temperature sensor
}


void configure_tc()
{
    ads1120.setTemperatureMode(0);    // Disable temperature sensor
   
    /*
    uint8_t config_reg;
    config_reg = ads1120.readRegister(CONFIG_REG0_ADDRESS);
    Serial.print("CONFIG_REG0_ADDRESS: "); Serial.println(config_reg, HEX); // should be A
    
    config_reg = ads1120.readRegister(CONFIG_REG1_ADDRESS);
    Serial.print("CONFIG_REG1_ADDRESS: "); Serial.println(config_reg, HEX); // should be 0
    
    config_reg = ads1120.readRegister(CONFIG_REG2_ADDRESS);
    Serial.print("CONFIG_REG2_ADDRESS: "); Serial.println(config_reg, HEX); // should be 0
    
    config_reg = ads1120.readRegister(CONFIG_REG3_ADDRESS);
    Serial.print("CONFIG_REG3_ADDRESS: "); Serial.println(config_reg, HEX); // should be 0
    */
}

void setup()
{
    Serial.begin(19200);
    #if defined (MEGACOREX) || defined (ARDUINO_avrdd)
    SPI.swap(0);
    #endif
    
    // Initialize the ADS1120
    ads1120.begin(CS, DRDY);
    configure_basic();
    configure_temperature();

    #ifdef SLEEP
    // set sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);      
    sleep_enable();    // enable sleep control
    #endif
    
    /*
    uint8_t config_reg;
    config_reg = ads1120.readRegister(CONFIG_REG0_ADDRESS);
    Serial.print("CONFIG_REG0_ADDRESS: "); Serial.println(config_reg, HEX); // should be 0
    
    config_reg = ads1120.readRegister(CONFIG_REG1_ADDRESS);
    Serial.print("CONFIG_REG1_ADDRESS: "); Serial.println(config_reg, HEX); // should be 2
    
    config_reg = ads1120.readRegister(CONFIG_REG2_ADDRESS);
    Serial.print("CONFIG_REG2_ADDRESS: "); Serial.println(config_reg, HEX); // should be 0
    
    config_reg = ads1120.readRegister(CONFIG_REG3_ADDRESS);
    Serial.print("CONFIG_REG3_ADDRESS: "); Serial.println(config_reg, HEX); // should be 0
    */
}

void loop()
{
    int tc_raw;
    configure_temperature();
    Serial.print("CJ_Temp:");
    double Tamb = ads1120.readADC_SingleTemp(POLL);
    Serial.print(Tamb);
    
    configure_tc();
    tc_raw = (int) ads1120.readADC_Single(POLL);
    ads1120.powerDown();
    double Vtc  = 2.048 * 1000000 * tc_raw / 32768 / 32; // in microV
    //Serial.print("Vtc: "); Serial.print(Vtc); Serial.println(" microV");
    
    Serial.print(",TC_Temp:"); Serial.println(Vtc/41.276 + Tamb); // poor man's conversion to temperature
    delay(5000);
}
