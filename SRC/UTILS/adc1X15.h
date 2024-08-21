/* 
 * File:   adc1X15.h
 * Author: pablo
 *
 * Created on 19 de agosto de 2024, 12:22 PM
 */

#ifndef ADC1X15_H
#define	ADC1X15_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
    
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include <avr/pgmspace.h>
    
#include "xprintf.h"    
#include "i2c.h"
    
// I2C addresses. You have to connect the address pin with the corresponding data line
#define ADS1115_I2C_ADDR_GND			0b1001000
#define ADS1115_I2C_ADDR_VDD			0b1001001
#define ADS1115_I2C_ADDR_SDA			0b1001010
#define ADS1115_I2C_ADDR_SCL			0b1001011

// This application is only using single ended signal measurement, which half the scale from ground to +VIN
#define ADS1115_MIN_MEASUREMENT_VAL		0x0000
#define ADS1115_MAX_MEASUREMENT_VAL		0x7FFF

// Register addresses
#define ADS1115_CONVERSION_REG_ADDR		0b00
#define ADS1115_CONFIG_REG_ADDR			0b01
#define ADS1115_LOW_TRESH_REG_ADDR		0b10
#define ADS1115_HIGH_TRESH_REG_ADDR		0b11

// Register defaults
#define ADS1115_CONVERSION_REG_DEF		0x0000
#define ADS1115_CONFIG_REG_DEF			0x8583
#define ADS1115_LOW_TRESH_REG_DEF		0x8000
#define ADS1115_HIGH_TRESH_REG_DEF		0x7FFF

// Config register flag/data positions
#define ADS1115_OS_FLAG_POS				15
#define ADS1115_MUX2_DAT_POS			14
#define ADS1115_MUX1_DAT_POS			13
#define ADS1115_MUX0_DAT_POS			12
#define ADS1115_PGA2_DAT_POS			11
#define ADS1115_PGA1_DAT_POS			10
#define ADS1115_PGA0_DAT_POS			9
#define ADS1115_MODE_FLAG_POS			8
#define ADS1115_DR2_DAT_POS				7
#define ADS1115_DR1_DAT_POS				6
#define ADS1115_DR0_DAT_POS				5
#define ADS1115_COMP_MODE_FLAG_POS		4
#define ADS1115_COMP_POL_FLAG_POS		3
#define ADS1115_COMP_LAT_FLAG_POS		2
#define ADS1115_COMP_QUE1_DAT_POS		1
#define ADS1115_COMP_QUE0_DAT_POS		0

// Operational status macros
#define ADS1115_OS_START_SINGLE			0b1
#define ADS1115_OS_ONGOING_CONV			0b0
#define ADS1115_OS_NO_CONV				0b1

// Input multiplexer macros
#define ADS1115_MUX_AIN0_AIN1			0b000
#define ADS1115_MUX_AIN0_AIN3			0b001
#define ADS1115_MUX_AIN1_AIN3			0b010
#define ADS1115_MUX_AIN2_AIN3			0b011
#define ADS1115_MUX_AIN0_GND			0b100
#define ADS1115_MUX_AIN1_GND			0b101
#define ADS1115_MUX_AIN2_GND			0b110
#define ADS1115_MUX_AIN3_GND			0b111

// Programmable gain amplifier macros
#define ADS1115_PGA_6_144				0b000
#define ADS1115_PGA_4_096				0b001
#define ADS1115_PGA_2_048				0b010
#define ADS1115_PGA_1_024				0b011
#define ADS1115_PGA_0_512				0b100
#define ADS1115_PGA_0_256				0b101

// Device operation mode macros
#define ADS1115_MODE_CONTINOUS			0
#define ADS1115_MODE_SINGLE				1

// Data rate macros
#define ADS1115_DR_8_SPS				0b000
#define ADS1115_DR_16_SPS				0b001
#define ADS1115_DR_32_SPS				0b010
#define ADS1115_DR_64_SPS				0b011
#define ADS1115_DR_128_SPS				0b100
#define ADS1115_DR_250_SPS				0b101
#define ADS1115_DR_475_SPS				0b110
#define ADS1115_DR_860_SPS				0b111

// Comparator mode macros
#define ADS1115_COMP_MODE_TRADITIONAL	0
#define ADS1115_COMP_MODE_WINDOW		1

// Comparator polarity macros
#define ADS1115_COMP_POL_LOW			0
#define ADS1115_COMP_POL_HIGH			1

// Latching comparator macros
#define ADS1115_COMP_LAT_NO_LATCH		0
#define ADS1115_COMP_LAT_LATCH			1

// Comparator queue and disable macros
#define ADS1115_COMP_QUE_ONE_CONV		0b00
#define ADS1115_COMP_QUE_TWO_CONV		0b01
#define ADS1115_COMP_QUE_FOUR_CONV		0b10
#define ADS1115_COMP_QUE_DISABLE		0b11

// Value multipliers
#define ADS1115_PGA_6_144_MULT          0.1875
#define ADS1115_PGA_4_096_MULT          0.125
#define ADS1115_PGA_2_048_MULT          0.625
#define ADS1115_PGA_1_024_MULT          0.3125
#define ADS1115_PGA_0_512_MULT          0.15625
#define ADS1115_PGA_0_256_MULT          0.78125

typedef uint8_t i2c_addr_t;
typedef uint8_t reg_addr_t;
typedef uint16_t reg_val_t;
typedef uint8_t flag_pos_t;
typedef uint8_t flag_val_t;

reg_val_t ADS1115_readRegister(reg_addr_t dataAddress);
void ADS1115_writeRegister(reg_addr_t dataAddress, uint16_t data);

void ADS1115_reset(void);
reg_val_t ADS1115_readRawValue(void);
float ADS1115_readConvertedValue(void);
void ADS1115_startSingleConvertion(void);
uint8_t ADS1115_getOperationalStatus(void);
void ADS1115_setMultiplexer(uint8_t mux);
uint8_t ADS1115_getMultiplexer(void);
void ADS1115_setPga(uint8_t pga);
uint8_t ADS1115_getPga(void);
void ADS1115_setDeviceMode(uint8_t mode);
uint8_t ADS1115_getDeviceMode(void);
void ADS1115_setDataRate(uint8_t dataRate);
uint8_t ADS1115_getDataRate(void);
void ADS1115_setComparatorMode(uint8_t mode);
uint8_t ADS1115_getComparatorMode(void);
void ADS1115_setComparatorPolarity(uint8_t polarity);
uint8_t ADS1115_getComparatorPolarity(void);
void ADS1115_setLatching(bool latching);
bool ADS1115_isLatching(void);
void ADS1115_setComparatorQueue(uint8_t queue);
uint8_t ADS1115_getComparatorQueue(void);
void ADS1115_setLowTreshold(reg_val_t lowTreshold);
reg_val_t ADS1115_getLowTreshold(void);
void ADS1115_setHighTreshold(reg_val_t highTreshold);
reg_val_t ADS1115_getHighTreshold(void);

void ADS1115_setup(void);
float ADS1115_readValue(uint8_t input);

void ADS1115_test_readSingle(void);
void ADS1115_test_readMultiple(uint8_t veces);
void ADS1115_test_readRawMultiple(uint8_t veces);
void ADS1115_test_readRawSingle(void);

void ADS1115_set_debug(void);
void ADS1115_clear_debug(void);

#ifdef	__cplusplus
}
#endif

#endif	/* ADC1X15_H */

