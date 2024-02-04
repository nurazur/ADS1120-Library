#ifndef ADS1120_h
#define ADS1120_h

#include <Arduino.h>
#include <SPI.h>

#define SPI_MASTER_DUMMY   0xFF
// Commands for the ADC
#define CMD_RESET 0x07
#define CMD_START_SYNC 0x08
#define CMD_PWRDWN 0x03
#define CMD_RDATA 0x1f
#define CMD_RREG 0x20
#define CMD_WREG 0x40

// Configuration registers
#define CONFIG_REG0_ADDRESS 0x00
#define CONFIG_REG1_ADDRESS 0x01
#define CONFIG_REG2_ADDRESS 0x02
#define CONFIG_REG3_ADDRESS 0x03

// Register masks for settings
// Register 0
#define REG_MASK_MUX 0xF0
#define REG_MASK_GAIN 0x0E
#define REG_MASK_PGA_BYPASS 0x01

// Register 1
#define REG_MASK_DATARATE 0xE0
#define REG_MASK_OP_MODE 0x18
#define REG_MASK_CONV_MODE 0x04
#define REG_MASK_TEMP_MODE 0x02
#define REG_MASK_BURNOUT_SOURCES 0x01

// Register 2
#define REG_MASK_VOLTAGE_REF 0xC0
#define REG_MASK_FIR_CONF 0x30
#define REG_MASK_PWR_SWITCH 0x08
#define REG_MASK_IDAC_CURRENT 0x07

// Register 3
#define REG_MASK_IDAC1_ROUTING 0xE0
#define REG_MASK_IDAC2_ROUTING 0x1C
#define REG_MASK_DRDY_MODE 0x02
#define REG_MASK_RESERVED 0x01

class ADS1120 {
  public:
    ADS1120();
    uint8_t ADS1120_CS_PIN;
    uint8_t ADS1120_DRDY_PIN;
    void writeRegister(uint8_t address, uint8_t value);
    uint8_t readRegister(uint8_t address);
    void begin(uint8_t cs_pin, uint8_t drdy_pin);
    bool isDataReady(void);
    uint16_t readADC(void);
    void startADC_Single(void); // trigger single shot conversion
    uint16_t getADC_Single(void); // get result fro single shot conversion
    uint16_t readADC_Single(uint8_t sleep=0);
    double readADC_SingleTemp(uint8_t sleep=0);
    double convertToTemp(uint16_t data);
    void sendCommand(uint8_t command);
    void reset(void);
    void startSync(void);
    void powerDown(void);
    void rdata(void);
    void writeRegisterMasked(uint8_t value, uint8_t mask, uint8_t address);
    void setMultiplexer(uint8_t value);
    void setGain(uint8_t gain);
    void setPGAbypass(bool value);
    void setDataRate(uint8_t value);
    void setOpMode(uint8_t value);
    void setConversionMode(uint8_t value);
    void setTemperatureMode(uint8_t value);
    void setBurnoutCurrentSources(bool value);
    void setVoltageRef(uint8_t value);
    void setFIR(uint8_t value);
    void setPowerSwitch(uint8_t value);
    void setIDACcurrent(uint8_t value);
    void setIDAC1routing(uint8_t value);
    void setIDAC2routing(uint8_t value);
    void setDRDYmode(uint8_t value);
  };
#endif
