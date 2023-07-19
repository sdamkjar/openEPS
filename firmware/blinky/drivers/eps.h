/** @file eps.h 
*   @brief EPS Board Definition File
*   @date 20-Dec-2022
*   @author Stefan Damkjar, Junqi Zhu
*
*/

/** 
 *  @defgroup EPS EPS
 *  @brief Portable I2C Driver Wrapper Module for TI HAL libraries.
 *
 *	Related Files
 *   - eps.h
 *   - eps.c
 *   - stdint.h
 */

#ifndef DRIVERS_EPS_H_
#define DRIVERS_EPS_H_

#include "port_i2c.h"
#include "print.h"
#include "stdint.h"
#include "ina226.h"
#include "tca9548a.h"

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

#define EPS_MAX_ARGS (10)

#define EPS_COMMAND_READ 

/*****************************************/
//  I2C Addresses
/*****************************************/

#define EPS_MUX1_I2CADDR (TCA9548A_Addr71) 
#define EPS_MUX2_I2CADDR (TCA9548A_Addr70)

/* TCA9548A Mux 1, Channel 0 */
#define EPS_MPPT1_I2CADDR (INA226_Addr42)
#define EPS_MPPT1_MUXCHAN (TCA9548A_Channel_0)
#define EPS_MPPT2_I2CADDR (INA226_Addr43)
#define EPS_MPPT2_MUXCHAN (TCA9548A_Channel_0)

#define EPS_MPPT3_I2CADDR (INA226_Addr41)
#define EPS_MPPT3_MUXCHAN (TCA9548A_Channel_0)
#define EPS_MPPT4_I2CADDR (INA226_Addr40)
#define EPS_MPPT4_MUXCHAN (TCA9548A_Channel_0)

#define EPS_EPS3V3_I2CADDR (INA226_Addr46)
#define EPS_EPS3V3_MUXCHAN (TCA9548A_Channel_0)
#define EPS_EPS1V2_I2CADDR (INA226_Addr45)
#define EPS_EPS1V2_MUXCHAN (TCA9548A_Channel_0)
#define EPS_PV3V3_I2CADDR  (INA226_Addr47)
#define EPS_PV3V3_MUXCHAN  (TCA9548A_Channel_0)

#define EPS_3V3BUS_I2CADDR (INA226_Addr4D)
#define EPS_3V3BUS_MUXCHAN (TCA9548A_Channel_0)
#define EPS_1V2BUS_I2CADDR (INA226_Addr4E)
#define EPS_1V2BUS_MUXCHAN (TCA9548A_Channel_0)
#define EPS_5V0BUS_I2CADDR (INA226_Addr4F)
#define EPS_5V0BUS_MUXCHAN (TCA9548A_Channel_0)
#define EPS_BATBUS_I2CADDR (INA226_Addr44) 
#define EPS_BATBUS_MUXCHAN (TCA9548A_Channel_0)

#define EPS_RTC_I2CADDR    (RV3032_Addr51)
#define EPS_RTC_MUXCHAN    (TCA9548A_Channel_0)

#define EPS_TEMP1_I2CADDR  (TMP117_Addr48)
#define EPS_TEMP1_MUXCHAN  (TCA9548A_Channel_0)
#define EPS_TEMP2_I2CADDR  (TMP117_Addr49)
#define EPS_TEMP2_MUXCHAN  (TCA9548A_Channel_0)
#define EPS_TEMP3_I2CADDR  (TMP117_Addr4A)
#define EPS_TEMP3_MUXCHAN  (TCA9548A_Channel_0)
#define EPS_TEMP4_I2CADDR  (TMP117_Addr4B)
#define EPS_TEMP4_MUXCHAN  (TCA9548A_Channel_0)

/* TCA9548A Mux 1, Channel 1 */
#define EPS_OUTPUT01_I2CADDR (INA226_Addr40)
#define EPS_OUTPUT01_MUXCHAN (TCA9548A_Channel_1)
#define EPS_OUTPUT02_I2CADDR (INA226_Addr41)
#define EPS_OUTPUT02_MUXCHAN (TCA9548A_Channel_1)
#define EPS_OUTPUT03_I2CADDR (INA226_Addr42)
#define EPS_OUTPUT03_MUXCHAN (TCA9548A_Channel_1)
#define EPS_OUTPUT04_I2CADDR (INA226_Addr43)
#define EPS_OUTPUT04_MUXCHAN (TCA9548A_Channel_1)
#define EPS_OUTPUT05_I2CADDR (INA226_Addr44)
#define EPS_OUTPUT05_MUXCHAN (TCA9548A_Channel_1)
#define EPS_OUTPUT06_I2CADDR (INA226_Addr45)
#define EPS_OUTPUT06_MUXCHAN (TCA9548A_Channel_1)
#define EPS_OUTPUT07_I2CADDR (INA226_Addr46)
#define EPS_OUTPUT07_MUXCHAN (TCA9548A_Channel_1)
#define EPS_OUTPUT08_I2CADDR (INA226_Addr47)
#define EPS_OUTPUT08_MUXCHAN (TCA9548A_Channel_1)
#define EPS_OUTPUT09_I2CADDR (INA226_Addr48)
#define EPS_OUTPUT09_MUXCHAN (TCA9548A_Channel_1)
#define EPS_OUTPUT10_I2CADDR (INA226_Addr49)
#define EPS_OUTPUT10_MUXCHAN (TCA9548A_Channel_1)
#define EPS_OUTPUT11_I2CADDR (INA226_Addr4A)
#define EPS_OUTPUT11_MUXCHAN (TCA9548A_Channel_1)
#define EPS_OUTPUT12_I2CADDR (INA226_Addr4B)
#define EPS_OUTPUT12_MUXCHAN (TCA9548A_Channel_1)
#define EPS_OUTPUT13_I2CADDR (INA226_Addr4C)
#define EPS_OUTPUT13_MUXCHAN (TCA9548A_Channel_1)
#define EPS_OUTPUT14_I2CADDR (INA226_Addr4D)
#define EPS_OUTPUT14_MUXCHAN (TCA9548A_Channel_1)
#define EPS_OUTPUT15_I2CADDR (INA226_Addr4E)
#define EPS_OUTPUT15_MUXCHAN (TCA9548A_Channel_1)
#define EPS_OUTPUT16_I2CADDR (INA226_Addr4F)
#define EPS_OUTPUT16_MUXCHAN (TCA9548A_Channel_1)

/* TCA9548A Mux 1, Channel 2 */
#define EPS_OUTPUT17_I2CADDR (INA226_Addr4C)
#define EPS_OUTPUT17_MUXCHAN (TCA9548A_Channel_2)
#define EPS_OUTPUT18_I2CADDR (INA226_Addr4E)
#define EPS_OUTPUT18_MUXCHAN (TCA9548A_Channel_2)

/*****************************************/
//  Current sense resistors in mOhm
/*****************************************/

#define EPS_OUTPUT01_SENSERESISTOR (5)
#define EPS_OUTPUT02_SENSERESISTOR (5)
#define EPS_OUTPUT03_SENSERESISTOR (5)
#define EPS_OUTPUT04_SENSERESISTOR (5)
#define EPS_OUTPUT05_SENSERESISTOR (5)
#define EPS_OUTPUT06_SENSERESISTOR (5)
#define EPS_OUTPUT07_SENSERESISTOR (5)
#define EPS_OUTPUT08_SENSERESISTOR (5)
#define EPS_OUTPUT09_SENSERESISTOR (5)
#define EPS_OUTPUT10_SENSERESISTOR (5)
#define EPS_OUTPUT11_SENSERESISTOR (5)
#define EPS_OUTPUT12_SENSERESISTOR (5)
#define EPS_OUTPUT13_SENSERESISTOR (5)
#define EPS_OUTPUT14_SENSERESISTOR (5)
#define EPS_OUTPUT15_SENSERESISTOR (5)
#define EPS_OUTPUT16_SENSERESISTOR (5)
#define EPS_OUTPUT17_SENSERESISTOR (5)
#define EPS_OUTPUT18_SENSERESISTOR (5)

#define EPS_3V3BUS_SENSERESISTOR   (5)
#define EPS_1V2BUS_SENSERESISTOR   (5)
#define EPS_5V0BUS_SENSERESISTOR   (5)
#define EPS_BATBUS_SENSERESISTOR   (5)

#define EPS_EPS3V3_SENSERESISTOR   (5)
#define EPS_EPS1V2_SENSERESISTOR   (5)
#define EPS_PV3V3_SENSERESISTOR    (5)

#define EPS_MPPT1_SENSERESISTOR    (5)  
#define EPS_MPPT2_SENSERESISTOR    (5)
#define EPS_MPPT3_SENSERESISTOR    (5)
#define EPS_MPPT4_SENSERESISTOR    (5)

/*****************************************/
//  GPIO Pins
/*****************************************/

#define EPS_GPIO_LED_PORT         (hetPORT1)
#define EPS_GPIO_LED_PIN          (16)

#define EPS_GPIO_I2CMUXRESET_PORT (hetPORT1)
#define EPS_GPIO_I2CMUXRESET_PIN  (26)

/** 
 *  @addtogroup EPS
 *  @{
 */

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/
typedef enum
{
  EPS_Err_NoError     = 0, /**< No error*/
  EPS_Err_Syntax = 1  /**< Invalid syntax (see manual for help)*/
} EPS_Err_TypeDef;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

EPS_Err_TypeDef EPS_runCommand(char * function,
                            char * arg[EPS_MAX_ARGS],
                            uint8_t numArgs);

/**@}*/

#endif /* DRIVERS_EPS_H_ */
