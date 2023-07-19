/** @file RV3032C7.h

@brief RV3032C7 Real-time Clock Chip Definition File
@date 19-Jul-2023
@author [Your Name]
*/
/**
 *
 *  @defgroup RV3032C7 RV3032C7
 *  @brief RV3032C7 Real-time Clock Chip Module.
 *  The RV3032C7 is a low-power real-time clock (RTC) chip with an I2C
 *  interface. It provides accurate timekeeping and calendar functions.
 *  Related Files
 *  RV3032C7.h
 *  RV3032C7.c
 *  port_i2c.h
 *  stdint.h
 */

#ifndef DRIVERS_RV3032C7_H_
#define DRIVERS_RV3032C7_H_

#include "port_i2c.h"
#include "stdint.h"

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

 /* 7-bit I2C address of the RV3032C7. */
#define RV3032C7_ADDR 0x51

#define RV3032C7_100THSEC_DIG0          (0xFUL << 0)
#define _RV3032C7_100THSEC_DIG0_SHIFT   0
#define _RV3032C7_100THSEC_DIG0_MASK    0xFUL
#define RV3032C7_100THSEC_DIG1          (0xFUL << 4)
#define _RV3032C7_100THSEC_DIG1_SHIFT   4
#define _RV3032C7_100THSEC_DIG1_MASK    0xF0UL
#define RV3032C7_SEC_DIG0               (0xFUL << 0)
#define _RV3032C7_SEC_DIG0_SHIFT        0
#define _RV3032C7_SEC_DIG0_MASK         0xFUL
#define RV3032C7_SEC_DIG1               (0x7UL << 4)
#define _RV3032C7_SEC_DIG1_SHIFT        4
#define _RV3032C7_SEC_DIG1_MASK         0x70UL
#define RV3032C7_MIN_DIG0               (0xFUL << 0)
#define _RV3032C7_MIN_DIG0_SHIFT        0
#define _RV3032C7_MIN_DIG0_MASK         0xFUL
#define RV3032C7_MIN_DIG1               (0x7UL << 4)
#define _RV3032C7_MIN_DIG1_SHIFT        4
#define _RV3032C7_MIN_DIG1_MASK         0x70UL
#define RV3032C7_HOUR_DIG0              (0xFUL << 0)
#define _RV3032C7_HOUR_DIG0_SHIFT       0
#define _RV3032C7_HOUR_DIG0_MASK        0xFUL
#define RV3032C7_HOUR_DIG1              (0x3UL << 4)
#define _RV3032C7_HOUR_DIG1_SHIFT       4
#define _RV3032C7_HOUR_DIG1_MASK        0x30UL
#define RV3032C7_WEEKDAY                (0x7UL << 0)
#define _RV3032C7_WEEKDAY_SHIFT         0
#define _RV3032C7_WEEKDAY_MASK          0x7UL
#define _RV3032C7_WEEKDAY_SUNDAY        0x0UL
#define _RV3032C7_WEEKDAY_MONDAY        0x1UL
#define _RV3032C7_WEEKDAY_TUESDAY       0x2UL
#define _RV3032C7_WEEKDAY_WEDNESDAY     0x3UL
#define _RV3032C7_WEEKDAY_THURSDAY      0x4UL
#define _RV3032C7_WEEKDAY_FRIDAY        0x5UL
#define _RV3032C7_WEEKDAY_SATURDAY      0x6UL
#define RV3032C7_DATE_DIG0              (0xFUL << 0)
#define _RV3032C7_DATE_DIG0_SHIFT       0
#define _RV3032C7_DATE_DIG0_MASK        0xFUL
#define RV3032C7_DATE_DIG1              (0x3UL << 4)
#define _RV3032C7_DATE_DIG1_SHIFT       4
#define _RV3032C7_DATE_DIG1_MASK        0x30UL
#define RV3032C7_MONTH_DIG0             (0xFUL << 0)
#define _RV3032C7_MONTH_DIG0_SHIFT      0
#define _RV3032C7_MONTH_DIG0_MASK       0xFUL
#define RV3032C7_MONTH_DIG1             (0x1UL << 4)
#define _RV3032C7_MONTH_DIG1_SHIFT      4
#define _RV3032C7_MONTH_DIG1_MASK       0x10UL
#define RV3032C7_YEAR_DIG0              (0xFUL << 0)
#define _RV3032C7_YEAR_DIG0_SHIFT       0
#define _RV3032C7_YEAR_DIG0_MASK        0xFUL
#define RV3032C7_YEAR_DIG1              (0xFUL << 4)
#define _RV3032C7_YEAR_DIG1_SHIFT       4
#define _RV3032C7_YEAR_DIG1_MASK        0xF0UL
#define RV3032C7_ALARM_MIN_DIG0         (0xFUL << 0)
#define _RV3032C7_ALARM_MIN_DIG0_SHIFT  0
#define _RV3032C7_ALARM_MIN_DIG0_MASK   0xFUL
#define RV3032C7_ALARM_MIN_DIG1         (0x7UL << 4)
#define _RV3032C7_ALARM_MIN_DIG1_SHIFT  4
#define _RV3032C7_ALARM_MIN_DIG1_MASK   0x70UL
#define RV3032C7_ALARM_MIN_EN           (0x1UL << 7)
#define _RV3032C7_ALARM_MIN_EN_SHIFT    7
#define _RV3032C7_ALARM_MIN_EN_MASK     0x80UL
#define RV3032C7_ALARM_HOUR_DIG0        (0xFUL << 0)
#define _RV3032C7_ALARM_HOUR_DIG0_SHIFT 0
#define _RV3032C7_ALARM_HOUR_DIG0_MASK  0xFUL
#define RV3032C7_ALARM_HOUR_DIG1        (0x3UL << 4)
#define _RV3032C7_ALARM_HOUR_DIG1_SHIFT 4
#define _RV3032C7_ALARM_HOUR_DIG1_MASK  0x30UL
#define RV3032C7_ALARM_HOUR_EN          (0x1UL << 7)






/* RV3032C7 Register Map */

/** @enum RV3032C7_Register_TypeDef
 *  @brief Alias names for RV3032C7 registers.
 */
typedef enum
{
    RV3032C7_Reg100thSec    = 0x00,     /**< 100th seconds register */
    RV3032C7_RegSec         = 0x01,     /**< Seconds register */
    RV3032C7_RegMin         = 0x02,     /**< Minutes register */
    RV3032C7_RegHour        = 0x03,     /**< Hours register */
    RV3032C7_RegWeekday     = 0x04,     /**< Weekday register */
    RV3032C7_RegDate        = 0x05,     /**< Date register */
    RV3032C7_RegMonth       = 0x06,     /**< Month register */
    RV3032C7_RegYear        = 0x07,     /**< Year register */
    RV3032C7_RegAlarmMin    = 0x08,     /**< Alarm minutes register */
    RV3032C7_RegAlarmHour   = 0x09,     /**< Alarm hours register */
    RV3032C7_RegAlarmDate   = 0x0A,     /**< Alarm date register */
    RV3032C7_RegTimer0      = 0x0B,     /**< Timer 0 register */
    RV3032C7_RegTimer1      = 0x0C,     /**< Timer 1 register */
    RV3032C7_RegStatus      = 0x0D,     /**< Status register */
    RV3032C7_RegTempLSB     = 0x0E,     /**< Temperature LSB register */
    RV3032C7_RegTempMSB     = 0x0F,     /**< Temperature MSB register */
    RV3032C7_RegControl1    = 0x10,     /**< Control 1 register */
    RV3032C7_RegControl2    = 0x11,     /**< Control 2 register */
    RV3032C7_RegControl3    = 0x12,     /**< Control 3 register */
    RV3032C7_RegTimeStamp   = 0x13,     /**< Time stamp control register */
    RV3032C7_RegIntMask     = 0x14,     /**< Interrupt mask register */
    RV3032C7_RegEVI         = 0x15,     /**< External event interrupt register */
    RV3032C7_RegTLow        = 0x16,     /**< Temperature low threshold register */
    RV3032C7_RegTHigh       = 0x17,     /**< Temperature high threshold register */
    RV3032C7_RegTLowCount   = 0x18,     /**< Temperature low count register */
    RV3032C7_RegTLowSec     = 0x19,     /**< Temperature low timestamp seconds register */
    RV3032C7_RegTLowMin     = 0x1A,     /**< Temperature low timestamp minutes register */
    RV3032C7_RegTLowHour    = 0x1B,     /**< Temperature low timestamp hours register */
    RV3032C7_RegTLowDate    = 0x1C,     /**< Temperature low timestamp date register */
    RV3032C7_RegTLowMonth   = 0x1D,     /**< Temperature low timestamp month register */
    RV3032C7_RegTLowYear    = 0x1E,     /**< Temperature low timestamp year register */
    RV3032C7_RegTHighCount  = 0x1F,     /**< Temperature high count register */
    RV3032C7_RegTHighSec    = 0x20,     /**< Temperature high timestamp seconds register */
    RV3032C7_RegTHighMin    = 0x21,     /**< Temperature high timestamp minutes register */
    RV3032C7_RegTHighHour   = 0x22,     /**< Temperature high timestamp hours register */
    RV3032C7_RegTHighDate   = 0x23,     /**< Temperature high timestamp date register */
    RV3032C7_RegTHighMonth  = 0x24,     /**< Temperature high timestamp month register */
    RV3032C7_RegTHighYear   = 0x25,     /**< Temperature high timestamp year register */
    RV3032C7_RegEVICount    = 0x26,     /**< External event interrupt count register */
    RV3032C7_RegEVI100thSec = 0x27,     /**< External event interrupt timestamp 100th seconds register */
    RV3032C7_RegEVIsec      = 0x28,     /**< External event interrupt timestamp seconds register */
    RV3032C7_RegEVIMin      = 0x29,     /**< External event interrupt timestamp minutes register */
    RV3032C7_RegEVIHour     = 0x2A,     /**< External event interrupt timestamp hours register */
    RV3032C7_RegEVIDate     = 0x2B,     /**< External event interrupt timestamp date register */
    RV3032C7_RegEVIMonth    = 0x2C,     /**< External event interrupt timestamp month register */
    RV3032C7_RegEVIYear     = 0x2D,     /**< External event interrupt timestamp year register */
    RV3032C7_RegPW0         = 0x39,     /**< Password 0 register */
    RV3032C7_RegPW1         = 0x3A,     /**< Password 1 register */
    RV3032C7_RegPW2         = 0x3B,     /**< Password 2 register */
    RV3032C7_RegPW3         = 0x3C,     /**< Password 3 register */
    RV3032C7_RegEEAddress   = 0x3D,     /**< EEPROM address register */ 
    RV3032C7_RegEEData      = 0x3E,     /**< EEPROM data register */
    RV3032C7_RegEECommand   = 0x3F,     /**< EEPROM command register */
    RV3032C7_RegUserRAM     = 0x40,     /**< User RAM Start Address */
    RV3032C7_RegUserRAMEnd  = 0x4F,     /**< User RAM End Address */
    RV3032C7_RegEEPMU       = 0xC0,     /**< EEPROM power management register */
    RV3032C7_RegEEOffset    = 0xC1,     /**< EEPROM offset register */
    RV3032C7_RegEEClkOut1   = 0xC2,     /**< EEPROM clock out 1 register */
    RV3032C7_RegEEClkOut2   = 0xC3,     /**< EEPROM clock out 2 register */
    RV3032C7_RegEETRefLSB   = 0xC4,     /**< EEPROM temperature reference LSB register */
    RV3032C7_RegEETRefMSB   = 0xC5,     /**< EEPROM temperature reference MSB register */
    RV3032C7_RegEEPW0       = 0xC6,     /**< EEPROM password 0 register */
    RV3032C7_RegEEPW1       = 0xC7,     /**< EEPROM password 1 register */
    RV3032C7_RegEEPW2       = 0xC8,     /**< EEPROM password 2 register */
    RV3032C7_RegEEPW3       = 0xC9,     /**< EEPROM password 3 register */
    RV3032C7_RegEEPWEn      = 0xCA,     /**< EEPROM password enable register */
    RV3032C7_RegUserEEPROM  = 0xCB,     /**< User EEPROM start register */
    RV3032C7_RegUserEEPROMEnd = 0xEA    /**< User EEPROM end register */
} RV3032C7_Register_TypeDef;





